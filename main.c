#include <stdio.h>
#include "huffman.h"
#include "minheap.h"
#include "utils.h"

#define ENCODING_LENGTH 128 // el texto puede no ser solo ASCII pero vamos a suponer que lo es de momento

int main(int argc, char** argv) {

    bool debug = 0;

    if (argc < 2) {
        printf("file path must be provided\n");
        return 1;
    }
    char *output;
    if (argc == 3) {
        output = argv[2];
    }
    else {
        output = "out";
    }
    char *filename = argv[1];
    char mode = 'r';
    FILE *file = fopen(filename, &mode);
    if (file == NULL) {
        printf("error reading file\n");
        return 1;
    }

    // contamos las frecuencias
    long counter[ENCODING_LENGTH] = {0};
    int c;
    while ((c = fgetc(file)) != EOF) {
        if (c > 127) {
            printf("ERROR: leyendo un carácter que no es ASCII\n");
            fclose(file);
            return 1;
        }
        counter[c]++;
    }
    fclose(file);

    // ahora creamos una hoja para cada uno de los carácteres y el minheap para llevar la prioridad
    HuffmanTree *temp;
    MinHeap *minheap;
    minheap = create_minheap();
    if (minheap == NULL) {
        return 1;
    }
    for (unsigned char c = 0; c < ENCODING_LENGTH; c++) {
        if (counter[c] > 0) {
            temp = create_leaf(counter[c], c);
            insert_key(minheap,  temp);
        }
        else if (debug){
            printf("INFO: saltando caracter %d\n", c);
        }
    }

    // creamos el arbol de huffman
    HuffmanTree *aux1, *aux2, *merge, *final;

    int size;
    while ((size = minheap->size) > 1) {
        aux1 = extract_min(minheap);
        aux2 = extract_min(minheap);
        merge = merge_trees(aux1, aux2);
        insert_key(minheap, merge);
    }
    final = extract_min(minheap);
    Code **codes = create_codes(final, ENCODING_LENGTH);
    if (codes == NULL) {
        printf("NULL codes\n");
        return 1;
    }
    // vamos a ver si ha habido overflow en los códigos
    int check = any_overflow(codes, ENCODING_LENGTH);
    if (check == 1) {
        printf("overflow generating codes\n");
        return 1;
    }


    FILE *out_file = fopen(output, "w");
    if (out_file == NULL) {
        printf("NULL output file\n");
        return 1;
    }
    char *header = traverse_tree(final);
    write_header(header, out_file);
    fclose(out_file);

    EncodingBuffer *encoding_buffer = create_buffer();
    if (encoding_buffer == NULL) {
        return 1;
    }
    int read_c;
    Code *code;
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
    while ((read_c = getc(read_file)) != EOF) {
        code = codes[read_c];
        if (code->length > (64 - encoding_buffer->used)) {
            write_buffer(encoding_buffer, write_file);
        }
        append_buffer(encoding_buffer, code->bits, code->length);
    }
    fclose(read_file);
    fclose(write_file);

    return 0;
}