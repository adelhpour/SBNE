#ifndef __NE_CORE_H_
#define __NE_CORE_H_

#include <string>
#include <vector>
#include <unordered_map>
#include <cmath>
#include <climits>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <regex>

#ifndef SWIG
#include "sbml/SBMLTypes.h"
#endif

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

std::string getKeyValue(std::unordered_map<std::string, std::string> info, const std::string& key);

bool isNumber(const std::string& str);

bool isBoolean(const std::string& str);

bool stringToBoolean(const std::string& str);

/// @brief Reads an SBML document from the given file.
/// @param filename the name or full pathname of the file to be read.
/// @return a pointer to the SBMLDocument object created from the SBML content in @p filename
_SBNEExport SBMLDocument* ne_doc_readSBML(const std::string& filename);

/// @brief Writes the given SBML document to filename.
/// @param doc the SBML document to be written.
/// @param filename the name or full pathname of the file where the SBML is to be written.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_doc_writeSBML(SBMLDocument* doc, const std::string& filename);

/// @brief Get the value of a desired feature of an SBML document.
/// @param infoList a list containg the info about the  SBML document and the desired feature.
/// @return a string containing the value of the desired feature of the SBML document,  or an empty string if either the value of the desired feature is not set or the desired feature does not exist.
_SBNEExport const std::string ne_get(std::unordered_map<std::string, std::string> infoList);

/// @brief Set the value of a desired feature of an SBML document.
/// @param infoList a list containg the info about the  SBML document, the desired feature, and its value.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_set(std::unordered_map<std::string, std::string> infoList);

}

#endif

