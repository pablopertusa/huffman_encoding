#include <stdio.h>
#include "minheap.h"
#include <stdlib.h>

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

void heapify_down(MinHeap *h, int i) {
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
        heapify_down(h, smallest);
    }
}

void heapify_up(MinHeap *h, int i) {
    
    while (i > 0 && is_less(h->arr[i], h->arr[parent(i)])) {
        swap(h->arr[i], h->arr[parent(i)]);
        
        i = parent(i);
    }
}

void insert_key(MinHeap *h, HuffmanTree *key) {
    if (h->size == MAX_SIZE) {
        fprintf(stderr, "Error: Heap lleno\n");
        return;
    }

    h->size++;
    int i = h->size - 1;
    h->arr[i] = key;

    heapify_up(h, i);
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

    heapify_down(h, 0);

    return root;
}

HuffmanTree *get_min(MinHeap *h) {
    if (h->size <= 0) {
        return NULL;
    }
    return h->arr[0];
}