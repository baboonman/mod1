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
        return;
    int pos = gid * 3;
    particlesVelocity[pos] += 0.0f;
    particlesVelocity[pos + 1] += 0.0f;
    particlesVelocity[pos + 2] += 0.1f;
    for (int i = 0; i < 3; i++) {
        particlesProjection[pos + i] = particles[pos + i] + particlesVelocity[pos + i];
    }
}
