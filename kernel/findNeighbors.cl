int     getCellId(int x, int y, int z, int nbParticlePerCell, int gridX, int sizePlane)
{
  //  printf("Here %d, %d, %d\n", x, y, z);
    return (nbParticlePerCell + 1) * (x + y * gridX + z * sizePlane);
}

int    findNeighborsPi(__global int *gridParticles,
                    int currentCell,
                    __global int *neighbours,
                    __global float *particles,
                    int nbNeighbours,
                    int currentParticleId,
                    float3 currentParticle)
{
    int     workingId;
    float3  neighbor;
    float       dist;

    int nbParticleInCell = gridParticles[currentCell];
    for (int i = 1; i <= nbParticleInCell; i++) {
        workingId = gridParticles[currentCell + i];
        if (workingId == currentParticleId)
            continue ;
        if (nbNeighbours > 199)
            return i;
        neighbor.x = particles[workingId];
        neighbor.y = particles[workingId + 1];
        neighbor.z = particles[workingId + 2];
        dist = distance(currentParticle, neighbor);
        if (dist < 2.0f) {
            neighbours[nbNeighbours + i] = workingId;
        }
        //printf("Dist: %f\n", dist);
    }
    return nbParticleInCell;
}

__kernel void   findNeighbors(
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
    int     x = particlesProjection[pos] / coef;
    int     y = particlesProjection[pos + 1] / coef;
    int     z = particlesProjection[pos + 2] / coef;
    int     sizePlane = gridX * gridY;
    int     currentCell;
    float3  currentParticle;
    int     xGrid;
    int     yGrid;
    int     zGrid;
    int     xGridStop;
    int     yGridStop;
    int     zGridStop;
    int     nbNeighbors = 0;

    x += gridX / 2;
    y += gridY / 2;
    z += gridZ / 2;

    currentParticle.x = particlesProjection[pos];
    currentParticle.y = particlesProjection[pos + 1];
    currentParticle.z = particlesProjection[pos + 2];
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
                nbNeighbors += findNeighborsPi(gridParticles, currentCell, neighbors + gid * 200, particles, nbNeighbors, gid, currentParticle);
            }
        }
    }
    neighbors[gid * 200] = nbNeighbors;
}
