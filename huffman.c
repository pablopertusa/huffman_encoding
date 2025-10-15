#include <stdio.h>
#include <stdlib.h>
#include "huffman.h"

bool is_less(HuffmanTree *t1, HuffmanTree *t2) {
    if (t1 == NULL || t2 == NULL) {
        printf("WARNING: una comparación ha recibido un tree a NULL\n");
        return false;
    }
    else if (t1->weight < t2->weight) {
        return true;
    }
    return false;
}

HuffmanTree *merge_trees(HuffmanTree *t1, HuffmanTree *t2) {
    if (t1 == NULL || t2 == NULL) {
        printf("WARNING: una comparación ha recibido un tree a NULL\n");
        return NULL;
    }
    HuffmanTree *new_tree = (HuffmanTree *)malloc(sizeof(HuffmanTree));

    if (new_tree == NULL) {
        printf("la asignación de memoria para un tree ha fallado\n");
        return NULL;
    }

    new_tree->is_leaf = false;
    new_tree->weight = t1->weight + t2->weight;
    new_tree->left = t1;
    new_tree->right = t2;

    return new_tree;
}

HuffmanTree *create_leaf(int weight) {
    HuffmanTree *new_leaf = (HuffmanTree *)malloc(sizeof(HuffmanTree));
    if (new_leaf == NULL) {
        printf("error allocating memory to leaf\n");
        return NULL;
    }
    new_leaf->weight = weight;
    new_leaf->left = NULL;
    new_leaf->right = NULL;
    new_leaf->is_leaf = true;
    return new_leaf;
}

void recursive_codes(Code **array, HuffmanTree *root) {
}

Code **create_codes(HuffmanTree *tree, size_t n) {
    if (n == 0 || tree == NULL) {
        return NULL;
    }

    Code **codes_array = (Code **)malloc(sizeof(Code *) * n);
    if (codes_array == NULL) {
        printf("error allocating memory while creating the codes\n");
    }



    return codes_array;
}