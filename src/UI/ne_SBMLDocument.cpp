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
            for (int i = 0; i < ne_net_getNumCompartments(getNetwork()); ++i) {
                c = ne_net_getCompartment(getNetwork(), i);
                
                if (ne_go_isSetGlyphId(c)) {
                    // create a new graphical compartment
                    _compInfo = new GraphicalCompartment();
                    
                    // set the compartment of graphical compartment
                    _compInfo->setNGraphicalObject(c);
                    
                    // set the id of graphical compartment
                    _compInfo->setId(ne_go_getGlyphId(c));
                    
                    // add the graphical compartment to the list of graphical compartments
                    mw->addGCompartment(_compInfo);
                }
            }
            
            // species and texts
            NSpecies* s = NULL;
            GraphicalSpecies* _speciesInfo = NULL;
            for (int i = 0; i < ne_net_getNumSpecies(getNetwork()); ++i) {
                s = ne_net_getSpecies(getNetwork(), i);
                
                if (ne_go_isSetGlyphId(s)) {
                    // create a new graphical species
                    _speciesInfo = new GraphicalSpecies();
                    
                    // set the species of graphical speceis
                    _speciesInfo->setNGraphicalObject(s);
                    
                    // set the id of graphical species
                    _speciesInfo->setId(ne_go_getGlyphId(s));
                    
                    // set the role of graphical species
                    //if (s->isSetObjectRole())
                        //_speciesInfo->setRole(s->getObjectRole());

                    // add the graphical species to the list of graphical species
                    mw->addGSpecies(_speciesInfo);
                }
            }
            
            // reactions
            NReaction* r = NULL;
            GraphicalReaction* _reactionInfo = NULL;
            NSpeciesReference* sr = NULL;
            GraphicalSReference* _sReferenceInfo = NULL;
            for (int i = 0; i < ne_net_getNumReactions(getNetwork()); ++i) {
                r = ne_net_getReaction(getNetwork(), i);
                
                if (ne_go_isSetGlyphId(r)) {
                    // create a new graphical reaction
                    _reactionInfo = new GraphicalReaction();
                    
                    // set the reaction of graphical reaction
                    _reactionInfo->setNGraphicalObject(r);
                    
                    // set the id of graphical reaction
                    _reactionInfo->setId(ne_go_getGlyphId(r));
                        
                    // get species references
                    for (int j = 0; j < ne_rxn_getNumSpeciesReferences(r); ++j) {
                        sr = ne_rxn_getSpeciesReference(r, j);
                        
                        if (ne_go_isSetGlyphId(sr)) {
                            // create a new graphical species reference
                            _sReferenceInfo = new GraphicalSReference();
                            
                            // set the species reference of graphical species reference
                            _sReferenceInfo->setNGraphicalObject(sr);
                            
                            // set the id of graphical species reference
                            _sReferenceInfo->setId(ne_go_getGlyphId(sr));
                            
                            // set the role of graphical species reference
                            if (ne_sr_isSetRole(sr))
                                _sReferenceInfo->setRole(ne_sr_getRoleAsString(sr));
                            
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
            
            // set scene extends
            mw->setSceneRect(QRectF(qreal(getNetwork()->getBox().x()), qreal(getNetwork()->getBox().y()), qreal(getNetwork()->getBox().width()), qreal(getNetwork()->getBox().height())));
            
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
            VLineEnding* lE = NULL;
            for (int i = 0; i < ne_ven_getNumLineEndings(getVeneer()); ++i) {
                lE = ne_ven_getLineEnding(getVeneer(), i);
                
                if (ne_ve_isSetId(lE)) {
                    // create a new graphical line ending
                    _gLEnding = new GraphicalLineEnding();
                    
                    // set the line endding of graphical line ending
                    _gLEnding->setLEnding(lE);
                    
                    // set the id of graphical line ending
                    _gLEnding->setId(ne_ve_getId(lE));
                    
                    // store the graphical line ending
                    mw->addGLEnding(_gLEnding);
                    
                    // get graphical line ending values
                    _gLEnding->updateValues(mw);
                }
            }
            
            // get compartments info from veneer
            for (MainWindow::constGCompartmentIt cIt = mw->gCompartmentsBegin(); cIt != mw->gCompartmentsEnd(); ++cIt){
                
                // find the style in veneer
                (*cIt)->setStyle(mw);
                
                // get graphical compartment values
                (*cIt)->updateValues(mw);
            }
            
            // get species info from veneer
            for (MainWindow::constGSpeciesIt sIt = mw->gSpeciesBegin(); sIt != mw->gSpeciesEnd(); ++sIt) {
                // find the style in veneer
                (*sIt)->setStyle(mw);
                
                // get graphical species values
                if (isLayoutAlreadyExisted())
                    (*sIt)->updateValues(mw, false);
                else
                    (*sIt)->updateValues(mw, true);
            }
            
            // get reactions info from veneer
            for (MainWindow::constGReactionIt rIt = mw->gReactionsBegin(); rIt != mw->gReactionsEnd(); ++rIt) {
                
                // find the style in veneer
                (*rIt)->setStyle(mw);
                
                // get graphical reaction values
                (*rIt)->updateValues(mw);

                // get species references
                for (GraphicalReaction::constGSReferenceIt sRIt = (*rIt)->gSReferencesBegin(); sRIt != (*rIt)->gSReferencesEnd(); ++sRIt) {
                    // find the style in veneer
                    (*sRIt)->setStyle(mw);
                    
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

std::vector<QGraphicsItem*> findLineEnding(MainWindow *mw, const std::string& lineEndingId, const QPen& curvePen, bool& enableRotation) {
    std::vector<QGraphicsItem*> gItems(0);
    QGraphicsItem* gItem;
    
    // search among all the line endings to find the one with the same id
    for (MainWindow::constGLEndingIt gLEIt = mw->gLEndingsBegin(); gLEIt != mw->gLEndingsEnd(); ++gLEIt) {
        // if the the line ending with the same id is found
        if (stringCompare((*gLEIt)->getId(), lineEndingId)) {
            // get enable rotation status of the line ending
            enableRotation = (*gLEIt)->getRotation();
            
            // make a copy of the graphical item of the line ending
            for (GraphicalObjectBase::constGItemIt gItemIt = (*gLEIt)->gItemsBegin(); gItemIt != (*gLEIt)->gItemsEnd(); ++gItemIt) {
                switch ((*gItemIt)->type()) {
                    case 2:
                        // create a new qpainter path item
                        gItem = new QGraphicsPathItem(((QGraphicsPathItem *)(*gItemIt))->path());
                        
                        // set the pen of graphical path item using the pen of the curve it is assigned to
                        ((QGraphicsPathItem *)gItem)->setPen(curvePen);
                        
                        // set the brush of graphical path item
                        ((QGraphicsPathItem *)gItem)->setBrush(((QGraphicsPathItem *)(*gItemIt))->brush());
                        break;
                        
                    case 3:
                        // create a new graphics rect item
                        gItem = new QGraphicsRectItem();
                        
                        // set the pen of graphics rect item using the pen of the curve it is assigned to
                        ((QGraphicsRectItem *)gItem)->setPen(curvePen);
                        
                        // set the brush of graphics rect item
                        ((QGraphicsRectItem *)gItem)->setBrush(((QGraphicsRectItem *)(*gItemIt))->brush());
                        
                        // set the bounding rect of graphics rect item
                        ((QGraphicsRectItem *)gItem)->setRect(((QGraphicsRectItem *)(*gItemIt))->rect());
                        break;
                        
                    case 4:
                        // create a new graphics ellipse item
                        gItem = new QGraphicsEllipseItem();
                        
                        // set the pen of graphics ellipse item using the pen of the curve it is assigned to
                        ((QGraphicsEllipseItem *)gItem)->setPen(curvePen);
                        
                        // set the brush of graphics ellipse item
                        ((QGraphicsEllipseItem *)gItem)->setBrush(((QGraphicsEllipseItem *)(*gItemIt))->brush());
                        
                        // set the bounding rect of graphics ellipse item
                        ((QGraphicsEllipseItem *)gItem)->setRect(((QGraphicsEllipseItem *)(*gItemIt))->rect());
                        break;
                        
                    case 5:
                        // create a new graphics polygon item
                        gItem = new QGraphicsPolygonItem();
                        
                        // set the pen of graphics polygon item using the pen of the curve it is assigned to
                        ((QGraphicsPolygonItem *)gItem)->setPen(curvePen);
                        
                        // set the brush of graphics polygon item
                        ((QGraphicsPolygonItem *)gItem)->setBrush(((QGraphicsPolygonItem *)(*gItemIt))->brush());
                        
                        // set the fill rule of graphics polygon item
                        ((QGraphicsPolygonItem *)gItem)->setFillRule(((QGraphicsPolygonItem *)(*gItemIt))->fillRule());
                        
                        // set the polygon of graphics polygon item
                        ((QGraphicsPolygonItem *)gItem)->setPolygon(((QGraphicsPolygonItem *)(*gItemIt))->polygon());
                        break;
                        
                    case 7:
                        gItem = new QGraphicsPixmapItem(((QGraphicsPixmapItem *)(*gItemIt))->pixmap());
                        ((QGraphicsPixmapItem*)(gItem))->setPos(((QGraphicsPixmapItem *)(*gItemIt))->x(), ((QGraphicsPixmapItem *)(*gItemIt))->y());
                        
                        break;
                }
                
                if (gItem)
                    gItems.push_back(gItem);
            }
            
            // make a copy of the graphical curve of the line ending
            for (GraphicalObjectBase::constGCurveIt gCurveIt = (*gLEIt)->gCurvesBegin(); gCurveIt != (*gLEIt)->gCurvesEnd(); ++gCurveIt) {
                
                for (GraphicalCurve::constGPathIt gPIt = (*gCurveIt)->gPathsBegin(); gPIt != (*gCurveIt)->gPathsEnd(); ++gPIt) {
                    // create a new qpainter path item
                    gItem = new QGraphicsPathItem((*gPIt)->path());
                    
                    // set the pen of graphical path item using the pen of the curve it is assigned to
                    ((QGraphicsPathItem *)gItem)->setPen(curvePen);
                    
                    gItems.push_back(gItem);
                }
            }
        }
    }
    
    return gItems;
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

std::vector<QGraphicsItem*> getInfoFromRenderGroup(MainWindow* mw, VRenderGroup* group, LBox* box) {
    std::string strokeColor;
    double strokeWidth = 0.00000001;
    std::vector<unsigned int>* dashes = NULL;
    std::string fillColor;
    std::string fillRule;
    std::string fontFamily;
    RAVector* fontSize = NULL;
    std::string fontWeight;
    std::string fontStyle;
    std::string hTextAnchor;
    std::string vTextAnchor;
    std::string startHead;
    std::string endHead;
    
    // get info of graphical item from group of veneer
    getGraphicalItemInfoFromRenderGroup(mw, group, strokeColor, strokeWidth, dashes, fillColor, fillRule, fontFamily, fontSize, fontWeight, fontStyle, hTextAnchor, vTextAnchor, startHead, endHead);
    
    // get info of the geometric shape of veneer
    return getGeometricShapes(mw, group, box, strokeColor, strokeWidth, dashes, fillColor, fillRule);
}

std::vector<GraphicalCurve*> getInfoFromRenderGroup(MainWindow* mw, VRenderGroup* group, LBox* box, LCurve* curve){
    std::vector<GraphicalCurve*> cItems(0);
    GraphicalCurve* cItem = NULL;
    std::string strokeColor;
    double strokeWidth = 0.00000001;
    std::vector<unsigned int>* dashes = NULL;
    std::string fillColor;
    std::string fillRule;
    std::string fontFamily;
    RAVector* fontSize = NULL;
    std::string fontWeight;
    std::string fontStyle;
    std::string hTextAnchor;
    std::string vTextAnchor;
    std::string startHead;
    std::string endHead;
    
    // get info of graphical item from group of veneer
    getGraphicalItemInfoFromRenderGroup(mw, group, strokeColor, strokeWidth, dashes, fillColor, fillRule, fontFamily, fontSize, fontWeight, fontStyle, hTextAnchor, vTextAnchor, startHead, endHead);
        
    // get info from the geometric shape of veneer
    if (box && group) {
        for (int i = 0; i <  ne_grp_getNumGeometricShapes(group); ++i) {
            if (ne_gs_getShape(ne_grp_getGeometricShape(group, i)) == 1) {
                cItem = getGraphicalCurve(mw, ne_grp_getGeometricShape(group, i), box, strokeColor, strokeWidth, dashes, startHead, endHead);
                if (cItem)
                    cItems.push_back(cItem);
            }
        }
    }
    
    // get info from curve
    if (curve) {
        cItem = getGraphicalCurve(mw, group, curve, strokeColor, strokeWidth, dashes, startHead, endHead);
        if (cItem)
            cItems.push_back(cItem);
    }
    
    return cItems;
}

void getGraphicalItemInfoFromRenderGroup(MainWindow* mw, VRenderGroup* group, std::string& strokeColor, double& strokeWidth, std::vector<unsigned int>*& dashes, std::string& fillColor, std::string& fillRule, std::string& fontFamily, RAVector* fontSize, std::string& fontWeight , std::string& fontStyle, std::string& hTextAnchor, std::string& vTextAnchor, std::string& startHead, std::string& endHead) {
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
    
    // get font family of group
    if (ne_grp_isSetFontFamily(group))
        fontFamily = ne_grp_getFontFamily(group);
    
    // get font size of group
    if (ne_grp_isSetFontSize(group))
        fontSize = ne_grp_getFontSize(group);
    
    // get font weight of group
    if (ne_grp_isSetFontWeight(group))
        fontWeight = ne_grp_getFontWeight(group);
    
    // get font style of group
    if (ne_grp_isSetFontStyle(group))
        fontStyle = ne_grp_getFontStyle(group);
    
    // get vertical text anchor of group
    if (ne_grp_isSetHTextAnchor(group))
        hTextAnchor = ne_grp_getHTextAnchor(group);
    
    // get horizontal text anchor of group
    if (ne_grp_isSetVTextAnchor(group))
        vTextAnchor = ne_grp_getVTextAnchor(group);
    
    // get start head of group
    if (ne_grp_isSetStartHead(group))
        startHead = ne_grp_getStartHead(group);
    
    // get end head of group
    if (ne_grp_isSetEndHead(group))
        endHead = ne_grp_getEndHead(group);
}

std::vector<QGraphicsItem*> getGeometricShapes(MainWindow* mw, VRenderGroup* group, LBox* box, std::string strokeColor, double strokeWidth, std::vector<unsigned int>* dashes, const std::string fillColor, const std::string fillRule) {
    std::vector<QGraphicsItem*> gItems(0);
    VTransformation2D* gS = NULL;
    QGraphicsItem* gItem = NULL;
    
    if (group) {
        for (int i = 0; i <  ne_grp_getNumGeometricShapes(group); ++i) {
            // get geometric shape associated with group
            gS = ne_grp_getGeometricShape(group, i);
            gItem = NULL;
            
            if (gS) {
                switch (ne_gs_getShape(gS)) {
                    // image
                    case 0:
                        gItem = getImageShape(mw, gS, box);
                        break;
                    // rectangle
                    case 3:
                        gItem = getRectangleShape(mw, gS, box, strokeColor, strokeWidth, dashes, fillColor);
                        break;
                    // ellipse
                    case 4:
                        gItem = getEllipseShape(mw, gS, box, strokeColor, strokeWidth, dashes, fillColor);
                        break;
                    // polygon
                    case 5:
                        gItem = getPolygonShape(mw, gS, box, strokeColor, strokeWidth, dashes, fillColor, fillRule);
                        break;
                        
                    default:
                        break;
                }
            }
            
            if (gItem)
                gItems.push_back(gItem);
        }
    }
    else
        gItems.push_back(new QGraphicsRectItem(qreal(ne_bb_getX(box)), qreal(ne_bb_getY(box)), qreal(ne_bb_getWidth(box)), qreal(ne_bb_getHeight(box))));
        
    return gItems;
}

QGraphicsItem* getImageShape(MainWindow* mw, VTransformation2D* gS, LBox* box) {
    QGraphicsItem* imageItem = NULL;
    QPixmap* image = NULL;
    RAVector* rAV = NULL;
    
    if (gS && box) {
        // get the exisitng bounding rect of rectangle
        QRectF layoutRect(qreal(ne_bb_getX(box)), qreal(ne_bb_getY(box)), qreal(ne_bb_getWidth(box)), qreal(ne_bb_getHeight(box)));
        
        // create the new bounding rect and initalize it with the existing one
        QRectF renderRect = layoutRect;
        
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
        
        // get the href of the image
        if (ne_img_isSetHref(gS)) {
            std::string href = ne_img_getHref(gS);
            image = new QPixmap(href.c_str());
            
            if (!image->isNull()) {
                imageItem = new QGraphicsPixmapItem(image->scaled(renderRect.width(), renderRect.height()));
                ((QGraphicsPixmapItem*)(imageItem))->setPos(renderRect.x(), renderRect.y());
            }
        }
        
        if (!image || image->isNull()) {
            imageItem = new QGraphicsRectItem(renderRect);
            ((QGraphicsRectItem *)imageItem)->setPen(getPenFeatures(mw, "Black", 2.0, NULL));
            ((QGraphicsRectItem *)imageItem)->setBrush(getBrushFeatures(mw, "Black"));
            
        }
    }
    
    return imageItem;
}

QGraphicsItem* getTextShape(MainWindow* mw, VTransformation2D* gS, LBox* box, std::string fontFamily, RAVector* fontSize, std::string fontWeight, std::string fontStyle, std::string hTextAnchor, std::string vTextAnchor) {
    QGraphicsItem* textItem = NULL;
    RAVector* rAV = NULL;
    
    if (gS && box) {
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
        
        // get the exisitng bounding rect of rectangle
        QRectF layoutRect(qreal(ne_bb_getX(box)), qreal(ne_bb_getY(box)), qreal(ne_bb_getWidth(box)), qreal(ne_bb_getHeight(box)));
        
        // create the new bounding rect and initalize it with the existing one
        QRectF renderRect = layoutRect;
        
        // if the x value of text shape is set, reset the x value of graphical text
        if (ne_txt_isSetPositionX(gS)) {
            rAV = ne_txt_getPositionX(gS);
            renderRect.setX(layoutRect.x() + ne_rav_getAbsoluteValue(rAV) + layoutRect.width() * (0.01 * ne_rav_getRelativeValue(rAV)));
        }
        
        // if the y value of text shape is set, reset the y value of graphical text
        if (ne_txt_isSetPositionY(gS)) {
            rAV = ne_txt_getPositionY(gS);
            renderRect.setY(layoutRect.y() + ne_rav_getAbsoluteValue(rAV) + (layoutRect.width() / 3.0) * (0.01 * ne_rav_getRelativeValue(rAV)));
        }
        
        // to be done in future (other aspects)
        
        
        
        textItem = new MyQGraphicsTextItem("", renderRect);
    }
    
    return textItem;
}

QGraphicsItem* getRectangleShape(MainWindow* mw, VTransformation2D* gS, LBox* box, std::string strokeColor, double strokeWidth, std::vector<unsigned int>* dashes, std::string fillColor) {
    QGraphicsItem* rectangleItem = NULL;
    RAVector* rAV = NULL;
    RAVector* rx = NULL;
    RAVector* ry = NULL;
    
    if (gS && box) {
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
        
        // get the exisitng bounding rect of rectangle
        QRectF layoutRect(qreal(ne_bb_getX(box)), qreal(ne_bb_getY(box)), qreal(ne_bb_getWidth(box)), qreal(ne_bb_getHeight(box)));
        
        // create the new bounding rect and initalize it with the existing one
        QRectF renderRect = layoutRect;
        
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
        
        // get the radii of cornver curvatures
        if (ne_rec_isSetCornerCurvatureRX(gS)) {
            rx = ne_rec_getCornerCurvatureRX(gS);
            if(!ne_rec_isSetCornerCurvatureRY(gS))
                ry = rx;
        }
        if (ne_rec_isSetCornerCurvatureRY(gS)) {
            ry = ne_rec_getCornerCurvatureRY(gS);
            if(!ne_rec_isSetCornerCurvatureRX(gS))
                rx = ry;
        }
        
        if (rx && ry) {
            // create a painter path to draw the rectangle with corner curvatures
            QPainterPath painterPath;
            painterPath.addRoundedRect(renderRect, qreal(ne_rav_getAbsoluteValue(rx) + layoutRect.width() * (0.01 * ne_rav_getRelativeValue(rx))), qreal(ne_rav_getAbsoluteValue(ry) + layoutRect.height() * (0.01 * ne_rav_getRelativeValue(ry))));
                    
            // generate a new graphical path item
            rectangleItem = new QGraphicsPathItem(painterPath);
            
            // set the pen of graphical path item
            ((QGraphicsPathItem *)rectangleItem)->setPen(getPenFeatures(mw, strokeColor, strokeWidth, dashes));
                   
            // set the brush of graphical path item
            ((QGraphicsPathItem *)rectangleItem)->setBrush(getBrushFeatures(mw, fillColor));
        }
        else {
            // set the bounding rect of graphical item using new bounding rect
            rectangleItem = new QGraphicsRectItem(renderRect);
            
            // set the pen of graphical path item
            ((QGraphicsRectItem *)rectangleItem)->setPen(getPenFeatures(mw, strokeColor, strokeWidth, dashes));
                   
            // set the brush of graphical path item
            ((QGraphicsRectItem *)rectangleItem)->setBrush(getBrushFeatures(mw, fillColor));
        }
    }
    
    return rectangleItem;
}

QGraphicsItem* getEllipseShape(MainWindow* mw, VTransformation2D* gS, LBox* box, std::string strokeColor, double strokeWidth, std::vector<unsigned int>* dashes, std::string fillColor) {
    QGraphicsItem* ellipseItem = NULL;
    RAVector* rAV1 = NULL;
    RAVector* rAV2 = NULL;
    
    if (gS && box) {
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
        
        // get the exisitng bounding rect of rectangle
        QRectF layoutRect(qreal(ne_bb_getX(box)), qreal(ne_bb_getY(box)), qreal(ne_bb_getWidth(box)), qreal(ne_bb_getHeight(box)));
        
        // create the new bounding rect and initalize it with the existing one
        QRectF renderRect = layoutRect;
        
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
            renderRect.setHeight(2 * qreal(ne_rav_getAbsoluteValue(rAV1) + layoutRect.height() * (0.01 * ne_rav_getRelativeValue(rAV1))));
            
            if (ne_elp_isSetPositionCY(gS)) {
                rAV2 = ne_elp_getPositionCY(gS);
                renderRect.setY(layoutRect.y()+ qreal((ne_rav_getAbsoluteValue(rAV2) + layoutRect.height() * (0.01 * ne_rav_getRelativeValue(rAV2))) - (ne_rav_getAbsoluteValue(rAV1) + layoutRect.height() * (0.01 * ne_rav_getRelativeValue(rAV1)))));
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
        ellipseItem = new QGraphicsEllipseItem(renderRect);

        // set the pen of graphical ellipse item
        ((QGraphicsEllipseItem *)ellipseItem)->setPen(getPenFeatures(mw, strokeColor, strokeWidth, dashes));
        
        // set the brush of graphical ellipse item
        ((QGraphicsEllipseItem *)ellipseItem)->setBrush(getBrushFeatures(mw, fillColor));
    }
    
    return ellipseItem;
}

QGraphicsItem* getPolygonShape(MainWindow* mw, VTransformation2D* gS, LBox* box, std::string strokeColor, double strokeWidth, std::vector<unsigned int>* dashes, std::string fillColor, std::string fillRule) {
    QGraphicsItem* polygonItem = NULL;
    RenPoint* rp = NULL;
    RPoint* point = NULL;
    RAVector* x = NULL;
    RAVector* y = NULL;
    RAVector* c1x = NULL;
    RAVector* c1y = NULL;
    RAVector* c2x = NULL;
    RAVector* c2y = NULL;
    
    if (gS && box) {
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
        
        // get the value of fill rule of geometric shape
        if (ne_gs_isSetFillRule(gS))
            fillRule = ne_gs_getFillRule(gS);
        
        // get the exisitng bounding rect of rectangle
        QRectF layoutRect(qreal(ne_bb_getX(box)), qreal(ne_bb_getY(box)), qreal(ne_bb_getWidth(box)), qreal(ne_bb_getHeight(box)));
        
        // create the new bounding rect and initalize it with the existing one
        QRectF renderRect = layoutRect;
        
        // create a polygonpath
        QPainterPath polygonPath;
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
        polygonItem = new QGraphicsPolygonItem(polygonPath.toFillPolygon());
        
        // set the pen of graphical ellipse item
        ((QGraphicsPolygonItem *)polygonItem)->setPen(getPenFeatures(mw, strokeColor, strokeWidth, dashes));
        
        // set the brush of graphical ellipse item
        ((QGraphicsPolygonItem *)polygonItem)->setBrush(getBrushFeatures(mw, fillColor));
        
        // if the value of the fill rule is non zero, set the fill rule of graphical polygon item
        if (fillRule == "nonzero")
            ((QGraphicsPolygonItem *)polygonItem)->setFillRule(Qt::WindingFill);
    }
    
    return polygonItem;
}

GraphicalCurve* getGraphicalCurve(MainWindow* mw, VTransformation2D* gS, LBox* box, std::string strokeColor, double strokeWidth, std::vector<unsigned int>* dashes, std::string startHead, std::string endHead) {
    GraphicalCurve* graphicalCurve = NULL;
    QGraphicsPathItem* gPath;
    QPainterPath* cubicbezier;
    
    if (gS && box) {
        graphicalCurve = new GraphicalCurve();
        
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
        
        // get the exisitng bounding rect of rectangle
        QRectF layoutRect(qreal(ne_bb_getX(box)), qreal(ne_bb_getY(box)), qreal(ne_bb_getWidth(box)), qreal(ne_bb_getHeight(box)));
        
        QPointF sPoint, ePoint;
        QPointF basePoint1, basePoint2;
        RenPoint* rp = NULL;
        RAVector* rPointX = NULL;
        RAVector* rPointY =NULL;
        for (int i = 0; i < ne_rc_getNumVertices(gS); ++i) {
            cubicbezier = NULL;
            rp = ne_rc_getVertex(gS, i);
            
            // render point
            rPointX = ne_rp_getX(ne_vrx_getRenderPoint(rp));
            rPointY = ne_rp_getX(ne_vrx_getRenderPoint(rp));
            
            if (i == 0)
                sPoint = QPointF(qreal(layoutRect.x() + ne_rav_getAbsoluteValue(rPointX) + layoutRect.width() * (0.01 * ne_rav_getRelativeValue(rPointX))), qreal(layoutRect.y() + ne_rav_getAbsoluteValue(rPointY) + layoutRect.height() * (0.01 * ne_rav_getRelativeValue(rPointY))));
            else {
                ePoint = QPointF(qreal(layoutRect.x() + ne_rav_getAbsoluteValue(rPointX) + layoutRect.width() * (0.01 * ne_rav_getRelativeValue(rPointX))), qreal(layoutRect.y() + ne_rav_getAbsoluteValue(rPointY) + layoutRect.height() * (0.01 * ne_rav_getRelativeValue(rPointY))));
                
                // get base points
                if (ne_vrx_isRenderCubicBezier(rp)) {
                    rPointX = ne_rp_getX(ne_vrx_getBasePoint1(rp));
                    rPointY = ne_rp_getY(ne_vrx_getBasePoint1(rp));
                    basePoint1 = QPointF(qreal(layoutRect.x() + ne_rav_getAbsoluteValue(rPointX) + layoutRect.width() * (0.01 * ne_rav_getRelativeValue(rPointX))), qreal(layoutRect.y() + ne_rav_getAbsoluteValue(rPointY) + layoutRect.height() * (0.01 * ne_rav_getRelativeValue(rPointY))));
                    
                    rPointX = ne_rp_getX(ne_vrx_getBasePoint2(rp));
                    rPointY = ne_rp_getY(ne_vrx_getBasePoint2(rp));
                    basePoint2 = QPointF(qreal(layoutRect.x() + ne_rav_getAbsoluteValue(rPointX) + layoutRect.width() * (0.01 * ne_rav_getRelativeValue(rPointX))), qreal(layoutRect.y() + ne_rav_getAbsoluteValue(rPointY) + layoutRect.height() * (0.01 * ne_rav_getRelativeValue(rPointY))));
                }
                else {
                    basePoint1 = sPoint;
                    basePoint2 = ePoint;
                }
                
                // create the graphical path
                cubicbezier = new QPainterPath(sPoint);
                if (i > 0)
                    cubicbezier->cubicTo(basePoint1, basePoint2, ePoint);
                gPath = new QGraphicsPathItem(*cubicbezier);
                gPath->setPen(getPenFeatures(mw, strokeColor, strokeWidth, dashes));
                graphicalCurve->addGraphicalPath(gPath);
                
                // set end point and slope
                if (i == 1) {
                    
                    // start point
                    graphicalCurve->setStartPoint(sPoint);
                    
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
                    graphicalCurve->setStartSlope((180.0 / pi) * std::atan2( numerator, denominator));
                }
                
                // set end point and slope
                if (i == ne_rc_getNumVertices(gS) - 1) {
                    
                    // end point
                    graphicalCurve->setEndPoint(ePoint);
                    
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
                    graphicalCurve->setEndSlope((180.0 / pi) * std::atan2( numerator, denominator));
                }
                
                sPoint = ePoint;
            }
        }
        
        // start head
        if (!startHead.empty())
            graphicalCurve->setStartLineEnding(startHead);
        
        // end head
        if (!endHead.empty())
            graphicalCurve->setEndLineEnding(endHead);
    }
    
    return graphicalCurve;
}

GraphicalCurve* getGraphicalCurve(MainWindow* mw, VRenderGroup* group, LCurve* curve, std::string strokeColor, double strokeWidth, std::vector<unsigned int>* dashes, std::string startHead, std::string endHead) {
    GraphicalCurve* graphicalCurve = NULL;
    QGraphicsPathItem* gPath = NULL;
    QPainterPath* cubicbezier = NULL;
    
    if (curve) {
        graphicalCurve = new GraphicalCurve();
        
        LLineSegment* l = NULL;
        QPointF sPoint, ePoint;
        QPointF basePoint1, basePoint2;
        
        for (int i = 0; i < ne_crv_getNumElements(curve); ++i) {
            l = ne_crv_getElement(curve, i);
            
            // get start point
            sPoint = QPointF(qreal(ne_point_getX(ne_ls_getStart(l))), qreal(ne_point_getY(ne_ls_getStart(l))));
            
            // get end point
            ePoint = QPointF(qreal(ne_point_getX(ne_ls_getEnd(l))), qreal(ne_point_getY(ne_ls_getEnd(l))));
            
            // get base points
            if (ne_ls_isCubicBezier(l)) {
                basePoint1 = QPointF(qreal(ne_point_getX(ne_cb_getBasePoint1(l))), qreal(ne_point_getY(ne_cb_getBasePoint1(l))));
                
                basePoint2 = QPointF(qreal(ne_point_getX(ne_cb_getBasePoint2(l))), qreal(ne_point_getY(ne_cb_getBasePoint2(l))));
            }
            else {
                basePoint1 = sPoint;
                basePoint2 = ePoint;
            }
            
            // create the graphical path
            cubicbezier = new QPainterPath(sPoint);
            cubicbezier->cubicTo(basePoint1, basePoint2, ePoint);
            gPath = new QGraphicsPathItem(*cubicbezier);
            gPath->setPen(getPenFeatures(mw, strokeColor, strokeWidth, dashes));
            graphicalCurve->addGraphicalPath(gPath);
            
            // set start point and slope
            if (i == 0) {
                
                // start point
                graphicalCurve->setStartPoint(sPoint);
                
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
                graphicalCurve->setStartSlope((180.0 / pi) * std::atan2( numerator, denominator));
            }
            
            // set end point and slope
            if (i == ne_crv_getNumElements(curve) - 1) {
                
                // end point
                graphicalCurve->setEndPoint(ePoint);
                
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
                graphicalCurve->setEndSlope((180.0 / pi) * std::atan2( numerator, denominator));
            }
        }
        
        // start head
        if (!startHead.empty())
            graphicalCurve->setStartLineEnding(startHead);
        
        // end head
        if (!endHead.empty())
            graphicalCurve->setEndLineEnding(endHead);
    }
    
    return graphicalCurve;
}
 
QPen getPenFeatures(MainWindow* mw, std::string strokeColor, double strokeWidth, std::vector<unsigned int>* dashes) {
    QPen pen;
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
 
    return pen;
}

QBrush getBrushFeatures(MainWindow* mw, std::string fillColor) {
    QBrush brush;
    if (!fillColor.empty()) {
        // set the brush of graphical item using fill color
        
        // if it is a gradient
        GraphicalGradient* gGradient = findGraphicalGradient(mw, fillColor);
        if (gGradient && gGradient->isSetGraphicalGradient())
            brush = QBrush(gGradient->getGraphicalGradient());
        // if it is a color
        else
            brush = QBrush(getQColor(mw, fillColor));
    }
    
    return brush;
}



















std::vector<QGraphicsItem*> getInfoFromRenderGroup(MainWindow* mw, VRenderGroup* group, MyQGraphicsTextItem* graphicalText){
    // get info of graphical text from group of veneer
    getGraphicalItemInfoFromVeneer(mw, group, graphicalText);
    
    // get info of the geometric shape of veneer
    return getInfoFromGeometricShape(mw, group, graphicalText);
}

std::vector<QGraphicsItem*> getInfoFromGeometricShape(MainWindow* mw, VRenderGroup* group, MyQGraphicsTextItem* graphicalText){
    std::vector<QGraphicsItem*> gItems(0);
    VTransformation2D* gS = NULL;
    QGraphicsItem* gItem = NULL;
    for (int i = 0; i <  ne_grp_getNumGeometricShapes(group); ++i) {
        // get geometric shape associated with group
        gS = ne_grp_getGeometricShape(group, i);
        
        gItem = NULL;
        if (ne_gs_getShape(gS) == 2) {
            getGraphicalItemInfoFromVeneer(mw, gS, graphicalText);
            gItem = getInfoFromTextShape(gS, graphicalText);
            gItems.push_back(gItem);
        }
    }
    
    if (!gItems.size())
        gItems.push_back(graphicalText);
    
    return gItems;
}

QGraphicsItem* getInfoFromTextShape(VTransformation2D* gS, MyQGraphicsTextItem* graphicalText) {
    if (graphicalText) {
        QGraphicsItem* textItem = new MyQGraphicsTextItem(graphicalText->toPlainText(), graphicalText->boundingRect());
        
        // get the existing postion and dimension of text
        qreal x = graphicalText->x();
        qreal y = graphicalText->y();
        qreal width = graphicalText->textWidth();
        
        RAVector* rAV = NULL;
        // if the x value of text shape is set, reset the x value of graphical text
        if (ne_txt_isSetPositionX(gS)) {
            rAV = ne_txt_getPositionX(gS);
            textItem->setX(x + ne_rav_getAbsoluteValue(rAV) + width * (0.01 * ne_rav_getRelativeValue(rAV)));
        }
        
        // if the y value of text shape is set, reset the y value of graphical text
        if (ne_txt_isSetPositionY(gS)) {
            rAV = ne_txt_getPositionY(gS);
            textItem->setY(y + ne_rav_getAbsoluteValue(rAV) + (width / 3.0) * (0.01 * ne_rav_getRelativeValue(rAV)));
        }
        
        // to be done in future (other aspects)
        return textItem;
    }
    
    return NULL;
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

void setLineEndings(MainWindow* mw, GraphicalCurve* gCurve) {
    if (gCurve) {
        QPen curvePen = gCurve->getGraphicalPaths().at(0)->pen();
        bool enableRotation = true;

        
        // if the start line ending of the graphical curve is set
        if (gCurve->isSetStartLineEnding()) {
            
            // find the line ending among the graphical line endings using its id and create a copy of it if it exists
            std::vector<QGraphicsItem*> startGraphicalItems = findLineEnding(mw, gCurve->getStartLineEnding(), curvePen, enableRotation);
            
            // set the graphical items as the start graphical items of graphical curve
            gCurve->setStartGraphicalItems(startGraphicalItems);
            
            // set start enable rotation
            gCurve->setStartEnableRotation(enableRotation);
            
            for (int i = 0; i < startGraphicalItems.size(); ++i) {
                // if enable rotation is true, rotate the line ending to make it in accordance to the start slope of graphical curve
                if (enableRotation) {
                    startGraphicalItems.at(i)->setRotation(gCurve->getStartSlope());
                    
                    if (startGraphicalItems.at(i)->type() == 7) {
                        ((QGraphicsPixmapItem*)(startGraphicalItems.at(i)))->setPos(((QGraphicsPixmapItem *)(startGraphicalItems.at(i)))->x() * std::cos(gCurve->getStartSlope() * (pi / 180.0)) - ((QGraphicsPixmapItem *)(startGraphicalItems.at(i)))->y() * std::sin(gCurve->getStartSlope() * (pi / 180.0)), -((QGraphicsPixmapItem *)(startGraphicalItems.at(i)))->x() * std::sin(gCurve->getStartSlope() * (pi / 180.0)) - ((QGraphicsPixmapItem *)(startGraphicalItems.at(i)))->y() * std::cos(gCurve->getStartSlope() * (pi / 180.0)));
                    }
                }
                
                // set the position of the line ending using the start position of the graphical curve
                startGraphicalItems.at(i)->moveBy(gCurve->getStartPoint().x(), gCurve->getStartPoint().y());
                
                startGraphicalItems.at(i)->setZValue(3);
                
                // display the start line ending
                mw->getScene()->addItem(startGraphicalItems.at(i));
            }
        }
        
        enableRotation = true;
        
        // if the end line ending of the graphical curve is set
        if (gCurve->isSetEndLineEnding()) {
            
            // find the line ending among the graphical line endings using its id and create a copy of it if it exists
            std::vector<QGraphicsItem*> endGraphicalItems = findLineEnding(mw, gCurve->getEndLineEnding(), curvePen, enableRotation);
            
            // set the graphical item as the end graphical item of graphical curve
            gCurve->setEndGraphicalItems(endGraphicalItems);
            
            // set end enable rotation
            gCurve->setEndEnableRotation(enableRotation);
            
            for (int i = 0; i < endGraphicalItems.size(); ++i) {
                if (enableRotation) {
                    endGraphicalItems.at(i)->setRotation(gCurve->getEndSlope());
                    
                    if (endGraphicalItems.at(i)->type() == 7) {
                        ((QGraphicsPixmapItem*)(endGraphicalItems.at(i)))->setPos(((QGraphicsPixmapItem *)(endGraphicalItems.at(i)))->x() * std::cos(gCurve->getEndSlope() * (pi / 180.0)) - ((QGraphicsPixmapItem *)(endGraphicalItems.at(i)))->y() * std::sin(gCurve->getEndSlope() * (pi / 180.0)), -((QGraphicsPixmapItem *)(endGraphicalItems.at(i)))->x() * std::sin(gCurve->getEndSlope() * (pi / 180.0)) - ((QGraphicsPixmapItem *)(endGraphicalItems.at(i)))->y() * std::cos(gCurve->getEndSlope() * (pi / 180.0)));
                    }
                }
                
                // set the position of the line ending using the end position of the graphical curve
                endGraphicalItems.at(i)->moveBy(gCurve->getEndPoint().x(), gCurve->getEndPoint().y());
                
                endGraphicalItems.at(i)->setZValue(3);
                
                // display the end line ending
                mw->getScene()->addItem(endGraphicalItems.at(i));
            }
        }
    }
}

