#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_KEY_LENGTH 256

// Function to XOR decrypt the text using the given key
void xor_decrypt(const unsigned char *ciphertext, unsigned char *plaintext, const unsigned char *key, int key_length, int text_length) {
    for (int i = 0; i < text_length; i++) {
        plaintext[i] = ciphertext[i] ^ key[i % key_length];
    }
}

// Function to attempt to recover the key
void recover_key(const unsigned char *ciphertext, unsigned char *recovered_key, int key_length, int text_length) {
    // Assuming that the plaintext is mostly printable ASCII characters
    for (int i = 0; i < key_length; i++) {
        int counts[256] = {0};

        // Count occurrences of each byte value in the ciphertext at positions corresponding to the key byte
        for (int j = i; j < text_length; j += key_length) {
            counts[ciphertext[j]]++;
        }

        // Find the most common byte value (this assumes the plaintext is in English and that space ' ' is the most common character)
        int max_count = 0;
        unsigned char likely_byte = 0;
        for (int k = 0; k < 256; k++) {
            if (counts[k] > max_count) {
                max_count = counts[k];
                likely_byte = k;
            }
        }

        // Recover the key byte assuming that the most frequent character in plaintext is space ' '
        recovered_key[i] = likely_byte ^ ' ';
    }
}

int main() {
    // Example ciphertext (this would be provided by the user)
    unsigned char ciphertext[] = {0x5A, 0x33, 0x67, 0x1A, 0x4D, 0x22, 0x2F, 0x6E};  // Example data
    int text_length = sizeof(ciphertext);

    int key_length = 3;  // Example key length (this would be provided by the user)
    unsigned char recovered_key[MAX_KEY_LENGTH];
    unsigned char plaintext[text_length + 1];

    // Recover the key
    recover_key(ciphertext, recovered_key, key_length, text_length);

    // Null-terminate the recovered key
    recovered_key[key_length] = '\0';

    // Decrypt the ciphertext using the recovered key
    xor_decrypt(ciphertext, plaintext, recovered_key, key_length, text_length);

    // Null-terminate the plaintext
    plaintext[text_length] = '\0';

    // Print the results
    printf("Recovered key: ");
    for (int i = 0; i < key_length; i++) {
        printf("%02X ", recovered_key[i]);
    }
    printf("\n");

    printf("Decrypted plaintext: %s\n", plaintext);

    return 0;
}
