#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#define VERSION "0.1.1"


int file_exists(char *file) {
    if (access(file, R_OK) == -1)
        return 0;
    return 1;
}

int get_config(char *file, char *name, char *c_item, char *error) {
    char* line = NULL;
    size_t len = 0;
    ssize_t read;

    if (!file_exists(file)) {
        strcpy(error, "file does not exist");
        return 1;
    }
    FILE* fp = fopen(file, "r");
    if (fp == NULL) {
        strcpy(error, "could not read from file");
        return 1;
    }
    while ((read = getline(&line, &len, fp)) != -1) {
        /* check for comments and empty lines. */
        if(strchr(line, '#') != NULL)
            continue;
        if(isspace(*line))
            continue;

        int i = 0;
        int found = 0;
        char* elem;

        /* Split the string by delimiter '=' */
        elem = strtok(line, "=");
        while (elem != NULL) {
            if (found == 1) {
                /* Item was found, allocate some memory and return to caller. */
                strcpy(c_item, elem);
                fclose(fp);
                if (line)
                    free(line);
                return 0;
            }
            int match = strcmp(elem, name);
            if (match == 0) {
                found++;
            }
            elem = strtok(NULL, "=");
            i++;
        }
    }
    /* If nothing matches, close file, free mem and return. */
    fclose(fp);
    if (line)
        free(line);
    strcpy(error, "item was not found");
    return 1;
}

// Casual help (print) message.
void help() {
    printf("parser -- v%s\n", VERSION);
    printf("Usage: parser -c /path/to/conf -i myConfigItem\n\n");
    printf("   -c\tprovide the path to the config file\n");
    printf("   -i\tspecify the item you want to retrieve\n");
    printf("\n");
}

int main(int argc, char* argv[]) {
    // Declare used vars and allocate some mem.
    char *item = malloc(sizeof(char) * 1000);
    char *err = malloc(sizeof(char) * 100);
    char *config_item = malloc(sizeof(char) * 100);
    char *config_file = malloc(sizeof(char) * 300); 

    if (argc == 1) {
        printf("error: no arguments provided\n");
        help();
        exit(EXIT_FAILURE);
    } else if (argc <= 4) {
        printf("error: not enough arguments\n");
        help();
        exit(EXIT_FAILURE);
    }

    // Check the proided arugments and define the config_item and the config_file.
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0) {
            help();
            exit(EXIT_SUCCESS);
        } else if (strcmp(argv[i], "-c") == 0) {
            if (argv[i+1] == NULL) {
                printf("error: no config path provided\n");
                help();
                exit(EXIT_FAILURE);
            }
            if (strlen(argv[i+1]) > 275) {
                printf("error: config path is too long\n");
                exit(EXIT_FAILURE);
            } 
            strcpy(config_file, argv[i+1]);
        } else if (strcmp(argv[i], "-i") == 0) {
            if (argv[i+1] == NULL) {
                printf("error: item to retrieve not given\n");
                help();
                exit(EXIT_FAILURE);
            }
            if (strlen(argv[i+1]) > 100) {
                printf("error: config item is too long\n");
                exit(EXIT_FAILURE);
            }
            strcpy(config_item, argv[i+1]);
        }
    }

    int result = get_config(config_file, config_item, item, err);
    if (result) {
        printf("error: %s\n", err);
        exit(EXIT_FAILURE);
    }
    printf("%s is: %s\n", config_item, item);
    exit(EXIT_SUCCESS);
}
