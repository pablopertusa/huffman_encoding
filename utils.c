#include <stdbool.h>
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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
    uint64_t bits_to_add = code & ((1U << n_bits) - 1);

    if ((64 - buffer->used) < n_bits) {
        fprintf(stderr, "ERROR too many n_bits appending to buffer\n");
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
        
        unsigned char byte_to_write = (unsigned char)((buffer->data >> shift) & 0xFFU);
        
        if (fputc(byte_to_write, output) == EOF) {
            fprintf(stderr, "ERROR writing bytes to file\n");
            return;
        }
        
        buffer->used -= BYTE_SIZE;
    }
}

char *read_header(FILE *input) {
    if (input == NULL) {
        fprintf(stderr, "ERROR null input to read the header");
        return NULL;
    }
    int initial_size = 128;
    char *buffer = (char *)malloc(initial_size * sizeof(char));
    if (buffer == NULL) {
        perror("ERROR allocating while reading header");
        return NULL;
    }
    int c, buffer_size, buffer_used;
    buffer_size = initial_size;
    buffer_used = 0;
    char *temp;
    char last = '\0';
    char current;

    while ((c = getc(input)) != EOF) {
        if (buffer_used >= buffer_size - 1) {
            buffer_size *= 2;
            temp = (char *)realloc(buffer, buffer_size * sizeof(char));
            if (temp == NULL) {
                perror("ERROR allocating temporal memory while reading header");
                return NULL;
            }
            buffer = temp;
        }
        current = (char)c;
        buffer[buffer_used++] = current;
        if (last == '#' && current == '#') {
            break;
        }
        last = current;
    }
    buffer[buffer_used] = '\0';
    return buffer;
}

long get_number_bits_from_header(char *filename) {
    FILE *stream = fopen(filename, "r");
    if (stream == NULL) {
        perror("while opening file to extract number of bits");
        return -1;
    }

    int c;
    int last = 'i';
    while ((c = fgetc(stream)) != EOF) {
        if (last == '#' && c == '#') {
            break;
        }
        last = c;
    }
    if (c == EOF) {
        fprintf(stderr, "ERROR delimiter not found in file\n");
        fclose(stream);
        return -1;
    }
    long number;
    if (fscanf(stream, "%ld", &number) != 1) {
        fprintf(stderr, "ERROR number of bits not found in file\n");
        fclose(stream);
        return -1;
    }

    fclose(stream);
    return number;
}

DecodingBuffer *create_decoding_buffer() {
    DecodingBuffer *buffer = (DecodingBuffer *)malloc(sizeof(DecodingBuffer));
    if (buffer == NULL) {
        perror("while creating decoding buffer");
        return NULL;
    }
    char *data = (char *)calloc(DECODING_BUFFER_INITAL_LENGTH, sizeof(char));
    if (data == NULL) {
        free(buffer);
        perror("while creating decoding buffer");
        return NULL;
    }
    buffer->data = data;
    buffer->size = DECODING_BUFFER_INITAL_LENGTH;
    buffer->used = 0;
    return buffer;
}

void free_decoding_buffer(DecodingBuffer *buffer) {
    if (buffer == NULL) {
        fprintf(stderr, "ERROR trying to free a null buffer\n");
        return;
    }
    if (buffer->data != NULL) {
        free(buffer->data);
    }
    free(buffer);
}

void append_decoding_buffer(DecodingBuffer *buffer, char character) {
    if (buffer == NULL) {
        fprintf(stderr, "ERROR trying to append to a null buffer\n");
        return;
    }
    if (buffer->data == NULL) {
        fprintf(stderr, "ERROR trying to append to a null data in buffer\n");
        return;
    }
    if (buffer->used == buffer->size - 1) {
        size_t new_size = buffer->size * DECODING_BUFFER_GROWTH_FACTOR;
        
        char *new_data = (char *)realloc(buffer->data, new_size * sizeof(char));
        if (new_data == NULL) {
            perror("Error during realloc of decoding buffer");
            return;
        }
        buffer->data = new_data;
        buffer->size = new_size;
    }
    buffer->data[buffer->used] = character;
    buffer->used++;
}

void terminate_decoding_buffer(DecodingBuffer *buffer) {
    if (buffer == NULL) {
        fprintf(stderr, "ERROR trying to add terminate char to a null buffer\n");
        return;
    }
    if (buffer->data == NULL) {
        fprintf(stderr, "ERROR trying to add terminate char to a null data in buffer\n");
        return;
    }
    buffer->data[buffer->used] = '\0';
}

unsigned char get_next_bit(unsigned char byte, int bits_read) {
    if (bits_read >= 8 || bits_read < 0) {
        fprintf(stderr, "ERROR invalid number of bits read\n");
        return 2;
    }
    int shift = BYTE_SIZE - bits_read - 1;
    unsigned char bit = (unsigned char)(byte >> shift & 1);
    return bit;
}

char *string_counter(long *counter, int encoding_length) {

    size_t max_size = 1048576;
    char record_sep = 30;
    char group_sep = 29;

    char *result_string = (char *)malloc(max_size);
    if (result_string == NULL) {
        return NULL;
    }

    size_t current_len = 0;

    for (int i = 0; i < encoding_length; i++) {
        long weight = counter[i];
        
        if (weight > 0) {
            char character = (char)i;
            char temp_buffer[20];

            int written = snprintf(
                temp_buffer, 
                sizeof(temp_buffer), 
                "%c%c%ld%c", 
                character, 
                record_sep, 
                weight, 
                group_sep
            );
            
            if (current_len + written < max_size) {
                strcat(result_string, temp_buffer);
                current_len += written;
            } else {
                fprintf(stderr, "Advertencia: String de conteo ha excedido el tamaño máximo.\n");
                return NULL;
            }
        }
    }
    
    char *final_string = (char *)realloc(result_string, current_len + 1);
    if (final_string != NULL) {
        return final_string;
    } else {
        return result_string;
    }
}