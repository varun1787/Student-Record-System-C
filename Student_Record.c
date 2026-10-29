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

int get_smallest_roll() {
    int r = 1;
    while (1) {
        int found = 0;
        struct Student *temp = head;
        while (temp != NULL) {
            if (temp->roll == r) {
                found = 1;
                break;
            }
            temp = temp->next;
        }
        if (found == 0) {
            return r;
        }
        r++;
    }
}

void stud_add() {
    int n, i;
    printf("\nHow many students to add: ");
    scanf("%d", &n);

    for (i = 0; i < n; i++) {
        struct Student *newNode = (struct Student *)malloc(sizeof(struct Student));

        newNode->roll = get_smallest_roll();
        printf("\n--- Student %d ---\n", i + 1);
        printf("Auto-assigned Roll Number: %d\n", newNode->roll);

        printf("Enter Student Name: ");
        scanf(" %[^\n]", newNode->name);

        printf("Enter Percentage: ");
        scanf("%f", &newNode->percentage);

        newNode->next = head;
        head = newNode;

        printf("Record added successfully!\n");
    }
}

void delete_node(int r) {
    struct Student *temp = head;
    struct Student *prev = NULL;

    while (temp != NULL && temp->roll != r) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        printf("Roll number %d not found!\n", r);
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

void stud_del() {
    char ch;
    if (head == NULL) {
        printf("\nNo records to delete!\n");
        return;
    }

    printf("\nDelete Menu:\n");
    printf("R/r : Delete using Roll Number\n");
    printf("N/n : Delete using Name\n");
    printf("Enter Your Choice: ");
    scanf(" %c", &ch);

    if (ch == 'R' || ch == 'r') {
        int r;
        printf("Enter Roll Number: ");
        scanf("%d", &r);
        delete_node(r);
    } else if (ch == 'N' || ch == 'n') {
        char name[50];
        int count = 0;
        printf("Enter Name: ");
        scanf(" %[^\n]", name);

        struct Student *temp = head;
        printf("\nMatching Records:\n");
        while (temp != NULL) {
            if (strcmp(temp->name, name) == 0) {
                printf("Roll: %d | Name: %s | Percentage: %.2f\n", temp->roll, temp->name, temp->percentage);
                count++;
            }
            temp = temp->next;
        }

        if (count == 0) {
            printf("No records found with name %s!\n", name);
            return;
        }

        int r;
        printf("Enter Roll Number to delete: ");
        scanf("%d", &r);
        delete_node(r);
    } else {
        printf("Invalid choice!\n");
    }
}

void stud_show() {
    struct Student *temp = head;

    if (temp == NULL) {
        printf("\nNo records to show!\n");
        return;
    }

    printf("\nRoll No\tName\t\tPercentage\n");
    printf("--------------------------------------\n");
    while (temp != NULL) {
        printf("%d\t%s\t\t%.2f\n", temp->roll, temp->name, temp->percentage);
        temp = temp->next;
    }
}

void stud_mod() {
    char ch, field;
    struct Student *temp = head;
    struct Student *target = NULL;

    if (head == NULL) {
        printf("\nNo records to modify!\n");
        return;
    }

    printf("\nStep 1: Search Record\n");
    printf("R/r : Roll Number\n");
    printf("N/n : Name\n");
    printf("P/p : Percentage\n");
    printf("Enter Your Choice: ");
    scanf(" %c", &ch);

    if (ch == 'R' || ch == 'r') {
        int r;
        printf("Enter Roll Number: ");
        scanf("%d", &r);
        while (temp != NULL) {
            if (temp->roll == r) {
                target = temp;
                break;
            }
            temp = temp->next;
        }
    } else if (ch == 'N' || ch == 'n') {
        char name[50];
        int count = 0;
        printf("Enter Name: ");
        scanf(" %[^\n]", name);

        temp = head;
        printf("\nMatching Records:\n");
        while (temp != NULL) {
            if (strcmp(temp->name, name) == 0) {
                printf("Roll: %d | Name: %s | Percentage: %.2f\n", temp->roll, temp->name, temp->percentage);
                count++;
            }
            temp = temp->next;
        }

        if (count > 0) {
            int r;
            printf("Enter Roll Number to modify: ");
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
    } else if (ch == 'P' || ch == 'p') {
        float p;
        int count = 0;
        printf("Enter Percentage: ");
        scanf("%f", &p);

        temp = head;
        printf("\nMatching Records:\n");
        while (temp != NULL) {
            if (temp->percentage == p) {
                printf("Roll: %d | Name: %s | Percentage: %.2f\n", temp->roll, temp->name, temp->percentage);
                count++;
            }
            temp = temp->next;
        }

        if (count > 0) {
            int r;
            printf("Enter Roll Number to modify: ");
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
    }

    if (target == NULL) {
        printf("Record not found!\n");
        return;
    }

    printf("\nStep 2: Modify Field\n");
    printf("N/n : Name\n");
    printf("P/p : Percentage\n");
    printf("Enter Your Choice: ");
    scanf(" %c", &field);

    if (field == 'N' || field == 'n') {
        printf("Enter New Name: ");
        scanf(" %[^\n]", target->name);
        printf("Name modified successfully!\n");
    } else if (field == 'P' || field == 'p') {
        printf("Enter New Percentage: ");
        scanf("%f", &target->percentage);
        printf("Percentage modified successfully!\n");
    } else {
        printf("Invalid choice!\n");
    }
}

void stud_save() {
    FILE *fp = fopen("student.dat", "w");
    struct Student *temp = head;

    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    while (temp != NULL) {
        fprintf(fp, "%d %s %f\n", temp->roll, temp->name, temp->percentage);
        temp = temp->next;
    }
    fclose(fp);
    printf("Records saved to student.dat successfully!\n");
}

void load_records() {
    FILE *fp = fopen("student.dat", "r");
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

void stud_sort() {
    char ch;
    struct Student *i, *j;

    if (head == NULL) {
        printf("\nNo records to sort!\n");
        return;
    }

    printf("\nSort Menu:\n");
    printf("N/n : Sort by Name\n");
    printf("P/p : Sort by Percentage\n");
    printf("Enter Your Choice: ");
    scanf(" %c", &ch);

    for (i = head; i != NULL; i = i->next) {
        for (j = i->next; j != NULL; j = j->next) {
            int swap = 0;
            if (ch == 'N' || ch == 'n') {
                if (strcmp(i->name, j->name) > 0) {
                    swap = 1;
                }
            } else if (ch == 'P' || ch == 'p') {
                if (i->percentage < j->percentage) {
                    swap = 1;
                }
            }

            if (swap == 1) {
                int tr = i->roll;
                char tn[50];
                float tp = i->percentage;
                strcpy(tn, i->name);

                i->roll = j->roll;
                strcpy(i->name, j->name);
                i->percentage = j->percentage;

                j->roll = tr;
                strcpy(j->name, tn);
                j->percentage = tp;
            }
        }
    }
    printf("List sorted successfully!\n");
    stud_show();
}

void exit_program() {
    char ch;
    while (1) {
        printf("\nExit Submenu:\n");
        printf("S/s : Save and Exit\n");
        printf("E/e : Exit Without Saving\n");
        printf("Enter Your Choice: ");
        scanf(" %c", &ch);

        if (ch == 'S' || ch == 's') {
            stud_save();
            exit(0);
        } else if (ch == 'E' || ch == 'e') {
            exit(0);
        } else {
            printf("Invalid choice!\n");
        }
    }
}

int main() {
    char choice;
    load_records();

    while (1) {
        printf("\n**** STUDENT RECORD MENU ****\n");
        printf("A/a : Add New Record\n");
        printf("D/d : Delete a Record\n");
        printf("S/s : Show the List\n");
        printf("M/m : Modify a Record\n");
        printf("V/v : Save\n");
        printf("T/t : Sort the List\n");
        printf("E/e : Exit\n");
        printf("Enter Your Choice: ");
        scanf(" %c", &choice);

        switch (choice) {
            case 'A':
            case 'a':
                stud_add();
                break;
            case 'D':
            case 'd':
                stud_del();
                break;
            case 'S':
            case 's':
                stud_show();
                break;
            case 'M':
            case 'm':
                stud_mod();
                break;
            case 'V':
            case 'v':
                stud_save();
                break;
            case 'T':
            case 't':
                stud_sort();
                break;
            case 'E':
            case 'e':
                exit_program();
                break;
            default:
                printf("Invalid Choice!\n");
        }
    }
    return 0;
}
