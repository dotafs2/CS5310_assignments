#ifdef __cplusplus
extern "C" {
#endif

#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "Image.h"

    // Edge linked list
    typedef struct EdgeRec {
        float x0, y0;          // 起始点的坐标
        float x1, y1;          // 终止点的坐标
        int yStart, yEnd;      // 扫描线起始和结束位置
        float xIntersect;      // 当前扫描线与边缘的交点x坐标
        float dxPerScan;       // 每个扫描线的x坐标变化量
        float zIntersect;      // 当前扫描线与边缘的交点z坐标
        float dzPerScan;       // 每个扫描线的z坐标变化量
        Color cIntersect;      // 当前扫描线与边缘的交点颜色
        Color dcPerScan;       // 每个扫描线的颜色变化量
    } EdgeRec;

    typedef struct Node {
        void *data;
        struct Node *next;
    } Node;

    typedef struct {
        Node *head;  // 头节点指针
        int size;  // 链表大小
    } LinkedList;

    LinkedList *ll_new();
    void ll_insert(LinkedList *list, EdgeRec *edge, int (*comp)(const void*, const void*));
    void ll_delete(LinkedList *list, void (*freeData)(const void *));
    EdgeRec* ll_pop(LinkedList *list);
    EdgeRec* ll_head(LinkedList *list);
    EdgeRec* ll_next(LinkedList *list);
    int ll_empty(LinkedList *list);
    void ll_print(LinkedList *list, FILE *fp);
#endif //DATASTRUCTURE_H

#ifdef __cplusplus
}
#endif
