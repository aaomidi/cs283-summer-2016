#include <iostream>
#include <stdlib.h>
#include <cstdio>

// My fast implementation of malloc, checks the output of malloc.
#define MY_MALLOC(ptr, count, lbl)\
do { \
 if (NULL == (ptr = malloc(count))) { \
    fprintf(stderr, "%s: Failed to allocate memory.\n",lbl); \
    exit(EXIT_FAILURE); \
 } \
} while(0)


using namespace std;

struct Node {
    struct Node *next;
    int value;
};

void part1() {
    int i;
    void *ptr = nullptr;
    MY_MALLOC(ptr, 10 * sizeof(int), "Integers");
    int *nums = (int *) ptr;

    cout << "Settings numbers:" << endl;
    for (i = 0; i < 10; i++) {
        int ran = rand();
        cout << i << ". " << ran << endl;
        nums[i] = ran;
    }

    cout << "Printing numbers:" << endl;
    for (i = 0; i < 10; i++) {
        cout << i << ". " << nums[i] << endl;
    }

    free(nums);
}

void part2() {
    int i;
    char **arr = nullptr;
    if (NULL == (arr = (char **) malloc(10 * sizeof(char *)))) {
        fprintf(stderr, "%s: Failed to allocate memory.\n", "Array-1"); // Throw error if something is going wrong.
        exit(EXIT_FAILURE);
    }

    cout << "Initializing strings." << endl;
    for (i = 0; i < 10; i++) {
        void *ptr;
        MY_MALLOC(ptr, 15 * sizeof(char), "Array - 2");
        arr[i] = (char *) ptr;
        sprintf(arr[i], "String #%d\0", i);
    }

    cout << "Reading strings" << endl;
    for (i = 0; i < 10; i++) {
        cout << arr[i] << endl;
    }

    for (i = 0; i < 10; i++) {
        free(arr[i]);
    }

    free(arr);
}

// Reference to Insertion Sort (wikipedia).
void part3Sort(int **a, int size) {
    int i, j, k, temp;
    for (i = 0; i < size; i++) {
        for (j = i, k = i; j < size; j++) {
            if ((*a)[j] < (*a)[k]) {
                k = j;
            }
        }
        temp = (*a)[i];
        (*a)[i] = (*a)[k];
        (*a)[k] = temp;
    }
}

void part3() {
    int i;
    void *ptr;
    MY_MALLOC(ptr, 8 * sizeof(int), "Part3");
    int *a = (int *) ptr;
    int size = 8;
    for (i = 0; i < size; i++) {
        a[i] = rand() * ((rand() % 2 == 0) ? -1 : 1);
    }

    cout << "Before sort" << endl;
    for (i = 0; i < size; i++) {
        printf("%d. %d\n", i, a[i]);
    }

    part3Sort(&a, size); // pass pointer by reference.

    cout << "After sort" << endl;
    for (i = 0; i < size; i++) {
        printf("%d. %d\n", i, a[i]);
    }

    free(a);
}

struct Node *part4Construct(int val) {
    void *ptr = nullptr;
    MY_MALLOC(ptr, sizeof(Node), "Constructing " + val);
    Node *boop = (Node *) ptr;
    boop->next = nullptr;
    boop->value = val;

    return boop;
}


void part4Sort(Node **root) { // Pass by reference. Needed for address manipulation.
    bool done = false;
    if (*root == NULL || (*root)->next == NULL) return;

    while (!done) {
        Node **previous = root; // Previous node

        Node *current = *root;
        Node *next = (*root)->next;

        done = true;

        while (next != NULL) {
            if (current->value > next->value) {
                current->next = next->next;
                next->next = current;
                *previous = next;
                done = 0;
            }
            previous = &current->next;
            current = next;
            next = next->next;
        }
    }
}

void part4Clean(Node *root) {
    if (root == NULL) {
        return;
    }
    part4Clean(root->next);
    free(root);
}

void part4() {
    Node *root = part4Construct(rand() * ((rand() % 2 == 0) ? -1 : 1));
    Node *next = root;
    Node *last = root;

    next = part4Construct(rand() * ((rand() % 2 == 0) ? -1 : 1));
    last->next = next;
    last = next;

    next = part4Construct(rand() * ((rand() % 2 == 0) ? -1 : 1));
    last->next = next;
    last = next;

    next = part4Construct(rand() * ((rand() % 2 == 0) ? -1 : 1));
    last->next = next;

    last = root;
    cout << "Before sort" << endl;
    while (last != NULL) {
        cout << last->value << endl;
        last = last->next;
    }

    cout << "After sort" << endl;
    last = root;
    part4Sort(&last);
    while (last != NULL) {
        cout << last->value << endl;
        last = last->next;
    }
    part4Clean(root);
}

int size;
int numElement;
int *array;

void resize(int type) {
    int newSize = 0;
    if (type == 1) {
        newSize = size + 1;
    } else {
        newSize = size * 2;
    }
    if (NULL == (array = (int *) realloc(array, newSize * sizeof(int)))) {
        cout << "Bad stuff happened" << endl;
        exit(EXIT_FAILURE);
    }
    size = newSize;
    for (int i = numElement; i < newSize; i++) {
        array[i] = 0;
    }

}

void part5Add(int i, int resizeType) {
    if (numElement == size) {
        resize(resizeType);
    }

    numElement++;
    array[numElement - 1] = i;
}


int part5Get(int i) {
    if (i > numElement || i < 0) {
        return -1;
    }
    return array[i];
}

void part5Remove(int i) {
    if (i > numElement || i < 0) {
        return;
    }
    for (int j = i + 1; j < numElement; j++) {
        array[j - 1] = array[j];
    }

    numElement--;
}

void printAllArray() {
    for (int i = 0; i < numElement; i++) {
        printf("%d. %d\n", i + 1, array[i]);
    }
}

void part5TimingTest() {
    clock_t start, stop;
    int sampleSize = 10000000;
    size = 5;
    numElement = 0;
    void *ptr;
    MY_MALLOC(ptr, size * sizeof(int), "Part5-init");
    array = (int *) ptr;

    start = clock();
    for (int i = 0; i < sampleSize; i++) {
        part5Add(i, 1); // Adding only one to the max size.
    }
    stop = clock();
    long timeTaken = 1000 * (stop - start) / CLOCKS_PER_SEC;
    printf("Testing by just extending the array by one for a sample size of %d elements took %ld milliseconds.\n",
           sampleSize, timeTaken);

    free(array);

    cout << "Cleaning up and starting second test" << endl;

    size = 5;
    numElement = 0;
    MY_MALLOC(ptr, size * sizeof(int), "Part5-init");
    array = (int *) ptr;

    start = clock();
    for (int i = 0; i < sampleSize; i++) {
        part5Add(i, 2); // Adding only one to the max size.
    }
    stop = clock();
    timeTaken = 1000 * (stop - start) / CLOCKS_PER_SEC;
    printf("Testing by extending the array to 2 x currentSize for a sample size of %d elements took %ld milliseconds.\n",
           sampleSize, timeTaken);
}

void part5() {
    size = 2;

    void *ptr;
    MY_MALLOC(ptr, size * sizeof(int), "Part5-init");
    array = (int *) ptr;

    part5Add(2, 1); // resize type 1.
    part5Add(4, 1);
    part5Add(5, 1);
    part5Add(6, 1);

    printAllArray();
    cout << "Removing second element" << endl;
    part5Remove(1);

    printAllArray();

    cout << "Number in 3rd spot: " << part5Get(2) << endl;

    part5Add(8, 1);

    cout << "Added element 8" << endl;
    printAllArray();
    free(array);

    cout << "Starting timing test" << endl;
    part5TimingTest();
}

int main() {
    cout << "Starting part one." << endl;
    part1();
    cout << "\tPart one finished." << endl;

    cout << "Starting part two." << endl;
    part2();
    cout << "\tPart two finished." << endl;

    cout << "Starting part three." << endl;
    part3();
    cout << "\tPart three finished." << endl;

    cout << "Starting part four." << endl;
    part4();
    cout << "\tPart four finished." << endl;

    cout << "Starting part five." << endl;
    part5();
    cout << "\tPart five finished." << endl;
}

