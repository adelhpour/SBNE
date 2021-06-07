#ifndef __NE_LAYOUT_H_
#define __NE_LAYOUT_H_

#include "sbne/network/ne_network.h"
#include "sbne/layout/ne_autoLayout.h"

namespace sbne {

/// @struct LayoutInfo
/// @brief Contains the layout information about the SBMLDocument.
typedef struct {
    /// Used to store the Network object
    void* net;

    /// Used to store the @c level of the SBMLDocument.
    unsigned int level;
    
    /// Used to store the @c version of the SBMLDocument.
    unsigned int version;
} LayoutInfo;

void populateLineInfo(LLineSegment* LLineSegment, LineSegment* lineSegment);

// SBMLDocument

/// @brief Process the layout info from an SBML document.
/// @param doc a pointer to the SBMLDocument object.
/// @return a pointer to the LayoutInfo object created from this SBML document content.
_SBNEExport LayoutInfo* ne_doc_processLayoutInfo(SBMLDocument* doc);

/// @brief Populate an SBML document with the layout info.
/// @param doc a pointer to the SBMLDocument object.
/// @param l a pointer to the LayoutInfo object.
/// @return a pointer to the SBMLDocument object populated with the layout info content.
_SBNEExport SBMLDocument* ne_doc_populateSBMLdocWithLayoutInfo(SBMLDocument* doc, LayoutInfo* l);

// LayoutInfo

/// @brief Gets the Network object of this LayoutInfo object.
/// @param l a pointer to the LayoutInfo object.
/// @return a pointer to the Network object of this LayoutInfo object.
_SBNEExport Network* ne_li_getNetwork(LayoutInfo* l);

/// @brief Sets the Network object of this LayoutInfo object.
/// @param l a pointer to the LayoutInfo object.
/// @param net a pointer to the Network object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_li_setNetwork(LayoutInfo* l, Network* net);

/// @brief Automattically generates all the required layout information for each NGraphicalObject object of the LayoutInfo
/// object's Network object.
/// @param l a pointer to the LayoutInfo object.
/// @return a pointer to the Network object of this LayoutInfo object.
_SBNEExport Network* ne_li_addLayoutFeaturesToNetowrk(LayoutInfo* l);

// NetworkElement

/// @brief Predicate returning @c true if the "id" attribute of this NetworkElement is set.
/// @param nE a pointer to the NetworkElement object.
/// @return @c true if the "id" attribute of this NetworkElement object is set, @c false if either the "id" attribute is not set or the object is @c NULL .
_SBNEExport bool ne_ne_isSetId(NetworkElement* nE);

/// @brief Returns the value of the "id" attribute of this NetworkElement object, if it has one
/// @param nE a pointer to the NetworkElement object.
/// @return the "id" attribute of this NetworkElement object, or an empty string if the "id" attribute is not set or the object is @c NULL .
_SBNEExport const std::string ne_ne_getId(NetworkElement* nE);

/// @brief Predicate returning @c true if the "name" attribute of this NetworkElement is set.
/// @param nE a pointer to the NetworkElement object.
/// @return @c true if the "name" attribute of this NetworkElement object is set, @c false if either the "name" attribute is not set or the object is @c NULL .
_SBNEExport bool ne_ne_isSetName(NetworkElement* nE);

/// @brief Returns the value of the "name" attribute of this NetworkElement object, if it has one
/// @param nE a pointer to the NetworkElement object.
/// @return the "name" attribute of this NetworkElement object, or an empty string if the "name" attribute is not set or the object is @c NULL .
_SBNEExport const std::string ne_ne_getName(NetworkElement* nE);

// Network

/// @brief Predicate returning @c true if this Network object's "layout" is set
/// @param net a pointer to the Network object.
/// @return @c true if the "layout" of this Network object is set, @c false if either the "layout" is not set or the object is @c NULL .
_SBNEExport bool ne_net_isLayoutSpecified(Network* net);

/// @brief Gets the number of NCompartment objects in this Network.
/// @param net a pointer to the Network object.
/// @return the number of NCompartments of this Network object.
_SBNEExport const size_t ne_net_getNumCompartments(Network* net);
    
/// @brief Gets the indexth NCompartment object of this Network object.
/// @param net a pointer to the Network object.
/// @param index the index of the NCompartment object to return.
/// @return a pointer to the indexth NCompartment object of this Network object.
_SBNEExport NCompartment* ne_net_getCompartment(Network* net, const unsigned int& index);

/// @brief Gets all the NCompartment objects of this Network object.
/// @param net a pointer to the Network object.
/// @return a vector of pointers to NCompartment objects of this Network object.
_SBNEExport compartmentVec ne_net_getCompartments(Network* net);

/// @brief Gets the number of NSpecies objects in this Network object.
/// @param net a pointer to the Network object.
/// @return the number of %NSpecies of this Network object.
_SBNEExport const size_t ne_net_getNumSpecies(Network* net);

/// @brief Gets the indexth NSpecies object of this Network object.
/// @param net a pointer to the Network object.
/// @param index the index of the NSpecies object to return.
/// @return a pointer to the indexth NSpecies object of this Network object.
_SBNEExport NSpecies* ne_net_getSpecies(Network* net, const unsigned int& index);

/// @brief Gets all the NSpecies objects of this Network object.
/// @param net a pointer to the Network object.
/// @return a vector pointers to NSpecies objects of this Network object.
_SBNEExport speciesVec ne_net_getSpecies(Network* net);

/// @brief Gets the number of NReaction objects in this Network object.
/// @param net a pointer to the Network object.
/// @return the number of NReactions of this Network object.
_SBNEExport const size_t ne_net_getNumReactions(Network* net);

/// @brief Gets the indexth NReaction object of this Network object.
/// @param net a pointer to the Network object.
/// @param index the index of the NReaction object to return.
/// @return a pointer to the indexth NReaction object of this Network object.
_SBNEExport NReaction* ne_net_getReaction(Network* net, const unsigned int& index);

/// @brief Gets all the NReaction objects of this Network object.
/// @param net a pointer to the Network object.
/// @return a vector of pointers to NReaction objects of this Network object.
_SBNEExport reactionVec ne_net_getReactions(Network* net);

/// @brief Gets the NText object assigned to this NGraphicalObject object, if its NText ojbect is @c NULL ,  first creates an
/// NText object and assign it to this NGraphicalObject object.
/// @param net a pointer to the Network object.
/// @param gO a pointer to the NGraphicalObject object.
/// @return a pointer to the NText object of this NGraphicalObject object.
_SBNEExport NText* ne_net_getText(Network* net, NGraphicalObject* gO);

// NGraphcialObject

/// @brief Predicate returning @c true if the "glyphid" attribute of this NetworkElement is set.
/// @param gO a pointer to the NGraphicalObject object.
/// @return @c true if the "glyphid" attribute of this NGraphicalObject object is set, @c false if either the "glyphid" attribute is not set or the object is @c NULL .
_SBNEExport bool ne_go_isSetGlyphId(NGraphicalObject* gO);

/// @brief Returns the value of the "glyphid" attribute of this NGraphicalObject object, if it has one
/// @param gO a pointer to the NGraphicalObject object.
/// @return the "glyphid" attribute of this NGraphicalObject object, or an empty string if either the "glyphid" attribute is not set or the object is @c NULL .
_SBNEExport const std::string ne_go_getGlyphId(NGraphicalObject* gO);

/// @brief Sets the value of the "glyphid" attribute of this NGraphicalObject object.
/// @param net a pointer to the Network object.
/// @param gO a pointer to the NGraphicalObject object.
/// @param gOId a string to use as the value of the "glyphid" attribute of this NGraphicalObject object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_go_setGlyphId(Network* net, NGraphicalObject* gO, const std::string& gOId);

/// @brief Predicate returning @c true if this NGraphicalObject object's NText object is set.
/// @param gO a pointer to the NGraphicalObject object.
/// @return @c true if the NText object of this NGraphicalObject object is set, @c false if either the NText object is not set or the object is @c NULL .
_SBNEExport bool ne_go_isSetText(NGraphicalObject* gO);

/// @brief Gets the NText object of this NGraphicalObject object.
/// @param gO a pointer to the NGraphicalObject object.
/// @return a pointer to the NText object of this NGraphicalObject object.
_SBNEExport NText* ne_go_getText(NGraphicalObject* gO);

/// @brief Sets the NText object of this NGraphicalObject object.
/// @param gO a pointer to the NGraphicalObject object.
/// @param t a pointer to the NText object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_go_setText(NGraphicalObject* gO, NText* t);

/// @brief Predicate returning @c true if this NGraphicalObject object's LBox object is set.
/// @param gO a pointer to the NGraphicalObject object.
/// @return @c true if the LBox object of this NGraphicalObject object is set, @c false if either the LBox object is not set or the object is @c NULL .
_SBNEExport bool ne_go_isSetBoundingBox(NGraphicalObject* gO);

/// @brief Gets the LBox object of this NGraphicalObject object.
/// @param gO a pointer to the NGraphicalObject object.
/// @return a pointer to the LBox object of this NGraphicalObject object.
_SBNEExport LBox* ne_go_getBoundingBox(NGraphicalObject* gO);

/// @brief Sets the LBox object of this NGraphicalObject object.
/// @param gO a pointer to the NGraphicalObject object.
/// @param b a pointer to the LBox object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_go_setBoundingBox(NGraphicalObject* gO, LBox* b);

/// @brief Unset the LBox object of this NGraphicalObject object.
/// @param gO a pointer to the NGraphicalObject object.
/// @param remove a bool value to determine whether to keep or remove the LBox object assigned to this
/// NGraphicalObject object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_go_unSetBoundingBox(NGraphicalObject* gO, const bool& remove);

/// @brief Gets the "type" attribute of this NGraphicalObject object.
/// @param gO a pointer to the NGraphicalObject object.
/// @return the "type" attribute of this NGraphicalObject object as a GrphObjType.
_SBNEExport const GrphObjType ne_go_getType(NGraphicalObject* gO);

// NSpecies

_SBNEExport int ne_spc_updateBoundingBox(NSpecies* s, const double& positionX, const double& positionY, const double& dimensionWidth, const double& dimensionHeight);

/// @brief Predicate returning @c true if the "compartment" attribute of this NSpecies is set.
/// @param s a pointer to the NSpecies object.
/// @return @c true if the "compartment" attribute of this NSpecies object is set, @c false if either the "compartment" attribute is not set or the object is @c NULL .
_SBNEExport bool ne_spc_isSetCompartment(NSpecies* s);

/// @brief Returns the value of the "compartment" attribute of this NSpecies object, if it has one
/// @param s a pointer to the NSpecies object.
/// @return the "compartment" attribute of this NSpecies object, or an empty string if the "compartment" attribute is not set or the object is @c NULL .
_SBNEExport const std::string ne_spc_getCompartment(NSpecies* s);

// NReaction

/// @brief Gets the number of NSpeciesReference objects in this NReaction object.
/// @param r a pointer to the NReaction object.
/// @return the number of NSpeciesReferences of this NReaction object.
_SBNEExport const size_t ne_rxn_getNumSpeciesReferences(NReaction* r);

/// @brief Gets the indexth NSpeciesReference object of this NReaction object.
/// @param r a pointer to the NReaction object.
/// @param index the index of the NSpeciesReference object to return.
/// @return a pointer to the indexth NSpeciesReference object of this NReaction object.
_SBNEExport NSpeciesReference* ne_rxn_getSpeciesReference(NReaction* r, const unsigned int& index);

/// @brief Gets all the NSpeciesReference objects of this NReaction object.
/// @param r a pointer to the NReaction object.
/// @return a vector of pointers to NSpeciesReference objects of this NReaction object.
_SBNEExport SReferenceVec ne_rxn_getSpeciesReferences(NReaction* r);

/// @brief Predicate returning @c true if this NReaction object's LCurve object is set.
/// @param r a pointer to the NReaction object.
/// @return @c true if the LCurve object of this NReaction object is set, @c false if either the LCurve object is not set or the object is @c NULL .
_SBNEExport bool ne_rxn_isSetCurve(NReaction* r);

/// @brief Gets the LCurve object of this NReaction object.
/// @param r a pointer to the NReaction object.
/// @return a pointer to the LCurve object of this NReaction object.
_SBNEExport LCurve* ne_rxn_getCurve(NReaction* r);

/// @brief Sets the LCurve object of this NReaction object.
/// @param r a pointer to the NReaction object.
/// @param c a pointer to the LCurve object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_rxn_setCurve(NReaction* r, LCurve* c);

/// @brief Unset the LCurve object of this NReaction object.
/// @param r a pointer to the NReaction object.
/// @param remove a bool value to determine whether to keep or remove the LCurve object assigned to this NReaction object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_rxn_unSetCurve(NReaction* r, const bool& remove);

/// @brief Predicate returning @c true if this NReaction object's "extentbox" attribute is set.
/// @param r a pointer to the NReaction object.
/// @return @c true if the "extentbox" attribute of this NReaction object is set, @c false if either the "extentbox" attribute is not set or the object is @c NULL .
_SBNEExport bool ne_rxn_isSetExtentBox(NReaction* r);

/// @brief Gets the "extentbox" attribute of this NReaction object.
/// @param r a pointer to the NReaction object.
/// @return a pointer to an LBox object with the same values as "extenbox" attribute of this NReaction object.
_SBNEExport LBox* ne_rxn_getExtentBox(NReaction* r);

// NSpeciesReference

/// @brief Predicate returning @c true if this NSpeciesReference object's NReaction object is set.
/// @param sR a pointer to the NSpeciesReference object.
/// @return @c true if the NReaction object of this NSpeciesReference object is set, @c false if either the NReaction object is not set or the object is @c NULL .
_SBNEExport bool ne_sr_isSetReaction(NSpeciesReference* sR);

/// @brief Gets the NReaction object of this NSpeciesReference object.
/// @param sR a pointer to the NSpeciesReference object.
/// @return a pointer to the NReaction object of this NSpeciesReference object.
_SBNEExport NReaction* ne_sr_getReaction(NSpeciesReference* sR);

/// @brief Predicate returning @c true if this NSpeciesReference object's NSpecies object is set.
/// @param sR a pointer to the NSpeciesReference object.
/// @return @c true if the NSpecies object of this NSpeciesReference object is set, @c false if either the NSpecies object is not set or the object is @c NULL .
_SBNEExport bool ne_sr_isSetSpecies(NSpeciesReference* sR);

/// @brief Gets the NSpecies object of this NSpeciesReference object.
/// @param sR a pointer to the NSpeciesReference object.
/// @return a pointer to the NSpecies object of this NSpeciesReference object.
_SBNEExport NSpecies* ne_sr_getSpecies(NSpeciesReference* sR);

/// @brief Predicate returning @c true if the "role" attribute of this NSpeciesReference is set.
/// @param sR a pointer to the NSpeciesReference object.
/// @return @c true if the "role" attribute of this NSpeciesReference object is set, @c false if either the "role" attribute is not set or the object is @c NULL .
_SBNEExport bool ne_sr_isSetRole(NSpeciesReference* sR);

/// @brief Returns the value of the "role" attribute of this NSpeciesReference object as a string, if it has one
/// @param sR a pointer to the NSpeciesReference object.
/// @return the "role" attribute of this NSpeciesReference object as a string, or an empty string if either the "role" attribute is not set or the object is @c NULL .
_SBNEExport const std::string ne_sr_getRoleAsString(NSpeciesReference* sR);

/// @brief Predicate returning @c true if this NSpeciesReference object's LCurve object is set.
/// @param sR a pointer to the NSpeciesReference object.
/// @return @c true if the LCurve object of this NSpeciesReference object is set, @c false if either the LCurve object is not set or the object is @c NULL .
_SBNEExport bool ne_sr_isSetCurve(NSpeciesReference* sR);

/// @brief Gets the LCurve object of this NSpeciesReference object.
/// @param sR a pointer to the NSpeciesReference object.
/// @return a pointer to the LCurve object of this NSpeciesReference object.
_SBNEExport LCurve* ne_sr_getCurve(NSpeciesReference* sR);

// NText

/// @brief Predicate returning @c true if the "text" attribute of this NText is set.
/// @param t a pointer to the NText object.
/// @return @c true if the "text" attribute of this NText object is set, @c false if either the "text" attribute is not set or the object is @c NULL .
_SBNEExport bool ne_gtxt_isSetPlainText(NText* t);

/// @brief Returns the value of the "text" attribute of this NText object as a string, if it has one
/// @param t a pointer to the NText object.
/// @return the "text" attribute of this NText object, or an empty string if either the "text" attribute is not set or the object is @c NULL .
_SBNEExport const std::string ne_gtxt_getPlainText(NText* t);

/// @brief Sets the value of the "text" attribute of this NText object.
/// @param t a pointer to the NText object.
/// @param plainText a string to use as the value of the "text" attribute of this NText object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_gtxt_setPlainText(NText* t, const std::string& plainText);

/// @brief Unset the "text" attribute of this NText object.
/// @param t a pointer to the NText object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_gtxt_unSetPlainText(NText* t);

// LCurve

/// @brief Gets the number of LLineSegment objects in this LCurve.
/// @param c a pointer to the LCurve object.
/// @return the number of LLineSegment of this LCurve object.
_SBNEExport const size_t ne_crv_getNumElements(LCurve* c);

/// @brief Gets the indexth LLineSegment object of this LCurve object.
/// @param c a pointer to the LCurve object.
/// @param index the index of the LLineSegment object to return.
/// @return a pointer to the indexth LLineSegment object of this LCurve object.
_SBNEExport LLineSegment* ne_crv_getElement(LCurve* c, const unsigned int& index);

/// @brief Gets all the LLineSegment objects of this LCurve object.
/// @param c a pointer to the LCurve object.
/// @return a vector of pointers to LLineSegment objects of this LCurve object.
_SBNEExport LCurve::listOfElements ne_crv_getElements(LCurve* c);

/// @brief Creates an LLineSegment object using startpoint and ednpoint LPoint objects and adds it to
/// this LCurve object (if basepoint1, and basepoint2 LPoint objects are not @c NULL , an LCubicBezier object is created).
/// @param c a pointer to the LCurve object.
/// @param index the index in the @c listofelements where the createed LLineSegment object is going to be placed
/// (placed at the of the list if @c index is not set).
/// @param startpoint a pointer to the startpoint LPoint object.
/// @param endpoint a pointer to the endpoint LPoint object.
/// @param basepoint1 a pointer to the control point 1 LPoint object.
/// @param basepoint2 a pointer to the control point 2 LPoint object.
_SBNEExport LLineSegment* ne_crv_addElement(LCurve* c, const int& index = -1, LPoint* startpoint = NULL, LPoint* endpoint = NULL, LPoint* basepoint1 = NULL, LPoint* basepoint2 = NULL);

/// @brief Removes the indexth LLineSegment object from this LCurve object
/// @param c a pointer to the LCurve object.
/// @param index the index of the LLineSegment object to remove.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_crv_removeElement(LCurve* c, const  int& index);

/// @brief Gets the "extentbox" attribute of this LCurve object.
/// @param c a pointer to the LCurve object.
/// @return a pointer to an LBox object with the same values as "extenbox" attribute of this LCurve object.
_SBNEExport LBox* ne_crv_getExtentBox(LCurve* c);

// LLineSegment

/// @brief Predicate returning @c true if this LLineSegment object's startpoint LPoint object is set.
/// @param l a pointer to the LLineSegment object.
/// @return @c true if the startpoint LPoint object of this LLineSegment object is set, @c false if either the startpoint LPoint object is not set or the object is @c NULL .
_SBNEExport bool ne_ls_isSetStart(LLineSegment* l);

/// @brief Gets the stratline LPoint object of this LLineSegment object.
/// @param l a pointer to the LLineSegment object.
/// @return a pointer to the startpoint LPoint object of this LLineSegment object.
_SBNEExport LPoint* ne_ls_getStart(LLineSegment* l);

/// @brief Sets the startpoint Lpoint object of this LLineSegment object.
/// @param line a pointer to the LLineSegment object.
/// @param p a pointer to the startpoint LPoint object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_ls_setStart(LLineSegment* line, LPoint* p);

/// @brief Predicate returning @c true if this LLineSegment object's endpoint LPoint object is set.
/// @param l a pointer to the LLineSegment object.
/// @return @c true if the endpoint LPoint object of this LLineSegment object is set, @c false if either the endpoint LPoint object is not set or the object is @c NULL .
_SBNEExport bool ne_ls_isSetEnd(LLineSegment* l);

/// @brief Gets the endpoint LPoint object of this LLineSegment object.
/// @param l a pointer to the LLineSegment object.
/// @return a pointer to the endpoint LPoint object of this LLineSegment object.
_SBNEExport LPoint* ne_ls_getEnd(LLineSegment* l);

/// @brief Sets the endpoint Lpoint object of this LLineSegment object.
/// @param line a pointer to the LLineSegment object.
/// @param p a pointer to the endpoint LPoint object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_ls_setEnd(LLineSegment* line, LPoint* p);

/// @brief Predicate returning @c true if this LLineSegment object is of type LCubicBezier
/// @param l a pointer to the LLineSegment object.
/// @return @c true if this LLineSegment object is of type LCubicBezier, @c false otherwise
_SBNEExport bool ne_ls_isCubicBezier(LLineSegment* l);

// LCubicBezier

/// @brief Predicate returning @c true if this LCubicBezier object's control point 1 LPoint object is set.
/// @param l a pointer to the LCubicBezier object.
/// @return @c true if the control point 1 LPoint object of this LCubicBezier object is set, @c false if either the control point 1 LPoint object is not set or the object is @c NULL .
_SBNEExport bool ne_cb_isSetBasePoint1(LLineSegment* l);

/// @brief Gets the control point 1 LPoint object of this LCubicBezier object.
/// @param l a pointer to the LCubicBezier object.
/// @return a pointer to the control point 1 LPoint object of this LCubicBezier object.
_SBNEExport LPoint* ne_cb_getBasePoint1(LLineSegment* l);

/// @brief Sets the control point 1 Lpoint object of this LCubicBezier object.
/// @param line a pointer to the LCubicBezier object.
/// @param p a pointer to the control point 1 LPoint object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_cb_setBasePoint1(LLineSegment* line, LPoint* p);

/// @brief Predicate returning @c true if this LCubicBezier object's control point 2 LPoint object is set.
/// @param l a pointer to the LCubicBezier object.
/// @return @c true if the control point 2 LPoint object of this LCubicBezier object is set, @c false if either the control point 2 LPoint object is not set or the object is @c NULL .
_SBNEExport bool ne_cb_isSetBasePoint2(LLineSegment* l);

/// @brief Gets the control point 2 LPoint object of this LCubicBezier object.
/// @param l a pointer to the LCubicBezier object.
/// @return a pointer to the control point 2 LPoint object of this LCubicBezier object.
_SBNEExport LPoint* ne_cb_getBasePoint2(LLineSegment* l);

/// @brief Sets the control point 2 Lpoint object of this LCubicBezier object.
/// @param line a pointer to the LCubicBezier object.
/// @param p a pointer to the control point 2 LPoint object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_cb_setBasePoint2(LLineSegment* line, LPoint* p);

// LBox

/// @brief Returns the value of the "x" attribute of this LBox object, if it has one
/// @param b a pointer to the LBox object.
/// @return the "x" attribute of this LBox object, or @c 0.0 if the "x" attribute is not set or the object is @c NULL .
_SBNEExport const double ne_bb_getX(LBox* b);

/// @brief Sets the value of the "x" attribute of this LBox object.
/// @param b a pointer to the LBox object.
/// @param x a double value to use as the value of the "x" attribute of this LBox object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_bb_setX(LBox* b, const double& x);

/// @brief Returns the value of the "y" attribute of this LBox object, if it has one
/// @param b a pointer to the LBox object.
/// @return the "y" attribute of this LBox object, or @c 0.0 if the "y" attribute is not set or the object is @c NULL .
_SBNEExport const double ne_bb_getY(LBox* b);

/// @brief Sets the value of the "y" attribute of this LBox object.
/// @param b a pointer to the LBox object.
/// @param y a double value to use as the value of the "y" attribute of this LBox object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_bb_setY(LBox* b, const double& y);

/// @brief Returns the value of the "width" attribute of this LBox object, if it has one
/// @param b a pointer to the LBox object.
/// @return the "width" attribute of this LBox object, or @c 0.0 if the "width" attribute is not set or the object is @c NULL .
_SBNEExport const double ne_bb_getWidth(LBox* b);

/// @brief Sets the value of the "width" attribute of this LBox object.
/// @param b a pointer to the LBox object.
/// @param width a double value to use as the value of the "width" attribute of this LBox object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_bb_setWidth(LBox* b, const double& width);

/// @brief Returns the value of the "height" attribute of this LBox object, if it has one
/// @param b a pointer to the LBox object.
/// @return the "height" attribute of this LBox object, or @c 0.0 if the "height" attribute is not set or the object is @c NULL .
_SBNEExport const double ne_bb_getHeight(LBox* b);

/// @brief Sets the value of the "height" attribute of this LBox object.
/// @param b a pointer to the LBox object.
/// @param height a double value to use as the value of the "height" attribute of this LBox object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_bb_setHeight(LBox* b, const double& height);

// LPoint

/// @brief Returns the value of the "x" attribute of this LPoint object, if it has one
/// @param p a pointer to the LPoint object.
/// @return the "x" attribute of this LPoint object, or @c 0.0 if the "x" attribute is not set or the object is @c NULL .
_SBNEExport const double ne_point_getX(LPoint* p);

/// @brief Sets the value of the "x" attribute of this LPoint object.
/// @param p a pointer to the LPoint object.
/// @param x a double value to use as the value of the "x" attribute of this LPoint object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_point_setX(LPoint* p, const double& x);

/// @brief Returns the value of the "y" attribute of this LPoint object, if it has one
/// @param p a pointer to the LPoint object.
/// @return the "y" attribute of this LPoint object, or @c 0.0 if the "y" attribute is not set or the object is @c NULL .
_SBNEExport const double ne_point_getY(LPoint* p);

/// @brief Sets the value of the "y" attribute of this LPoint object.
/// @param p a pointer to the LPoint object.
/// @param y a double value to use as the value of the "y" attribute of this LPoint object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_point_setY(LPoint* p, const double& y);
}

#endif
