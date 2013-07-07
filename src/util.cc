#include <sstream>
#include <istream>

#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>


#include "util.hh"

// STL sucks so much in this regard, even very basic stuff like "split some string"
// or "replace this with that" isn't implemented.

void split(const std::string &str, char delimiter, stringlist_t &elems)
{
    std::stringstream ss(str);
    std::string item;

    while (std::getline(ss, item, delimiter))
        elems.push_back(item);
}

std::pair<std::string, std::string> split(const std::string &str, const std::string &delimiter)
{
    size_t pos = 0;
    pos = str.find(delimiter);
    return std::make_pair(str.substr(0, pos), str.substr(pos+1, str.length()));
}


std::string &replace(std::string &str, const std::string &substr, const std::string &substitute)
{
    if(substr.empty())
        return str;
    size_t start_pos = 0;
    while((start_pos = str.find(substr, start_pos)) != std::string::npos) {
        str.replace(start_pos, substr.length(), substitute);
        start_pos += substitute.length(); // In case 'substitute' contains 'substr', like replacing 'x' with 'yx'
    }
    return str;
}

bool endswith(const std::string &str, const std::string &suffix)
{
    if(str.length() < suffix.length())
        return false;
    return str.compare(str.length() - suffix.length(), suffix.length(), suffix) == 0;
}

bool startswith(const std::string &str, const std::string &prefix)
{
    if(str.length() < prefix.length())
        return false;
    return str.compare(0, prefix.length(), prefix) == 0;
}

bool endswith(const char *str, const char *suffix)
{
    size_t lenstr = strlen(str);
    size_t lensuffix = strlen(suffix);
    if (lensuffix > lenstr)
        return false;
    return strncmp(str + lenstr - lensuffix, suffix, lensuffix) == 0;
}

bool is_directory(const char *path)
{
    int status;
    struct stat filestat;

    status = stat(path, &filestat);
    if(status)
        return false;

    return S_ISDIR(filestat.st_mode);
}

std::string get_variable(const std::string &var)
{
    const char *env = std::getenv(var.c_str());
    if(env) {
        return env;
    } else
        return "";
}

void find_files(const char *path, const char *name_suffix, file_cb cb)
{
    DIR *dir;
    dirent *entry;

    dir = opendir(path);
    if(!dir)
        return;

    char *pathspec = new char[512]; // Easily long enough
    strcpy(pathspec, path);
    int len = strlen(path);
    pathspec[len++] = '/';
    pathspec[len] = 0;

    while((entry = readdir(dir))) {
        if(entry->d_name[0] == '.' || strcmp(entry->d_name, "..") == 0)
            continue;

        strcpy(pathspec+len, entry->d_name);
        if(is_directory(pathspec)) {
            find_files(pathspec, name_suffix, cb);
        } else if(endswith(pathspec, name_suffix)) {
            cb(pathspec);
        }
    }

    delete[] pathspec;

    closedir(dir);
}

void free_cstringlist(cstringlist_t &stringlist)
{
    for(auto item : stringlist)
        free(item);
}
