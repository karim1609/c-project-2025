#include "student.h"
#include "config.h"
#include "crypto.h"
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


const char* filiere_vers_chaine(FiliereEtude filiere) {
    switch (filiere) {
        case ANNEES_PREPARATOIRES:
            return "Années Préparatoires au Cycle Ingénieur";
        case GENIE_INFORMATIQUE:
            return "Génie Informatique";
        case GENIE_CIVIL:
            return "Génie Civil";
        case GENIE_EAU_ENVIRONNEMENT:
            return "Génie de l'Eau et de l'Environnement";
        case GENIE_ENERGETIQUE:
            return "Génie Énergétique et Énergies Renouvelables";
        case GENIE_MECANIQUE:
            return "Génie Mécanique";
        default:
            return "Filière inconnue";
    }
}

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
        if (strcmp(list->students[i].email, email) == 0) {
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
       printf("age: %d\n", list->students[i].age);
       printf("Phone: %s\n", list->students[i].phone);
       printf("Address: %s\n", list->students[i].address);
       printf("Filiere: %d\n", list->students[i].filiere);
       printf("Year: %d\n", list->students[i].year);
       printf("GPA: %.2f\n", list->students[i].gpa);
       printf("enrollment_date: %lld\n", (long long)list->students[i].enrollment_date);
       printf("is_active: %d\n", list->students[i].is_active);
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
     printf("Filiere: %d\n", student->filiere);
    printf("Year: %d\n", student->year);
    printf("GPA: %.2f\n", student->gpa);
    printf("enrollment_date: %lld\n", (long long)student->enrollment_date); 
    printf("is_active: %d\n", student->is_active);
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
        fprintf(file, "%d,%s,%s,%s,%s,%s,%d,%d,%d,%.2f,%lld,%d\n",
            s->id,
            s->first_name,
            s->last_name,
            s->email,
            s->phone,
            s->address,
            s->age,
            s->year,
            s->filiere,
            s->gpa,
            (long long)s->enrollment_date,
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
    if (list->students == NULL) {
        printf("Error: Student list students array is not allocated\n");
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
        long long enrollment_date_temp;
       int fields = sscanf(line, "%d,%[^,],%[^,],%[^,],%[^,],%[^,],%d,%d,%d,%f,%lld,%d",
    &s.id,
    s.first_name,
    s.last_name,
    s.email,
    s.phone,
    s.address,
    &s.age,
    &s.year, 
    &s.filiere,
    &s.gpa,
    &enrollment_date_temp,
    &s.is_active
);
        // 12 fields expected
        if (fields == 12) {
            s.enrollment_date = (time_t)enrollment_date_temp;
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
}

int student_list_get_count(StudentList* list) {
    if (list == NULL) return 0;
    return list->count;
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
int student_list_ensure_loaded(StudentList* list){
    // Check for NULL pointer
    if (list == NULL) {
        printf("Error: Invalid student list (NULL pointer)\n");
        return 0;
    }
    
    // If already loaded, return success
    if (list->is_loaded) {
        return 1;
    }
    
    // Check if filename is set
    if (list->filename[0] == '\0') {
        printf("Error: No filename set for student list\n");
        return 0;
    }
    
    // Ensure students array is allocated
    if (list->students == NULL) {
        list->students = (Student*)malloc(MAX_STUDENTS * sizeof(Student));
        if (list->students == NULL) {
            printf("Error: Failed to allocate memory for students array\n");
            return 0;
        }
        list->capacity = MAX_STUDENTS;
    }
    
    // Reset count before loading to avoid appending to existing data
    list->count = 0;
    
    // Try to load data from file
    if (student_list_load_from_file(list, list->filename) == 0) {
        printf("Error: Failed to load student data from file: %s\n", list->filename);
        return 0;
    }
    
    // Mark as loaded on success
    list->is_loaded = 1;
    return 1;
}

int student_list_save_and_unload(StudentList* list){
    // Check for NULL pointer
    if (list == NULL) {
        printf("Error: Invalid student list (NULL pointer)\n");
        return 0;
    }
    
    // Check if filename is set
    if (list->filename[0] == '\0') {
        printf("Error: No filename set for student list\n");
        return 0;
    }
    
    // Check if data is loaded and has students to save
    if (!list->is_loaded || list->students == NULL) {
        printf("Error: No data loaded to save\n");
        return 0;
    }

    // Save data to file
    if (student_list_save_to_file(list, list->filename) == 0) {
        printf("Error: Failed to save student data to file: %s\n", list->filename);
        return 0;
    }
    
    // Update last save time
    list->last_save_time = time(NULL);
    
    // Free the students array to unload from memory
    if (list->students != NULL) {
        free(list->students);
        list->students = NULL;
    }
    
    // Reset count and capacity
    list->count = 0;
    list->capacity = 0;
    
    // Mark as not loaded
    list->is_loaded = 0;
    
    return 1;
}
int student_list_auto_save(StudentList* list){
    // Check for NULL pointer
    if (list == NULL) {
        return 0;
    }
    
    // Check if auto-save is enabled
    if (list->auto_save_enabled == 0) {
        return 0;
    }
    
    // Check if data is loaded
    if (!list->is_loaded || list->students == NULL) {
        return 0;
    }
    
    // Check if filename is set
    if (list->filename[0] == '\0') {
        return 0;
    }
    
    // Save data to file
    if (student_list_save_to_file(list, list->filename) == 0) {
        return 0;
    }
    
    // Update last save time
    list->last_save_time = time(NULL);
    
    return 1;   
}
int student_list_is_loaded(StudentList* list){
    if(list == NULL){
        return 0;
    }
    return list->is_loaded;
}
void student_list_set_filename(StudentList* list, const char* filename){
    if (list == NULL || filename == NULL) {
        return;
    }
    
    // Use strncpy to safely copy filename with size limit
    strncpy(list->filename, filename, sizeof(list->filename) - 1);
    list->filename[sizeof(list->filename) - 1] = '\0'; // Ensure null termination
}
int student_validate_gpa(float gpa) {
    if (gpa < 0.0 || gpa > 20.0) {
        printf(" GPA must be between 0.0 and 20.0\n");
        return 0;
    }
    return 1;
}

// Email Validation: Checks for emptiness, length, valid structure, and valid domain
int student_validate_email(const char* email) {
    if (!email || strlen(email) == 0 || strlen(email) > MAX_EMAIL_LENGTH) {
        printf("Invalid email: Length is 0 or exceeds maximum.\n");
        return 0;
    }
    char* at_pos = strchr(email, '@');
    if (!at_pos || strchr(at_pos + 1, '@')) {
        printf("Invalid email format. Please use format: user@domain.com\n");
        return 0;
    }
    // Check that there's at least one character before @
    if (at_pos == email) {
        printf("Invalid email format. Please use format: user@domain.com\n");
        return 0;
    }
    // Check that domain has at least one dot
    char* dot = strchr(at_pos + 1, '.');
    if (!dot || dot == at_pos + 1) {
        printf("Invalid email format. Please use format: user@domain.com\n");
        return 0;
    }
    // Check that there's at least one character after the last dot
    char* last_dot = strrchr(at_pos + 1, '.');
    if (last_dot && strlen(last_dot + 1) == 0) {
        printf("Invalid email format. Please use format: user@domain.com\n");
        return 0;
    }
    return 1;
}

// Phone Validation: Checks if phone contains valid characters and is in valid length
int student_validate_phone(const char* phone) {
    if (!phone || strlen(phone) == 0 || strlen(phone) > MAX_PHONE_LENGTH || strlen(phone) < 10) {
        printf("Invalid phone: Empty, too short, or too long.\n");
        return 0;
    }
    int digit_count = 0;
    for (size_t i = 0; i < strlen(phone); i++) {
        char c = phone[i];
        if (c >= '0' && c <= '9') {
            digit_count++;  // Count digits
        } else if (c != '+' && c != '-' && c != '(' && c != ')' && c != ' ' && c != '.') {
            printf("Invalid phone format: Only digits, +, -, (, ), spaces, and dots are allowed.\n");
            return 0;
        }
    }
    if (digit_count < 7) {
        printf("Invalid phone format: Phone must contain at least 7 digits.\n");
        return 0;
    }
    return 1;
}

// Age Validation: Checks if age is between 18 and 100
int student_validate_age(int age) {
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
        n = student_validate_email(s.email);
    } while (n == 0);

    do {
        printf("Telephone: ");
        scanf("%s", s.phone);
        n = student_validate_phone(s.phone);
    } while (n == 0);

    do {
        printf("Age: ");
        scanf("%d", &s.age);
        n = student_validate_age(s.age);
    } while (n == 0);

    printf("Filiere: ");
    scanf(" %[^\n]", s.filiere);

    printf("Adresse: ");
    scanf(" %[^\n]", s.address);

    printf("Year: ");
    scanf("%d", &s.year);

    do {
        printf("GPA: ");
        scanf("%f", &s.gpa);
        n = student_validate_gpa(s.gpa);
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
            scanf(" %[^\n]", student->filiere);
            break;
        case 8:
            printf("Nouvelle enrollment_date (timestamp entier): ");
            {
                long long temp;
                scanf("%lld", &temp);
                student->enrollment_date = (time_t)temp;
            }
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
        printf("| %-3d | %-15s | %-15s | %-22s | %-12s | %-4.2f | %-3d | %-6d | %-30s |\n",
       s.id, s.first_name, s.last_name, s.email, s.phone, s.gpa, s.age, s.is_active, 
       filiere_vers_chaine(s.filiere));
    }
    printf("--------------------------------------------------------------------------------------------------------------\n");
    printf("Total: %d étudiant(s)\n", list->count);
}