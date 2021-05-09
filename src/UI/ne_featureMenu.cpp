#include "ne_featureMenu.h"

// Feature Menu

FeatureMenu::FeatureMenu(QWidget* parent, MainWindow* mw) {
    _featureMenuElements.clear();
    FeatureMenuElement* fMElement;
    
    // add compartment feature menu
    fMElement = new CompartmentFeatureMenu(parent, mw);
    _featureMenuElements.push_back(fMElement);
    
    // add species feature menu
    fMElement = new SpeciesFeatureMenu(parent, mw);
    _featureMenuElements.push_back(fMElement);
    
    // add reaction feature menu
    fMElement = new ReactionFeatureMenu(parent, mw);
    _featureMenuElements.push_back(fMElement);
    
    // add species reference feature menu
    fMElement = new SpeciesReferenceFeatureMenu(parent, mw);
    _featureMenuElements.push_back(fMElement);
    
    // add line ending feature menu
    fMElement = new LineEndingFeatureMenu(parent, mw);
    _featureMenuElements.push_back(fMElement);
}

const FeatureMenu::fMenuElementVec& FeatureMenu::getFeatureMenuElements() const {
    return _featureMenuElements;
}

const size_t FeatureMenu::getNumFeatureMenuElements() const {
    return _featureMenuElements.size();
}

void FeatureMenu::showFeatureMenu(GraphicalCompartment* gC) {
    ((CompartmentFeatureMenu*)(_featureMenuElements.at(0)))->showInfo(gC);
}

void FeatureMenu::showFeatureMenu(GraphicalSpecies* gS) {
    ((SpeciesFeatureMenu*)(_featureMenuElements.at(1)))->showInfo(gS);
}

void FeatureMenu::showFeatureMenu(GraphicalReaction* gR) {
    ((ReactionFeatureMenu*)(_featureMenuElements.at(2)))->showInfo(gR);
}

void FeatureMenu::showFeatureMenu(GraphicalSReference* gSR) {
    ((SpeciesReferenceFeatureMenu*)(_featureMenuElements.at(3)))->showInfo(gSR);
}

void FeatureMenu::showFeatureMenu(GraphicalLineEnding* gLE, GraphicalSReference* gSR, QString head) {
    ((LineEndingFeatureMenu*)(_featureMenuElements.at(4)))->showInfo(gLE, gSR, head);
}

void FeatureMenu::hideFeatureMenu() {
    for (constFMenuElementIt fMEIt = fMenuElementsBegin(); fMEIt != fMenuElementsEnd(); ++fMEIt)
        (*fMEIt)->hideInfo();
}

// Feature Menu Element

FeatureMenuElement::FeatureMenuElement(QWidget* parent, MainWindow* mw) : QGroupBox(parent) {
    _mw = mw;
    hide();
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    setFixedSize(QSize(250, maximumHeight()));
    
    _gO = NULL;
    _style = NULL;
    _lE = NULL;
    _gText = NULL;
    modelMenu = NULL;
    layoutMenu = NULL;
    renderMenu = NULL;
    bBoxFMenu = NULL;
    curveFMenu = NULL;
    strokeFMenu = NULL;
    fillFMenu = NULL;
    textFMenu = NULL;
    gSFMenuElement = NULL;
    
    setStyleSheet("QGroupBox { background-color: pale gray; font: bold; border: 1px solid gray; border-radius: 5px; margin-top: 1ex; }" "QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top center; padding: 0px 5px 0px 5px; }");
    setContentsMargins(0, 0, 0, 0);
    setTitle("Item Features");
    
    headlineFont = itemLabel.font();
    featureTitleFont = itemLabel.font();
    headlineFont.setBold(true);
    featureTitleFont.setBold(true);
    headlineFont.setCapitalization(QFont::SmallCaps);
    featureTitleFont.setCapitalization(QFont::Capitalize);
    headlineFont.setPointSize(24);
    featureTitleFont.setPointSize(12);
    pushButtonStyleSheet = "QPushButton { border: 2px solid #8f8f91; border-radius: 4px; background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #f6f7fa, stop: 1 #dadbde); min-width: 80px; } QPushButton:pressed { background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #dadbde, stop: 1 #f6f7fa); }";
    disabledPushButtonStyleSheet = "QPushButton { border: 2px solid #8f8f91; border-radius: 4px; background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #f6f7fa, stop: 1 #dadbde); color: #bb848e;  min-width: 80px; }";
    
    itemLabel.setFont(headlineFont);
    headlineFont.setPointSize(14);

    qLayout.setAlignment(Qt::AlignTop);
    qLayout.setContentsMargins(10, 20, 10, 10);
    setLayout(&qLayout);
    qLayout.addWidget(&itemLabel, qLayout.rowCount() - 1, 0);
    
    // model features
    modelIdTitleLabel.setText("Id: ");
    modelIdTitleLabel.setFont(featureTitleFont);
    
    // layout features
    layoutIdLabel.setText("Layout Id: ");
    layoutIdLabel.setFont(featureTitleFont);
    layoutIdButton.setStyleSheet(pushButtonStyleSheet);
    connect(&layoutIdButton, SIGNAL(clicked()), this, SLOT(changeLayoutId()));
    
    // bounding box features
    boundingBoxTitleLabel.setFont(headlineFont);
    boundingBoxTitleLabel.setText("Bounding Box");
    boundingBoxLabel.setText("Bounding Box");
    boundingBoxLabel.setFont(featureTitleFont);
    boundingBoxButton.setStyleSheet(pushButtonStyleSheet);
    connect(&boundingBoxButton, SIGNAL(clicked()), this, SLOT(addOrRemoveBoundingBox()));
    
    // curve features
    curveTitleLabel.setFont(headlineFont);
    curveTitleLabel.setText("Curve");
    curveLabel.setText("Curve");
    curveLabel.setFont(featureTitleFont);
    curveButton.setStyleSheet(pushButtonStyleSheet);
    connect(&curveButton, SIGNAL(clicked()), this, SLOT(addOrRemoveCurve()));
    
    // style features
    styleLabel.setText("Style");
    styleLabel.setFont(featureTitleFont);
    styleButton.setStyleSheet(pushButtonStyleSheet);
    connect(&styleButton, SIGNAL(clicked()), this, SLOT(addOrRemoveStyle()));
    
    // stroke features
    strokeTitleLabel.setFont(headlineFont);
    strokeTitleLabel.setText("Stroke");
    
    // fill features
    fillTitleLabel.setFont(headlineFont);
    fillTitleLabel.setText("Fill");
    
    // text features
    textTitleLabel.setFont(headlineFont);
    textTitleLabel.setText("Text");
    textLabel.setText("Text");
    textLabel.setFont(featureTitleFont);
    textButton.setStyleSheet(pushButtonStyleSheet);
    connect(&textButton, SIGNAL(clicked()), this, SLOT(changeText()));
    
    // geometric shape features
    geometricShapeTitleLabel.setFont(headlineFont);
    geometricShapeTitleLabel.setText("Geometric Shape");
    geometricShapeLabel.setText("Shape");
    geometricShapeLabel.setFont(featureTitleFont);
    geometricShapeButton.setStyleSheet(pushButtonStyleSheet);
    connect(&geometricShapeButton, SIGNAL(clicked()), this, SLOT(changeGeometricShape()));
}

void FeatureMenuElement::hideInfo() {
    hide();
    
    if (modelMenu)
        modelMenu->collapseMenuWithoutAnimation();
    if (layoutMenu)
        layoutMenu->collapseMenuWithoutAnimation();
    if (renderMenu)
        renderMenu->collapseMenuWithoutAnimation();
}

void FeatureMenuElement::changeLayoutId() {
    QInputDialog* modificationBox =  new QInputDialog();
    modificationBox->setWindowTitle("Change Layout Id");
    modificationBox->setLabelText("Layout Id:");
    modificationBox->setOkButtonText("Apply");
    modificationBox->resize(300, 200);
    
    if (layoutIdButton.text() != "N/A")
        modificationBox->setTextValue(layoutIdButton.text());
    
    if (modificationBox->exec() && _mw && _mw->isSetSBMLDocument() && !modificationBox->textValue().isEmpty() && !ne_go_setGlyphId(_mw->getSBMLDocument()->getNetwork(), _gO, modificationBox->textValue().toStdString())) {
        _mw->getSBMLDocument()->setLayoutModified(true);
        updateValues();
    }
}

void FeatureMenuElement::addOrRemoveBoundingBox() {
    if (_gO && bBoxFMenu) {
        if (boundingBoxButton.text() == "Remove") {
            QMessageBox* removeMessageBox =  new QMessageBox();
            removeMessageBox->setWindowTitle("Remove Bounding Box");
            removeMessageBox->setInformativeText("Are you sure you want to remove the Bounding Box of this Graphical Object?");
            removeMessageBox->setText("Bounding Box                                                                                                 ");
            removeMessageBox->setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            removeMessageBox->setDefaultButton(QMessageBox::No);
            
            if (removeMessageBox->exec() ==  QMessageBox::Yes && ne_go_isSetBoundingBox(_gO)) {
                if (_gText && ne_go_isSetText(_gO)) {
                    NText* text = ne_go_getText(_gO);
                    if (_gText->isSetPlainText())
                        ne_gtxt_setPlainText(text, _gText->getPlainText());
                    _gText->setText(text);
                }
                
                ne_go_unSetBoundingBox(_gO, false);
                _mw->getSBMLDocument()->setLayoutModified(true);
                updateValues();
            }
        }
        else if (boundingBoxButton.text() == "Add") {
            QDialog* modificationBox =  new QDialog();
            modificationBox->setWindowTitle("Add Bounding Box to the Graphical Object");
            QGridLayout modificationBoxLayout(modificationBox);
            modificationBox->resize(300, 200);
            
            QLabel* boundingBoxFeaturesLabel = new QLabel("Bounding Box Features");
            boundingBoxFeaturesLabel->setFont(featureTitleFont);
            QDoubleSpinBox* xSpinBox = new QDoubleSpinBox(modificationBox);
            xSpinBox->setRange(_mw->getScene()->sceneRect().x(), _mw->getScene()->sceneRect().x() + _mw->getScene()->sceneRect().width());
            QDoubleSpinBox* ySpinBox = new QDoubleSpinBox(modificationBox);
            ySpinBox->setRange(_mw->getScene()->sceneRect().y(), _mw->getScene()->sceneRect().y() + _mw->getScene()->sceneRect().height());
            QDoubleSpinBox* widthSpinBox = new QDoubleSpinBox(modificationBox);
            widthSpinBox->setRange(0.0, _mw->getScene()->sceneRect().width());
            QDoubleSpinBox* heightSpinBox = new QDoubleSpinBox(modificationBox);
            heightSpinBox->setRange(0.0, _mw->getScene()->sceneRect().height());
            
            LBox* bBox = NULL;
            if (ne_go_getBoundingBox(_gO))
                bBox = ne_go_getBoundingBox(_gO);
            else if (ne_go_getType(_gO) == 2 && ne_rxn_getCurve((NReaction*)(_gO))) {
                bBox = ne_crv_getExtentBox(ne_rxn_getCurve((NReaction*)(_gO)));
                ne_bb_setWidth(bBox, std::max(5.0, bBox->width()));
                ne_bb_setHeight(bBox, std::max(5.0, bBox->height()));
            }
            
            if (bBox) {
                xSpinBox->setValue(ne_bb_getX(bBox));
                ySpinBox->setValue(ne_bb_getY(bBox));
                widthSpinBox->setValue(ne_bb_getWidth(bBox));
                heightSpinBox->setValue(ne_bb_getHeight(bBox));
            }
            
            modificationBoxLayout.addWidget(boundingBoxFeaturesLabel, modificationBoxLayout.rowCount(), 0, 1, 2);
            modificationBoxLayout.addWidget(new QLabel("X:"), modificationBoxLayout.rowCount(), 0);
            modificationBoxLayout.addWidget(xSpinBox, modificationBoxLayout.rowCount() - 1, 1, 1, 2);
            modificationBoxLayout.addWidget(new QLabel("Y:"), modificationBoxLayout.rowCount(), 0);
            modificationBoxLayout.addWidget(ySpinBox, modificationBoxLayout.rowCount() - 1, 1, 1, 2);
            modificationBoxLayout.addWidget(new QLabel("Width:"), modificationBoxLayout.rowCount(), 0);
            modificationBoxLayout.addWidget(widthSpinBox, modificationBoxLayout.rowCount() - 1, 1, 1, 2);
            modificationBoxLayout.addWidget(new QLabel("Height:"), modificationBoxLayout.rowCount(), 0);
            modificationBoxLayout.addWidget(heightSpinBox, modificationBoxLayout.rowCount() - 1, 1, 1, 2);
            
            QDialogButtonBox modificationBoxButtons(Qt::Horizontal, modificationBox);
            modificationBoxButtons.addButton(QDialogButtonBox::Cancel);
            modificationBoxButtons.addButton(QString("Add"), QDialogButtonBox::AcceptRole);
            modificationBoxLayout.addWidget(&modificationBoxButtons, modificationBoxLayout.rowCount(), 0, 1, 3);
            QObject::connect(&modificationBoxButtons, SIGNAL(accepted()), modificationBox, SLOT(accept()));
            QObject::connect(&modificationBoxButtons, SIGNAL(rejected()), modificationBox, SLOT(reject()));
            
            if (modificationBox->exec() == QDialog::Accepted && _mw && _mw->isSetSBMLDocument()) {
                if (ne_go_getType(_gO) == 2 && ne_rxn_isSetCurve((NReaction*)(_gO)) && curveFMenu) {
                    QMessageBox* replaceCurveMessageBox =  new QMessageBox();
                    replaceCurveMessageBox->setWindowTitle("Replace Curve");
                    replaceCurveMessageBox->setText("A Curve is already associated with this Reaction");
                    replaceCurveMessageBox->setInformativeText("Do you want this Bounding Box to replace it?");
                    
                    replaceCurveMessageBox->setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                    replaceCurveMessageBox->setDefaultButton(QMessageBox::No);
                    
                    if (replaceCurveMessageBox->exec() ==  QMessageBox::Yes) {
                        ne_rxn_unSetCurve((NReaction*)(_gO), false);
                        curveFMenu->resetValues();
                        curveButton.setText("Add");
                    }
                }
                
                if (!bBox)
                    bBox = new LBox();
                
                ne_bb_setX(bBox, xSpinBox->value());
                ne_bb_setY(bBox, ySpinBox->value());
                ne_bb_setWidth(bBox, widthSpinBox->value());
                ne_bb_setHeight(bBox, heightSpinBox->value());
                
                ne_go_setBoundingBox(_gO, bBox);
                _mw->getSBMLDocument()->setLayoutModified(true);
                updateValues();
            }
        }
    }
}

void FeatureMenuElement::addOrRemoveStyle() {
    if (_gO) {
        if (styleButton.text() == "Remove") {
            QMessageBox* removeMessageBox =  new QMessageBox();
            removeMessageBox->setWindowTitle("Remove Style");
            removeMessageBox->setInformativeText("Are you sure you want to remove the Style of this Graphical Object?");
            removeMessageBox->setText("Style                                                                                                 ");
            removeMessageBox->setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            removeMessageBox->setDefaultButton(QMessageBox::No);
            
            if (removeMessageBox->exec() ==  QMessageBox::Yes)
                unSetStyle();
        }
        else if (styleButton.text() == "Add")
             setStyle();
    }
}

void FeatureMenuElement::changeGeometricShape() {
    QDialog* modificationBox =  new QDialog();
    modificationBox->setWindowTitle("Change Geometric Shape");
    QGridLayout modificationBoxLayout(modificationBox);
    
    QComboBox* geometricShapeComboBox = new QComboBox(modificationBox);
    QStringList geometricShapeItems;
    geometricShapeItems << "Rectangle";
    geometricShapeItems << "Ellipse";
    geometricShapeItems << "Polygon";
    geometricShapeItems << "Image";
    if (ne_go_getType(_gO) == 2)
        geometricShapeItems << "RenderCurve";
    geometricShapeItems << "none";
    geometricShapeComboBox->addItems(geometricShapeItems);
    int geometricShapeIndex = 0;
    for (geometricShapeIndex = 0; geometricShapeIndex < geometricShapeItems.length(); ++geometricShapeIndex) {
        if (geometricShapeButton.text() == geometricShapeItems.at(geometricShapeIndex))
            break;
    }
    if (geometricShapeIndex == geometricShapeItems.length())
        --geometricShapeIndex;
    geometricShapeComboBox->setCurrentIndex(geometricShapeIndex);
    
    QLabel* polygonNumberOfVerticesLabel = new QLabel("Number of Vertices:");
    polygonNumberOfVerticesLabel->hide();
    QComboBox* polygonNumberOfVerticesComboBox = new QComboBox(modificationBox);
    QStringList polygonNumberOfVerticesItems;
    polygonNumberOfVerticesItems << QString::number(3) << QString::number(4) << QString::number(5) << QString::number(6) << QString::number(8);
    polygonNumberOfVerticesComboBox->addItems(polygonNumberOfVerticesItems);
    polygonNumberOfVerticesComboBox->hide();
    int numberOfPolygonVertices;
    
    QPushButton* imageInsertButton = new QPushButton("Insert", modificationBox);
    imageInsertButton->hide();
    QString imageFileName;
    
    modificationBoxLayout.addWidget(new QLabel("Geometric Shape:"), modificationBoxLayout.rowCount(), 0);
    modificationBoxLayout.addWidget(geometricShapeComboBox, modificationBoxLayout.rowCount() - 1, 1);
    modificationBoxLayout.addWidget(polygonNumberOfVerticesLabel, modificationBoxLayout.rowCount(), 0);
    modificationBoxLayout.addWidget(polygonNumberOfVerticesComboBox, modificationBoxLayout.rowCount() - 1, 1);
    modificationBoxLayout.addWidget(imageInsertButton, modificationBoxLayout.rowCount(), 1);
    
    QDialogButtonBox modificationBoxButtons(Qt::Horizontal, modificationBox);
    modificationBoxButtons.addButton(QDialogButtonBox::Cancel);
    modificationBoxButtons.addButton(QString("Apply"), QDialogButtonBox::AcceptRole);
    modificationBoxLayout.addWidget(&modificationBoxButtons, modificationBoxLayout.rowCount(), 1, 1, 2);
    QObject::connect(&modificationBoxButtons, SIGNAL(accepted()), modificationBox, SLOT(accept()));
    QObject::connect(&modificationBoxButtons, SIGNAL(rejected()), modificationBox, SLOT(reject()));
    
    if (geometricShapeComboBox->currentIndex() == 2) {
        polygonNumberOfVerticesLabel->show();
        polygonNumberOfVerticesComboBox->show();
        numberOfPolygonVertices = polygonNumberOfVerticesComboBox->currentText().toInt();
    }
    else if (geometricShapeComboBox->currentIndex() == 3) {
        imageInsertButton->show();
        modificationBoxButtons.buttons().at(0)->setEnabled(false);
    }
    
    QObject::connect(geometricShapeComboBox, QOverload<int>::of(&QComboBox::activated), modificationBox, [modificationBox, polygonNumberOfVerticesLabel, polygonNumberOfVerticesComboBox, &numberOfPolygonVertices, imageInsertButton,  &modificationBoxButtons, &imageFileName] (const int& index) {
        polygonNumberOfVerticesLabel->hide();
        polygonNumberOfVerticesComboBox->hide();
        numberOfPolygonVertices = -1;
        imageInsertButton->hide();
        modificationBoxButtons.buttons().at(0)->setEnabled(true);
        imageFileName.clear();
        
        if (index == 2) {
            polygonNumberOfVerticesLabel->show();
            polygonNumberOfVerticesComboBox->show();
            numberOfPolygonVertices = polygonNumberOfVerticesComboBox->currentText().toInt();
        }
        else if (index == 3) {
            imageInsertButton->show();
            modificationBoxButtons.buttons().at(0)->setEnabled(false);
        }
        else
            
        modificationBox->layout()->setAlignment(Qt::AlignTop);
        modificationBox->layout()->update();
        modificationBox->adjustSize();
    });
    
    QObject::connect(polygonNumberOfVerticesComboBox, &QComboBox::currentTextChanged, this, [&numberOfPolygonVertices] (const QString& text) {numberOfPolygonVertices = text.toInt(); });
    
    QObject::connect(imageInsertButton, &QPushButton::clicked, modificationBox, [modificationBox, &modificationBoxButtons, &imageFileName] () {
        imageFileName.clear();
        imageFileName = QFileDialog::getOpenFileName(modificationBox, "Select an image file", ".", tr("Image Files (*.png *.jpg *.jpeg)"));
        
        if (!imageFileName.isEmpty())
             modificationBoxButtons.buttons().at(0)->setEnabled(true);
    });
    
    if (modificationBox->exec())
        updateGeometricShape(geometricShapeComboBox->currentText(), numberOfPolygonVertices, imageFileName);
}

void FeatureMenuElement::updateGeometricShape(const QString& shapeType, const int& numberOfPolygonVertices, const QString& imageFileName) {
    if (_mw && _mw->isSetSBMLDocument()) {
        VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gO, _style);
        
        if (!ne_grp_removeGeometricShapes(ne_stl_getGroup(style))) {
            if (shapeType == "Rectangle")
                ne_grp_addGeometricShape(ne_stl_getGroup(style), GRP_ELT_SHPE_REC);
            else if (shapeType == "Ellipse")
                ne_grp_addGeometricShape(ne_stl_getGroup(style), GRP_ELT_SHPE_ELP);
            else if (shapeType == "Polygon" && numberOfPolygonVertices != -1)
                ne_grp_addGeometricShape(ne_stl_getGroup(style), GRP_ELT_SHPE_PLG, false, numberOfPolygonVertices);
            else if (shapeType == "Image" && !imageFileName.isEmpty())
                ne_grp_addGeometricShape(ne_stl_getGroup(style), GRP_ELT_SHPE_IMG, false, -1, imageFileName.toStdString());
            else if (shapeType == "RenderCurve")
                ne_grp_addGeometricShape(ne_stl_getGroup(style), GRP_ELT_SHPE_CRV);
            
            _mw->getSBMLDocument()->setRenderModified(true);
            updateStyle(style);
        }
    }
}

void FeatureMenuElement::changeText() {
    if (_gText) {
        QDialog* modificationBox =  new QDialog();
        modificationBox->setWindowTitle("Change Text");
        QGridLayout modificationBoxLayout(modificationBox);
        
        QString textId, textName, textLayoutId;
        if (ne_ne_isSetId(_gO))
            textId = QString(ne_ne_getId(_gO).c_str());
        if (ne_ne_isSetName(_gO))
            textName = QString(ne_ne_getName(_gO).c_str());
        if (ne_go_isSetGlyphId(_gO))
            textLayoutId = QString(ne_go_getGlyphId(_gO).c_str());
        
        QLineEdit* plainTextLineEdit = new QLineEdit();
        if (textButton.text() != "N/A")
            plainTextLineEdit->setText(textButton.text());
        
        QPushButton* importTextButton = new QPushButton("Import", modificationBox);
        
        QComboBox* textSourceComboBox = new QComboBox(modificationBox);
        QStringList textSourceItems;
        if (ne_ne_isSetName(_gO))
            textSourceItems << "Name";
        textSourceItems << "Id";
        textSourceItems << "Layout Id";
        textSourceComboBox->addItems(textSourceItems);
        
        modificationBoxLayout.addWidget(new QLabel("Text:"), modificationBoxLayout.rowCount(), 0);
        modificationBoxLayout.addWidget(plainTextLineEdit, modificationBoxLayout.rowCount() - 1, 1);
        modificationBoxLayout.addWidget(importTextButton, modificationBoxLayout.rowCount() - 1, 2);
        modificationBoxLayout.addWidget(new QLabel("from:"), modificationBoxLayout.rowCount() - 1, 3);
        modificationBoxLayout.addWidget(textSourceComboBox, modificationBoxLayout.rowCount() - 1, 4);
        
        QDialogButtonBox modificationBoxButtons(Qt::Horizontal, modificationBox);
        modificationBoxButtons.addButton(QDialogButtonBox::Cancel);
        modificationBoxButtons.addButton(QString("Apply"), QDialogButtonBox::AcceptRole);
        modificationBoxLayout.addWidget(&modificationBoxButtons, modificationBoxLayout.rowCount(), 1, 1, 4);
        QObject::connect(&modificationBoxButtons, SIGNAL(accepted()), modificationBox, SLOT(accept()));
        QObject::connect(&modificationBoxButtons, SIGNAL(rejected()), modificationBox, SLOT(reject()));
        
        QObject::connect(importTextButton, &QPushButton::clicked, modificationBox, [modificationBox, plainTextLineEdit, textSourceComboBox, &textSourceItems, &textId, &textName, &textLayoutId] () {
            if (textSourceItems.length() == 3 && textSourceComboBox->currentIndex() == 0 && !textName.isEmpty())
                plainTextLineEdit->setText(textName);
            else if (((textSourceItems.length() == 3 && textSourceComboBox->currentIndex() == 1) || (textSourceItems.length() == 2 && textSourceComboBox->currentIndex() == 0))  && (!textId.isEmpty()))
                plainTextLineEdit->setText(textId);
            else if (((textSourceItems.length() == 3 && textSourceComboBox->currentIndex() == 2) || (textSourceItems.length() == 2 && textSourceComboBox->currentIndex() == 1))  && !textLayoutId.isEmpty())
                plainTextLineEdit->setText(textLayoutId);
        });
        
        if (modificationBox->exec()) {
            NText* text = ne_net_getText(_mw->getSBMLDocument()->getNetwork(), _gO);
            
            if (text) {
                if (!plainTextLineEdit->text().isEmpty())
                    ne_gtxt_setPlainText(text, plainTextLineEdit->text().toStdString());
                else
                    ne_gtxt_unSetPlainText(text);
                    
                _gText->setText(text);
                _mw->getSBMLDocument()->setLayoutModified(true);
                updateValues();
            }
        }
    }
}

// Species Feature Menu

SpeciesFeatureMenu::SpeciesFeatureMenu(QWidget* parent, MainWindow* mw) : FeatureMenuElement(parent, mw) {
    itemLabel.setText("Species");
    
    /// model features
    // id
    modelContentLayout.addWidget(&modelIdTitleLabel, modelContentLayout.rowCount() - 1, 0);
    modelContentLayout.addWidget(&modelIdLabel, modelContentLayout.rowCount() - 1, 1);
    
    // comparment
    compartmentTitleLabel.setText("Compartment: ");
    compartmentTitleLabel.setFont(featureTitleFont);
    modelContentLayout.addWidget(&compartmentTitleLabel, modelContentLayout.rowCount(), 0);
    modelContentLayout.addWidget(&compartmentLabel, modelContentLayout.rowCount() - 1, 1);
    
    // add model features to menu
    modelMenu = new CollapsibleMenu(this, "Model Features", 300);
    modelMenu->setContentLayout(modelContentLayout);
    qLayout.addWidget(modelMenu, qLayout.rowCount(), 0);
    
    /// layout features
    // id
    layoutContentLayout.addWidget(&layoutIdLabel, layoutContentLayout.rowCount() - 1, 0);
    layoutContentLayout.addWidget(&layoutIdButton, layoutContentLayout.rowCount() - 1, 1, 1, 2);
    
    // bounding box
    layoutContentLayout.addWidget(&boundingBoxTitleLabel, layoutContentLayout.rowCount(), 0, 1, 3, Qt::AlignCenter);
    layoutContentLayout.addWidget(new SeparatingLine(this), layoutContentLayout.rowCount(), 0, 1, 3);
    layoutContentLayout.addWidget(&boundingBoxLabel, layoutContentLayout.rowCount(), 0, 1, 3, Qt::AlignLeft);
    layoutContentLayout.addWidget(&boundingBoxButton, layoutContentLayout.rowCount() - 1, 0, 1, 3, Qt::AlignRight);
    bBoxFMenu = new BoundingBoxFeatureMenu(this, _mw);
    layoutContentLayout.addWidget(bBoxFMenu, layoutContentLayout.rowCount(), 0, 1, 3);
    connect(bBoxFMenu, SIGNAL(bBoxChanged()), SLOT(updateValues()));
    connect(bBoxFMenu, SIGNAL(bBoxDimensionsChanged(QString, int, QString)), SLOT(updateGeometricShape(QString, int, QString)));
    
    // add layout features to menu
    layoutMenu = new CollapsibleMenu(this, "Layout Features", 300);
    layoutMenu->setContentLayout(layoutContentLayout);
    qLayout.addWidget(layoutMenu, qLayout.rowCount(), 0);
    
    /// render features
    // style
    renderContentLayout.addWidget(&styleLabel, renderContentLayout.rowCount() - 1, 0, 1, 2, Qt::AlignLeft);
    renderContentLayout.addWidget(&styleButton, renderContentLayout.rowCount() - 1, 0, 1, 2, Qt::AlignRight);
    
    // stroke
    renderContentLayout.addWidget(&strokeTitleLabel, renderContentLayout.rowCount(), 0, 1, 2, Qt::AlignCenter);
    renderContentLayout.addWidget(new SeparatingLine(this), renderContentLayout.rowCount(), 0, 1, 2);
    strokeFMenu = new StrokeFeatureMenu(this, _mw);
    renderContentLayout.addWidget(strokeFMenu, renderContentLayout.rowCount(), 0, 1, 2);
    connect(strokeFMenu, SIGNAL(styleChanged(VLocalStyle*)), SLOT(updateStyle(VLocalStyle*)));
    
    // fill
    renderContentLayout.addWidget(&fillTitleLabel, renderContentLayout.rowCount(), 0, 1, 2, Qt::AlignCenter);
    renderContentLayout.addWidget(new SeparatingLine(this), renderContentLayout.rowCount(), 0, 1, 2);
    fillFMenu = new FillFeatureMenu(this, _mw);
    renderContentLayout.addWidget(fillFMenu, renderContentLayout.rowCount(), 0, 1, 2);
    connect(fillFMenu, SIGNAL(styleChanged(VLocalStyle*)), SLOT(updateStyle(VLocalStyle*)));
    
    // text
    renderContentLayout.addWidget(&textTitleLabel, renderContentLayout.rowCount(), 0, 1, 2, Qt::AlignCenter);
    renderContentLayout.addWidget(new SeparatingLine(this), renderContentLayout.rowCount(), 0, 1, 2);
    renderContentLayout.addWidget(&textLabel, renderContentLayout.rowCount(), 0);
    renderContentLayout.addWidget(&textButton, renderContentLayout.rowCount() - 1, 1);
    textFMenu = new TextFeatureMenu(this, _mw);
    renderContentLayout.addWidget(textFMenu, renderContentLayout.rowCount(), 0, 1, 2);
    connect(textFMenu, SIGNAL(textChanged()), SLOT(updateValues()));
    
    // geometric shape
    renderContentLayout.addWidget(&geometricShapeTitleLabel, renderContentLayout.rowCount(), 0, 1, 2, Qt::AlignCenter);
    renderContentLayout.addWidget(new SeparatingLine(this), renderContentLayout.rowCount(), 0, 1, 2);
    renderContentLayout.addWidget(&geometricShapeLabel, renderContentLayout.rowCount(), 0, 1, 2, Qt::AlignLeft);
    renderContentLayout.addWidget(&geometricShapeButton, renderContentLayout.rowCount() - 1, 0, 1, 2, Qt::AlignRight);
    
    // add render features to menu
    renderMenu = new CollapsibleMenu(this, "Render Features", 300);
    renderMenu->setContentLayout(renderContentLayout);
    qLayout.addWidget(renderMenu, qLayout.rowCount(), 0);
    
    // reset values;
    resetValues();
}

void SpeciesFeatureMenu::showInfo(GraphicalSpecies* gS) {
    // reset values
    resetValues();
    
    if (gS) {
        gSpecies = gS;

        if (gSpecies->isSetSpecies()) {
            _gO = gS->getSpecies();
            
            /// model features
            // set id
            if (ne_ne_isSetId(_gO))
                modelIdLabel.setText(ne_ne_getId(_gO).c_str());
            
            // set compartment
            if (ne_spc_isSetCompartment((NSpecies*)_gO))
                compartmentLabel.setText(ne_spc_getCompartment((NSpecies*)_gO).c_str());
            
            /// layout features
            // set layout id
            if (ne_go_isSetGlyphId(_gO))
                layoutIdButton.setText(ne_go_getGlyphId(_gO).c_str());
            
            // bounding box
            if (ne_go_isSetBoundingBox(_gO)) {
                bBoxFMenu->showInfo(_gO);
                boundingBoxButton.setText("Remove");
            }
            
            // text features
            if (gSpecies->isSetGText()) {
                textButton.setStyleSheet(pushButtonStyleSheet);
                textButton.blockSignals(false);
                _gText = gSpecies->getGText();
                textFMenu->showInfo(gSpecies->getGText(), _gO);
                
                if (gSpecies->getGText()->isSetPlainText())
                    textButton.setText(QString(gSpecies->getGText()->getPlainText().c_str()));
            }
            
            /// render features
            // species
            if (gSpecies->isSetStyle()) {
                _style = gSpecies->getStyle();
                styleButton.setText("Remove");
                
                // stroke features
                strokeFMenu->showInfo(_gO, _style);
                
                if (ne_go_isSetBoundingBox(_gO)) {
                    // fill features
                    fillFMenu->showInfo(_gO, _style);
                    
                    // geometric shape features
                    geometricShapeButton.setStyleSheet(pushButtonStyleSheet);
                    geometricShapeButton.blockSignals(false);
                    
                    if (ne_grp_getNumGeometricShapes(ne_stl_getGroup(_style))) {
                        // stroke features
                        strokeFMenu->showInfo(ne_grp_getGeometricShape(ne_stl_getGroup(_style), 0));
                        
                        // fill features
                        fillFMenu->showInfo(ne_grp_getGeometricShape(ne_stl_getGroup(_style), 0));
                        
                        // set geometric shape feature menu
                        switch (ne_gs_getShape(ne_grp_getGeometricShape(ne_stl_getGroup(_style), 0))) {
                            case 0:
                                strokeFMenu->resetValues();
                                fillFMenu->resetValues();
                                gSFMenuElement = new ImageGeometricShapeFeatureMenu(this, _mw);
                                geometricShapeButton.setText("Image");
                             ((ImageGeometricShapeFeatureMenu*)(gSFMenuElement))->showInfo(_gO, _style);
                                break;
                                
                            case 3:
                                gSFMenuElement = new RectangleGeometricShapeFeatureMenu(this, _mw);
                                geometricShapeButton.setText("Rectangle");
                                ((RectangleGeometricShapeFeatureMenu*)(gSFMenuElement))->showInfo(_gO, _style);
                               break;
                                
                            case 4:
                                gSFMenuElement = new EllipseGeometricShapeFeatureMenu(this, _mw);
                                geometricShapeButton.setText("Ellipse");
                                ((EllipseGeometricShapeFeatureMenu*)(gSFMenuElement))->showInfo(_gO, _style);
                                break;
                                
                            case 5:
                                gSFMenuElement = new PolygonGeometricShapeFeatureMenu(this, _mw);
                                geometricShapeButton.setText("Polygon");
                                ((PolygonGeometricShapeFeatureMenu*)(gSFMenuElement))->showInfo(_gO, _style);
                                break;
                                
                            default:
                                break;
                        }
                        
                        if (gSFMenuElement) {
                            renderContentLayout.addWidget(gSFMenuElement, renderContentLayout.rowCount(), 0, 1, 2);
                            connect(gSFMenuElement, SIGNAL(styleChanged(VLocalStyle*)), SLOT(updateStyle(VLocalStyle*)));
                        }
                    }
                }
            }
        }
    }
    
    show();
}

void SpeciesFeatureMenu::resetValues() {
    modelIdLabel.setText("N/A");
    compartmentLabel.setText("N/A");
    layoutIdButton.setText("N/A");
    boundingBoxButton.setText("Add");
    bBoxFMenu->resetValues();
    styleButton.setText("Add");
    strokeFMenu->resetValues();
    fillFMenu->resetValues();
    textFMenu->resetValues();
    textButton.setText("N/A");
    textButton.blockSignals(true);
    textButton.setStyleSheet(disabledPushButtonStyleSheet);
    if (gSFMenuElement) {
        renderContentLayout.removeWidget(gSFMenuElement);
        delete gSFMenuElement;
        gSFMenuElement = NULL;
    }
    geometricShapeButton.setText("N/A");
    geometricShapeButton.blockSignals(true);
    geometricShapeButton.setStyleSheet(disabledPushButtonStyleSheet);
    
    gSpecies = NULL;
    _gO = NULL;
    _gText = NULL;
    _style = NULL;
}

void SpeciesFeatureMenu::updateValues() {
    if (gSpecies) {
        gSpecies->updateValues(_mw, true);
        showInfo(gSpecies);
    }
}

void SpeciesFeatureMenu::updateStyle(VLocalStyle* style) {
    if (gSpecies && style) {
        gSpecies->setStyle(style);
        updateValues();
    }
}

void SpeciesFeatureMenu::setStyle() {
    if (gSpecies) {
        gSpecies->setStyle(_mw, true);
        updateValues();
    }
}

void SpeciesFeatureMenu::unSetStyle() {
    if (gSpecies) {
        gSpecies->unSetStyle();
        updateValues();
    }
}

// Compartment Feature Menu

CompartmentFeatureMenu::CompartmentFeatureMenu(QWidget* parent, MainWindow* mw) : FeatureMenuElement(parent, mw) {
    itemLabel.setText("Compartment");
    
    /// model features
    // id
    modelContentLayout.addWidget(&modelIdTitleLabel, modelContentLayout.rowCount() - 1, 0);
    modelContentLayout.addWidget(&modelIdLabel, modelContentLayout.rowCount() - 1, 1);
    
    // add model features to menu
    modelMenu = new CollapsibleMenu(this, "Model Features", 300);
    modelMenu->setContentLayout(modelContentLayout);
    qLayout.addWidget(modelMenu, qLayout.rowCount(), 0);
    
    /// layout features
    // id
    layoutContentLayout.addWidget(&layoutIdLabel, layoutContentLayout.rowCount() - 1, 0);
    layoutContentLayout.addWidget(&layoutIdButton, layoutContentLayout.rowCount() - 1, 1, 1, 2);
    
    // bounding box
    layoutContentLayout.addWidget(&boundingBoxTitleLabel, layoutContentLayout.rowCount(), 0, 1, 3, Qt::AlignCenter);
    layoutContentLayout.addWidget(new SeparatingLine(this), layoutContentLayout.rowCount(), 0, 1, 3);
    layoutContentLayout.addWidget(&boundingBoxLabel, layoutContentLayout.rowCount(), 0, 1, 3, Qt::AlignLeft);
    layoutContentLayout.addWidget(&boundingBoxButton, layoutContentLayout.rowCount() - 1, 0, 1, 3, Qt::AlignRight);
    
    bBoxFMenu = new BoundingBoxFeatureMenu(this, _mw);
    layoutContentLayout.addWidget(bBoxFMenu, layoutContentLayout.rowCount(), 0, 1, 3);
    connect(bBoxFMenu, SIGNAL(bBoxChanged()), SLOT(updateValues()));
    connect(bBoxFMenu, SIGNAL(bBoxDimensionsChanged(QString, int, QString)), SLOT(updateGeometricShape(QString, int, QString)));
    
    // add layout features to menu
    layoutMenu = new CollapsibleMenu(this, "Layout Features", 300);
    layoutMenu->setContentLayout(layoutContentLayout);
    qLayout.addWidget(layoutMenu, qLayout.rowCount(), 0);
    
    /// render features
    // style
    renderContentLayout.addWidget(&styleLabel, renderContentLayout.rowCount() - 1, 0, 1, 2, Qt::AlignLeft);
    renderContentLayout.addWidget(&styleButton, renderContentLayout.rowCount() - 1, 0, 1, 2, Qt::AlignRight);
    
    // stroke
    renderContentLayout.addWidget(&strokeTitleLabel, renderContentLayout.rowCount(), 0, 1, 2, Qt::AlignCenter);
    renderContentLayout.addWidget(new SeparatingLine(this), renderContentLayout.rowCount(), 0, 1, 2);
    strokeFMenu = new StrokeFeatureMenu(this, _mw);
    renderContentLayout.addWidget(strokeFMenu, renderContentLayout.rowCount(), 0, 1, 2);
    connect(strokeFMenu, SIGNAL(styleChanged(VLocalStyle*)), SLOT(updateStyle(VLocalStyle*)));
    
    // fill
    renderContentLayout.addWidget(&fillTitleLabel, renderContentLayout.rowCount(), 0, 1, 2, Qt::AlignCenter);
    renderContentLayout.addWidget(new SeparatingLine(this), renderContentLayout.rowCount(), 0, 1, 2);
    fillFMenu = new FillFeatureMenu(this, _mw);
    renderContentLayout.addWidget(fillFMenu, renderContentLayout.rowCount(), 0, 1, 2);
    connect(fillFMenu, SIGNAL(styleChanged(VLocalStyle*)), SLOT(updateStyle(VLocalStyle*)));
    
    // text
    renderContentLayout.addWidget(&textTitleLabel, renderContentLayout.rowCount(), 0, 1, 2, Qt::AlignCenter);
    renderContentLayout.addWidget(new SeparatingLine(this), renderContentLayout.rowCount(), 0, 1, 2);
    renderContentLayout.addWidget(&textLabel, renderContentLayout.rowCount(), 0);
    renderContentLayout.addWidget(&textButton, renderContentLayout.rowCount() - 1, 1);
    textFMenu = new TextFeatureMenu(this, _mw);
    renderContentLayout.addWidget(textFMenu, renderContentLayout.rowCount(), 0, 1, 2);
    connect(textFMenu, SIGNAL(textChanged()), SLOT(updateValues()));
    
    // geometric shape
    renderContentLayout.addWidget(&geometricShapeTitleLabel, renderContentLayout.rowCount(), 0, 1, 2, Qt::AlignCenter);
    renderContentLayout.addWidget(new SeparatingLine(this), renderContentLayout.rowCount(), 0, 1, 2);
    renderContentLayout.addWidget(&geometricShapeLabel, renderContentLayout.rowCount(), 0, 1, 2, Qt::AlignLeft);
    renderContentLayout.addWidget(&geometricShapeButton, renderContentLayout.rowCount() - 1, 0, 1, 2, Qt::AlignRight);
    
    // add render features to menu
    renderMenu = new CollapsibleMenu(this, "Render Features", 300);
    renderMenu->setContentLayout(renderContentLayout);
    qLayout.addWidget(renderMenu, qLayout.rowCount(), 0);
    
    // reset values();
    resetValues();
}

void CompartmentFeatureMenu::showInfo(GraphicalCompartment* gC) {
    // reset values
    resetValues();
    
    if (gC) {
        gCompartment = gC;
        
        if (gCompartment->isSetCompartment()) {
            _gO = gCompartment->getCompartment();
            
            /// model features
            // set id
            if (ne_ne_isSetId(_gO))
                modelIdLabel.setText(ne_ne_getId(_gO).c_str());
            
            /// layout features
            // set layout id
            if (ne_go_isSetGlyphId(_gO))
                layoutIdButton.setText(ne_go_getGlyphId(_gO).c_str());
            
            // bounding box
            if (ne_go_isSetBoundingBox(_gO)) {
                bBoxFMenu->showInfo(_gO);
                boundingBoxButton.setText("Remove");
            }
            
            // text features
            if (ne_go_isSetText(_gO)) {
                textButton.setStyleSheet(pushButtonStyleSheet);
                textButton.blockSignals(false);
                _gText = gCompartment->getGText();
                textFMenu->showInfo(gCompartment->getGText(), _gO);
                
                if (gCompartment->getGText()->isSetPlainText())
                    textButton.setText(QString(gCompartment->getGText()->getPlainText().c_str()));
            }
            
            /// render features
            // compartment
            if (gCompartment->isSetStyle()) {
                _style = gCompartment->getStyle();
                styleButton.setText("Remove");
                
                // stroke features
                strokeFMenu->showInfo(_gO, _style);
                
                if (ne_go_isSetBoundingBox(_gO)) {
                    // fill features
                    fillFMenu->showInfo(_gO, _style);
                    
                    // geometric shape features
                    geometricShapeButton.setStyleSheet(pushButtonStyleSheet);
                    geometricShapeButton.blockSignals(false);
                    
                    if (ne_grp_getNumGeometricShapes(ne_stl_getGroup(_style))) {
                        // stroke features
                        strokeFMenu->showInfo(ne_grp_getGeometricShape(ne_stl_getGroup(_style), 0));
                        
                        // fill features
                        fillFMenu->showInfo(ne_grp_getGeometricShape(ne_stl_getGroup(_style), 0));
                        
                        // set geometric shape feautre menu
                        switch (ne_gs_getShape(ne_grp_getGeometricShape(ne_stl_getGroup(_style), 0))) {
                            case 0:
                                strokeFMenu->resetValues();
                                fillFMenu->resetValues();
                                gSFMenuElement = new ImageGeometricShapeFeatureMenu(this, _mw);
                                geometricShapeButton.setText("Image");
                             ((ImageGeometricShapeFeatureMenu*)(gSFMenuElement))->showInfo(_gO, _style);
                                break;
                                
                            case 3:
                                gSFMenuElement = new RectangleGeometricShapeFeatureMenu(this, _mw);
                                geometricShapeButton.setText("Rectangle");
                                ((RectangleGeometricShapeFeatureMenu*)(gSFMenuElement))->showInfo(_gO, _style);
                               break;
                                
                            case 4:
                                gSFMenuElement = new EllipseGeometricShapeFeatureMenu(this, _mw);
                                geometricShapeButton.setText("Ellipse");
                                ((EllipseGeometricShapeFeatureMenu*)(gSFMenuElement))->showInfo(_gO, _style);
                                break;
                                
                            case 5:
                                gSFMenuElement = new PolygonGeometricShapeFeatureMenu(this, _mw);
                                geometricShapeButton.setText("Polygon");
                                ((PolygonGeometricShapeFeatureMenu*)(gSFMenuElement))->showInfo(_gO, _style);
                                break;
                                
                            default:
                                break;
                        }
                        
                        if (gSFMenuElement) {
                            renderContentLayout.addWidget(gSFMenuElement, renderContentLayout.rowCount(), 0, 1, 2);
                            connect(gSFMenuElement, SIGNAL(styleChanged(VLocalStyle*)), SLOT(updateStyle(VLocalStyle*)));
                        }
                    }
                }
            }
        }
    }
    
    show();
}

void CompartmentFeatureMenu::resetValues() {
    modelIdLabel.setText("N/A");
    layoutIdButton.setText("N/A");
    boundingBoxButton.setText("Add");
    bBoxFMenu->resetValues();
    styleButton.setText("Add");
    strokeFMenu->resetValues();
    fillFMenu->resetValues();
    textFMenu->resetValues();
    textButton.setText("N/A");
    textButton.blockSignals(true);
    textButton.setStyleSheet(disabledPushButtonStyleSheet);
    if (gSFMenuElement) {
        renderContentLayout.removeWidget(gSFMenuElement);
        delete gSFMenuElement;
        gSFMenuElement = NULL;
    }
    geometricShapeButton.setText("N/A");
    geometricShapeButton.blockSignals(true);
    geometricShapeButton.setStyleSheet(disabledPushButtonStyleSheet);
    
    gCompartment = NULL;
    _gO = NULL;
    _gText = NULL;
    _style = NULL;
}

void CompartmentFeatureMenu::updateValues() {
    if (gCompartment) {
        gCompartment->updateValues(_mw);
        showInfo(gCompartment);
    }
}

void CompartmentFeatureMenu::updateStyle(VLocalStyle* style) {
    if (gCompartment && style) {
        gCompartment->setStyle(style);
        updateValues();
    }
}

void CompartmentFeatureMenu::setStyle() {
    if (gCompartment) {
        gCompartment->setStyle(_mw, true);
        updateValues();
    }
}

void CompartmentFeatureMenu::unSetStyle() {
    if (gCompartment) {
        gCompartment->unSetStyle();
        updateValues();
    }
}

// Reaction Feature Menu

ReactionFeatureMenu::ReactionFeatureMenu(QWidget* parent, MainWindow* mw) : FeatureMenuElement(parent, mw) {
    itemLabel.setText("Reaction");
    
    /// model features
    // id
    modelContentLayout.addWidget(&modelIdTitleLabel, modelContentLayout.rowCount() - 1, 0);
    modelContentLayout.addWidget(&modelIdLabel, modelContentLayout.rowCount() - 1, 1);
    
    // add model features to menu
    modelMenu = new CollapsibleMenu(this, "Model Features", 300);
    modelMenu->setContentLayout(modelContentLayout);
    qLayout.addWidget(modelMenu, qLayout.rowCount(), 0);
    
    /// layout features
    // id
    layoutContentLayout.addWidget(&layoutIdLabel, layoutContentLayout.rowCount() - 1, 0);
    layoutContentLayout.addWidget(&layoutIdButton, layoutContentLayout.rowCount() - 1, 1, 1, 2);
    
    // curve
    layoutContentLayout.addWidget(&curveTitleLabel, layoutContentLayout.rowCount(), 0, 1, 3, Qt::AlignCenter);
    layoutContentLayout.addWidget(new SeparatingLine(this), layoutContentLayout.rowCount(), 0, 1, 3);
    layoutContentLayout.addWidget(&curveLabel, layoutContentLayout.rowCount(), 0, 1, 3, Qt::AlignLeft);
    layoutContentLayout.addWidget(&curveButton, layoutContentLayout.rowCount() - 1, 0, 1, 3, Qt::AlignRight);
    curveFMenu = new CurveFeatureMenu(this, _mw);
    layoutContentLayout.addWidget(curveFMenu, layoutContentLayout.rowCount(), 0, 1, 3);
    connect(curveFMenu, SIGNAL(curveChanged()), SLOT(updateValues()));
    
    // bounding box
    layoutContentLayout.addWidget(&boundingBoxTitleLabel, layoutContentLayout.rowCount(), 0, 1, 3, Qt::AlignCenter);
    layoutContentLayout.addWidget(new SeparatingLine(this), layoutContentLayout.rowCount(), 0, 1, 3);
    layoutContentLayout.addWidget(&boundingBoxLabel, layoutContentLayout.rowCount(), 0, 1, 3, Qt::AlignLeft);
    layoutContentLayout.addWidget(&boundingBoxButton, layoutContentLayout.rowCount() - 1, 0, 1, 3, Qt::AlignRight);
    bBoxFMenu = new BoundingBoxFeatureMenu(this, _mw);
    layoutContentLayout.addWidget(bBoxFMenu, layoutContentLayout.rowCount(), 0, 1, 3);
    connect(bBoxFMenu, SIGNAL(bBoxChanged()), SLOT(updateValues()));
    connect(bBoxFMenu, SIGNAL(bBoxDimensionsChanged(QString, int, QString)), SLOT(updateGeometricShape(QString, int, QString)));
    
    // add layout features to menu
    layoutMenu = new CollapsibleMenu(this, "Layout Features", 300);
    layoutMenu->setContentLayout(layoutContentLayout);
    qLayout.addWidget(layoutMenu, qLayout.rowCount(), 0);
    
    /// render features
    // style
    renderContentLayout.addWidget(&styleLabel, renderContentLayout.rowCount() - 1, 0, 1, 2, Qt::AlignLeft);
    renderContentLayout.addWidget(&styleButton, renderContentLayout.rowCount() - 1, 0, 1, 2, Qt::AlignRight);
    
    // stroke
    renderContentLayout.addWidget(&strokeTitleLabel, renderContentLayout.rowCount(), 0, 1, 2, Qt::AlignCenter);
    renderContentLayout.addWidget(new SeparatingLine(this), renderContentLayout.rowCount(), 0, 1, 2);
    strokeFMenu = new StrokeFeatureMenu(this, _mw);
    renderContentLayout.addWidget(strokeFMenu, renderContentLayout.rowCount(), 0, 1, 2);
    connect(strokeFMenu, SIGNAL(styleChanged(VLocalStyle*)), SLOT(updateStyle(VLocalStyle*)));
    
    // fill
    renderContentLayout.addWidget(&fillTitleLabel, renderContentLayout.rowCount(), 0, 1, 2, Qt::AlignCenter);
    renderContentLayout.addWidget(new SeparatingLine(this), renderContentLayout.rowCount(), 0, 1, 2);
    fillFMenu = new FillFeatureMenu(this, _mw);
    renderContentLayout.addWidget(fillFMenu, renderContentLayout.rowCount(), 0, 1, 2);
    connect(fillFMenu, SIGNAL(styleChanged(VLocalStyle*)), SLOT(updateStyle(VLocalStyle*)));

    // geometric shape
    renderContentLayout.addWidget(&geometricShapeTitleLabel, renderContentLayout.rowCount(), 0, 1, 2, Qt::AlignCenter);
    renderContentLayout.addWidget(new SeparatingLine(this), renderContentLayout.rowCount(), 0, 1, 2);
    renderContentLayout.addWidget(&geometricShapeLabel, renderContentLayout.rowCount(), 0, 1, 2, Qt::AlignLeft);
    renderContentLayout.addWidget(&geometricShapeButton, renderContentLayout.rowCount() - 1, 0, 1, 2, Qt::AlignRight);
    
    // add render features to menu
    renderMenu = new CollapsibleMenu(this, "Render Features", 300);
    renderMenu->setContentLayout(renderContentLayout);
    qLayout.addWidget(renderMenu, qLayout.rowCount(), 0);
    
    // reset values();
    resetValues();
}

void ReactionFeatureMenu::showInfo(GraphicalReaction* gR) {
    // reset values
    resetValues();
    
    if (gR) {
        gReaction = gR;
        
        if (gReaction->isSetReaction()) {
            _gO = gReaction->getReaction();
            
            /// model features
            // set id
            if (ne_ne_isSetId(_gO))
                modelIdLabel.setText(ne_ne_getId(_gO).c_str());
            
            /// layout features
            // set layout id
            if (ne_go_isSetGlyphId(_gO))
                layoutIdButton.setText(ne_go_getGlyphId(_gO).c_str());
            
            // set curve
            if (ne_rxn_isSetCurve((NReaction*)_gO)) {
                curveFMenu->showInfo(ne_rxn_getCurve((NReaction*)_gO), false);
                curveButton.setText("Remove");
            }
            
            // set bounding box
            if (ne_go_isSetBoundingBox(_gO)) {
                bBoxFMenu->showInfo(_gO);
                boundingBoxButton.setText("Remove");
            }
            
            /// render features
            // reaction
            if (gReaction->isSetStyle()) {
                _style = gReaction->getStyle();
                styleButton.setText("Remove");
                
                // stroke features
                strokeFMenu->showInfo(_gO, _style);
                
                if (ne_go_isSetBoundingBox(_gO)) {
                    // fill features
                    fillFMenu->showInfo(_gO, _style);
                    
                    // geometric shape features
                    geometricShapeButton.setStyleSheet(pushButtonStyleSheet);
                    geometricShapeButton.blockSignals(false);
                    
                    if (ne_grp_getNumGeometricShapes(ne_stl_getGroup(_style))) {
                        // stroke features
                        strokeFMenu->showInfo(ne_grp_getGeometricShape(ne_stl_getGroup(_style), 0));
                        
                        // fill features
                        fillFMenu->showInfo(ne_grp_getGeometricShape(ne_stl_getGroup(_style), 0));
                        
                        // set geomtric shape feature menu
                        switch (ne_gs_getShape((ne_grp_getGeometricShape(ne_stl_getGroup(_style), 0)))) {
                            case 0:
                                strokeFMenu->resetValues();
                                fillFMenu->resetValues();
                                gSFMenuElement = new ImageGeometricShapeFeatureMenu(this, _mw);
                                geometricShapeButton.setText("Image");
                                ((ImageGeometricShapeFeatureMenu*)(gSFMenuElement))->showInfo(_gO, _style);
                                break;
                                
                            case 1:
                                fillFMenu->resetValues();
                                gSFMenuElement = new RenderCurveGeometricShapeFeatureMenu(this, _mw);
                                geometricShapeButton.setText("RenderCurve");
                                ((RenderCurveGeometricShapeFeatureMenu*)(gSFMenuElement))->showInfo(_gO, _style);
                               break;
                                
                            case 3:
                                gSFMenuElement = new RectangleGeometricShapeFeatureMenu(this, _mw);
                                geometricShapeButton.setText("Rectangle");
                                ((RectangleGeometricShapeFeatureMenu*)(gSFMenuElement))->showInfo(_gO, _style);
                               break;
                                
                            case 4:
                                gSFMenuElement = new EllipseGeometricShapeFeatureMenu(this, _mw);
                                geometricShapeButton.setText("Ellipse");
                                ((EllipseGeometricShapeFeatureMenu*)(gSFMenuElement))->showInfo(_gO, _style);
                                break;
                                
                            case 5:
                                gSFMenuElement = new PolygonGeometricShapeFeatureMenu(this, _mw);
                                geometricShapeButton.setText("Polygon");
                                ((PolygonGeometricShapeFeatureMenu*)(gSFMenuElement))->showInfo(_gO, _style);
                                break;
                                
                            default:
                                break;
                        }
                        
                        if (gSFMenuElement) {
                            renderContentLayout.addWidget(gSFMenuElement, renderContentLayout.rowCount(), 0, 1, 2);
                            connect(gSFMenuElement, SIGNAL(styleChanged(VLocalStyle*)), SLOT(updateStyle(VLocalStyle*)));
                        }
                    }
                }
            }
        }
    }
    
    show();
}

void ReactionFeatureMenu::resetValues() {
    modelIdLabel.setText("N/A");
    layoutIdButton.setText("N/A");
    curveButton.setText("Add");
    curveFMenu->resetValues();
    boundingBoxButton.setText("Add");
    bBoxFMenu->resetValues();
    styleButton.setText("Add");
    strokeFMenu->resetValues();
    fillFMenu->resetValues();
    if (gSFMenuElement) {
        renderContentLayout.removeWidget(gSFMenuElement);
        delete gSFMenuElement;
        gSFMenuElement = NULL;
    }
    geometricShapeButton.setText("N/A");
    geometricShapeButton.blockSignals(true);
    geometricShapeButton.setStyleSheet(disabledPushButtonStyleSheet);
    
    gReaction = NULL;
    _gO = NULL;
    _style = NULL;
}

void ReactionFeatureMenu::updateValues() {
    if (gReaction) {
        gReaction->updateValues(_mw);
        showInfo(gReaction);
    }
}

void ReactionFeatureMenu::updateStyle(VLocalStyle* style) {
    if (gReaction && style) {
        gReaction->setStyle(style);
        updateValues();
    }
}

void ReactionFeatureMenu::setStyle() {
    if (gReaction) {
        gReaction->setStyle(_mw, true);
        updateValues();
    }
}

void ReactionFeatureMenu::unSetStyle() {
    if (gReaction) {
        gReaction->unSetStyle();
        updateValues();
    }
}

void ReactionFeatureMenu::addOrRemoveCurve() {
    if (curveFMenu) {
        if (curveButton.text() == "Remove") {
            QMessageBox* removeMessageBox =  new QMessageBox();
            removeMessageBox->setWindowTitle("Remove Curve");
            removeMessageBox->setInformativeText("Are you sure you want to remove the Curve of the Reaction?");
            removeMessageBox->setText("Curve                                                                                                 ");
            removeMessageBox->setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            removeMessageBox->setDefaultButton(QMessageBox::No);
            
            if (removeMessageBox->exec() ==  QMessageBox::Yes && gReaction && ne_rxn_isSetCurve((NReaction*)_gO)) {
                ne_rxn_unSetCurve((NReaction*)_gO, false);
                _mw->getSBMLDocument()->setLayoutModified(true);
                updateValues();
            }
        }
        else if (curveButton.text() == "Add") {
            LCurve* curve = NULL;
            if (ne_rxn_getCurve((NReaction*)_gO)) {
                QMessageBox* addPreviousCurveMessageBox =  new QMessageBox();
                addPreviousCurveMessageBox->setWindowTitle("Use Previous Curve");
                addPreviousCurveMessageBox->setText("This reaction used to have a curve");
                addPreviousCurveMessageBox->setInformativeText("Do you want to set the previous curve with the reaction");
                addPreviousCurveMessageBox->setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                addPreviousCurveMessageBox->setDefaultButton(QMessageBox::No);
                if (addPreviousCurveMessageBox->exec() ==  QMessageBox::Yes)
                    curve = ne_rxn_getCurve((NReaction*)_gO);
            }
            
            if (!curve) {
                curve = new LCurve();
                curveFMenu->showInfo(curve, true);
            }
            
            if (!ne_rxn_setCurve((NReaction*)_gO, curve)) {
                _mw->getSBMLDocument()->setLayoutModified(true);
                updateValues();
            }
        }
    }
}

// Species Reference Feature Menu

SpeciesReferenceFeatureMenu::SpeciesReferenceFeatureMenu(QWidget* parent, MainWindow* mw) : FeatureMenuElement(parent, mw) {
    itemLabel.setText("Species Reference");
    
    /// model features
    // id
    modelContentLayout.addWidget(&modelIdTitleLabel, modelContentLayout.rowCount() - 1, 0);
    modelContentLayout.addWidget(&modelIdLabel, modelContentLayout.rowCount() - 1, 1);
    
    // reaction
    modelReactionTitleLabel.setText("Reaction: ");
    modelReactionTitleLabel.setFont(featureTitleFont);
    modelContentLayout.addWidget(&modelReactionTitleLabel, modelContentLayout.rowCount(), 0);
    modelReactionLabel.setStyleSheet(pushButtonStyleSheet);
    modelContentLayout.addWidget(&modelReactionLabel, modelContentLayout.rowCount() -1, 1);
    
    // species
    modelSpeciesTitleLabel.setText("Species: ");
    modelSpeciesTitleLabel.setFont(featureTitleFont);
    modelContentLayout.addWidget(&modelSpeciesTitleLabel, modelContentLayout.rowCount(), 0);
    modelSpeciesLabel.setStyleSheet(pushButtonStyleSheet);
    modelContentLayout.addWidget(&modelSpeciesLabel, modelContentLayout.rowCount() - 1, 1);
    
    // role
    modelRoleTitleLabel.setText("Role: ");
    modelRoleTitleLabel.setFont(featureTitleFont);
    modelContentLayout.addWidget(&modelRoleTitleLabel, modelContentLayout.rowCount(), 0);
    modelRoleLabel.setStyleSheet(pushButtonStyleSheet);
    modelContentLayout.addWidget(&modelRoleLabel, modelContentLayout.rowCount() - 1, 1);
    
    // add model features to menu
    modelMenu = new CollapsibleMenu(this, "Model Features", 300);
    modelMenu->setContentLayout(modelContentLayout);
    qLayout.addWidget(modelMenu, qLayout.rowCount(), 0);
    
    /// layout features
    // id
    layoutContentLayout.addWidget(&layoutIdLabel, layoutContentLayout.rowCount() - 1, 0);
    layoutContentLayout.addWidget(&layoutIdButton, layoutContentLayout.rowCount() - 1, 1, 1, 2);
    
    // curve
    layoutContentLayout.addWidget(&curveTitleLabel, layoutContentLayout.rowCount(), 0, 1, 3, Qt::AlignCenter);
    layoutContentLayout.addWidget(new SeparatingLine(this), layoutContentLayout.rowCount(), 0, 1, 3);
    layoutContentLayout.addWidget(&curveLabel, layoutContentLayout.rowCount(), 0, 1, 3, Qt::AlignLeft);
    layoutContentLayout.addWidget(&curveButton, layoutContentLayout.rowCount() - 1, 0, 1, 3, Qt::AlignRight);
    curveFMenu = new CurveFeatureMenu(this, _mw);
    layoutContentLayout.addWidget(curveFMenu, layoutContentLayout.rowCount(), 0, 1, 3);
    connect(curveFMenu, SIGNAL(curveChanged()), SLOT(updateValues()));
    
    // add layout features to menu
    layoutMenu = new CollapsibleMenu(this, "Layout Features", 300);
    layoutMenu->setContentLayout(layoutContentLayout);
    qLayout.addWidget(layoutMenu, qLayout.rowCount(), 0);
    
    /// render features
    // style
    renderContentLayout.addWidget(&styleLabel, renderContentLayout.rowCount() - 1, 0, 1, 2, Qt::AlignLeft);
    renderContentLayout.addWidget(&styleButton, renderContentLayout.rowCount() - 1, 0, 1, 2, Qt::AlignRight);
    
    // stroke
    renderContentLayout.addWidget(&strokeTitleLabel, renderContentLayout.rowCount(), 0, 1, 2, Qt::AlignCenter);
    renderContentLayout.addWidget(new SeparatingLine(this), renderContentLayout.rowCount(), 0, 1, 2);
    strokeFMenu = new StrokeFeatureMenu(this, _mw);
    renderContentLayout.addWidget(strokeFMenu, renderContentLayout.rowCount(), 0, 1, 2);
    connect(strokeFMenu, SIGNAL(styleChanged(VLocalStyle*)), SLOT(updateStyle(VLocalStyle*)));
    
    // heads
    headTitleLabel.setFont(headlineFont);
    headTitleLabel.setText("Heads");
    renderContentLayout.addWidget(&headTitleLabel, renderContentLayout.rowCount(), 0, 1, 2, Qt::AlignCenter);
    renderContentLayout.addWidget(new SeparatingLine(this), renderContentLayout.rowCount(), 0, 1, 2);
    headFMenu = new HeadFeatureMenu(this, _mw);
    renderContentLayout.addWidget(headFMenu, renderContentLayout.rowCount(), 0, 1, 2);
    connect(headFMenu, SIGNAL(styleChanged(VLocalStyle*)), SLOT(updateStyle(VLocalStyle*)));
    connect(_mw, SIGNAL(numberOfGLEndingChanged()), headFMenu, SLOT(updateValues()));
    
    // add render features to menu
    renderMenu = new CollapsibleMenu(this, "Render Features", 300);
    renderMenu->setContentLayout(renderContentLayout);
    qLayout.addWidget(renderMenu, qLayout.rowCount(), 0);
    
    // reset values
    resetValues();
}

void SpeciesReferenceFeatureMenu::showInfo(GraphicalSReference* gSR) {
    // reset values
    resetValues();
    
    if (gSR) {
        gSReference = gSR;
        
        if (gSReference->isSetSReference()) {
            _gO = gSReference->getSReference();
            
            /// model features
            // set id
            if (ne_ne_isSetId(_gO))
                modelIdLabel.setText(ne_ne_getId(_gO).c_str());
            
            // set reaction
            if (ne_ne_isSetId(ne_sr_getReaction((NSpeciesReference*)_gO)))
                modelReactionLabel.setText(ne_ne_getId(ne_sr_getReaction((NSpeciesReference*)_gO)).c_str());
            
            // set species
            if (ne_ne_isSetId(ne_sr_getSpecies((NSpeciesReference*)_gO)))
                modelSpeciesLabel.setText(ne_ne_getId(ne_sr_getSpecies((NSpeciesReference*)_gO)).c_str());
            
            // set role
            if (ne_sr_isSetRole((NSpeciesReference*)_gO))
                modelRoleLabel.setText(ne_sr_getRoleAsString((NSpeciesReference*)_gO).c_str());
            
            /// layout features
            // set layout id
            if (ne_go_isSetGlyphId(_gO))
                layoutIdButton.setText(ne_go_getGlyphId(_gO).c_str());
            
            // set curve
            if (ne_sr_isSetCurve((NSpeciesReference*)_gO)) {
                curveFMenu->showInfo(ne_sr_getCurve((NSpeciesReference*)_gO), false);
                curveButton.setText("Remove");
            }
            
            /// render features
            if (gSReference->isSetStyle()) {
                _style = gSReference->getStyle();
                styleButton.setText("Remove");
                
                // stroke features
                strokeFMenu->showInfo(_gO, _style);
                
                // head features
                headFMenu->showInfo(_gO, _style);
            }
        }
    }
    
    show();
}

void SpeciesReferenceFeatureMenu::resetValues() {
    modelIdLabel.setText("N/A");
    modelReactionLabel.setText("N/A");
    modelSpeciesLabel.setText("N/A");
    modelRoleLabel.setText("N/A");
    layoutIdButton.setText("N/A");
    curveButton.setText("Add");
    curveFMenu->resetValues();
    styleButton.setText("Add");
    strokeFMenu->resetValues();
    headFMenu->resetValues();
    
    gSReference = NULL;
    _gO = NULL;
    _style = NULL;
}

void SpeciesReferenceFeatureMenu::updateValues() {
    if (gSReference) {
        gSReference->updateValues(_mw);
        showInfo(gSReference);
    }
}

void SpeciesReferenceFeatureMenu::updateStyle(VLocalStyle* style) {
    if (gSReference && style) {
        gSReference->setStyle(style);
        updateValues();
    }
}

void SpeciesReferenceFeatureMenu::setStyle() {
    if (gSReference) {
        gSReference->setStyle(_mw, true);
        updateValues();
    }
}

void SpeciesReferenceFeatureMenu::unSetStyle() {
    if (gSReference) {
        gSReference->unSetStyle();
        updateValues();
    }
}

void SpeciesReferenceFeatureMenu::addOrRemoveCurve() {
    if (curveFMenu) {
        if (curveButton.text() == "Remove") {
            QMessageBox* removeMessageBox =  new QMessageBox();
            removeMessageBox->setWindowTitle("Remove Curve");
            removeMessageBox->setInformativeText("Are you sure you want to remove the Curve of the Species Reference?");
            removeMessageBox->setText("Curve                                                                                                 ");
            removeMessageBox->setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            removeMessageBox->setDefaultButton(QMessageBox::No);
            
            if (removeMessageBox->exec() ==  QMessageBox::Yes && gSReference && gSReference->isSetSReference() && gSReference->getSReference()->isSetCurve()) {
                gSReference->getSReference()->unSetCurve(false);
                _mw->getSBMLDocument()->setLayoutModified(true);
                updateValues();
            }
        }
        else if (curveButton.text() == "Add") {
            if (gSReference->isSetSReference()) {
                LCurve* curve = NULL;
                if (gSReference->getSReference()->getCurve()) {
                    QMessageBox* addPreviousCurveMessageBox =  new QMessageBox();
                    addPreviousCurveMessageBox->setWindowTitle("Use Previous Curve");
                    addPreviousCurveMessageBox->setText("This Species Reference used to have a curve");
                    addPreviousCurveMessageBox->setInformativeText("Do you want to set the previous curve with the Species Reference");
                    addPreviousCurveMessageBox->setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                    addPreviousCurveMessageBox->setDefaultButton(QMessageBox::No);
                    if (addPreviousCurveMessageBox->exec() ==  QMessageBox::Yes)
                        curve = gSReference->getSReference()->getCurve();
                }

                if (!curve) {
                    curve = new LCurve();
                    curveFMenu->showInfo(curve, true);
                }
                gSReference->getSReference()->setCurve(curve);
                _mw->getSBMLDocument()->setLayoutModified(true);
                updateValues();
            }
        }
    }
}

// Line Ending Feature Menu

LineEndingFeatureMenu::LineEndingFeatureMenu(QWidget* parent, MainWindow* mw) : FeatureMenuElement(parent, mw) {
    itemLabel.setText("Line Ending");
    
    /// render features
    // id label
    renderIdLabel.setText("Render Id: ");
    renderIdLabel.setFont(featureTitleFont);
    renderContentLayout.addWidget(&renderIdLabel, renderContentLayout.rowCount() - 1, 0);
    
    // id
    renderIdButton.setStyleSheet(pushButtonStyleSheet);
    renderContentLayout.addWidget(&renderIdButton, renderContentLayout.rowCount() - 1, 1, 1, 2);
    connect(&renderIdButton, SIGNAL(clicked()), this, SLOT(changeRenderId()));
    
    // enable rotation label
    enableRotationLabel.setText("Enable Rotation:");
    enableRotationLabel.setFont(featureTitleFont);
    renderContentLayout.addWidget(&enableRotationLabel, renderContentLayout.rowCount(), 0);
    
    // enable rotation
    enableRotationButton.setStyleSheet(pushButtonStyleSheet);
    renderContentLayout.addWidget(&enableRotationButton, renderContentLayout.rowCount() - 1, 1, 1, 2);
    connect(&enableRotationButton, SIGNAL(clicked()), this, SLOT(changeEnableRotation()));
    
    // fill
    renderContentLayout.addWidget(&fillTitleLabel, renderContentLayout.rowCount(), 0, 1, 3, Qt::AlignCenter);
    renderContentLayout.addWidget(new SeparatingLine(this), renderContentLayout.rowCount(), 0, 1, 3);
    fillFMenu = new FillFeatureMenu(this, _mw);
    renderContentLayout.addWidget(fillFMenu, renderContentLayout.rowCount(), 0, 1, 3);
    connect(fillFMenu, SIGNAL(lEndingChanged(VLineEnding*)), SLOT(updateLineEnding(VLineEnding*)));
    
    // bounding box
    renderContentLayout.addWidget(&boundingBoxTitleLabel, renderContentLayout.rowCount(), 0, 1, 3, Qt::AlignCenter);
    renderContentLayout.addWidget(new SeparatingLine(this), renderContentLayout.rowCount(), 0, 1, 3);
    bBoxFMenu = new BoundingBoxFeatureMenu(this, _mw);
    renderContentLayout.addWidget(bBoxFMenu, renderContentLayout.rowCount(), 0, 1, 3);
    connect(bBoxFMenu, SIGNAL(lEndingChanged(VLineEnding*)), SLOT(updateLineEnding(VLineEnding*)));
    
    // geometric shape
    renderContentLayout.addWidget(&geometricShapeTitleLabel, renderContentLayout.rowCount(), 0, 1, 3, Qt::AlignCenter);
    renderContentLayout.addWidget(new SeparatingLine(this), renderContentLayout.rowCount(), 0, 1, 3);
    renderContentLayout.addWidget(&geometricShapeLabel, renderContentLayout.rowCount(), 0, 1, 3, Qt::AlignLeft);
    renderContentLayout.addWidget(&geometricShapeButton, renderContentLayout.rowCount() - 1, 0, 1, 3, Qt::AlignRight);
    
    // add render features to menu
    renderMenu = new CollapsibleMenu(this, "Render Features", 300);
    renderMenu->setContentLayout(renderContentLayout);
    qLayout.addWidget(renderMenu, qLayout.rowCount(), 0);
    
    // reset values
    resetValues();
}

void LineEndingFeatureMenu::showInfo(GraphicalLineEnding* gLE, GraphicalSReference* gSR, QString head) {
    // reset values
    resetValues();
    
    // render features
    if (gLE && gSR) {
        gLineEnding = gLE;
        gSReference = gSR;
        _lE = gLE->getLEnding();
        _head = head;
        
        if (gLineEnding->isSetLEnding()) {
            
            // set id
            if (ne_ve_isSetId(_lE))
                renderIdButton.setText(ne_ve_getId(_lE).c_str());
            
            // set enable rotation
            if (ne_le_isSetEnableRotation(_lE) && !ne_le_getEnableRotation(_lE)) {
                enableRotationButton.setText("False");
            }
            else
                enableRotationButton.setText("True");
            
            if (ne_le_isSetBoundingBox(_lE)) {
                // bounding box
                bBoxFMenu->showInfo(_lE);
                boundingBoxButton.setText("Remove");
                
                // fill features
                fillFMenu->showInfo(_lE);
                
                // geometric shape features
                geometricShapeButton.setStyleSheet(pushButtonStyleSheet);
                geometricShapeButton.blockSignals(false);
                
                if (ne_grp_getNumGeometricShapes(ne_le_getGroup(_lE))) {
                    // fill features
                    fillFMenu->showInfo(ne_grp_getGeometricShape(ne_le_getGroup(_lE), 0));
                    
                    // set geometric shape features
                    switch (ne_gs_getShape(ne_grp_getGeometricShape(ne_le_getGroup(_lE), 0))) {
                        case 0:
                            fillFMenu->resetValues();
                            gSFMenuElement = new ImageGeometricShapeFeatureMenu(this, _mw);
                            geometricShapeButton.setText("Image");
                         ((ImageGeometricShapeFeatureMenu*)(gSFMenuElement))->showInfo(_lE);
                            break;
                            
                        case 3:
                            gSFMenuElement = new RectangleGeometricShapeFeatureMenu(this, _mw);
                            geometricShapeButton.setText("Rectangle");
                            ((RectangleGeometricShapeFeatureMenu*)(gSFMenuElement))->showInfo(_lE);
                           break;
                            
                        case 4:
                            gSFMenuElement = new EllipseGeometricShapeFeatureMenu(this, _mw);
                            geometricShapeButton.setText("Ellipse");
                            ((EllipseGeometricShapeFeatureMenu*)(gSFMenuElement))->showInfo(_lE);
                            break;
                            
                        case 5:
                            gSFMenuElement = new PolygonGeometricShapeFeatureMenu(this, _mw);
                            geometricShapeButton.setText("Polygon");
                            ((PolygonGeometricShapeFeatureMenu*)(gSFMenuElement))->showInfo(_lE);
                            break;
                            
                        default:
                            break;
                    }
                    
                    if (gSFMenuElement) {
                        renderContentLayout.addWidget(gSFMenuElement, renderContentLayout.rowCount(), 0, 1, 3);
                        connect(gSFMenuElement, SIGNAL(lEndingChanged(VLineEnding*)), SLOT(updateLineEnding(VLineEnding*)));
                    }
                }
            }
        }
    }
    
    show();
}

void LineEndingFeatureMenu::resetValues() {
    renderIdButton.setText("N/A");
    enableRotationButton.setText("N/A");
    bBoxFMenu->resetValues();
    fillFMenu->resetValues();
    if (gSFMenuElement) {
        renderContentLayout.removeWidget(gSFMenuElement);
        delete gSFMenuElement;
        gSFMenuElement = NULL;
    }
    geometricShapeButton.setText("N/A");
    geometricShapeButton.blockSignals(true);
    geometricShapeButton.setStyleSheet(disabledPushButtonStyleSheet);
    
    gLineEnding = NULL;
    gSReference = NULL;
    _lE = NULL;
    _head.clear();
}

void LineEndingFeatureMenu::updateValues() {
    if (gLineEnding && gSReference)
        showInfo(gLineEnding, gSReference, _head);
}

void LineEndingFeatureMenu::updateLineEnding(VLineEnding* lineEnding) {
    if (gLineEnding && gLineEnding->isSetLEnding() && gSReference && lineEnding) {
        if (!stringCompare(ne_ve_getId(gLineEnding->getLEnding()), ne_ve_getId(lineEnding))) {
            // create a new graphical line ending
            gLineEnding = new GraphicalLineEnding();
            
            // store the graphical line ending
            _mw->addGLEnding(gLineEnding);
            
            // set graphical line ending info using veneer line ending
            getInfoFromLineEnding(_mw, lineEnding, gLineEnding);
            
            VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), gSReference->getSReference(), gSReference->getStyle());
            
            if (_head == "start")
                ne_grp_setStartHead(ne_stl_getGroup(style), ne_ve_getId(lineEnding));
            else if (_head == "end")
                ne_grp_setEndHead(ne_stl_getGroup(style), ne_ve_getId(lineEnding));
            
            if (style) {
                gSReference->setStyle(style);
                gSReference->updateValues(_mw);
            }
        }
        else {
            getInfoFromLineEnding(_mw, lineEnding, gLineEnding);
            gSReference->updateValues(_mw);
        }
        
        updateValues();
    }
}

void LineEndingFeatureMenu::changeRenderId() {
    QInputDialog* modificationBox =  new QInputDialog();
    modificationBox->setWindowTitle("Change Render Id");
    modificationBox->setLabelText("Render Id:");
    modificationBox->setOkButtonText("Apply");
    modificationBox->resize(300, 200);
    
    if (renderIdButton.text() != "N/A")
        modificationBox->setTextValue(renderIdButton.text());
    
    if (modificationBox->exec() && _mw && _mw->isSetSBMLDocument() && _lE && !modificationBox->textValue().isEmpty()) {
        
        VLineEnding* lineEnding = ne_ven_getLocalLineEndingFromGlobalLineEnding(_mw->getSBMLDocument()->getVeneer(), _lE);
        
        if (!ne_ve_setId(_mw->getSBMLDocument()->getVeneer(), lineEnding, modificationBox->textValue().toStdString())) {
            _mw->getSBMLDocument()->setRenderModified(true);
            updateLineEnding(lineEnding);
        }
    }
}

void LineEndingFeatureMenu::changeEnableRotation() {
    QInputDialog* modificationBox =  new QInputDialog();
    modificationBox->setWindowTitle("Change Enable Rotation of the Line Ending");
    modificationBox->setLabelText("Enable Rotation:");
    modificationBox->setOkButtonText("Apply");
    modificationBox->resize(300, 200);
    
    QStringList enableRotationItems;
    enableRotationItems << "True" << "False";
    modificationBox->setComboBoxItems(enableRotationItems);
    
    if (enableRotationButton.text() != "N/A")
        modificationBox->setTextValue(enableRotationButton.text());
    
    if (modificationBox->exec() && _mw && _mw->isSetSBMLDocument() && !modificationBox->textValue().isEmpty()) {
        
        VLineEnding* lineEnding = ne_ven_getLocalLineEndingFromGlobalLineEnding(_mw->getSBMLDocument()->getVeneer(), _lE);
        if (modificationBox->textValue() == "True")
            ne_le_enableRotation(lineEnding, true);
        else
            ne_le_enableRotation(lineEnding, false);
        
        if (lineEnding) {
            _mw->getSBMLDocument()->setRenderModified(true);
            updateLineEnding(lineEnding);
        }
    }
}

void LineEndingFeatureMenu::updateGeometricShape(const QString& shapeType, const int& numberOfPolygonVertices, const QString& imageFileName) {
    if (_mw && _mw->isSetSBMLDocument()) {
        VLineEnding* lineEnding = ne_ven_getLocalLineEndingFromGlobalLineEnding(_mw->getSBMLDocument()->getVeneer(), _lE);
        
        if (lineEnding) {
            ne_grp_removeGeometricShapes(ne_le_getGroup(lineEnding));
            
            if (shapeType == "Rectangle")
                ne_grp_addGeometricShape(ne_le_getGroup(lineEnding), GRP_ELT_SHPE_REC, true);
            else if (shapeType == "Ellipse")
                ne_grp_addGeometricShape(ne_le_getGroup(lineEnding), GRP_ELT_SHPE_ELP, true);
            else if (shapeType == "Polygon" && numberOfPolygonVertices != -1)
                ne_grp_addGeometricShape(ne_le_getGroup(lineEnding), GRP_ELT_SHPE_PLG, true, numberOfPolygonVertices);
            else if (shapeType == "Image" && !imageFileName.isEmpty())
                ne_grp_addGeometricShape(ne_le_getGroup(lineEnding), GRP_ELT_SHPE_IMG, true, -1, imageFileName.toStdString());
            
            _mw->getSBMLDocument()->setRenderModified(true);
            updateLineEnding(lineEnding);
        }
    }
    
}

// BoundingBoxFeatureMenu

BoundingBoxFeatureMenu::BoundingBoxFeatureMenu(QWidget* parent, MainWindow* mw) : QGroupBox(parent) {
    _mw = mw;
    hide();
    
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setStyleSheet("QGroupBox { background-color: white; border: no-border; }");
    
    setLayout(&bBoxLayout);
    bBoxLayout.setContentsMargins(0, 0, 0, 0);
    
    featureTitleFont.setBold(true);
    featureTitleFont.setCapitalization(QFont::Capitalize);
    featureTitleFont.setPointSize(12);
    pushButtonStyleSheet = "QPushButton { border: 2px solid #8f8f91; border-radius: 4px; background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #f6f7fa, stop: 1 #dadbde); min-width: 80px; } QPushButton:pressed { background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #dadbde, stop: 1 #f6f7fa); }";
    
    // position label
    positionLabel.setText("Position:");
    positionLabel.setFont(featureTitleFont);
    bBoxLayout.addWidget(&positionLabel, bBoxLayout.rowCount(), 0, 1, 2);
    
    // x label
    positionXLabel.setText("X:");
    positionXLabel.setFont(featureTitleFont);
    bBoxLayout.addWidget(&positionXLabel, bBoxLayout.rowCount(), 1, Qt::AlignLeft);
    
    // x
    positionXButton.setStyleSheet(pushButtonStyleSheet);
    bBoxLayout.addWidget(&positionXButton, bBoxLayout.rowCount() - 1, 2, Qt::AlignRight);
    connect(&positionXButton, SIGNAL(clicked()), this, SLOT(changePositionX()));
    
    // y label
    positionYLabel.setText("Y:");
    positionYLabel.setFont(featureTitleFont);
    bBoxLayout.addWidget(&positionYLabel, bBoxLayout.rowCount(), 1, Qt::AlignLeft);
    
    // y
    positionYButton.setStyleSheet(pushButtonStyleSheet);
    bBoxLayout.addWidget(&positionYButton, bBoxLayout.rowCount() - 1, 2, Qt::AlignRight);
    connect(&positionYButton, SIGNAL(clicked()), this, SLOT(changePositionY()));
    
    // dimension label
    dimensionLabel.setText("Dimensions:");
    dimensionLabel.setFont(featureTitleFont);
    bBoxLayout.addWidget(&dimensionLabel, bBoxLayout.rowCount(), 0, 1, 2);
    
    // width label
    dimensionWidthLabel.setText("Width:");
    dimensionWidthLabel.setFont(featureTitleFont);
    bBoxLayout.addWidget(&dimensionWidthLabel, bBoxLayout.rowCount(), 1, Qt::AlignLeft);
    
    // width
    dimensionWidthButton.setStyleSheet(pushButtonStyleSheet);
    bBoxLayout.addWidget(&dimensionWidthButton, bBoxLayout.rowCount() - 1, 2, Qt::AlignRight);
    connect(&dimensionWidthButton, SIGNAL(clicked()), this, SLOT(changeDimensionWidth()));
    
    // height label
    dimensionHeightLabel.setText("Height:");
    dimensionHeightLabel.setFont(featureTitleFont);
    bBoxLayout.addWidget(&dimensionHeightLabel, bBoxLayout.rowCount(), 1, Qt::AlignLeft);
    
    // height
    dimensionHeightButton.setStyleSheet(pushButtonStyleSheet);
    bBoxLayout.addWidget(&dimensionHeightButton, bBoxLayout.rowCount() - 1, 2, Qt::AlignRight);
    connect(&dimensionHeightButton, SIGNAL(clicked()), this, SLOT(changeDimensionHeight()));
    
    // reset values
    resetValues();
}

void BoundingBoxFeatureMenu::showInfo(NGraphicalObject* gO) {
    // reset values
    resetValues();
    
    if (gO) {
        _gO = gO;
        
        if (ne_go_isSetBoundingBox(_gO)) {
            // set x
             positionXButton.setText(QString::number(ne_bb_getX(ne_go_getBoundingBox(_gO)), 'f', 1));
             
             // set y
             positionYButton.setText(QString::number(ne_bb_getY(ne_go_getBoundingBox(_gO)), 'f', 1));
             
            // set width
             dimensionWidthButton.setText(QString::number(ne_bb_getWidth(ne_go_getBoundingBox(_gO)), 'f', 1));
             
            // set height
             dimensionHeightButton.setText(QString::number(ne_bb_getHeight(ne_go_getBoundingBox(_gO)), 'f', 1));
            
            show();
        }
    }
}

void BoundingBoxFeatureMenu::showInfo(VLineEnding* lE) {
    // reset values
    resetValues();
    
    if (lE) {
        _lE = lE;
        
        if (ne_le_isSetBoundingBox(_lE)) {
            // set x
             positionXButton.setText(QString::number(ne_bb_getX(ne_le_getBoundingBox(_lE)), 'f', 1));
             
             // set y
             positionYButton.setText(QString::number(ne_bb_getY(ne_le_getBoundingBox(_lE)), 'f', 1));
             
            // set width
             dimensionWidthButton.setText(QString::number(ne_bb_getWidth(ne_le_getBoundingBox(_lE)), 'f', 1));
             
            // set height
             dimensionHeightButton.setText(QString::number(ne_bb_getHeight(ne_le_getBoundingBox(_lE)), 'f', 1));
            
            show();
        }
    }
}

void BoundingBoxFeatureMenu::resetValues() {
    hide();
    positionXButton.setText("N/A");
    positionYButton.setText("N/A");
    dimensionWidthButton.setText("N/A");
    dimensionHeightButton.setText("N/A");
    _gO = NULL;
    _lE = NULL;
}

void BoundingBoxFeatureMenu::changePositionX() {
    QInputDialog* modificationBox =  new QInputDialog();
    modificationBox->setWindowTitle("Change Position X of the Bounding Box");
    modificationBox->setLabelText("Position X:");
    modificationBox->setDoubleStep(0.1);
    modificationBox->setDoubleDecimals(1);
    modificationBox->setDoubleRange(_mw->getScene()->sceneRect().x(), _mw->getScene()->sceneRect().x() + _mw->getScene()->sceneRect().width());
    modificationBox->setOkButtonText("Apply");
    modificationBox->resize(300, 200);
    
    if (positionXButton.text() != "N/A")
        modificationBox->setDoubleValue(positionXButton.text().toDouble());
    
    if (modificationBox->exec() && _mw && _mw->isSetSBMLDocument()) {
        if (!ne_bb_setX(ne_go_getBoundingBox(_gO), modificationBox->doubleValue())) {
            _mw->getSBMLDocument()->setLayoutModified(true);
            emit bBoxChanged();
        }
        else {
            VLineEnding* lineEnding = ne_ven_getLocalLineEndingFromGlobalLineEnding(_mw->getSBMLDocument()->getVeneer(), _lE);
            
            if (!ne_bb_setX(ne_le_getBoundingBox(lineEnding), modificationBox->doubleValue())) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit lEndingChanged(lineEnding);
            }
        }
    }
}

void BoundingBoxFeatureMenu::changePositionY() {
    QInputDialog* modificationBox =  new QInputDialog();
    modificationBox->setWindowTitle("Change Position Y of the Bounding Box");
    modificationBox->setLabelText("Position Y:");
    modificationBox->setDoubleStep(0.1);
    modificationBox->setDoubleDecimals(1);
    modificationBox->setDoubleRange(_mw->getScene()->sceneRect().y(), _mw->getScene()->sceneRect().y() + _mw->getScene()->sceneRect().height());
    modificationBox->setOkButtonText("Apply");
    modificationBox->resize(300, 200);
    
    if (positionYButton.text() != "N/A")
        modificationBox->setDoubleValue(positionYButton.text().toDouble());
    
    if (modificationBox->exec() && _mw && _mw->isSetSBMLDocument()) {
        if (!ne_bb_setY(ne_go_getBoundingBox(_gO), modificationBox->doubleValue())) {
            _mw->getSBMLDocument()->setLayoutModified(true);
            emit bBoxChanged();
        }
        else {
            VLineEnding* lineEnding = ne_ven_getLocalLineEndingFromGlobalLineEnding(_mw->getSBMLDocument()->getVeneer(), _lE);
            
            if (!ne_bb_setY(ne_le_getBoundingBox(lineEnding), modificationBox->doubleValue())) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit lEndingChanged(lineEnding);
            }
        }
    }
}

void BoundingBoxFeatureMenu::changeDimensionWidth() {
    QInputDialog* modificationBox =  new QInputDialog();
    modificationBox->setWindowTitle("Change Dimension Width of the Bounding Box");
    modificationBox->setLabelText("Dimension Width:");
    modificationBox->setDoubleStep(0.1);
    modificationBox->setDoubleDecimals(1);
    modificationBox->setDoubleRange(0.0, _mw->getScene()->sceneRect().width());
    modificationBox->setOkButtonText("Apply");
    modificationBox->resize(300, 200);
    
    if (dimensionWidthButton.text() != "N/A")
        modificationBox->setDoubleValue(dimensionWidthButton.text().toDouble());
    
    if (modificationBox->exec() && _mw && _mw->isSetSBMLDocument()) {
        if (!ne_bb_setWidth(ne_go_getBoundingBox(_gO), modificationBox->doubleValue())) {
            _mw->getSBMLDocument()->setLayoutModified(true);
            emit bBoxDimensionsChanged("", -1, "");
            emit bBoxChanged();
        }
        else {
            VLineEnding* lineEnding = ne_ven_getLocalLineEndingFromGlobalLineEnding(_mw->getSBMLDocument()->getVeneer(), _lE);
            
            if (!ne_bb_setWidth(ne_le_getBoundingBox(lineEnding), modificationBox->doubleValue())) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit lEndingChanged(lineEnding);
            }
        }
    }
}

void BoundingBoxFeatureMenu::changeDimensionHeight() {
    QInputDialog* modificationBox =  new QInputDialog();
    modificationBox->setWindowTitle("Change Dimension Height of the Bounding Box");
    modificationBox->setLabelText("Dimension Height:");
    modificationBox->setDoubleStep(0.1);
    modificationBox->setDoubleDecimals(1);
    modificationBox->setDoubleRange(0.0, _mw->getScene()->sceneRect().height());
    modificationBox->setOkButtonText("Apply");
    modificationBox->resize(300, 200);
    
    if (dimensionHeightButton.text() != "N/A")
        modificationBox->setDoubleValue(dimensionHeightButton.text().toDouble());
    
    if (modificationBox->exec() && _mw && _mw->isSetSBMLDocument()) {
        if (!ne_bb_setHeight(ne_go_getBoundingBox(_gO), modificationBox->doubleValue())) {
            _mw->getSBMLDocument()->setLayoutModified(true);
            emit bBoxDimensionsChanged("", -1, "");
            emit bBoxChanged();
        }
        else {
            VLineEnding* lineEnding = ne_ven_getLocalLineEndingFromGlobalLineEnding(_mw->getSBMLDocument()->getVeneer(), _lE);
            
            if (!ne_bb_setHeight(ne_le_getBoundingBox(lineEnding), modificationBox->doubleValue())) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit lEndingChanged(lineEnding);
            }
        }
    }
}

// CurveFeatureMenu

CurveFeatureMenu::CurveFeatureMenu(QWidget* parent, MainWindow* mw) : QGroupBox(parent) {
    _mw = mw;
    hide();
    
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setStyleSheet("QGroupBox { background-color: white; border: no-border; }");
    
    setLayout(&curveLayout);
    curveLayout.setContentsMargins(0, 0, 0, 0);
    
    featureTitleFont.setBold(true);
    featureTitleFont.setCapitalization(QFont::Capitalize);
    featureTitleFont.setPointSize(12);
    pushButtonStyleSheet = "QPushButton { border: 2px solid #8f8f91; border-radius: 4px; background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #f6f7fa, stop: 1 #dadbde); min-width: 80px; } QPushButton:pressed { background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #dadbde, stop: 1 #f6f7fa); }";
    disabledPushButtonStyleSheet = "QPushButton { border: 2px solid #8f8f91; border-radius: 4px; background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #f6f7fa, stop: 1 #dadbde); color: #bb848e;  min-width: 80px; }";
    
    // addLine
    addLineButton.setStyleSheet(pushButtonStyleSheet);
    addLineButton.setText("Add");
    curveLayout.addWidget(&addLineButton, curveLayout.rowCount(), 0, 1, 3, Qt::AlignCenter);
    connect(&addLineButton, SIGNAL(clicked()), this, SLOT(addLineFeatureMenu()));
    
    // removeLine
    removeLineButton.setText("Remove");
    curveLayout.addWidget(&removeLineButton, curveLayout.rowCount(), 0, 1, 3, Qt::AlignCenter);
    connect(&removeLineButton, SIGNAL(clicked()), this, SLOT(removeLineFeatureMenu()));
    
    // reset values
    resetValues();
}

void CurveFeatureMenu::showInfo(LCurve* curve, const bool& askForElementForEmptyCurve) {
    // reset values
    resetValues();
    
    if(curve) {
        _curve = curve;
        LineFeatureMenu* lFeatureMenu = NULL;
        for (int i = 0; i < ne_crv_getNumElements(_curve); ++i) {
            lFeatureMenu = new LineFeatureMenu(this, _mw, i);
            curveLayout.addWidget(lFeatureMenu, curveLayout.rowCount(), 0, 1, 3);
            connect(lFeatureMenu, SIGNAL(lineChanged()), this, SLOT(changeCurve()));
            lFeatureMenu->showInfo(ne_crv_getElement(_curve, i));
            _lineFeatureMenus.push_back(lFeatureMenu);
        }
        
        if (_lineFeatureMenus.size() > 1) {
            removeLineButton.setStyleSheet(pushButtonStyleSheet);
            removeLineButton.blockSignals(false);
        }
        
        show();
    }
    
    if (!_lineFeatureMenus.size() && askForElementForEmptyCurve)
        addLineFeatureMenu();
}

void CurveFeatureMenu::resetValues() {
    hide();
    for (constLineFeatureMenuIt lFMIt = lineFeatureMenusBegin(); lFMIt != lineFeatureMenusEnd(); ++ lFMIt) {
        curveLayout.removeWidget(*lFMIt);
        delete *lFMIt;
    }
    _lineFeatureMenus.clear();
    removeLineButton.setStyleSheet(disabledPushButtonStyleSheet);
    removeLineButton.blockSignals(true);
    _curve = NULL;
}

void CurveFeatureMenu::addLineFeatureMenu() {
    QDialog* modificationBox =  new QDialog();
    modificationBox->setWindowTitle("Add an element to Curve");
    QGridLayout modificationBoxLayout(modificationBox);
    modificationBoxLayout.addWidget(new QLabel("Type"), modificationBoxLayout.rowCount() - 1, 0);
    QComboBox* elementTypeComboBox = new QComboBox(modificationBox);
    elementTypeComboBox->addItem(QString("Line"));
    elementTypeComboBox->addItem(QString("CubicBezier"));
    elementTypeComboBox->setCurrentIndex(0);
    modificationBoxLayout.addWidget(elementTypeComboBox, modificationBoxLayout.rowCount() - 1, 1);
    
    modificationBoxLayout.addWidget(new QLabel("Place"), modificationBoxLayout.rowCount() - 1, 2);
    QComboBox* placeComboBox = new QComboBox(modificationBox);
    for (int i = 1; i <= _lineFeatureMenus.size(); ++i)
        placeComboBox->addItem(QString::number(i));
    placeComboBox->addItem(QString("Last"));
    placeComboBox->setCurrentIndex(_lineFeatureMenus.size());
    modificationBoxLayout.addWidget(placeComboBox, modificationBoxLayout.rowCount() - 1, 3);
    
    // line
    QGroupBox* lineBox = new QGroupBox(modificationBox);
    QGridLayout lineBoxLayout(lineBox);
    lineBox->setStyleSheet("QGroupBox { background-color: transparent; border: no-border; }");
    lineBoxLayout.setContentsMargins(0, 5, 0, 0);
    modificationBoxLayout.addWidget(lineBox, modificationBoxLayout.rowCount(), 0, 1, 4);
    lineBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    
    // start
    QDoubleSpinBox* startXValueSpinBox = new QDoubleSpinBox(modificationBox);
    QDoubleSpinBox* startYValueSpinBox = new QDoubleSpinBox(modificationBox);
    startXValueSpinBox->setValue(0.0);
    startYValueSpinBox->setValue(0.0);
    startXValueSpinBox->setDecimals(1);
    startYValueSpinBox->setDecimals(1);
    startXValueSpinBox->setRange(_mw->getScene()->sceneRect().x(), _mw->getScene()->sceneRect().x() + _mw->getScene()->sceneRect().width());
    startYValueSpinBox->setRange(_mw->getScene()->sceneRect().y(), _mw->getScene()->sceneRect().y() + _mw->getScene()->sceneRect().height());
    
    lineBoxLayout.addWidget(new QLabel("Start"), lineBoxLayout.rowCount(), 0, 1, 2);
    lineBoxLayout.addWidget(new QLabel("X:"), lineBoxLayout.rowCount(), 1, Qt::AlignRight);
    lineBoxLayout.addWidget(startXValueSpinBox, lineBoxLayout.rowCount() - 1, 2, 1, 2);
    lineBoxLayout.addWidget(new QLabel("Y:"), lineBoxLayout.rowCount(), 1, Qt::AlignRight);
    lineBoxLayout.addWidget(startYValueSpinBox, lineBoxLayout.rowCount() - 1, 2, 1, 2);
    
    // end
    QDoubleSpinBox* endXValueSpinBox = new QDoubleSpinBox(modificationBox);
    QDoubleSpinBox* endYValueSpinBox = new QDoubleSpinBox(modificationBox);
    endXValueSpinBox->setValue(0.0);
    endYValueSpinBox->setValue(0.0);
    endXValueSpinBox->setDecimals(1);
    endYValueSpinBox->setDecimals(1);
    endXValueSpinBox->setRange(_mw->getScene()->sceneRect().x(), _mw->getScene()->sceneRect().x() + _mw->getScene()->sceneRect().width());
    endYValueSpinBox->setRange(_mw->getScene()->sceneRect().y(), _mw->getScene()->sceneRect().y() + _mw->getScene()->sceneRect().height());
    
    lineBoxLayout.addWidget(new QLabel("End"), lineBoxLayout.rowCount(), 0, 1, 2);
    lineBoxLayout.addWidget(new QLabel("X:"), lineBoxLayout.rowCount(), 1, Qt::AlignRight);
    lineBoxLayout.addWidget(endXValueSpinBox, lineBoxLayout.rowCount() - 1, 2, 1, 2);
    lineBoxLayout.addWidget(new QLabel("Y:"), lineBoxLayout.rowCount(), 1, Qt::AlignRight);
    lineBoxLayout.addWidget(endYValueSpinBox, lineBoxLayout.rowCount() - 1, 2, 1, 2);
    
    // cubic bezier
    
    QGroupBox* cubicBezierBox = new QGroupBox(modificationBox);
    QGridLayout cubicBezierBoxLayout(cubicBezierBox);
    cubicBezierBox->setStyleSheet("QGroupBox { background-color: transparent; border: no-border; }");
    cubicBezierBoxLayout.setContentsMargins(0, 5, 0, 0);
    modificationBoxLayout.addWidget(cubicBezierBox, modificationBoxLayout.rowCount(), 0, 1, 4);
    cubicBezierBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    cubicBezierBox->hide();
    
    // basepoint 1
    QDoubleSpinBox* basePoint1XValueSpinBox = new QDoubleSpinBox(modificationBox);
    QDoubleSpinBox* basePoint1YValueSpinBox = new QDoubleSpinBox(modificationBox);
    basePoint1XValueSpinBox->setValue(0.0);
    basePoint1YValueSpinBox->setValue(0.0);
    basePoint1XValueSpinBox->setDecimals(1);
    basePoint1YValueSpinBox->setDecimals(1);
    basePoint1XValueSpinBox->setRange(_mw->getScene()->sceneRect().x(), _mw->getScene()->sceneRect().x() + _mw->getScene()->sceneRect().width());
    basePoint1YValueSpinBox->setRange(_mw->getScene()->sceneRect().y(), _mw->getScene()->sceneRect().y() + _mw->getScene()->sceneRect().height());
    
    cubicBezierBoxLayout.addWidget(new QLabel("Base Point 1"), cubicBezierBoxLayout.rowCount(), 0, 1, 2);
    cubicBezierBoxLayout.addWidget(new QLabel("X:"), cubicBezierBoxLayout.rowCount(), 1, Qt::AlignRight);
    cubicBezierBoxLayout.addWidget(basePoint1XValueSpinBox, cubicBezierBoxLayout.rowCount() - 1, 2, 1, 2);
    cubicBezierBoxLayout.addWidget(new QLabel("Y:"), cubicBezierBoxLayout.rowCount(), 1, Qt::AlignRight);
    cubicBezierBoxLayout.addWidget(basePoint1YValueSpinBox, cubicBezierBoxLayout.rowCount() - 1, 2, 1, 2);
    
    // basepoint 2
    QDoubleSpinBox* basePoint2XValueSpinBox = new QDoubleSpinBox(modificationBox);
    QDoubleSpinBox* basePoint2YValueSpinBox = new QDoubleSpinBox(modificationBox);
    basePoint2XValueSpinBox->setValue(0.0);
    basePoint2YValueSpinBox->setValue(0.0);
    basePoint2XValueSpinBox->setDecimals(1);
    basePoint2YValueSpinBox->setDecimals(1);
    basePoint2XValueSpinBox->setRange(_mw->getScene()->sceneRect().x(), _mw->getScene()->sceneRect().x() + _mw->getScene()->sceneRect().width());
    basePoint2YValueSpinBox->setRange(_mw->getScene()->sceneRect().y(), _mw->getScene()->sceneRect().y() + _mw->getScene()->sceneRect().height());
    
    cubicBezierBoxLayout.addWidget(new QLabel("Base Point 2"), cubicBezierBoxLayout.rowCount(), 0, 1, 2);
    cubicBezierBoxLayout.addWidget(new QLabel("X:"), cubicBezierBoxLayout.rowCount(), 1, Qt::AlignRight);
    cubicBezierBoxLayout.addWidget(basePoint2XValueSpinBox, cubicBezierBoxLayout.rowCount() - 1, 2, 1, 2);
    cubicBezierBoxLayout.addWidget(new QLabel("Y:"), cubicBezierBoxLayout.rowCount(), 1, Qt::AlignRight);
    cubicBezierBoxLayout.addWidget(basePoint2YValueSpinBox, cubicBezierBoxLayout.rowCount() - 1, 2, 1, 2);
    
    QObject::connect(elementTypeComboBox, QOverload<int>::of(&QComboBox::activated), modificationBox, [modificationBox, cubicBezierBox] (const int& index) {
        if (index == 0)
            cubicBezierBox->hide();
        else if (index == 1)
            cubicBezierBox->show();
        
        modificationBox->layout()->setAlignment(Qt::AlignTop);
        modificationBox->layout()->update();
        modificationBox->adjustSize();
    });
    
    QDialogButtonBox modificationBoxButtons(Qt::Horizontal, modificationBox);
    modificationBoxButtons.addButton(QDialogButtonBox::Cancel);
    modificationBoxButtons.addButton(QString("Add"), QDialogButtonBox::AcceptRole);
    modificationBoxLayout.addWidget(&modificationBoxButtons, modificationBoxLayout.rowCount(), 1, 1, 3);
    QObject::connect(&modificationBoxButtons, SIGNAL(accepted()), modificationBox, SLOT(accept()));
    QObject::connect(&modificationBoxButtons, SIGNAL(rejected()), modificationBox, SLOT(reject()));
    
    if (modificationBox->exec() == QDialog::Accepted && _mw && _mw->isSetSBMLDocument() && _curve) {
        
        LPoint* startP = new LPoint(startXValueSpinBox->value(), startYValueSpinBox->value());
        LPoint* endP = new LPoint(endXValueSpinBox->value(), endYValueSpinBox->value());
        LPoint* bP1 = new LPoint(basePoint1XValueSpinBox->value(), basePoint1YValueSpinBox->value());
        LPoint* bP2 = new LPoint(basePoint2XValueSpinBox->value(), basePoint2YValueSpinBox->value());
        if ((elementTypeComboBox->currentIndex() == 0 && ne_crv_addElement(_curve, placeComboBox->currentIndex(), startP, endP)) || (elementTypeComboBox->currentIndex() == 1 && ne_crv_addElement(_curve, placeComboBox->currentIndex(), startP, endP, bP1, bP2))) {
            _mw->getSBMLDocument()->setLayoutModified(true);
            emit curveChanged();
        }
    }
}

void CurveFeatureMenu::removeLineFeatureMenu() {
    QInputDialog* modificationBox =  new QInputDialog();
    modificationBox->setWindowTitle("Remove an element from Curve");
    modificationBox->setLabelText("Element:");
    modificationBox->setOkButtonText("Remove");
    modificationBox->resize(300, 200);
    
    QStringList lineIndexItems;
    for (int i = 1; i <= _lineFeatureMenus.size(); ++i)
        lineIndexItems << QString::number(i);
        
    modificationBox->setComboBoxItems(lineIndexItems);
    modificationBox->setTextValue(0);
    
    if (modificationBox->exec() && _mw && _mw->isSetSBMLDocument() &&  !ne_crv_removeElement(_curve, modificationBox->textValue().toInt() - 1)) {
        _mw->getSBMLDocument()->setLayoutModified(true);
        emit curveChanged();
    }
}

void CurveFeatureMenu::changeCurve() {
    emit curveChanged();
}

// StrokeFeatureMenu

StrokeFeatureMenu::StrokeFeatureMenu(QWidget* parent, MainWindow* mw) : QGroupBox(parent) {
    _mw = mw;
    hide();
    
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setStyleSheet("QGroupBox { background-color: white; border: no-border; }");
    
    setLayout(&strokeLayout);
    strokeLayout.setContentsMargins(0, 0, 0, 0);
    
    featureTitleFont.setBold(true);
    featureTitleFont.setCapitalization(QFont::Capitalize);
    featureTitleFont.setPointSize(12);
    pushButtonStyleSheet = "QPushButton { border: 2px solid #8f8f91; border-radius: 4px; background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #f6f7fa, stop: 1 #dadbde); min-width: 80px; } QPushButton:pressed { background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #dadbde, stop: 1 #f6f7fa); }";
    
    // stroke color label
    strokeColorLabel.setText("Color:");
    strokeColorLabel.setFont(featureTitleFont);
    strokeLayout.addWidget(&strokeColorLabel, strokeLayout.rowCount(), 0);
    
    // stroke color
    strokeColorButton.setStyleSheet(pushButtonStyleSheet);
    strokeLayout.addWidget(&strokeColorButton, strokeLayout.rowCount() - 1, 1);
    connect(&strokeColorButton, SIGNAL(clicked()), this, SLOT(changeStrokeColor()));
    
    // stroke width label
    strokeWidthLabel.setText("Width:");
    strokeWidthLabel.setFont(featureTitleFont);
    strokeLayout.addWidget(&strokeWidthLabel, strokeLayout.rowCount(), 0);
    
    // stroke width
    strokeWidthButton.setStyleSheet(pushButtonStyleSheet);
    strokeLayout.addWidget(&strokeWidthButton, strokeLayout.rowCount() - 1, 1);
    connect(&strokeWidthButton, SIGNAL(clicked()), this, SLOT(changeStrokeWidth()));
    
    // stroke dash array label
    strokeDashArrayLabel.setText("DashArray:");
    strokeDashArrayLabel.setFont(featureTitleFont);
    strokeLayout.addWidget(&strokeDashArrayLabel, strokeLayout.rowCount(), 0);
    
    // stroke dash array
    strokeDashArrayButton.setStyleSheet(pushButtonStyleSheet);
    strokeLayout.addWidget(&strokeDashArrayButton, strokeLayout.rowCount() - 1, 1);
    connect(&strokeDashArrayButton, SIGNAL(clicked()), this, SLOT(changeStrokeDashArray()));
    
    // reset values
    resetValues();
}

void StrokeFeatureMenu::showInfo(NGraphicalObject* gO, VGlobalStyle* style) {
    // reset values
    resetValues();
    
    if (gO && style) {
        _style = style;
        _gO = gO;
        
        // set stroke color
        if (ne_grp_isSetStrokeColor(ne_stl_getGroup(_style)))
            strokeColorButton.setText(ne_grp_getStrokeColor(ne_stl_getGroup(_style)).c_str());
        
        // set stroke width
        if (ne_grp_isSetStrokeWidth(ne_stl_getGroup(_style)))
            strokeWidthButton.setText(QString::number(ne_grp_getStrokeWidth(ne_stl_getGroup(_style)), 'f', 1));
        
        // set stroke dash array
        if (ne_grp_isSetStrokeDashArray(ne_stl_getGroup(_style))) {
            QString dashArrayButtonText;
            for (int i = 0; i < ne_grp_getNumStrokeDashes(ne_stl_getGroup(_style)); ++i) {
                dashArrayButtonText.append(QString::number(ne_grp_getStrokeDash(ne_stl_getGroup(_style), i)));
                
                if (i < ne_grp_getNumStrokeDashes(ne_stl_getGroup(_style)) - 1)
                    dashArrayButtonText.append(", ");
            }
            
            strokeDashArrayButton.setText(dashArrayButtonText);
        }
    }

    show();
}

void StrokeFeatureMenu::showInfo(VTransformation2D* gS) {
    if (gS) {
        _gS = gS;
        
        // set stroke color
        if (ne_gs_isSetStrokeColor(_gS))
            strokeColorButton.setText(ne_gs_getStrokeColor(_gS).c_str());
        
        // set stroke width
        if (ne_gs_isSetStrokeWidth(_gS))
            strokeWidthButton.setText(QString::number(ne_gs_getStrokeWidth(gS), 'f', 1));
        
        // set stroke dash array
        if (ne_gs_isSetStrokeDashArray(gS)) {
            QString dashArrayButtonText;
            for (int i = 0; i < ne_gs_getNumStrokeDashes(_gS); ++i) {
                dashArrayButtonText.append(QString::number(ne_gs_getStrokeDash(_gS, i)));
                
                if (i < ne_gs_getNumStrokeDashes(_gS) - 1)
                    dashArrayButtonText.append(", ");
            }
            
            strokeDashArrayButton.setText(dashArrayButtonText);
        }
    }
}

void StrokeFeatureMenu::resetValues() {
    hide();
    strokeColorButton.setText("N/A");
    strokeWidthButton.setText("N/A");
    strokeDashArrayButton.setText("N/A");
    _gO = NULL;
    _style = NULL;
    _gS = NULL;
}

void StrokeFeatureMenu::changeStrokeColor() {
    QInputDialog* modificationBox =  new QInputDialog();
    modificationBox->setWindowTitle("Change Stroke Color");
    modificationBox->setLabelText("Stroke Color:");
    modificationBox->setOkButtonText("Apply");
    modificationBox->resize(300, 200);
    
    QStringList strokeColorItems;
    for (MainWindow::constGGradientIt gGIt = _mw->gGradientsBegin(); gGIt != _mw->gGradientsEnd(); ++gGIt) {
        if ((*gGIt)->isSetId())
            strokeColorItems << QString((*gGIt)->getId().c_str());
    }
    for (MainWindow::constGColorIt gCIt = _mw->gColorsBegin(); gCIt != _mw->gColorsEnd(); ++gCIt) {
        if ((*gCIt)->isSetId())
            strokeColorItems << QString((*gCIt)->getId().c_str());
    }
        
    modificationBox->setComboBoxItems(strokeColorItems);
    
    if (strokeColorButton.text() != "N/A")
        modificationBox->setTextValue(strokeColorButton.text());
    
    if (modificationBox->exec() && _mw && _mw->isSetSBMLDocument()) {
        if (!ne_ven_getColor(_mw->getSBMLDocument()->getVeneer(), -1, modificationBox->textValue().toStdString()) && !ne_ven_addNewColor(_mw->getSBMLDocument()->getVeneer(), modificationBox->textValue().toStdString()))
            _mw->getSBMLDocument()->setRenderModified(true);
        
        VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gO, _style);
        
        if (ne_gs_isSetStrokeColor(_gS)) {
            ne_gs_setStrokeColor(ne_grp_getGeometricShape(ne_stl_getGroup(style), 0), modificationBox->textValue().toStdString());
        }
        else
            ne_grp_setStrokeColor(ne_stl_getGroup(style), modificationBox->textValue().toStdString());
        
        if (style) {
            _mw->getSBMLDocument()->setRenderModified(true);
            emit styleChanged(style);
        }
    }
}

void StrokeFeatureMenu::changeStrokeWidth() {
    QInputDialog* modificationBox =  new QInputDialog();
    modificationBox->setWindowTitle("Change Stroke Width");
    modificationBox->setLabelText("Stroke Width:");
    modificationBox->setOkButtonText("Apply");
    modificationBox->resize(300, 200);
    
    QStringList strokeWidthItems;
    int strokeWidthLimit = 10;
    if (ne_go_getType(_gO) == 0)
        strokeWidthLimit = 20;
    for (int i = 0; i < strokeWidthLimit; ++i)
        strokeWidthItems << QString::number(i + 1);
    modificationBox->setComboBoxItems(strokeWidthItems);
    
    if (strokeWidthButton.text() != "N/A")
        modificationBox->setTextValue(QString::number(int(strokeWidthButton.text().toDouble())));
    
    if (modificationBox->exec() && _mw && _mw->isSetSBMLDocument()) {
        VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gO, _style);
        
        if (ne_gs_isSetStrokeWidth(_gS)) {
            ne_gs_setStrokeWidth(ne_grp_getGeometricShape(ne_stl_getGroup(style), 0), modificationBox->textValue().toInt());
        }
        else
            ne_grp_setStrokeWidth(ne_stl_getGroup(style), modificationBox->textValue().toInt());
        
        if (style) {
            _mw->getSBMLDocument()->setRenderModified(true);
            emit styleChanged(style);
        }
    }
}

void StrokeFeatureMenu::changeStrokeDashArray() {
    QDialog* modificationBox =  new QDialog();
    modificationBox->setWindowTitle("Change Stroke DashArray");
    QGridLayout modificationBoxLayout(modificationBox);
    
    QPushButton* addButton = new QPushButton("Add to DashPattern", modificationBox);
    QPushButton* removeButton = new QPushButton("Remove from DashPattern", modificationBox);
    
    QLabel* dashArrayPattern = new QLabel(modificationBox);
    dashArrayPattern->setStyleSheet("QLabel { border: 2px solid darkgray; border-radius: 4px; padding: 2px; background-color : white;}");
    if (strokeDashArrayButton.text() != "N/A")
        dashArrayPattern->setText(strokeDashArrayButton.text());
    else
        removeButton->setEnabled(false);
    
    modificationBoxLayout.addWidget(new QLabel("DashPattern:"), modificationBoxLayout.rowCount(), 0);
    modificationBoxLayout.addWidget(dashArrayPattern, modificationBoxLayout.rowCount() - 1, 1, 1, 2);
    modificationBoxLayout.addWidget(addButton, modificationBoxLayout.rowCount(), 1);
    modificationBoxLayout.addWidget(removeButton, modificationBoxLayout.rowCount() - 1, 2);
    
    QDialogButtonBox modificationBoxButtons(Qt::Horizontal, modificationBox);
    modificationBoxButtons.addButton(QDialogButtonBox::Cancel);
    modificationBoxButtons.addButton(QString("Apply"), QDialogButtonBox::AcceptRole);
    modificationBoxLayout.addWidget(&modificationBoxButtons, modificationBoxLayout.rowCount(), 1, 1, 2);
    QObject::connect(&modificationBoxButtons, SIGNAL(accepted()), modificationBox, SLOT(accept()));
    QObject::connect(&modificationBoxButtons, SIGNAL(rejected()), modificationBox, SLOT(reject()));
    
    QObject::connect(addButton, &QPushButton::clicked, modificationBox, [dashArrayPattern, removeButton] () {
        QDialog* addBox =  new QDialog();
        addBox->setWindowTitle("Add to DashArray");
        QGridLayout addBoxLayout(addBox);
        
        
        QStringList lengthItems;
        for (int i = 0; i < 20; ++i)
            lengthItems << QString::number(i + 1);
        
        QComboBox* dashLengthComboBox = new QComboBox(addBox);
        dashLengthComboBox->addItems(lengthItems);
        QComboBox* gapLengthComboBox = new QComboBox(addBox);
        gapLengthComboBox->addItems(lengthItems);
        
        addBoxLayout.addWidget(new QLabel("Dash Length:"), addBoxLayout.rowCount(), 0);
        addBoxLayout.addWidget(dashLengthComboBox, addBoxLayout.rowCount() - 1, 1, 1, 2);
        addBoxLayout.addWidget(new QLabel("Gap Length:"), addBoxLayout.rowCount(), 0);
        addBoxLayout.addWidget(gapLengthComboBox, addBoxLayout.rowCount() - 1, 1, 1, 2);
        
        QDialogButtonBox addBoxButtons(Qt::Horizontal, addBox);
        addBoxButtons.addButton(QDialogButtonBox::Cancel);
        addBoxButtons.addButton(QString("Add"), QDialogButtonBox::AcceptRole);
        addBoxLayout.addWidget(&addBoxButtons, addBoxLayout.rowCount(), 1, 1, 2);
        QObject::connect(&addBoxButtons, SIGNAL(accepted()), addBox, SLOT(accept()));
        QObject::connect(&addBoxButtons, SIGNAL(rejected()), addBox, SLOT(reject()));
        
        if (addBox->exec()) {
            if (dashArrayPattern->text().isEmpty()) {
                dashArrayPattern->setText(dashArrayPattern->text() + dashLengthComboBox->currentText() + ", " + gapLengthComboBox->currentText());
                removeButton->setEnabled(true);
            }
            else
                dashArrayPattern->setText(dashArrayPattern->text() + ", " + dashLengthComboBox->currentText() + ", " + gapLengthComboBox->currentText());
        }
    });
    
    QObject::connect(removeButton, &QPushButton::clicked, modificationBox, [dashArrayPattern, removeButton] () {
        if (!dashArrayPattern->text().isEmpty()) {
            QString dashArrayText = dashArrayPattern->text();
            dashArrayText.remove(dashArrayText.lastIndexOf(","), 3);
            
            if (dashArrayText.lastIndexOf(",") == -1) {
                dashArrayText.remove(0, 1);
                removeButton->setEnabled(false);
            }
            else
                dashArrayText.remove(dashArrayText.lastIndexOf(","), 3);
            
            dashArrayPattern->setText(dashArrayText);
        }
    });
    
    if (modificationBox->exec() && _mw && _mw->isSetSBMLDocument()) {
        std::vector<unsigned int>* dashArrayVector = new std::vector<unsigned int>();
        if (!dashArrayPattern->text().isEmpty()) {
            QStringList dashArrayItems = dashArrayPattern->text().split(",");
            while (dashArrayItems.size()) {
                dashArrayVector->push_back(dashArrayItems.first().toInt());
                dashArrayItems.removeFirst();
            }
        }
        
        VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gO, _style);
        
        if (ne_gs_isSetStrokeDashArray(_gS)) {
            ne_gs_setStrokeDashArray(ne_grp_getGeometricShape(ne_stl_getGroup(style), 0), dashArrayVector);
        }
        else
            ne_grp_setStrokeDashArray(ne_stl_getGroup(style), dashArrayVector);
        
        if (style) {
            _mw->getSBMLDocument()->setRenderModified(true);
            emit styleChanged(style);
        }
    }
}

// FillFeatureMenu

FillFeatureMenu::FillFeatureMenu(QWidget* parent, MainWindow* mw) : QGroupBox(parent) {
    _mw = mw;
    hide();
    
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setStyleSheet("QGroupBox { background-color: white; border: no-border; }");
    
    setLayout(&fillLayout);
    fillLayout.setContentsMargins(0, 0, 0, 0);
    
    featureTitleFont.setBold(true);
    featureTitleFont.setCapitalization(QFont::Capitalize);
    featureTitleFont.setPointSize(12);
    pushButtonStyleSheet = "QPushButton { border: 2px solid #8f8f91; border-radius: 4px; background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #f6f7fa, stop: 1 #dadbde); min-width: 80px; } QPushButton:pressed { background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #dadbde, stop: 1 #f6f7fa); }";
    disabledPushButtonStyleSheet = "QPushButton { border: 2px solid #8f8f91; border-radius: 4px; background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #f6f7fa, stop: 1 #dadbde); color: #bb848e;  min-width: 80px; }";
    
    // fill color label
    fillColorLabel.setText("Color:");
    fillColorLabel.setFont(featureTitleFont);
    fillLayout.addWidget(&fillColorLabel, fillLayout.rowCount(), 0);
    
    // fill color
    fillColorButton.setStyleSheet(pushButtonStyleSheet);
    fillLayout.addWidget(&fillColorButton, fillLayout.rowCount() - 1, 1);
    connect(&fillColorButton, SIGNAL(clicked()), this, SLOT(changeFillColor()));
    
    // reset values
    resetValues();
}

void FillFeatureMenu::showInfo(NGraphicalObject* gO, VGlobalStyle* style) {
    // reset values
    resetValues();

    if (gO && style) {
        _style = style;
        _gO = gO;
        
        if (!ne_go_isSetBoundingBox(_gO)) {
            fillColorButton.setStyleSheet(disabledPushButtonStyleSheet);
            fillColorButton.blockSignals(true);
            fillColorButton.setText("N/A");
        }
        else {
            fillColorButton.blockSignals(false);
            fillColorButton.setStyleSheet(pushButtonStyleSheet);
            
            // set fill color
            if (ne_grp_isSetFillColor(ne_stl_getGroup(_style)))
                fillColorButton.setText(ne_grp_getFillColor(ne_stl_getGroup(_style)).c_str());
        }
    }

    show();
}

void FillFeatureMenu::showInfo(VLineEnding* lE) {
    // reset values
    resetValues();
    
    if (lE) {
        _lE = lE;
        
        if (!ne_le_isSetBoundingBox(_lE)) {
            fillColorButton.setStyleSheet(disabledPushButtonStyleSheet);
            fillColorButton.blockSignals(true);
            fillColorButton.setText("N/A");
        }
        else {
            fillColorButton.blockSignals(false);
            fillColorButton.setStyleSheet(pushButtonStyleSheet);
            
            // set fill color
            if (ne_grp_isSetFillColor(ne_le_getGroup(_lE)))
                fillColorButton.setText(ne_grp_getFillColor(ne_le_getGroup(_lE)).c_str());
        }
    }

    show();
}

void FillFeatureMenu::showInfo(VTransformation2D* gS) {
    if (gS) {
        _gS = gS;
        
        fillColorButton.blockSignals(false);
        fillColorButton.setStyleSheet(pushButtonStyleSheet);
        
        // set fill color
        if (ne_gs_isSetFillColor(_gS))
            fillColorButton.setText(ne_gs_getFillColor(_gS).c_str());
    }
}

void FillFeatureMenu::resetValues() {
    hide();
    fillColorButton.setText("N/A");
    _gO = NULL;
    _style = NULL;
    _lE = NULL;
    _gS = NULL;
}

void FillFeatureMenu::changeFillColor() {
    QInputDialog* modificationBox =  new QInputDialog();
    modificationBox->setWindowTitle("Change Fill Color");
    modificationBox->setLabelText("Fill Color:");
    modificationBox->setOkButtonText("Apply");
    modificationBox->resize(300, 200);
    
    QStringList fillColorItems;
    for (MainWindow::constGGradientIt gGIt = _mw->gGradientsBegin(); gGIt != _mw->gGradientsEnd(); ++gGIt) {
        if ((*gGIt)->isSetId())
            fillColorItems << QString((*gGIt)->getId().c_str());
    }
    for (MainWindow::constGColorIt gCIt = _mw->gColorsBegin(); gCIt != _mw->gColorsEnd(); ++gCIt) {
        if ((*gCIt)->isSetId())
            fillColorItems << QString((*gCIt)->getId().c_str());
    }
        
    modificationBox->setComboBoxItems(fillColorItems);
    
    if (fillColorButton.text() != "N/A")
        modificationBox->setTextValue(fillColorButton.text());
    
    if (modificationBox->exec() && _mw && _mw->isSetSBMLDocument()) {
        if (!ne_ven_getColor(_mw->getSBMLDocument()->getVeneer(), -1, modificationBox->textValue().toStdString()) && !ne_ven_addNewColor(_mw->getSBMLDocument()->getVeneer(), modificationBox->textValue().toStdString()))
            _mw->getSBMLDocument()->setRenderModified(true);
        
        if (_gO && _style) {
            VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gO, _style);
            
            if (ne_gs_isSetFillColor(_gS)) {
                ne_gs_setFillColor(ne_grp_getGeometricShape(ne_stl_getGroup(style), 0), modificationBox->textValue().toStdString());
            }
            else
                ne_grp_setFillColor(ne_stl_getGroup(style), modificationBox->textValue().toStdString());
            
            if (style) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit styleChanged(style);
            }
        }
        else if (_lE) {
            VLineEnding* lineEnding = ne_ven_getLocalLineEndingFromGlobalLineEnding(_mw->getSBMLDocument()->getVeneer(), _lE);
            
            if (!ne_grp_setFillColor(ne_le_getGroup(lineEnding), modificationBox->textValue().toStdString())) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit lEndingChanged(lineEnding);
            }
        }
    }
}

// HeadFeatureMenu

HeadFeatureMenu::HeadFeatureMenu(QWidget* parent, MainWindow* mw) : QGroupBox(parent) {
    _mw = mw;
    hide();
    
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setStyleSheet("QGroupBox { background-color: white; border: no-border; }");
    
    setLayout(&headLayout);
    headLayout.setContentsMargins(0, 0, 0, 0);
    
    featureTitleFont.setBold(true);
    featureTitleFont.setCapitalization(QFont::Capitalize);
    featureTitleFont.setPointSize(12);
    pushButtonStyleSheet = "QPushButton { border: 2px solid #8f8f91; border-radius: 4px; background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #f6f7fa, stop: 1 #dadbde); min-width: 80px; } QPushButton:pressed { background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #dadbde, stop: 1 #f6f7fa); }";
    disabledPushButtonStyleSheet = "QPushButton { border: 2px solid #8f8f91; border-radius: 4px; background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #f6f7fa, stop: 1 #dadbde); color: #bb848e;  min-width: 80px; }";
    
    // start head label
    startHeadLabel.setText("Start:");
    startHeadLabel.setFont(featureTitleFont);
    headLayout.addWidget(&startHeadLabel, headLayout.rowCount(), 0);
    
    // start head
    headLayout.addWidget(&startHeadButton, headLayout.rowCount() - 1, 1);
    connect(&startHeadButton, SIGNAL(clicked()), this, SLOT(changeStartHead()));
    
    // end head label
    endHeadLabel.setText("End:");
    endHeadLabel.setFont(featureTitleFont);
    headLayout.addWidget(&endHeadLabel, headLayout.rowCount(), 0);
    
    // end head
    headLayout.addWidget(&endHeadButton, headLayout.rowCount() - 1, 1);
    connect(&endHeadButton, SIGNAL(clicked()), this, SLOT(changeEndHead()));
    
    // reset values
    resetValues();
}

void HeadFeatureMenu::showInfo(NGraphicalObject* gO, VGlobalStyle* style) {
    // reset values
    resetValues();

    if (gO && style) {
        _style = style;
        _gO = gO;
        
        if (_mw && _mw->getNumGLEndings() && (((ne_go_getType(_gO) == 2) && ne_rxn_isSetCurve((NReaction*)_gO)) || ((ne_go_getType(_gO) == 3) && ne_sr_isSetCurve((NSpeciesReference*)_gO)))) {
            startHeadButton.setStyleSheet(pushButtonStyleSheet);
            startHeadButton.blockSignals(false);
            endHeadButton.setStyleSheet(pushButtonStyleSheet);
            endHeadButton.blockSignals(false);
            
            for (MainWindow::constGLEndingIt gLEIt = _mw->gLEndingsBegin(); gLEIt != _mw->gLEndingsEnd(); ++gLEIt) {
                if ((*gLEIt)->isSetId()) {
                    if (ne_grp_isSetStartHead(ne_stl_getGroup(_style)) && stringCompare((*gLEIt)->getId(), ne_grp_getStartHead(ne_stl_getGroup(_style))))
                        startHeadButton.setText(ne_grp_getStartHead(ne_stl_getGroup(_style)).c_str());
                    
                    if (ne_grp_isSetEndHead(ne_stl_getGroup(_style)) && stringCompare((*gLEIt)->getId(), ne_grp_getEndHead(ne_stl_getGroup(_style))))
                        endHeadButton.setText(ne_grp_getEndHead(ne_stl_getGroup(_style)).c_str());
                }
            }
        }
    }

    show();
}

void HeadFeatureMenu::resetValues() {
    hide();
    
    startHeadButton.setStyleSheet(disabledPushButtonStyleSheet);
    startHeadButton.setText("N/A");
    startHeadButton.blockSignals(true);
    
    endHeadButton.setStyleSheet(disabledPushButtonStyleSheet);
    endHeadButton.setText("N/A");
    endHeadButton.blockSignals(true);
    _gO = NULL;
    _style = NULL;
}

void HeadFeatureMenu::updateValues() {
    if (_gO && _style)
        showInfo(_gO, _style);
}

void HeadFeatureMenu::changeStartHead() {
    QInputDialog* modificationBox =  new QInputDialog();
    modificationBox->setWindowTitle("Change StartHead");
    modificationBox->setLabelText("StartHead:");
    modificationBox->setOkButtonText("Apply");
    modificationBox->resize(350, 200);
    
    QStringList lineEndingItems;
    for (MainWindow::constGLEndingIt gLEIt = _mw->gLEndingsBegin(); gLEIt != _mw->gLEndingsEnd(); ++gLEIt) {
        if ((*gLEIt)->isSetId())
            lineEndingItems << QString((*gLEIt)->getId().c_str());
    }
    lineEndingItems << "none";
    modificationBox->setComboBoxItems(lineEndingItems);
    
    int lineEndingIndex = 0;
    for (lineEndingIndex = 0; lineEndingIndex < lineEndingItems.length(); ++lineEndingIndex) {
        if (startHeadButton.text() == lineEndingItems.at(lineEndingIndex))
            break;
    }
    if (lineEndingIndex == lineEndingItems.length())
        --lineEndingIndex;
    modificationBox->setTextValue(lineEndingItems.at(lineEndingIndex));
    
    if (modificationBox->exec() && _mw && _mw->isSetSBMLDocument()) {
        VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gO, _style);
        
        if(!ne_grp_setStartHead(ne_stl_getGroup(style), modificationBox->textValue().toStdString())) {
            _mw->getSBMLDocument()->setRenderModified(true);
            emit styleChanged(style);
        }
    }
}

void HeadFeatureMenu::changeEndHead() {
    QInputDialog* modificationBox =  new QInputDialog();
    modificationBox->setWindowTitle("Change EndHead");
    modificationBox->setLabelText("EndHead:");
    modificationBox->setOkButtonText("Apply");
    modificationBox->resize(350, 200);
    
    QStringList lineEndingItems;
    for (MainWindow::constGLEndingIt gLEIt = _mw->gLEndingsBegin(); gLEIt != _mw->gLEndingsEnd(); ++gLEIt) {
        if ((*gLEIt)->isSetId())
            lineEndingItems << QString((*gLEIt)->getId().c_str());
    }
    lineEndingItems << "none";
    modificationBox->setComboBoxItems(lineEndingItems);
    
    int lineEndingIndex = 0;
    for (lineEndingIndex = 0; lineEndingIndex < lineEndingItems.length(); ++lineEndingIndex) {
        if (endHeadButton.text() == lineEndingItems.at(lineEndingIndex))
            break;
    }
    if (lineEndingIndex == lineEndingItems.length())
        --lineEndingIndex;
    modificationBox->setTextValue(lineEndingItems.at(lineEndingIndex));
    
    if (modificationBox->exec() && _mw && _mw->isSetSBMLDocument()) {
        VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gO, _style);
        
        if(!ne_grp_setEndHead(ne_stl_getGroup(_style), modificationBox->textValue().toStdString())) {
            _mw->getSBMLDocument()->setRenderModified(true);
            emit styleChanged(style);
        }
    }
}

// TextFeatureMenu

TextFeatureMenu::TextFeatureMenu(QWidget* parent, MainWindow* mw) : QGroupBox(parent) {
    _mw = mw;
    hide();
    
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setStyleSheet("QGroupBox { background-color: white; border: no-border; }");
    
    setLayout(&textLayout);
    textLayout.setContentsMargins(0, 0, 0, 0);
    
    featureTitleFont.setBold(true);
    featureTitleFont.setCapitalization(QFont::Capitalize);
    featureTitleFont.setPointSize(12);
    pushButtonStyleSheet = "QPushButton { border: 2px solid #8f8f91; border-radius: 4px; background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #f6f7fa, stop: 1 #dadbde); min-width: 80px; } QPushButton:pressed { background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #dadbde, stop: 1 #f6f7fa); }";
    
    // font size label
    fontSizeLabel.setText("Font Size:");
    fontSizeLabel.setFont(featureTitleFont);
    textLayout.addWidget(&fontSizeLabel, textLayout.rowCount(), 0);
    
    // font size
    fontSizeButton.setStyleSheet(pushButtonStyleSheet);
    textLayout.addWidget(&fontSizeButton, textLayout.rowCount() - 1, 1);
    connect(&fontSizeButton, SIGNAL(clicked()), this, SLOT(changeFontSize()));
    
    // font family label
    fontFamilyLabel.setText("Font Family:");
    fontFamilyLabel.setFont(featureTitleFont);
    textLayout.addWidget(&fontFamilyLabel, textLayout.rowCount(), 0);
    
    // font family
    fontFamilyButton.setStyleSheet(pushButtonStyleSheet);
    textLayout.addWidget(&fontFamilyButton, textLayout.rowCount() - 1, 1);
    connect(&fontFamilyButton, SIGNAL(clicked()), this, SLOT(changeFontFamily()));
    
    // font weight label
    fontWeightLabel.setText("Font Weight:");
    fontWeightLabel.setFont(featureTitleFont);
    textLayout.addWidget(&fontWeightLabel, textLayout.rowCount(), 0);
    
    // font weight
    fontWeightButton.setStyleSheet(pushButtonStyleSheet);
    textLayout.addWidget(&fontWeightButton, textLayout.rowCount() - 1, 1);
    connect(&fontWeightButton, SIGNAL(clicked()), this, SLOT(changeFontWeight()));
    
    // font style label
    fontStyleLabel.setText("Font Style:");
    fontStyleLabel.setFont(featureTitleFont);
    textLayout.addWidget(&fontStyleLabel, textLayout.rowCount(), 0);
    
    // font style
    fontStyleButton.setStyleSheet(pushButtonStyleSheet);
    textLayout.addWidget(&fontStyleButton, textLayout.rowCount() - 1, 1);
    connect(&fontStyleButton, SIGNAL(clicked()), this, SLOT(changeFontStyle()));
    
    // text stroke color label
    fontColorLabel.setText("Font Color:");
    fontColorLabel.setFont(featureTitleFont);
    textLayout.addWidget(&fontColorLabel, textLayout.rowCount(), 0);
    
    // text stroke color
    fontColorButton.setStyleSheet(pushButtonStyleSheet);
    textLayout.addWidget(&fontColorButton, textLayout.rowCount() - 1, 1);
    connect(&fontColorButton, SIGNAL(clicked()), this, SLOT(changeFontColor()));
    
    // textanchor label
    textAnchorLabel.setText("TextAnchor:");
    textAnchorLabel.setFont(featureTitleFont);
    textLayout.addWidget(&textAnchorLabel, textLayout.rowCount(), 0);
    
    // textanchor
    textAnchorButton.setStyleSheet(pushButtonStyleSheet);
    textLayout.addWidget(&textAnchorButton, textLayout.rowCount() - 1, 1);
    connect(&textAnchorButton, SIGNAL(clicked()), this, SLOT(changeTextAnchor()));
    
    // vtextanchor label
    vTextAnchorLabel.setText("VTextAnchor:");
    vTextAnchorLabel.setFont(featureTitleFont);
    textLayout.addWidget(&vTextAnchorLabel, textLayout.rowCount(), 0);
    
    // vtextanchor
    vTextAnchorButton.setStyleSheet(pushButtonStyleSheet);
    textLayout.addWidget(&vTextAnchorButton, textLayout.rowCount() - 1, 1);
    connect(&vTextAnchorButton, SIGNAL(clicked()), this, SLOT(changeVTextAnchor()));
    
    // reset values
    resetValues();
}

void TextFeatureMenu::showInfo(GraphicalText* gT, NGraphicalObject* gO) {
    // reset values
    resetValues();
    
    if (gT && gT->isSetStyle() && gT->isSetPlainText()) {
        gText = gT;
        _gO = gO;
        
        // set font size
        if (ne_grp_isSetFontSize(ne_stl_getGroup(gText->getStyle()))) {
            RAVector* fSize = ne_grp_getFontSize(ne_stl_getGroup(gText->getStyle()));
            if (fSize) {
                QString sFSize;
                sFSize += QString::number(fSize->a(), 'f', 1);
                sFSize += " + ";
                sFSize += QString::number(fSize->r());
                sFSize += "%";
                
                fontSizeButton.setText(sFSize);
            }
        }
        
        // set font family
        if (ne_grp_isSetFontFamily(ne_stl_getGroup(gText->getStyle())))
            fontFamilyButton.setText(ne_grp_getFontFamily(ne_stl_getGroup(gText->getStyle())).c_str());
        
        // set font weight
        if (ne_grp_isSetFontWeight(ne_stl_getGroup(gText->getStyle())))
            fontWeightButton.setText(ne_grp_getFontWeight(ne_stl_getGroup(gText->getStyle())).c_str());
        
        // set font style
        if (ne_grp_isSetFontStyle(ne_stl_getGroup(gText->getStyle())))
            fontStyleButton.setText(ne_grp_getFontStyle(ne_stl_getGroup(gText->getStyle())).c_str());
        
        // set font color
        if (ne_grp_isSetStrokeColor(ne_stl_getGroup(gText->getStyle())))
            fontColorButton.setText(ne_grp_getStrokeColor(ne_stl_getGroup(gText->getStyle())).c_str());
        
        // set textanchor
        if (ne_grp_isSetHTextAnchor(ne_stl_getGroup(gText->getStyle())))
            textAnchorButton.setText(ne_grp_getHTextAnchor(ne_stl_getGroup(gText->getStyle())).c_str());
        
        // set vtextanchor
        if (ne_grp_isSetVTextAnchor(ne_stl_getGroup(gText->getStyle())))
            vTextAnchorButton.setText(ne_grp_getVTextAnchor(ne_stl_getGroup(gText->getStyle())).c_str());
        
        show();
    }
}

void TextFeatureMenu::resetValues() {
    hide();
    fontSizeButton.setText("N/A");
    fontFamilyButton.setText("N/A");
    fontWeightButton.setText("N/A");
    fontStyleButton.setText("N/A");
    fontColorButton.setText("N/A");
    textAnchorButton.setText("N/A");
    vTextAnchorButton.setText("N/A");
    gText = NULL;
    _gO = NULL;
}

void TextFeatureMenu::changeFontSize() {
    QDialog* modificationBox =  new QDialog();
    modificationBox->setWindowTitle("Change Font Size of the Text");
    QGridLayout modificationBoxLayout(modificationBox);
    modificationBoxLayout.addWidget(new QLabel("Font Size"), modificationBoxLayout.rowCount() - 1, 0);
    
    QComboBox* absoluteValueComboBox = new QComboBox(modificationBox);
    QComboBox* relativeValueComboBox = new QComboBox(modificationBox);
    QString currentAbsoluteValue, currentRelativeValue;
    if (fontSizeButton.text() != "N/A") {
        currentAbsoluteValue = fontSizeButton.text().split("+").at(0);
        currentRelativeValue = fontSizeButton.text().split("+").at(1).split("%").at(0);
    }
    if (currentAbsoluteValue.toDouble() != 0)
        relativeValueComboBox->setEnabled(false);

    QStringList fontSizeAbsoluteItems;
    QStringList fontSizeRelativeItems;
    int i = 0;
    int index = 0;
    int absoluteValueIndex = 0;
    while (i <= 72) {
        fontSizeAbsoluteItems << QString::number(i);
        if (i == currentAbsoluteValue.toDouble())
            absoluteValueIndex = index;
        
        if (i == 0)
            i += 5;
        else if (i < 12)
            ++i;
        else if (i < 28)
            i += 2;
        else if (i < 36)
            i += 8;
        else if (i < 48)
            i += 12;
        else
            i += 24;
        
        ++index;
    }
    
    index = 0;
    int relativeValueIndex = 0;
    for (int i = 0; i <= 100; i += 5) {
        fontSizeRelativeItems << QString::number(i);
        if (i == currentRelativeValue.toDouble())
            relativeValueIndex = index;
        
        ++index;
    }
    
    absoluteValueComboBox->addItems(fontSizeAbsoluteItems);
    relativeValueComboBox->addItems(fontSizeRelativeItems);
    absoluteValueComboBox->setCurrentIndex(absoluteValueIndex);
    relativeValueComboBox->setCurrentIndex(relativeValueIndex);
    
    QObject::connect(absoluteValueComboBox, QOverload<int>::of(&QComboBox::activated), relativeValueComboBox, [relativeValueComboBox] (const int& index) { (index == 0) ? relativeValueComboBox->setEnabled(true) : relativeValueComboBox->setEnabled(false), relativeValueComboBox->setCurrentIndex(0); });
    
    modificationBoxLayout.addWidget(new QLabel("Absolute Value:"), modificationBoxLayout.rowCount(), 0);
    modificationBoxLayout.addWidget(absoluteValueComboBox, modificationBoxLayout.rowCount() - 1, 1);
    modificationBoxLayout.addWidget(new QLabel("Relative Value:"), modificationBoxLayout.rowCount(), 0);
    modificationBoxLayout.addWidget(relativeValueComboBox, modificationBoxLayout.rowCount() - 1, 1);
    modificationBoxLayout.addWidget(new QLabel("%"), modificationBoxLayout.rowCount() - 1, 2);
    
    QDialogButtonBox modificationBoxButtons(Qt::Horizontal, modificationBox);
    modificationBoxButtons.addButton(QDialogButtonBox::Cancel);
    modificationBoxButtons.addButton(QString("Apply"), QDialogButtonBox::AcceptRole);
    modificationBoxLayout.addWidget(&modificationBoxButtons, modificationBoxLayout.rowCount(), 1, 1, 2);
    QObject::connect(&modificationBoxButtons, SIGNAL(accepted()), modificationBox, SLOT(accept()));
    QObject::connect(&modificationBoxButtons, SIGNAL(rejected()), modificationBox, SLOT(reject()));
    
    if (modificationBox->exec() == QDialog::Accepted && _mw && _mw->isSetSBMLDocument() && gText && gText->isSetGTextItem()) {
        
        RAVector* fontSize = new RAVector(absoluteValueComboBox->currentText().toInt(), relativeValueComboBox->currentText().toInt());
        
        NText* text = NULL;
        if (gText->isSetText())
            text = gText->getText();
        else
            text = ne_net_getText(_mw->getSBMLDocument()->getNetwork(), _gO);
        
        VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gO, gText->getStyle());
        
        if (!ne_grp_setFontSize(ne_stl_getGroup(style), fontSize)) {
            ne_gtxt_setPlainText(text, gText->getPlainText());
            gText->setText(text);
            gText->setStyle(style);
            
            _mw->getSBMLDocument()->setRenderModified(true);
            emit textChanged();
        }
    }
}

void TextFeatureMenu::changeFontFamily() {
    QInputDialog* modificationBox =  new QInputDialog();
    modificationBox->setWindowTitle("Change Font Family of the Text");
    modificationBox->setLabelText("Font Family:");
    modificationBox->setOkButtonText("Apply");
    modificationBox->resize(300, 200);
    
    QStringList fontFamilyItems;
    for (int i = 0; i < NUM_FontFamily; ++i) {
        if (!fontFamilyToStr(FontFamily(i)).empty())
            fontFamilyItems << QString(fontFamilyToStr(FontFamily(i)).c_str());
    }
        
    modificationBox->setComboBoxItems(fontFamilyItems);
    
    if (fontFamilyButton.text() != "N/A")
        modificationBox->setTextValue(fontFamilyButton.text());
    
    if (modificationBox->exec() && _mw && _mw->isSetSBMLDocument() && gText && gText->isSetGTextItem() && !modificationBox->textValue().isEmpty()) {
        
        NText* text = NULL;
        if (gText->isSetText())
            text = gText->getText();
        else
            text = ne_net_getText(_mw->getSBMLDocument()->getNetwork(), _gO);
        
        VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gO, gText->getStyle());
        
        if (!ne_grp_setFontFamily(ne_stl_getGroup(style), modificationBox->textValue().toStdString())) {
            ne_gtxt_setPlainText(text, gText->getPlainText());
            gText->setText(text);
            gText->setStyle(style);
            
            _mw->getSBMLDocument()->setRenderModified(true);
            emit textChanged();
        }
    }
}

void TextFeatureMenu::changeFontWeight() {
    QInputDialog* modificationBox =  new QInputDialog();
    modificationBox->setWindowTitle("Change Font Weight of the Text");
    modificationBox->setLabelText("Font Weight:");
    modificationBox->setOkButtonText("Apply");
    modificationBox->resize(300, 200);
    
    QStringList fontWeightItems;
    for (int i = 0; i < NUM_FontWeight; ++i) {
        if (!fontWeightToStr(FontWeight(i)).empty())
            fontWeightItems << QString(fontWeightToStr(FontWeight(i)).c_str());
    }
        
    modificationBox->setComboBoxItems(fontWeightItems);
    
    if (fontWeightButton.text() != "N/A")
        modificationBox->setTextValue(fontWeightButton.text());
    
    if (modificationBox->exec() && _mw && _mw->isSetSBMLDocument() && gText && gText->isSetGTextItem() && !modificationBox->textValue().isEmpty()) {
        
        NText* text = NULL;
        if (gText->isSetText())
            text = gText->getText();
        else
            text = ne_net_getText(_mw->getSBMLDocument()->getNetwork(), _gO);
        
        VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gO, gText->getStyle());
        
        if (!ne_grp_setFontWeight(ne_stl_getGroup(style), modificationBox->textValue().toStdString())) {
            ne_gtxt_setPlainText(text, gText->getPlainText());
            gText->setText(text);
            gText->setStyle(style);
            
            _mw->getSBMLDocument()->setRenderModified(true);
            emit textChanged();
        }
    }
}

void TextFeatureMenu::changeFontStyle() {
    QInputDialog* modificationBox =  new QInputDialog();
    modificationBox->setWindowTitle("Change Font Style of the Text");
    modificationBox->setLabelText("Font Style:");
    modificationBox->setOkButtonText("Apply");
    modificationBox->resize(300, 200);
    
    QStringList fontStyleItems;
    for (int i = 0; i < NUM_FontStyle; ++i) {
        if (!fontStyleToStr(FontStyle(i)).empty())
            fontStyleItems << QString(fontStyleToStr(FontStyle(i)).c_str());
    }
        
    modificationBox->setComboBoxItems(fontStyleItems);
    
    if (fontStyleButton.text() != "N/A")
        modificationBox->setTextValue(fontStyleButton.text());
    
    if (modificationBox->exec() && _mw && _mw->isSetSBMLDocument() && gText && gText->isSetGTextItem() && !modificationBox->textValue().isEmpty()) {
        
        NText* text = NULL;
        if (gText->isSetText())
            text = gText->getText();
        else
            text = ne_net_getText(_mw->getSBMLDocument()->getNetwork(), _gO);
        
        VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gO, gText->getStyle());
        
        if (!ne_grp_setFontStyle(ne_stl_getGroup(style), modificationBox->textValue().toStdString())) {
            ne_gtxt_setPlainText(text, gText->getPlainText());
            gText->setText(text);
            gText->setStyle(style);
            
            _mw->getSBMLDocument()->setRenderModified(true);
            emit textChanged();
        }
    }
}

void TextFeatureMenu::changeFontColor() {
    QInputDialog* modificationBox =  new QInputDialog();
    modificationBox->setWindowTitle("Change Font Color of the Text");
    modificationBox->setLabelText("Font Color:");
    modificationBox->setOkButtonText("Apply");
    modificationBox->resize(300, 200);
    
    QStringList strokeColorItems;
    for (MainWindow::constGGradientIt gGIt = _mw->gGradientsBegin(); gGIt != _mw->gGradientsEnd(); ++gGIt) {
        if ((*gGIt)->isSetId())
            strokeColorItems << QString((*gGIt)->getId().c_str());
    }
    for (MainWindow::constGColorIt gCIt = _mw->gColorsBegin(); gCIt != _mw->gColorsEnd(); ++gCIt) {
        if ((*gCIt)->isSetId())
            strokeColorItems << QString((*gCIt)->getId().c_str());
    }
        
    modificationBox->setComboBoxItems(strokeColorItems);
    
    if (fontColorButton.text() != "N/A")
        modificationBox->setTextValue(fontColorButton.text());
    
    if (modificationBox->exec() && _mw && _mw->isSetSBMLDocument() && _mw->getSBMLDocument()->getVeneer() && gText && gText->isSetGTextItem() && !modificationBox->textValue().isEmpty()) {
        
        if (!ne_ven_getColor(_mw->getSBMLDocument()->getVeneer(), -1, modificationBox->textValue().toStdString()) && !ne_ven_addNewColor(_mw->getSBMLDocument()->getVeneer(), modificationBox->textValue().toStdString()))
            _mw->getSBMLDocument()->setRenderModified(true);
        
        NText* text = NULL;
        if (gText->isSetText())
            text = gText->getText();
        else
            text = ne_net_getText(_mw->getSBMLDocument()->getNetwork(), _gO);
        
        VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gO, gText->getStyle());
        
        if (!ne_grp_setStrokeColor(ne_stl_getGroup(style), modificationBox->textValue().toStdString())) {
            ne_gtxt_setPlainText(text, gText->getPlainText());
            gText->setText(text);
            gText->setStyle(style);
            
            _mw->getSBMLDocument()->setRenderModified(true);
            emit textChanged();
        }
    }
}

void TextFeatureMenu::changeTextAnchor() {
    QInputDialog* modificationBox =  new QInputDialog();
    modificationBox->setWindowTitle("Change Horizontal TextAnchor of the Text");
    modificationBox->setLabelText("Horizontal TextAnchor:");
    modificationBox->setOkButtonText("Apply");
    modificationBox->resize(300, 200);
    
    QStringList textAnchorItems;
    for (int i = 0; i < NUM_HTextAnchor; ++i) {
        if (!hTextAnchorToStr(HTextAnchor(i)).empty())
            textAnchorItems << QString(hTextAnchorToStr(HTextAnchor(i)).c_str());
    }
        
    modificationBox->setComboBoxItems(textAnchorItems);
    
    if (textAnchorButton.text() != "N/A")
        modificationBox->setTextValue(textAnchorButton.text());
    
    if (modificationBox->exec() && _mw && _mw->isSetSBMLDocument() && gText && gText->isSetGTextItem() && !modificationBox->textValue().isEmpty()) {
        
        NText* text = NULL;
        if (gText->isSetText())
            text = gText->getText();
        else
            text = ne_net_getText(_mw->getSBMLDocument()->getNetwork(), _gO);
        
        VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gO, gText->getStyle());
        
        if (!ne_grp_setHTextAnchor(ne_stl_getGroup(style), modificationBox->textValue().toStdString())) {
            ne_gtxt_setPlainText(text, gText->getPlainText());
            gText->setText(text);
            gText->setStyle(style);
            
            _mw->getSBMLDocument()->setRenderModified(true);
            emit textChanged();
        }
    }
}

void TextFeatureMenu::changeVTextAnchor() {
    QInputDialog* modificationBox =  new QInputDialog();
    modificationBox->setWindowTitle("Change Vertical TextAnchor of the Text");
    modificationBox->setLabelText("Vertical TextAnchor:");
    modificationBox->setOkButtonText("Apply");
    modificationBox->resize(300, 200);
    
    QStringList vTextAnchorItems;
    for (int i = 0; i < NUM_VTextAnchor; ++i) {
        if (!vTextAnchorToStr(VTextAnchor(i)).empty())
            vTextAnchorItems << QString(vTextAnchorToStr(VTextAnchor(i)).c_str());
    }
        
    modificationBox->setComboBoxItems(vTextAnchorItems);
    
    if (vTextAnchorButton.text() != "N/A")
        modificationBox->setTextValue(vTextAnchorButton.text());
    
    if (modificationBox->exec() && _mw && _mw->isSetSBMLDocument() && gText && gText->isSetGTextItem() && !modificationBox->textValue().isEmpty()) {
        
        NText* text = NULL;
        if (gText->isSetText())
            text = gText->getText();
        else
            text = ne_net_getText(_mw->getSBMLDocument()->getNetwork(), _gO);
        
        VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gO, gText->getStyle());
        
        if (!ne_grp_setVTextAnchor(ne_stl_getGroup(style), modificationBox->textValue().toStdString())) {
            ne_gtxt_setPlainText(text, gText->getPlainText());
            gText->setText(text);
            gText->setStyle(style);
            
            _mw->getSBMLDocument()->setRenderModified(true);
            emit textChanged();
        }
    }
}

// GeometricShapeFeatureMenuElement

GeometricShapeFeatureMenuElement::GeometricShapeFeatureMenuElement(QWidget* parent, MainWindow* mw) : QGroupBox(parent) {
    _mw = mw;
    hide();
    
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setStyleSheet("QGroupBox { background-color: white; border: no-border; }");
    
    setLayout(&gShapeLayout);
    gShapeLayout.setContentsMargins(0, 0, 0, 0);
    
    featureTitleFont.setBold(true);
    featureTitleFont.setCapitalization(QFont::Capitalize);
    featureTitleFont.setPointSize(12);
    pushButtonStyleSheet = "QPushButton { border: 2px solid #8f8f91; border-radius: 4px; background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #f6f7fa, stop: 1 #dadbde); min-width: 80px; } QPushButton:pressed { background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #dadbde, stop: 1 #f6f7fa); }";
    disabledPushButtonStyleSheet = "QPushButton { border: 2px solid #8f8f91; border-radius: 4px; background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #f6f7fa, stop: 1 #dadbde); color: #bb848e;  min-width: 80px; }";
}

// RectangleGeometricShapeFeatureMenu

RectangleGeometricShapeFeatureMenu::RectangleGeometricShapeFeatureMenu(QWidget* parent, MainWindow* mw) : GeometricShapeFeatureMenuElement(parent, mw) {
    
    // position label
    positionLabel.setText("Position:");
    positionLabel.setFont(featureTitleFont);
    gShapeLayout.addWidget(&positionLabel, gShapeLayout.rowCount(), 0, 1, 2);
    
    // x label
    positionXLabel.setText("X:");
    positionXLabel.setFont(featureTitleFont);
    gShapeLayout.addWidget(&positionXLabel, gShapeLayout.rowCount(), 1, Qt::AlignLeft);
    
    // x
    positionXButton.setStyleSheet(pushButtonStyleSheet);
    gShapeLayout.addWidget(&positionXButton, gShapeLayout.rowCount() - 1, 2, Qt::AlignRight);
    connect(&positionXButton, SIGNAL(clicked()), this, SLOT(changePositionX()));
    
    // y label
    positionYLabel.setText("Y:");
    positionYLabel.setFont(featureTitleFont);
    gShapeLayout.addWidget(&positionYLabel, gShapeLayout.rowCount(), 1, Qt::AlignLeft);
    
    // y
    positionYButton.setStyleSheet(pushButtonStyleSheet);
    gShapeLayout.addWidget(&positionYButton, gShapeLayout.rowCount() - 1, 2, Qt::AlignRight);
    connect(&positionYButton, SIGNAL(clicked()), this, SLOT(changePositionY()));
    
    // dimension label
    dimensionLabel.setText("Dimensions:");
    dimensionLabel.setFont(featureTitleFont);
    gShapeLayout.addWidget(&dimensionLabel, gShapeLayout.rowCount(), 0, 1, 2);
    
    // width label
    dimensionWidthLabel.setText("Width:");
    dimensionWidthLabel.setFont(featureTitleFont);
    gShapeLayout.addWidget(&dimensionWidthLabel, gShapeLayout.rowCount(), 1, Qt::AlignLeft);
    
    // width
    dimensionWidthButton.setStyleSheet(pushButtonStyleSheet);
    gShapeLayout.addWidget(&dimensionWidthButton, gShapeLayout.rowCount() - 1, 2, Qt::AlignRight);
    connect(&dimensionWidthButton, SIGNAL(clicked()), this, SLOT(changeDimensionWidth()));
    
    // height label
    dimensionHeightLabel.setText("Height:");
    dimensionHeightLabel.setFont(featureTitleFont);
    gShapeLayout.addWidget(&dimensionHeightLabel, gShapeLayout.rowCount(), 1, Qt::AlignLeft);
    
    // height
    dimensionHeightButton.setStyleSheet(pushButtonStyleSheet);
    gShapeLayout.addWidget(&dimensionHeightButton, gShapeLayout.rowCount() - 1, 2, Qt::AlignRight);
    connect(&dimensionHeightButton, SIGNAL(clicked()), this, SLOT(changeDimensionHeight()));
    
    // ratio label
    dimensionRatioLabel.setText("Ratio:");
    dimensionRatioLabel.setFont(featureTitleFont);
    gShapeLayout.addWidget(&dimensionRatioLabel, gShapeLayout.rowCount(), 1, Qt::AlignLeft);
    
    // ratio
    dimensionRatioButton.setStyleSheet(pushButtonStyleSheet);
    gShapeLayout.addWidget(&dimensionRatioButton, gShapeLayout.rowCount() - 1, 2, Qt::AlignRight);
    connect(&dimensionRatioButton, SIGNAL(clicked()), this, SLOT(changeDimensionRatio()));
    
    // corner curvature label
    cornerCurvatureLabel.setText("Corner Curvature:");
    cornerCurvatureLabel.setFont(featureTitleFont);
    gShapeLayout.addWidget(&cornerCurvatureLabel, gShapeLayout.rowCount(), 0, 1, 2);
    
    // corner curvature rx label
    cornerCurvatureRXLabel.setText("RX:");
    cornerCurvatureRXLabel.setFont(featureTitleFont);
    gShapeLayout.addWidget(&cornerCurvatureRXLabel, gShapeLayout.rowCount(), 1, Qt::AlignLeft);
    
    // corner curvature rx
    cornerCurvatureRXButton.setStyleSheet(pushButtonStyleSheet);
    gShapeLayout.addWidget(&cornerCurvatureRXButton, gShapeLayout.rowCount() - 1, 2, Qt::AlignRight);
    connect(&cornerCurvatureRXButton, SIGNAL(clicked()), this, SLOT(changeCornerCurvatureRX()));
    
    // corner curvature ry label
    cornerCurvatureRYLabel.setText("RY:");
    cornerCurvatureRYLabel.setFont(featureTitleFont);
    gShapeLayout.addWidget(&cornerCurvatureRYLabel, gShapeLayout.rowCount(), 1, Qt::AlignLeft);
    
    // corner curvature ry
    cornerCurvatureRYButton.setStyleSheet(pushButtonStyleSheet);
    gShapeLayout.addWidget(&cornerCurvatureRYButton, gShapeLayout.rowCount() - 1, 2, Qt::AlignRight);
    connect(&cornerCurvatureRYButton, SIGNAL(clicked()), this, SLOT(changeCornerCurvatureRY()));
    
    // reset values
    resetValues();
}

void RectangleGeometricShapeFeatureMenu::showInfo(NGraphicalObject* gO, VGlobalStyle* style) {
    // reset values
    resetValues();
    
    if (ne_gs_getShape(ne_grp_getGeometricShape(ne_stl_getGroup(style), 0)) == 3) {
        _style = style;
        _gO = gO;
        getInfoFromRectangleShape((VRectangle*)ne_grp_getGeometricShape(ne_stl_getGroup(_style), 0));
    }

    show();
}

void RectangleGeometricShapeFeatureMenu::showInfo(VLineEnding* lE) {
    // reset values
    resetValues();
    
    if (ne_gs_getShape(ne_grp_getGeometricShape(ne_le_getGroup(lE), 0)) == 3) {
        _lE = lE;
        getInfoFromRectangleShape((VRectangle*)ne_grp_getGeometricShape(ne_le_getGroup(_lE), 0));
    }

    show();
}

void RectangleGeometricShapeFeatureMenu::resetValues() {
    positionXButton.setText("N/A");
    positionYButton.setText("N/A");
    dimensionWidthButton.setText("N/A");
    dimensionHeightButton.setText("N/A");
    dimensionRatioButton.setText("N/A");
    cornerCurvatureRXButton.setText("N/A");
    cornerCurvatureRYButton.setText("N/A");
    _gO = NULL;
    _style = NULL;
    _lE = NULL;
}

void RectangleGeometricShapeFeatureMenu::getInfoFromRectangleShape(VRectangle* rectangle) {
    if (rectangle) {
        RAVector* point = NULL;
        QString sPoint;
        
        // set x
        if (ne_rec_isSetPositionX(rectangle)) {
            point = ne_rec_getPositionX(rectangle);
            if (point) {
                sPoint = QString::number(point->a(), 'f', 1);
                sPoint += " + ";
                sPoint += QString::number(point->r());
                sPoint += "%";
                positionXButton.setText(sPoint);
            }
        }
        
        // set y
        if (ne_rec_isSetPositionY(rectangle)) {
            point = ne_rec_getPositionY(rectangle);
            if (point) {
                sPoint = QString::number(point->a(), 'f', 1);
                sPoint += " + ";
                sPoint += QString::number(point->r());
                sPoint += "%";
                positionYButton.setText(sPoint);
            }
        }
        
        // set width
        if (ne_rec_isSetDimensionWidth(rectangle)) {
            point = ne_rec_getDimensionWidth(rectangle);
            if (point) {
                sPoint = QString::number(point->a(), 'f', 1);
                sPoint += " + ";
                sPoint += QString::number(point->r());
                sPoint += "%";
                dimensionWidthButton.setText(sPoint);
            }
        }
        
        // set height
        if (ne_rec_isSetDimensionHeight(rectangle)) {
            point = ne_rec_getDimensionHeight(rectangle);
            if (point) {
                sPoint = QString::number(point->a(), 'f', 1);
                sPoint += " + ";
                sPoint += QString::number(point->r());
                sPoint += "%";
                dimensionHeightButton.setText(sPoint);
            }
        }
        
        // set ratio
        if (ne_rec_isSetRatio(rectangle))
            dimensionRatioButton.setText(QString::number(ne_rec_getRatio(rectangle)));
        
        // set rx
        if (ne_rec_isSetCornerCurvatureRX(rectangle)) {
            point = ne_rec_getCornerCurvatureRX(rectangle);
            if (point) {
                sPoint = QString::number(point->a(), 'f', 1);
                sPoint += " + ";
                sPoint += QString::number(point->r());
                sPoint += "%";
                cornerCurvatureRXButton.setText(sPoint);
            }
        }
        
        // set ry
        if (ne_rec_isSetCornerCurvatureRY(rectangle)) {
            point = ne_rec_getCornerCurvatureRY(rectangle);
            if (point) {
                sPoint = QString::number(point->a(), 'f', 1);
                sPoint += " + ";
                sPoint += QString::number(point->r());
                sPoint += "%";
                cornerCurvatureRYButton.setText(sPoint);
            }
        }
    }
}

void RectangleGeometricShapeFeatureMenu::changePositionX() {
    QDialog* modificationBox =  new QDialog();
    modificationBox->setWindowTitle("Change Position X of the Rectanlge Shape");
    QGridLayout modificationBoxLayout(modificationBox);
    modificationBoxLayout.addWidget(new QLabel("Position X"), modificationBoxLayout.rowCount() - 1, 0);
    
    QDoubleSpinBox* absoluteValueSpinBox = new QDoubleSpinBox(modificationBox);
    QSpinBox* relativeValueSpinBox = new QSpinBox(modificationBox);
    QString currentAbsoluteValue, currentRelativeValue;
    if (positionXButton.text() != "N/A") {
        currentAbsoluteValue = positionXButton.text().split("+").at(0);
        currentRelativeValue = positionXButton.text().split("+").at(1).split("%").at(0);
    }
    
    absoluteValueSpinBox->setValue(currentAbsoluteValue.toDouble());
    absoluteValueSpinBox->setDecimals(1);
    if (ne_go_isSetBoundingBox(_gO))
        absoluteValueSpinBox->setRange(_mw->getScene()->sceneRect().x() - ne_bb_getX(ne_go_getBoundingBox(_gO)), _mw->getScene()->sceneRect().x() + _mw->getScene()->sceneRect().width() - (ne_bb_getX(ne_go_getBoundingBox(_gO)) + ne_bb_getWidth(ne_go_getBoundingBox(_gO))));
    else if (ne_le_isSetBoundingBox(_lE))
        absoluteValueSpinBox->setRange(_mw->getScene()->sceneRect().x() - ne_bb_getX(ne_le_getBoundingBox(_lE)), _mw->getScene()->sceneRect().x() + _mw->getScene()->sceneRect().width() - (ne_bb_getX(ne_le_getBoundingBox(_lE)) + ne_bb_getWidth(ne_le_getBoundingBox(_lE))));
    relativeValueSpinBox->setValue(currentRelativeValue.toInt());
    relativeValueSpinBox->setRange(0, 100);
    
    modificationBoxLayout.addWidget(new QLabel("Absolute Value:"), modificationBoxLayout.rowCount(), 0);
    modificationBoxLayout.addWidget(absoluteValueSpinBox, modificationBoxLayout.rowCount() - 1, 1);
    modificationBoxLayout.addWidget(new QLabel("Relative Value:"), modificationBoxLayout.rowCount(), 0);
    modificationBoxLayout.addWidget(relativeValueSpinBox, modificationBoxLayout.rowCount() - 1, 1);
    modificationBoxLayout.addWidget(new QLabel("%"), modificationBoxLayout.rowCount() - 1, 2);
    
    QDialogButtonBox modificationBoxButtons(Qt::Horizontal, modificationBox);
    modificationBoxButtons.addButton(QDialogButtonBox::Cancel);
    modificationBoxButtons.addButton(QString("Apply"), QDialogButtonBox::AcceptRole);
    modificationBoxLayout.addWidget(&modificationBoxButtons, modificationBoxLayout.rowCount(), 1, 1, 2);
    QObject::connect(&modificationBoxButtons, SIGNAL(accepted()), modificationBox, SLOT(accept()));
    QObject::connect(&modificationBoxButtons, SIGNAL(rejected()), modificationBox, SLOT(reject()));
    
    if (modificationBox->exec() == QDialog::Accepted && _mw && _mw->isSetSBMLDocument()) {
        RAVector* pX = new RAVector(absoluteValueSpinBox->value(), relativeValueSpinBox->value());
        
        if (_gO && _style) {
            VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gO, _style);
            
            if (!ne_rec_setPositionX(ne_grp_getGeometricShape(ne_stl_getGroup(style), 0), pX)) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit styleChanged(style);
            }
        }
        else if (_lE) {
            VLineEnding* lineEnding = ne_ven_getLocalLineEndingFromGlobalLineEnding(_mw->getSBMLDocument()->getVeneer(), _lE);
            
            if (!ne_rec_setPositionX(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), 0), pX)) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit lEndingChanged(lineEnding);
            }
        }
    }
}

void RectangleGeometricShapeFeatureMenu::changePositionY() {
    QDialog* modificationBox =  new QDialog();
    modificationBox->setWindowTitle("Change Position Y of the Rectanlge Shape");
    QGridLayout modificationBoxLayout(modificationBox);
    modificationBoxLayout.addWidget(new QLabel("Position Y"), modificationBoxLayout.rowCount() - 1, 0);
    
    QDoubleSpinBox* absoluteValueSpinBox = new QDoubleSpinBox(modificationBox);
    QSpinBox* relativeValueSpinBox = new QSpinBox(modificationBox);
    QString currentAbsoluteValue, currentRelativeValue;
    if (positionYButton.text() != "N/A") {
        currentAbsoluteValue = positionYButton.text().split("+").at(0);
        currentRelativeValue = positionYButton.text().split("+").at(1).split("%").at(0);
    }
    absoluteValueSpinBox->setValue(currentAbsoluteValue.toDouble());
    absoluteValueSpinBox->setDecimals(1);
    if (ne_go_isSetBoundingBox(_gO))
        absoluteValueSpinBox->setRange(_mw->getScene()->sceneRect().y() - ne_bb_getY(ne_go_getBoundingBox(_gO)), _mw->getScene()->sceneRect().y() + _mw->getScene()->sceneRect().height() - (ne_bb_getY(ne_go_getBoundingBox(_gO)) + ne_bb_getHeight(ne_go_getBoundingBox(_gO))));
    else if (ne_le_isSetBoundingBox(_lE))
        absoluteValueSpinBox->setRange(_mw->getScene()->sceneRect().y() - ne_bb_getY(ne_le_getBoundingBox(_lE)), _mw->getScene()->sceneRect().y() + _mw->getScene()->sceneRect().height() - (ne_bb_getY(ne_le_getBoundingBox(_lE)) + ne_bb_getHeight(ne_le_getBoundingBox(_lE))));
    relativeValueSpinBox->setValue(currentRelativeValue.toInt());
    relativeValueSpinBox->setRange(0, 100);
    
    modificationBoxLayout.addWidget(new QLabel("Absolute Value:"), modificationBoxLayout.rowCount(), 0);
    modificationBoxLayout.addWidget(absoluteValueSpinBox, modificationBoxLayout.rowCount() - 1, 1);
    modificationBoxLayout.addWidget(new QLabel("Relative Value:"), modificationBoxLayout.rowCount(), 0);
    modificationBoxLayout.addWidget(relativeValueSpinBox, modificationBoxLayout.rowCount() - 1, 1);
    modificationBoxLayout.addWidget(new QLabel("%"), modificationBoxLayout.rowCount() - 1, 2);
    
    QDialogButtonBox modificationBoxButtons(Qt::Horizontal, modificationBox);
    modificationBoxButtons.addButton(QDialogButtonBox::Cancel);
    modificationBoxButtons.addButton(QString("Apply"), QDialogButtonBox::AcceptRole);
    modificationBoxLayout.addWidget(&modificationBoxButtons, modificationBoxLayout.rowCount(), 1, 1, 2);
    QObject::connect(&modificationBoxButtons, SIGNAL(accepted()), modificationBox, SLOT(accept()));
    QObject::connect(&modificationBoxButtons, SIGNAL(rejected()), modificationBox, SLOT(reject()));
    
    if (modificationBox->exec() == QDialog::Accepted && _mw && _mw->isSetSBMLDocument()) {
        RAVector* pY = new RAVector(absoluteValueSpinBox->value(), relativeValueSpinBox->value());
        
        if (_gO && _style) {
            VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gO, _style);
            
            if (!ne_rec_setPositionY(ne_grp_getGeometricShape(ne_stl_getGroup(style), 0), pY)) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit styleChanged(style);
            }
        }
        else if (_lE) {
            VLineEnding* lineEnding = ne_ven_getLocalLineEndingFromGlobalLineEnding(_mw->getSBMLDocument()->getVeneer(), _lE);
            
            if (!ne_rec_setPositionY(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), 0), pY)) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit lEndingChanged(lineEnding);
            }
        }
    }
}

void RectangleGeometricShapeFeatureMenu::changeDimensionWidth() {
    QDialog* modificationBox =  new QDialog();
    modificationBox->setWindowTitle("Change Dimension Width of the Rectanlge Shape");
    QGridLayout modificationBoxLayout(modificationBox);
    modificationBoxLayout.addWidget(new QLabel("Dimension Width"), modificationBoxLayout.rowCount() - 1, 0);
    modificationBox->resize(350, 150);
    
    QDoubleSpinBox* absoluteValueSpinBox = new QDoubleSpinBox(modificationBox);
    QSpinBox* relativeValueSpinBox = new QSpinBox(modificationBox);
    QString currentAbsoluteValue, currentRelativeValue;
    if (dimensionWidthButton.text() != "N/A") {
        currentAbsoluteValue = dimensionWidthButton.text().split("+").at(0);
        currentRelativeValue = dimensionWidthButton.text().split("+").at(1).split("%").at(0);
    }
    absoluteValueSpinBox->setValue(currentAbsoluteValue.toDouble());
    absoluteValueSpinBox->setDecimals(1);
    if (ne_go_isSetBoundingBox(_gO))
        absoluteValueSpinBox->setRange(0.0, _mw->getScene()->sceneRect().x() + _mw->getScene()->sceneRect().width() - ne_bb_getX(ne_go_getBoundingBox(_gO)));
    else if (ne_le_isSetBoundingBox(_lE))
        absoluteValueSpinBox->setRange(0.0, _mw->getScene()->sceneRect().x() + _mw->getScene()->sceneRect().width() - ne_bb_getX(ne_le_getBoundingBox(_lE)));
    relativeValueSpinBox->setValue(currentRelativeValue.toInt());
    relativeValueSpinBox->setRange(0, 100);
    
    modificationBoxLayout.addWidget(new QLabel("Absolute Value:"), modificationBoxLayout.rowCount(), 0);
    modificationBoxLayout.addWidget(absoluteValueSpinBox, modificationBoxLayout.rowCount() - 1, 1);
    modificationBoxLayout.addWidget(new QLabel("Relative Value:"), modificationBoxLayout.rowCount(), 0);
    modificationBoxLayout.addWidget(relativeValueSpinBox, modificationBoxLayout.rowCount() - 1, 1);
    modificationBoxLayout.addWidget(new QLabel("%"), modificationBoxLayout.rowCount() - 1, 2);
    
    QDialogButtonBox modificationBoxButtons(Qt::Horizontal, modificationBox);
    modificationBoxButtons.addButton(QDialogButtonBox::Cancel);
    modificationBoxButtons.addButton(QString("Apply"), QDialogButtonBox::AcceptRole);
    modificationBoxLayout.addWidget(&modificationBoxButtons, modificationBoxLayout.rowCount(), 1, 1, 2);
    QObject::connect(&modificationBoxButtons, SIGNAL(accepted()), modificationBox, SLOT(accept()));
    QObject::connect(&modificationBoxButtons, SIGNAL(rejected()), modificationBox, SLOT(reject()));
    
    if (modificationBox->exec() == QDialog::Accepted && _mw && _mw->isSetSBMLDocument()) {
        RAVector* pWidth = new RAVector(absoluteValueSpinBox->value(), relativeValueSpinBox->value());
        
        if (_gO && _style) {
            VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gO, _style);
            
            if (!ne_rec_setDimensionWidth(ne_grp_getGeometricShape(ne_stl_getGroup(style), 0), pWidth)) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit styleChanged(style);
            }
        }
        else if (_lE) {
            VLineEnding* lineEnding = ne_ven_getLocalLineEndingFromGlobalLineEnding(_mw->getSBMLDocument()->getVeneer(), _lE);
            
            if (!ne_rec_setDimensionWidth(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), 0), pWidth)) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit lEndingChanged(lineEnding);
            }
        }
    }
}

void RectangleGeometricShapeFeatureMenu::changeDimensionHeight() {
    QDialog* modificationBox =  new QDialog();
    modificationBox->setWindowTitle("Change Dimension Height of the Rectanlge Shape");
    QGridLayout modificationBoxLayout(modificationBox);
    modificationBoxLayout.addWidget(new QLabel("Dimension Height"), modificationBoxLayout.rowCount() - 1, 0);
    modificationBox->resize(350, 150);
    
    QDoubleSpinBox* absoluteValueSpinBox = new QDoubleSpinBox(modificationBox);
    QSpinBox* relativeValueSpinBox = new QSpinBox(modificationBox);
    QString currentAbsoluteValue, currentRelativeValue;
    if (dimensionHeightButton.text() != "N/A") {
        currentAbsoluteValue = dimensionHeightButton.text().split("+").at(0);
        currentRelativeValue = dimensionHeightButton.text().split("+").at(1).split("%").at(0);
    }
    absoluteValueSpinBox->setValue(currentAbsoluteValue.toDouble());
    absoluteValueSpinBox->setDecimals(1);
    if (ne_go_isSetBoundingBox(_gO))
        absoluteValueSpinBox->setRange(0.0, _mw->getScene()->sceneRect().y() + _mw->getScene()->sceneRect().height() - ne_bb_getY(ne_go_getBoundingBox(_gO)));
    else if (ne_le_isSetBoundingBox(_lE))
        absoluteValueSpinBox->setRange(0.0, _mw->getScene()->sceneRect().y() + _mw->getScene()->sceneRect().height() - ne_bb_getY(ne_le_getBoundingBox(_lE)));
    relativeValueSpinBox->setValue(currentRelativeValue.toInt());
    relativeValueSpinBox->setRange(0, 100);
    
    modificationBoxLayout.addWidget(new QLabel("Absolute Value:"), modificationBoxLayout.rowCount(), 0);
    modificationBoxLayout.addWidget(absoluteValueSpinBox, modificationBoxLayout.rowCount() - 1, 1);
    modificationBoxLayout.addWidget(new QLabel("Relative Value:"), modificationBoxLayout.rowCount(), 0);
    modificationBoxLayout.addWidget(relativeValueSpinBox, modificationBoxLayout.rowCount() - 1, 1);
    modificationBoxLayout.addWidget(new QLabel("%"), modificationBoxLayout.rowCount() - 1, 2);
    
    QDialogButtonBox modificationBoxButtons(Qt::Horizontal, modificationBox);
    modificationBoxButtons.addButton(QDialogButtonBox::Cancel);
    modificationBoxButtons.addButton(QString("Apply"), QDialogButtonBox::AcceptRole);
    modificationBoxLayout.addWidget(&modificationBoxButtons, modificationBoxLayout.rowCount(), 1, 1, 2);
    QObject::connect(&modificationBoxButtons, SIGNAL(accepted()), modificationBox, SLOT(accept()));
    QObject::connect(&modificationBoxButtons, SIGNAL(rejected()), modificationBox, SLOT(reject()));
    
    if (modificationBox->exec() == QDialog::Accepted && _mw && _mw->isSetSBMLDocument()) {
        RAVector* pHeight = new RAVector(absoluteValueSpinBox->value(), relativeValueSpinBox->value());
        
        if (_gO && _style) {
            VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gO, _style);
            
            if (!ne_rec_setDimensionHeight(ne_grp_getGeometricShape(ne_stl_getGroup(style), 0), pHeight)) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit styleChanged(style);
            }
        }
        else if (_lE) {
            VLineEnding* lineEnding = ne_ven_getLocalLineEndingFromGlobalLineEnding(_mw->getSBMLDocument()->getVeneer(), _lE);
            
            if (!ne_rec_setDimensionHeight(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), 0), pHeight)) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit lEndingChanged(lineEnding);
            }
        }
    }
}

void RectangleGeometricShapeFeatureMenu::changeDimensionRatio() {
    QInputDialog* modificationBox =  new QInputDialog();
    modificationBox->setWindowTitle("Change Dimension Ratio of the Rectanlge Shape");
    modificationBox->setLabelText("Dimension Ratio:");
    modificationBox->setOkButtonText("Apply");
    modificationBox->resize(300, 200);
    
    QStringList dimensionRatioItems;
    for (double i = 0.0; i <= 5.0;) {
        dimensionRatioItems << QString::number(i);
        if (i < 1)
            i += 0.2;
        else
            i += 0.5;
    }
    modificationBox->setComboBoxItems(dimensionRatioItems);
    
    if (dimensionRatioButton.text() != "N/A")
        modificationBox->setTextValue(dimensionRatioButton.text());
    
    if (modificationBox->exec() && _mw->isSetSBMLDocument()) {
        if (_gO && _style) {
            VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gO, _style);
            
            if (!ne_rec_setRatio(ne_grp_getGeometricShape(ne_stl_getGroup(style), 0), modificationBox->textValue().toDouble())) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit styleChanged(style);
            }
        }
        else if (_lE) {
            VLineEnding* lineEnding = ne_ven_getLocalLineEndingFromGlobalLineEnding(_mw->getSBMLDocument()->getVeneer(), _lE);
            
            if (!ne_rec_setRatio(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), 0), modificationBox->textValue().toDouble())) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit lEndingChanged(lineEnding);
            }
        }
    }
}

void RectangleGeometricShapeFeatureMenu::changeCornerCurvatureRX() {
    QDialog* modificationBox =  new QDialog();
    modificationBox->setWindowTitle("Change Corner Curvature Radius X of the Rectangle Shape");
    QGridLayout modificationBoxLayout(modificationBox);
    modificationBoxLayout.addWidget(new QLabel("Curvature Radius X"), modificationBoxLayout.rowCount() - 1, 0);
    modificationBox->resize(400, 150);
    
    QDoubleSpinBox* absoluteValueSpinBox = new QDoubleSpinBox(modificationBox);
    QSpinBox* relativeValueSpinBox = new QSpinBox(modificationBox);
    QString currentAbsoluteValue, currentRelativeValue;
    if (cornerCurvatureRXButton.text() != "N/A") {
        currentAbsoluteValue = cornerCurvatureRXButton.text().split("+").at(0);
        currentRelativeValue = cornerCurvatureRXButton.text().split("+").at(1).split("%").at(0);
    }
    absoluteValueSpinBox->setValue(currentAbsoluteValue.toDouble());
    absoluteValueSpinBox->setDecimals(1);
    if (ne_go_isSetBoundingBox(_gO))
        absoluteValueSpinBox->setRange(0.0, ne_bb_getWidth(ne_go_getBoundingBox(_gO)));
    else if (ne_le_isSetBoundingBox(_lE))
        absoluteValueSpinBox->setRange(0.0, ne_bb_getWidth(ne_le_getBoundingBox(_lE)));
    relativeValueSpinBox->setValue(currentRelativeValue.toInt());
    relativeValueSpinBox->setRange(0, 100);
    
    modificationBoxLayout.addWidget(new QLabel("Absolute Value:"), modificationBoxLayout.rowCount(), 0);
    modificationBoxLayout.addWidget(absoluteValueSpinBox, modificationBoxLayout.rowCount() - 1, 1);
    modificationBoxLayout.addWidget(new QLabel("Relative Value:"), modificationBoxLayout.rowCount(), 0);
    modificationBoxLayout.addWidget(relativeValueSpinBox, modificationBoxLayout.rowCount() - 1, 1);
    modificationBoxLayout.addWidget(new QLabel("%"), modificationBoxLayout.rowCount() - 1, 2);
    
    QDialogButtonBox modificationBoxButtons(Qt::Horizontal, modificationBox);
    modificationBoxButtons.addButton(QDialogButtonBox::Cancel);
    modificationBoxButtons.addButton(QString("Apply"), QDialogButtonBox::AcceptRole);
    modificationBoxLayout.addWidget(&modificationBoxButtons, modificationBoxLayout.rowCount(), 1, 1, 2);
    QObject::connect(&modificationBoxButtons, SIGNAL(accepted()), modificationBox, SLOT(accept()));
    QObject::connect(&modificationBoxButtons, SIGNAL(rejected()), modificationBox, SLOT(reject()));
    
    if (modificationBox->exec() == QDialog::Accepted && _mw && _mw->isSetSBMLDocument()) {
        RAVector* pRX = new RAVector(absoluteValueSpinBox->value(), relativeValueSpinBox->value());
        
        if (_gO && _style) {
            VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gO, _style);
            
            if (!ne_rec_setCornerCurvatureRX(ne_grp_getGeometricShape(ne_stl_getGroup(style), 0), pRX)) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit styleChanged(style);
            }
        }
        else if (_lE) {
            VLineEnding* lineEnding = ne_ven_getLocalLineEndingFromGlobalLineEnding(_mw->getSBMLDocument()->getVeneer(), _lE);
            
            if (!ne_rec_setCornerCurvatureRX(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), 0), pRX)) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit lEndingChanged(lineEnding);
            }
        }
    }
}

void RectangleGeometricShapeFeatureMenu::changeCornerCurvatureRY() {
    QDialog* modificationBox =  new QDialog();
    modificationBox->setWindowTitle("Change Corner Curvature Radius Y of the Rectangle Shape");
    QGridLayout modificationBoxLayout(modificationBox);
    modificationBoxLayout.addWidget(new QLabel("Curvature Radius Y"), modificationBoxLayout.rowCount() - 1, 0);
    modificationBox->resize(400, 150);
    
    QDoubleSpinBox* absoluteValueSpinBox = new QDoubleSpinBox(modificationBox);
    QSpinBox* relativeValueSpinBox = new QSpinBox(modificationBox);
    QString currentAbsoluteValue, currentRelativeValue;
    if (cornerCurvatureRYButton.text() != "N/A") {
        currentAbsoluteValue = cornerCurvatureRYButton.text().split("+").at(0);
        currentRelativeValue = cornerCurvatureRYButton.text().split("+").at(1).split("%").at(0);
    }
    absoluteValueSpinBox->setValue(currentAbsoluteValue.toDouble());
    absoluteValueSpinBox->setDecimals(1);
    if (ne_go_isSetBoundingBox(_gO))
        absoluteValueSpinBox->setRange(0.0, ne_bb_getHeight(ne_go_getBoundingBox(_gO)));
    else if (ne_le_isSetBoundingBox(_lE))
        absoluteValueSpinBox->setRange(0.0, ne_bb_getHeight(ne_le_getBoundingBox(_lE)));
    relativeValueSpinBox->setValue(currentRelativeValue.toInt());
    relativeValueSpinBox->setRange(0, 100);
    
    modificationBoxLayout.addWidget(new QLabel("Absolute Value:"), modificationBoxLayout.rowCount(), 0);
    modificationBoxLayout.addWidget(absoluteValueSpinBox, modificationBoxLayout.rowCount() - 1, 1);
    modificationBoxLayout.addWidget(new QLabel("Relative Value:"), modificationBoxLayout.rowCount(), 0);
    modificationBoxLayout.addWidget(relativeValueSpinBox, modificationBoxLayout.rowCount() - 1, 1);
    modificationBoxLayout.addWidget(new QLabel("%"), modificationBoxLayout.rowCount() - 1, 2);
    
    QDialogButtonBox modificationBoxButtons(Qt::Horizontal, modificationBox);
    modificationBoxButtons.addButton(QDialogButtonBox::Cancel);
    modificationBoxButtons.addButton(QString("Apply"), QDialogButtonBox::AcceptRole);
    modificationBoxLayout.addWidget(&modificationBoxButtons, modificationBoxLayout.rowCount(), 1, 1, 2);
    QObject::connect(&modificationBoxButtons, SIGNAL(accepted()), modificationBox, SLOT(accept()));
    QObject::connect(&modificationBoxButtons, SIGNAL(rejected()), modificationBox, SLOT(reject()));
    
    if (modificationBox->exec() == QDialog::Accepted && _mw && _mw->isSetSBMLDocument()) {
        RAVector* pRY = new RAVector(absoluteValueSpinBox->value(), relativeValueSpinBox->value());
        
        if (_gO && _style) {
            VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gO, _style);
            
            if (!ne_rec_setCornerCurvatureRY(ne_grp_getGeometricShape(ne_stl_getGroup(style), 0), pRY)) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit styleChanged(style);
            }
        }
        else if (_lE) {
            VLineEnding* lineEnding = ne_ven_getLocalLineEndingFromGlobalLineEnding(_mw->getSBMLDocument()->getVeneer(), _lE);
            
            if (!ne_rec_setCornerCurvatureRY(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), 0), pRY)) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit lEndingChanged(lineEnding);
            }
        }
    }
}

// ImageGeometricShapeFeatureMenu

ImageGeometricShapeFeatureMenu::ImageGeometricShapeFeatureMenu(QWidget* parent, MainWindow* mw) : GeometricShapeFeatureMenuElement(parent, mw) {
    
    // position label
    positionLabel.setText("Position:");
    positionLabel.setFont(featureTitleFont);
    gShapeLayout.addWidget(&positionLabel, gShapeLayout.rowCount(), 0, 1, 2);
    
    // x label
    positionXLabel.setText("X:");
    positionXLabel.setFont(featureTitleFont);
    gShapeLayout.addWidget(&positionXLabel, gShapeLayout.rowCount(), 1, Qt::AlignLeft);
    
    // x
    positionXButton.setStyleSheet(pushButtonStyleSheet);
    gShapeLayout.addWidget(&positionXButton, gShapeLayout.rowCount() - 1, 2, Qt::AlignRight);
    connect(&positionXButton, SIGNAL(clicked()), this, SLOT(changePositionX()));
    
    // y label
    positionYLabel.setText("Y:");
    positionYLabel.setFont(featureTitleFont);
    gShapeLayout.addWidget(&positionYLabel, gShapeLayout.rowCount(), 1, Qt::AlignLeft);
    
    // y
    positionYButton.setStyleSheet(pushButtonStyleSheet);
    gShapeLayout.addWidget(&positionYButton, gShapeLayout.rowCount() - 1, 2, Qt::AlignRight);
    connect(&positionYButton, SIGNAL(clicked()), this, SLOT(changePositionY()));
    
    // dimension label
    dimensionLabel.setText("Dimensions:");
    dimensionLabel.setFont(featureTitleFont);
    gShapeLayout.addWidget(&dimensionLabel, gShapeLayout.rowCount(), 0, 1, 2);
    
    // width label
    dimensionWidthLabel.setText("Width:");
    dimensionWidthLabel.setFont(featureTitleFont);
    gShapeLayout.addWidget(&dimensionWidthLabel, gShapeLayout.rowCount(), 1, Qt::AlignLeft);
    
    // width
    dimensionWidthButton.setStyleSheet(pushButtonStyleSheet);
    gShapeLayout.addWidget(&dimensionWidthButton, gShapeLayout.rowCount() - 1, 2, Qt::AlignRight);
    connect(&dimensionWidthButton, SIGNAL(clicked()), this, SLOT(changeDimensionWidth()));
    
    // height label
    dimensionHeightLabel.setText("Height:");
    dimensionHeightLabel.setFont(featureTitleFont);
    gShapeLayout.addWidget(&dimensionHeightLabel, gShapeLayout.rowCount(), 1, Qt::AlignLeft);
    
    // height
    dimensionHeightButton.setStyleSheet(pushButtonStyleSheet);
    gShapeLayout.addWidget(&dimensionHeightButton, gShapeLayout.rowCount() - 1, 2, Qt::AlignRight);
    connect(&dimensionHeightButton, SIGNAL(clicked()), this, SLOT(changeDimensionHeight()));
    
    // Path label
    pathLabel.setText("Path:");
    pathLabel.setFont(featureTitleFont);
    gShapeLayout.addWidget(&pathLabel, gShapeLayout.rowCount(), 0, 1, 2);
    
    // href label
    hrefLabel.setText("href:");
    hrefLabel.setFont(featureTitleFont);
    gShapeLayout.addWidget(&hrefLabel, gShapeLayout.rowCount(), 1, Qt::AlignLeft);
    
    // href
    hrefButton.setStyleSheet(pushButtonStyleSheet);
    gShapeLayout.addWidget(&hrefButton, gShapeLayout.rowCount() - 1, 2, Qt::AlignRight);
    connect(&hrefButton, SIGNAL(clicked()), this, SLOT(changeHref()));
    
    // reset values
    resetValues();
}

void ImageGeometricShapeFeatureMenu::showInfo(NGraphicalObject* gO, VGlobalStyle* style) {
    // reset values
    resetValues();
    
    if (ne_gs_getShape(ne_grp_getGeometricShape(ne_stl_getGroup(style), 0)) == 0) {
        _style = style;
        _gO = gO;
        getInfoFromImageShape((VImage*)ne_grp_getGeometricShape(ne_stl_getGroup(_style), 0));
    }
    
    show();
}

void ImageGeometricShapeFeatureMenu::showInfo(VLineEnding* lE) {
    // reset values
    resetValues();
    
    if (ne_gs_getShape(ne_grp_getGeometricShape(ne_le_getGroup(lE), 0)) == 0) {
        _lE = lE;
        getInfoFromImageShape((VImage*)ne_grp_getGeometricShape(ne_le_getGroup(_lE), 0));
    }

    show();
}

void ImageGeometricShapeFeatureMenu::resetValues() {
    positionXButton.setText("N/A");
    positionYButton.setText("N/A");
    dimensionWidthButton.setText("N/A");
    dimensionHeightButton.setText("N/A");
    hrefButton.setText("N/A");
    _gO = NULL;
    _style = NULL;
    _lE = NULL;
}

void ImageGeometricShapeFeatureMenu::getInfoFromImageShape(VImage* image) {
    if (image) {
        RAVector* point = NULL;
        QString sPoint;
        
        // set x
        if (ne_img_isSetPositionX(image)) {
            point = ne_img_getPositionX(image);
            if (point) {
                sPoint = QString::number(point->a(), 'f', 1);
                sPoint += " + ";
                sPoint += QString::number(point->r());
                sPoint += "%";
                positionXButton.setText(sPoint);
            }
        }
        
        // set y
        if (ne_img_isSetPositionY(image)) {
            point = ne_img_getPositionY(image);
            if (point) {
                sPoint = QString::number(point->a(), 'f', 1);
                sPoint += " + ";
                sPoint += QString::number(point->r());
                sPoint += "%";
                positionYButton.setText(sPoint);
            }
        }
        
        // set width
        if (ne_img_isSetDimensionWidth(image)) {
            point = ne_img_getDimensionWidth(image);
            if (point) {
                sPoint = QString::number(point->a(), 'f', 1);
                sPoint += " + ";
                sPoint += QString::number(point->r());
                sPoint += "%";
                dimensionWidthButton.setText(sPoint);
            }
        }
        
        // set height
        if (ne_img_isSetDimensionHeight(image)) {
            point = ne_img_getDimensionHeight(image);
            if (point) {
                sPoint = QString::number(point->a(), 'f', 1);
                sPoint += " + ";
                sPoint += QString::number(point->r());
                sPoint += "%";
                dimensionHeightButton.setText(sPoint);
            }
        }
        
        // set href
        if (ne_img_isSetHref(image)) {
            href = QString(ne_img_getHref(image).c_str());
            hrefButton.setText(QFileInfo(href).fileName());
        }
    }
}

void ImageGeometricShapeFeatureMenu::changePositionX() {
    QDialog* modificationBox =  new QDialog();
    modificationBox->setWindowTitle("Change Position X of the Image Shape");
    QGridLayout modificationBoxLayout(modificationBox);
    modificationBoxLayout.addWidget(new QLabel("Position X"), modificationBoxLayout.rowCount() - 1, 0);
    
    QDoubleSpinBox* absoluteValueSpinBox = new QDoubleSpinBox(modificationBox);
    QSpinBox* relativeValueSpinBox = new QSpinBox(modificationBox);
    QString currentAbsoluteValue, currentRelativeValue;
    if (positionXButton.text() != "N/A") {
        currentAbsoluteValue = positionXButton.text().split("+").at(0);
        currentRelativeValue = positionXButton.text().split("+").at(1).split("%").at(0);
    }
    
    absoluteValueSpinBox->setValue(currentAbsoluteValue.toDouble());
    absoluteValueSpinBox->setDecimals(1);
    if (ne_go_isSetBoundingBox(_gO))
        absoluteValueSpinBox->setRange(_mw->getScene()->sceneRect().x() - ne_bb_getX(ne_go_getBoundingBox(_gO)), _mw->getScene()->sceneRect().x() + _mw->getScene()->sceneRect().width() - (ne_bb_getX(ne_go_getBoundingBox(_gO)) + ne_bb_getWidth(ne_go_getBoundingBox(_gO))));
    else if (_lE && _lE->isSetBox())
        absoluteValueSpinBox->setRange(_mw->getScene()->sceneRect().x() - ne_bb_getX(ne_le_getBoundingBox(_lE)), _mw->getScene()->sceneRect().x() + _mw->getScene()->sceneRect().width() - (ne_bb_getX(ne_le_getBoundingBox(_lE)) + ne_bb_getWidth(ne_le_getBoundingBox(_lE))));
    relativeValueSpinBox->setValue(currentRelativeValue.toInt());
    relativeValueSpinBox->setRange(0, 100);
    
    modificationBoxLayout.addWidget(new QLabel("Absolute Value:"), modificationBoxLayout.rowCount(), 0);
    modificationBoxLayout.addWidget(absoluteValueSpinBox, modificationBoxLayout.rowCount() - 1, 1);
    modificationBoxLayout.addWidget(new QLabel("Relative Value:"), modificationBoxLayout.rowCount(), 0);
    modificationBoxLayout.addWidget(relativeValueSpinBox, modificationBoxLayout.rowCount() - 1, 1);
    modificationBoxLayout.addWidget(new QLabel("%"), modificationBoxLayout.rowCount() - 1, 2);
    
    QDialogButtonBox modificationBoxButtons(Qt::Horizontal, modificationBox);
    modificationBoxButtons.addButton(QDialogButtonBox::Cancel);
    modificationBoxButtons.addButton(QString("Apply"), QDialogButtonBox::AcceptRole);
    modificationBoxLayout.addWidget(&modificationBoxButtons, modificationBoxLayout.rowCount(), 1, 1, 2);
    QObject::connect(&modificationBoxButtons, SIGNAL(accepted()), modificationBox, SLOT(accept()));
    QObject::connect(&modificationBoxButtons, SIGNAL(rejected()), modificationBox, SLOT(reject()));
    
    if (modificationBox->exec() == QDialog::Accepted && _mw && _mw->isSetSBMLDocument()) {
        RAVector* pX = new RAVector(absoluteValueSpinBox->value(), relativeValueSpinBox->value());
        
        if (_gO && _style) {
            VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gO, _style);
            
            if (!ne_img_setPositionX(ne_grp_getGeometricShape(ne_stl_getGroup(style), 0), pX)) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit styleChanged(style);
            }
        }
        else if (_lE) {
            VLineEnding* lineEnding = ne_ven_getLocalLineEndingFromGlobalLineEnding(_mw->getSBMLDocument()->getVeneer(), _lE);
            
            if (!ne_img_setPositionX(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), 0), pX)) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit lEndingChanged(lineEnding);
            }
        }
    }
}

void ImageGeometricShapeFeatureMenu::changePositionY() {
    QDialog* modificationBox =  new QDialog();
    modificationBox->setWindowTitle("Change Position Y of the Image Shape");
    QGridLayout modificationBoxLayout(modificationBox);
    modificationBoxLayout.addWidget(new QLabel("Position Y"), modificationBoxLayout.rowCount() - 1, 0);
    
    QDoubleSpinBox* absoluteValueSpinBox = new QDoubleSpinBox(modificationBox);
    QSpinBox* relativeValueSpinBox = new QSpinBox(modificationBox);
    QString currentAbsoluteValue, currentRelativeValue;
    if (positionYButton.text() != "N/A") {
        currentAbsoluteValue = positionYButton.text().split("+").at(0);
        currentRelativeValue = positionYButton.text().split("+").at(1).split("%").at(0);
    }
    absoluteValueSpinBox->setValue(currentAbsoluteValue.toDouble());
    absoluteValueSpinBox->setDecimals(1);
    if (ne_go_isSetBoundingBox(_gO))
        absoluteValueSpinBox->setRange(_mw->getScene()->sceneRect().y() - ne_bb_getY(ne_go_getBoundingBox(_gO)), _mw->getScene()->sceneRect().y() + _mw->getScene()->sceneRect().height() - (ne_bb_getY(ne_go_getBoundingBox(_gO)) + ne_bb_getHeight(ne_go_getBoundingBox(_gO))));
    else if (_lE && _lE->isSetBox())
        absoluteValueSpinBox->setRange(_mw->getScene()->sceneRect().y() - ne_bb_getY(ne_le_getBoundingBox(_lE)), _mw->getScene()->sceneRect().y() + _mw->getScene()->sceneRect().height() - (ne_bb_getY(ne_le_getBoundingBox(_lE)) + ne_bb_getHeight(ne_le_getBoundingBox(_lE))));
    relativeValueSpinBox->setValue(currentRelativeValue.toInt());
    relativeValueSpinBox->setRange(0, 100);
    
    modificationBoxLayout.addWidget(new QLabel("Absolute Value:"), modificationBoxLayout.rowCount(), 0);
    modificationBoxLayout.addWidget(absoluteValueSpinBox, modificationBoxLayout.rowCount() - 1, 1);
    modificationBoxLayout.addWidget(new QLabel("Relative Value:"), modificationBoxLayout.rowCount(), 0);
    modificationBoxLayout.addWidget(relativeValueSpinBox, modificationBoxLayout.rowCount() - 1, 1);
    modificationBoxLayout.addWidget(new QLabel("%"), modificationBoxLayout.rowCount() - 1, 2);
    
    QDialogButtonBox modificationBoxButtons(Qt::Horizontal, modificationBox);
    modificationBoxButtons.addButton(QDialogButtonBox::Cancel);
    modificationBoxButtons.addButton(QString("Apply"), QDialogButtonBox::AcceptRole);
    modificationBoxLayout.addWidget(&modificationBoxButtons, modificationBoxLayout.rowCount(), 1, 1, 2);
    QObject::connect(&modificationBoxButtons, SIGNAL(accepted()), modificationBox, SLOT(accept()));
    QObject::connect(&modificationBoxButtons, SIGNAL(rejected()), modificationBox, SLOT(reject()));
    
    if (modificationBox->exec() == QDialog::Accepted && _mw && _mw->isSetSBMLDocument()) {
        RAVector* pY = new RAVector(absoluteValueSpinBox->value(), relativeValueSpinBox->value());
        
        if (_gO && _style) {
            VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gO, _style);
            
            if (!ne_img_setPositionY(ne_grp_getGeometricShape(ne_stl_getGroup(style), 0), pY)) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit styleChanged(style);
            }
        }
        else if (_lE) {
            VLineEnding* lineEnding = ne_ven_getLocalLineEndingFromGlobalLineEnding(_mw->getSBMLDocument()->getVeneer(), _lE);
            
            if (!ne_img_setPositionY(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), 0), pY)) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit lEndingChanged(lineEnding);
            }
        }
    }
}

void ImageGeometricShapeFeatureMenu::changeDimensionWidth() {
    QDialog* modificationBox =  new QDialog();
    modificationBox->setWindowTitle("Change Dimension Width of the Image Shape");
    QGridLayout modificationBoxLayout(modificationBox);
    modificationBoxLayout.addWidget(new QLabel("Dimension Width"), modificationBoxLayout.rowCount() - 1, 0);
    modificationBox->resize(350, 150);
    
    QDoubleSpinBox* absoluteValueSpinBox = new QDoubleSpinBox(modificationBox);
    QSpinBox* relativeValueSpinBox = new QSpinBox(modificationBox);
    QString currentAbsoluteValue, currentRelativeValue;
    if (dimensionWidthButton.text() != "N/A") {
        currentAbsoluteValue = dimensionWidthButton.text().split("+").at(0);
        currentRelativeValue = dimensionWidthButton.text().split("+").at(1).split("%").at(0);
    }
    absoluteValueSpinBox->setValue(currentAbsoluteValue.toDouble());
    absoluteValueSpinBox->setDecimals(1);
    if (ne_go_isSetBoundingBox(_gO))
        absoluteValueSpinBox->setRange(0.0, _mw->getScene()->sceneRect().x() + _mw->getScene()->sceneRect().width() - ne_bb_getX(ne_go_getBoundingBox(_gO)));
    else if (ne_le_isSetBoundingBox(_lE))
        absoluteValueSpinBox->setRange(0.0, _mw->getScene()->sceneRect().x() + _mw->getScene()->sceneRect().width() - ne_bb_getX(ne_le_getBoundingBox(_lE)));
    relativeValueSpinBox->setValue(currentRelativeValue.toInt());
    relativeValueSpinBox->setRange(0, 100);
    
    modificationBoxLayout.addWidget(new QLabel("Absolute Value:"), modificationBoxLayout.rowCount(), 0);
    modificationBoxLayout.addWidget(absoluteValueSpinBox, modificationBoxLayout.rowCount() - 1, 1);
    modificationBoxLayout.addWidget(new QLabel("Relative Value:"), modificationBoxLayout.rowCount(), 0);
    modificationBoxLayout.addWidget(relativeValueSpinBox, modificationBoxLayout.rowCount() - 1, 1);
    modificationBoxLayout.addWidget(new QLabel("%"), modificationBoxLayout.rowCount() - 1, 2);
    
    QDialogButtonBox modificationBoxButtons(Qt::Horizontal, modificationBox);
    modificationBoxButtons.addButton(QDialogButtonBox::Cancel);
    modificationBoxButtons.addButton(QString("Apply"), QDialogButtonBox::AcceptRole);
    modificationBoxLayout.addWidget(&modificationBoxButtons, modificationBoxLayout.rowCount(), 1, 1, 2);
    QObject::connect(&modificationBoxButtons, SIGNAL(accepted()), modificationBox, SLOT(accept()));
    QObject::connect(&modificationBoxButtons, SIGNAL(rejected()), modificationBox, SLOT(reject()));
    
    if (modificationBox->exec() == QDialog::Accepted && _mw && _mw->isSetSBMLDocument()) {
        RAVector* pWidth = new RAVector(absoluteValueSpinBox->value(), relativeValueSpinBox->value());
        
        if (_gO && _style) {
            VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gO, _style);
            
            if (!ne_img_setDimensionWidth(ne_grp_getGeometricShape(ne_stl_getGroup(style), 0), pWidth)) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit styleChanged(style);
            }
        }
        else if (_lE) {
            VLineEnding* lineEnding = ne_ven_getLocalLineEndingFromGlobalLineEnding(_mw->getSBMLDocument()->getVeneer(), _lE);
            
            if (!ne_img_setDimensionWidth(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), 0), pWidth)) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit lEndingChanged(lineEnding);
            }
        }
    }
}

void ImageGeometricShapeFeatureMenu::changeDimensionHeight() {
    QDialog* modificationBox =  new QDialog();
    modificationBox->setWindowTitle("Change Dimension Height of the Image Shape");
    QGridLayout modificationBoxLayout(modificationBox);
    modificationBoxLayout.addWidget(new QLabel("Dimension Height"), modificationBoxLayout.rowCount() - 1, 0);
    modificationBox->resize(350, 150);
    
    QDoubleSpinBox* absoluteValueSpinBox = new QDoubleSpinBox(modificationBox);
    QSpinBox* relativeValueSpinBox = new QSpinBox(modificationBox);
    QString currentAbsoluteValue, currentRelativeValue;
    if (dimensionHeightButton.text() != "N/A") {
        currentAbsoluteValue = dimensionHeightButton.text().split("+").at(0);
        currentRelativeValue = dimensionHeightButton.text().split("+").at(1).split("%").at(0);
    }
    absoluteValueSpinBox->setValue(currentAbsoluteValue.toDouble());
    absoluteValueSpinBox->setDecimals(1);
    if (ne_go_isSetBoundingBox(_gO))
        absoluteValueSpinBox->setRange(0.0, _mw->getScene()->sceneRect().y() + _mw->getScene()->sceneRect().height() - ne_bb_getY(ne_go_getBoundingBox(_gO)));
    else if (ne_le_isSetBoundingBox(_lE))
        absoluteValueSpinBox->setRange(0.0, _mw->getScene()->sceneRect().y() + _mw->getScene()->sceneRect().height() - ne_bb_getY(ne_le_getBoundingBox(_lE)));
    relativeValueSpinBox->setValue(currentRelativeValue.toInt());
    relativeValueSpinBox->setRange(0, 100);
    
    modificationBoxLayout.addWidget(new QLabel("Absolute Value:"), modificationBoxLayout.rowCount(), 0);
    modificationBoxLayout.addWidget(absoluteValueSpinBox, modificationBoxLayout.rowCount() - 1, 1);
    modificationBoxLayout.addWidget(new QLabel("Relative Value:"), modificationBoxLayout.rowCount(), 0);
    modificationBoxLayout.addWidget(relativeValueSpinBox, modificationBoxLayout.rowCount() - 1, 1);
    modificationBoxLayout.addWidget(new QLabel("%"), modificationBoxLayout.rowCount() - 1, 2);
    
    QDialogButtonBox modificationBoxButtons(Qt::Horizontal, modificationBox);
    modificationBoxButtons.addButton(QDialogButtonBox::Cancel);
    modificationBoxButtons.addButton(QString("Apply"), QDialogButtonBox::AcceptRole);
    modificationBoxLayout.addWidget(&modificationBoxButtons, modificationBoxLayout.rowCount(), 1, 1, 2);
    QObject::connect(&modificationBoxButtons, SIGNAL(accepted()), modificationBox, SLOT(accept()));
    QObject::connect(&modificationBoxButtons, SIGNAL(rejected()), modificationBox, SLOT(reject()));
    
    if (modificationBox->exec() == QDialog::Accepted && _mw && _mw->isSetSBMLDocument()) {
        RAVector* pHeight = new RAVector(absoluteValueSpinBox->value(), relativeValueSpinBox->value());
        
        if (_gO && _style) {
            VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gO, _style);
            
            if (!ne_img_setDimensionHeight(ne_grp_getGeometricShape(ne_stl_getGroup(style), 0), pHeight)) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit styleChanged(style);
            }
        }
        else if (_lE) {
            VLineEnding* lineEnding = ne_ven_getLocalLineEndingFromGlobalLineEnding(_mw->getSBMLDocument()->getVeneer(), _lE);
            
            if (!ne_img_setDimensionHeight(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), 0), pHeight)) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit lEndingChanged(lineEnding);
            }
        }
    }
}

void ImageGeometricShapeFeatureMenu::changeHref() {
    QString imageFileName = QFileDialog::getOpenFileName(this, "Select an image file", ".", tr("Image Files (*.png *.jpg *.jpeg)"));
    
    if (_mw->isSetSBMLDocument() && !imageFileName.isEmpty()) {
        if (_gO && _style) {
            VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gO, _style);
            
            if (!ne_img_setHref(ne_grp_getGeometricShape(ne_stl_getGroup(style), 0), imageFileName.toStdString())) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit styleChanged(style);
            }
        }
        else if (_lE) {
            VLineEnding* lineEnding = ne_ven_getLocalLineEndingFromGlobalLineEnding(_mw->getSBMLDocument()->getVeneer(), _lE);
            
            if (!ne_img_setHref(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), 0), imageFileName.toStdString())) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit lEndingChanged(lineEnding);
            }
        }
    }
}

// EllipseGeometricShapeFeatureMenu

EllipseGeometricShapeFeatureMenu::EllipseGeometricShapeFeatureMenu(QWidget* parent, MainWindow* mw) : GeometricShapeFeatureMenuElement(parent, mw) {
    
    // position label
    positionLabel.setText("Position:");
    positionLabel.setFont(featureTitleFont);
    gShapeLayout.addWidget(&positionLabel, gShapeLayout.rowCount(), 0, 1, 2);
    
    // cx label
    positionCXLabel.setText("CX:");
    positionCXLabel.setFont(featureTitleFont);
    gShapeLayout.addWidget(&positionCXLabel, gShapeLayout.rowCount(), 1, Qt::AlignLeft);
    
    // cx
    positionCXButton.setStyleSheet(pushButtonStyleSheet);
    gShapeLayout.addWidget(&positionCXButton, gShapeLayout.rowCount() - 1, 2, Qt::AlignRight);
    connect(&positionCXButton, SIGNAL(clicked()), this, SLOT(changePositionCX()));
    
    // cy label
    positionCYLabel.setText("CY:");
    positionCYLabel.setFont(featureTitleFont);
    gShapeLayout.addWidget(&positionCYLabel, gShapeLayout.rowCount(), 1, Qt::AlignLeft);
    
    // cy
    positionCYButton.setStyleSheet(pushButtonStyleSheet);
    gShapeLayout.addWidget(&positionCYButton, gShapeLayout.rowCount() - 1, 2, Qt::AlignRight);
    connect(&positionCYButton, SIGNAL(clicked()), this, SLOT(changePositionCY()));
    
    // dimension label
    dimensionLabel.setText("Dimensions:");
    dimensionLabel.setFont(featureTitleFont);
    gShapeLayout.addWidget(&dimensionLabel, gShapeLayout.rowCount(), 0, 1, 2);
    
    // rx label
    dimensionRXLabel.setText("RX:");
    dimensionRXLabel.setFont(featureTitleFont);
    gShapeLayout.addWidget(&dimensionRXLabel, gShapeLayout.rowCount(), 1, Qt::AlignLeft);
    
    // rx
    dimensionRXButton.setStyleSheet(pushButtonStyleSheet);
    gShapeLayout.addWidget(&dimensionRXButton, gShapeLayout.rowCount() - 1, 2, Qt::AlignRight);
    connect(&dimensionRXButton, SIGNAL(clicked()), this, SLOT(changeDimensionRX()));
    
    // ry label
    dimensionRYLabel.setText("RY:");
    dimensionRYLabel.setFont(featureTitleFont);
    gShapeLayout.addWidget(&dimensionRYLabel, gShapeLayout.rowCount(), 1, Qt::AlignLeft);
    
    // ry
    dimensionRYButton.setStyleSheet(pushButtonStyleSheet);
    gShapeLayout.addWidget(&dimensionRYButton, gShapeLayout.rowCount() - 1, 2, Qt::AlignRight);
    connect(&dimensionRYButton, SIGNAL(clicked()), this, SLOT(changeDimensionRY()));
    
    // ratio label
    dimensionRatioLabel.setText("Ratio:");
    dimensionRatioLabel.setFont(featureTitleFont);
    gShapeLayout.addWidget(&dimensionRatioLabel, gShapeLayout.rowCount(), 1, Qt::AlignLeft);
    
    // ratio
    dimensionRatioButton.setStyleSheet(pushButtonStyleSheet);
    gShapeLayout.addWidget(&dimensionRatioButton, gShapeLayout.rowCount() - 1, 2, Qt::AlignRight);
    connect(&dimensionRatioButton, SIGNAL(clicked()), this, SLOT(changeDimensionRatio()));
    
    // reset values
    resetValues();
}

void EllipseGeometricShapeFeatureMenu::showInfo(NGraphicalObject* gO, VGlobalStyle* style) {
    // reset values
    resetValues();
    
    if (ne_gs_getShape(ne_grp_getGeometricShape(ne_stl_getGroup(style), 0)) == 4) {
        _style = style;
        _gO = gO;
        getInfoFromEllipseShape((VEllipse*)ne_grp_getGeometricShape(ne_stl_getGroup(_style), 0));
    }

    show();
}

void EllipseGeometricShapeFeatureMenu::showInfo(VLineEnding* lE) {
    // reset values
    resetValues();
    
    if (ne_gs_getShape(ne_grp_getGeometricShape(ne_le_getGroup(lE), 0)) == 4) {
        _lE = lE;
        getInfoFromEllipseShape((VEllipse*)ne_grp_getGeometricShape(ne_le_getGroup(_lE), 0));
    }

    show();
}

void EllipseGeometricShapeFeatureMenu::resetValues() {
    positionCXButton.setText("N/A");
    positionCYButton.setText("N/A");
    dimensionRXButton.setText("N/A");
    dimensionRYButton.setText("N/A");
    dimensionRatioButton.setText("N/A");
    _gO = NULL;
    _style = NULL;
    _lE = NULL;
}

void EllipseGeometricShapeFeatureMenu::getInfoFromEllipseShape(VEllipse* ellipse) {
    if (ellipse) {
        RAVector* point = NULL;
        QString sPoint;
        
        // set x
        if (ne_elp_isSetPositionCX(ellipse)) {
            point = ne_elp_getPositionCX(ellipse);
            if (point) {
                sPoint = QString::number(point->a(), 'f', 1);
                sPoint += " + ";
                sPoint += QString::number(point->r());
                sPoint += "%";
                positionCXButton.setText(sPoint);
            }
        }
        
        // set cy
        if (ne_elp_isSetPositionCY(ellipse)) {
            point = ne_elp_getPositionCY(ellipse);
            if (point) {
                sPoint = QString::number(point->a(), 'f', 1);
                sPoint += " + ";
                sPoint += QString::number(point->r());
                sPoint += "%";
                positionCYButton.setText(sPoint);
            }
        }
        
        // set rx
        if (ne_elp_isSetDimensionRX(ellipse)) {
            point = ne_elp_getDimensionRX(ellipse);
            if (point) {
                sPoint = QString::number(point->a(), 'f', 1);
                sPoint += " + ";
                sPoint += QString::number(point->r());
                sPoint += "%";
                dimensionRXButton.setText(sPoint);
                
                if (!ne_elp_isSetDimensionRY(ellipse))
                    dimensionRYButton.setText(sPoint);
            }
        }
        
        // set ry
        if (ne_elp_isSetDimensionRY(ellipse)) {
            point = ne_elp_getDimensionRY(ellipse);
            if (point) {
                sPoint = QString::number(point->a(), 'f', 1);
                sPoint += " + ";
                sPoint += QString::number(point->r());
                sPoint += "%";
                dimensionRYButton.setText(sPoint);
                
                if (!ne_elp_isSetDimensionRX(ellipse))
                    dimensionRXButton.setText(sPoint);
            }
        }
        
        // set ratio
        if (ne_elp_isSetRatio(ellipse))
            dimensionRatioButton.setText(QString::number(ne_elp_getRatio(ellipse), 'f', 1));
    }
}

void EllipseGeometricShapeFeatureMenu::changePositionCX() {
    QDialog* modificationBox =  new QDialog();
    modificationBox->setWindowTitle("Change Position CX of the Ellipse Shape");
    QGridLayout modificationBoxLayout(modificationBox);
    modificationBoxLayout.addWidget(new QLabel("Position CX"), modificationBoxLayout.rowCount() - 1, 0);
    
    QDoubleSpinBox* absoluteValueSpinBox = new QDoubleSpinBox(modificationBox);
    QSpinBox* relativeValueSpinBox = new QSpinBox(modificationBox);
    QString currentAbsoluteValue, currentRelativeValue;
    if (positionCXButton.text() != "N/A") {
        currentAbsoluteValue = positionCXButton.text().split("+").at(0);
        currentRelativeValue = positionCXButton.text().split("+").at(1).split("%").at(0);
    }
    
    absoluteValueSpinBox->setValue(currentAbsoluteValue.toDouble());
    absoluteValueSpinBox->setDecimals(1);
    if (ne_go_isSetBoundingBox(_gO))
        absoluteValueSpinBox->setRange(_mw->getScene()->sceneRect().x() - ne_bb_getX(ne_go_getBoundingBox(_gO)), _mw->getScene()->sceneRect().x() + _mw->getScene()->sceneRect().width() - (ne_bb_getX(ne_go_getBoundingBox(_gO)) + ne_bb_getWidth(ne_go_getBoundingBox(_gO))));
    else if (ne_le_isSetBoundingBox(_lE))
        absoluteValueSpinBox->setRange(_mw->getScene()->sceneRect().x() - ne_bb_getX(ne_le_getBoundingBox(_lE)), _mw->getScene()->sceneRect().x() + _mw->getScene()->sceneRect().width() - (ne_bb_getX(ne_le_getBoundingBox(_lE)) + ne_bb_getWidth(ne_le_getBoundingBox(_lE))));
    relativeValueSpinBox->setValue(currentRelativeValue.toInt());
    relativeValueSpinBox->setRange(0, 100);
    
    modificationBoxLayout.addWidget(new QLabel("Absolute Value:"), modificationBoxLayout.rowCount(), 0);
    modificationBoxLayout.addWidget(absoluteValueSpinBox, modificationBoxLayout.rowCount() - 1, 1);
    modificationBoxLayout.addWidget(new QLabel("Relative Value:"), modificationBoxLayout.rowCount(), 0);
    modificationBoxLayout.addWidget(relativeValueSpinBox, modificationBoxLayout.rowCount() - 1, 1);
    modificationBoxLayout.addWidget(new QLabel("%"), modificationBoxLayout.rowCount() - 1, 2);
    
    QDialogButtonBox modificationBoxButtons(Qt::Horizontal, modificationBox);
    modificationBoxButtons.addButton(QDialogButtonBox::Cancel);
    modificationBoxButtons.addButton(QString("Apply"), QDialogButtonBox::AcceptRole);
    modificationBoxLayout.addWidget(&modificationBoxButtons, modificationBoxLayout.rowCount(), 1, 1, 2);
    QObject::connect(&modificationBoxButtons, SIGNAL(accepted()), modificationBox, SLOT(accept()));
    QObject::connect(&modificationBoxButtons, SIGNAL(rejected()), modificationBox, SLOT(reject()));
    
    if (modificationBox->exec() == QDialog::Accepted && _mw && _mw->isSetSBMLDocument()) {
        RAVector* pCX = new RAVector(absoluteValueSpinBox->value(), relativeValueSpinBox->value());
        
        if (_gO && _style) {
            VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gO, _style);
            
            if (!ne_elp_setPositionCX(ne_grp_getGeometricShape(ne_stl_getGroup(style), 0), pCX)) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit styleChanged(style);
            }
        }
        else if (_lE) {
            VLineEnding* lineEnding = ne_ven_getLocalLineEndingFromGlobalLineEnding(_mw->getSBMLDocument()->getVeneer(), _lE);
            
            if (!ne_elp_setPositionCX(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), 0), pCX)) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit lEndingChanged(lineEnding);
            }
        }
    }
}

void EllipseGeometricShapeFeatureMenu::changePositionCY() {
    QDialog* modificationBox =  new QDialog();
    modificationBox->setWindowTitle("Change Position CY of the Ellipse Shape");
    QGridLayout modificationBoxLayout(modificationBox);
    modificationBoxLayout.addWidget(new QLabel("Position CY"), modificationBoxLayout.rowCount() - 1, 0);
    
    QDoubleSpinBox* absoluteValueSpinBox = new QDoubleSpinBox(modificationBox);
    QSpinBox* relativeValueSpinBox = new QSpinBox(modificationBox);
    QString currentAbsoluteValue, currentRelativeValue;
    if (positionCYButton.text() != "N/A") {
        currentAbsoluteValue = positionCYButton.text().split("+").at(0);
        currentRelativeValue = positionCYButton.text().split("+").at(1).split("%").at(0);
    }
    
    absoluteValueSpinBox->setValue(currentAbsoluteValue.toDouble());
    absoluteValueSpinBox->setDecimals(1);
    if (ne_go_isSetBoundingBox(_gO))
        absoluteValueSpinBox->setRange(_mw->getScene()->sceneRect().y() - ne_bb_getY(ne_go_getBoundingBox(_gO)), _mw->getScene()->sceneRect().y() + _mw->getScene()->sceneRect().height() - (ne_bb_getY(ne_go_getBoundingBox(_gO)) + ne_bb_getHeight(ne_go_getBoundingBox(_gO))));
    else if (ne_le_isSetBoundingBox(_lE))
        absoluteValueSpinBox->setRange(_mw->getScene()->sceneRect().y() - ne_bb_getY(ne_le_getBoundingBox(_lE)), _mw->getScene()->sceneRect().y() + _mw->getScene()->sceneRect().height() - (ne_bb_getY(ne_le_getBoundingBox(_lE)) + ne_bb_getHeight(ne_le_getBoundingBox(_lE))));
    relativeValueSpinBox->setValue(currentRelativeValue.toInt());
    relativeValueSpinBox->setRange(0, 100);
    
    modificationBoxLayout.addWidget(new QLabel("Absolute Value:"), modificationBoxLayout.rowCount(), 0);
    modificationBoxLayout.addWidget(absoluteValueSpinBox, modificationBoxLayout.rowCount() - 1, 1);
    modificationBoxLayout.addWidget(new QLabel("Relative Value:"), modificationBoxLayout.rowCount(), 0);
    modificationBoxLayout.addWidget(relativeValueSpinBox, modificationBoxLayout.rowCount() - 1, 1);
    modificationBoxLayout.addWidget(new QLabel("%"), modificationBoxLayout.rowCount() - 1, 2);
    
    QDialogButtonBox modificationBoxButtons(Qt::Horizontal, modificationBox);
    modificationBoxButtons.addButton(QDialogButtonBox::Cancel);
    modificationBoxButtons.addButton(QString("Apply"), QDialogButtonBox::AcceptRole);
    modificationBoxLayout.addWidget(&modificationBoxButtons, modificationBoxLayout.rowCount(), 1, 1, 2);
    QObject::connect(&modificationBoxButtons, SIGNAL(accepted()), modificationBox, SLOT(accept()));
    QObject::connect(&modificationBoxButtons, SIGNAL(rejected()), modificationBox, SLOT(reject()));
    
    if (modificationBox->exec() == QDialog::Accepted && _mw && _mw->isSetSBMLDocument()) {
        RAVector* pCY = new RAVector(absoluteValueSpinBox->value(), relativeValueSpinBox->value());
        
        if (_gO && _style) {
            VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gO, _style);
            
            if (!ne_elp_setPositionCY(ne_grp_getGeometricShape(ne_stl_getGroup(style), 0), pCY)) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit styleChanged(style);
            }
        }
        else if (_lE) {
            VLineEnding* lineEnding = ne_ven_getLocalLineEndingFromGlobalLineEnding(_mw->getSBMLDocument()->getVeneer(), _lE);
            
            if (!ne_elp_setPositionCY(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), 0), pCY)) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit lEndingChanged(lineEnding);
            }
        }
    }
}

void EllipseGeometricShapeFeatureMenu::changeDimensionRX() {
    QDialog* modificationBox =  new QDialog();
    modificationBox->setWindowTitle("Change Dimension RX of the Ellipse Shape");
    QGridLayout modificationBoxLayout(modificationBox);
    modificationBoxLayout.addWidget(new QLabel("Dimension RX"), modificationBoxLayout.rowCount() - 1, 0);
    modificationBox->resize(350, 150);
    
    QDoubleSpinBox* absoluteValueSpinBox = new QDoubleSpinBox(modificationBox);
    QSpinBox* relativeValueSpinBox = new QSpinBox(modificationBox);
    QString currentAbsoluteValue, currentRelativeValue;
    if (dimensionRXButton.text() != "N/A") {
        currentAbsoluteValue = dimensionRXButton.text().split("+").at(0);
        currentRelativeValue = dimensionRXButton.text().split("+").at(1).split("%").at(0);
    }
    absoluteValueSpinBox->setValue(currentAbsoluteValue.toDouble());
    absoluteValueSpinBox->setDecimals(1);
    if (ne_go_isSetBoundingBox(_gO))
        absoluteValueSpinBox->setRange(0.0, _mw->getScene()->sceneRect().x() + _mw->getScene()->sceneRect().width() - ne_bb_getX(ne_go_getBoundingBox(_gO)));
    else if (ne_le_isSetBoundingBox(_lE))
        absoluteValueSpinBox->setRange(0.0, _mw->getScene()->sceneRect().x() + _mw->getScene()->sceneRect().width() - ne_bb_getX(ne_le_getBoundingBox(_lE)));
    relativeValueSpinBox->setValue(currentRelativeValue.toInt());
    relativeValueSpinBox->setRange(0, 100);
    
    modificationBoxLayout.addWidget(new QLabel("Absolute Value:"), modificationBoxLayout.rowCount(), 0);
    modificationBoxLayout.addWidget(absoluteValueSpinBox, modificationBoxLayout.rowCount() - 1, 1);
    modificationBoxLayout.addWidget(new QLabel("Relative Value:"), modificationBoxLayout.rowCount(), 0);
    modificationBoxLayout.addWidget(relativeValueSpinBox, modificationBoxLayout.rowCount() - 1, 1);
    modificationBoxLayout.addWidget(new QLabel("%"), modificationBoxLayout.rowCount() - 1, 2);
    
    QDialogButtonBox modificationBoxButtons(Qt::Horizontal, modificationBox);
    modificationBoxButtons.addButton(QDialogButtonBox::Cancel);
    modificationBoxButtons.addButton(QString("Apply"), QDialogButtonBox::AcceptRole);
    modificationBoxLayout.addWidget(&modificationBoxButtons, modificationBoxLayout.rowCount(), 1, 1, 2);
    QObject::connect(&modificationBoxButtons, SIGNAL(accepted()), modificationBox, SLOT(accept()));
    QObject::connect(&modificationBoxButtons, SIGNAL(rejected()), modificationBox, SLOT(reject()));
    
    if (modificationBox->exec() == QDialog::Accepted && _mw && _mw->isSetSBMLDocument()) {
        RAVector* pRX = new RAVector(absoluteValueSpinBox->value(), relativeValueSpinBox->value());
        
        if (_gO && _style) {
            VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gO, _style);
            
            if (!ne_elp_setDimensionRX(ne_grp_getGeometricShape(ne_stl_getGroup(style), 0), pRX)) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit styleChanged(style);
            }
        }
        else if (_lE) {
            VLineEnding* lineEnding = ne_ven_getLocalLineEndingFromGlobalLineEnding(_mw->getSBMLDocument()->getVeneer(), _lE);
            
            if (!ne_elp_setDimensionRX(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), 0), pRX)) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit lEndingChanged(lineEnding);
            }
        }
    }
}

void EllipseGeometricShapeFeatureMenu::changeDimensionRY() {
    QDialog* modificationBox =  new QDialog();
    modificationBox->setWindowTitle("Change Dimension RY of the Ellipse Shape");
    QGridLayout modificationBoxLayout(modificationBox);
    modificationBoxLayout.addWidget(new QLabel("Dimension RY"), modificationBoxLayout.rowCount() - 1, 0);
    modificationBox->resize(350, 150);
    
    QDoubleSpinBox* absoluteValueSpinBox = new QDoubleSpinBox(modificationBox);
    QSpinBox* relativeValueSpinBox = new QSpinBox(modificationBox);
    QString currentAbsoluteValue, currentRelativeValue;
    if (dimensionRYButton.text() != "N/A") {
        currentAbsoluteValue = dimensionRYButton.text().split("+").at(0);
        currentRelativeValue = dimensionRYButton.text().split("+").at(1).split("%").at(0);
    }
    absoluteValueSpinBox->setValue(currentAbsoluteValue.toDouble());
    absoluteValueSpinBox->setDecimals(1);
    if (ne_go_isSetBoundingBox(_gO))
        absoluteValueSpinBox->setRange(0.0, _mw->getScene()->sceneRect().y() + _mw->getScene()->sceneRect().height() - ne_bb_getY(ne_go_getBoundingBox(_gO)));
    else if (ne_le_isSetBoundingBox(_lE))
        absoluteValueSpinBox->setRange(0.0, _mw->getScene()->sceneRect().y() + _mw->getScene()->sceneRect().height() - ne_bb_getY(ne_le_getBoundingBox(_lE)));
    relativeValueSpinBox->setValue(currentRelativeValue.toInt());
    relativeValueSpinBox->setRange(0, 100);
    
    modificationBoxLayout.addWidget(new QLabel("Absolute Value:"), modificationBoxLayout.rowCount(), 0);
    modificationBoxLayout.addWidget(absoluteValueSpinBox, modificationBoxLayout.rowCount() - 1, 1);
    modificationBoxLayout.addWidget(new QLabel("Relative Value:"), modificationBoxLayout.rowCount(), 0);
    modificationBoxLayout.addWidget(relativeValueSpinBox, modificationBoxLayout.rowCount() - 1, 1);
    modificationBoxLayout.addWidget(new QLabel("%"), modificationBoxLayout.rowCount() - 1, 2);
    
    QDialogButtonBox modificationBoxButtons(Qt::Horizontal, modificationBox);
    modificationBoxButtons.addButton(QDialogButtonBox::Cancel);
    modificationBoxButtons.addButton(QString("Apply"), QDialogButtonBox::AcceptRole);
    modificationBoxLayout.addWidget(&modificationBoxButtons, modificationBoxLayout.rowCount(), 1, 1, 2);
    QObject::connect(&modificationBoxButtons, SIGNAL(accepted()), modificationBox, SLOT(accept()));
    QObject::connect(&modificationBoxButtons, SIGNAL(rejected()), modificationBox, SLOT(reject()));
    
    if (modificationBox->exec() == QDialog::Accepted && _mw && _mw->isSetSBMLDocument()) {
        RAVector* pRY = new RAVector(absoluteValueSpinBox->value(), relativeValueSpinBox->value());
        
        if (_gO && _style) {
            VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gO, _style);
            
            if (!ne_elp_setDimensionRY(ne_grp_getGeometricShape(ne_stl_getGroup(style), 0), pRY)) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit styleChanged(style);
            }
        }
        else if (_lE) {
            VLineEnding* lineEnding = ne_ven_getLocalLineEndingFromGlobalLineEnding(_mw->getSBMLDocument()->getVeneer(), _lE);
            
            if (!ne_elp_setDimensionRY(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), 0), pRY)) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit lEndingChanged(lineEnding);
            }
        }
    }
}

void EllipseGeometricShapeFeatureMenu::changeDimensionRatio() {
    QInputDialog* modificationBox =  new QInputDialog();
    modificationBox->setWindowTitle("Change Dimension Ratio of the Ellipse Shape");
    modificationBox->setLabelText("Dimension Ratio:");
    modificationBox->setOkButtonText("Apply");
    modificationBox->resize(300, 200);
    
    QStringList dimensionRatioItems;
    for (double i = 0.0; i <= 5.0;) {
        dimensionRatioItems << QString::number(i);
        if (i < 1)
            i += 0.2;
        else
            i += 0.5;
    }
    modificationBox->setComboBoxItems(dimensionRatioItems);
    
    if (dimensionRatioButton.text() != "N/A")
        modificationBox->setTextValue(dimensionRatioButton.text());
    
    if (modificationBox->exec() && _mw && _mw->isSetSBMLDocument() && _gO) {
        if (_gO && _style) {
            VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gO, _style);
            
            if (!ne_elp_setRatio(ne_grp_getGeometricShape(ne_stl_getGroup(style), 0), modificationBox->textValue().toDouble())) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit styleChanged(style);
            }
        }
        else if (_lE) {
            VLineEnding* lineEnding = ne_ven_getLocalLineEndingFromGlobalLineEnding(_mw->getSBMLDocument()->getVeneer(), _lE);
            
            if (!ne_elp_setRatio(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), 0), modificationBox->textValue().toDouble())) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit lEndingChanged(lineEnding);
            }
        }
    }
}

// PolygonGeometricShapeFeatureMenu

PolygonGeometricShapeFeatureMenu::PolygonGeometricShapeFeatureMenu(QWidget* parent, MainWindow* mw) : GeometricShapeFeatureMenuElement(parent, mw) {
    gShapeType = GRP_ELT_SHPE_PLG;
    
    // addPoint
    addPointButton.setStyleSheet(pushButtonStyleSheet);
    addPointButton.setText("Add");
    gShapeLayout.addWidget(&addPointButton, gShapeLayout.rowCount(), 0, 1, 3, Qt::AlignCenter);
    connect(&addPointButton, SIGNAL(clicked()), this, SLOT(addPointFeatureMenu()));
    
    // removePoint
    removePointButton.setText("Remove");
    gShapeLayout.addWidget(&removePointButton, gShapeLayout.rowCount(), 0, 1, 3, Qt::AlignCenter);
    connect(&removePointButton, SIGNAL(clicked()), this, SLOT(removePointFeatureMenu()));
    
    // reset values
    resetValues();
}

void PolygonGeometricShapeFeatureMenu::showInfo(NGraphicalObject* gO, VGlobalStyle* style) {
    // reset values
    resetValues();
    
    if (ne_gs_getShape(ne_grp_getGeometricShape(ne_stl_getGroup(style), 0)) == 5) {
        _style = style;
        _gO = gO;
        getInfoFromGShape((VPolygon*)ne_grp_getGeometricShape(ne_stl_getGroup(_style), 0));
        show();
    }
}

void PolygonGeometricShapeFeatureMenu::showInfo(VLineEnding* lE) {
    // reset values
    resetValues();
    
    if (ne_gs_getShape(ne_grp_getGeometricShape(ne_le_getGroup(lE), 0)) == 5) {
        _lE = lE;
        getInfoFromGShape((VPolygon*)ne_grp_getGeometricShape(ne_le_getGroup(_lE), 0));
        show();
    }
}

void PolygonGeometricShapeFeatureMenu::resetValues() {
    hide();
    for (constPointFeatureMenuIt pFMIt = pointFeatureMenusBegin(); pFMIt != pointFeatureMenusEnd(); ++ pFMIt) {
        gShapeLayout.removeWidget(*pFMIt);
        delete *pFMIt;
    }
    _pointFeatureMenus.clear();
    removePointButton.setStyleSheet(disabledPushButtonStyleSheet);
    removePointButton.blockSignals(true);
    _gO = NULL;
    _style = NULL;
    _lE = NULL;
}

void PolygonGeometricShapeFeatureMenu::getInfoFromGShape(VPolygon* polygon) {
    if (polygon) {
        PointFeatureMenu* pFeatureMenu = NULL;
        for (int i = 0; i < ne_plg_getNumVertices(polygon); ++i) {
            pFeatureMenu = new PointFeatureMenu(this, _mw, i);
            gShapeLayout.addWidget(pFeatureMenu, gShapeLayout.rowCount(), 0, 1, 3);
            
            if (_gO && _style) {
                connect(pFeatureMenu, SIGNAL(styleChanged(VLocalStyle*)), SLOT(changeStyle(VLocalStyle*)));
                pFeatureMenu->showInfo(_gO, _style, ne_plg_getVertex(polygon, i));
            }
            else if (_lE) {
                connect(pFeatureMenu, SIGNAL(lEndingChanged(VLineEnding*)), SLOT(changeLEnding(VLineEnding*)));
                pFeatureMenu->showInfo(_lE, ne_plg_getVertex(polygon, i));
            }
            _pointFeatureMenus.push_back(pFeatureMenu);
        }
        
        if (_pointFeatureMenus.size() > 3) {
            removePointButton.setStyleSheet(pushButtonStyleSheet);
            removePointButton.blockSignals(false);
        }
    }
}

void PolygonGeometricShapeFeatureMenu::addPointFeatureMenu() {
    QDialog* modificationBox =  new QDialog();
    modificationBox->setWindowTitle("Add an element to Geometric Shape");
    QGridLayout modificationBoxLayout(modificationBox);
    modificationBoxLayout.addWidget(new QLabel("Type"), modificationBoxLayout.rowCount() - 1, 0);
    QComboBox* elementTypeComboBox = new QComboBox(modificationBox);
    elementTypeComboBox->addItem(QString("RenderPoint"));
    elementTypeComboBox->addItem(QString("RenderCubicBezier"));
    elementTypeComboBox->setCurrentIndex(0);
    modificationBoxLayout.addWidget(elementTypeComboBox, modificationBoxLayout.rowCount() - 1, 1, Qt::AlignLeft);
    
    modificationBoxLayout.addWidget(new QLabel("Place"), modificationBoxLayout.rowCount() - 1, 2, Qt::AlignRight);
    QComboBox* placeComboBox = new QComboBox(modificationBox);
    for (int i = 1; i <= _pointFeatureMenus.size(); ++i)
        placeComboBox->addItem(QString::number(i));
    placeComboBox->addItem(QString("Last"));
    placeComboBox->setCurrentIndex(_pointFeatureMenus.size());
    modificationBoxLayout.addWidget(placeComboBox, modificationBoxLayout.rowCount() - 1, 3);
    
    // rpoint features
    QGroupBox* pointBox = new QGroupBox(modificationBox);
    QGridLayout pointBoxLayout(pointBox);
    pointBox->setStyleSheet("QGroupBox { background-color: transparent; border: no-border; }");
    pointBoxLayout.setContentsMargins(0, 5, 0, 0);
    modificationBoxLayout.addWidget(pointBox, modificationBoxLayout.rowCount(), 0, 1, 4);
    pointBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    
    // rpoint
    QDoubleSpinBox* xAbsoluteValueSpinBox = new QDoubleSpinBox(modificationBox);
    QSpinBox* xRelativeValueSpinBox = new QSpinBox(modificationBox);
    xAbsoluteValueSpinBox->setValue(0.0);
    xAbsoluteValueSpinBox->setDecimals(1);
    if (ne_go_isSetBoundingBox(_gO))
        xAbsoluteValueSpinBox->setRange(0.0, _mw->getScene()->sceneRect().x() + _mw->getScene()->sceneRect().width() - ne_bb_getX(ne_go_getBoundingBox(_gO)));
    else if (ne_le_isSetBoundingBox(_lE))
        xAbsoluteValueSpinBox->setRange(0.0, _mw->getScene()->sceneRect().x() + _mw->getScene()->sceneRect().width() - ne_bb_getX(ne_le_getBoundingBox(_lE)));
    xRelativeValueSpinBox->setValue(0);
    xRelativeValueSpinBox->setRange(0, 100);

    pointBoxLayout.addWidget(new QLabel("X"), pointBoxLayout.rowCount(), 0, 1, 2);
    pointBoxLayout.addWidget(new QLabel("Absolute Value:"), pointBoxLayout.rowCount(), 1);
    pointBoxLayout.addWidget(xAbsoluteValueSpinBox, pointBoxLayout.rowCount() - 1, 2, 1, 2);
    pointBoxLayout.addWidget(new QLabel("Relative Value:"), pointBoxLayout.rowCount(), 1);
    pointBoxLayout.addWidget(new QLabel("%"), pointBoxLayout.rowCount() - 1, 2, 1, 2, Qt::AlignLeft);
    pointBoxLayout.addWidget(xRelativeValueSpinBox, pointBoxLayout.rowCount() - 1, 2, 1, 2, Qt::AlignRight);
    
    QDoubleSpinBox* yAbsoluteValueSpinBox = new QDoubleSpinBox(modificationBox);
    QSpinBox* yRelativeValueSpinBox = new QSpinBox(modificationBox);
    yAbsoluteValueSpinBox->setValue(0.0);
    yAbsoluteValueSpinBox->setDecimals(1);
    if (ne_go_isSetBoundingBox(_gO))
        yAbsoluteValueSpinBox->setRange(0.0, _mw->getScene()->sceneRect().y() + _mw->getScene()->sceneRect().height() - ne_bb_getY(ne_go_getBoundingBox(_gO)));
    else if (ne_le_isSetBoundingBox(_lE))
        yAbsoluteValueSpinBox->setRange(0.0, _mw->getScene()->sceneRect().y() + _mw->getScene()->sceneRect().height() - ne_bb_getY(ne_le_getBoundingBox(_lE)));
    yRelativeValueSpinBox->setValue(0);
    yRelativeValueSpinBox->setRange(0, 100);

    pointBoxLayout.addWidget(new QLabel("Y"), pointBoxLayout.rowCount(), 0, 1, 2);
    pointBoxLayout.addWidget(new QLabel("Absolute Value:"), pointBoxLayout.rowCount(), 1);
    pointBoxLayout.addWidget(yAbsoluteValueSpinBox, pointBoxLayout.rowCount() - 1, 2, 1, 2);
    pointBoxLayout.addWidget(new QLabel("Relative Value:"), pointBoxLayout.rowCount(), 1);
    pointBoxLayout.addWidget(new QLabel("%"), pointBoxLayout.rowCount() - 1, 2, 1, 2, Qt::AlignLeft);
    pointBoxLayout.addWidget(yRelativeValueSpinBox, pointBoxLayout.rowCount() - 1, 2, 1, 2, Qt::AlignRight);
    
    // cubic bezier features
    QGroupBox* cubicBezierBox = new QGroupBox(modificationBox);
    QGridLayout cubicBezierBoxLayout(cubicBezierBox);
    cubicBezierBox->setStyleSheet("QGroupBox { background-color: transparent; border: no-border; }");
    cubicBezierBoxLayout.setContentsMargins(0, 5, 0, 0);
    modificationBoxLayout.addWidget(cubicBezierBox, modificationBoxLayout.rowCount(), 0, 1, 4);
    cubicBezierBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    cubicBezierBox->hide();
    
    // basepoint 1
    QDoubleSpinBox* c1XAbsoluteValueSpinBox = new QDoubleSpinBox(modificationBox);
    QSpinBox* c1XRelativeValueSpinBox = new QSpinBox(modificationBox);
    c1XAbsoluteValueSpinBox->setValue(0.0);
    c1XAbsoluteValueSpinBox->setDecimals(1);
    if (ne_go_isSetBoundingBox(_gO))
        c1XAbsoluteValueSpinBox->setRange(0.0, _mw->getScene()->sceneRect().x() + _mw->getScene()->sceneRect().width() - ne_bb_getX(ne_go_getBoundingBox(_gO)));
    else if (ne_le_isSetBoundingBox(_lE))
        c1XAbsoluteValueSpinBox->setRange(0.0, _mw->getScene()->sceneRect().x() + _mw->getScene()->sceneRect().width() - ne_bb_getX(ne_le_getBoundingBox(_lE)));
    c1XRelativeValueSpinBox->setValue(0);
    c1XRelativeValueSpinBox->setRange(0, 100);
    cubicBezierBoxLayout.addWidget(new QLabel("C1X"), cubicBezierBoxLayout.rowCount(), 0, 1, 2);
    cubicBezierBoxLayout.addWidget(new QLabel("Absolute Value:"), cubicBezierBoxLayout.rowCount(), 1);
    cubicBezierBoxLayout.addWidget(c1XAbsoluteValueSpinBox, cubicBezierBoxLayout.rowCount() - 1, 2, 1, 2);
    cubicBezierBoxLayout.addWidget(new QLabel("Relative Value:"), cubicBezierBoxLayout.rowCount(), 1);
    cubicBezierBoxLayout.addWidget(new QLabel("%"), cubicBezierBoxLayout.rowCount() - 1, 2, 1, 2, Qt::AlignLeft);
    cubicBezierBoxLayout.addWidget(c1XRelativeValueSpinBox, cubicBezierBoxLayout.rowCount() - 1, 2, 1, 2, Qt::AlignRight);
    
    QDoubleSpinBox* c1YAbsoluteValueSpinBox = new QDoubleSpinBox(modificationBox);
    QSpinBox* c1YRelativeValueSpinBox = new QSpinBox(modificationBox);
    c1YAbsoluteValueSpinBox->setValue(0.0);
    c1YAbsoluteValueSpinBox->setDecimals(1);
    if (ne_go_isSetBoundingBox(_gO))
        c1YAbsoluteValueSpinBox->setRange(0.0, _mw->getScene()->sceneRect().y() + _mw->getScene()->sceneRect().height() - ne_bb_getY(ne_go_getBoundingBox(_gO)));
    else if (ne_le_isSetBoundingBox(_lE))
        c1YAbsoluteValueSpinBox->setRange(0.0, _mw->getScene()->sceneRect().y() + _mw->getScene()->sceneRect().height() - ne_bb_getY(ne_le_getBoundingBox(_lE)));
    c1YRelativeValueSpinBox->setValue(0);
    c1YRelativeValueSpinBox->setRange(0, 100);
    cubicBezierBoxLayout.addWidget(new QLabel("C1Y"), cubicBezierBoxLayout.rowCount(), 0, 1, 2);
    cubicBezierBoxLayout.addWidget(new QLabel("Absolute Value:"), cubicBezierBoxLayout.rowCount(), 1);
    cubicBezierBoxLayout.addWidget(c1YAbsoluteValueSpinBox, cubicBezierBoxLayout.rowCount() - 1, 2, 1, 2);
    cubicBezierBoxLayout.addWidget(new QLabel("Relative Value:"), cubicBezierBoxLayout.rowCount(), 1);
    cubicBezierBoxLayout.addWidget(new QLabel("%"), cubicBezierBoxLayout.rowCount() - 1, 2, 1, 2, Qt::AlignLeft);
    cubicBezierBoxLayout.addWidget(c1YRelativeValueSpinBox, cubicBezierBoxLayout.rowCount() - 1, 2, 1, 2, Qt::AlignRight);
    
    // basepoint 2
    QDoubleSpinBox* c2XAbsoluteValueSpinBox = new QDoubleSpinBox(modificationBox);
    QSpinBox* c2XRelativeValueSpinBox = new QSpinBox(modificationBox);
    c2XAbsoluteValueSpinBox->setValue(0.0);
    c2XAbsoluteValueSpinBox->setDecimals(1);
    if (ne_go_isSetBoundingBox(_gO))
        c2XAbsoluteValueSpinBox->setRange(0.0, _mw->getScene()->sceneRect().x() + _mw->getScene()->sceneRect().width() - ne_bb_getX(ne_go_getBoundingBox(_gO)));
    else if (ne_le_isSetBoundingBox(_lE))
        c2XAbsoluteValueSpinBox->setRange(0.0, _mw->getScene()->sceneRect().x() + _mw->getScene()->sceneRect().width() - ne_bb_getX(ne_le_getBoundingBox(_lE)));
    c2XRelativeValueSpinBox->setValue(0);
    c2XRelativeValueSpinBox->setRange(0, 100);
    cubicBezierBoxLayout.addWidget(new QLabel("C2X"), cubicBezierBoxLayout.rowCount(), 0, 1, 2);
    cubicBezierBoxLayout.addWidget(new QLabel("Absolute Value:"), cubicBezierBoxLayout.rowCount(), 1);
    cubicBezierBoxLayout.addWidget(c2XAbsoluteValueSpinBox, cubicBezierBoxLayout.rowCount() - 1, 2, 1, 2);
    cubicBezierBoxLayout.addWidget(new QLabel("Relative Value:"), cubicBezierBoxLayout.rowCount(), 1);
    cubicBezierBoxLayout.addWidget(new QLabel("%"), cubicBezierBoxLayout.rowCount() - 1, 2, 1, 2, Qt::AlignLeft);
    cubicBezierBoxLayout.addWidget(c2XRelativeValueSpinBox, cubicBezierBoxLayout.rowCount() - 1, 2, 1, 2, Qt::AlignRight);
    
    QDoubleSpinBox* c2YAbsoluteValueSpinBox = new QDoubleSpinBox(modificationBox);
    QSpinBox* c2YRelativeValueSpinBox = new QSpinBox(modificationBox);
    c2YAbsoluteValueSpinBox->setValue(0.0);
    c2YAbsoluteValueSpinBox->setDecimals(1);
    if (ne_go_isSetBoundingBox(_gO))
        c2YAbsoluteValueSpinBox->setRange(0.0, _mw->getScene()->sceneRect().y() + _mw->getScene()->sceneRect().height() - ne_bb_getY(ne_go_getBoundingBox(_gO)));
    else if (ne_le_isSetBoundingBox(_lE))
        c2YAbsoluteValueSpinBox->setRange(0.0, _mw->getScene()->sceneRect().y() + _mw->getScene()->sceneRect().height() - ne_bb_getY(ne_le_getBoundingBox(_lE)));
    c2YRelativeValueSpinBox->setValue(0);
    c2YRelativeValueSpinBox->setRange(0, 100);
    cubicBezierBoxLayout.addWidget(new QLabel("C2Y"), cubicBezierBoxLayout.rowCount(), 0, 1, 2);
    cubicBezierBoxLayout.addWidget(new QLabel("Absolute Value:"), cubicBezierBoxLayout.rowCount(), 1);
    cubicBezierBoxLayout.addWidget(c2YAbsoluteValueSpinBox, cubicBezierBoxLayout.rowCount() - 1, 2, 1, 2);
    cubicBezierBoxLayout.addWidget(new QLabel("Relative Value:"), cubicBezierBoxLayout.rowCount(), 1);
    cubicBezierBoxLayout.addWidget(new QLabel("%"), cubicBezierBoxLayout.rowCount() - 1, 2, 1, 2, Qt::AlignLeft);
    cubicBezierBoxLayout.addWidget(c2YRelativeValueSpinBox, cubicBezierBoxLayout.rowCount() - 1, 2, 1, 2, Qt::AlignRight);
    
    QObject::connect(elementTypeComboBox, QOverload<int>::of(&QComboBox::activated), modificationBox, [modificationBox, cubicBezierBox] (const int& index) {
        if (index == 0)
            cubicBezierBox->hide();
        else if (index == 1)
            cubicBezierBox->show();
        
        modificationBox->layout()->setAlignment(Qt::AlignTop);
        modificationBox->layout()->update();
        modificationBox->adjustSize();
    });
    
    QDialogButtonBox modificationBoxButtons(Qt::Horizontal, modificationBox);
    modificationBoxButtons.addButton(QDialogButtonBox::Cancel);
    modificationBoxButtons.addButton(QString("Add"), QDialogButtonBox::AcceptRole);
    modificationBoxLayout.addWidget(&modificationBoxButtons, modificationBoxLayout.rowCount(), 1, 1, 3);
    QObject::connect(&modificationBoxButtons, SIGNAL(accepted()), modificationBox, SLOT(accept()));
    QObject::connect(&modificationBoxButtons, SIGNAL(rejected()), modificationBox, SLOT(reject()));
    
    if (modificationBox->exec() == QDialog::Accepted && _mw && _mw->isSetSBMLDocument()) {
        RAVector* pX = new RAVector(xAbsoluteValueSpinBox->value(), xRelativeValueSpinBox->value());
        RAVector* pY = new RAVector(yAbsoluteValueSpinBox->value(), yRelativeValueSpinBox->value());
        
        VLocalStyle* style = NULL;
        VLineEnding* lineEnding = NULL;
        RPoint* rP = new RPoint(RAVector(xAbsoluteValueSpinBox->value(), xRelativeValueSpinBox->value()), RAVector(yAbsoluteValueSpinBox->value(), yRelativeValueSpinBox->value()));
        
        if (elementTypeComboBox->currentIndex() == 0) {
            if (_gO && _style) {
                style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gO, _style);
                if (gShapeType == 5)
                    ne_plg_addVertex(ne_grp_getGeometricShape(ne_stl_getGroup(style), 0), rP);
                else if (gShapeType == 1)
                    ne_rc_addVertex(ne_grp_getGeometricShape(ne_stl_getGroup(style), 0), rP);
            }
            else if (_lE) {
                lineEnding = ne_ven_getLocalLineEndingFromGlobalLineEnding(_mw->getSBMLDocument()->getVeneer(), _lE);
                if (gShapeType == 5)
                    ne_plg_addVertex(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), 0), rP);
                else if (gShapeType == 1)
                    ne_rc_addVertex(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), 0), rP);
            }
        }
        else if (elementTypeComboBox->currentIndex() == 1) {
            RPoint* bP1 = new RPoint(RAVector(c1XAbsoluteValueSpinBox->value(), c1XRelativeValueSpinBox->value()), RAVector(c1YAbsoluteValueSpinBox->value(), c1YRelativeValueSpinBox->value()));
            RPoint* bP2 = new RPoint(RAVector(c2XAbsoluteValueSpinBox->value(), c2XRelativeValueSpinBox->value()), RAVector(c2YAbsoluteValueSpinBox->value(), c2YRelativeValueSpinBox->value()));
            
            if (_gO && _style) {
                style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gO, _style);
                if (gShapeType == 5)
                    ne_plg_addVertex(ne_grp_getGeometricShape(ne_stl_getGroup(style), 0), rP, bP1, bP2);
                else if (gShapeType == 1)
                    ne_rc_addVertex(ne_grp_getGeometricShape(ne_stl_getGroup(style), 0), rP, bP1, bP2);
            }
            else if (_lE) {
                lineEnding = ne_ven_getLocalLineEndingFromGlobalLineEnding(_mw->getSBMLDocument()->getVeneer(), _lE);
                if (gShapeType == 5)
                    ne_plg_addVertex(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), 0), rP, bP1, bP2);
                else if (gShapeType == 1)
                    ne_rc_addVertex(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), 0), rP, bP1, bP2);
            }
        }
        
        if (style) {
            _mw->getSBMLDocument()->setRenderModified(true);
            emit styleChanged(style);
        }
        else if (lineEnding) {
            _mw->getSBMLDocument()->setRenderModified(true);
            emit lEndingChanged(lineEnding);
        }
    }
}

void PolygonGeometricShapeFeatureMenu::removePointFeatureMenu() {
    QInputDialog* modificationBox =  new QInputDialog();
    modificationBox->setWindowTitle("Remove an element");
    modificationBox->setLabelText("Element:");
    modificationBox->setOkButtonText("Remove");
    modificationBox->resize(300, 200);
    
    QStringList pointIndexItems;
    for (int i = 1; i <= _pointFeatureMenus.size(); ++i)
        pointIndexItems << QString::number(i);
        
    modificationBox->setComboBoxItems(pointIndexItems);
    modificationBox->setTextValue(0);
    
    if (modificationBox->exec() && _mw && _mw->isSetSBMLDocument()) {
        VLocalStyle* style = NULL;
        VLineEnding* lineEnding = NULL;
        
        if (_gO && _style) {
            style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gO, _style);
            if (gShapeType == 5)
                ne_plg_removeVertex(ne_grp_getGeometricShape(ne_stl_getGroup(style), 0), modificationBox->textValue().toInt() - 1);
            else if (gShapeType == 1)
                ne_rc_removeVertex(ne_grp_getGeometricShape(ne_stl_getGroup(style), 0), modificationBox->textValue().toInt() - 1);
        }
        else if (_lE) {
            lineEnding = ne_ven_getLocalLineEndingFromGlobalLineEnding(_mw->getSBMLDocument()->getVeneer(), _lE);
            if (gShapeType == 5)
                ne_plg_removeVertex(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), 0), modificationBox->textValue().toInt() - 1);
            else if (gShapeType == 1)
                ne_rc_removeVertex(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), 0), modificationBox->textValue().toInt() - 1);
        }
            
        if (style) {
            _mw->getSBMLDocument()->setRenderModified(true);
            emit styleChanged(style);
        }
        else if (lineEnding) {
            _mw->getSBMLDocument()->setRenderModified(true);
            emit lEndingChanged(lineEnding);
        }
    }
}

void PolygonGeometricShapeFeatureMenu::changeStyle(VLocalStyle* style) {
    emit styleChanged(style);
}

void PolygonGeometricShapeFeatureMenu::changeLEnding(VLineEnding* lineEnding) {
    emit lEndingChanged(lineEnding);
}

// RenderCurveGeometricShapeFeatureMenu

RenderCurveGeometricShapeFeatureMenu::RenderCurveGeometricShapeFeatureMenu(QWidget* parent, MainWindow* mw) : PolygonGeometricShapeFeatureMenu(parent, mw) {
    gShapeType = GRP_ELT_SHPE_CRV;
};

void RenderCurveGeometricShapeFeatureMenu::showInfo(NGraphicalObject* gO, VGlobalStyle* style) {
    // reset values
    resetValues();
    
    if (ne_gs_getShape(ne_grp_getGeometricShape(ne_stl_getGroup(style), 0)) == 1) {
        _style = style;
        _gO = gO;
        getInfoFromGShape((RCurve*)ne_grp_getGeometricShape(ne_stl_getGroup(_style), 0));
        show();
    }
}

void RenderCurveGeometricShapeFeatureMenu::showInfo(VLineEnding* lE) {
    // reset values
    resetValues();
    
    if (ne_gs_getShape(ne_grp_getGeometricShape(ne_le_getGroup(lE), 0)) == 1) {
        _lE = lE;
        getInfoFromGShape((RCurve*)ne_grp_getGeometricShape(ne_le_getGroup(_lE), 0));
        show();
    }
}

void RenderCurveGeometricShapeFeatureMenu::getInfoFromGShape(RCurve* rcurve) {
    if (rcurve) {
        PointFeatureMenu* pFeatureMenu = NULL;
        for (int i = 0; i < ne_rc_getNumVertices(rcurve); ++i) {
            pFeatureMenu = new PointFeatureMenu(this, _mw, i);
            gShapeLayout.addWidget(pFeatureMenu, gShapeLayout.rowCount(), 0, 1, 3);
            
            if (_gO && _style) {
                connect(pFeatureMenu, SIGNAL(styleChanged(VLocalStyle*)), SLOT(changeStyle(VLocalStyle*)));
                pFeatureMenu->showInfo(_gO, _style, ne_rc_getVertex(rcurve, i));
            }
            else if (_lE) {
                connect(pFeatureMenu, SIGNAL(lEndingChanged(VLineEnding*)), SLOT(changeLEnding(VLineEnding*)));
                pFeatureMenu->showInfo(_lE, ne_rc_getVertex(rcurve, i));
            }
            _pointFeatureMenus.push_back(pFeatureMenu);
        }
        
        if (_pointFeatureMenus.size() > 2) {
            removePointButton.setStyleSheet(pushButtonStyleSheet);
            removePointButton.blockSignals(false);
        }
    }
}

// LineFeatureMenu

LineFeatureMenu::LineFeatureMenu(QWidget* parent, MainWindow* mw, const unsigned int& lineIndex) : QGroupBox(parent) {
    _mw = mw;
    _lineIndex = lineIndex;
    
    featureTitleFont = lineLabel.font();
    featureTitleFont.setBold(true);
    featureTitleFont.setCapitalization(QFont::Capitalize);
    featureTitleFont.setPointSize(12);
    pushButtonStyleSheet = "QPushButton { border: 2px solid #8f8f91; border-radius: 4px; background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #f6f7fa, stop: 1 #dadbde); min-width: 80px; } QPushButton:pressed { background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #dadbde, stop: 1 #f6f7fa); }";
    
    setLayout(&lineFeatureLayout);
    lineFeatureLayout.setContentsMargins(0, 0, 0, 0);
    
    // line label
    lineLabel.setFont(featureTitleFont);
    lineLabel.setText("Line Segment " + QString::number(_lineIndex + 1));
    lineFeatureLayout.addWidget(&lineLabel, lineFeatureLayout.rowCount() - 1, 0, 1, 3, Qt::AlignCenter);
    
    // start label
    startLabel.setFont(featureTitleFont);
    startLabel.setText("Start");
    lineFeatureLayout.addWidget(&startLabel, lineFeatureLayout.rowCount(), 0);
    
    // x label
    startXLabel.setFont(featureTitleFont);
    startXLabel.setText("X:");
    lineFeatureLayout.addWidget(&startXLabel, lineFeatureLayout.rowCount(), 1, Qt::AlignLeft);
    
    // x
    startXButton.setStyleSheet(pushButtonStyleSheet);
    lineFeatureLayout.addWidget(&startXButton, lineFeatureLayout.rowCount() - 1, 2, Qt::AlignRight);
    connect(&startXButton, SIGNAL(clicked()), this, SLOT(changeStartX()));
    
    // y label
    startYLabel.setFont(featureTitleFont);
    startYLabel.setText("Y:");
    lineFeatureLayout.addWidget(&startYLabel, lineFeatureLayout.rowCount(), 1, Qt::AlignLeft);
    
    // y
    startYButton.setStyleSheet(pushButtonStyleSheet);
    lineFeatureLayout.addWidget(&startYButton, lineFeatureLayout.rowCount() - 1, 2, Qt::AlignRight);
    connect(&startYButton, SIGNAL(clicked()), this, SLOT(changeStartY()));
    
    // end label
    endLabel.setFont(featureTitleFont);
    endLabel.setText("End");
    lineFeatureLayout.addWidget(&endLabel, lineFeatureLayout.rowCount(), 0);
    
    // x label
    endXLabel.setFont(featureTitleFont);
    endXLabel.setText("X:");
    lineFeatureLayout.addWidget(&endXLabel, lineFeatureLayout.rowCount(), 1, Qt::AlignLeft);
    
    // x
    endXButton.setStyleSheet(pushButtonStyleSheet);
    lineFeatureLayout.addWidget(&endXButton, lineFeatureLayout.rowCount() - 1, 2, Qt::AlignRight);
    connect(&endXButton, SIGNAL(clicked()), this, SLOT(changeEndX()));
    
    // y label
    endYLabel.setFont(featureTitleFont);
    endYLabel.setText("Y:");
    lineFeatureLayout.addWidget(&endYLabel, lineFeatureLayout.rowCount(), 1, Qt::AlignLeft);
    
    // y
    endYButton.setStyleSheet(pushButtonStyleSheet);
    lineFeatureLayout.addWidget(&endYButton, lineFeatureLayout.rowCount() - 1, 2, Qt::AlignRight);
    connect(&endYButton, SIGNAL(clicked()), this, SLOT(changeEndY()));
    
    // basepoint 1 label
    basePoint1Label.setFont(featureTitleFont);
    basePoint1Label.setText("BasePoint 1");
    lineFeatureLayout.addWidget(&basePoint1Label, lineFeatureLayout.rowCount(), 0);
    basePoint1Label.hide();
    
    // x label
    basePoint1XLabel.setFont(featureTitleFont);
    basePoint1XLabel.setText("X:");
    lineFeatureLayout.addWidget(&basePoint1XLabel, lineFeatureLayout.rowCount(), 1, Qt::AlignLeft);
    basePoint1XLabel.hide();
    
    // x
    basePoint1XButton.setStyleSheet(pushButtonStyleSheet);
    lineFeatureLayout.addWidget(&basePoint1XButton, lineFeatureLayout.rowCount() - 1, 2, Qt::AlignRight);
    basePoint1XButton.hide();
    connect(&basePoint1XButton, SIGNAL(clicked()), this, SLOT(changeBasePoint1X()));
    
    // y label
    basePoint1YLabel.setFont(featureTitleFont);
    basePoint1YLabel.setText("Y:");
    lineFeatureLayout.addWidget(&basePoint1YLabel, lineFeatureLayout.rowCount(), 1, Qt::AlignLeft);
    basePoint1YLabel.hide();
    
    // y
    basePoint1YButton.setStyleSheet(pushButtonStyleSheet);
    lineFeatureLayout.addWidget(&basePoint1YButton, lineFeatureLayout.rowCount() - 1, 2, Qt::AlignRight);
    basePoint1YButton.hide();
    connect(&basePoint1YButton, SIGNAL(clicked()), this, SLOT(changeBasePoint1Y()));
    
    // basepoint 2 label
    basePoint2Label.setFont(featureTitleFont);
    basePoint2Label.setText("BasePoint 2");
    lineFeatureLayout.addWidget(&basePoint2Label, lineFeatureLayout.rowCount(), 0);
    basePoint2Label.hide();
    
    // x label
    basePoint2XLabel.setFont(featureTitleFont);
    basePoint2XLabel.setText("X:");
    lineFeatureLayout.addWidget(&basePoint2XLabel, lineFeatureLayout.rowCount(), 1, Qt::AlignLeft);
    basePoint2XLabel.hide();
    
    // x
    basePoint2XButton.setStyleSheet(pushButtonStyleSheet);
    lineFeatureLayout.addWidget(&basePoint2XButton, lineFeatureLayout.rowCount() - 1, 2, Qt::AlignRight);
    basePoint2XButton.hide();
    connect(&basePoint2XButton, SIGNAL(clicked()), this, SLOT(changeBasePoint2X()));
    
    // y label
    basePoint2YLabel.setFont(featureTitleFont);
    basePoint2YLabel.setText("Y:");
    lineFeatureLayout.addWidget(&basePoint2YLabel, lineFeatureLayout.rowCount(), 1, Qt::AlignLeft);
    basePoint2YLabel.hide();
    
    // y
    basePoint2YButton.setStyleSheet(pushButtonStyleSheet);
    lineFeatureLayout.addWidget(&basePoint2YButton, lineFeatureLayout.rowCount() - 1, 2, Qt::AlignRight);
    basePoint2YButton.hide();
    connect(&basePoint2YButton, SIGNAL(clicked()), this, SLOT(changeBasePoint2Y()));
    
    lineFeatureLayout.setAlignment(Qt::AlignTop);
    lineFeatureLayout.update();
    
    // reset values
    resetValues();
}

void LineFeatureMenu::showInfo(LLineSegment* line) {
    // reset values
    resetValues();
    
    if (line) {
        _line = line;
        
        // start
        startXButton.setText(QString::number(_line->getStart().x(), 'f', 1));
        startYButton.setText(QString::number(_line->getStart().y(), 'f', 1));
        
        if (ne_ls_isSetStart(_line)) {
            startXButton.setText(QString::number(ne_point_getX(ne_ls_getStart(_line)), 'f', 1));
            startYButton.setText(QString::number(ne_point_getY(ne_ls_getStart(_line)), 'f', 1));
        }
        
        // end
        if (ne_ls_isSetEnd(_line)) {
            endXButton.setText(QString::number(ne_point_getX(ne_ls_getEnd(_line)), 'f', 1));
            endYButton.setText(QString::number(ne_point_getY(ne_ls_getEnd(_line)), 'f', 1));
        }
        
        // cubic bezier
        if (ne_ls_isCubicBezier(_line)) {
            basePoint1Label.show();
            basePoint1XLabel.show();
            basePoint1XButton.show();
            basePoint1YLabel.show();
            basePoint1YButton.show();
            basePoint2Label.show();
            basePoint2XLabel.show();
            basePoint2XButton.show();
            basePoint2YLabel.show();
            basePoint2YButton.show();
            
            // base point 1
            if (ne_cb_isSetBasePoint1(_line)) {
                basePoint1XButton.setText(QString::number(ne_point_getX(ne_cb_getBasePoint1(_line)), 'f', 1));
                basePoint1YButton.setText(QString::number(ne_point_getY(ne_cb_getBasePoint1(_line)), 'f', 1));
            }
            
            // base point 2
            if (ne_cb_isSetBasePoint2(_line)) {
                basePoint2XButton.setText(QString::number(ne_point_getX(ne_cb_getBasePoint2(_line)), 'f', 1));
                basePoint2YButton.setText(QString::number(ne_point_getY(ne_cb_getBasePoint2(_line)), 'f', 1));
            }
        }
    }
    
    show();
}

void LineFeatureMenu::resetValues() {
    startXButton.setText("N/A");
    startYButton.setText("N/A");
    endXButton.setText("N/A");
    endYButton.setText("N/A");
    basePoint1XButton.setText("N/A");
    basePoint1YButton.setText("N/A");
    basePoint2XButton.setText("N/A");
    basePoint2YButton.setText("N/A");
    _line = NULL;
}

void LineFeatureMenu::changeStartX() {
    QInputDialog* modificationBox =  new QInputDialog();
    modificationBox->setWindowTitle("Change Start Point X of the LineSegment");
    modificationBox->setLabelText("Start Point X:");
    modificationBox->setDoubleRange(_mw->getScene()->sceneRect().x(), _mw->getScene()->sceneRect().x() + _mw->getScene()->sceneRect().width());
    modificationBox->setOkButtonText("Apply");
    modificationBox->resize(300, 200);
    
    if (startXButton.text() != "N/A")
        modificationBox->setDoubleValue(startXButton.text().toDouble());
    
    if (modificationBox->exec()) {
        LPoint* point = ne_ls_getStart(_line);
        if (!ne_point_setX(point, modificationBox->doubleValue()) && !ne_ls_setStart(_line, point)) {
            _mw->getSBMLDocument()->setLayoutModified(true);
            emit lineChanged();
        }
    }
}

void LineFeatureMenu::changeStartY() {
    QInputDialog* modificationBox =  new QInputDialog();
    modificationBox->setWindowTitle("Change Start Point Y of the LineSegment");
    modificationBox->setLabelText("Start Point Y:");
    modificationBox->setDoubleRange(_mw->getScene()->sceneRect().y(), _mw->getScene()->sceneRect().y() + _mw->getScene()->sceneRect().height());
    modificationBox->setOkButtonText("Apply");
    modificationBox->resize(300, 200);
    
    if (startYButton.text() != "N/A")
        modificationBox->setDoubleValue(startYButton.text().toDouble());
    
    if (modificationBox->exec()) {
        LPoint* point = ne_ls_getStart(_line);
        if (!ne_point_setY(point, modificationBox->doubleValue()) && !ne_ls_setStart(_line, point)) {
            _mw->getSBMLDocument()->setLayoutModified(true);
            emit lineChanged();
        }
    }
}

void LineFeatureMenu::changeEndX() {
    QInputDialog* modificationBox =  new QInputDialog();
    modificationBox->setWindowTitle("Change End Point X of the LineSegment");
    modificationBox->setLabelText("End Point X:");
    modificationBox->setDoubleRange(_mw->getScene()->sceneRect().x(), _mw->getScene()->sceneRect().x() + _mw->getScene()->sceneRect().width());
    modificationBox->setOkButtonText("Apply");
    modificationBox->resize(300, 200);
    
    if (endXButton.text() != "N/A")
        modificationBox->setDoubleValue(endXButton.text().toDouble());
    
    if (modificationBox->exec()) {
        LPoint* point = ne_ls_getEnd(_line);
        if (!ne_point_setX(point, modificationBox->doubleValue()) && !ne_ls_setEnd(_line, point)) {
            _mw->getSBMLDocument()->setLayoutModified(true);
            emit lineChanged();
        }
    }
}

void LineFeatureMenu::changeEndY() {
    QInputDialog* modificationBox =  new QInputDialog();
    modificationBox->setWindowTitle("Change End Point Y of the LineSegment");
    modificationBox->setLabelText("End Point Y:");
    modificationBox->setDoubleRange(_mw->getScene()->sceneRect().y(), _mw->getScene()->sceneRect().y() + _mw->getScene()->sceneRect().height());
    modificationBox->setOkButtonText("Apply");
    modificationBox->resize(300, 200);
    
    if (endYButton.text() != "N/A")
        modificationBox->setDoubleValue(endYButton.text().toDouble());
    
    if (modificationBox->exec()) {
        LPoint* point = ne_ls_getEnd(_line);
        if (!ne_point_setY(point, modificationBox->doubleValue()) && !ne_ls_setEnd(_line, point)) {
            _mw->getSBMLDocument()->setLayoutModified(true);
            emit lineChanged();
        }
    }
}

void LineFeatureMenu::changeBasePoint1X() {
    QInputDialog* modificationBox =  new QInputDialog();
    modificationBox->setWindowTitle("Change BasePoint1 X of the Cubic Bezier");
    modificationBox->setLabelText("BasePoint1 X:");
    modificationBox->setDoubleRange(_mw->getScene()->sceneRect().x(), _mw->getScene()->sceneRect().x() + _mw->getScene()->sceneRect().width());
    modificationBox->setOkButtonText("Apply");
    modificationBox->resize(300, 200);
    
    if (basePoint1XButton.text() != "N/A")
        modificationBox->setDoubleValue(basePoint1XButton.text().toDouble());
    
    if (modificationBox->exec()) {
        LPoint* point = ne_cb_getBasePoint1(_line);
        if (!ne_point_setX(point, modificationBox->doubleValue()) && !ne_cb_setBasePoint1(_line, point)) {
            _mw->getSBMLDocument()->setLayoutModified(true);
            emit lineChanged();
        }
    }
}

void LineFeatureMenu::changeBasePoint1Y() {
    QInputDialog* modificationBox =  new QInputDialog();
    modificationBox->setWindowTitle("Change BasePoint1 Y of the Cubic Bezier");
    modificationBox->setLabelText("BasePoint1 Y:");
    modificationBox->setDoubleRange(_mw->getScene()->sceneRect().y(), _mw->getScene()->sceneRect().y() + _mw->getScene()->sceneRect().height());
    modificationBox->setOkButtonText("Apply");
    modificationBox->resize(300, 200);
    
    if (basePoint1YButton.text() != "N/A")
        modificationBox->setDoubleValue(basePoint1YButton.text().toDouble());
    
    if (modificationBox->exec()) {
        LPoint* point = ne_cb_getBasePoint1(_line);
        if (!ne_point_setY(point, modificationBox->doubleValue()) && !ne_cb_setBasePoint1(_line, point)) {
            _mw->getSBMLDocument()->setLayoutModified(true);
            emit lineChanged();
        }
    }
}

void LineFeatureMenu::changeBasePoint2X() {
    QInputDialog* modificationBox =  new QInputDialog();
    modificationBox->setWindowTitle("Change BasePoint2 X of the Cubic Bezier");
    modificationBox->setLabelText("BasePoint2 X:");
    modificationBox->setDoubleRange(_mw->getScene()->sceneRect().x(), _mw->getScene()->sceneRect().x() + _mw->getScene()->sceneRect().width());
    modificationBox->setOkButtonText("Apply");
    modificationBox->resize(300, 200);
    
    if (basePoint2XButton.text() != "N/A")
        modificationBox->setDoubleValue(basePoint2XButton.text().toDouble());
    
    if (modificationBox->exec()) {
        LPoint* point = ne_cb_getBasePoint2(_line);
        if (!ne_point_setX(point, modificationBox->doubleValue()) && !ne_cb_setBasePoint2(_line, point)) {
            _mw->getSBMLDocument()->setLayoutModified(true);
            emit lineChanged();
        }
    }
}

void LineFeatureMenu::changeBasePoint2Y() {
    QInputDialog* modificationBox =  new QInputDialog();
    modificationBox->setWindowTitle("Change BasePoint2 Y of the Cubic Bezier");
    modificationBox->setLabelText("BasePoint2 Y:");
    modificationBox->setDoubleRange(_mw->getScene()->sceneRect().y(), _mw->getScene()->sceneRect().y() + _mw->getScene()->sceneRect().height());
    modificationBox->setOkButtonText("Apply");
    modificationBox->resize(300, 200);
    
    if (basePoint2YButton.text() != "N/A")
        modificationBox->setDoubleValue(basePoint2YButton.text().toDouble());
    
    if (modificationBox->exec()) {
        LPoint* point = ne_cb_getBasePoint2(_line);
        if (!ne_point_setY(point, modificationBox->doubleValue()) && !ne_cb_setBasePoint2(_line, point)) {
            _mw->getSBMLDocument()->setLayoutModified(true);
            emit lineChanged();
        }
    }
}

// PointFeatureMenu

PointFeatureMenu::PointFeatureMenu(QWidget* parent, MainWindow* mw, const unsigned int& pointIndex) : QGroupBox(parent) {
    _mw = mw;
    _pointIndex = pointIndex;
    
    featureTitleFont = pointLabel.font();
    featureTitleFont.setBold(true);
    featureTitleFont.setCapitalization(QFont::Capitalize);
    featureTitleFont.setPointSize(12);
    pushButtonStyleSheet = "QPushButton { border: 2px solid #8f8f91; border-radius: 4px; background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #f6f7fa, stop: 1 #dadbde); min-width: 80px; } QPushButton:pressed { background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #dadbde, stop: 1 #f6f7fa); }";
    
    setLayout(&pointFeatureLayout);
    pointFeatureLayout.setContentsMargins(0, 0, 0, 0);
    
    /// point
    // point label
    pointLabel.setFont(featureTitleFont);
    pointLabel.setText("Point " + QString::number(_pointIndex + 1));
    pointFeatureLayout.addWidget(&pointLabel, pointFeatureLayout.rowCount() - 1, 0);
    
    // x label
    pointXLabel.setFont(featureTitleFont);
    pointXLabel.setText("X:");
    pointFeatureLayout.addWidget(&pointXLabel, pointFeatureLayout.rowCount(), 1, Qt::AlignLeft);
    
    // x
    pointXButton.setStyleSheet(pushButtonStyleSheet);
    pointFeatureLayout.addWidget(&pointXButton, pointFeatureLayout.rowCount() - 1, 2, Qt::AlignRight);
    connect(&pointXButton, SIGNAL(clicked()), this, SLOT(changeRPointX()));
    
    // y label
    pointYLabel.setFont(featureTitleFont);
    pointYLabel.setText("Y:");
    pointFeatureLayout.addWidget(&pointYLabel, pointFeatureLayout.rowCount(), 1, Qt::AlignLeft);
    
    // y
    pointYButton.setStyleSheet(pushButtonStyleSheet);
    pointFeatureLayout.addWidget(&pointYButton, pointFeatureLayout.rowCount() - 1, 2, Qt::AlignRight);
    connect(&pointYButton, SIGNAL(clicked()), this, SLOT(changeRPointY()));

    /// cubic bezier
    // basepoint1 x label
    basePoint1XLabel.setFont(featureTitleFont);
    basePoint1XLabel.setText("c1X:");
    pointFeatureLayout.addWidget(&basePoint1XLabel, pointFeatureLayout.rowCount(), 1, Qt::AlignLeft);
    basePoint1XLabel.hide();
    
    // basepoint1 x
    basePoint1XButton.setStyleSheet(pushButtonStyleSheet);
    pointFeatureLayout.addWidget(&basePoint1XButton, pointFeatureLayout.rowCount() - 1, 2, Qt::AlignRight);
    basePoint1XButton.hide();
    connect(&basePoint1XButton, SIGNAL(clicked()), this, SLOT(changeBasePoint1X()));
    
    // basepoint1 y label
    basePoint1YLabel.setFont(featureTitleFont);
    basePoint1YLabel.setText("c1Y:");
    pointFeatureLayout.addWidget(&basePoint1YLabel, pointFeatureLayout.rowCount(), 1, Qt::AlignLeft);
    basePoint1YLabel.hide();
    
    // basepoint1 y
    basePoint1YButton.setStyleSheet(pushButtonStyleSheet);
    pointFeatureLayout.addWidget(&basePoint1YButton, pointFeatureLayout.rowCount() - 1, 2, Qt::AlignRight);
    basePoint1YButton.hide();
    connect(&basePoint1YButton, SIGNAL(clicked()), this, SLOT(changeBasePoint1Y()));
    
    // basepoint2 x label
    basePoint2XLabel.setFont(featureTitleFont);
    basePoint2XLabel.setText("c2X:");
    pointFeatureLayout.addWidget(&basePoint2XLabel, pointFeatureLayout.rowCount(), 1, Qt::AlignLeft);
    basePoint2XLabel.hide();
    
    // basepoint2 x
    basePoint2XButton.setStyleSheet(pushButtonStyleSheet);
    pointFeatureLayout.addWidget(&basePoint2XButton, pointFeatureLayout.rowCount() - 1, 2, Qt::AlignRight);
    basePoint2XButton.hide();
    connect(&basePoint2XButton, SIGNAL(clicked()), this, SLOT(changeBasePoint2X()));
    
    // basepoint2 y label
    basePoint2YLabel.setFont(featureTitleFont);
    basePoint2YLabel.setText("c2Y:");
    pointFeatureLayout.addWidget(&basePoint2YLabel, pointFeatureLayout.rowCount(), 1, Qt::AlignLeft);
    basePoint2YLabel.hide();
    
    // basepoint2 y
    basePoint2YButton.setStyleSheet(pushButtonStyleSheet);
    pointFeatureLayout.addWidget(&basePoint2YButton, pointFeatureLayout.rowCount() - 1, 2, Qt::AlignRight);
    basePoint2YButton.hide();
    connect(&basePoint2YButton, SIGNAL(clicked()), this, SLOT(changeBasePoint2Y()));
    
    pointFeatureLayout.setAlignment(Qt::AlignTop);
    pointFeatureLayout.update();
    
    // reset values
    resetValues();
}

void PointFeatureMenu::showInfo(NGraphicalObject* gO, VGlobalStyle* style, RenPoint* renderPoint) {
    // reset values
    resetValues();
    
    if (gO && style) {
        _style = style;
        _gO = gO;
        getInfoFromRenderPoint(renderPoint);
    }
    
    show();
}

void PointFeatureMenu::showInfo(VLineEnding* lE, RenPoint* renderPoint) {
    // reset values
    resetValues();
    
    if (lE) {
        _lE = lE;
        getInfoFromRenderPoint(renderPoint);
    }
    
    show();
}

void PointFeatureMenu::resetValues() {
    pointXButton.setText("N/A");
    pointYButton.setText("N/A");
    basePoint1XButton.setText("N/A");
    basePoint1YButton.setText("N/A");
    basePoint2XButton.setText("N/A");
    basePoint2YButton.setText("N/A");
    _gO = NULL;
    _style = NULL;
    _lE = NULL;
    _renderPoint = NULL;
}

void PointFeatureMenu::getInfoFromRenderPoint(RenPoint* renderPoint) {
    if (renderPoint) {
        _renderPoint = renderPoint;
        QString sPoint;
        
        if (renderPoint->isSetRPoint()) {
            
            // x
            sPoint = QString::number(ne_rav_getAbsoluteValue(ne_rp_getX(ne_vrx_getRenderPoint(_renderPoint))), 'f', 1);
            sPoint += " + ";
            sPoint += QString::number(ne_rav_getRelativeValue(ne_rp_getX(ne_vrx_getRenderPoint(_renderPoint))), 'f', 1);
            sPoint += "%";
            pointXButton.setText(sPoint);
            
            // y
            sPoint = QString::number(ne_rav_getAbsoluteValue(ne_rp_getY(ne_vrx_getRenderPoint(_renderPoint))), 'f', 1);
            sPoint += " + ";
            sPoint += QString::number(ne_rav_getRelativeValue(ne_rp_getY(ne_vrx_getRenderPoint(_renderPoint))), 'f', 1);
            sPoint += "%";
            pointYButton.setText(sPoint);
        }
        
        // cubic bezier
        if (renderPoint->isRenderCubicBezier()) {
            basePoint1XLabel.show();
            basePoint1XButton.show();
            basePoint1YLabel.show();
            basePoint1YButton.show();
            basePoint2XLabel.show();
            basePoint2XButton.show();
            basePoint2YLabel.show();
            basePoint2YButton.show();
            
            // base point 1
            if (((RCubicBezier*)(renderPoint))->isSetBasePoint1()) {
                // x
                sPoint = QString::number(((RCubicBezier*)(renderPoint))->getBasePoint1().x().a(), 'f', 1);
                sPoint += " + ";
                sPoint += QString::number(((RCubicBezier*)(renderPoint))->getBasePoint1().x().r(), 'f', 1);
                sPoint += "%";
                basePoint1XButton.setText(sPoint);
                
                // y
                sPoint = QString::number(((RCubicBezier*)(renderPoint))->getBasePoint1().y().a(), 'f', 1);
                sPoint += " + ";
                sPoint += QString::number(((RCubicBezier*)(renderPoint))->getBasePoint1().y().r(), 'f', 1);
                sPoint += "%";
                basePoint1YButton.setText(sPoint);
            }
            
            // base point 2
            if (((RCubicBezier*)(renderPoint))->isSetBasePoint2()) {
                // x
                sPoint = QString::number(((RCubicBezier*)(renderPoint))->getBasePoint2().x().a(), 'f', 1);
                sPoint += " + ";
                sPoint += QString::number(((RCubicBezier*)(renderPoint))->getBasePoint2().x().r(), 'f', 1);
                sPoint += "%";
                basePoint2XButton.setText(sPoint);
                
                // y
                sPoint = QString::number(((RCubicBezier*)(renderPoint))->getBasePoint2().y().a(), 'f', 1);
                sPoint += " + ";
                sPoint += QString::number(((RCubicBezier*)(renderPoint))->getBasePoint2().y().r(), 'f', 1);
                sPoint += "%";
                basePoint2YButton.setText(sPoint);
            }
        }
    }
}

void PointFeatureMenu::changeRPointX() {
    QDialog* modificationBox =  new QDialog();
    modificationBox->setWindowTitle("Change RPointX of Point" + QString::number(_pointIndex + 1));
    QGridLayout modificationBoxLayout(modificationBox);
    modificationBoxLayout.addWidget(new QLabel("RPointX"), modificationBoxLayout.rowCount() - 1, 0);
    
    QDoubleSpinBox* absoluteValueSpinBox = new QDoubleSpinBox(modificationBox);
    QSpinBox* relativeValueSpinBox = new QSpinBox(modificationBox);
    QString currentAbsoluteValue, currentRelativeValue;
    if (pointXButton.text() != "N/A") {
        currentAbsoluteValue = pointXButton.text().split("+").at(0);
        currentRelativeValue = pointXButton.text().split("+").at(1).split("%").at(0);
    }
    
    absoluteValueSpinBox->setValue(currentAbsoluteValue.toDouble());
    absoluteValueSpinBox->setDecimals(1);
    absoluteValueSpinBox->setRange(0.0, _mw->getScene()->sceneRect().x() + _mw->getScene()->sceneRect().width());
    relativeValueSpinBox->setValue(currentRelativeValue.toInt());
    relativeValueSpinBox->setRange(0, 100);
    
    modificationBoxLayout.addWidget(new QLabel("Absolute Value:"), modificationBoxLayout.rowCount(), 0);
    modificationBoxLayout.addWidget(absoluteValueSpinBox, modificationBoxLayout.rowCount() - 1, 1);
    modificationBoxLayout.addWidget(new QLabel("Relative Value:"), modificationBoxLayout.rowCount(), 0);
    modificationBoxLayout.addWidget(relativeValueSpinBox, modificationBoxLayout.rowCount() - 1, 1);
    modificationBoxLayout.addWidget(new QLabel("%"), modificationBoxLayout.rowCount() - 1, 2);
    
    QDialogButtonBox modificationBoxButtons(Qt::Horizontal, modificationBox);
    modificationBoxButtons.addButton(QDialogButtonBox::Cancel);
    modificationBoxButtons.addButton(QString("Apply"), QDialogButtonBox::AcceptRole);
    modificationBoxLayout.addWidget(&modificationBoxButtons, modificationBoxLayout.rowCount(), 1, 1, 2);
    QObject::connect(&modificationBoxButtons, SIGNAL(accepted()), modificationBox, SLOT(accept()));
    QObject::connect(&modificationBoxButtons, SIGNAL(rejected()), modificationBox, SLOT(reject()));
    
    if (modificationBox->exec() == QDialog::Accepted && _mw && _mw->isSetSBMLDocument()) {
        RAVector* pX = new RAVector(absoluteValueSpinBox->value(), relativeValueSpinBox->value());
        
        if (_gO && _style) {
            VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gO, _style);
            
            if (!ne_rp_setX(ne_vrx_getRenderPoint(ne_rc_getVertex(ne_grp_getGeometricShape(ne_stl_getGroup(style), 0), _pointIndex)), pX) || !ne_rp_setX(ne_vrx_getRenderPoint(ne_plg_getVertex(ne_grp_getGeometricShape(ne_stl_getGroup(style), 0), _pointIndex)), pX)) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit styleChanged(style);
            }
        }
        else if (_lE) {
            VLineEnding* lineEnding = ne_ven_getLocalLineEndingFromGlobalLineEnding(_mw->getSBMLDocument()->getVeneer(), _lE);
            
            if (!ne_rp_setX(ne_vrx_getRenderPoint(ne_rc_getVertex(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), 0), _pointIndex)), pX) || !ne_rp_setX(ne_vrx_getRenderPoint(ne_plg_getVertex(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), 0), _pointIndex)), pX)) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit lEndingChanged(lineEnding);
            }
        }
    }
}

void PointFeatureMenu::changeRPointY() {
    QDialog* modificationBox =  new QDialog();
    modificationBox->setWindowTitle("Change RPointY of Point" + QString::number(_pointIndex + 1));
    QGridLayout modificationBoxLayout(modificationBox);
    modificationBoxLayout.addWidget(new QLabel("RPointY"), modificationBoxLayout.rowCount() - 1, 0);
    
    QDoubleSpinBox* absoluteValueSpinBox = new QDoubleSpinBox(modificationBox);
    QSpinBox* relativeValueSpinBox = new QSpinBox(modificationBox);
    QString currentAbsoluteValue, currentRelativeValue;
    if (pointYButton.text() != "N/A") {
        currentAbsoluteValue = pointYButton.text().split("+").at(0);
        currentRelativeValue = pointYButton.text().split("+").at(1).split("%").at(0);
    }
    
    absoluteValueSpinBox->setValue(currentAbsoluteValue.toDouble());
    absoluteValueSpinBox->setDecimals(1);
    absoluteValueSpinBox->setRange(0.0, _mw->getScene()->sceneRect().y() + _mw->getScene()->sceneRect().height());
    relativeValueSpinBox->setValue(currentRelativeValue.toInt());
    relativeValueSpinBox->setRange(0, 100);
    
    modificationBoxLayout.addWidget(new QLabel("Absolute Value:"), modificationBoxLayout.rowCount(), 0);
    modificationBoxLayout.addWidget(absoluteValueSpinBox, modificationBoxLayout.rowCount() - 1, 1);
    modificationBoxLayout.addWidget(new QLabel("Relative Value:"), modificationBoxLayout.rowCount(), 0);
    modificationBoxLayout.addWidget(relativeValueSpinBox, modificationBoxLayout.rowCount() - 1, 1);
    modificationBoxLayout.addWidget(new QLabel("%"), modificationBoxLayout.rowCount() - 1, 2);
    
    QDialogButtonBox modificationBoxButtons(Qt::Horizontal, modificationBox);
    modificationBoxButtons.addButton(QDialogButtonBox::Cancel);
    modificationBoxButtons.addButton(QString("Apply"), QDialogButtonBox::AcceptRole);
    modificationBoxLayout.addWidget(&modificationBoxButtons, modificationBoxLayout.rowCount(), 1, 1, 2);
    QObject::connect(&modificationBoxButtons, SIGNAL(accepted()), modificationBox, SLOT(accept()));
    QObject::connect(&modificationBoxButtons, SIGNAL(rejected()), modificationBox, SLOT(reject()));
    
    if (modificationBox->exec() == QDialog::Accepted && _mw && _mw->isSetSBMLDocument()) {
        RAVector* pY = new RAVector(absoluteValueSpinBox->value(), relativeValueSpinBox->value());
        
        if (_gO && _style) {
            VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gO, _style);
            
            if (!ne_rp_setY(ne_vrx_getRenderPoint(ne_rc_getVertex(ne_grp_getGeometricShape(ne_stl_getGroup(style), 0), _pointIndex)), pY) || !ne_rp_setY(ne_vrx_getRenderPoint(ne_plg_getVertex(ne_grp_getGeometricShape(ne_stl_getGroup(style), 0), _pointIndex)), pY)) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit styleChanged(style);
            }
        }
        else if (_lE) {
            VLineEnding* lineEnding = ne_ven_getLocalLineEndingFromGlobalLineEnding(_mw->getSBMLDocument()->getVeneer(), _lE);
            
            if (!ne_rp_setY(ne_vrx_getRenderPoint(ne_rc_getVertex(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), 0), _pointIndex)), pY) || !ne_rp_setY(ne_vrx_getRenderPoint(ne_plg_getVertex(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), 0), _pointIndex)), pY)) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit lEndingChanged(lineEnding);
            }
        }
    }
}

void PointFeatureMenu::changeBasePoint1X() {
    QDialog* modificationBox =  new QDialog();
    modificationBox->setWindowTitle("Change BasePoint1X of Point" + QString::number(_pointIndex + 1));
    QGridLayout modificationBoxLayout(modificationBox);
    modificationBoxLayout.addWidget(new QLabel("BasePoint1X"), modificationBoxLayout.rowCount() - 1, 0);
    
    QDoubleSpinBox* absoluteValueSpinBox = new QDoubleSpinBox(modificationBox);
    QSpinBox* relativeValueSpinBox = new QSpinBox(modificationBox);
    QString currentAbsoluteValue, currentRelativeValue;
    if (basePoint1XButton.text() != "N/A") {
        currentAbsoluteValue = basePoint1XButton.text().split("+").at(0);
        currentRelativeValue = basePoint1XButton.text().split("+").at(1).split("%").at(0);
    }
    
    absoluteValueSpinBox->setValue(currentAbsoluteValue.toDouble());
    absoluteValueSpinBox->setDecimals(1);
    absoluteValueSpinBox->setRange(0.0, _mw->getScene()->sceneRect().x() + _mw->getScene()->sceneRect().width());
    relativeValueSpinBox->setValue(currentRelativeValue.toInt());
    relativeValueSpinBox->setRange(0, 100);
    
    modificationBoxLayout.addWidget(new QLabel("Absolute Value:"), modificationBoxLayout.rowCount(), 0);
    modificationBoxLayout.addWidget(absoluteValueSpinBox, modificationBoxLayout.rowCount() - 1, 1);
    modificationBoxLayout.addWidget(new QLabel("Relative Value:"), modificationBoxLayout.rowCount(), 0);
    modificationBoxLayout.addWidget(relativeValueSpinBox, modificationBoxLayout.rowCount() - 1, 1);
    modificationBoxLayout.addWidget(new QLabel("%"), modificationBoxLayout.rowCount() - 1, 2);
    
    QDialogButtonBox modificationBoxButtons(Qt::Horizontal, modificationBox);
    modificationBoxButtons.addButton(QDialogButtonBox::Cancel);
    modificationBoxButtons.addButton(QString("Apply"), QDialogButtonBox::AcceptRole);
    modificationBoxLayout.addWidget(&modificationBoxButtons, modificationBoxLayout.rowCount(), 1, 1, 2);
    QObject::connect(&modificationBoxButtons, SIGNAL(accepted()), modificationBox, SLOT(accept()));
    QObject::connect(&modificationBoxButtons, SIGNAL(rejected()), modificationBox, SLOT(reject()));
    
    if (modificationBox->exec() == QDialog::Accepted && _mw && _mw->isSetSBMLDocument()) {
        RAVector* bp1X = new RAVector(absoluteValueSpinBox->value(), relativeValueSpinBox->value());
        
        if (_gO && _style) {
            VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gO, _style);
            
            if (!ne_rp_setX(ne_vrx_getBasePoint1(ne_rc_getVertex(ne_grp_getGeometricShape(ne_stl_getGroup(style), 0), _pointIndex)), bp1X) || !ne_rp_setX(ne_vrx_getBasePoint1(ne_plg_getVertex(ne_grp_getGeometricShape(ne_stl_getGroup(style), 0), _pointIndex)), bp1X)) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit styleChanged(style);
            }
        }
        else if (_lE) {
            VLineEnding* lineEnding = ne_ven_getLocalLineEndingFromGlobalLineEnding(_mw->getSBMLDocument()->getVeneer(), _lE);
            
            if (!ne_rp_setX(ne_vrx_getBasePoint1(ne_rc_getVertex(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), 0), _pointIndex)), bp1X) || !ne_rp_setX(ne_vrx_getBasePoint1(ne_plg_getVertex(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), 0), _pointIndex)), bp1X)) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit lEndingChanged(lineEnding);
            }
        }
    }
}

void PointFeatureMenu::changeBasePoint1Y() {
    QDialog* modificationBox =  new QDialog();
    modificationBox->setWindowTitle("Change BasePoint1Y of Point" + QString::number(_pointIndex + 1));
    QGridLayout modificationBoxLayout(modificationBox);
    modificationBoxLayout.addWidget(new QLabel("BasePoint1Y"), modificationBoxLayout.rowCount() - 1, 0);
    
    QDoubleSpinBox* absoluteValueSpinBox = new QDoubleSpinBox(modificationBox);
    QSpinBox* relativeValueSpinBox = new QSpinBox(modificationBox);
    QString currentAbsoluteValue, currentRelativeValue;
    if (basePoint1YButton.text() != "N/A") {
        currentAbsoluteValue = basePoint1YButton.text().split("+").at(0);
        currentRelativeValue = basePoint1YButton.text().split("+").at(1).split("%").at(0);
    }
    
    absoluteValueSpinBox->setValue(currentAbsoluteValue.toDouble());
    absoluteValueSpinBox->setDecimals(1);
    absoluteValueSpinBox->setRange(0.0, _mw->getScene()->sceneRect().y() + _mw->getScene()->sceneRect().height());
    relativeValueSpinBox->setValue(currentRelativeValue.toInt());
    relativeValueSpinBox->setRange(0, 100);
    
    modificationBoxLayout.addWidget(new QLabel("Absolute Value:"), modificationBoxLayout.rowCount(), 0);
    modificationBoxLayout.addWidget(absoluteValueSpinBox, modificationBoxLayout.rowCount() - 1, 1);
    modificationBoxLayout.addWidget(new QLabel("Relative Value:"), modificationBoxLayout.rowCount(), 0);
    modificationBoxLayout.addWidget(relativeValueSpinBox, modificationBoxLayout.rowCount() - 1, 1);
    modificationBoxLayout.addWidget(new QLabel("%"), modificationBoxLayout.rowCount() - 1, 2);
    
    QDialogButtonBox modificationBoxButtons(Qt::Horizontal, modificationBox);
    modificationBoxButtons.addButton(QDialogButtonBox::Cancel);
    modificationBoxButtons.addButton(QString("Apply"), QDialogButtonBox::AcceptRole);
    modificationBoxLayout.addWidget(&modificationBoxButtons, modificationBoxLayout.rowCount(), 1, 1, 2);
    QObject::connect(&modificationBoxButtons, SIGNAL(accepted()), modificationBox, SLOT(accept()));
    QObject::connect(&modificationBoxButtons, SIGNAL(rejected()), modificationBox, SLOT(reject()));
    
    if (modificationBox->exec() == QDialog::Accepted && _mw && _mw->isSetSBMLDocument()) {
        RAVector* bp1Y = new RAVector(absoluteValueSpinBox->value(), relativeValueSpinBox->value());
        
        if (_gO && _style) {
            VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gO, _style);
            
            if (!ne_rp_setY(ne_vrx_getBasePoint1(ne_rc_getVertex(ne_grp_getGeometricShape(ne_stl_getGroup(style), 0), _pointIndex)), bp1Y) || !ne_rp_setY(ne_vrx_getBasePoint1(ne_plg_getVertex(ne_grp_getGeometricShape(ne_stl_getGroup(style), 0), _pointIndex)), bp1Y)) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit styleChanged(style);
            }
        }
        else if (_lE) {
            VLineEnding* lineEnding = ne_ven_getLocalLineEndingFromGlobalLineEnding(_mw->getSBMLDocument()->getVeneer(), _lE);
            
            if (!ne_rp_setY(ne_vrx_getBasePoint1(ne_rc_getVertex(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), 0), _pointIndex)), bp1Y) || !ne_rp_setY(ne_vrx_getBasePoint1(ne_plg_getVertex(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), 0), _pointIndex)), bp1Y)) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit lEndingChanged(lineEnding);
            }
        }
    }
}

void PointFeatureMenu::changeBasePoint2X() {
    QDialog* modificationBox =  new QDialog();
    modificationBox->setWindowTitle("Change BasePoint2X of Point" + QString::number(_pointIndex + 1));
    QGridLayout modificationBoxLayout(modificationBox);
    modificationBoxLayout.addWidget(new QLabel("BasePoint2X"), modificationBoxLayout.rowCount() - 1, 0);
    
    QDoubleSpinBox* absoluteValueSpinBox = new QDoubleSpinBox(modificationBox);
    QSpinBox* relativeValueSpinBox = new QSpinBox(modificationBox);
    QString currentAbsoluteValue, currentRelativeValue;
    if (basePoint2XButton.text() != "N/A") {
        currentAbsoluteValue = basePoint2XButton.text().split("+").at(0);
        currentRelativeValue = basePoint2XButton.text().split("+").at(1).split("%").at(0);
    }
    
    absoluteValueSpinBox->setValue(currentAbsoluteValue.toDouble());
    absoluteValueSpinBox->setDecimals(1);
    absoluteValueSpinBox->setRange(0.0, _mw->getScene()->sceneRect().x() + _mw->getScene()->sceneRect().width());
    relativeValueSpinBox->setValue(currentRelativeValue.toInt());
    relativeValueSpinBox->setRange(0, 100);
    
    modificationBoxLayout.addWidget(new QLabel("Absolute Value:"), modificationBoxLayout.rowCount(), 0);
    modificationBoxLayout.addWidget(absoluteValueSpinBox, modificationBoxLayout.rowCount() - 1, 1);
    modificationBoxLayout.addWidget(new QLabel("Relative Value:"), modificationBoxLayout.rowCount(), 0);
    modificationBoxLayout.addWidget(relativeValueSpinBox, modificationBoxLayout.rowCount() - 1, 1);
    modificationBoxLayout.addWidget(new QLabel("%"), modificationBoxLayout.rowCount() - 1, 2);
    
    QDialogButtonBox modificationBoxButtons(Qt::Horizontal, modificationBox);
    modificationBoxButtons.addButton(QDialogButtonBox::Cancel);
    modificationBoxButtons.addButton(QString("Apply"), QDialogButtonBox::AcceptRole);
    modificationBoxLayout.addWidget(&modificationBoxButtons, modificationBoxLayout.rowCount(), 1, 1, 2);
    QObject::connect(&modificationBoxButtons, SIGNAL(accepted()), modificationBox, SLOT(accept()));
    QObject::connect(&modificationBoxButtons, SIGNAL(rejected()), modificationBox, SLOT(reject()));
    
    if (modificationBox->exec() == QDialog::Accepted && _mw && _mw->isSetSBMLDocument()) {
        RAVector* bp2X = new RAVector(absoluteValueSpinBox->value(), relativeValueSpinBox->value());
        
        if (_gO && _style) {
            VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gO, _style);
            
            if (!ne_rp_setX(ne_vrx_getBasePoint2(ne_rc_getVertex(ne_grp_getGeometricShape(ne_stl_getGroup(style), 0), _pointIndex)), bp2X) || !ne_rp_setX(ne_vrx_getBasePoint2(ne_plg_getVertex(ne_grp_getGeometricShape(ne_stl_getGroup(style), 0), _pointIndex)), bp2X)) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit styleChanged(style);
            }
        }
        else if (_lE) {
            VLineEnding* lineEnding = ne_ven_getLocalLineEndingFromGlobalLineEnding(_mw->getSBMLDocument()->getVeneer(), _lE);
            
            if (!ne_rp_setX(ne_vrx_getBasePoint2(ne_rc_getVertex(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), 0), _pointIndex)), bp2X) || !ne_rp_setX(ne_vrx_getBasePoint2(ne_plg_getVertex(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), 0), _pointIndex)), bp2X)) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit lEndingChanged(lineEnding);
            }
        }
    }
}

void PointFeatureMenu::changeBasePoint2Y() {
    QDialog* modificationBox =  new QDialog();
    modificationBox->setWindowTitle("Change BasePoint2Y of Point" + QString::number(_pointIndex + 1));
    QGridLayout modificationBoxLayout(modificationBox);
    modificationBoxLayout.addWidget(new QLabel("BasePoint2Y"), modificationBoxLayout.rowCount() - 1, 0);
    
    QDoubleSpinBox* absoluteValueSpinBox = new QDoubleSpinBox(modificationBox);
    QSpinBox* relativeValueSpinBox = new QSpinBox(modificationBox);
    QString currentAbsoluteValue, currentRelativeValue;
    if (basePoint2YButton.text() != "N/A") {
        currentAbsoluteValue = basePoint2YButton.text().split("+").at(0);
        currentRelativeValue = basePoint2YButton.text().split("+").at(1).split("%").at(0);
    }
    
    absoluteValueSpinBox->setValue(currentAbsoluteValue.toDouble());
    absoluteValueSpinBox->setDecimals(1);
    absoluteValueSpinBox->setRange(0.0, _mw->getScene()->sceneRect().y() + _mw->getScene()->sceneRect().height());
    relativeValueSpinBox->setValue(currentRelativeValue.toInt());
    relativeValueSpinBox->setRange(0, 100);
    
    modificationBoxLayout.addWidget(new QLabel("Absolute Value:"), modificationBoxLayout.rowCount(), 0);
    modificationBoxLayout.addWidget(absoluteValueSpinBox, modificationBoxLayout.rowCount() - 1, 1);
    modificationBoxLayout.addWidget(new QLabel("Relative Value:"), modificationBoxLayout.rowCount(), 0);
    modificationBoxLayout.addWidget(relativeValueSpinBox, modificationBoxLayout.rowCount() - 1, 1);
    modificationBoxLayout.addWidget(new QLabel("%"), modificationBoxLayout.rowCount() - 1, 2);
    
    QDialogButtonBox modificationBoxButtons(Qt::Horizontal, modificationBox);
    modificationBoxButtons.addButton(QDialogButtonBox::Cancel);
    modificationBoxButtons.addButton(QString("Apply"), QDialogButtonBox::AcceptRole);
    modificationBoxLayout.addWidget(&modificationBoxButtons, modificationBoxLayout.rowCount(), 1, 1, 2);
    QObject::connect(&modificationBoxButtons, SIGNAL(accepted()), modificationBox, SLOT(accept()));
    QObject::connect(&modificationBoxButtons, SIGNAL(rejected()), modificationBox, SLOT(reject()));
    
    if (modificationBox->exec() == QDialog::Accepted && _mw && _mw->isSetSBMLDocument()) {
        RAVector* bp2Y = new RAVector(absoluteValueSpinBox->value(), relativeValueSpinBox->value());
        
        if (_gO && _style) {
            VLocalStyle* style = ne_ven_getLocalStyleFromGlobalStyle(_mw->getSBMLDocument()->getVeneer(), _gO, _style);
            
            if (!ne_rp_setY(ne_vrx_getBasePoint2(ne_rc_getVertex(ne_grp_getGeometricShape(ne_stl_getGroup(style), 0), _pointIndex)), bp2Y) || !ne_rp_setY(ne_vrx_getBasePoint2(ne_plg_getVertex(ne_grp_getGeometricShape(ne_stl_getGroup(style), 0), _pointIndex)), bp2Y)) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit styleChanged(style);
            }
        }
        else if (_lE) {
            VLineEnding* lineEnding = ne_ven_getLocalLineEndingFromGlobalLineEnding(_mw->getSBMLDocument()->getVeneer(), _lE);
            
            if (!ne_rp_setY(ne_vrx_getBasePoint2(ne_rc_getVertex(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), 0), _pointIndex)), bp2Y) || !ne_rp_setY(ne_vrx_getBasePoint2(ne_plg_getVertex(ne_grp_getGeometricShape(ne_le_getGroup(lineEnding), 0), _pointIndex)), bp2Y)) {
                _mw->getSBMLDocument()->setRenderModified(true);
                emit lEndingChanged(lineEnding);
            }
        }
    }
}

// Collapsable Menu

CollapsibleMenu::CollapsibleMenu(QWidget* parent, const QString& title, const int& animationDuration) : QWidget(parent) {
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    
    _animationDuration = animationDuration;
    
    _toggleButton.setStyleSheet("QToolButton { border: none; font-size: 13px;}");
    _toggleButton.setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    _toggleButton.setArrowType(Qt::ArrowType::RightArrow);
    _toggleButton.setIconSize(QSize(8, 11));
    _toggleButton.setText(title);
    _toggleButton.setCheckable(true);
    _toggleButton.setChecked(false);
    
    _headerLine.setFrameShape(QFrame::HLine);
    _headerLine.setFrameShadow(QFrame::Sunken);
    _headerLine.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    
    _contentArea.setStyleSheet("QScrollArea { background-color: white; border-radius: 4px; }");
    _contentArea.setWidgetResizable(true);
    _contentArea.setWidget(new QWidget());
    _contentArea.widget()->setStyleSheet("background-color: white; border-radius: 4px; ");
    _contentArea.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    
    // start out collapsed
    _contentArea.setMaximumHeight(0);
    _contentArea.setMinimumHeight(0);
    
     // let the entire widget grow and shrink with its content
    _toggleAnimation.addAnimation(new QPropertyAnimation(this, "minimumHeight"));
    _toggleAnimation.addAnimation(new QPropertyAnimation(this, "maximumHeight"));
    _toggleAnimation.addAnimation(new QPropertyAnimation(&_contentArea, "maximumHeight"));
    
    // don't waste space
    _mainLayout.setVerticalSpacing(0);
    _mainLayout.setContentsMargins(0, 0, 0, 0);
    
    _mainLayout.addWidget(&_toggleButton, _mainLayout.rowCount() - 1, 0, 1, 1, Qt::AlignLeft);
    _mainLayout.addWidget(&_headerLine, _mainLayout.rowCount() - 1, 2, 1, 1);
    _mainLayout.addWidget(&_contentArea, _mainLayout.rowCount(), 0, 1, 3);
    setLayout(&_mainLayout);
    
    QObject::connect(&_toggleButton, &QToolButton::clicked, [this](const bool checked) {
        _toggleButton.setArrowType(checked ? Qt::ArrowType::DownArrow : Qt::ArrowType::RightArrow);
        _toggleAnimation.setDirection(checked ? QAbstractAnimation::Forward : QAbstractAnimation::Backward);
        _toggleAnimation.start();
    });
}

void CollapsibleMenu::setContentLayout(QLayout& contentLayout) {
    delete _contentArea.widget()->layout();
    //contentLayout.setSizeConstraint(QLayout::SetFixedSize);
    _contentArea.widget()->setLayout(&contentLayout);
    setFixedSize(QSize(235, maximumHeight()));
    
    qreal collapsedHeight = sizeHint().height() - _contentArea.maximumHeight();
    qreal contentHeight = contentLayout.sizeHint().height();
    
    for (int i = 0; i < _toggleAnimation.animationCount() - 1; ++i) {
        QPropertyAnimation* menuAnimation = static_cast<QPropertyAnimation *>(_toggleAnimation.animationAt(i));
        menuAnimation->setDuration(_animationDuration);
        menuAnimation->setStartValue(collapsedHeight);
        menuAnimation->setEndValue(collapsedHeight + contentHeight);
    }
    
    QPropertyAnimation* contentAnimation = static_cast<QPropertyAnimation *>(_toggleAnimation.animationAt(_toggleAnimation.animationCount() - 1));
    contentAnimation->setDuration(_animationDuration);
    contentAnimation->setStartValue(0);
    contentAnimation->setEndValue(contentHeight);
}

void CollapsibleMenu::collapseMenuWithoutAnimation() {
    _toggleButton.setArrowType(Qt::ArrowType::RightArrow);
    setMinimumHeight(0);
    _contentArea.setMaximumHeight(0);
    _toggleButton.setChecked(false);
}

SeparatingLine::SeparatingLine(QWidget* parent) : QFrame(parent) {
    setFrameShape(QFrame::HLine);
    setFrameShadow(QFrame::Sunken);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setStyleSheet(QString("background-color: #808080"));
}
