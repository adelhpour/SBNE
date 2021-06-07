#ifndef __NE_SBMLDOCUMENT_H
#define __NE_SBMLDOCUMENT_H

#include "ne_mainwindow.h"

using namespace sbne;

class MainWindow;
class GraphicalColor;
class GraphicalGradient;
class GraphicalLineEnding;
class GraphicalCompartment;
class GraphicalSpecies;
class GraphicalReaction;
class MyQGraphicsTextItem;
class GraphicalCurve;

class NESBMLDocument{
public:
    /// Constructors
    NESBMLDocument () {
        _isSetDocument = false;
        _isLayoutModified = false;
        _isLayoutAlreadyExisted = false;
        _isRenderModified = false;
        _isRenderAlreadyExisted = false;
    }

    /// Functions
    // load sbml model
    void loadSBML(MainWindow* mw);
    
    // get the layout info from the document and display it
    void loadLayoutInfo(MainWindow* mw);
    
    // get the render info from the document and display it
    void loadRenderInfo(MainWindow* mw);
    
    // save sbml model
    bool saveSBML(MainWindow* mw, const QString& fileName);
    
    // set sbml document
    void setDocument(SBMLDocument* document);
    
    // get sbml document
    SBMLDocument* getDocument();
    
    // show whether the sbml document is set
    const bool isSetDocument() const { return _isSetDocument; };
    
    // get layout info
    LayoutInfo* getLayoutInfo();
    
    // get render info
    RenderInfo* getRenderInfo();
    
    // get network
    Network* getNetwork();
    
    // get network
    Veneer* getVeneer();
    
    // set whether layout is modified or not
    void setLayoutModified(const bool& isLayoutModified);
    
    // show whether the layout is modified
    const bool isLayoutModified() const { return _isLayoutModified; };
    
    // set whether the layout has already existed or not
    void setLayoutAlreadyExisted(const bool& isLayoutAlreadyExisted);
    
    // show whether the layout has already existed
    const bool isLayoutAlreadyExisted() const { return _isLayoutAlreadyExisted; };
    
    // set whether render is modified or not
    void setRenderModified(const bool& isRenderModified);
    
    // show whether the render is modified
    const bool isRenderModified() const { return _isRenderModified; };
    
    // set whether the render has already existed or not
    void setRenderAlreadyExisted(const bool& isRenderAlreadyExisted);
    
    // show whether the rener has already existed
    const bool isRenderAlreadExited() const { return _isRenderAlreadyExisted; };
    
protected:
    SBMLDocument* _document;
    LayoutInfo* _li;
    RenderInfo* _ri;
    bool _isSetDocument;
    bool _isLayoutModified;
    bool _isLayoutAlreadyExisted;
    bool _isRenderModified;
    bool _isRenderAlreadyExisted;
};

// get the QColor using its id by searching in main window colors
const QColor getQColor(MainWindow *mw, const std::string& colorId);

// find the graphical gardient using its id. Returns NULL if such gradients does not exits
GraphicalGradient* findGraphicalGradient(MainWindow *mw, const std::string& gradientId);

// find a line ending by searching in main window line ending info and create a copy of it if exists
QGraphicsItem* findLineEnding(MainWindow* mw, const std::string& lineEndingId, const QPen& curvePen, bool& enableRotation);

// get info from veneer color and store it in a graphical color
void getInfoFromColor(GraphicalColor* gColor, VColorDefinition* color = NULL, const ColorString colorString = NUM_ColorString);

// get info from the gradient of veneer and store it in a graphical graidient
void getInfoFromGradient(MainWindow* mw, VGradientBase* gradient, GraphicalGradient* gGradient);

// get info from the line ending of veneer and store it in a graphical line ending
void getInfoFromLineEnding(MainWindow* mw, VLineEnding* le, GraphicalLineEnding* gLEnding);

// get info from the style of veneer and set the values of a graphical item using its value
void getInfoFromStyle(MainWindow* mw, VGlobalStyle* style, QGraphicsItem* graphicalItem);

// get info from the style of veneer and set the values of a graphical text item using its value
void getInfoFromStyle(MainWindow* mw, VGlobalStyle* style, MyQGraphicsTextItem* graphicalText);

// get info from the style of veneer and set the values of a graphical curve using its value
void getInfoFromStyle(MainWindow* mw, VGlobalStyle* style, GraphicalCurve* graphicalCurve);

// get info from the graphical shape of a group and set the values of graphics item using its values
void getInfoFromGeometricShape(MainWindow* mw, VRenderGroup* group, QGraphicsItem* graphicalItem, std::string& fillRule);

// get info from the graphical shape of a group and set the values of graphics text item using its values
void getInfoFromGeometricShape(MainWindow* mw, VRenderGroup* group, MyQGraphicsTextItem* graphicalText);

// get info from rectangle shape of a group and set the values of graphical item using its values
void getInfoFromRectangleShape(VTransformation2D* gShape, QGraphicsItem* graphicalItem);

// get info from ellipse shape of a group and set the values of graphical item using its values
void getInfoFromEllipseShape(VTransformation2D* gShape, QGraphicsItem* graphicalItem);

// get info from polygon shape of a group and set the values of graphical item using its values
void getInfoFromPolygonShape(VTransformation2D* gShape, QGraphicsItem* graphicalItem, std::string& fillRule);

// get info from image shape of a group and set the values of graphical item using its values
void getInfoFromImageShape(VTransformation2D* gShape, QGraphicsItem* graphicalItem);

// get info from text shape of a group and set the values of graphical text item using its values
void  getInfoFromTextShape(VTransformation2D *gShape, MyQGraphicsTextItem *graphicalText);

// get info of a group and set the values of graphical item using its values
void getGraphicalItemInfoFromVeneer(MainWindow* mw, VRenderGroup* group, QGraphicsItem* graphicalItem, std::string& fillRule);

// get info of a graphical shape and set the values of graphical item using its values
void getGraphicalItemInfoFromVeneer(MainWindow* mw, VTransformation2D* gShape, QGraphicsItem* graphicalItem, std::string& fillRule);

// get info of a group and set the values of graphical text item using its values
void getGraphicalItemInfoFromVeneer(MainWindow* mw, VRenderGroup* group, MyQGraphicsTextItem* graphicalText);

// get info of a graphical shape and set the values of graphical text item using its values
void getGraphicalItemInfoFromVeneer(MainWindow* mw, VTransformation2D* gShape, MyQGraphicsTextItem* graphicalText);

// get info of a group and set the values of graphical curve using its values
void getGraphicalItemInfoFromVeneer(MainWindow* mw, VRenderGroup* group, GraphicalCurve* graphicalCurve);

// get info of a graphical shape and set the values of graphical curve using its values
void getGraphicalItemInfoFromVeneer(MainWindow* mw, VTransformation2D* gShape, GraphicalCurve* graphicalCurve);

// set the values of a graphical item using either group or graphical shape info
void setGraphicalItemInfo(MainWindow* mw, QGraphicsItem* graphicalItem, std::string& strokeColor, double& strokeWidth, std::vector<unsigned int>* dashes, std::string& fillColor, std::string& fillRule);

// set the values of a graphical text item using either group or graphical shape info
void setGraphicalItemInfo(MainWindow* mw, MyQGraphicsTextItem* graphicalText, std::string& strokeColor, std::string& fontFamily, RAVector* fontSize, std::string& fontWeight, std::string& fontStyle, std::string& hTextAnchor, std::string& vTextAnchor);

// set the values of a graphical curve using either group or graphical shape info
void setGraphicalItemInfo(MainWindow* mw, GraphicalCurve* graphicalCurve, std::string& strokeColor, double& strokeWidth, std::vector<unsigned int>* dashes, std::string& startHead, std::string& endHead);

void setLineEndings(MainWindow* mw, GraphicalCurve* gCurve);




#endif // GFSBMLDOC_H
