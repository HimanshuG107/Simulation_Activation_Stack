#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PARAMS 10
#define MAX_NAME_LENGTH 50

// Define the structure for an activation record
typedef struct {
    char function_name[MAX_NAME_LENGTH];
    int parameters[MAX_PARAMS];
    int num_params;
    void (*return_address)();
} ActivationRecord;

// Define the structure for a node in the stack
typedef struct Node {
    ActivationRecord record;
    struct Node* next;
} Node;

// Define the stack structure
typedef struct {
    Node* top;
} Stack;

// Initialize the stack
void initStack(Stack* stack) {
    stack->top = NULL;
}

// Push an activation record onto the stack
void push(Stack* stack, ActivationRecord record) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    new_node->record = record;
    new_node->next = stack->top;
    stack->top = new_node;
}

// Pop an activation record from the stack
ActivationRecord pop(Stack* stack) {
    if (stack->top == NULL) {
        printf("Error: Stack is empty.\n");
        exit(EXIT_FAILURE);
    }
    Node* temp = stack->top;
    ActivationRecord record = temp->record;
    stack->top = stack->top->next;
    free(temp);
    return record;
}

// Simulate a function call
void callFunction(Stack* stack, const char* function_name, int* parameters, int num_params, void (*return_address)()) {
    ActivationRecord record;
    strncpy(record.function_name, function_name, MAX_NAME_LENGTH - 1);
    record.function_name[MAX_NAME_LENGTH - 1] = '\0';
    record.num_params = num_params;
    memcpy(record.parameters, parameters, num_params * sizeof(int));
    record.return_address = return_address;
    push(stack, record);
}

// Function to print the stack state
void printStack(const Stack* stack) {
    printf("Stack state:\n");
    Node* current = stack->top;
    while (current != NULL) {
        printf("Function: %s, Parameters: [", current->record.function_name);
        for (int i = 0; i < current->record.num_params; i++) {
            printf("%d%s", current->record.parameters[i], i == current->record.num_params - 1 ? "" : ", ");
        }
        printf("], Return Address: %p\n", current->record.return_address);
        current = current->next;
    }
}

// Recursive function to calculate factorial
int factorial(int n, Stack* stack) {
    if (n == 0 || n == 1) {
        callFunction(stack, "factorial", &n, 1, (void (*)())factorial);
        return 1;
    }
    else {
        int result = n * factorial(n - 1, stack);
        callFunction(stack, "factorial", &n, 1, (void (*)())factorial);
        return result;
    }
}

// Function to compute the sum of an array
int sumArray(int arr[], int size, Stack* stack) {
    int sum = 0;
    for (int i = 0; i < size; i++) {
        sum += arr[i];
    }
    callFunction(stack, "sumArray", arr, size, (void (*)())sumArray);
    return sum;
}

// Function to find the maximum number in an array
int maxInArray(int arr[], int size, Stack* stack) {
    int max = arr[0];
    for (int i = 1; i < size; i++) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }
    callFunction(stack, "maxInArray", arr, size, (void (*)())maxInArray);
    return max;
}

int main() {
    Stack stack;
    initStack(&stack);

    // Test cases
    int factorial_result = factorial(5, &stack);
    printf("Factorial of 5: %d\n", factorial_result);

    int array[] = {3, 7, 2, 9, 5};
    int sum = sumArray(array, 5, &stack);
    printf("Sum of array: %d\n", sum);

    int max = maxInArray(array, 5, &stack);
    printf("Max in array: %d\n", max);

    // Print the stack state after all function calls
    printStack(&stack);

    return 0;
}
