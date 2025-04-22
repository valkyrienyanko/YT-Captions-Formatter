#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

#define MAX_FILE_PATH_LENGTH 256
#define INPUT_DIRECTORY "./input"

void home_page();
void options();
void format_all_files();

int main()
{
    home_page();
    
    printf("\nProgram ended normally\n");
    return EXIT_SUCCESS;
}

void home_page()
{
    printf("---------- YouTube Captions Formatter ----------\n");
    printf("(1) Format Files\n");
    printf("(2) Configure Options\n");
    printf("(3) Exit Program\n");

    int choice = 0;
    
    while (true)
    {
        scanf("%d", &choice);
        
        switch (choice)
        {
            case 1:
                format_all_files();
                return;
            case 2:
                options();
                return;
            case 3:
                return;
            default:
                printf("Enter a valid choice.\n");
        }
    }
}

void options()
{
    printf("---------- Options ----------\n");
    printf("No options implemented yet sorry!\n");
    printf("(1) Go Back\n");
    
    int choice = 0;
    
    while (true)
    {
        scanf("%d", &choice);
        
        switch (choice)
        {
            case 1:
                home_page();
                return;
            default:
                printf("Enter a valid choice.\n");
        }
    }
}

void handle_file(const char* file)
{
    char filepath[MAX_FILE_PATH_LENGTH];
    snprintf(filepath, sizeof(filepath), "%s/%s", INPUT_DIRECTORY, file);
    
    format_captions_file(filepath);
    
    printf("  Processed '%s'\n", file);
}

void format_all_files()
{
    printf("Formatting all files in '%s' folder...\n", INPUT_DIRECTORY);
    
    traverse_directory(INPUT_DIRECTORY, handle_file);
    
    // Prevent terminal window from immediately closing after processing files
    // User should be able to see what files were processed along with any other feedback
    printf("\nPress Enter to continue...");
    getchar(); // Consume the newline left by previous input
    getchar(); // Wait for the user to press Enter
}
