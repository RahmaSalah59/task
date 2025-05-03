#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

// تعريف الدوال المستخدمة
void processFCFS(int req_array[], int req_count, int start_pos, int disk_size);
void processSCAN(int req_array[], int req_count, int start_pos, int disk_size, int move_dir);
void processCSCAN(int req_array[], int req_count, int start_pos, int disk_size, int move_dir);
void bubbleSort(int data[], int elements);
int getHeadDirection();

int main() {
    int disk_size = 5000;  // إجمالي عدد الاسطوانات (0-4999)
    int start_pos, req_count, algo_selection, move_dir;
    int *req_array;
    
    printf("Enter the number of requests (1-1000): ");
    scanf("%d", &req_count);
    
    if (req_count < 1 || req_count > 1000) {
        printf("Invalid number of requests. Please enter between 1 and 1000.\n");
        return 1;
    }
    
    req_array = (int *)malloc(req_count * sizeof(int));
    if (req_array == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }
    
    printf("Enter the request queue (%d numbers between 0-4999):\n", req_count);
    for (int i = 0; i < req_count; i++) {
        scanf("%d", &req_array[i]);
        if (req_array[i] < 0 || req_array[i] >= disk_size) {
            printf("Invalid cylinder number. Must be between 0-4999.\n");
            free(req_array);
            return 1;
        }
    }
    
    printf("Enter the initial head position (0-4999): ");
    scanf("%d", &start_pos);
    if (start_pos < 0 || start_pos >= disk_size) {
        printf("Invalid head position. Must be between 0-4999.\n");
        free(req_array);
        return 1;
    }
    
    printf("\nSelect disk scheduling algorithm:\n");
    printf("1. FCFS\n");
    printf("2. SCAN\n");
    printf("3. C-SCAN\n");
    printf("Enter your choice (1-3): ");
    scanf("%d", &algo_selection);
    
    if (algo_selection < 1 || algo_selection > 3) {
        printf("Invalid algorithm choice.\n");
        free(req_array);
        return 1;
    }
    
    if (algo_selection == 2 || algo_selection == 3) {
        move_dir = getHeadDirection();
    }
    
    printf("\nResults:\n");
    printf("Request queue: ");
    for (int i = 0; i < req_count; i++) {
        printf("%d ", req_array[i]);
    }
    printf("\nInitial head position: %d\n", start_pos);
    
    switch (algo_selection) {
        case 1:
            printf("Algorithm: FCFS\n");
            processFCFS(req_array, req_count, start_pos, disk_size);
            break;
        case 2:
            printf("Algorithm: SCAN\n");
            printf("Direction: %s\n", move_dir == 1 ? "Right (toward higher cylinders)" : "Left (toward lower cylinders)");
            processSCAN(req_array, req_count, start_pos, disk_size, move_dir);
            break;
        case 3:
            printf("Algorithm: C-SCAN\n");
            printf("Direction: %s\n", move_dir == 1 ? "Right (toward higher cylinders)" : "Left (toward lower cylinders)");
            processCSCAN(req_array, req_count, start_pos, disk_size, move_dir);
            break;
    }
    
    free(req_array);
    return 0;
}

int getHeadDirection() {
    int dir_choice;
    printf("Select head movement direction:\n");
    printf("1. Right (toward higher cylinders)\n");
    printf("2. Left (toward lower cylinders)\n");
    printf("Enter your choice (1-2): ");
    scanf("%d", &dir_choice);
    
    if (dir_choice != 1 && dir_choice != 2) {
        printf("Invalid direction choice. Defaulting to Right (1).\n");
        return 1;
    }
    
    return dir_choice;
}

void processFCFS(int req_array[], int req_count, int start_pos, int disk_size) {
    int total_seeks = 0;
    int current_pos = start_pos;
    
    printf("\nOrder of service:\n%d ", current_pos);
    for (int i = 0; i < req_count; i++) {
        total_seeks += abs(current_pos - req_array[i]);
        current_pos = req_array[i];
        printf("%d ", current_pos);
    }
    
    printf("\nTotal head movement: %d\n", total_seeks);
}

void processSCAN(int req_array[], int req_count, int start_pos, int disk_size, int move_dir) {
    int total_seeks = 0;
    int current_pos = start_pos;
    int *sorted_requests = (int *)malloc((req_count + 1) * sizeof(int));
    
    // Copy and prepare for sorting
    for (int i = 0; i < req_count; i++) {
        sorted_requests[i] = req_array[i];
    }
    sorted_requests[req_count] = start_pos;
    bubbleSort(sorted_requests, req_count + 1);
    
    int pos_index = 0;
    while (pos_index < req_count + 1 && sorted_requests[pos_index] != start_pos) {
        pos_index++;
    }
    
    printf("\nOrder of service:\n%d ", current_pos);
    
    if (move_dir == 1) { // Right direction
        // Process higher cylinders
        for (int i = pos_index + 1; i < req_count + 1; i++) {
            total_seeks += abs(current_pos - sorted_requests[i]);
            current_pos = sorted_requests[i];
            printf("%d ", current_pos);
        }
        
        // Go to end if needed
        if (current_pos != disk_size - 1) {
            total_seeks += abs(current_pos - (disk_size - 1));
            current_pos = disk_size - 1;
            printf("%d ", current_pos);
        }
        
        // Process lower cylinders
        for (int i = pos_index - 1; i >= 0; i--) {
            total_seeks += abs(current_pos - sorted_requests[i]);
            current_pos = sorted_requests[i];
            printf("%d ", current_pos);
        }
    } else { // Left direction
        // Process lower cylinders
        for (int i = pos_index - 1; i >= 0; i--) {
            total_seeks += abs(current_pos - sorted_requests[i]);
            current_pos = sorted_requests[i];
            printf("%d ", current_pos);
        }
        
        // Go to beginning if needed
        if (current_pos != 0) {
            total_seeks += abs(current_pos - 0);
            current_pos = 0;
            printf("%d ", current_pos);
        }
        
        // Process higher cylinders
        for (int i = pos_index + 1; i < req_count + 1; i++) {
            total_seeks += abs(current_pos - sorted_requests[i]);
            current_pos = sorted_requests[i];
            printf("%d ", current_pos);
        }
    }
    
    printf("\nTotal head movement: %d\n", total_seeks);
    free(sorted_requests);
}

void processCSCAN(int req_array[], int req_count, int start_pos, int disk_size, int move_dir) {
    int total_seeks = 0;
    int current_pos = start_pos;
    int *sorted_requests = (int *)malloc((req_count + 1) * sizeof(int));
    
    // Copy and prepare for sorting
    for (int i = 0; i < req_count; i++) {
        sorted_requests[i] = req_array[i];
    }
    sorted_requests[req_count] = start_pos;
    bubbleSort(sorted_requests, req_count + 1);
    
    int pos_index = 0;
    while (pos_index < req_count + 1 && sorted_requests[pos_index] != start_pos) {
        pos_index++;
    }
    
    printf("\nOrder of service:\n%d ", current_pos);
    
    if (move_dir == 1) { // Right direction
        // Process higher cylinders
        for (int i = pos_index + 1; i < req_count + 1; i++) {
            total_seeks += abs(current_pos - sorted_requests[i]);
            current_pos = sorted_requests[i];
            printf("%d ", current_pos);
        }
        
        // Go to end if needed
        if (current_pos != disk_size - 1) {
            total_seeks += abs(current_pos - (disk_size - 1));
            current_pos = disk_size - 1;
            printf("%d ", current_pos);
        }
        
        // Jump to beginning
        current_pos = 0;
        printf("%d ", current_pos);
        
        // Process from beginning to head position
        for (int i = 0; i < pos_index; i++) {
            total_seeks += abs(current_pos - sorted_requests[i]);
            current_pos = sorted_requests[i];
            printf("%d ", current_pos);
        }
    } else { // Left direction
        // Process lower cylinders
        for (int i = pos_index - 1; i >= 0; i--) {
            total_seeks += abs(current_pos - sorted_requests[i]);
            current_pos = sorted_requests[i];
            printf("%d ", current_pos);
        }
        
        // Go to beginning if needed
        if (current_pos != 0) {
            total_seeks += abs(current_pos - 0);
            current_pos = 0;
            printf("%d ", current_pos);
        }
        
        // Jump to end
        current_pos = disk_size - 1;
        printf("%d ", current_pos);
        
        // Process from end to head position
        for (int i = req_count; i > pos_index; i--) {
            total_seeks += abs(current_pos - sorted_requests[i]);
            current_pos = sorted_requests[i];
            printf("%d ", current_pos);
        }
    }
    
    printf("\nTotal head movement: %d\n", total_seeks);
    free(sorted_requests);
}

void bubbleSort(int data[], int elements) {
    for (int i = 0; i < elements - 1; i++) {
        for (int j = 0; j < elements - i - 1; j++) {
            if (data[j] > data[j + 1]) {
                int temp = data[j];
                data[j] = data[j + 1];
                data[j + 1] = temp;
            }
        }
    }
}
