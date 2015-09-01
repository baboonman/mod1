int     getCellId(int x, int y, int z, int nbParticlePerCell, int gridX, int sizePlane)
{
  //  printf("Here %d, %d, %d\n", x, y, z);
    return (nbParticlePerCell + 1) * (x + y * gridX + z * sizePlane);
}

int    findNeighborsPi(__global int *gridParticles,
                    int currentCell,
                    __global int *neighbours,
                    __global float *particles,
                    int nbNeighbors,
                    int currentParticleId,
                    float3 currentParticle,
                    int display,
                    int x,
                    int y,
                    int z)
{
    int     workingId;
    float3  neighbor;
    float       dist;
    float       H = 1.5f;

    int nbParticleInCell = gridParticles[currentCell];
    if (display) {
        //printf("Q gid: %d, nbParticleInCell: %d, id cell: %d, coord: %d, %d, %d\n", currentParticleId, nbParticleInCell, currentCell, x, y, z);
    }
    
    for (int i = 1; i <= nbParticleInCell; i++) {
        workingId = gridParticles[currentCell + i];
        if (display) {
            //printf("Q gid: %d, working id: %d, id cell: %d\n", currentParticleId, workingId, workingId / 3);
        }
        if (workingId / 3 == currentParticleId)
            continue ;
        if (nbNeighbors > 198)
            return nbNeighbors;
        neighbor.x = particles[workingId];
        neighbor.y = particles[workingId + 1];
        neighbor.z = particles[workingId + 2];
        dist = distance(currentParticle, neighbor);
        //printf("Dist: %f\n", dist);
        if (dist > H)
            continue ;
        neighbours[nbNeighbors + 1] = workingId;
        nbNeighbors++;
    }
    return nbNeighbors;
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
    int     x = particlesProjection[pos] / coef + (float)gridX / 2.0f;
    int     y = particlesProjection[pos + 1] / coef + (float)gridX / 2.0f;
    int     z = particlesProjection[pos + 2] / coef + (float)gridX / 2.0f;
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

    currentParticle.x = particlesProjection[pos];
    currentParticle.y = particlesProjection[pos + 1];
    currentParticle.z = particlesProjection[pos + 2];

    xGridStop = min(x + 1, gridX - 1);
    yGridStop = min(y + 1, gridY - 1);
    zGridStop = min(z + 1, gridZ - 1);

    xGrid = max(x - 1, 0);
   // printf("F gid: %d :: %d, %d, %d\n", gid, xGridStop, yGridStop, zGridStop);
   // printf("S gid: %d :: %d, %d, %d, xGrid: %d\n", gid, x, y, z, xGrid);
    for (; xGrid <= xGridStop; xGrid++) {
        yGrid = max(y - 1, 0);
        for (; yGrid <= yGridStop; yGrid++) {
            zGrid = max(z - 1, 0);
            for (; zGrid <= zGridStop; zGrid++) {
           //      printf("T gid: %d :: %d, %d, %d\n", gid, xGrid, yGrid, zGrid);
                currentCell = getCellId(
                                        xGrid,
                                        yGrid,
                                        zGrid,
                                        nbParticlePerCell,
                                        gridX,
                                        sizePlane);
                nbNeighbors = findNeighborsPi(gridParticles, currentCell, neighbors + gid * 200, particlesProjection, nbNeighbors, gid, currentParticle,
                                                yGrid == 10 && zGrid == 10,
                                                xGrid, yGrid, zGrid);
            }
        }
    }
    //printf("Findneighbors gid: %d, nb: %d\n", gid, nbNeighbors);
    neighbors[gid * 200] = nbNeighbors;
}
