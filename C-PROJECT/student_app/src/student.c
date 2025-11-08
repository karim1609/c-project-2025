#include "student.h"
#include "config.h"
#include "crypto.h"
#include "validation.h"
#include "ui.h"
#include "file.h"
#include "report.h"
#include "stats.h"
#include "auth.h"
#include "attendance.h"
#include "grade.h"
#include "club.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

StudentList* student_list_create(void) {
    StudentList* list = (StudentList*)malloc(sizeof(StudentList));
    if (list == NULL) {
        printf("Error: Failed to create student list\n");
        return NULL;
    }
    
    // Allocate memory for the students array
    list->students = (Student*)malloc(MAX_STUDENTS * sizeof(Student));
    if (list->students == NULL) {
        printf("Error: Failed to allocate memory for students array\n");
        free(list);
        return NULL;
    }
    
    // Initialize all fields
    list->count = 0;
    list->capacity = MAX_STUDENTS;
    list->is_loaded = 0;
    // Set the first character of the filename to the null terminator,
    // making the filename an empty string to indicate no file is set yet.
    list->filename[0] = '\0';
    list->auto_save_enabled = 1;
    list->last_save_time = 0;
    
    return list;
}

void student_list_destroy(StudentList* list) {
    if (list == NULL) {
        return;
    }
    
    // Free the students array if it was allocated
    if (list->students != NULL) {
        free(list->students);
    }
    
    // Free the list structure itself
    free(list);
}
int student_list_add(StudentList* list, Student student){
    if (list == NULL || list->students == NULL) {
        printf("ERROR DE LISTE OR STUDENT  ");
        return 0;
    }else{
        if (list->count >= list->capacity) {
            printf("Error: Student list is full, cannot add new student.\n");
            return 0;
        }
        list->students[list->count] = student;
        list->count++;
        return 1;
    }

    


}
int student_list_remove(StudentList* list, int student_id) {
    if (list == NULL || list->students == NULL) {
        printf("Error: Invalid student list\n");
        return 0;
    }

    for (int i = 0; i < list->count; i++) {
        if (list->students[i].id == student_id) {
            for (int j = i; j < list->count - 1; j++) {
                list->students[j] = list->students[j + 1];
            }

            memset(&list->students[list->count - 1], 0, sizeof(Student));
            list->count--;
            return 1;
        }
    }

    printf("Error: Student with ID %d not found\n", student_id);
    return 0;
}
Student* student_list_find_by_id(StudentList* list, int student_id) {
    if (list == NULL || list->students == NULL) {
        printf("Error: Invalid student list\n");
        return NULL;
    }
    
    for (int i = 0; i < list->count; i++) {
        if (list->students[i].id == student_id) {
            return &list->students[i];
        }
    }
    
    return NULL;
}
Student* student_list_find_by_name(StudentList* list, const char* first_name, const char* last_name) {
    if (list == NULL || list->students == NULL) {
        printf("Error: Invalid student list\n");
        return NULL;
    }

    if (first_name == NULL || last_name == NULL) {
        printf("Error: Invalid name parameters\n");
        return NULL;
    }

    for (int i = 0; i < list->count; i++) {
        if (strcmp(list->students[i].first_name, first_name) == 0 && 
            strcmp(list->students[i].last_name, last_name) == 0) {
            return &list->students[i];
        }
    }
    
    return NULL;
}
Student* student_list_find_by_email(StudentList* list, const char* email) {
    if (list == NULL || list->students == NULL) {
        printf("Error: Invalid student list\n");
        return NULL;
    }

    if (email == NULL) {
        printf("Error: Invalid email parameter\n");
        return NULL;
    }

    if (!student_validate_email(email)) {
        printf("Error: Invalid email format\n");
        return NULL;
    }

    for (int i = 0; i < list->count; i++) {
        if (list->students[i].email != NULL && strcmp(list->students[i].email, email) == 0) {
            return &list->students[i];
        }
    }

    return NULL;
}
void student_list_display_all(StudentList* list){
    if (list == NULL || list->students == NULL) {
       printf("Error: Invalid student list\n");
       return;
    }
    if(list->count==0){
       printf("LIST IS EMPTY \n");
       return;
    }
    for(int i = 0; i < list->count; i++) {
       printf("\nStudent %d:\n", i + 1);
       printf("ID: %d\n", list->students[i].id);
       printf("First Name: %s\n", list->students[i].first_name);
       printf("Last Name: %s\n", list->students[i].last_name);
       printf("Email: %s\n", list->students[i].email);
       printf("age: %s\n", list->students[i].age);
       printf("Phone: %s\n", list->students[i].phone);
       printf("Address: %s\n", list->students[i].address);
       printf("--------------------\n");
    }
}
void student_list_display_student(Student* student) {
    if (student == NULL) {
        printf("Error: Invalid student\n");
        return;
    }
    
    printf("\nStudent information:\n");
    printf("ID: %d\n", student->id);
    printf("First Name: %s\n", student->first_name);
    printf("Last Name: %s\n", student->last_name);
    printf("Email: %s\n", student->email);
    printf("AGE: %d\n", student->age);
    printf("Phone: %s\n", student->phone);
    printf("Address: %s\n", student->address);
}


int student_list_save_to_file(StudentList* list, const char* filename) {
    if (list == NULL || list->students == NULL || filename == NULL) {
        printf("Error: Invalid arguments to student_list_save_to_file\n");
        return 0;
    }
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Error: Could not open file %s for writing\n", filename);
        return 0;
    }
    // Save students as CSV (or adjust fields as necessary)
    for (int i = 0; i < list->count; i++) {
        Student* s = &list->students[i];
        fprintf(file, "%d,%s,%s,%s,%s,%s,%d,%s,%d,%.2f,%ld,%d\n",
            s->id,
            s->first_name,
            s->last_name,
            s->email,
            s->phone,
            s->address,
            s->age,
            s->course,
            s->year,
            s->gpa,
            (long)s->enrollment_date,
            s->is_active
        );
    }
    fclose(file);
    return 1;
}
int student_list_load_from_file(StudentList* list, const char* filename){
    if (list == NULL || filename == NULL) {
        printf("Error: Invalid arguments to student_list_load_from_file\n");
        return 0;
    }
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open file %s for reading\n", filename);
        return 0;
    }

    // We'll read each line and parse it into student fields
    char line[512];
    int index = 0;
    while (fgets(line, sizeof(line), file)) {
        Student s;
        // Adjust sscanf format string to match the fields written in save_to_file
        int fields = sscanf(line, "%d,%[^,],%[^,],%[^,],%[^,],%[^,],%d,%[^,],%d,%f,%ld,%d",
            &s.id,
            s.first_name,
            s.last_name,
            s.email,
            s.phone,
            s.address,
            &s.age,
            s.course,
            &s.year,
            &s.gpa,
            &s.enrollment_date,
            &s.is_active
        );
        // 12 fields expected
        if (fields == 12) {
            if (index < list->capacity) {
                list->students[index++] = s;
            } else {
                // Reallocate if necessary
                int new_capacity = list->capacity ? list->capacity * 2 : 16;
                Student *new_students = realloc(list->students, sizeof(Student) * new_capacity);
                if (!new_students) {
                    printf("Error: Unable to allocate more memory for students\n");
                    fclose(file);
                    return 0;
                }
                list->students = new_students;
                list->capacity = new_capacity;
                list->students[index++] = s;
            }
        }
    }
    list->count = index;
    fclose(file);
    return 1;
}
void student_list_sort_by_name(StudentList* list) {
    if (list == NULL || list->students == NULL) {
        printf("Error: Invalid student list\n");
        return;
    }
    // Simple bubble sort by last_name, then first_name if last names equal
    for (int i = 0; i < list->count - 1; i++) {
        for (int j = 0; j < list->count - 1 - i; j++) {
            int cmp = strcmp(list->students[j].last_name, list->students[j + 1].last_name);
            if (cmp > 0 || (cmp == 0 && strcmp(list->students[j].first_name, list->students[j + 1].first_name) > 0)) {
                Student temp = list->students[j];
                list->students[j] = list->students[j + 1];
                list->students[j + 1] = temp;
            }
        }
    }
}

// Sort students by ID in ascending order
void student_list_sort_by_id(StudentList* list) {
    if (list == NULL || list->students == NULL || list->count < 2) return;
    for (int i = 0; i < list->count - 1; i++) {
        for (int j = 0; j < list->count - i - 1; j++) {
            if (list->students[j].id > list->students[j + 1].id) {
                Student temp = list->students[j];
                list->students[j] = list->students[j + 1];
                list->students[j + 1] = temp;
            }
        }
    }
}

// Sort students by GPA in descending order
void student_list_sort_by_gpa(StudentList* list) {
    if (list == NULL || list->students == NULL || list->count < 2) return;
    for (int i = 0; i < list->count - 1; i++) {
        for (int j = 0; j < list->count - i - 1; j++) {
            if (list->students[j].gpa < list->students[j + 1].gpa) {
                Student temp = list->students[j];
                list->students[j] = list->students[j + 1];
                list->students[j + 1] = temp;
            }
        }
    }
    int student_list_get_count(StudentList* list) {
        if (list == NULL) return 0;
        return list->count;
    }
}
Student* student_list_get_student(StudentList* list, int index){
    if(list == NULL || list->students == NULL){
        return NULL;
    }
    // Is true if index is in valid range
    if(index >= 0 && index < list->count) {
        return &list->students[index];
    } else {
        return NULL;
    }
}
int student_validation_gpa(float gpa) {
    if (gpa < 0.0 || gpa > 4.0) {
        printf("GPA must be between 0.0 and 4.0\n");
        return 0;
    }
    return 1;
}

int student_validation_email(const char* email) {
    printf("Validation de l'email...\n");
    return 1; // always accept for now, simple
}

int student_validation_phone(const char* phone) {
    printf("Validation du téléphone...\n");
    return 1; // always accept for now, simple
}

int student_validation_age(int age) {
    if (age < 18 || age > 100) {
        printf("Age must be between 18 and 100 years\n");
        return 0;
    }
    return 1;
}

Student student_input_new() {
    Student s;
    int n;

    printf("ID: ");
    scanf("%d", &s.id);

    printf("Nom: ");
    scanf("%s", s.first_name);

    printf("Prenom: ");
    scanf("%s", s.last_name);

    do {
        printf("Email: ");
        scanf("%s", s.email);
        n = student_validation_email(s.email);
    } while (n == 0);

    do {
        printf("Telephone: ");
        scanf("%s", s.phone);
        n = student_validation_phone(s.phone);
    } while (n == 0);

    do {
        printf("Age: ");
        scanf("%d", &s.age);
        n = student_validation_age(s.age);
    } while (n == 0);

    printf("Filiere: ");
    scanf(" %[^\n]", s.course);

    printf("Adresse: ");
    scanf(" %[^\n]", s.address);

    printf("Year: ");
    scanf("%d", &s.year);

    do {
        printf("GPA: ");
        scanf("%f", &s.gpa);
        n = student_validation_gpa(s.gpa);
    } while (n == 0);

    s.enrollment_date = time(0);
    s.is_active = 1;

    return s;
}

void student_input_edit(Student* student) {
    int choice;
    printf("\nEdit student info (for now: just select and re-enter value, no validation):\n");
    printf("1 - Prenom\n");
    printf("2 - Nom\n");
    printf("3 - Email\n");
    printf("4 - Telephone\n");
    printf("5 - GPA\n");
    printf("6 - Age\n");
    printf("7 - Filiere\n");
    printf("8 - Enrollment_date\n");
    printf("9 - Is_active\n");
    printf("10 - Adresse\n");
    printf("11 - Year\n");
    printf("12 - Modifier tout\n");
    printf("0 - Annuler\n");
    printf("Choix: ");
    scanf("%d", &choice);
    switch (choice) {
        case 1:
            printf("Nouveau prenom: ");
            scanf("%s", student->last_name);
            break;
        case 2:
            printf("Nouveau nom: ");
            scanf("%s", student->first_name);
            break;
        case 3:
            printf("Nouvel email: ");
            scanf("%s", student->email);
            break;
        case 4:
            printf("Nouveau telephone: ");
            scanf("%s", student->phone);
            break;
        case 5:
            printf("Nouveau GPA: ");
            scanf("%f", &student->gpa);
            break;
        case 6:
            printf("Nouvel age: ");
            scanf("%d", &student->age);
            break;
        case 7:
            printf("Nouvelle filiere: ");
            scanf(" %[^\n]", student->course);
            break;
        case 8:
            printf("Nouvelle enrollment_date (timestamp entier): ");
            scanf("%ld", &student->enrollment_date);
            break;
        case 9:
            printf("is_active (0/1): ");
            scanf("%d", &student->is_active);
            break;
        case 10:
            printf("Nouvelle adresse: ");
            scanf(" %[^\n]", student->address);
            break;
        case 11:
            printf("Nouvelle annee: ");
            scanf("%d", &student->year);
            break;
        case 12:
            printf("Modifier tout:\n");
            *student = student_input_new();
            break;
        case 0:
        default:
            // Annuler ou choix invalide, ne rien faire
            break;
    }
}

void student_display_summary(StudentList* list) {
    if (!list || list->count == 0) {
        printf("Aucun étudiant à afficher.\n");
        return;
    }
    printf("| ID  | Prenom          | Nom             | Email                   | Telephone     | GPA  | Age | Statut | Filiere        |\n");
    printf("--------------------------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < list->count; i++) {
        Student s = list->students[i];
        printf("| %-3d | %-15s | %-15s | %-22s | %-12s | %-4.2f | %-3d | %-6d | %-14s |\n",
               s.id, s.first_name, s.last_name, s.email, s.phone, s.gpa, s.age, s.is_active, s.course);
    }
    printf("--------------------------------------------------------------------------------------------------------------\n");
    printf("Total: %d étudiant(s)\n", list->count);
}
