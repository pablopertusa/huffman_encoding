#include <stdio.h>
#include "minheap.h"
#include <stdlib.h>

// extract_min e insert_key testeados por separado queda testear a la vez 

int parent(int i) {
    return (i - 1) / 2;
}

int left_child(int i) {
    return 2 * i + 1;
}

int right_child(int i) {
    return 2 * i + 2;
}

void swap(HuffmanTree *a, HuffmanTree *b) {
    HuffmanTree temp = *a;
    *a = *b;
    *b = temp;
}

MinHeap *create_minheap() {
    MinHeap *minheap = (MinHeap *)malloc(sizeof(MinHeap));
    if (minheap == NULL) {
        return NULL;
    }
    minheap->size = 0;
    for (int i = 0; i < MAX_SIZE; i++) {
        minheap->arr[i] = NULL;
    }
    return minheap;
}

void min_heapify(MinHeap *h, int i) {
    int l = left_child(i);
    int r = right_child(i);
    int smallest = i;

    if (l < h->size && is_less(h->arr[l], h->arr[smallest])) {
        smallest = l;
    }
    if (r < h->size && is_less(h->arr[r], h->arr[smallest])) {
        smallest = r;
    }

    if (smallest != i) {
        swap(h->arr[i], h->arr[smallest]);
        min_heapify(h, smallest);
    }
}

void insert_key(MinHeap *h, HuffmanTree *key) {
    if (h->size == MAX_SIZE) {
        printf("Error: Heap lleno\n");
        return;
    }

    h->size++;
    int i = h->size - 1;
    h->arr[i] = key;

    min_heapify(h, 0);
}

HuffmanTree *extract_min(MinHeap *h) {
    if (h->size <= 0) {
        return NULL;
    }
    if (h->size == 1) {
        h->size--;
        return h->arr[0];
    }

    HuffmanTree *root = h->arr[0];

    h->arr[0] = h->arr[h->size - 1];
    h->size--;

    min_heapify(h, 0);

    return root;
}

HuffmanTree *get_min(MinHeap *h) {
    if (h->size <= 0) {
        return NULL;
    }
    return h->arr[0];
}