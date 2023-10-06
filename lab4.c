#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct Buffer {
    char *text;
    size_t size;
} Buffer;

void initializeBuffer(Buffer *buffer) {
    buffer->size = 1000;
    buffer->text = (char *)malloc(buffer->size * sizeof(char));
    if (!buffer->text) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    buffer->text[0] = '\0';
}

void enterText(Buffer *buffer) {
    char temp[500];
    printf("Enter your text: ");
    fgets(temp, sizeof(temp), stdin);
    
    size_t tempLength = strlen(temp);
    if (tempLength > 0 && temp[tempLength - 1] == '\n') {
        temp[tempLength - 1] = '\0';
    }

    char *slashN;
    while ((slashN = strstr(temp, "\\n")) != NULL) {
        *slashN = '\n';
        memmove(slashN + 1, slashN + 2, strlen(slashN + 2) + 1);
    }

    size_t newLength = strlen(buffer->text) + strlen(temp) + 1;
    if (newLength > buffer->size) {
        buffer->size *= 2;
        buffer->text = (char *)realloc(buffer->text, buffer->size * sizeof(char));
        if (!buffer->text) {
            printf("Memory allocation failed!\n");
            exit(1);
        }
    }
    strcat(buffer->text, temp);
    strcat(buffer->text, " ");
}



int isDelimiter(char c) {
    return !isalpha(c) && !isdigit(c) && c != '_';
}

int searchForWord(Buffer *buffer, const char *word) {
    int count = 0;
    char *ptr = buffer->text;
    while ((ptr = strstr(ptr, word)) != NULL) {
        char charBefore = (ptr > buffer->text) ? *(ptr - 1) : ' ';
        char charAfter = *(ptr + strlen(word));

        if (isDelimiter(charBefore) && isDelimiter(charAfter)) {
            count++;
        }
        ptr += strlen(word);
    }
    return count;
}

void replaceWord(Buffer *buffer, const char *oldWord, const char *newWord) {
    char *temp = (char *)malloc(buffer->size * sizeof(char));
    if (!temp) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    temp[0] = '\0';

    char *ptr = buffer->text;
    int replaced = 0;

    while (*ptr) {
        if (strstr(ptr, oldWord) == ptr) {
            char charBefore = (ptr > buffer->text) ? *(ptr - 1) : ' ';
            char charAfter = *(ptr + strlen(oldWord));

            if (isDelimiter(charBefore) && isDelimiter(charAfter)) {
                strcat(temp, newWord);
                ptr += strlen(oldWord);
                replaced = 1;
                continue;
            }
        }
        strncat(temp, ptr, 1);
        ptr++;
    }

    strcpy(buffer->text, temp);

    if (replaced) {
        printf("Word was successfully replaced\n");
    } else {
        printf("There is no such word\n");
    }
    free(temp);
}


void deleteBuffer(Buffer *buffer) {
    free(buffer->text);
    buffer->text = NULL;
    buffer->size = 0;
}

void saveToFile(Buffer *buffer, const char *filename) {
    FILE *fptr;
    char path[50];
    sprintf(path, "data/%s.txt", filename);
    fptr = fopen(path, "w");
    if (!fptr) {
        printf("Unable to open the file for writing.\n");
        return;
    }
    fputs(buffer->text, fptr);
    fclose(fptr);
    printf("Text saved to %s\n", path);
}

void loadFromFile(Buffer *buffer, const char *filename) {
    FILE *fptr;
    char path[50];
    sprintf(path, "data/%s.txt", filename);
    fptr = fopen(path, "r");
    if (!fptr) {
        printf("Unable to open the file for reading.\n");
        return;
    }

    fseek(fptr, 0, SEEK_END);
    size_t fileSize = ftell(fptr);
    fseek(fptr, 0, SEEK_SET);

    if (fileSize >= buffer->size) {
        buffer->size = fileSize + 1;  
        buffer->text = (char *)realloc(buffer->text, buffer->size);
        if (!buffer->text) {
            printf("Memory allocation failed!\n");
            fclose(fptr);
            return;
        }
    }

    size_t bytesRead = fread(buffer->text, sizeof(char), fileSize, fptr);
    buffer->text[bytesRead] = '\0';  

    fclose(fptr);
    printf("Loaded text from %s\n", path);
}


int main() {
    Buffer buffer;
    initializeBuffer(&buffer);
    int choice;

    do {
        printf("\n=== TEXT EDITOR ===\n");
        printf("1. Enter Text\n");
        printf("2. Search for Word\n");
        printf("3. Replace Word\n");
        printf("4. Delete Buffer\n");
        printf("5. Save to File\n");
        printf("6. Load from File\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                enterText(&buffer);
                printf("Text was added to buffer\n");
                break;
            case 2:
                {
                    char word[100];
                    printf("Enter word to search: ");
                    fgets(word, sizeof(word), stdin);
                    word[strcspn(word, "\n")] = '\0';
                    int count = searchForWord(&buffer, word);
                    printf("Occurrences of '%s': %d\n", word, count);
                }
                break;
            case 3:
                {
                    char oldWord[100], newWord[100];
                    printf("Enter word to replace: ");
                    fgets(oldWord, sizeof(oldWord), stdin);
                    oldWord[strcspn(oldWord, "\n")] = '\0';
                    printf("Enter new word: ");
                    fgets(newWord, sizeof(newWord), stdin);
                    newWord[strcspn(newWord, "\n")] = '\0';
                    replaceWord(&buffer, oldWord, newWord);
                }
                break;
            case 4:
                deleteBuffer(&buffer);
                printf("Buffer was deleted\n");
                break;
            case 5:
                {
                    char filename[100];
                    printf("Enter filename without .txt extension: ");
                    fgets(filename, sizeof(filename), stdin);
                    filename[strcspn(filename, "\n")] = '\0';
                    saveToFile(&buffer, filename);
                }
                break;
            case 6:
                {
                    char filename[100];
                    printf("Enter filename without .txt extension: ");
                    fgets(filename, sizeof(filename), stdin);
                    filename[strcspn(filename, "\n")] = '\0';
                    loadFromFile(&buffer, filename);
                }
                break;
            case 7:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice!\n");
                break;
        }
    } while (choice != 7);

    deleteBuffer(&buffer);
    return 0;
}
