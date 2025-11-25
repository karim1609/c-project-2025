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
#include <time.h>
#include <sys/time.h>

ClubList* club_list_create(void){
    ClubList* list = (ClubList*)malloc(sizeof(ClubList));
    if(list == NULL){
        printf("error: failed to create club list\n");
        return NULL;
    }
    Club *clubs = (Club*)malloc(sizeof(Club)*MAX_CLUBS);
    if(clubs == NULL){
        printf("error: failed to allocate memory for clubs\n");
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
    if(list == NULL || list->clubs == NULL){
        return 0;
    }
    if(list->count >= list->capacity){
        printf("error: club list is full\n");
        return 0;
    }
    list->clubs[list->count] = new_club;
    list->count++;
    return 1;
}
int club_list_remove(ClubList* list, int club_id){
    if(list == NULL || list->clubs == NULL){
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
    return 0;
}
Club* club_list_find_by_id(ClubList* list, int club_id){
    if(list == NULL || list->clubs == NULL){
        printf("list is null\n");
        return NULL;
    }
    for(int i = 0; i < list->count; i++){
        if(list->clubs[i].id == club_id){
            return &list->clubs[i];
        }
    }
    printf("club with id %d not found\n", club_id);
    return NULL;
}
Club* club_list_find_by_name(ClubList* list, const char* name){
    if(list == NULL || list->clubs == NULL){
        printf("list is null\n");
        return NULL;
    }
    for(int i = 0; i < list->count; i++){
        if(strcmp(list->clubs[i].name, name) == 0){
            return &list->clubs[i];
        }
    }
    printf("club with name %s not found\n", name);
    return NULL;
} 
void club_list_display_all(ClubList* list){
    if(list == NULL || list->clubs == NULL){
        printf("list is null\n");
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






MembershipList* membership_list_create(void) {
    MembershipList* list = (MembershipList*)malloc(sizeof(MembershipList));
    if (list == NULL) {
        printf("error: could not allocate memory for membership list\n");
        return NULL;
    }
    list->capacity = 16;
    list->count = 0;
    list->memberships = (ClubMembership*)malloc(sizeof(ClubMembership) * list->capacity);
    if (list->memberships == NULL) {
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
        if (new_memberships == NULL) {
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
        fprintf(file, "%d,%s,%s,%s,%d,%d,%d,%d,%lld,%lld,%s,%s,%s,%f,%d\n",
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
            cb->budget,
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
        int fields = sscanf(line, "%d,%49[^,],%499[^,],%49[^,],%d,%d,%d,%d,%lld,%lld,%19[^,],%19[^,],%99[^,],%f,%d",
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
            &cb.budget,
            &cb.is_active
        );

        if (fields == 15) {
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

// Improved version, fixing many critical issues and aligning with your structures.

// Function to create a new club (asks user for input)
Club club_input_new(void) {
    Club c;
    printf("Les informations du club :\n");
    printf("Id: ");
    scanf("%d", &c.id);

    printf("Name: ");
    scanf(" %[^\n]", c.name);

    printf("Description: ");
    scanf(" %[^\n]", c.description);

    printf("Category: ");
    scanf(" %[^\n]", c.category);

    printf("President_id: ");
    scanf("%d", &c.president_id);

    printf("Advisor_id: ");
    scanf("%d", &c.advisor_id);

    printf("Member_count: ");
    scanf("%d", &c.member_count);

    // If founded_date is time_t in struct, user must enter year,month,day and convert
    int year, month, day;
    struct tm tm_date = {0};
    printf("Founded date (jour-mois-annee, e.g. 30-05-2024): ");
    scanf("%d-%d-%d", &day, &month, &year);
    tm_date.tm_mday = day;
    tm_date.tm_mon = month - 1; // struct tm months are 0-11
    tm_date.tm_year = year - 1900;
    c.founded_date = mktime(&tm_date);

    printf("Budget: ");
    scanf("%f", &c.budget);

    c.is_active = 1;
    return c;
}

// Function to edit a club's information
void club_input_edit(Club* club) {
    int choice;
    printf("\nQue voulez-vous modifier ?\n");
    printf(" 1 -> Id\n");
    printf(" 2 -> Nom\n");
    printf(" 3 -> Description\n");
    printf(" 4 -> Category\n");
    printf(" 5 -> President_id\n");
    printf(" 6 -> Advisor_id\n");
    printf(" 7 -> Member_count\n");
    printf(" 8 -> Founded_date\n");
    printf(" 9 -> Budget\n");
    printf("10 -> Is_active\n");
    printf(" 0 -> Annuler\n");
    printf("Choix: ");
    scanf("%d", &choice);

    switch (choice) {
    case 0:
        printf("Rien n'a été modifié.\n");
        break;
    case 1:
        printf("Nouveau Id: ");
        scanf("%d", &club->id);
        printf("Id modifié.\n");
        break;
    case 2:
        printf("Nouveau Name: ");
        scanf(" %[^\n]", club->name);
        printf("Name modifié.\n");
        break;
    case 3:
        printf("Nouveau Description: ");
        scanf(" %[^\n]", club->description);
        printf("Description modifiée.\n");
        break;
    case 4:
        printf("Nouveau Category: ");
        scanf(" %[^\n]", club->category);
        printf("Category modifiée.\n");
        break;
    case 5:
        printf("Nouveau President_Id: ");
        scanf("%d", &club->president_id);
        printf("President_Id modifié.\n");
        break;
    case 6:
        printf("Nouveau Advisor_Id: ");
        scanf("%d", &club->advisor_id);
        printf("Advisor_Id modifié.\n");
        break;
    case 7:
        printf("Nouveau Member_count: ");
        scanf("%d", &club->member_count);
        printf("Member_count modifié.\n");
        break;
    case 8: {
        int day, month, year;
        struct tm tm_date = {0};
        printf("Nouvelle Founded_date (jour-mois-annee): ");
        scanf("%d-%d-%d", &day, &month, &year);
        tm_date.tm_mday = day;
        tm_date.tm_mon = month - 1;
        tm_date.tm_year = year - 1900;
        club->founded_date = mktime(&tm_date);
        printf("Founded_date modifiée.\n");
        break;
    }
    case 9:
        printf("Nouveau Budget: ");
        scanf("%f", &club->budget);
        printf("Budget modifié.\n");
        break;
    case 10:
        printf("Nouvelle Situation (0: not active / 1: active): ");
        scanf("%d", &club->is_active);
        printf("Situation modifiée.\n");
        break;
    default:
        printf("Choix invalide.\n");
        break;
    }
}

// Function to display a summary of all clubs
void club_display_summary(ClubList* list) {
    if (list == NULL || list->clubs == NULL) {
        printf("Error: Invalid club list\n");
        return;
    }
    
    if (list->count == 0) {
        printf("No clubs available.\n");
        return;
    }
    
    printf("\n=== CLUB SUMMARY ===\n");
    printf("%-5s %-30s %-20s %-8s %-8s %-10s\n", 
           "ID", "Name", "Category", "Members", "Max", "Status");
    printf("----------------------------------------------------------------------------\n");
    
    for (int i = 0; i < list->count; i++) {
        Club* club = &list->clubs[i];
        printf("%-5d %-30s %-20s %-8d %-8d %-10s\n",
               club->id,
               club->name,
               club->category,
               club->member_count,
               club->max_members,
               club->is_active ? "Active" : "Inactive");
    }
    
    printf("----------------------------------------------------------------------------\n");
    printf("Total clubs: %d\n\n", list->count);
}

// Function for a student to join a club (creates a new membership)
int join_club(MembershipList* list, int student_id, int club_id, const char* role) {
    if (!list || !role) return 0;

    ClubMembership mmbsh;
    mmbsh.id = 0; // Should be assigned properly elsewhere (e.g., unique id system)
    mmbsh.student_id = student_id;
    mmbsh.club_id = club_id;
    strncpy(mmbsh.role, role, sizeof(mmbsh.role) - 1);
    mmbsh.role[sizeof(mmbsh.role) - 1] = '\0';
    mmbsh.is_active = 1;

    int day, month, year;
    struct tm tm_date = {0};
    printf("Date d'adhesion (jour-mois-annee): ");
    scanf("%d-%d-%d", &day, &month, &year);
    tm_date.tm_mday = day;
    tm_date.tm_mon = month - 1;
    tm_date.tm_year = year - 1900;
    mmbsh.join_date = mktime(&tm_date);

    int result = membership_list_add(list, mmbsh);
    return result;
}

// Function for a student to leave a club (removes membership by membership id)
int leave_club(MembershipList* list, int membership_id) {
    if (list == NULL)
        return 0;
    return membership_list_remove(list, membership_id);
}

