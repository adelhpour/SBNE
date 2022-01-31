#ifndef __NE_VENEER_H_
#define __NE_VENEER_H_

#ifndef SWIG
#include "sbml/SBMLTypes.h"
#include "sbml/packages/render/common/RenderExtensionTypes.h"
#endif

#include "sbne/ne_core.h"
#include "sbne/layout/ne_network.h"
#include "sbne/render/ne_transform.h"
#include "sbne/render/ne_relAbsVector.h"
#include "sbne/render/ne_renderPoint.h"

namespace sbne {

/// @enum StyleType
/// @brief Enumeration of values permitted as the value of the items of "typelist" attribute of a VGlobalStyle object.
typedef enum {
    ST_TYPE_COMP, /**< COMPARTMENTGLYPH */
    ST_TYPE_SPC, /**< SPECIESGLYPH */
    ST_TYPE_RXN, /**< REACTIONGLYPH */
    ST_TYPE_SPC_REF, /**< SPECIESREFERENCEGLYPH */
    ST_TYPE_TXT, /**< TEXTGLYPH */
    ST_TYPE_GNR, /**< GENERALGLYPH */
    ST_TYPE_GRP_OBJ, /**< GRAPHICALOBJECT */
    ST_TYPE_ANY, /**< ANY */
    NUM_StyleType /**< Number of StyleTypes */
} StyleType;

/// @enum ColorString
/// @brief Enumeration of values whose converted to string equivalent is permitted as the "value" attribute of a
/// VColorDefinition object.
typedef enum {
    CLR_STR_WHITE, /**< White */
    CLR_STR_BLACK, /**< Black */
    CLR_STR_RED, /**< Red */
    CLR_STR_DARK_RED, /**< DarkRed */
    CLR_STR_GREEN, /**< Green */
    CLR_STR_DARK_GREEN, /**< DarkGreen */
    CLR_STR_BLUE, /**< Blue */
    CLR_STR_DARK_BLUE, /**< DarkBlue */
    CLR_STR_CYAN, /**< Cyan */
    CLR_STR_DARK_CYAN, /**< DarkCyan */
    CLR_STR_MAGENTA, /**< Magenta */
    CLR_STR_DARK_MAGENTA, /**< DarkMagenta */
    CLR_STR_YELLOW, /**< Yellow */
    CLR_STR_DARK_YELLOW, /**< DarkYellow */
    CLR_STR_GRAY, /**< Gray */
    CLR_STR_DARK_GRAY, /**< DarkGray */
    CLR_STR_LIGHT_GRAY, /**< LightGray */
    CLR_STR_NONE, /**< none */
    NUM_ColorString /**<  Number of ColorStrings */
} ColorString;

/// @enum GradientSpreadMethod
/// @brief Enumeration of values permitted as the value of "spreadmethod" attribute of a VGradientBase object.
typedef enum {
    GRD_SPR_MTH_PAD, /**< pad */
    GRD_SPR_MTH_RFL, /**< reflect */
    GRD_SPR_MTH_RPT, /**< repeat */
    NUM_GradientSpreadMethod /**< Number of GradientSpreadMethods */
} GradientSpreadMethod;

/// @enum FillRule
/// @brief Enumeration of values permitted as the value of "fill-rule" attribute of a VTransformation2D object.
typedef enum {
    FIL_RUL_NON_ZER, /**< nonzero */
    FIL_RUL_EVN_ODD, /**< evenodd */
    NUM_FillRule /**< Number of FillRules */
} FillRule;

/// @enum FontFamily
/// @brief Enumeration of values permitted as the value of "font-family" attribute of a VTransformation2D object.
typedef enum {
    FNT_FMLY_SRF, /**< serif */
    FNT_FMLY_SNS_SRF, /**< sans-serif */
    FNT_FMLY_MNO_SPC, /**< monospace */
    NUM_FontFamily /**< Number of FontFamilies */
} FontFamily;

/// @enum FontWeight
/// @brief Enumeration of values permitted as the value of "font-weight" attribute of a VTransformation2D object.
typedef enum {
    FNT_WGHT_BLD, /**< bold */
    FNT_WGHT_NRM, /**< normal */
    NUM_FontWeight /**< FontWeights */
} FontWeight;

/// @enum FontStyle
/// @brief Enumeration of values permitted as the value of "font-style" attribute of a VTransformation2D object.
typedef enum {
    FNT_STL_ITLC, /**< italic */
    FNT_STL_NRM, /**< normal */
    NUM_FontStyle /**< Number of FontStyles */
} FontStyle;

/// @enum HTextAnchor
/// @brief Enumeration of values permitted as the value of "text-anchor" attribute of a VTransformation2D object.
typedef enum {
    HTXT_ANCH_STRT, /**< start*/
    HTXT_ANCH_MDL, /**< middle */
    HTXT_ANCH_END, /**< end */
    NUM_HTextAnchor /**< Number of HTextAnchors */
} HTextAnchor;

/// @enum VTextAnchor
/// @brief Enumeration of values permitted as the value of "vtext-anchor" attribute of a VTransformation2D object.
typedef enum {
    VTXT_ANCH_TOP, /**< top */
    VTXT_ANCH_MDL, /**< middle*/
    VTXT_ANCH_BTM, /**< bottom */
    VTXT_ANCH_BLNE, /**< baseline */
    NUM_VTextAnchor /**< Number of VTextAnchors */
} VTextAnchor;

/// @enum RenderGroupElementShape
/// @brief Enumeration of values permitted as the value of "shape"attribute of a VTransformation2D object.
typedef enum {
    GRP_ELT_SHPE_IMG, /**< image */
    GRP_ELT_SHPE_CRV, /**< rendercurve */
    GRP_ELT_SHPE_TXT, /**< text */
    GRP_ELT_SHPE_REC, /**< rectangle */
    GRP_ELT_SHPE_ELP, /**< ellipse */
    GRP_ELT_SHPE_PLG, /**< polygon */
    NUM_RenderGroupElementShape /**< Number of RenderGroupElementShapes */
} RenderGroupElementShape;

/// @brief Predicates returning @c true if the two entered strings are the same (non-case-sensitive).
/// @param str1 a string to use as the first string.
/// @param str2 a string to use as the second string.
/// @return @c true if the two entered strings are the same, otherwise @c false .
bool stringCompare(const std::string& str1, const std::string& str2);

/// @brief Gets the string equivalent of this StyleType.
/// @param s a StyleType.
/// @return the string equivalent of this StyleType.
std::string styleTypeToStr(const StyleType s);

/// @brief Gets the StyleType associated with this GrphObjType.
/// @param g a GrphObjType.
/// @return the StyleType associated with this GrphObjType.
const StyleType styleTypeForGraphicalObjectType(const GrphObjType g);

/// @brief Gets the string equivalent of this ColorString.
/// @param c a ColorString.
/// @return the string equivalent of this ColorString.
std::string colorStringToStr(const ColorString c);

/// @brief Gets the hexadecimal string equivalent of this ColorString.
/// @param c a ColorString.
/// @return the  hexadecimal string equivalent of this ColorString.
std::string colorStringToHexStr(const ColorString c);

/// @brief Gets the string equivalent of this GradientSpreadMethod.
/// @param g a GradientSpreadMethod.
/// @return the string equivalent of this GradientSpreadMethod.
std::string gSMethodToStr(const GradientSpreadMethod g);

/// @brief Gets the string equivalent of this FillRule.
/// @param f a FillRule.
/// @return the string equivalent of this FillRule.
std::string fillRuleToStr(const FillRule f);

/// @brief Gets the string equivalent of this FontFamily.
/// @param f a FontFamily.
/// @return the string equivalent of this FontFamily.
std::string fontFamilyToStr(const FontFamily f);

/// @brief Gets the string equivalent of this FontWeight.
/// @param f a FontWeight.
/// @return the string equivalent of this FontWeight.
std::string fontWeightToStr(const FontWeight f);

/// @brief Gets the string equivalent of this FontStyle.
/// @param f a FontStyle.
/// @return the string equivalent of this FontStyle.
std::string fontStyleToStr(const FontStyle f);

/// @brief Gets the string equivalent of this HTextAnchor.
/// @param h a HTextAnchor.
/// @return the string equivalent of this HTextAnchor.
std::string hTextAnchorToStr(const HTextAnchor h);

/// @brief Gets the string equivalent of this VTextAnchor.
/// @param v a VTextAnchor.
/// @return the string equivalent of this VTextAnchor.
std::string vTextAnchorToStr(const VTextAnchor v);

/// @brief Gets the string equivalent of this RenderGroupElementShape.
/// @param s a RenderGroupElementShape.
/// @return the string equivalent of this RenderGroupElementShape.
std::string rGroupElementToStr(const RenderGroupElementShape s);

// declarae the veneer classes
class VeneerElement;
class Veneer;
class VColorDefinition;
class VGradientBase;
class VLinearGradient;
class VRadialGradient;
class VTransformation2D;
class VGraphicalPrimitive1D;
class VGraphicalPrimitive2D;
class VImage;
class RCurve;
class VText;
class VRectangle;
class VEllipse;
class VPolygon;
class VRenderGroup;
class VGlobalStyle;
class VLocalStyle;
class VLineEnding;
class VGradientStop;
class RenPoint;
class RCubicBezier;

/** @brief Construct the surface veneer from the given render information
* @param[in] ren render information
* @returns a pointer to the created veneer
* @details The veneer will be initialized with the
* configuration specified by the render info of the model
*/
Veneer* veneerFromRender(RenderInformationBase& ren, Veneer* ven);

/** @brief Construct an rgroup object from the given render group
* @param[in] rg render group
* @returns a pointer to the created rgroup
* @details The rgroup will be initialized with the
* configuration specified by the render group
*/
VRenderGroup* getRenderGroup(const RenderGroup* rg);

/** @brief Construct a RenPoint from the given render point
* @param[in] rp render point
* @returns a pointer to the created renpoint
* @details The renpoint will be initialized with the
* configuration specified by the render point
*/
RenPoint* getRenderPoint(const RenderPoint* rp);

/** @brief Construct a RCubicBezier from the given render cubic bezier
* @param[in] rcb render cubic bezier
* @returns a pointer to the created rcubicbezier
* @details The rcubicbezier will be initialized with the
* configuration specified by the render cubic bezier
*/
RCubicBezier* getRenderCubicBezier(const RenderCubicBezier* rcb);

/// @class VeneerElement
/// @brief Base class for Veneer objects.
class VeneerElement {
public:
    /// Constructors
    VeneerElement() {
        _isSetId = false;
        _isSetName = false;
        _isSetMetaId = false;
    }
    
    VeneerElement(const VeneerElement& vE);

    /// Functions
    // set the id of veneer element
    void setId(const std::string& id);

    // get the id of veneer element
    const std::string& getId() const;

    // show whether the id of veneer element is set
    const bool isSetId() const { return _isSetId; }

    // set the name of veneer element
    void setName(const std::string& name);

    // get the name of veneer element
    const std::string& getName() const;

    // show whether the name of veneer element is set
    const bool isSetName() const { return _isSetName; }
    
    // set the metaid of veneer element
    void setMetaId(const std::string& metaid);

    // get the metaid of veneer element
    const std::string& getMetaId() const;

    // show whether the metaid of veneer element is set
    const bool isSetMetaId() const { return _isSetMetaId; }
    
protected:
    // model info:
    std::string _id;
    std::string _name;
    std::string _metaid;
    bool _isSetId;
    bool _isSetName;
    bool _isSetMetaId;
};

/// @class Veneer
/// @brief The equivalent class for SBML [GlobalRenderInformation](http://model.caltech.edu/software/libsbml/5.18.0/docs/formatted/cpp-api/class_global_render_information.html) and [LocalRenderInformation](http://model.caltech.edu/software/libsbml/5.18.0/docs/formatted/cpp-api/class_local_render_information.html) .
class Veneer : public VeneerElement {
public:
    /// Constructors
    Veneer() {
        _colors.clear();
        _gradients.clear();
        _styles.clear();
        _lineEndings.clear();
        _isSetBackgroundColor = false;
        _isRenderSpecified = false;
    }
    
    Veneer(const Veneer& v);

    /// Containers
    // color
    typedef std::vector<VColorDefinition*> colorVec;

    // gradient
    typedef std::vector<VGradientBase*> gradientVec;

    // style
    typedef std::vector<VGlobalStyle*> styleVec;

    // line ending
    typedef std::vector<VLineEnding*> lineEndingVec;

    /// Iterators
    // color
    typedef colorVec::iterator colorIt;
    typedef colorVec::const_iterator constColorIt;

    // gradient
    typedef gradientVec::iterator gradientIt;
    typedef gradientVec::const_iterator constGradientIt;

    // style
    typedef styleVec::iterator styleIt;
    typedef styleVec::const_iterator constStyleIt;

    // line ending
    typedef lineEndingVec::iterator lineEndingIt;
    typedef lineEndingVec::const_iterator constLineEndingIt;

    /// Functions related to beginning and end of containers
    // color
    const constColorIt colorsBegin() const { return _colors.begin(); }
    const constColorIt colorsEnd() const { return _colors.end(); }

    // gradient
    const constGradientIt gradientsBegin() const { return _gradients.begin(); }
    const constGradientIt gradientsEnd() const { return _gradients.end(); }

    // style
    const constStyleIt stylesBegin() const { return _styles.begin(); }
    const constStyleIt stylesEnd() const { return _styles.end(); }

    // line ending
    const constLineEndingIt lineEndingsBegin() const { return _lineEndings.begin(); }
    const constLineEndingIt lineEndingsEnd() const { return _lineEndings.end(); }

    /// Functions
    // add a new color to veneer
    void addColor(VColorDefinition* c);

    // remove a color from veneer using its index
    void removeColor(unsigned int colorIndex);

    // set colorVec
    void setColors(const colorVec& cv);

    // get colorVec
    const colorVec& getColors() const;

    // get total number of colors
    const size_t getNumColors() const;

    // find a color by its specified Id. Returns NULL if no such color exists
    VColorDefinition* findColorById(const std::string& id);

    // find the index of a color by its specified Id. Returns -1 if no such color exists
    unsigned int findColorIndexById(const std::string& id);

    // generate unique Id for for a new color
    std::string getColorUniqueId();

    /// Functions related to gradients
    // add a new gradient to veneer
    void addGradient(VGradientBase* g);

    // remove a gradient from veneer
    void removeGradient(unsigned int gradientIndex);

    // set the vector of gradients
    void setGradients(const gradientVec& gv);

    // get the vector of gradients
    const gradientVec getGradients() const;

    // get total number of gradients
    const size_t getNumGradients() const;

    // find a gradient by its specified Id. Returns NULL if no such gradient exists
    VGradientBase* findGradientById(const std::string& id);

    // find the index of a gradient by its specified Id. Returns -1 if no such gradient exists
    unsigned int findGradientIndexById(const std::string& id);

    // generate unique Id for a new gradient
    std::string getGradientUniqueId();

    /// Functions related to styles
    // add a new style to veneer
    void addStyle(VGlobalStyle* s);

    // remove a style from veneer
    void removeStyle(unsigned int styleIndex);

    // set the vector of styles
    void setStyles(const styleVec& sv);

    // get the vector of styles
    const styleVec& getStyles() const;

    // get total number of styles
    const size_t getNumStyles() const;

    // find a style by its specified Id. Returns NULL if no such style exists
    VGlobalStyle* findStyleById(const std::string& id);
    
    // find the index of a style by its specified Id. Returns -1 if no such style exists
    unsigned int findStyleIndexById(const std::string& id);

    // find a style by a role list member assigned to it. Returns NULL if no such style exists
    VGlobalStyle* findStyleByRoleListMember(const std::string& role);
    
    // find the index of a style by a role list member assigned to it. Returns -1 if no such style exists
    unsigned int findStyleIndexByRoleListMember(const std::string& role);

    // find a style by a type list member assigned to it. Returns NULL if no such style exists
    VGlobalStyle* findStyleByTypeListMember(const std::string& type);
    
    // find the index of a style by a type list member assigned to it. Returns -1 if no such style exists
    unsigned int findStyleIndexByTypeListMember(const std::string& type);

    // find a style by an id list member assigned to it. Returns NULL if no such style exists
    VLocalStyle* findStyleByIdListMember(const std::string& id);
    
    // find the index of a style by an id list member assigned to it. Returns -1 if no such style exists
    unsigned int findStyleIndexByIdListMember(const std::string& id);

    // generate unique Id for a new style
    std::string getStyleUniqueId();

    /// Functions related to line endings
    // add a new line ending to veneer
    void addLineEnding(VLineEnding* l);

    // remove a line ending from the veneer
    void removeLineEnding(unsigned int lineEndingIndex);

    // set the vector of line enidings
    void setLineEndings(const lineEndingVec& lv);

    // get the vector of line endings
    const lineEndingVec& getLineEndings() const;

    // get total number of line endsings
    const size_t getNumLineEndings() const;

    // find a line ending by its specified Id. Returns NULL if no such line ending exists
    VLineEnding* findLineEndingById(const std::string& id);

    // find the index of a line ending by its specified Id. Returns -1 if no such lineending exists
    unsigned int findLineEndingIndexById(const std::string& id);

    // generate unique Id for a new line ending
    std::string getLineEndingUniqueId();
    
    // generate unique Id for a new line ending using an old line ending id
    std::string getLineEndingUniqueId(const std::string& lineEndingId);
    
    // set the background color of veneer element
    void setBackgroundColor(const std::string& bColor);
    
    // get the background color of veneer element
    const std::string& getBackgroundColor() const;

    // show whether the background color of veneer element is set
    const bool isSetBackgroundColor() const { return _isSetBackgroundColor; }

    // show whether the render is specified (set)
    const bool isRenderSpecified() const { return _isRenderSpecified; }
        
    // set whether the render is specified for veneer
    void setRenderSpecified(bool value);

protected:
    // model info:
    colorVec _colors;
    gradientVec _gradients;
    styleVec _styles;
    lineEndingVec _lineEndings;
    std::string _backgroundColor;
    bool _isSetBackgroundColor;
    bool _isRenderSpecified;
};

/// @class VColorDefinition
/// @brief The equivalent class for SBML [ColorDefinition](http://model.caltech.edu/software/libsbml/5.18.0/docs/formatted/cpp-api/class_color_definition.html) .
class VColorDefinition : public VeneerElement {
public:
    /// Constructors
    VColorDefinition() {
        _value = colorStringToHexStr(CLR_STR_NONE);
        _isSetValue = false;
    }
    
    VColorDefinition(const VColorDefinition& c);

    /// Functions
    // set the value of color
    void setValue(const std::string& value);

    // get the value of color
    const std::string& getValue() const;

    // show whether the value of the color is set
    const bool isSetValue() const { return _isSetValue; }

protected:
    // model info:
    std::string _value;
    bool _isSetValue;
};

/// @class VGradientBase
/// @brief The equivalent class for SBML [GradientBase](http://model.caltech.edu/software/libsbml/5.18.0/docs/formatted/cpp-api/class_gradient_base.html) .
class VGradientBase : public VeneerElement {
public:
    /// Constructors
    VGradientBase() {
        _stops.clear();
        _spreadMethod = gSMethodToStr(GRD_SPR_MTH_PAD);
        _isSetSpreadMethod = false;
    }
    
    VGradientBase(const VGradientBase& g);

    /// Containers
    // gradient stop
    typedef std::vector<VGradientStop*> stopVec;

    /// Iterators
    // gradient stop
    typedef stopVec::iterator stopIt;
    typedef stopVec::const_iterator constStopIt;

    /// Functions related to beginning and end of containers
    // gradient stop
    const constStopIt stopsBegin() const { return _stops.begin(); }
    const constStopIt stopsEnd() const { return _stops.end(); }

    /// Functions
    // set the spread method of gradient
    void setSpreadMehtod(const std::string& spreadMethod);

    // get the spreadmethod of gradient
    const std::string& getSpreadMethod() const;

    // show whether the spreadmethod of gradient is set
    const bool isSetSpreadMethod() const { return _isSetSpreadMethod; }

    // add a new gradient stop to gradient
    void addToStops(VGradientStop* stop);

    // remove a stop from gradient
    void removeStop(unsigned int stopIndex);
    
    // set the vector of stops
    void setStops(const stopVec& sv);

    // get the vector of stops
    const stopVec& getStops() const;
    
    // get total number of stops
    const size_t getNumStops() const;

    // find a gradient stop by its specified Id. Returns NULL if no such stop exists
    VGradientStop* findStopById(const std::string& id);
    
    // find the index of a gradient stop by its specified Id. Returns -1 if no such stop exists
    unsigned int findStopIndexById(const std::string& id);

    // find a gradient stop by its specified stop color. Returns NULL if no such stop exists
    VGradientStop* findStopByColor(const std::string& sColor);

    // find the index of a gradient stop by its specified stop color. Returns -1 if no such stop exists
    unsigned int findStopIndexByColor(const std::string& sColor);

    // generate unique Id for a new gradient stop
    std::string getStopUniqueId();
    
    // sort the stops based on their offset
    void sortStops();

#ifndef SWIG
    // show whether it is a linear gradient
    const virtual bool isSetAsLinearGradient() const { return false; };

    // show whether it is a radial gradient
    const virtual bool isSetAsRadialGradient() const { return false; };
#endif

protected:
    // model info:
    stopVec _stops;
    std::string _spreadMethod;
    bool _isSetSpreadMethod;
};

/// @class VLinearGradient
/// @brief The equivalent class for SBML [LinearGradient](http://model.caltech.edu/software/libsbml/5.18.0/docs/formatted/cpp-api/class_linear_gradient.html) .
class VLinearGradient : public VGradientBase {
public:
    /// Constructors
    VLinearGradient() {
        _x1 = RAVector(0.0, 0.0);
        _y1 = RAVector(0.0, 0.0);
        _z1 = RAVector(0.0, 0.0);
        _x2 = RAVector(0.0, 100.0);
        _y2 = RAVector(0.0, 100.0);
        _z2 = RAVector(0.0, 100.0);
        _isSetX1 = false;
        _isSetY1 = false;
        _isSetZ1 = false;
        _isSetX2 = false;
        _isSetY2 = false;
        _isSetZ2 = false;
    }
    
    VLinearGradient(const VLinearGradient& lG);

    /// Functions
    // set the x1 value of linear gradient
    void setX1(const RAVector& r);

    // get the x1 value of linear gradient
    const RAVector& getX1() const;

    // show whether the x1 value of linear gradient is set
    const bool isSetX1() const { return _isSetX1; }

    // set the the y1 value of linear gradient
    void setY1(const RAVector& r);

    // get the y1 value of linear gradient
    const RAVector& getY1() const;

    // show whether the y1 value of linear gradient is set
    const bool isSetY1() const { return _isSetY1; }
    
    // set the z1 value of linear gradient
    void setZ1(const RAVector& r);

    // get the z1 value of linear gradient
    const RAVector& getZ1() const;

    // show whether the z1 value of linear gradient is set
    const bool isSetZ1() const { return _isSetZ1; }
    
    // set the x2 value of linear gradient
    void setX2(const RAVector& r);

    // get the x2 value of linear gradient
    const RAVector& getX2() const;

    // show whether the x2 value of linear gradient is set
    const bool isSetX2() const { return _isSetX2; }
    
    // set the y2 value of linear gradient
    void setY2(const RAVector& r);

    // get the y2 value of linear gradient
    const RAVector& getY2() const;

    // show whether the y2 value of linear gradient is set
    const bool isSetY2() const { return _isSetY2; }
    
    // set the z2 value of linear gradient
    void setZ2(const RAVector& r);

    // get the z2 value of linear gradient
    const RAVector& getZ2() const;

    // show whether the z2 value of linear gradient is set
    const bool isSetZ2() const { return _isSetZ2; }
#ifndef SWIG
    // show whether it is a linear gradient
    const virtual bool isSetAsLinearGradient() const { return true; };
    
    // show whether it is a radial gradient
    const virtual bool isSetAsRadialGradient() const { return false; };
#endif

protected:
    // model info:
    RAVector _x1;
    RAVector _y1;
    RAVector _z1;
    RAVector _x2;
    RAVector _y2;
    RAVector _z2;
    bool _isSetX1;
    bool _isSetY1;
    bool _isSetZ1;
    bool _isSetX2;
    bool _isSetY2;
    bool _isSetZ2;
};

/// @class VRadialGradient
/// @brief The equivalent class for SBML [RadialGradient](http://model.caltech.edu/software/libsbml/5.18.0/docs/formatted/cpp-api/class_radial_gradient.html) .
class VRadialGradient : public VGradientBase {
public:

    /// Constructors
    VRadialGradient() {
        _cx = RAVector(0.0, 50.0);
        _cy = RAVector(0.0, 50.0);
        _cz = RAVector(0.0, 50.0);
        _fx = RAVector(0.0, 50.0);
        _fy = RAVector(0.0, 50.0);
        _fz = RAVector(0.0, 50.0);
        _r = RAVector(0.0, 50.0);
        _isSetCx = false;
        _isSetCy = false;
        _isSetCz = false;
        _isSetFx = false;
        _isSetFy = false;
        _isSetFz = false;
        _isSetR = false;
    }
    
    VRadialGradient(const VRadialGradient& rG);

    /// Functions
    // set the cx value of radial gradient
    void setCx(const RAVector& r );

    // get the cx value of radial gradient
    const RAVector& getCx() const;

    // show whether the cx value of radial gradient is set
    const bool isSetCx() const { return _isSetCx; }

    // set the cy value of radial gradient
    void setCy(const RAVector& r);

    // get the cy value of radial gradient
    const RAVector& getCy() const;

    // show whether the cy value of radial gradient is set
    const bool isSetCy() const { return _isSetCy; }

    // set the cz value of radial gradient
    void setCz(const RAVector& r);

    // get the cz value of radial gradient
    const RAVector& getCz() const;

    // show whether the cz value of radial gradient is set
    const bool isSetCz() const { return _isSetCz; }

    // set the fx value of radial gradient
    void setFx(const RAVector& r);

    // get the fx value of radial gradient
    const RAVector& getFx() const;

    // show whether the fx value of radial gradient is set
    const bool isSetFx() const { return _isSetFx; }

    // set the fy value of radial gradient
    void setFy(const RAVector& r);

    // get the fy value of radial gradient
    const RAVector& getFy() const;

    // show whether the fy value of radial gradient is set
    const bool isSetFy() const { return _isSetFy; }

    // set the fz value of radial gradient
    void setFz(const RAVector& r);

    // get the fz value of radial gradient
    const RAVector& getFz() const;

    // show whether the fz value of radial gradient is set
    const bool isSetFz() const { return _isSetFz; }

    // set the r value of radial gradient
    void setR(const RAVector& r);

    // get the r value of radial gradient
    const RAVector& getR() const;

    // show whether the r value of radial gradient is set
    const bool isSetR() const { return _isSetR; }
#ifndef SWIG
    // show whether it is a linear gradient
    const virtual bool isSetAsLinearGradient() const { return false; };
           
    // show whether it is a radial gradient
    const virtual bool isSetAsRadialGradient() const { return true; };
#endif

protected:
    // model info:
    RAVector _cx;
    RAVector _cy;
    RAVector _cz;
    RAVector _fx;
    RAVector _fy;
    RAVector _fz;
    RAVector _r;
    bool _isSetCx;
    bool _isSetCy;
    bool _isSetCz;
    bool _isSetFx;
    bool _isSetFy;
    bool _isSetFz;
    bool _isSetR;
};

/// @class VTransformation2D
/// @brief The equivalent class for SBML [Transformation2D](http://model.caltech.edu/software/libsbml/5.18.0/docs/formatted/cpp-api/class_transformation2_d.html) .
class VTransformation2D : public VeneerElement {
public:
    /// Constructors
    VTransformation2D() {
        setTransform(1.0, 0.0, 0.0, 0.0, 1.0, 0.0);
        _isSetTransform = false;
    }
    
    VTransformation2D(const VTransformation2D& t2d);

    /// Functions
    // get the shape type of render gorup element as render group element shape
    RenderGroupElementShape getShape();

    // get the shape type of render gorup element as string
    std::string getShapeAsString();
    
    // set the transform value of transformation2d using its elements
    void setTransform(const double& a, const double& b, const double& c, const double& d, const double& e, const double& f);
    
    // set the transform value of transformation2d using an array of elements
    void setTransform(const double m[6]);
    
    // update the transform value of transformation2d when its elements are changed
    void updateTransform(unsigned int index, const double& value);
    
    // update the transform value of transformation2d using values of affine2d elements
    void updateTransform();
    
    // get the transform value of transformation2d
    const std::string& getTransform() const;
    
    // get the tansform value of transformation2d as an array
    const void getTransform(double* m) const;

    // show whether transform of transformation2d is set
    const bool isSetTransform() const { return _isSetTransform; }
    
    // set the affined2d of transformation2d using its elements
    void setAffine2D(const double& a, const double& b, const double& c, const double& d, const double& e, const double& f);
    
    // update the affine2d of transformation2d when its elements are changed
    void updateAffine2D(const int& r, const int& c, const double& value);
    
    // get the transform value of transformation2d
    const Affine2d& getAffine2D() const;
    
    // translate the transform
    void translate(const double& tx, const double& ty);
    
    // scale the transform
    void scale(const double& sx, const double& sy);
    
    // rotate the transform
    void rotate(const double& alpha);
    
    // skew the transform along the x-axis
    void skewX(const double& alpha);
    
    // skew the transform along the y-axis
    void skewY(const double& beta);

protected:
    // model info:
    RenderGroupElementShape _shape;
    std::string _transform;
    Affine2d _affine;
    bool _isSetTransform;
};

/// @class VGraphicalPrimitive1D
/// @brief The equivalent class for SBML [GraphicalPrimitive1D](http://model.caltech.edu/software/libsbml/5.18.0/docs/formatted/cpp-api/class_graphical_primitive1_d.html) .
class VGraphicalPrimitive1D : public VTransformation2D {
public:
    /// Constructors
    VGraphicalPrimitive1D() {
        _stroke = colorStringToStr(CLR_STR_NONE);
        _strokeWidth = 0.0;
        _dashArray.clear();
        _isSetStroke = false;
        _isSetStrokeWidth = false;
        _isSetDashArray = false;
    }
    
    VGraphicalPrimitive1D(const VGraphicalPrimitive1D& g1d);

    /// Containers
    // dasharray
    typedef std::vector<unsigned int> dashArrayVec;

    /// Iterators
    // dasharray
    typedef dashArrayVec::iterator dashArrayIt;
    typedef dashArrayVec::const_iterator constDashArrayIt;

    /// Functions related to the beginning and end of containers
    // dasharray
    const constDashArrayIt dashArrayBegin() const { return _dashArray.begin(); }
    const constDashArrayIt dashArrayEnd() const { return _dashArray.end(); }

    /// Functions
    // set the stroke value of gprimitive1d
    void setStroke(const std::string& stroke);

    // get the stroke value of gprimitive1d
    const std::string& getStroke() const;

    // show whether the stroke value of gprimitive1d is set
    const bool isSetStroke() const { return _isSetStroke; }

    // set the stroke width value of gprimitive1d
    void setStrokeWidth(const double& sWidth);

    // get the stroke width value of gprimitive1d
    const double& getStrokeWidth() const;

    // show whether the stroke width value of gprimitive1d is set
    const bool isSetStrokeWidth() const { return _isSetStrokeWidth; }

    // add an element to the end of dash array
    void addToDashArrayEnd(const unsigned int& element);

    // add an element to the dash array
    void addToDashArray(const unsigned int& element, unsigned int index);

    // replace the value of an element in the dash array
    void replaceElementInDashArray(const unsigned int& element, unsigned int index);

    // remove an element from the dash array
    void removeElementFromDashArray(unsigned int index);
    
    // set the dash array value of gprimitive1d
    void setDashArray(const dashArrayVec& dashArray);

    // get the value of dash array as an array
    const dashArrayVec& getDashArray() const;

    // show whether dash array value of gprimitive1d is set
    const bool isSetDashArray() const { return _isSetDashArray; }
    
    static dashArrayVec* stringToDashArray(const std::string& str);
    
    static const std::string dashArrayToString(dashArrayVec* dav);

protected:
    // model info:
    std::string _stroke;
    double _strokeWidth;
    dashArrayVec _dashArray;
    bool _isSetStroke;
    bool _isSetStrokeWidth;
    bool _isSetDashArray;
};

/// @class VGraphicalPrimitive2D
/// @brief The equivalent class for SBML [GraphicalPrimitive2D](http://model.caltech.edu/software/libsbml/5.18.0/docs/formatted/cpp-api/class_graphical_primitive2_d.html) .
class VGraphicalPrimitive2D : public VGraphicalPrimitive1D {
public:

    /// Constructors
    VGraphicalPrimitive2D() {
        _fill = colorStringToStr(CLR_STR_NONE);
        _fillRule = fillRuleToStr(FIL_RUL_NON_ZER);
        _isSetFill = false;
        _isSetFillRule = false;
    }
    
    VGraphicalPrimitive2D(const VGraphicalPrimitive2D& g2d);

    /// Functions
    // set the fill value of gprimitive2d
    void setFill(const std::string& fill);

    // get the fill value of gprimitive2d
    const std::string& getFill() const;

    // show whether the fill value of gprimitive2d is set
    const bool isSetFill() const { return _isSetFill; }

    // set the fill rule value of gprimitive2d
    void setFillRule(const std::string& fillRule);

    // get the fill rule value of gprimitive2d
    const std::string& getFillRule() const;

    // show whether the fill rule value of gprimitive2d is set
    const bool isSetFillRule() const { return _isSetFillRule; }

protected:
    // model info:
    std::string _fill;
    std::string _fillRule;
    bool _isSetFill;
    bool _isSetFillRule;
};

/// @class VImage
/// @brief The equivalent class for SBML [Image](http://model.caltech.edu/software/libsbml/5.18.0/docs/formatted/cpp-api/class_image.html) .
class VImage : public VTransformation2D {
public:
    /// Constructors
    VImage() {
        _shape = GRP_ELT_SHPE_IMG;
        _isSetX = false;
        _isSetY = false;
        _isSetZ = false;
        _isSetWidth = false;
        _isSetHeight = false;
        _isSetHref = false;
    }
    
    VImage(const VImage& image);

    /// Functions
    // set the x value of image
    void setX(const RAVector& x);

    // get the x value of image
    const RAVector& getX() const;

    // show whether the x value of image is set
    const bool isSetX() const { return _isSetX; }

    // set the y value of image
    void setY(const RAVector& y);

    // get the y value of image
    const RAVector& getY() const;

    // show whether the y value of image is set
    const bool isSetY() const { return _isSetY; }

    // set the z value of image
    void setZ(const RAVector& z);

    // get the z value of image
    const RAVector& getZ() const;

    // show whether the z value of image is set
    const bool isSetZ() const { return _isSetZ; }

    // set the width value of image
    void setWidth(const RAVector& width);

    // get the width value of image
    const RAVector& getWidth() const;

    // show whether the width value of image is set
    const bool isSetWidth() const { return _isSetWidth; }

    // set the height value of image
    void setHeight(const RAVector& height);

    // get the height value of image
    const RAVector& getHeight() const;

    // show whether the height value of image is set
    const bool isSetHeight() const { return _isSetHeight; }

    // set the href value of image
    void setHref(const std::string& href);

    // get the href value of image
    const std::string& getHref() const;

    // show whether the href value of image is set
    const bool isSetHref() const { return _isSetHref; }

protected:
    // model info:
    RAVector _x;
    RAVector _y;
    RAVector _z;
    RAVector _width;
    RAVector _height;
    std::string _href;
    bool _isSetX;
    bool _isSetY;
    bool _isSetZ;
    bool _isSetWidth;
    bool _isSetHeight;
    bool _isSetHref;
};

/// @class RCurve
/// @brief The equivalent class for SBML [RenderCurve](http://model.caltech.edu/software/libsbml/5.18.0/docs/formatted/cpp-api/class_render_curve.html) .
class RCurve : public VGraphicalPrimitive1D {
public:
    /// Constructors
    RCurve() {
        _shape = GRP_ELT_SHPE_CRV;
        _listOfElements.clear();
        _isSetStartHead = false;
        _isSetEndHead = false;
    }
    
    RCurve(const RCurve& rCurve);

    /// Containers
    // element
    typedef std::vector<RenPoint*> listOfElements;

    /// Iterators
    // element
    typedef listOfElements::iterator elementIt;
    typedef listOfElements::const_iterator constElementIt;

    /// Functions related to beginning and end of containers
    // element
    const constElementIt elementsBegin() const { return _listOfElements.begin(); }
    const constElementIt elementsEnd() const { return _listOfElements.end(); }

    /// Functions
    // set the start head value of render curve
    void setStartHead(const std::string& startHead);

    // get the start head value of render curve
    const std::string& getStartHead() const;

    // show whether the start head value of rende curve is set
    const bool isSetStartHead() const { return _isSetStartHead; }

    // set the end head value of rende rcurve
    void setEndHead(const std::string& endHead);

    // get the end head value of rende rcurve
    const std::string& getEndHead() const;

    // show whether the end head value of rende curve is set
    const bool isSetEndHead() const { return _isSetEndHead; }

    // add a render point or render cubic bezier to the end of list of elements
    void addToListOfElementsEnd(RenPoint* rp);

    // add a render point or render cubic bezier to the list of elements of render curve
    void addToListOfElements(RenPoint* rp, unsigned int index);

    // remove an element from the list of elements of render curve
    void removeFromListOfElements(unsigned int index);

    // remove all elements from the list of elements of render curve
    void clearListOfElements();

    // get the list of elements of render curve
    const listOfElements& getListOfElements() const;

    // get total number of elements
    const size_t getNumElements() const;

    // find an element by its specified Id. Returns NULL if no such element exists
    RenPoint* findElementById(const std::string& id);

    // find the index of an element by its specified Id. Returns -1 if no such element exists
    unsigned int findElementIndexById(const std::string& id);

    // generate unique ID for a new element
    std::string getElementUniqueId();

protected:
    // model info:
    std::string _startHead;
    std::string _endHead;
    listOfElements _listOfElements;
    bool _isSetStartHead;
    bool _isSetEndHead;
};

/// @class VText
/// @brief The equivalent class for SBML [Text](http://model.caltech.edu/software/libsbml/5.18.0/docs/formatted/cpp-api/class_text.html) .
class VText : public VGraphicalPrimitive1D {
public:

    /// Constructors
    VText() {
        _shape = GRP_ELT_SHPE_TXT;
        _fontFamily = fontFamilyToStr(FNT_FMLY_SNS_SRF);
        _fontWeight = fontWeightToStr(FNT_WGHT_NRM);
        _fontStyle = fontStyleToStr(FNT_STL_NRM);
        _hTextAnchor = hTextAnchorToStr(HTXT_ANCH_STRT);
        _vTextAnchor = vTextAnchorToStr(VTXT_ANCH_TOP);
        _isSetX = false;
        _isSetY = false;
        _isSetZ = false;
        _isSetFontSize = false;
        _isSetFontFamily = false;
        _isSetFontWeight = false;
        _isSetFontStyle = false;
        _isSetHTextAnchor = false;
        _isSetVTextAnchor = false;
    }
    
    VText(const VText& text);

    /// Functions
    // set the x value of text
    void setX(const RAVector& x);

    // get the x value of text
    const RAVector& getX() const;

    // show whether the x value of text is set
    const bool isSetX() const { return _isSetX; }

    // set the y value of text
    void setY(const RAVector& y);

    // get the y value of text
    const RAVector& getY() const;

    // show whether the y value of text is set
    const bool isSetY() const { return _isSetY; }

    // set the z value of text
    void setZ(const RAVector& z);

    // get the z value of text
    const RAVector& getZ() const;

    // show whether the z value of text is set
    const bool isSetZ() const { return _isSetZ; }

    // set the font size value of text
    void setFontSize(const RAVector& fSize);

    // get the font size value of text
    const RAVector& getFontSize() const;

    // show whether the font size value of text is set
    const bool isSetFontSize() const { return _isSetFontSize; }

    // set the font family of text
    void setFontFamily(const std::string& fFamily);

    // get the font family of text
    const std::string& getFontFamily() const;

    // show whether the font family of text is set
    const bool isSetFontFamily() const { return _isSetFontFamily; }

    // set the font weight of text
    void setFontWeight(const std::string& fWeight);

    // get the font weight of text
    const std::string& getFontWeight() const;

    // show whether the font weight of text is set
    const bool isSetFontWeight() const { return _isSetFontWeight; }

    // set the font style of text
    void setFontStyle(const std::string& fStyle);

    // get the font style of text
    const std::string& getFontStyle() const;

    // show whether the font style of text is set
    const bool isSetFontStyle() const { return _isSetFontStyle; }

    // set the horizontal text anchor of text
    void setHTextAnchor(const std::string& hTAnchor);

    // get the horizontal text anchor of text
    const std::string& getHTextAnchor() const;

    // show whether the horizontal text anchor of text is set
    const bool isSetHTextAnchor() const { return _isSetHTextAnchor; }

    // set the vertical text anchor of text
    void setVTextAnchor(const std::string& vTAnchor);

    // get the vertical text anchor of text
    const std::string& getVTextAnchor() const;

    // show whether the vertical text anchor of text is set
    const bool isSetVTextAnchor() const { return _isSetVTextAnchor; }

protected:
    // model info:
    RAVector _x;
    RAVector _y;
    RAVector _z;
    RAVector _fontSize;
    std::string _fontFamily;
    std::string _fontWeight;
    std::string _fontStyle;
    std::string _hTextAnchor;
    std::string _vTextAnchor;
    bool _isSetX;
    bool _isSetY;
    bool _isSetZ;
    bool _isSetFontSize;
    bool _isSetFontFamily;
    bool _isSetFontWeight;
    bool _isSetFontStyle;
    bool _isSetHTextAnchor;
    bool _isSetVTextAnchor;
};

/// @class VRectangle
/// @brief The equivalent class for SBML [Rectangle](http://model.caltech.edu/software/libsbml/5.18.0/docs/formatted/cpp-api/class_rectangle.html) .
class VRectangle : public VGraphicalPrimitive2D {
public:
    /// Constructors
    VRectangle() {
        _shape = GRP_ELT_SHPE_REC;
        _ratio = 0.0;
        _isSetX = false;
        _isSetY = false;
        _isSetZ = false;
        _isSetWidth = false;
        _isSetHeight = false;
        _isSetRX = false;
        _isSetRY = false;
        _isSetRatio = false;
    }
    
    VRectangle(const VRectangle& rectangle);

    /// Functions
    // set the x value of rectangle
    void setX(const RAVector& x);

    // get the x value of rectangle
    const RAVector& getX() const;

    // show whether the x value of rectangle is set
    const bool isSetX() const { return _isSetX; }

    // set the y value of rectangle
    void setY(const RAVector& y);

    // get the y value of rectangle
    const RAVector& getY() const;

    // show whether the y value of rectangle is set
    const bool isSetY() const { return _isSetY; }

    // set the z value of rectangle
    void setZ(const RAVector& z);

    // get the z value of rectangle
    const RAVector& getZ() const;

    // show whether the z value of rectangle is set
    const bool isSetZ() const { return _isSetZ; }

    // set the width value of rectangle
    void setWidth(const RAVector& width);

    // get the width value of rectangle
    const RAVector& getWidth() const;

    // show whether the width value of rectangle is set
    const bool isSetWidth() const { return _isSetWidth; }

    // set the height value of rectangle
    void setHeight(const RAVector& height);

    // get the height value of rectangle
    const RAVector& getHeight() const;

    // show whether the height value of rectangle is set
    const bool isSetHeight() const { return _isSetHeight; }

    // set the rx value of rectangle
    void setRX(const RAVector& rx);

    // get the rx value of rectangle
    const RAVector& getRX() const;

    // show whether the rx value of rectangle is set
    const bool isSetRX() const { return _isSetRX; }

    // set the ry value of rectangle
    void setRY(const RAVector& ry);

    // get the ry value of rectangle
    const RAVector& getRY() const;

    // show whether the ry value of rectangle is set
    const bool isSetRY() const { return _isSetRY; }

    // set the ratio value of rectangle
    void setRatio(const double& ratio);
    
    // unset the ratio value of rectangle
    void unSetRatio();

    // get the ratio value of rectangle
    const double& getRatio() const;

    // show whether the ratio value of rectangle is set
    const bool isSetRatio() const { return _isSetRatio; }

protected:
    // model info:
    RAVector _x;
    RAVector _y;
    RAVector _z;
    RAVector _width;
    RAVector _height;
    RAVector _rx;
    RAVector _ry;
    double _ratio;
    bool _isSetX;
    bool _isSetY;
    bool _isSetZ;
    bool _isSetWidth;
    bool _isSetHeight;
    bool _isSetRX;
    bool _isSetRY;
    bool _isSetRatio;
};

/// @class VEllipse
/// @brief The equivalent class for SBML [Ellipse](http://model.caltech.edu/software/libsbml/5.18.0/docs/formatted/cpp-api/class_ellipse.html) .
class VEllipse : public VGraphicalPrimitive2D {
public:

    /// Constructors
    VEllipse() {
        _shape = GRP_ELT_SHPE_ELP;
        _ratio = 0.0;
        _isSetCX = false;
        _isSetCY = false;
        _isSetCZ = false;
        _isSetRX = false;
        _isSetRY = false;
        _isSetRatio = false;
    }
    
    VEllipse(const VEllipse& ellipse);

    /// Functions
    // set the cx value of ellipse
    void setCX(const RAVector& cx);

    // get the cx value of ellipse
    const RAVector& getCX() const;

    // show whether the cx value of ellipse is set
    const bool isSetCX() const { return _isSetCX; }

    // set the cy value of ellipse
    void setCY(const RAVector& cy);

    // get the cy value of ellipse
    const RAVector& getCY() const;

    // show whether the cy value of ellipse is set
    const bool isSetCY() const { return _isSetCY; }

    // set the cz value of ellipse
    void setCZ(const RAVector& cz);

    // get the cz value of ellipse
    const RAVector& getCZ() const;

    // show whether the cz value of ellipse is set
    const bool isSetCZ() const { return _isSetCZ; }

    // set the rx value of ellipse
    void setRX(const RAVector& rx);

    // get the rx value of ellipse
    const RAVector& getRX() const;

    // show whether the rx value of ellipse is set
    const bool isSetRX() const { return _isSetRX; }

    // set the ry value of ellipse
    void setRY(const RAVector& ry);

    // get the ry value of ellipse
    const RAVector& getRY() const;

    // show whether the ry value of ellipse is set
    const bool isSetRY() const { return _isSetRY; }

    // set the ratio value of ellipse
    void setRatio(const double& ratio);
    
    // unset the ratio value of ellipse
    void unSetRatio();

    // get the ratio value of ellipse
    const double& getRatio() const;

    // show whether the ratio value of ellipse is set
    const bool isSetRatio() const { return _isSetRatio; }

protected:
    // model info:
    RAVector _cx;
    RAVector _cy;
    RAVector _cz;
    RAVector _rx;
    RAVector _ry;
    double _ratio;
    bool _isSetCX;
    bool _isSetCY;
    bool _isSetCZ;
    bool _isSetRX;
    bool _isSetRY;
    bool _isSetRatio;
};

/// @class VPolygon
/// @brief The equivalent class for SBML [Polygon](http://model.caltech.edu/software/libsbml/5.18.0/docs/formatted/cpp-api/class_polygon.html) .
class VPolygon : public VGraphicalPrimitive2D {
public:
    /// Constructors
    VPolygon() {
        _shape = GRP_ELT_SHPE_PLG;
        _listOfElements.clear();
    }
    
    VPolygon(const VPolygon& polygon);

    /// Containers
    // element
    typedef std::vector<RenPoint*> listOfElements;

    /// Iterators
    // element
    typedef listOfElements::iterator elementIt;
    typedef listOfElements::const_iterator constElementIt;

    /// Functions related to beginning and end of containers
    // element
    const constElementIt elementsBegin() const { return _listOfElements.begin(); }
    const constElementIt elementsEnd() const { return _listOfElements.end(); }

    /// Functions
    // add a render point to the end of list of elements
    void addToListOfElementsEnd(RenPoint* rp);

    // add a render point to list of elements
    void addToListOfElements(RenPoint* rp, unsigned int index);

    // remove an element from list of elements
    void removeFromListOfElements(unsigned int index);

    // remove all elements from list of elements
    void clearListOfElements();

    // get the list of elements
    const listOfElements& getListOfElements() const;

    // get total number of elements
    const size_t getNumElements() const;

    // find an element by its specified Id. Returns NULL if no such element exists
    RenPoint* findElementById(const std::string& id);

    // find the index of an element by its specified Id. Returns -1 if no such element exists
    unsigned int findElementIndexById(const std::string& id);

    // generate unique Id for a new element
    std::string getElementUniqueId();

protected:
    // model info:
    listOfElements _listOfElements;
};

/// @class VRenderGroup
/// @brief The equivalent class for SBML [RenderGroup](http://model.caltech.edu/software/libsbml/5.18.0/docs/formatted/cpp-api/class_render_group.html) .
class VRenderGroup : public VGraphicalPrimitive2D {
public:
    /// Constructors
    VRenderGroup() {
        _fontFamily = fontFamilyToStr(FNT_FMLY_SNS_SRF);
        _fontWeight = fontWeightToStr(FNT_WGHT_NRM);
        _fontStyle = fontStyleToStr(FNT_STL_NRM);
        _hTextAnchor = hTextAnchorToStr(HTXT_ANCH_STRT);
        _vTextAnchor = vTextAnchorToStr(VTXT_ANCH_TOP);
        _listOfElements.clear();
        _isSetStartHead = false;
        _isSetEndHead = false;
        _isSetFontSize = false;
        _isSetFontFamily = false;
        _isSetFontWeight = false;
        _isSetFontStyle = false;
        _isSetHTextAnchor = false;
        _isSetVTextAnchor = false;
        _isSetListOfElements = false;
    }
    
    VRenderGroup(const VRenderGroup& g);

    /// Containers
    // group element
    typedef std::vector<VTransformation2D*> listOfElements;

    /// Iterators
    // group element
    typedef listOfElements::iterator elementIt;
    typedef listOfElements::const_iterator constElementIt;

    /// Functions related to beginning and end of containers
    // group element
    const constElementIt elementsBegin() const { return _listOfElements.begin(); }
    const constElementIt elementsEnd() const { return _listOfElements.end(); }

    /// Functions
    // set the start head of render group
    void setStartHead(const std::string& startHead);
    
    // unset the start head of render group
    void unSetStartHead();

    // get the start head of render group
    const std::string& getStartHead() const;

    // show whether the start head of render group is set
    const bool isSetStartHead() const { return _isSetStartHead; }

    // set the end head of render group
    void setEndHead(const std::string& endHead);
    
    // unset the end head of render group
    void unSetEndHead();

    // get the end head of render group
    const std::string& getEndHead() const;

    // show whether the end head of render group is set
    const bool isSetEndHead() const { return _isSetEndHead; }

    // set the font size of render group
    void setFontSize(const RAVector& fSize);

    // get the font size of render group
    const RAVector& getFontSize() const;

    // show whether the font size of render group is set
    const bool isSetFontSize() const { return _isSetFontSize; }

    // set the font family of render group
    void setFontFamily(const std::string& fFamily);

    // get the font family of render group
    const std::string& getFontFamily() const;

    // show whether the font family of render group is set
    const bool isSetFontFamily() const { return _isSetFontFamily; }

    // set the font weight of render group
    void setFontWeight(const std::string& fWeight);

    // get the font weight of render group
    const std::string& getFontWeight() const;

    // show whether the font weight of render group is set
    const bool isSetFontWeight() const { return _isSetFontWeight; }

    // set the font style of render group
    void setFontStyle(const std::string& fStyle);

    // get the font style of render group
    const std::string& getFontStyle() const;

    // show whether the font style of render group is set
    const bool isSetFontStyle() const { return _isSetFontStyle; }

    // set the horizontal text anchor of render group
    void setHTextAnchor(const std::string& hTAnchor);

    // get the horizontal text anchor of render group
    const std::string& getHTextAnchor() const;

    // show whether the horizontal text anchor of render group is set
    const bool isSetHTextAnchor() const { return _isSetHTextAnchor; }

    // set the vertical text anchor of render group
    void setVTextAnchor(const std::string& vTAnchor);

    // get the vertical text anchor of render group
    const std::string& getVTextAnchor() const;

    // show whether the vertical text anchor of render group is set
    const bool isSetVTextAnchor() const { return _isSetVTextAnchor; }

    // add a geometric shape to list of elements
    void addToListOfElements(VTransformation2D *t);

    // remove a geometric shape from list of elements
    void removeFromListOfElements(unsigned int index);

    // remove all geometric shapes from list of elements
    void clearListOfElements();

    // get the list of elements
    const listOfElements& getListOfElements() const;
    
    // set the list of elements
    void setListOfElements(const listOfElements& ev);

    // show whether list of elements is set
    const bool isSetListOfElements() const { return _isSetListOfElements; }

    // get the total number of geometric shapes
    const size_t getNumElements() const;

    // find a geometric shape by its specified Id. Returns NULL if no such geometric shape exists
    VTransformation2D* findElementById(const std::string& id);

    // find the index of a geometric shape by its specified Id. Returns -1 if no such geometric shape exists
    unsigned int findElementIndexById(const std::string& id);
    
    // find a geometric shape by its shape. Returns NULL if no such geometric shape or more than one geometric shape of this kind exists
    VTransformation2D* findElementByShape(const std::string& shape);

    // generate unique Id for a new geometric shape
    std::string getElementUniqueId();
    
protected:
    // model info:
    std::string _startHead;
    std::string _endHead;
    RAVector _fontSize;
    std::string _fontFamily;
    std::string _fontWeight;
    std::string _fontStyle;
    std::string _hTextAnchor;
    std::string _vTextAnchor;
    listOfElements _listOfElements;
    bool _isSetStartHead;
    bool _isSetEndHead;
    bool _isSetFontSize;
    bool _isSetFontFamily;
    bool _isSetFontWeight;
    bool _isSetFontStyle;
    bool _isSetHTextAnchor;
    bool _isSetVTextAnchor;
    bool _isSetListOfElements;
};

/// @class VGlobalStyle
/// @brief The equivalent class for SBML [GlobalStyle](http://model.caltech.edu/software/libsbml/5.18.0/docs/formatted/cpp-api/class_global_style.html) .
class VGlobalStyle : public VeneerElement {
public:
    /// Constructors
    VGlobalStyle() {
        _roleList.clear();
        _typeList.clear();
        _g = NULL;
        _isSetRoleList = false;
        _isSetTypeList = false;
        _isSetGroup = false;
    }
    
    VGlobalStyle(VGlobalStyle& gS);

    /// Containers
    // role list
    typedef std::set<std::string> roleListSet;

    // type list
    typedef std::set<std::string> typeListSet;

    /// Iterators
    // role ist
    typedef roleListSet::iterator roleListIt;
    typedef roleListSet::const_iterator constRoleListIt;

    // type list
    typedef typeListSet::iterator typeListIt;
    typedef typeListSet::const_iterator constTypeListIt;

    /// Functions related to beginning and end of containers
    // role list
    const constRoleListIt roleListBegin() const { return _roleList.begin(); }
    const constRoleListIt roleListEnd() const { return _roleList.end(); }

    // type list
    const constTypeListIt typeListBegin() const { return _typeList.begin(); }
    const constTypeListIt typeListEnd() const { return _typeList.end(); }

    /// Functions
    // set the role list of style
    void setRoleList(const roleListSet& roleList);

    // add a new role the role list of style
    void addToRoleList(const std::string& role);

    // remove a role from the role list of style
    void removeFromRoleList(const std::string& role);

    // clear the the role list of style
    void clearRoleList();

    // get the role list of style
    const roleListSet& getRoleList() const;

    // show whether the role list of style is set
    const bool isSetRoleList() const { return _isSetRoleList; }

    // set the type list of style
    void setTypeList(const typeListSet& typeList);

    // add a new type the type list of style
    void addToTypeList(const std::string& type);

    // remove a type from the type list of style
    void removeFromTypeList(const std::string& type);

    // clear the the type list of style
    void clearTypeList();

    // get the the type list of style
    const typeListSet& getTypeList() const;

    // show whether the type list of style is set
    const bool isSetTypeList() const { return _isSetTypeList; }

    // set the group of style
    void setGroup(VRenderGroup* g);

    // get the group of style
    VRenderGroup* getGroup();

    // show whether the group of style is set
    const bool isSetGroup() const { return _isSetGroup; }
#ifndef SWIG
    // show whether it is of type local style
    const virtual bool isLocalStyle() const { return false; }
#endif

protected:
    // model info:
    roleListSet _roleList;
    typeListSet _typeList;
    VRenderGroup* _g;
    bool _isSetRoleList;
    bool _isSetTypeList;
    bool _isSetGroup;
};

/// @class VLocalStyle
/// @brief The equivalent class for SBML [LocalStyle](http://model.caltech.edu/software/libsbml/5.18.0/docs/formatted/cpp-api/class_local_style.html) .
class VLocalStyle : public VGlobalStyle {
public:
    /// Constructors
    VLocalStyle() {
        _idList.clear();
        _isSetIdList = false;
    }
    
    VLocalStyle(VLocalStyle& lS);

    /// Containers
    // id list
    typedef std::set<std::string> idListSet;

    /// Iterators
    // id list
    typedef idListSet::iterator idListIt;
    typedef idListSet::const_iterator constIdListIt;

    /// Functions related to beginning and end of containers
    // id list
    const constIdListIt idListBegin() const { return _idList.begin(); }
    const constIdListIt idListEnd() const { return _idList.end(); }

    /// Functions
    // set the id list of style
    void setIdList(const idListSet& idList);

    // add a new id to the id list of style
    void addToIdList(const std::string& id);

    // remove an id from the id list of style
    void removeFromIdList(const std::string& id);

    // clear the id list of style
    void clearIdList();

    // get the id list of style
    const idListSet& getIdList() const;

    // show whether the id list of style is set
    const bool isSetIdList() const { return _isSetIdList; }
#ifndef SWIG
    // show whether it is of type local style
    const virtual bool isLocalStyle() const { return true; }
#endif

protected:
    // model info:
    idListSet _idList;
    bool _isSetIdList;
};

/// @class VLineEnding
/// @brief The equivalent class for SBML [LineEnding](http://model.caltech.edu/software/libsbml/5.18.0/docs/formatted/cpp-api/class_line_ending.html) .
class VLineEnding : public VGraphicalPrimitive2D {
public:
    /// Constructors
    VLineEnding() {
        _box = NULL;
        _g = NULL;
        _enableRotationMapping = true;
        _isSetBox = false;
        _isSetGroup = false;
        _isSetEnableRotationMapping = false;
        _isLocalLineEnding = false;
    }
    
    VLineEnding(VLineEnding& lE);

    /// Functions
    // set the bounding box of line ending
    void setBox(LBox* b);

    // get the bounding box of line ending
    LBox* getBox();

    // show whether the bounding box of line ending is set
    const bool isSetBox() const { return _isSetBox; }

    // set the group of line ending
    void setGroup(VRenderGroup* g);

    // get the group of line ending
    VRenderGroup* getGroup();

    // show whether the group of line ending is set
    const bool isSetGroup() const { return _isSetGroup; }

    // set the rotation status of line ending
    void setEnableRotationMapping(const bool& enable);

    // get the rotation status of line ending
    const bool getEnableRotationMapping() const;
    
    // show whether the rotation status of line ending is set
    const bool isSetEnableRotationMapping() const { return _isSetEnableRotationMapping; }
    
    // set the line ending as local
    void setAsLocalLineEnding(const bool& isLocal);
    
    // show whether it is used locally or globally
    const bool isLocalLineEnding() const { return _isLocalLineEnding; }

protected:
    // model info:
    LBox* _box;
    VRenderGroup* _g;
    bool _enableRotationMapping;
    bool _isSetBox;
    bool _isSetGroup;
    bool _isSetEnableRotationMapping;
    bool _isLocalLineEnding;
};

/// @class VGradientStop
/// @brief The equivalent class for SBML [GradientStop](http://model.caltech.edu/software/libsbml/5.18.0/docs/formatted/cpp-api/class_gradient_stop.html) .
class VGradientStop : public VeneerElement {
public:
    /// Constructors
    VGradientStop() {
        _isSetOffset = false;
        _isSetStopColor = false;
    }
    
    VGradientStop(const VGradientStop& s);

    /// Functions
    // set the offset value of gradient stop
    void setOffset(const RAVector& offset);

    // get the offset value of gradient stop
    const RAVector& getOffset() const;

    // show whether the offset value of gradient stop is set
    const bool isSetOffset() const { return _isSetOffset; }

    // set the stop color of gradient stop
    void setStopColor(const std::string& color);

    // get the stop color of gradient stop
    const std::string& getStopColor() const;

    // show whether the stop color of gradient stop is set
    const bool isSetStopColor() const { return _isSetStopColor; }

protected:
    // model info:
    RAVector _offset;
    std::string _stopColor;
    bool _isSetOffset;
    bool _isSetStopColor;
};

/// @class RenPoint
/// @brief The  class for the elements of VPloygon and RCurve
class RenPoint : public VeneerElement {
public:
    /// Constructors
    RenPoint() {
        _isSetRPoint = false;
    }
    
    RenPoint(const RenPoint& rp);

    /// Functions
    // set the rpoint value of render point
    void setRPoint(const RPoint& rp);

    // get the rpoint value of render point
    const RPoint& getRPoint() const;

    // show whether the rpoint value of render point is set
    const bool isSetRPoint() const { return _isSetRPoint; }

    // show whether it is of type render cubic bezier
    virtual bool isRenderCubicBezier() { return false; }

protected:
    // model info:
    RPoint _r;
    bool _isSetRPoint;
};

/// @class RCubicBezier
/// @brief The equivalent class for SBML [RenderCubicBezier](http://model.caltech.edu/software/libsbml/5.18.0/docs/formatted/cpp-api/class_render_cubic_bezier.html) .
class RCubicBezier : public RenPoint {
public:
    /// Constructors
    RCubicBezier() {
        _isSetBasePoint1 = false;
        _isSetBasePoint2 = false;
    }
    
    RCubicBezier(const RCubicBezier& rcb);

    /// Functions
    // set the base point 1 value of render cubic bezier
    void setBasePoint1(const RPoint& rp);

    // get the base point 1 value of render cubic bezier
    const RPoint& getBasePoint1() const;

    // show whether the base point 1 value of render cubic bezier is set
    const bool isSetBasePoint1() const { return _isSetBasePoint1; }

    // set the base point 2 value of render cubic bezier
    void setBasePoint2(const RPoint& rp);

    // get the base point 2 value of render cubic bezier
    const RPoint& getBasePoint2() const;

    // show whether the base point 2 value of render cubic bezier is set
    const bool isSetBasePoint2() const { return _isSetBasePoint2; }

    // show whether it is of type render cubic bezier
    virtual bool isRenderCubicBezier() { return true; }

protected:
    // model info:
    RPoint _basePoint1;
    RPoint _basePoint2;
    bool _isSetBasePoint1;
    bool _isSetBasePoint2;
};

}
#endif
