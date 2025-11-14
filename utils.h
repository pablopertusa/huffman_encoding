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

typedef struct DecodingBuffer {
    char *data;
    size_t size;
    size_t used;
} DecodingBuffer;

void write_buffer(EncodingBuffer *buffer, FILE *output);
void append_buffer(EncodingBuffer *buffer, uint32_t code, int n_bits);
EncodingBuffer *create_buffer();
char *read_header(FILE *input);

void append_decoding_buffer(DecodingBuffer *buffer, char character);
DecodingBuffer *create_decoding_buffer();
void free_decoding_buffer(DecodingBuffer *buffer);

#endif