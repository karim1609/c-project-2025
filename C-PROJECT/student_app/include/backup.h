#ifndef BACKUP_H
#define BACKUP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include "config.h"

// Backup result codes
typedef enum {
    BACKUP_SUCCESS = 0,
    BACKUP_ERROR_CREATE_DIR = -1,
    BACKUP_ERROR_COPY_FILE = -2,
    BACKUP_ERROR_COMPRESS = -3,
    BACKUP_ERROR_ENCRYPT = -4,
    BACKUP_ERROR_DISK_FULL = -5,
    BACKUP_ERROR_PERMISSION = -6,
    BACKUP_ERROR_INVALID_PATH = -7
} BackupResult;

// Backup information structure
typedef struct {
    char backup_name[256];
    char backup_path[512];
    time_t created_time;
    size_t backup_size;
    int file_count;
    char checksum[65];  // SHA-256 checksum
    int is_compressed;
    int is_encrypted;
} BackupInfo;

// Backup list structure
typedef struct {
    BackupInfo* backups;
    int count;
    int capacity;
} BackupList;

// Backup management functions
BackupList* backup_list_create(void);
void backup_list_destroy(BackupList* list);
int backup_list_add(BackupList* list, BackupInfo backup);
int backup_list_remove(BackupList* list, int index);
BackupInfo* backup_list_find_by_name(BackupList* list, const char* name);
BackupInfo* backup_list_find_by_date(BackupList* list, time_t date);

// Backup operations
BackupResult create_full_backup(const char* backup_name, const char* source_dir, const char* backup_dir);
BackupResult create_incremental_backup(const char* backup_name, const char* source_dir, const char* backup_dir);
BackupResult restore_backup(const char* backup_name, const char* backup_dir, const char* restore_dir);
BackupResult restore_file_from_backup(const char* backup_name, const char* file_path, const char* backup_dir, const char* restore_path);

// Individual file backup
BackupResult backup_file(const char* source_file, const char* backup_dir, const char* backup_name);
BackupResult restore_file(const char* backup_file, const char* restore_path);
BackupResult backup_directory(const char* source_dir, const char* backup_dir, const char* backup_name);

// Backup validation
int validate_backup_integrity(const char* backup_path);
int compare_backup_with_source(const char* backup_path, const char* source_path);
int verify_backup_checksum(const char* backup_path, const char* expected_checksum);

// Backup listing and management
int list_available_backups(const char* backup_dir, BackupList* backup_list);
void display_backup_info(BackupInfo* backup);
void display_all_backups(BackupList* backup_list);
int cleanup_old_backups(const char* backup_dir, int days_to_keep);
int delete_backup(const char* backup_name, const char* backup_dir);

// Automatic backup
typedef struct {
    int enabled;
    int interval_hours;
    int max_backups;
    int compress_backups;
    int encrypt_backups;
    char backup_dir[256];
    time_t last_backup_time;
} AutoBackupConfig;

AutoBackupConfig* auto_backup_config_create(void);
void auto_backup_config_destroy(AutoBackupConfig* config);
int auto_backup_config_load(AutoBackupConfig* config, const char* config_file);
int auto_backup_config_save(AutoBackupConfig* config, const char* config_file);

int start_auto_backup(AutoBackupConfig* config);
int stop_auto_backup(void);
int is_auto_backup_running(void);
int trigger_manual_backup(AutoBackupConfig* config);

// Backup compression
BackupResult compress_backup(const char* backup_path, const char* compressed_path);
BackupResult decompress_backup(const char* compressed_path, const char* backup_path);
int is_backup_compressed(const char* backup_path);

// Backup encryption
BackupResult encrypt_backup(const char* backup_path, const char* encrypted_path, const unsigned char* key);
BackupResult decrypt_backup(const char* encrypted_path, const char* backup_path, const unsigned char* key);
int is_backup_encrypted(const char* backup_path);

// Backup scheduling
typedef struct {
    int hour;
    int minute;
    int day_of_week;  // 0-6, 0=Sunday
    int day_of_month; // 1-31, 0=any
} BackupSchedule;

BackupSchedule* backup_schedule_create(int hour, int minute, int day_of_week, int day_of_month);
void backup_schedule_destroy(BackupSchedule* schedule);
int is_backup_time(BackupSchedule* schedule);
int schedule_backup(BackupSchedule* schedule, AutoBackupConfig* config);

// Backup reports
int generate_backup_report(BackupList* backup_list, const char* filename);
int generate_backup_status_report(const char* backup_dir, const char* filename);
int generate_backup_history_report(const char* backup_dir, const char* filename);

// File operations
int backup_list_save_to_file(BackupList* list, const char* filename);
int backup_list_load_from_file(BackupList* list, const char* filename);

// Utility functions
const char* backup_result_to_string(BackupResult result);
time_t get_backup_creation_time(const char* backup_path);
size_t get_backup_size(const char* backup_path);
int calculate_backup_checksum(const char* backup_path, char* checksum);
int compare_backup_dates(const BackupInfo* backup1, const BackupInfo* backup2);

// Backup sorting
void backup_list_sort_by_date(BackupList* list);
void backup_list_sort_by_size(BackupList* list);
void backup_list_sort_by_name(BackupList* list);

// Constants
#define BACKUP_NAME_FORMAT "backup_%Y%m%d_%H%M%S"
#define MAX_BACKUP_NAME_LENGTH 100
#define MAX_BACKUP_PATH_LENGTH 512
#define DEFAULT_BACKUP_RETENTION_DAYS 30
#define MAX_AUTO_BACKUPS 10
#define MAX_BACKUP_CAPACITY 1000

// Backup types
#define BACKUP_TYPE_FULL "full"
#define BACKUP_TYPE_INCREMENTAL "incremental"
#define BACKUP_TYPE_DIFFERENTIAL "differential"

// Backup status
#define BACKUP_STATUS_SUCCESS "success"
#define BACKUP_STATUS_FAILED "failed"
#define BACKUP_STATUS_IN_PROGRESS "in_progress"
#define BACKUP_STATUS_PARTIAL "partial"

#endif // BACKUP_H
