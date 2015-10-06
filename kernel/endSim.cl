__kernel void   endSim(
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
    float   scaleX = 1.0f;
    float   scaleY = 1.0f;
    float   scaleZ = 1.0f;
    if (gid > maxGID)
        return;
    gid *= 3;
    particlesVelocity[gid] += particlesProjection[gid] - particles[gid];
    particlesVelocity[gid + 1] += particlesProjection[gid + 1] - particles[gid + 1];
    particlesVelocity[gid + 2] += particlesProjection[gid + 2] - particles[gid + 2];

    particles[gid] += (particlesProjection[gid] - particles[gid]) * scaleX;
    particles[gid + 1] += (particlesProjection[gid + 1] - particles[gid + 1])  * scaleY;
    particles[gid + 2] += (particlesProjection[gid + 2] - particles[gid + 2])  * scaleZ;
;printf("LALALA gid: %d :: %f, %f, %f    %f, %f, %f\n", gid / 3, particles[gid], particles[gid + 1], particles[gid + 2], particlesProjection[gid], particlesProjection[gid + 1], particlesProjection[gid + 2]);
}
