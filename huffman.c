#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "huffman.h"
#include "minheap.h"

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

HuffmanTree *create_huffman_tree(MinHeap *h) {
    if (h == NULL) {
        printf("Minheap used is NULL\n");
        return NULL;
    }
    if (h->size == 0) {
        printf("Minheap is empty\n");
        return NULL;
    }
    HuffmanTree *t1;
    HuffmanTree *t2;
    HuffmanTree *new;
    while (h->size > 1) {
        t1 = extract_min(h);
        t2 = extract_min(h);
        new = merge_trees(t1, t2);
        insert_key(h, new);
    }
    return h->arr[0];

}