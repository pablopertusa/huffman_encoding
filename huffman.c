#include <stdio.h>
#include <stdlib.h>
#include "huffman.h"
#include <string.h>
#include "minheap.h"
#include "utils.h"

bool is_less(HuffmanTree *t1, HuffmanTree *t2) {
    if (t1 == NULL || t2 == NULL) {
        printf("WARNING: una comparación ha recibido un tree a NULL\n");
        return false;
    }
    else if (t1->weight == t2->weight) {
        return t1->character < t2->character;
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
    // ponemos un char que es un placeholder, esto es para que la creación del árbol sea única
    new_tree->character = 'm';

    return new_tree;
}

bool equal_trees(HuffmanTree *t1, HuffmanTree *t2) {
    if (t1 == NULL && t2 == NULL) {
        return true;
    }
    else if (t1 == NULL || t2 == NULL) {
        printf("uno es nulo y el otro no\n");
        return false;
    }
    bool is_leaf_1 = t1->is_leaf;
    bool is_leaf_2 = t2->is_leaf;
    if (is_leaf_1 && is_leaf_2) {
        if (t1->character == t2->character && t1->weight == t2->weight) {
            return true && equal_trees(t1->left, t2->left) && equal_trees(t1->right, t2->right);
        }
    }
    else if (!is_leaf_1 && !is_leaf_2) {
        return true && equal_trees(t1->left, t2->left) && equal_trees(t1->right, t2->right);
    }
    printf("w: %d, char: %c, is_leaf: %d\n", t1->weight, t1->character, t1->is_leaf);
    printf("w: %d, char: %c, is_leaf: %d\n", t2->weight, t2->character, t2->is_leaf);
    return false;
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

void free_tree(HuffmanTree *node) {
    if (node == NULL) {
        return;
    }
    free_tree(node->left);
    free_tree(node->right);

    free(node);
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

void free_codes(Code **codes_array, int n) {
    if (codes_array == NULL || n <= 0) {
        return;
    }

    for (int i = 0; i < n; i++) {
        if (codes_array[i] != NULL) {
            free(codes_array[i]);
            codes_array[i] = NULL;
        }
    }
    free(codes_array);
}

char *string_node(HuffmanTree *node) {
    if (node == NULL) {
        return NULL;
    }
    int weight = node->weight;
    unsigned char character = node->character;

    size_t buffer_size = 128;
    char *buffer = (char *)malloc(buffer_size*sizeof(char));
    if (buffer == NULL) {
        fprintf(stderr, "ERROR allocating memory while string_node\n");
        return NULL;
    }

    char record_sep = 30;
    char group_sep = 29;
    snprintf(buffer, buffer_size, "%c%c%d%c", character, record_sep, weight, group_sep);

    return buffer;
}

void recursive_traverse(HuffmanTree *node, char *buffer) {
    if (node == NULL) {
        return;
    }
    else if (node->is_leaf) {
        char *token = string_node(node);
        if (token == NULL) {
            return;
        }
        // POR HACER
        // aquí falta manejar que el buffer se llene y haya que reasignar
        strcat(buffer, token);
        free(token);
    }
    else {
        recursive_traverse(node->left, buffer);
        recursive_traverse(node->right, buffer);
    }
}

// cualquier forma de viajar por el árbol vale porque luego voy a volver a construirlo, más ineficiente pero simple
char *traverse_tree(HuffmanTree *tree) {
    size_t memory_size = 1024 * 16;
    char *buffer = (char *)malloc(memory_size * sizeof(char));
    if (buffer == NULL) {
        return NULL;
    }
    recursive_traverse(tree, buffer);
    return buffer;
}

void write_header(char *header, long n_bits, FILE *out) {
    if (header == NULL) {
        printf("NULL header\n");
        return;
    }
    fputs(header, out);
    fputs("##", out);
    fprintf(out, "%ld//", n_bits);
}

int any_overflow(Code **codes, int encoding_length) {
    Code *code;
    for (int i = 0; i < encoding_length; i++) {
        code = codes[i];
        if (code->length > 32) {
            return 1;
        }
    }
    return 0;
}

long get_number_bits_to_write(Code **codes, FILE *stream) {
    if (codes == NULL || stream == NULL) {
        fprintf(stderr, "ERROR null pointer while calculating number of bits to write\n");
        return -1;
    }
    int c;
    long n_bits = 0;
    while ((c = getc(stream)) != EOF) {
        n_bits += codes[c]->length;
    }
    return n_bits;
}

HuffmanTree *create_tree_from_minheap(MinHeap *minheap) {
    if (minheap == NULL) {
        fprintf(stderr, "ERROR null minheap while creating tree\n");
        return NULL;
    }
    if (minheap->size == 0) {
        return NULL; 
    }
    HuffmanTree *aux1, *aux2, *merge, *final;
    int size;

    while ((size = minheap->size) > 1) {
        aux1 = extract_min(minheap);
        aux2 = extract_min(minheap);
        merge = merge_trees(aux1, aux2);
        if (merge == NULL) {
            fprintf(stderr, "ERROR null merge creating the tree\n");
            return NULL;
        }
        insert_key(minheap, merge);
    }
    final = extract_min(minheap);
    return final;
}

HuffmanTree *create_tree_from_header(char *header) {
    if (header == NULL) {
        fprintf(stderr, "ERROR null header while creating the tree\n");
        return NULL;
    }
    char *copy = strdup(header);
    if (copy == NULL) {
        fprintf(stderr, "ERROR allocating while duplicating string\n");
        return NULL;
    }

    // delimitador principal
    char *token = strtok(copy, "\x1D");

    MinHeap *minheap = create_minheap();
    HuffmanTree *temp, *result;

    if (minheap == NULL) {
        perror("While creating the tree from header");
        return NULL;
    }

    while (token != NULL) {
        int count;
        char character;

        if (sscanf(token, "%c\x1E%d", &character, &count) == 2) {
            temp = create_leaf(count, character);
            insert_key(minheap, temp);

        }
        else if (strcmp(token, "##") != 0) {
            fprintf(stderr, "ERROR format not matched while reading header, token: %s\n", token);
            return NULL;
        }
        token = strtok(NULL, "\x1D");
    }
    result = create_tree_from_minheap(minheap);

    free(copy);
    free(minheap);
    return result;
}

char *decode_file(HuffmanTree *tree, char *filename) {
    FILE *stream = fopen(filename, "rb");
    if (tree == NULL || stream == NULL) {
        fprintf(stderr, "ERROR null tree or stream while decoding\n");
        return NULL;
    }

    long number_bits = get_number_bits_from_header(filename);
    if (number_bits == -1) {
        fprintf(stderr, "ERROR number of bits not found in header\n");
        return NULL;
    }

    // leemos primero la parte del header
    int c;
    int last = 'i';
    while ((c = fgetc(stream)) != EOF) {
        if (last == '/' && c == '/') {
            break;
        }
        last = c;
    }

    unsigned char buffer[READ_BUFFER_SIZE];
    size_t n_bytes;
    unsigned char byte, bit;
    HuffmanTree *node = tree;
    DecodingBuffer *decoding_buffer = create_decoding_buffer();
    if (decoding_buffer == NULL) {
        perror("error creating decoding buffer");
        fclose(stream);
        return NULL;
    }
    while ((n_bytes = fread(buffer, sizeof(unsigned char), READ_BUFFER_SIZE, stream)) > 0) {
        for (int i = 0; i < n_bytes; i++) {
            byte = buffer[i];
            for (int b = 0; b < BYTE_SIZE; b++) {
                if (node == NULL) {
                    fprintf(stderr, "ERROR null node while decoding, unexpected behaviour\n");
                    free_decoding_buffer(decoding_buffer);
                    fclose(stream);
                    return NULL;
                }
                else if (node->is_leaf) {
                    append_decoding_buffer(decoding_buffer, node->character);
                    node = tree;
                }
                bit = get_next_bit(byte, b);
                if (bit == 0) {
                    node = node->left;
                }
                else if (bit == 1) {
                    node = node->right;
                }
            }
        }
    }

    if (ferror(stream)) {
        perror("error reading decoding file");
        fclose(stream);
        return NULL;
    }

    fclose(stream);
    return decoding_buffer->data;
}