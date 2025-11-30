# FILE_MANAGER MODULE - ALL FUNCTIONS EXPLANATION

This document explains all functions in the file_manager module of the student management system.

## Data Structures

### FileResult (Enum)
Return codes for file operations:
- `FILE_SUCCESS = 0`: Operation completed successfully
- `FILE_ERROR_NOT_FOUND = -1`: File or directory not found
- `FILE_ERROR_PERMISSION_DENIED = -2`: Insufficient permissions
- `FILE_ERROR_DISK_FULL = -3`: No space available on disk
- `FILE_ERROR_CORRUPTED = -4`: File is corrupted or invalid
- `FILE_ERROR_ENCRYPTION_FAILED = -5`: Encryption operation failed
- `FILE_ERROR_DECRYPTION_FAILED = -6`: Decryption operation failed
- `FILE_ERROR_INVALID_FORMAT = -7`: File format is invalid

### FileInfo
Structure containing file metadata:
- `filename[256]`: Full path to the file
- `file_size`: Size of file in bytes
- `last_modified`: Timestamp of last modification
- `created`: Timestamp of file creation
- `is_encrypted`: Flag indicating if file is encrypted (1) or not (0)
- `checksum[65]`: SHA-256 hash of file contents for integrity checking

### FileMonitor
Structure for monitoring file changes:
- `filename[256]`: Path to file being monitored
- `last_check`: Timestamp of last check
- `last_size`: File size at last check
- `last_checksum[65]`: Checksum at last check

---

## 1. INITIALIZATION AND CLEANUP

### `file_manager_init(void)`
**Purpose**: Initializes the file manager module
**Returns**: `FileResult` - FILE_SUCCESS on success, error code otherwise
**Details**: 
- Sets up necessary data structures
- Checks system permissions
- Validates encryption keys if needed
- Creates default directories if they don't exist
- Should be called once at program startup

### `file_manager_cleanup(void)`
**Purpose**: Cleans up resources used by file manager
**Parameters**: None
**Returns**: void
**Details**: 
- Frees allocated memory
- Closes any open file handles
- Should be called before program termination

---

## 2. DIRECTORY OPERATIONS

### `create_directory(const char* path)`
**Purpose**: Creates a new directory at the specified path
**Parameters**: 
- `path`: Full or relative path to directory to create
**Returns**: 
- `1` if successful
- `0` if failed (directory already exists or creation failed)
- `-1` on error
**Details**: 
- Creates parent directories if needed (mkdir -p behavior)
- Handles platform-specific path separators
- Validates path format

### `directory_exists(const char* path)`
**Purpose**: Checks if a directory exists at the specified path
**Parameters**: 
- `path`: Path to check
**Returns**: 
- `1` if directory exists
- `0` if it doesn't exist
- `-1` on error
**Details**: 
- Distinguishes between files and directories
- Checks read permissions

### `ensure_data_directories(void)`
**Purpose**: Ensures all required data directories exist
**Returns**: 
- `1` if all directories created successfully
- `0` if any directory creation failed
**Details**: 
- Creates standard directories (data/, logs/, reports/, backups/)
- Essential for application startup
- Creates directories with appropriate permissions

### `cleanup_temp_files(void)`
**Purpose**: Removes temporary files created during operations
**Returns**: 
- Number of files deleted
- `-1` on error
**Details**: 
- Removes files from temp directory
- Only deletes files matching temp file patterns
- Safe operation that doesn't affect user data

---

## 3. FILE EXISTENCE AND VALIDATION

### `file_exists(const char* filename)`
**Purpose**: Checks if a file exists
**Parameters**: 
- `filename`: Path to file to check
**Returns**: 
- `1` if file exists
- `0` if file doesn't exist
- `-1` on error
**Details**: 
- Uses stat() system call
- Checks if path points to a regular file (not directory)

### `is_file_encrypted(const char* filename)`
**Purpose**: Determines if a file is encrypted
**Parameters**: 
- `filename`: Path to file to check
**Returns**: 
- `1` if file is encrypted
- `0` if file is not encrypted
- `-1` on error
**Details**: 
- Checks file header/magic numbers
- May read metadata or file extension
- Used to determine which read function to use

### `validate_file_integrity(const char* filename)`
**Purpose**: Validates file integrity using checksum
**Parameters**: 
- `filename`: Path to file to validate
**Returns**: 
- `1` if file integrity is valid
- `0` if file is corrupted
- `-1` on error
**Details**: 
- Calculates current file checksum
- Compares with stored checksum (if available)
- Detects file corruption or tampering

### `get_file_info(const char* filename)`
**Purpose**: Retrieves detailed information about a file
**Parameters**: 
- `filename`: Path to file
**Returns**: 
- Pointer to `FileInfo` structure (must be freed with `free_file_info`)
- `NULL` on error
**Details**: 
- Populates all fields of FileInfo structure
- Calculates checksum
- Detects encryption status
- Allocates memory for the structure

### `free_file_info(FileInfo* info)`
**Purpose**: Frees memory allocated for FileInfo structure
**Parameters**: 
- `info`: Pointer to FileInfo to free
**Returns**: void
**Details**: 
- Must be called after using `get_file_info`
- Prevents memory leaks
- Safe to call with NULL pointer

---

## 4. BASIC FILE OPERATIONS

### `read_file_content(const char* filename, char** content, size_t* content_size)`
**Purpose**: Reads entire file contents into memory
**Parameters**: 
- `filename`: Path to file to read
- `content`: Output parameter - pointer to allocated buffer with file contents
- `content_size`: Output parameter - size of read content in bytes
**Returns**: `FileResult` code
**Details**: 
- Allocates memory for content (caller must free)
- Reads file in binary or text mode
- Handles large files efficiently
- Returns FILE_ERROR_NOT_FOUND if file doesn't exist

### `write_file_content(const char* filename, const char* content, size_t content_size)`
**Purpose**: Writes content to a file (overwrites existing file)
**Parameters**: 
- `filename`: Path to file to write
- `content`: Data to write
- `content_size`: Size of data in bytes
**Returns**: `FileResult` code
**Details**: 
- Creates file if it doesn't exist
- Overwrites existing file
- Creates parent directories if needed
- Returns FILE_ERROR_DISK_FULL if no space

### `append_to_file(const char* filename, const char* content)`
**Purpose**: Appends content to end of existing file
**Parameters**: 
- `filename`: Path to file
- `content`: String to append (null-terminated)
**Returns**: `FileResult` code
**Details**: 
- Opens file in append mode
- Adds content to end without overwriting
- Creates file if it doesn't exist
- Useful for log files

### `copy_file(const char* source, const char* destination)`
**Purpose**: Copies a file from source to destination
**Parameters**: 
- `source`: Path to source file
- `destination`: Path to destination file
**Returns**: `FileResult` code
**Details**: 
- Copies file contents byte-by-byte
- Preserves file permissions if possible
- Creates destination directory if needed
- Validates source file exists

### `move_file(const char* source, const char* destination)`
**Purpose**: Moves (renames) a file from source to destination
**Parameters**: 
- `source`: Current file path
- `destination`: New file path
**Returns**: `FileResult` code
**Details**: 
- Uses rename() system call when possible
- Falls back to copy + delete if cross-filesystem
- Atomic operation when possible
- Preserves file metadata

### `delete_file(const char* filename)`
**Purpose**: Deletes a file from filesystem
**Parameters**: 
- `filename`: Path to file to delete
**Returns**: `FileResult` code
**Details**: 
- Permanently removes file
- Cannot be undone
- Returns FILE_ERROR_NOT_FOUND if file doesn't exist
- Returns FILE_ERROR_PERMISSION_DENIED if no write access

---

## 5. ENCRYPTED FILE OPERATIONS

### `read_encrypted_file(const char* filename, char** content, size_t* content_size, const unsigned char* key)`
**Purpose**: Reads and decrypts an encrypted file
**Parameters**: 
- `filename`: Path to encrypted file
- `content`: Output parameter - pointer to decrypted content
- `content_size`: Output parameter - size of decrypted content
- `key`: Encryption key (typically 32 bytes for AES-256)
**Returns**: `FileResult` code
**Details**: 
- Reads encrypted file from disk
- Decrypts using provided key
- Allocates memory for decrypted content (caller must free)
- Returns FILE_ERROR_DECRYPTION_FAILED if key is wrong or file corrupted

### `write_encrypted_file(const char* filename, const char* content, size_t content_size, const unsigned char* key)`
**Purpose**: Encrypts and writes content to a file
**Parameters**: 
- `filename`: Path to destination file
- `content`: Plaintext data to encrypt
- `content_size`: Size of plaintext data
- `key`: Encryption key
**Returns**: `FileResult` code
**Details**: 
- Encrypts content using provided key
- Writes encrypted data to file
- May include metadata (IV, algorithm info)
- Returns FILE_ERROR_ENCRYPTION_FAILED on encryption errors

### `encrypt_existing_file(const char* filename, const unsigned char* key)`
**Purpose**: Encrypts an existing plaintext file in-place
**Parameters**: 
- `filename`: Path to file to encrypt
- `key`: Encryption key
**Returns**: `FileResult` code
**Details**: 
- Reads plaintext file
- Encrypts content
- Writes encrypted version back
- May create backup before encrypting
- Original file is replaced with encrypted version

### `decrypt_existing_file(const char* filename, const unsigned char* key)`
**Purpose**: Decrypts an existing encrypted file in-place
**Parameters**: 
- `filename`: Path to encrypted file
- `key`: Decryption key
**Returns**: `FileResult` code
**Details**: 
- Reads encrypted file
- Decrypts content
- Writes plaintext version back
- Original encrypted file is replaced with plaintext version
- Returns FILE_ERROR_DECRYPTION_FAILED if key is incorrect

---

## 6. DATA SERIALIZATION/DESERIALIZATION

### `save_data_to_file(const void* data, size_t data_size, const char* filename, const unsigned char* key)`
**Purpose**: Saves raw binary data to file (with optional encryption)
**Parameters**: 
- `data`: Pointer to data to save
- `data_size`: Size of data in bytes
- `filename`: Destination file path
- `key`: Encryption key (NULL for unencrypted)
**Returns**: `FileResult` code
**Details**: 
- Writes binary data as-is
- Encrypts if key is provided
- Preserves exact byte representation
- Useful for binary structures

### `load_data_from_file(void* data, size_t data_size, const char* filename, const unsigned char* key)`
**Purpose**: Loads raw binary data from file (with optional decryption)
**Parameters**: 
- `data`: Pointer to buffer to store loaded data
- `data_size`: Expected size of data
- `filename`: Source file path
- `key`: Decryption key (NULL for unencrypted)
**Returns**: `FileResult` code
**Details**: 
- Reads binary data into provided buffer
- Decrypts if key is provided
- Buffer must be pre-allocated
- Validates size matches expected

### `save_struct_to_file(const void* struct_data, size_t struct_size, int count, const char* filename, const unsigned char* key)`
**Purpose**: Saves an array of structures to file
**Parameters**: 
- `struct_data`: Pointer to array of structures
- `struct_size`: Size of one structure in bytes
- `count`: Number of structures in array
- `filename`: Destination file path
- `key`: Encryption key (NULL for unencrypted)
**Returns**: `FileResult` code
**Details**: 
- Saves multiple structures as binary data
- May include header with count information
- Preserves structure layout
- Used for saving arrays of student records, etc.

### `load_struct_from_file(void** struct_data, size_t struct_size, int* count, const char* filename, const unsigned char* key)`
**Purpose**: Loads an array of structures from file
**Parameters**: 
- `struct_data`: Output parameter - pointer to allocated array
- `struct_size`: Size of one structure
- `count`: Output parameter - number of structures loaded
- `filename`: Source file path
- `key`: Decryption key (NULL for unencrypted)
**Returns**: `FileResult` code
**Details**: 
- Allocates memory for structures (caller must free)
- Reads count from file header
- Loads all structures
- Returns pointer to array in struct_data
- Caller must free allocated memory

---

## 7. BACKUP OPERATIONS

### `create_backup(const char* source_file, const char* backup_dir)`
**Purpose**: Creates a backup copy of a file with timestamp
**Parameters**: 
- `source_file`: Path to file to backup
- `backup_dir`: Directory to store backup
**Returns**: `FileResult` code
**Details**: 
- Copies file to backup directory
- Adds timestamp to backup filename
- Creates backup directory if needed
- Preserves original file

### `restore_backup(const char* backup_file, const char* destination)`
**Purpose**: Restores a file from backup
**Parameters**: 
- `backup_file`: Path to backup file
- `destination`: Path to restore to
**Returns**: `FileResult` code
**Details**: 
- Copies backup file to destination
- May create backup of current file before restoring
- Validates backup file exists and is valid

### `list_backups(const char* backup_dir, char*** backup_list, int* count)`
**Purpose**: Lists all backup files in a directory
**Parameters**: 
- `backup_dir`: Directory to scan
- `backup_list`: Output parameter - array of backup filenames
- `count`: Output parameter - number of backups found
**Returns**: `FileResult` code
**Details**: 
- Scans directory for backup files
- Allocates array of strings (caller must free)
- Sorts by timestamp (newest first)
- Returns all backup filenames

### `cleanup_old_backups(const char* backup_dir, int days_to_keep)`
**Purpose**: Removes backup files older than specified days
**Parameters**: 
- `backup_dir`: Directory containing backups
- `days_to_keep`: Number of days to keep backups
**Returns**: `FileResult` code
**Details**: 
- Calculates cutoff date
- Deletes backups older than cutoff
- Preserves recent backups
- Returns number of files deleted
- Useful for automatic cleanup

---

## 8. FILE MONITORING

### `create_file_monitor(const char* filename)`
**Purpose**: Creates a monitor to track changes to a file
**Parameters**: 
- `filename`: Path to file to monitor
**Returns**: 
- Pointer to `FileMonitor` structure (must be freed with `destroy_file_monitor`)
- `NULL` on error
**Details**: 
- Initializes monitor with current file state
- Records initial size and checksum
- Allocates memory for monitor structure

### `destroy_file_monitor(FileMonitor* monitor)`
**Purpose**: Frees resources used by file monitor
**Parameters**: 
- `monitor`: Pointer to monitor to destroy
**Returns**: void
**Details**: 
- Frees allocated memory
- Safe to call with NULL pointer

### `check_file_changes(FileMonitor* monitor)`
**Purpose**: Checks if monitored file has changed since last check
**Parameters**: 
- `monitor`: Pointer to file monitor
**Returns**: 
- `1` if file has changed
- `0` if file unchanged
- `-1` on error
**Details**: 
- Compares current file state with stored state
- Updates monitor with new state if changed
- Checks both size and checksum
- Detects modifications, not just size changes

---

## 9. UTILITY FUNCTIONS

### `file_result_to_string(FileResult result)`
**Purpose**: Converts FileResult code to human-readable string
**Parameters**: 
- `result`: FileResult code
**Returns**: 
- Constant string describing the result
**Details**: 
- Useful for error messages and logging
- Returns descriptive error messages
- Never returns NULL

### `get_file_size(const char* filename)`
**Purpose**: Gets the size of a file in bytes
**Parameters**: 
- `filename`: Path to file
**Returns**: 
- File size in bytes
- `-1` on error
**Details**: 
- Uses stat() system call
- Returns exact byte count
- Fast operation

### `get_file_modification_time(const char* filename)`
**Purpose**: Gets the last modification timestamp of a file
**Parameters**: 
- `filename`: Path to file
**Returns**: 
- `time_t` timestamp
- `0` on error
**Details**: 
- Returns Unix timestamp
- Can be converted to readable date
- Useful for sorting and filtering

### `calculate_file_checksum(const char* filename, char* checksum)`
**Purpose**: Calculates SHA-256 checksum of file contents
**Parameters**: 
- `filename`: Path to file
- `checksum`: Output buffer (must be at least 65 bytes)
**Returns**: 
- `1` on success
- `0` on failure
**Details**: 
- Calculates SHA-256 hash
- Stores hex string in checksum buffer
- Used for integrity verification
- Takes time for large files

### `compare_files(const char* file1, const char* file2)`
**Purpose**: Compares two files to check if they're identical
**Parameters**: 
- `file1`: Path to first file
- `file2`: Path to second file
**Returns**: 
- `1` if files are identical
- `0` if files differ
- `-1` on error
**Details**: 
- Compares byte-by-byte
- Stops at first difference
- Efficient for large files
- Can use checksums for faster comparison

---

## 10. CONFIGURATION FILE OPERATIONS

### `load_config_file(const char* filename)`
**Purpose**: Loads configuration from a file
**Parameters**: 
- `filename`: Path to config file
**Returns**: `FileResult` code
**Details**: 
- Parses configuration file (INI, JSON, or custom format)
- Updates global config structure
- Validates configuration values
- Creates default config if file doesn't exist

### `save_config_file(const char* filename)`
**Purpose**: Saves current configuration to file
**Parameters**: 
- `filename`: Path to save config file
**Returns**: `FileResult` code
**Details**: 
- Writes configuration in readable format
- Includes all current settings
- Creates backup before overwriting
- Preserves comments if supported format

### `update_config_value(const char* filename, const char* key, const char* value)`
**Purpose**: Updates a single configuration value
**Parameters**: 
- `filename`: Path to config file
- `key`: Configuration key to update
- `value`: New value for the key
**Returns**: `FileResult` code
**Details**: 
- Reads existing config file
- Updates specified key-value pair
- Writes updated config back
- Creates key if it doesn't exist
- Preserves other settings

### `get_config_value(const char* filename, const char* key, char* value, size_t value_size)`
**Purpose**: Retrieves a configuration value from file
**Parameters**: 
- `filename`: Path to config file
- `key`: Configuration key to read
- `value`: Output buffer for value
- `value_size`: Size of value buffer
**Returns**: `FileResult` code
**Details**: 
- Reads config file
- Finds specified key
- Copies value to buffer
- Returns FILE_ERROR_NOT_FOUND if key doesn't exist
- Truncates if value too large

---

## 11. LOG FILE OPERATIONS

### `append_to_log(const char* log_file, const char* message)`
**Purpose**: Appends a log message to a log file with timestamp
**Parameters**: 
- `log_file`: Path to log file
- `message`: Log message to append
**Returns**: `FileResult` code
**Details**: 
- Adds timestamp prefix to message
- Appends to end of log file
- Creates log file if it doesn't exist
- Thread-safe if needed
- Formats: `[YYYY-MM-DD HH:MM:SS] message`

### `rotate_log_file(const char* log_file, int max_size)`
**Purpose**: Rotates log file when it exceeds maximum size
**Parameters**: 
- `log_file`: Path to log file
- `max_size`: Maximum size in bytes before rotation
**Returns**: `FileResult` code
**Details**: 
- Checks file size
- If exceeds max_size, renames current log with timestamp
- Creates new empty log file
- Keeps old logs for history
- Prevents log files from growing indefinitely

### `cleanup_log_files(const char* log_dir, int days_to_keep)`
**Purpose**: Removes old log files older than specified days
**Parameters**: 
- `log_dir`: Directory containing log files
- `days_to_keep`: Number of days to keep logs
**Returns**: `FileResult` code
**Details**: 
- Scans directory for log files
- Calculates cutoff date
- Deletes logs older than cutoff
- Preserves recent logs
- Returns count of deleted files
- Automatic cleanup for disk space management

---

## Usage Patterns

### Basic File Reading:
```c
char* content;
size_t size;
FileResult result = read_file_content("data.txt", &content, &size);
if (result == FILE_SUCCESS) {
    // Use content
    free(content);
}
```

### Encrypted File Operations:
```c
unsigned char key[32] = {...}; // Your encryption key
FileResult result = write_encrypted_file("data.enc", "secret data", 11, key);
if (result == FILE_SUCCESS) {
    // File encrypted and saved
}
```

### File Monitoring:
```c
FileMonitor* monitor = create_file_monitor("important.dat");
// ... later ...
if (check_file_changes(monitor)) {
    printf("File was modified!\n");
}
destroy_file_monitor(monitor);
```

### Backup Operations:
```c
create_backup("students.dat", "./backups/");
// Creates: ./backups/students_2024-01-15_14-30-00.dat
```

---

## Error Handling

All file operations return `FileResult` enum codes. Always check the return value:

```c
FileResult result = write_file_content("data.txt", content, size);
if (result != FILE_SUCCESS) {
    printf("Error: %s\n", file_result_to_string(result));
    // Handle error appropriately
}
```

---

## Security Considerations

1. **Encryption Keys**: Never hardcode encryption keys. Store securely.
2. **File Permissions**: Set appropriate permissions on sensitive files.
3. **Checksums**: Always validate file integrity before use.
4. **Backup Encryption**: Consider encrypting backup files too.
5. **Temp Files**: Clean up temporary files to prevent data leaks.

---

## Summary

The file_manager module provides comprehensive functionality for:
- ✅ File and directory management
- ✅ Encrypted file operations with security
- ✅ Data serialization for persistence
- ✅ Backup and restore capabilities
- ✅ File integrity validation
- ✅ Configuration file management
- ✅ Log file management with rotation
- ✅ File monitoring for change detection

This module is essential for data persistence, security, and reliable file operations in the student management system.

