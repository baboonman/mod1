void    getDir(int id, int lineWidth, int size, int *dirId) {
    int tmp;

    dirId[0] = id - lineWidth;//NORTH
    dirId[1] = -1;//SOUTH
    if ((tmp = id + lineWidth) < size) {
        dirId[1] = tmp;
    }
    dirId[2] = -1;//WEST
    if (id % lineWidth != 0) {
        dirId[2] = id - 1;
    }
    dirId[3] = -1;//EAST
    if ((id + 1) % lineWidth != 0) {
        dirId[3] = id + 1;
    }
}

void    calcCellsToUpdate(int *dirId, __global float *vbo, float currentHeight, int *cells) {
    int     cellsNumber = 0;

    for (int i = 0; i < 4; i++) {
        if (dirId[i] < 0 || vbo[dirId[i]] > currentHeight) {
            continue ;
        }
        cells[cellsNumber] = dirId[i];
        cellsNumber++;
    }
}

void    updateCell(__global float *vbo, int id, int *cellToUpdateIds, double currentAmount) {
    float   diffAdjacent[4];
    int     diffAdjacentId[4];
    int     i;
    float   tmp;
    float   tmpWeight;
    int     tmpId;
    int     tot = 0;
    float   sumAdjacent = 0.0f;
    float   weightDiff[4];
    float   diff;
    float   available;

    diffAdjacent[0] = -1.0f;
    diffAdjacent[1] = -1.0f;
    diffAdjacent[2] = -1.0f;
    diffAdjacent[3] = -1.0f;
    weightDiff[0] = 0.0f;
    weightDiff[1] = 0.0f;
    weightDiff[2] = 0.0f;
    weightDiff[3] = 0.0f;
    for (int i = 0; i < 4 && cellToUpdateIds[i] >= 0; i++) {
       diffAdjacent[i] = vbo[cellToUpdateIds[i]];
       diffAdjacentId[i] = cellToUpdateIds[i];
 //      sumAdjacent += diffAdjacent[i];
       tot++;
    }
    /*
    for (int i = 0; i < tot; i++) {
        if (diffAdjacent[i] <= 0.001f) {
            weightDiff[i] = 0.0f;
            continue ;
        }
        weightDiff[i] = diffAdjacent[i] / sumAdjacent;
    }
    */
    i = 1;
    while (i < tot) {

        if (diffAdjacent[i] > diffAdjacent[i - 1]) {
            tmp = diffAdjacent[i];
            tmpId = diffAdjacentId[i];
        //    tmpWeight = weightDiff[i];

            diffAdjacentId[i] = diffAdjacentId[i - 1];
            diffAdjacent[i] = diffAdjacent[i - 1];
        //    weightDiff[i] = weightDiff[i - 1];

            diffAdjacent[i - 1] = tmp;
            diffAdjacentId[i - 1] = tmpId;
       //     weightDiff[i - 1] = tmpWeight;
            i = 1;
        }
        i++;
    }
    //printf("Diff: %f, %f, %f, %f\nwieght: %f, %f, %f, %f\n\n",
           diffAdjacent[0],
           diffAdjacent[1],
           diffAdjacent[2],
           diffAdjacent[3],
           weightDiff[0],
           weightDiff[1],
           weightDiff[2],
           weightDiff[3]);

    for (int i = 0; i < tot; i++) {
        tmp = currentAmount - diffAdjacent[tot - 1] / 2.0f;
        if (tmp <= 0.00001f) {
            break ;
        }
        diff = diffAdjacent[i + 1] - diffAdjacent[i];
        available = currentAmount - diffAdjacent[i];
        diff = min(diff, available);
        diff = min(diff, tmp);
        for (int j = 0; j <= i; j++) {
            vbo[diffAdjacentId[i]] += diff;
        }
        currentAmount -= diff * (i + 1);
        /*
        diff = diffAdjacent[i] * weightDiff[i] * 0.1;
        if (currentAmount - diff <= vbo[diffAdjacentId[i]]) {
            diff = diffAdjacent[i] / 2.0f;
        }
        vbo[diffAdjacentId[i]] += diff;
        currentAmount -= diff;
        */
    }
    vbo[id] = currentAmount;
}

void    generateWater(__global float *vbo, int id, int lineWidth, int size) {
    int     dirId[4];
    int     cellToUpdateIds[4] = {-1, -1, -1, -1};
    float   total = vbo[id];
    float   currentWaterAmount = total;
    bool    isUnder = true;
    int     nbDir = 1;

    /*
    if (currentWaterAmount < 0.0000001f) {
        vbo[id] = 0.0f;
        return ;
    }
    */
    getDir(id, lineWidth, size, dirId);
    calcCellsToUpdate(dirId, vbo, currentWaterAmount, cellToUpdateIds);
    updateCell(vbo, id, cellToUpdateIds, currentWaterAmount);
    /*
    for (int i = 0; i < 4; i++) {
        if (dirId[i] < 0) {
            continue ;
        }
        if (vbo[dirId[i]] > currentWaterAmount) {
            isUnder = false;
        }
        printf("to: %f\n", total);
        total += vbo[dirId[i]];
        nbDir++;

    }
    if (isUnder) {
        return ;
    }
    currentWaterAmount = total / nbDir;
    for (int i = 0; i < 4; i++) {
        if (dirId[i] < 0) {
            continue ;
        }
        vbo[dirId[i]] = total;
    }
    */
}

__kernel void   waterSimulation(
        __global float *vbo,
        int step,
        int lineWidth,
        int size,
        int maxGid
        )
{
    int gid = get_global_id(0);
    if (gid >= maxGid) {
        return ;
    }
    if (gid < lineWidth) {
        vbo[gid] = 1.0f;
    }

    int i;
    int j;
    int res;

    i = gid * 3;
    j = (i / lineWidth) * (3 * lineWidth);
    res = i % lineWidth + j;
    res += step % 3 + lineWidth * (step / 3);
    generateWater(vbo, res, lineWidth, size);
}
