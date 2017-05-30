#ifndef PATH_H_
#define PATH_H_

#include <string>

std::string MakeAbsolute(const char *path);

bool PathExists(const std::string& path);

bool GetRealPath(const std::string& path, std::string* out);

bool IsAbsolute(const std::string& path);

std::string GetDirectoryName(const std::string& path);

std::string JoinPath(const std::string& path1, const std::string& path2);

#endif  // PATH_H_
