#include <ctype.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "utils.h"

#define MAX_LINE_LENGTH 256
#define MAX_LINES 1000

void format_captions_file(char* file_name)
{
    FILE* input_file = fopen(file_name, "r");
    
    if (input_file == NULL) {
        perror("Error opening file");
        return;
    }
    
    char line[MAX_LINE_LENGTH];
    char* lines[MAX_LINES];
    int line_count = 0;

    while (fgets(line, sizeof(line), input_file)) 
    {
        if (line[0] != '\n' && !contains_timestamp(line, (int)strlen(line))) 
        {
            lines[line_count++] = strdup(line);
        }
    }

    fclose(input_file);
    
    FILE* output_file = fopen(file_name, "w");
    
    for (int i = 0; i < line_count; i++)
    {
        fprintf(output_file, "%s", lines[i]);
        free(lines[i]);
    }
    
    fclose(output_file);
}

void traverse_directory(char* dir_path, void (*file)(char*))
{
    DIR* d;
    
    struct dirent* dir;
    d = opendir(dir_path);
    
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            char* name = dir->d_name;
            
            if (strcmp(name, ".") != 0 && strcmp(name, "..") != 0)
            {
                file(name);
            }
        }
        closedir(d);
    }
}

bool contains_timestamp(char* line, int line_length)
{
    int colon_index = -1;

    for (int i = 0; i < line_length; i++)
    {
        if (line[i] == ':')
        {
            colon_index = i;
            break;
        }    
    }
    
    if (colon_index == -1)
    {
        return false;
    }
    
    bool number_on_left = colon_index - 1 >= 0 && isdigit(line[colon_index - 1]);
    bool number_on_right = colon_index + 1 < line_length && isdigit(line[colon_index + 1]);
    
    bool is_timestamp = number_on_left && number_on_right;
    
    return is_timestamp;
}
