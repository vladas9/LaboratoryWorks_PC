#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct Buffer {
    char *text;
    size_t size;
} Buffer;

void displayHeader(const char *message) {
    printf("\033[1;34m┌───────────────────────────────────────┐\033[0m\n");
    printf("\033[1;34m│\033[0m%*s%*s\033[1;34m│\033[0m\n",
           (40 + strlen(message)) / 2, message, 
           40 - (40 + strlen(message)) / 2 -  1, "");
    printf("\033[1;34m└───────────────────────────────────────┘\033[0m\n");
}

void initializeBuffer(Buffer *buffer) {
    buffer->size = 1000;
    buffer->text = (char *)malloc(buffer->size * sizeof(char));
    if (!buffer->text) {
        printf("\033[1;31mMemory allocation failed!\033[0m\n");
        exit(1);
    }
    buffer->text[0] = '\0';
}

void enterText(Buffer *buffer) {
    char temp[500];
    displayHeader("Enter your text below");
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
            printf("\033[1;31mMemory allocation failed!\033[0m\n");
            exit(1);
        }
    }
    strcat(buffer->text, temp);
    strcat(buffer->text, " ");
}

void printBuffer(Buffer *buffer){
    displayHeader("Text in buffer");
    printf("%s", buffer->text);
    printf("\n");
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
        printf("\033[1;31mMemory allocation failed!\033[0m\n");
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
        printf("\033[1;32mWord was successfully replaced\033[0m\n");
    } else {
        printf("\033[1;31mThere is no such word\033[0m\n");
    }
    free(temp);
}


void deleteBuffer(Buffer *buffer) {
    free(buffer->text);
    buffer->size = 0;
}

void saveToFile(Buffer *buffer, const char *filename) {
    FILE *fptr;
    char path[50];
    sprintf(path, "%s", filename);
    fptr = fopen(path, "w");
    if (!fptr) {
        printf("\033[1;31mUnable to open the file for writing.\033[0m\n");
        return;
    }
    fputs(buffer->text, fptr);
    fclose(fptr);
    printf("\033[1;32mText saved to %s\033[0m\n", path);
}

void loadFromFile(Buffer *buffer, const char *filename) {
    FILE *fptr;
    char path[50];
    sprintf(path, "%s", filename);
    fptr = fopen(path, "r");
    if (!fptr) {
        printf("\033[1;31mUnable to open the file for reading.\033[0m\n");
        return;
    }

    fseek(fptr, 0, SEEK_END);
    size_t fileSize = ftell(fptr);
    fseek(fptr, 0, SEEK_SET);

    if (fileSize >= buffer->size) {
        buffer->size = fileSize + 1;  
        buffer->text = (char *)realloc(buffer->text, buffer->size);
        if (!buffer->text) {
            printf("\033[1;31mMemory allocation failed!\033[0m\n");
            fclose(fptr);
            return;
        }
    }

    size_t bytesRead = fread(buffer->text, sizeof(char), fileSize, fptr);
    buffer->text[bytesRead] = '\0';  

    fclose(fptr);
    printf("\033[1;32mLoaded text from %s\033[0m\n", path);
}


int main() {
    Buffer buffer;
    initializeBuffer(&buffer);
    int choice;

    do {
        printf("\033[H\033[J");
        printf("\n\033[1;34m┌───────────────────────────────────────┐\033[0m\n");
        printf("\033[1;34m│\033[0m           \033[1;36mTEXT EDITOR MENU\033[0m            \033[1;34m│\033[0m\n");
        printf("\033[1;34m├───────────────────────────────────────┤\033[0m\n");

        printf("\033[1;31m│ [1]\033[0m Enter Text                        \033[1;34m│\033[0m\n");
        printf("\033[1;32m│ [2]\033[0m Print Buffer                      \033[1;34m│\033[0m\n");
        printf("\033[1;33m│ [3]\033[0m Search for Word                   \033[1;34m│\033[0m\n");
        printf("\033[1;34m│ [4]\033[0m Replace Word                      \033[1;34m│\033[0m\n");
        printf("\033[1;35m│ [5]\033[0m Delete Buffer                     \033[1;34m│\033[0m\n");
        printf("\033[1;36m│ [6]\033[0m Save to File                      \033[1;34m│\033[0m\n");
        printf("\033[1;37m│ [7]\033[0m Load from File                    \033[1;34m│\033[0m\n");
        printf("\033[1;38;5m│ [8]\033[0m Exit                              \033[1;34m│\033[0m\n");
        printf("\033[1;34m└───────────────────────────────────────┘\033[0m\n");
        
        printf("\033[1;36mChoice:\033[0m ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                printf("\033[1;38;5m To add a new line, use \\n \033[0m\n");
                enterText(&buffer);
                printf("\033[1;32mText was added to buffer\033[0m\n");
                break;
            case 2: 
                printBuffer(&buffer);
                break;
            case 3:  
                {
                    char word[100];
                    displayHeader("Enter word to search");
                    printf("->");
                    fgets(word, sizeof(word), stdin);
                    word[strcspn(word, "\n")] = '\0';
                    int count = searchForWord(&buffer, word);
                    printf("Occurrences of '%s': %d\n", word, count);
                }
                break;
            case 4:
                {
                    char oldWord[100], newWord[100];
                    displayHeader("Enter word to replace");
                    printf("->");
                    fgets(oldWord, sizeof(oldWord), stdin);
                    oldWord[strcspn(oldWord, "\n")] = '\0';
                    displayHeader("Enter new word");
                    printf("->");
                    fgets(newWord, sizeof(newWord), stdin);
                    newWord[strcspn(newWord, "\n")] = '\0';
                    replaceWord(&buffer, oldWord, newWord);
                }
                break;
            case 5:
                deleteBuffer(&buffer);
                printf("\033[1;32mBuffer was deleted\033[0m\n");
                initializeBuffer(&buffer);
                break;
            case 6:
                {
                    char filename[100];
                    displayHeader("Enter filename");
                    printf("->");
                    fgets(filename, sizeof(filename), stdin);
                    filename[strcspn(filename, "\n")] = '\0';
                    saveToFile(&buffer, filename);
                }
                break;
            case 7:
                {
                    char filename[100];
                    displayHeader("Enter filename");
                    printf("->");
                    fgets(filename, sizeof(filename), stdin);
                    filename[strcspn(filename, "\n")] = '\0';
                    loadFromFile(&buffer, filename);
                }
                break;
            case 8:  
                printf("\033[1;38;5mExiting...\033[0m\n");
                break;
            default:
                printf("\033[1;31mInvalid choice!\033[0m\n");
                break;
        }
        printf("\033[1;33m\nPress enter to continue...\033[0m");
        getchar();
    } while (choice != 8); 

    deleteBuffer(&buffer);
    return 0;
}


