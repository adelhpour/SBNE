#ifndef __NE_RENDER_H_
#define __NE_RENDER_H_

#include "sbne/network/ne_network.h"
#include "sbne/veneer/ne_veneer.h"
#include "sbne/sbml/ne_layout.h"

namespace sbne {

// render info
typedef struct {
    // veneer
    void* ven;
    
    // global render major version
    unsigned int majorVersion;
           
    // global render minor version
    unsigned int minorVersion;
    
    // sbml level
    unsigned int level;
    
    // sbml version
    unsigned int version;
} ne_renderInfo;

// -- Object API --

// Veneer Element

_SBNEExport bool ne_ve_isSetId(VeneerElement* vE);

_SBNEExport const std::string ne_ve_getId(VeneerElement* vE);

_SBNEExport int ne_ve_setId(Veneer* ven, VeneerElement* vE, const std::string& elementId);

_SBNEExport bool ne_ve_isSetName(VeneerElement* vE);

_SBNEExport const std::string ne_ve_getName(VeneerElement* vE);

_SBNEExport int ne_ve_setName(VeneerElement* vE, const std::string& name);

// Veneer

_SBNEExport bool ne_ven_isRenderSpecified(Veneer* ven);

_SBNEExport bool ne_ven_isSetBackgroundColor(Veneer* ven);

_SBNEExport const std::string ne_ven_getBackgroundColor(Veneer* ven);

_SBNEExport int ne_ven_setBackgroundColor(Veneer* ven, const std::string& color);

_SBNEExport const size_t ne_ven_getNumColors(Veneer* ven);

_SBNEExport VColorDefinition* ne_ven_getColor(Veneer* ven, const unsigned int& index = -1, const std::string& colorId = "");

_SBNEExport Veneer::colorVec ne_ven_getColors(Veneer* ven);

_SBNEExport VColorDefinition* ne_ven_addNewColor(Veneer* ven, std::string colorId = "", std::string value = "");

_SBNEExport const size_t ne_ven_getNumGradients(Veneer* ven);

_SBNEExport VGradientBase* ne_ven_getGradient(Veneer* ven, const unsigned int& index = -1, const std::string& gradientId = "");

_SBNEExport Veneer::gradientVec ne_ven_getGradients(Veneer* ven);

_SBNEExport const size_t ne_ven_getNumLineEndings(Veneer* ven);

_SBNEExport VLineEnding* ne_ven_getLineEnding(Veneer* ven, const unsigned int& index = -1, const std::string& lineEndingId = "");

_SBNEExport Veneer::lineEndingVec ne_ven_getLineEndings(Veneer* ven);

_SBNEExport VLineEnding* ne_ven_addNewLineEnding(Veneer* ven,  RxnRoleType role = NUM_RxnRoleType, std::string lineEndingId = "");

_SBNEExport VLineEnding* ne_ven_getLocalLineEndingFromGlobalLineEnding(Veneer* ven, VLineEnding* gLineEnding);

_SBNEExport const size_t ne_ven_getNumStyles(Veneer* ven);

_SBNEExport VGlobalStyle* ne_ven_getStyle(Veneer* ven, const unsigned int& index);

_SBNEExport Veneer::styleVec ne_ven_getStyles(Veneer* ven);

_SBNEExport VGlobalStyle* ne_ven_addNewGlobalStyle(Veneer* ven, StyleType type = NUM_StyleType, RxnRoleType role = NUM_RxnRoleType, std::string styleId = "");

_SBNEExport VLocalStyle* ne_ven_addNewLocalStyle(Veneer* ven, std::string styleId = "");

_SBNEExport VGlobalStyle* ne_ven_findStyle(Veneer* ven, NGraphicalObject* gO = NULL, StyleType type = NUM_StyleType);

_SBNEExport VLocalStyle* ne_ven_getLocalStyleFromGlobalStyle(Veneer* ven, NGraphicalObject* gO, VGlobalStyle* gStyle);

// Color

_SBNEExport bool ne_clr_isSetValue(VColorDefinition* c);

_SBNEExport const std::string ne_clr_getValue(VColorDefinition* c);

_SBNEExport int ne_clr_setValue(VColorDefinition* c, const std::string& value);

// Gradient

_SBNEExport bool ne_grd_isSetSpreadMethod(VGradientBase* g);

_SBNEExport const std::string ne_grd_getSpreadMethod(VGradientBase* g);

_SBNEExport const size_t ne_grd_getNumStops(VGradientBase* g);

_SBNEExport VGradientStop* ne_grd_getStop(VGradientBase* g, const unsigned int& index);

_SBNEExport bool ne_grd_isLinearGradient(VGradientBase* g);

_SBNEExport bool ne_grd_isRadialGradient(VGradientBase* g);

_SBNEExport RAVector* ne_grd_getX1(VGradientBase* g);

_SBNEExport RAVector* ne_grd_getY1(VGradientBase* g);

_SBNEExport RAVector* ne_grd_getX2(VGradientBase* g);

_SBNEExport RAVector* ne_grd_getY2(VGradientBase* g);

_SBNEExport RAVector* ne_grd_getCx(VGradientBase* g);

_SBNEExport RAVector* ne_grd_getCy(VGradientBase* g);

_SBNEExport RAVector* ne_grd_getFx(VGradientBase* g);

_SBNEExport RAVector* ne_grd_getFy(VGradientBase* g);

_SBNEExport RAVector* ne_grd_getR(VGradientBase* g);

// Gradient Stop

_SBNEExport bool ne_gstp_isSetOffset(VGradientStop* s);

_SBNEExport RAVector* ne_gstp_getOffset(VGradientStop* s);

_SBNEExport bool ne_gstp_isSetColor(VGradientStop* s);

_SBNEExport const std::string ne_gstp_getColor(VGradientStop* s);

// Line Ending

_SBNEExport int ne_le_setLineEndingValues(Veneer* ven, VLineEnding* lE, RxnRoleType role);

_SBNEExport bool ne_le_isSetBoundingBox(VLineEnding* lE);

_SBNEExport LBox* ne_le_getBoundingBox(VLineEnding* lE);

_SBNEExport int ne_le_setBoundingBox(VLineEnding* lE, LBox* b);

_SBNEExport bool ne_le_isSetGroup(VLineEnding* lE);

_SBNEExport VRenderGroup* ne_le_getGroup(VLineEnding* lE);

_SBNEExport int ne_le_setGroup(VLineEnding* lE, VRenderGroup* g);

_SBNEExport bool ne_le_isSetEnableRotation(VLineEnding* lE);

_SBNEExport bool ne_le_getEnableRotation(VLineEnding* lE);

_SBNEExport int ne_le_enableRotation(VLineEnding* lE, const bool& enable);

// Style

_SBNEExport int ne_stl_setStyleValues(VGlobalStyle* s, StyleType type = NUM_StyleType, RxnRoleType role = NUM_RxnRoleType);

_SBNEExport bool ne_stl_isSetGroup(VGlobalStyle* s);

_SBNEExport VRenderGroup* ne_stl_getGroup(VGlobalStyle* s);

_SBNEExport int ne_stl_setGroup(VGlobalStyle* s, VRenderGroup* g);

_SBNEExport int ne_stl_addToTypeList(VGlobalStyle* s, const std::string& type);

_SBNEExport int ne_stl_addToRoleList(VGlobalStyle* s, const std::string& role);

_SBNEExport int ne_stl_addToIdList(VGlobalStyle* s, const std::string& gOId);

// Render Group

_SBNEExport bool ne_grp_isSetStrokeColor(VRenderGroup* g);

_SBNEExport const std::string ne_grp_getStrokeColor(VRenderGroup* g);

_SBNEExport int ne_grp_setStrokeColor(VRenderGroup* g, const std::string& color);

_SBNEExport bool ne_grp_isSetStrokeWidth(VRenderGroup* g);

_SBNEExport const double ne_grp_getStrokeWidth(VRenderGroup* g);

_SBNEExport int ne_grp_setStrokeWidth(VRenderGroup* g, const double& width);

_SBNEExport bool ne_grp_isSetStrokeDashArray(VRenderGroup* g);

_SBNEExport const size_t ne_grp_getNumStrokeDashes(VRenderGroup* g);

_SBNEExport const int ne_grp_getStrokeDash(VRenderGroup* g, const unsigned int& index);

_SBNEExport VGraphicalPrimitive1D::dashArrayVec* ne_grp_getStrokeDashArray(VRenderGroup* g);

_SBNEExport int ne_grp_setStrokeDashArray(VRenderGroup* g, VGraphicalPrimitive1D::dashArrayVec* dashArray);

_SBNEExport bool ne_grp_isSetFillColor(VRenderGroup* g);

_SBNEExport const std::string ne_grp_getFillColor(VRenderGroup* g);

_SBNEExport int ne_grp_setFillColor(VRenderGroup* g, const std::string& color);

_SBNEExport bool ne_grp_isSetFillRule(VRenderGroup* g);

_SBNEExport const std::string ne_grp_getFillRule(VRenderGroup* g);

_SBNEExport int ne_grp_setFillRule(VRenderGroup* g, const std::string& fillRule);

_SBNEExport bool ne_grp_isSetFontFamily(VRenderGroup* g);

_SBNEExport const std::string ne_grp_getFontFamily(VRenderGroup* g);

_SBNEExport int ne_grp_setFontFamily(VRenderGroup* g, const std::string& fontFamily);

_SBNEExport bool ne_grp_isSetFontSize(VRenderGroup* g);

_SBNEExport RAVector* ne_grp_getFontSize(VRenderGroup* g);

_SBNEExport int ne_grp_setFontSize(VRenderGroup* g, RAVector* fontSize);

_SBNEExport bool ne_grp_isSetFontStyle(VRenderGroup* g);

_SBNEExport const std::string ne_grp_getFontStyle(VRenderGroup* g);

_SBNEExport int ne_grp_setFontStyle(VRenderGroup* g, const std::string& fontStyle);

_SBNEExport bool ne_grp_isSetFontWeight(VRenderGroup* g);

_SBNEExport const std::string ne_grp_getFontWeight(VRenderGroup* g);

_SBNEExport int ne_grp_setFontWeight(VRenderGroup* g, const std::string& fontWeight);

_SBNEExport bool ne_grp_isSetHTextAnchor(VRenderGroup* g);

_SBNEExport const std::string ne_grp_getHTextAnchor(VRenderGroup* g);

_SBNEExport int ne_grp_setHTextAnchor(VRenderGroup* g, const std::string& anchor);

_SBNEExport bool ne_grp_isSetVTextAnchor(VRenderGroup* g);

_SBNEExport const std::string ne_grp_getVTextAnchor(VRenderGroup* g);

_SBNEExport int ne_grp_setVTextAnchor(VRenderGroup* g, const std::string& anchor);

_SBNEExport bool ne_grp_isSetStartHead(VRenderGroup* g);

_SBNEExport const std::string ne_grp_getStartHead(VRenderGroup* g);

_SBNEExport int ne_grp_setStartHead(VRenderGroup* g, const std::string& head);

_SBNEExport bool ne_grp_isSetEndHead(VRenderGroup* g);

_SBNEExport const std::string ne_grp_getEndHead(VRenderGroup* g);

_SBNEExport int ne_grp_setEndHead(VRenderGroup* g, const std::string& head);

_SBNEExport const size_t ne_grp_getNumGeometricShapes(VRenderGroup* g);

_SBNEExport VTransformation2D* ne_grp_getGeometricShape(VRenderGroup* g, const unsigned int& index);

_SBNEExport int ne_grp_removeGeometricShapes(VRenderGroup* g);

_SBNEExport VTransformation2D* ne_grp_addGeometricShape(VRenderGroup* g, const RenderGroupElementShape rGEShape, const bool& forLineEnding = false, const int& numberOfPolygonVertices = -1, const std::string& imageHref = "");

// Geometric Shape

_SBNEExport const RenderGroupElementShape ne_gs_getShape(VTransformation2D* gS);

_SBNEExport int ne_gs_setShapeFeatures(VTransformation2D* gs, const int& numberOfPolygonVertices, const std::string& imageHref, const bool& forLineEnding);

_SBNEExport bool ne_gs_isSetStrokeColor(VTransformation2D* gS);

_SBNEExport const std::string ne_gs_getStrokeColor(VTransformation2D* gS);

_SBNEExport int ne_gs_setStrokeColor(VTransformation2D* gS, const std::string& color);

_SBNEExport bool ne_gs_isSetStrokeWidth(VTransformation2D* gS);

_SBNEExport const double ne_gs_getStrokeWidth(VTransformation2D* gS);

_SBNEExport int ne_gs_setStrokeWidth(VTransformation2D* gS, const double& width);

_SBNEExport bool ne_gs_isSetStrokeDashArray(VTransformation2D* gS);

_SBNEExport const size_t ne_gs_getNumStrokeDashes(VTransformation2D* gS);

_SBNEExport const int ne_gs_getStrokeDash(VTransformation2D* gS, const unsigned int& index);

_SBNEExport VGraphicalPrimitive1D::dashArrayVec* ne_gs_getStrokeDashArray(VTransformation2D* gS);

_SBNEExport int ne_gs_setStrokeDashArray(VTransformation2D* gS, VGraphicalPrimitive1D::dashArrayVec* dashArray);

_SBNEExport bool ne_gs_isSetFillColor(VTransformation2D* gS);

_SBNEExport const std::string ne_gs_getFillColor(VTransformation2D* gS);

_SBNEExport int ne_gs_setFillColor(VTransformation2D* gS, const std::string& color);

_SBNEExport bool ne_gs_isSetFillRule(VTransformation2D* gS);

_SBNEExport const std::string ne_gs_getFillRule(VTransformation2D* gS);

_SBNEExport int ne_gs_setFillRule(VTransformation2D* gS, const std::string& fillRule);

// Image

_SBNEExport bool ne_img_isSetPositionX(VTransformation2D* gS);

_SBNEExport RAVector* ne_img_getPositionX(VTransformation2D* gS);

_SBNEExport int ne_img_setPositionX(VTransformation2D* gS, RAVector* x);

_SBNEExport bool ne_img_isSetPositionY(VTransformation2D* gS);

_SBNEExport RAVector* ne_img_getPositionY(VTransformation2D* gS);

_SBNEExport int ne_img_setPositionY(VTransformation2D* gS, RAVector* y);

_SBNEExport bool ne_img_isSetDimensionWidth(VTransformation2D* gS);

_SBNEExport RAVector* ne_img_getDimensionWidth(VTransformation2D* gS);

_SBNEExport int ne_img_setDimensionWidth(VTransformation2D* gS, RAVector* width);

_SBNEExport bool ne_img_isSetDimensionHeight(VTransformation2D* gS);

_SBNEExport RAVector* ne_img_getDimensionHeight(VTransformation2D* gS);

_SBNEExport int ne_img_setDimensionHeight(VTransformation2D* gS, RAVector* height);

_SBNEExport bool ne_img_isSetHref(VTransformation2D* gS);

_SBNEExport const std::string ne_img_getHref(VTransformation2D* gS);

_SBNEExport int ne_img_setHref(VTransformation2D* gS, const std::string& href);

// Render Curve

_SBNEExport const size_t ne_rc_getNumVertices(VTransformation2D* gS);

_SBNEExport RenPoint* ne_rc_getVertex(VTransformation2D* gS, const unsigned int& index);

_SBNEExport RenPoint* ne_rc_addVertex(VTransformation2D* gS, RPoint* point = NULL, RPoint* basePoint1 = NULL, RPoint* basePoint2 = NULL, const int& index = -1);

_SBNEExport int ne_rc_removeVertex(VTransformation2D* gS, const unsigned int& index);

_SBNEExport bool ne_rc_isSetStartHead(VTransformation2D* gS);

_SBNEExport const std::string ne_rc_getStartHead(VTransformation2D* gS);

_SBNEExport int ne_rc_setStartHead(VTransformation2D* gS, const std::string& head);

_SBNEExport bool ne_rc_isSetEndHead(VTransformation2D* gS);

_SBNEExport const std::string ne_rc_getEndHead(VTransformation2D* gS);

_SBNEExport int ne_rc_setEndHead(VTransformation2D* gS, const std::string& head);

// Text

_SBNEExport bool ne_txt_isSetPositionX(VTransformation2D* gS);

_SBNEExport RAVector* ne_txt_getPositionX(VTransformation2D* gS);

_SBNEExport int ne_txt_setPositionX(VTransformation2D* gS, RAVector* x);

_SBNEExport bool ne_txt_isSetPositionY(VTransformation2D* gS);

_SBNEExport RAVector* ne_txt_getPositionY(VTransformation2D* gS);

_SBNEExport int ne_txt_setPositionY(VTransformation2D* gS, RAVector* y);

_SBNEExport bool ne_txt_isSetFontFamily(VTransformation2D* gS);

_SBNEExport const std::string ne_txt_getFontFamily(VTransformation2D* gS);

_SBNEExport int ne_txt_setFontFamily(VTransformation2D* gS, const std::string& fontFamily);

_SBNEExport bool ne_txt_isSetFontSize(VTransformation2D* gS);

_SBNEExport RAVector* ne_txt_getFontSize(VTransformation2D* gS);

_SBNEExport int ne_txt_setFontSize(VTransformation2D* gS, RAVector* fontSize);

_SBNEExport bool ne_txt_isSetFontStyle(VTransformation2D* gS);

_SBNEExport const std::string ne_txt_getFontStyle(VTransformation2D* gS);

_SBNEExport int ne_txt_setFontStyle(VTransformation2D* gS, const std::string& fontStyle);

_SBNEExport bool ne_txt_isSetFontWeight(VTransformation2D* gS);

_SBNEExport const std::string ne_txt_getFontWeight(VTransformation2D* gS);

_SBNEExport int ne_txt_setFontWeight(VTransformation2D* gS, const std::string& fontWeight);

_SBNEExport bool ne_txt_isSetHTextAnchor(VTransformation2D* gS);

_SBNEExport const std::string ne_txt_getHTextAnchor(VTransformation2D* gS);

_SBNEExport int ne_txt_setHTextAnchor(VTransformation2D* gS, const std::string& anchor);

_SBNEExport bool ne_txt_isSetVTextAnchor(VTransformation2D* gS);

_SBNEExport const std::string ne_txt_getVTextAnchor(VTransformation2D* gS);

_SBNEExport int ne_txt_setVTextAnchor(VTransformation2D* gS, const std::string& anchor);

// Rectangle

_SBNEExport bool ne_rec_isSetPositionX(VTransformation2D* gS);

_SBNEExport RAVector* ne_rec_getPositionX(VTransformation2D* gS);

_SBNEExport int ne_rec_setPositionX(VTransformation2D* gS, RAVector* x);

_SBNEExport bool ne_rec_isSetPositionY(VTransformation2D* gS);

_SBNEExport RAVector* ne_rec_getPositionY(VTransformation2D* gS);

_SBNEExport int ne_rec_setPositionY(VTransformation2D* gS, RAVector* y);

_SBNEExport bool ne_rec_isSetDimensionWidth(VTransformation2D* gS);

_SBNEExport RAVector* ne_rec_getDimensionWidth(VTransformation2D* gS);

_SBNEExport int ne_rec_setDimensionWidth(VTransformation2D* gS, RAVector* width);

_SBNEExport bool ne_rec_isSetDimensionHeight(VTransformation2D* gS);

_SBNEExport RAVector* ne_rec_getDimensionHeight(VTransformation2D* gS);

_SBNEExport int ne_rec_setDimensionHeight(VTransformation2D* gS, RAVector* height);

_SBNEExport bool ne_rec_isSetCornerCurvatureRX(VTransformation2D* gS);

_SBNEExport RAVector* ne_rec_getCornerCurvatureRX(VTransformation2D* gS);

_SBNEExport int ne_rec_setCornerCurvatureRX(VTransformation2D* gS, RAVector* rx);

_SBNEExport bool ne_rec_isSetCornerCurvatureRY(VTransformation2D* gS);

_SBNEExport RAVector* ne_rec_getCornerCurvatureRY(VTransformation2D* gS);

_SBNEExport int ne_rec_setCornerCurvatureRY(VTransformation2D* gS, RAVector* ry);

_SBNEExport bool ne_rec_isSetRatio(VTransformation2D* gS);

_SBNEExport const double ne_rec_getRatio(VTransformation2D* gS);

_SBNEExport int ne_rec_setRatio(VTransformation2D* gS, const double& ratio);

// Ellipse

_SBNEExport bool ne_elp_isSetPositionCX(VTransformation2D* gS);

_SBNEExport RAVector* ne_elp_getPositionCX(VTransformation2D* gS);

_SBNEExport int ne_elp_setPositionCX(VTransformation2D* gS, RAVector* cx);

_SBNEExport bool ne_elp_isSetPositionCY(VTransformation2D* gS);

_SBNEExport RAVector* ne_elp_getPositionCY(VTransformation2D* gS);

_SBNEExport int ne_elp_setPositionCY(VTransformation2D* gS, RAVector* cy);

_SBNEExport bool ne_elp_isSetDimensionRX(VTransformation2D* gS);

_SBNEExport RAVector* ne_elp_getDimensionRX(VTransformation2D* gS);

_SBNEExport int ne_elp_setDimensionRX(VTransformation2D* gS, RAVector* rx);

_SBNEExport bool ne_elp_isSetDimensionRY(VTransformation2D* gS);

_SBNEExport RAVector* ne_elp_getDimensionRY(VTransformation2D* gS);

_SBNEExport int ne_elp_setDimensionRY(VTransformation2D* gS, RAVector* ry);

_SBNEExport bool ne_elp_isSetRatio(VTransformation2D* gS);

_SBNEExport const double ne_elp_getRatio(VTransformation2D* gS);

_SBNEExport int ne_elp_setRatio(VTransformation2D* gS, const double& ratio);

// Polygon

_SBNEExport const size_t ne_plg_getNumVertices(VTransformation2D* gS);

_SBNEExport RenPoint* ne_plg_getVertex(VTransformation2D* gS, const unsigned int& index);

_SBNEExport RenPoint* ne_plg_addVertex(VTransformation2D* gS, RPoint* point = NULL, RPoint* basePoint1 = NULL, RPoint* basePoint2 = NULL, const int& index = -1);

_SBNEExport int ne_plg_removeVertex(VTransformation2D* gS, const unsigned int& index);

// Vertex

_SBNEExport RPoint* ne_vrx_getRenderPoint(RenPoint* rP);

_SBNEExport int ne_vrx_setRenderPoint(RenPoint* rp, RPoint* point);

_SBNEExport bool ne_vrx_isRenderCubicBezier(RenPoint* rP);

_SBNEExport RPoint* ne_vrx_getBasePoint1(RenPoint* rP);

_SBNEExport int ne_vrx_setBasePoint1(RenPoint* rp, RPoint* point);

_SBNEExport RPoint* ne_vrx_getBasePoint2(RenPoint* rP);

_SBNEExport int ne_vrx_setBasePoint2(RenPoint* rp, RPoint* point);

// Render Point

_SBNEExport RAVector* ne_rp_getX(RPoint* rP);

 _SBNEExport int ne_rp_setX(RPoint* rP, RAVector* x);

_SBNEExport RAVector* ne_rp_getY(RPoint* rP);

 _SBNEExport int ne_rp_setY(RPoint* rP, RAVector* y);

// RelAbsVector

_SBNEExport const double ne_rav_getAbsoluteValue(RAVector* rAV);

_SBNEExport const double ne_rav_getRelativeValue(RAVector* rAV);

_SBNEExport int ne_rav_setAbsoluteValue(RAVector* rAV, const double& abs);

_SBNEExport int ne_rav_setRelativeValue(RAVector* rAV, const double& rel);

// -- Input --

_SBNEExport ne_renderInfo* ne_doc_processRenderInfo(SBMLDocument* doc);

_SBNEExport int ne_ri_setVeneer(ne_renderInfo* r, Veneer* ven);

_SBNEExport Veneer* ne_ri_getVeneer(ne_renderInfo* r);

_SBNEExport Veneer* ne_ri_addDefaultRenderFeaturesToVeneer(ne_renderInfo* r);

// -- Output --

_SBNEExport SBMLDocument* ne_doc_populateSBMLdocWithRenderInfo(SBMLDocument* doc, ne_renderInfo* r);

_SBNEExport RenderGroup* ne_grp_populateRenderGroup(VRenderGroup* grp);

}

#endif
