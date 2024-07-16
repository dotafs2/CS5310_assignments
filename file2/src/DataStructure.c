//
// Created by DOTAFS on 2024/7/12.
//

#include "DataStructure.h"

LinkedList* ll_new() {
    LinkedList *list = (LinkedList *)malloc(sizeof(LinkedList));
    list->head = NULL;
    list->size = 0;
    return list;
}

void ll_insert(LinkedList *list, EdgeRec *edge, int (*comp)(const void*, const void*)) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->data = edge;
    newNode->next = NULL;

    if (list->head == NULL || comp(edge, list->head->data) < 0) {
        newNode->next = list->head;
        list->head = newNode;
    } else {
        Node *current = list->head;
        while (current->next != NULL && comp(edge, current->next->data) >= 0) {
            current = current->next;
        }
        newNode->next = current->next;
        current->next = newNode;
    }

    list->size++;
}

void ll_delete(LinkedList *list, void (*freeData)(const void *)) {
    Node *current = list->head;
    Node *nextNode;

    while (current) {
        nextNode = current->next;
        if (freeData) {
            freeData(current->data);
        }
        free(current);
        current = nextNode;
    }

    free(list);
}


EdgeRec* ll_pop(LinkedList *list) {
    if (list->head == NULL) {
        return NULL;
    }

    Node *temp = list->head;
    EdgeRec *edge = temp->data;

    list->head = list->head->next;
    free(temp);
    list->size--;

    return edge;
}


EdgeRec* ll_head(LinkedList *list) {
    return list->head ? list->head->data : NULL;
}


EdgeRec* ll_next(LinkedList *list) {
    static Node *current = NULL;

    if (!current) {
        current = list->head;
    } else {
        current = current->next;
    }

    return current ? current->data : NULL;
}


int ll_empty(LinkedList *list) {
    return list->size == 0;
}
void ll_print(LinkedList *list, FILE *fp) {
    if (list == NULL || fp == NULL) {
        return;
    }

    Node *current = list->head;
    fprintf(fp, "LinkedList with %d edges:\n", list->size);

    while (current != NULL) {
        EdgeRec *edge = (EdgeRec *)current->data;
        fprintf(fp, "Edge from (%f, %f) to (%f, %f):\n", edge->x0, edge->y0, edge->x1, edge->y1);
        fprintf(fp, "  yStart: %d, yEnd: %d\n", edge->yStart, edge->yEnd);
        fprintf(fp, "  xIntersect: %f, dxPerScan: %f\n", edge->xIntersect, edge->dxPerScan);
        fprintf(fp, "  zIntersect: %f, dzPerScan: %f\n", edge->zIntersect, edge->dzPerScan);
        current = current->next;
    }
}