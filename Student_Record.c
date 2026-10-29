#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define FILE_NAME "students.txt"

struct Student {
    int roll;
    char name[50];
    float percentage;
    struct Student *next;
};

struct Student *head = NULL;

// Function Prototypes
void addStudentRecord();
void deleteStudentRecord();
void modifyStudentRecord();
void showAllStudents();
void sortStudentRecords();
void saveRecords();
void exitProgram();
void loadRecords();

// Helper to count nodes
int getCount() {
    int count = 0;
    struct Student *temp = head;
    while (temp != NULL) {
        count++;
        temp = temp->next;
    }
    return count;
}

// [A] Add New Student Record
void addStudentRecord() {
    int n, i;
    printf("\nHow many students do you want to add? ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        while (getchar() != '\n');
        printf("Invalid number!\n");
        return;
    }

    for (i = 0; i < n; i++) {
        struct Student *newNode = (struct Student *)malloc(sizeof(struct Student));
        if (!newNode) {
            printf("Memory allocation failed!\n");
            return;
        }

        printf("\n--- Student %d ---\n", i + 1);
        printf("Enter Roll Number: ");
        scanf("%d", &newNode->roll);

        // Check if Roll Number already exists
        struct Student *check = head;
        int duplicate = 0;
        while (check != NULL) {
            if (check->roll == newNode->roll) {
                duplicate = 1;
                break;
            }
            check = check->next;
        }

        if (duplicate) {
            printf("Error: Roll Number %d already exists! Skipping.\n", newNode->roll);
            free(newNode);
            continue;
        }

        printf("Enter Name: ");
        scanf(" %[^\n]", newNode->name);

        printf("Enter Percentage: ");
        scanf("%f", &newNode->percentage);

        newNode->next = head;
        head = newNode;

        printf("Student added successfully!\n");
    }
}

// [S] Show All Student Records
void showAllStudents() {
    struct Student *temp = head;
    if (temp == NULL) {
        printf("\nNo student records available.\n");
        return;
    }

    printf("\n-------------------------------------------------------------\n");
    printf("%-10s %-30s %-15s\n", "Roll No", "Name", "Percentage");
    printf("-------------------------------------------------------------\n");
    while (temp != NULL) {
        printf("%-10d %-30s %-15.2f\n", temp->roll, temp->name, temp->percentage);
        temp = temp->next;
    }
    printf("-------------------------------------------------------------\n");
}

// [M] Modify Student Record (Sub-menu: R, N, B)
void modifyStudentRecord() {
    if (head == NULL) {
        printf("\nNo records available to modify.\n");
        return;
    }

    char subChoice;
    printf("\nModify Menu:\n");
    printf("  [R/r] : Search by Roll\n");
    printf("  [N/n] : Search by Student Name\n");
    printf("  [B/b] : Back to Menu\n");
    printf("Enter your choice: ");
    scanf(" %c", &subChoice);
    subChoice = toupper(subChoice);

    struct Student *target = NULL;

    if (subChoice == 'R') {
        int r;
        printf("Enter Roll Number to search: ");
        scanf("%d", &r);

        struct Student *temp = head;
        while (temp != NULL) {
            if (temp->roll == r) {
                target = temp;
                break;
            }
            temp = temp->next;
        }
    } else if (subChoice == 'N') {
        char searchName[50];
        printf("Enter Name to search: ");
        scanf(" %[^\n]", searchName);

        // Count matching names
        int matches = 0;
        struct Student *temp = head;
        while (temp != NULL) {
            if (strcasecmp(temp->name, searchName) == 0) {
                matches++;
            }
            temp = temp->next;
        }

        if (matches == 0) {
            printf("No student found with name: %s\n", searchName);
            return;
        } else if (matches == 1) {
            temp = head;
            while (temp != NULL) {
                if (strcasecmp(temp->name, searchName) == 0) {
                    target = temp;
                    break;
                }
                temp = temp->next;
            }
        } else {
            // More than one student has the same name -> ask confirmation by Roll Number
            printf("\nMultiple students found with the name '%s':\n", searchName);
            temp = head;
            while (temp != NULL) {
                if (strcasecmp(temp->name, searchName) == 0) {
                    printf("Roll No: %d | Name: %s | Percentage: %.2f\n", temp->roll, temp->name, temp->percentage);
                }
                temp = temp->next;
            }

            int confirmRoll;
            printf("\nPlease confirm student by Roll Number: ");
            scanf("%d", &confirmRoll);

            temp = head;
            while (temp != NULL) {
                if (temp->roll == confirmRoll && strcasecmp(temp->name, searchName) == 0) {
                    target = temp;
                    break;
                }
                temp = temp->next;
            }
        }
    } else if (subChoice == 'B') {
        return;
    } else {
        printf("Invalid choice!\n");
        return;
    }

    if (target == NULL) {
        printf("Student record not found.\n");
        return;
    }

    printf("\nCurrent Details -> Roll No: %d, Name: %s, Percentage: %.2f\n",
           target->roll, target->name, target->percentage);

    printf("Enter New Name: ");
    scanf(" %[^\n]", target->name);

    printf("Enter New Percentage: ");
    scanf("%f", &target->percentage);

    printf("Record modified successfully!\n");
}

// [D] Delete Student Record (with confirmation)
void deleteStudentRecord() {
    if (head == NULL) {
        printf("\nNo records available to delete.\n");
        return;
    }

    int r;
    printf("\nEnter Roll Number to delete: ");
    scanf("%d", &r);

    struct Student *temp = head;
    struct Student *prev = NULL;

    while (temp != NULL && temp->roll != r) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        printf("Student with Roll Number %d not found!\n", r);
        return;
    }

    // Ask confirmation before deleting
    char confirm;
    printf("Are you sure you want to delete Roll No: %d (%s)? [Y/N]: ", temp->roll, temp->name);
    scanf(" %c", &confirm);

    if (toupper(confirm) != 'Y') {
        printf("Deletion cancelled.\n");
        return;
    }

    if (prev == NULL) {
        head = temp->next;
    } else {
        prev->next = temp->next;
    }

    free(temp);
    printf("Record deleted successfully!\n");
}

// [T] Sort Student Records (Print only, does not modify main linked list)
void sortStudentRecords() {
    int count = getCount();
    if (count == 0) {
        printf("\nNo records to sort.\n");
        return;
    }

    // Copy pointers into a temporary array so the original list remains untouched
    struct Student **arr = (struct Student **)malloc(count * sizeof(struct Student *));
    struct Student *temp = head;
    for (int i = 0; i < count; i++) {
        arr[i] = temp;
        temp = temp->next;
    }

    int sortChoice;
    printf("\nSort Options (Print Only):\n");
    printf("  1. Sort by Name\n");
    printf("  2. Sort by Percentage\n");
    printf("Enter option (1 or 2): ");
    scanf("%d", &sortChoice);

    // Bubble sort on the array of pointers
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            int swapNeeded = 0;
            if (sortChoice == 1) {
                if (strcasecmp(arr[j]->name, arr[j + 1]->name) > 0) {
                    swapNeeded = 1;
                }
            } else if (sortChoice == 2) {
                if (arr[j]->percentage < arr[j + 1]->percentage) { // Highest to lowest
                    swapNeeded = 1;
                }
            }
            if (swapNeeded) {
                struct Student *swapTemp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = swapTemp;
            }
        }
    }

    // Print sorted output
    printf("\n------------------- SORTED RESULTS -------------------\n");
    printf("%-10s %-30s %-15s\n", "Roll No", "Name", "Percentage");
    printf("------------------------------------------------------\n");
    for (int i = 0; i < count; i++) {
        printf("%-10d %-30s %-15.2f\n", arr[i]->roll, arr[i]->name, arr[i]->percentage);
    }
    printf("------------------------------------------------------\n");

    free(arr); // Free the temporary pointer array
}

// [V] Save Records
void saveRecords() {
    FILE *fp = fopen(FILE_NAME, "w");
    if (fp == NULL) {
        printf("Error opening file for writing!\n");
        return;
    }

    struct Student *temp = head;
    while (temp != NULL) {
        fprintf(fp, "%d %s %f\n", temp->roll, temp->name, temp->percentage);
        temp = temp->next;
    }

    fclose(fp);
    printf("Records saved successfully to %s!\n", FILE_NAME);
}

// Load Records at startup
void loadRecords() {
    FILE *fp = fopen(FILE_NAME, "r");
    if (fp == NULL) {
        return;
    }

    while (1) {
        struct Student *newNode = (struct Student *)malloc(sizeof(struct Student));
        if (fscanf(fp, "%d %s %f", &newNode->roll, newNode->name, &newNode->percentage) != 3) {
            free(newNode);
            break;
        }
        newNode->next = head;
        head = newNode;
    }
    fclose(fp);
}

// [E] Exit Program with confirmation
void exitProgram() {
    char exitChoice;
    while (1) {
        printf("\nExit Confirmation Menu:\n");
        printf("  [S/s] : Save & Exit\n");
        printf("  [E/e] : Exit Without Saving\n");
        printf("  [B/b] : Back to Menu\n");
        printf("Enter your choice: ");
        scanf(" %c", &exitChoice);
        exitChoice = toupper(exitChoice);

        if (exitChoice == 'S') {
            saveRecords();
            printf("Exiting program...\n");
            exit(0);
        } else if (exitChoice == 'E') {
            printf("Exiting without saving changes...\n");
            exit(0);
        } else if (exitChoice == 'B') {
            return;
        } else {
            printf("Invalid choice! Choose S, E, or B.\n");
        }
    }
}

int main() {
    char choice;
    loadRecords();

    while (1) {
        printf("\n=========================================\n");
        printf("       Student Record Management         \n");
        printf("=========================================\n");
        printf("  [A/a] : Add New Student Record\n");
        printf("  [D/d] : Delete Student Record\n");
        printf("  [M/m] : Modify Student Record\n");
        printf("  [S/s] : Show All Student Records\n");
        printf("  [T/t] : Sort Student Records\n");
        printf("  [V/v] : Save Records\n");
        printf("  [E/e] : Exit Program\n");
        printf("-----------------------------------------\n");
        printf("Enter choice: ");

        if (scanf(" %c", &choice) != 1) {
            while (getchar() != '\n');
            continue;
        }
        choice = toupper(choice);

        switch (choice) {
            case 'A':
                addStudentRecord();
                break;
            case 'D':
                deleteStudentRecord();
                break;
            case 'M':
                modifyStudentRecord();
                break;
            case 'S':
                showAllStudents();
                break;
            case 'T':
                sortStudentRecords();
                break;
            case 'V':
                saveRecords();
                break;
            case 'E':
                exitProgram();
                break;
            default:
                printf("Invalid selection! Please enter A, D, M, S, T, V, or E.\n");
        }
    }
    return 0;
}
