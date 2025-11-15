#ifndef UTILS_H
#define UTILS_H

#define BYTE_SIZE 8
#define ENCODING_BUFFER_LENGTH 64
#define DECODING_BUFFER_INITAL_LENGTH 1024
#define DECODING_BUFFER_GROWTH_FACTOR 2

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
long get_number_bits_from_header(char *filename);
void terminate_decoding_buffer(DecodingBuffer *buffer);
unsigned char get_next_bit(unsigned char byte, int bits_read);

#endif