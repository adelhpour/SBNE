#include "sbne/veneer/ne_veneer.h"

namespace sbne {

	std::string styleTypeToStr(const StyleType s) {
		switch (s) {
            case ST_TYPE_COMP:
                return "COMPARTMENTGLYPH";
            case ST_TYPE_SPC:
                return "SPECIESGLYPH";
            case ST_TYPE_RXN:
                return "REACTIONGLYPH";
            case ST_TYPE_SPC_REF:
                return "SPECIESREFERENCEGLYPH";
            case ST_TYPE_TXT:
                return "TEXTGLYPH";
            case ST_TYPE_GNR:
                return "GENERALGLYPH";
            case ST_TYPE_GRP_OBJ:
                return "GRAPHICALOBJECT";
            case ST_TYPE_ANY:
                return "ANY";
            case NUM_StyleType:
            default:
                std::cerr << "Unknown StyleType\n";
                return std::string();
		}
	}

    const StyleType styleTypeForGraphicalObjectType(const GrphObjType g) {
        switch (g) {
            case GRP_OBJ_CMP:
                return ST_TYPE_COMP;
            case GRP_OBJ_SPC:
                return ST_TYPE_SPC;
            case GRP_OBJ_RXN:
                return ST_TYPE_RXN;
            case GRP_OBJ_SPC_RFC:
                return ST_TYPE_SPC_REF;
            case GRP_OBJ_TXT:
                return ST_TYPE_TXT;
            default:
                return ST_TYPE_ANY;
        }
    }

	std::string colorStringToStr(const ColorString c) {
		switch (c) {
            case CLR_STR_WHITE:
                return "white";
            case CLR_STR_BLACK:
                return "black";
            case CLR_STR_RED:
                return "red";
            case CLR_STR_DARK_RED:
                return "darkRed";
            case CLR_STR_GREEN:
                return "green";
            case CLR_STR_DARK_GREEN:
                return "darkGreen";
            case CLR_STR_BLUE:
                return "blue";
            case CLR_STR_DARK_BLUE:
                return "darkBlue";
            case CLR_STR_CYAN:
                return "cyan";
            case CLR_STR_DARK_CYAN:
                return "darkCyan";
            case CLR_STR_MAGNETA:
                return "magneta";
            case CLR_STR_DARK_MAGNETA:
                return "darkMagneta";
            case CLR_STR_YELLOW:
                return "yellow";
            case CLR_STR_DARK_YELLOW:
                return "darkYellow";
            case CLR_STR_GRAY:
                return "gray";
            case CLR_STR_DARK_GRAY:
                return "darkGray";
            case CLR_STR_LIGHT_GRAY:
                return "lightGray";
            case CLR_STR_NONE:
                return "none";
            default:
				std::cerr << "Unknown ColorString\n";
                return std::string();
		}
	}

	std::string colorStringToHexStr(const ColorString c) {
		switch (c) {
            case CLR_STR_BLACK:
                return "#000000";
            case CLR_STR_WHITE:
                return "#ffffff";
            case CLR_STR_BLUE:
                return "#0000ff";
            case CLR_STR_DARK_BLUE:
                return "#000080";
            case CLR_STR_CYAN:
                return "#00ffff";
            case CLR_STR_DARK_CYAN:
                return "#008080";
            case CLR_STR_GRAY:
                return "#a0a0a4";
            case CLR_STR_DARK_GRAY:
                return "#808080";
            case CLR_STR_LIGHT_GRAY:
                return "#c0c0c0";
            case CLR_STR_RED:
                return "#ff0000";
            case CLR_STR_DARK_RED:
                return "#800000";
            case CLR_STR_GREEN:
                return "#00ff00";
            case CLR_STR_DARK_GREEN:
                return "#008000";
            case CLR_STR_YELLOW:
                return "#ffff00";
            case CLR_STR_DARK_YELLOW:
                return "#808000";
            case CLR_STR_MAGNETA:
                return "#ff00ff";
            case CLR_STR_DARK_MAGNETA:
                return "#800080";
            case CLR_STR_NONE:
                return "#000000";
			default:
				std::cerr << "Unknown ColorString\n";
                return std::string();
		}
	}

	std::string gSMethodToStr(const GradientSpreadMethod g) {
		switch (g) {
            case GRD_SPR_MTH_PAD:
                return "pad";
            case GRD_SPR_MTH_RFL:
                return "reflect";
            case GRD_SPR_MTH_RPT:
                return "repeat";
			default:
				std::cerr << "Unknown GradientSpreadMethod\n";
                return std::string();
		}
	}

	std::string fillRuleToStr(const FillRule f) {
		switch (f) {
            case FIL_RUL_NON_ZER:
                return "nonzero";
            case FIL_RUL_EVN_ODD:
                return "evenodd";
			default:
				std::cerr << "Unknown FillRule\n";
                return std::string();
		}
	}

    std::string fontFamilyToStr(const FontFamily f) {
		switch (f) {
            case FNT_FMLY_SRF:
                return "serif";
            case FNT_FMLY_SNS_SRF:
                return "sans-serif";
            case FNT_FMLY_MNO_SPC:
                return "monospace";
			default:
				std::cerr << "Unknown FontFamily\n";
                return std::string();
		}
	}

    std::string fontWeightToStr(const FontWeight f) {
		switch (f) {
            case FNT_WGHT_BLD:
                return "bold";
            case FNT_WGHT_NRM:
                return "normal";
			default:
				std::cerr << "Unknown FontWeight\n";
                return std::string();
		}
	}

    std::string fontStyleToStr(const FontStyle f) {
		switch (f) {
            case FNT_STL_ITLC:
                return "italic";
            case FNT_STL_NRM:
                return "normal";
			default:
				std::cerr <<"Unknown FontStyle\n";
                return std::string();
		}
	}

	std::string hTextAnchorToStr(const HTextAnchor h) {
		switch (h) {
            case HTXT_ANCH_STRT:
                return "start";
            case HTXT_ANCH_MDL:
                return "middle";
            case HTXT_ANCH_END:
                return "end";
			default:
				std::cerr <<"Unknown HTextAnchor\n";
                return std::string();
		}
	}

	std::string vTextAnchorToStr(const VTextAnchor v) {
		switch (v) {
            case VTXT_ANCH_TOP:
			return "top";
            case VTXT_ANCH_MDL:
                return "middle";
            case VTXT_ANCH_BTM:
                return "bottom";
            case VTXT_ANCH_BLNE:
                return "baseline";
			default:
				std::cerr <<"Unknown VTextAnchor\n";
                return std::string();
		}
	}

	std::string rGroupElementToStr(const RenderGroupElementShape s) {
		switch (s) {
            case GRP_ELT_SHPE_CRV:
                return "rendercurve";
            case GRP_ELT_SHPE_PLG:
                return "polygon";
            case GRP_ELT_SHPE_REC:
                return "rectangle";
            case GRP_ELT_SHPE_ELP:
                return "ellipse";
            case GRP_ELT_SHPE_TXT:
                return "text";
            case GRP_ELT_SHPE_IMG:
                return "image";
			default:
				std::cerr <<"Unknown RenderGroupElementShape\n";
                return std::string();
		}
	}

	Veneer* veneerFromRender(RenderInformationBase& ren, Veneer* ven) {
        // get background color
        if (ren.isSetBackgroundColor())
            ven->setBackgroundColor(ren.getBackgroundColor());
        
		// get colors
		for (int i = 0; i < ren.getNumColorDefinitions(); ++i) {
			const ColorDefinition * cd = ren.getColorDefinition(i);
			VColorDefinition* c = new VColorDefinition();

			// get id
			if (cd->isSetId())
				c->setId(cd->getId());
            else
                c->setId(ven->getColorUniqueId());

			// get name
			if (cd->isSetName())
				c->setName(cd->getName());

			// get value
			if (cd->isSetValue())
				c->setValue(cd->getValue());

			ven->addColor(c);
		}

		// get gradients
		for (int i = 0; i < ren.getNumGradientDefinitions(); ++i) {
			const GradientBase * gb = ren.getGradientDefinition(i);
			VGradientBase* g = NULL;

			// if it is a linear gradient
			if (gb->isLinearGradient())
				g = new VLinearGradient();

			// if it is a radial gradient
			if (gb->isRadialGradient())
				g = new VRadialGradient();

			// get id
			if (gb->isSetId())
				g->setId(gb->getId());
            else
                g->setId(ven->getGradientUniqueId());
                
			// get name
			if (gb->isSetName())
				g->setName(gb->getName());

			// get gradient spread method
			if (gb->isSetSpreadMethod())
				g->setSpreadMehtod(gb->getSpreadMethodAsString());

			// get gradient stops
			for (int j = 0; j < gb->getNumGradientStops(); ++j) {
				const GradientStop * gs = gb->getGradientStop(j);
				VGradientStop* s = new VGradientStop();

				// get id
				if (gs->isSetId())
					s->setId(gs->getId());
                else
                    s->setId(g->getStopUniqueId());

				// get name
				if (gs->isSetName())
					s->setName(gs->getName());

				// get stop color
                if (gs->isSetStopColor()) {
                    // check to see it is not a gradient id
                    if (ven->findGradientById(gs->getStopColor()))
                        std::cerr << "a gradient cannot be set as the stop color\n";
                    // check to see it is not none
                    else if (gs->getStopColor() == "none")
                        std::cerr << "none cannot be set as the stop color\n";
                    // if the color fulfill the conditions
                    else
                        s->setStopColor(gs->getStopColor());
                }
                
				// get offset
				if (gs->isSetOffset()) {
                    RAVector r(0.0, 0.0);
                    
                    // check to see it is not biggger than 100
                    if (gs->getOffset().getRelativeValue() > 100.0)
                        r.setR(100.);
                    // check to see it is not smaller than 0
                    else if (gs->getOffset().getRelativeValue() < 0.0)
                        r.setR(0.);
                    
                    s->setOffset(r);
                }
                
                // add the stop to gradient
				g->addToStops(s);
			}
            
            // sort the gradient stops based on their offset values
            g->sortStops();

			// if it is a linear gradient
			if (gb->isLinearGradient()) {
				VLinearGradient* lgrad = NULL;
                
                // cast the gradient to a linear gradient
                try {
                     lgrad = dynamic_cast<VLinearGradient*>(g);
                }
                catch(std::bad_cast) {
                     std::cerr << "Unable to cast the gradient to linear gradient\n";
                }
                
				// get x1
				if (((LinearGradient *)gb)->isSetX1()) {
                    RAVector r;
					r.setA(((LinearGradient *)gb)->getX1().getAbsoluteValue());
					r.setR(((LinearGradient *)gb)->getX1().getRelativeValue());
					lgrad->setX1(r);
				}

				// get y1
				if (((LinearGradient *)gb)->isSetY1()) {
                    RAVector r;
                    r.setA(((LinearGradient *)gb)->getY1().getAbsoluteValue());
                    r.setR(((LinearGradient *)gb)->getY1().getRelativeValue());
                    lgrad->setY1(r);
                }
                
				// get z1
				if (((LinearGradient *)gb)->isSetZ1()) {
                    RAVector r;
                    r.setA(((LinearGradient *)gb)->getZ1().getAbsoluteValue());
                    r.setR(((LinearGradient *)gb)->getZ1().getRelativeValue());
                    lgrad->setZ1(r);
                }

				// get x2
                if (((LinearGradient *)gb)->isSetX2()) {
                    RAVector r;
                    r.setA(((LinearGradient *)gb)->getX2().getAbsoluteValue());
                    r.setR(((LinearGradient *)gb)->getX2().getRelativeValue());
                    lgrad->setX2(r);
                }
                
				// get y2
                if (((LinearGradient *)gb)->isSetY2()) {
                    RAVector r;
                    r.setA(((LinearGradient *)gb)->getY2().getAbsoluteValue());
                    r.setR(((LinearGradient *)gb)->getY2().getRelativeValue());
                    lgrad->setY2(r);
                }

				// get z2
				if (((LinearGradient *)gb)->isSetZ2()) {
                    RAVector r;
                    r.setA(((LinearGradient *)gb)->getZ2().getAbsoluteValue());
                    r.setR(((LinearGradient *)gb)->getZ2().getRelativeValue());
                    lgrad->setZ2(r);
                }
                
                // add the linear gradient to veneer
				ven->addGradient(lgrad);
			}

			// if it's a radial gradient
			if (gb->isRadialGradient()) {
				VRadialGradient* rgrad = NULL;
                
                // cast the gradient to a radial gradient
                try {
                    rgrad = dynamic_cast<VRadialGradient*>(g);
                }
                catch(std::bad_cast) {
                    std::cerr <<"Unable to cast the gradient to radial gradient\n";
                }

				// get cx
				if (((RadialGradient *)gb)->isSetCx()) {
                    RAVector r;
                    r.setA(((RadialGradient *)gb)->getCx().getAbsoluteValue());
                    r.setR(((RadialGradient *)gb)->getCx().getRelativeValue());
                    rgrad->setCx(r);
                }

				// get cy
				if (((RadialGradient *)gb)->isSetCy()) {
                    RAVector r;
                    r.setA(((RadialGradient *)gb)->getCy().getAbsoluteValue());
                    r.setR(((RadialGradient *)gb)->getCy().getRelativeValue());
                    rgrad->setCy(r);
                }
                
				// get cz
				if (((RadialGradient *)gb)->isSetCz()) {
                    RAVector r;
                    r.setA(((RadialGradient *)gb)->getCz().getAbsoluteValue());
                    r.setR(((RadialGradient *)gb)->getCz().getRelativeValue());
                    rgrad->setCz(r);
                }

				// get fx
				if (((RadialGradient *)gb)->isSetFx()) {
                    RAVector r;
                    r.setA(((RadialGradient *)gb)->getFx().getAbsoluteValue());
                    r.setR(((RadialGradient *)gb)->getFx().getRelativeValue());
                    rgrad->setFx(r);
                }
                // if fx is not set, set it the same as cx
                else
                    rgrad->setFx(rgrad->getCx());

				// get fy
				if (((RadialGradient *)gb)->isSetFy()) {
                    RAVector r;
                    r.setA(((RadialGradient *)gb)->getFy().getAbsoluteValue());
                    r.setR(((RadialGradient *)gb)->getFy().getRelativeValue());
                    rgrad->setFy(r);
                }
                // if fy is not set, set it the same as cy
                else
                    rgrad->setFy(rgrad->getCy());

				// get fz
				if (((RadialGradient *)gb)->isSetFz()) {
                    RAVector r;
                    r.setA(((RadialGradient *)gb)->getFz().getAbsoluteValue());
                    r.setR(((RadialGradient *)gb)->getFz().getRelativeValue());
                    rgrad->setFz(r);
                }
                // if fz is not set, set it the same as cz
                else
                    rgrad->setFz(rgrad->getCz());

				// get r
				if (((RadialGradient *)gb)->isSetR()) {
                    RAVector r;
                    r.setA(((RadialGradient *)gb)->getR().getAbsoluteValue());
                    r.setR(((RadialGradient *)gb)->getR().getRelativeValue());
                    rgrad->setR(r);
                }
                
                // add the radial gradient to veneer
				ven->addGradient(rgrad);
			}
		}
        
        // get styles
        // global styles
        if (ren.isGlobalRenderInformation()){
            for (int i = 0; i < ((GlobalRenderInformation&)ren).getNumStyles(); ++i) {
                const Style * sb = ((GlobalRenderInformation&)ren).getStyle(i);
                VGlobalStyle* s = new VGlobalStyle();
                
                // get id
                if (sb->isSetId())
                    s->setId(sb->getId());

                // get name
                if (sb->isSetName())
                    s->setName(sb->getName());

                // get role list
                s->setRoleList(sb->getRoleList());

                // get type list
                s->setTypeList(sb->getTypeList());
                
                // get render group
                if (sb->isSetGroup()) {
                    const RenderGroup* rg = sb->getGroup();
                    VRenderGroup* g = getRenderGroup(rg);
                    s->setGroup(g);
                }
                
                // add the global style to veneer
                ven->addStyle(s);
            }
        }
        // local styles
        else if(ren.isLocalRenderInformation()){
            for (int i = 0; i < ((LocalRenderInformation&)ren).getNumStyles(); ++i) {
                const Style * sb = ((LocalRenderInformation&)ren).getStyle(i);
                VLocalStyle* s = new VLocalStyle();

                // get id
                if (sb->isSetId())
                    s->setId(sb->getId());

                // get name
                if (sb->isSetName())
                    s->setName(sb->getName());

                // get role list
                s->setRoleList(sb->getRoleList());

                // get type list
                s->setTypeList(sb->getTypeList());
                
                // get id list
                s->setIdList(((LocalStyle*)sb)->getIdList());

                // get render group
                if (sb->isSetGroup()) {
                    const RenderGroup *rg = sb->getGroup();
                    VRenderGroup* g = getRenderGroup(rg);
                    s->setGroup(g);
                }

                 // add the local style to veneer
                ven->addStyle(s);
            }
        }
        
		// line endings
		for (int i = 0; i < ren.getNumLineEndings(); ++i) {
			const LineEnding* le = ren.getLineEnding(i);
			VLineEnding* l = new VLineEnding();

			// get id
			if (le->isSetId())
				l->setId(le->getId());

			// get name
			if (le->isSetName())
				l->setName(le->getName());

			// get enable rotation mapping
			if (le->isSetEnableRotationalMapping())
                l->setEnableRotationMapping(le->getEnableRotationalMapping());

			// get bounding box
			if (le->isSetBoundingBox()) {
				LBox* b = new LBox();
				b->setX(le->getBoundingBox()->x());
				b->setY(le->getBoundingBox()->y());
				b->setWidth(le->getBoundingBox()->width());
				b->setHeight(le->getBoundingBox()->height());
				l->setBox(b);
			}

			// get render group
			if (le->isSetGroup()) {
				const RenderGroup* rg = le->getGroup();
				VRenderGroup* g = getRenderGroup(rg);
				l->setGroup(g);
			}
            
			ven->addLineEnding(l);
		}

        // enable render specified in veneer
		ven->setRenderSpecified(true);
        
		return ven;
	}

	VRenderGroup* getRenderGroup(const RenderGroup* rg) {
		VRenderGroup* g = new VRenderGroup();

		// get id
		if (rg->isSetId())
            g->setId(rg->getId());

		// get name
		if (rg->isSetName())
			g->setName(rg->getName());

		// get stroke
		if (rg->isSetStroke())
			g->setStroke(rg->getStroke());
        
		// get stroke width
		if (rg->isSetStrokeWidth())
			g->setStrokeWidth(rg->getStrokeWidth());

		// get stroke dash array
		if (rg->isSetStrokeDashArray())
			g->setDashArray(rg->getStrokeDashArray());

		// get fill
		if (rg->isSetFill())
			g->setFill(rg->getFill());

		// get fill rule
		if (rg->isSetFillRule())
			g->setFillRule(rg->getFillRuleAsString());

		// get start head
		if (rg->isSetStartHead())
			g->setStartHead(rg->getStartHead());

		// get end head
		if (rg->isSetEndHead())
			g->setEndHead(rg->getEndHead());

		// get font size
		if (rg->isSetFontSize()) {
            RAVector r;
            r.setA(rg->getFontSize().getAbsoluteValue());
            r.setR(rg->getFontSize().getRelativeValue());
            g->setFontSize(r);
        }
            
		// get font family
		if (rg->isSetFontFamily())
			g->setFontFamily(rg->getFontFamily());

		// get font weight
		if (rg->isSetFontWeight())
			g->setFontWeight(rg->getFontWeightAsString());
        
		// get font style
		if (rg->isSetFontStyle())
			g->setFontStyle(rg->getFontStyleAsString());
       
		// get horizontal text anchor
		if (rg->isSetTextAnchor())
			g->setHTextAnchor(rg->getTextAnchorAsString());

		// get vertical text anchor
		if (rg->isSetVTextAnchor())
			g->setVTextAnchor(rg->getVTextAnchorAsString());
        
        // get transform
        if (rg->isSetTransform())
            g->setTransform(rg->getMatrix2D());
       
		// get graphical shapes
		for (int j = 0; j < rg->getNumElements(); ++j) {
			const Transformation2D * t2d = rg->getElement(j);

			// if it is an image
			if (t2d->isImage()) {
				VImage *iShape = new VImage();

				// get id
				if (t2d->isSetId())
					iShape->setId(t2d->getId());

				// get name
				if (t2d->isSetName())
					iShape->setName(t2d->getName());
                
                // get transform
                if (t2d->isSetTransform())
                    iShape->setTransform(t2d->getMatrix2D());

				// get x
				if (((Image *)t2d)->isSetX()) {
                    RAVector r;
                    r.setA((((Image *)t2d)->getX().getAbsoluteValue()));
                    r.setR((((Image *)t2d)->getX().getRelativeValue()));
					iShape->setX(r);
				}

				// get y
				if (((Image *)t2d)->isSetY()) {
                    RAVector r;
                    r.setA((((Image *)t2d)->getY().getAbsoluteValue()));
                    r.setR((((Image *)t2d)->getY().getRelativeValue()));
                    iShape->setY(r);
				}

				// get z
				if (((Image *)t2d)->isSetZ()) {
                    RAVector r;
                    r.setA((((Image *)t2d)->getZ().getAbsoluteValue()));
                    r.setR((((Image *)t2d)->getZ().getRelativeValue()));
                    iShape->setZ(r);
				}
                             
                // get width
                if (((Image *)t2d)->isSetWidth()) {
                    RAVector r;
                    r.setA((((Image *)t2d)->getWidth().getAbsoluteValue()));
                    r.setR((((Image *)t2d)->getWidth().getRelativeValue()));
                    iShape->setWidth(r);
                }

				// get height
				if (((Image *)t2d)->isSetHeight()) {
                    RAVector r;
                    r.setA((((Image *)t2d)->getHeight().getAbsoluteValue()));
                    r.setR((((Image *)t2d)->getHeight().getRelativeValue()));
                    iShape->setHeight(r);
				}

				// get href
				if (((Image *)t2d)->isSetHref())
					iShape->setHref(((Image *)t2d)->getHref());

				g->addToListOfElements(iShape);
			}

			// if it's a render curve
			if (t2d->isRenderCurve()) {
				RCurve *rCurve = new RCurve();

				// get id
				if (t2d->isSetId())
					rCurve->setId(t2d->getId());

				// get name
				if (t2d->isSetName())
					rCurve->setName(t2d->getName());
                
                // get transform
                if (t2d->isSetTransform())
                    rCurve->setTransform(t2d->getMatrix2D());

				// get stroke
				if (((RenderCurve*)t2d)->isSetStroke())
					rCurve->setStroke(((RenderCurve*)t2d)->getStroke());

				// get stroke width
				if (((RenderCurve*)t2d)->isSetStrokeWidth())
					rCurve->setStrokeWidth(((RenderCurve*)t2d)->getStrokeWidth());

				// get stroke dash array
				if (((RenderCurve*)t2d)->isSetStrokeDashArray())
					rCurve->setDashArray(((RenderCurve*)t2d)->getStrokeDashArray());

				// get start head
				if (((RenderCurve *)t2d)->isSetStartHead())
					rCurve->setStartHead(((RenderCurve *)t2d)->getStartHead());

				// get end head
				if (((RenderCurve *)t2d)->isSetEndHead())
					rCurve->setEndHead(((RenderCurve *)t2d)->getEndHead());

				// get elements
				const ListOfCurveElements * lCurveElements = ((RenderCurve *)t2d)->getListOfElements();

				for (int k = 0; k < lCurveElements->getNumRenderPoints(); ++k) {
					const RenderPoint * rp = lCurveElements->get(k);

					// if it is a render point
					if (rp->isRenderPoint())
						rCurve->addToListOfElementsEnd(getRenderPoint(rp));

					// if it is a render cubic bezier
					if (rp->isRenderCubicBezier())
                        rCurve->addToListOfElementsEnd(getRenderCubicBezier((RenderCubicBezier *)rp));
				}

				g->addToListOfElements(rCurve);
			}

			// if it is a text
			if (t2d->isText()) {
				VText* tShape = new VText();

				// get id
				if (t2d->isSetId())
					tShape->setId(t2d->getId());

				// get name
				if (t2d->isSetName())
					tShape->setName(t2d->getName());
                
                // get transform
                if (t2d->isSetTransform())
                    tShape->setTransform(t2d->getMatrix2D());

				// get stroke
				if (((Text*)t2d)->isSetStroke())
                    tShape->setStroke(((Text*)t2d)->getStroke());

				// get stroke width
				if (((Text*)t2d)->isSetStrokeWidth())
					tShape->setStrokeWidth(((Text*)t2d)->getStrokeWidth());

				// get stroke dash array
				if (((Text*)t2d)->isSetStrokeDashArray())
					tShape->setDashArray(((Text*)t2d)->getStrokeDashArray());

				// get x
				if (((Text *)t2d)->isSetX()) {
                    RAVector r;
                    r.setA(((Text *)t2d)->getX().getAbsoluteValue());
                    r.setR(((Text *)t2d)->getX().getRelativeValue());
                    tShape->setX(r);
				}

				// get y
				if (((Text *)t2d)->isSetY()) {
					RAVector r;
                    r.setA(((Text *)t2d)->getY().getAbsoluteValue());
                    r.setR(((Text *)t2d)->getY().getRelativeValue());
                    tShape->setY(r);
				}

				// get z
				if (((Text *)t2d)->isSetZ()) {
					RAVector r;
                    r.setA(((Text *)t2d)->getZ().getAbsoluteValue());
                    r.setR(((Text *)t2d)->getZ().getRelativeValue());
                    tShape->setZ(r);
				}

				// get font size
				if (((Text *)t2d)->isSetFontSize()) {
                    RAVector r;
                    r.setA(((Text *)t2d)->getFontSize().getAbsoluteValue());
                    r.setR(((Text *)t2d)->getFontSize().getRelativeValue());
                    tShape->setFontSize(r);
				}

				// get font family
				if (((Text *)t2d)->isSetFontFamily())
					tShape->setFontFamily(((Text *)t2d)->getFontFamily());

				// get font weight
				if (((Text *)t2d)->isSetFontWeight())
					tShape->setFontWeight(((Text *)t2d)->getFontWeightAsString());

				// get font style
				if (((Text *)t2d)->isSetFontStyle())
					tShape->setFontStyle(((Text *)t2d)->getFontStyleAsString());

				// get horizontal text anchor
				if (((Text *)t2d)->isSetTextAnchor())
					tShape->setHTextAnchor(((Text *)t2d)->getTextAnchorAsString());
                
				// get vertical text anchor
				if (((Text *)t2d)->isSetVTextAnchor())
					tShape->setVTextAnchor(((Text *)t2d)->getVTextAnchorAsString());

				g->addToListOfElements(tShape);
			}

			// if it is a rectangle
			if (t2d->isRectangle()) {
				VRectangle* rShape = new VRectangle();

				// get id
				if (t2d->isSetId())
					rShape->setId(t2d->getId());

				// get name
				if (t2d->isSetName())
					rShape->setName(t2d->getName());
                
                // get transform
                if (t2d->isSetTransform())
                    rShape->setTransform(t2d->getMatrix2D());

				// get stroke
				if (((Rectangle*)t2d)->isSetStroke())
					rShape->setStroke(((Rectangle*)t2d)->getStroke());

				// get stroke width
				if (((Rectangle*)t2d)->isSetStrokeWidth())
					rShape->setStrokeWidth(((Rectangle*)t2d)->getStrokeWidth());

				// get stroke dash array
				if (((Rectangle*)t2d)->isSetStrokeDashArray())
					rShape->setDashArray(((Rectangle*)t2d)->getStrokeDashArray());

				// get fill
				if (((Rectangle*)t2d)->isSetFill())
					rShape->setFill(((Rectangle*)t2d)->getFill());

				// get x
				if (((Rectangle *)t2d)->isSetX()) {
                    RAVector r;
                    r.setA(((Rectangle *)t2d)->getX().getAbsoluteValue());
                    r.setR(((Rectangle *)t2d)->getX().getRelativeValue());
                    rShape->setX(r);
				}

				// get y
				if (((Rectangle *)t2d)->isSetY()) {
					RAVector r;
                    r.setA(((Rectangle *)t2d)->getY().getAbsoluteValue());
                    r.setR(((Rectangle *)t2d)->getY().getRelativeValue());
                    rShape->setY(r);
				}

				// get z
				if (((Rectangle *)t2d)->isSetZ()) {
					RAVector r;
                    r.setA(((Rectangle *)t2d)->getZ().getAbsoluteValue());
                    r.setR(((Rectangle *)t2d)->getZ().getRelativeValue());
                    rShape->setZ(r);
				}
                                      
                // get width
                if (((Rectangle *)t2d)->isSetWidth()) {
                    RAVector r;
                    r.setA(((Rectangle *)t2d)->getWidth().getAbsoluteValue());
                    r.setR(((Rectangle *)t2d)->getWidth().getRelativeValue());
                    rShape->setWidth(r);
                }

				// get height
				if (((Rectangle *)t2d)->isSetHeight()) {
					RAVector r;
                    r.setA(((Rectangle *)t2d)->getHeight().getAbsoluteValue());
                    r.setR(((Rectangle *)t2d)->getHeight().getRelativeValue());
                    rShape->setHeight(r);
				}

				// get rx
				if (((Rectangle *)t2d)->isSetRX()) {
					RAVector r;
                    r.setA(((Rectangle *)t2d)->getRX().getAbsoluteValue());
                    r.setR(((Rectangle *)t2d)->getRX().getRelativeValue());
                    rShape->setRX(r);
				}

				// get ry
				if (((Rectangle *)t2d)->isSetRY()) {
					RAVector r;
                    r.setA(((Rectangle *)t2d)->getRY().getAbsoluteValue());
                    r.setR(((Rectangle *)t2d)->getRY().getRelativeValue());
                    rShape->setRY(r);
				}

				// get ratio
				if (((Rectangle *)t2d)->isSetRatio())
					rShape->setRatio(((Rectangle *)t2d)->getRatio());

				g->addToListOfElements(rShape);
			}

			// if it is an ellipse
			if (t2d->isEllipse()) {
				VEllipse* eShape = new VEllipse();

				// get id
				if (t2d->isSetId())
					eShape->setId(t2d->getId());

				// get name
				if (t2d->isSetName())
					eShape->setName(t2d->getName());
                
                // get transform
                if (t2d->isSetTransform())
                    eShape->setTransform(t2d->getMatrix2D());

				// get stroke
				if (((Ellipse*)t2d)->isSetStroke())
                    eShape->setStroke(((Ellipse*)t2d)->getStroke());
                
				// get stroke width
				if (((Ellipse*)t2d)->isSetStrokeWidth())
					eShape->setStrokeWidth(((Ellipse*)t2d)->getStrokeWidth());

				// get stroke dash array
				if (((Ellipse*)t2d)->isSetStrokeDashArray())
					eShape->setDashArray(((Ellipse*)t2d)->getStrokeDashArray());

				// get fill
				if (((Ellipse*)t2d)->isSetFill())
					eShape->setFill(((Ellipse*)t2d)->getFill());
                
				// get cx
				if (((Ellipse *)t2d)->isSetCX()) {
                    RAVector r;
                    r.setA(((Ellipse *)t2d)->getCX().getAbsoluteValue());
                    r.setR(((Ellipse *)t2d)->getCX().getRelativeValue());
                    eShape->setCX(r);
				}

				// get cy
				if (((Ellipse *)t2d)->isSetCY()) {
					RAVector r;
                    r.setA(((Ellipse *)t2d)->getCY().getAbsoluteValue());
                    r.setR(((Ellipse *)t2d)->getCY().getRelativeValue());
                    eShape->setCY(r);
				}

				// get cz
				if (((Ellipse *)t2d)->isSetCZ()) {
					RAVector r;
                    r.setA(((Ellipse *)t2d)->getCZ().getAbsoluteValue());
                    r.setR(((Ellipse *)t2d)->getCZ().getRelativeValue());
                    eShape->setCZ(r);
				}

				// get rx
				if (((Ellipse *)t2d)->isSetRX()) {
					RAVector r;
                    r.setA(((Ellipse *)t2d)->getRX().getAbsoluteValue());
                    r.setR(((Ellipse *)t2d)->getRX().getRelativeValue());
                    eShape->setRX(r);

                    // if ry is not set, set its value the same as rx
					if (!((Ellipse *)t2d)->isSetRY())
						eShape->setRY(r);
				}

				// get ry
				if (((Ellipse *)t2d)->isSetRY()) {
					RAVector r;
                    r.setA(((Ellipse *)t2d)->getRY().getAbsoluteValue());
                    r.setR(((Ellipse *)t2d)->getRY().getRelativeValue());
                    eShape->setRY(r);

                    // if rx is not set, set its value the same as ry
					if (!((Ellipse *)t2d)->isSetRX())
                        eShape->setRX(r);
				}

				// get ratio
				if (((Ellipse *)t2d)->isSetRatio())
					eShape->setRatio(((Ellipse *)t2d)->getRatio());

				g->addToListOfElements(eShape);
			}

			// if it is a polygon
			if (t2d->isPolygon()) {
				VPolygon* pShape = new VPolygon();

				// get id
				if (t2d->isSetId())
					pShape->setId(t2d->getId());

				// get name
				if (t2d->isSetName())
					pShape->setName(t2d->getName());
                
                // get transform
                if (t2d->isSetTransform())
                    pShape->setTransform(t2d->getMatrix2D());

				// get stroke
				if (((Polygon*)t2d)->isSetStroke())
					pShape->setStroke(((Polygon*)t2d)->getStroke());

				// get stroke width
				if (((Polygon*)t2d)->isSetStrokeWidth())
					pShape->setStrokeWidth(((Polygon*)t2d)->getStrokeWidth());

				// get stroke dash array
				if (((Polygon*)t2d)->isSetStrokeDashArray())
					pShape->setDashArray(((Polygon*)t2d)->getStrokeDashArray());

				// get fill
				if (((Polygon*)t2d)->isSetFill())
					pShape->setFill(((Polygon*)t2d)->getFill());

				// get fill rule
				if (((Polygon*)t2d)->isSetFillRule())
					pShape->setFillRule(((Polygon*)t2d)->getFillRuleAsString());

				// get elements
				const ListOfCurveElements * lce = ((Polygon *)t2d)->getListOfElements();

				for (int k = 0; k < lce->getNumRenderPoints(); ++k) {
					const RenderPoint * rp = lce->get(k);
                    
                    // if it is a render point
                    if (rp->isRenderPoint())
                        pShape->addToListOfElementsEnd(getRenderPoint(rp));
                    
                    // if it is a render cubic bezier
                    if (rp->isRenderCubicBezier())
                        pShape->addToListOfElementsEnd(getRenderCubicBezier((RenderCubicBezier *)rp));
				}

				g->addToListOfElements(pShape);
			}
		}

		return g;
	}

	RenPoint* getRenderPoint(const RenderPoint* rp) {
        RenPoint* renPoint = new RenPoint();
        RPoint rPoint;
        
        // rpoint
        // get x
        rPoint.setX(RAVector(rp->getX().getAbsoluteValue(), rp->getX().getRelativeValue()));
                        
        // get y
        rPoint.setY(RAVector(rp->getY().getAbsoluteValue(), rp->getY().getRelativeValue()));
                        
        // get z
        rPoint.setZ(RAVector(rp->getZ().getAbsoluteValue(), rp->getZ().getRelativeValue()));
                        
        renPoint->setRPoint(rPoint);
        return renPoint;
	}

	RCubicBezier* getRenderCubicBezier(const RenderCubicBezier* rcb) {
        RCubicBezier* rCubicBezier = new RCubicBezier();
        RPoint rPoint;
                        
        // rpoint
        // get x
        rPoint.setX(RAVector(rcb->getX().getAbsoluteValue(), rcb->getX().getRelativeValue()));
                        
        // get y
        rPoint.setY(RAVector(rcb->getY().getAbsoluteValue(), rcb->getY().getRelativeValue()));
                        
        // get z
        rPoint.setZ(RAVector(rcb->getZ().getAbsoluteValue(), rcb->getZ().getRelativeValue()));
                        
        rCubicBezier->setRPoint(rPoint);
                        
        // base point 1
        // get x
        rPoint.setX(RAVector(rcb->getBasePoint1_x().getAbsoluteValue(), rcb->getBasePoint1_x().getRelativeValue()));
                        
        // get y
        rPoint.setY(RAVector(rcb->getBasePoint1_y().getAbsoluteValue(), rcb->getBasePoint1_y().getRelativeValue()));
                        
        // get z
        rPoint.setZ(RAVector(rcb->getBasePoint1_z().getAbsoluteValue(), rcb->getBasePoint1_z().getRelativeValue()));
                        
        rCubicBezier->setBasePoint1(rPoint);
                        
        // base point 2
        // get x
        rPoint.setX(RAVector(rcb->getBasePoint2_x().getAbsoluteValue(), rcb->getBasePoint2_x().getRelativeValue()));
                        
        // get y
        rPoint.setY(RAVector(rcb->getBasePoint2_y().getAbsoluteValue(), rcb->getBasePoint2_y().getRelativeValue()));
                        
        // get z
        rPoint.setZ(RAVector(rcb->getBasePoint2_z().getAbsoluteValue(), rcb->getBasePoint2_z().getRelativeValue()));
                        
        rCubicBezier->setBasePoint2(rPoint);
                        
        return rCubicBezier;
	}

	//--CLASS VeneerElement--

	void VeneerElement::setId(const std::string& id) {
		_id = id;
		_isSetId = true;
	}

	const std::string& VeneerElement::getId() const {
		return _id;
	}

	void VeneerElement::setName(const std::string& name) {
		_name = name;
		_isSetName = true;
	}

	const std::string& VeneerElement::getName() const {
		return _name;
	}

	//--CLASS Veneer--

	void Veneer::addColor(VColorDefinition* c) {
        if(c)
            _colors.push_back(c);
        else
            std::cout << "No color to add\n";
	}

	void Veneer::removeColor(unsigned int colorIndex) {
        if (colorIndex >= _colors.size() || colorIndex < 0)
            std::cerr << "the entered color index is not within the colorVec range\n";
        else {
            // set the iterator to the desired color
            constColorIt _it = colorsBegin();
            for (int i = 0; i < colorIndex; ++i)
                ++_it;
            // remove the desired color from the colorVec
            _colors.erase(_it);
        }
	}

	void Veneer::setColors(const colorVec& cv) {
		_colors = cv;
	}

	const Veneer::colorVec& Veneer::getColors() const {
		return _colors;
	}

	const size_t Veneer::getNumColors() const {
		return _colors.size();
	}

	VColorDefinition* Veneer::findColorById(const std::string& id) {
		// search in colors to find the desired one
		for (constColorIt i = colorsBegin(); i != colorsEnd(); ++i) {
			if (stringCompare((*i)->getId(), id))
				return *i;
		}
		// not found
        return NULL;
	}

	unsigned int Veneer::findColorIndexById(const std::string& id) {
		unsigned int _colorIndex = 0;

		// search in colors to find the desired one
		for (constColorIt i = colorsBegin(); i != colorsEnd(); ++i) {
			if (stringCompare((*i)->getId(), id))
				return _colorIndex;
			++_colorIndex;
		}
		// not found
        return -1;
	}
	
	std::string Veneer::getColorUniqueId() {
		size_t k = 0;
		VColorDefinition* c = NULL;

		// create a new color id and search in the exisiting ones to ensure it has not already used
        while (1) {
            ++k;
            std::stringstream ss;
            ss << "Color_" << k;
            c = findColorById(ss.str());
            if (!c)
                return ss.str();
        }
	}

	void Veneer::addGradient(VGradientBase* g) {
		if (g)
            _gradients.push_back(g);
        else
            std::cout << "No gradient to add\n";
	}

	void Veneer::removeGradient(unsigned int gradientIndex) {
        if (gradientIndex >= _gradients.size() || gradientIndex < 0)
            std::cerr << "the entered gradient index is not within the gradientVec range\n";
        else {
            // set the iterator to the desired gradient
            constGradientIt _it = gradientsBegin();
            for (int i = 0; i < gradientIndex; ++i)
                ++_it;
            // remove the desired gradient from the gradientVec
            _gradients.erase(_it);
        }
	}

	void Veneer::setGradients(const gradientVec& gv) {
		_gradients = gv;
	}

	const Veneer::gradientVec Veneer::getGradients() const {
		return _gradients;
	}

	const size_t Veneer::getNumGradients() const {
		return _gradients.size();
	}

	VGradientBase* Veneer::findGradientById(const std::string& id) {
		// search in the gradients to find the desired one
		for (constGradientIt i = gradientsBegin(); i != gradientsEnd(); ++i) {
			if (stringCompare((*i)->getId(), id))
				return *i;
		}
		// not found
        return NULL;
	}

	unsigned int Veneer::findGradientIndexById(const std::string& id) {
		unsigned int _gradientIndex = 0;

		// search in gradients to find the desired one
		for (constGradientIt i = gradientsBegin(); i != gradientsEnd(); ++i) {
			if (stringCompare((*i)->getId(), id))
				return _gradientIndex;
			++_gradientIndex;
		}
		// not found
        return -1;
	}

	std::string Veneer::getGradientUniqueId() {
		std::size_t k = 0;
		VGradientBase* g = NULL;

		// create a new gradient id and search in the exisiting ones to ensure it has not already used
        while (1) {
            ++k;
            std::stringstream ss;
            ss << "Gradient_" << k;
            g = findGradientById(ss.str());
            if (!g)
                return ss.str();
        }
	}

	void Veneer::addStyle(VGlobalStyle* s) {
		if (s)
            _styles.push_back(s);
        else
            std::cout << "No style to add\n";
	}

	void Veneer::removeStyle(unsigned int styleIndex) {
        if (styleIndex >= _styles.size() || styleIndex < 0)
            std::cerr << "the entered style index is not within the styleVec range\n";
        else {
            // set the iterator to the desired style
            constStyleIt _it = stylesBegin();
            for (unsigned int i = 0; i < styleIndex; i++)
                ++_it;
             // remove the desired style from the styleVec
            _styles.erase(_it);
        }
	}

	void Veneer::setStyles(const styleVec& sv) {
		_styles = sv;
	}

	const Veneer::styleVec& Veneer::getStyles() const {
		return _styles;
	}

	const size_t Veneer::getNumStyles() const {
		return _styles.size();
	}

	VGlobalStyle* Veneer::findStyleById(const std::string& id) {
		// search in the styles to find the desired one
		for (constStyleIt i = stylesBegin(); i != stylesEnd(); ++i) {
			if (stringCompare((*i)->getId(), id))
				return *i;
		}
		// not found
        return NULL;
	}

    unsigned int Veneer::findStyleIndexById(const std::string& id) {
        unsigned int _styleIndex = 0;

        // search in the styles to find the desired one
        for (constStyleIt i = stylesBegin(); i != stylesEnd(); ++i) {
            if (stringCompare((*i)->getId(), id))
                return _styleIndex;
            ++_styleIndex;
        }
        // not found
        return -1;
    }

	VGlobalStyle* Veneer::findStyleByRoleListMember(const std::string& role) {
		// search in the styles to find the desired one
		for (constStyleIt i = stylesBegin(); i != stylesEnd(); ++i) {
			VGlobalStyle* s = *i;
            
			// search in the role list of the style to find the desired role
			for (VGlobalStyle::constRoleListIt j = s->roleListBegin(); j != s->roleListEnd(); ++j) {
				if (stringCompare(*j, role))
					return s;
			}
		}
		// not found
        return NULL;
	}

    unsigned int Veneer::findStyleIndexByRoleListMember(const std::string& role) {
        unsigned int _styleIndex = 0;
        
        // search in the styles to find the desired one
        for (constStyleIt i = stylesBegin(); i != stylesEnd(); ++i) {
            VGlobalStyle* s = *i;
            
            // search in the role list of the style to find the desired role
            for (VGlobalStyle::constRoleListIt j = s->roleListBegin(); j != s->roleListEnd(); ++j) {
                if (stringCompare(*j, role))
                    return _styleIndex;
            }
            ++_styleIndex;
        }
        // not found
        return -1;
    }

	VGlobalStyle* Veneer::findStyleByTypeListMember(const std::string& type) {
		// search in the styles to find the desired one
		for (constStyleIt i = stylesBegin(); i != stylesEnd(); ++i) {
			VGlobalStyle* s = *i;

			// search in the type list of the style to find the desired type
			for (VGlobalStyle::constTypeListIt j = s->typeListBegin(); j != s->typeListEnd(); ++j) {
				if (stringCompare(*j, type))
					return s;
			}
		}
		// not found
        return NULL;
	}

    unsigned int Veneer::findStyleIndexByTypeListMember(const std::string& type) {
        unsigned int _styleIndex = 0;
        
        // search in the styles to find the desired one
        for (constStyleIt i = stylesBegin(); i != stylesEnd(); ++i) {
            VGlobalStyle* s = *i;
            
            // search in the type list of the style to find the desired role
            for (VGlobalStyle::constTypeListIt j = s->typeListBegin(); j != s->typeListEnd(); ++j) {
                if (stringCompare(*j, type))
                    return _styleIndex;
            }
            ++_styleIndex;
        }
        // not found
        return -1;
    }

	VLocalStyle* Veneer::findStyleByIdListMember(const std::string& id) {
		// search in the styles to find the desired one
		for (constStyleIt i = stylesBegin(); i != stylesEnd(); ++i) {
			VGlobalStyle* s = *i;
            
            if (s->isLocalStyle()) {
                VLocalStyle* ls = NULL;
                
                // cast the style to a local style
                try {
                     ls = dynamic_cast<VLocalStyle*>(s);
                }
                catch(std::bad_cast) {
                     std::cerr << "Unable to cast the style to local style\n";
                }
                
                // search in the id list of the style to find the desired id
                for (VLocalStyle::constIdListIt j = ls->idListBegin(); j != ls->idListEnd(); ++j) {
                    if (stringCompare(*j, id)) {
                        return ls;
                    }
                }
            }
		}
		// not found
        return NULL;
	}

    unsigned int Veneer::findStyleIndexByIdListMember(const std::string& id) {
        unsigned int _styleIndex = 0;
        
        // search in the styles to find the desired one
        for (constStyleIt i = stylesBegin(); i != stylesEnd(); ++i) {
            VGlobalStyle* s = *i;
            
            if (s->isLocalStyle()) {
                VLocalStyle* ls = NULL;
                
                // cast the style to a local style
                try {
                     ls = dynamic_cast<VLocalStyle*>(s);
                }
                catch(std::bad_cast) {
                     std::cout << "Unable to cast the style to local style\n";
                }
                
                // search in the id list of the style to find the desired id
                for (VLocalStyle::constIdListIt j = ls->idListBegin(); j != ls->idListEnd(); ++j) {
                    if (stringCompare(*j, id)) {
                        return _styleIndex;
                    }
                }
            }
            ++_styleIndex;
        }
        // not found
        return -1;
    }

	std::string Veneer::getStyleUniqueId() {
		std::size_t k = 0;
		VGlobalStyle* s = NULL;

		// create a new style id and search in the exisiting ones to ensure it has not already used
        while (1) {
            ++k;
            std::stringstream ss;
            ss << "Style_" << k;
            s = findStyleById(ss.str());
            if (!s)
                return ss.str();
        }
	}

	void Veneer::addLineEnding(VLineEnding* l) {
		if (l)
            _lineEndings.push_back(l);
        else
            std::cout << "No line ending to add\n";
	}

	void Veneer::removeLineEnding(unsigned int lineEndingIndex) {
        if (lineEndingIndex >= _lineEndings.size() || lineEndingIndex < 0)
            std::cerr << "the entered line ending index is not within the lineEndingVec range\n";
        else {
            // set the iterator to the desired line ending
            constLineEndingIt _it = lineEndingsBegin();
            for (int i = 0; i < lineEndingIndex; ++i)
                ++_it;
            // remove the desired line ending from the lineEndingVec
            _lineEndings.erase(_it);
        }
	}

	void Veneer::setLineEndings(const lineEndingVec& lv) {
		_lineEndings = lv;
	}

	const Veneer::lineEndingVec& Veneer::getLineEndings() const {
		return _lineEndings;
	}

	const size_t Veneer::getNumLineEndings() const {
		return _lineEndings.size();
	}

	VLineEnding* Veneer::findLineEndingById(const std::string& id) {
		// search in the line endings to find the desired one
		for (constLineEndingIt i = lineEndingsBegin(); i != lineEndingsEnd(); ++i) {
			if ((stringCompare((*i)->getId(), id)))
				return *i;
		}
		// not found
        return NULL;
	}

	unsigned int Veneer::findLineEndingIndexById(const std::string& id) {
		unsigned int _lineEndingIndex = 0;

		// search in the line endings to find the desired one
		for (constLineEndingIt i = lineEndingsBegin(); i != lineEndingsEnd(); ++i) {
			if ((stringCompare((*i)->getId(), id)))
				return _lineEndingIndex;
			++_lineEndingIndex;
		}
		// not found
        return -1;
	}

	std::string Veneer::getLineEndingUniqueId() {
		std::size_t k = 0;
		const VLineEnding* l = NULL;

		// create a new line ending id and search in the exisiting ones to ensure it has not already used
        while (1) {
            ++k;
            std::stringstream ss;
            ss << "LineEnding_" << k;
            l = findLineEndingById(ss.str());
            if (!l)
                return ss.str();
        }
	}

    std::string Veneer::getLineEndingUniqueId(const std::string& lineEndingId) {
        std::string previousLineEndingId = lineEndingId;
        std::size_t k = 0;
    
        while (findLineEndingById(previousLineEndingId)) {
            ++k;
            std::stringstream ss;
            ss << lineEndingId << "_" << k;
            previousLineEndingId = ss.str();
        }
        
        return previousLineEndingId;
    }

    void Veneer::setBackgroundColor(const std::string& bColor) {
        _backgroundColor = bColor;
        _isSetBackgroundColor = true;
    }
    
    const std::string& Veneer::getBackgroundColor() const {
        return _backgroundColor;
    }

    void Veneer::setRenderSpecified(bool value) {
        _isRenderSpecified = value;
    }

	//--CLASS VColorDefention--

	void VColorDefinition::setValue(const std::string& value) {
		_value = value;
		_isSetValue = true;
	}

	const std::string& VColorDefinition::getValue() const {
		return _value;
	}

	//--CLASS VGradientBase--

	void VGradientBase::setSpreadMehtod(const std::string& spreadMethod) {
		_spreadMethod = spreadMethod;
		_isSetSpreadMethod = true;
	}

	const std::string& VGradientBase::getSpreadMethod() const {
		return _spreadMethod;
	}

	void VGradientBase::addToStops(VGradientStop* stop) {
        if (stop) {
            _stops.push_back(stop);
            sortStops();
        }
        else
            std::cout << "No gradient stop to add\n";
	}

	void VGradientBase::removeStop(unsigned int stopIndex) {
        if (stopIndex >= _stops.size() || stopIndex < 0)
            std::cerr << "the entered gradient stop index is not within the stopVec range\n";
        else {
            // set the iterator to the desired stop
            constStopIt _it = stopsBegin();
            for (unsigned int i = 0; i < stopIndex; i++)
                ++_it;
            // remove the desired gradient stop from the stopVec
            _stops.erase(_it);
        }
	}

    void VGradientBase::setStops(const stopVec& sv) {
        _stops = sv;
        sortStops();
    }

	const VGradientBase::stopVec& VGradientBase::getStops() const {
		return _stops;
	}

    const size_t VGradientBase::getNumStops() const {
        return _stops.size();
    }

	VGradientStop* VGradientBase::findStopById(const std::string& id) {
        // search in the stops to find the desired one
		for (constStopIt i = stopsBegin(); i != stopsEnd(); ++i) {
			if (stringCompare((*i)->getId(), id))
				return *i;
		}
		// not found
        return NULL;
	}

    unsigned int VGradientBase::findStopIndexById(const std::string& id) {
        unsigned int _stopIndex = 0;

        // search in the stops to find the desired one
        for (constStopIt i = stopsBegin(); i != stopsEnd(); ++i) {
            if (stringCompare((*i)->getId(), id))
                return _stopIndex;
            ++_stopIndex;
        }
        // not found
        return -1;
    }

	VGradientStop* VGradientBase::findStopByColor(const std::string& sColor) {
		// search in the stops to find the desired one
		for (constStopIt i = stopsBegin(); i != stopsEnd(); ++i) {
			if (stringCompare((*i)->getStopColor(), sColor))
				return *i;
		}
		// not found
        return NULL;
	}

	unsigned int VGradientBase::findStopIndexByColor(const std::string& sColor) {
		unsigned int _stopIndex = 0;

		// search in the stops to find the desired one
		for (constStopIt i = stopsBegin(); i != stopsEnd(); ++i) {
			if (stringCompare((*i)->getStopColor(), sColor))
				return _stopIndex;
			++_stopIndex;
		}
		// not found
        return -1;
	}

	std::string VGradientBase::getStopUniqueId() {
		std::size_t k = 0;
		VGradientStop* s = NULL;
    
		// create a new gradient stop id and search in the exisiting ones to ensure it has not already used
        while (1) {
            ++k;
            std::stringstream ss;
            ss << "GradientStop_" << k;
            s = findStopById(ss.str());
            if (!s)
                return ss.str();
        }
	}

    bool compareStopsOffset(VGradientStop* s1, VGradientStop* s2) {
        return (s1->getOffset().r() <= s2->getOffset().r());
    }

    void VGradientBase::sortStops() {
        sort(_stops.begin(), _stops.end(), compareStopsOffset);
    }

	//--CLASS VLinearGradient--

	void VLinearGradient::setX1(const RAVector& r) {
		_x1 = r;
		_isSetX1 = true;
	}

	const RAVector& VLinearGradient::getX1() const {
		return _x1;
	}

	void VLinearGradient::setY1(const RAVector& r) {
		_y1 = r;
		_isSetY1 = true;
	}

	const RAVector& VLinearGradient::getY1() const {
		return _y1;
	}

	void VLinearGradient::setZ1(const RAVector& r) {
		_z1 = r;
		_isSetZ1 = true;
	}

	const RAVector& VLinearGradient::getZ1() const {
		return _z1;
	}

	void VLinearGradient::setX2(const RAVector& r) {
		_x2 = r;
		_isSetX2 = true;
	}

	const RAVector& VLinearGradient::getX2() const {
		return _x2;
	}

	void VLinearGradient::setY2(const RAVector& r) {
		_y2 = r;
		_isSetY2 = true;
	}

	const RAVector& VLinearGradient::getY2() const {
		return _y2;
	}

	void VLinearGradient::setZ2(const RAVector& r) {
		_z2 = r;
		_isSetZ2 = true;
	}

	const RAVector& VLinearGradient::getZ2() const {
		return _z2;
	}

	//--CLASS VRadialGradient--

	void VRadialGradient::setCx(const RAVector& r) {
		_cx = r;
		_isSetCx = true;
	}

	const RAVector& VRadialGradient::getCx() const {
		return _cx;
	}

	void VRadialGradient::setCy(const RAVector& r) {
		_cy = r;
		_isSetCy = true;
	}

	const RAVector& VRadialGradient::getCy() const {
		return _cy;
	}

	void VRadialGradient::setCz(const RAVector& r) {
		_cz = r;
		_isSetCz = true;
	}

	const RAVector& VRadialGradient::getCz() const {
		return _cz;
	}

	void VRadialGradient::setFx(const RAVector& r) {
		_fx = r;
		_isSetFx = true;
	}

	const RAVector& VRadialGradient::getFx() const {
		return _fx;
	}

	void VRadialGradient::setFy(const RAVector& r) {
		_fy = r;
		_isSetFy = true;
	}

	const RAVector& VRadialGradient::getFy() const {
		return _fy;
	}

	void VRadialGradient::setFz(const RAVector& r) {
		_fz = r;
		_isSetFz = true;
	}

	const RAVector& VRadialGradient::getFz() const {
		return _fz;
	}

	void VRadialGradient::setR(const RAVector& r) {
		_r = r;
		_isSetR = true;
	}

	const RAVector& VRadialGradient::getR() const {
		return _r;
	}

	//--CLASS VTransformation2D--

	RenderGroupElementShape VTransformation2D::getShape() {
		return _shape;
	}

	std::string VTransformation2D::getShapeAsString() {
		return rGroupElementToStr(_shape);
	}

    void VTransformation2D::setTransform(const double& a, const double& b, const double& c, const double& d, const double& e, const double& f) {
        std::stringstream ss;
        ss << a << ", " << b << ", " << c << ", " << d << ", " << e << ", " << f;
        _transform = ss.str();
        setAffine2D(a, b, c, d, e, f);
        _isSetTransform = true;
    }

    void VTransformation2D::setTransform(const double m[6]) {
        std::stringstream ss;
        ss << m[0] << ", " << m[1] << ", " << m[2] << ", " << m[3] << ", " << m[4] << ", " << m[5];
        _transform = ss.str();
        setAffine2D(m[0], m[1], m[2], m[3], m[4], m[5]);
        _isSetTransform = true;
    }

    void VTransformation2D::updateTransform(unsigned int index, const double& value) {
        std::string::iterator it;
        it = _transform.begin();
        for (int i = 0; i < index; i++)
            it += 3;
        _transform.replace(it, it + 1, std::to_string(value));
        updateAffine2D(index / 3, index % 3, value);
        _isSetTransform = true;
    }

    void VTransformation2D::updateTransform() {
        setTransform(_affine.element(0, 0), _affine.element(0, 1), _affine.element(0, 2), _affine.element(1, 0), _affine.element(1, 1), _affine.element(1, 2));
        _isSetTransform = true;
    }

	const std::string& VTransformation2D::getTransform() const {
		return _transform;
	}

    const void VTransformation2D::getTransform(double* m) const {
        delete [] m;
        m = new double[6];
        m[0] = _affine.element(0, 0);
        m[1] = _affine.element(0, 1);
        m[2] = _affine.element(0, 2);
        m[3] = _affine.element(1, 0);
        m[4] = _affine.element(1, 1);
        m[5] = _affine.element(1, 2);
    }

    void VTransformation2D::setAffine2D(const double& a, const double& b, const double& c, const double& d, const double& e, const double& f) {
        _affine = Affine2d(a , b, c, d, e, f, 0.0, 0.0, 1.0);
        _isSetTransform = true;
    }

    void VTransformation2D::updateAffine2D(const int& r, const int& c, const double& value) {
        _affine.setElement(r, c, value);
        _isSetTransform = true;
    }

    const Affine2d& VTransformation2D::getAffine2D() const {
        return _affine;
    }

    void VTransformation2D::translate(const double& tx, const double& ty) {
        Affine2d transform(1.0, 0.0, tx, 0.0, 1.0, ty, 0.0, 0.0, 1.0);
        _affine = transform * _affine;
        updateTransform();
    }

    void VTransformation2D::scale(const double& sx, const double& sy) {
        Affine2d scale(sx, 0.0, 0.0, 0.0, sy, 0.0, 0.0, 0.0, 1.0);
        _affine = scale * _affine;
        updateTransform();
    }

    void VTransformation2D::rotate(const double& alpha) {
        Affine2d rotate(std::cos(alpha), -1 * std::sin(alpha), 0.0, std::sin(alpha), std::cos(alpha), 0.0, 0.0, 0.0, 1.0);
        _affine = rotate * _affine;
        updateTransform();
    }

    void VTransformation2D::skewX(const double& alpha) {
        Affine2d skew(1.0, std::tan(alpha), 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0);
        _affine = skew * _affine;
        updateTransform();
    }

    void VTransformation2D::skewY(const double& beta) {
        Affine2d skew(1.0, 0.0, 0.0, std::tan(beta), 1.0, 0.0, 0.0, 0.0, 1.0);
        _affine = skew * _affine;
        updateTransform();
    }

	//--CLASS VGraphicalPrimitive1D--

	void VGraphicalPrimitive1D::setStroke(const std::string& stroke) {
		_stroke = stroke;
		_isSetStroke = true;
	}

	const std::string& VGraphicalPrimitive1D::getStroke() const {
		return _stroke;
	}

	void VGraphicalPrimitive1D::setStrokeWidth(const double& sWidth) {
		_strokeWidth = sWidth;
		_isSetStrokeWidth = true;
	}

	const double& VGraphicalPrimitive1D::getStrokeWidth() const {
		return _strokeWidth;
	}

	void VGraphicalPrimitive1D::addToDashArrayEnd(const unsigned int& element) {
		_dashArray.push_back(element);
		_isSetDashArray = true;
	}

	void VGraphicalPrimitive1D::addToDashArray(const unsigned int& element, unsigned int index) {
		_dashArray.insert(_dashArray.begin() + index, element);
		_isSetDashArray = true;
	}

	void VGraphicalPrimitive1D::replaceElementInDashArray(const unsigned int& element, unsigned int index) {
		_dashArray.erase(_dashArray.begin() + index);
		_dashArray.insert(_dashArray.begin() + index, element);
	}

	void VGraphicalPrimitive1D::removeElementFromDashArray(unsigned int index) {
		_dashArray.erase(_dashArray.begin() + index);
		if (_dashArray.size() == 0)
			_isSetDashArray = false;
	}

    void VGraphicalPrimitive1D::setDashArray(const dashArrayVec& dashArray) {
        _dashArray = dashArray;
        if (_dashArray.size())
            _isSetDashArray = true;
        else
            _isSetDashArray = false;
    }

	const VGraphicalPrimitive1D::dashArrayVec& VGraphicalPrimitive1D::getDashArray() const {
		return _dashArray;
	}

	//--CLASS VGraphicalPrimitive2D--

	void VGraphicalPrimitive2D::setFill(const std::string& fill) {
		_fill = fill;
		_isSetFill = true;
	}

	const std::string& VGraphicalPrimitive2D::getFill() const {
		return _fill;
	}

	void VGraphicalPrimitive2D::setFillRule(const std::string& fillRule) {
		_fillRule = fillRule;
		_isSetFillRule = true;
	}

	const std::string& VGraphicalPrimitive2D::getFillRule() const {
		return _fillRule;
	}

	//--CLASS VImage--

	void VImage::setX(const RAVector& x) {
		_x = x;
		_isSetX = true;
	}

	const RAVector& VImage::getX() const {
		return _x;
	}

	void VImage::setY(const RAVector& y) {
		_y = y;
		_isSetY = true;
	}

	const RAVector& VImage::getY() const {
		return _y;
	}

	void VImage::setZ(const RAVector& z) {
		_z = z;
		_isSetZ = true;
	}

	const RAVector& VImage::getZ() const {
		return _z;
	}

	void VImage::setWidth(const RAVector& width) {
		_width = width;
		_isSetWidth = true;
	}

	const RAVector& VImage::getWidth() const {
		return _width;
	}

	void VImage::setHeight(const RAVector& height) {
		_height = height;
		_isSetHeight = true;
	}

	const RAVector& VImage::getHeight() const {
		return _height;
	}

	void VImage::setHref(const std::string& href) {
		_href = href;
		_isSetHref = true;
	}

	const std::string& VImage::getHref() const {
		return _href;
	}

	//--CLASS RCurve--

	void RCurve::setStartHead(const std::string& startHead) {
		_startHead = startHead;
		_isSetStartHead = true;
	}

	const std::string& RCurve::getStartHead() const {
		return _startHead;
	}

	void RCurve::setEndHead(const std::string& endHead) {
		_endHead = endHead;
		_isSetEndHead = true;
	}

	const std::string& RCurve::getEndHead() const {
		return _endHead;
	}

	void RCurve::addToListOfElementsEnd(RenPoint* rp) {
        if (rp) {
            if (_listOfElements.size() == 0) {
                if (!rp->isRenderCubicBezier())
                    _listOfElements.push_back(rp);
            }
            else
                _listOfElements.push_back(rp);
        }
        else
            std::cout << "No render point to add\n";
	}

	void RCurve::addToListOfElements(RenPoint* rp, unsigned int index) {
        if (rp) {
            if (index == 0) {
                if (!rp->isRenderCubicBezier())
                    _listOfElements.insert(_listOfElements.begin() + index, rp);
            }
            else
                _listOfElements.insert(_listOfElements.begin() + index, rp);
        }
        else
            std::cout << "No render point to add\n";
	}

	void RCurve::removeFromListOfElements(unsigned int index) {
        constElementIt _it = elementsBegin();
        
        if (index == 0) {
            if (!_listOfElements.at(1)->isRenderCubicBezier())
                _listOfElements.erase(_it);
        }
        else {
            for (int i = 0; i < index; ++i)
                ++_it;
            _listOfElements.erase(_it);
        }
	}

	void RCurve::clearListOfElements() {
		_listOfElements.clear();
	}

	const RCurve::listOfElements& RCurve::getListOfElements() const {
		return _listOfElements;
	}

	const size_t RCurve::getNumElements() const {
		return _listOfElements.size();
	}

	RenPoint* RCurve::findElementById(const std::string& id) {
		// search in the elements to find the one with the same id
		for (constElementIt i = elementsBegin(); i != elementsEnd(); ++i) {
			if (stringCompare((*i)->getId(), id))
				return *i;
		}
		// not found
        return NULL;
	}

	unsigned int RCurve::findElementIndexById(const std::string& id) {
		unsigned int _elementIndex = 0;

		// search in the elements to find the one with the same id
		for (constElementIt i = elementsBegin(); i != elementsEnd(); ++i) {
			if (stringCompare((*i)->getId(), id))
				return _elementIndex;
			++_elementIndex;
		}
		// not found
        return -1;
	}

	std::string RCurve::getElementUniqueId() {
        size_t k = 0;
        RenPoint* rp = NULL;

        // create a new render point id and search in the exisiting ones to ensure it has not already used
        while (1) {
            ++k;
            std::stringstream ss;
            ss << "RenderPoint_" << k;
            rp = findElementById(ss.str());
            if (!rp)
                return ss.str();
        }
	}

	//--CLASS VText--
	void VText::setX(const RAVector& x) {
		_x = x;
		_isSetX = true;
	}

	const RAVector& VText::getX() const {
		return _x;
	}

	void VText::setY(const RAVector& y) {
		_y = y;
		_isSetY = true;
	}

	const RAVector& VText::getY() const {
		return _y;
	}

	void VText::setZ(const RAVector& z) {
		_z = z;
		_isSetZ = true;
	}

	const RAVector& VText::getZ() const {
		return _z;
	}

	void VText::setFontSize(const RAVector& fSize) {
        if (fSize.a() > 0.000001)
            _fontSize.setA(fSize.a());
        else
            _fontSize.setR(fSize.r());
		_isSetFontSize = true;
	}

	const RAVector& VText::getFontSize() const {
		return _fontSize;
	}

	void VText::setFontFamily(const std::string& fFamily) {
		_fontFamily = fFamily;
		_isSetFontFamily = true;
	}

	const std::string& VText::getFontFamily() const {
		return _fontFamily;
	}

	void VText::setFontWeight(const std::string& fWeight) {
		_fontWeight = fWeight;
		_isSetFontWeight = true;
	}

	const std::string& VText::getFontWeight() const {
		return _fontWeight;
	}

	void VText::setFontStyle(const std::string& fStyle) {
		_fontStyle = fStyle;
		_isSetFontStyle = true;
	}

	const std::string& VText::getFontStyle() const {
		return _fontStyle;
	}

	void VText::setHTextAnchor(const std::string& hTAnchor) {
		_hTextAnchor = hTAnchor;
		_isSetHTextAnchor = true;
	}

	const std::string& VText::getHTextAnchor() const {
		return _hTextAnchor;
	}

	void VText::setVTextAnchor(const std::string& vTAnchor) {
		_vTextAnchor = vTAnchor;
		_isSetVTextAnchor = true;
	}

	const std::string& VText::getVTextAnchor() const {
		return _vTextAnchor;
	}

	//--CLASS VRectangle--
	void VRectangle::setX(const RAVector& x) {
		_x = x;
		_isSetX = true;
	}

	const RAVector& VRectangle::getX() const {
		return _x;
	}

	void VRectangle::setY(const RAVector& y) {
		_y = y;
		_isSetY = true;
	}

	const RAVector& VRectangle::getY() const {
		return _y;
	}

	void VRectangle::setZ(const RAVector& z) {
		_z = z;
		_isSetZ = true;
	}

	const RAVector& VRectangle::getZ() const {
		return _z;
	}

	void VRectangle::setWidth(const RAVector& width) {
		_width = width;
		_isSetWidth = true;
	}

	const RAVector& VRectangle::getWidth() const {
		return _width;
	}

	void VRectangle::setHeight(const RAVector& height) {
		_height = height;
		_isSetHeight = true;
	}

	const RAVector& VRectangle::getHeight() const {
		return _height;
	}

	void VRectangle::setRX(const RAVector& rx) {
		_rx = rx;
		_isSetRX = true;
	}

	const RAVector& VRectangle::getRX() const {
		return _rx;
	}

	void VRectangle::setRY(const RAVector& ry) {
		_ry = ry;
		_isSetRY = true;
	}

	const RAVector& VRectangle::getRY() const {
		return _ry;
	}

	void VRectangle::setRatio(const double& ratio) {
		_ratio = ratio;
		_isSetRatio = true;
	}

    void VRectangle::unSetRatio() {
        _ratio = 0.0;
        _isSetRatio = false;
    }

	const double& VRectangle::getRatio() const {
		return _ratio;
	}

	//--CLASS VEllipse--
	void VEllipse::setCX(const RAVector& cx) {
		_cx = cx;
		_isSetCX = true;
	}

	const RAVector& VEllipse::getCX() const {
		return _cx;
	}

	void VEllipse::setCY(const RAVector& cy) {
		_cy = cy;
		_isSetCY = true;
	}

	const RAVector& VEllipse::getCY() const {
		return _cy;
	}

	void VEllipse::setCZ(const RAVector& cz) {
		_cz = cz;
		_isSetCZ = true;
	}

	const RAVector& VEllipse::getCZ() const {
		return _cz;
	}

	void VEllipse::setRX(const RAVector& rx) {
		_rx = rx;
		_isSetRX = true;
	}

	const RAVector& VEllipse::getRX() const {
		return _rx;
	}

	void VEllipse::setRY(const RAVector& ry) {
		_ry = ry;
		_isSetRY = true;
	}

	const RAVector& VEllipse::getRY() const {
		return _ry;
	}

	void VEllipse::setRatio(const double& ratio) {
		_ratio = ratio;
		_isSetRatio = true;
	}

    void VEllipse::unSetRatio() {
        _ratio = 0.0;
        _isSetRatio = false;
    }

	const double& VEllipse::getRatio() const {
		return _ratio;
	}

	//--CLASS VPolygon--
	void VPolygon::addToListOfElementsEnd(RenPoint* rp) {
		if (rp)
            _listOfElements.push_back(rp);
        else
            std::cout << "No render point to add\n";
	}

	void VPolygon::addToListOfElements(RenPoint* rp, unsigned int index) {
		if (rp)
            _listOfElements.insert(_listOfElements.begin() + index, rp);
        else
            std::cout << "No render point to add\n";
	}

	void  VPolygon::removeFromListOfElements(unsigned int index) {
		constElementIt _it = elementsBegin();
		for (int i = 0; i < index; ++i)
			++_it;
		_listOfElements.erase(_it);
	}

	void VPolygon::clearListOfElements() {
		_listOfElements.clear();
	}

	const VPolygon::listOfElements& VPolygon::getListOfElements() const {
		return _listOfElements;
	}

	const size_t VPolygon::getNumElements() const {
		return _listOfElements.size();
	}

	RenPoint* VPolygon::findElementById(const std::string& id) {
        // search in the elements to find the desired one
		for (constElementIt i = elementsBegin(); i != elementsEnd(); ++i) {
			if (stringCompare((*i)->getId(), id))
				return *i;
		}
		// not found
        return NULL;
	}

	unsigned int VPolygon::findElementIndexById(const std::string& id) {
		unsigned int _elementIndex = 0;

		// search in the elements to find the desired one
		for (constElementIt i = elementsBegin(); i != elementsEnd(); ++i) {
			if (stringCompare((*i)->getId(), id)) {
				return _elementIndex;
			}
			++_elementIndex;
		}
		// not found
        return -1;
	}

	std::string VPolygon::getElementUniqueId() {
        size_t k = 0;
        RenPoint* rp = NULL;

        // create a new render point id and search in the exisiting ones to ensure it has not already used
        while (1) {
            ++k;
            std::stringstream ss;
            ss << "RenderPoint_" << k;
            rp = findElementById(ss.str());
            if (!rp)
                return ss.str();
        }
	}

	//--CLASS VRenderGroup--
	void VRenderGroup::setStartHead(const std::string& startHead) {
		_startHead = startHead;
		_isSetStartHead = true;
	}

    void VRenderGroup::unSetStartHead() {
        _startHead = "";
        _isSetStartHead = false;
    }

	const std::string& VRenderGroup::getStartHead() const {
		return _startHead;
	}

	void VRenderGroup::setEndHead(const std::string& endHead) {
		_endHead = endHead;
		_isSetEndHead = true;
	}

    void VRenderGroup::unSetEndHead() {
        _endHead = "";
        _isSetEndHead = false;
    }

	const std::string& VRenderGroup::getEndHead() const {
		return _endHead;
	}

	void VRenderGroup::setFontSize(const RAVector& fSize) {
        if (fSize.a() > 0.000001) {
            _fontSize.setR(0.);
            _fontSize.setA(fSize.a());
        }
        else {
            _fontSize.setA(0.);
            _fontSize.setR(fSize.r());
        }
        _isSetFontSize = true;
	}

	const RAVector& VRenderGroup::getFontSize() const {
		return _fontSize;
	}

	void VRenderGroup::setFontFamily(const std::string& fFamily) {
		_fontFamily = fFamily;
		_isSetFontFamily = true;
	}

	const std::string& VRenderGroup::getFontFamily() const {
		return _fontFamily;
	}

	void VRenderGroup::setFontWeight(const std::string& fWeight) {
		_fontWeight = fWeight;
		_isSetFontWeight = true;
	}

	const std::string& VRenderGroup::getFontWeight() const {
		return _fontWeight;
	}

	void VRenderGroup::setFontStyle(const std::string& fStyle) {
		_fontStyle = fStyle;
		_isSetFontStyle = true;
	}

	const std::string& VRenderGroup::getFontStyle() const {
		return _fontStyle;
	}

	void VRenderGroup::setHTextAnchor(const std::string& hTAnchor) {
		_hTextAnchor = hTAnchor;
		_isSetHTextAnchor = true;
	}

	const std::string& VRenderGroup::getHTextAnchor() const {
		return _hTextAnchor;
	}

	void VRenderGroup::setVTextAnchor(const std::string& vTAnchor) {
		_vTextAnchor = vTAnchor;
		_isSetVTextAnchor = true;
	}

	const std::string& VRenderGroup::getVTextAnchor() const {
		return _vTextAnchor;
	}

	void VRenderGroup::addToListOfElements(VTransformation2D* t) {
        if (t) {
            _listOfElements.push_back(t);
            _isSetListOfElements = true;
        }
        else
            std::cout << "No graphical shape to add\n";
	}

	void VRenderGroup::removeFromListOfElements(unsigned int index) {
		constElementIt _it = elementsBegin();
		for (int i = 0; i < index; ++i)
			++_it;
		_listOfElements.erase(_it);
	}

	void VRenderGroup::clearListOfElements() {
		_listOfElements.clear();
		_isSetListOfElements = false;
	}

	const VRenderGroup::listOfElements& VRenderGroup::getListOfElements() const {
		return _listOfElements;
	}

    void VRenderGroup::setListOfElements(const listOfElements& ev) {
        _listOfElements = ev;
        _isSetListOfElements = true;
    }

	const size_t VRenderGroup::getNumElements() const {
		return _listOfElements.size();
	}

	VTransformation2D* VRenderGroup::findElementById(const std::string& id) {
		// search in the graphical shapes to find the desired one
		for (constElementIt i = elementsBegin(); i != elementsEnd(); ++i) {
			if (stringCompare((*i)->getId(), id))
				return *i;
		}
		// not found
        return NULL;
	}

	unsigned int VRenderGroup::findElementIndexById(const std::string& id) {
		unsigned int _elementIndex = 0;

		// search in the graphical shapes to find the desired one
		for (constElementIt i = elementsBegin(); i != elementsEnd(); ++i) {
			if (stringCompare((*i)->getId(), id))
				return _elementIndex;
			++_elementIndex;
        }
		// not found
        return -1;
	}

	std::string VRenderGroup::getElementUniqueId() {
        std::size_t k = 0;
        VTransformation2D* t2d = NULL;

        // create a new graphical shape id and search in the exisiting ones to ensure it has not already used
        while (1) {
            ++k;
            std::stringstream ss;
            ss << "GeometricShape_" << k;
            t2d = findElementById(ss.str());
            if (!t2d)
                return ss.str();
        }
	}

    void VRenderGroup::extractTextFeatures(const VRenderGroup& g) {
        // set font size
        if (g.isSetFontSize())
            setFontSize(g.getFontSize());
        
        // set font size
        if (g.isSetFontFamily())
            setFontFamily(g.getFontFamily());
        
        // set font weight
        if (g.isSetFontWeight())
            setFontWeight(g.getFontWeight());
        
        // set font style
        if (g.isSetFontStyle())
            setFontStyle(g.getFontStyle());
        
        // set htext anchor
        if (g.isSetHTextAnchor())
            setHTextAnchor(g.getHTextAnchor());
        
        // set vtext anchor
        if (g.isSetVTextAnchor())
            setVTextAnchor(g.getVTextAnchor());
    }

    VRenderGroup VRenderGroup::operator = (const VRenderGroup& g) {
        // set id
        if (g.isSetId())
            setId(g.getId());
        
        // set name
        if (g.isSetName())
            setName(g.getName());
        
        // set transform
        if (g.isSetTransform()) {
            double* m;
            g.getTransform(m);
            setTransform(m);
        }
        
        // set stroke color
        if (g.isSetStroke())
            setStroke(g.getStroke());
        
        // set stroke width
        if (g.isSetStrokeWidth())
            setStrokeWidth(g.getStrokeWidth());
        
        // set dash array
        if (g.isSetDashArray())
            setDashArray(g.getDashArray());
        
        // set fill color
        if (g.isSetFill())
            setFill(g.getFill());
        
        // set fill rule
        if (g.isSetFillRule())
            setFillRule(g.getFillRule());
        
        // set start head
        if (g.isSetStartHead())
            setStartHead(g.getStartHead());
        
        // set end head
        if (g.isSetEndHead())
            setEndHead(g.getEndHead());
        
        // set font size
        if (g.isSetFontSize())
            setFontSize(g.getFontSize());
        
        // set font size
        if (g.isSetFontFamily())
            setFontFamily(g.getFontFamily());
        
        // set font weight
        if (g.isSetFontWeight())
            setFontWeight(g.getFontWeight());
        
        // set font style
        if (g.isSetFontStyle())
            setFontStyle(g.getFontStyle());
        
        // set htext anchor
        if (g.isSetHTextAnchor())
            setHTextAnchor(g.getHTextAnchor());
        
        // set vtext anchor
        if (g.isSetVTextAnchor())
            setVTextAnchor(g.getVTextAnchor());
        
        // set elements
        if (g.getNumElements())
            setListOfElements(g.getListOfElements());

        return* this;
    }

	//--CLASS VGlobalStyle--
	void VGlobalStyle::setRoleList(const roleListSet& roleList) {
		_roleList = roleList;
		_isSetRoleList = true;
	}

	void VGlobalStyle::addToRoleList(const std::string& role) {
		_roleList.insert(role);
		_isSetRoleList = true;
	}

	void VGlobalStyle::removeFromRoleList(const std::string& role) {
        // search in role list to find the desired one
        for (constRoleListIt i = roleListBegin(); i != roleListEnd(); ++i) {
            if (stringCompare(*i, role))
                _roleList.erase(i);
        }
		if (_roleList.size() == 0)
			_isSetRoleList = false;
	}

	void VGlobalStyle::clearRoleList() {
		_roleList.clear();
		_isSetRoleList = false;
	}

	const VGlobalStyle::roleListSet& VGlobalStyle::getRoleList() const {
		return _roleList;
	}

	void VGlobalStyle::setTypeList(const typeListSet& typelist) {
		_typeList = typelist;
		_isSetTypeList = true;
	}

	void VGlobalStyle::addToTypeList(const std::string& type) {
		_typeList.insert(type);
		_isSetTypeList = true;
	}

	void VGlobalStyle::removeFromTypeList(const std::string& type) {
		// search in type list to find the desired one
        for (constTypeListIt i = typeListBegin(); i != typeListEnd(); ++i) {
            if (stringCompare(*i, type))
                _typeList.erase(i);
        }
        if (_typeList.size() == 0)
            _isSetTypeList = false;
	}

	void VGlobalStyle::clearTypeList() {
		_typeList.clear();
		_isSetTypeList = false;
	}

	const VGlobalStyle::typeListSet& VGlobalStyle::getTypeList() const {
		return _typeList;
	}

	void VGlobalStyle::setGroup(VRenderGroup* g) {
		_g = g;
		_isSetGroup = true;
	}

	VRenderGroup* VGlobalStyle::getGroup() {
		return _g;
	}

    //--CLASS VLocalStyle--
    void VLocalStyle::setIdList(const idListSet& idList) {
        _idList = idList;
        _isSetIdList = true;
    }

    void VLocalStyle::addToIdList(const std::string& id) {
        _idList.insert(id);
        _isSetIdList = true;
    }

    void VLocalStyle::removeFromIdList(const std::string& id) {
        // search in id list to find the desired one
        for (constIdListIt i = idListBegin(); i != idListEnd(); ++i) {
            if (stringCompare(*i, id))
                _idList.erase(i);
        }
        if (_idList.size() == 0)
            _isSetIdList = false;
    }

    void VLocalStyle::clearIdList() {
        _idList.clear();
        _isSetIdList = false;
    }

    const VLocalStyle::idListSet& VLocalStyle::getIdList() const {
        return _idList;
    }

	//--CLASS VLineEnding--
	void VLineEnding::setBox(LBox* b) {
		_box = b;
		_isSetBox = true;
	}

	LBox* VLineEnding::getBox() {
		return _box;
	}

	void VLineEnding::setGroup(VRenderGroup* g) {
		_g = g;
		_isSetGroup = true;
	}

	VRenderGroup* VLineEnding::getGroup() {
		return _g;
	}

	void VLineEnding::setEnableRotationMapping(const bool& enable) {
		_enableRotationMapping = enable;
        _isSetEnableRotationMapping = true;
	}

    const bool VLineEnding::getEnableRotationMapping() const {
        return _enableRotationMapping;
    }


    void VLineEnding::setAsLocalLineEnding(const bool& isLocal) {
        _isLocalLineEnding = isLocal;
    }

	//--CLASS VGradientStop--
	void VGradientStop::setOffset(const RAVector& offset) {
        _offset = offset;
		_isSetOffset = true;
	}

	const RAVector& VGradientStop::getOffset() const {
		return _offset;
	}

	void VGradientStop::setStopColor(const std::string& color) {
		_stopColor = color;
		_isSetStopColor = true;
	}

	const std::string& VGradientStop::getStopColor() const {
		return _stopColor;
	}

	// --Class RenPoint
	void RenPoint::setRPoint(const RPoint& rp) {
		_r = rp;
		_isSetRPoint = true;
	}

	const RPoint& RenPoint::getRPoint() const {
		return _r;
	}

	//--CLASS RCubicBezier--
	void RCubicBezier::setBasePoint1(const RPoint& rp) {
		_basePoint1 = rp;
		_isSetBasePoint1 = true;
	}

	const RPoint& RCubicBezier::getBasePoint1() const {
		return _basePoint1;
	}

	void RCubicBezier::setBasePoint2(const RPoint& rp) {
		_basePoint2 = rp;
		_isSetBasePoint2 = true;
	}

	const RPoint& RCubicBezier::getBasePoint2() const {
		return _basePoint2;
	}
}
