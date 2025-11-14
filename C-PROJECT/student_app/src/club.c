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

ClubList* club_list_create(void){
    clublist* list = (clublist*)malloc(sizeof(clubliste));
    if(list == NULL){
        printf("error: faild to create club list");
        return NULL;
    }
    club *clubs = (club*)malloc(sizeof(club)*MAX_CLUBS);
    if(clubs == NULL){
        printf("error: faild to allocate memory for clubs");
        free(list);
        return NULL;
    }
    list->clubs = clubs;
    list->count = 0;
    list->capacity = MAX_CLUBS;
    return list;    

}
void club_list_destroy(ClubList* list){
    if(list == NULL){
        return;
    }
    if(list->clubs != NULL){
        free(list->clubs);
    }
    free(list);
}

int club_list_add(ClubList* list, Club new_club){
    if(list == NULL){
        return 0;
    }
    if(list->count >= list->capacity){
        printf("error: club list is full");
        return 0;
    }
    list->clubs[list->count] = new_club;
    list->count++;
    return 1;
}
int club_list_remove(ClubList* list, int club_id){
    if(list == NULL){
        return 0;
    }
    for(int i = 0; i < list->count; i++){
        if(list->clubs[i].id == club_id){
            for(int j = i; j < list->count - 1; j++){
                list->clubs[j] = list->clubs[j + 1];
            }
            memset(&list->clubs[list->count - 1], 0, sizeof(Club));
            list->count--;
            return 1;
        }
        
}
Club* club_list_find_by_id(ClubList* list, int club_id){
    if(list == NULL){
        printf("list is null");
        return NULL;
    }
    for(int i = 0; i < list->count; i++){
        if(list->clubs[i].id == club_id){
            return &list->clubs[i];
        }
    }
    printf("club with id %d not found", club_id);
    return NULL;
}
Club* club_list_find_by_name(ClubList* list, const char* name){
    if(list == NULL){
        printf("list is null");
        return NULL;
    }
    for(int i = 0; i < list->count; i++){
        if(strcmp(list->clubs[i].name, name) == 0){
            return &list->clubs[i];
        }
    }
    printf("club with name %s not found", name);
    return NULL;
} 
void club_list_display_all(ClubList* list){
    if(list == NULL){
        printf("list is null");
        return;
    }
  for(int i = 0; i < list->count; i++){
    printf("\nClub %d:\n", i + 1);
    printf("ID: %d\n", list->clubs[i].id);
    printf("Name: %s\n", list->clubs[i].name);
    printf("Description: %s\n", list->clubs[i].description);
    printf("Category: %s\n", list->clubs[i].category);
    printf("President ID: %d\n", list->clubs[i].president_id);
    printf("Advisor ID: %d\n", list->clubs[i].advisor_id);
    printf("Member Count: %d\n", list->clubs[i].member_count);
    printf("Meeting Day: %s\n", list->clubs[i].meeting_day);
    printf("Meeting Time: %s\n", list->clubs[i].meeting_time);
    printf("Meeting Location: %s\n", list->clubs[i].meeting_location);
    printf("Is Active: %d\n", list->clubs[i].is_active);
    printf("--------------------\n");
  }
}
void club_list_display_club(Club* club){
    if(club == NULL){
        printf("club is null");
        return;
    }
    printf("\nClub information:\n");
    printf("ID: %d\n", club->id);
    printf("Name: %s\n", club->name);
    printf("Description: %s\n", club->description);
    printf("Category: %s\n", club->category);
    printf("President ID: %d\n", club->president_id);
    printf("Advisor ID: %d\n", club->advisor_id);
    printf("Member Count: %d\n", club->member_count);
    printf("Meeting Day: %s\n", club->meeting_day);
    printf("Meeting Time: %s\n", club->meeting_time);
    printf("Meeting Location: %s\n", club->meeting_location);
    printf("Is Active: %d\n", club->is_active);
    printf("--------------------\n");
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
