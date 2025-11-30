#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>
#include "config.h"

// Log levels
typedef enum {
    LOG_LEVEL_DEBUG = 0,
    LOG_LEVEL_INFO = 1,
    LOG_LEVEL_WARNING = 2,
    LOG_LEVEL_ERROR = 3,
    LOG_LEVEL_CRITICAL = 4
} LogLevel;

// Log entry structure
typedef struct {
    time_t timestamp;
    LogLevel level;
    char module[50];
    char message[500];
    int user_id;
    char username[50];
    char ip_address[16];
} LogEntry;

// Log list structure
typedef struct {
    LogEntry* entries;
    int count;
    int capacity;
} LogList;

// Log configuration
typedef struct {
    LogLevel min_level;
    int max_file_size;
    int max_files;
    int enable_console_output;
    int enable_file_output;
    int enable_encryption;
    char log_directory[256];
    char log_filename[256];
    int auto_rotate;
    int compress_old_logs;
} LogConfig;

// Log management functions
LogList* log_list_create(void);
void log_list_destroy(LogList* list);
int log_list_add(LogList* list, LogEntry entry);
int log_list_remove(LogList* list, int index);
LogEntry* log_list_find_by_timestamp(LogList* list, time_t timestamp);
LogEntry* log_list_find_by_level(LogList* list, LogLevel level);

// Logging functions
int log_init(LogConfig* config);
void log_cleanup(void);
int log_message(LogLevel level, const char* module, const char* message, ...);
int log_debug(const char* module, const char* message, ...);
int log_info(const char* module, const char* message, ...);
int log_warning(const char* module, const char* message, ...);
int log_error(const char* module, const char* message, ...);
int log_critical(const char* module, const char* message, ...);

// User activity logging
int log_user_login(int user_id, const char* username, const char* ip_address);
int log_user_logout(int user_id, const char* username);
int log_user_action(int user_id, const char* username, const char* action, const char* details);
int log_user_error(int user_id, const char* username, const char* error_message);

// System event logging
int log_system_startup(void);
int log_system_shutdown(void);
int log_system_error(const char* error_message);
int log_backup_created(const char* backup_name);
int log_backup_restored(const char* backup_name);
int log_data_exported(const char* export_type, const char* filename);
int log_data_imported(const char* import_type, const char* filename);

// Student management logging
int log_student_added(int student_id, const char* student_name, int user_id);
int log_student_updated(int student_id, const char* student_name, int user_id);
int log_student_deleted(int student_id, const char* student_name, int user_id);
int log_student_viewed(int student_id, const char* student_name, int user_id);

// Grade management logging
int log_grade_added(int student_id, int course_id, float grade, int user_id);
int log_grade_updated(int student_id, int course_id, float old_grade, float new_grade, int user_id);
int log_grade_deleted(int student_id, int course_id, float grade, int user_id);

// Attendance logging
int log_attendance_marked(int student_id, int course_id, int status, int user_id);
int log_attendance_updated(int student_id, int course_id, int old_status, int new_status, int user_id);

// Log display and filtering
void log_display_all(LogList* list);
void log_display_by_level(LogList* list, LogLevel level);
void log_display_by_module(LogList* list, const char* module);
void log_display_by_user(LogList* list, int user_id);
void log_display_by_date_range(LogList* list, time_t start_date, time_t end_date);
void log_display_entry(LogEntry* entry);

// Log filtering
LogList* log_filter_by_level(LogList* list, LogLevel level);
LogList* log_filter_by_module(LogList* list, const char* module);
LogList* log_filter_by_user(LogList* list, int user_id);
LogList* log_filter_by_date_range(LogList* list, time_t start_date, time_t end_date);
LogList* log_filter_by_keyword(LogList* list, const char* keyword);

// Log analysis
typedef struct {
    int total_entries;
    int entries_by_level[5];
    int entries_by_hour[24];
    int entries_by_day[7];
    int most_active_user;
    int most_logged_module;
    time_t first_entry;
    time_t last_entry;
} LogStatistics;

LogStatistics* calculate_log_statistics(LogList* list);
void display_log_statistics(LogStatistics* stats);
void free_log_statistics(LogStatistics* stats);

// Log reports
int generate_log_report(LogList* list, const char* filename);
int generate_user_activity_report(LogList* list, int user_id, const char* filename);
int generate_system_log_report(LogList* list, const char* filename);
int generate_error_log_report(LogList* list, const char* filename);
int generate_daily_log_summary(LogList* list, time_t date, const char* filename);

// File operations
int log_list_save_to_file(LogList* list, const char* filename);
int log_list_load_from_file(LogList* list, const char* filename);
int log_export_to_csv(LogList* list, const char* filename);
int log_export_to_json(LogList* list, const char* filename);

// Log rotation and cleanup
int log_rotate_file(const char* log_file, int max_size);
int log_cleanup_old_files(const char* log_dir, int days_to_keep);
int log_compress_old_files(const char* log_dir);
int log_delete_old_files(const char* log_dir, int days_to_keep);

// Log configuration
LogConfig* log_config_create(void);
void log_config_destroy(LogConfig* config);
int log_config_load(LogConfig* config, const char* config_file);
int log_config_save(LogConfig* config, const char* config_file);
int log_config_set_level(LogConfig* config, LogLevel level);
int log_config_set_file_size(LogConfig* config, int max_size);
int log_config_set_max_files(LogConfig* config, int max_files);

// Log sorting
void log_list_sort_by_timestamp(LogList* list);
void log_list_sort_by_level(LogList* list);
void log_list_sort_by_module(LogList* list);
void log_list_sort_by_user(LogList* list);

// Utility functions
const char* log_level_to_string(LogLevel level);
LogLevel string_to_log_level(const char* level_str);
const char* log_level_to_color(LogLevel level);
time_t log_get_current_timestamp(void);
int log_format_timestamp(time_t timestamp, char* formatted_time, size_t size);

// Log validation
int log_validate_level(LogLevel level);
int log_validate_module(const char* module);
int log_validate_message(const char* message);
int log_validate_user_id(int user_id);

// Constants
#define LOG_MAX_MESSAGE_LENGTH 500
#define LOG_MAX_MODULE_LENGTH 50
#define LOG_MAX_USERNAME_LENGTH 50
#define LOG_MAX_FILENAME_LENGTH 256
#define LOG_MAX_DIRECTORY_LENGTH 256

#define DEFAULT_LOG_FILE_SIZE 10485760  // 10MB
#define DEFAULT_MAX_LOG_FILES 5
#define DEFAULT_LOG_RETENTION_DAYS 30

// Log modules
#define LOG_MODULE_SYSTEM "SYSTEM"
#define LOG_MODULE_AUTH "AUTH"
#define LOG_MODULE_STUDENT "STUDENT"
#define LOG_MODULE_GRADE "GRADE"
#define LOG_MODULE_ATTENDANCE "ATTENDANCE"
#define LOG_MODULE_CLUB "CLUB"
#define LOG_MODULE_BACKUP "BACKUP"
#define LOG_MODULE_FILE "FILE"
#define LOG_MODULE_UI "UI"
#define LOG_MODULE_CONFIG "CONFIG"

// Log level strings
#define LOG_LEVEL_DEBUG_STR "DEBUG"
#define LOG_LEVEL_INFO_STR "INFO"
#define LOG_LEVEL_WARNING_STR "WARNING"
#define LOG_LEVEL_ERROR_STR "ERROR"
#define LOG_LEVEL_CRITICAL_STR "CRITICAL"

#endif // LOG_H
