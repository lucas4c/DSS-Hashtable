#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 20000000
#define OUTPUT_FILE_NAME "experimento20000000.txt"

typedef struct entry_t {
    char *key;
    char *value;
    struct entry_t *next;
} entry_t;

typedef struct {
    entry_t **entries;
} ht_t;

unsigned int hash(const char *key) {
    unsigned long int value = 0;
    unsigned int i = 0;
    unsigned int key_len = strlen(key);

    for(; i < key_len; i++) {
        value = value * 83 + key[i];
    }

    value = value % TABLE_SIZE;

    return value;
}

entry_t *htPair (const char *key, const char *value) {
    entry_t *entry = malloc(sizeof(entry_t) * 1);
    entry->key = malloc(strlen(key) + 1);
    entry->value = malloc(strlen(value) + 1);

    strcpy(entry->key, key);
    strcpy(entry->value, value);

    entry->next = NULL;

    return entry;
}

ht_t *htCreate(void) {
    ht_t *hashtable = malloc(sizeof(ht_t) * 1);

    hashtable->entries = malloc(sizeof(entry_t*) * TABLE_SIZE);

    for (int i=0; i < TABLE_SIZE; ++i) {
        hashtable->entries[i] = NULL;
    }

    return hashtable;
}





void htSet(ht_t *hashtable, const char *key, const char *value) {
    unsigned int slot = hash(key);

    entry_t *entry = hashtable->entries[slot];

    if (entry==NULL) {
        hashtable->entries[slot] = htPair(key, value);
        return;
    }

    entry_t *prev;

    while (entry!=NULL) {
        if(strcmp(entry->key, key) == 0) {
            free(entry->value);
            entry->value = malloc(strlen(value)+1);
            strcpy(entry->value, value);
            return;
        }

        prev = entry;
        entry = prev->next;
    }

    prev->next = htPair(key,value);
}

int htGet(ht_t *hashtable, const char *key) {
    int queryNumber = 0;
    unsigned int slot = hash(key);

    entry_t *entry = hashtable->entries[slot];

    if(entry==NULL) {
        return 1;
    }

    while (entry !=NULL) {
        queryNumber++;
        if(strcmp(entry->key, key) == 0) {
            return queryNumber;
        }
        entry = entry->next;
    }
    return queryNumber+1;
}

void htDump (ht_t *hashtable) {
    for(int i=0; i < TABLE_SIZE; ++i) {
        entry_t *entry = hashtable->entries[i];

        if(entry==NULL) {
            continue;
        }

        printf("slot[%4d]: ", i);

        for (;;) {
            printf("%s=%s ", entry->key, entry->value);

            if(entry->next == NULL) {
                break;
            }

            entry = entry->next;
        }

        printf("\n");
    }
}

int max(int arr[], int n) {
    int max = arr[0];

    for (int i=1; i < n; i++)
        if(arr[i] > max)
            max = arr[i];

    return max;
}

int average(int arr[], int n) {
    int sum = 0;
    for (int i=0; i < n; i++)
        sum += arr[i];

    return sum/n;
}

void printArray(int *arr, int size) {
    for(int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
}

int main() {
    FILE *names;
    FILE *query;
    FILE *output;
    char line[256];
    char name[128];

    ht_t *ht = htCreate();

    names = fopen("nomes_10000.txt", "r");

    while (fgets(line, sizeof(line), names)) {
        line[strlen(line)-1] = '\0';
        //printf("(%s) ", line);
        htSet(ht, line, line);

        //printf("\n\n");
    }
    //htDump(ht);

    fclose(names);
    query = fopen("consultas.txt", "r");
    output = fopen(OUTPUT_FILE_NAME, "w");
    int queryComparisons[50] = {0};
    int queryCounter = 0;

    while (fgets(line, sizeof(line), query)) {
        line[strlen(line)-1] = '\0';
        queryComparisons[queryCounter] = htGet(ht, line);
        fprintf(output, "%s #%d\n", line, queryComparisons[queryCounter]);
        queryCounter++;
    }

    fclose(query);

    int queryMax = max(queryComparisons, queryCounter);
    int queryAvg = average(queryComparisons, queryCounter);

    fprintf(output, "MEDIA #%d\n", queryAvg);
    fprintf(output, "MAXIMO #%d\n", queryMax);

    fclose(output);


    return 0;
}
