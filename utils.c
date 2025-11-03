#include "utils.h"
#include <stdio.h>
#include <stdlib.h>


EncodingBuffer *create_buffer() {
    EncodingBuffer *buffer = (EncodingBuffer *)malloc(sizeof(EncodingBuffer));
    if (buffer == NULL) {
        printf("ERROR allocating memory to encoding buffer\n");
        return NULL;
    }
    buffer->data = 0;
    buffer->used = 0;
    return buffer;
}

void append_buffer(EncodingBuffer *buffer, uint32_t code, int n_bits) {
    if (n_bits < 0 || n_bits > 32) {
        printf("ERROR invalid n_bits appending to buffer\n");
        return;
    }
    uint32_t bits_to_add = code & ((1U << n_bits) - 1);

    if ((64 - buffer->used) < n_bits) {
        printf("ERROR too many n_bits appending to buffer\n");
        return;
    }

    buffer->data = buffer->data << n_bits;

    buffer->data = buffer->data | bits_to_add;
    buffer->used += n_bits;
}

void write_buffer(EncodingBuffer *buffer, FILE *output) {
    int bytes_to_write = buffer->used / BYTE_SIZE;
    
    for (int i = 0; i < bytes_to_write; i++) {
        
        int shift = buffer->used - BYTE_SIZE;
        
        unsigned char byte_to_write = (unsigned char)((buffer->data >> shift) & 0xFFULL);
        
        if (fputc(byte_to_write, output) == EOF) {
            printf("ERROR writing bytes to file\n");
            return;
        }
        
        buffer->used -= BYTE_SIZE;
    }
    
    if (buffer->used > 0) {
        int bits_written = bytes_to_write * BYTE_SIZE;
        buffer->data >>= bits_written; 
    } else {
        buffer->data = 0;
    }
}