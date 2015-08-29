
float3      calcDelta(__global float *particles, float3 currentParticle, int *neighbors, int nbNeighbors, float lambda) {
    float3  delta;
    float3  neighbor;
    float   sCorr = 0.0f;

    for (int i = 0; i < nbNeighbors; i++) {
        neighbor.x = particles[neighbors[i]];
        neighbor.y = particles[neighbors[i] + 1];
        neighbor.z = particles[neighbors[i] + 2];

   //     delta += (lambda + neighbors)
    }
    return delta;
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
    int     sizePlane = gridX * gridY;
    int     currentCell;
    float   c;
    float3  w;
    int     nbParticleInCell;
    int     workingId;
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

