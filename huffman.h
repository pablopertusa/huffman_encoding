#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stdbool.h>
#include <stdio.h>

typedef struct HuffmanTree {
    int weight;
    bool is_leaf;
    struct HuffmanTree *left;
    struct HuffmanTree *right;
    char *code;
} HuffmanTree;

typedef struct Code {
    int lenght;
    unsigned int bits;
} Code;

bool is_less(HuffmanTree *t1, HuffmanTree *t2);
HuffmanTree *merge_trees(HuffmanTree *t1, HuffmanTree *t2);
HuffmanTree *create_leaf(int weight);
Code **create_codes(HuffmanTree *tree, size_t n);
void recursive_codes(Code **array, HuffmanTree *root);

#endif