#pragma once

#include <stdbool.h>

void format_captions_file(const char* file_name);
void traverse_directory(const char* dir_path, void (*file)(const char*));
bool contains_timestamp(const char* line, int line_length);