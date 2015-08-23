__kernel void   waterSimulation(__global float *vbo) {
    int gid = get_global_id(0);
    vbo[gid] = 2;
}
