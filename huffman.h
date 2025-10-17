#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

typedef struct Code {
    int length;
    uint32_t bits;
} Code;

typedef struct HuffmanTree {
    int weight;
    bool is_leaf;
    char character;
    struct HuffmanTree *left;
    struct HuffmanTree *right;
} HuffmanTree;


bool is_less(HuffmanTree *t1, HuffmanTree *t2);
HuffmanTree *merge_trees(HuffmanTree *t1, HuffmanTree *t2);
HuffmanTree *create_leaf(int weight, char character);
Code **create_codes(HuffmanTree *tree, char n);
void recursive_codes(Code **array, HuffmanTree *root, uint32_t acum_code, int length);

#endif