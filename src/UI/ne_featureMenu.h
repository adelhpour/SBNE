#ifndef __NE_FEATUREMENU_H
#define __NE_FEATUREMENU_H

#include "ne_mainwindow.h"
#include "sbne/layout/ne_layout.h"
#include "sbne/render/ne_render.h"

using namespace sbne;

class MainWindow;
class GraphicalObjectBase;
class GraphicalSpecies;
class GraphicalCompartment;
class GraphicalReaction;
class GraphicalSReference;
class GraphicalLineEnding;
class GraphicalText;
class FeatureMenu;
class FeatureMenuElement;
class ItemFeatureMenuElement;
class SpeciesFeatureMenu;
class CompartmentFeatureMenu;
class ReactionReferenceFeatureMenu;
class SpeciesReferenceFeatureMenu;
class LineEndingFeatureMenu;
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
class RenderPointFeatureMenu;
class RenderCubicBezierFeatureMenu;

class MyLabel : public QLabel {
    
public:
    MyLabel(const QString& text = "", QWidget* parent = 0);
};

class MyLineEdit : public QLineEdit {
    
public:
    MyLineEdit(QWidget* parent = 0);
    
    void setText(const QString& text);
    
    void insert(const QString& text);
};

class MySpinBox : public QSpinBox {
    
public:
    MySpinBox(QWidget* parent = 0);
};

class MyDoubleSpinBox : public QDoubleSpinBox {
    
public:
    MyDoubleSpinBox(QWidget* parent = 0);
};

class MyDualSpinBox : public QWidget {
    Q_OBJECT
    
public:
    MyDualSpinBox(const QString& type = "");
    
    const double first() const;
    
    const double second() const;
    
    void setType(const QString& type);
    
    void setCurrentValue(const double& first, const double& second);
    
    void setRange(MainWindow* mw);
    
    void setFirstRange(const double& min, const double& max);
    
    void setSecondRange(const double& min, const double& max);
    
    void resetValues();
    
signals:
    void valueChanged(const double& first, const double& second);
    
protected:
    QGridLayout contentLayout;
    QString _type;
    MyLabel firstLabel;
    MyLabel secondLabel;
    MyDoubleSpinBox firstSpinBox;
    MyDoubleSpinBox secondSpinBox;
};

class MyRelAbsSpinBox : public QWidget {
    Q_OBJECT
    
public:
    MyRelAbsSpinBox();
    
    const double absoluteValue() const;
    
    const int relativeValue() const;
    
    void setCurrentValue(const double& abs, const double& rel);
    
    void setRange(const double& min, const double& max);
    
    void resetValues();
    
signals:
    void valueChanged(const double& abs, const double& rel);
    
protected:
    QGridLayout contentLayout;
    MyDoubleSpinBox absoluteValueSpinBox;
    MySpinBox relativeValueSpinBox;
};

class MyPlainTextWidget : public QWidget {
    Q_OBJECT
    
public:
    MyPlainTextWidget();
    
    void setPlainText(const QString& plainText);
    
    QString plainText();
    
    void enable(NGraphicalObject* gO);
    
    void resetValues();
    
signals:
    void plainTextChanged(const QString& plainText);
    
protected:
    QGridLayout contentLayout;
    MyLineEdit plainTextLineEdit;
    QPushButton importTextPushButton;
    QMenu importMenu;
};

class MyComboBox : public QComboBox {
    
public:
    MyComboBox(QWidget* parent = 0);
};

class MyFontSizeComboBox : public QWidget {
    Q_OBJECT
    
public:
    MyFontSizeComboBox();
    
    void setCurrentValue(const double& abs, const double& rel);
    
    void resetValues();
    
signals:
    void valueChanged(const QString& abs, const QString& rel);
    
protected:
    QGridLayout contentLayout;
    MyComboBox absoluteValueComboBox;
    MyComboBox relativeValueComboBox;
};

class MyDashArrayComboBox : public MyComboBox {
    Q_OBJECT
    
public:
    MyDashArrayComboBox(QWidget* parent = 0);
    
    void setDashArray(std::vector<unsigned int>* dashArrayVector);
    
    void resetValues();
    
signals:
    void dashArrayChosen(std::vector<unsigned int>* dashArrayVector);
};

class MyHeadPickerComboBox : public QComboBox {
    Q_OBJECT
    
public:
    MyHeadPickerComboBox(QWidget* parent = 0);
    
    void enableItems(MainWindow* _mw);
    
    void setHead(const QString& head);
    
    void resetValues();
    
signals:
    void headChosen(const QString& head);
};

class MyDimensionRatioComboBox : public MyComboBox {
    Q_OBJECT
    
public:
    MyDimensionRatioComboBox(QWidget* parent = 0);
    
    void setRatio(const double& ratio);
    
    void resetValues();
    
signals:
    void ratioChosen(const double& ratio);
};

class MyPauseOrResumeButton : public QPushButton {
    Q_OBJECT
    
public:
    MyPauseOrResumeButton(QWidget* parent = 0);
    
    void enable();
    
    void disable();
    
    void mousePressEvent(QMouseEvent *e);
    
signals:
    
    void paused();
    
    void resumed();
    
private slots:
    
    void pause();
    
    void resume();
};

class MyColorTileButton : public QPushButton {
        
public:
    MyColorTileButton(const QString& color, const QString& value, QWidget* parent = 0);
    
    const QString& color() const;
    
    const QString& value() const;
    
protected:
    QString _color;
    QString _value;
};

class MyColorPickerMenu : public QMenu {
    Q_OBJECT
    
public:
    MyColorPickerMenu();
    
signals:
    void colorChosen(const QString& color, const QString& value);
        
private slots:
    void colorTileButtonPicked(MyColorTileButton* colorTileButton);
};

class MyColorPickerButton : public QToolButton {
    Q_OBJECT
        
public:
    MyColorPickerButton(MainWindow* mw);
    
    void setBackgroundColor(const QString& color);
    
signals:
    void colorChosen(const QString& color, const QString& value);
    
private slots:
    
    void setColorMenu() ;
    
protected:
    MainWindow* _mw;
};


class MyAddOrRemoveButton : public QPushButton {
    Q_OBJECT
    
public:
    MyAddOrRemoveButton(QWidget* parent = 0);
    
    void enableRemove();
    
    void resetValues();
};

class MyAddOrRemoveReactionBoundingBoxButton : public MyAddOrRemoveButton {
    Q_OBJECT
    
public:
    MyAddOrRemoveReactionBoundingBoxButton(MainWindow* mw = NULL, QWidget* parent = 0);
    
    void enableFromCurveExtents(LBox* extentBox);
    
    void initialize(LBox* box);
    
    void enableRemove();
    
    void setBoxRanges(MainWindow* mw);
    
    void resetValues();
    
signals:
    void boundingBoxFeaturesChosen(const double& x, const double& y, const double& width, const double& height);
    
    void removeBoundingBox();
    
protected:
    QMenu* addingMenu;
    QPushButton fromCurveExtentsButton;
    MyDualSpinBox positionDualSpinBox;
    MyDualSpinBox dimensionsDualSpinBox;
};

class MyAddRemoveButtons : public QDialogButtonBox {
    Q_OBJECT
    
public:
    MyAddRemoveButtons(QWidget* parent = 0);
    
signals:
    void removeItemFeaturesChosen(const unsigned int& itemIndex);
    
protected:
    QPushButton* addPushButton;
    QPushButton* removePushButton;
    QMenu* addingMenu;
    QMenu* removingMenu;
};

class MyAddRemoveTextButtons : public MyAddRemoveButtons {
    Q_OBJECT
    
public:
    MyAddRemoveTextButtons(QWidget* parent = 0);
    
    void enableAddButton(NGraphicalObject* gO);
    
    void enableRemoveButton(const unsigned int& numberOfElements);
    
    void resetValues();
    
signals:
    void textChosen(const QString& plainText);

protected:
    MyPlainTextWidget* plainTextWidget;
};

class MyAddRemoveGeometricShapeButtons : public MyAddRemoveButtons {
    Q_OBJECT
    
public:
    MyAddRemoveGeometricShapeButtons(QWidget* parent = 0);
        
    void enableAddButton();
    
    void enableRemoveButton(VRenderGroup* group);
    
    void resetValues(const bool& disableAddButton = false);
    
signals:
    void geometricShapeChosen(const QString& geometricShape);
};

class MyAddRemoveSegmentButtons : public MyAddRemoveButtons {
    Q_OBJECT
    
public:
    
    MyAddRemoveSegmentButtons(QWidget* parent = 0);
        
    void enableAddButton(MainWindow* mw, const unsigned int& numberOfElements);
    
    void enableRemoveButton(const unsigned int& numberOfElements);
    
    void resetValues();
        
signals:
    void addItemFeaturesChosen(const unsigned int& itemIndex, const double& itemPointStartXValue, const double& itemPointStartYValue, const double& itemPointEndXValue, const double& itemPointEndYValue);
        
    void addItemFeaturesChosen(const unsigned int& itemIndex, const double& itemCubicBezierStartXValue, const double& itemCubicBezierStartYValue, const double& itemCubicBezierEndXValue, const double& itemCubicBezierEndYValue, const double& itemCubicBezierBasePoint1XValue, const double& itemCubicBezierBasePoint1YValue, const double& itemCubicBezierBasePoint2XValue, const double& itemCubicBezierBasePoint2YValue);
};


class MyAddRemoveVertexButtons : public MyAddRemoveButtons {
    Q_OBJECT
    
public:
    
    MyAddRemoveVertexButtons(QWidget* parent = 0);
        
    void enableAddButton(MainWindow* mw, LBox* bBox, const unsigned int& numberOfElements);
    
    void enableRemoveButton(const unsigned int& numberOfElements);
    
    void resetValues();
        
signals:
    void addItemFeaturesChosen(const unsigned int& itemIndex, const double& itemPointXAbsoluteValue, const double& itemPointXRelativeValue, const double& itemPointYAbsoluteValue, const double& itemPointYRelativeValue);
        
    void addItemFeaturesChosen(const unsigned int& itemIndex, const double& itemCubicBezierXAbsoluteValue, const double& itemCubicBezierXRelativeValue, const double& itemCubicBezierYAbsoluteValue, const double& itemCubicBezierYRelativeValue, const double& itemCubicBezierC1XAbsoluteValue, const double& itemCubicBezierC1XRelativeValue, const double& itemCubicBezierC1YAbsoluteValue, const double& itemCubicBezierC1YRelativeValue, const double& itemCubicBezierC2XAbsoluteValue, const double& itemCubicBezierC2XRelativeValue, const double& itemCubicBezierC2YAbsoluteValue, const double& itemCubicBezierC2YRelativeValue);
};

class MyStandardItem : public QStandardItem {
    
public:
    MyStandardItem(const QString& text = "", const qreal& fontsize = qreal(12.0), const bool& isbold = false, const QColor& color = QColor(Qt::black));
};

class MyGroupBox : public QGroupBox {
    Q_OBJECT
    
public:
    MyGroupBox(QWidget* parent);
};

class MyTreeView : public QTreeView {
    
public:
    MyTreeView(QWidget* parent);
    
    void clearModel();
    
    /// Containers
    // branches
    typedef std::vector<std::pair<MyStandardItem*, MyStandardItem*>> branchVec;
    
    /// Iterators
    // branches
    typedef branchVec::iterator branchIt;
    typedef branchVec::const_iterator constBranchIt;
    
    /// Functions related to beginning and end of containers
    // branches
    const constBranchIt BranchesBegin() const { return _branches.begin(); }
    const constBranchIt BranchesEnd() const { return _branches.end(); }
    
    void addBranchWidget(QWidget* branchWidget = NULL, const QString& branchTitle = "", const QString& rootTitle= "");
    
    void removeBranches(const QString& rootTitle= "", const unsigned int& staticbranches = 0);
    
protected:
    QStandardItemModel* treeModel;
    branchVec _branches;
};

class MyTabWidget : public QTabWidget {
    Q_OBJECT
    
public:
    MyTabWidget(QWidget* parent);
};

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
    
    // show the the feature of the model on the menu
    void showFeatureMenu();
    
    // show the the feature of a species on the menu
    void showFeatureMenu(GraphicalSpecies* gS);
    
    // show the the feature of a compartment on the menu
    void showFeatureMenu(GraphicalCompartment* gC);
    
    // show the the feature of a reaction on the menu
    void showFeatureMenu(GraphicalReaction* gR);
    
    // show the the feature of a species reference on the menu
    void showFeatureMenu(GraphicalSReference* gSR);
    
    // show the the feature of a line ending on the menu
    void showFeatureMenu(GraphicalLineEnding* gLE, GraphicalSReference* gSR, QString head);
    
    // hide all feature menu elemnts
    void hideFeatureMenu();
    
    // get color picker menu
    QMenu* getColorPickerMenu();
    
protected:
    fMenuElementVec _featureMenuElements;
    MyColorPickerMenu* _colorPickerMenu;
};

class FeatureMenuElement : public QGroupBox {
    Q_OBJECT
    
public:
    FeatureMenuElement(QWidget* parent = 0, MainWindow* mw = NULL);
    
    /// Functions
    void hideInfo();
    
protected:
    // item
    QLabel titleLabel;
    
    // tab
    MyTabWidget* tabMenu;
    
    // layouts
    QGridLayout qLayout;
    
    // info
    MainWindow* _mw;
};

class ModelFeatureMenu : public FeatureMenuElement {
    Q_OBJECT
    
public:
    ModelFeatureMenu(QWidget* parent = 0, MainWindow* mw = NULL);
    
    void showInfo();
    
private slots:
    
    void resetInfo();

    void simulate();
    
protected:
    // features
    MyGroupBox* featuresBranch;
    
    // simulation
    MyGroupBox* simulationBranch;
    MyDoubleSpinBox startTimeSpinBox;
    MyDoubleSpinBox endTimeSpinBox;
    MySpinBox timePointsSpinBox;
    QPushButton resetButton;
    MyPauseOrResumeButton pauseOrResumeButton;
    QPushButton simulateButton;
    QSlider simulationTimeSlider;
    
    // layouts
    QGridLayout featuresContentLayout;
    QGridLayout simulationContentLayout;
};

class ItemFeatureMenuElement : public FeatureMenuElement {
    Q_OBJECT
    
public:
    ItemFeatureMenuElement(QWidget* parent = 0, MainWindow* mw = NULL);
    
    /// Containers
    // geometric shape feature menu elemnt
    typedef std::vector<GeometricShapeFeatureMenuElement *> gSFMenuElementVec;
    
    // text feature menu
    typedef std::vector<TextFeatureMenu *> textFMenuVec;
    
    /// Iterators
    // geometric shape feature menu element
    typedef gSFMenuElementVec::iterator gSFMenuElementIt;
    typedef gSFMenuElementVec::const_iterator constGSFMenuElementIt;
    
    // text feature menu
    typedef textFMenuVec::iterator textFMenuIt;
    typedef textFMenuVec::const_iterator constTextFMenuIt;
    
    /// Functions related to beginning and end of containers
    // geometric shape feature menu element
    const constGSFMenuElementIt gSFMenuElementsBegin() const { return _gSFMenuElements.begin(); }
    const constGSFMenuElementIt gSFMenuElementsEnd() const { return _gSFMenuElements.end(); }
    
    // text feature menu
    const constTextFMenuIt textFMenuBegin() const { return _textFMenus.begin(); }
    const constTextFMenuIt textFMenuEnd() const { return _textFMenus.end(); }
    
    /// Functions
    void hideInfo();
    
    virtual void resetValues() = 0;
    
    virtual void updateLineEnding(VLineEnding* lineEnding) {};
    
    void setStyle();
    
    void unSetStyle();
    
    virtual void updateValues() = 0;
    
    void clearGSFMenuElements();
    
    void clearTextFMenus();
    
private slots:
    
    void updateStyle(VLocalStyle* style);
    
    void changeLayoutId();
    
    void addOrRemoveBoundingBox();
    
    void addOrRemoveCurve();
    
    void addOrRemoveStyle();
    
    void addGeometricShape(const QString& geometricShape);
    
    void removeGeometricShape(const unsigned int& itemIndex);
    
    void addText(const QString& plainText);
    
    void removeText(const unsigned int& itemIndex);
    
protected slots:
    
    void addBoundingBox(const double& x = 0, const double& y = 0, const double& width = 0, const double& height = 0);
    
    void removeBoundingBox();
    
protected:
    // tree
    MyTreeView* renderTreeView;
    MyTreeView* textTreeView;
    
    // model
    MyGroupBox* modelBranch;
    MyLineEdit modelIdLineEdit;
    
    // layout
    MyGroupBox* layoutBranch;
    MyLineEdit layoutIdLineEdit;
    MyPlainTextWidget plainTextWidget;
    MyAddOrRemoveButton  boundingBoxAddOrRemoveButton;
    BoundingBoxFeatureMenu* bBoxFMenu;
    MyAddOrRemoveButton curveAddOrRemoveButton;
    CurveFeatureMenu* curveFMenu;
    
    // render
    MyGroupBox* renderStyleBranch;
    MyGroupBox* renderGeometricShapeBranch;
    MyAddOrRemoveButton styleAddOrRemoveButton;
    StrokeFeatureMenu* strokeFMenu;
    FillFeatureMenu* fillFMenu;
    MyAddRemoveGeometricShapeButtons addRemoveGeometricShapeButtons;
    gSFMenuElementVec _gSFMenuElements;
    
    // text
    MyGroupBox* textBranch;
    MyAddRemoveTextButtons addRemoveTextButtons;
    textFMenuVec _textFMenus;
    
    // layouts
    QGridLayout modelContentLayout;
    QGridLayout layoutContentLayout;
    QGridLayout renderStyleContentLayout;
    QGridLayout renderGeometricShapeContentLayout;
    QGridLayout textContentLayout;
    
    // info
    GraphicalObjectBase* _gObject;
    GraphicalText* _gText;
    NGraphicalObject* _gO;
    VGlobalStyle* _style;
    VLineEnding* _lE;
};

class SpeciesFeatureMenu : public ItemFeatureMenuElement {
    Q_OBJECT
    
public:
    SpeciesFeatureMenu(QWidget* parent = 0, MainWindow* mw = NULL);
    
    void showInfo(GraphicalObjectBase* gObject);
    
    void resetValues();
    
private slots:
    
    virtual void updateValues();
    
protected:
    // model
    MyLineEdit compartmentIdLineEdit;
};

class CompartmentFeatureMenu : public ItemFeatureMenuElement {
    Q_OBJECT
    
public:
    CompartmentFeatureMenu(QWidget* parent = 0, MainWindow* mw = NULL);
    
    void showInfo(GraphicalObjectBase* gObject);
    
    void resetValues();
    
private slots:
    
    virtual void updateValues();
    
protected:
    
};

class ReactionFeatureMenu : public ItemFeatureMenuElement {
    Q_OBJECT
    
public:
    ReactionFeatureMenu(QWidget* parent = 0, MainWindow* mw = NULL);
    
    void showInfo(GraphicalObjectBase* gObject);
    
    void resetValues();
    
private slots:
    
    virtual void updateValues();
    
protected:
    // layout
    MyAddOrRemoveReactionBoundingBoxButton addOrRemoveReactionBoundingBoxButton;
};

class SpeciesReferenceFeatureMenu : public ItemFeatureMenuElement {
    Q_OBJECT
    
public:
    SpeciesReferenceFeatureMenu(QWidget* parent = 0, MainWindow* mw = NULL);
    
    void showInfo(GraphicalObjectBase* gObject);
    
    void hideInfo();
    
    void resetValues();
    
private slots:
    
    virtual void updateValues();
    
protected:
    // model
    MyLineEdit reactionIdLineEdit;
    MyLineEdit speciesIdLineEdit;
    MyLineEdit roleLineEdit;
    
    // render
    HeadFeatureMenu* headFMenu;
};

class LineEndingFeatureMenu : public ItemFeatureMenuElement {
    Q_OBJECT
    
public:
    LineEndingFeatureMenu(QWidget* parent = 0, MainWindow* mw = NULL);
    
    void showInfo(GraphicalObjectBase* gObject, GraphicalSReference* gSReference, QString head);
    
    void resetValues();
    
private slots:
    
    virtual void updateLineEnding(VLineEnding* lineEnding);
    
    virtual void updateValues();
    
    void changeRenderId();
    
    void changeEnableRotation();
    
protected:
    // layout
    
    // render
    MyLineEdit renderIdLineEdit;
    QPushButton enableRotationButton;
    
    // info
    GraphicalSReference* _gSReference;
    QString _head;
};

class BoundingBoxFeatureMenu : public MyGroupBox {
    Q_OBJECT
    
public:
    BoundingBoxFeatureMenu(QWidget* parent = 0, MainWindow* mw = NULL);
    
    void showInfo(NGraphicalObject* gO);
    
    void showInfo(VLineEnding* lE);
    
    void resetValues();
    
    void collapseTree();
    
signals:
    
    void bBoxPositionChanged();
    
    void bBoxDimensionsChanged();
    
    void lEndingChanged(VLineEnding* lineEnding);
    
private slots:
    
    void changePosition(const double& x, const double& y);
    
    void changeDimensions(const double& width, const double& height);
    
protected:
    // features
    MyTreeView* bBoxTreeView;
    MyGroupBox* bBoxPositionBranch;
    MyGroupBox* bBoxDimensionsBranch;
    MyDualSpinBox positionDualSpinBox;
    MyDualSpinBox dimensionsDualSpinBox;
    
    // layouts
    QGridLayout bBoxPositionLayout;
    QGridLayout bBoxDimensionsLayout;
    QGridLayout bBoxLayout;
    
    // info
    MainWindow* _mw;
    NGraphicalObject* _gO;
    VLineEnding* _lE;
};

class CurveFeatureMenu : public MyGroupBox  {
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
    void showInfo(LCurve* curve);
    
    void resetValues();
    
    void collapseTree();
    
signals:
    void curveChanged();
    
private slots:
    
    void addLine(const unsigned int& itemIndex, const double& itemPointStartXValue, const double& itemPointStartYValue, const double& itemPointEndXValue, const double& itemPointEndYValue);
    
    void addCubicBezier(const unsigned int& itemIndex, const double& itemCubicBezierStartXValue, const double& itemCubicBezierStartYValue, const double& itemCubicBezierEndXValue, const double& itemCubicBezierEndYValue, const double& itemCubicBezierBasePoint1XValue, const double& itemCubicBezierBasePoint1YValue, const double& itemCubicBezierBasePoint2XValue, const double& itemCubicBezierBasePoint2YValue);
    
    void removeLine(const unsigned int& itemIndex);
    
protected:
    // features
    MyAddRemoveSegmentButtons addRemoveSegmentButtons;
    lineFeatureMenuVec _lineFeatureMenus;
    MyTreeView* curveTreeView;
    MyGroupBox* curveSegmentsBranch;
    
    // layouts
    QGridLayout curveLayout;
    QGridLayout curveSegmentsLayout;
    
    // info
    MainWindow* _mw;
    LCurve* _curve;
    QWidget* _parent;
};

class StrokeFeatureMenu : public MyGroupBox {
    Q_OBJECT
    
public:
    StrokeFeatureMenu(QWidget* parent = 0, MainWindow* mw = NULL);
    
    void showInfo(NGraphicalObject* gO, VGlobalStyle* style, const int& gSIndex = -1);
    
    void resetValues();
    
signals:
    
    void styleChanged(VLocalStyle* style);
    
private slots:
    void changeStrokeColor(const QString& color, const QString& value);
    
    void changeStrokeWidth();
    
    void changeStrokeDashArray(std::vector<unsigned int>* dashArrayVector);
    
protected:
    // features
    MyColorPickerButton* strokeColorPickerButton;
    MySpinBox strokeWidthSpinBox;
    MyDashArrayComboBox strokedashArrayComboBox;
    
    // layouts
    QGridLayout strokeLayout;
    
    // info
    MainWindow* _mw;
    NGraphicalObject* _gO;
    VGlobalStyle* _style;
    int _gSIndex;
};

class FillFeatureMenu : public MyGroupBox {
    Q_OBJECT
    
public:
    FillFeatureMenu(QWidget* parent = 0, MainWindow* mw = NULL);
    
    void showInfo(NGraphicalObject* gO, VGlobalStyle* style, const int& gSIndex = -1);
    
    void showInfo(VLineEnding* lE);
    
    void resetValues();
    
signals:
    
    void styleChanged(VLocalStyle* style);
    
    void lEndingChanged(VLineEnding* lineEnding);
    
private slots:
    
    void changeFillColor(const QString& color, const QString& value);
    
protected:
    // features
    MyColorPickerButton* fillColorPickerButton;
    
    // layouts
    QGridLayout fillLayout;
    
    // info
    MainWindow* _mw;
    NGraphicalObject* _gO;
    VGlobalStyle* _style;
    VLineEnding* _lE;
    int _gSIndex;
};

class HeadFeatureMenu : public MyGroupBox {
    Q_OBJECT
    
public:
    HeadFeatureMenu(QWidget* parent = 0, MainWindow* mw = NULL);
    
    void showInfo(NGraphicalObject* gO, VGlobalStyle* style, const int& gSIndex = -1);
    
    void showInfo(VLineEnding* lE);
    
    void resetValues();
    
    void collapseTree();
    
signals:
    
    void styleChanged(VLocalStyle* style);
    
private slots:
    
    void changeStartHead(const QString& head);
    
    void changeEndHead(const QString& head);
    
    void updateValues();
    
protected:
    // features
    MyHeadPickerComboBox startHeadPickerComboBox;
    MyHeadPickerComboBox endHeadPickerComboBox;
    MyTreeView* headTreeView;
    MyGroupBox* headStartBranch;
    MyGroupBox* headEndBranch;
    
    // layouts
    QGridLayout headStartLayout;
    QGridLayout headEndLayout;
    QGridLayout headLayout;
    
    // info
    MainWindow* _mw;
    NGraphicalObject* _gO;
    VGlobalStyle* _style;
    VLineEnding* _lE;
    int _gSIndex;
};

class TextFeatureMenu : public MyGroupBox {
    Q_OBJECT
    
public:
    TextFeatureMenu(QWidget* parent = 0, MainWindow* mw = NULL);
    
    void showInfo(GraphicalText* gT, NGraphicalObject* gO);
    
    void resetValues();
    
signals:
    
    void textChanged();
    
private slots:
    
    void changePlainText(const QString& plainText);
    
    void changeFontSize(const QString& abs, const QString& rel);
    
    void changeFontFamily(int index);
    
    void changeFontWeight(int index);
    
    void changeFontStyle(int index);
    
    void changeFontColor(const QString& color, const QString& value);
    
    void changeTextAnchor(int index);
    
    void changeVTextAnchor(int index);
    
protected:
    // features
    MyPlainTextWidget plainTextWidget;
    MyFontSizeComboBox fontSizeComboBox;
    MyComboBox fontFamilyComboBox;
    MyComboBox fontWeightComboBox;
    MyComboBox fontStyleComboBox;
    MyColorPickerButton* fontColorPickerButton;
    MyComboBox textAnchorComboBox;
    MyComboBox vTextAnchorComboBox;
    MyGroupBox* textFontBranch;
    MyGroupBox* textAlignmentBranch;
    BoundingBoxFeatureMenu* bBoxFMenu;
    MyTreeView* textFeatureTreeView;
    
    // layout
    QGridLayout textFontLayout;
    QGridLayout textAlignmentLayout;
    QGridLayout textLayout;
    
    // info
    MainWindow* _mw;
    GraphicalText* _gText;
    NGraphicalObject* _gO;
};

class GeometricShapeFeatureMenuElement : public MyGroupBox {
    Q_OBJECT
    
public:
    GeometricShapeFeatureMenuElement(QWidget* parent = 0, MainWindow* mw = NULL);
    
    virtual void resetValues() = 0;
    
    void collapseTree();
    
signals:
    
    void styleChanged(VLocalStyle* style);
    
    void lEndingChanged(VLineEnding* lineEnding);
    
protected:
    // features
    MyTreeView* gShapeTreeView;
    StrokeFeatureMenu* strokeFMenu;
    FillFeatureMenu* fillFMenu;
    
    // layouts
    QGridLayout gShapeLayout;
    
    // info
    MainWindow* _mw;
    NGraphicalObject* _gO;
    VGlobalStyle* _style;
    VLineEnding* _lE;
    unsigned int _gSIndex;
};

class RectangleGeometricShapeFeatureMenu : public GeometricShapeFeatureMenuElement {
    Q_OBJECT
    
public:
    RectangleGeometricShapeFeatureMenu(QWidget* parent = 0, MainWindow* mw = NULL, const bool& isLineEnding = false);
    
    void showInfo(NGraphicalObject* gO, VGlobalStyle* style, const unsigned int& gSIndex);
    
    void showInfo(VLineEnding* lE, const unsigned int& gSIndex);
    
    virtual void resetValues();
    
    void getInfoFromRectangleShape(VRectangle* rectangle);
    
private slots:
    
    void changePositionX(const double& abs, const double& rel);
    
    void changePositionY(const double& abs, const double& rel);

    void changeDimensionWidth(const double& abs, const double& rel);
    
    void changeDimensionHeight(const double& abs, const double& rel);
    
    void changeDimensionRatio(const double& ratio);
    
    void changeCornerCurvatureRX(const double& abs, const double& rel);
    
    void changeCornerCurvatureRY(const double& abs, const double& rel);
    
protected:
    // features
    MyRelAbsSpinBox positionXRelAbsSpinBox;
    MyRelAbsSpinBox positionYRelAbsSpinBox;
    MyRelAbsSpinBox dimensionWidthRelAbsSpinBox;
    MyRelAbsSpinBox dimensionHeightRelAbsSpinBox;
    MyDimensionRatioComboBox dimensionRatioComboBox;
    MyRelAbsSpinBox cornerCurvatureRXRelAbsSpinBox;
    MyRelAbsSpinBox cornerCurvatureRYRelAbsSpinBox;
    MyGroupBox* rectanglePositionBranch;
    MyGroupBox* rectangleDimensionsBranch;
    MyGroupBox* rectangleCornerCurvaturesBranch;
    
    // layouts
    QGridLayout rectanglePositionLayout;
    QGridLayout rectangleDimensionsLayout;
    QGridLayout rectangleCornerCurvaturesLayout;
};

class ImageGeometricShapeFeatureMenu : public GeometricShapeFeatureMenuElement {
    Q_OBJECT
    
public:
    ImageGeometricShapeFeatureMenu(QWidget* parent = 0, MainWindow* mw = NULL);
    
    void showInfo(NGraphicalObject* gO, VGlobalStyle* style, const unsigned int& gSIndex);
    
    void showInfo(VLineEnding* lE, const unsigned int& gSIndex);
    
    virtual void resetValues();
    
    void getInfoFromImageShape(VImage* image);
    
private slots:
    
    void changePositionX(const double& abs, const double& rel);
    
    void changePositionY(const double& abs, const double& rel);

    void changeDimensionWidth(const double& abs, const double& rel);
    
    void changeDimensionHeight(const double& abs, const double& rel);
    
    void changeHref();
    
protected:
    // features
    MyRelAbsSpinBox positionXRelAbsSpinBox;
    MyRelAbsSpinBox positionYRelAbsSpinBox;
    MyRelAbsSpinBox dimensionWidthRelAbsSpinBox;
    MyRelAbsSpinBox dimensionHeightRelAbsSpinBox;
    QPushButton hrefButton;
    QString href;
    MyGroupBox* imagePositionBranch;
    MyGroupBox* imageDimensionsBranch;
    MyGroupBox* imagePathBranch;
    
    // layouts
    QGridLayout imagePositionLayout;
    QGridLayout imageDimensionsLayout;
    QGridLayout imagePathLayout;
};

class EllipseGeometricShapeFeatureMenu : public GeometricShapeFeatureMenuElement {
    Q_OBJECT
    
public:
    EllipseGeometricShapeFeatureMenu(QWidget* parent = 0, MainWindow* mw = NULL, const bool& isLineEnding = false);
    
    void showInfo(NGraphicalObject* gO, VGlobalStyle* style, const unsigned int& gSIndex);
    
    void showInfo(VLineEnding* lE, const unsigned int& gSIndex);
    
    virtual void resetValues();
    
    void getInfoFromEllipseShape(VEllipse* ellipse);
    
private slots:
    
    void changePositionCX(const double& abs, const double& rel);
    
    void changePositionCY(const double& abs, const double& rel);
    
    void changeDimensionRX(const double& abs, const double& rel);
    
    void changeDimensionRY(const double& abs, const double& rel);
    
    void changeDimensionRatio(const double& ratio);
    
protected:
    // features
    MyRelAbsSpinBox positionCXRelAbsSpinBox;
    MyRelAbsSpinBox positionCYRelAbsSpinBox;
    MyRelAbsSpinBox dimensionRXRelAbsSpinBox;
    MyRelAbsSpinBox dimensionRYRelAbsSpinBox;
    MyDimensionRatioComboBox dimensionRatioComboBox;
    MyGroupBox* ellipsePositionBranch;
    MyGroupBox* ellipseDimensionsBranch;
    
    // layouts
    QGridLayout ellipsePositionLayout;
    QGridLayout ellipseDimensionsLayout;
};

class PolygonGeometricShapeFeatureMenu : public GeometricShapeFeatureMenuElement {
    Q_OBJECT
    
public:
    PolygonGeometricShapeFeatureMenu(QWidget* parent = 0, MainWindow* mw = NULL, const bool& isRCurve = false, const bool& isLineEnding = false);
    
    /// Containers
    // point feature menu
    typedef std::vector<RenderPointFeatureMenu*> pointFeatureMenuVec;
    
    /// Iterators
    // point feature menu
    typedef pointFeatureMenuVec::iterator pointFeatureMenuIt;
    typedef pointFeatureMenuVec::const_iterator constPointFeatureMenuIt;
    
    /// Functions related to beginning and end of containers
    // point feature menu
    const constPointFeatureMenuIt pointFeatureMenusBegin() const { return _pointFeatureMenus.begin(); }
    const constPointFeatureMenuIt pointFeatureMenusEnd() const { return _pointFeatureMenus.end(); }
    
    /// Functions
    void showInfo(NGraphicalObject* gO, VGlobalStyle* style, const unsigned int& gSIndex);
    
    void showInfo(VLineEnding* lE, const unsigned int& gSIndex);

    virtual void resetValues();
    
    void collapseTree();
    
    virtual void getInfoFromGShape(VTransformation2D* t2d);
    
private slots:
    
    void addPoint(const unsigned int& itemIndex, const double& itemPointXAbsoluteValue, const double& itemPointXRelativeValue, const double& itemPointYAbsoluteValue, const double& itemPointYRelativeValue);
    
    void addCubicBezier(const unsigned int& itemIndex, const double& itemCubicBezierXAbsoluteValue, const double& itemCubicBezierXRelativeValue, const double& itemCubicBezierYAbsoluteValue, const double& itemCubicBezierYRelativeValue, const double& itemCubicBezierC1XAbsoluteValue, const double& itemCubicBezierC1XRelativeValue, const double& itemCubicBezierC1YAbsoluteValue, const double& itemCubicBezierC1YRelativeValue, const double& itemCubicBezierC2XAbsoluteValue, const double& itemCubicBezierC2XRelativeValue, const double& itemCubicBezierC2YAbsoluteValue, const double& itemCubicBezierC2YRelativeValue);
    
    void removePoint(const unsigned int& itemIndex);
    
protected:
    // features
    MyAddRemoveVertexButtons addRemoveVertexButtons;
    pointFeatureMenuVec _pointFeatureMenus;
    MyGroupBox* gShapeVerticesBranch;
    
    // layouts
    QGridLayout gShapeVerticesLayout;
    
    // info
    RenderGroupElementShape gShapeType;
};

class RenderCurveGeometricShapeFeatureMenu : public PolygonGeometricShapeFeatureMenu {
    Q_OBJECT
    
public:
    RenderCurveGeometricShapeFeatureMenu(QWidget* parent = 0, MainWindow* mw = NULL, const bool& isLineEnding = false);
    
    /// Functions
    virtual void resetValues();
    
    virtual void getInfoFromGShape(VTransformation2D* t2d);
    
protected:
    // features
    HeadFeatureMenu* headFMenu;
};

class LineFeatureMenu : public MyGroupBox {
    Q_OBJECT
    
public:
    LineFeatureMenu(QWidget* parent = 0, MainWindow* mw = NULL);
    
    virtual void showInfo(LLineSegment* line);
    
    virtual void resetValues();
    
    void collapseTree();
    
signals:
    
    void lineChanged();
    
private slots:
    
    void changeStart(const double& x, const double& y);
    
    void changeEnd(const double& x, const double& y);
    
protected:
    // featurs
    MyDualSpinBox startDualSpinBox;
    MyDualSpinBox endDualSpinBox;
    MyDualSpinBox basePoint1DualSpinBox;
    MyDualSpinBox basePoint2DualSpinBox;
    MyTreeView* lineTreeView;
    MyGroupBox* lineStartBranch;
    MyGroupBox* lineEndBranch;
    MyGroupBox* lineBasePoint1Branch;
    MyGroupBox* lineBasePoint2Branch;
    
    // layouts
    QGridLayout lineFeatureLayout;
    QGridLayout lineStartLayout;
    QGridLayout lineEndLayout;
    QGridLayout lineBasePoint1Layout;
    QGridLayout lineBasePoint2Layout;
    
    // info
    MainWindow* _mw;
    LLineSegment* _line;
};

class CubicBezierFeatureMenu : public LineFeatureMenu {
    Q_OBJECT
    
public:
    CubicBezierFeatureMenu(QWidget* parent = 0, MainWindow* mw = NULL);
    
    virtual void showInfo(LLineSegment* line);
    
    virtual void resetValues();
    
private slots:
    
    void changeBasePoint1(const double& x, const double& y);
    
    void changeBasePoint2(const double& x, const double& y);
    
protected:
    // featurs
    MyDualSpinBox basePoint1DualSpinBox;
    MyDualSpinBox basePoint2DualSpinBox;
    MyGroupBox* cubicBezierBasePoint1Branch;
    MyGroupBox* cubicBezierBasePoint2Branch;
    
    // layouts
    QGridLayout cubicBezierBasePoint1Layout;
    QGridLayout cubicBezierBasePoint2Layout;
};

class RenderPointFeatureMenu : public MyGroupBox {
    Q_OBJECT
    
public:
    RenderPointFeatureMenu(QWidget* parent = 0, MainWindow* mw = NULL);
    
    void showInfo(NGraphicalObject* gO, VGlobalStyle* style, const unsigned int& gSIndex, const unsigned int& pointIndex);
    
    void showInfo(VLineEnding* lE, const unsigned int& gSIndex, const unsigned int& pointIndex);
    
    virtual void resetValues();
    
    virtual void getInfoFromRenderPoint(RenPoint* renderPoint);
    
signals:
    
    void styleChanged(VLocalStyle* style);
    
    void lEndingChanged(VLineEnding* lineEnding);
    
private slots:
    
    void changeRPointX(const double& abs, const double& rel);
    
    void changeRPointY(const double& abs, const double& rel);
    
protected:
    // features
    MyRelAbsSpinBox pointXRelAbsSpinBox;
    MyRelAbsSpinBox pointYRelAbsSpinBox;
    
    // layouts
    QGridLayout pointFeatureLayout;
    
    // info
    MainWindow* _mw;
    NGraphicalObject* _gO;
    VGlobalStyle* _style;
    VLineEnding* _lE;
    int _gSIndex;
    unsigned int _pointIndex;
};

class RenderCubicBezierFeatureMenu : public RenderPointFeatureMenu {
    Q_OBJECT
    
public:
    RenderCubicBezierFeatureMenu(QWidget* parent = 0, MainWindow* mw = NULL);
    
    virtual void resetValues();
    
    virtual void getInfoFromRenderPoint(RenPoint* renderPoint);
    
private slots:
    
    void changeBasePoint1X(const double& abs, const double& rel);
    
    void changeBasePoint1Y(const double& abs, const double& rel);
    
    void changeBasePoint2X(const double& abs, const double& rel);
    
    void changeBasePoint2Y(const double& abs, const double& rel);
    
protected:
    // features
    MyRelAbsSpinBox basePoint1XRelAbsSpinBox;
    MyRelAbsSpinBox basePoint1YRelAbsSpinBox;
    MyRelAbsSpinBox basePoint2XRelAbsSpinBox;
    MyRelAbsSpinBox basePoint2YRelAbsSpinBox;
};

#endif // NEFEAUTREMENU_H
