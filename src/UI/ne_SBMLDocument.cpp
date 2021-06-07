#include "ne_SBMLDocument.h"

void NESBMLDocument::loadSBML(MainWindow* mw){
    // get document from by reading the current file
    _document = ne_doc_readSBML(mw->getCurrentFileName().toStdString().c_str());

    // get the layout info from the document and display it
    loadLayoutInfo(mw);
    
    // get the render info from the document and display it
    loadRenderInfo(mw);
}

void NESBMLDocument::loadLayoutInfo(MainWindow *mw){
    
    //get layout information from the SBML document
    _li = ne_doc_processLayoutInfo(getDocument());
    
    if (ne_li_getNetwork(getLayoutInfo())) {
        // if layout is not specified
        if (!ne_net_isLayoutSpecified(getNetwork())) {
                    
            // implement layout algorithm
            ne_li_addLayoutFeaturesToNetowrk(_li);
            
            // set the layout modification flag as true
            setLayoutModified(true);
        }
        // if layout has already specified
        else
            setLayoutAlreadyExisted(true);
        
        // if layout is now specified
        if (getNetwork()->isLayoutSpecified()) {
            
            // compartments
            NCompartment* c = NULL;
            GraphicalCompartment* _compInfo = NULL;
            GraphicalText* _graphicalText = NULL;
            for (int i = 0; i < ne_net_getNumCompartments(getNetwork()); ++i) {
                c = ne_net_getCompartment(getNetwork(), i);
                
                if (ne_go_isSetGlyphId(c)) {
                    // create a new graphical compartment
                    _compInfo = new GraphicalCompartment();
                    
                    // set the compartment of graphical compartment
                    _compInfo->setCompartment(c);
                    
                    // set the id of graphical compartment
                    _compInfo->setId(ne_go_getGlyphId(c));
                    
                    // set the graphical text of graphical compartment
                    _graphicalText =  new GraphicalText();
                    
                    // set the compartment as the graphical object of graphical text
                    _graphicalText->setGObject(c);
                    
                    // set text glyph
                    if (ne_go_isSetText(c)) {
                        _graphicalText->setText(ne_go_getText(c));
                        
                        if (ne_go_isSetGlyphId(ne_go_getText(c)))
                            _graphicalText->setId(ne_go_getGlyphId(ne_go_getText(c)));
                    }
                    
                    // set the graphical text of graphical compartment
                    _compInfo->setGText(_graphicalText);
                    
                    // add the graphical compartment to the list of graphical compartments
                    mw->addGCompartment(_compInfo);
                }
            }
            
            // species and texts
            NSpecies* s = NULL;
            GraphicalSpecies* _speciesInfo = NULL;
            _graphicalText = NULL;
            for (int i = 0; i < ne_net_getNumSpecies(getNetwork()); ++i) {
                s = ne_net_getSpecies(getNetwork(), i);
                
                if (ne_go_isSetGlyphId(s)) {
                    // create a new graphical species
                    _speciesInfo = new GraphicalSpecies();
                    
                    // set the species of graphical speceis
                    _speciesInfo->setSpecies(s);
                    
                    // set the id of graphical species
                    _speciesInfo->setId(ne_go_getGlyphId(s));
                    
                    // set the role of graphical species
                    //if (s->isSetObjectRole())
                        //_speciesInfo->setRole(s->getObjectRole());
                    
                    // set the graphical text of graphical species
                    GraphicalText* _graphicalText =  new GraphicalText();
                    
                    // set the species as the graphical object of graphical text
                    _graphicalText->setGObject(s);
                    
                    // set text glyph
                    if (ne_go_isSetText(s)) {
                        _graphicalText->setText(ne_go_getText(s));
                        
                        if (ne_go_isSetGlyphId(ne_go_getText(s)))
                            _graphicalText->setId(ne_go_getGlyphId(ne_go_getText(s)));
                    }
                    else {
                        // set plain text
                        if (ne_ne_isSetName(s))
                            _graphicalText->setPlainText(ne_ne_getName(s));
                        else
                            _graphicalText->setPlainText(ne_ne_getId(s));
                    }
                    
                    // set the graphical text of graphical species
                    _speciesInfo->setGText(_graphicalText);

                    // add the graphical species to the list of graphical species
                    mw->addGSpecies(_speciesInfo);
                }
            }
            
            // reactions
            NReaction* r = NULL;
            GraphicalReaction* _reactionInfo = NULL;
            for (int i = 0; i < ne_net_getNumReactions(getNetwork()); ++i) {
                r = ne_net_getReaction(getNetwork(), i);
                
                if (ne_go_isSetGlyphId(r)) {
                    // create a new graphical reaction
                    _reactionInfo = new GraphicalReaction();
                    
                    // set the reaction of graphical reaction
                    _reactionInfo->setReaction(r);
                    
                    // set the id of graphical reaction
                    _reactionInfo->setId(ne_go_getGlyphId(r));
                    
                    // set the graphical curve of graphical reaction
                    GraphicalCurve* _graphicalCurve1 = new GraphicalCurve();
                    
                    // set the graphical curve of graphical reaction
                    _reactionInfo->setGCurve(_graphicalCurve1);
                        
                    // get species references
                    NSpeciesReference* sr = NULL;
                    GraphicalSReference* _sReferenceInfo = NULL;
                    for (int j = 0; j < ne_rxn_getNumSpeciesReferences(r); ++j) {
                        sr = ne_rxn_getSpeciesReference(r, j);
                        
                        if (ne_go_isSetGlyphId(sr)) {
                            // create a new graphical species reference
                            _sReferenceInfo = new GraphicalSReference();
                            
                            // set the species reference of graphical species reference
                            _sReferenceInfo->setSReference(sr);
                            
                            // set the id of graphical species reference
                            _sReferenceInfo->setId(ne_go_getGlyphId(sr));
                            
                            // set the role of graphical species reference
                            if (ne_sr_isSetRole(sr))
                                _sReferenceInfo->setRole(ne_sr_getRoleAsString(sr));
                            
                            // set the graphical curve of graphical species reference
                            GraphicalCurve* _graphicalCurve2 = new GraphicalCurve();
                            
                            // set curve
                            if (ne_sr_isSetCurve(sr)) {
                                _graphicalCurve2->setLCurve(ne_sr_getCurve(sr));
                                
                                if (ne_ne_isSetId(ne_sr_getCurve(sr)))
                                    _graphicalCurve2->setId(ne_ne_getId(ne_sr_getCurve(sr)));
                            }
                            
                            // set the graphical curve of graphical species reference
                            _sReferenceInfo->setGCurve(_graphicalCurve2);
                            
                            // add the graphical species reference to the list of graphical species references
                            _reactionInfo->addGSReference(_sReferenceInfo);
                        }
                    }
                    
                    // add the graphical reaction to the list of graphical reactions
                    mw->addGReaction(_reactionInfo);
                }
            }
        }
    }
    else {
        QMessageBox* failedModelMessageBox = new QMessageBox();
        failedModelMessageBox->setWindowTitle("Failed");
        failedModelMessageBox->setText("Cannot load the model!");
        failedModelMessageBox->setIcon(QMessageBox::Warning);
        failedModelMessageBox->exec();
    }
}

void NESBMLDocument::loadRenderInfo(MainWindow *mw){
    if (getNetwork()) {
        // get render information from the SBML document
        _ri = ne_doc_processRenderInfo(getDocument());
        
        // if veneer does not exist
        if (!ne_ven_isRenderSpecified(getVeneer())) {
            // add default values to veneer
            ne_ri_addDefaultRenderFeaturesToVeneer(getRenderInfo());
            
            // enable the flag of render modification
            setRenderModified(true);
        }
        // if render has already specified
        else
            setRenderAlreadyExisted(true);
        
        // if veneer exists
        if (ne_ven_isRenderSpecified(getVeneer())) {
            
            // get colors from veneer
            GraphicalColor *_gColor = NULL;
            for (int i = 0; i < ne_ven_getNumColors(getVeneer()); ++i) {
                // create a new graphical color
                _gColor = new GraphicalColor();
                
                // get info from veneer color
                getInfoFromColor(_gColor, ne_ven_getColor(getVeneer(), i));
                
                // store the graphical color
                mw->addGColor(_gColor);
            }
            
            // add default colors
            MainWindow::constGColorIt gCIt;
            for (int i = 0; i < NUM_ColorString; ++i) {
                for (gCIt = mw->gColorsBegin(); gCIt != mw->gColorsEnd(); ++gCIt) {
                    if ((*gCIt)->isSetId() && stringCompare((*gCIt)->getId(), colorStringToStr(ColorString(i))))
                        break;
                }
                
                // if deafult color does not exists in the veneer colors list
                if (gCIt == mw->gColorsEnd()) {
                    // create a new graphical color
                    _gColor = new GraphicalColor();
                    
                    // get info from veneer color
                    getInfoFromColor(_gColor, NULL, ColorString(i));
                    
                    // store the graphical color
                    mw->addGColor(_gColor);
                }
            }
            
            // get the background color of the veneer
            if (ne_ven_isSetBackgroundColor(getVeneer()))
                mw->getScene()->setBackgroundBrush(QBrush(getQColor(mw, ne_ven_getBackgroundColor(getVeneer()))));
            
            // get gradients from veneer
            GraphicalGradient *_gGradient = NULL;
            for (int i = 0; i < ne_ven_getNumGradients(getVeneer()); ++i) {
                // create a new graphical gradient
                _gGradient = new GraphicalGradient();
                
                // get info from veneer gradient
                getInfoFromGradient(mw, ne_ven_getGradient(getVeneer(), i), _gGradient);
                
                // store the graphcial gradient
                mw->addGGradient(_gGradient);
            }

            // get line endings from vneeer
            GraphicalLineEnding *_gLEnding = NULL;
            for (int i = 0; i < ne_ven_getNumLineEndings(getVeneer()); ++i) {
                // create a new graphical line ending
                _gLEnding = new GraphicalLineEnding();
                
                // get info from vneeer line ending
                getInfoFromLineEnding(mw, ne_ven_getLineEnding(getVeneer(), i), _gLEnding);
                
                // store the graphical line ending
                mw->addGLEnding(_gLEnding);
            }
            
            // get compartments info from veneer
            for (MainWindow::constGCompartmentIt cIt = mw->gCompartmentsBegin(); cIt != mw->gCompartmentsEnd(); ++cIt){
                
                // find the style in veneer
                (*cIt)->setStyle(mw, false);
                
                // get graphical compartment values
                (*cIt)->updateValues(mw);
            }
            
            // get species info from veneer
            for (MainWindow::constGSpeciesIt sIt = mw->gSpeciesBegin(); sIt != mw->gSpeciesEnd(); ++sIt) {
                // find the style in veneer
                (*sIt)->setStyle(mw, false);
                
                // get graphical species values
                if (isLayoutAlreadyExisted())
                    (*sIt)->updateValues(mw, false);
                else
                    (*sIt)->updateValues(mw, true);
            }
            
            // get reactions info from veneer
            for (MainWindow::constGReactionIt rIt = mw->gReactionsBegin(); rIt != mw->gReactionsEnd(); ++rIt) {
                
                // find the style in veneer
                (*rIt)->setStyle(mw, false);
                
                // get graphical reaction values
                (*rIt)->updateValues(mw);

                // get species references
                for (GraphicalReaction::constGSReferenceIt sRIt = (*rIt)->gSReferencesBegin(); sRIt != (*rIt)->gSReferencesEnd(); ++sRIt) {
                    // find the style in veneer
                    (*sRIt)->setStyle(mw, false);
                    
                    // get graphical species reference values
                    (*sRIt)->updateValues(mw);
                }
            }
        }
    }
}

bool NESBMLDocument::saveSBML(MainWindow *mw, const QString& fileName) {
    // if layout features are modified
    if (_isLayoutModified) {
        _document = ne_doc_populateSBMLdocWithLayoutInfo(_document, _li);
        _isLayoutModified = false;
    }
    
    // if render features are modified
    if (_isRenderModified) {
        _document = ne_doc_populateSBMLdocWithRenderInfo(_document, _ri);
        _isRenderModified = false;
    }
    
    if (!ne_doc_writeSBML(_document, fileName.toStdString().c_str()))
        return true;
    
    return false;
}

void NESBMLDocument::setDocument(SBMLDocument* document) {
    _document = document;
    _isSetDocument = true;
}

SBMLDocument* NESBMLDocument::getDocument() {
    return _document;
}

LayoutInfo* NESBMLDocument::getLayoutInfo() {
    return _li;
}

RenderInfo* NESBMLDocument::getRenderInfo() {
    return _ri;
}

Network* NESBMLDocument::getNetwork() {
    return (Network*)(_li->net);
}

Veneer* NESBMLDocument::getVeneer() {
    return (Veneer*)(_ri->ven);
}

void NESBMLDocument::setLayoutModified(const bool& isLayoutModified) {
    _isLayoutModified = isLayoutModified;
}

void NESBMLDocument::setLayoutAlreadyExisted(const bool& isLayoutAlreadyExisted) {
    _isLayoutAlreadyExisted = isLayoutAlreadyExisted;
}

void NESBMLDocument::setRenderModified(const bool& isRenderModified) {
    _isRenderModified = isRenderModified;
}

void NESBMLDocument::setRenderAlreadyExisted(const bool& isRenderAlreadyExisted) {
    _isRenderAlreadyExisted = isRenderAlreadyExisted;
}

const QColor getQColor(MainWindow *mw, const std::string& colorId) {
    if (colorId.front() == '#')
        return QColor(colorId.c_str());
    
    for (MainWindow::constGColorIt gCIt = mw->gColorsBegin(); gCIt != mw->gColorsEnd(); ++gCIt) {
        if (colorId != "none" && stringCompare ((*gCIt)->getId(), colorId))
                   return (*gCIt)->getGraphicalColor();
    }
    
    // not found
    return QColor(Qt::transparent);
}

GraphicalGradient* findGraphicalGradient(MainWindow *mw, const std::string& gradientId) {
    for (MainWindow::constGGradientIt gGIt = mw->gGradientsBegin(); gGIt != mw->gGradientsEnd(); ++gGIt) {
        if (stringCompare((*gGIt)->getId(), gradientId))
            return (*gGIt);
    }
    
    // not found
    return NULL;
}

QGraphicsItem* findLineEnding(MainWindow *mw, const std::string& lineEndingId, const QPen& curvePen, bool& enableRotation){
    
    // search among all the line endings to find the one with the same id
    for (int i = 0; i < mw->getNumGLEndings(); ++i) {
        
        // if the the line ending with the same id is found
        if (stringCompare(mw->getLEndingInfo().at(i)->getId(), lineEndingId)) {
            
            // get enable rotation status of the line ending
            enableRotation = mw->getLEndingInfo().at(i)->getRotation();
            
            // make a copy of the graphical line ending
            QGraphicsItem* gItem;
            
            switch (mw->getLEndingInfo().at(i)->getGraphicalItem()->type()) {
                case 2:
                    // create a new qpainter path item
                    gItem = new QGraphicsPathItem(((QGraphicsPathItem *)(mw->getLEndingInfo().at(i)->getGraphicalItem()))->path());
                    
                    // set the pen of graphical path item using the pen of the curve it is assigned to
                    ((QGraphicsPathItem *)gItem)->setPen(curvePen);
                    
                    // set the brush of graphical path item
                    ((QGraphicsPathItem *)gItem)->setBrush(((QGraphicsPathItem *)(mw->getLEndingInfo().at(i)->getGraphicalItem()))->brush());
                    
                    return gItem;
                    
                case 3:
                    // create a new graphics rect item
                    gItem = new QGraphicsRectItem();
                    
                    // set the pen of graphics rect item using the pen of the curve it is assigned to
                    ((QGraphicsRectItem *)gItem)->setPen(curvePen);
                    
                    // set the brush of graphics rect item
                    ((QGraphicsRectItem *)gItem)->setBrush(((QGraphicsRectItem *)(mw->getLEndingInfo().at(i)->getGraphicalItem()))->brush());
                    
                    // set the bounding rect of graphics rect item
                    ((QGraphicsRectItem *)gItem)->setRect(((QGraphicsRectItem *)(mw->getLEndingInfo().at(i)->getGraphicalItem()))->rect());
                    
                    return gItem;
                    
                case 4:
                    // create a new graphics ellipse item
                    gItem = new QGraphicsEllipseItem();
                    
                    // set the pen of graphics ellipse item using the pen of the curve it is assigned to
                    ((QGraphicsEllipseItem *)gItem)->setPen(curvePen);
                    
                    // set the brush of graphics ellipse item
                    ((QGraphicsEllipseItem *)gItem)->setBrush(((QGraphicsEllipseItem *)(mw->getLEndingInfo().at(i)->getGraphicalItem()))->brush());
                    
                    // set the bounding rect of graphics ellipse item
                    ((QGraphicsEllipseItem *)gItem)->setRect(((QGraphicsEllipseItem *)(mw->getLEndingInfo().at(i)->getGraphicalItem()))->rect());
                    
                    return gItem;
                    
                case 5:
                    // create a new graphics polygon item
                    gItem = new QGraphicsPolygonItem();
                    
                    // set the pen of graphics polygon item using the pen of the curve it is assigned to
                    ((QGraphicsPolygonItem *)gItem)->setPen(curvePen);
                    
                    // set the brush of graphics polygon item
                    ((QGraphicsPolygonItem *)gItem)->setBrush(((QGraphicsPolygonItem *)(mw->getLEndingInfo().at(i)->getGraphicalItem()))->brush());
                    
                    // set the fill rule of graphics polygon item
                    ((QGraphicsPolygonItem *)gItem)->setFillRule(((QGraphicsPolygonItem *)(mw->getLEndingInfo().at(i)->getGraphicalItem()))->fillRule());
                    
                    // set the polygon of graphics polygon item
                    ((QGraphicsPolygonItem *)gItem)->setPolygon(((QGraphicsPolygonItem *)(mw->getLEndingInfo().at(i)->getGraphicalItem()))->polygon());
                    
                    return gItem;
                    
                case 7:
                    gItem = new QGraphicsPixmapItem(((QGraphicsPixmapItem *)(mw->getLEndingInfo().at(i)->getGraphicalItem()))->pixmap());
                    ((QGraphicsPixmapItem*)(gItem))->setPos(((QGraphicsPixmapItem *)(mw->getLEndingInfo().at(i)->getGraphicalItem()))->x(), ((QGraphicsPixmapItem *)(mw->getLEndingInfo().at(i)->getGraphicalItem()))->y());
                    
                    return gItem;
            }
        }
    }
    
    return NULL;
}

void getInfoFromColor(GraphicalColor* gColor, VColorDefinition* color, const ColorString colorString) {
    if (color) {
        // set the id of graphical color using color id
        if (ne_ve_isSetId(color))
            gColor->setId(ne_ve_getId(color));
        
        // set the color of graphical color
        if (ne_clr_isSetValue(color))
            gColor->setGraphicalColor(QColor(ne_clr_getValue(color).c_str()));
        
        // set the colordefinition of graphical color
        gColor->setColor(color);
    }
    else if (colorString < NUM_ColorString) {
        // set the graphical color of graphical color
        gColor->setGraphicalColor(QColor(colorStringToHexStr(colorString).c_str()));
        
        // set the id of graphical color using color id
        gColor->setId(colorStringToStr(colorString));
    }
}

void getInfoFromGradient(MainWindow* mw, VGradientBase* gradient, GraphicalGradient* gGradient){
    QGradient* _graphicalGradient = NULL;
            
    // if it is a linear gradient
    if (ne_grd_isLinearGradient(gradient)) {
        // create a qlinear gradient
        _graphicalGradient = new QLinearGradient();
        RAVector* rAVX = NULL;
        RAVector* rAVY = NULL;
        
        // get the value of start point
        rAVX = ne_grd_getX1(gradient);
        rAVY = ne_grd_getY1(gradient);
        
        // set the start point of linear gradient
        if (rAVX && rAVY)
            ((QLinearGradient*)(_graphicalGradient))->setStart(ne_rav_getAbsoluteValue(rAVX), ne_rav_getAbsoluteValue(rAVY));
        
        // get the value of start point
        rAVX = ne_grd_getX2(gradient);
        rAVY = ne_grd_getY2(gradient);
        
        // set the start point of linear gradient
        if (rAVX && rAVY)
            ((QLinearGradient*)(_graphicalGradient))->setFinalStop(ne_rav_getAbsoluteValue(rAVX), ne_rav_getAbsoluteValue(rAVY));
    }
               
    // if it is a radial gradient
    else if (ne_grd_isRadialGradient(gradient)) {
        // create a qradial gradient
        _graphicalGradient = new QRadialGradient();
        RAVector* rAVX = NULL;
        RAVector* rAVY = NULL;
        
        // get the value of center point
        rAVX = ne_grd_getCx(gradient);
        rAVY = ne_grd_getCy(gradient);
        
        // set the center point of radial gradient
        if (rAVX && rAVY)
            ((QRadialGradient*)(_graphicalGradient))->setCenter(ne_rav_getAbsoluteValue(rAVX), ne_rav_getAbsoluteValue(rAVY));
        
        // get the value of focal point
        rAVX = ne_grd_getFx(gradient);
        rAVY = ne_grd_getFy(gradient);
        
        // set the focal point of radial gradient
        if (rAVX && rAVY)
            ((QRadialGradient*)(_graphicalGradient))->setFocalPoint(ne_rav_getAbsoluteValue(rAVX), ne_rav_getAbsoluteValue(rAVY));
        
        // get the value of radius
        rAVX = ne_grd_getR(gradient);
        
        // set the radius of radial gradient
        if (rAVX)
            ((QRadialGradient*)(_graphicalGradient))->setFocalRadius(ne_rav_getAbsoluteValue(rAVX));
    }
               
    // set the spread method of gradient
    if (ne_grd_isSetSpreadMethod(gradient)) {
        if (stringCompare(ne_grd_getSpreadMethod(gradient), "pad"))
            _graphicalGradient->setSpread(QGradient::PadSpread);
        else if (stringCompare(ne_grd_getSpreadMethod(gradient), "reflect"))
            _graphicalGradient->setSpread(QGradient::ReflectSpread);
        else if (stringCompare(ne_grd_getSpreadMethod(gradient), "repeat"))
            _graphicalGradient->setSpread(QGradient::RepeatSpread);
    }

    // get the gradient stops
    QGradientStop* _gradientStop;
    QGradientStops _gradientStops;
    VGradientStop* s = NULL;
    for (int i = 0; i < ne_grd_getNumStops(gradient); ++i) {
        s = ne_grd_getStop(gradient, i);
        
        // create a qgradient stop
        _gradientStop = new QGradientStop();
        
        // set the offset
        if (ne_gstp_isSetOffset(s))
            _gradientStop->first = 0.01 * ne_rav_getRelativeValue(ne_gstp_getOffset(s));
        
        // get the stop color
        if (ne_gstp_isSetColor(s))
            _gradientStop->second = getQColor(mw, ne_gstp_getColor(s));
        
        // add the stop to the list of stops
        _gradientStops.push_back(*_gradientStop);
    }
    
    // add the list of stops to the gradient
    _graphicalGradient->setStops(_gradientStops);
    
    // set the graphical gradient of graphical gradient
    gGradient->setGraphicalGradient(*_graphicalGradient);
    
    // set the gradient of graphical gradient
    gGradient->setGradient(gradient);
    
    // set the id of graphical gradient
    gGradient->setId(gradient->getId());
}

void getInfoFromLineEnding(MainWindow *mw, VLineEnding* lE, GraphicalLineEnding* gLEnding){
    if (gLEnding) {
        gLEnding->unSetGraphicalItem();
        gLEnding->unSetLEnding();
        gLEnding->unSetRotation();
        gLEnding->unSetId();
        
        // get bounding box of line ending
        LBox* b = ne_le_getBoundingBox(lE);
        
        // get the position of bounding box
        double x = ne_bb_getX(b);
        double y = ne_bb_getY(b);
        
        // get the dimensions of bounding box
        double width = ne_bb_getWidth(b);
        double height = ne_bb_getHeight(b);
        
        // set the positon of dimension of bounding rectangle
        QRectF boundingBox(x, y, width, height);
        
        // create the graphical item (the default one is a rectangle) using the bounding rectangle
        QGraphicsItem* _graphicalItem = new QGraphicsRectItem(boundingBox);
        
        // get the group of line ending
        VRenderGroup* group = ne_le_getGroup(lE);
        
        // get info of the graphical item from group of veneer
        std::string fillRule;
        getGraphicalItemInfoFromVeneer(mw, group, _graphicalItem, fillRule);
            
        // get info of the graphical item from geometric shape of veneer
        getInfoFromGeometricShape(mw, group, _graphicalItem, fillRule);
        
        // set the graphical item of graphical line ending
        gLEnding->setGraphicalItem(_graphicalItem);
        
        // set the rotation of graphical line ending
        gLEnding->setRotation(ne_le_getEnableRotation(lE));
        
        // set the line ending of graphical line ending
        gLEnding->setLEnding(lE);
        
        // set the id of graphical line ending
        gLEnding->setId(ne_ve_getId(lE));
    }
}

void getInfoFromStyle(MainWindow* mw, VGlobalStyle* style, QGraphicsItem* graphicalItem){
    VRenderGroup* group = ne_stl_getGroup(style);
    std::string fillRule;
    
    // get info of graphical item from group of veneer
    getGraphicalItemInfoFromVeneer(mw, group, graphicalItem, fillRule);
    
    // get info of the geometric shape of veneer
    getInfoFromGeometricShape(mw, group, graphicalItem, fillRule);
}

void getInfoFromStyle(MainWindow* mw, VGlobalStyle* style, MyQGraphicsTextItem* graphicalText){
    VRenderGroup* group = ne_stl_getGroup(style);
    
    // get info of graphical text from group of veneer
    getGraphicalItemInfoFromVeneer(mw, group, graphicalText);
    
    // get info of the geometric shape of veneer
    getInfoFromGeometricShape(mw, group, graphicalText);
}

void getInfoFromStyle(MainWindow* mw, VGlobalStyle* style, GraphicalCurve* graphicalCurve){
    VRenderGroup* group = ne_stl_getGroup(style);
    
    // get info of graphical curve from group of veneer
    getGraphicalItemInfoFromVeneer(mw, group, graphicalCurve);
}

void getInfoFromGeometricShape(MainWindow* mw, VRenderGroup* group, QGraphicsItem* graphicalItem, std::string& fillRule){
    
    VTransformation2D* gS = NULL;
    for (int i = 0; i <  ne_grp_getNumGeometricShapes(group); ++i) {
        // get geometric shape associated with group
        gS = ne_grp_getGeometricShape(group, i);
        
        if (gS) {
            switch (ne_gs_getShape(gS)) {
                // if the geometric shape is an image
                case 0:
                    getInfoFromImageShape(gS, graphicalItem);
                    break;
                   
                // if the geometric shape is a rectangle
                case 3:
                    getGraphicalItemInfoFromVeneer(mw, gS, graphicalItem, fillRule);
                    getInfoFromRectangleShape(gS, graphicalItem);
                    break;
                   
                // if the geometric shape is an ellipse
                case 4:
                    getGraphicalItemInfoFromVeneer(mw, gS, graphicalItem, fillRule);
                    getInfoFromEllipseShape(gS, graphicalItem);
                    break;
                    
                // if the geometric shape is a polygon
                case 5:
                    getGraphicalItemInfoFromVeneer(mw, gS, graphicalItem, fillRule);
                    getInfoFromPolygonShape(gS, graphicalItem, fillRule);
                    break;
                    
                default:
                    break;
            }
        }
    }
}

void getInfoFromGeometricShape(MainWindow* mw, VRenderGroup* group, MyQGraphicsTextItem* graphicalText){
    
    VTransformation2D* gS = NULL;
    for (int i = 0; i <  ne_grp_getNumGeometricShapes(group); ++i) {
        // get geometric shape associated with group
        VTransformation2D* gS = ne_grp_getGeometricShape(group, i);
        
        if (gS){
            // if the graphical shape is a text
            if (ne_gs_getShape(gS) == 2) {
                getGraphicalItemInfoFromVeneer(mw, gS, graphicalText);
                getInfoFromTextShape(gS, graphicalText);
            }
        }
    }
}

void getInfoFromRectangleShape(VTransformation2D* gS, QGraphicsItem *graphicalItem) {
    
    // get the exisitng pen of rectangle
    QPen pen = ((QGraphicsRectItem *)graphicalItem)->pen();
    
    // get the exisitng brush of rectangle
    QBrush brush = ((QGraphicsRectItem *)graphicalItem)->brush();
    
    // get the exisitng bounding rect of rectangle
    QRectF layoutRect = ((QGraphicsRectItem *)graphicalItem)->rect();
    
    // create the new bounding rect and initalize it with the existing one
    QRectF renderRect = layoutRect;
    
    RAVector* rAV = NULL;
    // if the x value of rectangle shape is set, reset the x value of new bounding rect using its value
    if (ne_rec_isSetPositionX(gS)) {
        rAV = ne_rec_getPositionX(gS);
        renderRect.setX(layoutRect.x()+ qreal(ne_rav_getAbsoluteValue(rAV) + layoutRect.width() * (0.01 * ne_rav_getRelativeValue(rAV))));
    }
    
    // if the y value of rectangle shape is set, reset the y value of new bounding rect using its value
    if (ne_rec_isSetPositionY(gS)) {
        rAV = ne_rec_getPositionY(gS);
        renderRect.setY(layoutRect.y()+ qreal(ne_rav_getAbsoluteValue(rAV) + layoutRect.height() * (0.01 * ne_rav_getRelativeValue(rAV))));
    }
    
    // if the width value of rectangle shape is set, reset the width value of new bounding rect using its value
    if (ne_rec_isSetDimensionWidth(gS)) {
        rAV = ne_rec_getDimensionWidth(gS);
        renderRect.setWidth(qreal(ne_rav_getAbsoluteValue(rAV) + layoutRect.width() * (0.01 * ne_rav_getRelativeValue(rAV))));
    }
    
    // if the height value of rectangle shape is set, reset the height value of new bounding rect using its value
    if (ne_rec_isSetDimensionHeight(gS)) {
        rAV = ne_rec_getDimensionHeight(gS);
        renderRect.setHeight(qreal(ne_rav_getAbsoluteValue(rAV) + layoutRect.height() * (0.01 * ne_rav_getRelativeValue(rAV))));
    }
    
    // the ratio of rectangle shape
    if (ne_rec_isSetRatio(gS)) {
        // reset the values of new bounding rect using the ratio of rectangle shape
        double ratio = ne_rec_getRatio(gS);
        if ((layoutRect.width() / layoutRect.height()) <= ratio) {
            renderRect.setWidth(layoutRect.width());
            renderRect.setHeight(layoutRect.width() / ratio);
            renderRect.moveTo(renderRect.x(), renderRect.y() + 0.5 * (layoutRect.height() - renderRect.height()));
        }
        else {
            renderRect.setWidth(ratio * layoutRect.height());
            renderRect.setHeight(layoutRect.height());
            renderRect.moveTo(renderRect.x() + 0.5 * (layoutRect.width() - renderRect.width()), renderRect.y());
        }
    }
    
    // set the bounding rect of graphical item using new bounding rect
    ((QGraphicsRectItem *)graphicalItem)->setRect(renderRect);
    
    // get the radii of cornver curvatures
    RAVector* rx = NULL;
    RAVector* ry = NULL;
    if (ne_rec_isSetCornerCurvatureRX(gS))
        rx = ne_rec_getCornerCurvatureRX(gS);
    if (ne_rec_isSetCornerCurvatureRY(gS))
        ry = ne_rec_getCornerCurvatureRY(gS);
    
    if (rx && !ry)
        ry = rx;
    else if (!rx && ry)
        rx = ry;
    
    if (rx && ry) {
        // create a painter path to draw the rectangle with corner curvatures
        QPainterPath painterPath;
        painterPath.addRoundedRect(renderRect, qreal(ne_rav_getAbsoluteValue(rx) + layoutRect.width() * (0.01 * ne_rav_getRelativeValue(rx))), qreal(ne_rav_getAbsoluteValue(ry) + layoutRect.height() * (0.01 * ne_rav_getRelativeValue(ry))));
                
        // cast the graphical item into a graphical path item
        graphicalItem = new (graphicalItem) QGraphicsPathItem(painterPath);
        
        // set the pen of graphical path item
        ((QGraphicsPathItem *)graphicalItem)->setPen(pen);
               
        // set the brush of graphical path item
        ((QGraphicsPathItem *)graphicalItem)->setBrush(brush);
    }
}

void getInfoFromEllipseShape(VTransformation2D* gS, QGraphicsItem *graphicalItem) {
    
    // get the exisitng pen of ellipse
    QPen pen = ((QGraphicsRectItem *)graphicalItem)->pen();
    
    // get the exisitng brush of ellipse
    QBrush brush = ((QGraphicsRectItem *)graphicalItem)->brush();
    
    // get the exisitng bounding rect of ellipse
    QRectF layoutRect = ((QGraphicsRectItem *)graphicalItem)->rect();
    
    // create the new bounding rect and initalize it with the existing one
    QRectF renderRect = layoutRect;
    
    RAVector* rAV1 = NULL;
    RAVector* rAV2 = NULL;
    if (ne_elp_isSetDimensionRX(gS)) {
        rAV1 = ne_elp_getDimensionRX(gS);
        renderRect.setWidth(2 * qreal(ne_rav_getAbsoluteValue(rAV1) + layoutRect.width() * (0.01 * ne_rav_getRelativeValue(rAV1))));
        
        if (ne_elp_isSetPositionCX(gS)) {
            rAV2 = ne_elp_getPositionCX(gS);
            renderRect.setX(layoutRect.x()+ qreal((ne_rav_getAbsoluteValue(rAV2) + layoutRect.width() * (0.01 * ne_rav_getRelativeValue(rAV2))) - (ne_rav_getAbsoluteValue(rAV1) + layoutRect.width() * (0.01 * ne_rav_getRelativeValue(rAV1)))));
        }
        
    }
    
    if (ne_elp_isSetDimensionRY(gS)) {
        rAV1 = ne_elp_getDimensionRY(gS);
        renderRect.setWidth(2 * qreal(ne_rav_getAbsoluteValue(rAV1) + layoutRect.height() * (0.01 * ne_rav_getRelativeValue(rAV1))));
        
        if (ne_elp_isSetPositionCY(gS)) {
            rAV2 = ne_elp_getPositionCY(gS);
            renderRect.setX(layoutRect.y()+ qreal((ne_rav_getAbsoluteValue(rAV2) + layoutRect.height() * (0.01 * ne_rav_getRelativeValue(rAV2))) - (ne_rav_getAbsoluteValue(rAV1) + layoutRect.height() * (0.01 * ne_rav_getRelativeValue(rAV1)))));
        }
        
    }
    
    // the ratio of ellipse shape
    if (ne_elp_isSetRatio(gS)) {
        // reset the values of new bounding rect using the ratio of ellipse shape
        double ratio = ne_elp_getRatio(gS);
        if ((layoutRect.width() / layoutRect.height()) <= ratio) {
            renderRect.setWidth(layoutRect.width());
            renderRect.setHeight(layoutRect.width() / ratio);
            renderRect.moveTo(renderRect.x(), renderRect.y() + 0.5 * (layoutRect.height() - renderRect.height()));
        }
        else {
            renderRect.setWidth(ratio * layoutRect.height());
            renderRect.setHeight(layoutRect.height());
            renderRect.moveTo(renderRect.x() + 0.5 * (layoutRect.width() - renderRect.width()), renderRect.y());
        }
    }
    
    // cast the graphical item into an graphical ellipse item
    graphicalItem = new (graphicalItem) QGraphicsEllipseItem(renderRect);

    // set the pen of graphical ellipse item
    ((QGraphicsEllipseItem *)graphicalItem)->setPen(pen);
    
    // set the brush of graphical ellipse item
    ((QGraphicsEllipseItem *)graphicalItem)->setBrush(brush);
}

void getInfoFromPolygonShape(VTransformation2D* gS, QGraphicsItem* graphicalItem, std::string& fillRule) {
    
    // get the exisitng pen of polygon
    QPen pen = ((QGraphicsRectItem *)graphicalItem)->pen();
    
    // get the exisitng brush of polygon
    QBrush brush = ((QGraphicsRectItem *)graphicalItem)->brush();
    
    // get the exisitng bounding rect of polygon
    QRectF layoutRect = ((QGraphicsRectItem *)graphicalItem)->rect();
    
    // create a polygonpath
    QPainterPath polygonPath;
    
    // get the vertices (elments) of polygon shape
    RenPoint* rp = NULL;
    RPoint* point = NULL;
    RAVector* x = NULL;
    RAVector* y = NULL;
    RAVector* c1x = NULL;
    RAVector* c1y = NULL;
    RAVector* c2x = NULL;
    RAVector* c2y = NULL;
    for (int i = 0; i < ne_plg_getNumVertices(gS); ++i) {
        // get the vertex of polygon
        rp = ne_plg_getVertex(gS, i);
        point = ne_vrx_getRenderPoint(rp);
        x = ne_rp_getX(point);
        y = ne_rp_getY(point);
        
        if (x && y) {
            if (i == 0)
                polygonPath.moveTo(QPointF(layoutRect.x() + ne_rav_getAbsoluteValue(x) + layoutRect.width() * (0.01 * ne_rav_getRelativeValue(x)), layoutRect.y() + ne_rav_getAbsoluteValue(y) + layoutRect.height() * (0.01 * ne_rav_getRelativeValue(y))));
            else {
                if (ne_vrx_isRenderCubicBezier(rp)) {
                    point = ne_vrx_getBasePoint1(rp);
                    c1x = ne_rp_getX(point);
                    c1y = ne_rp_getY(point);
                    point = ne_vrx_getBasePoint2(rp);
                    c2x = ne_rp_getX(point);
                    c2y = ne_rp_getY(point);
                    
                    polygonPath.cubicTo(QPointF(layoutRect.x() + ne_rav_getAbsoluteValue(c1x) + layoutRect.width() * (0.01 * ne_rav_getRelativeValue(c1x)), layoutRect.y() + ne_rav_getAbsoluteValue(c1y) + layoutRect.height() * (0.01 * ne_rav_getRelativeValue(c1y))), QPointF(layoutRect.x() + ne_rav_getAbsoluteValue(c2x) + layoutRect.width() * (0.01 * ne_rav_getRelativeValue(c2x)), layoutRect.y() + ne_rav_getAbsoluteValue(c2y) + layoutRect.height() * (0.01 * ne_rav_getRelativeValue(c2y))), QPointF(layoutRect.x() + ne_rav_getAbsoluteValue(x) + layoutRect.width() * (0.01 * ne_rav_getRelativeValue(x)), layoutRect.y() + ne_rav_getAbsoluteValue(y) + layoutRect.height() * (0.01 * ne_rav_getRelativeValue(y))));
                }
                else
                    polygonPath.lineTo(QPointF(layoutRect.x() + ne_rav_getAbsoluteValue(x) + layoutRect.width() * (0.01 * ne_rav_getRelativeValue(x)), layoutRect.y() + ne_rav_getAbsoluteValue(y) + layoutRect.height() * (0.01 * ne_rav_getRelativeValue(y))));
            }
        }
    }
    polygonPath.closeSubpath();
    
    // cast the graphical item into an graphical polygon item and set its polygon using the created polygon
    graphicalItem = new (graphicalItem) QGraphicsPolygonItem(polygonPath.toFillPolygon());
    
    // set the pen of graphical polygon item
    ((QGraphicsPolygonItem *)graphicalItem)->setPen(pen);
    
    // set the brush of graphical polygon item
    ((QGraphicsPolygonItem *)graphicalItem)->setBrush(brush);
    
    // get the vlaue of fill rule of graphical shape
    if (ne_gs_isSetFillRule(gS))
        fillRule = ne_gs_getFillRule(gS);
    
    // if the value of the fill rule is non zero, set the fill rule of graphical polygon item
    if (fillRule == "nonzero")
        ((QGraphicsPolygonItem *)graphicalItem)->setFillRule(Qt::WindingFill);
}

void getInfoFromImageShape(VTransformation2D* gS, QGraphicsItem* graphicalItem) {
    
    // get the exisitng bounding rect of image
    QRectF layoutRect = ((QGraphicsRectItem *)graphicalItem)->rect();
    
    // create the new bounding rect and initalize it with the existing one
    QRectF renderRect = layoutRect;
    
    RAVector* rAV = NULL;
    // if the x value of image shape is set, reset the x value of new bounding rect using its value
    if (ne_img_isSetPositionX(gS)) {
        rAV = ne_img_getPositionX(gS);
        renderRect.setX(layoutRect.x()+ qreal(ne_rav_getAbsoluteValue(rAV) + layoutRect.width() * (0.01 * ne_rav_getRelativeValue(rAV))));
    }
    
    // if the y value of image shape is set, reset the y value of new bounding rect using its value
    if (ne_img_isSetPositionY(gS)) {
        rAV = ne_img_getPositionY(gS);
        renderRect.setY(layoutRect.y()+ qreal(ne_rav_getAbsoluteValue(rAV) + layoutRect.height() * (0.01 * ne_rav_getRelativeValue(rAV))));
    }
    
    // if the width value of image shape is set, reset the width value of new bounding rect using its value
    if (ne_img_isSetDimensionWidth(gS)) {
        rAV = ne_img_getDimensionWidth(gS);
        renderRect.setWidth(qreal(ne_rav_getAbsoluteValue(rAV) + layoutRect.width() * (0.01 * ne_rav_getRelativeValue(rAV))));
    }
    
    // if the height value of image shape is set, reset the height value of new bounding rect using its value
    if (ne_img_isSetDimensionHeight(gS)) {
        rAV = ne_img_getDimensionHeight(gS);
        renderRect.setHeight(qreal(ne_rav_getAbsoluteValue(rAV) + layoutRect.height() * (0.01 * ne_rav_getRelativeValue(rAV))));
    }
    
    // set the bounding rect of graphical image using new bounding rect
    ((QGraphicsRectItem *)graphicalItem)->setRect(renderRect);
    
    // get the href of the image
    QPixmap* image = NULL;
    if (ne_img_isSetHref(gS)) {
        std::string href = ne_img_getHref(gS);
        image = new QPixmap(href.c_str());
        
        if (!image->isNull()) {
            graphicalItem = new (graphicalItem) QGraphicsPixmapItem(image->scaled(renderRect.width(), renderRect.height()));
            ((QGraphicsPixmapItem*)(graphicalItem))->setPos(renderRect.x(), renderRect.y());
        }
    }
    
    if (!image || image->isNull()) {
        QBrush brush = ((QGraphicsRectItem *)graphicalItem)->brush();
        QPen pen = ((QGraphicsRectItem *)graphicalItem)->pen();
        brush.setColor(Qt::black);
        pen.setColor(Qt::black);
        ((QGraphicsRectItem *)graphicalItem)->setBrush(brush);
        ((QGraphicsRectItem *)graphicalItem)->setPen(pen);
    }
}

void getInfoFromTextShape(VTransformation2D* gS, MyQGraphicsTextItem* graphicalText) {
    // get the existing postion and dimension of text
    qreal x = graphicalText->x();
    qreal y = graphicalText->y();
    qreal width = graphicalText->textWidth();
    
    RAVector* rAV = NULL;
    // if the x value of text shape is set, reset the x value of graphical text
    if (ne_txt_isSetPositionX(gS)) {
        rAV = ne_txt_getPositionX(gS);
        graphicalText->setX(x + ne_rav_getAbsoluteValue(rAV) + width * (0.01 * ne_rav_getRelativeValue(rAV)));
    }
    
    // if the y value of text shape is set, reset the y value of graphical text
    if (ne_txt_isSetPositionY(gS)) {
        rAV = ne_txt_getPositionY(gS);
        graphicalText->setX(y + ne_rav_getAbsoluteValue(rAV) + (width / 3.0) * (0.01 * ne_rav_getRelativeValue(rAV)));
    }
}

void getGraphicalItemInfoFromVeneer(MainWindow* mw, VRenderGroup* group, QGraphicsItem* graphicalItem, std::string& fillRule) {
    std::string strokeColor;
    double strokeWidth = 0.00000001;
    std::vector<unsigned int>* dashes = NULL;
    std::string fillColor;
    
    // get stroke color of group
    if (ne_grp_isSetStrokeColor(group))
        strokeColor = ne_grp_getStrokeColor(group);
       
    // get stroke width of group
    if (ne_grp_isSetStrokeWidth(group))
        strokeWidth = ne_grp_getStrokeWidth(group);
       
    // get stroke dash array of group
    if (ne_grp_isSetStrokeDashArray(group))
        dashes = ne_grp_getStrokeDashArray(group);
    
    // get the fill color of group
    if (ne_grp_isSetFillColor(group))
        fillColor = ne_grp_getFillColor(group);
       
    // get fill rule of group
    if (ne_grp_isSetFillRule(group))
        fillRule = ne_grp_getFillRule(group);
       
    setGraphicalItemInfo(mw, graphicalItem, strokeColor, strokeWidth, dashes, fillColor, fillRule);
}

void getGraphicalItemInfoFromVeneer(MainWindow* mw, VTransformation2D* gS, QGraphicsItem* graphicalItem, std::string& fillRule){
    std::string strokeColor;
    double strokeWidth = 0.00000001;
    std::vector<unsigned int>* dashes = NULL;
    std::string fillColor;
    
    // get stroke color of geometric shape
    if (ne_gs_isSetStrokeColor(gS))
        strokeColor = ne_gs_getStrokeColor(gS);
    
    // get stroke width of geometric shape
    if (ne_gs_isSetStrokeWidth(gS))
        strokeWidth = ne_gs_getStrokeWidth(gS);
    
    // get stroke dash array of geometric shape
    if (ne_gs_isSetStrokeDashArray(gS))
        dashes = ne_gs_getStrokeDashArray(gS);
 
    // get the fill color of geometric shape
    if (ne_gs_isSetFillColor(gS))
        fillColor = ne_gs_getFillColor(gS);
 
    // get fill rule of graphical shape
    if (ne_gs_isSetFillRule(gS))
        fillRule = ne_gs_getFillRule(gS);
 
    setGraphicalItemInfo(mw, graphicalItem, strokeColor, strokeWidth, dashes, fillColor, fillRule);
}

void getGraphicalItemInfoFromVeneer(MainWindow* mw, VRenderGroup* group, MyQGraphicsTextItem* graphicalText) {
    std::string strokeColor;
    std::string fontFamily;
    std::string fontWeight;
    std::string fontStyle;
    std::string hTextAnchor;
    std::string vTextAnchor;
    RAVector* fontSize = NULL;
    
    // get stroke color of group
    if (ne_grp_isSetStrokeColor(group))
        strokeColor = ne_grp_getStrokeColor(group);
    
    // get the font family of group
    if (ne_grp_isSetFontFamily(group))
        fontFamily = ne_grp_getFontFamily(group);
    
    // get the font size of group
    if (ne_grp_isSetFontSize(group))
        fontSize = ne_grp_getFontSize(group);
    
    // get the font wieght of group
    if (ne_grp_isSetFontWeight(group))
        fontWeight = ne_grp_getFontWeight(group);

    // get the font style of group
    if (ne_grp_isSetFontStyle(group))
        fontStyle = ne_grp_getFontStyle(group);
    
    // get the horizontal text anchor of group
    if (ne_grp_isSetHTextAnchor(group))
        hTextAnchor = ne_grp_getHTextAnchor(group);
    
    // get the vertical text anchor of group
    if (ne_grp_isSetVTextAnchor(group))
        vTextAnchor = ne_grp_getVTextAnchor(group);
    
    setGraphicalItemInfo(mw, graphicalText, strokeColor, fontFamily, fontSize, fontWeight, fontStyle, hTextAnchor, vTextAnchor);
}

void getGraphicalItemInfoFromVeneer(MainWindow* mw, VTransformation2D* gS, MyQGraphicsTextItem* graphicalText) {
    std::string strokeColor;
    std::string fontFamily;
    std::string fontWeight;
    std::string fontStyle;
    std::string hTextAnchor;
    std::string vTextAnchor;
    RAVector* fontSize = NULL;
    
    // get stroke color of group
    if (ne_gs_isSetStrokeColor(gS))
        strokeColor = ne_gs_getStrokeColor(gS);
    
    // get the font family of group
    if (ne_txt_isSetFontFamily(gS))
        fontFamily = ne_txt_getFontFamily(gS);
    
    // get the font size of group
    if (ne_txt_isSetFontSize(gS))
        fontSize = ne_txt_getFontSize(gS);
    
    // get the font wieght of group
    if (ne_txt_isSetFontWeight(gS))
        fontWeight = ne_txt_getFontWeight(gS);
    
    // get the font style of group
    if (ne_txt_isSetFontStyle(gS))
        fontStyle = ne_txt_getFontStyle(gS);
    
    // get the horizontal text anchor of group
    if (ne_txt_isSetHTextAnchor(gS))
        hTextAnchor = ne_txt_getHTextAnchor(gS);
    
    // get the vertical text anchor of group
    if (ne_txt_isSetVTextAnchor(gS))
        vTextAnchor = ne_txt_getVTextAnchor(gS);
    
    setGraphicalItemInfo(mw, graphicalText, strokeColor, fontFamily, fontSize, fontWeight, fontStyle, hTextAnchor, vTextAnchor);
}

void getGraphicalItemInfoFromVeneer(MainWindow* mw, VRenderGroup* group, GraphicalCurve* graphicalCurve) {
    std::string strokeColor;
    double strokeWidth = 0.0000001;
    std::vector<unsigned int>* dashes = NULL;
    std::string startHead;
    std::string endHead;
    
    // get stroke color of group
    if (ne_grp_isSetStrokeColor(group))
        strokeColor = ne_grp_getStrokeColor(group);
       
    // get stroke width of group
    if (ne_grp_isSetStrokeWidth(group))
        strokeWidth = ne_grp_getStrokeWidth(group);
       
    // get stroke dash array of group
    if (ne_grp_isSetStrokeDashArray(group))
        dashes = ne_grp_getStrokeDashArray(group);
    
    // get start head of group
    if (ne_grp_isSetStartHead(group))
        startHead = ne_grp_getStartHead(group);
    
    // get end head of group
    if (ne_grp_isSetEndHead(group))
        endHead = ne_grp_getEndHead(group);
    
    setGraphicalItemInfo(mw, graphicalCurve, strokeColor, strokeWidth, dashes, startHead, endHead);
}

void getGraphicalItemInfoFromVeneer(MainWindow* mw, VTransformation2D* gS, GraphicalCurve* graphicalCurve) {
    std::string strokeColor;
    double strokeWidth;
    std::vector<unsigned int>* dashes = NULL;
    std::string startHead;
    std::string endHead;
    
    // get stroke color of geometric shape
    if (ne_gs_isSetStrokeColor(gS))
        strokeColor = ne_gs_getStrokeColor(gS);
       
    // get stroke width of geometric shape
    if (ne_gs_isSetStrokeWidth(gS))
        strokeWidth = ne_gs_getStrokeWidth(gS);
       
    // get stroke dash array of geometric shape
    if (ne_gs_isSetStrokeDashArray(gS))
        dashes = ne_gs_getStrokeDashArray(gS);
    
    // get start head of geometric shape
    if (ne_rc_isSetStartHead(gS))
        startHead = ne_rc_getStartHead(gS);
    
    // get end head of geometric shape
    if (ne_rc_isSetEndHead(gS))
        endHead = ne_rc_getEndHead(gS);
    
    setGraphicalItemInfo(mw, graphicalCurve, strokeColor, strokeWidth, dashes, startHead, endHead);
}

void setGraphicalItemInfo(MainWindow* mw, QGraphicsItem* graphicalItem, std::string& strokeColor, double& strokeWidth, std::vector<unsigned int>* dashes, std::string& fillColor, std::string& fillRule){
    
    // get the exisitng pen of graphical item
    QPen pen = ((QGraphicsRectItem *)graphicalItem)->pen();
    
    // set stroke color of the pen
    if (!strokeColor.empty())
        pen.setColor(getQColor(mw, strokeColor));
    
    // if stroke width is set, set the width of pen
    if (strokeWidth > 0.000001)
        pen.setWidth(strokeWidth);
           
    // if stroke dash array is set
    if (dashes) {
        // get elements from stroke dash array
        QVector<qreal> _dashes;
        for (int i = 0; i < dashes->size(); ++i)
            _dashes.push_back(qreal(dashes->at(i)));
        
        // set the dash pattern of pen
        pen.setDashPattern(_dashes);
    }
    
    // set the pen of graphical item
    ((QGraphicsRectItem *)graphicalItem)->setPen(pen);
 
    // if fill color is set, set the brush of graphical item using fill color
    if (!fillColor.empty()) {
        
        // if it is a gradient
        GraphicalGradient* gGradient = findGraphicalGradient(mw, fillColor);
        if (gGradient && gGradient->isSetGraphicalGradient())
            ((QGraphicsRectItem *)graphicalItem)->setBrush(QBrush(gGradient->getGraphicalGradient()));
        else
            ((QGraphicsRectItem *)graphicalItem)->setBrush(QBrush(getQColor(mw, fillColor)));
    }
}

void setGraphicalItemInfo(MainWindow* mw, MyQGraphicsTextItem* graphicalText, std::string& strokeColor, std::string& fontFamily, RAVector* fontSize, std::string& fontWeight, std::string& fontStyle, std::string& hTextAnchor, std::string& vTextAnchor) {
    QPainter* painter = new QPainter();
    QFont font;
    QPen pen;
    
    // set stroke color of the pen
    if (!strokeColor.empty())
        pen.setColor(getQColor(mw, strokeColor));
    
    // if font family is set
    if (!fontFamily.empty()) {
        font.setFamily(fontFamily.c_str());
        QFontDatabase database;
        const QStringList fontFamilies = database.families();
        
        if (fontFamily == "sans-serif") {
            for (int i = 0; i < fontFamilies.size(); ++i) {
                if (!QString::compare(fontFamilies.at(i), "Helvetica", Qt::CaseInsensitive))
                    font.setFamily(fontFamilies.at(i));
            }
        }
        else if (fontFamily == "serif") {
            for (int i = 0; i < fontFamilies.size(); ++i) {
                if (!QString::compare(fontFamilies.at(i), "Times New Roman", Qt::CaseInsensitive))
                    font.setFamily(fontFamilies.at(i));
            }
        }
        else if (fontFamily == "monospace") {
            for (int i = 0; i < fontFamilies.size(); ++i) {
                if (!QString::compare(fontFamilies.at(i), "Courier", Qt::CaseInsensitive))
                    font.setFamily(fontFamilies.at(i));
            }
        }
    }
    
    // if font size is set, set the point size of font
    if (fontSize)
        font.setPointSize(int(ne_rav_getAbsoluteValue(fontSize) + graphicalText->boundingRect().width() * (0.01 * ne_rav_getRelativeValue(fontSize))));
    
    // if font weight is set to bold
    if (fontWeight == "bold")
        font.setBold(true);
    
    // if font style is set to italic
    if (fontStyle == "italic")
        font.setItalic(true);
    
    // set the modified pen and font of graphical text
    graphicalText->setTextPen(pen);
    graphicalText->setTextFont(font);
    
    if (!hTextAnchor.empty())
        graphicalText->setHorizontalAlignment(hTextAnchor);
    
    if (!vTextAnchor.empty())
        graphicalText->setVerticalAlignment(vTextAnchor);
    
    graphicalText->paint(painter, new QStyleOptionGraphicsItem(), mw);
}

void setGraphicalItemInfo(MainWindow* mw, GraphicalCurve* graphicalCurve, std::string& strokeColor, double& strokeWidth, std::vector<unsigned int>* dashes, std::string& startHead, std::string& endHead) {
    // get the exisitng pen of graphical path
    QPen pen = graphicalCurve->getGraphicalPaths().at(0)->pen();
    
    // set stroke color of the pen
    if (!strokeColor.empty())
        pen.setColor(getQColor(mw, strokeColor));
    
    // if stroke width is set, set the width of pen
    if (strokeWidth > 0.000001)
        pen.setWidth(strokeWidth);
    
    // if stroke dash array is set
    if (dashes) {
        // get elements from stroke dash array
        QVector<qreal> _dashes;
        for (int i = 0; i < dashes->size(); ++i)
            _dashes.push_back(qreal(dashes->at(i)));
        
        // set the dash pattern of pen
        pen.setDashPattern(_dashes);
    }
    
    // set the pen of graphical paths
    for (GraphicalCurve::constGPathIt gPIt = graphicalCurve->gPathsBegin(); gPIt != graphicalCurve->gPathsEnd(); ++gPIt)
        (*gPIt)->setPen(pen);
    
    // set the start line ending of graphical path
    if (!startHead.empty())
        graphicalCurve->setStartLineEnding(startHead);
    
    // set the end line ending of graphical path
    if (!endHead.empty())
        graphicalCurve->setEndLineEnding(endHead);
}

void setLineEndings(MainWindow* mw, GraphicalCurve* gCurve) {
    if (gCurve) {
        QPen curvePen = gCurve->getGraphicalPaths().at(0)->pen();
        bool enableRotation = true;

        
        // if the start line ending of the graphical curve is set
        if (gCurve->isSetStartLineEnding()) {
            
            // find the line ending among the graphical line endings using its id and create a copy of it if it exists
            QGraphicsItem* startGraphicalItem = findLineEnding(mw, gCurve->getStartLineEnding(), curvePen, enableRotation);
            
            // if line ending is found
            if (startGraphicalItem) {
                
                // if enable rotation is true, rotate the line ending to make it in accordance to the start slope of graphical curve
                if (enableRotation) {
                    startGraphicalItem->setRotation(gCurve->getStartSlope());
                    
                    if (startGraphicalItem->type() == 7) {
                        ((QGraphicsPixmapItem*)(startGraphicalItem))->setPos(((QGraphicsPixmapItem *)(startGraphicalItem))->x() * std::cos(gCurve->getStartSlope() * (pi / 180.0)) - ((QGraphicsPixmapItem *)(startGraphicalItem))->y() * std::sin(gCurve->getStartSlope() * (pi / 180.0)), -((QGraphicsPixmapItem *)(startGraphicalItem))->x() * std::sin(gCurve->getStartSlope() * (pi / 180.0)) - ((QGraphicsPixmapItem *)(startGraphicalItem))->y() * std::cos(gCurve->getStartSlope() * (pi / 180.0)));
                    }
                }
                
                // set the position of the line ending using the start position of the graphical curve
                startGraphicalItem->moveBy(gCurve->getStartPoint().x(), gCurve->getStartPoint().y());
                
                startGraphicalItem->setZValue(3);
                
                // display the start line ending
                mw->getScene()->addItem(startGraphicalItem);
                
                // set the graphical item as the start graphical item of graphical curve
                gCurve->setStartGraphicalItem(startGraphicalItem);
                
                // set start enable rotation
                gCurve->setStartEnableRotation(enableRotation);
            }
        }
        
        enableRotation = true;
        
        // if the end line ending of the graphical curve is set
        if (gCurve->isSetEndLineEnding()) {
            
            // find the line ending among the graphical line endings using its id and create a copy of it if it exists
            QGraphicsItem* endGraphicalItem = findLineEnding(mw, gCurve->getEndLineEnding(), curvePen, enableRotation);
            
            // if line ending is found
            if (endGraphicalItem) {
                
                if (enableRotation) {
                    endGraphicalItem->setRotation(gCurve->getEndSlope());
                    
                    if (endGraphicalItem->type() == 7) {
                        ((QGraphicsPixmapItem*)(endGraphicalItem))->setPos(((QGraphicsPixmapItem *)(endGraphicalItem))->x() * std::cos(gCurve->getEndSlope() * (pi / 180.0)) - ((QGraphicsPixmapItem *)(endGraphicalItem))->y() * std::sin(gCurve->getEndSlope() * (pi / 180.0)), -((QGraphicsPixmapItem *)(endGraphicalItem))->x() * std::sin(gCurve->getEndSlope() * (pi / 180.0)) - ((QGraphicsPixmapItem *)(endGraphicalItem))->y() * std::cos(gCurve->getEndSlope() * (pi / 180.0)));
                    }
                }
                
                // set the position of the line ending using the end position of the graphical curve
                endGraphicalItem->moveBy(gCurve->getEndPoint().x(), gCurve->getEndPoint().y());
                
                endGraphicalItem->setZValue(3);
                
                // display the end line ending
                mw->getScene()->addItem(endGraphicalItem);
                
                // set the graphical item as the end graphical item of graphical curve
                gCurve->setEndGraphicalItem(endGraphicalItem);
                
                // set end enable rotation
                gCurve->setEndEnableRotation(enableRotation);
            }
        }
    }
}

