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
std::vector<QGraphicsItem*> findLineEnding(MainWindow* mw, const std::string& lineEndingId, const QPen& curvePen, bool& enableRotation);

// get info from veneer color and store it in a graphical color
void getInfoFromColor(GraphicalColor* gColor, VColorDefinition* color = NULL, const ColorString colorString = NUM_ColorString);

// get info from the gradient of veneer and store it in a graphical graidient
void getInfoFromGradient(MainWindow* mw, VGradientBase* gradient, GraphicalGradient* gGradient);



std::vector<QGraphicsItem*> getInfoFromRenderGroup(MainWindow* mw, VRenderGroup* group, LBox* box);

std::vector<GraphicalCurve*> getInfoFromRenderGroup(MainWindow* mw, VRenderGroup* group, LBox* box, LCurve* curve);

void getGraphicalItemInfoFromRenderGroup(MainWindow* mw, VRenderGroup* group, std::string& strokeColor, double& strokeWidth, std::vector<unsigned int>*& dashes, std::string& fillColor, std::string& fillRule, std::string& fontFamily, RAVector* fontSize, std::string& fontWeight , std::string& fontStyle, std::string& hTextAnchor, std::string& vTextAnchor, std::string& startHead, std::string& endHead);

std::vector<QGraphicsItem*> getGeometricShapes(MainWindow* mw, VRenderGroup* group, LBox* box, std::string strokeColor, double strokeWidth, std::vector<unsigned int>* dashes, const std::string fillColor, const std::string fillRule);

QGraphicsItem* getImageShape(MainWindow* mw, VTransformation2D* gS, LBox* box);

QGraphicsItem* getTextShape(MainWindow* mw, VTransformation2D* gS, LBox* box, std::string fontFamily, RAVector* fontSize, std::string fontWeight, std::string fontStyle, std::string hTextAnchor, std::string vTextAnchor);

QGraphicsItem* getRectangleShape(MainWindow* mw, VTransformation2D* gS, LBox* box, std::string strokeColor, double strokeWidth, std::vector<unsigned int>* dashes, std::string fillColor);

QGraphicsItem* getEllipseShape(MainWindow* mw, VTransformation2D* gS, LBox* box, std::string strokeColor, double strokeWidth, std::vector<unsigned int>* dashes, std::string fillColor);

QGraphicsItem* getPolygonShape(MainWindow* mw, VTransformation2D* gS, LBox* box, std::string strokeColor, double strokeWidth, std::vector<unsigned int>* dashes, std::string fillColor, std::string fillRule);

GraphicalCurve* getGraphicalCurve(MainWindow* mw, VTransformation2D* gS, LBox* box, std::string strokeColor, double strokeWidth, std::vector<unsigned int>* dashes, std::string startHead, std::string endHead);

GraphicalCurve* getGraphicalCurve(MainWindow* mw, VRenderGroup* group, LCurve* curve, std::string strokeColor, double strokeWidth, std::vector<unsigned int>* dashes, std::string startHead, std::string endHead);

QPen getPenFeatures(MainWindow* mw, std::string strokeColor, double strokeWidth, std::vector<unsigned int>* dashes);

QBrush getBrushFeatures(MainWindow* mw, std::string fillColor);










// get info from the style of veneer and set the values of a graphical text item using its value
std::vector<QGraphicsItem*> getInfoFromRenderGroup(MainWindow* mw, VRenderGroup* group, MyQGraphicsTextItem* graphicalText);

// get info from the graphical shape of a group and set the values of graphics text item using its values
std::vector<QGraphicsItem*> getInfoFromGeometricShape(MainWindow* mw, VRenderGroup* group, MyQGraphicsTextItem* graphicalText);

// get info from text shape of a group and set the values of graphical text item using its values
QGraphicsItem*  getInfoFromTextShape(VTransformation2D *gShape, MyQGraphicsTextItem *graphicalText);

// get info of a group and set the values of graphical text item using its values
void getGraphicalItemInfoFromVeneer(MainWindow* mw, VRenderGroup* group, MyQGraphicsTextItem* graphicalText);

// get info of a graphical shape and set the values of graphical text item using its values
void getGraphicalItemInfoFromVeneer(MainWindow* mw, VTransformation2D* gShape, MyQGraphicsTextItem* graphicalText);

// get info of a graphical shape and set the values of graphical curve using its values
void getGraphicalItemInfoFromVeneer(MainWindow* mw, VTransformation2D* gShape, GraphicalCurve* graphicalCurve);

// set the values of a graphical text item using either group or graphical shape info
void setGraphicalItemInfo(MainWindow* mw, MyQGraphicsTextItem* graphicalText, std::string& strokeColor, std::string& fontFamily, RAVector* fontSize, std::string& fontWeight, std::string& fontStyle, std::string& hTextAnchor, std::string& vTextAnchor);

void setLineEndings(MainWindow* mw, GraphicalCurve* gCurve);




#endif // GFSBMLDOC_H
