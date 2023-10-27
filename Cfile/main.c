#include <stdio.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <readline/readline.h>
#include <readline/history.h>  // Include history support
#include <termios.h>  // Include termios for terminal configuration
#include <unistd.h>

#define HASHED_KEY "9917f777c07b978d862e33c6638b132710c6ef1dcab6b8174fa50e476e316e5a"
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
// Function to disable terminal echoing
void disableEcho() {
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

// Function to enable terminal echoing
void enableEcho() {
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag |= ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

int main() {
    char *password = NULL;
    char salt[MAX_STR_LEN];

    disableEcho();  // Disable echoing for password input
    password = readline("Enter the password: ");
    enableEcho();  // Enable echoing after password input
    printf("\n");  // Move to a new line after password input

    if (password == NULL) {
        perror("readline");
        return 1;
    }

    printf("Enter the salt (16 bytes, e.g., a random hexadecimal string): ");
    fgets(salt, sizeof(salt), stdin);
    salt[strcspn(salt, "\n")] = '\0';

    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((const unsigned char *)password, strlen(password), hash);

    char hashed_key[2 * SHA256_DIGEST_LENGTH + 1];
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(&hashed_key[i * 2], "%02x", hash[i]);
    }
    printf("%s", hashed_key);
    if (strcmp(hashed_key, HASHED_KEY) != 0) {
        printf("Access Denied.\n");
        return 0;
    }
    while (1) {
        char option;
        printf("Do you want to:\nRead from the file   ->(R) \nWrite to the file   ->(W)\nQuit   ->(Q) ");
        scanf(" %c", &option);

        if (option == 'R' || option == 'r') {
            clearTerminal();

            char line[MAX_STR_LEN];
            FILE *file = fopen(FILENAME, "a+");


            while (fgets(line, sizeof(line), file)) {
                line[strcspn(line, "\n")] = '\0';
                if(strlen(line) <=0){
                    fclose(file);
                    continue;
                }
                encryptDecrypt(line, salt, password, 0);
                printf("Decrypted: %s\n", line);
            }
            fclose(file);

        } else if (option == 'W' || option == 'w') {
            clearTerminal();
            FILE *file = fopen(FILENAME, "a+");

            char new_string[MAX_STR_LEN];
            printf("Enter the string to write: ");
            scanf(" %[^\n]", new_string);

            encryptDecrypt(new_string, salt, password, 1);
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
