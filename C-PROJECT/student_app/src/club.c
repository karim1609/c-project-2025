#include "student.h"
#include "config.h"
#include "crypto.h"
#include "ui.h"
#include "file_manager.h"
#include "stats.h"
#include "auth.h"
#include "attendance.h"
#include "grade.h"
#include "club.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

MembershipList* membership_list_create(void) {
    MembershipList* list = (MembershipList*)malloc(sizeof(MembershipList));
    if (!list) {
        printf("error: could not allocate memory for membership list\n");
        return NULL;
    }
    list->capacity = 16;
    list->count = 0;
    list->memberships = (ClubMembership*)malloc(sizeof(ClubMembership) * list->capacity);
    if (!list->memberships) {
        printf("error: could not allocate memory for memberships array\n");
        free(list);
        return NULL;
    }
    return list;
}

void membership_list_destroy(MembershipList* list) {
    if (list == NULL) {
        return;
    }
    if (list->memberships != NULL) {
        free(list->memberships);
    }
    free(list);
}

int membership_list_add(MembershipList* list, ClubMembership membership) {
    if (list == NULL || list->memberships == NULL) {
        printf("error: invalid arguments to membership_list_add\n");
        return 0;
    }
    
    if (list->count >= list->capacity) {
        int new_capacity = list->capacity * 2;
        ClubMembership* new_memberships = (ClubMembership*)realloc(list->memberships, sizeof(ClubMembership) * new_capacity);
        if (!new_memberships) {
            printf("error: could not allocate more memory for memberships\n");
            return 0;
        }
        list->memberships = new_memberships;
        list->capacity = new_capacity;
    }
    
    list->memberships[list->count++] = membership;
    return 1;
}

int membership_list_remove(MembershipList* list, int membership_id) {
    if (list == NULL || list->memberships == NULL) {
        printf("error: invalid arguments to membership_list_remove\n");
        return 0;
    }
    
    for (int i = 0; i < list->count; i++) {
        if (list->memberships[i].id == membership_id) {
            for (int j = i; j < list->count - 1; j++) {
                list->memberships[j] = list->memberships[j + 1];
            }
            list->count--;
            return 1;
        }
    }
    printf("error: membership with id %d not found\n", membership_id);
    return 0;
}

ClubMembership* membership_list_find_by_id(MembershipList* list, int membership_id) {
    if (list == NULL || list->memberships == NULL) {
        printf("error: invalid arguments to membership_list_find_by_id\n");
        return NULL;
    }
    
    for (int i = 0; i < list->count; i++) {
        if (list->memberships[i].id == membership_id) {
            return &list->memberships[i];
        }
    }
    return NULL;
}

int club_list_save_to_file(ClubList* list, const char* filename) {
    if (list == NULL || list->clubs == NULL || filename == NULL) {
        printf("error: invalid arguments to club_list_save_to_file\n");
        return 0;
    }
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("error: could not open file %s for writing\n", filename);
        return 0;
    }
    for (int i = 0; i < list->count; i++) {
        Club* cb = &list->clubs[i];
        // Save all fields in a CSV format
        fprintf(file, "%d,%s,%s,%s,%d,%d,%d,%d,%lld,%lld,%s,%s,%s,%d\n",
            cb->id,
            cb->name,
            cb->description,
            cb->category,
            cb->president_id,
            cb->advisor_id,
            cb->member_count,
            cb->max_members,
            (long long)cb->founded_date,
            (long long)cb->last_meeting,
            cb->meeting_day,
            cb->meeting_time,
            cb->meeting_location,
            cb->is_active
        );
    }
    fclose(file);
    return 1;
}
int club_list_load_from_file(ClubList* list, const char* filename){
    if(list == NULL || list->clubs == NULL || filename == NULL){
        printf("error: invalid arguments to club_list_load_from_file\n");
        return 0;
    }

    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("error: could not open file %s for reading\n", filename);
        return 0;
    }

    char line[1024];
    int index = 0;
    while (fgets(line, sizeof(line), file)) {
        Club cb;
        long long founded_date_temp, last_meeting_temp;

        // Parse the CSV line, matching the save format
        int fields = sscanf(line, "%d,%[^,],%[^,],%[^,],%d,%d,%d,%d,%lld,%lld,%[^,],%[^,],%[^,],%d",
            &cb.id,
            cb.name,
            cb.description,
            cb.category,
            &cb.president_id,
            &cb.advisor_id,
            &cb.member_count,
            &cb.max_members,
            &founded_date_temp,
            &last_meeting_temp,
            cb.meeting_day,
            cb.meeting_time,
            cb.meeting_location,
            &cb.is_active
        );

        if (fields == 14) {
            cb.founded_date = (time_t)founded_date_temp;
            cb.last_meeting = (time_t)last_meeting_temp;

            if (index < list->capacity) {
                list->clubs[index++] = cb;
            } else {
                // Reallocate more space if needed (double the capacity)
                int new_capacity = list->capacity ? list->capacity * 2 : 16;
                Club* new_clubs = realloc(list->clubs, sizeof(Club) * new_capacity);
                if (!new_clubs) {
                    printf("error: could not allocate more memory for clubs\n");
                    fclose(file);
                    return 0;
                }
                list->clubs = new_clubs;
                list->capacity = new_capacity;
                list->clubs[index++] = cb;
            }
        }
    }
    list->count = index;
    fclose(file);
    return 1;
}
int membership_list_save_to_file(MembershipList* list, const char* filename) {
    if (list == NULL || list->memberships == NULL || filename == NULL) {
        printf("error: invalid arguments to membership_list_save_to_file\n");
        return 0;
    }
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("error: could not open file %s for writing\n", filename);
        return 0;
    }
    for (int i = 0; i < list->count; i++) {
        ClubMembership* mmbsh = &list->memberships[i];
        // id,student_id,club_id,join_date,role,is_active
        fprintf(file, "%d,%d,%d,%lld,%s,%d\n",
            mmbsh->id,
            mmbsh->student_id,
            mmbsh->club_id,
            (long long)mmbsh->join_date,
            mmbsh->role,
            mmbsh->is_active
        );
    }
    fclose(file);
    return 1;
}
int membership_list_load_from_file(MembershipList* list, const char* filename){
    if (list == NULL || list->memberships == NULL || filename == NULL) {
        printf("error: invalid arguments to membership_list_load_from_file\n");
        return 0;
    }
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("error: could not open file %s for reading\n", filename);
        return 0;
    }

    char line[512];
    int index = 0;
    while (fgets(line, sizeof(line), file)) {
        ClubMembership mmbsh;
        long long join_date_tmp;

        // id,student_id,club_id,join_date,role,is_active
        int fields = sscanf(line, "%d,%d,%d,%lld,%49[^,],%d",
            &mmbsh.id,
            &mmbsh.student_id,
            &mmbsh.club_id,
            &join_date_tmp,
            mmbsh.role,
            &mmbsh.is_active
        );

        if (fields == 6) {
            mmbsh.join_date = (time_t)join_date_tmp;

            if (index < list->capacity) {
                list->memberships[index++] = mmbsh;
            } else {
                // Reallocate more space if needed (double the capacity)
                int new_capacity = list->capacity ? list->capacity * 2 : 16;
                ClubMembership* new_memberships = realloc(list->memberships, sizeof(ClubMembership) * new_capacity);
                if (!new_memberships) {
                    printf("error: could not allocate more memory for memberships\n");
                    fclose(file);
                    return 0;
                }
                list->memberships = new_memberships;
                list->capacity = new_capacity;
                list->memberships[index++] = mmbsh;
            }
        }
    }
    list->count = index;
    fclose(file);
    return 1;
}

