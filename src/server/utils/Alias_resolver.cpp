#include "Alias_resolver.h"
#include <unordered_map>


std::string resolveAlias(const std::string& action){
    static const std::unordered_map<std::string, std::string> aliases = {
    {"ta", "add"},
	{"tl", "list"},
	{"ts", "show"},
	{"th", "help"},
	{"tm", "man"}



   };

   auto it = aliases.find(action);
   return it != aliases.end() ? it->second : action; 

}

