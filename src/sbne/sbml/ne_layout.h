#ifndef __NE_LAYOUT_H_
#define __NE_LAYOUT_H_

#include "sbne/network/ne_network.h"
#include "sbne/layout/ne_autoLayout.h"

namespace sbne {

// layout info
typedef struct {
    // network
    void* net;

    // sbml level
    unsigned int level;
    
    // sbml version
    unsigned int version;
} ne_layoutInfo;

/// -- Object API --

// Network Element

_SBNEExport bool ne_ne_isSetId(NetworkElement* nE);

_SBNEExport const std::string ne_ne_getId(NetworkElement* nE);

_SBNEExport bool ne_ne_isSetName(NetworkElement* nE);

_SBNEExport const std::string ne_ne_getName(NetworkElement* nE);

// Network

_SBNEExport void ne_li_setNetwork(ne_layoutInfo* l, Network* net);

_SBNEExport Network* ne_li_getNetwork(ne_layoutInfo* l);

_SBNEExport Network* ne_li_addLayoutFeaturesToNetowrk(ne_layoutInfo* l);

_SBNEExport bool ne_net_isLayoutSpecified(Network* net);

_SBNEExport const size_t ne_net_getNumCompartments(Network* net);

_SBNEExport NCompartment* ne_net_getCompartment(Network* net, const unsigned int& index);

_SBNEExport compartmentVec ne_net_getCompartments(Network* net);

_SBNEExport const size_t ne_net_getNumSpecies(Network* net);

_SBNEExport NSpecies* ne_net_getSpecies(Network* net, const unsigned int& index);

_SBNEExport speciesVec ne_net_getSpecies(Network* net);

_SBNEExport const size_t ne_net_getNumReactions(Network* net);

_SBNEExport NReaction* ne_net_getReaction(Network* net, const unsigned int& index);

_SBNEExport reactionVec ne_net_getReactions(Network* net);

NText* ne_net_getText(Network* net, NGraphicalObject* gO);

// Graphcial Object

_SBNEExport bool ne_go_isSetGlyphId(NGraphicalObject* gO);

_SBNEExport const std::string ne_go_getGlyphId(NGraphicalObject* gO);

_SBNEExport int ne_go_setGlyphId(Network* net, NGraphicalObject* gO, const std::string& gOId);

_SBNEExport bool ne_go_isSetText(NGraphicalObject* gO);

_SBNEExport NText* ne_go_getText(NGraphicalObject* gO);

_SBNEExport int ne_go_setText(NGraphicalObject* gO, NText* t);

_SBNEExport bool ne_go_isSetBoundingBox(NGraphicalObject* gO);

_SBNEExport LBox* ne_go_getBoundingBox(NGraphicalObject* gO);

_SBNEExport int ne_go_setBoundingBox(NGraphicalObject* gO, LBox* b);

_SBNEExport int ne_go_unSetBoundingBox(NGraphicalObject* gO, const bool& removeBoundingBox);

_SBNEExport const GrphObjType ne_go_getType(NGraphicalObject* gO);

// Species

_SBNEExport int ne_spc_updateBoundingBox(NSpecies* s, const double& positionX, const double& positionY, const double& dimensionWidth, const double& dimensionHeight);

_SBNEExport bool ne_spc_isSetCompartment(NSpecies* s);

_SBNEExport const std::string ne_spc_getCompartment(NSpecies* s);

// Reaction

_SBNEExport const size_t ne_rxn_getNumSpeciesReferences(NReaction* r);

_SBNEExport NSpeciesReference* ne_rxn_getSpeciesReference(NReaction* r, const unsigned int& index);

_SBNEExport SReferenceVec ne_net_getSpeciesReferences(NReaction* r);

_SBNEExport bool ne_rxn_isSetCurve(NReaction* r);

_SBNEExport LCurve* ne_rxn_getCurve(NReaction* r);

_SBNEExport int ne_rxn_setCurve(NReaction* r, LCurve* c);

_SBNEExport int ne_rxn_unSetCurve(NReaction* r, const bool& removeCurve);

_SBNEExport bool ne_rxn_isSetExtentBox(NReaction* r);

_SBNEExport LBox* ne_rxn_getExtentBox(NReaction* r);

// Species References

_SBNEExport bool ne_sr_isSetReaction(NSpeciesReference* sR);

_SBNEExport NReaction* ne_sr_getReaction(NSpeciesReference* sR);

_SBNEExport bool ne_sr_isSetSpecies(NSpeciesReference* sR);

_SBNEExport NSpecies* ne_sr_getSpecies(NSpeciesReference* sR);

_SBNEExport bool ne_sr_isSetRole(NSpeciesReference* sR);

_SBNEExport const std::string ne_sr_getRoleAsString(NSpeciesReference* sR);

_SBNEExport bool ne_sr_isSetCurve(NSpeciesReference* sR);

_SBNEExport LCurve* ne_sr_getCurve(NSpeciesReference* sR);

// Text

_SBNEExport bool ne_gtxt_isSetPlainText(NText* t);

_SBNEExport const std::string ne_gtxt_getPlainText(NText* t);

_SBNEExport int ne_gtxt_setPlainText(NText* t, const std::string& plainText);

_SBNEExport int ne_gtxt_unSetPlainText(NText* t);

// Curve

_SBNEExport const size_t ne_crv_getNumElements(LCurve* c);

_SBNEExport LLineSegment* ne_crv_getElement(LCurve* c, const unsigned int& index);

_SBNEExport LCurve::listOfElements ne_crv_getElements(LCurve* c);

_SBNEExport LLineSegment* ne_crv_addElement(LCurve* c, const  int& index = -1, LPoint* startPoint = NULL, LPoint* endPoint = NULL, LPoint* basePoint1 = NULL,  LPoint* basePoint2 = NULL);

_SBNEExport int ne_crv_removeElement(LCurve* c, const  int& index);

_SBNEExport LBox* ne_crv_getExtentBox(LCurve* c);

// Line Segment

_SBNEExport bool ne_ls_isSetStart(LLineSegment* l);

_SBNEExport LPoint* ne_ls_getStart(LLineSegment* l);

_SBNEExport int ne_ls_setStart(LLineSegment* line, LPoint* p);

_SBNEExport bool ne_ls_isSetEnd(LLineSegment* l);

_SBNEExport LPoint* ne_ls_getEnd(LLineSegment* l);

_SBNEExport int ne_ls_setEnd(LLineSegment* line, LPoint* p);

_SBNEExport bool ne_ls_isCubicBezier(LLineSegment* l);

// Cubic Bezier

_SBNEExport bool ne_cb_isSetBasePoint1(LLineSegment* l);

_SBNEExport LPoint* ne_cb_getBasePoint1(LLineSegment* l);

_SBNEExport int ne_cb_setBasePoint1(LLineSegment* line, LPoint* p);

_SBNEExport bool ne_cb_isSetBasePoint2(LLineSegment* l);

_SBNEExport LPoint* ne_cb_getBasePoint2(LLineSegment* l);

_SBNEExport int ne_cb_setBasePoint2(LLineSegment* line, LPoint* p);

// Bounding Box

_SBNEExport const double ne_bb_getX(LBox* b);

_SBNEExport int ne_bb_setX(LBox* b, const double& x);

_SBNEExport const double ne_bb_getY(LBox* b);

_SBNEExport int ne_bb_setY(LBox* b, const double& y);

_SBNEExport const double ne_bb_getWidth(LBox* b);

_SBNEExport int ne_bb_setWidth(LBox* b, const double& width);

_SBNEExport const double ne_bb_getHeight(LBox* b);

_SBNEExport int ne_bb_setHeight(LBox* b, const double& height);

// Point

_SBNEExport const double ne_point_getX(LPoint* p);

_SBNEExport int ne_point_setX(LPoint* p, const double& x);

_SBNEExport const double ne_point_getY(LPoint* p);

_SBNEExport int ne_point_setY(LPoint* p, const double& y);


// -- Input --

_SBNEExport ne_layoutInfo* ne_doc_processLayoutInfo(SBMLDocument* doc);

// -- Output --

_SBNEExport SBMLDocument* ne_doc_populateSBMLdocWithLayoutInfo(SBMLDocument* doc, ne_layoutInfo* l);

_SBNEExport void populateLineInfo(LLineSegment* lLineSegment, LineSegment* lineSegment);

}

#endif
