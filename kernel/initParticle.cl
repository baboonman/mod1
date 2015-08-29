__kernel void   initParticle(__global float3 *particles,
                               __global float3 *particlesVelocity,
                               int maxGID)
{
    int gid = get_global_id(0);

    if (gid > maxGID) {
        return ;
    }
    particles[gid].x = (float)(gid % 10) - 5.0f;
    particles[gid].y = (float)(gid / 10) - 5.0f;
    particles[gid].z = -10.0f;
    particlesVelocity[gid].x = 0.0f;
    particlesVelocity[gid].y = 0.0f;
    particlesVelocity[gid].z = 0.0f;
}
