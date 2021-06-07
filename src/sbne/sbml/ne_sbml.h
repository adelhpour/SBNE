#ifndef __NE_SBML_H_
#define __NE_SBML_H_

#ifndef SWIG
#include "sbml/SBMLTypes.h"
#endif

#include "sbne/core/ne_core.h"

namespace sbne {
/// @brief Reads an SBML document from the given file.
/// @param filename the name or full pathname of the file to be read.
/// @return a pointer to the SBMLDocument object created from the SBML content in @p filename
_SBNEExport SBMLDocument* ne_doc_readSBML(const std::string& filename);

/// @brief Writes the given SBML document to filename.
/// @param doc the SBML document to be written.
/// @param filename the name or full pathname of the file where the SBML is to be written.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_doc_writeSBML(SBMLDocument* doc, const std::string& filename);
}

#endif
