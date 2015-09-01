float   poly6Kernel(float3 currentParticle, float3 neighbor) {
    float   H = 1.5f;// smoothing radius

    float r = distance(currentParticle, neighbor);
    //printf("r: %f\n", r);
    float hrTerm = (H * H - r * r);
    float div = 64.0f * M_PI * pown(H, 9);
    return 315.0f / div * hrTerm * hrTerm * hrTerm;
}

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

float3  calcCiGradient(float3 currentParticle, float3 neighbor) {
    float3  cI = -1.0f / 10000.0f * spikyGradientKernel(currentParticle, neighbor);
    return cI;
}

float3  calcCiGradientATI(__global float *particles, float3 currentParticle, __global int *neighbors, int nbNeighbors) {
    float3  accum = float3(0.0f);
    float3  neighbor;

    for (int i = 0; i < nbNeighbors; i++) {
        neighbor.x = particles[neighbors[i]];
        neighbor.y = particles[neighbors[i] + 1];
        neighbor.z = particles[neighbors[i] + 2];
        accum += spikyGradientKernel(currentParticle, neighbor);
    }
    return 1.0f / 10000.0f * accum;
}

float   calcRo(__global float *particles, float3 currentParticle, __global int *neighbors, int nbNeighbors)
{
    float   ro = 0.0f;
    float3  neighbor;

    for (int i = 0; i < nbNeighbors; i++) {
        neighbor.x = particles[neighbors[i]];
        neighbor.y = particles[neighbors[i] + 1];
        neighbor.z = particles[neighbors[i] + 2];
  //      printf("neighbor: %f, %f, %f\n", neighbor.x, neighbor.y, neighbor.z);
        ro += poly6Kernel(currentParticle, neighbor);
       // printf("ro: %f\n", ro);
    }
    return ro;
}

float   calcLambdaPi(__global float *particles, float3 currentParticle, __global int *neighbors, int nbNeighbors, int gid)
{
//    printf("nb neighbors: %d\n", nbNeighbors);
    float   pI = calcRo(particles, currentParticle, neighbors, nbNeighbors);
    float   cI =  (pI / 10000.0f) - 1.0f;
    float   cIGradient, sumGradient = 0.0f;
    float3  neighbor;

    for (int i = 0; i < nbNeighbors; i++) {
 //       printf("gid: %d :: id: %d\n", gid, neighbors[i] / 3);
        neighbor.x = particles[neighbors[i]];
        neighbor.y = particles[neighbors[i] + 1];
        neighbor.z = particles[neighbors[i] + 2];
        cIGradient = length(calcCiGradient(currentParticle, neighbor));
        sumGradient += cIGradient * cIGradient;
    }
    cIGradient = length(calcCiGradientATI(particles, currentParticle, neighbors, nbNeighbors));
    sumGradient += cIGradient * cIGradient;
    sumGradient += 0.01f;
    return -1.0f * (cI / sumGradient);
}

__kernel void   calcLambda (
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

    int     currentCell;
    float3  currentParticle;

    currentParticle.x = particlesProjection[pos];
    currentParticle.y = particlesProjection[pos + 1];
    currentParticle.z = particlesProjection[pos + 2];
  //  printf("Init swwwwwwwid: %d, x: %f, y: %f, z: %f\n", gid, particles[pos], particles[pos + 1], particles[pos + 2]);

    lambda[gid] = calcLambdaPi(
                                    particlesProjection,
                                    currentParticle,
                                    neighbors + gid * 200 + 1,
                                    neighbors[gid * 200],
                                    gid);
  //  printf("gid: %d :: lambda: %f\n", gid, lambda[gid]);
}
