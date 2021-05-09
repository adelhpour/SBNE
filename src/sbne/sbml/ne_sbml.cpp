#include "sbne/sbml/ne_sbml.h"

namespace sbne {

// -- Input --

SBMLDocument* ne_doc_readSBML(const char* fileName) {
    SBMLDocument* document = readSBMLFromFile(fileName);
    
    if (document) {
        if (!document->getNumErrors())
            return document;
        else {
            std::cerr << "Failed to parse SBML correctly because of : \n";
            for (int i = 0; i < document->getNumErrors(); ++i)
                std::cerr << i + 1 << ": " << document->getError(i)->getMessage() << std::endl;
        }
    }
        
    return document;
}

// -- Output --

int ne_doc_writeSBML(SBMLDocument* doc, const char* fileName) {
    SBMLWriter writer;
    if (writer.writeSBML(doc, fileName))
        return 0;
    
    std::cerr << "Failed to save SBML\n";
    return -1;
}

}
