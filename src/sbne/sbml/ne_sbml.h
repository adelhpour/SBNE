#ifndef __NE_SBML_H_
#define __NE_SBML_H_

#ifndef SWIG
#include "sbml/SBMLTypes.h"
#endif

#include "sbne/core/ne_core.h"

namespace sbne {

_SBNEExport SBMLDocument* ne_doc_readSBML(const char* fileName);

_SBNEExport int ne_doc_writeSBML(SBMLDocument* doc, const char* fileName);

}

#endif
