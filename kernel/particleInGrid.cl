__kernel void   particleInGrid(
        __global float      *particles,
        __global float      *particlesVelocity,
        __global float      *particleProjection,
        __global int        *gridParticles,
        float               coef,
        int                 gridX,
        int                 gridY,
        int                 gridZ,
        int                 nbParticlePerCell,
        int                 maxGID)
{
    int gid = get_global_id(0);
//    printf("max: %d\n", maxGID);
    if (gid > maxGID)
        return;
    int pos = gid * 3;
    particles[pos] = (float)(gid % 10) + 4.0f;
    particles[pos + 1] = (float)(gid / 10);
    particles[pos + 2] = 5.0f ;
    particlesVelocity[pos] = 0.0f;
    particlesVelocity[pos + 1] = 0.0f;
    particlesVelocity[pos + 2] = 0.0f;
    int x = particles[pos] / coef;
    int y = particles[pos + 1] / coef;
    int z = particles[pos + 2] / coef;
 /*   printf("%f, %f, %f ::: %d, %d, %d\n",
           (float)(gid % 10) + 4.0f,
           (float)(gid / 10),
           5.0f,
           x,
           y,
           z);*/
    int id = (nbParticlePerCell + 1) * (x + y * gridX + z * gridX * gridZ);
    int offset = atomic_inc(gridParticles + id) + 1;//Update particle counter for cell
    if (offset >= nbParticlePerCell) {
        printf("FAILED, offset(%d), %f, %f, %f :: %d, %d, %d\n",
           offset,
           (float)(gid % 10) + 4.0f,
           (float)(gid / 10),
           5.0f,
           x,
           y,
           z);
        return ;
    }
    gridParticles[id + offset] = pos;
}
