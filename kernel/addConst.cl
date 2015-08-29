
int     getCellId(int x, int y, int z, int nbParticlePerCell, int gridX, int sizePlane)
{
    return (nbParticlePerCell + 1) * (x + y * gridX + z * sizePlane);
}

float   processCell(__global int    *gridParticles,
                    __global float3 *particlesProjection,
                    int             nbParticleInCell,
                    int             currentCell,
                    float3          currentParticle,
                    int             currentParticleId,
                    float           h,
                    float           c,
                    float           res)
{
    int     workingId;
    float   dist;
    float   w;
    float3  forces;

    for (int i = 1; i <= nbParticleInCell; i++) {
        workingId = gridParticles[currentCell + i];
        if (workingId == currentParticleId)
            continue ;
        float3 coord = particlesProjection[workingId];
        dist = distance(currentParticle, coord);
        if (dist <= 2.0f)
            printf("dist: %f\n", dist);
        if (dist > 2.0f * h) {
            continue ;
        }
        if (dist < h)
            w = c * (3 / 2) * ((2 / 3) - pown((dist / h), 2) + (1 / 2) * pown((dist / h), 3));
        else
            w = c * (1 / 4) * pown((2 - dist / h), 3);
        //printf("w: %f, h: %f, dist: %f\n", w, h, dist);
    }
    return res;
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
    if (gid > maxGID)
        return;

    int     x = particlesProjection[gid].x / coef;
    int     y = particlesProjection[gid].y / coef;
    int     z = particlesProjection[gid].z / coef;
    int     sizePlane = gridX * gridZ;
    int     currentCell;
    float   c;
    float   w;
    int     nbParticleInCell;
    int     workingId;

    float3 currentParticle = particlesProjection[gid];

    c = 1.0f / (3.14159f * pown(h, 3));
    for (int xGrid = -1; xGrid < 2; xGrid++) {
        for (int yGrid = -1; yGrid < 2; yGrid++) {
            for (int zGrid = -1; zGrid < 2; zGrid++) {
                currentCell = getCellId(
                                        xGrid + x,
                                        yGrid + y,
                                        zGrid + z,
                                        nbParticlePerCell,
                                        gridX,
                                        sizePlane);
                nbParticleInCell = gridParticles[currentCell];
                w = processCell(gridParticles,
                                particlesProjection,
                                nbParticleInCell,
                                currentCell,
                                currentParticle,
                                gid,
                                h,
                                c,
                                w);
            }
        }
    }
}
