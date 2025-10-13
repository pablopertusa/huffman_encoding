#include <stdio.h>
#include "huffman.h"
#include "minheap.h"

#define ENCODING_LENGTH 128 // el texto puede no ser solo ASCII pero vamos a suponer que lo es de momento

int main(int argc, char** argv) {

    if (argc < 2) {
        printf("file path must be provided\n");
        return 1;
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
            printf("ERROR: leyendo un carácter que no solo es ASCII\n");
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
    for (int i = 0; i < ENCODING_LENGTH; i++) {
        temp = create_leaf(counter[i]);
        insert_key(minheap,  temp);
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
    printf("%d\n", final->weight); // es correcto

    return 0;
}