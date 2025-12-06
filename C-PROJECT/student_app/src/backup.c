#include "backup.h"
#include "file_manager.h"
#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <openssl/sha.h>

BackupList* backup_list_create(void){
    BackupList* list = (BackupList*)malloc(sizeof(BackupList));
    if (!list) {
        return NULL;
    }
    list->backups = NULL;
    list->count = 0;
    list->capacity = 0;
    return list;
}

void backup_list_destroy(BackupList* list) {
    if (!list) {
        return;
    }
    if (list->backups) {
        free(list->backups);
    }
    free(list);
}
int   backup_list_add(BackupList* list, BackupInfo backup) {
    if (!list) {
        fprintf(stderr, "Erreur : liste invalide\n");
        return -1;
        }
    if (list->count >= list->capacity) {
        int new_capacity = list->capacity * 2 + 1;
        if (new_capacity > MAX_BACKUP_CAPACITY) {
            fprintf(stderr, "capacite maximale atteinte\n");
            return -1;
        }
        list->capacity = new_capacity;
        BackupInfo* new_backups = (BackupInfo*)realloc(list->backups, sizeof(BackupInfo) * list->capacity);
        if (!new_backups) {
            return -1;
        }
        list->backups = new_backups;
    }

    list->backups[list->count] = backup;
    list->count++;
    return 0;
}

int backup_list_remove(BackupList* list, int index) {
    if (!list) {
        return -1;
    }
    if (index < 0 || index >= list->count) {
        return -1;
    }
    for (int i = index; i < list->count - 1; i++) {
        list->backups[i] = list->backups[i + 1];
    }
    list->count--;
    return 0;
}
BackupInfo* backup_list_find_by_name(BackupList* list, const char* name) {
    if (!list) {
        return NULL;
    }
    for (int i = 0; i < list->count; i++) {
        if (strcmp(list->backups[i].backup_name, name) == 0) {
            return &(list->backups[i]);
        }  
    }
    return NULL;
}
BackupInfo* backup_list_find_by_date(BackupList* list, time_t date) {
    if (!list) {
        return NULL;
    }
    for (int i = 0; i < list->count; i++) {
        if (list->backups[i].created_time == date) {
            return &(list->backups[i]);
        }
    }
    return NULL;
}
/////////////////////////////////////////////////////////////////////////////




// Helper function to get file size
static size_t get_file_size(const char* filename) {
    struct stat st;
    if (stat(filename, &st) == 0)
        return st.st_size;
    return 0;
}

// Helper function to copy a single file
static int copy_file(const char* source, const char* dest) {
    FILE* src = fopen(source, "rb");
    if (!src) return -1;
    
    FILE* dst = fopen(dest, "wb");
    if (!dst) {
        fclose(src);
        return -1;
    }
    
    char buffer[8192];
    size_t bytes;
    
    while ((bytes = fread(buffer, 1, sizeof(buffer), src)) > 0) {
        if (fwrite(buffer, 1, bytes, dst) != bytes) {
            fclose(src);
            fclose(dst);
            return -1;
        }
    }
    
    fclose(src);
    fclose(dst);
    return 0;
}

// Helper function to copy directory recursively
static int copy_directory_recursive(const char* source, const char* dest, int* file_count, size_t* total_size) {
    DIR* dir = opendir(source);
    if (!dir) return -1;
    
    create_directory(dest);
    
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;
        
        char src_path[1024];
        char dst_path[1024];
        snprintf(src_path, sizeof(src_path), "%s/%s", source, entry->d_name);
        snprintf(dst_path, sizeof(dst_path), "%s/%s", dest, entry->d_name);
        
        struct stat st;
        if (stat(src_path, &st) == 0) {
            if (S_ISDIR(st.st_mode)) {
                if (copy_directory_recursive(src_path, dst_path, file_count, total_size) != 0) {
                    closedir(dir);
                    return -1;
                }
            } else {
                if (copy_file(src_path, dst_path) != 0) {
                    closedir(dir);
                    return -1;
                }
                (*file_count)++;
                *total_size += get_file_size(src_path);
            }
        }
    }
    
    closedir(dir);
    return 0;
}

// Helper function to calculate SHA-256 checksum of directory
static int calculate_directory_checksum(const char* dir_path, char* checksum_out) {
    // This is a simplified version - calculates checksum of directory path
    // For production, you'd want to hash all file contents
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    
    if (!SHA256_Init(&sha256))
        return -1;
    
    SHA256_Update(&sha256, dir_path, strlen(dir_path));
    SHA256_Final(hash, &sha256);
    
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(checksum_out + (i * 2), "%02x", hash[i]);
    }
    checksum_out[64] = '\0';
    
    return 0;
}

// Helper function to find the latest backup with the same name
static BackupInfo* find_latest_backup(const char* backup_name, const char* backup_dir) {
    if (!backup_name || !backup_dir) {
        return NULL;
    }
    
    DIR* dir = opendir(backup_dir);
    if (!dir) {
        return NULL;
    }
    
    BackupInfo* latest = NULL;
    time_t latest_time = 0;
    
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        
        // Check if this entry starts with backup_name
        if (strncmp(entry->d_name, backup_name, strlen(backup_name)) != 0) {
            continue;
        }
        
        char backup_path[512];
        snprintf(backup_path, sizeof(backup_path), "%s/%s", backup_dir, entry->d_name);
        
        // Check if it's a directory
        struct stat st;
        if (stat(backup_path, &st) != 0 || !S_ISDIR(st.st_mode)) {
            continue;
        }
        
        // Use directory modification time to find latest backup
        // This is more portable than parsing metadata files
        if (st.st_mtime > latest_time) {
            latest_time = st.st_mtime;
            if (!latest) {
                latest = (BackupInfo*)malloc(sizeof(BackupInfo));
            }
            if (latest) {
                strncpy(latest->backup_path, backup_path, sizeof(latest->backup_path) - 1);
                latest->backup_path[sizeof(latest->backup_path) - 1] = '\0';
                latest->created_time = st.st_mtime;
            }
        }
    }
    
    closedir(dir);
    return latest;
}

// Helper function to copy only modified files recursively
static int copy_modified_files_recursive(const char* source, const char* dest, 
                                         const char* base_source_dir, const char* base_backup_path, 
                                         time_t last_backup_time, int* file_count, size_t* total_size) {
    DIR* dir = opendir(source);
    if (!dir) return -1;
    
    create_directory(dest);
    
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        
        char src_path[1024];
        char dst_path[1024];
        snprintf(src_path, sizeof(src_path), "%s/%s", source, entry->d_name);
        snprintf(dst_path, sizeof(dst_path), "%s/%s", dest, entry->d_name);
        
        struct stat src_st;
        if (stat(src_path, &src_st) != 0) {
            continue;
        }
        
        if (S_ISDIR(src_st.st_mode)) {
            // Recursively process subdirectories
            if (copy_modified_files_recursive(src_path, dst_path, base_source_dir, base_backup_path, 
                                             last_backup_time, file_count, total_size) != 0) {
                closedir(dir);
                return -1;
            }
        } else {
            // Check if file needs to be backed up
            int needs_backup = 0;
            
            if (last_backup_time == 0 || strlen(base_backup_path) == 0) {
                // No previous backup, backup everything
                needs_backup = 1;
            } else {
                // Check if file is newer than last backup
                if (src_st.st_mtime > last_backup_time) {
                    needs_backup = 1;
                } else {
                    // Check if file exists in previous backup
                    char backup_file_path[1024];
                    // Calculate relative path from base source directory
                    size_t base_source_len = strlen(base_source_dir);
                    size_t src_path_len = strlen(src_path);
                    if (src_path_len > base_source_len && 
                        strncmp(src_path, base_source_dir, base_source_len) == 0) {
                        const char* rel_path = src_path + base_source_len;
                        // Skip leading '/' if present
                        if (rel_path[0] == '/') {
                            rel_path++;
                        }
                        snprintf(backup_file_path, sizeof(backup_file_path), "%s/%s", base_backup_path, rel_path);
                        
                        struct stat backup_st;
                        if (stat(backup_file_path, &backup_st) != 0) {
                            // File doesn't exist in backup, needs backup
                            needs_backup = 1;
                        }
                    } else {
                        // Can't determine relative path, backup it
                        needs_backup = 1;
                    }
                }
            }
            
            if (needs_backup) {
                if (copy_file(src_path, dst_path) != 0) {
                    closedir(dir);
                    return -1;
                }
                (*file_count)++;
                *total_size += get_file_size(src_path);
            }
        }
    }
    
    closedir(dir);
    return 0;
}

// Main backup function
BackupResult create_full_backup(const char* backup_name, const char* source_dir, const char* backup_dir) {
    // Validate parameters
    if (!backup_name || !source_dir || !backup_dir) {
        return BACKUP_ERROR_INVALID_PARAMETERS;
    }
    
    if (strlen(backup_name) == 0 || strlen(source_dir) == 0 || strlen(backup_dir) == 0) {
        return BACKUP_ERROR_INVALID_PARAMETERS;
    }
    
    // Check if source directory exists
    if (!directory_exists(source_dir)) {
        fprintf(stderr, "Error: Source directory '%s' does not exist\n", source_dir);
        return BACKUP_ERROR_SOURCE_DIR_NOT_FOUND;
    }
    
    // Create backup directory if it doesn't exist
    if (!directory_exists(backup_dir)) {
        if (create_directory(backup_dir) != 0) {
            fprintf(stderr, "Error: Failed to create backup directory '%s'\n", backup_dir);
            return BACKUP_ERROR_CREATE_DIR;
        }
    }
    
    // Create timestamped backup path
    char backup_path[512];
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    snprintf(backup_path, sizeof(backup_path), "%s/%s_%04d%02d%02d_%02d%02d%02d",
             backup_dir, backup_name,
             t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
             t->tm_hour, t->tm_min, t->tm_sec);
    
    // Copy files
    int file_count = 0;
    size_t backup_size = 0;
    
    printf("Creating backup: %s\n", backup_path);
    printf("Source: %s\n", source_dir);
    
    if (copy_directory_recursive(source_dir, backup_path, &file_count, &backup_size) != 0) {
        fprintf(stderr, "Error: Failed to copy files\n");
        return BACKUP_ERROR_COPY_FILE;
    }
    
    // Calculate checksum
    char checksum[65];
    if (calculate_directory_checksum(backup_path, checksum) != 0) {
        fprintf(stderr, "Warning: Failed to calculate checksum\n");
        strcpy(checksum, "N/A");
    }
    
    // Create BackupInfo structure
    BackupInfo info;
    strncpy(info.backup_name, backup_name, sizeof(info.backup_name) - 1);
    info.backup_name[sizeof(info.backup_name) - 1] = '\0';
    
    strncpy(info.backup_path, backup_path, sizeof(info.backup_path) - 1);
    info.backup_path[sizeof(info.backup_path) - 1] = '\0';
    
    info.created_time = now;
    info.backup_size = backup_size;
    info.file_count = file_count;
    
    strncpy(info.checksum, checksum, sizeof(info.checksum) - 1);
    info.checksum[sizeof(info.checksum) - 1] = '\0';
    
    info.is_compressed = 0;
    info.is_encrypted = 0;
    
    // Save backup metadata to a file
    char metadata_file[600];
    snprintf(metadata_file, sizeof(metadata_file), "%s/backup_info.txt", backup_path);
    
    FILE* meta = fopen(metadata_file, "w");
    if (meta) {
        fprintf(meta, "Backup Name: %s\n", info.backup_name);
        fprintf(meta, "Backup Path: %s\n", info.backup_path);
        fprintf(meta, "Created: %s", ctime(&info.created_time));
        fprintf(meta, "Size: %zu bytes\n", info.backup_size);
        fprintf(meta, "File Count: %d\n", info.file_count);
        fprintf(meta, "Checksum: %s\n", info.checksum);
        fprintf(meta, "Compressed: %s\n", info.is_compressed ? "Yes" : "No");
        fprintf(meta, "Encrypted: %s\n", info.is_encrypted ? "Yes" : "No");
        fclose(meta);
    }
    
    printf("\nBackup completed successfully!\n");
    printf("Location: %s\n", backup_path);
    printf("Files backed up: %d\n", file_count);
    printf("Total size: %zu bytes\n", backup_size);
    printf("Checksum: %s\n", checksum);
    
    return BACKUP_SUCCESS;
}


BackupResult create_incremental_backup(const char* backup_name, const char* source_dir, const char* backup_dir) {
    // Validate parameters
    if (!backup_name || !source_dir || !backup_dir) {
        return BACKUP_ERROR_INVALID_PARAMETERS;
    }
    
    if (strlen(backup_name) == 0 || strlen(source_dir) == 0 || strlen(backup_dir) == 0) {
        return BACKUP_ERROR_INVALID_PARAMETERS;
    }
    
    // Check if source directory exists
    if (!directory_exists(source_dir)) {
        fprintf(stderr, "Error: Source directory '%s' does not exist\n", source_dir);
        return BACKUP_ERROR_SOURCE_DIR_NOT_FOUND;
    }
    
    // Create backup directory if it doesn't exist
    if (!directory_exists(backup_dir)) {
        if (create_directory(backup_dir) != 0) {
            fprintf(stderr, "Error: Failed to create backup directory '%s'\n", backup_dir);
            return BACKUP_ERROR_CREATE_DIR;
        }
    }
    
    // Find the latest backup with the same name
    BackupInfo* last_backup = find_latest_backup(backup_name, backup_dir);
    time_t last_backup_time = 0;
    char last_backup_path[512] = {0};
    
    if (last_backup) {
        last_backup_time = last_backup->created_time;
        strncpy(last_backup_path, last_backup->backup_path, sizeof(last_backup_path) - 1);
        last_backup_path[sizeof(last_backup_path) - 1] = '\0';
        printf("Found previous backup: %s (from %s)\n", last_backup_path, ctime(&last_backup_time));
    } else {
        printf("No previous backup found. Creating first incremental backup (will backup all files).\n");
    }
    
    // Create timestamped backup path
    char backup_path[512];
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    snprintf(backup_path, sizeof(backup_path), "%s/%s_inc_%04d%02d%02d_%02d%02d%02d",
             backup_dir, backup_name,
             t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
             t->tm_hour, t->tm_min, t->tm_sec);
    
    // Copy only modified files
    int file_count = 0;
    size_t backup_size = 0;
    
    printf("Creating incremental backup: %s\n", backup_path);
    printf("Source: %s\n", source_dir);
    
    if (copy_modified_files_recursive(source_dir, backup_path, source_dir, last_backup_path, 
                                     last_backup_time, &file_count, &backup_size) != 0) {
        fprintf(stderr, "Error: Failed to copy modified files\n");
        if (last_backup) {
            free(last_backup);
        }
        return BACKUP_ERROR_COPY_FILE;
    }
    
    // Free the last_backup if it was allocated
    if (last_backup) {
        free(last_backup);
    }
    
    // Calculate checksum
    char checksum[65];
    if (calculate_directory_checksum(backup_path, checksum) != 0) {
        fprintf(stderr, "Warning: Failed to calculate checksum\n");
        strcpy(checksum, "N/A");
    }
    
    // Create BackupInfo structure
    BackupInfo info;
    strncpy(info.backup_name, backup_name, sizeof(info.backup_name) - 1);
    info.backup_name[sizeof(info.backup_name) - 1] = '\0';
    
    strncpy(info.backup_path, backup_path, sizeof(info.backup_path) - 1);
    info.backup_path[sizeof(info.backup_path) - 1] = '\0';
    
    info.created_time = now;
    info.backup_size = backup_size;
    info.file_count = file_count;
    
    strncpy(info.checksum, checksum, sizeof(info.checksum) - 1);
    info.checksum[sizeof(info.checksum) - 1] = '\0';
    
    info.is_compressed = 0;
    info.is_encrypted = 0;
    
    // Save backup metadata to a file
    char metadata_file[600];
    snprintf(metadata_file, sizeof(metadata_file), "%s/backup_info.txt", backup_path);
    
    FILE* meta = fopen(metadata_file, "w");
    if (meta) {
        fprintf(meta, "Backup Name: %s\n", info.backup_name);
        fprintf(meta, "Backup Type: Incremental\n");
        fprintf(meta, "Backup Path: %s\n", info.backup_path);
        fprintf(meta, "Created: %s", ctime(&info.created_time));
        fprintf(meta, "Size: %zu bytes\n", info.backup_size);
        fprintf(meta, "File Count: %d\n", info.file_count);
        fprintf(meta, "Checksum: %s\n", info.checksum);
        fprintf(meta, "Compressed: %s\n", info.is_compressed ? "Yes" : "No");
        fprintf(meta, "Encrypted: %s\n", info.is_encrypted ? "Yes" : "No");
        if (last_backup_time > 0) {
            fprintf(meta, "Based on backup from: %s", ctime(&last_backup_time));
        }
        fclose(meta);
    }
    
    printf("\nIncremental backup completed successfully!\n");
    printf("Location: %s\n", backup_path);
    printf("Files backed up: %d\n", file_count);
    printf("Total size: %zu bytes\n", backup_size);
    printf("Checksum: %s\n", checksum);
    
    return BACKUP_SUCCESS;
}
// Fonction helper pour supprimer un répertoire récursivement (optionnel, pour nettoyage)
static int remove_directory_recursive(const char* path) {
    DIR* dir = opendir(path);
    if (!dir) return -1;
    
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;
        
        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);
        
        struct stat st;
        if (stat(full_path, &st) == 0) {
            if (S_ISDIR(st.st_mode)) {
                remove_directory_recursive(full_path);
            } else {
                remove(full_path);
            }
        }
    }
    
    closedir(dir);
    rmdir(path);
    return 0;
}

// Fonction pour trouver le chemin complet du backup
static int find_backup_path(const char* backup_name, const char* backup_dir, char* backup_path_out, size_t path_size) {
    DIR* dir = opendir(backup_dir);
    if (!dir) {
        fprintf(stderr, "Error: Cannot open backup directory '%s'\n", backup_dir);
        return -1;
    }
    
    struct dirent* entry;
    int found = 0;
    
    // Chercher un répertoire qui commence par backup_name
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        
        // Vérifier si le nom commence par backup_name
        if (strncmp(entry->d_name, backup_name, strlen(backup_name)) == 0) {
            char test_path[512];
            snprintf(test_path, sizeof(test_path), "%s/%s", backup_dir, entry->d_name);
            
            // Vérifier que c'est bien un répertoire valide
            struct stat st;
            if (stat(test_path, &st) == 0 && S_ISDIR(st.st_mode)) {
                snprintf(backup_path_out, path_size, "%s/%s", backup_dir, entry->d_name);
                found = 1;
                break;
            }
        }
    }
    
    closedir(dir);
    
    if (!found) {
        fprintf(stderr, "Error: Backup '%s' not found in '%s'\n", backup_name, backup_dir);
        return -1;
    }
    
    return 0;
}

// Fonction pour vérifier l'intégrité du backup via le checksum
static int verify_backup_integrity(const char* backup_path, char* stored_checksum) {
    char metadata_file[600];
    snprintf(metadata_file, sizeof(metadata_file), "%s/backup_info.txt", backup_path);
    
    FILE* meta = fopen(metadata_file, "r");
    if (!meta) {
        fprintf(stderr, "Warning: Cannot open backup metadata file\n");
        return 0; // Continue anyway
    }
    
    char line[512];
    int checksum_found = 0;
    
    while (fgets(line, sizeof(line), meta)) {
        if (strncmp(line, "Checksum: ", 10) == 0) {
            // Extraire le checksum
            char* checksum_start = line + 10; // Après "Checksum: "
            // Supprimer le newline et les espaces en fin
            char* newline = strchr(checksum_start, '\n');
            if (newline) *newline = '\0';
            // Supprimer les espaces en fin
            size_t len = strlen(checksum_start);
            while (len > 0 && (checksum_start[len - 1] == ' ' || checksum_start[len - 1] == '\t')) {
                checksum_start[--len] = '\0';
            }
            
            strncpy(stored_checksum, checksum_start, 64);
            stored_checksum[64] = '\0';
            checksum_found = 1;
            break;
        }
    }
    
    fclose(meta);
    return checksum_found;
}

// Fonction pour copier un répertoire en excluant les fichiers metadata
static int restore_directory_recursive(const char* source, const char* dest, int* file_count, size_t* total_size) {
    DIR* dir = opendir(source);
    if (!dir) return -1;
    
    create_directory(dest);
    
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;
        
        // Ignorer le fichier de metadata
        if (strcmp(entry->d_name, "backup_info.txt") == 0)
            continue;
        
        char src_path[1024];
        char dst_path[1024];
        snprintf(src_path, sizeof(src_path), "%s/%s", source, entry->d_name);
        snprintf(dst_path, sizeof(dst_path), "%s/%s", dest, entry->d_name);
        
        struct stat st;
        if (stat(src_path, &st) == 0) {
            if (S_ISDIR(st.st_mode)) {
                if (restore_directory_recursive(src_path, dst_path, file_count, total_size) != 0) {
                    closedir(dir);
                    return -1;
                }
            } else {
                if (copy_file(src_path, dst_path) != 0) {
                    closedir(dir);
                    return -1;
                }
                (*file_count)++;
                *total_size += get_file_size(src_path);
            }
        }
    }
    
    closedir(dir);
    return 0;
}

// Fonction principale de restauration
BackupResult restore_backup(const char* backup_name, const char* backup_dir, const char* restore_dir) {
    // Validation des paramètres
    if (!backup_name || !backup_dir || !restore_dir) {
        fprintf(stderr, "Error: Invalid parameters (NULL pointer)\n");
        return BACKUP_ERROR_INVALID_PARAMETERS;
    }
    
    if (strlen(backup_name) == 0 || strlen(backup_dir) == 0 || strlen(restore_dir) == 0) {
        fprintf(stderr, "Error: Invalid parameters (empty string)\n");
        return BACKUP_ERROR_INVALID_PARAMETERS;
    }
    
    // Vérifier que le répertoire de backup existe
    if (!directory_exists(backup_dir)) {
        fprintf(stderr, "Error: Backup directory '%s' does not exist\n", backup_dir);
        return BACKUP_ERROR_SOURCE_DIR_NOT_FOUND;
    }
    
    // Trouver le chemin complet du backup
    char backup_path[512];
    if (find_backup_path(backup_name, backup_dir, backup_path, sizeof(backup_path)) != 0) {
        return BACKUP_ERROR_SOURCE_DIR_NOT_FOUND;
    }
    
    printf("Found backup: %s\n", backup_path);
    
    // Vérifier l'intégrité du backup (optionnel mais recommandé)
    char stored_checksum[65];
    if (verify_backup_integrity(backup_path, stored_checksum)) {
        printf("Backup checksum: %s\n", stored_checksum);
        // Vous pouvez ajouter une vérification réelle ici si nécessaire
    }
    
    // Créer le répertoire de restauration s'il n'existe pas
    if (!directory_exists(restore_dir)) {
        if (create_directory(restore_dir) != 0) {
            fprintf(stderr, "Error: Failed to create restore directory '%s'\n", restore_dir);
            return BACKUP_ERROR_CREATE_DIR;
        }
    }
    
    // Restaurer les fichiers
    int file_count = 0;
    size_t total_size = 0;
    
    printf("Restoring backup to: %s\n", restore_dir);
    printf("Source: %s\n", backup_path);
    
    if (restore_directory_recursive(backup_path, restore_dir, &file_count, &total_size) != 0) {
        fprintf(stderr, "Error: Failed to restore files\n");
        return BACKUP_ERROR_COPY_FILE;
    }
    
    // Afficher le résumé
    printf("\nRestore completed successfully!\n");
    printf("Location: %s\n", restore_dir);
    printf("Files restored: %d\n", file_count);
    printf("Total size: %zu bytes\n", total_size);
    
    // Lire et afficher les informations du backup
    char metadata_file[600];
    snprintf(metadata_file, sizeof(metadata_file), "%s/backup_info.txt", backup_path);
    
    FILE* meta = fopen(metadata_file, "r");
    if (meta) {
        printf("\n--- Backup Information ---\n");
        char line[512];
        while (fgets(line, sizeof(line), meta)) {
            printf("%s", line);
        }
        fclose(meta);
    }
    
    return BACKUP_SUCCESS;
}


    
BackupResult restore_file_from_backup(const char* backup_name, const char* file_path, const char* backup_dir, const char* restore_path) {
    // Validation des paramètres
    if (!backup_name || !file_path || !backup_dir || !restore_path) {
        fprintf(stderr, "Error: Invalid parameters (NULL pointer)\n");
        return BACKUP_ERROR_INVALID_PARAMETERS;
    }
    
    if (strlen(backup_name) == 0 || strlen(file_path) == 0 || 
        strlen(backup_dir) == 0 || strlen(restore_path) == 0) {
        fprintf(stderr, "Error: Invalid parameters (empty string)\n");
        return BACKUP_ERROR_INVALID_PARAMETERS;
    }
    
    // Vérifier que le répertoire de backup existe
    if (!directory_exists(backup_dir)) {
        fprintf(stderr, "Error: Backup directory '%s' does not exist\n", backup_dir);
        return BACKUP_ERROR_SOURCE_DIR_NOT_FOUND;
    }
    
    // Trouver le chemin complet du backup
    DIR* dir = opendir(backup_dir);
    if (!dir) {
        fprintf(stderr, "Error: Cannot open backup directory '%s'\n", backup_dir);
        return BACKUP_ERROR_SOURCE_DIR_NOT_FOUND;
    }
    
    char backup_path[512] = {0};
    int found = 0;
    struct dirent* entry;
    
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;
        
        // Vérifier si le nom commence par backup_name
        if (strncmp(entry->d_name, backup_name, strlen(backup_name)) == 0) {
            snprintf(backup_path, sizeof(backup_path), "%s/%s", backup_dir, entry->d_name);
            
            // Vérifier que c'est bien un répertoire
            struct stat backup_st;
            if (stat(backup_path, &backup_st) == 0 && S_ISDIR(backup_st.st_mode)) {
                found = 1;
                break;
            }
        }
    }
    closedir(dir);
    
    if (!found) {
        fprintf(stderr, "Error: Backup '%s' not found in '%s'\n", backup_name, backup_dir);
        return BACKUP_ERROR_SOURCE_DIR_NOT_FOUND;
    }
    
    printf("Found backup: %s\n", backup_path);
    
    // Construire le chemin complet du fichier dans le backup
    char source_file_path[1024];
    snprintf(source_file_path, sizeof(source_file_path), "%s/%s", backup_path, file_path);
    
    // Vérifier que le fichier existe dans le backup
    struct stat file_st;
    if (stat(source_file_path, &file_st) != 0) {
        fprintf(stderr, "Error: File '%s' not found in backup '%s'\n", file_path, backup_name);
        return BACKUP_ERROR_SOURCE_DIR_NOT_FOUND;
    }
    
    if (S_ISDIR(file_st.st_mode)) {
        fprintf(stderr, "Error: '%s' is a directory, not a file\n", file_path);
        return BACKUP_ERROR_INVALID_PARAMETERS;
    }
    
    printf("Found file in backup: %s\n", source_file_path);
    printf("File size: %ld bytes\n", (long)file_st.st_size);
    
    // Créer les répertoires parents du chemin de restauration si nécessaire
    char restore_dir_path[1024];
    strncpy(restore_dir_path, restore_path, sizeof(restore_dir_path) - 1);
    restore_dir_path[sizeof(restore_dir_path) - 1] = '\0';
    
    // Trouver le dernier '/' pour séparer le répertoire du nom de fichier
    char* last_slash = strrchr(restore_dir_path, '/');
    if (last_slash != NULL) {
        *last_slash = '\0';
        
        // Créer les répertoires récursivement
        char tmp[1024];
        char* p = NULL;
        size_t len;
        
        snprintf(tmp, sizeof(tmp), "%s", restore_dir_path);
        len = strlen(tmp);
        
        if (tmp[len - 1] == '/')
            tmp[len - 1] = 0;
        
        for (p = tmp + 1; *p; p++) {
            if (*p == '/') {
                *p = 0;
                create_directory(tmp);
                *p = '/';
            }
        }
        
        create_directory(tmp);
    }
    
    // Ouvrir le fichier source
    FILE* src = fopen(source_file_path, "rb");
    if (!src) {
        fprintf(stderr, "Error: Cannot open source file '%s'\n", source_file_path);
        return BACKUP_ERROR_COPY_FILE;
    }
    
    // Ouvrir le fichier destination
    FILE* dst = fopen(restore_path, "wb");
    if (!dst) {
        fprintf(stderr, "Error: Cannot create destination file '%s'\n", restore_path);
        fclose(src);
        return BACKUP_ERROR_COPY_FILE;
    }
    
    // Copier le fichier
    char buffer[8192];
    size_t bytes_read;
    size_t total_bytes = 0;
    
    printf("Restoring file...\n");
    
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), src)) > 0) {
        size_t bytes_written = fwrite(buffer, 1, bytes_read, dst);
        if (bytes_written != bytes_read) {
            fprintf(stderr, "Error: Write failed\n");
            fclose(src);
            fclose(dst);
            return BACKUP_ERROR_COPY_FILE;
        }
        total_bytes += bytes_written;
    }
    
    // Vérifier les erreurs de lecture
    if (ferror(src)) {
        fprintf(stderr, "Error: Read failed\n");
        fclose(src);
        fclose(dst);
        return BACKUP_ERROR_COPY_FILE;
    }
    
    fclose(src);
    fclose(dst);
    
    // Afficher le résumé
    printf("\nFile restore completed successfully!\n");
    printf("Source: %s\n", source_file_path);
    printf("Destination: %s\n", restore_path);
    printf("Size: %zu bytes\n", total_bytes);
    
    return BACKUP_SUCCESS;
}

// Helper function to extract basename from file path
static const char* get_basename(const char* path) {
    const char* basename = strrchr(path, '/');
    if (basename) {
        return basename + 1;
    }
    #ifdef _WIN32
    basename = strrchr(path, '\\');
    if (basename) {
        return basename + 1;
    }
    #endif
    return path;
}

BackupResult backup_file(const char* source_file, const char* backup_dir, const char* backup_name) {
    // Validate parameters
    if (!source_file || !backup_dir || !backup_name) {
        fprintf(stderr, "Error: Invalid parameters (NULL pointer)\n");
        return BACKUP_ERROR_INVALID_PARAMETERS;
    }
    
    if (strlen(source_file) == 0 || strlen(backup_dir) == 0 || strlen(backup_name) == 0) {
        fprintf(stderr, "Error: Invalid parameters (empty string)\n");
        return BACKUP_ERROR_INVALID_PARAMETERS;
    }
    
    // Check if source file exists
    struct stat file_st;
    if (stat(source_file, &file_st) != 0) {
        fprintf(stderr, "Error: Source file '%s' does not exist\n", source_file);
        return BACKUP_ERROR_SOURCE_DIR_NOT_FOUND;
    }
    
    if (S_ISDIR(file_st.st_mode)) {
        fprintf(stderr, "Error: '%s' is a directory, not a file\n", source_file);
        return BACKUP_ERROR_INVALID_PARAMETERS;
    }
    
    // Create backup directory if it doesn't exist
    if (!directory_exists(backup_dir)) {
        if (create_directory(backup_dir) != 0) {
            fprintf(stderr, "Error: Failed to create backup directory '%s'\n", backup_dir);
            return BACKUP_ERROR_CREATE_DIR;
        }
    }
    
    // Get basename of source file
    const char* base_name = get_basename(source_file);
    
    // Create timestamped backup filename
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    char timestamp[64];
    if (t) {
        snprintf(timestamp, sizeof(timestamp), "%04d%02d%02d_%02d%02d%02d",
                 t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
                 t->tm_hour, t->tm_min, t->tm_sec);
    } else {
        snprintf(timestamp, sizeof(timestamp), "unknown_time");
    }
    
    // Create backup file path
    char backup_file_path[512];
    snprintf(backup_file_path, sizeof(backup_file_path), "%s/%s_%s_%s",
             backup_dir, backup_name, base_name, timestamp);
    
    printf("Creating backup of file: %s\n", source_file);
    printf("Backup location: %s\n", backup_file_path);
    
    // Open source file
    FILE* src = fopen(source_file, "rb");
    if (!src) {
        fprintf(stderr, "Error: Cannot open source file '%s'\n", source_file);
        return BACKUP_ERROR_COPY_FILE;
    }
    
    // Open destination file
    FILE* dst = fopen(backup_file_path, "wb");
    if (!dst) {
        fprintf(stderr, "Error: Cannot create backup file '%s'\n", backup_file_path);
        fclose(src);
        return BACKUP_ERROR_COPY_FILE;
    }
    
    // Copy file
    char buffer[8192];
    size_t bytes_read;
    size_t total_bytes = 0;
    
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), src)) > 0) {
        size_t bytes_written = fwrite(buffer, 1, bytes_read, dst);
        if (bytes_written != bytes_read) {
            fprintf(stderr, "Error: Write failed during backup\n");
            fclose(src);
            fclose(dst);
            remove(backup_file_path);
            return BACKUP_ERROR_COPY_FILE;
        }
        total_bytes += bytes_written;
    }
    
    // Check for read errors
    if (ferror(src)) {
        fprintf(stderr, "Error: Read failed during backup\n");
        fclose(src);
        fclose(dst);
        remove(backup_file_path);
        return BACKUP_ERROR_COPY_FILE;
    }
    
    fclose(src);
    fclose(dst);
    
    // Calculate checksum if available
    char checksum[65] = "N/A";
    // Try to calculate checksum using file_manager function
    if (file_exists(backup_file_path)) {
        if (calculate_file_checksum(backup_file_path, checksum) != 0) {
            strcpy(checksum, "N/A");
        }
    }
    
    // Create metadata file
    char metadata_file[600];
    snprintf(metadata_file, sizeof(metadata_file), "%s.meta", backup_file_path);
    
    FILE* meta = fopen(metadata_file, "w");
    if (meta) {
        fprintf(meta, "Backup Name: %s\n", backup_name);
        fprintf(meta, "Source File: %s\n", source_file);
        fprintf(meta, "Backup File: %s\n", backup_file_path);
        fprintf(meta, "Created: %s", ctime(&now));
        fprintf(meta, "Size: %zu bytes\n", total_bytes);
        fprintf(meta, "Checksum: %s\n", checksum);
        fclose(meta);
    }
    
    printf("\nFile backup completed successfully!\n");
    printf("Source: %s\n", source_file);
    printf("Backup: %s\n", backup_file_path);
    printf("Size: %zu bytes\n", total_bytes);
    
    return BACKUP_SUCCESS;
}


