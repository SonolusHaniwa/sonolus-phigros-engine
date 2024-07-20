SonolusApi getValue(let index) {
    FUNCBEGIN
    Return(EntitySharedMemoryArray[index].get(1));
    return VAR;
}

SonolusApi merge(let a, let b, let Asize, let Bsize) {
    FUNCBEGIN
    var Alen = 0, Blen = 0, A = a, B = b;
    var newHead = If(getValue(A) > getValue(B), B, A);
    var pointer = newHead.get();
    IF (getValue(A) > getValue(B)) {
        Blen = Blen + 1; 
        B = EntitySharedMemoryArray[B].get(0);
    } ELSE {
        Alen = Alen + 1; 
        A = EntitySharedMemoryArray[A].get(0);
    } FI
    WHILE (Alen < Asize && Blen < Bsize) {
        IF (getValue(A) > getValue(B)) {
            EntitySharedMemoryArray[pointer].set(0, B);
            pointer = B.get();
            B = EntitySharedMemoryArray[B].get(0);
            Blen = Blen + 1;
        } ELSE {
            EntitySharedMemoryArray[pointer].set(0, A);
            pointer = A.get();
            A = EntitySharedMemoryArray[A].get(0);
            Alen = Alen + 1;
        } FI
    } DONE
    // 一定要记得把两个链表连起来，不然就爆了！！！
    IF (Alen < Asize) EntitySharedMemoryArray[pointer].set(0, A); FI
    IF (Blen < Bsize) EntitySharedMemoryArray[pointer].set(0, B); FI
    	// DebugPause();
    Return(newHead);
    return VAR;
}

SonolusApi StageController::calcCombo() {
    FUNCBEGIN
    // 获取实体总数 n
    // 通过 EntityIndex 是否正确判断是否为最后一个实体
    // 时间复杂度 O(n)
    var entityCount = 0;
    WHILE (EntityInfoArray[entityCount].get(0) == entityCount) entityCount = entityCount + 1; DONE
    // 构建按键实体链表
    // 设其长度为 m 
    // 时间复杂度 O(n)
    var next = 0, lineLength = 0;
    FOR (i, 0, entityCount, 1) {
        var ii = entityCount - 1 - i;
        var archetypeIndex = EntityInfoArray[ii].get(1);
        IF (
            archetypeIndex == getArchetypeId(NormalNote) ||
            archetypeIndex == getArchetypeId(DragNote) ||
            archetypeIndex == getArchetypeId(HoldNote) ||
            archetypeIndex == getArchetypeId(FlickNote)
        ) {
            lineLength = lineLength + 1;
            EntitySharedMemoryArray[ii].set(0, next);
            next = ii.get();
        } FI
    } DONE
    // 链表的归并排序非递归版本
    // Sonolus 不支持任何形式的递归函数
    // 因为归并排序的非递归版本就像线段树上传一样
    // 因此需要提前申请数组来保存已经排好序的片段
    // 其中该数组的第 i 位存储长为 2 ^ i 的片段的头实体
    // 时间复杂度 O(mlogm)
    // 空间复杂度 O(logm)
    // 该算法在 C++ 中的实现: 见根目录下的 mergeSort.cpp
    // 该算法的正确性: 见 https://www.luogu.com.cn/record/167007458
    Array<TemporaryMemoryId, var> cachedSortedListHead(32);
    var currentEntity = next.get();
    FOR (i, 0, lineLength, 1) {
        var currentHead = currentEntity.get();
        currentEntity = EntitySharedMemoryArray[currentEntity].get(0);
        FOR (j, 0, 32, 1) {
            IF (cachedSortedListHead.get(j) == 0) {
                cachedSortedListHead.set(j, currentHead); 
                BREAK;
            } FI
            var A = cachedSortedListHead.get(j);
            var B = currentHead.get();
            cachedSortedListHead.set(j, 0);
            currentHead = merge(A, B, Power({2, j}), Power({2, j}));
        } DONE
    } DONE
    // 剩余片段合并
    var head = -1, currentLen = 0;
    FOR (i, 0, 32, 1) {
        IF (cachedSortedListHead.get(i) == 0) CONTINUE; FI
        IF (head == -1) {
            head.set(cachedSortedListHead.get(i));
            currentLen.set(Power({2, i}));
            CONTINUE;
        } FI
        var A = head.get();
        var B = cachedSortedListHead.get(i);
        var Asize = currentLen.get(), Bsize = Power({2, i});
        cachedSortedListHead.set(i, 0);
        head = merge(A, B, Asize, Bsize);
        currentLen = Asize + Bsize;
    } DONE
    EntityMemory.set(1, head);
    EntityMemory.set(2, lineLength);
    // 验证(只要没有输出就是正序)
    // FOR (i, 0, lineLength, 1) {
    //     IF (head < lineLength - 1 && getValue(head) > getValue(EntitySharedMemoryArray[head].get(0))) Debuglog(head); FI
    //     head = EntitySharedMemoryArray[head].get(0);
    // } DONE
    return VOID;
}