#include "backup.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>

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
        fprintf("Erreur : liste invalide\n");
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


