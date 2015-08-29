__kernel void   initParticle(__global float *particles,
                               __global float3 *particlesVelocity,
                               int maxGID)
{
    int gid = get_global_id(0);

    if (gid > maxGID) {
        return ;
    }
    particles[gid * 3] = (float)(gid % 10) - 5.0f;
    particles[gid * 3 + 1] = (float)(gid / 10) - 5.0f;
    particles[gid * 3 + 2] = -10.0f;
//    printf("Init gid: %d, x: %f, y: %f, z: %f\n", gid, particles[gid].x, particles[gid].y, particles[gid].z);
    particlesVelocity[gid].x = 0.0f;
    particlesVelocity[gid].y = 0.0f;
    particlesVelocity[gid].z = 0.0f;
}
