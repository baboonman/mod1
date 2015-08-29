float3   spikyGradientKernel(float3 currentParticle, float3 neighbor) {
    float3  res;
    float   length;
    float   H = 1.5f;// smoothing radius

    res = currentParticle - neighbor;
    length = distance(currentParticle, neighbor);
    float   hrTerm = H - length; 
    float   gradientMagnitude = 45.0f / (M_PI * pown(H, 6)) * hrTerm * hrTerm;
    float   div = length + 0.001f;
    return gradientMagnitude * 1.0f / div * res;

}

float3      calcDelta(__global float *particles, float3 currentParticle, __global int *neighbors, int nbNeighbors, __global float *lstLambda, float lambda) {
    float3  delta;
    float3  neighbor;
    int     neighborId;

    for (int i = 0; i < nbNeighbors; i++) {
        neighborId = neighbors[i];
        neighbor.x = particles[neighborId];
        neighbor.y = particles[neighborId + 1];
        neighbor.z = particles[neighborId + 2];

        delta += (lambda + lstLambda[neighborId / 3]) * spikyGradientKernel(currentParticle, neighbor);
    }
    return 1.0f / 10000.0f * delta;
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
    float3  delta = calcDelta(particlesProjection, currentParticle, neighbors + gid * 200, gid, lambda, lambda[gid / 3]);
    particlesProjection[pos] += delta.x;
    particlesProjection[pos + 1] += delta.y;
    particlesProjection[pos + 2] += delta.z;
}
