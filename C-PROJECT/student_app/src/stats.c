#include "stats.h"


SystemStats* calculate_system_stats(StudentList* students, CourseList* courses, 
                                   GradeList* grades, AttendanceList* attendance, 
                                   ClubList* clubs, MembershipList* memberships) {
    SystemStats* stats = (SystemStats*)malloc(sizeof(SystemStats));
    if (!stats) return NULL;
    
    memset(stats, 0, sizeof(SystemStats));
    
    // Count students
    if (students) {
        stats->total_students = students->count;
        for (int i = 0; i < students->count; i++) {
            if (students->students[i].is_active) {
                stats->active_students++;
            } else {
                stats->inactive_students++;
            }
        }
    }
    
    // Count courses
    if (courses) {
        stats->total_courses = courses->count;
    }
    
    // Count grades
    if (grades) {
        stats->total_grades = grades->count;
    }
    
    // Count attendance records
    if (attendance) {
        stats->total_attendance_records = attendance->count;
    }
    
    // Count clubs
    if (clubs) {
        stats->total_clubs = clubs->count;
    }
    
    // Count memberships
    if (memberships) {
        stats->total_memberships = memberships->count;
    }
    
    stats->last_updated = time(NULL);
    
    return stats;
}

void display_system_stats(SystemStats* stats) {
    if (!stats) {
        printf("No statistics available.\n");
        return;
    }
    
    printf("\n========== SYSTEM STATISTICS OVERVIEW ==========\n\n");
    printf("Total Students:           %d\n", stats->total_students);
    printf("  - Active:               %d\n", stats->active_students);
    printf("  - Inactive:             %d\n", stats->inactive_students);
    printf("\n");
    printf("Total Courses:            %d\n", stats->total_courses);
    printf("Total Grades:             %d\n", stats->total_grades);
    printf("Total Attendance Records: %d\n", stats->total_attendance_records);
    printf("\n");
    printf("Total Clubs:              %d\n", stats->total_clubs);
    printf("Total Memberships:        %d\n", stats->total_memberships);
    printf("\n");
    
    char time_str[26];
    struct tm* timeinfo = localtime(&stats->last_updated);
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", timeinfo);
    printf("Last Updated: %s\n", time_str);
    printf("\n================================================\n");
}

void free_system_stats(SystemStats* stats) {
    if (stats) {
        free(stats);
    }
}


StudentStats* calculate_student_stats(StudentList* students, GradeList* grades) {
    if (!students || students->count == 0) return NULL;
    
    StudentStats* stats = (StudentStats*)malloc(sizeof(StudentStats));
    if (!stats) return NULL;
    
    memset(stats, 0, sizeof(StudentStats));
    
    stats->total_students = students->count;
    
    int total_age = 0;
    int student_count_with_age = 0;
    
    // Calculate statistics for each student
    for (int i = 0; i < students->count; i++) {
        Student* s = &students->students[i];
        
        // Count by year
        if (s->year >= 1 && s->year <= 4) {
            stats->students_by_year[s->year]++;
        }
        
        // Use age directly from student structure
        int age = s->age;
        
        if (age > 0 && age < 100) {
            total_age += age;
            student_count_with_age++;
            
            // Age distribution (16-65+)
            int age_index = (age - 16) / 5;
            if (age_index < 0) age_index = 0;
            if (age_index >= 10) age_index = 9;
            stats->age_distribution[age_index]++;
        }
    }
    
    // Calculate average age
    if (student_count_with_age > 0) {
        stats->average_age = (float)total_age / student_count_with_age;
    }
    
    // Calculate GPA statistics if grades are available
    if (grades && grades->count > 0) {
        float total_gpa = 0;
        int student_gpa_count = 0;
        
        typedef struct {
            int student_id;
            float gpa;
        } StudentGPA;
        
        StudentGPA* student_gpas = (StudentGPA*)malloc(sizeof(StudentGPA) * students->count);
        if (!student_gpas) {
            free(stats);
            return NULL;
        }
        
        // Calculate GPA for each student
        for (int i = 0; i < students->count; i++) {
            float gpa = calculate_student_gpa(grades, students->students[i].id);
            if (gpa >= 0) {
                student_gpas[student_gpa_count].student_id = students->students[i].id;
                student_gpas[student_gpa_count].gpa = gpa;
                total_gpa += gpa;
                student_gpa_count++;
                
                // GPA distribution
                int gpa_index = (int)(gpa / 1.0);
                if (gpa_index >= 5) gpa_index = 4;
                stats->gpa_distribution[gpa_index]++;
            }
        }
        
        if (student_gpa_count > 0) {
            stats->average_gpa = total_gpa / student_gpa_count;
            
            // Sort by GPA for top/bottom performers
            for (int i = 0; i < student_gpa_count - 1; i++) {
                for (int j = 0; j < student_gpa_count - i - 1; j++) {
                    if (student_gpas[j].gpa < student_gpas[j + 1].gpa) {
                        StudentGPA temp = student_gpas[j];
                        student_gpas[j] = student_gpas[j + 1];
                        student_gpas[j + 1] = temp;
                    }
                }
            }
            
            // Top 10 performers
            int top_count = (student_gpa_count < 10) ? student_gpa_count : 10;
            for (int i = 0; i < top_count; i++) {
                stats->top_performers[i] = student_gpas[i].student_id;
            }
            
            // Bottom 10 performers (struggling students) - reverse order
            int bottom_count = (student_gpa_count < 10) ? student_gpa_count : 10;
            int start_idx = student_gpa_count - bottom_count;
            for (int i = 0; i < bottom_count; i++) {
                stats->struggling_students[i] = student_gpas[start_idx + i].student_id;
            }
        }
        
        free(student_gpas);
    }
    
    return stats;
}

void display_student_stats(StudentStats* stats) {
    if (!stats) {
        printf("No student statistics available.\n");
        return;
    }
    
    printf("\n========== STUDENT STATISTICS ==========\n\n");
    printf("Total Students: %d\n\n", stats->total_students);
    
    printf("Distribution by Year:\n");
    for (int i = 1; i <= 4; i++) {
        printf("  Year %d: %d students\n", i, stats->students_by_year[i]);
    }
    printf("\n");
    
    printf("Average Age: %.1f years\n\n", stats->average_age);
    
    printf("Age Distribution:\n");
    const char* age_ranges[] = {
        "16-20", "21-25", "26-30", "31-35", "36-40",
        "41-45", "46-50", "51-55", "56-60", "61+"
    };
    for (int i = 0; i < 10; i++) {
        if (stats->age_distribution[i] > 0) {
            printf("  %s: %d students\n", age_ranges[i], stats->age_distribution[i]);
        }
    }
    printf("\n");
    
    printf("Average GPA: %.2f\n\n", stats->average_gpa);
    
    printf("GPA Distribution:\n");
    const char* gpa_ranges[] = {"0.0-1.0", "1.0-2.0", "2.0-3.0", "3.0-4.0", "4.0+"};
    for (int i = 0; i < 5; i++) {
        if (stats->gpa_distribution[i] > 0) {
            printf("  %s: %d students\n", gpa_ranges[i], stats->gpa_distribution[i]);
        }
    }
    printf("\n");
    
    printf("Top 10 Performers (Student IDs): ");
    int found_top = 0;
    for (int i = 0; i < 10 && stats->top_performers[i] != 0; i++) {
        if (found_top > 0) printf(", ");
        printf("%d", stats->top_performers[i]);
        found_top++;
    }
    if (found_top == 0) printf("None");
    printf("\n");
    
    printf("Struggling Students (Student IDs): ");
    int found_struggling = 0;
    for (int i = 0; i < 10 && stats->struggling_students[i] != 0; i++) {
        if (found_struggling > 0) printf(", ");
        printf("%d", stats->struggling_students[i]);
        found_struggling++;
    }
    if (found_struggling == 0) printf("None");
    printf("\n");
    
    printf("\n========================================\n");
}

void free_student_stats(StudentStats* stats) {
    if (stats) {
        free(stats);
    }
}


GradeStats* calculate_grade_stats(GradeList* grades, CourseList* courses) {
    if (!grades || grades->count == 0) return NULL;
    
    GradeStats* stats = (GradeStats*)malloc(sizeof(GradeStats));
    if (!stats) return NULL;
    
    memset(stats, 0, sizeof(GradeStats));
    
    stats->total_grades = grades->count;
    stats->highest_gpa = 0.0;
    stats->lowest_gpa = 5.0;
    
    float total_gpa = 0;
    int gpa_count = 0;
    
    // Analyze each grade
    for (int i = 0; i < grades->count; i++) {
        Grade* g = &grades->grades[i];
        
        // Count by grade level
        switch (g->grade_level) {
            case GRADE_A: stats->grades_by_level[0]++; break;
            case GRADE_B: stats->grades_by_level[1]++; break;
            case GRADE_C: stats->grades_by_level[2]++; break;
            case GRADE_D: stats->grades_by_level[3]++; break;
            case GRADE_F: stats->grades_by_level[4]++; break;
        }
        
        // Count passing/failing
        if (g->grade_level != GRADE_F) {
            stats->passing_grades++;
        } else {
            stats->failing_grades++;
        }
        
        // Calculate GPA statistics
        float gpa = grade_level_to_numeric(g->grade_level);
        if (gpa >= 0) {
            total_gpa += gpa;
            gpa_count++;
            
            if (gpa > stats->highest_gpa) {
                stats->highest_gpa = gpa;
            }
            if (gpa < stats->lowest_gpa) {
                stats->lowest_gpa = gpa;
            }
        }
    }
    
    // Calculate averages
    if (gpa_count > 0) {
        stats->average_gpa = total_gpa / gpa_count;
    }
    
    if (stats->total_grades > 0) {
        stats->pass_rate = (float)stats->passing_grades / stats->total_grades * 100.0;
    }
    
    return stats;
}

void display_grade_stats(GradeStats* stats) {
    if (!stats) {
        printf("No grade statistics available.\n");
        return;
    }
    
    printf("\n========== GRADE STATISTICS ==========\n\n");
    printf("Total Grades: %d\n\n", stats->total_grades);
    
    printf("Grade Distribution:\n");
    const char* grade_labels[] = {"A", "B", "C", "D", "F"};
    for (int i = 0; i < 5; i++) {
        float percentage = (stats->total_grades > 0) ? 
                          (float)stats->grades_by_level[i] / stats->total_grades * 100.0 : 0;
        printf("  %s: %d (%.1f%%)\n", grade_labels[i], stats->grades_by_level[i], percentage);
    }
    printf("\n");
    
    printf("Average GPA: %.2f\n", stats->average_gpa);
    printf("Highest GPA: %.2f\n", stats->highest_gpa);
    printf("Lowest GPA:  %.2f\n", stats->lowest_gpa);
    printf("\n");
    
    printf("Passing Grades: %d\n", stats->passing_grades);
    printf("Failing Grades: %d\n", stats->failing_grades);
    printf("Pass Rate: %.1f%%\n", stats->pass_rate);
    
    printf("\n======================================\n");
}

void free_grade_stats(GradeStats* stats) {
    if (stats) {
        free(stats);
    }
}


AttendanceStats* calculate_attendance_stats(AttendanceList* attendance) {
    if (!attendance || attendance->count == 0) return NULL;
    
    AttendanceStats* stats = (AttendanceStats*)malloc(sizeof(AttendanceStats));
    if (!stats) return NULL;
    
    memset(stats, 0, sizeof(AttendanceStats));
    
    stats->total_records = attendance->count;
    
    int month_counts[12] = {0};
    
    // Analyze each attendance record
    for (int i = 0; i < attendance->count; i++) {
        AttendanceRecord* a = &attendance->records[i];
        
        // Count by status
        switch (a->status) {
            case ATTENDANCE_PRESENT:
                stats->present_count++;
                break;
            case ATTENDANCE_ABSENT:
                stats->absent_count++;
                break;
            case ATTENDANCE_LATE:
                stats->late_count++;
                break;
            case ATTENDANCE_EXCUSED:
                stats->excused_count++;
                break;
        }
        
        // Count by month
        struct tm* date_tm = localtime(&a->date);
        int month = date_tm->tm_mon;
        if (month >= 0 && month < 12) {
            month_counts[month]++;
            if (a->status == ATTENDANCE_PRESENT || a->status == ATTENDANCE_LATE) {
                stats->attendance_by_month[month]++;
            }
        }
    }
    
    // Calculate overall attendance rate
    int total_countable = stats->present_count + stats->absent_count + 
                         stats->late_count + stats->excused_count;
    if (total_countable > 0) {
        stats->overall_attendance_rate = 
            (float)(stats->present_count + stats->late_count) / total_countable * 100.0;
    }
    
    // Calculate monthly attendance rates
    for (int i = 0; i < 12; i++) {
        if (month_counts[i] > 0) {
            stats->attendance_by_month[i] = 
                stats->attendance_by_month[i] / month_counts[i] * 100.0;
        }
    }
    
    return stats;
}

void display_attendance_stats(AttendanceStats* stats) {
    if (!stats) {
        printf("No attendance statistics available.\n");
        return;
    }
    
    printf("\n========== ATTENDANCE STATISTICS ==========\n\n");
    printf("Total Records: %d\n\n", stats->total_records);
    
    printf("Status Distribution:\n");
    printf("  Present: %d\n", stats->present_count);
    printf("  Absent:  %d\n", stats->absent_count);
    printf("  Late:    %d\n", stats->late_count);
    printf("  Excused: %d\n", stats->excused_count);
    printf("\n");
    
    printf("Overall Attendance Rate: %.1f%%\n\n", stats->overall_attendance_rate);
    
    printf("Monthly Attendance Rates:\n");
    const char* months[] = {
        "January", "February", "March", "April", "May", "June",
        "July", "August", "September", "October", "November", "December"
    };
    for (int i = 0; i < 12; i++) {
        if (stats->attendance_by_month[i] > 0) {
            printf("  %s: %.1f%%\n", months[i], stats->attendance_by_month[i]);
        }
    }
    
    printf("\n===========================================\n");
}

void free_attendance_stats(AttendanceStats* stats) {
    if (stats) {
        free(stats);
    }
}


ClubStats* calculate_club_stats(ClubList* clubs, MembershipList* memberships) {
    if (!clubs || clubs->count == 0) return NULL;
    
    ClubStats* stats = (ClubStats*)malloc(sizeof(ClubStats));
    if (!stats) return NULL;
    
    memset(stats, 0, sizeof(ClubStats));
    
    stats->total_clubs = clubs->count;
    
    int max_members = 0;
    int min_members = 999999;
    
    // Analyze each club
    for (int i = 0; i < clubs->count; i++) {
        Club* c = &clubs->clubs[i];
        
        if (c->is_active) {
            stats->active_clubs++;
        }
        
        // Count memberships for this club
        int club_members = 0;
        if (memberships) {
            for (int j = 0; j < memberships->count; j++) {
                if (memberships->memberships[j].club_id == c->id &&
                    memberships->memberships[j].is_active) {
                    club_members++;
                }
            }
        }
        
        // Track most/least popular clubs
        if (club_members > max_members) {
            max_members = club_members;
            stats->most_popular_club_id = c->id;
        }
        if (club_members < min_members && c->is_active) {
            min_members = club_members;
            stats->least_popular_club_id = c->id;
        }
    }
    
    // Count total memberships
    if (memberships) {
        for (int i = 0; i < memberships->count; i++) {
            stats->total_memberships++;
            if (memberships->memberships[i].is_active) {
                stats->active_memberships++;
            }
        }
    }
    
    // Calculate average members per club
    if (stats->total_clubs > 0) {
        stats->average_members_per_club = 
            (float)stats->active_memberships / stats->total_clubs;
    }
    
    return stats;
}

void display_club_stats(ClubStats* stats) {
    if (!stats) {
        printf("No club statistics available.\n");
        return;
    }
    
    printf("\n========== CLUB STATISTICS ==========\n\n");
    printf("Total Clubs: %d\n", stats->total_clubs);
    printf("  Active Clubs: %d\n\n", stats->active_clubs);
    
    printf("Total Memberships: %d\n", stats->total_memberships);
    printf("  Active Memberships: %d\n\n", stats->active_memberships);
    
    printf("Average Members per Club: %.1f\n\n", stats->average_members_per_club);
    
    printf("Most Popular Club ID: %d\n", stats->most_popular_club_id);
    printf("Least Popular Club ID: %d\n", stats->least_popular_club_id);
    
    printf("\n=====================================\n");
}

void free_club_stats(ClubStats* stats) {
    if (stats) {
        free(stats);
    }
}