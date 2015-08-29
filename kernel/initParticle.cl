__kernel void   initParticle(__global float *particles,
                               __global float *particlesVelocity,
                               int maxGID)
{
    int gid = get_global_id(0);

    if (gid > maxGID) {
        return ;
    }
    int pos = gid * 3.0f;
    particles[pos] = (float)(gid % 10) - 5.0f;
    particles[pos + 1] = (float)(gid / 10) - 5.0f;
    particles[pos + 2] = -10.0f;
//    printf("Init gid: %d, x: %f, y: %f, z: %f\n", gid, particles[gid].x, particles[gid].y, particles[gid].z);
    particlesVelocity[pos] = 0.0f;
    particlesVelocity[pos + 1] = 0.0f;
    particlesVelocity[pos + 2] = 0.0f;
}
