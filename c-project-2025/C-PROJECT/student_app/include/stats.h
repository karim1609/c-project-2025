#ifndef STATS_H
#define STATS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "config.h"
#include "student.h"
#include "grade.h"
#include "attendance.h"
#include "club.h"

// General statistics structure
typedef struct {
    int total_students;
    int active_students;
    int inactive_students;
    int total_courses;
    int total_grades;
    int total_attendance_records;
    int total_clubs;
    int total_memberships;
    time_t last_updated;
} SystemStats;

// Student statistics structure
typedef struct {
    int total_students;
    int students_by_year[5];  // Index 0 unused, 1-4 for years
    int students_by_course[20];  // Top 20 courses
    float average_age;
    int age_distribution[10];  // Age ranges
    float average_gpa;
    float gpa_distribution[5];  // GPA ranges
    int top_performers[10];  // Top 10 student IDs
    int struggling_students[10];  // Bottom 10 student IDs
} StudentStats;

// Grade statistics structure
typedef struct {
    int total_grades;
    int grades_by_level[5];  // A, B, C, D, F counts
    float average_gpa;
    float highest_gpa;
    float lowest_gpa;
    int passing_grades;
    int failing_grades;
    float pass_rate;
    int courses_with_grades;
    float course_averages[20];  // Top 20 courses
} GradeStats;

// Attendance statistics structure
typedef struct {
    int total_records;
    int present_count;
    int absent_count;
    int late_count;
    int excused_count;
    float overall_attendance_rate;
    float average_daily_attendance;
    int students_with_perfect_attendance;
    int students_with_poor_attendance;
    float attendance_by_month[12];
} AttendanceStats;

// Club statistics structure
typedef struct {
    int total_clubs;
    int active_clubs;
    int total_memberships;
    int active_memberships;
    float average_members_per_club;
    int most_popular_club_id;
    int least_popular_club_id;
    int clubs_by_category[10];
    int students_in_multiple_clubs;
} ClubStats;

// Statistics management functions
SystemStats* calculate_system_stats(StudentList* students, CourseList* courses, 
                                   GradeList* grades, AttendanceList* attendance, 
                                   ClubList* clubs, MembershipList* memberships);
void display_system_stats(SystemStats* stats);
void free_system_stats(SystemStats* stats);

StudentStats* calculate_student_stats(StudentList* students, GradeList* grades);
void display_student_stats(StudentStats* stats);
void free_student_stats(StudentStats* stats);

GradeStats* calculate_grade_stats(GradeList* grades, CourseList* courses);
void display_grade_stats(GradeStats* stats);
void free_grade_stats(GradeStats* stats);

AttendanceStats* calculate_attendance_stats(AttendanceList* attendance);
void display_attendance_stats(AttendanceStats* stats);
void free_attendance_stats(AttendanceStats* stats);

ClubStats* calculate_club_stats(ClubList* clubs, MembershipList* memberships);
void display_club_stats(ClubStats* stats);
void free_club_stats(ClubStats* stats);

// Top performers analysis
typedef struct {
    int student_id;
    char student_name[MAX_NAME_LENGTH * 2];
    float gpa;
    int rank;
} TopPerformer;

TopPerformer* get_top_performers(StudentList* students, GradeList* grades, int count);
void display_top_performers(TopPerformer* performers, int count);
void free_top_performers(TopPerformer* performers);

// Course performance analysis
typedef struct {
    int course_id;
    char course_name[MAX_COURSE_LENGTH];
    float average_grade;
    int total_students;
    int passing_students;
    float pass_rate;
    int rank;
} CoursePerformance;

CoursePerformance* get_course_performance(GradeList* grades, CourseList* courses, int count);
void display_course_performance(CoursePerformance* performance, int count);
void free_course_performance(CoursePerformance* performance);

// Attendance analysis
typedef struct {
    int student_id;
    char student_name[MAX_NAME_LENGTH * 2];
    float attendance_rate;
    int total_days;
    int absent_days;
    int rank;
} AttendanceAnalysis;

AttendanceAnalysis* get_attendance_analysis(StudentList* students, AttendanceList* attendance, int count);
void display_attendance_analysis(AttendanceAnalysis* analysis, int count);
void free_attendance_analysis(AttendanceAnalysis* analysis);

// Club popularity analysis
typedef struct {
    int club_id;
    char club_name[MAX_CLUB_LENGTH];
    int member_count;
    char category[50];
    float popularity_score;
    int rank;
} ClubPopularity;

ClubPopularity* get_club_popularity(ClubList* clubs, MembershipList* memberships, int count);
void display_club_popularity(ClubPopularity* popularity, int count);
void free_club_popularity(ClubPopularity* popularity);

// Trend analysis
typedef struct {
    time_t start_date;
    time_t end_date;
    float gpa_trend;  // Positive = improving, negative = declining
    float attendance_trend;
    int enrollment_trend;
    int club_participation_trend;
} TrendAnalysis;

TrendAnalysis* analyze_trends(StudentList* students, GradeList* grades, 
                            AttendanceList* attendance, ClubList* clubs, 
                            MembershipList* memberships, time_t start_date, time_t end_date);
void display_trend_analysis(TrendAnalysis* trends);
void free_trend_analysis(TrendAnalysis* trends);

// Statistical calculations
float calculate_mean(float* values, int count);
float calculate_median(float* values, int count);
float calculate_standard_deviation(float* values, int count);
float calculate_percentile(float* values, int count, float percentile);
float calculate_correlation(float* x_values, float* y_values, int count);

// Report generation
int generate_comprehensive_report(SystemStats* sys_stats, StudentStats* student_stats,
                                GradeStats* grade_stats, AttendanceStats* attendance_stats,
                                ClubStats* club_stats, const char* filename);

int generate_student_performance_report(StudentList* students, GradeList* grades,
                                      AttendanceList* attendance, const char* filename);

int generate_course_analysis_report(CourseList* courses, GradeList* grades,
                                  AttendanceList* attendance, const char* filename);

int generate_club_activity_report(ClubList* clubs, MembershipList* memberships,
                                const char* filename);

int generate_monthly_summary_report(StudentList* students, GradeList* grades,
                                  AttendanceList* attendance, ClubList* clubs,
                                  MembershipList* memberships, const char* filename);

// Data visualization helpers
void print_bar_chart(const char* title, const char* labels[], float values[], int count);
void print_pie_chart(const char* title, const char* labels[], int values[], int count);
void print_line_chart(const char* title, const char* x_label, const char* y_label,
                     float* x_values, float* y_values, int count);

// Export functions
int export_stats_to_csv(SystemStats* stats, const char* filename);
int export_student_stats_to_csv(StudentStats* stats, const char* filename);
int export_grade_stats_to_csv(GradeStats* stats, const char* filename);
int export_attendance_stats_to_csv(AttendanceStats* stats, const char* filename);
int export_club_stats_to_csv(ClubStats* stats, const char* filename);

// Utility functions
const char* get_performance_level(float gpa);
const char* get_attendance_level(float attendance_rate);
int is_performance_excellent(float gpa);
int is_performance_good(float gpa);
int is_performance_average(float gpa);
int is_performance_poor(float gpa);
int is_attendance_excellent(float attendance_rate);
int is_attendance_good(float attendance_rate);
int is_attendance_poor(float attendance_rate);

// Constants
#define EXCELLENT_GPA_THRESHOLD 3.5
#define GOOD_GPA_THRESHOLD 3.0
#define AVERAGE_GPA_THRESHOLD 2.5
#define POOR_GPA_THRESHOLD 2.0

#define EXCELLENT_ATTENDANCE_THRESHOLD 0.95
#define GOOD_ATTENDANCE_THRESHOLD 0.85
#define POOR_ATTENDANCE_THRESHOLD 0.75

#define TOP_PERFORMERS_COUNT 10
#define COURSE_ANALYSIS_COUNT 20
#define ATTENDANCE_ANALYSIS_COUNT 15
#define CLUB_POPULARITY_COUNT 10

#endif // STATS_H
