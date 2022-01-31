#ifndef __NE_MAINWINDOW_H
#define __NE_MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QTextDocument>
#include <QPainterPath>
#include <QWheelEvent>
#include <QBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QFrame>
#include <QPrinter>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
#include <QToolButton>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QLabel>
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QTimeLine>
#include <QScrollBar>
#include <QScrollArea>
#include <QFontDatabase>
#include <QStandardItem>
#include <QTreeWidget>
#include <QWidgetAction>
#include <QMenu>

#include "sbne//ne_core.h"
#include "sbne/layout/ne_layout.h"
#include "sbne/render/ne_render.h"
#include "ne_featureMenu.h"
#include "ne_SBMLDocument.h"



namespace Ui {
class MainWindow;
}

class MyQGraphicsView;
class NESBMLDocument;
class FeatureMenu;
class GraphicalColor;
class GraphicalGradient;
class GraphicalLineEnding;
class GraphicalCompartment;
class GraphicalSpecies;
class GraphicalReaction;
class GraphicalSReference;
class GraphicalText;
class MyQGraphicsTextItem;
class GraphicalCurve;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    /// Constructor
    explicit MainWindow(QWidget *parent = nullptr);
    
    /// Destructor
    ~MainWindow();
    
    /// Containers
    // colors
    typedef std::vector<GraphicalColor *> gColorVec;
    
    // gradients
    typedef std::vector<GraphicalGradient *> gGradientVec;
    
    // line endings
    typedef std::vector <GraphicalLineEnding *> gLEndingVec;
    
    // compartments
    typedef std::vector <GraphicalCompartment *> gCompartmentVec;
    
    // species
    typedef std::vector <GraphicalSpecies *> gSpeciesVec;
    
    // reactions
    typedef std::vector<GraphicalReaction *> gReactionVec;
    
    /// Iterators
    // colors
    typedef gColorVec::iterator gColorIt;
    typedef gColorVec::const_iterator constGColorIt;
    
    // gradients
    typedef gGradientVec::iterator gGradientIt;
    typedef gGradientVec::const_iterator constGGradientIt;
    
    // line endings
    typedef gLEndingVec::iterator gLEndingIt;
    typedef gLEndingVec::const_iterator constGLEndingIt;
    
    // compartments
    typedef gCompartmentVec::iterator gCompartmentIt;
    typedef gCompartmentVec::const_iterator constGCompartmentIt;
    
    // species
    typedef gSpeciesVec::iterator gSpeciesIt;
    typedef gSpeciesVec::const_iterator constGSpeciesIt;
    
    // reactions
    typedef gReactionVec::iterator gReactionIt;
    typedef gReactionVec::const_iterator constGReactionIt;
    
    /// Functions related to beginning and end of containers
    // colors
    const constGColorIt gColorsBegin() const { return _colorInfo.begin(); }
    const constGColorIt gColorsEnd() const { return _colorInfo.end(); }
    
    // gradients
    const constGGradientIt gGradientsBegin() const { return _gradientInfo.begin();}
    const constGGradientIt gGradientsEnd() const { return _gradientInfo.end(); }
    
    // line endings
    const constGLEndingIt gLEndingsBegin() const { return _lineEndingInfo.begin();}
    const constGLEndingIt gLEndingsEnd() const { return _lineEndingInfo.end(); }
    
    // compartments
    const constGCompartmentIt gCompartmentsBegin() const { return _compartmentInfo.begin(); }
    const constGCompartmentIt gCompartmentsEnd() const { return _compartmentInfo.end(); }
    
    // species
    const constGSpeciesIt gSpeciesBegin() const { return _speciesInfo.begin(); }
    const constGSpeciesIt gSpeciesEnd() const { return _speciesInfo.end(); }
    
    // reactions
    const constGReactionIt gReactionsBegin() const { return _reactionInfo.begin();}
    const constGReactionIt gReactionsEnd() const { return _reactionInfo.end(); }
    
    /// Functinos
    // set view
    void setView(MyQGraphicsView* view);
    
    // get view
    QGraphicsView* getView();
    
    // show whether the view is set
    const bool isSetView() const { return _isSetView; }
    
    // set scene
    void setScene(QGraphicsScene* scene);
    
    // get scene
    QGraphicsScene* getScene();
    
    // show whether the scene is set
    const bool isSetScene() const { return _isSetScene; }
    
    // reset scene extents
    void resetSceneRectExtents();
    
    // set scene extends
    void setSceneRect(QRectF rect);
    
    // set feature menu
    void setFeatureMenu(FeatureMenu* featureMenu);
    
    // get feature menu
    FeatureMenu* getFeatureMenu();
    
    // show whether the feature menu is set
    const bool isSetFeatureMenu() const { return _isSetFeatureMenu; }
    
    // set current file name
    void setCurrentFileName(const QString& fName);
    
    // get current file name
    const QString& getCurrentFileName();
    
    // show whether the current file name is set
    const bool isSetCurrentFileName() const { return _isSetCurrentFileName; }
    
    // set sbml document
    void setSBMLDocument(NESBMLDocument* SBMLDocument);
    
    // get sbml document
    NESBMLDocument* getSBMLDocument();
    
    // show whether the sbml document is set
    const bool isSetSBMLDocument() const { return _isSetSBMLDocument; }
    
    // add a new graphical color to list of color info
    void addGColor(GraphicalColor* c);

    // remove a graphical color from list of color info using its index
    void removeGColor(unsigned int graphicalColorIndex);
    
    // set the color info
    void setColorInfo(const gColorVec& cv);

    // get the color info
    const gColorVec& getColorInfo() const;
    
    // get total number of graphical colors
    const size_t getNumGColors() const;
    
    // add a new graphical gradient to list of gradient info
    void addGGradient(GraphicalGradient* g);

    // remove a graphical gradient from list of gradient info using its index
    void removeGGradient(unsigned int graphicalGradientIndex);
    
    // set the gradient info
    void setGradientInfo(const gGradientVec& gv);

    // get the gradient info
    const gGradientVec& getGradientInfo() const;
    
    // get total number of graphical gradients
    const size_t getNumGGradients() const;
    
    // add a new graphical line ending to list of line ending info
    void addGLEnding(GraphicalLineEnding* l);

    // remove a graphical line ending from list of line ending info using its index
    void removeGLEnding(unsigned int graphicalLEndingIndex);
    
    // find a graphical line ending by its specified Id. Returns NULL if no such graphical line ending exists
    GraphicalLineEnding* findGLEndingById(const std::string& graphicalLineEndingId);
    
    // set the line ending info
    void setLEndingInfo(const gLEndingVec& lv);

    // get the line ending info
    const gLEndingVec& getLEndingInfo() const;
    
    // get total number of graphical line endings
    const size_t getNumGLEndings() const;
    
    // add a new graphical color to list of color info
    void addGCompartment(GraphicalCompartment* c);

    // remove a graphical compartment from list of compartment info using its index
    void removeGCompartment(unsigned int graphicalCompartmentIndex);
    
    // set the compartment info
    void setCompartmentInfo(const gCompartmentVec& cv);

    // get the compartment info
    const gCompartmentVec& getCompartmentInfo() const;
    
    // get total number of graphcial compartments
    const size_t getNumGCompartments() const;
    
    // add a new graphical species to list of species info
    void addGSpecies(GraphicalSpecies* s);

    // remove a graphical species from list of species info using its index
    void removeGSpecies(unsigned int graphicalSpeciesIndex);
    
    // set the species info
    void setSpeciesInfo(const gSpeciesVec& sv);

    // get the species info
    const gSpeciesVec& getSpeciesInfo() const;
    
    // get total number of graphcial species
    const size_t getNumGSpecies() const;
    
    // add a new graphical reaction to list of reaction info
    void addGReaction(GraphicalReaction* r);

    // remove a graphical reaction from list of reaction info using its index
    void removeGReaction(unsigned int graphicalReactionIndex);
    
    // set the reaction info
    void setReactionInfo(const gReactionVec& rv);

    // get the reaction info
    const gReactionVec& getReactionInfo() const;
    
    // get total number of graphical reactions
    const size_t getNumGReactions() const;
    
    // clear all the info
    void clearInfo();
    
signals:
    void numberOfGLEndingChanged();

private slots:
    // open act
    void open();
    
    // save act
    void save();
    
    // screenshot act
    void screenshot();
    
    // exit act
    void exit();

private:
    // main window info
    Ui::MainWindow* ui;
    QMenu* fileMenu;
    QAction* openAct;
    QAction* saveAct;
    QAction* screenshotAct;
    QAction* exitAct;
    MyQGraphicsView* _view;
    QGraphicsScene* _scene;
    FeatureMenu* _featureMenu;
    QString _currentFileName;
    bool _isSetView;
    bool _isSetScene;
    bool _isSetFeatureMenu;
    bool _isSetCurrentFileName;
    
    // main window
    void createActions();
    void createMenus();
    qreal _minSceneX;
    qreal _minSceneY;
    qreal _maxSceneX;
    qreal _maxSceneY;
    
    // sbml info
    NESBMLDocument* _SBMLDocument;
    gColorVec _colorInfo;
    gGradientVec _gradientInfo;
    gLEndingVec _lineEndingInfo;
    gCompartmentVec _compartmentInfo;
    gSpeciesVec _speciesInfo;
    gReactionVec _reactionInfo;
    bool _isSetSBMLDocument;
};

class MyQGraphicsView : public QGraphicsView {
    Q_OBJECT
    
public:
    MyQGraphicsView(QWidget* parent = 0, MainWindow* mw = NULL) : QGraphicsView(parent) {
        _maxScale = 3.0;
        _minScale = 1.0 / 3.0;
        _numScheduledScalings = 0;
        _mw = mw;
    };
    
    // get the current scale of the view
    const qreal currentScale() const;
    
    // set the maximum allowed scale of the view
    void setMaxScale(const qreal& maxScale);
    
    // get the maximum allowed scale of the view
    const qreal getMaxScale() const;
    
    // set the minimum allowed scale of the view
    void setMinScale(const qreal& minScale);
    
    // get the minimum allowed scale of the view
    const qreal getMinScale() const;
    
    // find which item of view is located in the entred position. Return -1 if no such item is located.
    int findGraphicalItem(QPointF position);
    
    // check to see if the enetered position is close enough to the path to consider it as a part of the path
    bool isCloseEnoughToCurveSemgent(const QPainterPath& path, const QPointF& position, const qreal& minRequiredDistance);
    
    // check to see if the enetered position is inside the rotated rectangle
    bool isInsideTheRotatedRectangle(const QRectF& boundingRect, const QPointF& rotationCenter, const qreal& rotatedAngle, const QPointF& position);
    
private slots:
    void scalingTime(qreal x);
    void animFinished();
    
protected:
    virtual void wheelEvent(QWheelEvent* event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
    int _numScheduledScalings;
    qreal _maxScale;
    qreal _minScale;
    bool _pan;
    int _panStartX;
    int _panStartY;
    MainWindow* _mw;
};

class MyQGraphicsTextItem : public QGraphicsTextItem {
    Q_OBJECT
    
public:
    MyQGraphicsTextItem(QString plainText, QRectF boundingRect, QGraphicsItem *parent = 0) :
        QGraphicsTextItem(parent) {
            _plainText = plainText;
            _boundingRect = boundingRect;
            _verticalAlignment = "middle";
            _horizontalAlignment = "middle";
        }
    
    QRectF boundingRect() const;
    
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
    
    void setTextPen(const QPen& textPen);

    const QPen& textPen() const;
       
    void setTextFont(const QFont& textFont);

    const QFont& textFont() const;

    void setVerticalAlignment(const std::string& vAlignment);

    const std::string& verticalAlignment() const;
    
    void setHorizontalAlignment(const std::string& hAlignment);

    const std::string& horizontalAlignment() const;
    
protected:
    QString _plainText;
    QPen _textPen;
    QFont _textFont;
    std::string _verticalAlignment;
    std::string _horizontalAlignment;
    QRectF _boundingRect;
};

class GraphicalElement {
public:
    /// Constructors
    GraphicalElement() {
        _isSetId = false;
    }
    
    /// Functions
    // set the id of graphical element
    void setId(const std::string& id);

    // get the id of graphical element
    const std::string& getId() const;
    
    // show whether the of graphical element the id is set
    const bool isSetId() const { return _isSetId; }
    
    // unset the id of graphical element
    void unSetId();
    
protected:
    std::string _id;
    bool _isSetId;
};

class GraphicalObjectBase : public GraphicalElement {
public:
    /// Constructors
    GraphicalObjectBase() {
        _nGraphicalObject = NULL;
        _style = NULL;
        _graphicalItems.clear();
        _graphicalCurves.clear();
        _graphicalTexts.clear();
        _isSetNGraphicalObject = false;
        _isSetStyle = false;
    }
    
    /// Containers
    // graphical items
    typedef std::vector<QGraphicsItem *> gItemVec;
    
    // graphical curves
    typedef std::vector<GraphicalCurve *> gCurveVec;
    
    // graphical texts
    typedef std::vector<GraphicalText *> gTextVec;
    
    /// Iterators
    // graphical items
    typedef gItemVec::iterator gItemIt;
    typedef gItemVec::const_iterator constGItemIt;
    
    // graphical curves
    typedef gCurveVec::iterator gCurveIt;
    typedef gCurveVec::const_iterator constGCurveIt;
    
    // graphical texts
    typedef gTextVec::iterator gTextIt;
    typedef gTextVec::const_iterator constGTextIt;
    
    /// Functions related to beginning and end of containers
    // graphical items
    const constGItemIt gItemsBegin() const { return _graphicalItems.begin(); }
    const constGItemIt gItemsEnd() const { return _graphicalItems.end(); }
    
    // graphical curves
    const constGCurveIt gCurvesBegin() const { return _graphicalCurves.begin(); }
    const constGCurveIt gCurvesEnd() const { return _graphicalCurves.end(); }
    
    // graphical texts
    const constGTextIt gTextsBegin() const { return _graphicalTexts.begin(); }
    const constGTextIt gTextsEnd() const { return _graphicalTexts.end(); }
    
    /// Functinos
    // add a new graphical item to the list of graphical items
    void addGraphicalItem(QGraphicsItem* g);
    
    // remove a graphical item from list of graphical items using its index
    void removeGraphicalItem(unsigned int graphicalItemIndex);
    
    // set gItemsVec
    void setGraphicalItems(const gItemVec& giv);

    // get gItemsVec
    const gItemVec& getGraphicalItems() const;
    
    // remove all graphical items from of graphical object
    void resetGraphicalItems(MainWindow* mw, const bool& isLineEnding = false);
    
    // set the network graphical object of graphical object
    void setNGraphicalObject(sbne::NGraphicalObject* gO);

    // get the network graphical object of graphical object
    sbne::NGraphicalObject* getNGraphicalObject();
          
    // show whether the network graphical object of graphical object is set
    const bool isSetNGraphicalObject() const { return _isSetNGraphicalObject; }
    
    // set the style of graphical object
    void setStyle(sbne::VGlobalStyle* s);
    
    // find the style and set it to the graphical object
    virtual void setStyle(MainWindow* mw, const bool& addNewStyle = false);
    
    // unset the style of graphical object
    void unSetStyle();

    // get the style of graphical object
    sbne::VGlobalStyle* getStyle();
    
    // show whether the style is set
    const bool isSetStyle() const { return _isSetStyle; }
    
    // add a new graphical curve to the list of graphical curves
    void addGraphicalCurve(GraphicalCurve* c);
    
    // remove a graphical curve from list of graphical curves using its index
    void removeGraphicalCurve(unsigned int graphicalCurveIndex);
    
    // set gCurveVec
    void setGraphicalCurves(const gCurveVec& gcv);

    // get gCurveVec
    const gCurveVec& getGraphicalCurves() const;
    
    // get total number of graphical curves
    const size_t getNumGraphicalCurves() const;
    
    // remove all graphical curves from graphical object
    void resetGraphicalCurves(MainWindow* mw, const bool& isLineEnding = false);
    
    // add a new graphical text to the list of graphical texts
    void addGraphicalText(GraphicalText* t);
    
    // remove a graphical text from list of graphical texts using its index
    void removeGraphicalText(unsigned int graphicalTextIndex);
    
    // set gTextVec
    void setGraphicalTexts(const gTextVec& gtv);

    // get gTextVec
    const gTextVec& getGraphicalTexts() const;
    
    // get total number of graphical texts
    const size_t getNumGraphicalTexts() const;
    
    // remove all graphical texts from graphical object
    void resetGraphicalTexts(MainWindow* mw);
    
protected:
    gItemVec _graphicalItems;
    sbne::NGraphicalObject* _nGraphicalObject;
    sbne::VGlobalStyle* _style;
    gCurveVec _graphicalCurves;
    gTextVec _graphicalTexts;
    bool _isSetNGraphicalObject;
    bool _isSetGCurve;
    bool _isSetStyle;
};

class GraphicalColor : public GraphicalElement {
public:
    /// Constructors
    GraphicalColor() {
        _color = NULL;
        _isSetGraphicalColor = false;
        _isSetColor = false;
    }
    
    /// Functions
    // set the graphical color of graphical color
    void setGraphicalColor(const QColor& c);

    // get the graphical color of graphical color
    const QColor& getGraphicalColor() const;
    
    // show whether the the graphical color is set
    const bool isSetGraphicalColor() const { return _isSetGraphicalColor; }
    
    // set the color of graphical color
    void setColor(sbne::VColorDefinition* c);

    // get the color of graphical color
    sbne::VColorDefinition* getColor();
    
    // show whether the the color is set
    const bool isSetColor() const { return _isSetColor; }
    
protected:
    QColor _graphicalColor;
    sbne::VColorDefinition* _color;
    bool _isSetGraphicalColor;
    bool _isSetColor;
};

class GraphicalGradient : public GraphicalElement {
public:
    /// Constructors
    GraphicalGradient() {
        _gradient = NULL;
        _isSetGraphicalGradient = false;
        _isSetGradient = false;
    }
    
    /// Functions
    // set the graphical gradient of graphical gradient
    void setGraphicalGradient(const QGradient& g);

    // get the graphical gradient of graphical gradient
    const QGradient& getGraphicalGradient() const;
    
    // show whether the the graphical gradient is set
    const bool isSetGraphicalGradient() const { return _isSetGraphicalGradient; }
    
    // set the gradient of graphical gradient
    void setGradient(sbne::VGradientBase* g);

    // get the gradient of graphical gradient
    sbne::VGradientBase* getGradient();
    
    // show whether the the gradient is set
    const bool isSetGradient() const { return _isSetGradient; }
    
protected:
    QGradient _graphicalGradient;
    sbne::VGradientBase* _gradient;
    bool _isSetGraphicalGradient;
    bool _isSetGradient;
};

class GraphicalLineEnding : public GraphicalObjectBase {
public:
    /// Constructors
    GraphicalLineEnding() {
        _lEnding = NULL;
        _enableRotation = true;
        _isSetLEnding = false;
        _isSetEnableRotation = false;
    }
    
    /// Functions
    // set the line ending of graphical line ending
    void setLEnding(sbne::VLineEnding* le);
    
    // unset the line ending of graphical line ending
    void unSetLEnding();

    // get the line ending of graphical line ending
    sbne::VLineEnding* getLEnding();
    
    // show whether the the line ending is set
    const bool isSetLEnding() const { return _isSetLEnding; }
    
    // set the enable rotation of graphical line ending
    void setRotation(const bool& rotation);
    
    // unset the enable rotation of graphical line ending
    void unSetRotation();

    // get the enable rotation of graphical line ending
    const bool& getRotation() const;
    
    // show whether the enable rotation is set
    const bool isSetEnableRotation() const { return _isSetEnableRotation; }
    
    void updateValues(MainWindow* mw);
    
protected:
    sbne::VLineEnding* _lEnding;
    bool _enableRotation;
    bool _isSetLEnding;
    bool _isSetEnableRotation;
};
 
class GraphicalCompartment : public GraphicalObjectBase {
public:
    
    /// Functinos
    void updateValues(MainWindow* mw);
};

class GraphicalSpecies : public GraphicalObjectBase {
public:
    
    /// Functinos
    void updateValues(MainWindow* mw, const bool& _fitConnectedItems);
    
    // fit the species connected items to its bounding box
    void fitConnectedItemsToBoundingBox(MainWindow* mw);
};

class GraphicalReaction : public GraphicalObjectBase {
public:
    /// Constructors
    GraphicalReaction() {
        _sReferenceInfo.clear();
    }
    
    /// Containers
    // graphical species references
    typedef std::vector<GraphicalSReference *> gSReferenceVec;
    
    /// Iterators
    // graphical species references
    typedef gSReferenceVec::iterator gSReferenceIt;
    typedef gSReferenceVec::const_iterator constGSReferenceIt;
    
    /// Functions related to beginning and end of containers
    // graphical species references
    const constGSReferenceIt gSReferencesBegin() const { return _sReferenceInfo.begin(); }
    const constGSReferenceIt gSReferencesEnd() const { return _sReferenceInfo.end(); }
    
    /// Functinos
    // add a new graphical species reference to list of species references
    void addGSReference(GraphicalSReference* sr);

    // remove a graphical species reference from list of species references using its index
    void removeGSReference(unsigned int graphicalSReferenceIndex);

    // set gSReferenceVec
    void setGSReferences(const gSReferenceVec& srv);

    // get gSReferenceVec
    const gSReferenceVec& getGSReferences() const;

    // get total number of graphical species references
    const size_t getNumGSReference() const;
    
    void updateValues(MainWindow* mw);
    
protected:
    gSReferenceVec _sReferenceInfo;
};

class GraphicalSReference : public GraphicalObjectBase {
public:
    /// Constructors
    GraphicalSReference() {
        _isSetRole = false;
    }
    
    /// Functinos
    // set the role of graphical species reference
    void setRole(const std::string& role);
    
    // unset the role of graphical species reference
    void unSetRole();

    // get the role of graphical species reference
    const std::string& getRole() const;
       
    // show whether the role is set
    const bool isSetRole() const { return _isSetRole; }
    
    // fit the species reference graphical curve to to the species position
    void fitToSpeciesPosition(MainWindow* mw);
    
    void updateValues(MainWindow* mw);
    
protected:
    std::string _role;
    bool _isSetRole;
};

class GraphicalText : public GraphicalObjectBase {
public:
    /// Constructors
    GraphicalText() {
        _associatedGObject = NULL;
        _isSetAssociatedGObject = false;
        _isSetPlainText = false;
    }
    
    // set the associated graphical object of graphical text
    void setAssociatedGObject(sbne::NGraphicalObject* gO);

    // get the associated graphical object of graphical text
    sbne::NGraphicalObject* getAssociatedGObject();
       
    // show whether the associated graphical object of graphical text is set
    const bool isSetAssociatedGObject() const { return _isSetAssociatedGObject; }
    
    // find the style and set it to the graphical text
    void setStyle(MainWindow* mw, const sbne::StyleType& styleType);
    
    // set the plain text of graphical text
    void setPlainText(const std::string& plainText);
    
    // unset the plain text of graphical text
    void unSetPlainText();

    // get the plain text of graphical text
    const std::string& getPlainText() const;
       
    // show whether the plain text is set
    const bool isSetPlainText() const { return _isSetPlainText; }
    
    void updateValues(MainWindow* mw);
    
protected:
    sbne::NGraphicalObject* _associatedGObject;
    std::string _plainText;
    bool _isSetAssociatedGObject;
    bool _isSetPlainText;
};

class GraphicalCurve : public GraphicalElement {
public:
    /// Constructors
    GraphicalCurve() {
        _graphicalPaths.clear();
        _lcurve = NULL;
        _rcurve = NULL;
        _boundingBox = NULL;
        _startGraphicalItems.clear();
        _startGraphicalItems.clear();
        _startSlope = 0.0;
        _endSlope = 0.0;
        _startEnableRotation = true;
        _endEnableRotation = true;
        _isSetLCurve = false;
        _isSetRCurve = false;
        _isSetBoundingBox = false;
        _isSetStartPoint = false;
        _isSetEndPoint = false;
        _isSetStartSlope = false;
        _isSetEndSlope = false;
        _isSetStartEnableRotation = false;
        _isSetEndEnableRotation = false;
        _isSetStartLineEnding = false;
        _isSetEndLineEnding = false;
    }
    
    /// Containers
    // graphical paths
    typedef std::vector<QGraphicsPathItem *> gPathVec;
    
    // graphical items
    typedef std::vector<QGraphicsItem *> gItemVec;
    
    /// Iterators
    // graphical paths
    typedef gPathVec::iterator gPathIt;
    typedef gPathVec::const_iterator constGPathIt;
    
    // graphical items
    typedef gItemVec::iterator gItemIt;
    typedef gItemVec::const_iterator constGItemIt;
    
    /// Functions related to beginning and end of containers
    // graphical paths
    const constGPathIt gPathsBegin() const { return _graphicalPaths.begin(); }
    const constGPathIt gPathsEnd() const { return _graphicalPaths.end(); }
    
    // start graphical items
    const constGItemIt startGItemsBegin() const { return _startGraphicalItems.begin(); }
    const constGItemIt startGItemsEnd() const { return _startGraphicalItems.end(); }
    
    // end graphical items
    const constGItemIt endGItemsBegin() const { return _endGraphicalItems.begin(); }
    const constGItemIt endGItemsEnd() const { return _endGraphicalItems.end(); }

    /// Functions
    // set the layout curve of graphical curve
    void setLCurve(sbne::LCurve* lc);
    
    // unset the layout curve of graphical curve
    void unSetLCurve();

    // get the layout curve of graphical curve
    sbne::LCurve* getLCurve();
    
    // show whether the layout curve is set
    const bool isSetLCurve() const { return _isSetLCurve; }
    
    // set the render curve of graphical curve
    void setRCurve(sbne::RCurve* rc);
    
    // unset the render curve of graphical curve
    void unSetRCurve();

    // get the render curve of graphical curve
    sbne::RCurve* getRCurve();
    
    // show whether the render curve is set
    const bool isSetRCurve() const { return _isSetRCurve; }
    
    // set the bounding box of graphical curve
    void setBoundingBox(sbne::LBox* b);
    
    // unset the bounding box of graphical curve
    void unSetBoundingBox();

    // get the bounding box of graphical curve
    sbne::LBox* getBoundingBox();
    
    // show whether the bounding box is set
    const bool isSetBoundingBox() const { return _isSetBoundingBox; }
    
    // add a new graphical path to list of graphical paths
    void addGraphicalPath(QGraphicsPathItem* p);

    // remove a graphical path from list of graphical paths using its index
    void removeGraphicalPath(unsigned int graphicalPathIndex);
    
    // remove all graphical paths from list of graphical paths
    void clearGraphicalPaths(MainWindow* mw, const bool& isLineEnding = false);

    // set gPathVec
    void setGraphicalPaths(const gPathVec& pv);

    // get gPathVec
    const gPathVec& getGraphicalPaths() const;

    // get total number of graphical paths
    const size_t getNumGraphicalPaths() const;
    
    // add a new graphical item to the list of start graphical items
    void addStartGraphicalItem(QGraphicsItem* g);
    
    // remove a graphical item from list of start graphical items using its index
    void removeStartGraphicalItem(unsigned int graphicalItemIndex);
    
    // set startGItemsVec
    void setStartGraphicalItems(const gItemVec& giv);

    // get startgItemsVec
    const gItemVec& getStartGraphicalItems() const;
    
    // remove all graphical items from the list of start graphical items
    void clearStartGraphicalItems(MainWindow* mw, const bool& isLineEnding = false);
    
    // add a new graphical item to the list of end graphical items
    void addEndGraphicalItem(QGraphicsItem* g);
    
    // remove a graphical item from list of end graphical items using its index
    void removeEndGraphicalItem(unsigned int graphicalItemIndex);
    
    // set endGItemsVec
    void setEndGraphicalItems(const gItemVec& giv);

    // get endgItemsVec
    const gItemVec& getEndGraphicalItems() const;
    
    // remove all graphical items from the list of end graphical items
    void clearEndGraphicalItems(MainWindow* mw, const bool& isLineEnding = false);
    
    
    /*
    // set the start graphical item of graphical curve
    void setStartGraphicalItem(QGraphicsItem* startGraphicalItem);
    
    // unset the start graphical item of graphical curve
    void unSetStartGraphicalItem(MainWindow* mw);

    // get the start graphical item of graphical curve
    QGraphicsItem* getStartGraphicalItem();
          
    // show whether the start graphical item of graphical curve is set
    const bool isSetStartGraphicalItem() const { return _isSetStartGraphicalItem; }
    
    // set the end graphical item of graphical curve
    void setEndGraphicalItem(QGraphicsItem* endGraphicalItem);
    
    // unset the end graphical item of graphical curve
    void unSetEndGraphicalItem(MainWindow* mw);

    // get the end graphical item of graphical curve
    QGraphicsItem* getEndGraphicalItem();
          
    // show whether the end graphical item of graphical curve is set
    const bool isSetEndGraphicalItem() const { return _isSetEndGraphicalItem; }
     */
    
    // set the start point of graphical curve
    void setStartPoint(const QPointF& sPoint);
    
    // unset the start point of graphical curve
    void unSetStartPoint();

    // get the start point of graphical curve
    const QPointF& getStartPoint() const;
       
    // show whether the start point is set
    const bool isSetStartPoint() const { return _isSetStartPoint; }
    
    // set the end point of graphical curve
    void setEndPoint(const QPointF& ePoint);
    
    // unset the end point of graphical curve
    void unSetEndPoint();

    // get the end point of graphical curve
    const QPointF& getEndPoint() const;
       
    // show whether the end point is set
    const bool isSetEndPoint() const { return _isSetEndPoint; }
    
    // set the start slope of graphical curve
    void setStartSlope(const qreal& sSlope);
    
    // unset the start slope of graphical curve
    void unSetStartSlope();

    // get the start slope of graphical curve
    const qreal& getStartSlope() const;
       
    // show whether the start slope is set
    const bool isSetStartSlope() const { return _isSetStartSlope; }
    
    // set the end slope of graphical curve
    void setEndSlope(const qreal& eSlope);
    
    // unset the end slope of graphical curve
    void unSetEndSlope();

    // get the end slope of graphical curve
    const qreal& getEndSlope() const;
       
    // show whether the end slope is set
    const bool isSetEndSlope() const { return _isSetEndSlope; }
    
    // set the enable rotation of start graphical item
    void setStartEnableRotation(const bool& enableRotation);
    
    // unset the enable rotation of start graphical item
    void unSetStartEnableRotation();

    // get the enable rotation of start graphical item
    const bool& getStartEnableRotation() const;
    
    // show whether the enable rotation of start graphical item is set
    const bool isSetStartEnableRotation() const { return _isSetStartEnableRotation; }
    
    // set the enable rotation of end graphical item
    void setEndEnableRotation(const bool& enableRotation);
    
    // unset the enable rotation of end graphical item
    void unSetEndEnableRotation();

    // get the enable rotation of end graphical item
    const bool& getEndEnableRotation() const;
    
    // show whether the enable rotation of end graphical item is set
    const bool isSetEndEnableRotation() const { return _isSetEndEnableRotation; }
    
    // set the start line ending of graphical curve
    void setStartLineEnding(const std::string& sLEnding);
    
    // unset the start line ending of graphical curve
    void unSetStartLineEnding();

    // get the start line ending of graphical curve
    const std::string& getStartLineEnding() const;
       
    // show whether the start line ending is set
    const bool isSetStartLineEnding() const { return _isSetStartLineEnding; }
    
    // set the end line ending of graphical curve
    void setEndLineEnding(const std::string& eLEnding);
    
    // unset the end line ending of graphical curve
    void unSetEndLineEnding();

    // get the end line ending of graphical curve
    const std::string& getEndLineEnding() const;
       
    // show whether the end line ending is set
    const bool isSetEndLineEnding() const { return _isSetEndLineEnding; }
    
    void updateValues(MainWindow* mw, sbne::VGlobalStyle* style, const bool& _setLineEndings);
    
protected:
    sbne::LCurve* _lcurve;
    sbne::RCurve* _rcurve;
    sbne::LBox* _boundingBox;
    gPathVec _graphicalPaths;
    gItemVec _startGraphicalItems;
    gItemVec _endGraphicalItems;
    QPointF _startPoint;
    QPointF _endPoint;
    qreal _startSlope;
    qreal _endSlope;
    bool _startEnableRotation;
    bool _endEnableRotation;
    std::string _startLineEnding;
    std::string _endLineEnding;
    bool _isSetLCurve;
    bool _isSetRCurve;
    bool _isSetBoundingBox;
    bool _isSetStartPoint;
    bool _isSetEndPoint;
    bool _isSetStartSlope;
    bool _isSetEndSlope;
    bool _isSetStartEnableRotation;
    bool _isSetEndEnableRotation;
    bool _isSetStartLineEnding;
    bool _isSetEndLineEnding;
};

/*
 class GraphicalLineEnding : public GraphicalElement {
 public:
     /// Constructors
     GraphicalLineEnding() {
         _graphicalItem = NULL;
         _lEnding = NULL;
         _enableRotation = true;
         _isSetGraphicalItem = false;
         _isSetLEnding = false;
         _isSetEnableRotation = false;
     }
     
     /// Functions
     // set the graphical item of graphical line ending
     void setGraphicalItem(QGraphicsItem* g);
     
     // unset the graphical item of graphical line ending
     void unSetGraphicalItem();

     // get the graphical item of graphical line ending
     QGraphicsItem* getGraphicalItem();
     
     // show whether the the graphical item is set
     const bool isSetGraphicalItem() const { return _isSetGraphicalItem; }
     
     // set the line ending of graphical line ending
     void setLEnding(sbne::VLineEnding* le);
     
     // unset the line ending of graphical line ending
     void unSetLEnding();

     // get the line ending of graphical line ending
     sbne::VLineEnding* getLEnding();
     
     // show whether the the line ending is set
     const bool isSetLEnding() const { return _isSetLEnding; }
     
     // set the enable rotation of graphical line ending
     void setRotation(const bool& rotation);
     
     // unset the enable rotation of graphical line ending
     void unSetRotation();

     // get the enable rotation of graphical line ending
     const bool& getRotation() const;
     
     // show whether the enable rotation is set
     const bool isSetEnableRotation() const { return _isSetEnableRotation; }
     
 protected:
     QGraphicsItem* _graphicalItem;
     sbne::VLineEnding* _lEnding;
     bool _enableRotation;
     bool _isSetGraphicalItem;
     bool _isSetLEnding;
     bool _isSetEnableRotation;
 };

 
 
 */


/*
class GraphicalCompartment : public GraphicalElement {
public:
    /// Constructors
    GraphicalCompartment() {
        //_graphicalItem = NULL;
        _compartment = NULL;
        _style = NULL;
        _graphicalText = NULL;
        _graphicalItems.clear();
        //_isSetGraphicalItem = false;
        _isSetCompartment = false;
        _isSetStyle = false;
        _isSetGraphicalText = false;
    }
    
    /// Containers
    // graphical items
    typedef std::vector<QGraphicsItem *> gItemVec;
    
    /// Iterators
    // graphical items
    typedef gItemVec::iterator gItemIt;
    typedef gItemVec::const_iterator constGItemIt;
    
    /// Functions related to beginning and end of containers
    // graphical items
    const constGItemIt gItemsBegin() const { return _graphicalItems.begin(); }
    const constGItemIt gItemsEnd() const { return _graphicalItems.end(); }
    
    /// Functinos
    // add a new graphical item to the list of graphical items
    void addGraphicalItems(QGraphicsItem* g);
    
    // remove a graphical item from list of graphical items using its index
    void removeGraphicalItems(unsigned int graphicalItemIndex);
    
    // set gItemsVec
    void setGraphicalItems(const gItemVec& giv);

    // get gItemsVec
    const gItemVec& getGraphicalItems() const;
    
    // set the graphical item of graphical compartment
    //void setGraphicalItem(QGraphicsItem* g);
    
    // unset the graphical item of graphical compartment
    //void unSetGraphicalItem();

    // get the graphical item of graphical compartment
    //QGraphicsItem* getGraphicalItem();
          
    // show whether the the graphical item is set
    //const bool isSetGraphicalItem() const { return _isSetGraphicalItem; }
    

    // set the compartment of graphical compartment
    void setCompartment(sbne::NCompartment* c);

    // get the compartment of graphical compartment
    sbne::NCompartment* getCompartment();
          
    // show whether the the compartment is set
    const bool isSetCompartment() const { return _isSetCompartment; }

    // set the style of graphical compartment
    void setStyle(sbne::VGlobalStyle* s);
    
    // find the style and set it to the graphical compartment
    void setStyle(MainWindow* mw, const bool& addNewStyle);
    
    // unset the style of graphical compartment
    void unSetStyle();

    // get the style of graphical compartment
    sbne::VGlobalStyle* getStyle();
       
    // show whether the style is set
    const bool isSetStyle() const { return _isSetStyle; }
    
    // set the graphical text of graphical compartment
    void setGText(GraphicalText* t);

    // get the graphical text of graphical compartment
    GraphicalText* getGText();
       
    // show whether the graphical text is set
    const bool isSetGText() const { return _isSetGraphicalText; }
    
    void updateValues(MainWindow* mw);
    
    void resetGraphicalItems(MainWindow* mw);
    
protected:
    //QGraphicsItem* _graphicalItem;
    gItemVec _graphicalItems;
    sbne::NCompartment* _compartment;
    sbne::VGlobalStyle* _style;
    GraphicalText* _graphicalText;
    //bool _isSetGraphicalItem;
    bool _isSetCompartment;
    bool _isSetStyle;
    bool _isSetGraphicalText;
};
 */

/*
class GraphicalSpecies : public GraphicalElement {
public:
    /// Constructors
    GraphicalSpecies() {
        //_graphicalItem = NULL;
        _species = NULL;
        _style = NULL;
        _graphicalText = NULL;
        _graphicalItems.clear();
        //_isSetGraphicalItem = false;
        _isSetSpecies = false;
        _isSetStyle = false;
        _isSetGraphicalText = false;
    }
    
    /// Containers
    // graphical items
    typedef std::vector<QGraphicsItem *> gItemVec;
    
    /// Iterators
    // graphical items
    typedef gItemVec::iterator gItemIt;
    typedef gItemVec::const_iterator constGItemIt;
    
    /// Functions related to beginning and end of containers
    // graphical items
    const constGItemIt gItemsBegin() const { return _graphicalItems.begin(); }
    const constGItemIt gItemsEnd() const { return _graphicalItems.end(); }
    
    /// Functinos
    // add a new graphical item to the list of graphical items
    void addGraphicalItems(QGraphicsItem* g);
    
    // remove a graphical item from list of graphical items using its index
    void removeGraphicalItems(unsigned int graphicalItemIndex);
    
    // set gItemsVec
    void setGraphicalItems(const gItemVec& giv);

    // get gItemsVec
    const gItemVec& getGraphicalItems() const;
    
    // set the graphical item of graphical species
    //void setGraphicalItem(QGraphicsItem* g);
    
    // unset the graphical item of graphical species
    //void unSetGraphicalItem();

    // get the graphical item of graphical species
    //QGraphicsItem* getGraphicalItem();
          
    // show whether the the graphical item is set
    //const bool isSetGraphicalItem() const { return _isSetGraphicalItem; }
    
    // set the species of graphical species
    void setSpecies(sbne::NSpecies* s);

    // get the species of graphical species
    sbne::NSpecies* getSpecies();
       
    // show whether the the species is set
    const bool isSetSpecies() const { return _isSetSpecies; }
    
    // set the style of graphical species
    void setStyle(sbne::VGlobalStyle* s);
    
    // find the style and set it to the graphical species
    void setStyle(MainWindow* mw, const bool& addNewStyle);
    
    // unset the style of graphical species
    void unSetStyle();

    // get the style of graphical species
    sbne::VGlobalStyle* getStyle();
       
    // show whether the the style is set
    const bool isSetStyle() const { return _isSetStyle; }
    
    // set the graphical text of graphical species
    void setGText(GraphicalText* t);

    // get the graphical text of graphical species
    GraphicalText* getGText();
       
    // show whether the graphical text is set
    const bool isSetGText() const { return _isSetGraphicalText; }
    
    // fit the species connected items to its bounding box
    void fitConnectedItemsToBoundingBox(MainWindow* mw);
    
    void updateValues(MainWindow* mw, const bool& _fitConnectedItems);
    
    void resetGraphicalItems(MainWindow* mw);
    
protected:
    //QGraphicsItem* _graphicalItem;
    gItemVec _graphicalItems;
    sbne::NSpecies* _species;
    sbne::VGlobalStyle* _style;
    GraphicalText* _graphicalText;
    //bool _isSetGraphicalItem;
    bool _isSetSpecies;
    bool _isSetStyle;
    bool _isSetGraphicalText;
};
 */

/*
class GraphicalReaction : public GraphicalElement {
public:
    /// Constructors
    GraphicalReaction() {
        _sReferenceInfo.clear();
        _gCurve = NULL;
        _graphicalItem = NULL;
        _reaction = NULL;
        _style = NULL;
        _isSetGCurve = false;
        _isSetGraphicalItem = false;
        _isSetReaction = false;
        _isSetStyle = false;
    }
    
    /// Containers
    // graphical species references
    typedef std::vector<GraphicalSReference *> gSReferenceVec;
    
    /// Iterators
    // graphical species references
    typedef gSReferenceVec::iterator gSReferenceIt;
    typedef gSReferenceVec::const_iterator constGSReferenceIt;
    
    /// Functions related to beginning and end of containers
    // graphical species references
    const constGSReferenceIt gSReferencesBegin() const { return _sReferenceInfo.begin(); }
    const constGSReferenceIt gSReferencesEnd() const { return _sReferenceInfo.end(); }

    /// Functions
    // add a new graphical species reference to list of species references
    void addGSReference(GraphicalSReference* sr);

    // remove a graphical species reference from list of species references using its index
    void removeGSReference(unsigned int graphicalSReferenceIndex);

    // set gSReferenceVec
    void setGSReferences(const gSReferenceVec& srv);

    // get gSReferenceVec
    const gSReferenceVec& getGSReferences() const;

    // get total number of graphical species references
    const size_t getNumGSReference() const;
    
    // set the graphical curve of graphical reaction
    void setGCurve(GraphicalCurve* c);
    
    // get the graphical curve of graphical reaction
    GraphicalCurve* getGCurve();
    
    // show whether the graphical curve is set
    const bool isSetGCurve() const { return _isSetGCurve; }
    
    // set the graphical item of graphical reaction
    void setGraphicalItem(QGraphicsItem* g);
    
    // unset the graphical item of graphical reaction
    void unSetGraphicalItem();

    // get the graphical item of graphical reaction
    QGraphicsItem* getGraphicalItem();
          
    // show whether the the graphical item is set
    const bool isSetGraphicalItem() const { return _isSetGraphicalItem; }
    
    // set the reaction of graphical reaction
    void setReaction(sbne::NReaction* r);

    // get the reaction of graphical reaction
    sbne::NReaction* getReaction();
          
    // show whether the reaction is set
    const bool isSetReaction() const { return _isSetReaction; }
    
    // set the style of graphical reaction
    void setStyle(sbne::VGlobalStyle* s);
    
    // find the style and set it to the graphical reaction
    void setStyle(MainWindow* mw, const bool& addNewStyle);
    
    // unset the style of graphical reaction
    void unSetStyle();

    // get the style of graphical reaction
    sbne::VGlobalStyle* getStyle();
          
    // show whether the style is set
    const bool isSetStyle() const { return _isSetStyle; }
    
    void updateValues(MainWindow* mw);
    
protected:
    gSReferenceVec _sReferenceInfo;
    GraphicalCurve* _gCurve;
    QGraphicsItem* _graphicalItem;
    sbne::NReaction* _reaction;
    sbne::VGlobalStyle* _style;
    bool _isSetGCurve;
    bool _isSetGraphicalItem;
    bool _isSetReaction;
    bool _isSetStyle;
};
 */

/*
class GraphicalSReference : public GraphicalElement {
public:
    /// Constructors
    GraphicalSReference() {
        _gCurve = NULL;
        _speciesReference = NULL;
        _style = NULL;
        _isSetGCurve = false;
        _isSetSReference = false;
        //_isSetSpecies = false;
        _isSetStyle = false;
        _isSetRole = false;
    }
    
    /// Functions
    // set the graphical curve of graphical species reference
    void setGCurve(GraphicalCurve* c);
    
    // get the graphical curve of graphical species reference
    GraphicalCurve* getGCurve();
    
    // show whether the graphical species reference is set
    const bool isSetGCurve() const { return _isSetGCurve; }
    
    // set the species reference of graphical species reference
    void setSReference(sbne::NSpeciesReference* sr);

    // get the species reference of graphical species reference
    sbne::NSpeciesReference* getSReference();
          
    // show whether the species reference is set
    const bool isSetSReference() const { return _isSetSReference; }
    
    // set the style of graphical species reference
    void setStyle(sbne::VGlobalStyle* s);
    
    // find the style and set it to the graphical species reference
    void setStyle(MainWindow* mw, const bool& addNewStyle);
    
    // unset the style of graphical species reference
    void unSetStyle();

    // get the style of graphical species reference
    sbne::VGlobalStyle* getStyle();
          
    // show whether the style is set
    const bool isSetStyle() const { return _isSetStyle; }
    
    // set the role of graphical species reference
    void setRole(const std::string& role);
    
    // unset the role of graphical species reference
    void unSetRole();

    // get the role of graphical species reference
    const std::string& getRole() const;
       
    // show whether the role is set
    const bool isSetRole() const { return _isSetRole; }
    
    // fit the species reference graphical curve to to the species position
    void fitToSpeciesPosition(MainWindow* mw);
    
    void updateValues(MainWindow* mw);
    
protected:
    GraphicalCurve* _gCurve;
    sbne::NSpeciesReference* _speciesReference;
    sbne::VGlobalStyle* _style;
    std::string _role;
    bool _isSetGCurve;
    bool _isSetSReference;
    //bool _isSetSpecies;
    bool _isSetStyle;
    bool _isSetRole;
};
 */

/*
class GraphicalText : public GraphicalElement {
public:
    /// Constructors
    GraphicalText() {
        _gTextItem = NULL;
        _text = NULL;
        _style = NULL;
        _isSetGTextItem = false;
        _isSetText = false;
        _isSetGObject = false;
        _isSetStyle = false;
        _isSetPlainText = false;
    }
    
    /// Functions
    // set the graphical text item of graphical text
    void setGTextItem(MyQGraphicsTextItem* t);
    
    // unset the graphical text item of graphical text
    void unSetGTextItem();

    // get the graphical text item of graphical text
    MyQGraphicsTextItem* getGTextItem();
          
    // show whether the the graphical text item is set
    const bool isSetGTextItem() const { return _isSetGTextItem; }
    
    // set the text glyph of graphical text
    void setText(sbne::NText* t);

    // get the text glyph of graphical text
    sbne::NText* getText();
       
    // show whether the text glyph is set
    const bool isSetText() const { return _isSetText; }
    
    // set the graphical object of graphical text
    void setGObject(sbne::NGraphicalObject* gO);

    // get the graphical object of graphical text
    sbne::NGraphicalObject* getGObject();
       
    // show whether the graphical object is set
    const bool isSetGObject() const { return _isSetGObject; }
    
    // set the style of graphical text
    void setStyle(sbne::VGlobalStyle* s);
    
    // find the style and set it to the graphical text
    void setStyle(MainWindow* mw, const bool& addNewStyle);
    
    // unset the style of graphical text
    void unSetStyle();

    // get the style of graphical text
    sbne::VGlobalStyle* getStyle();
          
    // show whether the style is set
    const bool isSetStyle() const { return _isSetStyle; }
    
    void updateValues(MainWindow* mw);
    
    // set the plain text of graphical text
    void setPlainText(const std::string& plainText);
    
    // unset the plain text of graphical text
    void unSetPlainText();

    // get the plain text of graphical text
    const std::string& getPlainText() const;
       
    // show whether the plain text is set
    const bool isSetPlainText() const { return _isSetPlainText; }
    
protected:
    MyQGraphicsTextItem* _gTextItem;
    sbne::NText* _text;
    sbne::NGraphicalObject* _gObject;
    sbne::VGlobalStyle* _style;
    std::string _plainText;
    bool _isSetGTextItem;
    bool _isSetText;
    bool _isSetGObject;
    bool _isSetStyle;
    bool _isSetPlainText;
};
 */

#endif
