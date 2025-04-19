// Simple Memory Allocator in C
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MEMORY_SIZE 100

typedef struct {
    int start;
    int size;
    char process[10];
    bool free;
} Block;

Block memory[MEMORY_SIZE];
int blockCount = 0;

void initMemory() {
    memory[0].start = 0;
    memory[0].size = MEMORY_SIZE;
    strcpy(memory[0].process, "");
    memory[0].free = true;
    blockCount = 1;
}

void printMemory() {
    printf("\nMemory State:\n");
    for (int i = 0; i < blockCount; i++) {
        printf("Block %d: Start = %d, Size = %d, %s %s\n",
               i, memory[i].start, memory[i].size,
               memory[i].free ? "Free" : "Used",
               memory[i].free ? "" : memory[i].process);
    }
    printf("\n");
}

void allocate(char *process, int size) {
    for (int i = 0; i < blockCount; i++) {
        if (memory[i].free && memory[i].size >= size) {
            if (memory[i].size > size) {
                for (int j = blockCount; j > i; j--) {
                    memory[j] = memory[j - 1];
                }
                memory[i + 1].start = memory[i].start + size;
                memory[i + 1].size = memory[i].size - size;
                memory[i + 1].free = true;
                strcpy(memory[i + 1].process, "");
                memory[i].size = size;
                blockCount++;
            }
            memory[i].free = false;
            strcpy(memory[i].process, process);
            printf("Allocated %s\n", process);
            return;
        }
    }
    printf("Failed to allocate %s: Not enough space or fragmentation\n", process);
}

void freeMemory(char *process) {
    for (int i = 0; i < blockCount; i++) {
        if (!memory[i].free && strcmp(memory[i].process, process) == 0) {
            memory[i].free = true;
            strcpy(memory[i].process, "");
            printf("Freed %s\n", process);
            return;
        }
    }
    printf("Process %s not found\n", process);
}

void compact() {
    int index = 0;
    for (int i = 0; i < blockCount; i++) {
        if (!memory[i].free) {
            memory[index] = memory[i];
            memory[index].start = index == 0 ? 0 : memory[index - 1].start + memory[index - 1].size;
            index++;
        }
    }
    if (index < blockCount) {
        memory[index].start = index == 0 ? 0 : memory[index - 1].start + memory[index - 1].size;
        memory[index].size = MEMORY_SIZE - memory[index].start;
        memory[index].free = true;
        strcpy(memory[index].process, "");
        blockCount = index + 1;
    }
    printf("Memory compacted.\n");
}

int main() {
    initMemory();
    printMemory();

    allocate("P1", 30);
    allocate("P2", 20);
    allocate("P3", 25);
    printMemory();

    freeMemory("P2");
    printMemory();

    allocate("P4", 22); // Should fail due to fragmentation
    printMemory();

    compact();
    printMemory();

    allocate("P4", 22); // Should succeed after compaction
    printMemory();

    return 0;
}