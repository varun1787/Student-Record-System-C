#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Student {
    int roll;
    char name[50];
    float marks;
    struct Student *next;
};

struct Student *head = NULL;

void addStudent() {
    struct Student *newNode = (struct Student *)malloc(sizeof(struct Student));
    
    printf("\nEnter Roll Number: ");
    scanf("%d", &newNode->roll);
    
    printf("Enter Name: ");
    scanf(" %[^\n]", newNode->name);
    
    printf("Enter Marks: ");
    scanf("%f", &newNode->marks);
    
    newNode->next = head;
    head = newNode;
    
    printf("Student added successfully!\n");
}

void displayStudents() {
    struct Student *temp = head;
    
    if (temp == NULL) {
        printf("\nNo records found!\n");
        return;
    }
    
    printf("\n--- Student Records ---\n");
    while (temp != NULL) {
        printf("Roll No: %d | Name: %s | Marks: %.2f\n", temp->roll, temp->name, temp->marks);
        temp = temp->next;
    }
}

void searchStudent() {
    int roll;
    struct Student *temp = head;
    
    printf("\nEnter Roll Number to search: ");
    scanf("%d", &roll);
    
    while (temp != NULL) {
        if (temp->roll == roll) {
            printf("\nRecord Found:\n");
            printf("Roll No: %d\n", temp->roll);
            printf("Name: %s\n", temp->name);
            printf("Marks: %.2f\n", temp->marks);
            return;
        }
        temp = temp->next;
    }
    printf("\nStudent not found!\n");
}

void deleteStudent() {
    int roll;
    struct Student *temp = head;
    struct Student *prev = NULL;
    
    printf("\nEnter Roll Number to delete: ");
    scanf("%d", &roll);
    
    if (temp != NULL && temp->roll == roll) {
        head = temp->next;
        free(temp);
        printf("Record deleted successfully!\n");
        return;
    }
    
    while (temp != NULL && temp->roll != roll) {
        prev = temp;
        temp = temp->next;
    }
    
    if (temp == NULL) {
        printf("Record not found!\n");
        return;
    }
    
    prev->next = temp->next;
    free(temp);
    printf("Record deleted successfully!\n");
}

void saveToFile() {
    FILE *fp = fopen("students.txt", "w");
    struct Student *temp = head;
    
    if (fp == NULL) {
        return;
    }
    
    while (temp != NULL) {
        fprintf(fp, "%d %s %f\n", temp->roll, temp->name, temp->marks);
        temp = temp->next;
    }
    fclose(fp);
}

void loadFromFile() {
    FILE *fp = fopen("students.txt", "r");
    if (fp == NULL) {
        return;
    }
    
    while (1) {
        struct Student *newNode = (struct Student *)malloc(sizeof(struct Student));
        if (fscanf(fp, "%d %s %f", &newNode->roll, newNode->name, &newNode->marks) != 3) {
            free(newNode);
            break;
        }
        newNode->next = head;
        head = newNode;
    }
    fclose(fp);
}

int main() {
    int choice;
    loadFromFile();
    
    while (1) {
        printf("\n*** Student Record Menu ***\n");
        printf("1. Add Student\n");
        printf("2. Display Students\n");
        printf("3. Search Student\n");
        printf("4. Delete Student\n");
        printf("5. Save and Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                addStudent();
                break;
            case 2:
                displayStudents();
                break;
            case 3:
                searchStudent();
                break;
            case 4:
                deleteStudent();
                break;
            case 5:
                saveToFile();
                printf("Saved. Exiting...\n");
                return 0;
            default:
                printf("Invalid choice! Try again.\n");
        }
    }
    return 0;
}
