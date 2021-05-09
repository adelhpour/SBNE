#include "ne_mainwindow.h"
#include "ui_ne_mainwindow.h"

// main window constructor
MainWindow::MainWindow(QWidget *parent) :  QMainWindow(parent), ui(new Ui::MainWindow)
{
    _isSetView = false;
    _isSetScene = false;
    _isSetFeatureMenu = false;
    _isSetCurrentFileName = false;
    _isSetSBMLDocument = false;
    resetSceneRectExtents();
    
    ui->setupUi(this);
    
    QWidget* widget = new QWidget;
    setCentralWidget(widget);
    
    QWidget* topFiller = new QWidget;
    topFiller->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    
    QWidget* bottomFiller = new QWidget;
    bottomFiller->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    
    setWindowTitle(tr("SBNE"));
    setMinimumSize(120, 80);
    resize(1050, 700);
    //resize(QWidget::maximumSize());
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    createActions();
    createMenus();
    
    setScene(new QGraphicsScene());
    getScene()->setSceneRect(30.0, 20.0, 840.0, 560.0);

    MyQGraphicsView* view = new MyQGraphicsView(this, this);
    view->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    view->setGeometry(75, 50, 900, 600);
    view->setScene(getScene());
    view->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    view->show();
    setView(view);
    
    FeatureMenu* featureMenu = new FeatureMenu(this, this);
    setFeatureMenu(featureMenu);

    QGridLayout* layout = new QGridLayout;
    layout->setContentsMargins(5, 5, 5, 5);
    layout->addWidget(topFiller, 0, 0);
    for (FeatureMenu::constFMenuElementIt fMEIt = getFeatureMenu()->fMenuElementsBegin(); fMEIt != getFeatureMenu()->fMenuElementsEnd(); ++fMEIt)
        layout->addWidget((*fMEIt), 1, 0);
    layout->addWidget(getView(), 1, 1);
    layout->addWidget(bottomFiller, 2, 0);
    widget->setLayout(layout);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createActions() {
    openAct = new QAction(tr("&Open"), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an XML file"));
    connect(openAct, &QAction::triggered, this, &MainWindow::open);
    
    saveAct = new QAction(tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the file with the enetered file name"));
    saveAct->setEnabled(false);
    connect(saveAct, &QAction::triggered, this, &MainWindow::save);
    
    screenshotAct = new QAction(tr("Export as &JPEG"), this);
    screenshotAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_J));
    screenshotAct->setStatusTip(tr("Export a drawing of the model as a jpg file"));
    screenshotAct->setEnabled(false);
    connect(screenshotAct, &QAction::triggered, this, &MainWindow::screenshot);
    
    exitAct = new QAction(tr("&Exit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, &QAction::triggered, this, &MainWindow::exit);
}

void MainWindow::createMenus() {
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(screenshotAct);
    fileMenu->addAction(exitAct);
}

void MainWindow::open() {
    bool openWithoutSave = true;
    
    if (isSetSBMLDocument()) {
        if (getSBMLDocument()->isLayoutModified() || getSBMLDocument()->isRenderModified()) {
            openWithoutSave = false;
            QMessageBox* saveMessageBox =  new QMessageBox();
            saveMessageBox->setWindowTitle("Save Action");
            saveMessageBox->setText("The current model has been modified.");
            saveMessageBox->setInformativeText("Do you want to save your changes?");
            saveMessageBox->setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
            saveMessageBox->setDefaultButton(QMessageBox::Save);
            
            int answer = saveMessageBox->exec();
            if (answer == QMessageBox::Save)
                save();
            else if (answer == QMessageBox::Discard)
                openWithoutSave = true;
        }
    }
    
    if (openWithoutSave) {
        // get the file name by showing a file dialog to the user and asking them to select their desired file
        QString fileName = QFileDialog::getOpenFileName(this, "Open an XML File", ".", tr("Xml files (*.xml)"));
        
        // if file name is set successfully
        if (!fileName.isEmpty()){
            // clear the previously displayed info
            clearInfo();
            
            // hide all the previous feature menu
            getFeatureMenu()->hideFeatureMenu();
            
            // set the current file name using the file name
            setCurrentFileName(fileName);
            
            // set the title of window as the file name
            setWindowTitle(QFileInfo(fileName).fileName());
            
            // reset the scene extents and view scale
            resetSceneRectExtents();
            getView()->resetMatrix();
            
            // create a new sbml document
            setSBMLDocument(new NESBMLDocument());
            
            // load sbml
            getSBMLDocument()->loadSBML(this);
            
            // scale the view to show all the layout
            if ((getView()->geometry().width() / getScene()->sceneRect().width()) < (getView()->geometry().height() / getScene()->sceneRect().height()))
                getView()->scale(getView()->geometry().width() / getScene()->sceneRect().width(), getView()->geometry().width() / getScene()->sceneRect().width());
            else
                getView()->scale(getView()->geometry().height() / getScene()->sceneRect().height(), getView()->geometry().height() / getScene()->sceneRect().height());
            
            // reset max and min scale range
            ((MyQGraphicsView*)getView())->setMaxScale(std::max(3.0, getView()->geometry().width() / (2 * maxSpeciesBoxWidth)));
            ((MyQGraphicsView*)getView())->setMinScale(std::min(1.0 / 3.0, ((MyQGraphicsView*)getView())->currentScale()));
            
            // enable the save act
            saveAct->setEnabled(true);
            
            // enable screenshot act
            screenshotAct->setEnabled(true);
        }
    }
}

void MainWindow::save() {
    // get the file name by showing a file dialog to the user and asking them to enter their desired palce to save and the name of the file
    QString fileName = QFileDialog::getSaveFileName(this, "Save File",  getCurrentFileName(), tr("Xml files (*.xml)"));
    
    // if file name is set successfully
    if (!fileName.isEmpty()) {
        // if saved
        if (getSBMLDocument()->saveSBML(this, fileName))
            setCurrentFileName(fileName);
        // if not saved
        else {
            QMessageBox* savedMessageBox = new QMessageBox();
            savedMessageBox->setWindowTitle("Failed");
            savedMessageBox->setText("The model could not be saved");
            savedMessageBox->setIcon(QMessageBox::Critical);
            savedMessageBox->exec();
        }
    }
}

void MainWindow::screenshot() {
    // get the image file name by showing a file dialog to the user and asking them to enter their desired palce to save and the name of the file
    QString fileName = QFileDialog::getSaveFileName(this, "Save Image File", QFileInfo(getCurrentFileName()).baseName() + "_Image" , "Image (*.jpg)");
    
    // if file name is set successfully
    if (!fileName.isEmpty()) {
        QPixmap pixmap = QPixmap::grabWidget(_view);
        
        // if not saved
        if (!pixmap.save(fileName)) {
            QMessageBox* savedMessageBox = new QMessageBox();
            savedMessageBox->setWindowTitle("Failed");
            savedMessageBox->setText("The image of the model could not be saved");
            savedMessageBox->setIcon(QMessageBox::Critical);
            savedMessageBox->exec();
        }
    }
}

void MainWindow::exit() {
    bool closeWithoutSave = true;
    
    if (isSetSBMLDocument()) {
        if (getSBMLDocument()->isLayoutModified() || getSBMLDocument()->isRenderModified()) {
            closeWithoutSave = false;
            QMessageBox* saveMessageBox =  new QMessageBox();
            saveMessageBox->setWindowTitle("Save Action");
            saveMessageBox->setText("The current model has been modified.");
            saveMessageBox->setInformativeText("Do you want to save your changes?");
            saveMessageBox->setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
            saveMessageBox->setDefaultButton(QMessageBox::Save);
            
            int answer = saveMessageBox->exec();
            if (answer == QMessageBox::Save) {
                save();
                closeWithoutSave = true;
            }
            else if (answer == QMessageBox::Discard)
                closeWithoutSave = true;
        }
    }
    
    if (closeWithoutSave) {
        clearInfo();
        close();
    }
}

void MainWindow::setView(MyQGraphicsView* view) {
    _view = view;
    _isSetView = true;
}
   
QGraphicsView* MainWindow::getView() {
    return _view;
}

void MainWindow::setScene(QGraphicsScene* scene) {
    _scene = scene;
    _isSetScene = true;
}
   
QGraphicsScene* MainWindow::getScene() {
    return _scene;
}

void MainWindow::resetSceneRectExtents() {
    _minSceneX = qreal(INT_MAX);
    _minSceneY = qreal(INT_MAX);
    _maxSceneX = qreal(INT_MIN);
    _maxSceneY = qreal(INT_MIN);
}

void MainWindow::setSceneRect(QRectF rect) {
    if (rect.x() < _minSceneX)
        _minSceneX = rect.x();
    if (rect.y() < _minSceneY)
        _minSceneY = rect.y();
    if (rect.x() + rect.width() > _maxSceneX)
        _maxSceneX = rect.x() + rect.width();
    if (rect.y() + rect.height() > _maxSceneY)
        _maxSceneY = rect.y() + rect.height();
    
    _scene->setSceneRect(_minSceneX - 50.0, _minSceneY - 50.0, _maxSceneX - _minSceneX + 100.0, _maxSceneY - _minSceneY + 100);
}

void MainWindow::setFeatureMenu(FeatureMenu* featureMenu) {
    _featureMenu = featureMenu;
    _isSetFeatureMenu = true;
}

FeatureMenu* MainWindow::getFeatureMenu() {
    return _featureMenu;
}

void MainWindow::setCurrentFileName(const QString& fName) {
    _currentFileName = fName;
    _isSetCurrentFileName = true;
}

const QString& MainWindow::getCurrentFileName() {
    return _currentFileName;
}

void MainWindow::setSBMLDocument(NESBMLDocument* SBMLDocument) {
    _SBMLDocument = SBMLDocument;
    _isSetSBMLDocument = true;
}

NESBMLDocument* MainWindow::getSBMLDocument() {
    return _SBMLDocument;
}

void MainWindow::addGColor(GraphicalColor* c) {
    _colorInfo.push_back(c);
}

void MainWindow::removeGColor(unsigned int graphicalColorIndex) {
    if (graphicalColorIndex >= _colorInfo.size() || graphicalColorIndex < 0)
        std::cerr << "the entered graphical color index is not within the gColorVec range\n";
    else {
        // set the iterator to the desired graphical color
        constGColorIt _it = gColorsBegin();
        for (int i = 0; i < graphicalColorIndex; ++i)
            ++_it;
        // remove the desired graphical color from the gColorVec
        _colorInfo.erase(_it);
    }
}

void MainWindow::setColorInfo(const gColorVec& cv) {
    _colorInfo = cv;
}

const MainWindow::gColorVec& MainWindow::getColorInfo() const {
    return _colorInfo;
}

const size_t MainWindow::getNumGColors() const {
    return _colorInfo.size();
}

void MainWindow::addGGradient(GraphicalGradient* g) {
    _gradientInfo.push_back(g);
}

void MainWindow::removeGGradient(unsigned int graphicalGradientIndex) {
    if (graphicalGradientIndex >= _gradientInfo.size() || graphicalGradientIndex < 0)
        std::cerr << "the entered graphical gradient index is not within the gGradientVec range\n";
    else {
        // set the iterator to the desired graphical gradient
        constGGradientIt _it = gGradientsBegin();
        for (int i = 0; i < graphicalGradientIndex; ++i)
            ++_it;
        
        // remove the desired graphical gradient from the gGradientVec
        _gradientInfo.erase(_it);
    }
}

void MainWindow::setGradientInfo(const gGradientVec& gv) {
    _gradientInfo = gv;
}

const MainWindow::gGradientVec& MainWindow::getGradientInfo() const {
    return _gradientInfo;
}

const size_t MainWindow::getNumGGradients() const {
    return _gradientInfo.size();
}

void MainWindow::addGLEnding(GraphicalLineEnding* l) {
    _lineEndingInfo.push_back(l);
    
    emit numberOfGLEndingChanged();
}

void MainWindow::removeGLEnding(unsigned int graphicalLEndingIndex) {
    if (graphicalLEndingIndex >= _lineEndingInfo.size() || graphicalLEndingIndex < 0)
        std::cerr << "the entered graphical line ending index is not within the gLEndingVec range\n";
    else {
        // set the iterator to the desired graphical line ending
        constGLEndingIt _it = gLEndingsBegin();
        for (int i = 0; i < graphicalLEndingIndex; ++i)
            ++_it;
        // remove the desired graphical line ending from the gLEndingVec
        _lineEndingInfo.erase(_it);
        
        emit numberOfGLEndingChanged();
    }
}

GraphicalLineEnding* MainWindow::findGLEndingById(const std::string& graphicalLineEndingId) {
    for (constGLEndingIt gLEIt = gLEndingsBegin(); gLEIt != gLEndingsEnd(); ++gLEIt) {
        if ((*gLEIt)->isSetId() && sbne::stringCompare((*gLEIt)->getId(), graphicalLineEndingId))
            return *gLEIt;
    }
    
    // not found
    return NULL;
}

void MainWindow::setLEndingInfo(const gLEndingVec& lv) {
    _lineEndingInfo = lv;
    emit numberOfGLEndingChanged();
}

const MainWindow::gLEndingVec& MainWindow::getLEndingInfo() const {
    return _lineEndingInfo;
}

const size_t MainWindow::getNumGLEndings() const {
    return _lineEndingInfo.size();
}

void MainWindow::addGCompartment(GraphicalCompartment* c) {
    _compartmentInfo.push_back(c);
}

void MainWindow::removeGCompartment(unsigned int graphicalCompartmentIndex) {
    if (graphicalCompartmentIndex >= _compartmentInfo.size() || graphicalCompartmentIndex < 0)
        std::cerr << "the entered graphical compartment index is not within the gCompartmentVec range\n";
    else {
        // set the iterator to the desired compartment
        constGCompartmentIt _it = gCompartmentsBegin();
        for (int i = 0; i < graphicalCompartmentIndex; ++i)
            ++_it;
        
        // remove the desired graphical compartment from the gCompartmentVec
        _compartmentInfo.erase(_it);
    }
}

void MainWindow::setCompartmentInfo(const gCompartmentVec& cv) {
    _compartmentInfo = cv;
}

const MainWindow::gCompartmentVec& MainWindow::getCompartmentInfo() const {
    return _compartmentInfo;
}

const size_t MainWindow::getNumGCompartments() const {
    return _compartmentInfo.size();
}

void MainWindow::addGSpecies(GraphicalSpecies* s) {
    _speciesInfo.push_back(s);
}

void MainWindow::removeGSpecies(unsigned int graphicalSpeciesIndex) {
    if (graphicalSpeciesIndex >= _speciesInfo.size() || graphicalSpeciesIndex < 0)
        std::cerr << "the entered graphical species index is not within the gSpeciesVec range\n";
    else {
        // set the iterator to the desired species
        constGSpeciesIt _it = gSpeciesBegin();
        for (int i = 0; i < graphicalSpeciesIndex; ++i)
            ++_it;
        
        // remove the desired graphical species from the gSpeciesVec
        _speciesInfo.erase(_it);
    }
}

void MainWindow::setSpeciesInfo(const gSpeciesVec& sv) {
    _speciesInfo = sv;
}

const MainWindow::gSpeciesVec& MainWindow::getSpeciesInfo() const {
    return _speciesInfo;
}

const size_t MainWindow::getNumGSpecies() const {
    return _speciesInfo.size();
}

void MainWindow::addGReaction(GraphicalReaction* r) {
    _reactionInfo.push_back(r);
}

void MainWindow::removeGReaction(unsigned int graphicalReactionIndex) {
    if (graphicalReactionIndex >= _reactionInfo.size() || graphicalReactionIndex < 0)
        std::cerr << "the entered graphical reaction index is not within the gReactionVec range\n";
    else {
        // set the iterator to the desired reaction
        constGReactionIt _it = gReactionsBegin();
        for (int i = 0; i < graphicalReactionIndex; ++i)
            ++_it;
        
        // remove the desired graphical reaction from the gReactionVec
        _reactionInfo.erase(_it);
    }
}

void MainWindow::setReactionInfo(const gReactionVec& rv) {
    _reactionInfo = rv;
}

const MainWindow::gReactionVec& MainWindow::getReactionInfo() const {
    return _reactionInfo;
}

const size_t MainWindow::getNumGReactions() const {
    return _reactionInfo.size();
}

void MainWindow::clearInfo() {
    _scene->clear();
    _SBMLDocument = NULL;
    _colorInfo.clear();
    _gradientInfo.clear();
    _lineEndingInfo.clear();
    _compartmentInfo.clear();
    _speciesInfo.clear();
    _reactionInfo.clear();
    _isSetCurrentFileName = false;
    _isSetSBMLDocument = false;
}

// MyQGraphicsView

void MyQGraphicsView::wheelEvent (QWheelEvent * event) {
    int numDegrees = event->delta();
    int numSteps = numDegrees;
    _numScheduledScalings += numSteps;
    
    if (_numScheduledScalings * numSteps < 0)
        _numScheduledScalings = numSteps;
    
    QTimeLine* anim = new QTimeLine(20, this);
    anim->setUpdateInterval(1);
    
    connect(anim, SIGNAL(valueChanged(qreal)), SLOT(scalingTime(qreal)));
    connect(anim, SIGNAL(finished()), SLOT(animFinished()));
    anim->start();
}

void MyQGraphicsView::scalingTime(qreal x) {
    qreal factor = 1.0 + qreal(_numScheduledScalings) / 10000.0;
    if ((factor  > 1.00000 && (currentScale() < _maxScale)) || (factor  < 1.00000 && (currentScale() > _minScale)))
        scale(factor, factor);
}

void MyQGraphicsView::animFinished() {
    if (_numScheduledScalings > 0)
        _numScheduledScalings--;
    else
        _numScheduledScalings++;
    sender()->~QObject();
}

const qreal MyQGraphicsView::currentScale() const {
    return transform().m11();
}

void MyQGraphicsView::setMaxScale(const qreal& maxScale) {
    _maxScale = maxScale;
}

const qreal MyQGraphicsView::getMaxScale() const {
    return _maxScale;
}

void MyQGraphicsView::setMinScale(const qreal& minScale) {
    _minScale = minScale;
}

const qreal MyQGraphicsView::getMinScale() const {
    return _minScale;
}

void MyQGraphicsView::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton)
    {
        // start panning
        _pan = true;
        _panStartX = event->x();
        _panStartY = event->y();
        setCursor(Qt::OpenHandCursor);
        event->accept();
        
        return;
    }
    event->ignore();
}

void MyQGraphicsView::mouseMoveEvent(QMouseEvent *event) {
    if (_pan)
    {
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - (event->x() - _panStartX));
        verticalScrollBar()->setValue(verticalScrollBar()->value() - (event->y() - _panStartY));
        _panStartX = event->x();
        _panStartY = event->y();
        event->accept();
        return;
    }
    event->ignore();
}

void MyQGraphicsView::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton)
    {
        _pan = false;
        setCursor(Qt::ArrowCursor);
        event->accept();
        return;
    }
    event->ignore();
}

void MyQGraphicsView::mouseDoubleClickEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton)
    {
        // hide all the previous feature menu
        _mw->getFeatureMenu()->hideFeatureMenu();
        
        // search for the graphical elements
        findGraphicalItem(mapToScene(event->pos()));
        
        return;
    }
    event->ignore();
}

int MyQGraphicsView::findGraphicalItem(QPointF position) {
    QRectF boundingRect;
    
    // search among the species
    for (MainWindow::constGSpeciesIt gSIt = _mw->gSpeciesBegin(); gSIt != _mw->gSpeciesEnd(); ++gSIt) {
        if ((*gSIt)->isSetGraphicalItem()) {
            boundingRect = ((QGraphicsRectItem *)((*gSIt)->getGraphicalItem()))->boundingRect();
            if ((position.x() >= boundingRect.x() && position.x() <=  boundingRect.x() + boundingRect.width()) && (position.y() >= boundingRect.y() && position.y() <=  boundingRect.y() + boundingRect.height())) {
                _mw->getFeatureMenu()->showFeatureMenu(*gSIt);
                return 0;
            }
        }
        else if ((*gSIt)->isSetGText() && (*gSIt)->getGText()->isSetGTextItem()) {
            boundingRect = (*gSIt)->getGText()->getGTextItem()->boundingRect();
            if ((position.x() >= boundingRect.x() && position.x() <=  boundingRect.x() + boundingRect.width()) && (position.y() >= boundingRect.y() && position.y() <=  boundingRect.y() + boundingRect.height())) {
                _mw->getFeatureMenu()->showFeatureMenu(*gSIt);
                return 0;
            }
        }
    }
    
    // search among the reactions
    for (MainWindow::constGReactionIt gRIt = _mw->gReactionsBegin(); gRIt != _mw->gReactionsEnd(); ++gRIt) {
        if ((*gRIt)->isSetGCurve() && (*gRIt)->getGCurve()->getNumGraphicalPaths()) {
            if (isCloseEnoughToCurveSemgent((*gRIt)->getGCurve()->getGraphicalPaths().at(0)->path(), position, qreal(1.0))) {
                _mw->getFeatureMenu()->showFeatureMenu(*gRIt);
                return 0;
            }
        }
        
        // search for bounding box of reaction
        if ((*gRIt)->isSetGraphicalItem()) {
            boundingRect = ((QGraphicsRectItem *)((*gRIt)->getGraphicalItem()))->boundingRect();
            if ((position.x() >= boundingRect.x() && position.x() <=  boundingRect.x() + boundingRect.width()) && (position.y() >= boundingRect.y() && position.y() <=  boundingRect.y() + boundingRect.height())) {
                _mw->getFeatureMenu()->showFeatureMenu(*gRIt);
                return 0;
            }
        }
        
        // search among the species references curves of the reaction
        for (GraphicalReaction::constGSReferenceIt gSRIt = (*gRIt)->gSReferencesBegin(); gSRIt != (*gRIt)->gSReferencesEnd(); ++gSRIt) {
            if ((*gSRIt)->isSetGCurve()) {
                
                // check the start graphical item of the graphcial curve
                if ((*gSRIt)->getGCurve()->isSetStartGraphicalItem()) {
                    QPointF rotationCenter((*gSRIt)->getGCurve()->getStartGraphicalItem()->x(), (*gSRIt)->getGCurve()->getStartGraphicalItem()->y());
                    qreal rotatedAngle = 0.0;
                    if ((*gSRIt)->getGCurve()->isSetStartEnableRotation() && (*gSRIt)->getGCurve()->getStartEnableRotation() && (*gSRIt)->getGCurve()->isSetStartSlope())
                        rotatedAngle = (*gSRIt)->getGCurve()->getStartSlope() * (pi / 180.0);
                    
                    if (isInsideTheRotatedRectangle((*gSRIt)->getGCurve()->getStartGraphicalItem()->boundingRect(), rotationCenter, rotatedAngle, position)) {
                        _mw->getFeatureMenu()->showFeatureMenu( _mw->findGLEndingById((*gSRIt)->getGCurve()->getStartLineEnding()), *gSRIt, "start");
                        return 0;
                    }
                }
                
                // check the end graphical item of the graphcial curve
                if ((*gSRIt)->getGCurve()->isSetEndGraphicalItem()) {
                    QPointF rotationCenter((*gSRIt)->getGCurve()->getEndGraphicalItem()->x(), (*gSRIt)->getGCurve()->getEndGraphicalItem()->y());
                    qreal rotatedAngle = 0.0;
                    if ((*gSRIt)->getGCurve()->isSetEndEnableRotation() && (*gSRIt)->getGCurve()->getEndEnableRotation() && (*gSRIt)->getGCurve()->isSetEndSlope())
                        rotatedAngle = (*gSRIt)->getGCurve()->getEndSlope() * (pi / 180.0);

                    if (isInsideTheRotatedRectangle((*gSRIt)->getGCurve()->getEndGraphicalItem()->boundingRect(), rotationCenter, rotatedAngle, position)) {
                        _mw->getFeatureMenu()->showFeatureMenu( _mw->findGLEndingById((*gSRIt)->getGCurve()->getEndLineEnding()), *gSRIt, "end");
                        return 0;
                    }
                }
                
                // search among the curve elements of the graphical curve
                for (GraphicalCurve::constGPathIt gPIt = (*gSRIt)->getGCurve()->gPathsBegin(); gPIt != (*gSRIt)->getGCurve()->gPathsEnd(); ++gPIt) {
                    if (isCloseEnoughToCurveSemgent((*gPIt)->path(), position, qreal(1.0))) {
                        _mw->getFeatureMenu()->showFeatureMenu(*gSRIt);
                        return 0;
                    }
                }
            }
        }
    }
    
    // search among the compartments
    for (MainWindow::constGCompartmentIt gCIt = _mw->gCompartmentsBegin(); gCIt != _mw->gCompartmentsEnd(); ++gCIt) {
        if ((*gCIt)->isSetGraphicalItem()) {
            boundingRect = ((QGraphicsRectItem *)((*gCIt)->getGraphicalItem()))->boundingRect();
            if ((position.x() >= boundingRect.x() && position.x() <=  boundingRect.x() + boundingRect.width()) && (position.y() >= boundingRect.y() && position.y() <=  boundingRect.y() + boundingRect.height())) {
                _mw->getFeatureMenu()->showFeatureMenu(*gCIt);
                return 0;
            }
        }
        else if ((*gCIt)->isSetGText() && (*gCIt)->getGText()->isSetGTextItem()) {
            boundingRect = (*gCIt)->getGText()->getGTextItem()->boundingRect();
            if ((position.x() >= boundingRect.x() && position.x() <=  boundingRect.x() + boundingRect.width()) && (position.y() >= boundingRect.y() && position.y() <=  boundingRect.y() + boundingRect.height())) {
                _mw->getFeatureMenu()->showFeatureMenu(*gCIt);
                return 0;
            }
        }
    }
    
    return -1;
}

bool MyQGraphicsView::isCloseEnoughToCurveSemgent(const QPainterPath& path, const QPointF& position, const qreal& minRequiredDistance) {
    QPointF distance;
    qreal distanceSize = 0.0;
    qreal oneDistanceAgoSize = INT_MAX;
    qreal twoDistanceAgoSize = INT_MAX;
    for (int i = 0; i <= 100; ++i) {
        distance = position - path.pointAtPercent(qreal(i / 100.0));
        distanceSize = std::sqrt(distance.x() * distance.x() + distance.y() * distance.y());
        
        if (distanceSize < minRequiredDistance)
            return true;
        else if (oneDistanceAgoSize < twoDistanceAgoSize && distanceSize > oneDistanceAgoSize) {
            for (double j = std::max(0, i - 2); j < i; j+= 0.01) {
                distance = position - path.pointAtPercent(qreal(j / 100.0));
                distanceSize = std::sqrt(distance.x() * distance.x() + distance.y() * distance.y());
                if (distanceSize < minRequiredDistance)
                    return true;
            }
            
            break;
        }
        
        twoDistanceAgoSize = oneDistanceAgoSize;
        oneDistanceAgoSize = distanceSize;
    }
    
    return false;
}

bool MyQGraphicsView::isInsideTheRotatedRectangle(const QRectF& boundingRect, const QPointF& rotationCenter, const qreal& rotatedAngle, const QPointF& position) {
    
    QPointF A = QPointF(rotationCenter.x() + boundingRect.x() * std::cos(rotatedAngle) - boundingRect.y() * std::sin(rotatedAngle), rotationCenter.y() + boundingRect.x() * std::sin(rotatedAngle) + boundingRect.y() * std::cos(rotatedAngle));
    QPointF B = QPointF(rotationCenter.x() + (boundingRect.x() + boundingRect.width()) * std::cos(rotatedAngle) - boundingRect.y() * std::sin(rotatedAngle), rotationCenter.y() + (boundingRect.x() + boundingRect.width()) * std::sin(rotatedAngle) + boundingRect.y() * std::cos(rotatedAngle));
    QPointF C = QPointF(rotationCenter.x() + (boundingRect.x() + boundingRect.width()) * std::cos(rotatedAngle) - (boundingRect.y() + boundingRect.height()) * std::sin(rotatedAngle), rotationCenter.y() + (boundingRect.x() + boundingRect.width()) * std::sin(rotatedAngle) + (boundingRect.y() + boundingRect.height()) * std::cos(rotatedAngle));
    QPointF D = QPointF(rotationCenter.x() + boundingRect.x() * std::cos(rotatedAngle) - (boundingRect.y() + boundingRect.height()) * std::sin(rotatedAngle), rotationCenter.y() + boundingRect.x() * std::sin(rotatedAngle) + (boundingRect.y() + boundingRect.height()) * std::cos(rotatedAngle));
    
    qreal area1 = 0.5 * std::abs((B.x() * A.y() - A.x() * B.y()) + (position.x() * B.y() - B.x() * position.y()) + (A.x() * position.y() - position.x() * A.y()));
    qreal area2 = 0.5 * std::abs((D.x() * A.y() - A.x() * D.y()) + (position.x() * D.y() - D.x() * position.y()) + (A.x() * position.y() - position.x() * A.y()));
    qreal area3 = 0.5 * std::abs((B.x() * C.y() - C.x() * B.y()) + (position.x() * B.y() - B.x() * position.y()) + (C.x() * position.y() - position.x() * C.y()));
    qreal area4 = 0.5 * std::abs((D.x() * C.y() - C.x() * D.y()) + (position.x() * D.y() - D.x() * position.y()) + (C.x() * position.y() - position.x() * C.y()));
    
    if (std::abs(area1 + area2 + area3 + area4 - boundingRect.width() * boundingRect.height()) < 0.00001)
        return true;
    else
        return false;
}
 
// Graphical Color

void GraphicalColor::setGraphicalColor(const QColor& c) {
    _graphicalColor = c;
    _isSetGraphicalColor = true;
}

const QColor& GraphicalColor::getGraphicalColor() const {
    return _graphicalColor;
}

void GraphicalColor::setColor(sbne::VColorDefinition* c) {
    _color = c;
    _isSetColor = true;
}

sbne::VColorDefinition* GraphicalColor::getColor() {
    return _color;
}

void GraphicalColor::setId(const std::string& id) {
    _id = id;
    _isSetId = true;
}

const std::string& GraphicalColor::getId() const {
    return _id;
}

// Graphical Gradient

void GraphicalGradient::setGraphicalGradient(const QGradient& g) {
    _graphicalGradient = g;
    _isSetGraphicalGradient = true;
}

const QGradient& GraphicalGradient::getGraphicalGradient() const {
    return _graphicalGradient;
}

void GraphicalGradient::setGradient(sbne::VGradientBase* g) {
    _gradient = g;
    _isSetGradient = true;
}

sbne::VGradientBase* GraphicalGradient::getGradient() {
    return _gradient;
}

void GraphicalGradient::setId(const std::string& id) {
    _id = id;
    _isSetId = true;
}

const std::string& GraphicalGradient::getId() const {
    return _id;
}

// Graphical Line Ending

void GraphicalLineEnding::setGraphicalItem(QGraphicsItem* g) {
    _graphicalItem = g;
    _isSetGraphicalItem = true;
}

void GraphicalLineEnding::unSetGraphicalItem() {
    if (_graphicalItem)
        delete _graphicalItem;
    
    _graphicalItem = NULL;
    _isSetGraphicalItem = false;
}

QGraphicsItem* GraphicalLineEnding::getGraphicalItem() {
    return _graphicalItem;
}
   
void GraphicalLineEnding::setLEnding(sbne::VLineEnding* le) {
    _lEnding = le;
    _isSetLEnding = true;
}

void GraphicalLineEnding::unSetLEnding() {
    _lEnding = NULL;
    _isSetLEnding = false;
}

sbne::VLineEnding* GraphicalLineEnding::getLEnding() {
    return _lEnding;
}
   
void GraphicalLineEnding::setId(const std::string& id) {
    _id = id;
    _isSetId = true;
}

void GraphicalLineEnding::unSetId() {
    _id.clear();
    _isSetId = false;
}

const std::string& GraphicalLineEnding::getId() const {
    return _id;
}
   
void GraphicalLineEnding::setRotation(const bool& rotation) {
    _enableRotation = rotation;
    _isSetEnableRotation = true;
}

void GraphicalLineEnding::unSetRotation() {
    _enableRotation = true;
    _isSetEnableRotation = false;
}

const bool& GraphicalLineEnding::getRotation() const {
    return _enableRotation;
}

// Graphical Compartment

void GraphicalCompartment::setGraphicalItem(QGraphicsItem* g) {
    _graphicalItem = g;
    _isSetGraphicalItem = true;
}

void GraphicalCompartment::unSetGraphicalItem() {
    _graphicalItem = NULL;
    _isSetGraphicalItem = false;
}

QGraphicsItem* GraphicalCompartment::getGraphicalItem() {
    return _graphicalItem;
}

void GraphicalCompartment::setCompartment(sbne::NCompartment* c) {
    _compartment = c;
    _isSetCompartment = true;
}

sbne::NCompartment* GraphicalCompartment::getCompartment() {
    return _compartment;
}

void GraphicalCompartment::setStyle(sbne::VGlobalStyle* s) {
    _style = s;
    _isSetStyle = true;
}

void GraphicalCompartment::setStyle(MainWindow* mw, const bool& addNewStyle) {
    if (mw && mw->isSetSBMLDocument()) {
        sbne::VGlobalStyle* style = ne_ven_findStyle(mw->getSBMLDocument()->getVeneer(), getCompartment());
        
        if (!style && addNewStyle && isSetId()) {
            style = ne_ven_addNewLocalStyle(mw->getSBMLDocument()->getVeneer());
            ne_stl_setStyleValues(style);
            ne_stl_addToIdList(style, getId());
            mw->getSBMLDocument()->setRenderModified(true);
        }
        
        if (style)
            setStyle(style);
        
        if (isSetGText())
            getGText()->setStyle(mw, addNewStyle);
    }
}

void GraphicalCompartment::unSetStyle() {
    _style = NULL;
    _isSetStyle = false;
    
    if (isSetGText())
        getGText()->unSetStyle();
}

sbne::VGlobalStyle* GraphicalCompartment::getStyle() {
    return _style;
}

void GraphicalCompartment::setGText(GraphicalText* t) {
    _graphicalText = t;
    _isSetGraphicalText = true;
}

GraphicalText* GraphicalCompartment::getGText() {
    return _graphicalText;
}

void GraphicalCompartment::updateValues(MainWindow* mw) {
    // reset values
    if (_graphicalItem) {
        mw->getScene()->removeItem(_graphicalItem);
        delete _graphicalItem;
        _graphicalItem = NULL;
        unSetGraphicalItem();
    }
    unSetId();
    
    if (isSetCompartment()) {
        sbne::NCompartment* c = getCompartment();
        // set id
        if (ne_go_isSetGlyphId(c))
            setId(ne_go_getGlyphId(c));
        
        // set graphical item
        if (ne_go_isSetBoundingBox(c)) {
            sbne::LBox* bbox = ne_go_getBoundingBox(c);
            
            // set scene extends
            mw->setSceneRect(QRectF(qreal(ne_bb_getX(bbox)), qreal(ne_bb_getY(bbox)), qreal(ne_bb_getWidth(bbox)), qreal(ne_bb_getHeight(bbox))));
            
            // graphical item
            _graphicalItem = new QGraphicsRectItem(qreal(ne_bb_getX(bbox)), qreal(ne_bb_getY(bbox)), qreal(ne_bb_getWidth(bbox)), qreal(ne_bb_getHeight(bbox)));
            
            if (isSetStyle())
                getInfoFromStyle(mw, getStyle(), _graphicalItem);
            
            _graphicalItem->setZValue(0);
            
            setGraphicalItem(_graphicalItem);
            mw->getScene()->addItem(_graphicalItem);
        }
        
        // graphical text
        if (isSetGText())
            getGText()->updateValues(mw);
    }
}

void GraphicalCompartment::setId(const std::string& id) {
    _id = id;
    _isSetId = true;
}

void GraphicalCompartment::unSetId() {
    _id.clear();
    _isSetId = false;
}

const std::string& GraphicalCompartment::getId() const {
    return _id;
}

// Graphical Species

void GraphicalSpecies::setGraphicalItem(QGraphicsItem* g) {
    _graphicalItem = g;
    _isSetGraphicalItem = true;
}

void GraphicalSpecies::unSetGraphicalItem() {
    _graphicalItem = NULL;
    _isSetGraphicalItem = false;
}

QGraphicsItem* GraphicalSpecies::getGraphicalItem() {
    return _graphicalItem;
}
      
void GraphicalSpecies::setSpecies(sbne::NSpecies* s) {
    _species = s;
    _isSetSpecies = true;
}

sbne::NSpecies* GraphicalSpecies::getSpecies() {
    return _species;
}
   
void GraphicalSpecies::setStyle(sbne::VGlobalStyle* s) {
    _style = s;
    _isSetStyle = true;
}

void GraphicalSpecies::setStyle(MainWindow* mw, const bool& addNewStyle) {
    if (mw && mw->isSetSBMLDocument()) {
        sbne::VGlobalStyle* style = ne_ven_findStyle(mw->getSBMLDocument()->getVeneer(), getSpecies());
        
        if (!style && addNewStyle && isSetId()) {
            style = ne_ven_addNewLocalStyle(mw->getSBMLDocument()->getVeneer());
            ne_stl_setStyleValues(style);
            ne_stl_addToIdList(style, getId());
            mw->getSBMLDocument()->setRenderModified(true);
        }
        
        if (style)
            setStyle(style);
        
        if (isSetGText())
            getGText()->setStyle(mw, addNewStyle);
    }
}

void GraphicalSpecies::unSetStyle() {
    _style = NULL;
    _isSetStyle = false;
    
    if (isSetGText())
        getGText()->unSetStyle();
}

sbne::VGlobalStyle* GraphicalSpecies::getStyle() {
    return _style;
}

void GraphicalSpecies::fitConnectedItemsToBoundingBox(MainWindow* mw) {
    if (isSetSpecies() && ne_go_isSetBoundingBox(getSpecies())) {
        if (isSetGText() && getGText()->isSetGTextItem() && getGText()->isSetPlainText()) {
            QFontMetrics fontMetrics(getGText()->getGTextItem()->textFont());
            qreal textWidth = fontMetrics.width((QString(getGText()->getPlainText().c_str())));
            qreal textHeight = fontMetrics.height();
            sbne::LBox* bbox = ne_go_getBoundingBox(getSpecies());
            bool isBoxModified = false;
            
            // for width
            if (textWidth > 0.9 * ne_bb_getWidth(bbox)) {
                textWidth *= 1.15;
                textWidth = std::min(std::max(maxSpeciesBoxWidth, ne_bb_getWidth(bbox)), double(textWidth));
                
                ne_bb_setX(bbox, ne_bb_getX(bbox) - 0.5 * (textWidth - ne_bb_getWidth(bbox)));
                ne_bb_setWidth(bbox, textWidth);
                if (getGText()->isSetText() && ne_go_isSetBoundingBox(getGText()->getText())) {
                    ne_bb_setX(ne_go_getBoundingBox(getGText()->getText()), ne_bb_getX(bbox));
                    ne_bb_setWidth(ne_go_getBoundingBox(getGText()->getText()), ne_bb_getWidth(bbox));
                }
                
                isBoxModified = true;
            }
            
            // for height
            if (textHeight > 0.9 * ne_bb_getHeight(bbox)) {
                textHeight *= 1.15;
                textHeight = std::min(std::max(maxSpeciesBoxHeight, ne_bb_getHeight(bbox)), double(textHeight));
                
                ne_bb_setY(bbox, ne_bb_getY(bbox) - 0.5 * (textHeight - ne_bb_getHeight(bbox)));
                ne_bb_setHeight(bbox, textHeight);
                if (getGText()->isSetText() && ne_go_isSetBoundingBox(getGText()->getText())) {
                    ne_bb_setY(ne_go_getBoundingBox(getGText()->getText()), ne_bb_getY(bbox));
                    ne_bb_setHeight(ne_go_getBoundingBox(getGText()->getText()), ne_bb_getHeight(bbox));
                }
                
                isBoxModified = true;
            }
            
            if (isBoxModified) {
                // enable the flag of layout modification
                mw->getSBMLDocument()->setLayoutModified(true);
                
                // update graphical species and graphical text
                updateValues(mw, false);
            }
        }
        
        
        // update the location of curves of graphical species references
        if (mw->isSetSBMLDocument() && !mw->getSBMLDocument()->isLayoutAlreadyExisted()) {
            for (MainWindow::constGReactionIt rIt = mw->gReactionsBegin(); rIt != mw->gReactionsEnd(); ++rIt) {
                for (GraphicalReaction::constGSReferenceIt sRIt = (*rIt)->gSReferencesBegin(); sRIt != (*rIt)->gSReferencesEnd(); ++sRIt) {
                    if ((*sRIt)->isSetSReference() && (*sRIt)->getSReference()->isSetSpecies() && (*sRIt)->getSReference()->getSpecies()->isSetId() && sbne::stringCompare((*sRIt)->getSReference()->getSpecies()->getId(), getSpecies()->getId())) {
                        (*sRIt)->fitToSpeciesPosition(mw);
                    }
                }
            }
        }
    }
}

void GraphicalSpecies::updateValues(MainWindow* mw, const bool& _fitConnectedItems) {
    // reset values
    if (_graphicalItem) {
        mw->getScene()->removeItem(_graphicalItem);
        delete _graphicalItem;
        _graphicalItem = NULL;
        unSetGraphicalItem();
    }
    
    unSetId();
    
    if (isSetSpecies()) {
        sbne::NSpecies* s = getSpecies();
        
        // set id
        if (ne_go_isSetGlyphId(s))
            setId(ne_go_getGlyphId(s));
        
        // set graphical item
        if (ne_go_isSetBoundingBox(s)) {
            sbne::LBox* bbox = ne_go_getBoundingBox(s);
            
            // graphical item
            _graphicalItem = new QGraphicsRectItem(qreal(ne_bb_getX(bbox)), qreal(ne_bb_getY(bbox)), qreal(ne_bb_getWidth(bbox)), qreal(ne_bb_getHeight(bbox)));
            
            if (isSetStyle())
                getInfoFromStyle(mw, getStyle(), _graphicalItem);
            
            _graphicalItem->setZValue(4);
            
            setGraphicalItem(_graphicalItem);
            mw->getScene()->addItem(_graphicalItem);
        }
        
        // graphical text
        if (isSetGText())
            getGText()->updateValues(mw);
        
        // fit species connected items to the species bounding box
        if (_fitConnectedItems)
            fitConnectedItemsToBoundingBox(mw);
    }
}

void GraphicalSpecies::setGText(GraphicalText* t) {
    _graphicalText = t;
    _isSetGraphicalText = true;
}

GraphicalText* GraphicalSpecies::getGText() {
    return _graphicalText;
}

void GraphicalSpecies::setId(const std::string& id) {
    _id = id;
    _isSetId = true;
}

void GraphicalSpecies::unSetId() {
    _id.clear();
    _isSetId = false;
}

const std::string& GraphicalSpecies::getId() const {
    return _id;
}

// Graphical Reaction

void GraphicalReaction::addGSReference(GraphicalSReference* sr) {
    _sReferenceInfo.push_back(sr);
}

void GraphicalReaction::removeGSReference(unsigned int graphicalSReferenceIndex) {
    if (graphicalSReferenceIndex >= _sReferenceInfo.size() || graphicalSReferenceIndex < 0)
        std::cerr << "the entered graphical species reference index is not within the gSReferenceVec range\n";
    else {
        // set the iterator to the desired graphical species reference
        constGSReferenceIt _it = gSReferencesBegin();
        for (int i = 0; i < graphicalSReferenceIndex; ++i)
            ++_it;
        // remove the desired graphical species reference from the gSReferenceVec
        _sReferenceInfo.erase(_it);
    }
}

void GraphicalReaction::setGSReferences(const gSReferenceVec& srv) {
    _sReferenceInfo = srv;
}

const GraphicalReaction::gSReferenceVec& GraphicalReaction::getGSReferences() const {
    return _sReferenceInfo;
}

const size_t GraphicalReaction::getNumGSReference() const {
    return _sReferenceInfo.size();
}

void GraphicalReaction::setGCurve(GraphicalCurve* c) {
    _gCurve = c;
    _isSetGCurve = true;
}

GraphicalCurve* GraphicalReaction::getGCurve() {
    return _gCurve;
}

void GraphicalReaction::setGraphicalItem(QGraphicsItem* g) {
    _graphicalItem = g;
    _isSetGraphicalItem = true;
}

void GraphicalReaction::unSetGraphicalItem() {
    _graphicalItem = NULL;
    _isSetGraphicalItem = false;
}

QGraphicsItem* GraphicalReaction::getGraphicalItem() {
    return _graphicalItem;
}

void GraphicalReaction::setReaction(sbne::NReaction* r) {
    _reaction = r;
    _isSetReaction = true;
}

sbne::NReaction* GraphicalReaction::getReaction() {
    return _reaction;
}

void GraphicalReaction::setStyle(sbne::VGlobalStyle* s) {
    _style = s;
    _isSetStyle = true;
}

void GraphicalReaction::setStyle(MainWindow* mw, const bool& addNewStyle) {
    if (mw && mw->isSetSBMLDocument()) {
        sbne::VGlobalStyle* style = ne_ven_findStyle(mw->getSBMLDocument()->getVeneer(), getReaction());
        
        if (!style && addNewStyle && isSetId()) {
            style = ne_ven_addNewLocalStyle(mw->getSBMLDocument()->getVeneer());
            ne_stl_setStyleValues(style);
            ne_stl_addToIdList(style, getId());
            mw->getSBMLDocument()->setRenderModified(true);
        }
        
        if (style)
            setStyle(style);
    }
}

void GraphicalReaction::unSetStyle() {
    _style = NULL;
    _isSetStyle = false;
}

sbne::VGlobalStyle* GraphicalReaction::getStyle() {
    return _style;
}

void GraphicalReaction::updateValues(MainWindow* mw) {
    // reset values
    if (_graphicalItem) {
        mw->getScene()->removeItem(_graphicalItem);
        delete _graphicalItem;
        _graphicalItem = NULL;
        unSetGraphicalItem();
    }
    unSetId();
    
    if (isSetReaction()) {
        sbne::NReaction* r = getReaction();
        
        // set id
        if (ne_go_isSetGlyphId(r))
            setId(ne_go_getGlyphId(r));
            
        // graphical curve
        if (isSetGCurve()) {
            getGCurve()->unSetLCurve();
            getGCurve()->unSetRCurve();
            getGCurve()->unSetBoundingBox();
            
            if (ne_rxn_isSetCurve(r))
                getGCurve()->setLCurve(ne_rxn_getCurve(r));
            else if (ne_go_isSetBoundingBox(r) && isSetStyle()) {
                for (int i = 0; i < ne_grp_getNumGeometricShapes(ne_stl_getGroup(getStyle())); ++i) {
                    if (ne_gs_getShape(ne_grp_getGeometricShape(ne_stl_getGroup(getStyle()), i)) == 1) {
                        getGCurve()->setRCurve((sbne::RCurve*)(ne_grp_getGeometricShape(ne_stl_getGroup(getStyle()), i)));
                        getGCurve()->setBoundingBox(ne_go_getBoundingBox(r));
                    }
                }
            }
            
            getGCurve()->updateValues(mw, getStyle(), false);
        }
        
        if ((!isSetGCurve() || (isSetGCurve() && !getGCurve()->getNumGraphicalPaths())) && ne_go_isSetBoundingBox(r)) {
            // graphical item
            _graphicalItem = new QGraphicsRectItem(qreal(ne_bb_getX(ne_go_getBoundingBox(r))), qreal(ne_bb_getY(ne_go_getBoundingBox(r))), qreal(ne_bb_getWidth(ne_go_getBoundingBox(r))), qreal(ne_bb_getHeight(ne_go_getBoundingBox(r))));
            
            if (isSetStyle())
                getInfoFromStyle(mw, getStyle(), _graphicalItem);
            
            _graphicalItem->setZValue(2);
            
            setGraphicalItem(_graphicalItem);
            mw->getScene()->addItem(_graphicalItem);
        }
    }
}
      
void GraphicalReaction::setId(const std::string& id) {
    _id = id;
    _isSetId = true;
}

void GraphicalReaction::unSetId() {
    _id.clear();
    _isSetId = false;
}

const std::string& GraphicalReaction::getId() const {
    return _id;
}

// Graphical SReference

void GraphicalSReference::setGCurve(GraphicalCurve* c) {
    _gCurve = c;
    _isSetGCurve = true;
}

GraphicalCurve* GraphicalSReference::getGCurve() {
    return _gCurve;
}

void GraphicalSReference::setSReference(sbne::NSpeciesReference* sr) {
    _speciesReference = sr;
    _isSetSReference = true;
}

sbne::NSpeciesReference* GraphicalSReference::getSReference() {
    return _speciesReference;
}

void GraphicalSReference::setStyle(sbne::VGlobalStyle* s) {
    _style = s;
    _isSetStyle = true;
}

void GraphicalSReference::setStyle(MainWindow* mw, const bool& addNewStyle) {
    if (mw && mw->isSetSBMLDocument()) {
        sbne::VGlobalStyle* style = ne_ven_findStyle(mw->getSBMLDocument()->getVeneer(), getSReference());
        
        if (!style && addNewStyle && isSetId()) {
            style = ne_ven_addNewLocalStyle(mw->getSBMLDocument()->getVeneer());
            ne_stl_setStyleValues(style);
            ne_stl_addToIdList(style, getId());
            mw->getSBMLDocument()->setRenderModified(true);
        }
        
        if (style)
            setStyle(style);
    }
}

void GraphicalSReference::unSetStyle() {
    _style = NULL;
    _isSetStyle = false;
}

sbne::VGlobalStyle* GraphicalSReference::getStyle() {
    return _style;
}

void GraphicalSReference::setRole(const std::string& role) {
    _role = role;
    _isSetRole = true;
}

void GraphicalSReference::unSetRole() {
    _role.clear();
    _isSetRole = false;
}

const std::string& GraphicalSReference::getRole() const {
    return _role;
}

void GraphicalSReference::fitToSpeciesPosition(MainWindow* mw) {
    if (isSetSReference() && getSReference()->isSetSpecies() && isSetGCurve()) {
        sbne::NSpeciesReference* sr = getSReference();
        sbne::NSpecies* s = getSReference()->getSpecies();
        GraphicalCurve* gCurve = getGCurve();
        
        if (gCurve->isSetLCurve()) {
            sbne::LCurve* curve = gCurve->getLCurve();
            sbne::LCurve::constElementIt eIt;
            sbne::LPoint boxPoint;
            sbne::SpcSide speciesSide;
            
            switch(sr->getQuadrant()) {
                case sbne::Quad_I_1:
                case sbne::Quad_IV_2:
                    speciesSide = sbne::LEFT_SIDE;
                    break;
                
                case sbne::Quad_I_2:
                case sbne::Quad_II_1:
                    speciesSide = sbne::BOTTOM_SIDE;
                    break;
                    
                case sbne::Quad_II_2:
                case sbne::Quad_III_1:
                    speciesSide = sbne::RIGHT_SIDE;
                    break;
                    
                case sbne::Quad_III_2:
                case sbne::Quad_IV_1:
                    speciesSide = sbne::TOP_SIDE;
                    break;
            }
            
            // get the species box point of species reference
            boxPoint = getSReferenceSpeciesBoxPoint(s, sr, speciesSide);
            
            // for products
            if (sr->getRole() == 1 || sr->getRole() == 3) {
                // update the end point
                eIt = curve->elementsEnd() - 1;
                (*eIt)->setEnd(boxPoint);
            }
            // for substrates, modifiers, activators, and inhibitors
            else {
                // update the start point
                eIt = curve->elementsBegin();
                (*eIt)->setStart(boxPoint);
            }
            
            // enable the flag of layout modification
            mw->getSBMLDocument()->setLayoutModified(true);
            
            gCurve->updateValues(mw, getStyle(), true);
        }
    }
}

void GraphicalSReference::updateValues(MainWindow* mw) {
    // reset values
    unSetId();
    unSetRole();
    
    if (isSetSReference()) {
        sbne::NSpeciesReference* sr = getSReference();
        
        // set id
        if (ne_go_isSetGlyphId(sr))
            setId(ne_go_getGlyphId(sr));
        
        // set the role of graphical species reference
        if (ne_sr_isSetRole(sr))
            setRole(ne_sr_getRoleAsString(sr));
            
        // graphical curve
        if (isSetGCurve()) {
            getGCurve()->unSetLCurve();
            
            if (ne_sr_isSetCurve(sr))
                getGCurve()->setLCurve(ne_sr_getCurve(sr));
            
            getGCurve()->updateValues(mw, getStyle(), true);
        }
    }
}
      
void GraphicalSReference::setId(const std::string& id) {
    _id = id;
    _isSetId = true;
}

void GraphicalSReference::unSetId() {
    _id.clear();
    _isSetId = false;
}

const std::string& GraphicalSReference::getId() const {
    return _id;
}

// Graphical Text
void GraphicalText::setGTextItem(MyQGraphicsTextItem* t) {
    _gTextItem = t;
    _isSetGTextItem = true;
}

void GraphicalText::unSetGTextItem() {
    _gTextItem = NULL;
    _isSetGTextItem = false;
}

MyQGraphicsTextItem* GraphicalText::getGTextItem() {
    return _gTextItem;
}

void GraphicalText::setText(sbne::NText* t) {
    _text = t;
    _isSetText = true;
}

sbne::NText* GraphicalText::getText() {
    return _text;
}

void GraphicalText::setGObject(sbne::NGraphicalObject* gO) {
    _gObject = gO;
    _isSetGObject = true;
}

sbne::NGraphicalObject* GraphicalText::getGObject() {
    return _gObject;
}

void GraphicalText::setStyle(sbne::VGlobalStyle* s) {
    _style = s;
    _isSetStyle = true;
}

void GraphicalText::setStyle(MainWindow* mw, const bool& addNewStyle) {
    if (mw && mw->isSetSBMLDocument()) {
        sbne::VGlobalStyle* style = ne_ven_findStyle(mw->getSBMLDocument()->getVeneer(), NULL, sbne::ST_TYPE_TXT);
        
        if (!style && addNewStyle && isSetGObject()) {
            if (!isSetText()) {
                sbne::NText* text = ne_net_getText(mw->getSBMLDocument()->getNetwork(), getGObject());
                
                if (text) {
                    if (isSetPlainText())
                        text->setText(getPlainText());
                    setText(text);
                    if (text->isSetGlyphId())
                        setId(text->getGlyphId());
                }
            }
            
            if (isSetId()) {
                style = ne_ven_addNewLocalStyle(mw->getSBMLDocument()->getVeneer());
                ne_stl_setStyleValues(style);
                ne_stl_addToIdList(style, getId());
                mw->getSBMLDocument()->setRenderModified(true);
            }
        }
        
        if (style)
            setStyle(style);
    }
}

void GraphicalText::unSetStyle() {
    _style = NULL;
    _isSetStyle = false;
}

sbne::VGlobalStyle* GraphicalText::getStyle() {
    return _style;
}

void GraphicalText::updateValues(MainWindow* mw) {
    // reset values
    if (_gTextItem) {
        mw->getScene()->removeItem(_gTextItem);
        delete _gTextItem;
        _gTextItem = NULL;
        unSetGTextItem();
    }
    unSetId();
    sbne::NText* t = NULL;
    
    if (isSetText()) {
        unSetPlainText();
        t = getText();
        
        // set id
        if (ne_go_isSetGlyphId(t))
            setId(ne_go_getGlyphId(t));
        
        // set plain text
        if (ne_gtxt_isSetPlainText(t))
            setPlainText(ne_gtxt_getPlainText(t));
    }
    
    if (isSetGObject()) {
        
        // set graphical item
        if (((t && ne_go_isSetBoundingBox(t)) || ne_go_isSetBoundingBox(getGObject())) && isSetPlainText()) {
            
            sbne::LBox* bbox = ne_go_getBoundingBox(getText());
            if (!bbox)
                bbox = ne_go_getBoundingBox(getGObject());
            
            if (bbox) {
                _gTextItem = new MyQGraphicsTextItem(QString(getPlainText().c_str()), QRectF(ne_bb_getX(bbox) + 0.05 * ne_bb_getWidth(bbox), ne_bb_getY(bbox) + 0.05 * ne_bb_getHeight(bbox), 0.9 * ne_bb_getWidth(bbox), 0.9 * ne_bb_getHeight(bbox)));
                
                if (isSetStyle())
                    getInfoFromStyle(mw, getStyle(), _gTextItem);
                
                _gTextItem->setZValue(5);
                
                setGTextItem(_gTextItem);
                mw->getScene()->addItem(_gTextItem);
            }
        }
    }
}

void GraphicalText::setPlainText(const std::string& plainText) {
    _plainText = plainText;
    _isSetPlainText = true;
    
    if (isSetText())
        getText()->setText(plainText);
}

void GraphicalText::unSetPlainText() {
    _plainText.clear();
    _isSetPlainText = false;
}

const std::string& GraphicalText::getPlainText() const {
    return _plainText;
}
      
void GraphicalText::setId(const std::string& id) {
    _id = id;
    _isSetId = true;
}

void GraphicalText::unSetId() {
    _id.clear();
    _isSetId = false;
}

const std::string& GraphicalText::getId() const {
    return _id;
}

// MyQGraphicsTextItem
QRectF MyQGraphicsTextItem::boundingRect() const {
    return _boundingRect;
}

void MyQGraphicsTextItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    
    // set the color of text
    painter->setPen(_textPen);
    
    // set the font of painter
    painter->setFont(_textFont);
    
    // adjust text size
    QFontMetrics fontMetrics(_textFont);
    QString ellidedText = fontMetrics.elidedText(_plainText, Qt::ElideRight, _boundingRect.width());
    
    if (_horizontalAlignment == "start") {
        if (_verticalAlignment == "top")
            painter->drawText(_boundingRect, ellidedText, Qt::AlignLeft | Qt::AlignTop);
        else if (_verticalAlignment == "middle")
            painter->drawText(_boundingRect, ellidedText, Qt::AlignLeft | Qt::AlignVCenter);
        else if (_verticalAlignment == "baseline")
            painter->drawText(_boundingRect, ellidedText, Qt::AlignLeft | Qt::AlignBaseline);
        else if (_verticalAlignment == "bottom")
            painter->drawText(_boundingRect, ellidedText, Qt::AlignLeft | Qt::AlignBottom);
    }
    else if (_horizontalAlignment == "middle") {
        if (_verticalAlignment == "top")
            painter->drawText(_boundingRect, ellidedText, Qt::AlignHCenter | Qt::AlignTop);
        else if (_verticalAlignment == "middle")
            painter->drawText(_boundingRect, ellidedText, Qt::AlignHCenter | Qt::AlignVCenter);
        else if (_verticalAlignment == "baseline")
            painter->drawText(_boundingRect, ellidedText, Qt::AlignHCenter | Qt::AlignBaseline);
        else if (_verticalAlignment == "bottom")
            painter->drawText(_boundingRect, ellidedText, Qt::AlignHCenter | Qt::AlignBottom);
    }
    else if (_horizontalAlignment == "end") {
        if (_verticalAlignment == "top")
            painter->drawText(_boundingRect, ellidedText, Qt::AlignRight | Qt::AlignTop);
        else if (_verticalAlignment == "middle")
            painter->drawText(_boundingRect, ellidedText, Qt::AlignRight | Qt::AlignVCenter);
        else if (_verticalAlignment == "baseline")
            painter->drawText(_boundingRect, ellidedText, Qt::AlignRight | Qt::AlignBaseline);
        else if (_verticalAlignment == "bottom")
            painter->drawText(_boundingRect, ellidedText, Qt::AlignRight | Qt::AlignBottom);
    }
}

void MyQGraphicsTextItem::setTextPen(const QPen& textPen) {
    _textPen = textPen;
}

const QPen& MyQGraphicsTextItem::textPen() const {
    return _textPen;
}

void MyQGraphicsTextItem::setTextFont(const QFont& textFont) {
    _textFont = textFont;
}

const QFont& MyQGraphicsTextItem::textFont() const {
    return _textFont;
}

void MyQGraphicsTextItem::setVerticalAlignment(const std::string& vAlignment) {
    _verticalAlignment = vAlignment;
}

const std::string& MyQGraphicsTextItem::verticalAlignment() const {
    return _verticalAlignment;
}

void MyQGraphicsTextItem::setHorizontalAlignment(const std::string& hAlignment) {
    _horizontalAlignment = hAlignment;
}

const std::string& MyQGraphicsTextItem::horizontalAlignment() const {
    return _horizontalAlignment;
}

// Graphical Curve

void GraphicalCurve::setLCurve(sbne::LCurve* lc) {
    _lcurve = lc;
    _isSetLCurve = true;
}

void GraphicalCurve::unSetLCurve() {
    _lcurve = NULL;
    _isSetLCurve = false;
}

sbne::LCurve* GraphicalCurve::getLCurve() {
    return _lcurve;
}

void GraphicalCurve::setRCurve(sbne::RCurve* rc) {
    _rcurve = rc;
    _isSetRCurve = true;
}

void GraphicalCurve::unSetRCurve() {
    _rcurve = NULL;
    _isSetRCurve = false;
}

sbne::RCurve* GraphicalCurve::getRCurve() {
    return _rcurve;
}

void GraphicalCurve::setBoundingBox(sbne::LBox* b) {
    _boundingBox = b;
    _isSetBoundingBox = true;
}

void GraphicalCurve::unSetBoundingBox() {
    _boundingBox = NULL;;
    _isSetBoundingBox = false;
}

sbne::LBox* GraphicalCurve::getBoundingBox() {
    return _boundingBox;
}

void GraphicalCurve::removeGraphicalPath(unsigned int graphicalPathIndex) {
    if (graphicalPathIndex >= _graphicalPaths.size() || graphicalPathIndex < 0)
        std::cerr << "the entered graphical path index is not within the gPathVec range\n";
    else {
        // set the iterator to the desired graphical path
        constGPathIt _it = gPathsBegin();
        for (int i = 0; i < graphicalPathIndex; ++i)
            ++_it;
        // remove the desired graphical path from the gPathVec
        _graphicalPaths.erase(_it);
    }
}

void GraphicalCurve::clearGraphicalPaths(MainWindow* mw) {
    gPathIt pIt = _graphicalPaths.begin();
    while (pIt != _graphicalPaths.end()) {
        mw->getScene()->removeItem(*pIt);
        delete *pIt;
        _graphicalPaths.erase(pIt);
        pIt = _graphicalPaths.begin();
    }
}

void GraphicalCurve::setGraphicalPaths(const gPathVec& pv) {
    _graphicalPaths = pv;
}

const GraphicalCurve::gPathVec& GraphicalCurve::getGraphicalPaths() const {
    return _graphicalPaths;
}

const size_t GraphicalCurve::getNumGraphicalPaths() const {
    return _graphicalPaths.size();
}

void GraphicalCurve::addGraphicalPath(QGraphicsPathItem* p) {
    _graphicalPaths.push_back(p);
}

void GraphicalCurve::setStartGraphicalItem(QGraphicsItem* startGraphicalItem) {
    _startGraphicalItem = startGraphicalItem;
    _isSetStartGraphicalItem = true;
}

void GraphicalCurve::unSetStartGraphicalItem(MainWindow* mw) {
    if (_startGraphicalItem) {
        mw->getScene()->removeItem(_startGraphicalItem);
        delete _startGraphicalItem;
    }
    
    _startGraphicalItem = NULL;
    _isSetStartGraphicalItem = false;
}

QGraphicsItem* GraphicalCurve::getStartGraphicalItem() {
    return _startGraphicalItem;
}

void GraphicalCurve::setEndGraphicalItem(QGraphicsItem* endGraphicalItem) {
    _endGraphicalItem = endGraphicalItem;
    _isSetEndGraphicalItem = true;
}

void GraphicalCurve::unSetEndGraphicalItem(MainWindow* mw) {
    if (_endGraphicalItem) {
        mw->getScene()->removeItem(_endGraphicalItem);
        delete _endGraphicalItem;
    }
    
    _endGraphicalItem = NULL;
    _isSetEndGraphicalItem = false;
}

QGraphicsItem* GraphicalCurve::getEndGraphicalItem() {
    return _endGraphicalItem;
}
      
void GraphicalCurve::setStartPoint(const QPointF& sPoint) {
    _startPoint = sPoint;
    _isSetStartPoint = true;
}

void GraphicalCurve::unSetStartPoint() {
    _startPoint = QPointF(0.0, 0.0);
    _isSetStartPoint = false;
}

const QPointF& GraphicalCurve::getStartPoint() const {
    return _startPoint;
}
   
void GraphicalCurve::setEndPoint(const QPointF& ePoint) {
    _endPoint = ePoint;
    _isSetEndPoint = true;
}

void GraphicalCurve::unSetEndPoint() {
    _endPoint = QPointF(0.0, 0.0);
    _isSetEndPoint = false;
}

const QPointF& GraphicalCurve::getEndPoint() const {
    return _endPoint;
}

void GraphicalCurve::setStartSlope(const qreal& sSlope) {
    _startSlope = sSlope;
    _isSetStartSlope = true;
}

void GraphicalCurve::unSetStartSlope() {
    _startSlope = 0.0;
    _isSetStartSlope = false;
}

const qreal& GraphicalCurve::getStartSlope() const {
    return _startSlope;
}

void GraphicalCurve::setEndSlope(const qreal& eSlope) {
    _endSlope = eSlope;
    _isSetEndSlope = true;
}

void GraphicalCurve::unSetEndSlope() {
    _endSlope = 0.0;
    _isSetEndSlope = false;
}

const qreal& GraphicalCurve::getEndSlope() const {
    return _endSlope;
}

void GraphicalCurve::setStartEnableRotation(const bool& enableRotation) {
    _startEnableRotation = enableRotation;
    _isSetStartEnableRotation = true;
}

void GraphicalCurve::unSetStartEnableRotation() {
    _startEnableRotation = true;
    _isSetStartEnableRotation = false;
}

const bool& GraphicalCurve::getStartEnableRotation() const {
    return _startEnableRotation;
}

void GraphicalCurve::setEndEnableRotation(const bool& enableRotation) {
    _endEnableRotation = enableRotation;
    _isSetEndEnableRotation = true;
}

void GraphicalCurve::unSetEndEnableRotation() {
    _endEnableRotation = true;
    _isSetEndEnableRotation = false;
}

const bool& GraphicalCurve::getEndEnableRotation() const {
    return _endEnableRotation;
}

void GraphicalCurve::setStartLineEnding(const std::string& sLEnding) {
    _startLineEnding = sLEnding;
    _isSetStartLineEnding = true;
}

void GraphicalCurve::unSetStartLineEnding() {
    _startLineEnding.clear();
    _isSetStartLineEnding = false;
}

const std::string& GraphicalCurve::getStartLineEnding() const {
    return _startLineEnding;
}
   
void GraphicalCurve::setEndLineEnding(const std::string& eLEnding) {
    _endLineEnding = eLEnding;
    _isSetEndLineEnding = true;
}

void GraphicalCurve::unSetEndLineEnding() {
    _endLineEnding.clear();
    _isSetEndLineEnding = false;
}

const std::string& GraphicalCurve::getEndLineEnding() const {
    return _endLineEnding;
}

void GraphicalCurve::updateValues(MainWindow* mw, sbne::VGlobalStyle* style, const bool& _setLineEndings) {
    
    // rest graphical curve values
    clearGraphicalPaths(mw);
    unSetStartGraphicalItem(mw);
    unSetEndGraphicalItem(mw);
    unSetStartPoint();
    unSetEndPoint();
    unSetStartSlope();
    unSetEndSlope();
    unSetStartEnableRotation();
    unSetEndEnableRotation();
    unSetStartLineEnding();
    unSetEndLineEnding();
    unSetId();
    
    // set graphical item using layout curve
    if (isSetLCurve()) {
        sbne::LCurve* lcurve = getLCurve();
        QGraphicsPathItem* _graphicalPath;
        QPainterPath* _cubicbezier;
        
        // set id
        if (ne_ne_isSetId(lcurve))
            setId(ne_ne_getId(lcurve));
        
        for (int i = 0; i < ne_crv_getNumElements(lcurve); ++i) {
            sbne::LLineSegment* l = ne_crv_getElement(lcurve, i);
            
            // get start point
            QPointF sPoint(qreal(ne_point_getX(ne_ls_getStart(l))), qreal(ne_point_getY(ne_ls_getStart(l))));
            
            // get end point
            QPointF ePoint(qreal(ne_point_getX(ne_ls_getEnd(l))), qreal(ne_point_getY(ne_ls_getEnd(l))));
            setEndPoint(ePoint);
            
            // get base points
            QPointF basePoint1, basePoint2;
            if (ne_ls_isCubicBezier(l)) {
                basePoint1 = QPointF(qreal(ne_point_getX(ne_cb_getBasePoint1(l))), qreal(ne_point_getY(ne_cb_getBasePoint1(l))));
                
                basePoint2 = QPointF(qreal(ne_point_getX(ne_cb_getBasePoint2(l))), qreal(ne_point_getY(ne_cb_getBasePoint2(l))));
            }
            else {
                basePoint1 = sPoint;
                basePoint2 = ePoint;
            }
            
            // create the graphical path
            _cubicbezier = new QPainterPath(sPoint);
            _cubicbezier->cubicTo(basePoint1, basePoint2, ePoint);
            _graphicalPath = new QGraphicsPathItem(*_cubicbezier);
            _graphicalPath->setZValue(1);
            addGraphicalPath(_graphicalPath);
            
            // set start point and slope
            if (i == 0) {
                
                // start point
                setStartPoint(sPoint);
                
                // start slope
                double numerator, denominator;
                if (sPoint == basePoint1) {
                    numerator = sPoint.y() - ePoint.y();
                    denominator = sPoint.x() - ePoint.x();
                }
                else {
                    numerator = sPoint.y() - basePoint1.y();
                    denominator = sPoint.x() - basePoint1.x();
                }
                
                // set the start slope of graphical curve
                setStartSlope((180.0 / pi) * std::atan2( numerator, denominator));
            }
            
            // set end point and slope
            if (i == ne_crv_getNumElements(lcurve) - 1) {
                
                // end point
                setEndPoint(ePoint);
                
                // end slope
                double numerator, denominator;
                if (ePoint == basePoint2) {
                    numerator = ePoint.y() - sPoint.y();
                    denominator = ePoint.x() - sPoint.x();
                }
                else {
                    numerator = ePoint.y() - basePoint2.y();
                    denominator = ePoint.x() - basePoint2.x();
                }
                
                // set the start slope of graphical curve
                setEndSlope((180.0 / pi) * std::atan2( numerator, denominator));
            }
        }
        
        if (style)
            getInfoFromStyle(mw, style, this);
        
        // display the reaction graphical curve
        for (constGPathIt gPIt = gPathsBegin(); gPIt != gPathsEnd(); ++gPIt)
            mw->getScene()->addItem(*gPIt);
        
        // set line endings
        if (_setLineEndings)
            setLineEndings(mw, this);
    }
    
    // set graphical item using rendre curve
    else if (isSetRCurve() && ne_rc_getNumVertices(getRCurve()) > 1) {
        sbne::RCurve* rcurve = getRCurve();
        QGraphicsPathItem* _graphicalPath;
        QPainterPath* _cubicbezier;
        
        // set id
        if (ne_ve_isSetId(rcurve))
            setId(ne_ve_getId(rcurve));
        
        QRectF boundingRect(0.0, 0.0, 0.0, 0.0);
        if (isSetBoundingBox())
            boundingRect = QRectF(ne_bb_getX(getBoundingBox()), ne_bb_getY(getBoundingBox()), ne_bb_getWidth(getBoundingBox()), ne_bb_getHeight(getBoundingBox()));
        
        QPointF sPoint, ePoint;
        for (int i = 0; i < ne_rc_getNumVertices(rcurve); ++i) {
            sbne::RenPoint* rp = ne_rc_getVertex(rcurve, i);
            sbne::RAVector* rPointX,* rPointY;
            QPointF basePoint1, basePoint2;
            
            // render point
            rPointX = ne_rp_getX(ne_vrx_getRenderPoint(rp));
            rPointY = ne_rp_getX(ne_vrx_getRenderPoint(rp));
            
            if (i == 0)
                sPoint = QPointF(qreal(boundingRect.x() + ne_rav_getAbsoluteValue(rPointX) + boundingRect.width() * (0.01 * ne_rav_getRelativeValue(rPointX))), qreal(boundingRect.y() + ne_rav_getAbsoluteValue(rPointY) + boundingRect.height() * (0.01 * ne_rav_getRelativeValue(rPointY))));
            else {
                ePoint = QPointF(qreal(boundingRect.x() + ne_rav_getAbsoluteValue(rPointX) + boundingRect.width() * (0.01 * ne_rav_getRelativeValue(rPointX))), qreal(boundingRect.y() + ne_rav_getAbsoluteValue(rPointY) + boundingRect.height() * (0.01 * ne_rav_getRelativeValue(rPointY))));
                
                // get base points
                if (ne_vrx_isRenderCubicBezier(rp)) {
                    rPointX = ne_rp_getX(ne_vrx_getBasePoint1(rp));
                    rPointY = ne_rp_getY(ne_vrx_getBasePoint1(rp));
                    basePoint1 = QPointF(qreal(boundingRect.x() + ne_rav_getAbsoluteValue(rPointX) + boundingRect.width() * (0.01 * ne_rav_getRelativeValue(rPointX))), qreal(boundingRect.y() + ne_rav_getAbsoluteValue(rPointY) + boundingRect.height() * (0.01 * ne_rav_getRelativeValue(rPointY))));
                    
                    rPointX = ne_rp_getX(ne_vrx_getBasePoint2(rp));
                    rPointY = ne_rp_getY(ne_vrx_getBasePoint2(rp));
                    basePoint2 = QPointF(qreal(boundingRect.x() + ne_rav_getAbsoluteValue(rPointX) + boundingRect.width() * (0.01 * ne_rav_getRelativeValue(rPointX))), qreal(boundingRect.y() + ne_rav_getAbsoluteValue(rPointY) + boundingRect.height() * (0.01 * ne_rav_getRelativeValue(rPointY))));
                }
                else {
                    basePoint1 = sPoint;
                    basePoint2 = ePoint;
                }
                
                // create the graphical path
                _cubicbezier = new QPainterPath(sPoint);
                if (i > 0)
                    _cubicbezier->cubicTo(basePoint1, basePoint2, ePoint);
                _graphicalPath = new QGraphicsPathItem(*_cubicbezier);
                _graphicalPath->setZValue(1);
                addGraphicalPath(_graphicalPath);
                
                // set end point and slope
                if (i == 1) {
                    
                    // start point
                    setStartPoint(sPoint);
                    
                    // start slope
                    double numerator, denominator;
                    if (sPoint == basePoint1) {
                        numerator = sPoint.y() - ePoint.y();
                        denominator = sPoint.x() - ePoint.x();
                    }
                    else {
                        numerator = sPoint.y() - basePoint1.y();
                        denominator = sPoint.x() - basePoint1.x();
                    }
                    
                    // set the start slope of graphical curve
                    setStartSlope((180.0 / pi) * std::atan2( numerator, denominator));
                }
                
                // set end point and slope
                if (i == ne_rc_getNumVertices(rcurve) - 1) {
                    
                    // end point
                    setEndPoint(ePoint);
                    
                    // end slope
                    double numerator, denominator;
                    if (ePoint == basePoint2) {
                        numerator = ePoint.y() - sPoint.y();
                        denominator = ePoint.x() - sPoint.x();
                    }
                    else {
                        numerator = ePoint.y() - basePoint2.y();
                        denominator = ePoint.x() - basePoint2.x();
                    }
                    
                    // set the start slope of graphical curve
                    setEndSlope((180.0 / pi) * std::atan2( numerator, denominator));
                }
                
                sPoint = ePoint;
            }
        }
        
        if (style)
            getInfoFromStyle(mw, style, this);
        
        // display the reaction graphical curve
        for (constGPathIt gPIt = gPathsBegin(); gPIt != gPathsEnd(); ++gPIt)
            mw->getScene()->addItem(*gPIt);
        
        // set line endings
        if (_setLineEndings)
            setLineEndings(mw, this);
    }
}

void GraphicalCurve::setId(const std::string& id) {
    _id = id;
    _isSetId = true;
}

void GraphicalCurve::unSetId() {
    _id.clear();
    _isSetId = false;
}

const std::string& GraphicalCurve::getId() const {
    return _id;
}

