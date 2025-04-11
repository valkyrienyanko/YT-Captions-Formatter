#include <ctype.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "utils.h"

#define MAX_LINE_LENGTH 256
#define MAX_LINES 1000

// For example: "hello World" becomes "hello. World"
static void add_periods_before_capitals(char *line) 
{
    int length = (int)strlen(line);
    char* result = (char*)malloc(((size_t)length * 2 + 1) * sizeof(char));
    
    if (result == NULL) 
    {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    
    int j = 0;

    for (int i = 0; i < length; i++) 
    {
        if (i > 0 && isupper(line[i]) && line[i - 1] == ' ') 
        {
            result[j - 1] = '.';
            result[j++] = ' ';
        }
        result[j++] = line[i];
    }
    
    result[j] = '\0';
    strcpy(line, result);
    free(result);
}

static void trim(char* line)
{
    char* start = line;
    while (isspace((unsigned char)*start)) start++;
    char* end = line + strlen(line) - 1;
    while (end > start && isspace((unsigned char)*end)) end--;
    *(end + 1) = '\0';
}

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
        int line_length = (int)strlen(line);
    
        // This is not a blank line or a line that contains a timestamp
        if (line[0] != '\n' && !contains_timestamp(line, line_length)) 
        {
            // Remove newline character at the end of the line
            if (line_length > 0 && line[line_length - 1] == '\n') 
            {
                line[line_length - 1] = '\0';
            }
            
            // Remove whitespace characters from beginning and ending of line
            trim(line);
            
            add_periods_before_capitals(line);
            
            // Keep track of line
            lines[line_count++] = strdup(line);
        }
    }

    fclose(input_file);
    
    FILE* output_file = fopen(file_name, "w");
    
    for (int i = 0; i < line_count; i++)
    {
        fprintf(output_file, "%s ", lines[i]);
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
