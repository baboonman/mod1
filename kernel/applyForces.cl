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
    float   minVel = -5.0f;
    float   maxVel = 5.0f;
    if (gid > maxGID)
        return ;
    gid *= 3;
    particlesVelocity[gid] = min(particlesVelocity[gid] + 0.00f, maxVel);
    particlesVelocity[gid] = max(particlesVelocity[gid] - 0.00f, minVel);

    particlesVelocity[gid + 1] = min(particlesVelocity[gid + 1] + 0.00f, maxVel);
    particlesVelocity[gid + 1] = max(particlesVelocity[gid + 1] - 0.05f, minVel);

    particlesVelocity[gid + 2] = min(particlesVelocity[gid + 2] + 0.00f, maxVel);
    particlesVelocity[gid + 2] = max(particlesVelocity[gid + 2] - 0.00f, minVel);

    particlesProjection[gid] = particles[gid] + particlesVelocity[gid];
    particlesProjection[gid + 1] = particles[gid + 1] + particlesVelocity[gid + 1];
    particlesProjection[gid + 2] = particles[gid + 2] + particlesVelocity[gid + 2];

/*
    printf("current: %f, %f, %f :: %f, %f, %f :: %f, %f, %f\n",
           particlesProjection[gid], 
           particlesProjection[gid + 1],
           particlesProjection[gid + 2],
           particlesVelocity[gid], 
           particlesVelocity[gid + 1],
           particlesVelocity[gid + 2],
           particles[gid], 
           particles[gid + 1],
           particles[gid + 2]
           );

*/
   // particlesProjection[gid] = 0.0f;
   // particlesProjection[gid + 1] = 0.0f;
   // particlesProjection[gid + 2] = 0.0f;
}
