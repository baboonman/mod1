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
/*
    float   maxX = (float)gridX * coef / 2.0f - 0.01f;
    float   maxY = (float)gridY * coef / 2.0f - 0.01f;
    float   maxZ = (float)gridZ * coef / 2.0f - 0.01f;
    particles[pos] = min(particles[pos], maxX);
    particles[pos] = max(-maxX, particles[pos]);
    particles[pos + 1] = min(particles[pos + 1], maxY);
    particles[pos + 1] = max(-maxY, particles[pos + 1]);
    particles[pos + 2] = min(particles[pos + 2], maxZ);
    particles[pos + 2] = max(-maxZ, particles[pos + 2]);
*/
 //   printf("Grid gid: %d, coef: %f, x: %f, y: %f, z: %f\n", gid, coef, particles[pos], particles[pos + 1], particles[pos + 2]);

    int x = (particles[pos] / coef) + (float)gridX / 2.0f;
    int y = (particles[pos + 1] / coef) + (float)gridY / 2.0f;
    int z = (particles[pos + 2] / coef) + (float)gridZ / 2.0f;
  /*  if (particles[pos + 1] > 50.0f)
    {
        printf("Shit gid: %d :: %f, %f, %f\n", gid, particles[pos], particles[pos + 1], particles[pos + 2]);
     }*/
    if (x >= gridX | x < 0 | y >= gridY | y < 0 | z >= gridZ | z < 0 
          | particles[pos] != particles[pos]) {
  //      printf("gid: %d, %d, %d, %d\n", gid, x, y, z);
        printf("gid: %d :: %f, %f, %f\n", gid, particles[pos], particles[pos + 1], particles[pos + 2]);
        particles[pos] = 0.0f;
        particles[pos + 1] = 10.0f;
        particles[pos + 2] = 0.0f;
        particles[pos] = 0.0f;
        particles[pos + 1] = -1.0f;
        particles[pos + 2] = 0.0f;
        return;
    }
    int id = (nbParticlePerCell + 1) * (x + y * gridX + z * gridX * gridY);
    int offset = atomic_inc(gridParticles + id) + 1;//Update particle counter for cell
//    if (particles[pos] != particles[pos]) {
     //   printf("gid: %d pos : %f\n", gid, particles[pos]);
 //   }
    if (offset >= nbParticlePerCell) {
        printf("gid: %d, FAILED offset(%d, %d) :: %d, %d, %d\n \
               %f, %f, %f :: %f, %f, %f\n",
           gid,
           offset,
           id,
           x,
           y,
           z,
           particles[pos],
           particles[pos + 1],
           particles[pos + 2],
           particlesVelocity[pos],
           particlesVelocity[pos + 1],
           particlesVelocity[pos + 2]
           );
        return ;
    }

    gridParticles[id + offset] = pos;
}
