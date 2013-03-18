
#include <cstdlib>
#include <map>
#include <unordered_map>
#include <list>
#include <set>
#include <string>
#include <utility>
#include <istream>
#include <algorithm>

typedef std::map<std::string, std::string> stringmap_t;
typedef std::list<std::string> stringlist_t;
typedef std::vector<char *> cstringlist_t;

typedef void (*file_cb)(const char *file);

void split(const std::string &str, char delimiter, stringlist_t &elems);
std::pair<std::string, std::string> split(const std::string &str, const std::string &delimiter);
std::string &replace(std::string &str, const std::string &substr, const std::string &substitute);
bool endswith(const std::string &str, const std::string &suffix);
bool startswith(const std::string &str, const std::string &prefix);

bool is_directory(const char *path);
std::string get_variable(const std::string &var);

void find_files(const char *path, const char *name_suffix, file_cb cb);

void free_cstringlist(cstringlist_t &stringlist);
