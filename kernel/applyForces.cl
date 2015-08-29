__kernel void   applyForces(
        __global float3     *particles,
        __global float3     *particlesVelocity,
        __global float3     *particlesProjection,
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
    particlesProjection[gid] = particles[gid];
    particlesVelocity[gid].y -= 0.001f;
    particles[gid].y -= 0.01f; 
}
