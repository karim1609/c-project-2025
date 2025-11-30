#ifndef CONFIG_H
#define CONFIG_H

// Application configuration constants
#define APP_NAME "Student Management System"
#define APP_VERSION "1.0.0"
#define MAX_STUDENTS 1000
#define MAX_NAME_LENGTH 100
#define MAX_EMAIL_LENGTH 150
#define MAX_PHONE_LENGTH 20
#define MAX_ADDRESS_LENGTH 200
#define MAX_COURSE_LENGTH 50
#define MAX_CLUB_LENGTH 50
#define MAX_CLUBS 15 
// File paths
#define DATA_DIR "data/"
#define STUDENTS_FILE "students.enc"
#define USERS_FILE "users.enc"
#define GRADES_FILE "grades.enc"
#define CLUBS_FILE "clubs.enc"
#define ATTENDANCE_FILE "attendance.enc"
#define LOGS_FILE "logs.enc"
#define BACKUP_DIR "data/backup/"
#define KEYS_DIR "data/keys/"
#define MASTER_KEY_FILE "master.key"

// Encryption settings
#define AES_KEY_SIZE 32
#define AES_IV_SIZE 16
#define SALT_SIZE 32

// UI settings
#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800
#define THEME_DARK "dark"
#define THEME_LIGHT "light"

// User roles
typedef enum {
    ROLE_ADMIN = 1,
    ROLE_TEACHER = 2,
    ROLE_STUDENT = 3
} UserRole;

// Grade levels
typedef enum {
    GRADE_A = 4,
    GRADE_B = 3,
    GRADE_C = 2,
    GRADE_D = 1,
    GRADE_F = 0
} GradeLevel;

#endif // CONFIG_H
