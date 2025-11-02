#include <stdio.h>
#include <stdlib.h>
#include "huffman.h"

bool is_less(HuffmanTree *t1, HuffmanTree *t2) {
    if (t1 == NULL || t2 == NULL) {
        printf("WARNING: una comparación ha recibido un tree a NULL\n");
        return false;
    }
    return t1->weight < t2->weight;
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

HuffmanTree *create_leaf(int weight, unsigned char character) {
    HuffmanTree *new_leaf = (HuffmanTree *)malloc(sizeof(HuffmanTree));
    if (new_leaf == NULL) {
        printf("error allocating memory to leaf\n");
        return NULL;
    }
    new_leaf->weight = weight;
    new_leaf->left = NULL;
    new_leaf->right = NULL;
    new_leaf->is_leaf = true;
    new_leaf->character = character;
    return new_leaf;
}

void recursive_codes(Code **array, HuffmanTree *node, uint32_t acum_code, int length) {
    if (node == NULL || array == NULL) {
        printf("recursive codes received NULL");
        return;
    }
    HuffmanTree *r,*l;
    // si es una hoja guardamos el código que llevamos hasta ahora
    if (node->is_leaf) {
        array[node->character]->bits = acum_code;
        array[node->character]->length = length;
        return;
    }
    // se añade un cero al final
    if ((l = node->left) != NULL) {
        recursive_codes(array, node->left, acum_code << 1, length + 1);
    }

    // se añade un uno al final
    if ((r = node->right) != NULL) {
        recursive_codes(array, node->right, acum_code << 1 | 1, length + 1);
    }
}

Code **create_codes(HuffmanTree *tree, int n) {
    if (n == 0 || tree == NULL) {
        return NULL;
    }

    // n va a ser el tamaño del encoding, en este caso como es ASCII el índice de cada carácter será su valor
    Code **codes_array = (Code **)malloc(sizeof(Code *) * n);
    if (codes_array == NULL) {
        printf("error allocating memory while creating the codes\n");
    }
    for (int i = 0; i < n; i++) {
        Code *new_code = (Code *)malloc(sizeof(Code));
        if (new_code == NULL) {
            printf("error allocating memory while creating a code\n");
            return NULL;
        }
        new_code->bits = 0;
        new_code->length = 0;
        codes_array[i] = new_code;
    }

    //crea los códigos y los almacena en el array
    uint32_t code = 0;
    int length = 0;
    recursive_codes(codes_array, tree, code, length);

    return codes_array;
}

char *string_node(HuffmanTree *node) {
    if (node == NULL) {
        return "#;";
    }
    int weight = node->weight;
    unsigned char character = node->character;

    size_t buffer_size = 128;
    char *buffer = (char *)malloc(buffer_size*sizeof(char));

    snprintf(buffer, buffer_size, "%c,%d;", character, weight);

    return buffer;
}

char *recursive_traverse(HuffmanTree *node) {
    return NULL;
}


char *traverse_tree(HuffmanTree *tree) {
    size_t memory_size = 4096;
    char *m = (char *)malloc(memory_size * sizeof(char));
    return NULL;
}