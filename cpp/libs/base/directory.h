#ifndef BASE_DIRECTORY_H_
#define BASE_DIRECTORY_H_

#include <string>

// Returns whether the given path is a directory.
bool IsDirectory(const std::string& path);

bool CreateDirectory(const std::string& full_path);

#endif  // BASE_DIRECTORY_H_
