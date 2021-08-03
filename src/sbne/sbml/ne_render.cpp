#include "sbne/sbml/ne_render.h"

namespace sbne {

RenderGroup* ne_grp_populateRenderGroup(VRenderGroup* g) {
    // create group
    RenderGroup* rg = new RenderGroup();

    // set id
    if (g->isSetId())
        rg->setId(g->getId());

    // set name
    if (g->isSetName())
        rg->setName(g->getName());

    // set stroke
    if (g->isSetStroke())
        rg->setStroke(g->getStroke());

    // set stroke width
    if (g->isSetStrokeWidth())
        rg->setStrokeWidth(g->getStrokeWidth());

    // set dash array
    if (g->isSetDashArray()) {
        rg->setStrokeDashArray(g->getDashArray());
    }

    // set fill
    if (g->isSetFill())
        rg->setFill(g->getFill());

    // set fill rule
    if (g->isSetFillRule())
        rg->setFillRule(g->getFillRule());

    // set start head
    if (g->isSetStartHead())
        rg->setStartHead(g->getStartHead());

    // set end head
    if (g->isSetEndHead())
        rg->setEndHead(g->getEndHead());

    // set font size
    if (g->isSetFontSize()) {
        RelAbsVector ra;
        ra.setAbsoluteValue(g->getFontSize().a());
        ra.setRelativeValue(g->getFontSize().r());
        rg->setFontSize(ra);
    }

    // set font family
    if (g->isSetFontFamily())
        rg->setFontFamily(g->getFontFamily());

    // set font weight
    if (g->isSetFontWeight())
        rg->setFontWeight(g->getFontWeight());

    // set font style
    if (g->isSetFontStyle())
        rg->setFontStyle(g->getFontStyle());

    // set horizontal text anchor
    if (g->isSetHTextAnchor())
        rg->setTextAnchor(g->getHTextAnchor());

    // set vertical text anchor
    if (g->isSetVTextAnchor())
        rg->setVTextAnchor(g->getVTextAnchor());
    
    // set transform
    if (g->isSetTransform()) {
        double* m;
        ((VTransformation2D *)g)->getTransform(m);
        rg->setTransform(m);
    }

    // add graphical shapes
    for (VRenderGroup::constElementIt j = g->elementsBegin(); j != g->elementsEnd(); ++j) {
        VTransformation2D *_t2d = *j;

        // if it is an image
        if (_t2d->getShapeAsString() == "image") {

            // create an image
            Image* image = rg->createImage();

            // set id
            if (_t2d->isSetId())
                image->setId(_t2d->getId());

            // set name
            if (_t2d->isSetName())
                image->setName(_t2d->getName());
            
            // set transform
            if (_t2d->isSetTransform()) {
                double* m;
                _t2d->getTransform(m);
                image->setTransform(m);
            }

            // set x
            if (((VImage *)_t2d)->isSetX()) {
                RelAbsVector ra;
                ra.setAbsoluteValue(((VImage *)_t2d)->getX().a());
                ra.setRelativeValue(((VImage *)_t2d)->getX().r());
                image->setX(ra);
            }

            // set y
            if (((VImage *)_t2d)->isSetY()) {
                RelAbsVector ra;
                ra.setAbsoluteValue(((VImage *)_t2d)->getY().a());
                ra.setRelativeValue(((VImage *)_t2d)->getY().r());
                image->setY(ra);
            }

            // set z
            if (((VImage *)_t2d)->isSetZ()) {
                RelAbsVector ra;
                ra.setAbsoluteValue(((VImage *)_t2d)->getZ().a());
                ra.setRelativeValue(((VImage *)_t2d)->getZ().r());
                image->setZ(ra);
            }
            
            // set width
            if (((VImage *)_t2d)->isSetWidth()) {
                RelAbsVector ra;
                ra.setAbsoluteValue(((VImage *)_t2d)->getWidth().a());
                ra.setRelativeValue(((VImage *)_t2d)->getWidth().r());
                image->setWidth(ra);
            }

            // set height
            if (((VImage *)_t2d)->isSetHeight()) {
                RelAbsVector ra;
                ra.setAbsoluteValue(((VImage *)_t2d)->getHeight().a());
                ra.setRelativeValue(((VImage *)_t2d)->getHeight().r());
                image->setHeight(ra);
            }

            // set href
            if (((VImage *)_t2d)->isSetHref()) {
                image->setHref(((VImage *)_t2d)->getHref());
            }
        }

        // if it is a render curve
        if (_t2d->getShapeAsString() == "rendercurve") {

            // create curve
            RenderCurve* curve = rg->createCurve();

            // set id
            if (_t2d->isSetId())
                curve->setId(_t2d->getId());

            // set name
            if (_t2d->isSetName())
                curve->setName(_t2d->getName());
            
            // set transform
            if (_t2d->isSetTransform()) {
                double* m;
                _t2d->getTransform(m);
                curve->setTransform(m);
            }

            // set stroke color
            if (((RCurve *)_t2d)->isSetStroke())
                curve->setStroke(((RCurve *)_t2d)->getStroke());

            // set stroke width
            if (((RCurve *)_t2d)->isSetStrokeWidth())
                curve->setStrokeWidth(((RCurve *)_t2d)->getStrokeWidth());

            // set dash array
            if (((RCurve *)_t2d)->isSetDashArray())
                curve->setStrokeDashArray(((RCurve *)_t2d)->getDashArray());

            // set start head
            if (((RCurve *)_t2d)->isSetStartHead())
                curve->setStartHead(((RCurve *)_t2d)->getStartHead());

            // set end head
            if (((RCurve *)_t2d)->isSetEndHead()) {
                curve->setEndHead(((RCurve *)_t2d)->getEndHead());
            }

            // set elements
            for (RCurve::constElementIt k = ((RCurve *)_t2d)->elementsBegin(); k != ((RCurve *)_t2d)->elementsEnd(); ++k) {
                RenderPoint* rp = NULL;
                RelAbsVector ra;

                // if it's a render cubic bezier
                if ((*k)->isRenderCubicBezier()) {
                    rp = curve->createCubicBezier();

                    /// rpoint
                    // set x
                    ra.setAbsoluteValue((*k)->getRPoint().x().a());
                    ra.setRelativeValue((*k)->getRPoint().x().r());
                    rp->setX(ra);

                    // set y
                    ra.setAbsoluteValue((*k)->getRPoint().y().a());
                    ra.setRelativeValue((*k)->getRPoint().y().r());
                    rp->setY(ra);

                    /// basepoint1
                    // set x
                    ra.setAbsoluteValue(((RCubicBezier *)(*k))->getBasePoint1().x().a());
                    ra.setRelativeValue(((RCubicBezier *)(*k))->getBasePoint1().x().r());
                   ((RenderCubicBezier *)rp)->setBasePoint1_x(ra);
                    
                    // set y
                    ra.setAbsoluteValue(((RCubicBezier *)(*k))->getBasePoint1().y().a());
                     ra.setRelativeValue(((RCubicBezier *)(*k))->getBasePoint1().y().r());
                    ((RenderCubicBezier *)rp)->setBasePoint1_y(ra);

                    /// basepoint2
                    // set x
                     ra.setAbsoluteValue(((RCubicBezier *)(*k))->getBasePoint2().x().a());
                     ra.setRelativeValue(((RCubicBezier *)(*k))->getBasePoint2().x().r());
                    ((RenderCubicBezier *)rp)->setBasePoint2_x(ra);
                     
                     // set y
                     ra.setAbsoluteValue(((RCubicBezier *)(*k))->getBasePoint2().y().a());
                      ra.setRelativeValue(((RCubicBezier *)(*k))->getBasePoint2().y().r());
                     ((RenderCubicBezier *)rp)->setBasePoint2_y(ra);
                }
                else {
                    rp = curve->createPoint();

                    ///rpoint
                    // set x
                    ra.setAbsoluteValue((*k)->getRPoint().x().a());
                    ra.setRelativeValue((*k)->getRPoint().x().r());
                    rp->setX(ra);

                    // set y
                    ra.setAbsoluteValue((*k)->getRPoint().y().a());
                    ra.setRelativeValue((*k)->getRPoint().y().r());
                    rp->setY(ra);
                }
            }
        }

        // if it is a text
        if (_t2d->getShapeAsString() == "text") {

            // create text
            Text* text = rg->createText();

            // set id
            if (_t2d->isSetId())
                text->setId(_t2d->getId());

            // set name
            if (_t2d->isSetName())
                text->setName(_t2d->getName());
            
            // set transform
            if (_t2d->isSetTransform()) {
                double* m;
                _t2d->getTransform(m);
                text->setTransform(m);
            }

            // set stroke color
            if (((VText *)_t2d)->isSetStroke())
                text->setStroke(((VText *)_t2d)->getStroke());

            // set stroke width
            if (((VText *)_t2d)->isSetStrokeWidth())
                text->setStrokeWidth(((VText *)_t2d)->getStrokeWidth());

            // set dash array
            if (((VText *)_t2d)->isSetDashArray())
                text->setStrokeDashArray(((VText *)_t2d)->getDashArray());

            // set x
            if (((VText *)_t2d)->isSetX()) {
                RelAbsVector ra;
                ra.setAbsoluteValue(((VText *)_t2d)->getX().a());
                ra.setRelativeValue(((VText *)_t2d)->getX().r());
                text->setX(ra);
            }

            // set y
            if (((VText *)_t2d)->isSetY()) {
                RelAbsVector ra;
                ra.setAbsoluteValue(((VText *)_t2d)->getY().a());
                ra.setRelativeValue(((VText *)_t2d)->getY().r());
                text->setY(ra);
            }

            // set z
            if (((VText *)_t2d)->isSetZ()) {
                RelAbsVector ra;
                ra.setAbsoluteValue(((VText *)_t2d)->getZ().a());
                ra.setRelativeValue(((VText *)_t2d)->getZ().r());
                text->setZ(ra);
            }

            // set font size
            if (((VText *)_t2d)->isSetFontSize()) {
                RelAbsVector ra;
                ra.setAbsoluteValue(((VText *)_t2d)->getFontSize().a());
                ra.setRelativeValue(((VText *)_t2d)->getFontSize().r());
                text->setFontSize(ra);
            }

            // set font family
            if (((VText *)_t2d)->isSetFontFamily())
                text->setFontFamily(((VText *)_t2d)->getFontFamily());

            // set font weight
            if (((VText *)_t2d)->isSetFontWeight())
                text->setFontWeight(((VText *)_t2d)->getFontWeight());

            // set font style
            if (((VText *)_t2d)->isSetFontStyle())
                text->setFontStyle(((VText *)_t2d)->getFontStyle());

            // set horizontal text anchor
            if (((VText *)_t2d)->isSetHTextAnchor())
                text->setTextAnchor(((VText *)_t2d)->getHTextAnchor());

            // set vertical text anchor
            if (((VText *)_t2d)->isSetVTextAnchor())
                text->setVTextAnchor(((VText *)_t2d)->getVTextAnchor());
        }

        // if it is a rectangle
        if (_t2d->getShapeAsString() == "rectangle") {

            // create rectangle
            Rectangle* rectangle = rg->createRectangle();

            // set id
            if (_t2d->isSetId())
                rectangle->setId(_t2d->getId());

            // set name
            if (_t2d->isSetName())
                rectangle->setName(_t2d->getName());
            
            // set transform
            if (_t2d->isSetTransform()) {
                double* m;
                _t2d->getTransform(m);
                rectangle->setTransform(m);
            }

            // set stroke
            if (((VRectangle *)_t2d)->isSetStroke())
                rectangle->setStroke(((VRectangle *)_t2d)->getStroke());

            // set stroke width
            if (((VRectangle *)_t2d)->isSetStrokeWidth())
                rectangle->setStrokeWidth(((VRectangle *)_t2d)->getStrokeWidth());

            // set dash array
            if (((VRectangle *)_t2d)->isSetDashArray())
                rectangle->setStrokeDashArray(((VRectangle *)_t2d)->getDashArray());

            // set fill
            if (((VRectangle *)_t2d)->isSetFill())
                rectangle->setFill(((VRectangle *)_t2d)->getFill());
            
            // set x
            if (((VRectangle *)_t2d)->isSetX()) {
                RelAbsVector ra;
                ra.setAbsoluteValue(((VRectangle *)_t2d)->getX().a());
                ra.setRelativeValue(((VRectangle *)_t2d)->getX().r());
                rectangle->setX(ra);
            }

            // set y
            if (((VRectangle *)_t2d)->isSetY()) {
                RelAbsVector ra;
                ra.setAbsoluteValue(((VRectangle *)_t2d)->getY().a());
                ra.setRelativeValue(((VRectangle *)_t2d)->getY().r());
                rectangle->setY(ra);
            }

            // set z
            if (((VRectangle *)_t2d)->isSetZ()) {
                RelAbsVector ra;
                ra.setAbsoluteValue(((VRectangle *)_t2d)->getZ().a());
                ra.setRelativeValue(((VRectangle *)_t2d)->getZ().r());
                rectangle->setZ(ra);
            }
            
            // set width
            if (((VRectangle *)_t2d)->isSetWidth()) {
                RelAbsVector ra;
                ra.setAbsoluteValue(((VRectangle *)_t2d)->getWidth().a());
                ra.setRelativeValue(((VRectangle *)_t2d)->getWidth().r());
                rectangle->setWidth(ra);
            }

            // set height
            if (((VRectangle *)_t2d)->isSetHeight()) {
                RelAbsVector ra;
                ra.setAbsoluteValue(((VRectangle *)_t2d)->getHeight().a());
                ra.setRelativeValue(((VRectangle *)_t2d)->getHeight().r());
                rectangle->setHeight(ra);
            }

            // set rx
            if (((VRectangle *)_t2d)->isSetRX()) {
                RelAbsVector ra;
                ra.setAbsoluteValue(((VRectangle *)_t2d)->getRX().a());
                ra.setRelativeValue(((VRectangle *)_t2d)->getRX().r());
                rectangle->setRX(ra);
            }


            // set ry
            if (((VRectangle *)_t2d)->isSetRY()) {
                RelAbsVector ra;
                ra.setAbsoluteValue(((VRectangle *)_t2d)->getRY().a());
                ra.setRelativeValue(((VRectangle *)_t2d)->getRY().r());
                rectangle->setRY(ra);
            }

            // set ratio
            if (((VRectangle *)_t2d)->isSetRatio())
                rectangle->setRatio(((VRectangle *)_t2d)->getRatio());
        }

        // if it is an ellipse
        if (_t2d->getShapeAsString() == "ellipse") {

            // create ellipse
            Ellipse* ellipse = rg->createEllipse();

            // set id
            if (_t2d->isSetId())
                ellipse->setId(_t2d->getId());

            // set name
            if (_t2d->isSetName())
                ellipse->setName(_t2d->getName());
            
            // set transform
            if (_t2d->isSetTransform()) {
                double* m;
                _t2d->getTransform(m);
                ellipse->setTransform(m);
            }

            // set stroke
            if (((VEllipse *)_t2d)->isSetStroke())
                ellipse->setStroke(((VEllipse *)_t2d)->getStroke());

            // set stroke width
            if (((VEllipse *)_t2d)->isSetStrokeWidth())
                ellipse->setStrokeWidth(((VEllipse *)_t2d)->getStrokeWidth());

            // set dash array
            if (((VEllipse *)_t2d)->isSetDashArray())
                ellipse->setStrokeDashArray(((VEllipse *)_t2d)->getDashArray());

            // set fill
            if (((VEllipse *)_t2d)->isSetFill())
                ellipse->setFill(((VEllipse *)_t2d)->getFill());

            // set cx
            if (((VEllipse *)_t2d)->isSetCX()) {
                RelAbsVector ra;
                ra.setAbsoluteValue(((VEllipse *)_t2d)->getCX().a());
                ra.setRelativeValue(((VEllipse *)_t2d)->getCX().r());
                ellipse->setCX(ra);
            }

            // set cy
            if (((VEllipse *)_t2d)->isSetCY()) {
                RelAbsVector ra;
                ra.setAbsoluteValue(((VEllipse *)_t2d)->getCY().a());
                ra.setRelativeValue(((VEllipse *)_t2d)->getCY().r());
                ellipse->setCY(ra);
            }

            // set cz
            if (((VEllipse *)_t2d)->isSetCZ()) {
                RelAbsVector ra;
                ra.setAbsoluteValue(((VEllipse *)_t2d)->getCZ().a());
                ra.setRelativeValue(((VEllipse *)_t2d)->getCZ().r());
                ellipse->setCZ(ra);
            }

            // set rx
            if (((VEllipse *)_t2d)->isSetRX()) {
                RelAbsVector ra;
                ra.setAbsoluteValue(((VEllipse *)_t2d)->getRX().a());
                ra.setRelativeValue(((VEllipse *)_t2d)->getRX().r());
                ellipse->setRX(ra);
            }

            // set ry
            if (((VEllipse *)_t2d)->isSetRY()) {
                RelAbsVector ra;
                ra.setAbsoluteValue(((VEllipse *)_t2d)->getRY().a());
                ra.setRelativeValue(((VEllipse *)_t2d)->getRY().r());
                ellipse->setRY(ra);
            }

            // set ratio
            if (((VEllipse *)_t2d)->isSetRatio())
                ellipse->setRatio(((VEllipse *)_t2d)->getRatio());
        }

        // if it is a polygon
        if (_t2d->getShapeAsString() == "polygon") {

            // create polygon
            Polygon* polygon = rg->createPolygon();

            // set id
            if (_t2d->isSetId())
                polygon->setId(_t2d->getId());

            // set name
            if (_t2d->isSetName())
                polygon->setName(_t2d->getName());
            
            // set transform
            if (_t2d->isSetTransform()) {
                double* m;
                _t2d->getTransform(m);
                polygon->setTransform(m);
            }

            // set stroke
            if (((VPolygon *)_t2d)->isSetStroke())
                polygon->setStroke(((VPolygon *)_t2d)->getStroke());

            // set stroke width
            if (((VPolygon *)_t2d)->isSetStrokeWidth())
                polygon->setStrokeWidth(((VPolygon *)_t2d)->getStrokeWidth());

            // set dash array
            if (((VPolygon *)_t2d)->isSetDashArray())
                polygon->setStrokeDashArray(((VPolygon *)_t2d)->getDashArray());

            // set fill
            if (((VPolygon *)_t2d)->isSetFill())
                polygon->setFill(((VPolygon *)_t2d)->getFill());

            // set fill rule
            if (((VPolygon *)_t2d)->isSetFillRule())
                polygon->setFillRule(((VPolygon *)_t2d)->getFillRule());

            // set elements
            for (VPolygon::constElementIt k = ((VPolygon *)_t2d)->elementsBegin(); k != ((VPolygon *)_t2d)->elementsEnd(); ++k) {
                RenderPoint* rp = NULL;
                RelAbsVector ra;
                
                // if it's a render cubic bezier
                if ((*k)->isRenderCubicBezier()) {
                    rp = polygon->createCubicBezier();

                    /// rpoint
                    // set x
                    ra.setAbsoluteValue((*k)->getRPoint().x().a());
                    ra.setRelativeValue((*k)->getRPoint().x().r());
                    rp->setX(ra);

                    // set y
                    ra.setAbsoluteValue((*k)->getRPoint().y().a());
                    ra.setRelativeValue((*k)->getRPoint().y().r());
                    rp->setY(ra);

                    /// basepoint1
                    // set x
                    ra.setAbsoluteValue(((RCubicBezier *)(*k))->getBasePoint1().x().a());
                    ra.setRelativeValue(((RCubicBezier *)(*k))->getBasePoint1().x().r());
                   ((RenderCubicBezier *)rp)->setBasePoint1_x(ra);
                    
                    // set y
                    ra.setAbsoluteValue(((RCubicBezier *)(*k))->getBasePoint1().y().a());
                     ra.setRelativeValue(((RCubicBezier *)(*k))->getBasePoint1().y().r());
                    ((RenderCubicBezier *)rp)->setBasePoint1_y(ra);

                    /// basepoint2
                    // set x
                     ra.setAbsoluteValue(((RCubicBezier *)(*k))->getBasePoint2().x().a());
                     ra.setRelativeValue(((RCubicBezier *)(*k))->getBasePoint2().x().r());
                    ((RenderCubicBezier *)rp)->setBasePoint2_x(ra);
                     
                     // set y
                     ra.setAbsoluteValue(((RCubicBezier *)(*k))->getBasePoint2().y().a());
                      ra.setRelativeValue(((RCubicBezier *)(*k))->getBasePoint2().y().r());
                     ((RenderCubicBezier *)rp)->setBasePoint2_y(ra);
                }
                else {
                    rp = polygon->createPoint();

                    ///rpoint
                    // set x
                    ra.setAbsoluteValue((*k)->getRPoint().x().a());
                    ra.setRelativeValue((*k)->getRPoint().x().r());
                    rp->setX(ra);

                    // set y
                    ra.setAbsoluteValue((*k)->getRPoint().y().a());
                    ra.setRelativeValue((*k)->getRPoint().y().r());
                    rp->setY(ra);
                }
            }
        }
    }

    return rg;
}

// SBMLDocument

RenderInfo* ne_doc_processRenderInfo(SBMLDocument* doc) {
    RenderInfo* r = new RenderInfo();
    GlobalRenderInformation* gri = NULL;
    LocalRenderInformation* lri = NULL;
    Veneer* ven = new Veneer();
    
    // get the model from SBMLDocument
    Model* mod = doc->getModel();
    
    // get the layout plugin from model
    SBasePlugin* layoutBase = mod->getPlugin("layout");
    if (layoutBase) {
        LayoutModelPlugin* layoutPlugin = NULL;
        
        // cast sbase to the layout model plugin
        try {
            layoutPlugin = dynamic_cast<LayoutModelPlugin*>(layoutBase);
        }
        catch(std::bad_cast) {
            std::cerr << "Unable to get layout information\n";
        }
        
        // get the global render plugin from list of layouts
        ListOfLayouts* lOfLayouts = layoutPlugin->getListOfLayouts();
        SBasePlugin* gRenderBase = lOfLayouts->getPlugin("render");
        if (gRenderBase) {
            RenderListOfLayoutsPlugin* gRenderPlugin = NULL;
            
            // cast sbase to the render list of layouts plugin
            try {
                gRenderPlugin = dynamic_cast<RenderListOfLayoutsPlugin*>(gRenderBase);
            }
            catch(std::bad_cast) {
                std::cerr << "Unable to get global render information\n";
            }
            if (gRenderPlugin->getNumGlobalRenderInformationObjects() > 1)
                std::cout <<"Warning: Multiple golobal renders. Using first" << std::endl;
            gri = gRenderPlugin->getRenderInformation(0);
            
            ListOfGlobalRenderInformation* lOfGRender = gRenderPlugin->getListOfGlobalRenderInformation();
            if(lOfGRender) {
                // get major version
                if (lOfGRender->isSetMajorVersion())
                    r->majorVersion = lOfGRender->getMajorVersion();
                else
                    r->majorVersion = 0;
                // get minor version
                if (lOfGRender->isSetMinorVersion())
                    r->minorVersion = lOfGRender->getMinorVersion();
                else
                    r->minorVersion = 0;
            }
        }
        
        // get the local render plugin from layout
        if (lOfLayouts->size() > 1)
            std::cout <<"Warning: Multiple list of layouts. Using first" << std::endl;
        Layout* layout = lOfLayouts->get(0);
        if (layout) {
            SBasePlugin* lRenderBase = layout->getPlugin("render");
            if (lRenderBase) {
                RenderLayoutPlugin* lRenderPlugin = NULL;
                
                // cast sbase to the render layout plugin
                try {
                    lRenderPlugin = dynamic_cast<RenderLayoutPlugin*>(lRenderBase);
                }
                catch(std::bad_cast) {
                    std::cerr << "Unable to get local render information\n";
                }
                ListOfLocalRenderInformation* lOfLRender = lRenderPlugin->getListOfLocalRenderInformation();
                if (lOfLRender->size() > 1)
                    std::cout <<"Warning: Multiple local renders. Using first" << std::endl;
                lri = lOfLRender->get(0);
            }
        }
    }
    
    // get the veneer from render information
    // global render
    if (gri)
        ven = veneerFromRender(*gri, ven);
    
    // local render
    if(lri)
        ven = veneerFromRender(*lri, ven);
    
    // get sbml level
    r->level = doc->getLevel();
    
    // get sbml version
    r->version = doc->getVersion();
    
    // get veneer
    r->ven = ven;

    return r;
}

SBMLDocument* ne_doc_populateSBMLdocWithRenderInfo(SBMLDocument* doc, RenderInfo* r) {
    if (doc) {
        GlobalRenderInformation* gri = NULL;
        LocalRenderInformation* lri = NULL;
        
        // get the model from SBMLDocument
        Model *mod = doc->getModel();
        
        // get the layout plugin from the model
        SBasePlugin* layoutBase = mod->getPlugin("layout");
        if (layoutBase) {
            LayoutModelPlugin* layoutPlugin = NULL;
            
             // cast sbase to the layout model plugin
            try {
                layoutPlugin = dynamic_cast<LayoutModelPlugin*>(layoutBase);
            }
            catch(std::bad_cast) {
                std::cerr << "Unable to get layout information\n";
            }
            
            // get list of layouts
            ListOfLayouts* lOfLayouts = layoutPlugin->getListOfLayouts();
            
            /// global render information
            // get the global render plugin from list of layouts
            SBasePlugin* gRenderBase = lOfLayouts->getPlugin("render");
            
            // if render package is not enabled
            if (!gRenderBase) {
                if (doc->getLevel() == 2)
                    doc->enablePackage(RenderExtension::getXmlnsL2(), "render",  true);
                else if (doc->getLevel() == 3)
                    doc->enablePackage(RenderExtension::getXmlnsL3V1V1(), "render", true);
                gRenderBase = lOfLayouts->getPlugin("render");
            }
            RenderListOfLayoutsPlugin* gRenderPlugin = NULL;
            
            // cast sbase to the render list of layouts plugin
            try {
                gRenderPlugin = dynamic_cast<RenderListOfLayoutsPlugin*>(gRenderBase);
            } catch(std::bad_cast) {
                std::cerr << "Unable to get global render information\n";
            }
            
            // remove the existing global render plugins
            while (gRenderPlugin->getNumGlobalRenderInformationObjects())
                gRenderPlugin->removeGlobalRenderInformation(0);
            
            // create a new global redner plugin
            gri = gRenderPlugin->createGlobalRenderInformation();
            
            ListOfGlobalRenderInformation* lOfGRender = gRenderPlugin->getListOfGlobalRenderInformation();
            if(lOfGRender) {
                // set major version
                if (r->majorVersion)
                    lOfGRender->setMajorVersion(r->majorVersion);
                               
                // set minor version
                if (r->minorVersion)
                    lOfGRender->setMinorVersion(r->minorVersion);
            }
            
            // set the global render id
            gri->setId("SBNE_GlobalRender");
            
            // set program name
            gri->setProgramName("SBNE");
            
            // set program version
            gri->setProgramVersion(std::string("1"));
            
            /// local render information
            // get layout from the list of layouts
            Layout* layout = lOfLayouts->get(0);
            if (lOfLayouts->size() > 1)
                std::cout << "Warning: Multiple list of layouts. Using first" << std::endl;
            if (layout) {
                
                // get the local render plugin from layout
                SBasePlugin* lRenderBase = layout->getPlugin("render");
                if (lRenderBase) {
                    RenderLayoutPlugin* lRenderPlugin = NULL;
                    
                    // cast sbase to the render layout plugin
                    try {
                        lRenderPlugin = dynamic_cast<RenderLayoutPlugin*>(lRenderBase);
                    }
                    catch(std::bad_cast) {
                        std::cerr << "Unable to get local render information\n";;
                    }
                    ListOfLocalRenderInformation* lofLRender = lRenderPlugin->getListOfLocalRenderInformation();
                    
                    // remove the existing local render plugins
                    while (lofLRender->getNumLocalRenderInformation())
                        lri = lofLRender->remove(0);
                    
                    // create a new local render plugin
                    lri = lofLRender->createLocalRenderInformation();
                    
                    // set the local render id
                    lri->setId("SBNE_LocalRender");
                }
            }
        }
        else
            std::cerr << "No plugin named layout\n";
         
        Veneer* ven = (Veneer*)r->ven;

        // add the veneer info to render info
        if (ven) {
            // set background color
            if (ven->isSetBackgroundColor())
                gri->setBackgroundColor(ven->getBackgroundColor());
            
            // add colors
            ColorDefinition* cd = NULL;
            for (Veneer::constColorIt i = ven->colorsBegin(); i != ven->colorsEnd(); ++i) {
                const VColorDefinition* c = *i;
                
                // create a color definition
                cd = gri->createColorDefinition();

                // set id
                if (c->isSetId())
                    cd->setId(c->getId());

                // set name
                if (c->isSetName())
                    cd->setName(c->getName());

                // set color value
                if (c->isSetValue() && c->getValue() != colorStringToHexStr(CLR_STR_NONE))
                    cd->setValue(c->getValue());
            }
            
            // add gradients
            GradientBase* gb = NULL;
            for (Veneer::constGradientIt i = ven->gradientsBegin(); i != ven->gradientsEnd(); ++i) {
                const VGradientBase* g = *i;

                // create a linear gradient if it is a linear gradient
                if (g->isSetAsLinearGradient())
                    gb = gri->createLinearGradientDefinition();

                // create a radial gradient if it is a radial gradient
                if (g->isSetAsRadialGradient())
                    gb = gri->createRadialGradientDefinition();

                // set id
                if (g->isSetId())
                    gb->setId(g->getId());

                // set name
                if (g->isSetName())
                    gb->setName(g->getName());

                // set spread method
                if (g->isSetSpreadMethod())
                    gb->setSpreadMethod(g->getSpreadMethod());
                
                // add gradient stops
                GradientStop* gs = NULL;
                for (VGradientBase::constStopIt j = g->stopsBegin(); j != g->stopsEnd(); ++j) {
                    const VGradientStop* s = *j;

                    // create gradient stop
                    gs = gb->createGradientStop();

                    // set id
                    if (s->isSetId())
                        gs->setId(s->getId());

                    // set name
                    if (s->isSetName())
                        gs->setName(s->getName());

                    // set stop color
                    if (s->isSetStopColor())
                        gs->setStopColor(s->getStopColor());

                    // set offset
                    if (s->isSetOffset())
                        gs->setOffset(s->getOffset().a(), s->getOffset().r());
                }

                // if it is a linear gradient
                if (g->isSetAsLinearGradient()) {
                    // cast the gradient to a linear gradient
                    LinearGradient* lg = NULL;
                    try {
                         lg = dynamic_cast<LinearGradient*>(gb);
                    }
                    catch(std::bad_cast) {
                         std::cerr << "Unable to cast the gradient to linear gradient\n";
                    }

                    // set x1
                    if (((VLinearGradient*)g)->isSetX1()) {
                        RelAbsVector ra;
                        ra.setAbsoluteValue(((VLinearGradient*)g)->getX1().a());
                        ra.setRelativeValue(((VLinearGradient*)g)->getX1().r());
                        lg->setX1(ra);
                    }
                    
                    // set y1
                    if (((VLinearGradient*)g)->isSetY1()) {
                        RelAbsVector ra;
                        ra.setAbsoluteValue(((VLinearGradient*)g)->getY1().a());
                        ra.setRelativeValue(((VLinearGradient*)g)->getY1().r());
                        lg->setY1(ra);
                    }

                    // set z1
                    if (((VLinearGradient*)g)->isSetZ1()) {
                        RelAbsVector ra;
                        ra.setAbsoluteValue(((VLinearGradient*)g)->getZ1().a());
                        ra.setRelativeValue(((VLinearGradient*)g)->getZ1().r());
                        lg->setZ1(ra);
                    }

                    // set x2
                    if (((VLinearGradient*)g)->isSetX2()) {
                        RelAbsVector ra;
                        ra.setAbsoluteValue(((VLinearGradient*)g)->getX2().a());
                        ra.setRelativeValue(((VLinearGradient*)g)->getX2().r());
                        lg->setX2(ra);
                    }

                    // set y2
                    if (((VLinearGradient*)g)->isSetY2()) {
                        RelAbsVector ra;
                        ra.setAbsoluteValue(((VLinearGradient*)g)->getY2().a());
                        ra.setRelativeValue(((VLinearGradient*)g)->getY2().r());
                        lg->setY2(ra);
                    }

                    // set z2
                    if (((VLinearGradient*)g)->isSetZ2()) {
                        RelAbsVector ra;
                        ra.setAbsoluteValue(((VLinearGradient*)g)->getZ2().a());
                        ra.setRelativeValue(((VLinearGradient*)g)->getZ2().r());
                        lg->setZ2(ra);
                    }
                }

                // if it is a radial gradient
                if (g->isSetAsRadialGradient()) {
                    // cast the gradient to a radial gradient
                    RadialGradient* rg = NULL;
                    try {
                         rg = dynamic_cast<RadialGradient*>(gb);
                    }
                    catch(std::bad_cast) {
                         std::cerr << "Unable to cast the gradient to radial gradient\n";
                    }

                    // set cx
                    if (((VRadialGradient*)g)->isSetCx()) {
                        RelAbsVector ra;
                        ra.setAbsoluteValue(((VRadialGradient*)g)->getCx().a());
                        ra.setRelativeValue(((VRadialGradient*)g)->getCx().r());
                        rg->setCx(ra);
                    }
            
                    // set cy
                    if (((VRadialGradient*)g)->isSetCy()) {
                        RelAbsVector ra;
                        ra.setAbsoluteValue(((VRadialGradient*)g)->getCy().a());
                        ra.setRelativeValue(((VRadialGradient*)g)->getCy().r());
                        rg->setCy(ra);
                    }

                    // set cz
                    if (((VRadialGradient*)g)->isSetCz()) {
                        RelAbsVector ra;
                        ra.setAbsoluteValue(((VRadialGradient*)g)->getCz().a());
                        ra.setRelativeValue(((VRadialGradient*)g)->getCz().r());
                        rg->setCz(ra);
                    }

                    // set fx
                    if (((VRadialGradient*)g)->isSetFx()) {
                        RelAbsVector ra;
                        ra.setAbsoluteValue(((VRadialGradient*)g)->getFx().a());
                        ra.setRelativeValue(((VRadialGradient*)g)->getFx().r());
                        rg->setFx(ra);
                    }

                    // set fy
                    if (((VRadialGradient*)g)->isSetFy()) {
                        RelAbsVector ra;
                        ra.setAbsoluteValue(((VRadialGradient*)g)->getFy().a());
                        ra.setRelativeValue(((VRadialGradient*)g)->getFy().r());
                        rg->setFy(ra);
                    }

                    // set fz
                    if (((VRadialGradient*)g)->isSetFz()) {
                        RelAbsVector ra;
                        ra.setAbsoluteValue(((VRadialGradient*)g)->getFz().a());
                        ra.setRelativeValue(((VRadialGradient*)g)->getFz().r());
                        rg->setFz(ra);
                    }

                    // set r
                    if (((VRadialGradient*)g)->isSetR()) {
                        RelAbsVector ra;
                        ra.setAbsoluteValue(((VRadialGradient*)g)->getR().a());
                        ra.setRelativeValue(((VRadialGradient*)g)->getR().r());
                        rg->setR(ra);
                    }
                }
            }
            
            // add styles
            Style* sb = NULL;
            for (Veneer::constStyleIt i = ven->stylesBegin(); i != ven->stylesEnd(); ++i) {
                VGlobalStyle* s = *i;

                // if it is a local style
                if (s->isLocalStyle())
                    sb = lri->createStyle(s->getId());
                // if it is a global style
                else
                    sb = gri->createStyle(s->getId());

                // set name
                if (s->isSetName())
                    sb->setName(s->getName());

                // set role list
                if (s->isSetRoleList())
                    sb->setRoleList(s->getRoleList());

                // set type list
                if (s->isSetTypeList())
                    sb->setTypeList(s->getTypeList());
                
                // if it is a local style
                if (s->isLocalStyle()) {
                    // set id list
                    if (((VLocalStyle *)s)->isSetIdList())
                        ((LocalStyle*)sb)->setIdList(((VLocalStyle *)s)->getIdList());
                }
                
                // set render group
                if (s->isSetGroup()) {
                    // get the render group info from veneer
                    RenderGroup* rg = sb->createGroup();
                    rg = ne_grp_populateRenderGroup(s->getGroup());
                    
                    // set render group to the style
                    sb->setGroup(rg);
                }
            }
            
            // add line endings
            LineEnding* le = NULL;
            for (Veneer::constLineEndingIt i = ven->lineEndingsBegin(); i != ven->lineEndingsEnd(); ++i) {
                VLineEnding* l = *i;

                // create line endig
                le = gri->createLineEnding();

                // set id
                if (l->isSetId())
                    le->setId(l->getId());

                // set name
                if (l->isSetName())
                    le->setName(l->getName());

                // set enable rotation mapping
                if (l->isSetEnableRotationMapping())
                    le->setEnableRotationalMapping(l->getEnableRotationMapping());

                // set bounding box
                if (l->isSetBox()) {
                    // create bounding box
                    BoundingBox* bb = le->createBoundingBox();

                    // set position
                    bb->setX(l->getBox()->x());
                    bb->setY(l->getBox()->y());

                    // set dimensions
                    bb->setWidth(l->getBox()->width());
                    bb->setHeight(l->getBox()->height());
                }

                // set render group
                if (l->isSetGroup()) {
                    // get the render group info from veneer
                    RenderGroup* rg = le->createGroup();
                    rg = ne_grp_populateRenderGroup(l->getGroup());
                    
                    // set the render group to the line ending
                    le->setGroup(rg);
                }
            }
        }
    }
    
    return doc;
}

// RenderInfo

Veneer* ne_ri_getVeneer(RenderInfo* r) {
    if (r->ven)
        return (Veneer*)(r->ven);
    
    return NULL;
}

int ne_ri_setVeneer(RenderInfo* r, Veneer* ven) {
    if (ven) {
        r->ven = ven;
        return 0;
    }
    
    return -1;
}

Veneer* ne_ri_addDefaultRenderFeaturesToVeneer(RenderInfo* r) {
    if (r) {
        Veneer* ven = ne_ri_getVeneer(r);
        
        // set background color
        ne_ven_setBackgroundColor(ven, colorStringToStr(CLR_STR_LIGHT_GRAY));
        
        // add default colors
        for (int i = 0; i < NUM_ColorString; i++)
            ne_ven_addNewColor(ven, colorStringToStr(ColorString(i)));
            
        // add lineendings
        for (int i = 0; i < NUM_RxnRoleType; i++) {
            if (rxnRoleToString(RxnRoleType(i)) != "substrate" && rxnRoleToString(RxnRoleType(i)) != "side substrate") {
                
                // creare a new line ending and set its value using the reaction role type
                ne_ven_addNewLineEnding(ven, RxnRoleType(i));
            }
        }
        
        // add type styles
        for (int i = 0; i < NUM_StyleType; i++) {
            // creare a new style and set its value using the style type
            ne_ven_addNewGlobalStyle(ven, StyleType(i), NUM_RxnRoleType);
        }
           
        // add role styles
        for (int i = 0; i < NUM_RxnRoleType; i++) {
            // creare a new style and set its value using the reaction role type
            ne_ven_addNewGlobalStyle(ven, NUM_StyleType, RxnRoleType(i));
        }
        
        ven->setRenderSpecified(true);
        
        ne_ri_setVeneer(r, ven);
        
        return ven;
    }
    
    return NULL;
}

// VenerElement

bool ne_ve_isSetId(VeneerElement* vE) {
   if (vE)
       return vE->isSetId();
   
   return false;
}

const std::string ne_ve_getId(VeneerElement* vE) {
    if (vE)
        return vE->getId();
   
    return std::string();
}

int ne_ve_setId(Veneer* ven, VeneerElement* vE, const std::string& elementId) {
    if (ven && vE && !ven->findColorById(elementId) && !ven->findGradientById(elementId) && !ven->findStyleById(elementId) && !ven->findLineEndingById(elementId)) {
        vE->setId(elementId);
        return 0;
    }
    
    return -1;
}

bool ne_ve_isSetName(VeneerElement* vE) {
    if (vE)
        return vE->isSetName();
       
    return false;
}

const std::string ne_ve_getName(VeneerElement* vE) {
    if (vE)
        return vE->getName();
       
    return std::string();
}

int ne_ve_setName(VeneerElement* vE, const std::string& name) {
    if (vE) {
        vE->setName(name);
        return 0;
    }
    
    return -1;
}

// Veneer

bool ne_ven_isRenderSpecified(Veneer* ven) {
    if (ven)
        return ven->isRenderSpecified();
    
    return false;
}

bool ne_ven_isSetBackgroundColor(Veneer* ven) {
    if (ven)
        return ven->isSetBackgroundColor();
    
    return false;
}

const std::string ne_ven_getBackgroundColor(Veneer* ven) {
    if (ven && ven->isSetBackgroundColor())
        return ven->getBackgroundColor();

    return std::string();
}

int ne_ven_setBackgroundColor(Veneer* ven, const std::string& color) {
    if (ven) {
        ven->setBackgroundColor(color);
        return 0;
    }
    
    return -1;
}

const size_t ne_ven_getNumColors(Veneer* ven) {
    if (ven)
        return ven->getNumColors();
    
    return 0;
}

VColorDefinition* ne_ven_getColor(Veneer* ven, const unsigned int& index, const std::string& colorId) {
    if (ven) {
        if (index >= 0 && index < int(ven->getNumColors()))
            return ven->getColors().at(index);
        
        if (!colorId.empty())
            return ven->findColorById(colorId);
    }
    
    return NULL;
}

Veneer::colorVec ne_ven_getColors(Veneer* ven) {
    if (ven)
        return ven->getColors();
    
    return Veneer::colorVec(0);
}

VColorDefinition* ne_ven_addNewColor(Veneer* ven, std::string colorId, std::string value) {
    if (ven) {
        if (!colorId.empty() && value.empty()) {
            // check to see if the color already exists
            for (int i = 0; i < NUM_ColorString; ++i) {
                if (stringCompare(colorStringToStr(ColorString(i)), colorId))
                    value = colorStringToHexStr(ColorString(i));
            }
        }
        else if (colorId.empty())
            colorId = ven->getColorUniqueId();
        
        if (!ven->findColorById(colorId)) {
            // create new color
            VColorDefinition* c = new VColorDefinition();
            
            // set id
            c->setId(colorId);
            
            // set value
            if (!value.empty())
                c->setValue(value);
            
            // add the color to veneer
            ven->addColor(c);
            
            return c;
        }
    }
    
    return NULL;
}

const size_t ne_ven_getNumGradients(Veneer* ven) {
    if (ven)
        return ven->getNumGradients();
    
    return 0;
}

VGradientBase* ne_ven_getGradient(Veneer* ven, const unsigned int& index, const std::string& gradientId) {
    if (ven) {
        if (index >= 0 && index < int(ven->getNumGradients()))
            return ven->getGradients().at(index);
        
        if (!gradientId.empty())
            return ven->findGradientById(gradientId);
    }
    
    return NULL;
}

Veneer::gradientVec ne_ven_getGradients(Veneer* ven) {
    if (ven)
        return ven->getGradients();
    
    return Veneer::gradientVec(0);
}

const size_t ne_ven_getNumLineEndings(Veneer* ven) {
    if (ven)
        return ven->getNumLineEndings();
    
    return 0;
}

VLineEnding* ne_ven_getLineEnding(Veneer* ven, const unsigned int& index, const std::string& lineendingId) {
    if (ven) {
        if (index >= 0 && index < int(ven->getNumLineEndings()))
            return ven->getLineEndings().at(index);
        
        if (!lineendingId.empty())
            return ven->findLineEndingById(lineendingId);
    }
    
    return NULL;
}

Veneer::lineEndingVec ne_ven_getLineEndings(Veneer* ven) {
    if (ven)
        return ven->getLineEndings();
    
    return Veneer::lineEndingVec(0);
}

VLineEnding* ne_ven_addNewLineEnding(Veneer* ven, RxnRoleType role, std::string lineEndingId) {
    if (ven) {
        // create a new line ending
        VLineEnding* lE = new VLineEnding();
        
        // set the the id of line ending
        if (!lineEndingId.empty())
            lE->setId(lineEndingId);
        else
            lE->setId(ven->getLineEndingUniqueId());
        
        if (role < NUM_RxnRoleType)
            ne_le_setLineEndingValues(ven, lE, role);
        
        // add the line ending to veneer
        ven->addLineEnding(lE);
        
        return lE;
    }
    
    return NULL;
}

VLineEnding* ne_ven_getLocalLineEndingFromGlobalLineEnding(Veneer* ven, VLineEnding* gLE) {
    if (ven && gLE) {
        if (gLE->isLocalLineEnding())
            return gLE;
        else if (gLE->isSetGroup() && gLE->isSetBox()) {
            // create a local line ending
            VLineEnding* lLE = new VLineEnding();
            
            // set the id of new line ending
            if (gLE->isSetId())
                lLE->setId(ven->getLineEndingUniqueId(gLE->getId()));
            else
                lLE->setId(ven->getLineEndingUniqueId());
            
            // set the group of the line ending
            VRenderGroup* group = new VRenderGroup();
            *group = *(gLE->getGroup());
            lLE->setGroup(group);
            
            // set the box of the line ending
            LBox* box = new LBox(gLE->getBox()->x(), gLE->getBox()->y(), gLE->getBox()->width(), gLE->getBox()->height());
            if (gLE->getBox()->isSetId())
                box->setId(gLE->getBox()->getId());
            lLE->setBox(box);
            
            // set the rotation of the line ending
            if (gLE->isSetEnableRotationMapping())
                lLE->setEnableRotationMapping(gLE->getEnableRotationMapping());
            
            // set as local line ending
            lLE->setAsLocalLineEnding(true);
            
            // add the new line ending to veneer
            ven->addLineEnding(lLE);
            
            return lLE;
        }
    }
    
    return NULL;
}

const size_t ne_ven_getNumStyles(Veneer* ven) {
    if (ven)
        return ven->getNumStyles();
    
    return 0;
}

VGlobalStyle* ne_ven_getStyle(Veneer* ven, const unsigned int& index, const std::string& styleId) {
    if (ven) {
        if (index >= 0 && index < int(ven->getNumStyles()))
            return ven->getStyles().at(index);
        
        if (!styleId.empty())
            return ven->findStyleById(styleId);
    }
    
    return NULL;
}

Veneer::styleVec ne_ven_getStyles(Veneer* ven) {
    if (ven)
        return ven->getStyles();
    
    return Veneer::styleVec(0);
}

VGlobalStyle* ne_ven_addNewGlobalStyle(Veneer* ven, StyleType type, RxnRoleType role, std::string styleId) {
    if (ven) {
        // create a new global style
        VGlobalStyle* s = new VGlobalStyle();
        
        // set the the id of global style
        if (!styleId.empty())
            s->setId(styleId);
        else
            s->setId(ven->getStyleUniqueId());
        
        // set the default values of style based on its style type or reaction role
        if (type < NUM_StyleType)
            ne_stl_setStyleValues(s, type, NUM_RxnRoleType);
        else if (role < NUM_RxnRoleType)
            ne_stl_setStyleValues(s, NUM_StyleType, role);
        
        // add the global style to veneer
        ven->addStyle(s);
        
        return s;
    }
    
    return NULL;
}

VLocalStyle* ne_ven_addNewLocalStyle(Veneer* ven, std::string styleId) {
    if (ven) {
        // create a new local style
        VLocalStyle* s = new VLocalStyle();
        
        // set the the id of local style
        if (!styleId.empty())
            s->setId(styleId);
        else
            s->setId(ven->getStyleUniqueId());
        
        // set the default values of style
        ne_stl_setStyleValues(s);
        
        // add the local style to veneer
        ven->addStyle(s);
        
        return s;
    }
    
    return NULL;
}

VGlobalStyle* ne_ven_findStyle(Veneer* ven, NGraphicalObject* gO, StyleType type) {
    VGlobalStyle* style = NULL;
    
    if (ven) {
        if (gO) {
            // get style using graphical object id by searching in the style's idlist
            if (gO->isSetGlyphId()) {
                style = ven->findStyleByIdListMember(gO->getGlyphId());
                if (style)
                    return style;
            }
            
            // get style using the role of the graphical object by searching in the style's rolelist
            if (gO->getType() == 3 && ((NSpeciesReference*)gO)->isSetRole()) {
                style = ven->findStyleByRoleListMember(rxnRoleToString(((NSpeciesReference*)gO)->getRole()));
                if (style)
                    return style;
            }
                
            
            // get style using the type of the graphical object by searching in the style's typelist
            style = ven->findStyleByTypeListMember(styleTypeToStr(styleTypeForGraphicalObjectType(gO->getType())));
        }
        else if (type < NUM_StyleType)
            style = ven->findStyleByTypeListMember(styleTypeToStr(type));
    }
    
    return style;
}

VLocalStyle* ne_ven_getLocalStyleFromGlobalStyle(Veneer* ven, NGraphicalObject* gO, VGlobalStyle* gStyle) {
    if (ven && gO && gStyle && gStyle->isSetGroup()) {
        if (gStyle->isLocalStyle())
            return (VLocalStyle*)gStyle;
        else if (gO->isSetGlyphId()) {
            // create a new local style
            VLocalStyle* lStyle = new VLocalStyle();
            
            // set the id of local style
            lStyle->setId(ven->getStyleUniqueId());
            
            // add the graphical object id to the idlist of the style
            lStyle->addToIdList(gO->getGlyphId());
            
            // set the group of the style
            VRenderGroup* group = new VRenderGroup();
            *group = *(gStyle->getGroup());
            lStyle->setGroup(group);
            
            // add the local style to veneer
            ven->addStyle(lStyle);
            
            return lStyle;
        }
    }
    
    return NULL;
}

// Color

bool ne_clr_isSetValue(VColorDefinition* c) {
    if (c)
        return c->isSetValue();
    
    return false;
}

const std::string ne_clr_getValue(VColorDefinition* c) {
    if (c)
        return c->getValue();
    
    return std::string();
}

int ne_clr_setValue(VColorDefinition* c, const std::string& value) {
    if (c) {
        c->setValue(value);
        return 0;
    }
    
    return -1;
}

// VGradientBase

bool ne_grd_isSetSpreadMethod(VGradientBase* g) {
    if (g)
        return g->isSetSpreadMethod();
    
    return false;
}

const std::string ne_grd_getSpreadMethod(VGradientBase* g) {
    if (g)
        return g->getSpreadMethod();
    
    return std::string();
}

const size_t ne_grd_getNumStops(VGradientBase* g) {
    if (g)
        return g->getNumStops();
    
    return 0;
}

VGradientStop* ne_grd_getStop(VGradientBase* g, const unsigned int& index) {
    if (g) {
        if (index >= 0 && index < int(g->getNumStops()))
            return g->getStops().at(index);
    }
    
    return NULL;
}

bool ne_grd_isLinearGradient(VGradientBase* g) {
    if (g) {
        return g->isSetAsLinearGradient();
    }
    
    return false;
}

bool ne_grd_isRadialGradient(VGradientBase* g) {
    if (g) {
        return g->isSetAsRadialGradient();
    }
    
    return false;
}

// VLinearGradient

RAVector* ne_grd_getX1(VGradientBase* g) {
    RAVector* rAVector = NULL;
    if (g && g->isSetAsLinearGradient())
        rAVector = new RAVector(((VLinearGradient*)g)->getX1());
    
    return rAVector;
}

RAVector* ne_grd_getY1(VGradientBase* g) {
    RAVector* rAVector = NULL;
    if (g && g->isSetAsLinearGradient())
        rAVector = new RAVector(((VLinearGradient*)g)->getY1());
    
    return rAVector;
}

RAVector* ne_grd_getX2(VGradientBase* g) {
    RAVector* rAVector = NULL;
    if (g && g->isSetAsLinearGradient())
        rAVector = new RAVector(((VLinearGradient*)g)->getX2());
    
    return rAVector;
}

RAVector* ne_grd_getY2(VGradientBase* g) {
    RAVector* rAVector = NULL;
    if (g && g->isSetAsLinearGradient())
        rAVector = new RAVector(((VLinearGradient*)g)->getY2());
    
    return rAVector;
}

// VRadialGradient

RAVector* ne_grd_getCx(VGradientBase* g) {
    RAVector* rAVector = NULL;
    if (g && g->isSetAsRadialGradient())
        rAVector = new RAVector(((VRadialGradient*)g)->getCx());
    
    return rAVector;
}

RAVector* ne_grd_getCy(VGradientBase* g) {
    RAVector* rAVector = NULL;
    if (g && g->isSetAsRadialGradient())
        rAVector = new RAVector(((VRadialGradient*)g)->getCy());
    
    return rAVector;
}

RAVector* ne_grd_getFx(VGradientBase* g) {
    RAVector* rAVector = NULL;
    if (g && g->isSetAsRadialGradient())
        rAVector = new RAVector(((VRadialGradient*)g)->getFx());
    
    return rAVector;
}

RAVector* ne_grd_getFy(VGradientBase* g) {
    RAVector* rAVector = NULL;
    if (g && g->isSetAsRadialGradient())
        rAVector = new RAVector(((VRadialGradient*)g)->getFy());
    
    return rAVector;
}

RAVector* ne_grd_getR(VGradientBase* g) {
    RAVector* rAVector = NULL;
    if (g && g->isSetAsRadialGradient())
        rAVector = new RAVector(((VRadialGradient*)g)->getR());
    
    return rAVector;
}

// Gradient Stop

bool ne_gstp_isSetOffset(VGradientStop* s) {
    if (s)
        return s->isSetOffset();
    
    return false;
}

RAVector* ne_gstp_getOffset(VGradientStop* s) {
    RAVector* rAVector = NULL;
    if (ne_gstp_isSetOffset(s))
        rAVector = new RAVector(s->getOffset());
    
    return rAVector;
}

bool ne_gstp_isSetColor(VGradientStop* s) {
    if (s)
        return s->isSetStopColor();
    
    return false;
}

const std::string ne_gstp_getColor(VGradientStop* s) {
    if (s)
        return s->getStopColor();
    
    return std::string();
}

// Line Ending

int ne_le_setLineEndingValues(Veneer* ven, VLineEnding* lE, RxnRoleType role) {
    if (ven && lE) {
        // enable the rotation of the line ending
        lE->setEnableRotationMapping(true);
        
        // create a new bounding box
        LBox* bBox = new LBox();
        
        // create a new group
        VRenderGroup* group = new VRenderGroup();
        
        // set the default stroke color
        ne_grp_setStrokeColor(group, colorStringToStr(CLR_STR_BLACK));
        
        // set the default stroke width
        ne_grp_setStrokeWidth(group, 2.0);
        
        // set the default fill color
        ne_grp_setFillColor(group, colorStringToStr(CLR_STR_WHITE));
        
        // product line ending
        if (rxnRoleToString(role) == "product") {
            // set id
            if (!ven->findLineEndingById("productHead"))
                lE->setId("productHead");
            
            // set position
            ne_bb_setX(bBox, -12.0);
            ne_bb_setY(bBox, -6.0);
            
            // set dimensions
            ne_bb_setWidth(bBox, 12.0);
            ne_bb_setHeight(bBox, 12.0);
            
            // set fill color
            ne_grp_setFillColor(group, colorStringToStr(CLR_STR_BLACK));
            
            // add a triangle as a geometric shape to group
            VTransformation2D* gs = ne_grp_addGeometricShape(group, GRP_ELT_SHPE_PLG, true, 3);
        }
        // side product line ending
        else if (rxnRoleToString(role) == "side product") {
            // set id
            if (!ven->findLineEndingById("sideProductHead"))
                lE->setId("sideProductHead");
            
            // set position
            ne_bb_setX(bBox, -12.0);
            ne_bb_setY(bBox, -6.0);
            
            // set dimensions
            ne_bb_setWidth(bBox, 12.0);
            ne_bb_setHeight(bBox, 12.0);
            
            // set fill color
            ne_grp_setFillColor(group, colorStringToStr(CLR_STR_WHITE));
            
            // add a triangle as a geometric shape to group
            VTransformation2D* gs = ne_grp_addGeometricShape(group, GRP_ELT_SHPE_PLG, true, 3);
        }
        // modifier line ending
        else if (rxnRoleToString(role) == "modifier") {
            // set id
            if (!ven->findLineEndingById("modifierHead"))
                lE->setId("modifierHead");
            
            // set position
            ne_bb_setX(bBox, -12.0);
            ne_bb_setY(bBox, -6.0);
            
            // set dimensions
            ne_bb_setWidth(bBox, 12.0);
            ne_bb_setHeight(bBox, 12.0);
            
            // set fill color
            ne_grp_setFillColor(group, colorStringToStr(CLR_STR_WHITE));
            
            // add a diamond as a geometric shape to group
            VTransformation2D* gs = ne_grp_addGeometricShape(group, GRP_ELT_SHPE_PLG, true, 4);
        }
        // acitivator line ending
        else if (rxnRoleToString(role) == "activator") {
            // set id
            if (!ven->findLineEndingById("activatorHead"))
                lE->setId("activatorHead");
            
            // set position
            ne_bb_setX(bBox, -12.0);
            ne_bb_setY(bBox, -6.0);
            
            // set dimensions
            ne_bb_setWidth(bBox, 12.0);
            ne_bb_setHeight(bBox, 12.0);
            
            // set fill color
            ne_grp_setFillColor(group, colorStringToStr(CLR_STR_WHITE));
            
            // add an ellipse as a geometric shape to group
            VTransformation2D* gs = ne_grp_addGeometricShape(group, GRP_ELT_SHPE_ELP, true);
        }
        // inhibitor line ending
        else if (rxnRoleToString(role) == "inhibitor") {
            // set id
            if (!ven->findLineEndingById("inhibitorHead"))
                lE->setId("inhibitorHead");
            
            // set position
            ne_bb_setX(bBox, -2.0);
            ne_bb_setY(bBox, -6.0);
            
            // set dimensions
            ne_bb_setWidth(bBox, 2.0);
            ne_bb_setHeight(bBox, 12.0);
            
            // set fill color
            ne_grp_setFillColor(group, colorStringToStr(CLR_STR_BLACK));
            
            // add an ellipse as a geometric shape to group
            VTransformation2D* gs = ne_grp_addGeometricShape(group, GRP_ELT_SHPE_REC, true);
        }
        
        ne_le_setBoundingBox(lE, bBox);
        ne_le_setGroup(lE, group);
        
        return 0;
    }
    
    return -1;
}

bool ne_le_isSetBoundingBox(VLineEnding* lE) {
    if (lE)
        return lE->isSetBox();
    
    return false;
}

LBox* ne_le_getBoundingBox(VLineEnding* lE) {
    if (lE)
        return lE->getBox();
    
    return NULL;
}

int ne_le_setBoundingBox(VLineEnding* lE, LBox* b) {
    if (lE && b) {
        lE->setBox(b);
        return 0;
    }
    
    return -1;
}

bool ne_le_isSetGroup(VLineEnding* lE) {
    if (lE)
        return lE->isSetGroup();
        
    return false;
}

VRenderGroup* ne_le_getGroup(VLineEnding* lE) {
    if (lE)
        return lE->getGroup();
    
    return NULL;
}

int ne_le_setGroup(VLineEnding* lE, VRenderGroup* g) {
    if (lE && g) {
        lE->setGroup(g);
        return 0;
    }
    
    return -1;
}

bool ne_le_isSetEnableRotation(VLineEnding* lE) {
    if (lE)
        return lE->isSetEnableRotationMapping();
        
    return false;
}

bool ne_le_getEnableRotation(VLineEnding* lE) {
    if (lE)
        return lE->getEnableRotationMapping();
    
    return true;
}

int ne_le_enableRotation(VLineEnding* lE, const bool& enable) {
    if (lE) {
        lE->setEnableRotationMapping(enable);
        return 0;
    }
    
    return -1;
}

// Style

int ne_stl_setStyleValues(VGlobalStyle* s, StyleType type, RxnRoleType role) {
    if (s) {
        RAVector* rAV = new RAVector();
        
        // create a new group
        VRenderGroup* group = new VRenderGroup();
        
        // set the default stroke color
        ne_grp_setStrokeColor(group, colorStringToStr(CLR_STR_BLACK));
        
        // set the default stroke width
        ne_grp_setStrokeWidth(group, 2.0);
        
        // set the default fill color
        ne_grp_setFillColor(group, colorStringToStr(CLR_STR_WHITE));
        
        // set the default font size
        rAV->setA(24.);
        ne_grp_setFontSize(group, rAV);
        
        // set the default font family
        ne_grp_setFontFamily(group, fontFamilyToStr(FNT_FMLY_MNO_SPC));
        
        // set the default font weight
        ne_grp_setFontWeight(group, fontWeightToStr(FNT_WGHT_NRM));
        
        // set the default font style
        ne_grp_setFontStyle(group, fontStyleToStr(FNT_STL_NRM));
        
        // set the default horizontal text anchor
        ne_grp_setHTextAnchor(group, hTextAnchorToStr(HTXT_ANCH_MDL));
        
        // set the default vertical text anchor
        ne_grp_setVTextAnchor(group, vTextAnchorToStr(VTXT_ANCH_MDL));
        
        if (type < NUM_StyleType) {
            // set the default style for compartments
            if (styleTypeToStr(type) == "COMPARTMENTGLYPH") {
                // set stroke color
                ne_grp_setStrokeColor(group, colorStringToStr(CLR_STR_DARK_CYAN));
                
                // set stroke width
                ne_grp_setStrokeWidth(group, 6.0);
                
                // set fill color
                ne_grp_setFillColor(group, colorStringToStr(CLR_STR_GRAY));
                
                // add a rectangle as a graphical shape to group
                VTransformation2D* gS = ne_grp_addGeometricShape(group, GRP_ELT_SHPE_REC, false);
                
                // set the radii of corner curvatures of rectangle
                rAV->setA(0.);
                rAV->setR(5.);
                ne_rec_setCornerCurvatureRX(gS, rAV);
                ne_rec_setCornerCurvatureRY(gS, rAV);
            }
            // set the default style for species
            else if (styleTypeToStr(type) == "SPECIESGLYPH") {
                // add a rectangle as a graphical shape to group
                ne_grp_addGeometricShape(group, GRP_ELT_SHPE_REC, false);
            }
            // set the default style for reactions
            else if (styleTypeToStr(type) == "REACTIONGLYPH") {
                // add a circle as a graphical shape to group
                VTransformation2D* gS = ne_grp_addGeometricShape(group, GRP_ELT_SHPE_ELP, false);
            }
            // set the default style for texts
            else if (styleTypeToStr(type) == "TEXTGLYPH") {
                // set font weight
                ne_grp_setFontWeight(group, fontWeightToStr(FNT_WGHT_NRM));
                
                // set font style
                ne_grp_setFontStyle(group, fontStyleToStr(FNT_STL_NRM));
            }
            
            // add the type to the type list of style
            ne_stl_addToTypeList(s, styleTypeToStr(type));
        }
        else if (role < NUM_RxnRoleType) {
            // set the style for substrates
            if (rxnRoleToString(role) == "substrate") {
                // to be added if required
            }
            // set the style for side substrates
            else if (rxnRoleToString(role) == "side substrate") {
                // to be added if required
            }
            // set the style for products
            else if (rxnRoleToString(role) == "product") {
                // set the start head
                ne_grp_setEndHead(group, "productHead");
            }
            // set the style for side products
            else if (rxnRoleToString(role) == "side product") {
                // set the start head
                ne_grp_setEndHead(group, "sideProductHead");
            }
            // set the style for modifiers
            else if (rxnRoleToString(role) == "modifier") {
                // set the start head
                ne_grp_setEndHead(group, "modifierHead");
            }
            // set the style for activators
            else if (rxnRoleToString(role) == "activator") {
                // set the start head
                ne_grp_setEndHead(group, "activatorHead");
            }
            // set the style for inhibitors
            else if (rxnRoleToString(role) == "inhibitor") {
                // set the start head
                ne_grp_setEndHead(group, "inhibitorHead");
            }
            
            // add the role to the role list of style
            ne_stl_addToRoleList(s, rxnRoleToString(role));
        }
        
        // set group to style
        ne_stl_setGroup(s, group);
        
        return 0;
    }
    
    return -1;
}

bool ne_stl_isSetGroup(VGlobalStyle* s) {
    if (s)
        return s->isSetGroup();
        
    return false;
}

VRenderGroup* ne_stl_getGroup(VGlobalStyle* s) {
    if (s)
        return s->getGroup();
    
    return NULL;
}

int ne_stl_setGroup(VGlobalStyle* s, VRenderGroup* g) {
    if (s && g) {
        s->setGroup(g);
        return 0;
    }
    
    return -1;
}

int ne_stl_addToTypeList(VGlobalStyle* s, const std::string& type) {
    // add the type to the type list of style
    if (s) {
        s->addToTypeList(type);
        return 0;
    }
    
    return -1;
}

int ne_stl_addToRoleList(VGlobalStyle* s, const std::string& role) {
    // add the role to the role list of style
    if (s) {
        s->addToRoleList(role);
        return 0;
    }
    
    return -1;
}

int ne_stl_addToIdList(VGlobalStyle* s, const std::string& gOId) {
    // add the role to the role list of style
    if (s && s->isLocalStyle()) {
        ((VLocalStyle*)s)->addToIdList(gOId);
        return 0;
    }
    
    return -1;
}

// Render Group

bool ne_grp_isSetStrokeColor(VRenderGroup* g) {
    if (g)
        return g->isSetStroke();
    
    return false;
}

const std::string ne_grp_getStrokeColor(VRenderGroup* g) {
    if (g)
        return g->getStroke();
    
    return std::string();
}

int ne_grp_setStrokeColor(VRenderGroup* g, const std::string& color) {
    if (g) {
        g->setStroke(color);
        return 0;
    }
    
    return -1;
}

bool ne_grp_isSetStrokeWidth(VRenderGroup* g) {
    if (g)
        return g->isSetStrokeWidth();
    
    return false;
}

const double ne_grp_getStrokeWidth(VRenderGroup* g) {
    if (g)
        return g->getStrokeWidth();
    
    return 0.0;
}

int ne_grp_setStrokeWidth(VRenderGroup* g, const double& width) {
    if (g) {
        g->setStrokeWidth(width);
        return 0;
    }
    
    return -1;
}

bool ne_grp_isSetStrokeDashArray(VRenderGroup* g) {
    if (g) {
        if (g->getDashArray().size())
            return true;
    }
    
    return false;
}

const size_t ne_grp_getNumStrokeDashes(VRenderGroup* g) {
    if (g)
        return g->getDashArray().size();
    
    return 0;
}

const int ne_grp_getStrokeDash(VRenderGroup* g, const unsigned int& index) {
    if (g) {
        if (index >= 0 && index < int(g->getDashArray().size()))
            return g->getDashArray().at(index);
    }
    
    return -1;
}

VGraphicalPrimitive1D::dashArrayVec* ne_grp_getStrokeDashArray(VRenderGroup* g) {
    VGraphicalPrimitive1D::dashArrayVec* dashArray = NULL;
    if (g) {
        dashArray = new VRenderGroup::dashArrayVec();
        for (int i = 0; i < g->getDashArray().size(); ++i)
            dashArray->push_back(g->getDashArray().at(i));
    }
    
    return dashArray;
}

int ne_grp_setStrokeDashArray(VRenderGroup* g, VGraphicalPrimitive1D::dashArrayVec* dashArray) {
    if (g && dashArray) {
        VGraphicalPrimitive1D::dashArrayVec newDashArray;
        for (int i = 0; i < dashArray->size(); ++i)
            newDashArray.push_back(dashArray->at(i));
        
        g->setDashArray(newDashArray);
        return 0;
    }
    
    return -1;
}

bool ne_grp_isSetFillColor(VRenderGroup* g) {
    if (g)
        return g->isSetFill();
    
    return false;
}

const std::string ne_grp_getFillColor(VRenderGroup* g) {
    if (g)
        return g->getFill();
    
    return std::string();
}

int ne_grp_setFillColor(VRenderGroup* g, const std::string& color) {
    if (g) {
        g->setFill(color);
        return 0;
    }
    
    return -1;
}

bool ne_grp_isSetFillRule(VRenderGroup* g) {
    if (g)
        return g->isSetFillRule();
    
    return false;
}

const std::string ne_grp_getFillRule(VRenderGroup* g) {
    if (g)
        return g->getFillRule();
    
    return std::string();
}

int ne_grp_setFillRule(VRenderGroup* g, const std::string& fillrule) {
    if (g) {
        g->setFillRule(fillrule);
        return 0;
    }
    
    return -1;
}

bool ne_grp_isSetFontFamily(VRenderGroup* g) {
    if (g)
        return g->isSetFontFamily();
    
    return false;
}

const std::string ne_grp_getFontFamily(VRenderGroup* g) {
    if (g)
        return g->getFontFamily();
    
    return std::string();
}

int ne_grp_setFontFamily(VRenderGroup* g, const std::string& fontfamily) {
    if (g) {
        g->setFontFamily(fontfamily);
        return 0;
    }
    
    return -1;
}

bool ne_grp_isSetFontSize(VRenderGroup* g) {
    if (g)
        return g->isSetFontSize();
    
    return false;
}

RAVector* ne_grp_getFontSize(VRenderGroup* g) {
    RAVector* rAVector = NULL;
    if (ne_grp_isSetFontSize(g))
        rAVector = new RAVector(g->getFontSize());
    
    return rAVector;
}

int ne_grp_setFontSize(VRenderGroup* g, RAVector* fontsize) {
    if (g && fontsize) {
        g->setFontSize(RAVector(fontsize->a(), fontsize->r()));
        return 0;
    }
    
    return -1;
}

bool ne_grp_isSetFontStyle(VRenderGroup* g) {
    if (g)
        return g->isSetFontStyle();
    
    return false;
}

const std::string ne_grp_getFontStyle(VRenderGroup* g) {
    if (g)
        return g->getFontStyle();
    
    return std::string();
}

int ne_grp_setFontStyle(VRenderGroup* g, const std::string& fontstyle) {
    if (g) {
        g->setFontStyle(fontstyle);
        return 0;
    }
    
    return -1;
}

bool ne_grp_isSetFontWeight(VRenderGroup* g) {
    if (g)
        return g->isSetFontWeight();
    
    return false;
}

const std::string ne_grp_getFontWeight(VRenderGroup* g) {
    if (g)
        return g->getFontWeight();
    
    return std::string();
}

int ne_grp_setFontWeight(VRenderGroup* g, const std::string& fontweight) {
    if (g) {
        g->setFontWeight(fontweight);
        return 0;
    }
    
    return -1;
}

bool ne_grp_isSetHTextAnchor(VRenderGroup* g) {
    if (g)
        return g->isSetHTextAnchor();
    
    return false;
}

const std::string ne_grp_getHTextAnchor(VRenderGroup* g) {
    if (g)
        return g->getHTextAnchor();
    
    return std::string();
}

int ne_grp_setHTextAnchor(VRenderGroup* g, const std::string& anchor) {
    if (g) {
        g->setHTextAnchor(anchor);
        return 0;
    }
    
    return -1;
}

bool ne_grp_isSetVTextAnchor(VRenderGroup* g) {
    if (g)
        return g->isSetVTextAnchor();
    
    return false;
}

const std::string ne_grp_getVTextAnchor(VRenderGroup* g) {
    if (g)
        return g->getVTextAnchor();
    
    return std::string();
}

int ne_grp_setVTextAnchor(VRenderGroup* g, const std::string& anchor) {
    if (g) {
        g->setVTextAnchor(anchor);
        return 0;
    }
    
    return -1;
}

bool ne_grp_isSetStartHead(VRenderGroup* g) {
    if (g)
        return g->isSetStartHead();
    
    return false;
}

const std::string ne_grp_getStartHead(VRenderGroup* g) {
    if (g)
        return g->getStartHead();
    
    return std::string();
}

int ne_grp_setStartHead(VRenderGroup* g, const std::string& head) {
    if (g) {
        g->setStartHead(head);
        return 0;
    }
    
    return -1;
}

bool ne_grp_isSetEndHead(VRenderGroup* g) {
    if (g)
        return g->isSetEndHead();
    
    return false;
}

const std::string ne_grp_getEndHead(VRenderGroup* g) {
    if (g)
        return g->getEndHead();
    
    return std::string();
}

int ne_grp_setEndHead(VRenderGroup* g, const std::string& head) {
    if (g) {
        g->setEndHead(head);
        return 0;
    }
    
    return -1;
}

const size_t ne_grp_getNumGeometricShapes(VRenderGroup* g) {
    if (g)
        return g->getNumElements();
    
    return 0;
}

VTransformation2D* ne_grp_getGeometricShape(VRenderGroup* g, const unsigned int& index) {
    if (g) {
        if (index >= 0 && index < int(g->getNumElements()))
            return g->getListOfElements().at(index);
    }
    
    return NULL;
}

int ne_grp_removeGeometricShapes(VRenderGroup* g) {
    if (g) {
        g->clearListOfElements();
        return 0;
    }
    
    return -1;
}

VTransformation2D* ne_grp_addGeometricShape(VRenderGroup* g, const RenderGroupElementShape rGEShape, const bool& lineending, const int& numvertices, const std::string& href) {
    if (g) {
        // add a geometric shape to group
        VTransformation2D* gS = NULL;
        
        switch (rGEShape) {
            // image
            case 0:
                gS = new VImage();
                break;
                
            // rendercurve
            case 1:
                gS = new RCurve();
                break;
                
            // text
            case 2:
                gS = new VText();
                break;
                
            // rectangle
            case 3:
                gS = new VRectangle();
                break;
                
            // ellipse
            case 4:
                gS = new VEllipse();
                break;
                
            // polygon
            case 5:
                gS = new VPolygon();
                break;
                
            default:
                break;
        }

        // set the id the geometric shape
        gS->setId(g->getElementUniqueId());
        
        // set geometric shape features
        ne_gs_setShapeFeatures(gS, numvertices, href, lineending);

        // add the geometric shape to the list of elements
        g->addToListOfElements(gS);
        
        return gS;
    }
    
    return NULL;
}

// VTransformation2D

const RenderGroupElementShape ne_gs_getShape(VTransformation2D* gS) {
    if (gS)
        return gS->getShape();
    
    return NUM_RenderGroupElementShape;
}

int ne_gs_setShapeFeatures(VTransformation2D* gS, const int& numvertices, const std::string& href, const bool& lineending) {
    if (gS) {
        RAVector* rAV = new RAVector();
        switch (gS->getShape()) {
                
            // image
            case 0:
                // set the positon of image
                rAV->setR(0.);
                ne_img_setPositionX(gS, rAV);
                rAV->setR(0.);
                ne_img_setPositionY(gS, rAV);
                
                // set the dimension of image
                rAV->setR(100.);
                ne_img_setDimensionWidth(gS , rAV);
                rAV->setR(100.);
                ne_img_setDimensionHeight(gS , rAV);
                
                // set the href of image
                if (!href.empty())
                ne_img_setHref(gS, href);
                
                return 0;
                
            // rendercurve
            case 1:
                if (!lineending) {
                    RenPoint* vertex = NULL;
                    RPoint* point = NULL;
                    
                    // add a vertex to render curve
                    vertex = ne_rc_addVertex(gS);
                    
                    // set the values of vertex
                    point = new RPoint();
                    rAV->setR(0.);
                    ne_rp_setX(point, rAV);
                    rAV->setR(50.);
                    ne_rp_setY(point, rAV);
                    ne_vrx_setRenderPoint(vertex, point);
                    
                    // add a vertex to render curve
                    vertex = ne_rc_addVertex(gS);
                    
                    // set the values of vertex
                    point = new RPoint();
                    rAV->setR(100.);
                    ne_rp_setX(point, rAV);
                    rAV->setR(50.);
                    ne_rp_setY(point, rAV);
                    ne_vrx_setRenderPoint(vertex, point);
                    
                    return 0;
                }
                
                break;
                
            // rectangle
            case 3:
                // set the positon of rectangle
                rAV->setR(0.);
                ne_rec_setPositionX(gS, rAV);
                rAV->setR(0.);
                ne_rec_setPositionY(gS, rAV);
                
                // set the dimension of rectangle
                rAV->setR(100.);
                ne_rec_setDimensionWidth(gS , rAV);
                rAV->setR(100.);
                ne_rec_setDimensionHeight(gS , rAV);
                
                // set the radii of corner curvatures of rectangle
                rAV->setR(10.);
                ne_rec_setCornerCurvatureRX(gS , rAV);
                rAV->setR(10.);
                ne_rec_setCornerCurvatureRY(gS , rAV);
                
                return 0;
                
            // ellipse
            case 4:
                // set the positon of ellipse
                rAV->setR(50.);
                ne_elp_setPositionCX(gS, rAV);
                rAV->setR(50.);
                ne_elp_setPositionCY(gS, rAV);
                    
                // set the dimension of ellipse
                rAV->setR(50.);
                ne_elp_setDimensionRX(gS, rAV);
                rAV->setR(50.);
                ne_elp_setDimensionRY(gS, rAV);
                    
                return 0;
                
            // polygon
            case 5: {
                RenPoint* vertex = NULL;
                RPoint* point = NULL;
                
                switch (numvertices) {
                    // triangle
                    case 3:
                        if (!lineending) {
                            // add a vertex to polygon
                            vertex = ne_plg_addVertex(gS);
                            
                            // set the values of vertex
                            point = new RPoint();
                            rAV->setR(0.);
                            ne_rp_setX(point, rAV);
                            rAV->setR(0.);
                            ne_rp_setY(point, rAV);
                            ne_vrx_setRenderPoint(vertex, point);
                            
                            // add a vertex to polygon
                            vertex = ne_plg_addVertex(gS);
                            
                            // set the values of vertex
                            point = new RPoint();
                            rAV->setR(100.);
                            ne_rp_setX(point, rAV);
                            rAV->setR(0.);
                            ne_rp_setY(point, rAV);
                            ne_vrx_setRenderPoint(vertex, point);
                            
                            // add a vertex to polygon
                            vertex = ne_plg_addVertex(gS);
                            
                            // set the values of vertex
                            point = new RPoint();
                            rAV->setR(50.);
                            ne_rp_setX(point, rAV);
                            rAV->setR(100.);
                            ne_rp_setY(point, rAV);
                            ne_vrx_setRenderPoint(vertex, point);
                        }
                        else {
                            // add a vertex to polygon
                            vertex = ne_plg_addVertex(gS);
                            
                            // set the values of vertex
                            point = new RPoint();
                            rAV->setR(0.);
                            ne_rp_setX(point, rAV);
                            rAV->setR(0.);
                            ne_rp_setY(point, rAV);
                            ne_vrx_setRenderPoint(vertex, point);
                            
                            // add a vertex to polygon
                            vertex = ne_plg_addVertex(gS);
                            
                            // set the values of vertex
                            point = new RPoint();
                            rAV->setR(100.);
                            ne_rp_setX(point, rAV);
                            rAV->setR(50.);
                            ne_rp_setY(point, rAV);
                            ne_vrx_setRenderPoint(vertex, point);
                            
                            // add a vertex to polygon
                            vertex = ne_plg_addVertex(gS);
                            
                            // set the values of vertex
                            point = new RPoint();
                            rAV->setR(0.);
                            ne_rp_setX(point, rAV);
                            rAV->setR(100.);
                            ne_rp_setY(point, rAV);
                            ne_vrx_setRenderPoint(vertex, point);
                        }
                        
                        return 0;
                       
                    // diamond
                    case 4:
                        // add a vertex to polygon
                        vertex = ne_plg_addVertex(gS);
                        
                        // set the values of vertex
                        point = new RPoint();
                        rAV->setR(0.);
                        ne_rp_setX(point, rAV);
                        rAV->setR(50.);
                        ne_rp_setY(point, rAV);
                        ne_vrx_setRenderPoint(vertex, point);
                        
                        // add a vertex to polygon
                        vertex = ne_plg_addVertex(gS);
                        
                        // set the values of vertex
                        point = new RPoint();
                        rAV->setR(50.);
                        ne_rp_setX(point, rAV);
                        rAV->setR(0.);
                        ne_rp_setY(point, rAV);
                        ne_vrx_setRenderPoint(vertex, point);
                        
                        // add a vertex to polygon
                        vertex = ne_plg_addVertex(gS);
                        
                        // set the values of vertex
                        point = new RPoint();
                        rAV->setR(100.);
                        ne_rp_setX(point, rAV);
                        rAV->setR(50.);
                        ne_rp_setY(point, rAV);
                        ne_vrx_setRenderPoint(vertex, point);
                        
                        // add a vertex to polygon
                        vertex = ne_plg_addVertex(gS);
                        
                        // set the values of vertex
                        point = new RPoint();
                        rAV->setR(50.);
                        ne_rp_setX(point, rAV);
                        rAV->setR(100.);
                        ne_rp_setY(point, rAV);
                        ne_vrx_setRenderPoint(vertex, point);
                        
                        return 0;
                        
                    // pentagon
                    case 5:
                        if (!lineending) {
                            // add a vertex to polygon
                            vertex = ne_plg_addVertex(gS);
                            
                            // set the values of vertex
                            point = new RPoint();
                            rAV->setR(50. * (1 - std::sin(0.4 * pi)));
                            ne_rp_setX(point, rAV);
                            rAV->setR(50. * (1 - std::cos(0.4 * pi)));
                            ne_rp_setY(point, rAV);
                            ne_vrx_setRenderPoint(vertex, point);
                            
                            // add a vertex to polygon
                            vertex = ne_plg_addVertex(gS);
                            
                            // set the values of vertex
                            point = new RPoint();
                            rAV->setR(50.);
                            ne_rp_setX(point, rAV);
                            rAV->setR(0.);
                            ne_rp_setY(point, rAV);
                            ne_vrx_setRenderPoint(vertex, point);
                            
                            // add a vertex to polygon
                            vertex = ne_plg_addVertex(gS);
                            
                            // set the values of vertex
                            point = new RPoint();
                            rAV->setR(50. * (1 + std::sin(0.4 * pi)));
                            ne_rp_setX(point, rAV);
                            rAV->setR(50. * (1 - std::cos(0.4 * pi)));
                            ne_rp_setY(point, rAV);
                            ne_vrx_setRenderPoint(vertex, point);
                            
                            // add a vertex to polygon
                            vertex = ne_plg_addVertex(gS);
                            
                            // set the values of vertex
                            point = new RPoint();
                            rAV->setR(50. * (1 + std::sin(0.2 * pi)));
                            ne_rp_setX(point, rAV);
                            rAV->setR(50. * (1 + std::cos(0.2 * pi)));
                            ne_rp_setY(point, rAV);
                            ne_vrx_setRenderPoint(vertex, point);
                            
                            // add a vertex to polygon
                            vertex = ne_plg_addVertex(gS);
                            
                            // set the values of vertex
                            point = new RPoint();
                            rAV->setR(50. * (1 - std::sin(0.2 * pi)));
                            ne_rp_setX(point, rAV);
                            rAV->setR(50. * (1 + std::cos(0.2 * pi)));
                            ne_rp_setY(point, rAV);
                            ne_vrx_setRenderPoint(vertex, point);
                        }
                        else {
                            // add a vertex to polygon
                            vertex = ne_plg_addVertex(gS);
                            
                            // set the values of vertex
                            point = new RPoint();
                            rAV->setR(50. * (1 - std::cos(0.2 * pi)));
                            ne_rp_setX(point, rAV);
                            rAV->setR(50. * (1 - std::sin(0.2 * pi)));
                            ne_rp_setY(point, rAV);
                            ne_vrx_setRenderPoint(vertex, point);

                            // add a vertex to polygon
                            vertex = ne_plg_addVertex(gS);
                            
                            // set the values of vertex
                            point = new RPoint();
                            rAV->setR(50. * (2 - std::cos(0.4 * pi)));
                            ne_rp_setX(point, rAV);
                            rAV->setR(50. * (1 - std::sin(0.2 * pi)));
                            ne_rp_setY(point, rAV);
                            ne_vrx_setRenderPoint(vertex, point);
                            
                            // add a vertex to polygon
                            vertex = ne_plg_addVertex(gS);
                            
                            // set the values of vertex
                            point = new RPoint();
                            rAV->setR(100.);
                            ne_rp_setX(point, rAV);
                            rAV->setR(50.);
                            ne_rp_setY(point, rAV);
                            ne_vrx_setRenderPoint(vertex, point);
                            
                            // add a vertex to polygon
                            vertex = ne_plg_addVertex(gS);
                            
                            // set the values of vertex
                            point = new RPoint();
                            rAV->setR(50. * (2 - std::cos(0.4 * pi)));
                            ne_rp_setX(point, rAV);
                            rAV->setR(50. * (1 + std::sin(0.2 * pi)));
                            ne_rp_setY(point, rAV);
                            ne_vrx_setRenderPoint(vertex, point);
                            
                            // add a vertex to polygon
                            vertex = ne_plg_addVertex(gS);
                            
                            // set the values of vertex
                            point = new RPoint();
                            rAV->setR(50. * (1 - std::cos(0.2 * pi)));
                            ne_rp_setX(point, rAV);
                            rAV->setR(50. * (1 + std::sin(0.2 * pi)));
                            ne_rp_setY(point, rAV);
                            ne_vrx_setRenderPoint(vertex, point);
                        }
                        
                        return 0;
                        
                    // hexagon
                    case 6:
                        // add a vertex to polygon
                        vertex = ne_plg_addVertex(gS);
                        
                        // set the values of vertex
                        point = new RPoint();
                        rAV->setR(0.);
                        ne_rp_setX(point, rAV);
                        rAV->setR(25.);
                        ne_rp_setY(point, rAV);
                        ne_vrx_setRenderPoint(vertex, point);
                        
                        // add a vertex to polygon
                        vertex = ne_plg_addVertex(gS);
                        
                        // set the values of vertex
                        point = new RPoint();
                        rAV->setR(50.);
                        ne_rp_setX(point, rAV);
                        rAV->setR(0.);
                        ne_rp_setY(point, rAV);
                        ne_vrx_setRenderPoint(vertex, point);
                        
                        // add a vertex to polygon
                        vertex = ne_plg_addVertex(gS);
                        
                        // set the values of vertex
                        point = new RPoint();
                        rAV->setR(100.);
                        ne_rp_setX(point, rAV);
                        rAV->setR(25.);
                        ne_rp_setY(point, rAV);
                        ne_vrx_setRenderPoint(vertex, point);
                        
                        // add a vertex to polygon
                        vertex = ne_plg_addVertex(gS);
                        
                        // set the values of vertex
                        point = new RPoint();
                        rAV->setR(100.);
                        ne_rp_setX(point, rAV);
                        rAV->setR(75.);
                        ne_rp_setY(point, rAV);
                        ne_vrx_setRenderPoint(vertex, point);
                        
                        // add a vertex to polygon
                        vertex = ne_plg_addVertex(gS);
                        
                        // set the values of vertex
                        point = new RPoint();
                        rAV->setR(50.);
                        ne_rp_setX(point, rAV);
                        rAV->setR(100.);
                        ne_rp_setY(point, rAV);
                        ne_vrx_setRenderPoint(vertex, point);
                        
                        // add a vertex to polygon
                        vertex = ne_plg_addVertex(gS);
                        
                        // set the values of vertex
                        point = new RPoint();
                        rAV->setR(0.);
                        ne_rp_setX(point, rAV);
                        rAV->setR(75.);
                        ne_rp_setY(point, rAV);
                        ne_vrx_setRenderPoint(vertex, point);
                        
                        return 0;
                        
                    // octagon
                    case 8:
                        // add a vertex to polygon
                        vertex = ne_plg_addVertex(gS);
                        
                        // set the values of vertex
                        point = new RPoint();
                        rAV->setR(0.);
                        ne_rp_setX(point, rAV);
                        rAV->setR(50.);
                        ne_rp_setY(point, rAV);
                        ne_vrx_setRenderPoint(vertex, point);
                        
                        // add a vertex to polygon
                        vertex = ne_plg_addVertex(gS);
                        
                        // set the values of vertex
                        point = new RPoint();
                        rAV->setR(50. * (1 - std::cos(pi / 4.0)));
                        ne_rp_setX(point, rAV);
                        rAV->setR(50. * (1 - std::sin(pi / 4.0)));
                        ne_rp_setY(point, rAV);
                        ne_vrx_setRenderPoint(vertex, point);
                        
                        // add a vertex to polygon
                        vertex = ne_plg_addVertex(gS);
                        
                        // set the values of vertex
                        point = new RPoint();
                        rAV->setR(50.);
                        ne_rp_setX(point, rAV);
                        rAV->setR(0.);
                        ne_rp_setY(point, rAV);
                        ne_vrx_setRenderPoint(vertex, point);
                        
                        // add a vertex to polygon
                        vertex = ne_plg_addVertex(gS);
                        
                        // set the values of vertex
                        point = new RPoint();
                        rAV->setR(50. * (1 + std::cos(pi / 4.0)));
                        ne_rp_setX(point, rAV);
                        rAV->setR(50. * (1 - std::sin(pi / 4.0)));
                        ne_rp_setY(point, rAV);
                        ne_vrx_setRenderPoint(vertex, point);
                        
                        // add a vertex to polygon
                        vertex = ne_plg_addVertex(gS);
                        
                        // set the values of vertex
                        point = new RPoint();
                        rAV->setR(100.);
                        ne_rp_setX(point, rAV);
                        rAV->setR(50.);
                        ne_rp_setY(point, rAV);
                        ne_vrx_setRenderPoint(vertex, point);
                        
                        // add a vertex to polygon
                        vertex = ne_plg_addVertex(gS);
                        
                        // set the values of vertex
                        point = new RPoint();
                        rAV->setR(50. * (1 + std::cos(pi / 4.0)));
                        ne_rp_setX(point, rAV);
                        rAV->setR(50. * (1 + std::sin(pi / 4.0)));
                        ne_rp_setY(point, rAV);
                        ne_vrx_setRenderPoint(vertex, point);
                        
                        // add a vertex to polygon
                        vertex = ne_plg_addVertex(gS);
                        
                        // set the values of vertex
                        point = new RPoint();
                        rAV->setR(50.);
                        ne_rp_setX(point, rAV);
                        rAV->setR(100.);
                        ne_rp_setY(point, rAV);
                        ne_vrx_setRenderPoint(vertex, point);
                        
                        // add a vertex to polygon
                        vertex = ne_plg_addVertex(gS);
                        
                        // set the values of vertex
                        point = new RPoint();
                        rAV->setR(50. * (1 - std::cos(pi / 4.0)));
                        ne_rp_setX(point, rAV);
                        rAV->setR(50. * (1 + std::sin(pi / 4.0)));
                        ne_rp_setY(point, rAV);
                        ne_vrx_setRenderPoint(vertex, point);
                        
                        return 0;
                        
                    default:
                        break;
                }
            }
                
                break;
                
            default:
                break;
        }
    }
    
    return -1;
}


bool ne_gs_isSetStrokeColor(VTransformation2D* gS) {
    if (gS && gS->getShape() > 0)
        return ((VGraphicalPrimitive1D *)gS)->isSetStroke();
    
    return false;
}

const std::string ne_gs_getStrokeColor(VTransformation2D* gS) {
    if (gS && gS->getShape() > 0)
        return ((VGraphicalPrimitive1D *)gS)->getStroke();
    
    return std::string();
}

int ne_gs_setStrokeColor(VTransformation2D* gS, const std::string& color) {
    if (gS && gS->getShape() > 0) {
        ((VGraphicalPrimitive1D *)gS)->setStroke(color);
        return 0;
    }
    
    return -1;
}

bool ne_gs_isSetStrokeWidth(VTransformation2D* gS) {
    if (gS && gS->getShape() > 0)
        return ((VGraphicalPrimitive1D *)gS)->isSetStrokeWidth();
    
    return false;
}

const double ne_gs_getStrokeWidth(VTransformation2D* gS) {
    if (gS && gS->getShape() > 0)
        return ((VGraphicalPrimitive1D *)gS)->getStrokeWidth();
    
    return 0.0;
}

int ne_gs_setStrokeWidth(VTransformation2D* gS, const double& width) {
    if (gS && gS->getShape() > 0) {
        ((VGraphicalPrimitive1D *)gS)->setStrokeWidth(width);
        return 0;
    }
    
    return -1;
}

bool ne_gs_isSetStrokeDashArray(VTransformation2D* gS) {
    if (gS && gS->getShape() > 0) {
        if (((VGraphicalPrimitive1D *)gS)->getDashArray().size())
            return true;
    }
    
    return false;
}

const size_t ne_gs_getNumStrokeDashes(VTransformation2D* gS) {
    if (gS && gS->getShape() > 0)
        return ((VGraphicalPrimitive1D *)gS)->getDashArray().size();
    
    return 0;
}

const int ne_gs_getStrokeDash(VTransformation2D* gS, const unsigned int& index) {
    if (gS && gS->getShape() > 0) {
        if (index >= 0 && index < int(((VGraphicalPrimitive1D *)gS)->getDashArray().size()))
            return ((VGraphicalPrimitive1D *)gS)->getDashArray().at(index);
    }
    
    return -1;
}

VGraphicalPrimitive1D::dashArrayVec* ne_gs_getStrokeDashArray(VTransformation2D* gS) {
    VGraphicalPrimitive1D::dashArrayVec* dashArray = NULL;
    if (gS && gS->getShape() > 0) {
        dashArray = new VRenderGroup::dashArrayVec();
        for (int i = 0; i < ((VGraphicalPrimitive1D *)gS)->getDashArray().size(); ++i)
            dashArray->push_back(((VGraphicalPrimitive1D *)gS)->getDashArray().at(i));
    }
    
    return dashArray;
}

int ne_gs_setStrokeDashArray(VTransformation2D* gS, VGraphicalPrimitive1D::dashArrayVec* dasharray) {
    if (gS && gS->getShape() > 0 && dasharray) {
        VGraphicalPrimitive1D::dashArrayVec newDashArray;
        for (int i = 0; i < dasharray->size(); ++i)
            newDashArray.push_back(dasharray->at(i));
        
        ((VGraphicalPrimitive1D *)gS)->setDashArray(newDashArray);
        return 0;
    }
    
    return -1;
}

bool ne_gs_isSetFillColor(VTransformation2D* gS) {
    if (gS && gS->getShape() > 2)
        return ((VGraphicalPrimitive2D*)gS)->isSetFill();
    
    return false;
}

const std::string ne_gs_getFillColor(VTransformation2D* gS) {
    if (gS && gS->getShape() > 2)
        return ((VGraphicalPrimitive2D*)gS)->getFill();
    
    return std::string();
}

int ne_gs_setFillColor(VTransformation2D* gS, const std::string& color) {
    if (gS && gS->getShape() > 2) {
        ((VGraphicalPrimitive2D*)gS)->setFill(color);
        return 0;
    }
    
    return -1;
}

bool ne_gs_isSetFillRule(VTransformation2D* gS) {
    if (gS && gS->getShape() == 5)
        return ((VGraphicalPrimitive2D*)gS)->isSetFillRule();
    
    return false;
}

const std::string ne_gs_getFillRule(VTransformation2D* gS) {
    if (gS && gS->getShape() == 5)
        return ((VGraphicalPrimitive2D*)gS)->getFillRule();
    
    return std::string();
}

int ne_gs_setFillRule(VTransformation2D* gS, const std::string& fillrule) {
    if (gS && gS->getShape() == 5) {
        ((VGraphicalPrimitive2D*)gS)->setFillRule(fillrule);
        return 0;
    }
    
    return -1;
}

// VImage

bool ne_img_isSetPositionX(VTransformation2D* gS) {
    if (gS && gS->getShape() == 0)
        return ((VImage *)gS)->isSetX();
    
    return false;
}

RAVector* ne_img_getPositionX(VTransformation2D* gS) {
    RAVector* rAVector = NULL;
    if (ne_img_isSetPositionX(gS))
        rAVector = new RAVector(((VImage *)gS)->getX());
    
    return rAVector;
}

int ne_img_setPositionX(VTransformation2D* gS, RAVector* x) {
    if (gS && gS->getShape() == 0 && x) {
        ((VImage *)gS)->setX(RAVector(x->a(), x->r()));
        return 0;
    }
    
    return -1;
}

bool ne_img_isSetPositionY(VTransformation2D* gS) {
    if (gS && gS->getShape() == 0)
        return ((VImage *)gS)->isSetY();
    
    return false;
}

RAVector* ne_img_getPositionY(VTransformation2D* gS) {
    RAVector* rAVector = NULL;
    if (ne_img_isSetPositionY(gS))
        rAVector = new RAVector(((VImage *)gS)->getY());
    
    return rAVector;
}

int ne_img_setPositionY(VTransformation2D* gS, RAVector* y) {
    if (gS && gS->getShape() == 0 && y) {
        ((VImage *)gS)->setY(RAVector(y->a(), y->r()));
        return 0;
    }
    
    return -1;
}

bool ne_img_isSetDimensionWidth(VTransformation2D* gS) {
    if (gS && gS->getShape() == 0)
        return ((VImage *)gS)->isSetWidth();
    
    return false;
}

RAVector* ne_img_getDimensionWidth(VTransformation2D* gS) {
    RAVector* rAVector = NULL;
    if (ne_img_isSetDimensionWidth(gS))
        rAVector = new RAVector(((VImage *)gS)->getWidth());
    
    return rAVector;
}

int ne_img_setDimensionWidth(VTransformation2D* gS, RAVector* width) {
    if (gS && gS->getShape() == 0 && width) {
        ((VImage *)gS)->setWidth(RAVector(width->a(), width->r()));
        return 0;
    }
    
    return -1;
}

bool ne_img_isSetDimensionHeight(VTransformation2D* gS) {
    if (gS && gS->getShape() == 0)
        return ((VImage *)gS)->isSetHeight();
    
    return false;
}

RAVector* ne_img_getDimensionHeight(VTransformation2D* gS) {
    RAVector* rAVector = NULL;
    if (ne_img_isSetDimensionHeight(gS))
        rAVector = new RAVector(((VImage *)gS)->getHeight());
    
    return rAVector;
}

int ne_img_setDimensionHeight(VTransformation2D* gS, RAVector* height) {
    if (gS && gS->getShape() == 0 && height) {
        ((VImage *)gS)->setHeight(RAVector(height->a(), height->r()));
        return 0;
    }
    
    return -1;
}

bool ne_img_isSetHref(VTransformation2D* gS) {
    if (gS && gS->getShape() == 0)
        return ((VImage *)gS)->isSetHref();
    
    return false;
}

const std::string ne_img_getHref(VTransformation2D* gS) {
    if (gS && gS->getShape() == 0)
        return ((VImage *)gS)->getHref();
    
    return std::string();
}

int ne_img_setHref(VTransformation2D* gS, const std::string& href) {
    if (gS && gS->getShape() == 0) {
        ((VImage*)gS)->setHref(href);
        return 0;
    }
    
    return -1;
}

// RCurve

const size_t ne_rc_getNumVertices(VTransformation2D* gS) {
    if (gS && gS->getShape() == 1)
        return ((RCurve *)gS)->getNumElements();
    
    return 0;
}

RenPoint* ne_rc_getVertex(VTransformation2D* gS, const unsigned int& index) {
    if (gS && gS->getShape() == 1) {
        if (index >= 0 && index < int(((RCurve *)gS)->getNumElements()))
            return ((RCurve *)gS)->getListOfElements().at(index);
    }
    
    return NULL;
}

RenPoint* ne_rc_addVertex(VTransformation2D* gS, RPoint* point, RPoint* basepoint1, RPoint* basepoint2, const int& index) {
    RenPoint* rp = NULL;
    if (gS && gS->getShape() == 1 && index >= -1 && index <= int(((RCurve *)gS)->getNumElements())) {
        if (basepoint1 && basepoint2) {
            // create a new cubic bezier
            rp = new RCubicBezier();
            
            // set base points
            ((RCubicBezier*)rp)->setBasePoint1(RPoint(RAVector(basepoint1->x().a(), basepoint1->x().r()), RAVector(basepoint1->y().a(), basepoint1->y().r())));
            ((RCubicBezier*)rp)->setBasePoint2(RPoint(RAVector(basepoint2->x().a(), basepoint2->x().r()), RAVector(basepoint2->y().a(), basepoint2->y().r())));
        }
        else {
            // create a new render point
            rp = new RenPoint();
        }
        
        // set rpoint value
        if (point)
            rp->setRPoint(RPoint(RAVector(point->x().a(), point->x().r()), RAVector(point->y().a(), point->y().r())));
        
        // add the render point to the list of elements
        if (index == -1)
            ((RCurve *)gS)->addToListOfElementsEnd(rp);
        else
            ((RCurve *)gS)->addToListOfElements(rp, index);
    }
    
    return rp;
}

int ne_rc_removeVertex(VTransformation2D* gS, const unsigned int& index) {
    if (gS && gS->getShape() == 1 && index >= 0 && index < int(((RCurve *)gS)->getNumElements())) {
        
        // remove from list
        ((RCurve *)gS)->removeFromListOfElements(index);
        
        // if the removed element was the first one, and the new first element is a cubic bezier
        if (index == 0 && ((RCurve *)gS)->getNumElements() && ((RCurve *)gS)->getListOfElements().at(0)->isRenderCubicBezier()) {
            RenPoint* rp = new RenPoint();
            if (((RCurve *)gS)->getListOfElements().at(0)->isSetId())
                rp->setId(((RCurve *)gS)->getListOfElements().at(0)->getId());
            if (((RCurve *)gS)->getListOfElements().at(0)->isSetRPoint())
                rp->setRPoint(((RCurve *)gS)->getListOfElements().at(0)->getRPoint());
            ((RCurve *)gS)->addToListOfElements(rp, 0);
            ((RCurve *)gS)->removeFromListOfElements(1);
        }
        
        return 0;
    }
    
    return -1;
}

bool ne_rc_isSetStartHead(VTransformation2D* gS) {
    if (gS && gS->getShape() == 1)
        return ((RCurve *)gS)->isSetStartHead();
    
    return false;
}

const std::string ne_rc_getStartHead(VTransformation2D* gS) {
    if (gS && gS->getShape() == 1)
        return ((RCurve *)gS)->getStartHead();
    
    return std::string();
}

int ne_rc_setStartHead(VTransformation2D* gS, const std::string& head) {
    if (gS && gS->getShape() == 1) {
        ((RCurve *)gS)->setStartHead(head);
        return 0;
    }
    
    return -1;
}

bool ne_rc_isSetEndHead(VTransformation2D* gS) {
    if (gS && gS->getShape() == 1)
        return ((RCurve *)gS)->isSetEndHead();
    
    return false;
}

const std::string ne_rc_getEndHead(VTransformation2D* gS) {
    if (gS && gS->getShape() == 1)
        return ((RCurve *)gS)->getEndHead();
    
    return std::string();
}

int ne_rc_setEndHead(VTransformation2D* gS, const std::string& head) {
    if (gS && gS->getShape() == 1) {
        ((RCurve *)gS)->setEndHead(head);
        return 0;
    }
    
    return -1;
}

// VText

bool ne_txt_isSetPositionX(VTransformation2D* gS) {
    if (gS && gS->getShape() == 2)
        return ((VText *)gS)->isSetX();
    
    return false;
}

RAVector* ne_txt_getPositionX(VTransformation2D* gS) {
    RAVector* rAVector = NULL;
    if (ne_txt_isSetPositionX(gS))
        rAVector = new RAVector(((VText *)gS)->getX());
    
    return rAVector;
}

int ne_txt_setPositionX(VTransformation2D* gS, RAVector* x) {
    if (gS && gS->getShape() == 2 && x) {
        ((VText *)gS)->setX(RAVector(x->a(), x->r()));
        return 0;
    }
    
    return -1;
}

bool ne_txt_isSetPositionY(VTransformation2D* gS) {
    if (gS && gS->getShape() == 2)
        return ((VText *)gS)->isSetY();
    
    return false;
}

RAVector* ne_txt_getPositionY(VTransformation2D* gS) {
    RAVector* rAVector = NULL;
    if (ne_txt_isSetPositionY(gS))
        rAVector = new RAVector(((VText *)gS)->getY());
    
    return rAVector;
}

int ne_txt_setPositionY(VTransformation2D* gS, RAVector* y) {
    if (gS && gS->getShape() == 2 && y) {
        ((VText *)gS)->setY(RAVector(y->a(), y->r()));
        return 0;
    }
    
    return -1;
}

bool ne_txt_isSetFontFamily(VTransformation2D* gS) {
    if (gS && gS->getShape() == 2)
        return ((VText *)gS)->isSetFontFamily();
    
    return false;
}

const std::string ne_txt_getFontFamily(VTransformation2D* gS) {
    if (gS && gS->getShape() == 2)
        return ((VText *)gS)->getFontFamily();
    
    return std::string();
}

int ne_txt_setFontFamily(VTransformation2D* gS, const std::string& fontfamily) {
    if (gS && gS->getShape() == 2) {
        ((VText *)gS)->setFontFamily(fontfamily);
        return 0;
    }
    
    return -1;
}

bool ne_txt_isSetFontSize(VTransformation2D* gS) {
    if (gS && gS->getShape() == 2)
        return ((VText *)gS)->isSetFontSize();
    
    return false;
}

RAVector* ne_txt_getFontSize(VTransformation2D* gS) {
    RAVector* rAVector = NULL;
    if (ne_txt_isSetFontSize(gS))
        rAVector = new RAVector(((VText *)gS)->getFontSize());
    
    return rAVector;
}

int ne_txt_setFontSize(VTransformation2D* gS, RAVector* fontsize) {
    if (gS && gS->getShape() == 2 && fontsize) {
        ((VText *)gS)->setFontSize(RAVector(fontsize->a(), fontsize->r()));
        return 0;
    }
    
    return -1;
}

bool ne_txt_isSetFontStyle(VTransformation2D* gS) {
    if (gS && gS->getShape() == 2)
        return ((VText *)gS)->isSetFontStyle();
    
    return false;
}

const std::string ne_txt_getFontStyle(VTransformation2D* gS) {
    if (gS && gS->getShape() == 2)
        return ((VText *)gS)->getFontStyle();
    
    return std::string();
}

int ne_txt_setFontStyle(VTransformation2D* gS, const std::string& fontstyle) {
    if (gS && gS->getShape() == 2) {
        ((VText *)gS)->setFontStyle(fontstyle);
        return 0;
    }
    
    return -1;
}

bool ne_txt_isSetFontWeight(VTransformation2D* gS) {
    if (gS && gS->getShape() == 2)
        return ((VText *)gS)->isSetFontWeight();
    
    return false;
}

const std::string ne_txt_getFontWeight(VTransformation2D* gS) {
    if (gS && gS->getShape() == 2)
        return ((VText *)gS)->getFontWeight();
    
    return std::string();
}

int ne_txt_setFontWeight(VTransformation2D* gS, const std::string& fontweight) {
    if (gS && gS->getShape() == 2) {
        ((VText *)gS)->setFontWeight(fontweight);
        return 0;
    }
    
    return -1;
}

bool ne_txt_isSetHTextAnchor(VTransformation2D* gS) {
    if (gS && gS->getShape() == 2)
        return ((VText *)gS)->isSetHTextAnchor();
    
    return false;
}

const std::string ne_txt_getHTextAnchor(VTransformation2D* gS) {
    if (gS && gS->getShape() == 2)
        return ((VText *)gS)->getHTextAnchor();
    
    return std::string();
}

int ne_txt_setHTextAnchor(VTransformation2D* gS, const std::string& anchor) {
    if (gS && gS->getShape() == 2) {
        ((VText *)gS)->setHTextAnchor(anchor);
        return 0;
    }
    
    return -1;
}

bool ne_txt_isSetVTextAnchor(VTransformation2D* gS) {
    if (gS && gS->getShape() == 2)
        return ((VText *)gS)->isSetVTextAnchor();
    
    return false;
}

const std::string ne_txt_getVTextAnchor(VTransformation2D* gS) {
    if (gS && gS->getShape() == 2)
        return ((VText *)gS)->getVTextAnchor();
    
    return std::string();
}

int ne_txt_setVTextAnchor(VTransformation2D* gS, const std::string& anchor) {
    if (gS && gS->getShape() == 2) {
        ((VText *)gS)->setVTextAnchor(anchor);
        return 0;
    }
    
    return -1;
}

// VRectangle

bool ne_rec_isSetPositionX(VTransformation2D* gS) {
    if (gS && gS->getShape() == 3)
        return ((VRectangle *)gS)->isSetX();
    
    return false;
}

RAVector* ne_rec_getPositionX(VTransformation2D* gS) {
    RAVector* rAVector = NULL;
    if (ne_rec_isSetPositionX(gS))
        rAVector = new RAVector(((VRectangle *)gS)->getX());
    
    return rAVector;
}

int ne_rec_setPositionX(VTransformation2D* gS, RAVector* x) {
    if (gS && gS->getShape() == 3 && x) {
        ((VRectangle *)gS)->setX(RAVector(x->a(), x->r()));
        return 0;
    }
    
    return -1;
}

bool ne_rec_isSetPositionY(VTransformation2D* gS) {
    if (gS && gS->getShape() == 3)
        return ((VRectangle *)gS)->isSetY();
    
    return false;
}

RAVector* ne_rec_getPositionY(VTransformation2D* gS) {
    RAVector* rAVector = NULL;
    if (ne_rec_isSetPositionY(gS))
        rAVector = new RAVector(((VRectangle *)gS)->getY());
    
    return rAVector;
}

int ne_rec_setPositionY(VTransformation2D* gS, RAVector* y) {
    if (gS && gS->getShape() == 3 && y) {
        ((VRectangle *)gS)->setY(RAVector(y->a(), y->r()));
        return 0;
    }
    
    return -1;
}

bool ne_rec_isSetDimensionWidth(VTransformation2D* gS) {
    if (gS && gS->getShape() == 3)
        return ((VRectangle *)gS)->isSetWidth();
    
    return false;
}

RAVector* ne_rec_getDimensionWidth(VTransformation2D* gS) {
    RAVector* rAVector = NULL;
    if (ne_rec_isSetDimensionWidth(gS))
        rAVector = new RAVector(((VRectangle *)gS)->getWidth());
    
    return rAVector;
}

int ne_rec_setDimensionWidth(VTransformation2D* gS, RAVector* width) {
    if (gS && gS->getShape() == 3 && width) {
        ((VRectangle *)gS)->setWidth(RAVector(width->a(), width->r()));
        return 0;
    }
    
    return -1;
}

bool ne_rec_isSetDimensionHeight(VTransformation2D* gS) {
    if (gS && gS->getShape() == 3)
        return ((VRectangle *)gS)->isSetHeight();
    
    return false;
}

RAVector* ne_rec_getDimensionHeight(VTransformation2D* gS) {
    RAVector* rAVector = NULL;
    if (ne_rec_isSetDimensionHeight(gS))
        rAVector = new RAVector(((VRectangle *)gS)->getHeight());
    
    return rAVector;
}

int ne_rec_setDimensionHeight(VTransformation2D* gS, RAVector* height) {
    if (gS && gS->getShape() == 3 && height) {
        ((VRectangle *)gS)->setHeight(RAVector(height->a(), height->r()));
        return 0;
    }
    
    return -1;
}

bool ne_rec_isSetCornerCurvatureRX(VTransformation2D* gS) {
    if (gS && gS->getShape() == 3)
        return ((VRectangle *)gS)->isSetRX();
    
    return false;
}

RAVector* ne_rec_getCornerCurvatureRX(VTransformation2D* gS) {
    RAVector* rAVector = NULL;
    if (ne_rec_isSetCornerCurvatureRX(gS))
        rAVector = new RAVector(((VRectangle *)gS)->getRX());
    
    return rAVector;
}

int ne_rec_setCornerCurvatureRX(VTransformation2D* gS, RAVector* rx) {
    if (gS && gS->getShape() == 3 && rx) {
        ((VRectangle *)gS)->setRX(RAVector(rx->a(), rx->r()));
        return 0;
    }
    
    return -1;
}

bool ne_rec_isSetCornerCurvatureRY(VTransformation2D* gS) {
    if (gS && gS->getShape() == 3)
        return ((VRectangle *)gS)->isSetRY();
    
    return false;
}

RAVector* ne_rec_getCornerCurvatureRY(VTransformation2D* gS) {
    RAVector* rAVector = NULL;
    if (ne_rec_isSetCornerCurvatureRY(gS))
        rAVector = new RAVector(((VRectangle *)gS)->getRY());
    
    return rAVector;
}

int ne_rec_setCornerCurvatureRY(VTransformation2D* gS, RAVector* ry) {
    if (gS && gS->getShape() == 3 && ry) {
        ((VRectangle *)gS)->setRY(RAVector(ry->a(), ry->r()));
        return 0;
    }
    
    return -1;
}

bool ne_rec_isSetRatio(VTransformation2D* gS) {
    if (gS && gS->getShape() == 3)
        return ((VRectangle *)gS)->isSetRatio();
    
    return false;
}

const double ne_rec_getRatio(VTransformation2D* gS) {
    if (gS && gS->getShape() == 3)
        return ((VRectangle *)gS)->getRatio();
    
    return 0.0;
}

int ne_rec_setRatio(VTransformation2D* gS, const double& ratio) {
    if (gS && gS->getShape() == 3) {
        ((VRectangle *)gS)->setRatio(ratio);
        return 0;
    }
    
    return -1;
}

// VEllipse

bool ne_elp_isSetPositionCX(VTransformation2D* gS) {
    if (gS && gS->getShape() == 4)
        return ((VEllipse *)gS)->isSetCX();
    
    return false;
}

RAVector* ne_elp_getPositionCX(VTransformation2D* gS) {
    RAVector* rAVector = NULL;
    if (ne_elp_isSetPositionCX(gS))
        rAVector = new RAVector(((VEllipse *)gS)->getCX());
    
    return rAVector;
}

int ne_elp_setPositionCX(VTransformation2D* gS, RAVector* cx) {
    if (gS && gS->getShape() == 4 && cx) {
        ((VEllipse *)gS)->setCX(RAVector(cx->a(), cx->r()));
        return 0;
    }
    
    return -1;
}

bool ne_elp_isSetPositionCY(VTransformation2D* gS) {
    if (gS && gS->getShape() == 4)
        return ((VEllipse *)gS)->isSetCY();
    
    return false;
}

RAVector* ne_elp_getPositionCY(VTransformation2D* gS) {
    RAVector* rAVector = NULL;
    if (ne_elp_isSetPositionCY(gS))
        rAVector = new RAVector(((VEllipse *)gS)->getCY());
    
    return rAVector;
}

int ne_elp_setPositionCY(VTransformation2D* gS, RAVector* cy) {
    if (gS && gS->getShape() == 4 && cy) {
        ((VEllipse *)gS)->setCY(RAVector(cy->a(), cy->r()));
        return 0;
    }
    
    return -1;
}

bool ne_elp_isSetDimensionRX(VTransformation2D* gS) {
    if (gS && gS->getShape() == 4)
        return ((VEllipse *)gS)->isSetRX();
    
    return false;
}

RAVector* ne_elp_getDimensionRX(VTransformation2D* gS) {
    RAVector* rAVector = NULL;
    if (ne_elp_isSetDimensionRX(gS))
        rAVector = new RAVector(((VEllipse *)gS)->getRX());
    
    return rAVector;
}

int ne_elp_setDimensionRX(VTransformation2D* gS, RAVector* rx) {
    if (gS && gS->getShape() == 4 && rx) {
        ((VEllipse *)gS)->setRX(RAVector(rx->a(), rx->r()));
        return 0;
    }
    
    return -1;
}

bool ne_elp_isSetDimensionRY(VTransformation2D* gS) {
    if (gS && gS->getShape() == 4)
        return ((VEllipse *)gS)->isSetRY();
    
    return false;
}

RAVector* ne_elp_getDimensionRY(VTransformation2D* gS) {
    RAVector* rAVector = NULL;
    if (ne_elp_isSetDimensionRY(gS))
        rAVector = new RAVector(((VEllipse *)gS)->getRY());
    
    return rAVector;
}

int ne_elp_setDimensionRY(VTransformation2D* gS, RAVector* ry) {
    if (gS && gS->getShape() == 4 && ry) {
        ((VEllipse *)gS)->setRY(RAVector(ry->a(), ry->r()));
        return 0;
    }
    
    return -1;
}

bool ne_elp_isSetRatio(VTransformation2D* gS) {
    if (gS && gS->getShape() == 4)
        return ((VEllipse *)gS)->isSetRatio();
    
    return false;
}

const double ne_elp_getRatio(VTransformation2D* gS) {
    if (gS && gS->getShape() == 4)
        return ((VEllipse *)gS)->getRatio();
    
    return 0.0;
}

int ne_elp_setRatio(VTransformation2D* gS, const double& ratio) {
    if (gS && gS->getShape() == 4) {
        ((VEllipse *)gS)->setRatio(ratio);
        return 0;
    }
    
    return -1;
}

// VPolygon

const size_t ne_plg_getNumVertices(VTransformation2D* gS) {
    if (gS && gS->getShape() == 5)
        return ((VPolygon *)gS)->getNumElements();
    
    return 0;
}

RenPoint* ne_plg_getVertex(VTransformation2D* gS, const unsigned int& index) {
    if (gS && gS->getShape() == 5) {
        if (index >= 0 && index < int(((VPolygon *)gS)->getNumElements()))
            return ((VPolygon *)gS)->getListOfElements().at(index);
    }
    
    return NULL;
}

RenPoint* ne_plg_addVertex(VTransformation2D* gS, RPoint* point, RPoint* basepoint1, RPoint* basepoint2, const int& index) {
    RenPoint* rp = NULL;
    if (gS && gS->getShape() == 5 && index >= -1 && index <= int(((VPolygon *)gS)->getNumElements())) {
        if (basepoint1 && basepoint2) {
            // create a new cubic bezier
            rp = new RCubicBezier();
            
            // set base points
            ((RCubicBezier*)rp)->setBasePoint1(RPoint(RAVector(basepoint1->x().a(), basepoint1->x().r()), RAVector(basepoint1->y().a(), basepoint1->y().r())));
            ((RCubicBezier*)rp)->setBasePoint2(RPoint(RAVector(basepoint2->x().a(), basepoint2->x().r()), RAVector(basepoint2->y().a(), basepoint2->y().r())));
        }
        else {
            // create a new render point
            rp = new RenPoint();
        }
        
        // set rpoint value
        if (point)
            rp->setRPoint(RPoint(RAVector(point->x().a(), point->x().r()), RAVector(point->y().a(), point->y().r())));
        
        // add the render point to the list of elements
        if (index == -1)
            ((VPolygon *)gS)->addToListOfElementsEnd(rp);
        else
            ((VPolygon *)gS)->addToListOfElements(rp, index);
    }
    
    return rp;
}

int ne_plg_removeVertex(VTransformation2D* gS, const unsigned int& index) {
    if (gS && gS->getShape() == 5 && index >= 0 && index < int(((VPolygon *)gS)->getNumElements())) {
        
        // remove from list
        ((VPolygon *)gS)->removeFromListOfElements(index);
        
        // if the removed element was the first one, and the new first element is a cubic bezier
        if (index == 0 && ((VPolygon *)gS)->getNumElements() && ((VPolygon *)gS)->getListOfElements().at(0)->isRenderCubicBezier()) {
            RenPoint* rp = new RenPoint();
            if (((VPolygon *)gS)->getListOfElements().at(0)->isSetId())
                rp->setId(((VPolygon *)gS)->getListOfElements().at(0)->getId());
            if (((VPolygon *)gS)->getListOfElements().at(0)->isSetRPoint())
                rp->setRPoint(((VPolygon *)gS)->getListOfElements().at(0)->getRPoint());
            ((VPolygon *)gS)->addToListOfElements(rp, 0);
            ((VPolygon *)gS)->removeFromListOfElements(1);
        }
        
        return 0;
    }
    
    return -1;
}

// RenPoint

RPoint* ne_vrx_getRenderPoint(RenPoint* rp) {
    RPoint* point = NULL;
    if (rp && rp->isSetRPoint())
        point = new RPoint(rp->getRPoint());
    
    return point;
}

int ne_vrx_setRenderPoint(RenPoint* rp, RPoint* point) {
    if (rp && point) {
        rp->setRPoint(RPoint(RAVector(point->x().a(), point->x().r()), RAVector(point->y().a(), point->y().r())));
        return 0;
    }
        
    return -1;
}

bool ne_vrx_isRenderCubicBezier(RenPoint* rp) {
    if (rp)
        return rp->isRenderCubicBezier();
    
    return false;
}

// RCubicBezier

RPoint* ne_vrx_getBasePoint1(RenPoint* rp) {
    RPoint* point = NULL;
    if (rp && rp->isRenderCubicBezier() && ((RCubicBezier*)rp)->isSetBasePoint1())
        point = new RPoint(((RCubicBezier*)rp)->getBasePoint1());
    
    return point;
}

int ne_vrx_setBasePoint1(RenPoint* rp, RPoint* point) {
    if (rp && rp->isRenderCubicBezier() && point) {
        ((RCubicBezier*)rp)->setBasePoint1(RPoint(RAVector(point->x().a(), point->x().r()), RAVector(point->y().a(), point->y().r())));
        return 0;
    }
        
    return -1;
}

RPoint* ne_vrx_getBasePoint2(RenPoint* rp) {
    RPoint* point = NULL;
    if (rp && rp->isRenderCubicBezier() && ((RCubicBezier*)rp)->isSetBasePoint2())
        point = new RPoint(((RCubicBezier*)rp)->getBasePoint2());
    
    return point;
}

int ne_vrx_setBasePoint2(RenPoint* rp, RPoint* point) {
    if (rp && rp->isRenderCubicBezier() && point) {
        ((RCubicBezier*)rp)->setBasePoint2(RPoint(RAVector(point->x().a(), point->x().r()), RAVector(point->y().a(), point->y().r())));
        return 0;
    }
        
    return -1;
}


// RPoint

RAVector* ne_rp_getX(RPoint* point) {
    RAVector* rAVector = NULL;
    if (point)
        rAVector = new RAVector(point->x());
    
    return rAVector;
}

int ne_rp_setX(RPoint* point, RAVector* x) {
    if (point && x) {
        point->setX(RAVector(x->a(), x->r()));
        return 0;
    }
        
    return -1;
}

RAVector* ne_rp_getY(RPoint* point) {
    RAVector* rAVector = NULL;
    if (point)
        rAVector = new RAVector(point->y());
    
    return rAVector;
}

int ne_rp_setY(RPoint* point, RAVector* y) {
    if (point && y) {
        point->setY(RAVector(y->a(), y->r()));
        return 0;
    }
        
    return -1;
}

// RAVector

const double ne_rav_getAbsoluteValue(RAVector* rAV) {
    if (rAV)
        return rAV->a();
    
    return 0.0;
}

int ne_rav_setAbsoluteValue(RAVector* rAV, const double& abs) {
    if (rAV) {
        rAV->setA(abs);
        return 0;
    }
        
    return -1;
}

const double ne_rav_getRelativeValue(RAVector* rAV) {
    if (rAV)
        return rAV->r();
    
    return 0.0;
}

int ne_rav_setRelativeValue(RAVector* rAV, const double& rel) {
    if (rAV) {
        rAV->setR(rel);
        return 0;
    }
        
    return -1;
}

}



