#ifndef __NE_FEATUREMENU_H
#define __NE_FEATUREMENU_H

#include "ne_mainwindow.h"
#include "sbne/layout/ne_layout.h"
#include "sbne/render/ne_render.h"

using namespace sbne;

class MainWindow;
class FeatureMenu;
class FeatureMenuElement;
class SpeciesFeatureMenu;
class CompartmentFeatureMenu;
class ReactionReferenceFeatureMenu;
class SpeciesReferenceFeatureMenu;
class LineEndingFeatureMenu;
class GraphicalSpecies;
class GraphicalCompartment;
class GraphicalReaction;
class GraphicalSReference;
class GraphicalLineEnding;
class GraphicalText;
class BoundingBoxFeatureMenu;
class CurveFeatureMenu;
class StrokeFeatureMenu;
class FillFeatureMenu;
class HeadFeatureMenu;
class TextFeatureMenu;
class GeometricShapeFeatureMenuElement;
class RectangleGeometricShapeFeatureMenu;
class EllipseGeometricShapeFeatureMenu;
class PolygonGeometricShapeFeatureMenu;
class RenderCurveGeometricShapeFeatureMenu;
class LineFeatureMenu;
class PointFeatureMenu;
class CollapsibleMenu;
class SeparatingLine;

class FeatureMenu {
    
public:
    FeatureMenu(QWidget* parent = 0, MainWindow* mw = NULL);
    
    /// Containers
    // feature menu elemnt
    typedef std::vector<FeatureMenuElement *> fMenuElementVec;
    
    /// Iterators
    // feature menu element
    typedef fMenuElementVec::iterator fMenuElementIt;
    typedef fMenuElementVec::const_iterator constFMenuElementIt;
    
    /// Functions related to beginning and end of containers
    // feature menu element
    const constFMenuElementIt fMenuElementsBegin() const { return _featureMenuElements.begin(); }
    const constFMenuElementIt fMenuElementsEnd() const { return _featureMenuElements.end(); }
    
    /// Functions
    // get the feature menu elements
    const fMenuElementVec& getFeatureMenuElements() const;
    
    // get total number of feature menu elements
    const size_t getNumFeatureMenuElements() const;
    
    // show the the feature of a compartment on the menu
    void showFeatureMenu(GraphicalCompartment* gC);
    
    // show the the feature of a species on the menu
    void showFeatureMenu(GraphicalSpecies* gS);
    
    // show the the feature of a reaction on the menu
    void showFeatureMenu(GraphicalReaction* gR);
    
    // show the the feature of a species reference on the menu
    void showFeatureMenu(GraphicalSReference* gSR);
    
    // show the the feature of a line ending on the menu
    void showFeatureMenu(GraphicalLineEnding* gLE, GraphicalSReference* gSR, QString head);
    
    // hide all feature menu elemnts
    void hideFeatureMenu();
    
protected:
    fMenuElementVec _featureMenuElements;
};

class FeatureMenuElement : public QGroupBox {
    Q_OBJECT
    
public:
    FeatureMenuElement(QWidget* parent = 0, MainWindow* mw = NULL);
    
    void hideInfo();
    
    virtual void resetValues() = 0;
    
    virtual void updateStyle(VLocalStyle* style) {};
    
    virtual void setStyle() {};
    
    virtual void unSetStyle() {};
    
    virtual void updateValues() = 0;
    
    QScrollArea contentArea;
    
private slots:
    
    void changeLayoutId();
    
    void addOrRemoveBoundingBox();
    
    virtual void addOrRemoveCurve() {};
    
    void addOrRemoveStyle();
    
    void changeText();
    
    void changeGeometricShape();
    
    virtual void updateGeometricShape(const QString& shapeType, const int& numberOfPolygonVertices, const QString& imageFileName);
    
protected:
    QLabel itemLabel;
    //QGridLayout baseLayout;
    QGridLayout qLayout;
    QGridLayout modelContentLayout;
    QGridLayout layoutContentLayout;
    QGridLayout renderContentLayout;
    CollapsibleMenu* modelMenu;
    CollapsibleMenu* layoutMenu;
    CollapsibleMenu* renderMenu;
    
    // labels
    QLabel modelIdTitleLabel;
    QLabel layoutIdLabel;
    
    // model features
    QLabel modelIdLabel;
    
    // layout features
    QPushButton layoutIdButton;
    
    // bounding box features
    QLabel boundingBoxTitleLabel;
    QLabel boundingBoxLabel;
    QPushButton boundingBoxButton;
    BoundingBoxFeatureMenu* bBoxFMenu;
    
    // curve features
    QLabel curveTitleLabel;
    QLabel curveLabel;
    QPushButton curveButton;
    CurveFeatureMenu* curveFMenu;
    
    // style features
    QLabel styleLabel;
    QPushButton styleButton;
    
    // stroke features
    QLabel strokeTitleLabel;
    StrokeFeatureMenu* strokeFMenu;
    
    // fill features
    QLabel fillTitleLabel;
    FillFeatureMenu* fillFMenu;
    
    // text features
    QLabel textTitleLabel;
    QLabel textLabel;
    QPushButton textButton;
    TextFeatureMenu* textFMenu;
        
    // geometric shape
    QLabel geometricShapeTitleLabel;
    QLabel geometricShapeLabel;
    QPushButton geometricShapeButton;
    GeometricShapeFeatureMenuElement* gSFMenuElement;
    
    QFont featureTitleFont;
    QFont headlineFont;
    QString pushButtonStyleSheet;
    QString disabledPushButtonStyleSheet;
    MainWindow* _mw;
    GraphicalText* _gText;
    NGraphicalObject* _gO;
    VGlobalStyle* _style;
    VLineEnding* _lE;
};

class SpeciesFeatureMenu : public FeatureMenuElement {
    Q_OBJECT
    
public:
    SpeciesFeatureMenu(QWidget* parent = 0, MainWindow* mw = NULL);
    
    void showInfo(GraphicalSpecies* gS);
    
    void resetValues();
    
private slots:
    
    virtual void updateStyle(VLocalStyle* style);
    
    virtual void setStyle();
    
    virtual void unSetStyle();
    
    virtual void updateValues();
    
protected:
    // info
    GraphicalSpecies* gSpecies;
    
    // labels
    QLabel compartmentTitleLabel;
    
    // model features
    QLabel compartmentLabel;
};

class CompartmentFeatureMenu : public FeatureMenuElement {
    Q_OBJECT
    
public:
    CompartmentFeatureMenu(QWidget* parent = 0, MainWindow* mw = NULL);
    
    void showInfo(GraphicalCompartment* gC);
    
    void resetValues();
    
private slots:
    
    virtual void updateStyle(VLocalStyle* style);
    
    virtual void setStyle();
    
    virtual void unSetStyle();
    
    virtual void updateValues();
    
protected:
    // info
    GraphicalCompartment* gCompartment;
};

class ReactionFeatureMenu : public FeatureMenuElement {
    Q_OBJECT
    
public:
    ReactionFeatureMenu(QWidget* parent = 0, MainWindow* mw = NULL);
    
    void showInfo(GraphicalReaction* gR);
    
    void resetValues();
    
private slots:
    
    virtual void addOrRemoveCurve();
    
    virtual void updateStyle(VLocalStyle* style);
    
    virtual void setStyle();
    
    virtual void unSetStyle();
    
    virtual void updateValues();
    
protected:
    // info
    GraphicalReaction* gReaction;
};

class SpeciesReferenceFeatureMenu : public FeatureMenuElement {
    Q_OBJECT
    
public:
    SpeciesReferenceFeatureMenu(QWidget* parent = 0, MainWindow* mw = NULL);
    
    void showInfo(GraphicalSReference* gSR);
    
    void resetValues();
    
private slots:
    
    virtual void addOrRemoveCurve();
    
    virtual void updateStyle(VLocalStyle* style);
    
    virtual void setStyle();
    
    virtual void unSetStyle();
    
    virtual void updateValues();
    
protected:
    GraphicalSReference* gSReference;
    
    // labels
    QLabel modelReactionTitleLabel;
    QLabel modelSpeciesTitleLabel;
    QLabel modelRoleTitleLabel;

    // model features
    QLabel modelReactionLabel;
    QLabel modelSpeciesLabel;
    QLabel modelRoleLabel;
    
    // head features
    QLabel headTitleLabel;
    HeadFeatureMenu* headFMenu;
};

class LineEndingFeatureMenu : public FeatureMenuElement {
    Q_OBJECT
    
public:
    LineEndingFeatureMenu(QWidget* parent = 0, MainWindow* mw = NULL);
    
    void showInfo(GraphicalLineEnding* gLE, GraphicalSReference* gSR, QString head);
    
    void resetValues();
    
private slots:
    
    void updateLineEnding(VLineEnding* lineEnding);
    
    virtual void updateGeometricShape(const QString& shapeType, const int& numberOfPolygonVertices, const QString& imageFileName);
    
    virtual void updateValues();
    
    void changeRenderId();
    
    void changeEnableRotation();
    
protected:
    // info
    GraphicalLineEnding* gLineEnding;
    GraphicalSReference* gSReference;
    QString _head;
    
    // labels
    QLabel positionLabel;
    QLabel positionXLabel;
    QLabel positionYLabel;
    QLabel renderIdLabel;
    QLabel boundingBoxLabel;
    QLabel boundingBoxXLabel;
    QLabel boundingBoxYLabel;
    QLabel boundingBoxWidthLabel;
    QLabel boundingBoxHeightLabel;
    QLabel graphicalShapeLabel;
    QLabel enableRotationLabel;
    QLabel strokeColorLabel;
    QLabel strokeWidthLabel;
    QLabel fillColorLabel;

    // layout features
    QPushButton positionXButton;
    QPushButton positionYButton;
    
    // render features
    QPushButton renderIdButton;
    QPushButton boundingBoxXButton;
    QPushButton boundingBoxYButton;
    QPushButton boundingBoxWidthButton;
    QPushButton boundingBoxHeightButton;
    QPushButton graphicalShapeButton;
    QPushButton enableRotationButton;
    QPushButton strokeColorButton;
    QPushButton strokeWidthButton;
    QPushButton fillColorButton;
};

class BoundingBoxFeatureMenu : public QGroupBox {
    Q_OBJECT
    
public:
    BoundingBoxFeatureMenu(QWidget* parent = 0, MainWindow* mw = NULL);
    
    void showInfo(NGraphicalObject* gO);
    
    void showInfo(VLineEnding* lE);
    
    void resetValues();
    
signals:
    
    void bBoxChanged();
    
    void bBoxDimensionsChanged(QString shapeType, int numberOfPolygonVertices, QString imageFileName);
    
    void lEndingChanged(VLineEnding* lineEnding);
    
private slots:
    
    void changePositionX();
    
    void changePositionY();
    
    void changeDimensionWidth();
    
    void changeDimensionHeight();
    
protected:
    
    // labels
    QLabel positionLabel;
    QLabel positionXLabel;
    QLabel positionYLabel;
    QLabel dimensionLabel;
    QLabel dimensionWidthLabel;
    QLabel dimensionHeightLabel;
    
    // features
    QPushButton positionXButton;
    QPushButton positionYButton;
    QPushButton dimensionWidthButton;
    QPushButton dimensionHeightButton;
    
    QGridLayout bBoxLayout;
    QFont featureTitleFont;
    QString pushButtonStyleSheet;
    MainWindow* _mw;
    NGraphicalObject* _gO;
    VLineEnding* _lE;
};

class CurveFeatureMenu : public QGroupBox  {
    Q_OBJECT
    
public:
    CurveFeatureMenu(QWidget* parent = 0, MainWindow* mw = NULL);
    
    /// Containers
    // line feature menu
    typedef std::vector<LineFeatureMenu*> lineFeatureMenuVec;
    
    /// Iterators
    // line feature menu
    typedef lineFeatureMenuVec::iterator lineFeatureMenuIt;
    typedef lineFeatureMenuVec::const_iterator constLineFeatureMenuIt;
    
    /// Functions related to beginning and end of containers
    // line feature menu
    const constLineFeatureMenuIt lineFeatureMenusBegin() const { return _lineFeatureMenus.begin(); }
    const constLineFeatureMenuIt lineFeatureMenusEnd() const { return _lineFeatureMenus.end(); }
    
    /// Functions
    void showInfo(LCurve* curve, const bool& askForElementForEmptyCurve);
    
    void resetValues();
    
signals:
    void curveChanged();
    
private slots:
    
    void changeCurve();
    
    void addLineFeatureMenu();
    
    void removeLineFeatureMenu();
    
protected:
    QPushButton addLineButton;
    QPushButton removeLineButton;
    lineFeatureMenuVec _lineFeatureMenus;
    
    QGridLayout curveLayout;
    QFont featureTitleFont;
    QString pushButtonStyleSheet;
    QString disabledPushButtonStyleSheet;
    MainWindow* _mw;
    LCurve* _curve;
};

class StrokeFeatureMenu : public QGroupBox {
    Q_OBJECT
    
public:
    StrokeFeatureMenu(QWidget* parent = 0, MainWindow* mw = NULL);
    
    void showInfo(NGraphicalObject* gO, VGlobalStyle* style);
    
    void showInfo(VTransformation2D* gS);
    
    void resetValues();
    
signals:
    
    void styleChanged(VLocalStyle* style);
    
private slots:
    void changeStrokeColor();
    
    void changeStrokeWidth();
    
    void changeStrokeDashArray();
    
protected:
    
    // labels
    QLabel strokeColorLabel;
    QLabel strokeWidthLabel;
    QLabel strokeDashArrayLabel;
    
    // render features
    QPushButton strokeColorButton;
    QPushButton strokeWidthButton;
    QPushButton strokeDashArrayButton;
    
    QGridLayout strokeLayout;
    QFont featureTitleFont;
    QString pushButtonStyleSheet;
    MainWindow* _mw;
    NGraphicalObject* _gO;
    VGlobalStyle* _style;
    VTransformation2D* _gS;
};

class FillFeatureMenu : public QGroupBox {
    Q_OBJECT
    
public:
    FillFeatureMenu(QWidget* parent = 0, MainWindow* mw = NULL);
    
    void showInfo(NGraphicalObject* gO, VGlobalStyle* style);
    
    void showInfo(VLineEnding* lE);
    
    void showInfo(VTransformation2D* gS);
    
    void resetValues();
    
signals:
    
    void styleChanged(VLocalStyle* style);
    
    void lEndingChanged(VLineEnding* lineEnding);
    
private slots:
    void changeFillColor();
    
protected:
    
    // labels
    QLabel fillColorLabel;
    
    // render features
    QPushButton fillColorButton;
    
    QGridLayout fillLayout;
    QFont featureTitleFont;
    QString pushButtonStyleSheet;
    QString disabledPushButtonStyleSheet;
    MainWindow* _mw;
    NGraphicalObject* _gO;
    VGlobalStyle* _style;
    VLineEnding* _lE;
    VTransformation2D* _gS;
};

class HeadFeatureMenu : public QGroupBox {
    Q_OBJECT
    
public:
    HeadFeatureMenu(QWidget* parent = 0, MainWindow* mw = NULL);
    
    void showInfo(NGraphicalObject* gO, VGlobalStyle* style);
    
    void resetValues();
    
signals:
    
    void styleChanged(VLocalStyle* style);
    
private slots:
    void changeStartHead();
    
    void changeEndHead();
    
    void updateValues();
    
protected:
    
    // labels
    QLabel startHeadLabel;
    QLabel endHeadLabel;
    
    // render features
    QPushButton startHeadButton;
    QPushButton endHeadButton;
    
    QGridLayout headLayout;
    QFont featureTitleFont;
    QString pushButtonStyleSheet;
    QString disabledPushButtonStyleSheet;
    MainWindow* _mw;
    NGraphicalObject* _gO;
    VGlobalStyle* _style;
};

class TextFeatureMenu : public QGroupBox {
    Q_OBJECT
    
public:
    TextFeatureMenu(QWidget* parent = 0, MainWindow* mw = NULL);
    
    void showInfo(GraphicalText* gT, NGraphicalObject* gO);
    
    void resetValues();
    
signals:
    
    void textChanged();
    
private slots:
    void changeFontSize();
    
    void changeFontFamily();
    
    void changeFontWeight();
    
    void changeFontStyle();
    
    void changeFontColor();
    
    void changeTextAnchor();
    
    void changeVTextAnchor();
    
protected:
    
    // labels
    QLabel fontSizeLabel;
    QLabel fontFamilyLabel;
    QLabel fontWeightLabel;
    QLabel fontStyleLabel;
    QLabel fontColorLabel;
    QLabel textAnchorLabel;
    QLabel vTextAnchorLabel;
    
    // features
    QPushButton fontSizeButton;
    QPushButton fontFamilyButton;
    QPushButton fontWeightButton;
    QPushButton fontStyleButton;
    QPushButton fontColorButton;
    QPushButton textAnchorButton;
    QPushButton vTextAnchorButton;
    
    QGridLayout textLayout;
    QFont featureTitleFont;
    QString pushButtonStyleSheet;
    QString disabledPushButtonStyleSheet;
    MainWindow* _mw;
    GraphicalText* gText;
    NGraphicalObject* _gO;
};

class GeometricShapeFeatureMenuElement : public QGroupBox {
    Q_OBJECT
    
public:
    GeometricShapeFeatureMenuElement(QWidget* parent = 0, MainWindow* mw = NULL);
    
signals:
    
    void styleChanged(VLocalStyle* style);
    
    void lEndingChanged(VLineEnding* lineEnding);
    
private slots:
    
protected:
    QGridLayout gShapeLayout;
    QFont featureTitleFont;
    QString pushButtonStyleSheet;
    QString disabledPushButtonStyleSheet;
    MainWindow* _mw;
    NGraphicalObject* _gO;
    VGlobalStyle* _style;
    VLineEnding* _lE;
};

class RectangleGeometricShapeFeatureMenu : public GeometricShapeFeatureMenuElement {
    Q_OBJECT
    
public:
    RectangleGeometricShapeFeatureMenu(QWidget* parent = 0, MainWindow* mw = NULL);
    
    void showInfo(NGraphicalObject* gO, VGlobalStyle* style);
    
    void showInfo(VLineEnding* lE);
    
    void resetValues();
    
    void getInfoFromRectangleShape(VRectangle* rectangle);
    
private slots:
    
    void changePositionX();
    
    void changePositionY();

    void changeDimensionWidth();
    
    void changeDimensionHeight();
    
    void changeDimensionRatio();
    
    void changeCornerCurvatureRX();
    
    void changeCornerCurvatureRY();
    
protected:
    // labels
    QLabel positionLabel;
    QLabel positionXLabel;
    QLabel positionYLabel;
    QLabel dimensionLabel;
    QLabel dimensionWidthLabel;
    QLabel dimensionHeightLabel;
    QLabel dimensionRatioLabel;
    QLabel cornerCurvatureLabel;
    QLabel cornerCurvatureRXLabel;
    QLabel cornerCurvatureRYLabel;
    
    // features
    QPushButton positionXButton;
    QPushButton positionYButton;
    QPushButton dimensionWidthButton;
    QPushButton dimensionHeightButton;
    QPushButton dimensionRatioButton;
    QPushButton cornerCurvatureRXButton;
    QPushButton cornerCurvatureRYButton;
};

class ImageGeometricShapeFeatureMenu : public GeometricShapeFeatureMenuElement {
    Q_OBJECT
    
public:
    ImageGeometricShapeFeatureMenu(QWidget* parent = 0, MainWindow* mw = NULL);
    
    void showInfo(NGraphicalObject* gO, VGlobalStyle* style);
    
    void showInfo(VLineEnding* lE);
    
    void resetValues();
    
    void getInfoFromImageShape(VImage* image);
    
private slots:
    
    void changePositionX();
    
    void changePositionY();

    void changeDimensionWidth();
    
    void changeDimensionHeight();
    
    void changeHref();
    
protected:
    // labels
    QLabel positionLabel;
    QLabel positionXLabel;
    QLabel positionYLabel;
    QLabel dimensionLabel;
    QLabel dimensionWidthLabel;
    QLabel dimensionHeightLabel;
    QLabel pathLabel;
    QLabel hrefLabel;
    
    // features
    QPushButton positionXButton;
    QPushButton positionYButton;
    QPushButton dimensionWidthButton;
    QPushButton dimensionHeightButton;
    QPushButton hrefButton;
    
    QString href;
};

class EllipseGeometricShapeFeatureMenu : public GeometricShapeFeatureMenuElement {
    Q_OBJECT
    
public:
    EllipseGeometricShapeFeatureMenu(QWidget* parent = 0, MainWindow* mw = NULL);
    
    void showInfo(NGraphicalObject* gO, VGlobalStyle* style);
    
    void showInfo(VLineEnding* lE);
    
    void resetValues();
    
    void getInfoFromEllipseShape(VEllipse* ellipse);
    
private slots:
    
    void changePositionCX();
    
    void changePositionCY();
    
    void changeDimensionRX();
    
    void changeDimensionRY();
    
    void changeDimensionRatio();
    
protected:
    // labels
    QLabel positionLabel;
    QLabel positionCXLabel;
    QLabel positionCYLabel;
    QLabel dimensionLabel;
    QLabel dimensionRXLabel;
    QLabel dimensionRYLabel;
    QLabel dimensionRatioLabel;
    
    // features
    QPushButton positionCXButton;
    QPushButton positionCYButton;
    QPushButton dimensionRXButton;
    QPushButton dimensionRYButton;
    QPushButton dimensionRatioButton;
};

class PolygonGeometricShapeFeatureMenu : public GeometricShapeFeatureMenuElement {
    Q_OBJECT
    
public:
    PolygonGeometricShapeFeatureMenu(QWidget* parent = 0, MainWindow* mw = NULL);
    
    /// Containers
    // point feature menu
    typedef std::vector<PointFeatureMenu*> pointFeatureMenuVec;
    
    /// Iterators
    // point feature menu
    typedef pointFeatureMenuVec::iterator pointFeatureMenuIt;
    typedef pointFeatureMenuVec::const_iterator constPointFeatureMenuIt;
    
    /// Functions related to beginning and end of containers
    // point feature menu
    const constPointFeatureMenuIt pointFeatureMenusBegin() const { return _pointFeatureMenus.begin(); }
    const constPointFeatureMenuIt pointFeatureMenusEnd() const { return _pointFeatureMenus.end(); }
    
    /// Functions
    virtual void showInfo(NGraphicalObject* gO, VGlobalStyle* style);
    
    virtual void showInfo(VLineEnding* lE);

    void resetValues();
    
    void getInfoFromGShape(VPolygon* polygon);
    
signals:
    
private slots:
    
    void changeStyle(VLocalStyle* style);
    
    void changeLEnding(VLineEnding* lineEnding);
    
    void addPointFeatureMenu();
    
    void removePointFeatureMenu();
    
protected:
    QPushButton addPointButton;
    QPushButton removePointButton;
    RenderGroupElementShape gShapeType;
    pointFeatureMenuVec _pointFeatureMenus;
};

class RenderCurveGeometricShapeFeatureMenu : public PolygonGeometricShapeFeatureMenu {
    Q_OBJECT
    
public:
    RenderCurveGeometricShapeFeatureMenu(QWidget* parent = 0, MainWindow* mw = NULL);
    
    /// Functions
    virtual void showInfo(NGraphicalObject* gO, VGlobalStyle* style);
    
    virtual void showInfo(VLineEnding* lE);
    
    void getInfoFromGShape(RCurve* rcurve);
};

class LineFeatureMenu : public QGroupBox {
    Q_OBJECT
    
public:
    LineFeatureMenu(QWidget* parent = 0, MainWindow* mw = NULL, const unsigned int& pointIndex = 0);
    
    void showInfo(LLineSegment* line);
    
    void resetValues();
    
signals:
    
    void lineChanged();
    
private slots:
    
    void changeStartX();
    
    void changeStartY();
    
    void changeEndX();
    
    void changeEndY();
    
    void changeBasePoint1X();
    
    void changeBasePoint1Y();
    
    void changeBasePoint2X();
    
    void changeBasePoint2Y();
    
protected:
    // labels
    QLabel lineLabel;
    QLabel startLabel;
    QLabel startXLabel;
    QLabel startYLabel;
    QLabel endLabel;
    QLabel endXLabel;
    QLabel endYLabel;
    QLabel basePoint1Label;
    QLabel basePoint1XLabel;
    QLabel basePoint1YLabel;
    QLabel basePoint2Label;
    QLabel basePoint2XLabel;
    QLabel basePoint2YLabel;
    
    // featurs
    QPushButton startXButton;
    QPushButton startYButton;
    QPushButton endXButton;
    QPushButton endYButton;
    QPushButton basePoint1XButton;
    QPushButton basePoint1YButton;
    QPushButton basePoint2XButton;
    QPushButton basePoint2YButton;
    
    QFont featureTitleFont;
    QString pushButtonStyleSheet;
    QGridLayout lineFeatureLayout;
    LLineSegment* _line;
    MainWindow* _mw;
    
    unsigned int _lineIndex;
};

class PointFeatureMenu : public QGroupBox {
    Q_OBJECT
    
public:
    PointFeatureMenu(QWidget* parent = 0, MainWindow* mw = NULL, const unsigned int& pointIndex = 0);
    
    void showInfo(NGraphicalObject* gO, VGlobalStyle* style, RenPoint* renderPoint);
    
    void showInfo(VLineEnding* lE, RenPoint* renderPoint);
    
    void resetValues();
    
    void getInfoFromRenderPoint(RenPoint* renderPoint);
    
signals:
    
    void styleChanged(VLocalStyle* style);
    
    void lEndingChanged(VLineEnding* lineEnding);
    
private slots:
    
    void changeRPointX();
    
    void changeRPointY();
    
    void changeBasePoint1X();
    
    void changeBasePoint1Y();
    
    void changeBasePoint2X();
    
    void changeBasePoint2Y();
    
protected:
    QLabel pointLabel;
    QLabel pointXLabel;
    QLabel pointYLabel;
    QLabel basePoint1XLabel;
    QLabel basePoint1YLabel;
    QLabel basePoint2XLabel;
    QLabel basePoint2YLabel;
    QPushButton pointXButton;
    QPushButton pointYButton;
    QPushButton basePoint1XButton;
    QPushButton basePoint1YButton;
    QPushButton basePoint2XButton;
    QPushButton basePoint2YButton;
    
    QFont featureTitleFont;
    QString pushButtonStyleSheet;
    QGridLayout pointFeatureLayout;
    RenPoint* _renderPoint;
    MainWindow* _mw;
    NGraphicalObject* _gO;
    VGlobalStyle* _style;
    VLineEnding* _lE;
    
    unsigned int _pointIndex;
};

class CollapsibleMenu : public QWidget {
    Q_OBJECT
    
public:
    CollapsibleMenu(QWidget* parent, const QString& title, const int& animationDuration);
    
    void setContentLayout(QLayout& contentLayout);
    
    void collapseMenuWithoutAnimation();
  
private slots:
    
protected:
    QGridLayout _mainLayout;
    QToolButton _toggleButton;
    QFrame _headerLine;
    QParallelAnimationGroup _toggleAnimation;
    QScrollArea _contentArea;
    int _animationDuration;
};

class SeparatingLine : public QFrame {
    Q_OBJECT
    
public:
    SeparatingLine(QWidget* parent);
  
private slots:
    
protected:
    
};

#endif // GFFEAUTREMENU_H
