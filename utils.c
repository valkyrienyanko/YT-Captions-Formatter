#include <ctype.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "utils.h"

#define MAX_LINE_LENGTH 256
#define MAX_LINES 1000

// For example: "Sentence one Xentence two Sentence three " becomes "Sentence one. Xentence two. Sentence three "
static void add_periods_before_capitals(char *line) 
{
    size_t length = strlen(line);
    if (!line || length == 0)
        return;

    size_t buff_len = length;
    
    // Count periods required to then allocate with correct size
    for (size_t i = 1; i < length - 1; i++)
    {
        if (isalpha(line[i - 1]) && line[i] == ' ' && isupper(line[i + 1]))
        {
            buff_len += 1;
        }
    }


    char* buff = (char*)malloc(buff_len + 1);
    if (!buff)
    {
        return;
    }
    
    buff[0] = line[0];
    int buff_index = 1;
    for (size_t i = 1; i < length; i++)
    {
        if (isalpha(line[i - 1]) && line[i] == ' ' && isupper(line[i + 1]))
        {
            buff[buff_index++] = '.';
        }
        buff[buff_index++] = line[i];
    }
    
    buff[buff_index] = '\0';

    strcpy(line, buff);
    free(buff);
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

    // Keep track of lines we care about
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
            
            // Keep track of line
            lines[line_count++] = strdup(line);
        }
    }

    fclose(input_file);
    
    // Combine all lines into one string
    char* text = malloc(sizeof(char) * (size_t)line_count * MAX_LINE_LENGTH * 2); // * 2 to be sure enough space for periods?
    
    if (!text)
    {
        return;
    }
    
    text[0] = '\0';
    
    for (int i = 0; i < line_count; i++)
    {
        strcat(text, lines[i]);
        strcat(text, " ");
        free(lines[i]);
    }
    
    trim(text);
    
    // Make sure first character is uppercase
    text[0] = (char)toupper((unsigned char)text[0]);
    
    // Make sure period is added at the very end
    int text_length = (int)strlen(text);
    if (text_length > 0 && text[text_length - 1] != '.') {
        text[text_length] = '.';
        text[text_length + 1] = '\0';
    }
    
    // Add periods before capitals
    add_periods_before_capitals(text);
    
    // Print to file
    FILE* output_file = fopen(file_name, "w");
    
    fprintf(output_file, "%s ", text);
    
    fclose(output_file);
    free(text);
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
