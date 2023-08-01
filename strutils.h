#ifndef _STRUTILS_
#define _STRUTILS_

#include <stddef.h>
#include <ctype.h>

inline bool strdiffer(const char* str1, const char* str2) {for(size_t i=0; str1[i]==str2[i]; ++i) {if(!str1[i]) return false;} return true;}
inline bool strndiffer(const char* str1, const char* str2, const size_t& n) {size_t i; for(i=0; i<n && str1[i]==str2[i]; ++i) {if(!str1[i]) return false;} if(i==n) return false; return true;}

inline void toLower(char* s) {
  for(char *p=s; *p; p++) *p=tolower(*p);
}

inline void toUpper(char* s) {
  for(char *p=s; *p; p++) *p=toupper(*p);
}

#endif
