#ifndef READ_FILE_H_
#define READ_FILE_H_

#include <string>

// Code from Bazel's https://github.com/google/bazel/blob/master/src/main/cpp/blaze_util.cc function.
bool ReadFile(const std::string& filename, std::string* content);

#endif  // READ_FILE_H_
