float   poly6Kernel(float3 currentParticle, float3 neighbor) {
    float   H = 1.5f;// smoothing radius

    float r = distance(currentParticle, neighbor);
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
    float3  cI = -1.0f / 1000.0f * spikyGradientKernel(currentParticle, neighbor);
    return cI;
}

float3  calcCiGradientATI(__global float *particles, float3 currentParticle, int *neighbors, int nbNeighbors) {
    float3  accum = float3(0.0f);
    float3  neighbor;

    for (int i = 0; i < nbNeighbors; i++) {
        neighbor.x = particles[neighbors[i]];
        neighbor.y = particles[neighbors[i] + 1];
        neighbor.z = particles[neighbors[i] + 2];
        accum += spikyGradientKernel(currentParticle, neighbor);
    }
    return 1.0f / 1000.0f * accum;
}

float   calcRo(__global float *particles, float3 currentParticle, int *neighbors, int nbNeighbors)
{
    float   ro = 0.0f;
    float3  neighbor;

    for (int i = 0; i < nbNeighbors; i++) {
        neighbor.x = particles[neighbors[i]];
        neighbor.y = particles[neighbors[i] + 1];
        neighbor.z = particles[neighbors[i] + 2];
       ro += poly6Kernel(currentParticle, neighbor);
    }
    return ro;
}

float   calcLambda(__global float *particles, float3 currentParticle, int *neighbors, int nbNeighbors)
{
    float   pI = calcRo(particles, currentParticle, neighbors, nbNeighbors);
    float   cI =  (pI / 1000.0f) - 1.0f;
    float   cIGradient, sumGradient = 0.0f;
    float3  neighbor;

    for (int i = 0; i < nbNeighbors; i++) {
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

int     getCellId(int x, int y, int z, int nbParticlePerCell, int gridX, int sizePlane)
{
  //  printf("Here %d, %d, %d\n", x, y, z);
    return (nbParticlePerCell + 1) * (x + y * gridX + z * sizePlane);
}

int    findNeighbors(__global int *gridParticles,//TODO Remove particle by distance
                    int currentCell,
                    int *neighbours,
                    int nbNeighbours,
                    int currentParticleId)
{
    int     workingId;

    int nbParticleInCell = gridParticles[currentCell];
    for (int i = 1; i <= nbParticleInCell; i++) {
        workingId = gridParticles[currentCell + i];
        if (workingId == currentParticleId)
            continue ;
        if (nbNeighbours > 255)
            return i;
        neighbours[nbNeighbours + i - 1] = workingId;
    }
    return nbParticleInCell;
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
        __global float      *lambda
        )
{
    int gid = get_global_id(0);
    if (gid > maxGID)
        return;

    gid *= 3;

    int     x = particlesProjection[gid] / coef;
    int     y = particlesProjection[gid + 1] / coef;
    int     z = particlesProjection[gid + 2] / coef;
    int     currentCell;
    float3  currentParticle;
    int     xGrid;
    int     yGrid;
    int     zGrid;
    int     xGridStop;
    int     yGridStop;
    int     zGridStop;
    int     neighbors[256];
    int     nbNeighbors = 0;

    x += gridX / 2;
    y += gridY / 2;
    z += gridZ / 2;

    currentParticle.x = particlesProjection[gid];
    currentParticle.y = particlesProjection[gid + 1];
    currentParticle.z = particlesProjection[gid + 2];
    xGrid = max(x - 1, 0);
    xGridStop = min(x + 1, gridX - 1);
    yGridStop = min(y + 1, gridY - 1);
    zGridStop = min(z + 1, gridZ - 1);
    for (; xGrid < xGridStop; xGrid++) {
        yGrid = max(y - 1, 0);
        for (; yGrid < yGridStop; yGrid++) {
            zGrid = max(z - 1, 0);
            for (; zGrid < zGridStop; zGrid++) {
                currentCell = getCellId(
                                        xGrid,
                                        yGrid,
                                        zGrid,
                                        nbParticlePerCell,
                                        gridX,
                                        sizePlane);
                nbNeighbors += findNeighbors(gridParticles, currentCell, neighbors, nbNeighbors, gid);
            }
        }
    }

    for (int i = 0; i < 3; i++) {
        calcLambda(particlesProjection, currentParticle, neighbors, nbNeighbors);
        calcDelta(particlesProjection, currentParticle, neighbors, nbNeighbors, 0.0f);
    }
}
