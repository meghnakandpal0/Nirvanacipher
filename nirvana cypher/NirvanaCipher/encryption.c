#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void encrypt(const char *filename, const char *key) {
    FILE *input_file = fopen(filename, "rb");
    if (!input_file) {
        perror("Error opening input file");
        exit(1);
    }

    char output_filename[256];
    snprintf(output_filename, sizeof(output_filename), "%s.encrypted", filename);
    FILE *output_file = fopen(output_filename, "wb");
    if (!output_file) {
        perror("Error creating output file");
        fclose(input_file);
        exit(1);
    }

    int key_len = strlen(key);
    int ch;
    size_t key_index = 0;

    while ((ch = fgetc(input_file)) != EOF) {
        char encrypted_ch = ch ^ key[key_index];
        fputc(encrypted_ch, output_file);

        key_index = (key_index + 1) % key_len;
    }

    fclose(input_file);
    fclose(output_file);

    // Remove the original file and rename encrypted file
    if (remove(filename) != 0) {
        perror("Error removing original file");
        exit(1);
    }

    if (rename(output_filename, filename) != 0) {
        perror("Error renaming encrypted file");
        exit(1);
    }
}

void decrypt(const char *filename, const char *key) {
    FILE *input_file = fopen(filename, "rb");
    if (!input_file) {
        perror("Error opening input file");
        exit(1);
    }

    char output_filename[256];
    snprintf(output_filename, sizeof(output_filename), "%s.decrypted", filename);
    FILE *output_file = fopen(output_filename, "wb");
    if (!output_file) {
        perror("Error creating output file");
        fclose(input_file);
        exit(1);
    }

    int key_len = strlen(key);
    int ch;
    size_t key_index = 0;

    while ((ch = fgetc(input_file)) != EOF) {
        char decrypted_ch = ch ^ key[key_index];
        fputc(decrypted_ch, output_file);

        key_index = (key_index + 1) % key_len;
    }

    fclose(input_file);
    fclose(output_file);

    // Remove the original file and rename decrypted file
    if (remove(filename) != 0) {
        perror("Error removing original file");
        exit(1);
    }

    if (rename(output_filename, filename) != 0) {
        perror("Error renaming decrypted file");
        exit(1);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <encrypt|decrypt> <filename> <key>\n", argv[0]);
        return 1;
    }

    const char *action = argv[1];
    const char *filename = argv[2];
    const char *key = argv[3];

    if (strcmp(action, "encrypt") == 0) {
        encrypt(filename, key);
        printf("File encrypted successfully\n");
    } else if (strcmp(action, "decrypt") == 0) {
        decrypt(filename, key);
        printf("File decrypted successfully\n");
    } else {
        fprintf(stderr, "Invalid action: %s\n", action);
        return 1;
    }

    return 0;
}