float3   spikyGradientKernel(float3 currentParticle, float3 neighbor) {
    float3  res;
    float   length;
    float   H = 1.5f;// smoothing radius

    res = currentParticle - neighbor;
    length = distance(currentParticle, neighbor);
  //  printf("delta: %f.%f.%f  %f %f %f, dist: %f\n", currentParticle.x, currentParticle.y, currentParticle.z, neighbor.x, neighbor.y, neighbor.z, length);
    float   hrTerm = H - length; 
    float   gradientMagnitude = 45.0f / (M_PI * pown(H, 6)) * hrTerm * hrTerm;
    float   div = length + 0.001f;
    return gradientMagnitude * 1.0f / div * res;

}

float   poly6Kernel(float3 currentParticle, float3 neighbor) {
    float   H = 1.5f;// smoothing radius

    float r = distance(currentParticle, neighbor);
    float hrTerm = (H * H - r * r);
    float div = 64.0f * M_PI * pown(H, 9);
    return 315.0f / div * hrTerm * hrTerm * hrTerm;
}
    #define DELTA_Q (float)(0.1*H)
    #define PRESSURE_K 0.1
    #define PRESSURE_N 6

float3      calcDelta(__global float *particles, float3 currentParticle, __global int *neighbors, int nbNeighbors, __global float *lstLambda, float lambda) {
    float3  delta = 1.0f;
    float3  neighbor;
    float3  dQ;
    int     neighborId;
    float   kTerm;
    float   h = 1.5f;
    float   sCorr = 0.0f;

    delta.x = 1.0f;
    delta.y = 1.0f;
    delta.z = 1.0f;
    dQ = (0.1f * h) * delta + currentParticle;
    delta.x = 0.0f;
    delta.y = 0.0f;
    delta.z = 0.0f;
    for (int i = 1; i < nbNeighbors; i++) {
        neighborId = neighbors[i];
        neighbor.x = particles[neighborId];
        neighbor.y = particles[neighborId + 1];
        neighbor.z = particles[neighborId + 2];

        float poly6PDQ = poly6Kernel(currentParticle, dQ);
        if (poly6PDQ < 0.0001f)
        {
            kTerm = 0.0f;
            sCorr = 0.0f;
        }
        else
        {
            kTerm = poly6Kernel(currentParticle, neighbor) / poly6PDQ;
            sCorr = -1.0f * 0.1f * pown(kTerm, 6);
        }

        float3 result = spikyGradientKernel(currentParticle, neighbor);
   //     printf("l: %f, ll: %f, grad: %f, %f, %f\n", lambda, lstLambda[neighborId / 3], result.x, result.y, result.z);
        delta += (lambda + lstLambda[neighborId / 3] + sCorr) * result;
      /*  printf("delta: %f, %f, %f    %d\n",
            delta.x, delta.y, delta.z, i);*/
    }
    return 1.0f / 10000.0f * delta * 0.000001f;
}

void        boxCollision(__global float *particle, float3 currentParticle, __global float *particleVelocity) {
    float4      normal = 0.0f;
    float4      tmp = 0.0f;
    float4      vel = 0.0f;
    float       N = 1.0f;

    vel.x = particleVelocity[0];
    vel.y = particleVelocity[1];
    vel.z = particleVelocity[2];
    if (particle[1] < -15.0f) {
        particle[1] = -14.99f;
        normal.x = 0.0f;
        normal.y = 1.0f;
        normal.z = 0.0f;
        tmp = dot(vel, normal);
        tmp *= normal * 2.0f;
        tmp -= vel * N;
        particleVelocity[1] = tmp.y;
    }

    if (particle[1] > 10.0f) {
        particle[1] = 49.99f;
        normal.x = 0.0f;
        normal.y = -1.0f;
        normal.z = 0.0f;
        tmp = dot(vel, normal);
        tmp *= normal * 2.0f;
        tmp -= vel * N;
        particleVelocity[1] = tmp.y;
    }

    if (particle[2] < -5.0f) {
        particle[2] = -4.99f;
        normal.x = 0.0f;
        normal.y = 0.0f;
        normal.z = 1.0f;
        tmp = dot(vel, normal);
        tmp *= normal * 2.0f;
        tmp -= vel * N;
        particleVelocity[2] = tmp.z;
    }

    if (particle[2] > 10.0f) {
        particle[2] = 9.99f;
        normal.x = 0.0f;
        normal.y = 0.0f;
        normal.z = -1.0f;
        tmp = dot(vel, normal);
        tmp *= normal * 2.0f;
        tmp -= vel * N;
        particleVelocity[2] = tmp.z;
    }

    if (particle[0] < -10.0f) {
        particle[0] = -9.999f;
        normal.x = 1.0f;
        normal.y = 0.0f;
        normal.z = 0.0f;
        tmp = dot(vel, normal);
        tmp *= normal * 2.0f;
        tmp -= vel * N;
        particleVelocity[0] = tmp.x;
    }
    if (particle[0] > 10.0f) {
        particle[0] = 9.99f;
        normal.x = 0.0f;
        normal.y = 0.0f;
        normal.z = -1.0f;
        tmp = dot(vel, normal);
        tmp *= normal * 2.0f;
        tmp -= vel * N;
        particleVelocity[0] = tmp.x;
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
        __global int        *neighbors
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
 //   printf("current: %f.%f.%f\n", particlesProjection[pos], particlesProjection[pos + 1], particlesProjection[pos + 2]);
    float3  delta = calcDelta(particlesProjection, currentParticle, neighbors + gid * 200, gid, lambda, lambda[gid]);
    //printf("delta: %f %f %f\n", delta.x, delta.y, delta.z);
  //  printf("projection: %f.%f.%f\n", particlesProjection[pos], particlesProjection[pos + 1], particlesProjection[pos + 2]);
    particlesProjection[pos] += delta.x;
    particlesProjection[pos + 1] += delta.y;
    particlesProjection[pos + 2] += delta.z;
  //  printf("oa: %f, %f, %f\n\
delta: %f, %f, %f \n", particlesProjection[pos], particlesProjection[pos + 1], particlesProjection[pos + 2],
    //       delta.x, delta.y, delta.z);

    boxCollision(particlesProjection + pos, currentParticle, particlesVelocity + pos);
}
