#ifndef GRADE_H
#define GRADE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "config.h"

// Grade structure
typedef struct {
    int id;
    int student_id;
    int course_id;
    char course_name[MAX_COURSE_LENGTH];
    GradeLevel grade_level;
    float numeric_grade;
    char assignment_name[100];
    time_t date_assigned;
    time_t date_due;
    time_t date_submitted;
    int is_submitted;
    int is_late;
    char comments[500];
    int teacher_id;
} Grade;

// Grade list structure
typedef struct {
    Grade* grades;
    int count;
    int capacity;
} GradeList;

// Course structure
typedef struct {
    int id;
    char name[MAX_COURSE_LENGTH];
    char code[20];
    char description[200];
    int credits;
    int teacher_id;
    int semester;
    int year;
    int is_active;
} Course;

// Course list structure
typedef struct {
    Course* courses;
    int count;
    int capacity;
} CourseList;

// Grade management functions
GradeList* grade_list_create(void);
void grade_list_destroy(GradeList* list);
int grade_list_add(GradeList* list, Grade grade);
int grade_list_remove(GradeList* list, int grade_id);
Grade* grade_list_find_by_id(GradeList* list, int grade_id);
Grade* grade_list_find_by_student(GradeList* list, int student_id);
Grade* grade_list_find_by_course(GradeList* list, int course_id);
void grade_list_display_all(GradeList* list);
void grade_list_display_student_grades(GradeList* list, int student_id);
void grade_list_display_course_grades(GradeList* list, int course_id);

// Grade calculations
float calculate_student_gpa(GradeList* list, int student_id);
float calculate_course_average(GradeList* list, int course_id);
float calculate_class_average(GradeList* list, int course_id);
int count_passing_grades(GradeList* list, int student_id);
int count_failing_grades(GradeList* list, int student_id);

// Grade validation
int grade_validate_numeric(float grade);
int grade_validate_level(GradeLevel level);
int grade_validate_assignment(const char* assignment_name);
int grade_validate_dates(time_t assigned, time_t due, time_t submitted);

// Grade input/output
Grade grade_input_new(int student_id, int course_id);
void grade_input_edit(Grade* grade);
void grade_display(Grade* grade);
void grade_display_summary(GradeList* list, int student_id);

// Course management functions
CourseList* course_list_create(void);
void course_list_destroy(CourseList* list);
int course_list_add(CourseList* list, Course course);
int course_list_remove(CourseList* list, int course_id);
Course* course_list_find_by_id(CourseList* list, int course_id);
Course* course_list_find_by_code(CourseList* list, const char* code);
Course* course_list_find_by_name(CourseList* list, const char* name);
void course_list_display_all(CourseList* list);
void course_list_display_course(Course* course);

// Course validation
int course_validate_name(const char* name);
int course_validate_code(const char* code);
int course_validate_credits(int credits);
int course_validate_semester(int semester);

// Course input/output
Course course_input_new(void);
void course_input_edit(Course* course);
void course_display_summary(CourseList* list);

// Grade statistics
typedef struct {
    int total_students;
    int passing_students;
    int failing_students;
    float average_grade;
    float highest_grade;
    float lowest_grade;
    int total_assignments;
    int submitted_assignments;
    int late_submissions;
} GradeStatistics;

GradeStatistics* calculate_grade_statistics(GradeList* list, int course_id);
void display_grade_statistics(GradeStatistics* stats);
void free_grade_statistics(GradeStatistics* stats);

// Grade reports
int generate_student_report(GradeList* list, int student_id, const char* filename);
int generate_course_report(GradeList* list, int course_id, const char* filename);
int generate_class_report(GradeList* list, int course_id, const char* filename);
int generate_gpa_report(GradeList* list, const char* filename);

// File operations
int grade_list_save_to_file(GradeList* list, const char* filename);
int grade_list_load_from_file(GradeList* list, const char* filename);
int course_list_save_to_file(CourseList* list, const char* filename);
int course_list_load_from_file(CourseList* list, const char* filename);

// Grade sorting
void grade_list_sort_by_student(GradeList* list);
void grade_list_sort_by_course(GradeList* list);
void grade_list_sort_by_date(GradeList* list);
void grade_list_sort_by_grade(GradeList* list);

// Utility functions
const char* grade_level_to_string(GradeLevel level);
GradeLevel string_to_grade_level(const char* grade_str);
float grade_level_to_numeric(GradeLevel level);
GradeLevel numeric_to_grade_level(float numeric);
int is_grade_passing(GradeLevel level);
int is_grade_failing(GradeLevel level);

// Grade analysis
int analyze_student_performance(GradeList* list, int student_id);
int analyze_course_performance(GradeList* list, int course_id);
int identify_struggling_students(GradeList* list, int course_id, float threshold);
int identify_excellent_students(GradeList* list, int course_id, float threshold);

#endif // GRADE_H
