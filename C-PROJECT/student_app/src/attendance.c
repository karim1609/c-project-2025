#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "attendance.h"
#include "config.h"
#include "crypto.h"
#include "ui.h"
#include "file.h"
#include "report.h"
#include "stats.h"
#include "auth.h"
#include "club.h"



AttendanceList* attendance_list_create() {
    AttendanceList* list = (AttendanceList*)malloc(sizeof(AttendanceList));
    if (!list) return NULL;

    list->count = 0;
    list->capacity = 40;
    list->records = (AttendanceRecord*)malloc(sizeof(AttendanceRecord) * list->capacity);
    if (!list->records) {
        free(list);
        return NULL;
    }
    return list;
}

void attendance_list_destroy(AttendanceList **list) {
    if (list == NULL || *list == NULL)
        return;
    if ((*list)->records) {
        free((*list)->records);
        (*list)->records = NULL;
    }
    (*list)->count = 0;
    (*list)->capacity = 0;
    free(*list);
    *list = NULL;
}

int attendance_list_add(AttendanceList *list, AttendanceRecord record) {
    if (list == NULL)
        return 0;

    if (list->count >= list->capacity)
        return 0;

    list->records[list->count++] = record;
    return 1;
}

int attendance_list_remove(AttendanceList* list, int record_id) {
    if (list == NULL || list->count == 0)
        return 0;

    for (int i = 0; i < list->count; i++) {
        if (list->records[i].id == record_id) {
            for (int j = i; j < list->count - 1; j++) {
                list->records[j] = list->records[j + 1];
            }
            list->count--;
            return 1;
        }
    }
    return 0;
}

AttendanceRecord* attendance_list_find_by_id(AttendanceList* list, int record_id) {
    if (list == NULL)
        return NULL;
    for (int i = 0; i < list->count; i++) {
        if (list->records[i].id == record_id) {
            return &(list->records[i]);
        }
    }
    return NULL;
}

AttendanceRecord* attendance_list_find_by_student_date(AttendanceList* list, int student_id, time_t date) {
    if (list == NULL)
        return NULL;
    for (int i = 0; i < list->count; i++) {
        if (list->records[i].student_id == student_id) {
            struct tm tm1 = *localtime(&date);
            struct tm tm2 = *localtime(&(list->records[i].recorded_time));

            if (tm1.tm_year == tm2.tm_year &&
                tm1.tm_mon == tm2.tm_mon &&
                tm1.tm_mday == tm2.tm_mday) {
                return &(list->records[i]);
            }
        }
    }
    return NULL;
}

AttendanceRecord* attendance_list_find_by_course_date(AttendanceList* list, int course_id, time_t date) {
    if (list == NULL)
        return NULL;
    for (int i = 0; i < list->count; i++) {
        if (list->records[i].course_id == course_id) {
            struct tm tm1 = *localtime(&date);
            struct tm tm2 = *localtime(&(list->records[i].date));

            if (tm1.tm_year == tm2.tm_year &&
                tm1.tm_mon == tm2.tm_mon &&
                tm1.tm_mday == tm2.tm_mday) {
                return &(list->records[i]);
            }
        }
    }
    return NULL;
}


int attendance_list_save_to_file(AttendanceList* list, const char* filename) {
    if (!list || !filename)
        return -1;

    FILE* fp = fopen(filename, "wb");
    if (!fp)
        return -1;

    // Save the count
    if (fwrite(&(list->count), sizeof(int), 1, fp) != 1) {
        fclose(fp);
        return -1;
    }

    // Save the records
    if (list->count > 0) {
        if (fwrite(list->records, sizeof(AttendanceRecord), list->count, fp) != (size_t)list->count) {
            fclose(fp);
            return -1;
        }
    }

    fclose(fp);
    return 0;
}

int attendance_list_load_from_file(AttendanceList* list, const char* filename){
    if (!list || !filename)
        return -1;

    FILE* fp = fopen(filename, "rb");
    if (!fp)
        return -1;

    int count = 0;
    // Read the count first
    if (fread(&count, sizeof(int), 1, fp) != 1) {
        fclose(fp);
        return -1;
    }

    // Allocate if necessary
    if (list->records) {
        free(list->records);
        list->records = NULL;
    }
    if (count > 0) {
        list->records = (AttendanceRecord*)malloc(sizeof(AttendanceRecord) * count);
        if (!list->records) {
            fclose(fp);
            return -1;
        }
        if (fread(list->records, sizeof(AttendanceRecord), count, fp) != (size_t)count) {
            free(list->records);
            list->records = NULL;
            fclose(fp);
            return -1;
        }
    } else {
        list->records = NULL;
    }
    list->count = count;
    list->capacity = (count > 0) ? count : 0;

    fclose(fp);
    return 0;
}