#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stdbool.h>

typedef struct MinHeap MinHeap; 

typedef struct HuffmanTree {
    int weight;
    bool is_leaf;
    struct HuffmanTree *left;
    struct HuffmanTree *right;
} HuffmanTree;

bool is_less(HuffmanTree *t1, HuffmanTree *t2);
HuffmanTree *merge_trees(HuffmanTree *t1, HuffmanTree *t2);
HuffmanTree *create_leaf(int weight);
HuffmanTree *create_huffman_tree(MinHeap *h);

#endif