#ifndef __NE_CORE_H_
#define __NE_CORE_H_

#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <sstream>

// set the default values

#ifndef pi
#define pi 3.14159265359
#endif

#define lLendingSize 12.0
#define cReactionRadius 75.0
#define minSpeciesBoxWidth 60.0
#define minSpeciesBoxHeight 36.0
#define maxSpeciesBoxWidth 120.0
#define maxSpeciesBoxHeight 120.0

// set the platfrom-specific features
#define _SBNEExport
#if defined( __WIN32__ ) || defined ( _WIN32 )
#   define OS_SEP MACRO_QUOTE(/)
#   if(!SBNE_CLIENT_BUILD)
#       define _SBNEExport  __declspec(dllexport)
#   else
#       define _SBNEExport  __declspec(dllimport)
#   endif
#elif defined ( __linux__ )
#   define OS_SEP MACRO_QUOTE(/)
#   if defined( SAGITTARIUS_GCC_VISIBILITY )
#       define _SBNEExport  __attribute__ ((visibility("default")))
#   endif
#endif

namespace sbne {

bool compareChar(const char& c1, const char& c2);

bool stringCompare(const std::string& str1, const std::string& str2);

}

#endif

