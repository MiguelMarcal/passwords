#include <stdio.h>
#include <string.h>
#include <openssl/evp.h>

#define FILENAME "data.txt"
#define MAX_STR_LEN 128
#define SALT_SIZE 16

void clearTerminal() {
    printf("\033[H\033[J");
}

void encryptDecrypt(char *input, char *salt, char *password, int do_encrypt) {
    EVP_CIPHER_CTX *ctx;
    int len, ciphertext_len;

    ctx = EVP_CIPHER_CTX_new();

    EVP_CipherInit_ex(ctx, EVP_aes_256_cbc(), NULL, (const unsigned char *)password, (const unsigned char *)salt, do_encrypt);

    if (do_encrypt) {
        EVP_CipherUpdate(ctx, (unsigned char *)input, &len, (const unsigned char *)input, strlen(input));
        ciphertext_len = len;
        EVP_CipherFinal_ex(ctx, (unsigned char *)input + len, &len);
        ciphertext_len += len;
    } else {
        EVP_CipherUpdate(ctx, (unsigned char *)input, &len, (const unsigned char *)input, strlen(input));
        ciphertext_len = len;
        EVP_CipherFinal_ex(ctx, (unsigned char *)input + len, &len);
        ciphertext_len += len;
    }

    EVP_CIPHER_CTX_free(ctx);
}
int main() {
    char password[MAX_STR_LEN];
    char salt[SALT_SIZE];

    printf("Enter the password: ");
    fgets(password, sizeof(password), stdin);

    printf("Enter the salt (16 bytes, e.g., a random hexadecimal string): ");
    fgets(salt, sizeof(salt), stdin);

    // Remove newline characters from the input
    password[strcspn(password, "\n")] = '\0';
    salt[strcspn(salt, "\n")] = '\0';

    while (1) {
        char option;
        printf("Do you want to:\nRead from the file   ->(R) \nWrite to the file   ->(W)\nQuit   ->(Q) ");
        scanf(" %c", &option);


        if (option == 'R' || option == 'r') {
            // Read and decrypt strings from the file
            char line[MAX_STR_LEN];
            FILE *file = fopen(FILENAME, "a+");

            clearTerminal();

            while (fgets(line, sizeof(line), file)) {
                line[strcspn(line, "\n")] = '\0'; // Remove newline character
                encryptDecrypt(line, salt, password, 0); // Decrypt the string
                printf("Decrypted: %s\n", line);
            }
            fclose(file);

        } else if (option == 'W' || option == 'w') {
            FILE *file = fopen(FILENAME, "a+");

            // Write encrypted strings to the file
            clearTerminal();
            char new_string[MAX_STR_LEN];
            printf("Enter the string to write: ");
            scanf(" %[^\n]", new_string);

            encryptDecrypt(new_string, salt, password, 1); // Encrypt the new string
            fprintf(file, "%s\n", new_string);
            printf("String added to the file.\n");
            fclose(file);

        } else if (option == 'Q' || option == 'q') {
            clearTerminal();
            return 0;
        } else {
            printf("Invalid option.\n");
        }

    }

    return 0;
}