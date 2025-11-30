# ATTENDANCE MODULE - ALL FUNCTIONS EXPLANATION

This document explains all functions in the attendance module of the student management system.

## Data Structures

### AttendanceRecord
Represents a single attendance record with:
- `id`: Unique record identifier
- `student_id`: Student who the record belongs to
- `course_id`: Course the record is for
- `date`: Date of the attendance record
- `status`: Attendance status (0=absent, 1=present, 2=late, 3=excused)
- `reason`: Optional reason for absence/excuse
- `teacher_id`: Teacher who recorded the attendance
- `recorded_time`: Timestamp when the record was created

### AttendanceList
Manages a collection of attendance records:
- `records`: Dynamic array of AttendanceRecord
- `count`: Current number of records
- `capacity`: Maximum capacity of the array

### AttendanceStats
Contains calculated statistics:
- `student_id`, `course_id`: Identifiers
- `total_days`, `present_days`, `absent_days`, `late_days`, `excused_days`: Counts
- `attendance_percentage`: Calculated percentage
- `consecutive_absences`, `consecutive_presents`: Consecutive day counts

---

## 1. ATTENDANCE LIST MANAGEMENT FUNCTIONS

### `attendance_list_create()`
**Purpose**: Creates and initializes a new AttendanceList structure
**Returns**: Pointer to newly allocated AttendanceList
**Details**: Allocates memory for the list and initializes count and capacity

### `attendance_list_destroy(AttendanceList* list)`
**Purpose**: Frees all memory associated with an AttendanceList
**Parameters**: `list` - Pointer to the list to destroy
**Details**: Frees the records array and the list structure itself

### `attendance_list_add(AttendanceList* list, AttendanceRecord record)`
**Purpose**: Adds a new attendance record to the list
**Parameters**: 
- `list`: Pointer to the attendance list
- `record`: The attendance record to add
**Returns**: 0 on success, -1 on failure
**Details**: Automatically expands the list if capacity is reached

### `attendance_list_remove(AttendanceList* list, int record_id)`
**Purpose**: Removes an attendance record by its ID
**Parameters**:
- `list`: Pointer to the attendance list
- `record_id`: ID of the record to remove
**Returns**: 0 on success, -1 if record not found
**Details**: Shifts remaining records to fill the gap

### `attendance_list_find_by_id(AttendanceList* list, int record_id)`
**Purpose**: Finds an attendance record by its unique ID
**Parameters**:
- `list`: Pointer to the attendance list
- `record_id`: ID to search for
**Returns**: Pointer to the record if found, NULL otherwise

### `attendance_list_find_by_student_date(AttendanceList* list, int student_id, time_t date)`
**Purpose**: Finds attendance record for a specific student on a specific date
**Parameters**:
- `list`: Pointer to the attendance list
- `student_id`: Student identifier
- `date`: Date to search for
**Returns**: Pointer to the record if found, NULL otherwise
**Details**: Useful for checking if attendance was already marked

### `attendance_list_find_by_course_date(AttendanceList* list, int course_id, time_t date)`
**Purpose**: Finds all attendance records for a course on a specific date
**Parameters**:
- `list`: Pointer to the attendance list
- `course_id`: Course identifier
- `date`: Date to search for
**Returns**: Pointer to first matching record (may need iteration for all records)

---

## 2. ATTENDANCE OPERATIONS

### `mark_attendance(AttendanceList* list, int student_id, int course_id, time_t date, int status, int teacher_id)`
**Purpose**: Creates a new attendance record for a student
**Parameters**:
- `list`: Pointer to the attendance list
- `student_id`: Student identifier
- `course_id`: Course identifier
- `date`: Date of attendance
- `status`: Attendance status (0=absent, 1=present, 2=late, 3=excused)
- `teacher_id`: Teacher recording the attendance
**Returns**: 0 on success, -1 on failure
**Details**: Main function for marking attendance, validates inputs

### `update_attendance(AttendanceList* list, int record_id, int new_status, const char* reason)`
**Purpose**: Updates an existing attendance record
**Parameters**:
- `list`: Pointer to the attendance list
- `record_id`: ID of the record to update
- `new_status`: New attendance status
- `reason`: Optional reason for the change
**Returns**: 0 on success, -1 on failure
**Details**: Allows correcting attendance mistakes

### `excuse_absence(AttendanceList* list, int record_id, const char* reason)`
**Purpose**: Changes an absence to an excused absence
**Parameters**:
- `list`: Pointer to the attendance list
- `record_id`: ID of the absence record
- `reason`: Reason for the excuse
**Returns**: 0 on success, -1 on failure
**Details**: Updates status to excused and saves the reason

### `get_attendance_for_date(AttendanceList* list, int course_id, time_t date, AttendanceRecord** records, int* count)`
**Purpose**: Retrieves all attendance records for a course on a specific date
**Parameters**:
- `list`: Pointer to the attendance list
- `course_id`: Course identifier
- `date`: Date to query
- `records`: Output parameter - pointer to array of records
- `count`: Output parameter - number of records found
**Returns**: 0 on success, -1 on failure
**Details**: Allocates memory for the returned array (caller must free)

---

## 3. ATTENDANCE DISPLAY FUNCTIONS

### `attendance_list_display_all(AttendanceList* list)`
**Purpose**: Displays all attendance records in the list
**Parameters**: `list` - Pointer to the attendance list
**Details**: Prints formatted output of all records

### `attendance_list_display_student_attendance(AttendanceList* list, int student_id)`
**Purpose**: Displays all attendance records for a specific student
**Parameters**:
- `list`: Pointer to the attendance list
- `student_id`: Student identifier
**Details**: Shows attendance history for one student

### `attendance_list_display_course_attendance(AttendanceList* list, int course_id)`
**Purpose**: Displays all attendance records for a specific course
**Parameters**:
- `list`: Pointer to the attendance list
- `course_id`: Course identifier
**Details**: Shows attendance for all students in a course

### `attendance_list_display_date_attendance(AttendanceList* list, time_t date)`
**Purpose**: Displays all attendance records for a specific date
**Parameters**:
- `list`: Pointer to the attendance list
- `date`: Date to display
**Details**: Shows attendance across all courses for that date

### `attendance_display_record(AttendanceRecord* record)`
**Purpose**: Displays a single attendance record in formatted output
**Parameters**: `record` - Pointer to the record to display
**Details**: Prints all fields of the record

---

## 4. ATTENDANCE STATISTICS FUNCTIONS

### `calculate_student_attendance_stats(AttendanceList* list, int student_id, int course_id)`
**Purpose**: Calculates attendance statistics for a student in a specific course
**Parameters**:
- `list`: Pointer to the attendance list
- `student_id`: Student identifier
- `course_id`: Course identifier
**Returns**: Pointer to AttendanceStats structure (must be freed with free_attendance_stats)
**Details**: Calculates percentages, consecutive days, totals

### `calculate_course_attendance_stats(AttendanceList* list, int course_id)`
**Purpose**: Calculates overall attendance statistics for an entire course
**Parameters**:
- `list`: Pointer to the attendance list
- `course_id`: Course identifier
**Returns**: Pointer to AttendanceStats structure
**Details**: Aggregates statistics across all students in the course

### `display_attendance_stats(AttendanceStats* stats)`
**Purpose**: Displays formatted attendance statistics
**Parameters**: `stats` - Pointer to statistics to display
**Details**: Prints all statistical information in readable format

### `free_attendance_stats(AttendanceStats* stats)`
**Purpose**: Frees memory allocated for statistics structure
**Parameters**: `stats` - Pointer to statistics to free
**Details**: Must be called after using calculate functions

---

## 5. ATTENDANCE VALIDATION FUNCTIONS

### `attendance_validate_status(int status)`
**Purpose**: Validates if an attendance status is valid
**Parameters**: `status` - Status value to validate
**Returns**: 1 if valid (0-3), 0 if invalid
**Details**: Checks if status is within allowed range

### `attendance_validate_date(time_t date)`
**Purpose**: Validates if a date is valid for attendance
**Parameters**: `date` - Date to validate
**Returns**: 1 if valid, 0 if invalid
**Details**: Checks if date is not in future, not too old, etc.

### `attendance_validate_reason(const char* reason)`
**Purpose**: Validates the reason string for attendance
**Parameters**: `reason` - Reason string to validate
**Returns**: 1 if valid, 0 if invalid
**Details**: Checks length, special characters, etc.

### `attendance_validate_student_course(int student_id, int course_id)`
**Purpose**: Validates if a student is enrolled in a course
**Parameters**:
- `student_id`: Student identifier
- `course_id`: Course identifier
**Returns**: 1 if valid enrollment, 0 otherwise
**Details**: Checks enrollment records

---

## 6. ATTENDANCE INPUT/OUTPUT FUNCTIONS

### `attendance_input_new(int student_id, int course_id, int teacher_id)`
**Purpose**: Prompts user to input a new attendance record
**Parameters**:
- `student_id`: Student identifier
- `course_id`: Course identifier
- `teacher_id`: Teacher identifier
**Returns**: AttendanceRecord structure with inputted data
**Details**: Interactive function that asks user for date, status, reason

### `attendance_input_edit(AttendanceRecord* record)`
**Purpose**: Prompts user to edit an existing attendance record
**Parameters**: `record` - Pointer to record to edit
**Details**: Allows user to modify status, reason, date

### `attendance_display_summary(AttendanceList* list, int student_id)`
**Purpose**: Displays a summary of attendance for a student
**Parameters**:
- `list`: Pointer to the attendance list
- `student_id`: Student identifier
**Details**: Shows quick overview with percentages and key dates

---

## 7. BULK ATTENDANCE OPERATIONS

### `mark_class_attendance(AttendanceList* list, int course_id, time_t date, int* student_ids, int* statuses, int count, int teacher_id)`
**Purpose**: Marks attendance for multiple students at once (entire class)
**Parameters**:
- `list`: Pointer to the attendance list
- `course_id`: Course identifier
- `date`: Date of attendance
- `student_ids`: Array of student IDs
- `statuses`: Array of corresponding statuses
- `count`: Number of students
- `teacher_id`: Teacher recording
**Returns**: Number of successfully marked records
**Details**: Efficient for marking entire class attendance

### `import_attendance_from_file(AttendanceList* list, const char* filename)`
**Purpose**: Imports attendance records from a file
**Parameters**:
- `list`: Pointer to the attendance list
- `filename`: Path to import file
**Returns**: Number of records imported, -1 on error
**Details**: Supports CSV or custom format

### `export_attendance_to_file(AttendanceList* list, const char* filename)`
**Purpose**: Exports attendance records to a file
**Parameters**:
- `list`: Pointer to the attendance list
- `filename`: Path to export file
**Returns**: 0 on success, -1 on failure
**Details**: Saves in CSV or readable format

---

## 8. ATTENDANCE REPORTS

### `generate_student_attendance_report(AttendanceList* list, int student_id, const char* filename)`
**Purpose**: Generates a detailed attendance report for a student
**Parameters**:
- `list`: Pointer to the attendance list
- `student_id`: Student identifier
- `filename`: Output file path
**Returns**: 0 on success, -1 on failure
**Details**: Creates formatted report with statistics

### `generate_course_attendance_report(AttendanceList* list, int course_id, const char* filename)`
**Purpose**: Generates attendance report for an entire course
**Parameters**:
- `list`: Pointer to the attendance list
- `course_id`: Course identifier
- `filename`: Output file path
**Returns**: 0 on success, -1 on failure
**Details**: Shows all students' attendance in the course

### `generate_daily_attendance_report(AttendanceList* list, time_t date, const char* filename)`
**Purpose**: Generates attendance report for a specific date
**Parameters**:
- `list`: Pointer to the attendance list
- `date`: Date to report
- `filename`: Output file path
**Returns**: 0 on success, -1 on failure
**Details**: Shows attendance across all courses for that day

### `generate_attendance_summary_report(AttendanceList* list, const char* filename)`
**Purpose**: Generates overall attendance summary report
**Parameters**:
- `list`: Pointer to the attendance list
- `filename`: Output file path
**Returns**: 0 on success, -1 on failure
**Details**: Comprehensive report with trends and statistics

---

## 9. FILE OPERATIONS

### `attendance_list_save_to_file(AttendanceList* list, const char* filename)`
**Purpose**: Saves attendance list to a file for persistence
**Parameters**:
- `list`: Pointer to the attendance list
- `filename`: Path to save file
**Returns**: 0 on success, -1 on failure
**Details**: Binary or text format for data persistence

### `attendance_list_load_from_file(AttendanceList* list, const char* filename)`
**Purpose**: Loads attendance list from a saved file
**Parameters**:
- `list`: Pointer to the attendance list (must be initialized)
- `filename`: Path to load file
**Returns**: Number of records loaded, -1 on error
**Details**: Restores previously saved attendance data

---

## 10. ATTENDANCE SORTING FUNCTIONS

### `attendance_list_sort_by_student(AttendanceList* list)`
**Purpose**: Sorts attendance records by student ID
**Parameters**: `list` - Pointer to the attendance list
**Details**: Reorders records for easier student lookup

### `attendance_list_sort_by_course(AttendanceList* list)`
**Purpose**: Sorts attendance records by course ID
**Parameters**: `list` - Pointer to the attendance list
**Details**: Groups records by course

### `attendance_list_sort_by_date(AttendanceList* list)`
**Purpose**: Sorts attendance records by date (chronological)
**Parameters**: `list` - Pointer to the attendance list
**Details**: Orders records from oldest to newest (or vice versa)

### `attendance_list_sort_by_status(AttendanceList* list)`
**Purpose**: Sorts attendance records by status
**Parameters**: `list` - Pointer to the attendance list
**Details**: Groups records by attendance status

---

## 11. ATTENDANCE ANALYSIS FUNCTIONS

### `identify_absent_students(AttendanceList* list, int course_id, time_t date)`
**Purpose**: Identifies which students were absent on a specific date
**Parameters**:
- `list`: Pointer to the attendance list
- `course_id`: Course identifier
- `date`: Date to check
**Returns**: Number of absent students
**Details**: Useful for follow-up actions

### `identify_frequently_absent_students(AttendanceList* list, int course_id, float threshold)`
**Purpose**: Finds students with attendance below a threshold
**Parameters**:
- `list`: Pointer to the attendance list
- `course_id`: Course identifier
- `threshold`: Minimum attendance percentage (e.g., 0.75 for 75%)
**Returns**: Number of students below threshold
**Details**: Helps identify students needing intervention

### `calculate_attendance_trends(AttendanceList* list, int student_id, int course_id)`
**Purpose**: Analyzes attendance trends over time
**Parameters**:
- `list`: Pointer to the attendance list
- `student_id`: Student identifier
- `course_id`: Course identifier
**Returns**: Trend indicator (improving/declining/stable)
**Details**: Analyzes pattern changes over time periods

### `get_attendance_patterns(AttendanceList* list, int student_id)`
**Purpose**: Identifies attendance patterns for a student
**Parameters**:
- `list`: Pointer to the attendance list
- `student_id`: Student identifier
**Returns**: Pattern identifier code
**Details**: Detects patterns like "always absent on Fridays"

---

## 12. UTILITY FUNCTIONS

### `attendance_status_to_string(int status)`
**Purpose**: Converts status code to readable string
**Parameters**: `status` - Status code (0-3)
**Returns**: String representation ("Absent", "Present", "Late", "Excused")
**Details**: Helper for display functions

### `string_to_attendance_status(const char* status_str)`
**Purpose**: Converts string to status code
**Parameters**: `status_str` - String representation
**Returns**: Status code, -1 if invalid
**Details**: Helper for input parsing

### `is_attendance_present(int status)`
**Purpose**: Checks if status represents present
**Parameters**: `status` - Status to check
**Returns**: 1 if present, 0 otherwise
**Details**: Convenience function for status checks

### `is_attendance_absent(int status)`
**Purpose**: Checks if status represents absent
**Parameters**: `status` - Status to check
**Returns**: 1 if absent, 0 otherwise

### `is_attendance_late(int status)`
**Purpose**: Checks if status represents late
**Parameters**: `status` - Status to check
**Returns**: 1 if late, 0 otherwise

### `is_attendance_excused(int status)`
**Purpose**: Checks if status represents excused
**Parameters**: `status` - Status to check
**Returns**: 1 if excused, 0 otherwise

---

## 13. DATE UTILITY FUNCTIONS

### `get_today_date(void)`
**Purpose**: Gets current system date
**Returns**: time_t representing today's date
**Details**: Returns date at midnight (no time component)

### `get_class_date(int course_id, int day_of_week)`
**Purpose**: Gets the date for a class on a specific day of week
**Parameters**:
- `course_id`: Course identifier
- `day_of_week`: Day (0=Sunday, 1=Monday, etc.)
**Returns**: time_t representing the class date
**Details**: Calculates next or last occurrence

### `is_weekend(time_t date)`
**Purpose**: Checks if a date falls on weekend
**Parameters**: `date` - Date to check
**Returns**: 1 if weekend, 0 otherwise
**Details**: Validates attendance dates

### `is_holiday(time_t date)`
**Purpose**: Checks if a date is a holiday
**Parameters**: `date` - Date to check
**Returns**: 1 if holiday, 0 otherwise
**Details**: May need holiday calendar configuration

### `get_days_between_dates(time_t start_date, time_t end_date)`
**Purpose**: Calculates number of days between two dates
**Parameters**:
- `start_date`: Starting date
- `end_date`: Ending date
**Returns**: Number of days (can be negative)
**Details**: Useful for date range calculations

---

## Constants

- **ATTENDANCE_ABSENT**: 0 - Student was absent
- **ATTENDANCE_PRESENT**: 1 - Student was present
- **ATTENDANCE_LATE**: 2 - Student was late
- **ATTENDANCE_EXCUSED**: 3 - Student was excused

- **ATTENDANCE_THRESHOLD_WARNING**: 0.85 (85%) - Warning level
- **ATTENDANCE_THRESHOLD_CRITICAL**: 0.75 (75%) - Critical level
- **MAX_CONSECUTIVE_ABSENCES**: 5 - Maximum allowed consecutive absences

---

## Summary

The attendance module provides comprehensive functionality for:
- ✅ Recording and managing attendance records
- ✅ Displaying attendance information in various formats
- ✅ Calculating statistics and generating reports
- ✅ Analyzing attendance patterns and trends
- ✅ Bulk operations for efficiency
- ✅ Data persistence through file operations
- ✅ Validation and utility functions for reliability

This module is designed to handle all aspects of student attendance tracking in an educational management system.

