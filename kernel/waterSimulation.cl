__kernel void   waterSimulation(__global float *vbo, int sizeBuffer) {
    int gid = get_global_id(0);
    if (gid >= sizeBuffer) {
        return ;
    }
//    printf("gid: %d, size: %d\n", gid, sizeBuffer);
    vbo[gid] = vbo[gid] * 1.01f;
}
