SonolusApi getValue(var index) {
    return EntitySharedMemoryArray[index].generic[1];
}

SonolusApi merge2(var a, var b, var Asize, var Bsize) {
    var Alen = 0, Blen = 0, A = a, B = b;
    var newHead = If(getValue(A) > getValue(B), B, A);
    var pointer = newHead;
    if (getValue(A) > getValue(B)) {
        Blen++; 
        B = EntitySharedMemoryArray[B].generic[0];
    } else {
        Alen++; 
        A = EntitySharedMemoryArray[A].generic[0];
    }
    while (Alen < Asize && Blen < Bsize) {
        if (getValue(A) > getValue(B)) {
            EntitySharedMemoryArray[pointer].generic[0] = B;
            pointer = B;
            B = EntitySharedMemoryArray[B].generic[0];
            Blen++;
        } else {
            EntitySharedMemoryArray[pointer].generic[0] = A;
            pointer = A;
            A = EntitySharedMemoryArray[A].generic[0];
            Alen++;
        }
    }
    // 一定要记得把两个链表连起来，不然就爆了！！！
    if (Alen < Asize) EntitySharedMemoryArray[pointer].generic[0] = A;
    if (Blen < Bsize) EntitySharedMemoryArray[pointer].generic[0] = B;
    return newHead;
}

SonolusApi StageController::calcCombo() {
    // 获取实体总数 n
    // 通过 EntityIndex 是否正确判断是否为最后一个实体
    // 时间复杂度 O(n)
    var entityCount = 0;
    while (EntityInfoArray[entityCount].index == entityCount) entityCount++;
    // 构建按键实体链表
    // 设其长度为 m 
    // 时间复杂度 O(n)
    var next = 0, lineLength = 0;
    for (var i = 0; i < entityCount; i++) {
        var ii = entityCount - 1 - i;
        var archetypeIndex = EntityInfoArray[ii].archetype;
        if (
            archetypeIndex == getAid(NormalNote) ||
            archetypeIndex == getAid(DragNote) ||
            archetypeIndex == getAid(HoldNote) ||
            archetypeIndex == getAid(FlickNote)
        ) {
            lineLength = lineLength + 1;
            EntitySharedMemoryArray[ii].generic[0] = next;
            next = ii;
        }
    }
    // 链表的归并排序非递归版本
    // Sonolus 不支持任何形式的递归函数
    // 因为归并排序的非递归版本就像线段树上传一样
    // 因此需要提前申请数组来保存已经排好序的片段
    // 其中该数组的第 i 位存储长为 2 ^ i 的片段的头实体
    // 时间复杂度 O(mlogm)
    // 空间复杂度 O(logm)
    // 该算法在 C++ 中的实现: 见根目录下的 mergeSort.cpp
    // 该算法的正确性: 见 https://www.luogu.com.cn/record/167007458
    Array<var, 32> cachedSortedListHead;
    for (var i = 0; i < 32; i++) cachedSortedListHead[i] = -1;
    var currentEntity = next;
    for (var i = 0; i < lineLength; i++) {
        var currentHead = currentEntity;
        currentEntity = EntitySharedMemoryArray[currentEntity].generic[0];
        for (var j = 0; j < 32; j++) {
            if (cachedSortedListHead[j] == -1) {
                cachedSortedListHead[j] = currentHead; 
                // DebugLog(i); DebugLog(j); DebugLog(currentHead);
                break;
            }
            var A; A = cachedSortedListHead[j];
            var B = currentHead;
            cachedSortedListHead[j] = -1;
            currentHead = merge2(A, B, Power({2, j}), Power({2, j}));
        }
    }
    // 剩余片段合并
    var head = -1, currentLen = 0;
    for (var i = 0; i < 32; i++) {
        if (cachedSortedListHead[i] == -1) continue;
        if (head == -1) {
            head = cachedSortedListHead[i];
            currentLen = Power({2, i});
            continue;
        }
        var A = head;
        var B; B = cachedSortedListHead[i];
        cachedSortedListHead[i] = 0;
        var Asize = currentLen, Bsize = Power({2, i});
        head = merge2(A, B, Asize, Bsize);
        currentLen = Asize + Bsize;
    }
    EntityMemory[2] = head;
    EntityMemory[3] = lineLength;
    // 验证(只要没有输出就是正序)
    // for (var i = 0; i < lineLength; i++) {
    //     if (head < lineLength - 1 && getValue(head) > getValue(EntitySharedMemoryArray[head].generic[0])) DebugLog(head);
    //     head = EntitySharedMemoryArray[head].generic[0];
    // }
}