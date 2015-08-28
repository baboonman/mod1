
int     getCellId(int x, int y, int z, int nbParticlePerCell, int gridX, int sizePlane)
{
    return (nbParticlePerCell + 1) * (x + y * gridX + z * sizePlane);
}

__kernel void   addConst (
        __global float3     *particles,
        __global float3     *particlesVelocity,
        __global float3     *particlesProjection,
        __global int        *gridParticles,
        float               coef,
        int                 gridX,
        int                 gridY,
        int                 gridZ,
        int                 nbParticlePerCell,
        int                 maxGID,
        float               h
        )
{
    int gid = get_global_id(0);
//    printf("max: %d\n", maxGID);
    if (gid > maxGID)
        return;
  //  int pos = gid * 3;
    int x = particlesProjection[gid].x / coef;
    int y = particlesProjection[gid].y / coef;
    int z = particlesProjection[gid].z / coef;
    int sizePlane = gridX * gridZ;
    int currentCell = getCellId(x, y, z, nbParticlePerCell, gridX, sizePlane); 

    int     nbParticleInCell = gridParticles[currentCell];
    int     workingId;

    float3 currentParticle = particlesProjection[gid];
    float   dist;

    float   w;
    w = 1.0f / (M_PI * pown(h, 3));
    for (int i = 1; i <= nbParticleInCell; i++) {
        workingId = gridParticles[currentCell + i];
        float3 coord = particlesProjection[workingId];
        dist = distance(currentParticle, coord);
        if (dist > 2h) {
            continue ;
        }
    }
}
