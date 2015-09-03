#define BOX_SIZE2 BOX_SIZE * BOX_SIZE
#define BOX_HALF_SIZE (((float)BOX_SIZE) / 2.0f)

__kernel void   initParticle(__global float *particles,
                               __global float *particlesVelocity,
                               int maxGID)
{
    int gid = get_global_id(0);
    if (gid > maxGID) {
        return ;
    }
    int pos = gid * 3;
    int calc = gid;

    particles[pos] = ((float)(calc % BOX_SIZE))
        - BOX_HALF_SIZE;
    particles[pos + 1] = ((float)((calc % (BOX_SIZE2)) / BOX_SIZE))
        - BOX_HALF_SIZE;
    particles[pos + 2] = ((float)((calc / (BOX_SIZE2))))
        - BOX_HALF_SIZE;

    particlesVelocity[pos] = 0.0f;
    particlesVelocity[pos + 1] = 0.0f;
    particlesVelocity[pos + 2] = 0.0f;
}
