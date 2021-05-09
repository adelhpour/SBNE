#include "sbne/sbml/ne_sbml.h"
#include "sbne/sbml/ne_layout.h"
#include "sbne/sbml/ne_render.h"

using namespace sbne;

int main () {
    // set inputfile value to the absolute path of the input SBML (xml) file.
    const std::string inputfile = "Absolute path to the input sbml (.xml) file";
    
    // set outputfile value to the absolute path of the output SBML (xml) file.
    const std::string outputfile = "Absolute path to the output sbml (.xml) file";

    // read the input file and get its SBML document
    SBMLDocument* document = ne_doc_readSBML(inputfile.c_str());

    //get layout information (if exists) from the SBML document
    ne_layoutInfo* li = ne_doc_processLayoutInfo(document);

    // get render information (if exists) from the SBML document
    ne_renderInfo* ri = ne_doc_processRenderInfo(document);

    // if layout information does not exists
    if (!ne_net_isLayoutSpecified(ne_li_getNetwork(li))) {
        // apply autolayout algorithm
        ne_li_addLayoutFeaturesToNetowrk(li);
    }

    // if render information does not exists
    if (!ne_ven_isRenderSpecified(ne_ri_getVeneer(ri))) {
        // apply autorender algorithm
        ne_ri_addDefaultRenderFeaturesToVeneer(ri);
    }

    // add newly-added or already-included layout information to the SBML document
    document = ne_doc_populateSBMLdocWithLayoutInfo(document, li);
    
    // add newly-added or already-included render information to the SBML document
    document = ne_doc_populateSBMLdocWithRenderInfo(document, ri);

    // write the SBML document to the output file
    ne_doc_writeSBML(document, outputfile.c_str());
};
