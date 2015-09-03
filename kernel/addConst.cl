float3   spikyGradientKernel(float3 currentParticle, float3 neighbor) {
    float3  res;
    float   length;

    res = currentParticle - neighbor;
    length = distance(currentParticle, neighbor);
  //  printf("delta: %f.%f.%f  %f %f %f, dist: %f\n", currentParticle.x, currentParticle.y, currentParticle.z, neighbor.x, neighbor.y, neighbor.z, length);
    float   hrTerm = RANGE - length; 
    float   gradientMagnitude = 45.0f / (M_PI * pown(RANGE, 6)) * hrTerm * hrTerm;
    float   div = length + 0.001f;
    return gradientMagnitude * 1.0f / div * res;

}

float   poly6Kernel(float3 currentParticle, float3 neighbor, int gid) {
    float r = distance(currentParticle, neighbor);
    float hrTerm = (RANGE * RANGE - r * r);
    float div = 64.0f * M_PI * pown(RANGE, 9);
    float res = 315.0f / div * hrTerm * hrTerm * hrTerm;
    if (res != res) {
        printf("FAILddd gid: %d :: %f, %f, %f, r: %f (%f, %f, %f :: %f, %f, %f)\n", gid, div, hrTerm, res, r, currentParticle.x, currentParticle.y, currentParticle.z, neighbor.x, neighbor.y, neighbor.z);
    }
    return res;
}
#define PRESSURE_K 0.1
#define PRESSURE_N 1

float3      calcDelta(__global float *particles,
                      float3 currentParticle,
                      __global int *neighbors,
                      int nbNeighbors,
                      __global float *lstLambda,
                      float lambda,
                      int gid) {
    float3  delta = 1.0f;
    float3  neighbor;
    float3  dQ;
    int     neighborId;
    float   kTerm;
    float   sCorr = 0.0f;

    delta.x = 1.0f;
    delta.y = 1.0f;
    delta.z = 1.0f;
    dQ = (0.1f * RANGE) * delta + currentParticle;

    delta.x = 0.0f;
    delta.y = 0.0f;
    delta.z = 0.0f;
    for (int i = 0; i < nbNeighbors; i++) {
        neighborId = neighbors[i];
        neighbor.x = particles[neighborId];
        neighbor.y = particles[neighborId + 1];
        neighbor.z = particles[neighborId + 2];

        float poly6PDQ = poly6Kernel(currentParticle, dQ, gid);
        if (poly6PDQ < 0.0001f)
        {
            kTerm = 0.0f;
            sCorr = 0.0f;
        }
        else
        {
            kTerm = poly6Kernel(currentParticle, neighbor, gid) / poly6PDQ;
            sCorr = -1.0f * PRESSURE_K * pown(kTerm, PRESSURE_N);
        }

        float3 result = spikyGradientKernel(currentParticle, neighbor);
        delta += (lambda + lstLambda[neighborId / 3] + sCorr) * result;
    }
    float3 res = 1.0f / REST_DENSITY * delta;
    return res;
}

void        boxCollision(__global float *particle, float3 currentParticle, __global float *particleVelocity, float step) {
    float4      normal = 0.0f;
    float4      tmp = 0.0f;
    float4      vel = 0.0f;
    float       N = 1.0f;
    float       max = MAX;
    float       min = MIN;
    float       offset = 0.001f;
    float       maxX = step / 100.0f + 1.0f;

    vel.x = particleVelocity[0];
    vel.y = particleVelocity[1];
    vel.z = particleVelocity[2];
    particleVelocity[0] = 0.0f;
    particleVelocity[1] = 0.0f;
    particleVelocity[2] = 0.0f;

    if (particle[0] < min) {
        particle[0] = min + offset;
        normal.x = 1.0f;
        normal.y = 0.0f;
        normal.z = 0.0f;
        tmp = vel - 2.0f * (normal * dot(vel, normal));
        particleVelocity[0] = tmp.x;
    }

    if (particle[0] > max) {
        particle[0] = max - offset;
        normal.x = -1.0f;
        normal.y = 0.0f;
        normal.z = 0.0f;
        tmp = vel - 2.0f * (normal * dot(vel, normal));
        particleVelocity[0] = tmp.x;
    }

    if (particle[1] < min) {
        particle[1] = min + offset;
        normal.x = 0.0f;
        normal.y = 1.0f;
        normal.z = 0.0f;
        tmp = vel - 2.0f * (normal * dot(vel, normal));
        particleVelocity[1] = tmp.y;
    }

    if (particle[1] > max) {
        particle[1] = max - offset;
        normal.x = 0.0f;
        normal.y = -1.0f;
        normal.z = 0.0f;
        tmp = vel - 2.0f * (normal * dot(vel, normal));
        particleVelocity[1] = tmp.y;
    }

    if (particle[2] < min) {
        particle[2] = min + offset;
        normal.x = 0.0f;
        normal.y = 0.0f;
        normal.z = 1.0f;
        tmp = vel - 2.0f * (normal * dot(vel, normal));
        particleVelocity[2] = tmp.z;
    }

    if (particle[2] > max) {
        particle[2] = max - offset;
        normal.x = 0.0f;
        normal.y = 0.0f;
        normal.z = -1.0f;
        tmp = vel - 2.0f * (normal * dot(vel, normal));
        particleVelocity[2] = tmp.z;
    }

}

__kernel void   addConst (
        __global float     *particles,
        __global float     *particlesVelocity,
        __global float     *particlesProjection,
        __global int        *gridParticles,
        float               coef,
        int                 gridX,
        int                 gridY,
        int                 gridZ,
        int                 nbParticlePerCell,
        int                 maxGID,
        __global float      *lambda,
        __global int        *neighbors,
        float               wallStep
        )
{
    int gid = get_global_id(0);
    if (gid > maxGID)
        return;

    int pos = gid * 3;
    float3  currentParticle;

    currentParticle.x = particlesProjection[pos];
    currentParticle.y = particlesProjection[pos + 1];
    currentParticle.z = particlesProjection[pos + 2];

    float3  delta = calcDelta(particlesProjection, currentParticle, neighbors + gid * 200 + 1, neighbors[gid * 200], lambda, lambda[gid], gid);

    if (particlesProjection[pos] != particlesProjection[pos] || delta.x != delta.x)
    {
        printf("gid: %d FAIL :: pos: %f, %f, %f delta: %f, %f, %f neighbors: %d \n",
            gid,
            particlesProjection[pos],
            particlesProjection[pos + 1],
            particlesProjection[pos + 2],
            delta.x, delta.y, delta.z,
            neighbors[gid * 200]);
    }

    particlesProjection[pos] += delta.x;
    particlesProjection[pos + 1] += delta.y;
    particlesProjection[pos + 2] += delta.z;

    boxCollision(particlesProjection + pos, currentParticle, particlesVelocity + pos, wallStep);
}
