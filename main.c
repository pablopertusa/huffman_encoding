#include "stdio.h"
#define ENCODING_LENGTH 128 // el texto es solo ASCII

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

    long counter[ENCODING_LENGTH] = {0};
    int c;
    while ((c = fgetc(file)) != EOF) {
        counter[c]++;
    }

    fclose(file);
    for (int i = 0; i < ENCODING_LENGTH; i++) {
        printf("%c : %ld\n", i, counter[i]);
    }
    return 0;
}