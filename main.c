#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

#define MAX_FILE_PATH_LENGTH 256
#define INPUT_DIRECTORY "./input"

void handle_file(char* file)
{
    char filepath[MAX_FILE_PATH_LENGTH];
    snprintf(filepath, sizeof(filepath), "%s/%s", INPUT_DIRECTORY, file);
    
    format_captions_file(filepath);
    
    printf("  Processed '%s'\n", file);
}

int main()
{
    printf("---------- YT Captions Formatter v1.0 ----------\n");
    printf("This will remove all lines with timestamps and remove any blank lines from each file.\n");
    printf("\n");
    printf("Formatting all files in '%s' folder...\n", INPUT_DIRECTORY);
    
    traverse_directory(INPUT_DIRECTORY, handle_file);
    
    printf("\n");
    printf("Program ended normally\n");
    return EXIT_SUCCESS;
}
