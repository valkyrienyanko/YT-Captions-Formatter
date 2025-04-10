#pragma once

#include <stdbool.h>

void format_captions_file(char* file_name);
void traverse_directory(char* dir_path, void (*file)(char*));
bool contains_timestamp(char* line, int line_length);