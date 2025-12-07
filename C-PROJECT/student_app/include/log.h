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
    int enable_console_output;
    int enable_file_output;
    char log_directory[256];
    char log_filename[256];
} LogConfig;

// Core logging functions
int log_init(LogConfig* config);
void log_cleanup(void);
int log_message(LogLevel level, const char* module, const char* message, ...);

// Convenience wrappers
int log_debug(const char* module, const char* message, ...);
int log_info(const char* module, const char* message, ...);
int log_warning(const char* module, const char* message, ...);
int log_error(const char* module, const char* message, ...);
int log_critical(const char* module, const char* message, ...);

// Log list management
LogList* log_list_create(void);
void log_list_destroy(LogList* list);
int log_list_add(LogList* list, LogEntry entry);
void log_list_sort_by_timestamp(LogList* list);

// Display and filtering
void log_display(LogList* list, LogLevel level, const char* module, int user_id);
LogList* log_filter(LogList* list, LogLevel level, const char* module, int user_id, time_t start_date, time_t end_date);

// File operations
int log_list_save(LogList* list, const char* filename);
int log_list_load(LogList* list, const char* filename);
int log_export_csv(LogList* list, const char* filename);

// Configuration
LogConfig* log_config_create(void);
void log_config_destroy(LogConfig* config);

// Utilities
const char* log_level_to_string(LogLevel level);
time_t log_get_current_timestamp(void);

// Constants
#define LOG_MAX_MESSAGE_LENGTH 500
#define LOG_MAX_MODULE_LENGTH 50
#define DEFAULT_LOG_FILE_SIZE 10485760  // 10MB

// Log modules
#define LOG_MODULE_SYSTEM "SYSTEM"
#define LOG_MODULE_AUTH "AUTH"
#define LOG_MODULE_STUDENT "STUDENT"
#define LOG_MODULE_GRADE "GRADE"
#define LOG_MODULE_ATTENDANCE "ATTENDANCE"

#endif // LOG_H
