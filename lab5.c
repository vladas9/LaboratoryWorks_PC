#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

typedef enum {
    et_str,
    et_ch,
    et_int,
    et_dub
} Element_type;

typedef struct {
    Element_type type;
    union {
        char *str;
        char c;
        int i;
        double d;
    } data;
} Element;

typedef struct {
    Element *elements;
    int size;     
    int capacity;  
} stack;

stack* create_stack(int capacity) {
    stack* s = (stack*)malloc(sizeof(stack));
    s->elements = (Element *)malloc(capacity * sizeof(Element));
    if (!s || !s->elements) {
        printf("Error can't allocate memory\n");
    }
    s->size = 0;
    s->capacity = capacity;
    return s;
}

bool IsInteger(const char* str) {
    while (*str) {
        if (!isdigit(*str)) return false;
        str++;
    }
    return true;
}

bool IsDouble(const char* str) {
    int dotcount = 0;
    while (*str) {
        if (!isdigit(*str)) {
            if (*str == '.' && dotcount == 0) {
                dotcount++;
            } else {
                return false;
            }
        }
        str++;
    }
    return true;
}

void push(stack* s, Element value) {
    if (s->size == s->capacity) {
        s->capacity += 1;
        s->elements = (Element*)realloc(s->elements, s->capacity * sizeof(Element));
        if (!s->elements) {
            printf("Error can't reallocate memory\n");
        }
    }
    s->elements[s->size++] = value;
}

void determinate(stack* s) {
    printf("Enter the value to push in stack: ");
    char str[100];
    scanf("%s", str);

    Element value;
    if (IsInteger(str)) {
        value.type = et_int;
        value.data.i = atoi(str);
        printf("Element %i pushed in the stack\n", value.data.i);
    } else if (IsDouble(str)) {
        value.type = et_dub;
        value.data.d = atof(str);
        printf("Element %f pushed in the stack\n", value.data.d);
    } else if (strlen(str) == 1) {
        value.type = et_ch;
        value.data.c = str[0];
        printf("Element %c pushed in the stack\n", value.data.c);
    } else {
        value.type = et_str;
        value.data.str = strdup(str);
        printf("Element %s pushed in the stack\n", value.data.str);
    }
    push(s, value);
}

Element pop(stack* s) {
    if (s->size == 0) {
        printf("stack is empty\n");
        Element e;
        e.type = et_int;
        e.data.i = -1;
        return e;
    }
    return s->elements[--s->size];
}

Element peek(stack* s) {
    if (s->size == 0) {
        printf("stack is empty\n");
        Element e;
        e.type = et_int;
        e.data.i = -1;
        return e;
    }
    return s->elements[s->size - 1];
}

void freestack(stack* s) {
    for (int i = 0; i < s->size; i++) {
        if (s->elements[i].type == et_str) {
            free(s->elements[i].data.str);
        }
    }
    free(s->elements);
    free(s);
}

int main() {
    stack* s = create_stack(1);
    int choice, value;
    while (1) {
        printf("Stack operation menu:\n1.Push\n2.Pop\n3.Peek\n4.IsEmpty\n5.IsFull\n6.Exit\n");
        printf("Enter your choice: ");
        scanf("%i", &choice);
        switch (choice) {
        case 1:
            determinate(s);
            break;
        case 2:
            Element popped = pop(s);
            if (popped.data.i != -1) {
                switch (popped.type) {
                case et_int:
                    printf("Popped value: %i\n", popped.data.i);
                    break;
                case et_dub:
                    printf("Popped value: %f\n", popped.data.d);
                    break;
                case et_ch:
                    printf("Popped value: %c\n", popped.data.c);
                    break;
                case et_str:
                    printf("Popped value: %s\n", popped.data.str);
                    break;
                default:
                    break;
                }
            }
            break;
        case 3:
            Element peeked = peek(s);
            if (peeked.data.i != -1) {  
                switch (peeked.type) {
                case et_int:
                    printf("Peeked value: %i\n", peeked.data.i);
                    break;
                case et_dub:
                    printf("Peeked value: %f\n", peeked.data.d);
                    break;
                case et_ch:
                    printf("Peeked value: %c\n", peeked.data.c);
                    break;
                case et_str:
                    printf("Peeked value: %s\n", peeked.data.str);
                    break;
                default:
                    break;
                }
            }
            break;
        case 4:
            if (s->size == 0) {
                printf("Stack is empty\n");
            } else {
                printf("Stack isn't empty\n");
            }
            break;
        case 5:
            if (s->size == s->capacity) {
                printf("Stack is full\n");
            } else {
                printf("Stack isn't full\n");
            }
            break;
        case 6:
            freestack(s);
            return 0;
        default:
            break;
        }
    }
}
