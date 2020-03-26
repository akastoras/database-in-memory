/*By: akastoras, Project1, AEM: 03101
This program is a database for students classes, that uses an array of pointers to structs*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<ctype.h>

/*  STRUCTS  */
typedef struct student {
    long unsigned int AEM;
    char name[64];
    short unsigned int numOfClasses;
    struct node *classesHead;
} student;

typedef struct studentArray {
    student **array;
    int size;
    int used;
    bool isSorted;
    short int k;
} studentArray;

typedef struct node {
    unsigned short code;
    struct node *next;
} node;

/*  FUNCTION SIGNATURES  */

void arrayInit(studentArray *myArray, char *argv[]);
/*Initializes the array*/

void upper(char *str);
/*Changes a strings alphabets to upper case*/

int find(studentArray *array, long unsigned int AEM, int count);
/*Searches for a student with the given AEM, returns the index in the pointer array.
Else returns -1. Insert count = 0, to print the number of comparissons as stderr, else count = -1*/

int binarySearch(student **array, long unsigned int AEM, int size, int *count);
/*Used if the array is sorted. Insert count = 0, to print the number of comparissons as stderr, , else insert count = -1*/

int linearSearch(studentArray *array, long unsigned int AEM, int *count);
/*Used if the array isn't sorted. Insert count = 0, to print the number of comparissons as stderr, , else insert count = -1*/

void sort(studentArray *array);
/*Uses insertion sort algorithm, to sort a array of pointers to student, prints as
stderr the number of comparisons, after that, changes isSorted to true*/

int add(studentArray *array, int newAEM, char newName[64], unsigned short int newNumOfClasses);
/*Adds a new student, reallocating the arrays size if needed,
returns 1 for success, returns 0 if student is already in the array,
returns -1 if there is no more memory. If the array is sorted AND the new additions AEM is
smaller than the last element, changes isSorted to false*/

int rmv(studentArray *array, int AEM);
/*Removes a student from the array, and moves the last element to the new empty space.
Changes isSorted if needed. Reallocates size if needed*/

int mod(studentArray *array, int AEM, unsigned short int classes);
/*Change a students num of classes, returns 1 for success, 0 for failture*/

void print(studentArray *array);
/*Print the array*/

void clear(studentArray *array);
/*Free memory*/

/**Simple List Functions**/
int add_class(node **head, short int code);
/*Adds a new class to the liked list of a student's classes, returns 1 for success, else 0*/

int find_class(node **head, short int code);
/*Return 1 if the class with the given code is in the list, else 0*/

int rmv_class(node **head, short int code);
/*Removes the given class from the list, returns 1 for success, else 0*/

void print_classes(node *head);
/*Prints a student's classes*/

/**/

/*LEITOURGIESSSSS(kai afto tha fugei)*/

/**/
int reg(studentArray *array, long unsigned int AEM, unsigned short code);
/*Returns 1 for succes, 0 for non-existing AEM, -1 for existing class*/
int unreg(studentArray *array, long unsigned int AEM, unsigned short code);
/*Returns 1 for succes, 0 for non-existing AEM, -1 for non-existing class*/
int isreg(studentArray *array, long unsigned int AEM, unsigned short code);
/*Returns 1 for succes, 0 for non-existing AEM, -1 for non-existing class*/
int list_courses(studentArray *array, long unsigned int AEM);
/*Prints all the courses of the student with the given AEM, returns 1, if student-not-found returns 0*/

/*MAIN*/

int main(int argc, char* argv[]) {
    if (argc != 3 || atoi(argv[1]) < 0 || atoi(argv[2]) < 1) {
        fprintf(stderr, "\n***\nWrong arguments!\n***\n");
        return 0;
    }
    int res=0;
    short unsigned int newNumOfClasses, classes, c_code;
    long unsigned int newAEM, AEM;
    char newName[64], func;

    studentArray myArray;
    arrayInit(&myArray, argv);

    while (1) {
        scanf(" %c", &func);
        switch (func) {
            case 'a': {
                res = scanf("%lu %s %hu", &newAEM, newName, &newNumOfClasses);
                upper(newName);
                if (res != 0) {
                    res = add(&myArray, newAEM, newName, newNumOfClasses);
                    if (res == 1) {
                        printf("\nA-OK %lu, %d %d\n", newAEM, myArray.used, myArray.size);
                    }
                    else {
                        printf("\nA-NOK %lu, %d %d\n", newAEM, myArray.used, myArray.size);
                    }
                }
                break;
            }
            case 'r': {
                scanf("%lu",&AEM);
                res = rmv(&myArray, AEM);
                if (res == 1) {
                    printf("\nR-OK %lu, %d %d\n", AEM, myArray.used, myArray.size);    
                }
                else {
                    printf("\nR-NOK %lu, %d %d\n", AEM, myArray.used, myArray.size);
                }
                break;
            }
            case 'm': {
                scanf("%ld %hu", &AEM, &classes);
                res = mod(&myArray, AEM, classes);
                if (res == 1) {
                    printf("\nM-OK %ld\n", AEM);
                }
                else {
                    printf("\nM-NOK %ld\n", AEM);
                }
                break;
            }
            case 's': {
                sort(&myArray);
                printf("\nS-OK\n");
                break;
            }
            case 'f': {
                scanf("%ld", &AEM);
                res = find(&myArray, AEM, 0);
                if (res == -1) {
                    printf("\nF-NOK %ld\n", AEM);
                }
                else {
                    printf("\nF-OK %s %hu\n", myArray.array[res]->name, myArray.array[res]->numOfClasses);
                }
                break;
            }
            case 'p': {
                print(&myArray);
                break;
            }
            case 'c': {
                clear(&myArray);
                printf("\nC-OK\n");
                break;
            }
            case 'q': {
                clear(&myArray);
                return(0);
            }
            case 'g': {
                scanf("%ld %hu", &AEM, &c_code);
                res = reg(&myArray, AEM, c_code);
                if (res == 1) {
                    printf("\nG-OK %ld %hu\n", AEM, c_code);
                }
                else {
                    printf("\nG-NOK ");
                    if (res == 0) {
                        printf("%ld\n", AEM);
                    }
                    else {
                        printf("%hu\n", c_code);
                    }
                }
                break;
            }
            case 'u': {
                scanf("%ld %hu", &AEM, &c_code);
                res = unreg(&myArray, AEM, c_code);
                if (res == 1) {
                    printf("\nU-OK %ld %hu\n", AEM, c_code);
                }
                else {
                    printf("\nU-NOK ");
                    if (res == 0) {
                        printf("%ld\n", AEM);
                    }
                    else {
                        printf("%hu\n", c_code);
                    }
                }
                break;
            }
            case 'l': {
                scanf("%lu", &AEM);
                res = list_courses(&myArray, AEM);
                if (res == 0) {
                    printf("\nL-NOK %lu\n", AEM);
                }
                break;
            }
            case 'i': {
                scanf("%ld %hu", &AEM, &c_code);
                res = isreg(&myArray, AEM, c_code);
                if (res == 1) {
                    printf("\nYES\n");
                }
                else if (res == -1) {
                    printf("\nNO\n");
                }
                else {
                    printf("\nI-NOK %ld\n", AEM);
                }
                break;
            }
            default: {
                break;
            }
        }
    }
}

/*  FUNCTIONS  */
void arrayInit(studentArray *myArray, char *argv[]) {
    myArray->array = calloc(atoi(argv[1]), sizeof(student *));
    myArray->size = atoi(argv[1]);
    myArray->isSorted = true;
    myArray->k = atoi(argv[2]);
}
/**/
void upper(char *str) {
    unsigned int i;
    for (i=0; i < strlen(str); i++) {
        if (isalpha(str[i])) {
            str[i] = toupper(str[i]);
        }
    }
}
/**/
int binarySearch(student **array, long unsigned int AEM, int size, int *count) {
    int left = 0, right = size - 1, mid, add;
    if (*count == -1) {
        add = 0;
    }
    else {
        add = 1;
    }

    while (left <= right && left >= 0) {
        mid = left + (right - left) / 2;
        (*count) += add;
        if (array[mid]->AEM == AEM) {
            return mid;
        } 
        if (array[mid]->AEM < AEM) {
            (*count) += add;
            left = mid + 1;
        }
        else {
            (*count) += add;
            right = mid - 1;
            
        }
    }
    return -1;
}
/**/
int linearSearch(studentArray *array, long unsigned int AEM, int *count) {
    int i;
    for (i = 0; i < array->size && array->array[i] != NULL; i++) {
        if ((*count) >= 0) {
            (*count)++;
        }
        if (array->array[i]->AEM == AEM) {
            return i;
        }
    }
    /*else*/
    return -1;
}
/**/
int find(studentArray *array, long unsigned int AEM, int count) {
    int res = 0;
    if (array->used == 0) {
        return -1;
    }
    /*else*/
    if (array->isSorted) {
        res = binarySearch(array->array, AEM, array->used, &count);
        if (count != -1) {
            fprintf(stderr,"\n$%d\n", count);
        }
        return res;
    }
    else {
        res = linearSearch(array, AEM, &count);
        if (count != -1) {
            fprintf(stderr,"\n$%d\n", count);
        }
        return res;
    }
}
/**/
void sort(studentArray *array) {
    int i, j;
    long unsigned cnt = 0;
    for (i = 0; i < array->used && array->array[i] != NULL; i++) {
        student *key = array->array[i];
        j = i - 1;
        while (j >= 0 && array->array[j]->AEM > key->AEM) {
            array->array[j + 1] = array->array[j];
            j--;
            cnt++;
        }
        array->array[j+1] = key;
    }
    cnt--;
    fprintf(stderr,"\n$%lu\n", cnt);
    array->isSorted = true;
}
/**/
int add(studentArray *array, int newAEM, char newName[64], unsigned short int newNumOfClasses) {
    student **help;
    int i;
    if (find(array, newAEM, -1) != -1) {
        return 0;
    }
    /*else*/
    student *std = calloc(1, sizeof(student));
    if (std == NULL) {
        return -1;
    }
    /*else*/
    std->AEM = newAEM;
    strcpy(std->name, newName);
    std->numOfClasses = newNumOfClasses;
    std->classesHead = NULL;
    if (array->size == array->used) {
        array->size += array->k;
        help = realloc(array->array, array->size * sizeof(student *));
        if (help == NULL) {
            return -1;  
        }
        else {
            array->array = help;
            for (i = 1; i < array->k; i++) {
                array->array[array->size - i] = NULL;
            }
        }
    }
    array->array[array->used] = std;
    if (array->used >= 1 && array->array[array->used]->AEM < array->array[array->used - 1]->AEM) {
        array->isSorted = false;
    }
    array->used++;
    return 1;
}
/**/
int rmv(studentArray *array, int AEM) {
    int index = find(array, AEM, -1);
    node *ptr, *prev;

    if (index == -1) {
        return 0;
    }
    for (prev = array->array[index]->classesHead, ptr = prev->next; ptr->next != NULL; prev = ptr, ptr = ptr->next) {
        free(prev);
    }
    free(array->array[index]);
    array->array[index] = array->array[array->used - 1];
    array->array[array->used - 1] = NULL;

    if (array->size - (array->used - 1) > array->k) {
        array->size -= array->k;
        array->array = realloc(array->array, array->size * sizeof(student));
    }
    if (index < array->used - 1) {
        array->isSorted = false;
    }
    array->used--;
    return 1;
}
/**/
int mod(studentArray *array, int AEM, unsigned short int classes) {
    int index = find(array, AEM, -1);
    if (index == -1) {
        return 0;
    }
    else {
        array->array[index]->numOfClasses = classes;
        return 1;
    }
}
/**/
void print(studentArray *array) {
    int i;
    printf("\n#\n");
    for (i=0; i < array->used; i++) {
        if (array->array[i] != NULL) {
            printf("%ld %s %hu\n", array->array[i]->AEM, array->array[i]->name, array->array[i]->numOfClasses);
        }
        else {
            break;
        }
    }
}
/**/
void clear(studentArray *array) {
    int i;
    node *ptr, *help;

    for (i = 0; i < array->used; i++) {
        for (ptr = array->array[i]->classesHead; ptr != NULL; ) {
            help = ptr;
            ptr = ptr->next;
            free(help);
        }
    }
    array->array = NULL;
    array->size = 0;
    array->used = 0;
    array->isSorted = true;
    free(array->array);
    return;    
}
/**/
int add_class(node **head, short int code) {
    node *ptr, *prev, *newNode;
    
    for (ptr = *head, prev = NULL; ptr != NULL && ptr->code <= code; prev = ptr, ptr = ptr->next);

    if (prev != NULL && prev->code == code) {
        return 0;
    }

    newNode = (node *) malloc(sizeof(node));
    
    if (newNode == NULL) {
        return -1;
    }
    if (ptr == *head) {
        *head = newNode;
    }
    else {    
        prev->next = newNode;
    }
    newNode->code = code;
    newNode->next = ptr;
    return 1;
}
/**/
int find_class(node **head, short int code) {
    node *ptr, *prev;

    for (ptr = *head, prev = NULL; ptr != NULL && ptr->code <= code; prev = ptr, ptr = ptr->next);

    if (prev != NULL && prev->code == code) {
        return 1;
    }
    else {
        return 0;
    }
}
/**/
int rmv_class(node **head, short int code) {
    node *ptr, *prev;

    for (ptr = *head, prev = NULL; ptr != NULL && ptr->code < code; ) {
        prev = ptr;
        ptr = ptr->next;
    }

    if (ptr != NULL) {
        if (ptr->code == code) {
            prev->next = ptr->next;
            free(ptr);
            return 1;
        } 
    }
    return 0;
}
/**/
void print_classes(node *head) {
    node *ptr = head;
    
    printf("\n#\n");
    while (ptr != NULL) {
        printf("%hu\n", ptr->code);
        ptr = ptr->next;
    }
}
/**/
int reg(studentArray *array, long unsigned int AEM, unsigned short code) {
    int index = find(array, AEM, -1), res;
    if (index != -1) {
        res = add_class(&(array->array[index]->classesHead), code);
        if (res == 1) {
            return 1;
        }
        else {
            return -1;
        }
    }
    else {
        return 0;
    }
}
/**/
int unreg(studentArray *array, long unsigned int AEM, unsigned short code) {
    int index = find(array, AEM, -1), res;
    if (index != -1) {
        res = rmv_class(&(array->array[index]->classesHead), code);
        if (res == 1) {
            return 1;
        }
        else {
            return -1;
        }
    }
    return 0;
}
/**/
int isreg(studentArray *array, long unsigned int AEM, unsigned short code) {
    int index = find(array, AEM, -1), res;
    if (index == -1) {
        return 0;
    }
    res = find_class(&(array->array[index]->classesHead), code);
    if (res == 1) {
        return 1;
    }
    return -1;
}
/**/
int list_courses(studentArray *array, long unsigned int AEM) {
    int index = find(array, AEM, -1);

    if (index != -1) {
        print_classes(array->array[index]->classesHead);
        return 1;
    }
    else {
        return 0;
    }
}