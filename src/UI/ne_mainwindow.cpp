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
    
    screenshotAct = new QAction(tr("Export as &PDF"), this);
    screenshotAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_P));
    screenshotAct->setStatusTip(tr("Export a drawing of the model as a PDF file"));
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
    QString fileName = QFileDialog::getSaveFileName(this, "Save PDF File", QFileInfo(getCurrentFileName()).baseName() + "_drawing", "(*.pdf)");
    
    // if file name is set successfully
    if (!fileName.isEmpty()) {
        //QPixmap pixmap = QPixmap::grabWidget(_view);
        
        QPrinter printer(QPrinter::ScreenResolution);
        printer.setPageSize(QPageSize(QSize(getScene()->sceneRect().width(), getScene()->sceneRect().height()), QPageSize::Point));
        printer.setPageMargins(QMarginsF(0.0, 0.0, 0.0, 0.0));
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(fileName);
        
        QPainter painter(&printer);
        getScene()->render(&painter);
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
    _scene->setBackgroundBrush(Qt::NoBrush);
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
        // search among the graphical items of species
        for (GraphicalObjectBase::constGItemIt gIIt = (*gSIt)->gItemsBegin(); gIIt != (*gSIt)->gItemsEnd(); ++gIIt) {
            boundingRect = ((QGraphicsRectItem *)(*gIIt))->boundingRect();
            if ((position.x() >= boundingRect.x() && position.x() <=  boundingRect.x() + boundingRect.width()) && (position.y() >= boundingRect.y() && position.y() <=  boundingRect.y() + boundingRect.height())) {
                _mw->getFeatureMenu()->showFeatureMenu(*gSIt);
                return 0;
            }
        }
        
        // search among the curves of species
        for (GraphicalObjectBase::constGCurveIt gCIt = (*gSIt)->gCurvesBegin(); gCIt != (*gSIt)->gCurvesEnd(); ++gCIt) {
            if ((*gCIt)->getNumGraphicalPaths()) {
                if (isCloseEnoughToCurveSemgent((*gCIt)->getGraphicalPaths().at(0)->path(), position, qreal(1.0))) {
                    _mw->getFeatureMenu()->showFeatureMenu(*gSIt);
                    return 0;
                }
            }
        }
        
        // search among the text of species
        for (GraphicalObjectBase::constGTextIt gTIt = (*gSIt)->gTextsBegin(); gTIt != (*gSIt)->gTextsEnd(); ++gTIt) {
            for (GraphicalText::constGItemIt gIIt = (*gTIt)->gItemsBegin(); gIIt != (*gTIt)->gItemsEnd(); ++gIIt) {
                boundingRect = ((MyQGraphicsTextItem*)(*gIIt))->boundingRect();
                if ((position.x() >= boundingRect.x() && position.x() <=  boundingRect.x() + boundingRect.width()) && (position.y() >= boundingRect.y() && position.y() <=  boundingRect.y() + boundingRect.height())) {
                    _mw->getFeatureMenu()->showFeatureMenu(*gSIt);
                    return 0;
                }
            }
        }
    }
    
    // search among the reactions
    for (MainWindow::constGReactionIt gRIt = _mw->gReactionsBegin(); gRIt != _mw->gReactionsEnd(); ++gRIt) {
        
        // search among the curves of reaction
        for (GraphicalObjectBase::constGCurveIt gCIt = (*gRIt)->gCurvesBegin(); gCIt != (*gRIt)->gCurvesEnd(); ++gCIt) {
            if ((*gCIt)->getNumGraphicalPaths()) {
                if (isCloseEnoughToCurveSemgent((*gCIt)->getGraphicalPaths().at(0)->path(), position, qreal(1.0))) {
                    _mw->getFeatureMenu()->showFeatureMenu(*gRIt);
                    return 0;
                }
            }
        }
        
        // search for bounding box of reaction
        for (GraphicalObjectBase::constGItemIt gIIt = (*gRIt)->gItemsBegin(); gIIt != (*gRIt)->gItemsEnd(); ++gIIt) {
            boundingRect = ((QGraphicsRectItem *)(*gIIt))->boundingRect();
            if ((position.x() >= boundingRect.x() && position.x() <=  boundingRect.x() + boundingRect.width()) && (position.y() >= boundingRect.y() && position.y() <=  boundingRect.y() + boundingRect.height())) {
                _mw->getFeatureMenu()->showFeatureMenu(*gRIt);
                return 0;
            }
        }
        
        // search among the species references curves of the reaction
        for (GraphicalReaction::constGSReferenceIt gSRIt = (*gRIt)->gSReferencesBegin(); gSRIt != (*gRIt)->gSReferencesEnd(); ++gSRIt) {
            for (GraphicalObjectBase::constGCurveIt gCIt = (*gSRIt)->gCurvesBegin(); gCIt != (*gSRIt)->gCurvesEnd(); ++gCIt) {
                // check the start graphical items of the graphcial curve
                for (GraphicalSReference::constGItemIt gIIt = (*gCIt)->startGItemsBegin(); gIIt != (*gCIt)->startGItemsEnd(); ++gIIt) {
                    QPointF rotationCenter((*gIIt)->x(), (*gIIt)->y());
                    qreal rotatedAngle = 0.0;
                    if ((*gCIt)->isSetStartEnableRotation() && (*gCIt)->getStartEnableRotation() && (*gCIt)->isSetStartSlope())
                        rotatedAngle = (*gCIt)->getStartSlope() * (pi / 180.0);
                    
                    if (isInsideTheRotatedRectangle((*gIIt)->boundingRect(), rotationCenter, rotatedAngle, position)) {
                        _mw->getFeatureMenu()->showFeatureMenu( _mw->findGLEndingById((*gCIt)->getStartLineEnding()), *gSRIt, "start");
                        return 0;
                    }
                }
                
                // check the end graphical items of the graphcial curve
                for (GraphicalSReference::constGItemIt gIIt = (*gCIt)->endGItemsBegin(); gIIt != (*gCIt)->endGItemsEnd(); ++gIIt) {
                    QPointF rotationCenter((*gIIt)->x(), (*gIIt)->y());
                    qreal rotatedAngle = 0.0;
                    if ((*gCIt)->isSetEndEnableRotation() && (*gCIt)->getEndEnableRotation() && (*gCIt)->isSetEndSlope())
                        rotatedAngle = (*gCIt)->getEndSlope() * (pi / 180.0);
                    
                    if (isInsideTheRotatedRectangle((*gIIt)->boundingRect(), rotationCenter, rotatedAngle, position)) {
                        _mw->getFeatureMenu()->showFeatureMenu( _mw->findGLEndingById((*gCIt)->getEndLineEnding()), *gSRIt, "end");
                        return 0;
                    }
                }
                
                // search among the curve elements of the graphical curve
                for (GraphicalCurve::constGPathIt gPIt = (*gCIt)->gPathsBegin(); gPIt != (*gCIt)->gPathsEnd(); ++gPIt) {
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
        // search among the graphical items of compartment
        for (int i = 0; i < (*gCIt)->getGraphicalItems().size(); ++i) {
            boundingRect = ((QGraphicsRectItem *)((*gCIt)->getGraphicalItems().at(i)))->boundingRect();
            if ((position.x() >= boundingRect.x() && position.x() <=  boundingRect.x() + boundingRect.width()) && (position.y() >= boundingRect.y() && position.y() <=  boundingRect.y() + boundingRect.height())) {
                _mw->getFeatureMenu()->showFeatureMenu(*gCIt);
                return 0;
            }
        }
        
        // search among the curves of compartment
        for (GraphicalObjectBase::constGCurveIt gCrIt = (*gCIt)->gCurvesBegin(); gCrIt != (*gCIt)->gCurvesEnd(); ++gCrIt) {
            if ((*gCrIt)->getNumGraphicalPaths()) {
                if (isCloseEnoughToCurveSemgent((*gCrIt)->getGraphicalPaths().at(0)->path(), position, qreal(1.0))) {
                    _mw->getFeatureMenu()->showFeatureMenu(*gCIt);
                    return 0;
                }
            }
        }
        
        // search for the text of compartments
        for (GraphicalObjectBase::constGTextIt gTIt = (*gCIt)->gTextsBegin(); gTIt != (*gCIt)->gTextsEnd(); ++gTIt) {
            for (GraphicalText::constGItemIt gIIt = (*gTIt)->gItemsBegin(); gIIt != (*gTIt)->gItemsEnd(); ++gIIt) {
                boundingRect = ((MyQGraphicsTextItem*)(*gIIt))->boundingRect();
                if ((position.x() >= boundingRect.x() && position.x() <=  boundingRect.x() + boundingRect.width()) && (position.y() >= boundingRect.y() && position.y() <=  boundingRect.y() + boundingRect.height())) {
                    _mw->getFeatureMenu()->showFeatureMenu(*gCIt);
                    return 0;
                }
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

// Graphical Element

void GraphicalElement::setId(const std::string& id) {
    _id = id;
    _isSetId = true;
}

const std::string& GraphicalElement::getId() const {
    return _id;
}

void GraphicalElement::unSetId() {
    _id.clear();
    _isSetId = false;
}

// Graphical Object Base

void GraphicalObjectBase::addGraphicalItem(QGraphicsItem* g) {
    _graphicalItems.push_back(g);
}

void GraphicalObjectBase::removeGraphicalItem(unsigned int graphicalItemIndex) {
    if (graphicalItemIndex >= _graphicalItems.size() || graphicalItemIndex < 0)
        std::cerr << "the entered graphical item index is not within the gItemVec range\n";
    else {
        // set the iterator to the desired graphical item
        constGItemIt _it = gItemsBegin();
        for (int i = 0; i < graphicalItemIndex; ++i)
            ++_it;
        // remove the desired graphical species reference from the gItemVec
        _graphicalItems.erase(_it);
    }
}

void GraphicalObjectBase::setGraphicalItems(const gItemVec& giv) {
    _graphicalItems = giv;
}

const GraphicalObjectBase::gItemVec& GraphicalObjectBase::getGraphicalItems() const {
    return _graphicalItems;
}

void GraphicalObjectBase::resetGraphicalItems(MainWindow* mw, const bool& isLineEnding) {
    for (constGItemIt gIIt = gItemsBegin(); gIIt != gItemsEnd(); ++gIIt) {
        if (!isLineEnding)
            mw->getScene()->removeItem((*gIIt));
        delete *gIIt;
    }
    
    _graphicalItems.clear();
}

void GraphicalObjectBase::setNGraphicalObject(sbne::NGraphicalObject* gO) {
    _nGraphicalObject = gO;
    _isSetNGraphicalObject = true;
}

sbne::NGraphicalObject* GraphicalObjectBase::getNGraphicalObject() {
    return _nGraphicalObject;
}

void GraphicalObjectBase::setStyle(sbne::VGlobalStyle* s) {
    _style = s;
    _isSetStyle = true;
}

void GraphicalObjectBase::setStyle(MainWindow* mw, const bool& addNewStyle) {
    if (mw && mw->isSetSBMLDocument()) {
        sbne::VGlobalStyle* style = ne_ven_findStyle(mw->getSBMLDocument()->getVeneer(), getNGraphicalObject());
        
        if (!style && addNewStyle) {
            if (isSetNGraphicalObject())
                style = ne_ven_addNewGlobalStyle(mw->getSBMLDocument()->getVeneer(), styleTypeForGraphicalObjectType(getNGraphicalObject()->getType()), NUM_RxnRoleType);
            else if (isSetId()) {
                style = ne_ven_addNewLocalStyle(mw->getSBMLDocument()->getVeneer());
                ne_stl_addToIdList(style, getId());
            }
            mw->getSBMLDocument()->setRenderModified(true);
        }
        
        if (style)
            setStyle(style);
    }
}

void GraphicalObjectBase::unSetStyle() {
    _style = NULL;
    _isSetStyle = false;
    
    for (constGTextIt gTIt = gTextsBegin(); gTIt != gTextsEnd(); ++gTIt)
        (*gTIt)->unSetStyle();
}

sbne::VGlobalStyle* GraphicalObjectBase::getStyle() {
    return _style;
}

void GraphicalObjectBase::addGraphicalCurve(GraphicalCurve* c) {
    _graphicalCurves.push_back(c);
}

void GraphicalObjectBase::removeGraphicalCurve(unsigned int graphicalCurveIndex) {
    if (graphicalCurveIndex >= _graphicalCurves.size() || graphicalCurveIndex < 0)
        std::cerr << "the entered graphical curve index is not within the gCurveVec range\n";
    else {
        // set the iterator to the desired graphical curve
        constGCurveIt _it = gCurvesBegin();
        for (int i = 0; i < graphicalCurveIndex; ++i)
            ++_it;
        // remove the desired graphical species reference from the gCurveVec
        _graphicalCurves.erase(_it);
    }
}

void GraphicalObjectBase::setGraphicalCurves(const gCurveVec& gcv) {
    _graphicalCurves = gcv;
}

const GraphicalObjectBase::gCurveVec& GraphicalObjectBase::getGraphicalCurves() const {
    return _graphicalCurves;
}

const size_t GraphicalObjectBase::getNumGraphicalCurves() const {
    return _graphicalCurves.size();
}

void GraphicalObjectBase::resetGraphicalCurves(MainWindow* mw, const bool& isLineEnding) {
    for (constGCurveIt gCIt = gCurvesBegin(); gCIt != gCurvesEnd(); ++gCIt) {
        (*gCIt)->clearGraphicalPaths(mw, isLineEnding);
        (*gCIt)->clearStartGraphicalItems(mw, isLineEnding);
        (*gCIt)->clearEndGraphicalItems(mw, isLineEnding);
        delete *gCIt;
    }
    
    _graphicalCurves.clear();
}

void GraphicalObjectBase::addGraphicalText(GraphicalText* t) {
    _graphicalTexts.push_back(t);
}

void GraphicalObjectBase::removeGraphicalText(unsigned int graphicalTextIndex) {
    if (graphicalTextIndex >= _graphicalTexts.size() || graphicalTextIndex < 0)
        std::cerr << "the entered graphical text index is not within the gTextVec range\n";
    else {
        // set the iterator to the desired graphical text
        constGTextIt _it = gTextsBegin();
        for (int i = 0; i < graphicalTextIndex; ++i)
            ++_it;
        // remove the desired graphical species reference from the gTextVec
        _graphicalTexts.erase(_it);
    }
}

void GraphicalObjectBase::setGraphicalTexts(const gTextVec& gtv) {
    _graphicalTexts = gtv;
}

const GraphicalObjectBase::gTextVec& GraphicalObjectBase::getGraphicalTexts() const {
    return _graphicalTexts;
}

const size_t GraphicalObjectBase::getNumGraphicalTexts() const {
    return _graphicalTexts.size();
}

void GraphicalObjectBase::resetGraphicalTexts(MainWindow* mw) {
    for (constGTextIt gTIt = gTextsBegin(); gTIt != gTextsEnd(); ++gTIt) {
        (*gTIt)->resetGraphicalItems(mw);
        delete *gTIt;
    }
    
    _graphicalTexts.clear();
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

// Graphical Line Ending

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

void GraphicalLineEnding::updateValues(MainWindow* mw) {
    // reset values
    resetGraphicalItems(mw, true);
    resetGraphicalCurves(mw, true);
    unSetId();
    unSetRotation();
    
    if (isSetLEnding()) {
        sbne::VLineEnding* lE = getLEnding();
        
        // set id
        if (ne_ve_isSetId(lE))
            setId(ne_ve_getId(lE));
        
        // set rotation
        if (ne_le_isSetEnableRotation(lE))
            setRotation(ne_le_getEnableRotation(lE));
        
        // graphical items
        if (ne_le_isSetBoundingBox(lE)) {
            LBox* box = ne_le_getBoundingBox(lE);
            gItemVec gItems = getInfoFromRenderGroup(mw, ne_le_getGroup(lE), box);
            for (int i = 0; i < gItems.size(); ++i)
                addGraphicalItem(gItems.at(i));
            
            gCurveVec gCurves = getInfoFromRenderGroup(mw, ne_le_getGroup(lE), box,  NULL);
            if (gCurves.size())
                setGraphicalCurves(gCurves);
        }
    }
}

// Graphical Compartment

void GraphicalCompartment::updateValues(MainWindow* mw) {
    // reset values
    resetGraphicalItems(mw);
    resetGraphicalCurves(mw);
    resetGraphicalTexts(mw);
    unSetId();
    
    if (isSetNGraphicalObject()) {
        sbne::NCompartment* c = (sbne::NCompartment*)getNGraphicalObject();
        // set id
        if (ne_go_isSetGlyphId(c))
            setId(ne_go_getGlyphId(c));
        
        // graphical items
        if (ne_go_isSetBoundingBox(c)) {
            LBox* box = ne_go_getBoundingBox(c);
            
            // set scene extends
            mw->setSceneRect(QRectF(qreal(ne_bb_getX(box)), qreal(ne_bb_getY(box)), qreal(ne_bb_getWidth(box)), qreal(ne_bb_getHeight(box))));
            
            gItemVec gItems = getInfoFromRenderGroup(mw, ne_stl_getGroup(getStyle()), box);
            
            for (int i = 0; i < gItems.size(); ++i) {
                gItems.at(i)->setZValue(0);
                addGraphicalItem(gItems.at(i));
                mw->getScene()->addItem(gItems.at(i));
            }
            
            gCurveVec gCurves = getInfoFromRenderGroup(mw, ne_stl_getGroup(getStyle()), box,  NULL);
            for(int i = 0; i < gCurves.size(); ++i) {
                addGraphicalCurve(gCurves.at(i));
                for (GraphicalCurve::constGPathIt gPIt = gCurves.at(i)->gPathsBegin(); gPIt != gCurves.at(i)->gPathsEnd(); ++gPIt) {
                    (*gPIt)->setZValue(0);
                    mw->getScene()->addItem(*gPIt);
                }
                setLineEndings(mw, gCurves.at(i));
            }
        }
        
        // graphical text
        GraphicalText* _graphicalText = NULL;
        NText* text = NULL;
        for (int j = 0; j < ne_go_getNumTexts(c); ++j) {
            text = ne_go_getText(c, j);
            
            // create graphical text
            _graphicalText =  new GraphicalText();
            
            // set the compartment as the graphical object of graphical text
            _graphicalText->setAssociatedGObject(c);
            
            // set text glyph
            _graphicalText->setNGraphicalObject(text);
            
            // set id
            if (ne_go_isSetGlyphId(text))
                _graphicalText->setId(ne_go_getGlyphId(text));
            
            // set style
            _graphicalText->setStyle(mw, ST_TYPE_COMP);
            
            // update values
            _graphicalText->updateValues(mw);
            
            // add to the graphical texts of graphical species
            addGraphicalText(_graphicalText);
        }
    }
}

// Graphical Species

void GraphicalSpecies::fitConnectedItemsToBoundingBox(MainWindow* mw) {
    if (isSetNGraphicalObject() && ne_go_isSetBoundingBox(getNGraphicalObject())) {
        for (constGTextIt gTIt = gTextsBegin(); gTIt != gTextsEnd(); ++gTIt) {
            if ((*gTIt)->getGraphicalItems().at(0) && (*gTIt)->isSetPlainText()) {
                QFontMetrics fontMetrics(((MyQGraphicsTextItem*)((*gTIt)->getGraphicalItems().at(0)))->textFont());
                qreal textWidth = fontMetrics.width((QString((*gTIt)->getPlainText().c_str())));
                qreal textHeight = fontMetrics.height();
                sbne::LBox* bbox = ne_go_getBoundingBox(getNGraphicalObject());
                bool isBoxModified = false;
                
                // for width
                if (textWidth > 0.9 * ne_bb_getWidth(bbox)) {
                    textWidth *= 1.15;
                    textWidth = std::min(std::max(maxSpeciesBoxWidth, ne_bb_getWidth(bbox)), double(textWidth));
                    
                    ne_bb_setX(bbox, ne_bb_getX(bbox) - 0.5 * (textWidth - ne_bb_getWidth(bbox)));
                    ne_bb_setWidth(bbox, textWidth);
                    if ((*gTIt)->isSetNGraphicalObject() && ne_go_isSetBoundingBox((*gTIt)->getNGraphicalObject())) {
                        ne_bb_setX(ne_go_getBoundingBox((*gTIt)->getNGraphicalObject()), ne_bb_getX(bbox));
                        ne_bb_setWidth(ne_go_getBoundingBox((*gTIt)->getNGraphicalObject()), ne_bb_getWidth(bbox));
                    }
                    
                    isBoxModified = true;
                }
                
                // for height
                if (textHeight > 0.9 * ne_bb_getHeight(bbox)) {
                    textHeight *= 1.15;
                    textHeight = std::min(std::max(maxSpeciesBoxHeight, ne_bb_getHeight(bbox)), double(textHeight));
                    
                    ne_bb_setY(bbox, ne_bb_getY(bbox) - 0.5 * (textHeight - ne_bb_getHeight(bbox)));
                    ne_bb_setHeight(bbox, textHeight);
                    if ((*gTIt)->isSetNGraphicalObject() && ne_go_isSetBoundingBox((*gTIt)->getNGraphicalObject())) {
                        ne_bb_setY(ne_go_getBoundingBox((*gTIt)->getNGraphicalObject()), ne_bb_getY(bbox));
                        ne_bb_setHeight(ne_go_getBoundingBox((*gTIt)->getNGraphicalObject()), ne_bb_getHeight(bbox));
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
        }
        
        // update the location of curves of graphical species references
        if (mw->isSetSBMLDocument() && !mw->getSBMLDocument()->isLayoutAlreadyExisted()) {
            for (MainWindow::constGReactionIt rIt = mw->gReactionsBegin(); rIt != mw->gReactionsEnd(); ++rIt) {
                for (GraphicalReaction::constGSReferenceIt sRIt = (*rIt)->gSReferencesBegin(); sRIt != (*rIt)->gSReferencesEnd(); ++sRIt) {
                    if ((*sRIt)->isSetNGraphicalObject() && ((NSpeciesReference*)((*sRIt)->getNGraphicalObject()))->isSetSpecies() && ((NSpeciesReference*)((*sRIt)->getNGraphicalObject()))->getSpecies()->isSetId() && sbne::stringCompare(((NSpeciesReference*)((*sRIt)->getNGraphicalObject()))->getSpecies()->getId(), getNGraphicalObject()->getId())) {
                        (*sRIt)->fitToSpeciesPosition(mw);
                    }
                }
            }
        }
    }
}

void GraphicalSpecies::updateValues(MainWindow* mw, const bool& _fitConnectedItems) {
    // reset values
    resetGraphicalItems(mw);
    resetGraphicalCurves(mw);
    resetGraphicalTexts(mw);
    unSetId();
    
    if (isSetNGraphicalObject()) {
        sbne::NSpecies* s = (sbne::NSpecies*)getNGraphicalObject();
        
        // set id
        if (ne_go_isSetGlyphId(s))
            setId(ne_go_getGlyphId(s));
        
        // graphical items
        if (ne_go_isSetBoundingBox(s)) {
            LBox* box = ne_go_getBoundingBox(s);
            gItemVec gItems = getInfoFromRenderGroup(mw, ne_stl_getGroup(getStyle()), box);
                
            for (int i = 0; i < gItems.size(); ++i) {
                gItems.at(i)->setZValue(4);
                addGraphicalItem(gItems.at(i));
                mw->getScene()->addItem(gItems.at(i));
            }
            
            gCurveVec gCurves = getInfoFromRenderGroup(mw, ne_stl_getGroup(getStyle()), box,  NULL);
            for (int i = 0; i < gCurves.size(); ++i) {
                addGraphicalCurve(gCurves.at(i));
                for (GraphicalCurve::constGPathIt gPIt = gCurves.at(i)->gPathsBegin(); gPIt != gCurves.at(i)->gPathsEnd(); ++gPIt) {
                    (*gPIt)->setZValue(4);
                    mw->getScene()->addItem(*gPIt);
                }
                setLineEndings(mw, gCurves.at(i));
            }
        }
        
        // graphical text
        GraphicalText* _graphicalText = NULL;
        NText* text = NULL;
        for (int j = 0; j < ne_go_getNumTexts(s); ++j) {
            text = ne_go_getText(s, j);
            
            // create graphical text
            _graphicalText =  new GraphicalText();
            
            // set the species as the graphical object of graphical text
            _graphicalText->setAssociatedGObject(s);
            
            // set text glyph
            _graphicalText->setNGraphicalObject(text);
            
            // set id
            if (ne_go_isSetGlyphId(text))
                _graphicalText->setId(ne_go_getGlyphId(text));
            
            // set style
            _graphicalText->setStyle(mw, ST_TYPE_TXT);
            
            // update values
            _graphicalText->updateValues(mw);
            
            // add to the graphical texts of graphical species
            addGraphicalText(_graphicalText);
        }
        
#if !GRAPHVIZ_INCLUDED
        // fit species connected items to the species bounding box
        if (_fitConnectedItems)
            fitConnectedItemsToBoundingBox(mw);
#endif
    }
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

void GraphicalReaction::updateValues(MainWindow* mw) {
    // reset values
    resetGraphicalItems(mw);
    resetGraphicalCurves(mw);
    resetGraphicalTexts(mw);
    unSetId();
    
    if (isSetNGraphicalObject()) {
        sbne::NReaction* r = (sbne::NReaction*)getNGraphicalObject();
        
        // set id
        if (ne_go_isSetGlyphId(r))
            setId(ne_go_getGlyphId(r));
        
        // curve
        if (ne_rxn_isSetCurve(r)) {
            gCurveVec gCurves = getInfoFromRenderGroup(mw, ne_stl_getGroup(getStyle()), NULL,  ne_rxn_getCurve(r));
            for (int i = 0; i < gCurves.size(); ++i) {
                addGraphicalCurve(gCurves.at(i));
                for (GraphicalCurve::constGPathIt gPIt = gCurves.at(i)->gPathsBegin(); gPIt != gCurves.at(i)->gPathsEnd(); ++gPIt) {
                    (*gPIt)->setZValue(2);
                    mw->getScene()->addItem(*gPIt);
                }
                setLineEndings(mw, gCurves.at(i));
            }
        }
        // graphical items
        else if (ne_go_isSetBoundingBox(r)) {
            LBox* box = ne_go_getBoundingBox(r);
            gItemVec gItems = getInfoFromRenderGroup(mw, ne_stl_getGroup(getStyle()), box);
                
            for(int i = 0; i < gItems.size(); ++i) {
                gItems.at(i)->setZValue(2);
                addGraphicalItem(gItems.at(i));
                mw->getScene()->addItem(gItems.at(i));
            }
            
            gCurveVec gCurves = getInfoFromRenderGroup(mw, ne_stl_getGroup(getStyle()), box,  NULL);
            for (int i = 0; i < gCurves.size(); ++i) {
                addGraphicalCurve(gCurves.at(i));
                for (GraphicalCurve::constGPathIt gPIt = gCurves.at(i)->gPathsBegin(); gPIt != gCurves.at(i)->gPathsEnd(); ++gPIt) {
                    (*gPIt)->setZValue(2);
                    mw->getScene()->addItem(*gPIt);
                }
                setLineEndings(mw, gCurves.at(i));
            }
        }
        
        // graphical text
        GraphicalText* _graphicalText = NULL;
        NText* text = NULL;
        for (int j = 0; j < ne_go_getNumTexts(r); ++j) {
            text = ne_go_getText(r, j);
            
            // create graphical text
            _graphicalText =  new GraphicalText();
            
            // set the species as the graphical object of graphical text
            _graphicalText->setAssociatedGObject(r);
            
            // set text glyph
            _graphicalText->setNGraphicalObject(text);
            
            // set id
            if (ne_go_isSetGlyphId(text))
                _graphicalText->setId(ne_go_getGlyphId(text));
            
            // set style
            _graphicalText->setStyle(mw, ST_TYPE_TXT);
            
            // update values
            _graphicalText->updateValues(mw);
            
            // add to the graphical texts of graphical species
            addGraphicalText(_graphicalText);
        }
    }
}

// Graphical SReference

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
    if (isSetNGraphicalObject() && ((NSpeciesReference*)getNGraphicalObject())->isSetSpecies()) {
        NSpeciesReference* sr = (NSpeciesReference*)getNGraphicalObject();
        NSpecies* s =((NSpeciesReference*)getNGraphicalObject())->getSpecies();
        LCurve::constElementIt eIt;
        LPoint boxPoint;
        SpcSide speciesSide;
        for (constGCurveIt gCIt = gCurvesBegin(); gCIt != gCurvesEnd(); ++gCIt) {
            if ((*gCIt)->isSetLCurve()) {
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
                    eIt = (*gCIt)->getLCurve()->elementsEnd() - 1;
                    (*eIt)->setEnd(boxPoint);
                }
                // for substrates, modifiers, activators, and inhibitors
                else {
                    // update the start point
                    eIt = (*gCIt)->getLCurve()->elementsBegin();
                    (*eIt)->setStart(boxPoint);
                }
                
                // enable the flag of layout modification
                mw->getSBMLDocument()->setLayoutModified(true);
                
                (*gCIt)->updateValues(mw, getStyle(), true);
            }
        }
    }
}

void GraphicalSReference::updateValues(MainWindow* mw) {
    // reset values
    resetGraphicalItems(mw);
    resetGraphicalCurves(mw);
    unSetId();
    unSetRole();
    
    if (isSetNGraphicalObject()) {
        NSpeciesReference* sr = (NSpeciesReference*)getNGraphicalObject();
        
        // set id
        if (ne_go_isSetGlyphId(sr))
            setId(ne_go_getGlyphId(sr));
        
        // set the role of graphical species reference
        if (ne_sr_isSetRole(sr))
            setRole(ne_sr_getRoleAsString(sr));
        
        // curve
        if (ne_sr_isSetCurve(sr)) {
            gCurveVec gCurves = getInfoFromRenderGroup(mw, ne_stl_getGroup(getStyle()), NULL,  ne_sr_getCurve(sr));
            for (int i = 0; i < gCurves.size(); ++i) {
                addGraphicalCurve(gCurves.at(i));
                for (GraphicalCurve::constGPathIt gPIt = gCurves.at(i)->gPathsBegin(); gPIt != gCurves.at(i)->gPathsEnd(); ++gPIt) {
                    (*gPIt)->setZValue(1);
                    mw->getScene()->addItem(*gPIt);
                }
                setLineEndings(mw, gCurves.at(i));
            }
        }
    }
}

// Graphical Text

void GraphicalText::setAssociatedGObject(sbne::NGraphicalObject* gO) {
    _associatedGObject = gO;
    _isSetAssociatedGObject = true;
}

sbne::NGraphicalObject* GraphicalText::getAssociatedGObject() {
    return _associatedGObject;
}

void GraphicalText::setStyle(MainWindow* mw, const sbne::StyleType& styleType) {
    if (mw && mw->isSetSBMLDocument()) {
        sbne::VGlobalStyle* style = ne_ven_findStyle(mw->getSBMLDocument()->getVeneer(), getNGraphicalObject(), styleType);
        if (style)
            GraphicalObjectBase::setStyle(style);
        else
            GraphicalObjectBase::setStyle(mw, true);
    }
}

void GraphicalText::updateValues(MainWindow* mw) {
    // reset values
    resetGraphicalItems(mw);
    unSetId();
    
    sbne::NText* t = NULL;
    if (isSetNGraphicalObject()) {
        unSetPlainText();
        t = (NText*)getNGraphicalObject();
        
        // set id
        if (ne_go_isSetGlyphId(t))
            setId(ne_go_getGlyphId(t));
        
        // set plain text
        if (ne_gtxt_isSetPlainText(t))
            setPlainText(ne_gtxt_getPlainText(t));
        else if (mw && mw->isSetSBMLDocument() && mw->getSBMLDocument()->getLayoutInfo()) {
            NGraphicalObject* gO = NULL;
            // origin of text id is set
            if (ne_gtxt_isSetOriginOfTextId(t))
                gO = ne_net_getNetworkElement(mw->getSBMLDocument()->getLayoutInfo()->net, ne_gtxt_getOriginOfTextId(t));
            // graphical object id is set
            else if (ne_gtxt_isSetGraphicalObjectId(t))
                gO = ne_net_getNetworkElement(mw->getSBMLDocument()->getLayoutInfo()->net, ne_gtxt_getGraphicalObjectId(t));
            
            if (gO) {
                // get name
                if (gO->isSetName())
                    setPlainText(gO->getName());
                // get id
                else if (gO->isSetId())
                    setPlainText(gO->getId());
                // get glyph id
                else if (gO->isSetGlyphId())
                    setPlainText(gO->getGlyphId());
            }
        }
    }
    
    if (isSetAssociatedGObject()) {
        
        // set graphical item
        if (((t && ne_go_isSetBoundingBox(t)) || ne_go_isSetBoundingBox(getAssociatedGObject())) && isSetPlainText()) {
            
            sbne::LBox* bbox = ne_go_getBoundingBox(getNGraphicalObject());
            if (!bbox)
                bbox = ne_go_getBoundingBox(getAssociatedGObject());
            
            if (bbox) {
                // graphical item
                gItemVec _gItems(0);
                _gItems.push_back(new MyQGraphicsTextItem(QString(getPlainText().c_str()), QRectF(ne_bb_getX(bbox) + 0.05 * ne_bb_getWidth(bbox), ne_bb_getY(bbox) + 0.05 * ne_bb_getHeight(bbox), 0.9 * ne_bb_getWidth(bbox), 0.9 * ne_bb_getHeight(bbox))));
                
                if (isSetStyle())
                    _gItems = getInfoFromRenderGroup(mw, ne_stl_getGroup(getStyle()), (MyQGraphicsTextItem*)_gItems.at(0));
                
                for(int i = 0; i < _gItems.size(); ++i) {
                    _gItems.at(i)->setZValue(5);
                    addGraphicalItem(_gItems.at(i));
                    mw->getScene()->addItem(_gItems.at(i));
                }
            }
        }
    }
}

void GraphicalText::setPlainText(const std::string& plainText) {
    _plainText = plainText;
    _isSetPlainText = true;
}

void GraphicalText::unSetPlainText() {
    _plainText.clear();
    _isSetPlainText = false;
}

const std::string& GraphicalText::getPlainText() const {
    return _plainText;
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

void GraphicalCurve::clearGraphicalPaths(MainWindow* mw, const bool& isLineEnding) {
    gPathIt pIt = _graphicalPaths.begin();
    while (pIt != _graphicalPaths.end()) {
        if (!isLineEnding)
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

void GraphicalCurve::addStartGraphicalItem(QGraphicsItem* g) {
    _startGraphicalItems.push_back(g);
}

void GraphicalCurve::removeStartGraphicalItem(unsigned int graphicalItemIndex) {
    if (graphicalItemIndex >= _startGraphicalItems.size() || graphicalItemIndex < 0)
        std::cerr << "the entered graphical item index is not within the startGItemVec range\n";
    else {
        // set the iterator to the desired graphical item
        constGItemIt _it = startGItemsBegin();
        for (int i = 0; i < graphicalItemIndex; ++i)
            ++_it;
        // remove the desired graphical species reference from the gItemVec
        _startGraphicalItems.erase(_it);
    }
}

void GraphicalCurve::setStartGraphicalItems(const gItemVec& giv) {
    _startGraphicalItems = giv;
}

const GraphicalCurve::gItemVec& GraphicalCurve::getStartGraphicalItems() const {
    return _startGraphicalItems;
}

void GraphicalCurve::clearStartGraphicalItems(MainWindow* mw, const bool& isLineEnding) {
    gItemIt iIt = _startGraphicalItems.begin();
    while (iIt != _startGraphicalItems.end()) {
        if (!isLineEnding)
            mw->getScene()->removeItem(*iIt);
        delete *iIt;
        _startGraphicalItems.erase(iIt);
        iIt = _startGraphicalItems.begin();
    }
}

void GraphicalCurve::addEndGraphicalItem(QGraphicsItem* g) {
    _endGraphicalItems.push_back(g);
}

void GraphicalCurve::removeEndGraphicalItem(unsigned int graphicalItemIndex) {
    if (graphicalItemIndex >= _endGraphicalItems.size() || graphicalItemIndex < 0)
        std::cerr << "the entered graphical item index is not within the endGItemVec range\n";
    else {
        // set the iterator to the desired graphical item
        constGItemIt _it = endGItemsBegin();
        for (int i = 0; i < graphicalItemIndex; ++i)
            ++_it;
        // remove the desired graphical species reference from the gItemVec
        _endGraphicalItems.erase(_it);
    }
}

void GraphicalCurve::setEndGraphicalItems(const gItemVec& giv) {
    _endGraphicalItems = giv;
}

const GraphicalCurve::gItemVec& GraphicalCurve::getEndGraphicalItems() const {
    return _endGraphicalItems;
}

void GraphicalCurve::clearEndGraphicalItems(MainWindow* mw, const bool& isLineEnding) {
    gItemIt iIt = _endGraphicalItems.begin();
    while (iIt != _endGraphicalItems.end()) {
        if (!isLineEnding)
            mw->getScene()->removeItem(*iIt);
        delete *iIt;
        _endGraphicalItems.erase(iIt);
        iIt = _endGraphicalItems.begin();
    }
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
    // reset values
    clearStartGraphicalItems(mw);
    clearEndGraphicalItems(mw);
    unSetStartPoint();
    unSetEndPoint();
    unSetStartSlope();
    unSetEndSlope();
    unSetStartEnableRotation();
    unSetEndEnableRotation();
    unSetStartLineEnding();
    unSetEndLineEnding();
    unSetId();
}
