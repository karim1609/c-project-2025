# Configuration System - Complete Explanation

## 📋 Overview
This document explains the configuration system (`config.h`) that defines all constants, settings, and data types used throughout the Student Management System.

## 🔍 Table of Contents
1. [Application Metadata](#application-metadata)
2. [System Limits](#system-limits)
3. [File Paths](#file-paths)
4. [Encryption Settings](#encryption-settings)
5. [User Interface Settings](#user-interface-settings)
6. [User Roles](#user-roles)
7. [Grade Levels](#grade-levels)
8. [Usage Examples](#usage-examples)
9. [Best Practices](#best-practices)

---

## 📱 Application Metadata

### Application Information
```c
#define APP_NAME "Student Management System"
#define APP_VERSION "1.0.0"
```

**Purpose:** Defines basic application information
**Usage:** Displayed in UI, used in logging, and for version checking

```c
printf("Welcome to %s version %s\n", APP_NAME, APP_VERSION);
// Output: "Welcome to Student Management System version 1.0.0"
```

**Benefits:**
- **Consistency:** Same name/version used everywhere
- **Easy Updates:** Change version in one place
- **Professional:** Proper application identification

---

## 📊 System Limits

### Student Capacity
```c
#define MAX_STUDENTS 1000
```

**Purpose:** Maximum number of students the system can handle
**Why 1000:**
- Large enough for most educational institutions
- Small enough to fit in memory efficiently
- Provides good performance for most operations

```c
// Usage in code
if (student_list->count >= MAX_STUDENTS) {
    printf("System at capacity. Cannot add more students.\n");
    return 0;
}
```

### String Length Limits
```c
#define MAX_NAME_LENGTH 100        // First/Last names
#define MAX_EMAIL_LENGTH 150      // Email addresses
#define MAX_PHONE_LENGTH 20       // Phone numbers
#define MAX_ADDRESS_LENGTH 200    // Street addresses
#define MAX_COURSE_LENGTH 50      // Course names
#define MAX_CLUB_LENGTH 50        // Club names
```

**Purpose:** Prevents buffer overflows and ensures data consistency
**Security Benefits:**
- **Buffer Overflow Prevention:** Limits input size
- **Memory Management:** Controls memory usage
- **Database Efficiency:** Fixed-size fields are faster

**Usage Examples:**
```c
// Safe string declarations
char first_name[MAX_NAME_LENGTH];
char email[MAX_EMAIL_LENGTH];
char phone[MAX_PHONE_LENGTH];

// Safe input with limits
scanf("%99s", first_name);  // 99 + null terminator = 100
scanf("%149s", email);      // 149 + null terminator = 150
scanf("%19s", phone);        // 19 + null terminator = 20
```

**Why These Specific Lengths:**
- **MAX_NAME_LENGTH (100):** Accommodates long names, international characters
- **MAX_EMAIL_LENGTH (150):** RFC standard email length limit
- **MAX_PHONE_LENGTH (20):** Includes country codes, formatting characters
- **MAX_ADDRESS_LENGTH (200):** Full street addresses with apartment numbers
- **MAX_COURSE_LENGTH (50):** Course names like "Advanced Computer Science"
- **MAX_CLUB_LENGTH (50):** Club names like "Computer Science Society"

---

## 📁 File Paths

### Data Directory Structure
```c
#define DATA_DIR "data/"
#define BACKUP_DIR "data/backup/"
#define KEYS_DIR "data/keys/"
```

**Purpose:** Organizes all data files in a structured directory
**Benefits:**
- **Organization:** Clear separation of data types
- **Backup:** Easy to backup entire data directory
- **Security:** Centralized location for access control

### Encrypted Data Files
```c
#define STUDENTS_FILE "students.enc"
#define USERS_FILE "users.enc"
#define GRADES_FILE "grades.enc"
#define CLUBS_FILE "clubs.enc"
#define ATTENDANCE_FILE "attendance.enc"
#define LOGS_FILE "logs.enc"
```

**Purpose:** Defines encrypted file names for different data types
**Security Benefits:**
- **Encryption:** All sensitive data is encrypted
- **File Extension:** `.enc` clearly indicates encrypted files
- **Separation:** Different data types in separate files

**Usage Examples:**
```c
// Load student data
student_list_load_from_file(list, DATA_DIR STUDENTS_FILE);
// Equivalent to: "data/students.enc"

// Save user data
auth_save_users(users, DATA_DIR USERS_FILE);
// Equivalent to: "data/users.enc"

// Backup operations
backup_create_backup(DATA_DIR STUDENTS_FILE, BACKUP_DIR "students.bak");
```

### Key Management
```c
#define MASTER_KEY_FILE "master.key"
```

**Purpose:** Defines the master encryption key file location
**Security:** Master key used to encrypt/decrypt all other data files

```c
// Load master key
char master_key[AES_KEY_SIZE];
load_master_key(KEYS_DIR MASTER_KEY_FILE, master_key);
// Equivalent to: "data/keys/master.key"
```

---

## 🔐 Encryption Settings

### AES Encryption Parameters
```c
#define AES_KEY_SIZE 32    // 256-bit encryption key
#define AES_IV_SIZE 16     // Initialization Vector size
#define SALT_SIZE 32       // Password salt size
```

**Purpose:** Defines encryption parameters for data security
**Security Level:** Military-grade encryption

### AES_KEY_SIZE (32 bytes = 256 bits)
**Purpose:** Defines the size of encryption keys
**Why 256-bit:**
- **Military Grade:** Used by government agencies
- **Future Proof:** Resistant to quantum computing attacks
- **Industry Standard:** Recommended by security experts

```c
// Usage in encryption functions
char encryption_key[AES_KEY_SIZE];
char decrypted_data[MAX_STUDENTS * sizeof(Student)];

// Generate random key
generate_random_key(encryption_key, AES_KEY_SIZE);

// Encrypt data
aes_encrypt(decrypted_data, encrypted_data, encryption_key, AES_KEY_SIZE);
```

### AES_IV_SIZE (16 bytes)
**Purpose:** Initialization Vector for AES encryption
**Why needed:**
- **Uniqueness:** Makes same data look different each time
- **Security:** Prevents pattern recognition in encrypted data
- **Standard:** Required by AES encryption algorithm

```c
// Generate random IV for each encryption
char iv[AES_IV_SIZE];
generate_random_iv(iv, AES_IV_SIZE);

// Use IV in encryption
aes_encrypt_with_iv(data, encrypted_data, key, iv);
```

### SALT_SIZE (32 bytes)
**Purpose:** Salt for password hashing
**Security Benefits:**
- **Rainbow Table Protection:** Prevents precomputed hash attacks
- **Uniqueness:** Each password has unique salt
- **Strength:** Makes password cracking extremely difficult

```c
// Generate salt for password hashing
char salt[SALT_SIZE];
generate_random_salt(salt, SALT_SIZE);

// Hash password with salt
char password_hash[65];  // SHA-256 output size
hash_password_with_salt(password, salt, password_hash);
```

---

## 🖥️ User Interface Settings

### Window Dimensions
```c
#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800
```

**Purpose:** Defines the main application window size
**Why These Dimensions:**
- **Modern Standard:** Good for current screen resolutions
- **Content Display:** Wide enough for tables and forms
- **Usability:** Not too large for smaller screens
- **Balance:** Good ratio for data visualization

```c
// Usage in GTK window creation
GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
gtk_window_set_default_size(GTK_WINDOW(window), WINDOW_WIDTH, WINDOW_HEIGHT);
```

### Theme Support
```c
#define THEME_DARK "dark"
#define THEME_LIGHT "light"
```

**Purpose:** Defines available UI themes
**Benefits:**
- **User Preference:** Users can choose their preferred theme
- **Accessibility:** Dark theme reduces eye strain
- **Modern UI:** Professional appearance

```c
// Theme switching
void switch_theme(const char* theme) {
    if (strcmp(theme, THEME_DARK) == 0) {
        apply_dark_theme();
    } else if (strcmp(theme, THEME_LIGHT) == 0) {
        apply_light_theme();
    }
}

// Usage
switch_theme(THEME_DARK);   // Apply dark theme
switch_theme(THEME_LIGHT);  // Apply light theme
```

---

## 👥 User Roles

### Role Definition
```c
typedef enum {
    ROLE_ADMIN = 1,
    ROLE_TEACHER = 2,
    ROLE_STUDENT = 3
} UserRole;
```

**Purpose:** Defines user permission levels
**Hierarchy:** Lower numbers = higher privileges

### ROLE_ADMIN (1) - Highest Privileges
**Capabilities:**
- Full system access
- User management (create, edit, delete users)
- System configuration
- All student data access
- Report generation
- Backup and restore operations

```c
// Admin-only operations
if (session->role == ROLE_ADMIN) {
    // Allow user deletion
    user_list_remove(users, user_id);
    
    // Allow system configuration
    update_system_settings(new_settings);
    
    // Allow all data access
    student_list_display_all(students);
}
```

### ROLE_TEACHER (2) - Medium Privileges
**Capabilities:**
- Student data management
- Grade input and editing
- Attendance tracking
- Report generation
- Cannot access admin functions

```c
// Teacher operations
if (session->role <= ROLE_TEACHER) {
    // Allow grade editing
    student->gpa = new_grade;
    
    // Allow attendance input
    mark_attendance(student_id, date, present);
    
    // Allow report generation
    generate_class_report(class_id);
}
```

### ROLE_STUDENT (3) - Lowest Privileges
**Capabilities:**
- View own data only
- Cannot edit other students' information
- Limited to personal records
- Cannot access administrative functions

```c
// Student operations
if (session->role == ROLE_STUDENT) {
    // Only allow viewing own data
    if (student->id == session->user_id) {
        display_student_info(student);
    } else {
        printf("Access denied. You can only view your own data.\n");
    }
}
```

### Role Hierarchy Usage
```c
// Check if user has sufficient privileges
int has_admin_access(UserRole role) {
    return role == ROLE_ADMIN;
}

int has_teacher_access(UserRole role) {
    return role <= ROLE_TEACHER;  // Admin or Teacher
}

int has_student_access(UserRole role) {
    return role <= ROLE_STUDENT;  // Any valid user
}

// Usage in functions
void delete_user(UserList* users, int user_id, UserRole current_user_role) {
    if (!has_admin_access(current_user_role)) {
        printf("Error: Admin privileges required\n");
        return;
    }
    
    user_list_remove(users, user_id);
    printf("User deleted successfully\n");
}
```

---

## 📊 Grade Levels

### Grade Definition
```c
typedef enum {
    GRADE_A = 4,
    GRADE_B = 3,
    GRADE_C = 2,
    GRADE_D = 1,
    GRADE_F = 0
} GradeLevel;
```

**Purpose:** Defines standard grade point values
**Scale:** 4.0 scale used in most educational institutions

### Grade Values and Meanings
- **GRADE_A (4):** Excellent performance
- **GRADE_B (3):** Good performance  
- **GRADE_C (2):** Average performance
- **GRADE_D (1):** Below average performance
- **GRADE_F (0):** Failing performance

### Usage Examples
```c
// Grade input and validation
GradeLevel get_grade_from_user() {
    char grade_char;
    printf("Enter grade (A, B, C, D, F): ");
    scanf(" %c", &grade_char);
    
    switch (toupper(grade_char)) {
        case 'A': return GRADE_A;
        case 'B': return GRADE_B;
        case 'C': return GRADE_C;
        case 'D': return GRADE_D;
        case 'F': return GRADE_F;
        default:
            printf("Invalid grade. Please enter A, B, C, D, or F.\n");
            return get_grade_from_user();  // Retry
    }
}

// GPA calculation
float calculate_gpa(GradeLevel* grades, int num_grades) {
    float total_points = 0;
    for (int i = 0; i < num_grades; i++) {
        total_points += grades[i];
    }
    return total_points / num_grades;
}

// Grade display
void display_grade_info(GradeLevel grade) {
    const char* grade_names[] = {"F", "D", "C", "B", "A"};
    printf("Grade: %s (%.1f points)\n", grade_names[grade], (float)grade);
}
```

### Grade Conversion Functions
```c
// Convert grade level to string
const char* grade_level_to_string(GradeLevel grade) {
    switch (grade) {
        case GRADE_A: return "A";
        case GRADE_B: return "B";
        case GRADE_C: return "C";
        case GRADE_D: return "D";
        case GRADE_F: return "F";
        default: return "Unknown";
    }
}

// Convert string to grade level
GradeLevel string_to_grade_level(const char* grade_str) {
    if (strcmp(grade_str, "A") == 0) return GRADE_A;
    if (strcmp(grade_str, "B") == 0) return GRADE_B;
    if (strcmp(grade_str, "C") == 0) return GRADE_C;
    if (strcmp(grade_str, "D") == 0) return GRADE_D;
    if (strcmp(grade_str, "F") == 0) return GRADE_F;
    return GRADE_F;  // Default to F for invalid input
}
```

---

## 💻 Complete Usage Examples

### System Initialization
```c
#include "config.h"
#include <stdio.h>
#include <stdlib.h>

int initialize_system() {
    printf("=== %s v%s ===\n", APP_NAME, APP_VERSION);
    
    // Create data directories
    create_directory(DATA_DIR);
    create_directory(BACKUP_DIR);
    create_directory(KEYS_DIR);
    
    // Initialize encryption
    char master_key[AES_KEY_SIZE];
    generate_master_key(master_key, AES_KEY_SIZE);
    save_master_key(KEYS_DIR MASTER_KEY_FILE, master_key);
    
    printf("System initialized successfully\n");
    return 1;
}
```

### Student Data Management
```c
int add_student_with_validation(StudentList* list) {
    Student new_student;
    
    // Get student information with length validation
    printf("Enter student information:\n");
    
    printf("First name: ");
    scanf("%99s", new_student.first_name);  // MAX_NAME_LENGTH - 1
    
    printf("Last name: ");
    scanf("%99s", new_student.last_name);
    
    printf("Email: ");
    scanf("%149s", new_student.email);  // MAX_EMAIL_LENGTH - 1
    
    printf("Phone: ");
    scanf("%19s", new_student.phone);   // MAX_PHONE_LENGTH - 1
    
    printf("Address: ");
    scanf("%199s", new_student.address); // MAX_ADDRESS_LENGTH - 1
    
    printf("Course: ");
    scanf("%49s", new_student.course);   // MAX_COURSE_LENGTH - 1
    
    // Validate input lengths
    if (strlen(new_student.first_name) == 0) {
        printf("Error: First name cannot be empty\n");
        return 0;
    }
    
    if (strlen(new_student.email) == 0) {
        printf("Error: Email cannot be empty\n");
        return 0;
    }
    
    // Check system capacity
    if (list->count >= MAX_STUDENTS) {
        printf("Error: System at maximum capacity (%d students)\n", MAX_STUDENTS);
        return 0;
    }
    
    // Add student
    if (student_list_add(list, new_student)) {
        printf("Student added successfully\n");
        return 1;
    } else {
        printf("Failed to add student\n");
        return 0;
    }
}
```

### Role-Based Access Control
```c
void handle_user_request(Session* session, const char* action) {
    // Check session validity
    if (!session->is_valid) {
        printf("Error: Invalid session\n");
        return;
    }
    
    // Role-based access control
    if (strcmp(action, "delete_user") == 0) {
        if (session->role == ROLE_ADMIN) {
            printf("Admin: User deletion allowed\n");
            // Perform user deletion
        } else {
            printf("Error: Admin privileges required\n");
        }
    }
    else if (strcmp(action, "edit_grades") == 0) {
        if (session->role <= ROLE_TEACHER) {
            printf("Teacher/Admin: Grade editing allowed\n");
            // Perform grade editing
        } else {
            printf("Error: Teacher privileges required\n");
        }
    }
    else if (strcmp(action, "view_profile") == 0) {
        if (session->role <= ROLE_STUDENT) {
            printf("User: Profile viewing allowed\n");
            // Perform profile viewing
        } else {
            printf("Error: Please log in\n");
        }
    }
    else {
        printf("Error: Unknown action\n");
    }
}
```

### File Operations with Configuration
```c
int save_all_data(StudentList* students, UserList* users) {
    char master_key[AES_KEY_SIZE];
    
    // Load master key
    if (!load_master_key(KEYS_DIR MASTER_KEY_FILE, master_key)) {
        printf("Error: Failed to load master key\n");
        return 0;
    }
    
    // Save students
    if (!student_list_save_to_file(students, DATA_DIR STUDENTS_FILE)) {
        printf("Error: Failed to save students\n");
        return 0;
    }
    
    // Save users
    if (!auth_save_users(users, DATA_DIR USERS_FILE)) {
        printf("Error: Failed to save users\n");
        return 0;
    }
    
    // Create backup
    char backup_path[256];
    sprintf(backup_path, "%sbackup_%ld.bak", BACKUP_DIR, time(NULL));
    
    if (!backup_create_backup(DATA_DIR, backup_path)) {
        printf("Warning: Failed to create backup\n");
    }
    
    printf("All data saved successfully\n");
    return 1;
}
```

---

## 🛡️ Best Practices

### 1. Always Use Configuration Constants
```c
// ❌ Bad - Hard-coded values
char name[100];
char email[150];

// ✅ Good - Using configuration
char name[MAX_NAME_LENGTH];
char email[MAX_EMAIL_LENGTH];
```

### 2. Validate Against Limits
```c
// Always check system limits
if (student_count >= MAX_STUDENTS) {
    printf("System at capacity (%d students)\n", MAX_STUDENTS);
    return 0;
}
```

### 3. Use Role Hierarchy
```c
// Check permissions using hierarchy
if (user_role <= ROLE_TEACHER) {
    // Allow teacher and admin operations
} else {
    // Deny access
}
```

### 4. Secure File Paths
```c
// Always use defined paths
char full_path[256];
sprintf(full_path, "%s%s", DATA_DIR, STUDENTS_FILE);
// Result: "data/students.enc"
```

### 5. Consistent Grade Handling
```c
// Use grade enums instead of strings
GradeLevel grade = GRADE_A;  // Instead of "A"
float gpa = (float)grade;   // Convert to float for calculations
```

---

## 📊 Configuration Summary Table

| Category | Constants | Purpose | Security Level |
|----------|-----------|---------|----------------|
| **App Info** | APP_NAME, APP_VERSION | Application identification | Low |
| **Limits** | MAX_STUDENTS, MAX_*_LENGTH | System capacity and safety | High |
| **Files** | DATA_DIR, *.enc files | Data organization | High |
| **Encryption** | AES_KEY_SIZE, AES_IV_SIZE, SALT_SIZE | Data security | Critical |
| **UI** | WINDOW_WIDTH, WINDOW_HEIGHT, THEME_* | User interface | Low |
| **Roles** | ROLE_ADMIN, ROLE_TEACHER, ROLE_STUDENT | Access control | High |
| **Grades** | GRADE_A, GRADE_B, GRADE_C, GRADE_D, GRADE_F | Academic system | Medium |

---

## 🎯 Conclusion

The configuration system provides:
- **Centralized Settings:** All constants in one place
- **Security Parameters:** Encryption and access control
- **System Limits:** Prevents overflow and capacity issues
- **Consistency:** Same values used throughout the system
- **Maintainability:** Easy to update and modify

This configuration system ensures the Student Management System is secure, scalable, and maintainable.

---

*This documentation is part of the Student Management System project.*
*Last updated: 2024*
