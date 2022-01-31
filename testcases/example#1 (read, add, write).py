import libsbne as sbne

# set inputfile value to the absolute path of the input SBML (.xml) file.
inputfile = "Absolute path to the input sbml (.xml) file"

# set outputfile value to the absolute path of the output SBML (.xml) file.
outputfile = "Absolute path to the output sbml (.xml) file"

# read the inputfile and get its SBML document
document = sbne.ne_doc_readSBML(inputfile)

# get layout information (if exists) from the SBML document
layoutinfo = sbne.ne_doc_processLayoutInfo(document)

# get render information (if exists) from the SBML document
renderinfo = sbne.ne_doc_processRenderInfo(document)

# if layout information does not exist
if not sbne.ne_net_isLayoutSpecified(sbne.ne_li_getNetwork(layoutinfo)):
    # apply autolayout algorithm
    sbne.ne_li_addLayoutFeaturesToNetowrk(layoutinfo)

# if render information does not exist
if not sbne.ne_ven_isRenderSpecified(sbne.ne_ri_getVeneer(renderinfo))):
    # apply autorender algorithm
    sbne.ne_ri_addDefaultRenderFeaturesToVeneer(renderinfo)

# add newly-added or already-included layout information to the SBML document
document = sbne.ne_doc_populateSBMLdocWithLayoutInfo(document, layoutinfo)

# add newly-added or already-included render information to the SBML document
document = sbne.ne_doc_populateSBMLdocWithRenderInfo(document, renderinfo)

# write the SBML document to the outputfile
sbne.ne_doc_writeSBML(document, outputfile)
