// 归并排序 链表型 非递归 C++ 实现
#include<bits/stdc++.h>
using namespace std;
int n, a[100001];
int nxt[100001];
int cachedSortedListHead[32];
int getValue(int index) { return a[index]; }
int merge(int A, int B, int Asize, int Bsize) {
    int Alen = 0, Blen = 0;
    int newHead = getValue(A) > getValue(B) ? B : A;
    int pointer = newHead;
    if (getValue(A) > getValue(B)) {
        Blen++;
        B = nxt[B];
    } else {
        Alen++;
        A = nxt[A];
    }
    while (Alen < Asize && Blen < Bsize) {
        if (getValue(A) > getValue(B)) {
            nxt[pointer] = B;
            pointer = B;
            B = nxt[B];
            Blen++;
        } else {
            nxt[pointer] = A;
            pointer = A;
            A = nxt[A];
            Alen++;
        }
    }
    // 一定要记得把两个链表连起来，不然就爆了！！！
    if (Alen < Asize) nxt[pointer] = A;
    if (Blen < Bsize) nxt[pointer] = B;
    return newHead;
}
int main() {
    cin >> n;
    for (int i = 0; i < n; i ++) cin >> a[i];
    for (int i = 0; i < n; i++) nxt[i] = i + 1;
    memset(cachedSortedListHead, 0xff, sizeof cachedSortedListHead);

    int currentEntity = 0;
    for (int i = 0; i < n; i++) {
        int currentHead = currentEntity;
        currentEntity = nxt[currentEntity];
        for (int j = 0; j < 32; j++) {
            if (cachedSortedListHead[j] == -1) {
                cachedSortedListHead[j] = currentHead;
                break;
            }
            int A = cachedSortedListHead[j];
            int B = currentHead;
            cachedSortedListHead[j] = -1;
            currentHead = merge(A, B, 1 << j, 1 << j);
        }
    }
    // 剩余片段合并
    int head = -1, currentLen = 0;
    for (int i = 0; i < 32; i++) {
        if (cachedSortedListHead[i] == -1) continue;
        if (head == -1) {
            head = cachedSortedListHead[i];
            currentLen = 1 << i;
            continue;
        }
        int A = head, B = cachedSortedListHead[i];
        int Asize = currentLen, Bsize = 1 << i;
        cachedSortedListHead[i] = 0;
        head = merge(A, B, Asize, Bsize);
        currentLen = Asize + Bsize;
    }
    // 打印
    for (int i = 0; i < n; i++) cout << a[head] << " ", head = nxt[head];
}