#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main() {
    char text[200], key[200];
    int i, j = 0;

    printf("Enter plaintext: ");
    scanf("%s", text);

    printf("Enter key (text): ");
    scanf("%s", key);

    int textLen = strlen(text);
    int keyLen = strlen(key);

    char cipher[200];

    // 🔐 Encryption
    for(i = 0; i < textLen; i++) {

        if(isalpha(text[i])) {

            int shift;
            
            // Convert key character to shift (A/a = 0, B/b = 1...)
            if(isupper(key[j]))
                shift = key[j] - 'A';
            else
                shift = key[j] - 'a';

            if(isupper(text[i])) {
                cipher[i] = ((text[i] - 'A' + shift) % 26) + 'A';
            } else {
                cipher[i] = ((text[i] - 'a' + shift) % 26) + 'a';
            }

            j = (j + 1) % keyLen;
        } else {
            cipher[i] = text[i];
        }
    }
    cipher[i] = '\0';

    printf("Encrypted Text: %s\n", cipher);

    // 🔓 Decryption
    j = 0;
    char decrypted[200];

    for(i = 0; i < textLen; i++) {

        if(isalpha(cipher[i])) {

            int shift;

            if(isupper(key[j]))
                shift = key[j] - 'A';
            else
                shift = key[j] - 'a';

            if(isupper(cipher[i])) {
                decrypted[i] = ((cipher[i] - 'A' - shift + 26) % 26) + 'A';
            } else {
                decrypted[i] = ((cipher[i] - 'a' - shift + 26) % 26) + 'a';
            }

            j = (j + 1) % keyLen;
        } else {
            decrypted[i] = cipher[i];
        }
    }
    decrypted[i] = '\0';

    printf("Decrypted Text: %s\n", decrypted);

    return 0;
}
