#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

// Define constants for maximum lengths
#define MAX_PATH_LENGTH 260
#define MAX_SEARCH_STR 100
#define MAX_LINE_LENGTH 1024
#define MAX_THREADS 10

// Structure to store search results
typedef struct Result {
    char line[MAX_LINE_LENGTH];
    struct Result *next;
} Result;

// Structure to pass data to threads
typedef struct {
    char filePath[MAX_PATH_LENGTH];
    char searchString[MAX_SEARCH_STR];
} ThreadData;

// Critical section for thread-safe operations on the linked list
CRITICAL_SECTION cs;
Result *head = NULL; // Head of the linked list for storing results
volatile long occurrences = 0; // Counter for the number of occurrences found

// Function to add a line to the linked list
void addToList(char *line) {
    EnterCriticalSection(&cs);
    Result *newResult = (Result *)malloc(sizeof(Result));
    strcpy(newResult->line, line);
    newResult->next = head;
    head = newResult;
    LeaveCriticalSection(&cs);
}

// Thread function to search within a file
DWORD WINAPI searchInFile(LPVOID lpParam) {
    ThreadData *data = (ThreadData *)lpParam;
    FILE *file = fopen(data->filePath, "r");
    char line[MAX_LINE_LENGTH];

    if (file == NULL) {
        perror("Error opening file");
        free(data);
        return 1;
    }

    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        if (strstr(line, data->searchString) != NULL) {
            addToList(line);
            long count = InterlockedIncrement(&occurrences);
            printf("Occurrence %ld found in file: %s\n", count, data->filePath);
        }
    }

    fclose(file);
    free(data);
    return 0;
}

// Function to recursively search through directories
void searchDirectory(const char *folderPath, const char *searchString) {
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    char dirPath[MAX_PATH_LENGTH];
    char filePath[MAX_PATH_LENGTH];
    HANDLE threads[MAX_THREADS];
    int threadCount = 0;

    snprintf(dirPath, sizeof(dirPath), "%s\\*", folderPath);

    hFind = FindFirstFile(dirPath, &findFileData);
    if (hFind == INVALID_HANDLE_VALUE) {
        printf("FindFirstFile failed (%ld)\n", (long)GetLastError());
        return;
    } 

    do {
        if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            // If it's a directory, recursively search it
            if (strcmp(findFileData.cFileName, ".") != 0 && strcmp(findFileData.cFileName, "..") != 0) {
                snprintf(filePath, sizeof(filePath), "%s\\%s", folderPath, findFileData.cFileName);
                searchDirectory(filePath, searchString);
            }
        } else {
            // If it's a file, create a thread to search it
            snprintf(filePath, sizeof(filePath), "%s\\%s", folderPath, findFileData.cFileName);
            ThreadData *data = (ThreadData *)malloc(sizeof(ThreadData));
            strcpy(data->filePath, filePath);
            strcpy(data->searchString, searchString);

            threads[threadCount] = CreateThread(NULL, 0, searchInFile, data, 0, NULL);
            if (threads[threadCount] == NULL) {
                printf("Error creating thread\n");
                free(data);
                continue;
            }

            threadCount++;
            // Wait for threads to finish if max threads are reached
            if (threadCount >= MAX_THREADS) {
                WaitForMultipleObjects(MAX_THREADS, threads, TRUE, INFINITE);
                for (int i = 0; i < MAX_THREADS; i++) {
                    CloseHandle(threads[i]);
                }
                threadCount = 0;
            }
        }
    } while (FindNextFile(hFind, &findFileData) != 0);

    // Wait for any remaining threads to finish
    if (threadCount > 0) {
        WaitForMultipleObjects(threadCount, threads, TRUE, INFINITE);
        for (int i = 0; i < threadCount; i++) {
            CloseHandle(threads[i]);
        }
    }

    FindClose(hFind);
}

// Function to remove duplicate lines from the linked list
void removeDuplicates() {
    Result *current, *temp, *prev;
    EnterCriticalSection(&cs);
    for (current = head; current != NULL; current = current->next) {
        prev = current;
        for (temp = current->next; temp != NULL; ) {
            if (strcmp(current->line, temp->line) == 0) {
                // Duplicate found, remove it
                prev->next = temp->next;
                Result *duplicate = temp;
                temp = temp->next;
                free(duplicate);
            } else {
                prev = temp;
                temp = temp->next;
            }
        }
    }
    LeaveCriticalSection(&cs);
}

// Function to split results and write them to a CSV file
void splitAndWriteResultsToCSV(const char *filename) {
    FILE *csvFile = fopen(filename, "w");
    if (csvFile == NULL) {
        perror("Error opening CSV file");
        return;
    }

    Result *current = head;
    while (current != NULL) {
        char *token;
        const char delim[2] = ":";
        token = strtok(current->line, delim);
        if (token != NULL) {
            fprintf(csvFile, "%s,", token); // Write the part before the colon
            token = strtok(NULL, delim);
            if (token != NULL) {
                fprintf(csvFile, "%s\n", token); // Write the part after the colon
            }
        }
        current = current->next;
    }

    fclose(csvFile);
}

// Function to free the linked list
void freeList() {
    Result *current = head;
    while (current != NULL) {
        Result *temp = current;
        current = current->next;
        free(temp);
    }
}

int main() {
    InitializeCriticalSection(&cs);

    // ASCII art for the program header
    const char *asciiArt =
        " ____ ____ ____ ____ ____ ____ ____ _________ ____ ____ ____ ____ ____ ____ \n"
        "||S |||C |||U |||F |||F |||E |||D |||       |||S |||E |||A |||R |||C |||H ||\n"
        "||__|||__|||__|||__|||__|||__|||__|||_______|||__|||__|||__|||__|||__|||__||\n"
        "|/__\\|/__\\|/__\\|/__\\|/__\\|/__\\|/__\\|/_______\\|/__\\|/__\\|/__\\|/__\\|/__\\|/__\\|\n"
        "                                                                            \n";
    printf("%s", asciiArt);

    char folderPath[MAX_PATH_LENGTH];
    char searchString[MAX_SEARCH_STR];

    printf("Enter the folder path: ");
    fgets(folderPath, MAX_PATH_LENGTH, stdin);
    folderPath[strcspn(folderPath, "\n")] = 0;

    printf("Enter the search string: ");
    fgets(searchString, MAX_SEARCH_STR, stdin);
    searchString[strcspn(searchString, "\n")] = 0;

    searchDirectory(folderPath, searchString);

    printf("Processing complete. Removing duplicates...\n");
    removeDuplicates();

    printf("Splitting data and writing results to CSV...\n");
    splitAndWriteResultsToCSV("output.csv");

    freeList();
    DeleteCriticalSection(&cs);

    return 0;
}
