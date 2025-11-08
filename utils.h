#ifndef UTILS_H
#define UTILS_H

#define BYTE_SIZE 8
#define ENCODING_BUFFER_LENGTH 64

#include <stdint.h>
#include <stdio.h>

typedef struct {
    uint64_t data;
    int used;
} EncodingBuffer;

void write_buffer(EncodingBuffer *buffer, FILE *output);
void append_buffer(EncodingBuffer *buffer, uint32_t code, int n_bits);
EncodingBuffer *create_buffer();
char *read_header(FILE *input);

#endif