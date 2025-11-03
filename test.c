#include "huffman.h"
#include "minheap.h"
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

void print_bitstream(Code *code_ptr) {
    if (code_ptr == NULL) {
        fprintf(stderr, "Error: El puntero a Code es NULL.\n");
        return;
    }

    int len = code_ptr->length;
    uint32_t encoding = code_ptr->bits;

    if (len <= 0 || len > 32) {
        fprintf(stderr, "Error: Longitud de encoding (%d) inválida o fuera de rango (1-32).\n", len);
        return;
    }

    printf("Bits: ");
    for (int i = len - 1; i >= 0; i--) {
        int bit = (encoding >> i) & 1;
        printf("%d", bit);
    }
    printf("\n");
}

void print_file_in_binary(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        fprintf(stderr, "Error: No se pudo abrir el archivo de salida '%s' para la lectura binaria.\n", filename);
        return;
    }

    int byte;
    long total_bytes = 0;

    printf("\n============================================\n");
    printf("CONTENIDO BINARIO DEL ARCHIVO '%s'\n", filename);
    printf("============================================\n");
    
    while ((byte = fgetc(file)) != EOF) {
        printf("Byte %03ld: ", total_bytes);
        
        for (int i = 7; i >= 0; i--) {
            int bit = (byte >> i) & 1;
            printf("%d", bit);
        }
        
        printf("\n");
        total_bytes++;
    }

    printf("--------------------------------------------\n");
    printf("Lectura binaria finalizada. Total de bytes leídos: %ld\n", total_bytes);
    printf("============================================\n\n");

    fclose(file);
}

int main() {
    
    HuffmanTree *node1 = create_leaf(32, 'C');
    HuffmanTree *node2 = create_leaf(42, 'D');
    HuffmanTree *node3 = create_leaf(120, 'E');
    HuffmanTree *node4 = create_leaf(7, 'K');
    HuffmanTree *node5 = create_leaf(42, 'L');
    HuffmanTree *node6 = create_leaf(24, 'M');
    HuffmanTree *node7 = create_leaf(37, 'U');
    HuffmanTree *node8 = create_leaf(2, 'Z');

    MinHeap *minheap = create_minheap();

    insert_key(minheap, node1);
    insert_key(minheap, node2);
    insert_key(minheap, node3);
    insert_key(minheap, node4);
    insert_key(minheap, node5);
    insert_key(minheap, node6);
    insert_key(minheap, node7);
    insert_key(minheap, node8);


    int size;
    HuffmanTree *aux1, *aux2, *merge, *final;
    while ((size = minheap->size) > 1) {
        aux1 = extract_min(minheap);
        aux2 = extract_min(minheap);
        merge = merge_trees(aux1, aux2);
        insert_key(minheap, merge);
    }
    final = extract_min(minheap);
    
    Code **codes_array = create_codes(final, 128);
    unsigned char c = 'C';
    printf("%c\n", c);
    print_bitstream(codes_array[c]);
    printf("--------------\n");
    c = 'D';
    printf("%c\n", c);
    print_bitstream(codes_array[c]);
    printf("--------------\n");
    c = 'E';
    printf("%c\n", c);
    print_bitstream(codes_array[c]);
    printf("--------------\n");
    c = 'K';
    printf("%c\n", c);
    print_bitstream(codes_array[c]);
    printf("--------------\n");
    c = 'L';
    printf("%c\n", c);
    print_bitstream(codes_array[c]);
    printf("--------------\n");
    c = 'M';
    printf("%c\n", c);
    print_bitstream(codes_array[c]);
    printf("--------------\n");
    c = 'U';
    printf("%c\n", c);
    print_bitstream(codes_array[c]);
    printf("--------------\n");
    c = 'Z';
    printf("%c\n", c);
    print_bitstream(codes_array[c]);
    printf("--------------\n");

    char *filename = "test.txt";
    char *output = "test_out";

    FILE *read_file = fopen(filename, "r");
    if (read_file == NULL) {
        printf("NULL input file\n");
        return 1;
    }
    FILE *write_file = fopen(output, "wb");
    if (write_file == NULL) {
        printf("NULL output file\n");
        return 1;
    }
    int read_c;
    Code *code;
    EncodingBuffer *encoding_buffer = create_buffer();
    if (encoding_buffer == NULL) {
        return 1;
    }
    while ((read_c = getc(read_file)) != EOF) {
        code = codes_array[read_c];
        if (code != NULL && code->length > (64 - encoding_buffer->used)) {
             write_buffer(encoding_buffer, write_file); 
        }
        if (code != NULL) {
            append_buffer(encoding_buffer, code->bits, code->length);
        }
    }
    
    fclose(read_file);
    fclose(write_file);
    
    print_file_in_binary(output);

    return 0;
}