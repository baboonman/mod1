__kernel void   applyForces(
        __global float      *particles,
        __global float      *particlesVelocity,
        __global float      *particlesProjection,
        __global int        *gridParticles,
        float               coef,
        int                 gridX,
        int                 gridY,
        int                 gridZ,
        int                 nbParticlePerCell,
        int                 maxGID)
{
    int gid = get_global_id(0);
    if (gid > maxGID)
        return ;
    gid *= 3;
    particlesVelocity[gid + 1] -= 0.1f;
    particlesVelocity[gid] -= 0.0f;
    particlesVelocity[gid + 2] -= 0.0f;
    particlesProjection[gid] = particles[gid] + particlesVelocity[gid];
    particlesProjection[gid + 1] = particles[gid + 1] + particlesVelocity[gid + 1];
    particlesProjection[gid + 2] = particles[gid + 2] + particlesVelocity[gid + 2];
    particlesVelocity[gid + 1] = 0.0f;
    particlesVelocity[gid] = 0.0f;
    particlesVelocity[gid + 2] = 0.0f;

//    printf("current: %f.%f.%f\n", particlesProjection[gid], particlesProjection[gid + 1], particlesProjection[gid + 2]);
   // particlesProjection[gid] = 0.0f;
   // particlesProjection[gid + 1] = 0.0f;
   // particlesProjection[gid + 2] = 0.0f;
}
