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
    if (gid > maxGID)
        return;
    gid *= 3.0f;
    particles[gid] = particlesProjection[gid];
    particles[gid + 1] = particlesProjection[gid + 1];
    particles[gid + 2] = particlesProjection[gid + 2];

    float x = gridX * coef / 2.0f;
    float y = gridY * coef / 2.0f;
    float z = gridZ * coef / 2.0f;

    if (particles[gid] < -x) {
        particles[gid] = -x;
    }

    if (particles[gid] > x) {
        particles[gid] = x;
    }

    if (particles[gid + 1] < -y) {
        particles[gid + 1] = -y;
    }

    if (particles[gid + 1] > y) {
        particles[gid + 1] = y;
    }
    
    if (particles[gid + 2] < -z) {
        particles[gid + 2] = -z;
    }

    if (particles[gid + 2] > z) {
        particles[gid + 2] = z;
    }
}
