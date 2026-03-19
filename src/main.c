#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 8192

static inline double log16(double x) {
    static const double LOG16 = 2.772588722239781;
    return log(x) / LOG16;
}

static inline size_t countDigits(size_t n) {
    if (n == 0) return 1;
    return (size_t)floor(log16((double)n)) + 1;
}

int main(int argc, char *argv[]) {
    int canonical = 0;
    char *filename = NULL;
    if (argc != 2 && argc != 3) {
        printf("Bad argument. Example: %s [-C] <filepath>\n", argv[0]);
        return EXIT_FAILURE;
    }

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-C") == 0) {
            canonical = 1;
        } else if (argv[i][0] == '-') {
            printf("Unknown option: %s\n", argv[i]);
            return EXIT_FAILURE;
        } else {
            filename = argv[i];
        }
    }

    if (filename == NULL) {
        printf("No file specified.\n");
        return EXIT_FAILURE;
    }

    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Error: could not open file '%s'.\n", filename);
        return EXIT_FAILURE;
    }

    unsigned char buffer[BUFFER_SIZE] = {0};

    if (fseek(file, 0, SEEK_END) != 0) {
        fclose(file);
        return EXIT_FAILURE;
    }

    long end = ftell(file);
    if (end < 0) {
        fclose(file);
        return EXIT_FAILURE;
    }

    size_t size = (size_t)end;

    if (fseek(file, 0, SEEK_SET) != 0) {
        fclose(file);
        return EXIT_FAILURE;
    }

    if (size == 0) {
        fclose(file);
        return EXIT_SUCCESS;
    }

    size_t offset = 0;
    size_t chunk_bytes_read = 0;

    while ((chunk_bytes_read = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
        for (size_t buffer_idx = 0; buffer_idx < chunk_bytes_read; buffer_idx += 16) {
            size_t bytes_in_row = 16;
            if (buffer_idx + 16 > chunk_bytes_read) {
                bytes_in_row = chunk_bytes_read - buffer_idx;
            }

            unsigned char *row = &buffer[buffer_idx];

            printf("%0*zx ", (int)countDigits(size), offset);

            if (canonical > 0) {
                for (size_t i = 0; i < 16; i++) {
                    if (i < bytes_in_row) {
                        printf("%02x ", row[i]);
                    } else {
                        printf("   ");
                    }

                    if (i == 7) putchar(' ');
                }

                printf("|");
                for (size_t i = 0; i < bytes_in_row; i++) {
                    putchar(isprint(row[i]) ? row[i] : '.');
                }
                for (size_t i = bytes_in_row; i < 16; i++) {
                    putchar(' ');
                }
                printf("|");
            } else {
                for (size_t i = 0; i < 16; i += 2) {
                    if (i < bytes_in_row) {
                        unsigned int word = row[i];
                        if (i + 1 < bytes_in_row) {
                            word |= ((unsigned int)row[i + 1]) << 8;
                        }
                        printf("%04x ", word);
                    } else {
                        printf("     ");
                    }
                }
            }

            printf("\n");
            offset += bytes_in_row;
        }
    }

    fclose(file);
    return EXIT_SUCCESS;
}
