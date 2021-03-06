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
    struct student *next;
    struct student *prev;
} student;

typedef struct node {
    unsigned short code;
    struct node *next;
} node;

typedef struct bucket {
    student *head;
    int size;
} bucket;

typedef struct hashTable {
    int size;
    int min_size;
    float load_factor;
    int largest_bucket;
    bucket *bucketArray;
} hashTable;

typedef struct studentArray {
    student **array;
    int size;
    int used;
    bool isSorted;
    short int k;
    hashTable table;
} studentArray;

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

void clear(studentArray *array, char *argv[]);
/*Free memory*/

/**/

/**Simple Linked-List Functions**/

int add_class(node **head, short int code);
/*Adds a new class to the liked list of a student's classes, returns 1 for success, else 0*/

int find_class(node **head, short int code);
/*Return 1 if the class with the given code is in the list, else 0*/

int rmv_class(node **head, short int code);
/*Removes the given class from the list, returns 1 for success, else 0*/

void print_classes(node *head);
/*Prints a student's classes*/

/**/

int reg(studentArray *array, long unsigned int AEM, unsigned short code);
/*Returns 1 for succes, 0 for non-existing AEM, -1 for existing class*/

int unreg(studentArray *array, long unsigned int AEM, unsigned short code);
/*Returns 1 for succes, 0 for non-existing AEM, -1 for non-existing class*/

int isreg(studentArray *array, long unsigned int AEM, unsigned short code);
/*Returns 1 for succes, 0 for non-existing AEM, -1 for non-existing class*/

int list_courses(studentArray *array, long unsigned int AEM);
/*Prints all the courses of the student with the given AEM, returns 1, if student-not-found returns 0*/

/**/

/**Double Linked-List Functions**/

void init_list(bucket *bucket);
/*Initializes an empty double linked-list*/

int find_std(bucket bucket, char *name);
/*Searches for a student in a double linked-list by name, and returns how many
students with that name are in the list*/

void insert_std(bucket *bucket, student *std);
/*Inserts an existing student node to a double linked-list*/

void rmv_std(student *std);
/*Removes an student node from a double linked-list, does not delete the node*/

void init_string(char *string) {
    char c = string[0];
    while (c != '\0') {
        c = '\0';
    }
}
/*Clears a string*/

/**/

/**Hash table functions**/

unsigned long hash(char *str) {
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;

    return hash;
}
/*Returns hash value for a string*/

int get_hash(hashTable *table, char *str);
/*Returns the hash value mod the tables size*/

void init_hash_table(hashTable *table);
/*Initializes an empty hash table*/

float get_load_factor(studentArray *array);
/*Returns the hash tables load facrot*/

void set_largest_bucket(hashTable *table);
/*Changes the largest bucket's value, if needed*/

void add_to_hash(studentArray *array, student *std);
/*adds a student to the hash table*/

void rmv_from_hash(studentArray *array, student *std);
/*removes an student from the hash table*/

student **find_by_name(studentArray *array, char *name, int *num_of_std);
/*Finds a student using the hash table, returns an array of pointers to the students if they exist,
 else returns NULL, sets nom_of_std to the size of the returned array*/

void rehash(studentArray *array);
/*Depending on the lages bucket and the load factor's value, changes the tables size
and reenters each student to the hash table*/
/**/

/*MAIN*/

int main(int argc, char* argv[]) {
    if (argc != 4 || atoi(argv[1]) < 0 || atoi(argv[2]) < 1) {
        fprintf(stderr, "\n***\nWrong arguments!\n***\n");
        return 0;
    }
    int res=0, num_of_stds, i;
    short unsigned int newNumOfClasses, classes, c_code;
    long unsigned int newAEM, AEM;
    char newName[64], func, name[64];
    student **array_of_stds;

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
                clear(&myArray, argv);
                printf("\nC-OK\n");
                break;
            }
            case 'q': {
                clear(&myArray, argv);
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
                printf("\n");
                res = list_courses(&myArray, AEM);
                if (res == 0) {
                    printf("L-NOK %lu\n", AEM);
                }
                break;
            }
            case 'n': {
                scanf("%s", name);
                upper(name);
                array_of_stds = find_by_name(&myArray, name, &num_of_stds);
                if (num_of_stds == 0) {
                    printf("\nN-NOK %s\n", name);
                    break;
                }
                printf("\nN-OK %s\n", name);
                for (i = 0; i < num_of_stds; i++) {
                    printf("%ld %hu\n", array_of_stds[i]->AEM, array_of_stds[i]->numOfClasses);
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
    myArray->used = 0;
    myArray->isSorted = true;
    myArray->k = atoi(argv[2]);
    myArray->table.min_size = atoi(argv[3]);
    init_hash_table(&myArray->table);
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
            // fprintf(stderr,"\n$%d\n", count);
        }
        return res;
    }
    else {
        res = linearSearch(array, AEM, &count);
        if (count != -1) {
            // fprintf(stderr,"\n$%d\n", count);
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
    // fprintf(stderr,"\n$%lu\n", cnt);
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
    std->next = std->prev = NULL;
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
    std->classesHead = NULL;
    add_to_hash(array, std);
    rehash(array);
    return 1;
}
/**/
int rmv(studentArray *array, int AEM) {
    int index = find(array, AEM, -1);
    node *ptr;
    node *help;

    if (index == -1) {
        return 0;
    }
    for (ptr = array->array[index]->classesHead; ptr != help && ptr != NULL; ) {
        help = ptr;
        ptr = ptr->next;
        rmv_class(&array->array[index]->classesHead, help->code);
    }
    rmv_from_hash(array, array->array[index]);
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
    array->table.load_factor = get_load_factor(array);
    rehash(array);
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
    student *ptr;
    printf("\n#\n");
    printf("%d %d %.2f %d\n", array->table.size, array->used, array->table.load_factor, array->table.largest_bucket);
    for (i=0; i < array->table.size; i++) {
        printf("%d %d", i, array->table.bucketArray[i].size);
        for (ptr = array->table.bucketArray[i].head->next; ptr != array->table.bucketArray[i].head; ptr = ptr->next) {
            printf(" [%ld %s %hu]", ptr->AEM, ptr->name, ptr->numOfClasses);
        }
        printf("\n\n");
    }
}
/**/
void clear(studentArray *array, char *argv[]) {
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

    init_hash_table(&array->table);
    array->array = realloc(array->array, 0);

    return;    
}
/**/
int add_class(node **head, short int code) {
    node *ptr, *prev, *newNode;

    newNode = (node *) calloc(1, sizeof(node));
    newNode->code = code;

    for (ptr = *head, prev = NULL; ptr != NULL && ptr->code <= code; prev = ptr, ptr = ptr->next);

    if (prev != NULL && prev->code == code) {
        return 0;
    }
    
    if (newNode == NULL) {
        return -1;
    }
    if (ptr == *head) {
        newNode->next = *head;
        *head = newNode;
    }
    else {    
        prev->next = newNode;
        newNode->next = ptr;
    }
    return 1;
}
/**/
int find_class(node **head, short int code) {
    node *ptr;

    for (ptr = *head; ptr != NULL && ptr->code < code; ptr = ptr->next);

    if (ptr != NULL && ptr->code == code) {
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
            if (prev != NULL) {
                prev->next = ptr->next;
                free(ptr);
                return 1;
            }
            else {
                *head = ptr->next;
                free(ptr);
                return 1;
            }
        }
    }   
    return 0;
}
/**/
void print_classes(node *head) {
    node *ptr = head;
    
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
        printf("L-OK %s\n", array->array[index]->name);
        print_classes(array->array[index]->classesHead);
        return 1;
    }
    else {
        return 0;
    }
}
/**/

/*Double linked list functions*/

void init_list(bucket *bucket) {
    bucket->head = (student *) calloc(1,sizeof(student));
    bucket->head->next = bucket->head;
    bucket->head->prev = bucket->head;
}
/**/
int find_std(bucket bucket, char *name) {
    student *ptr;
    int num_of_stds=0;
    
    strcpy(bucket.head->name, name);
    for(ptr = bucket.head->next; strcmp(ptr->name, name); ptr = ptr->next);
    init_string(bucket.head->name);
    
    if (ptr == bucket.head) {
        return 0;
    }
    else {
        strcpy(bucket.head->name, "\0");
        while (strcmp(ptr->name, name) == 0) {
            num_of_stds++;
            ptr = ptr->next;
        }
        return num_of_stds;
    }
}
/**/
void insert_std(bucket *bucket, student *std) {
    student *ptr=NULL;
    /*Define ptr*/
    strcpy(bucket->head->name, std->name);
    bucket->head->AEM = std->AEM + 1;
    for(ptr = bucket->head->next; strcmp(ptr->name, std->name) < 0; ptr = ptr->next);
    
    if (strcmp(ptr->name, std->name) > 0) {
        ptr = ptr->prev;
    }
    else {
        if (ptr->AEM > std->AEM) {
            ptr = ptr->prev;
        }
        else {
            while (ptr != bucket->head && strcmp(ptr->next->name, std->name) == 0 && ptr->next->AEM < std->AEM) {
                ptr = ptr->next;
            }
        }
    }
    
    
    /*Add std after ptr*/
    std->next = ptr->next;
    std->prev = ptr;
    ptr->next = std;
    std->next->prev = std;
    bucket->size++;
    init_string(bucket->head->name);
}
/**/
void rmv_std(student *std) {
    std->prev->next = std->next;
    std->next->prev = std->prev;
}
/**/

/**Hash table functions**/

int get_hash(hashTable *table, char *str) {
    return (hash(str) % (table->size));
}
/**/
void init_hash_table(hashTable *table) {
    int i;
    
    table->size = table->min_size;
    table->load_factor = 0;
    table->largest_bucket = 0;
    table->bucketArray = (bucket *) calloc(table->size, sizeof(bucket));
    for (i = 0; i < table->size; i++) {
        table->bucketArray[i].size = 0;    
        init_list(&table->bucketArray[i]);
    }
    return;
}
/**/
float get_load_factor(studentArray *array) {
    return ((float) array->used / array->table.size);
}
void set_largest_bucket(hashTable *table) {
    int max=0, i;

    for (i = 0, max = table->bucketArray[i].size; i < table->size - 1; i++, (table->bucketArray[i].size > max ? max = table->bucketArray[i].size : 1));

    table->largest_bucket = max;
}

/**/
void add_to_hash(studentArray *array, student *std) {
    insert_std(&array->table.bucketArray[get_hash(&array->table, std->name)], std);
    int size = array->table.bucketArray[get_hash(&array->table, std->name)].size;
    if (size > array->table.largest_bucket) {
        array->table.largest_bucket = size;
    }
    set_largest_bucket(&array->table);
    array->table.load_factor = get_load_factor(array);
}
/**/
void rmv_from_hash(studentArray *array, student *std) {
    rmv_std(std);
    array->table.bucketArray[get_hash(&array->table, std->name)].size -= 1;
    set_largest_bucket(&array->table);
}
/**/
student **find_by_name(studentArray *array, char *name, int *num_of_stds) {
    int i = 0;
    *num_of_stds = 0;
    *num_of_stds = find_std(array->table.bucketArray[get_hash(&array->table, name)], name);
    student **array_of_stds = (student **) calloc((*num_of_stds), sizeof(student *));
    student *ptr = NULL;

    for (i = 0, ptr = array->table.bucketArray[get_hash(&array->table, name)].head; i < *num_of_stds; ptr = ptr->next) {
        if (strcmp(name, ptr->name) == 0) {
            array_of_stds[i] = ptr;
            i++;
        }
    }
    return array_of_stds;
}
/**/
void rehash(studentArray *array) {
    float lf = get_load_factor(array);
    int i;
    /*Check if rehash is needed*/
    if (lf > 1 && lf < 4) {
        return;
    }
    if (lf <= 1 && array->table.size == array->table.min_size) {
        return;
    }
    /*Empty the bucket array*/
    for (i = 0; i < array->table.size; i++) {
        array->table.bucketArray[i].head = NULL;
    }
    /*Define the new size of the table*/
    if (lf <= 1) {
        array->table.size = array->table.size / 2;
    }
    if (lf >= 4) {
        array->table.size *= 2;
    }

    /*Change the arrays size, to the defined one*/
    array->table.bucketArray = realloc(array->table.bucketArray, array->table.size * sizeof(bucket));

    /*Initialize the hash table*/
    array->table.load_factor = 0;
    array->table.largest_bucket = 0;
    for (i = 0; i < array->table.size; i++) { 
        init_list(&array->table.bucketArray[i]);
        array->table.bucketArray[i].size = 0; 
    }   

    /*Add every student to the new array*/
    for (i = 0; i < array->used; i++) {
        array->array[i]->next = array->array[i]->prev = NULL;
        add_to_hash(array, array->array[i]);
    }
}