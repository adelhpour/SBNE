#ifndef __NE_RENDER_H_
#define __NE_RENDER_H_

#include "sbne/network/ne_network.h"
#include "sbne/veneer/ne_veneer.h"
#include "sbne/sbml/ne_layout.h"

namespace sbne {

/// @struct RenderInfo
/// @brief Contains the render information about the SBMLDocument.
typedef struct {
    /// Used to store the Veneer object
    void* ven;
    
    /// Used to store the @c majorVersion of the render information.
    unsigned int majorVersion;
           
    /// Used to store the @c minorVersion of the render information.
    unsigned int minorVersion;
    
    /// Used to store the @c level of the SBMLDocument.
    unsigned int level;
    
    /// Used to store the @c version of the SBMLDocument.
    unsigned int version;
} RenderInfo;

RenderGroup* ne_grp_populateRenderGroup(VRenderGroup* grp);

// SBMLDocument

/// @brief Processes the render info from an SBML document.
/// @param doc a pointer to the SBMLDocument object.
/// @return a pointer to the RenderInfo object created from this SBML document content.
_SBNEExport RenderInfo* ne_doc_processRenderInfo(SBMLDocument* doc);

/// @brief Populates an SBML document with the render info.
/// @param doc a pointer to the SBMLDocument object.
/// @param r a pointer to the RenderInfo object.
/// @return a pointer to the SBMLDocument object populated with the render info content.
_SBNEExport SBMLDocument* ne_doc_populateSBMLdocWithRenderInfo(SBMLDocument* doc, RenderInfo* r);

// RenderInfo

/// @brief Gets the Veneer object of this RenderInfo object.
/// @param r a pointer to the RenderInfo object.
/// @return a pointer to the Veneer object of this RenderInfo object.
_SBNEExport Veneer* ne_ri_getVeneer(RenderInfo* r);

/// @brief Sets the Veneer object of this RenderInfo object.
/// @param r a pointer to the RenderInfo object.
/// @param ven a pointer to the Veneer object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_ri_setVeneer(RenderInfo* r, Veneer* ven);

/// @brief Automattically adds pre-defined features to the RenderInfo object's Veneer object.
/// @param r a pointer to the RenderInfo object.
/// @return a pointer to the Veneer object of this RenderInfo object.
_SBNEExport Veneer* ne_ri_addDefaultRenderFeaturesToVeneer(RenderInfo* r);

// VeneerElement

/// @brief Predicates returning @c true if the "id" attribute of this VeneerElement is set.
/// @param vE a pointer to the VeneerElement object.
/// @return @c true if the "id" attribute of this VeneerElement object is set, @c false if either the "id" attribute is not set or the object is @c NULL .
_SBNEExport bool ne_ve_isSetId(VeneerElement* vE);

/// @brief Returns the value of the "id" attribute of this VeneerElement object, if it has one
/// @param vE a pointer to the VeneerElement object.
/// @return the "id" attribute of this VeneerElement object, or an empty string if the "id" attribute is not set or the object is @c NULL .
_SBNEExport const std::string ne_ve_getId(VeneerElement* vE);

/// @brief Sets the value of the "id" attribute of this VeneerElement object.
/// @param ven a pointer to the Veneer object.
/// @param vE a pointer to the VeneerElement object.
/// @param elementId a string to use as the value of the "id" attribute of this VeneerElement object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_ve_setId(Veneer* ven, VeneerElement* vE, const std::string& elementId);

/// @brief Predicates returning @c true if the "name" attribute of this VeneerElement is set.
/// @param vE a pointer to the VeneerElement object.
/// @return @c true if the "name" attribute of this VeneerElement object is set, @c false if either the "name" attribute is not set or the object is @c NULL .
_SBNEExport bool ne_ve_isSetName(VeneerElement* vE);

/// @brief Returns the value of the "name" attribute of this VeneerElement object, if it has one
/// @param vE a pointer to the VeneerElement object.
/// @return the "name" attribute of this VeneerElement object, or an empty string if the "name" attribute is not set or the object is @c NULL .
_SBNEExport const std::string ne_ve_getName(VeneerElement* vE);

/// @brief Sets the value of the "name" attribute of this VeneerElement object.
/// @param vE a pointer to the VeneerElement object.
/// @param name a string to use as the value of the "name" attribute of this VeneerElement object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_ve_setName(VeneerElement* vE, const std::string& name);

// Veneer

/// @brief Predicates returning @c true if this Veneer object's "render" is set
/// @param ven a pointer to the Veneer object.
/// @return @c true if the "render" of this Veneer object is set, @c false if either the "render" is not set or the object is @c NULL .
_SBNEExport bool ne_ven_isRenderSpecified(Veneer* ven);

/// @brief Predicatesreturning @c true if the "backgroundcolor" attribute of this Veneer is set.
/// @param ven a pointer to the Veneer object.
/// @return @c true if the "backgroundcolor" attribute of this Veneerobject is set, @c false if either the "backgroundcolor" attribute is not set or the object is @c NULL .
_SBNEExport bool ne_ven_isSetBackgroundColor(Veneer* ven);

/// @brief Returns the value of the "backgroundcolor" attribute of this Veneer object, if it has one
/// @param ven a pointer to the Veneer object.
/// @return the "backgroundcolor" attribute of this Veneer object as a string, or an empty string if the "backgroundcolor" attribute is not set or the object is @c NULL .
_SBNEExport const std::string ne_ven_getBackgroundColor(Veneer* ven);

/// @brief Sets the value of the "backgroundcolor" attribute of this Veneer object.
/// @param ven a pointer to the Veneer object.
/// @param color a string to use as the value of the "backgroundcolor" attribute of this Veneer object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_ven_setBackgroundColor(Veneer* ven, const std::string& color);

/// @brief Gets the number of VColorDefinition objects in this Veneer object.
/// @param ven a pointer to the Veneer object.
/// @return the number of VColorDefinitions of this Veneer object.
_SBNEExport const size_t ne_ven_getNumColors(Veneer* ven);

/// @brief Gets the indexth VColorDefinition object of this Veneer object.
/// @param ven a pointer to the Veneer object.
/// @param index the index of the VColorDefinition object to return.
/// @param colorId a string containing the id of the VColorDefinition object to return, used if the @c index is not given.
/// @return a pointer to either the indexth VColorDefinition object or the one with provided id of this Veneer object.
_SBNEExport VColorDefinition* ne_ven_getColor(Veneer* ven, const unsigned int& index = -1, const std::string& colorId = "");

/// @brief Gets all the VColorDefinition objects of this Veneer object.
/// @param ven a pointer to the Veneer object.
/// @return a vector of pointers to VColorDefinition objects of this Veneer object.
_SBNEExport Veneer::colorVec ne_ven_getColors(Veneer* ven);

/// @brief Creates a VColorDefinition object, sets the values of its attributes, and adds it to the Veneer object.
/// @param ven a pointer to the Veneer  object.
/// @param colorId a string to use as the value of the "id" attribute of the VColorDefinition object.
/// @param value a string to use as the value of the "value" attribute of the VColorDefinition object.
/// @return a pointer to the added VColorDefinition object.
_SBNEExport VColorDefinition* ne_ven_addNewColor(Veneer* ven, std::string colorId = "", std::string value = "");

/// @brief Gets the number of VGradientBase objects in this Veneer object.
/// @param ven a pointer to the Veneer object.
/// @return the number of VGradientBases of this Veneer object.
_SBNEExport const size_t ne_ven_getNumGradients(Veneer* ven);

/// @brief Gets the indexth VGradientBase object of this Veneer object.
/// @param ven a pointer to the Veneer object.
/// @param index the index of the VGradientBase object to return.
/// @param gradientId a string containing the id of the VGradientBase object to return, used if the @c index is not given.
/// @return a pointer to either the indexth VGradientBase object or the one with provided id of this Veneer object.
_SBNEExport VGradientBase* ne_ven_getGradient(Veneer* ven, const unsigned int& index = -1, const std::string& gradientId = "");

/// @brief Gets all the VGradientBase objects of this Veneer object.
/// @param ven a pointer to the Veneer object.
/// @return a vector of pointers to VGradientBase objects of this Veneer object.
_SBNEExport Veneer::gradientVec ne_ven_getGradients(Veneer* ven);

/// @brief Gets the number of VLineEnding objects in this Veneer object.
/// @param ven a pointer to the Veneer object.
/// @return the number of VLineEndings of this Veneer object.
_SBNEExport const size_t ne_ven_getNumLineEndings(Veneer* ven);

/// @brief Gets the indexth VLineEnding object of this Veneer object.
/// @param ven a pointer to the Veneer object.
/// @param index the index of the VLineEnding object to return.
/// @param lineendingId a string containing the id of the VLineEnding object to return, used if the @c index is not given.
/// @return a pointer to either the indexth VLineEnding object or the one with provided id of this Veneer object.
_SBNEExport VLineEnding* ne_ven_getLineEnding(Veneer* ven, const unsigned int& index = -1, const std::string& lineendingId = "");

/// @brief Gets all the VLineEnding objects of this Veneer object.
/// @param ven a pointer to the Veneer object.
/// @return a vector of pointers to VLineEnding objects of this Veneer object.
_SBNEExport Veneer::lineEndingVec ne_ven_getLineEndings(Veneer* ven);

/// @brief Creates a VLineEnding object, sets the values of its attributes, and adds it to the Veneer object.
/// @param ven a pointer to the Veneer  object.
/// @param role a RxnRoleType to use as the value of the "role" attribute of the VLineEnding object.
/// @param lineEndingId a string to use as the value of the "id" attribute of the VLineEnding object.
/// @return a pointer to the added VLineEnding object.
_SBNEExport VLineEnding* ne_ven_addNewLineEnding(Veneer* ven,  RxnRoleType role = NUM_RxnRoleType, std::string lineEndingId = "");

/// @brief Creates a VLineEnding object localized for a specific curve end from a global VLineEnding object.
/// @param ven a pointer to the Veneer  object.
/// @param gLineEnding a pointer to the global VLineEnding object.
/// @return a pointer to the local VLineEnding object.
_SBNEExport VLineEnding* ne_ven_getLocalLineEndingFromGlobalLineEnding(Veneer* ven, VLineEnding* gLineEnding);

/// @brief Gets the number of VGlobalStyle objects in this Veneer object.
/// @param ven a pointer to the Veneer object.
/// @return the number of VGlobalStyles of this Veneer object.
_SBNEExport const size_t ne_ven_getNumStyles(Veneer* ven);

/// @brief Gets the indexth VGlobalStyle object of this Veneer object.
/// @param ven a pointer to the Veneer object.
/// @param index the index of the VGlobalStyle object to return.
/// @param styleId a string containing the id of the VGlobalStyle object to return, used if the @c index is not given.
/// @return a pointer to either the indexth VGlobalStyle object or the one with provided id of this Veneer object.
_SBNEExport VGlobalStyle* ne_ven_getStyle(Veneer* ven, const unsigned int& index = -1, const std::string& styleId = "");

/// @brief Gets all the VGlobalStyle objects of this Veneer object.
/// @param ven a pointer to the Veneer object.
/// @return a vector of pointers to VGlobalStyle objects of this Veneer object.
_SBNEExport Veneer::styleVec ne_ven_getStyles(Veneer* ven);

/// @brief Creates a VGlobalStyle object, sets the values of its attributes, and adds it to the Veneer object.
/// @param ven a pointer to the Veneer  object.
/// @param type a StyleType to be added to the "typelist" attribute of the VGlobalStyle object.
/// @param role a RxnRoleType to be added to the "roleelist" attribute of the VGlobalStyle object.
/// @param styleId a string to use as the value of the "id" attribute of the VGlobalStyle object.
/// @return a pointer to the added VGlobalStyle object.
_SBNEExport VGlobalStyle* ne_ven_addNewGlobalStyle(Veneer* ven, StyleType type = NUM_StyleType, RxnRoleType role = NUM_RxnRoleType, std::string styleId = "");

/// @brief Creates a VLocalStyle object, sets the values of its attributes, and adds it to the Veneer object.
/// @param ven a pointer to the Veneer  object.
/// @param styleId a string to use as the value of the "id" attribute of the VLocalStyle object.
/// @return a pointer to the added VLocalStyle object.
_SBNEExport VLocalStyle* ne_ven_addNewLocalStyle(Veneer* ven, std::string styleId = "");

/// @brief Searches among all VGlobalStyle and VLocalStyle objects of this Veneer object, and returns one according to the
//// following priority:
/// * the one with the "id" attribute of the NGraphicalObject object in its "idlist" attribute list.
/// * the one with the "role" attribute of the NGraphicalObject object in its "rolelist" attribute list.
/// * the one with the "type" attribute of the NGraphicalObject object in its "typelist" attribute list.
/// * the one with the entered @c type in  in its "typelist" attribute list.
/// @param ven a pointer to the Veneer object.
/// @param gO  a pointer to the NGraphicalObject object.
/// @param type a StyleType.
/// @return a pointer to the found VGlobalStyle object.
_SBNEExport VGlobalStyle* ne_ven_findStyle(Veneer* ven, NGraphicalObject* gO = NULL, StyleType type = NUM_StyleType);

/// @brief Creates a VLocalStyle object localized for a NGraphicalObject object from a VGlobalStyle object.
/// @param ven a pointer to the Veneer  object.
/// @param gLineEnding a pointer to the global VLineEnding object.
/// @return a pointer to the local VLineEnding object.
_SBNEExport VLocalStyle* ne_ven_getLocalStyleFromGlobalStyle(Veneer* ven, NGraphicalObject* gO, VGlobalStyle* gStyle);

// VColorDefenition

/// @brief Predicatesreturning @c true if the "value" attribute of this VColorDefinition is set.
/// @param c a pointer to the VColorDefinition object.
/// @return @c true if the "value" attribute of this VColorDefinition object is set, @c false if either the "value" attribute is not set or the object is @c NULL .
_SBNEExport bool ne_clr_isSetValue(VColorDefinition* c);

/// @brief Returns the value of the "value" attribute of this VColorDefinition object, if it has one
/// @param c a pointer to the VColorDefinition object.
/// @return the "value" attribute of this VColorDefinition object, or an empty string if the "value" attribute is not set or the object is @c NULL .
_SBNEExport const std::string ne_clr_getValue(VColorDefinition* c);

/// @brief Sets the value of the "value" attribute of this VColorDefinition object.
/// @param c a pointer to the VColorDefinition object.
/// @param value a  string to use as the value of the "value" attribute of this VColorDefinition object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_clr_setValue(VColorDefinition* c, const std::string& value);

// VGradientBase

/// @brief Predicatesreturning @c true if the "spreadmethod" attribute of this VGradientBase is set.
/// @param g a pointer to the VGradientBase object.
/// @return @c true if the "spreadmethod" attribute of this VGradientBase object is set, @c false if either the "spreadmethod" attribute is not set or the object is @c NULL .
_SBNEExport bool ne_grd_isSetSpreadMethod(VGradientBase* g);

/// @brief Returns the value of the "spreadmethod" attribute of this VGradientBase object, if it has one
/// @param s a pointer to the VGradientBase object.
/// @return the "spreadmethod" attribute of this VGradientBase object, or an empty string if the "spreadmethod" attribute is not set or the object is @c NULL .
_SBNEExport const std::string ne_grd_getSpreadMethod(VGradientBase* g);

/// @brief Gets the number of VGradientStop objects in this VGradientBase object.
/// @param g a pointer to the VGradientBase object.
/// @return the number of VGradientStops of this VGradientBase object.
_SBNEExport const size_t ne_grd_getNumStops(VGradientBase* g);

/// @brief Gets the indexth VGlobalStyle object of this VGradientBase object.
/// @param g a pointer to the VGradientBase object.
/// @param index the index of the VGradientStop object to return.
/// @return a pointer to either the indexth VGradientStop object of this VGradientBase object.
_SBNEExport VGradientStop* ne_grd_getStop(VGradientBase* g, const unsigned int& index);

/// @brief Predicatesreturning @c true if this VGradientBase object is of type VLinearGradient
/// @param g a pointer to the VGradientBase object.
/// @return @c true if this VGradientBase object is of type VLinearGradient, @c false otherwise
_SBNEExport bool ne_grd_isLinearGradient(VGradientBase* g);

/// @brief Predicatesreturning @c true if this VGradientBase object is of type VRadialGradient
/// @param g a pointer to the VGradientBase object.
/// @return @c true if this VGradientBase object is of type VRadialGradient, @c false otherwise
_SBNEExport bool ne_grd_isRadialGradient(VGradientBase* g);

// VLinearGradient

/// @brief Gets the "x1" RAVector object of this VLinearGradient object.
/// @param g a pointer to the VLinearGradient object.
/// @return a pointer to the  "x1" RAVector object of this VLinearGradient object.
_SBNEExport RAVector* ne_grd_getX1(VGradientBase* g);

/// @brief Gets the "y1" RAVector object of this VLinearGradient object.
/// @param g a pointer to the VLinearGradient object.
/// @return a pointer to the  "y1" RAVector object of this VLinearGradient object.
_SBNEExport RAVector* ne_grd_getY1(VGradientBase* g);

/// @brief Gets the "x2" RAVector object of this VLinearGradient object.
/// @param g a pointer to the VLinearGradient object.
/// @return a pointer to the  "x2" RAVector object of this VLinearGradient object.
_SBNEExport RAVector* ne_grd_getX2(VGradientBase* g);

/// @brief Gets the "y2" RAVector object of this VLinearGradient object.
/// @param g a pointer to the VLinearGradient object.
/// @return a pointer to the  "y2" RAVector object of this VLinearGradient object.
_SBNEExport RAVector* ne_grd_getY2(VGradientBase* g);

// VRadialGradient

/// @brief Gets the "cx" RAVector object of this VRadialGradient object.
/// @param g a pointer to the VRadialGradient object.
/// @return a pointer to the  "cx" RAVector object of this VRadialGradient object.
_SBNEExport RAVector* ne_grd_getCx(VGradientBase* g);

/// @brief Gets the "cy" RAVector object of this VRadialGradient object.
/// @param g a pointer to the VRadialGradient object.
/// @return a pointer to the  "cy" RAVector object of this VRadialGradient object.
_SBNEExport RAVector* ne_grd_getCy(VGradientBase* g);

/// @brief Gets the "fx" RAVector object of this VRadialGradient object.
/// @param g a pointer to the VRadialGradient object.
/// @return a pointer to the  "fx" RAVector object of this VRadialGradient object.
_SBNEExport RAVector* ne_grd_getFx(VGradientBase* g);

/// @brief Gets the "fy" RAVector object of this VRadialGradient object.
/// @param g a pointer to the VRadialGradient object.
/// @return a pointer to the  "fy" RAVector object of this VRadialGradient object.
_SBNEExport RAVector* ne_grd_getFy(VGradientBase* g);

/// @brief Gets the "r" RAVector object of this VRadialGradient object.
/// @param g a pointer to the VRadialGradient object.
/// @return a pointer to the "r" RAVector object of this VRadialGradient object.
_SBNEExport RAVector* ne_grd_getR(VGradientBase* g);

// VGradientStop

/// @brief Predicatesreturning @c true if the "offset" attribute of this VGradientStop is set.
/// @param s a pointer to the VGradientStop object.
/// @return @c true if the "offset" attribute of this VGradientStop object is set, @c false if either the "offset" attribute is not set or the object is @c NULL .
_SBNEExport bool ne_gstp_isSetOffset(VGradientStop* s);

/// @brief Returns the value of the "offset" attribute of this VGradientStop object, if it has one
/// @param s a pointer to the VGradientStop object.
/// @return the "offset" attribute of this VGradientStop object, or an empty string if the "offset" attribute is not set or the object is @c NULL .
_SBNEExport RAVector* ne_gstp_getOffset(VGradientStop* s);

/// @brief Predicatesreturning @c true if the "color" attribute of this VGradientStop is set.
/// @param s a pointer to the VGradientStop object.
/// @return @c true if the "color" attribute of this VGradientStop object is set, @c false if either the "color" attribute is not set or the object is @c NULL .
_SBNEExport bool ne_gstp_isSetColor(VGradientStop* s);

/// @brief Returns the value of the "color" attribute of this VGradientStop object, if it has one
/// @param s a pointer to the VGradientStop object.
/// @return the "color" attribute of this VGradientStop object, or an empty string if the "color" attribute is not set or the object is @c NULL .
_SBNEExport const std::string ne_gstp_getColor(VGradientStop* s);

// VLineEnding

/// @brief Automattically sets pre-defined values to the features of the VLineEnding object based on its "role" attribute.
/// @param ven a pointer to the Veneer object.
/// @param lE a pointer to the VLineEnding object.
/// @param role a RxnRoleType to use as the value of the "role" attribute of this VLineEnding object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_le_setLineEndingValues(Veneer* ven, VLineEnding* lE, RxnRoleType role);

/// @brief Predicatesreturning @c true if this VLineEnding object's LBox object is set.
/// @param lE a pointer to the VLineEnding object.
/// @return @c true if the LBox object of this VLineEnding object is set, @c false if either the LBox object is not set or the object is @c NULL .
_SBNEExport bool ne_le_isSetBoundingBox(VLineEnding* lE);

/// @brief Gets the LBox object of this VLineEnding object.
/// @param lE a pointer to the VLineEnding object.
/// @return a pointer to the LBox object of this VLineEnding object.
_SBNEExport LBox* ne_le_getBoundingBox(VLineEnding* lE);

/// @brief Sets the LBox object of this VLineEnding object.
/// @param lE a pointer to the VLineEnding object.
/// @param b a pointer to the LBox object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_le_setBoundingBox(VLineEnding* lE, LBox* b);

/// @brief Predicatesreturning @c true if this VLineEnding object's VRenderGroup object is set.
/// @param lE a pointer to the VLineEnding object.
/// @return @c true if the VRenderGroup object of this VLineEnding object is set, @c false if either the VRenderGroup object is not set or the object is @c NULL .
_SBNEExport bool ne_le_isSetGroup(VLineEnding* lE);

/// @brief Gets the VRenderGroup object of this VLineEnding object.
/// @param lE a pointer to the VLineEnding object.
/// @return a pointer to the VRenderGroup object of this VLineEnding object.
_SBNEExport VRenderGroup* ne_le_getGroup(VLineEnding* lE);

/// @brief Sets the VRenderGroup object of this VLineEnding object.
/// @param lE a pointer to the VLineEnding object.
/// @param g a pointer to the VRenderGroup object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_le_setGroup(VLineEnding* lE, VRenderGroup* g);

/// @brief Predicatesreturning @c true if the "enablerotationmapping" attribute of this VLineEnding is set.
/// @param lE a pointer to the VLineEnding object.
/// @return @c true if the "enablerotationmapping" attribute of this VLineEnding object is set, @c false if either the "enablerotationmapping" attribute is not set or the object is @c NULL .
_SBNEExport bool ne_le_isSetEnableRotation(VLineEnding* lE);

/// @brief Returns the value of the "enablerotationmapping" attribute of this VLineEnding object, if it has one
/// @param lE a pointer to the VLineEnding object.
/// @return the "enablerotationmapping" attribute of this VLineEnding object, or an empty string if the "enablerotationmapping" attribute is not set or the object is @c NULL .
_SBNEExport bool ne_le_getEnableRotation(VLineEnding* lE);

/// @brief Sets the value of the "enableRotationalMapping" attribute of this VLineEnding object.
/// @param lE a pointer to the VLineEnding object.
/// @param enable bool value of the "enableRotationalMapping" attribute to be set.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_le_enableRotation(VLineEnding* lE, const bool& enable);

// VGlobalStyle

_SBNEExport int ne_stl_setStyleValues(VGlobalStyle* s, StyleType type = NUM_StyleType, RxnRoleType role = NUM_RxnRoleType);

/// @brief Predicatesreturning @c true if this VGlobalStyle object's VRenderGroup object is set.
/// @param s a pointer to the VGlobalStyle object.
/// @return @c true if the VRenderGroup object of this VGlobalStyle object is set, @c false if either the VRenderGroup object is not set or the object is @c NULL .
_SBNEExport bool ne_stl_isSetGroup(VGlobalStyle* s);

/// @brief Gets the VRenderGroup object of this VGlobalStyle object.
/// @param s a pointer to the VGlobalStyle object.
/// @return a pointer to the VRenderGroup object of this VGlobalStyle object.
_SBNEExport VRenderGroup* ne_stl_getGroup(VGlobalStyle* s);

/// @brief Sets the VRenderGroup object of this VGlobalStyle object.
/// @param s a pointer to the VGlobalStyle object.
/// @param g a pointer to the VRenderGroup object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_stl_setGroup(VGlobalStyle* s, VRenderGroup* g);

/// @brief Adds another type to the "typelist" attribute of this VGlobalStyle object.
/// @param s a pointer to the VGlobalStyle object.
/// @param type a string to add to the "typelist" attribute of the VGlobalStyle object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_stl_addToTypeList(VGlobalStyle* s, const std::string& type);

/// @brief Adds another type to the "rolelist" attribute of this VGlobalStyle object.
/// @param s a pointer to the VGlobalStyle object.
/// @param role a string to add to the "rolelist" attribute of the VGlobalStyle object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_stl_addToRoleList(VGlobalStyle* s, const std::string& role);

// VLocalStyle

/// @brief Adds another id to the "idlist" attribute of this VLocalStyle object.
/// @param s a pointer to the VLocalStyle object.
/// @param gOId a string containing an NGraphicalObject object's "id" attribute to add to the "idlist" attribute of the
/// VGlobalStyle object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_stl_addToIdList(VGlobalStyle* s, const std::string& gOId);

//VRenderGroup

/// @brief Predicatesreturning @c true if the "stroke" attribute of this VRenderGroup is set.
/// @param g a pointer to the VRenderGroup object.
/// @return @c true if the "stroke" attribute of this VRenderGroup object is set, @c false if either the "stroke" attribute is not set or the object is @c NULL .
_SBNEExport bool ne_grp_isSetStrokeColor(VRenderGroup* g);

/// @brief Returns the value of the "stroke" attribute of this VRenderGroup object, if it has one
/// @param g a pointer to the VRenderGroup object.
/// @return the "stroke" attribute of this VRenderGroup object, or an empty string if the "stroke" attribute is not set or the object is @c NULL .
_SBNEExport const std::string ne_grp_getStrokeColor(VRenderGroup* g);

/// @brief Sets the value of the "stroke" attribute of this VRenderGroup object.
/// @param g a pointer to the VRenderGroup object.
/// @param color a  string to use as the value of the "stroke" attribute of this VRenderGroup object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_grp_setStrokeColor(VRenderGroup* g, const std::string& color);

/// @brief Predicatesreturning @c true if the "strokewidth" attribute of this VRenderGroup is set.
/// @param g a pointer to the VRenderGroup object.
/// @return @c true if the "strokewidth" attribute of this VRenderGroup object is set, @c false if either the "strokewidth" attribute is not set or the object is @c NULL .
_SBNEExport bool ne_grp_isSetStrokeWidth(VRenderGroup* g);

/// @brief Returns the value of the "strokewidth" attribute of this VRenderGroup object, if it has one
/// @param g a pointer to the VRenderGroup object.
/// @return the "strokewidth" attribute of this VRenderGroup object, or @c 0.0 if the "strokewidth" attribute is not set or the object is @c NULL .
_SBNEExport const double ne_grp_getStrokeWidth(VRenderGroup* g);

/// @brief Sets the value of the "strokewidth" attribute of this VRenderGroup object.
/// @param g a pointer to the VRenderGroup object.
/// @param width a  double value to use as the value of the "strokewidth" attribute of this VRenderGroup object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_grp_setStrokeWidth(VRenderGroup* g, const double& width);

/// @brief Predicatesreturning @c true if the "dasharray" attribute of this VRenderGroup is set.
/// @param g a pointer to the VRenderGroup object.
/// @return @c true if the "dasharray" attribute of this VRenderGroup object is set, @c false if either the "dasharray" attribute is not set or the object is @c NULL .
_SBNEExport bool ne_grp_isSetStrokeDashArray(VRenderGroup* g);

/// @brief Gets the size of  "dasharray" attribute of this VRenderGroup.
/// @param g a pointer to the VRenderGroup object.
/// @return the size of "dasharray" attribute of this VRenderGroup object.
_SBNEExport const size_t ne_grp_getNumStrokeDashes(VRenderGroup* g);

/// @brief Gets the indexth item of  "dasharray" attribute of this VRenderGroup object.
/// @param g a pointer to the VRenderGroup object.
/// @param index the index of the item of "dasharray" attribute of this VRenderGroup to return.
/// @return the indexth item of "dasharray" attribute of this VRenderGroup object, or @c -1 if this item of "dasharray" attribute is not set or the object is @c NULL .
_SBNEExport const int ne_grp_getStrokeDash(VRenderGroup* g, const unsigned int& index);

/// @brief Returns the vector of the "dasharray" attribute of this VRenderGroup object, if it has one
/// @param g a pointer to the VRenderGroup object.
/// @return a pointer to a vector contining the "dasharray" attribute of this VRenderGroup object.
_SBNEExport VGraphicalPrimitive1D::dashArrayVec* ne_grp_getStrokeDashArray(VRenderGroup* g);

/// @brief Sets the "dasharray" attribute of this VRenderGroup object.
/// @param g a pointer to the VRenderGroup object.
/// @param dashArray a pointer to a vector contining the "dasharray" attribute of this VRenderGroup object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_grp_setStrokeDashArray(VRenderGroup* g, VGraphicalPrimitive1D::dashArrayVec* dashArray);

/// @brief Predicatesreturning @c true if the "fil" attribute of this VRenderGroup is set.
/// @param g a pointer to the VRenderGroup object.
/// @return @c true if the "fill" attribute of this VRenderGroup object is set, @c false if either the "fill" attribute is not set or the object is @c NULL .
_SBNEExport bool ne_grp_isSetFillColor(VRenderGroup* g);

/// @brief Returns the value of the "fill" attribute of this VRenderGroup object, if it has one
/// @param g a pointer to the VRenderGroup object.
/// @return the "fill" attribute of this VRenderGroup object, or an empty string if the "fill" attribute is not set or the object is @c NULL .
_SBNEExport const std::string ne_grp_getFillColor(VRenderGroup* g);

/// @brief Sets the value of the "fill" attribute of this VRenderGroup object.
/// @param g a pointer to the VRenderGroup object.
/// @param color a  string to use as the value of the "fill" attribute of this VRenderGroup object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_grp_setFillColor(VRenderGroup* g, const std::string& color);

/// @brief Predicatesreturning @c true if the "filrule" attribute of this VRenderGroup is set.
/// @param g a pointer to the VRenderGroup object.
/// @return @c true if the "fillrule" attribute of this VRenderGroup object is set, @c false if either the "fillrule" attribute is not set or the object is @c NULL .
_SBNEExport bool ne_grp_isSetFillRule(VRenderGroup* g);

/// @brief Returns the value of the "fillrule" attribute of this VRenderGroup object, if it has one
/// @param g a pointer to the VRenderGroup object.
/// @return the "fillrule" attribute of this VRenderGroup object, or an empty string if the "fillrule" attribute is not set or the object is @c NULL .
_SBNEExport const std::string ne_grp_getFillRule(VRenderGroup* g);

/// @brief Sets the value of the "fillrule" attribute of this VRenderGroup object.
/// @param g a pointer to the VRenderGroup object.
/// @param fillrule a  string to use as the value of the "fillrule" attribute of this VRenderGroup object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_grp_setFillRule(VRenderGroup* g, const std::string& fillrule);

/// @brief Predicatesreturning @c true if the "fontfamily" attribute of this VRenderGroup is set.
/// @param g a pointer to the VRenderGroup object.
/// @return @c true if the "fontfamily" attribute of this VRenderGroup object is set, @c false if either the "fontfamily" attribute is not set or the object is @c NULL .
_SBNEExport bool ne_grp_isSetFontFamily(VRenderGroup* g);

/// @brief Returns the value of the "fontfamily" attribute of this VRenderGroup object, if it has one
/// @param g a pointer to the VRenderGroup object.
/// @return the "fontfamily" attribute of this VRenderGroup object, or an empty string if the "fontfamily" attribute is not set or the object is @c NULL .
_SBNEExport const std::string ne_grp_getFontFamily(VRenderGroup* g);

/// @brief Sets the value of the "fontfamily" attribute of this VRenderGroup object.
/// @param g a pointer to the VRenderGroup object.
/// @param fontfamily a  string to use as the value of the "fontfamily" attribute of this VRenderGroup object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_grp_setFontFamily(VRenderGroup* g, const std::string& fontfamily);

/// @brief Predicatesreturning @c true if this VRenderGroup object's "fontsize"  RAVector object is set.
/// @param g a pointer to the VRenderGroup object.
/// @return @c true if the "fontsize"  RAVector object of this VRenderGroup object is set, @c false if either the "fontsize"  RAVector object is not set or the object is @c NULL .
_SBNEExport bool ne_grp_isSetFontSize(VRenderGroup* g);

/// @brief Gets the "fontsize" RAVector object of this VRenderGroup object.
/// @param g a pointer to the VRenderGroup object.
/// @return a pointer to the "fontsize" RAVector object of this VRenderGroup object.
_SBNEExport RAVector* ne_grp_getFontSize(VRenderGroup* g);

/// @brief Sets the "fontsize" RAVector object of this VRenderGroup object.
/// @param g a pointer to the VRenderGroup object.
/// @param fontsize a pointer to the  "fontsize" RAVector object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_grp_setFontSize(VRenderGroup* g, RAVector* fontsize);

/// @brief Predicatesreturning @c true if the "fontstyle" attribute of this VRenderGroup is set.
/// @param g a pointer to the VRenderGroup object.
/// @return @c true if the "fontstyle" attribute of this VRenderGroup object is set, @c false if either the "fontstyle" attribute is not set or the object is @c NULL .
_SBNEExport bool ne_grp_isSetFontStyle(VRenderGroup* g);

/// @brief Returns the value of the "fontstyle" attribute of this VRenderGroup object, if it has one
/// @param g a pointer to the VRenderGroup object.
/// @return the "fontstyle" attribute of this VRenderGroup object, or an empty string if the "fontstyle" attribute is not set or the object is @c NULL .
_SBNEExport const std::string ne_grp_getFontStyle(VRenderGroup* g);

/// @brief Sets the value of the "fontstyle" attribute of this VRenderGroup object.
/// @param g a pointer to the VRenderGroup object.
/// @param fontstyle a  string to use as the value of the "fontstyle" attribute of this VRenderGroup object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_grp_setFontStyle(VRenderGroup* g, const std::string& fontstyle);

/// @brief Predicatesreturning @c true if the "fontweight" attribute of this VRenderGroup is set.
/// @param g a pointer to the VRenderGroup object.
/// @return @c true if the "fontweight" attribute of this VRenderGroup object is set, @c false if either the "fontweight" attribute is not set or the object is @c NULL .
_SBNEExport bool ne_grp_isSetFontWeight(VRenderGroup* g);

/// @brief Returns the value of the "fontweight" attribute of this VRenderGroup object, if it has one
/// @param g a pointer to the VRenderGroup object.
/// @return the "fontweight" attribute of this VRenderGroup object, or an empty string if the "fontweight" attribute is not set or the object is @c NULL .
_SBNEExport const std::string ne_grp_getFontWeight(VRenderGroup* g);

/// @brief Sets the value of the "fontweight" attribute of this VRenderGroup object.
/// @param g a pointer to the VRenderGroup object.
/// @param fontweight a  string to use as the value of the "fontweight" attribute of this VRenderGroup object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_grp_setFontWeight(VRenderGroup* g, const std::string& fontweight);

/// @brief Predicatesreturning @c true if the "textanchor" attribute of this VRenderGroup is set.
/// @param g a pointer to the VRenderGroup object.
/// @return @c true if the "textanchor" attribute of this VRenderGroup object is set, @c false if either the "textanchor" attribute is not set or the object is @c NULL .
_SBNEExport bool ne_grp_isSetHTextAnchor(VRenderGroup* g);

/// @brief Returns the value of the "textanchor" attribute of this VRenderGroup object, if it has one
/// @param g a pointer to the VRenderGroup object.
/// @return the "textanchor" attribute of this VRenderGroup object, or an empty string if the "textanchor" attribute is not set or the object is @c NULL .
_SBNEExport const std::string ne_grp_getHTextAnchor(VRenderGroup* g);

/// @brief Sets the value of the "textanchor" attribute of this VRenderGroup object.
/// @param g a pointer to the VRenderGroup object.
/// @param anchor a  string to use as the value of the "textanchor" attribute of this VRenderGroup object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_grp_setHTextAnchor(VRenderGroup* g, const std::string& anchor);

/// @brief Predicatesreturning @c true if the "vtextanchor" attribute of this VRenderGroup is set.
/// @param g a pointer to the VRenderGroup object.
/// @return @c true if the "vtextanchor" attribute of this VRenderGroup object is set, @c false if either the "vtextanchor" attribute is not set or the object is @c NULL .
_SBNEExport bool ne_grp_isSetVTextAnchor(VRenderGroup* g);

/// @brief Returns the value of the "vtextanchor" attribute of this VRenderGroup object, if it has one
/// @param g a pointer to the VRenderGroup object.
/// @return the "vtextanchor" attribute of this VRenderGroup object, or an empty string if the "vtextanchor" attribute is not set or the object is @c NULL .
_SBNEExport const std::string ne_grp_getVTextAnchor(VRenderGroup* g);

/// @brief Sets the value of the "vtextanchor" attribute of this VRenderGroup object.
/// @param g a pointer to the VRenderGroup object.
/// @param anchor a  string to use as the value of the "vtextanchor" attribute of this VRenderGroup object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_grp_setVTextAnchor(VRenderGroup* g, const std::string& anchor);

/// @brief Predicatesreturning @c true if the "starthead" attribute of this VRenderGroup is set.
/// @param g a pointer to the VRenderGroup object.
/// @return @c true if the "starthead" attribute of this VRenderGroup object is set, @c false if either the "starthead" attribute is not set or the object is @c NULL .
_SBNEExport bool ne_grp_isSetStartHead(VRenderGroup* g);

/// @brief Returns the value of the "starthead" attribute of this VRenderGroup object, if it has one
/// @param g a pointer to the VRenderGroup object.
/// @return the "starthead" attribute of this VRenderGroup object, or an empty string if the "starthead" attribute is not set or the object is @c NULL .
_SBNEExport const std::string ne_grp_getStartHead(VRenderGroup* g);

/// @brief Sets the value of the "starthead" attribute of this VRenderGroup object.
/// @param g a pointer to the VRenderGroup object.
/// @param head a  string to use as the value of the "starthead" attribute of this VRenderGroup object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_grp_setStartHead(VRenderGroup* g, const std::string& head);

/// @brief Predicatesreturning @c true if the "endhead" attribute of this VRenderGroup is set.
/// @param g a pointer to the VRenderGroup object.
/// @return @c true if the "endhead" attribute of this VRenderGroup object is set, @c false if either the "endhead" attribute is not set or the object is @c NULL .
_SBNEExport bool ne_grp_isSetEndHead(VRenderGroup* g);

/// @brief Returns the value of the "endhead" attribute of this VRenderGroup object, if it has one
/// @param g a pointer to the VRenderGroup object.
/// @return the "endhead" attribute of this VRenderGroup object, or an empty string if the "endhead" attribute is not set or the object is @c NULL .
_SBNEExport const std::string ne_grp_getEndHead(VRenderGroup* g);

/// @brief Sets the value of the "endhead" attribute of this VRenderGroup object.
/// @param g a pointer to the VRenderGroup object.
/// @param head a  string to use as the value of the "endhead" attribute of this VRenderGroup object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_grp_setEndHead(VRenderGroup* g, const std::string& head);

/// @brief Gets the number of VTransformation2D objects in this VRenderGroup object.
/// @param g a pointer to the VRenderGroup object.
/// @return the number of VTransformation2Ds of this VRenderGroup object.
_SBNEExport const size_t ne_grp_getNumGeometricShapes(VRenderGroup* g);

/// @brief Gets the indexth VTransformation2D object of this VRenderGroup object.
/// @param g a pointer to the VRenderGroup object.
/// @param index the index of the VTransformation2D object to return.
/// @return a pointer to the indexth VTransformation2D object of this VRenderGroup object.
_SBNEExport VTransformation2D* ne_grp_getGeometricShape(VRenderGroup* g, const unsigned int& index);

/// @brief Removes all VTransformation2D objects from this VRenderGroup object.
/// @param g a pointer to the VRenderGroup object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_grp_removeGeometricShapes(VRenderGroup* g);

/// @brief Creates a VTransformation2D object, sets the values of its attributes, and adds it to the VRenderGroup object.
/// @param g a pointer to the VRenderGroup  object.
/// @param rGEShape a RenderGroupElementShape to use as the "shape" attribute of  the VTransformation2D  object.
/// @param numvertices an  integer value indicating the number of vertices for a VPolygon object.
/// @param linending  a bool value to determine whether the VTransformation2D object is used for a line ending or not.
/// @param href a string to use as the value of the "href" attribute of a VImage object.
/// @return a pointer to the added VTransformation2D object.
_SBNEExport VTransformation2D* ne_grp_addGeometricShape(VRenderGroup* g, const RenderGroupElementShape rGEShape, const bool& lineending = false, const int& numvertices = -1, const std::string& href = "");

// VTransformation2D

/// @brief Gets the "shape" attribute of this VTransformation2D object.
/// @param gS a pointer to the VTransformation2D object.
/// @return the "shape" attribute of this VTransformation2D object as a RenderGroupElementShape.
_SBNEExport const RenderGroupElementShape ne_gs_getShape(VTransformation2D* gS);

/// @brief Automattically sets pre-defined values to the features of the VTransformation2D object based on its "shape" attribute.
/// @param gS a pointer to the VTransformation2D object.
/// @param numvertices an  integer value indicating the number of vertices for a VPolygon object.
/// @param href a string to use as the value of the "href" attribute of a VImage object.
/// @param linending  a bool value to determine whether the VTransformation2D object is used for a line ending or not.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_gs_setShapeFeatures(VTransformation2D* gS, const int& numvertices, const std::string& href, const bool& lineending);

/// @brief Predicatesreturning @c true if the "stroke" attribute of this VTransformation2D is set.
/// @param gS a pointer to the VTransformation2D object.
/// @return @c true if the "stroke" attribute of this VTransformation2D object is set, @c false if either the "stroke" attribute is not set or the object is @c NULL .
_SBNEExport bool ne_gs_isSetStrokeColor(VTransformation2D* gS);

/// @brief Returns the value of the "stroke" attribute of this VTransformation2D object, if it has one
/// @param gS a pointer to the VTransformation2D object.
/// @return the "stroke" attribute of this VTransformation2D object, or an empty string if the "stroke" attribute is not set or the object is @c NULL .
_SBNEExport const std::string ne_gs_getStrokeColor(VTransformation2D* gS);

/// @brief Sets the value of the "stroke" attribute of this VTransformation2D object.
/// @param gS a pointer to the VTransformation2D object.
/// @param color a  string to use as the value of the "stroke" attribute of this VTransformation2D object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_gs_setStrokeColor(VTransformation2D* gS, const std::string& color);

/// @brief Predicatesreturning @c true if the "strokewidth" attribute of this VTransformation2D is set.
/// @param gS a pointer to the VTransformation2D object.
/// @return @c true if the "strokewidth" attribute of this VTransformation2D object is set, @c false if either the "strokewidth" attribute is not set or the object is @c NULL .
_SBNEExport bool ne_gs_isSetStrokeWidth(VTransformation2D* gS);

/// @brief Returns the value of the "strokewidth" attribute of this VTransformation2D object, if it has one
/// @param gS a pointer to the VTransformation2D object.
/// @return the "strokewidth" attribute of this VTransformation2D object, or @c 0.0 if the "strokewidth" attribute is not set or the object is @c NULL .
_SBNEExport const double ne_gs_getStrokeWidth(VTransformation2D* gS);

/// @brief Sets the value of the "strokewidth" attribute of this VTransformation2D object.
/// @param gS a pointer to the VTransformation2D object.
/// @param width a double value to use as the value of the "strokewidth" attribute of this VTransformation2D object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_gs_setStrokeWidth(VTransformation2D* gS, const double& width);

/// @brief Predicatesreturning @c true if the "dasharray" attribute of this VTransformation2D is set.
/// @param gS a pointer to the VTransformation2D object.
/// @return @c true if the "dasharray" attribute of this VTransformation2D object is set, @c false if either the "dasharray" attribute is not set or the object is @c NULL .
_SBNEExport bool ne_gs_isSetStrokeDashArray(VTransformation2D* gS);

/// @brief Gets the size of  "dasharray" attribute of this VTransformation2D.
/// @param gS a pointer to the VTransformation2D object.
/// @return the size of "dasharray" attribute of this VTransformation2D object.
_SBNEExport const size_t ne_gs_getNumStrokeDashes(VTransformation2D* gS);

/// @brief Gets the indexth item of  "dasharray" attribute of this VTransformation2D object.
/// @param gS a pointer to the VTransformation2D object.
/// @param index the index of the item of "dasharray" attribute of this VTransformation2D to return.
/// @return the indexth item of "dasharray" attribute of this VTransformation2D object, or @c -1 if this item of "dasharray" attribute is not set or the object is @c NULL .
_SBNEExport const int ne_gs_getStrokeDash(VTransformation2D* gS, const unsigned int& index);

/// @brief Returns the vector of the "dasharray" attribute of this VTransformation2D object, if it has one
/// @param gS a pointer to the VTransformation2D object.
/// @return a pointer to a vector contining the "dasharray" attribute of this VTransformation2D object.
_SBNEExport VGraphicalPrimitive1D::dashArrayVec* ne_gs_getStrokeDashArray(VTransformation2D* gS);

/// @brief Sets the "dasharray" attribute of this VTransformation2D object.
/// @param gS a pointer to the VTransformation2D object.
/// @param dashArray a pointer to a vector contining the "dasharray" attribute of this VTransformation2D object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_gs_setStrokeDashArray(VTransformation2D* gS, VGraphicalPrimitive1D::dashArrayVec* dasharray);

/// @brief Predicatesreturning @c true if the "fill" attribute of this VTransformation2D is set.
/// @param gS a pointer to the VTransformation2D object.
/// @return @c true if the "fill" attribute of this VTransformation2D object is set, @c false if either the "fill" attribute is not set or the object is @c NULL .
_SBNEExport bool ne_gs_isSetFillColor(VTransformation2D* gS);

/// @brief Returns the value of the "fill" attribute of this VTransformation2D object, if it has one
/// @param gS a pointer to the VTransformation2D object.
/// @return the "fill" attribute of this VTransformation2D object, or an empty string if the "fill" attribute is not set or the object is @c NULL .
_SBNEExport const std::string ne_gs_getFillColor(VTransformation2D* gS);

/// @brief Sets the value of the "fill" attribute of this VTransformation2D object.
/// @param gS a pointer to the VTransformation2D object.
/// @param color a string to use as the value of the "fill" attribute of this VTransformation2D object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_gs_setFillColor(VTransformation2D* gS, const std::string& color);

/// @brief Predicatesreturning @c true if the "fillrule" attribute of this VTransformation2D is set.
/// @param gS a pointer to the VTransformation2D object.
/// @return @c true if the "fillrule" attribute of this VTransformation2D object is set, @c false if either the "fillrule" attribute is not set or the object is @c NULL .
_SBNEExport bool ne_gs_isSetFillRule(VTransformation2D* gS);

/// @brief Returns the value of the "fillrule" attribute of this VTransformation2D object, if it has one
/// @param gS a pointer to the VTransformation2D object.
/// @return the "fillrule" attribute of this VTransformation2D object, or an empty string if the "fillrule" attribute is not set or the object is @c NULL .
_SBNEExport const std::string ne_gs_getFillRule(VTransformation2D* gS);

/// @brief Sets the value of the "fillrule" attribute of this VTransformation2D object.
/// @param gS a pointer to the VTransformation2D object.
/// @param fillrule a string to use as the value of the "fillrule" attribute of this VTransformation2D object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_gs_setFillRule(VTransformation2D* gS, const std::string& fillrule);

// VImage

/// @brief Predicatesreturning @c true if this VImage object's "x"  RAVector object is set.
/// @param gS a pointer to the VImage object.
/// @return @c true if the "x"  RAVector object of this VImage object is set, @c false if either the "x"  RAVector object is not set or the object is @c NULL .
_SBNEExport bool ne_img_isSetPositionX(VTransformation2D* gS);

/// @brief Gets the "x" RAVector object of this VImage object.
/// @param gS a pointer to the VImage object.
/// @return a pointer to the "x" RAVector object of this VImage object.
_SBNEExport RAVector* ne_img_getPositionX(VTransformation2D* gS);

/// @brief Sets the "x" RAVector object of this VImage object.
/// @param gS a pointer to the VImage object.
/// @param x a pointer to the  "x" RAVector object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_img_setPositionX(VTransformation2D* gS, RAVector* x);

/// @brief Predicatesreturning @c true if this VImage object's "y"  RAVector object is set.
/// @param gS a pointer to the VImage object.
/// @return @c true if the "y"  RAVector object of this VImage object is set, @c false if either the "y"  RAVector object is not set or the object is @c NULL .
_SBNEExport bool ne_img_isSetPositionY(VTransformation2D* gS);

/// @brief Gets the "y" RAVector object of this VImage object.
/// @param gS a pointer to the VImage object.
/// @return a pointer to the "y" RAVector object of this VImage object.
_SBNEExport RAVector* ne_img_getPositionY(VTransformation2D* gS);

/// @brief Sets the "y" RAVector object of this VImage object.
/// @param gS a pointer to the VImage object.
/// @param y a pointer to the  "y" RAVector object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_img_setPositionY(VTransformation2D* gS, RAVector* y);

/// @brief Predicatesreturning @c true if this VImage object's "width"  RAVector object is set.
/// @param gS a pointer to the VImage object.
/// @return @c true if the "width"  RAVector object of this VImage object is set, @c false if either the "width"  RAVector object is not set or the object is @c NULL .
_SBNEExport bool ne_img_isSetDimensionWidth(VTransformation2D* gS);

/// @brief Gets the "width" RAVector object of this VImage object.
/// @param gS a pointer to the VImage object.
/// @return a pointer to the "width" RAVector object of this VImage object.
_SBNEExport RAVector* ne_img_getDimensionWidth(VTransformation2D* gS);

/// @brief Sets the "width" RAVector object of this VImage object.
/// @param gS a pointer to the VImage object.
/// @param width a pointer to the  "width" RAVector object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_img_setDimensionWidth(VTransformation2D* gS, RAVector* width);

/// @brief Predicatesreturning @c true if this VImage object's "height"  RAVector object is set.
/// @param gS a pointer to the VImage object.
/// @return @c true if the "height"  RAVector object of this VImage object is set, @c false if either the "height"  RAVector object is not set or the object is @c NULL .
_SBNEExport bool ne_img_isSetDimensionHeight(VTransformation2D* gS);

/// @brief Gets the "height" RAVector object of this VImage object.
/// @param gS a pointer to the VImage object.
/// @return a pointer to the "height" RAVector object of this VImage object.
_SBNEExport RAVector* ne_img_getDimensionHeight(VTransformation2D* gS);

/// @brief Sets the "height" RAVector object of this VImage object.
/// @param gS a pointer to the VImage object.
/// @param height a pointer to the  "height" RAVector object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_img_setDimensionHeight(VTransformation2D* gS, RAVector* height);

/// @brief Predicatesreturning @c true if the "href" attribute of this VImage is set.
/// @param gS a pointer to the VImage object.
/// @return @c true if the "href" attribute of this VImage object is set, @c false if either the "href" attribute is not set or the object is @c NULL .
_SBNEExport bool ne_img_isSetHref(VTransformation2D* gS);

/// @brief Returns the value of the "href" attribute of this VImage object, if it has one
/// @param gS a pointer to the VImage object.
/// @return the "href" attribute of this VImage object, or an empty string if the "href" attribute is not set or the object is @c NULL .
_SBNEExport const std::string ne_img_getHref(VTransformation2D* gS);

/// @brief Sets the value of the "href" attribute of this VImage object.
/// @param gS a pointer to the VImage object.
/// @param href a string to use as the value of the "href" attribute of this VImage object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_img_setHref(VTransformation2D* gS, const std::string& href);

// RCurve

/// @brief Gets the number of RenPoint objects in this RCurve.
/// @param gS a pointer to the RCurve object.
/// @return the number of RenPoints of this RCurve object.
_SBNEExport const size_t ne_rc_getNumVertices(VTransformation2D* gS);

/// @brief Gets the indexth RenPoint object of this RCurve object.
/// @param gS a pointer to the RCurve object.
/// @param index the index of the RenPoint object to return.
/// @return a pointer to the indexth RenPoint object of this RCurve object.
_SBNEExport RenPoint* ne_rc_getVertex(VTransformation2D* gS, const unsigned int& index);

/// @brief Creates an RenPoint object using "point" RPoint object and adds it to
/// this RCurve object (if "basepoint1", and "basepoint2" RPoint objects are not @c NULL , an RCubicBezier object is created).
/// @param gS a pointer to the RCurve  object.
/// @param point a pointer to the "point" RPoint object.
/// @param basepoint1 a pointer to the "controlpoint1" RPoint object.
/// @param basepoint2 a pointer to the "controlpoint2" RPoint object.
/// @param index the index in the @c listofelements where the createed RenPoint object is going to be placed
/// (placed at the of the list if @c index is not set).
/// @return a pointer to the added RenPoint object.
_SBNEExport RenPoint* ne_rc_addVertex(VTransformation2D* gS, RPoint* point = NULL, RPoint* basePoint1 = NULL, RPoint* basePoint2 = NULL, const int& index = -1);

/// @brief Removes the indexth RenPoint object from this RCurve object.
/// @param gS a pointer to the RCurve object.
/// @param index the index of the RenPoint object to remove.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_rc_removeVertex(VTransformation2D* gS, const unsigned int& index);

/// @brief Predicatesreturning @c true if the "starthead" attribute of this RCurve is set.
/// @param gS a pointer to the RCurve object.
/// @return @c true if the "starthead" attribute of this RCurve object is set, @c false if either the "starthead" attribute is not set or the object is @c NULL .
_SBNEExport bool ne_rc_isSetStartHead(VTransformation2D* gS);

/// @brief Returns the value of the "starthead" attribute of this RCurve object, if it has one
/// @param gS a pointer to the RCurve object.
/// @return the "starthead" attribute of this RCurve object, or an empty string if the "starthead" attribute is not set or the object is @c NULL .
_SBNEExport const std::string ne_rc_getStartHead(VTransformation2D* gS);

/// @brief Sets the value of the "starthead" attribute of this RCurve object.
/// @param gS a pointer to the RCurve object.
/// @param head a string to use as the value of the "starthead" attribute of this RCurve object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_rc_setStartHead(VTransformation2D* gS, const std::string& head);

/// @brief Predicatesreturning @c true if the "endhead" attribute of this RCurve is set.
/// @param gS a pointer to the RCurve object.
/// @return @c true if the "endhead" attribute of this RCurve object is set, @c false if either the "endhead" attribute is not set or the object is @c NULL .
_SBNEExport bool ne_rc_isSetEndHead(VTransformation2D* gS);

/// @brief Returns the value of the "endhead" attribute of this RCurve object, if it has one
/// @param gS a pointer to the RCurve object.
/// @return the "endhead" attribute of this RCurve object, or an empty string if the "endhead" attribute is not set or the object is @c NULL .
_SBNEExport const std::string ne_rc_getEndHead(VTransformation2D* gS);

/// @brief Sets the value of the "endhead" attribute of this RCurve object.
/// @param gS a pointer to the RCurve object.
/// @param head a string to use as the value of the "endhead" attribute of this RCurve object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_rc_setEndHead(VTransformation2D* gS, const std::string& head);

// VText

/// @brief Predicatesreturning @c true if this VText object's "x"  RAVector object is set.
/// @param gS a pointer to the VText object.
/// @return @c true if the "x"  RAVector object of this VText object is set, @c false if either the "x"  RAVector object is not set or the object is @c NULL .
_SBNEExport bool ne_txt_isSetPositionX(VTransformation2D* gS);

/// @brief Gets the "x" RAVector object of this VText object.
/// @param gS a pointer to the VText object.
/// @return a pointer to the "x" RAVector object of this VText object.
_SBNEExport RAVector* ne_txt_getPositionX(VTransformation2D* gS);

/// @brief Sets the "x" RAVector object of this VText object.
/// @param gS a pointer to the VText object.
/// @param y a pointer to the  "x" RAVector object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_txt_setPositionX(VTransformation2D* gS, RAVector* x);

/// @brief Predicatesreturning @c true if this VText object's "y"  RAVector object is set.
/// @param gS a pointer to the VText object.
/// @return @c true if the "y"  RAVector object of this VText object is set, @c false if either the "y"  RAVector object is not set or the object is @c NULL .
_SBNEExport bool ne_txt_isSetPositionY(VTransformation2D* gS);

/// @brief Gets the "y" RAVector object of this VText object.
/// @param gS a pointer to the VText object.
/// @return a pointer to the "y" RAVector object of this VText object.
_SBNEExport RAVector* ne_txt_getPositionY(VTransformation2D* gS);

/// @brief Sets the "y" RAVector object of this VText object.
/// @param gS a pointer to the VText object.
/// @param y a pointer to the  "y" RAVector object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_txt_setPositionY(VTransformation2D* gS, RAVector* y);

/// @brief Predicatesreturning @c true if the "fontfamily" attribute of this VText is set.
/// @param gS a pointer to the VText object.
/// @return @c true if the "fontfamily" attribute of this VText object is set, @c false if either the "fontfamily" attribute is not set or the object is @c NULL .
_SBNEExport bool ne_txt_isSetFontFamily(VTransformation2D* gS);

/// @brief Returns the value of the "fontfamily" attribute of this VText object, if it has one
/// @param gS a pointer to the VText object.
/// @return the "fontfamily" attribute of this VText object, or an empty string if the "fontfamily" attribute is not set or the object is @c NULL .
_SBNEExport const std::string ne_txt_getFontFamily(VTransformation2D* gS);

/// @brief Sets the value of the "fontfamily" attribute of this VText object.
/// @param gS a pointer to the VText object.
/// @param fontfamily a string to use as the value of the "fontfamily" attribute of this VText object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_txt_setFontFamily(VTransformation2D* gS, const std::string& fontfamily);

/// @brief Predicatesreturning @c true if this VText object's "fontsize"  RAVector object is set.
/// @param gS a pointer to the VText object.
/// @return @c true if the "fontsize"  RAVector object of this VText object is set, @c false if either the "fontsize"  RAVector object is not set or the object is @c NULL .
_SBNEExport bool ne_txt_isSetFontSize(VTransformation2D* gS);

/// @brief Gets the "fontsize" RAVector object of this VText object.
/// @param gS a pointer to the VText object.
/// @return a pointer to the "fontsize" RAVector object of this VText object.
_SBNEExport RAVector* ne_txt_getFontSize(VTransformation2D* gS);

/// @brief Sets the "fontsize" RAVector object of this VText object.
/// @param gS a pointer to the VText object.
/// @param fontsize a pointer to the  "fontsize" RAVector object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_txt_setFontSize(VTransformation2D* gS, RAVector* fontsize);

/// @brief Predicatesreturning @c true if the "fontstyle" attribute of this VText is set.
/// @param gS a pointer to the VText object.
/// @return @c true if the "fontstyle" attribute of this VText object is set, @c false if either the "fontstyle" attribute is not set or the object is @c NULL .
_SBNEExport bool ne_txt_isSetFontStyle(VTransformation2D* gS);

/// @brief Returns the value of the "fontstyle" attribute of this VText object, if it has one
/// @param gS a pointer to the VText object.
/// @return the "fontstyle" attribute of this VText object, or an empty string if the "fontstyle" attribute is not set or the object is @c NULL .
_SBNEExport const std::string ne_txt_getFontStyle(VTransformation2D* gS);

/// @brief Sets the value of the "fontstyle" attribute of this VText object.
/// @param gS a pointer to the VText object.
/// @param fontstyle a string to use as the value of the "fontstyle" attribute of this VText object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_txt_setFontStyle(VTransformation2D* gS, const std::string& fontstyle);

/// @brief Predicatesreturning @c true if the "fontweight" attribute of this VText is set.
/// @param gS a pointer to the VText object.
/// @return @c true if the "fontweight" attribute of this VText object is set, @c false if either the "fontweight" attribute is not set or the object is @c NULL .
_SBNEExport bool ne_txt_isSetFontWeight(VTransformation2D* gS);

/// @brief Returns the value of the "fontweight" attribute of this VText object, if it has one
/// @param gS a pointer to the VText object.
/// @return the "fontweight" attribute of this VText object, or an empty string if the "fontweight" attribute is not set or the object is @c NULL .
_SBNEExport const std::string ne_txt_getFontWeight(VTransformation2D* gS);

/// @brief Sets the value of the "fontweight" attribute of this VText object.
/// @param gS a pointer to the VText object.
/// @param fontweight a string to use as the value of the "fontweight" attribute of this VText object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_txt_setFontWeight(VTransformation2D* gS, const std::string& fontweight);

/// @brief Predicatesreturning @c true if the "textanchor" attribute of this VText is set.
/// @param gS a pointer to the VText object.
/// @return @c true if the "textanchor" attribute of this VText object is set, @c false if either the "textanchor" attribute is not set or the object is @c NULL .
_SBNEExport bool ne_txt_isSetHTextAnchor(VTransformation2D* gS);

/// @brief Returns the value of the "textanchor" attribute of this VText object, if it has one
/// @param gS a pointer to the VText object.
/// @return the "textanchor" attribute of this VText object, or an empty string if the "textanchor" attribute is not set or the object is @c NULL .
_SBNEExport const std::string ne_txt_getHTextAnchor(VTransformation2D* gS);

/// @brief Sets the value of the "textanchor" attribute of this VText object.
/// @param gS a pointer to the VText object.
/// @param anchor a string to use as the value of the "textanchor" attribute of this VText object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_txt_setHTextAnchor(VTransformation2D* gS, const std::string& anchor);

/// @brief Predicatesreturning @c true if the "vtextanchor" attribute of this VText is set.
/// @param gS a pointer to the VText object.
/// @return @c true if the "vtextanchor" attribute of this VText object is set, @c false if either the "vtextanchor" attribute is not set or the object is @c NULL .
_SBNEExport bool ne_txt_isSetVTextAnchor(VTransformation2D* gS);

/// @brief Returns the value of the "vtextanchor" attribute of this VText object, if it has one
/// @param gS a pointer to the VText object.
/// @return the "vtextanchor" attribute of this VText object, or an empty string if the "vtextanchor" attribute is not set or the object is @c NULL .
_SBNEExport const std::string ne_txt_getVTextAnchor(VTransformation2D* gS);

/// @brief Sets the value of the "vtextanchor" attribute of this VText object.
/// @param gS a pointer to the VText object.
/// @param anchor a string to use as the value of the "vtextanchor" attribute of this VText object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_txt_setVTextAnchor(VTransformation2D* gS, const std::string& anchor);

// VRectangle

/// @brief Predicatesreturning @c true if this VRectangle object's "x"  RAVector object is set.
/// @param gS a pointer to the VRectangle object.
/// @return @c true if the "x"  RAVector object of this VRectangle object is set, @c false if either the "x"  RAVector object is not set or the object is @c NULL .
_SBNEExport bool ne_rec_isSetPositionX(VTransformation2D* gS);

/// @brief Gets the "x" RAVector object of this VRectangle object.
/// @param gS a pointer to the VRectangle object.
/// @return a pointer to the "x" RAVector object of this VRectangle object.
_SBNEExport RAVector* ne_rec_getPositionX(VTransformation2D* gS);

/// @brief Sets the "x" RAVector object of this VRectangle object.
/// @param gS a pointer to the VRectangle object.
/// @param x a pointer to the  "x" RAVector object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_rec_setPositionX(VTransformation2D* gS, RAVector* x);

/// @brief Predicatesreturning @c true if this VRectangle object's "y" RAVector object is set.
/// @param gS a pointer to the VRectangle object.
/// @return @c true if the "y"  RAVector object of this VRectangle object is set, @c false if either the "y"  RAVector object is not set or the object is @c NULL .
_SBNEExport bool ne_rec_isSetPositionY(VTransformation2D* gS);

/// @brief Gets the "y" RAVector object of this VRectangle object.
/// @param gS a pointer to the VRectangle object.
/// @return a pointer to the "y" RAVector object of this VRectangle object.
_SBNEExport RAVector* ne_rec_getPositionY(VTransformation2D* gS);

/// @brief Sets the "y" RAVector object of this VRectangle object.
/// @param gS a pointer to the VRectangle object.
/// @param y a pointer to the  "y" RAVector object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_rec_setPositionY(VTransformation2D* gS, RAVector* y);

/// @brief Predicatesreturning @c true if this VRectangle object's "width" RAVector object is set.
/// @param gS a pointer to the VRectangle object.
/// @return @c true if the "width" RAVector object of this VRectangle object is set, @c false if either the "width" RAVector object is not set or the object is @c NULL .
_SBNEExport bool ne_rec_isSetDimensionWidth(VTransformation2D* gS);

/// @brief Gets the "width" RAVector object of this VRectangle object.
/// @param gS a pointer to the VRectangle object.
/// @return a pointer to the "width" RAVector object of this VRectangle object.
_SBNEExport RAVector* ne_rec_getDimensionWidth(VTransformation2D* gS);

/// @brief Sets the "width" RAVector object of this VRectangle object.
/// @param gS a pointer to the VRectangle object.
/// @param width a pointer to the  "width" RAVector object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_rec_setDimensionWidth(VTransformation2D* gS, RAVector* width);

/// @brief Predicatesreturning @c true if this VRectangle object's "height" RAVector object is set.
/// @param gS a pointer to the VRectangle object.
/// @return @c true if the "height" RAVector object of this VRectangle object is set, @c false if either the "height" RAVector object is not set or the object is @c NULL .
_SBNEExport bool ne_rec_isSetDimensionHeight(VTransformation2D* gS);

/// @brief Gets the "height" RAVector object of this VRectangle object.
/// @param gS a pointer to the VRectangle object.
/// @return a pointer to the "height" RAVector object of this VRectangle object.
_SBNEExport RAVector* ne_rec_getDimensionHeight(VTransformation2D* gS);

/// @brief Sets the "height" RAVector object of this VRectangle object.
/// @param gS a pointer to the VRectangle object.
/// @param height a pointer to the  "height" RAVector object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_rec_setDimensionHeight(VTransformation2D* gS, RAVector* height);

/// @brief Predicatesreturning @c true if this VRectangle object's "rx" RAVector object is set.
/// @param gS a pointer to the VRectangle object.
/// @return @c true if the "rx" RAVector object of this VRectangle object is set, @c false if either the "rx" RAVector object is not set or the object is @c NULL .
_SBNEExport bool ne_rec_isSetCornerCurvatureRX(VTransformation2D* gS);

/// @brief Gets the "rx" RAVector object of this VRectangle object.
/// @param gS a pointer to the VRectangle object.
/// @return a pointer to the "rx" RAVector object of this VRectangle object.
_SBNEExport RAVector* ne_rec_getCornerCurvatureRX(VTransformation2D* gS);

/// @brief Sets the "rx" RAVector object of this VRectangle object.
/// @param gS a pointer to the VRectangle object.
/// @param rx a pointer to the  "rx" RAVector object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_rec_setCornerCurvatureRX(VTransformation2D* gS, RAVector* rx);

/// @brief Predicatesreturning @c true if this VRectangle object's "ry" RAVector object is set.
/// @param gS a pointer to the VRectangle object.
/// @return @c true if the "ry" RAVector object of this VRectangle object is set, @c false if either the "ry" RAVector object is not set or the object is @c NULL .
_SBNEExport bool ne_rec_isSetCornerCurvatureRY(VTransformation2D* gS);

/// @brief Gets the "ry" RAVector object of this VRectangle object.
/// @param gS a pointer to the VRectangle object.
/// @return a pointer to the "ry" RAVector object of this VRectangle object.
_SBNEExport RAVector* ne_rec_getCornerCurvatureRY(VTransformation2D* gS);

/// @brief Sets the "ry" RAVector object of this VRectangle object.
/// @param gS a pointer to the VRectangle object.
/// @param ry a pointer to the  "ry" RAVector object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_rec_setCornerCurvatureRY(VTransformation2D* gS, RAVector* ry);

/// @brief Predicatesreturning @c true if the "ratio" attribute of this VRectangle is set.
/// @param gS a pointer to the VRectangle object.
/// @return @c true if the "ratio" attribute of this VRectangle object is set, @c false if either the "ratio" attribute is not set or the object is @c NULL .
_SBNEExport bool ne_rec_isSetRatio(VTransformation2D* gS);

/// @brief Returns the value of the "ratio" attribute of this VRectangle object, if it has one
/// @param gS a pointer to the VRectangle object.
/// @return the "ratio" attribute of this VRectangle object, or @c 0.0 if the "ratio" attribute is not set or the object is @c NULL .
_SBNEExport const double ne_rec_getRatio(VTransformation2D* gS);

/// @brief Sets the value of the "ratio" attribute of this VRectangle object.
/// @param gS a pointer to the VRectangle object.
/// @param ratio a double value to use as the value of the "ratio" attribute of this VRectangle object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_rec_setRatio(VTransformation2D* gS, const double& ratio);

// VEllipse

/// @brief Predicatesreturning @c true if this VEllipse object's "cx" RAVector object is set.
/// @param gS a pointer to the VEllipse object.
/// @return @c true if the "cx" RAVector object of this VEllipse object is set, @c false if either the "cx" RAVector object is not set or the object is @c NULL .
_SBNEExport bool ne_elp_isSetPositionCX(VTransformation2D* gS);

/// @brief Gets the "cx" RAVector object of this VEllipse object.
/// @param gS a pointer to the VEllipse object.
/// @return a pointer to the "cx" RAVector object of this VEllipse object.
_SBNEExport RAVector* ne_elp_getPositionCX(VTransformation2D* gS);

/// @brief Sets the "cx" RAVector object of this VEllipse object.
/// @param gS a pointer to the VEllipse object.
/// @param cx a pointer to the  "cx" RAVector object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_elp_setPositionCX(VTransformation2D* gS, RAVector* cx);

/// @brief Predicatesreturning @c true if this VEllipse object's "cy" RAVector object is set.
/// @param gS a pointer to the VEllipse object.
/// @return @c true if the "cy" RAVector object of this VEllipse object is set, @c false if either the "cy" RAVector object is not set or the object is @c NULL .
_SBNEExport bool ne_elp_isSetPositionCY(VTransformation2D* gS);

/// @brief Gets the "cy" RAVector object of this VEllipse object.
/// @param gS a pointer to the VEllipse object.
/// @return a pointer to the "cy" RAVector object of this VEllipse object.
_SBNEExport RAVector* ne_elp_getPositionCY(VTransformation2D* gS);

/// @brief Sets the "cy" RAVector object of this VEllipse object.
/// @param gS a pointer to the VEllipse object.
/// @param cy a pointer to the  "cy" RAVector object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_elp_setPositionCY(VTransformation2D* gS, RAVector* cy);

/// @brief Predicatesreturning @c true if this VEllipse object's "rx" RAVector object is set.
/// @param gS a pointer to the VEllipse object.
/// @return @c true if the "rx" RAVector object of this VEllipse object is set, @c false if either the "rx" RAVector object is not set or the object is @c NULL .
_SBNEExport bool ne_elp_isSetDimensionRX(VTransformation2D* gS);

/// @brief Gets the "rx" RAVector object of this VEllipse object.
/// @param gS a pointer to the VEllipse object.
/// @return a pointer to the "rx" RAVector object of this VEllipse object.
_SBNEExport RAVector* ne_elp_getDimensionRX(VTransformation2D* gS);

/// @brief Sets the "rx" RAVector object of this VEllipse object.
/// @param gS a pointer to the VEllipse object.
/// @param rx a pointer to the  "rx" RAVector object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_elp_setDimensionRX(VTransformation2D* gS, RAVector* rx);

/// @brief Predicatesreturning @c true if this VEllipse object's "ry" RAVector object is set.
/// @param gS a pointer to the VEllipse object.
/// @return @c true if the "ry" RAVector object of this VEllipse object is set, @c false if either the "ry" RAVector object is not set or the object is @c NULL .
_SBNEExport bool ne_elp_isSetDimensionRY(VTransformation2D* gS);

/// @brief Gets the "ry" RAVector object of this VEllipse object.
/// @param gS a pointer to the VEllipse object.
/// @return a pointer to the "ry" RAVector object of this VEllipse object.
_SBNEExport RAVector* ne_elp_getDimensionRY(VTransformation2D* gS);

/// @brief Sets the "ry" RAVector object of this VEllipse object.
/// @param gS a pointer to the VEllipse object.
/// @param ry a pointer to the  "ry" RAVector object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_elp_setDimensionRY(VTransformation2D* gS, RAVector* ry);

/// @brief Predicatesreturning @c true if the "ratio" attribute of this VEllipse is set.
/// @param gS a pointer to the VEllipse object.
/// @return @c true if the "ratio" attribute of this VEllipse object is set, @c false if either the "ratio" attribute is not set or the object is @c NULL .
_SBNEExport bool ne_elp_isSetRatio(VTransformation2D* gS);

/// @brief Returns the value of the "ratio" attribute of this VEllipse object, if it has one
/// @param gS a pointer to the VEllipse object.
/// @return the "ratio" attribute of this VEllipse object, or @c 0.0 if the "ratio" attribute is not set or the object is @c NULL .
_SBNEExport const double ne_elp_getRatio(VTransformation2D* gS);

/// @brief Sets the value of the "ratio" attribute of this VEllipse object.
/// @param gS a pointer to the VEllipse object.
/// @param ratio a double value to use as the value of the "ratio" attribute of this VEllipse object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_elp_setRatio(VTransformation2D* gS, const double& ratio);

// VPolygon

/// @brief Gets the number of RenPoint objects in this VPolygon.
/// @param gS a pointer to the VPolygon object.
/// @return the number of RenPoints of this VPolygon object.
_SBNEExport const size_t ne_plg_getNumVertices(VTransformation2D* gS);

/// @brief Gets the indexth RenPoint object of this VPolygon object.
/// @param gS a pointer to the VPolygon object.
/// @param index the index of the RenPoint object to return.
/// @return a pointer to the indexth RenPoint object of this VPolygon object.
_SBNEExport RenPoint* ne_plg_getVertex(VTransformation2D* gS, const unsigned int& index);

/// @brief Creates an RenPoint object using "point" RPoint object and adds it to
/// this VPolygon object (if "basepoint1", and "basepoint2" RPoint objects are not @c NULL , an RCubicBezier object is created).
/// @param gS a pointer to the VPolygon  object.
/// @param point a pointer to the "point" RPoint object.
/// @param basepoint1 a pointer to the "controlpoint1" RPoint object.
/// @param basepoint2 a pointer to the "controlpoint2" RPoint object.
/// @param index the index in the @c listofelements where the createed RenPoint object is going to be placed
/// (placed at the of the list if @c index is not set).
/// @return a pointer to the added RenPoint object.
_SBNEExport RenPoint* ne_plg_addVertex(VTransformation2D* gS, RPoint* point = NULL, RPoint* basepoint1 = NULL, RPoint* basepoint2 = NULL, const int& index = -1);

/// @brief Removes the indexth RenPoint object from this VPolygon object
/// @param gS a pointer to the VPolygon object.
/// @param index the index of the RenPoint object to remove.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_plg_removeVertex(VTransformation2D* gS, const unsigned int& index);

// RenPoint

/// @brief Gets the "renderpoint" RAVector object of this RenPoint object.
/// @param rP a pointer to the RenPoint object.
/// @return a pointer to the "renderpoint" RAVector object of this RenPoint object.
_SBNEExport RPoint* ne_vrx_getRenderPoint(RenPoint* rP);

/// @brief Sets the "renderpoint" RAVector object of this RenPoint object.
/// @param rP a pointer to the RenPoint object.
/// @param point a pointer to the  "renderpoint" RAVector object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_vrx_setRenderPoint(RenPoint* rp, RPoint* point);

/// @brief Predicatesreturning @c true if this RenPoint object is of type RCubicBezier
/// @param l a pointer to the LLineSegment object.
/// @return @c true if this LLineSegment object is of type LCubicBezier, @c false otherwise
_SBNEExport bool ne_vrx_isRenderCubicBezier(RenPoint* rP);

// RCubicBezier

/// @brief Gets the "controlpoint1" RAVector object of this RCubicBezier object.
/// @param rP a pointer to the RCubicBezier object.
/// @return a pointer to the "controlpoint1" RAVector object of this RCubicBezier object.
_SBNEExport RPoint* ne_vrx_getBasePoint1(RenPoint* rP);

/// @brief Sets the "controlpoint1" RAVector object of this RCubicBezier object.
/// @param rP a pointer to the RCubicBezier object.
/// @param point a pointer to the  "controlpoint1" RAVector object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_vrx_setBasePoint1(RenPoint* rp, RPoint* point);

/// @brief Gets the "controlpoint2" RAVector object of this RCubicBezier object.
/// @param rP a pointer to the RCubicBezier object.
/// @return a pointer to the "controlpoint2" RAVector object of this RCubicBezier object.
_SBNEExport RPoint* ne_vrx_getBasePoint2(RenPoint* rP);

/// @brief Sets the "controlpoint2" RAVector object of this RCubicBezier object.
/// @param rP a pointer to the RCubicBezier object.
/// @param point a pointer to the  "controlpoint2" RAVector object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_vrx_setBasePoint2(RenPoint* rp, RPoint* point);

// RPoint

/// @brief Gets the "x" RAVector object of this RPoint object.
/// @param point a pointer to the RPoint object.
/// @return a pointer to the "x" RAVector object of this RPoint object.
_SBNEExport RAVector* ne_rp_getX(RPoint* point);

/// @brief Sets the "x" RAVector object of this RPoint object.
/// @param point a pointer to the RPoint object.
/// @param x a pointer to the "x" RAVector object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_rp_setX(RPoint* point, RAVector* x);

/// @brief Gets the "y" RAVector object of this RPoint object.
/// @param point a pointer to the RPoint object.
/// @return a pointer to the "y" RAVector object of this RPoint object.
_SBNEExport RAVector* ne_rp_getY(RPoint* point);

/// @brief Sets the "y" RAVector object of this RPoint object.
/// @param point a pointer to the RPoint object.
/// @param y a pointer to the "y"  RAVector object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_rp_setY(RPoint* point, RAVector* y);

// RAVector

/// @brief Returns the value of the "absolute" attribute of this RAVector object, if it has one
/// @param rAV a pointer to the RAVector object.
/// @return the "absolute" attribute of this RAVector object, or @c 0.0 if the "absolute" attribute is not set or the object is @c NULL .
_SBNEExport const double ne_rav_getAbsoluteValue(RAVector* rAV);

/// @brief Sets the value of the "absolute" attribute of this RAVector object.
/// @param rAV a pointer to the RAVector object.
/// @param abs a double value to use as the value of the "absolute" attribute of this RAVector object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_rav_setAbsoluteValue(RAVector* rAV, const double& abs);

/// @brief Returns the value of the "relative" attribute of this RAVector object, if it has one
/// @param rAV a pointer to the RAVector object.
/// @return the "relative" attribute of this RAVector object, or @c 0.0 if the "relative" attribute is not set or the object is @c NULL .
_SBNEExport const double ne_rav_getRelativeValue(RAVector* rAV);

/// @brief Sets the value of the "relative" attribute of this RAVector object.
/// @param rAV a pointer to the RAVector object.
/// @param rel a double value to use as the value of the "relative" attribute of this RAVector object.
/// @return integer value indicating success/failure of the function.
_SBNEExport int ne_rav_setRelativeValue(RAVector* rAV, const double& rel);

}

#endif
