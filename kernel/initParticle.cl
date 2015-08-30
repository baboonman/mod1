__kernel void   initParticle(__global float *particles,
                               __global float *particlesVelocity,
                               int maxGID)
{
    int gid = get_global_id(0);
    float   N = 1.0f;

    if (gid > maxGID) {
        return ;
    }
    int pos = gid * 3;
    particles[pos] = (float)(gid % 10) * N - 5.134434f;
    particles[pos + 1] = (float)((gid % 100) / 10.0f) * N + 1.0f;
    particles[pos + 2] = (float)(gid / 100.0f) * N;
    //printf("Init gid: %d, x: %f, y: %f, z: %f\n", pos, particles[pos], particles[pos + 1], particles[pos + 2]);
    particlesVelocity[pos] = 2.0f;
    particlesVelocity[pos + 1] = 0.0f;
    particlesVelocity[pos + 2] = 0.0f;
}
