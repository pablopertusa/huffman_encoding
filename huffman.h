#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

struct MinHeap;

typedef struct Code {
    unsigned char character;
    int length;
    uint32_t bits;
} Code;

typedef struct HuffmanTree {
    int weight;
    bool is_leaf;
    unsigned char character;
    struct HuffmanTree *left;
    struct HuffmanTree *right;
} HuffmanTree;


bool is_less(HuffmanTree *t1, HuffmanTree *t2);
HuffmanTree *merge_trees(HuffmanTree *t1, HuffmanTree *t2);
HuffmanTree *create_leaf(int weight, unsigned char character);
void free_tree(HuffmanTree *tree);
Code **create_codes(HuffmanTree *tree, int n);
void free_codes(Code **codes, int n);
void recursive_codes(Code **array, HuffmanTree *root, uint32_t acum_code, int length);
char *traverse_tree(HuffmanTree *tree);
char *string_node(HuffmanTree *node);
void recursive_traverse(HuffmanTree *node, char *buffer);
char *traverse_tree(HuffmanTree *tree);
void write_header(char *header, FILE *out);
int any_overflow(Code **codes, int encoding_length);
HuffmanTree *create_tree_from_minheap(struct MinHeap *minheap);
HuffmanTree *create_tree_from_header(char *string);
bool equal_trees(HuffmanTree *t1, HuffmanTree *t2);

#endif