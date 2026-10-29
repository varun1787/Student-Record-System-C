#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Student {
    int roll;
    char name[50];
    float percentage;
    struct Student *next;
};

struct Student *head = NULL;

void addStudentRecord() {
    int n, i;
    printf("\nHow many students to add: ");
    scanf("%d", &n);

    for (i = 0; i < n; i++) {
        struct Student *newNode = (struct Student *)malloc(sizeof(struct Student));
        
        printf("\nStudent %d\n", i + 1);
        printf("Enter Roll: ");
        scanf("%d", &newNode->roll);

        printf("Enter Name: ");
        scanf(" %[^\n]", newNode->name);

        printf("Enter Percentage: ");
        scanf("%f", &newNode->percentage);

        newNode->next = head;
        head = newNode;

        printf("Added successfully!\n");
    }
}

void showAllStudents() {
    struct Student *temp = head;
    if (temp == NULL) {
        printf("\nNo records found!\n");
        return;
    }

    printf("\n--- All Students ---\n");
    while (temp != NULL) {
        printf("Roll: %d | Name: %s | Percentage: %.2f\n", temp->roll, temp->name, temp->percentage);
        temp = temp->next;
    }
}

void modifyStudentRecord() {
    char ch;
    struct Student *temp = head;
    struct Student *target = NULL;

    if (head == NULL) {
        printf("\nNo records to modify!\n");
        return;
    }

    printf("\n[R] Search by Roll\n");
    printf("[N] Search by Name\n");
    printf("[B] Back to Menu\n");
    printf("Enter choice: ");
    scanf(" %c", &ch);

    if (ch == 'R' || ch == 'r') {
        int r;
        printf("Enter Roll: ");
        scanf("%d", &r);
        while (temp != NULL) {
            if (temp->roll == r) {
                target = temp;
                break;
            }
            temp = temp->next;
        }
    } else if (ch == 'N' || ch == 'n') {
        char sname[50];
        int count = 0;
        printf("Enter Name: ");
        scanf(" %[^\n]", sname);

        temp = head;
        while (temp != NULL) {
            if (strcmp(temp->name, sname) == 0) {
                count++;
            }
            temp = temp->next;
        }

        if (count == 0) {
            printf("Name not found!\n");
            return;
        } else if (count == 1) {
            temp = head;
            while (temp != NULL) {
                if (strcmp(temp->name, sname) == 0) {
                    target = temp;
                    break;
                }
                temp = temp->next;
            }
        } else {
            printf("\nMore than one student found with same name:\n");
            temp = head;
            while (temp != NULL) {
                if (strcmp(temp->name, sname) == 0) {
                    printf("Roll: %d | Name: %s\n", temp->roll, temp->name);
                }
                temp = temp->next;
            }
            int r;
            printf("Confirm by Roll number: ");
            scanf("%d", &r);
            temp = head;
            while (temp != NULL) {
                if (temp->roll == r) {
                    target = temp;
                    break;
                }
                temp = temp->next;
            }
        }
    } else if (ch == 'B' || ch == 'b') {
        return;
    } else {
        printf("Invalid choice!\n");
        return;
    }

    if (target == NULL) {
        printf("Student not found!\n");
        return;
    }

    printf("Enter New Name: ");
    scanf(" %[^\n]", target->name);
    printf("Enter New Percentage: ");
    scanf("%f", &target->percentage);
    printf("Record modified!\n");
}

void deleteStudentRecord() {
    int r;
    char confirm;
    struct Student *temp = head;
    struct Student *prev = NULL;

    if (head == NULL) {
        printf("\nNo records to delete!\n");
        return;
    }

    printf("\nEnter Roll to delete: ");
    scanf("%d", &r);

    while (temp != NULL && temp->roll != r) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        printf("Roll number not found!\n");
        return;
    }

    printf("Are you sure to delete %s? (y/n): ", temp->name);
    scanf(" %c", &confirm);

    if (confirm != 'y' && confirm != 'Y') {
        printf("Deletion cancelled!\n");
        return;
    }

    if (prev == NULL) {
        head = temp->next;
    } else {
        prev->next = temp->next;
    }

    free(temp);
    printf("Deleted successfully!\n");
}

void sortStudentRecords() {
    int count = 0, i, j, opt;
    struct Student *temp = head;

    while (temp != NULL) {
        count++;
        temp = temp->next;
    }

    if (count == 0) {
        printf("\nNo records to sort!\n");
        return;
    }

    struct Student arr[count];
    temp = head;
    for (i = 0; i < count; i++) {
        arr[i] = *temp;
        temp = temp->next;
    }

    printf("\n1. Sort by Name\n");
    printf("2. Sort by Percentage\n");
    printf("Enter option: ");
    scanf("%d", &opt);

    for (i = 0; i < count - 1; i++) {
        for (j = 0; j < count - i - 1; j++) {
            if (opt == 1) {
                if (strcmp(arr[j].name, arr[j + 1].name) > 0) {
                    struct Student t = arr[j];
                    arr[j] = arr[j + 1];
                    arr[j + 1] = t;
                }
            } else if (opt == 2) {
                if (arr[j].percentage < arr[j + 1].percentage) {
                    struct Student t = arr[j];
                    arr[j] = arr[j + 1];
                    arr[j + 1] = t;
                }
            }
        }
    }

    printf("\n--- Sorted List ---\n");
    for (i = 0; i < count; i++) {
        printf("Roll: %d | Name: %s | Percentage: %.2f\n", arr[i].roll, arr[i].name, arr[i].percentage);
    }
}

void saveRecords() {
    FILE *fp = fopen("students.txt", "w");
    struct Student *temp = head;

    if (fp == NULL) {
        return;
    }

    while (temp != NULL) {
        fprintf(fp, "%d %s %f\n", temp->roll, temp->name, temp->percentage);
        temp = temp->next;
    }
    fclose(fp);
    printf("Records saved successfully!\n");
}

void loadRecords() {
    FILE *fp = fopen("students.txt", "r");
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

void exitProgram() {
    char ch;
    while (1) {
        printf("\n[S] Save & Exit\n");
        printf("[E] Exit without Save\n");
        printf("[B] Back to Menu\n");
        printf("Enter choice: ");
        scanf(" %c", &ch);

        if (ch == 'S' || ch == 's') {
            saveRecords();
            exit(0);
        } else if (ch == 'E' || ch == 'e') {
            exit(0);
        } else if (ch == 'B' || ch == 'b') {
            return;
        } else {
            printf("Invalid choice!\n");
        }
    }
}

int main() {
    char choice;
    loadRecords();

    while (1) {
        printf("\n*** Student Record Management ***\n");
        printf("[A] Add New Student Record\n");
        printf("[D] Delete Student Record\n");
        printf("[M] Modify Student Record\n");
        printf("[S] Show All Student Records\n");
        printf("[T] Sort Student Records\n");
        printf("[V] Save Records\n");
        printf("[E] Exit Program\n");
        printf("Enter choice: ");

        scanf(" %c", &choice);

        switch (choice) {
            case 'A':
            case 'a':
                addStudentRecord();
                break;
            case 'D':
            case 'd':
                deleteStudentRecord();
                break;
            case 'M':
            case 'm':
                modifyStudentRecord();
                break;
            case 'S':
            case 's':
                showAllStudents();
                break;
            case 'T':
            case 't':
                sortStudentRecords();
                break;
            case 'V':
            case 'v':
                saveRecords();
                break;
            case 'E':
            case 'e':
                exitProgram();
                break;
            default:
                printf("Invalid choice! Try again.\n");
        }
    }
    return 0;
}
