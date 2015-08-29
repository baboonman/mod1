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
 //   printf("max: %d, cur: %d\n", maxGID, gid);
    int pos = gid * 3;
    particles[pos] = max(particles[pos], gridX * coef);
    particles[pos] = min(0.0f, particles[pos]);
    particles[pos + 1] = max(particles[pos + 1], gridY * coef);
    particles[pos + 1] = min(0.0f, particles[pos + 1]);
    particles[pos + 2] = max(particles[pos + 2], gridZ * coef);
    particles[pos + 2] = min(0.0f, particles[pos + 2]);
/*
*/
    int x = particles[pos] / coef;
    int y = particles[pos + 1] / coef;
    int z = particles[pos + 2] / coef;
    x += gridX / 2;
    y += gridY / 2;
    z += gridZ / 2;
    int id = (nbParticlePerCell + 1) * (x + y * gridX + z * gridX * gridY);
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
  //  gridParticles[id + offset] = pos;
}
