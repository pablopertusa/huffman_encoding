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

void min_heapify(MinHeap *h, int i) {
    int l = left_child(i);
    int r = right_child(i);
    int smallest = i;

    // 1. Encontrar el más pequeño entre el nodo actual y sus hijos
    if (l < h->size && is_less(h->arr[l], h->arr[smallest])) {
        smallest = l;
    }
    if (r < h->size && is_less(h->arr[r], h->arr[smallest])) {
        smallest = r;
    }

    // 2. Si el más pequeño no es el nodo actual, intercambiar y recursivamente aplicar
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

    // insertamos la nueva clave al final del array
    h->size++;
    int i = h->size - 1;
    h->arr[i] = key;

    // volvemos a restaurar la propiedad de minheap
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

    // 1. Almacenar la raíz (el mínimo)
    HuffmanTree *root = h->arr[0];

    // 2. Mover el último elemento a la raíz
    h->arr[0] = h->arr[h->size - 1];
    h->size--;

    // 3. Restaurar la propiedad de MinHeap desde la raíz
    min_heapify(h, 0);

    return root;
}

HuffmanTree *get_min(MinHeap *h) {
    if (h->size <= 0) {
        return NULL;
    }
    return h->arr[0];
}