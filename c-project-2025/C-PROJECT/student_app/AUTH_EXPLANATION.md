# Authentication System - Complete Explanation

## 📋 Overview
This document explains the complete authentication system used in the Student Management System, including user management, password security, session handling, and role-based access control.

## 🔍 Table of Contents
1. [User Management](#user-management)
2. [Password Security](#password-security)
3. [Session Management](#session-management)
4. [Authentication Functions](#authentication-functions)
5. [Role-Based Access Control](#role-based-access-control)
6. [File Operations](#file-operations)
7. [Usage Examples](#usage-examples)
8. [Security Best Practices](#security-best-practices)

---

## 👤 User Management

### User Structure
```c
typedef struct {
    int id;                    // Unique user identifier
    char username[50];         // Login username
    char email[MAX_EMAIL_LENGTH]; // User's email address
    char password_hash[65];    // SHA-256 hash of password
    char salt[33];            // Random salt for password security
    UserRole role;            // User permissions level
    time_t created_at;         // Account creation timestamp
    time_t last_login;        // Last login timestamp
    int is_active;            // Account status (1=active, 0=disabled)
} User;
```

### UserList Structure
```c
typedef struct {
    User* users;              // Dynamic array of users
    int count;                // Number of users currently stored
    int capacity;             // Maximum number of users that can be stored
} UserList;
```

### User Management Functions

#### `UserList* user_list_create(void)`
**Purpose:** Creates a new empty user list
**Returns:** Pointer to new UserList structure
**Memory:** Allocates memory for the list structure

```c
UserList* users = user_list_create();
if (users == NULL) {
    printf("Failed to create user list\n");
    exit(1);
}
```

#### `void user_list_destroy(UserList* list)`
**Purpose:** Frees all memory associated with the user list
**Parameters:** Pointer to UserList to destroy
**Memory:** Frees all allocated memory

```c
user_list_destroy(users);  // Clean up when done
```

#### `int user_list_add(UserList* list, User user)`
**Purpose:** Adds a new user to the list
**Parameters:** 
- `list`: Pointer to UserList
- `user`: User structure to add
**Returns:** `1` if successful, `0` if failed

```c
User new_user;
strcpy(new_user.username, "john_doe");
strcpy(new_user.email, "john@email.com");
new_user.role = ROLE_STUDENT;
new_user.is_active = 1;

if (user_list_add(users, new_user)) {
    printf("User added successfully\n");
} else {
    printf("Failed to add user\n");
}
```

#### `int user_list_remove(UserList* list, int user_id)`
**Purpose:** Removes a user from the list by ID
**Parameters:**
- `list`: Pointer to UserList
- `user_id`: ID of user to remove
**Returns:** `1` if successful, `0` if user not found

```c
if (user_list_remove(users, 1001)) {
    printf("User removed successfully\n");
} else {
    printf("User not found\n");
}
```

#### `User* user_list_find_by_username(UserList* list, const char* username)`
**Purpose:** Finds user by username
**Parameters:**
- `list`: Pointer to UserList
- `username`: Username to search for
**Returns:** Pointer to User if found, NULL if not found

```c
User* user = user_list_find_by_username(users, "john_doe");
if (user != NULL) {
    printf("Found user: %s\n", user->username);
} else {
    printf("User not found\n");
}
```

#### `User* user_list_find_by_email(UserList* list, const char* email)`
**Purpose:** Finds user by email address
**Parameters:**
- `list`: Pointer to UserList
- `email`: Email address to search for
**Returns:** Pointer to User if found, NULL if not found

```c
User* user = user_list_find_by_email(users, "john@email.com");
if (user != NULL) {
    printf("Found user with email: %s\n", user->email);
}
```

#### `User* user_list_find_by_id(UserList* list, int user_id)`
**Purpose:** Finds user by ID
**Parameters:**
- `list`: Pointer to UserList
- `user_id`: User ID to search for
**Returns:** Pointer to User if found, NULL if not found

```c
User* user = user_list_find_by_id(users, 1001);
if (user != NULL) {
    printf("User ID %d: %s\n", user->id, user->username);
}
```

---

## 🔐 Password Security

### Password Hashing Functions

#### `void auth_hash_password(const char* password, const char* salt, char* hash)`
**Purpose:** Creates a secure hash of a password using salt
**Parameters:**
- `password`: Plain text password
- `salt`: Random salt string
- `hash`: Output buffer for the hash (must be at least 65 characters)

```c
char password[] = "mypassword123";
char salt[33];
char hash[65];

auth_generate_salt(salt);  // Generate random salt
auth_hash_password(password, salt, hash);  // Create hash
printf("Password hash: %s\n", hash);
```

#### `int auth_verify_password(const char* password, const char* hash, const char* salt)`
**Purpose:** Verifies if a password matches its hash
**Parameters:**
- `password`: Plain text password to verify
- `hash`: Stored password hash
- `salt`: Salt used when creating the hash
**Returns:** `1` if password matches, `0` if not

```c
char input_password[] = "mypassword123";
char stored_hash[] = "5e884898da28047151d0e56f8dc6292773603d0d6aabbdd62a11ef721d1542d8";
char stored_salt[] = "a8f5f167f44f4964e6c998dee827110c";

if (auth_verify_password(input_password, stored_hash, stored_salt)) {
    printf("Password correct!\n");
} else {
    printf("Password incorrect!\n");
}
```

#### `void auth_generate_salt(char* salt)`
**Purpose:** Generates a random salt for password hashing
**Parameters:**
- `salt`: Output buffer for salt (must be at least 33 characters)

```c
char salt[33];
auth_generate_salt(salt);
printf("Generated salt: %s\n", salt);
```

#### `int auth_validate_password_strength(const char* password)`
**Purpose:** Validates password strength requirements
**Parameters:**
- `password`: Password to validate
**Returns:** `1` if password meets requirements, `0` if not

**Password Requirements:**
- At least 8 characters long
- Contains at least one uppercase letter
- Contains at least one lowercase letter
- Contains at least one digit
- Contains at least one special character

```c
char password[] = "MyPass123!";
if (auth_validate_password_strength(password)) {
    printf("Password meets strength requirements\n");
} else {
    printf("Password too weak. Requirements:\n");
    printf("- At least 8 characters\n");
    printf("- Uppercase and lowercase letters\n");
    printf("- At least one digit\n");
    printf("- At least one special character\n");
}
```

---

## 🔑 Authentication Functions

### Registration and Login

#### `int auth_register(UserList* list, const char* username, const char* email, const char* password, UserRole role)`
**Purpose:** Registers a new user account
**Parameters:**
- `list`: Pointer to UserList
- `username`: Desired username
- `email`: User's email address
- `password`: Plain text password
- `role`: User role (ROLE_ADMIN, ROLE_TEACHER, ROLE_STUDENT)
**Returns:** `1` if successful, `0` if failed

```c
if (auth_register(users, "john_doe", "john@email.com", "mypassword123", ROLE_STUDENT)) {
    printf("Registration successful!\n");
} else {
    printf("Registration failed. Username or email may already exist.\n");
}
```

#### `int auth_login(UserList* list, const char* username, const char* password, Session* session)`
**Purpose:** Authenticates user and creates session
**Parameters:**
- `list`: Pointer to UserList
- `username`: Username to authenticate
- `password`: Password to verify
- `session`: Session structure to populate
**Returns:** `1` if login successful, `0` if failed

```c
Session* session = session_create();
if (auth_login(users, "john_doe", "mypassword123", session)) {
    printf("Login successful! Welcome, %s\n", session->username);
} else {
    printf("Login failed. Invalid username or password.\n");
}
```

#### `int auth_logout(Session* session)`
**Purpose:** Logs out user and invalidates session
**Parameters:**
- `session`: Session to invalidate
**Returns:** `1` if successful, `0` if failed

```c
if (auth_logout(session)) {
    printf("Logged out successfully\n");
} else {
    printf("Logout failed\n");
}
```

#### `int auth_validate_session(Session* session)`
**Purpose:** Validates if session is still active
**Parameters:**
- `session`: Session to validate
**Returns:** `1` if session is valid, `0` if expired/invalid

```c
if (auth_validate_session(session)) {
    printf("Session is active\n");
} else {
    printf("Session expired or invalid\n");
}
```

### Password Management

#### `int auth_change_password(UserList* list, int user_id, const char* old_password, const char* new_password)`
**Purpose:** Changes user's password
**Parameters:**
- `list`: Pointer to UserList
- `user_id`: ID of user changing password
- `old_password`: Current password for verification
- `new_password`: New password
**Returns:** `1` if successful, `0` if failed

```c
if (auth_change_password(users, 1001, "oldpass123", "newpass456")) {
    printf("Password changed successfully\n");
} else {
    printf("Password change failed. Old password incorrect.\n");
}
```

#### `int auth_reset_password(UserList* list, const char* email)`
**Purpose:** Initiates password reset process
**Parameters:**
- `list`: Pointer to UserList
- `email`: Email address of user
**Returns:** `1` if reset initiated, `0` if email not found

```c
if (auth_reset_password(users, "john@email.com")) {
    printf("Password reset email sent\n");
} else {
    printf("Email address not found\n");
}
```

---

## 🎫 Session Management

### Session Structure
```c
typedef struct {
    int user_id;        // ID of logged-in user
    char username[50];  // Username for display
    UserRole role;      // User's permission level
    time_t login_time;  // When user logged in
    int is_valid;       // Session status (1=valid, 0=invalid)
} Session;
```

### Session Functions

#### `Session* session_create(void)`
**Purpose:** Creates a new session structure
**Returns:** Pointer to new Session structure
**Memory:** Allocates memory for session

```c
Session* session = session_create();
if (session == NULL) {
    printf("Failed to create session\n");
    exit(1);
}
```

#### `void session_destroy(Session* session)`
**Purpose:** Destroys session and frees memory
**Parameters:**
- `session`: Session to destroy
**Memory:** Frees allocated memory

```c
session_destroy(session);  // Clean up when done
```

#### `int session_is_admin(Session* session)`
**Purpose:** Checks if session belongs to admin user
**Parameters:**
- `session`: Session to check
**Returns:** `1` if admin, `0` if not

```c
if (session_is_admin(session)) {
    printf("Admin privileges granted\n");
    // Allow admin-only operations
} else {
    printf("Admin privileges required\n");
}
```

#### `int session_is_teacher(Session* session)`
**Purpose:** Checks if session belongs to teacher or admin
**Parameters:**
- `session`: Session to check
**Returns:** `1` if teacher or admin, `0` if not

```c
if (session_is_teacher(session)) {
    printf("Teacher privileges granted\n");
    // Allow teacher operations
} else {
    printf("Teacher privileges required\n");
}
```

#### `int session_is_student(Session* session)`
**Purpose:** Checks if session belongs to any valid user
**Parameters:**
- `session`: Session to check
**Returns:** `1` if valid user, `0` if not

```c
if (session_is_student(session)) {
    printf("User privileges granted\n");
    // Allow basic operations
} else {
    printf("Please log in\n");
}
```

---

## 👥 Role-Based Access Control

### User Roles
```c
typedef enum {
    ROLE_ADMIN = 1,    // Full system access
    ROLE_TEACHER = 2,  // Can manage students and grades
    ROLE_STUDENT = 3   // Limited access to own data
} UserRole;
```

### Role Hierarchy
- **ROLE_ADMIN (1)**: Highest privileges
  - Can manage all users
  - Can access all student data
  - Can modify system settings
  - Can view all reports

- **ROLE_TEACHER (2)**: Medium privileges
  - Can view and edit student records
  - Can input grades and attendance
  - Can generate reports
  - Cannot access admin functions

- **ROLE_STUDENT (3)**: Lowest privileges
  - Can only view own data
  - Cannot edit other students' information
  - Limited to personal records

### Role Checking Functions

#### `const char* auth_role_to_string(UserRole role)`
**Purpose:** Converts role enum to string
**Parameters:**
- `role`: UserRole enum value
**Returns:** String representation of role

```c
UserRole role = ROLE_TEACHER;
const char* role_str = auth_role_to_string(role);
printf("User role: %s\n", role_str);  // "Teacher"
```

#### `UserRole auth_string_to_role(const char* role_str)`
**Purpose:** Converts string to role enum
**Parameters:**
- `role_str`: String representation of role
**Returns:** UserRole enum value

```c
UserRole role = auth_string_to_role("admin");
if (role == ROLE_ADMIN) {
    printf("Admin role detected\n");
}
```

### Access Control Examples

#### Admin-Only Operations
```c
void delete_user(UserList* users, int user_id, Session* session) {
    if (!session_is_admin(session)) {
        printf("Error: Admin privileges required\n");
        return;
    }
    
    if (user_list_remove(users, user_id)) {
        printf("User deleted successfully\n");
    } else {
        printf("User not found\n");
    }
}
```

#### Teacher Operations
```c
void edit_student_grade(StudentList* students, int student_id, float new_grade, Session* session) {
    if (!session_is_teacher(session)) {
        printf("Error: Teacher privileges required\n");
        return;
    }
    
    Student* student = student_list_find_by_id(students, student_id);
    if (student != NULL) {
        student->gpa = new_grade;
        printf("Grade updated successfully\n");
    } else {
        printf("Student not found\n");
    }
}
```

#### Student Operations
```c
void view_own_profile(StudentList* students, Session* session) {
    if (!session_is_student(session)) {
        printf("Error: Please log in\n");
        return;
    }
    
    Student* student = student_list_find_by_id(students, session->user_id);
    if (student != NULL) {
        printf("Your profile:\n");
        printf("Name: %s %s\n", student->first_name, student->last_name);
        printf("GPA: %.2f\n", student->gpa);
    } else {
        printf("Profile not found\n");
    }
}
```

---

## 💾 File Operations

### User Data Persistence

#### `int auth_save_users(UserList* list, const char* filename)`
**Purpose:** Saves user list to encrypted file
**Parameters:**
- `list`: UserList to save
- `filename`: Path to encrypted file
**Returns:** `1` if successful, `0` if failed

```c
if (auth_save_users(users, "data/users.enc")) {
    printf("Users saved successfully\n");
} else {
    printf("Failed to save users\n");
}
```

#### `int auth_load_users(UserList* list, const char* filename)`
**Purpose:** Loads user list from encrypted file
**Parameters:**
- `list`: UserList to populate
- `filename`: Path to encrypted file
**Returns:** `1` if successful, `0` if failed

```c
if (auth_load_users(users, "data/users.enc")) {
    printf("Users loaded successfully\n");
} else {
    printf("Failed to load users\n");
}
```

---

## 💻 Complete Usage Examples

### User Registration Process
```c
#include <stdio.h>
#include <string.h>
#include "auth.h"
#include "config.h"

int register_new_user(UserList* users) {
    char username[50];
    char email[MAX_EMAIL_LENGTH];
    char password[100];
    char confirm_password[100];
    UserRole role;
    
    printf("=== User Registration ===\n");
    
    // Get username
    printf("Username: ");
    scanf("%49s", username);
    
    // Check if username already exists
    if (user_list_find_by_username(users, username) != NULL) {
        printf("Error: Username already exists\n");
        return 0;
    }
    
    // Get email
    printf("Email: ");
    scanf("%149s", email);
    
    // Check if email already exists
    if (user_list_find_by_email(users, email) != NULL) {
        printf("Error: Email already registered\n");
        return 0;
    }
    
    // Get password with confirmation
    do {
        printf("Password: ");
        scanf("%99s", password);
        
        if (!auth_validate_password_strength(password)) {
            printf("Password too weak. Try again.\n");
            continue;
        }
        
        printf("Confirm password: ");
        scanf("%99s", confirm_password);
        
        if (strcmp(password, confirm_password) != 0) {
            printf("Passwords don't match. Try again.\n");
        }
    } while (strcmp(password, confirm_password) != 0);
    
    // Get role
    printf("Role (1=Admin, 2=Teacher, 3=Student): ");
    int role_input;
    scanf("%d", &role_input);
    role = (UserRole)role_input;
    
    // Register user
    if (auth_register(users, username, email, password, role)) {
        printf("Registration successful!\n");
        return 1;
    } else {
        printf("Registration failed\n");
        return 0;
    }
}
```

### Login Process
```c
Session* login_user(UserList* users) {
    char username[50];
    char password[100];
    Session* session = session_create();
    
    printf("=== User Login ===\n");
    
    printf("Username: ");
    scanf("%49s", username);
    
    printf("Password: ");
    scanf("%99s", password);
    
    if (auth_login(users, username, password, session)) {
        printf("Login successful!\n");
        printf("Welcome, %s (%s)\n", 
               session->username, 
               auth_role_to_string(session->role));
        return session;
    } else {
        printf("Login failed. Invalid username or password.\n");
        session_destroy(session);
        return NULL;
    }
}
```

### Complete Authentication Flow
```c
int main() {
    UserList* users = user_list_create();
    Session* session = NULL;
    
    // Load existing users
    if (!auth_load_users(users, "data/users.enc")) {
        printf("No existing users found. Creating admin account...\n");
        
        // Create default admin account
        auth_register(users, "admin", "admin@system.com", "admin123", ROLE_ADMIN);
        auth_save_users(users, "data/users.enc");
    }
    
    // Main menu loop
    int choice;
    do {
        printf("\n=== Student Management System ===\n");
        printf("1. Login\n");
        printf("2. Register\n");
        printf("3. Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                session = login_user(users);
                if (session != NULL) {
                    // User is logged in - show main menu
                    show_main_menu(session);
                    auth_logout(session);
                    session_destroy(session);
                    session = NULL;
                }
                break;
                
            case 2:
                register_new_user(users);
                auth_save_users(users, "data/users.enc");
                break;
                
            case 3:
                printf("Goodbye!\n");
                break;
                
            default:
                printf("Invalid choice\n");
        }
    } while (choice != 3);
    
    // Cleanup
    if (session != NULL) {
        session_destroy(session);
    }
    user_list_destroy(users);
    
    return 0;
}
```

---

## 🛡️ Security Best Practices

### 1. Password Security
```c
// Always hash passwords before storing
char password[] = "userpassword";
char salt[33];
char hash[65];

auth_generate_salt(salt);
auth_hash_password(password, salt, hash);

// Store hash and salt, never the plain password
strcpy(user.password_hash, hash);
strcpy(user.salt, salt);
```

### 2. Session Validation
```c
// Always validate session before operations
if (!auth_validate_session(session)) {
    printf("Session expired. Please log in again.\n");
    return;
}

// Check permissions
if (!session_is_admin(session)) {
    printf("Insufficient privileges\n");
    return;
}
```

### 3. Input Validation
```c
// Validate all user input
char username[50];
scanf("%49s", username);  // Limit input length

if (strlen(username) < 3) {
    printf("Username too short\n");
    return;
}
```

### 4. Error Handling
```c
// Never reveal sensitive information in error messages
if (!auth_login(users, username, password, session)) {
    printf("Login failed\n");  // Don't say "wrong password"
}
```

### 5. Memory Management
```c
// Always clean up allocated memory
Session* session = session_create();
// ... use session ...
session_destroy(session);  // Clean up
```

---

## 📊 Authentication Summary Table

| Function | Purpose | Parameters | Returns | Security Level |
|----------|---------|------------|---------|----------------|
| `auth_register` | Create new user | username, email, password, role | success/failure | High |
| `auth_login` | Authenticate user | username, password, session | success/failure | High |
| `auth_logout` | End user session | session | success/failure | Medium |
| `auth_change_password` | Update password | user_id, old_pass, new_pass | success/failure | High |
| `auth_validate_session` | Check session validity | session | valid/invalid | Medium |
| `session_is_admin` | Check admin privileges | session | admin/not admin | Medium |
| `session_is_teacher` | Check teacher privileges | session | teacher+/not teacher | Medium |
| `session_is_student` | Check user privileges | session | user/not user | Low |

---

## 🎯 Conclusion

The authentication system provides:
- **Secure password storage** with hashing and salting
- **Session management** for temporary access control
- **Role-based permissions** for different user types
- **File persistence** with encryption
- **Comprehensive validation** and error handling

This system ensures that only authorized users can access the student management system with appropriate permissions for their role.

---

*This documentation is part of the Student Management System project.*
*Last updated: 2024*
