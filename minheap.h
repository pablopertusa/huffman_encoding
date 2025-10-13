#ifndef MINHEAP_H
#define MINHEAP_H

#include "huffman.h"

#define MAX_SIZE 128

typedef struct MinHeap {
    HuffmanTree* arr[MAX_SIZE];
    int size;
} MinHeap;


MinHeap *create_minheap();
void heapify_down(MinHeap *h, int i);
void heapify_up(MinHeap *h, int i);
void insert_key(MinHeap *h, HuffmanTree *key);
HuffmanTree *extract_min(MinHeap *h);
HuffmanTree *get_min(MinHeap *h);
int parent(int i);
int left_child(int i);
int right_child(int i);
void swap(HuffmanTree *a, HuffmanTree *b);
#endif