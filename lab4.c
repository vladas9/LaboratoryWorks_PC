#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 1000  

typedef struct Buffer {
    char text[BUFFER_SIZE];
} Buffer;

void enterText(Buffer *buffer){
    char temp[BUFFER_SIZE/2];
    printf("Enter your text: ");
    fgets(temp, sizeof(temp), stdin);
    if(strlen(buffer->text) + strlen(temp) > BUFFER_SIZE){
        printf("Memory overflow");
        return;
    }
    strcat(buffer->text, temp);
}

int isDelimiter(char c) {
    return !isalpha(c) && !isdigit(c);  
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
        ptr++;
    }
    return count;
}
void replaceWord(Buffer *buffer,const char *oldWord, const char *newWord ){
    char temp[BUFFER_SIZE] = {""};
    char *ptr = buffer->text;
    while(*ptr){
        if(strstr(ptr, oldWord) == ptr){
            strcat(temp, newWord);
            ptr += strlen(oldWord);
        }else{
            strncat(temp, ptr, 1);
            ptr++;
        }
    }
    strcpy(buffer->text, temp);
}

void deleteBuffer(Buffer *buffer){
    char temp[BUFFER_SIZE] = {""};
    strcpy(buffer->text, temp);
}

void saveToFile(Buffer *buffer,const char *filename){
    FILE *fptr;
    char path[50];
    sprintf(path, "data/%s.txt", filename);
    fptr = fopen(path ,"a");
    fputs(buffer->text, fptr);
    fclose(fptr);
}

void loadFromFile(Buffer *buffer, const char *filename){
    FILE *fptr;
    char path[50];
    sprintf(path, "data/%s.txt", filename);
    fptr = fopen(path ,"r");
    fread(buffer->text, sizeof(char), BUFFER_SIZE - 1, fptr);
    fclose(fptr);

    printf("Loaded text from %s\n", fptr);
}

int main() {
    Buffer buffer = {""};
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
                    printf("Word was successfully replaced\n");
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

    return 0;
}