#include "sbne/sbml/ne_layout.h"

namespace sbne {

void populateLineInfo(LLineSegment* lLineSegment, LineSegment* lineSegment) {
    // set id
    if (lLineSegment->isSetId())
        lineSegment->setId(lLineSegment->getId());
    
    // set start and end point
    lineSegment->setStart(lLineSegment->getStart().x(), lLineSegment->getStart().y(), lLineSegment->getStart().z());
    
    lineSegment->setEnd(lLineSegment->getEnd().x(), lLineSegment->getEnd().y(), lLineSegment->getEnd().z());
    
    // set basepoint 1 and basepoint 2
    if (lLineSegment->isCubicBezier()) {
        ((CubicBezier*)lineSegment)->setBasePoint1(((LCubicBezier*)lLineSegment)->getBasePoint1().x(), ((LCubicBezier*)lLineSegment)->getBasePoint1().y(), ((LCubicBezier*)lLineSegment)->getBasePoint1().z());
        ((CubicBezier*)lineSegment)->setBasePoint2(((LCubicBezier*)lLineSegment)->getBasePoint2().x(), ((LCubicBezier*)lLineSegment)->getBasePoint2().y(), ((LCubicBezier*)lLineSegment)->getBasePoint2().z());
    }
}

// SBMLDocument

LayoutInfo* ne_doc_processLayoutInfo(SBMLDocument* doc) {
    LayoutInfo* l = new LayoutInfo();
    
    if (doc) {
        Network* net = NULL;
            
        // get the model from SBMLDocument
        Model* mod = doc->getModel();
        
        if (mod) {
            // get network from model
            net = new Network();
            net = networkFromModel(*mod, net);
            
            // get the layout plugin from model
            SBasePlugin* layoutBase = mod->getPlugin("layout");
            if (layoutBase) {
                LayoutModelPlugin* layoutPlugin = NULL;
                
                // cast sbase to the layout model plugin
                try {
                    layoutPlugin = dynamic_cast<LayoutModelPlugin*>(layoutBase); }
                catch(std::bad_cast) {
                    std::cerr << "Unable to get layout information\n"; }
                
                // have more than one layout
                if (layoutPlugin->getNumLayouts() > 1)
                    std::cout <<"Warning: Multiple layouts. Using first" << std::endl;
                
                // get the network from layout information
                const Layout* layout = layoutPlugin->getLayout(0);
                if (layout)
                    net = networkFromLayout(*layout, net);
            }
        }
        else
            std::cerr << "Failed to load model\n";
            
        // get sbml level
        l->level = doc->getLevel();
            
        // get sbml version
        l->version = doc->getVersion();
            
        // get network
        l->net = net;
    }
    else
        std::cerr << "Failed to load document\n";
    
    return l;
}

SBMLDocument* ne_doc_populateSBMLdocWithLayoutInfo(SBMLDocument* doc ,LayoutInfo* l) {
    if (doc && l) {
        // get the model from SBML
        Model* model = doc->getModel();
        
        // get the layout plugin from the model
        SBasePlugin* layoutBase = model->getPlugin("layout");
        
        // if render package is not enabled
        if (!layoutBase) {
            if (doc->getLevel() == 2)
                doc->enablePackage(LayoutExtension::getXmlnsL2(), "layout",  true);
            else if (doc->getLevel() == 3)
                doc->enablePackage(LayoutExtension::getXmlnsL3V1V1(), "layout",  true);
            layoutBase = model->getPlugin("layout");
        }
        
        // cast sbase to the layout plugin
        LayoutModelPlugin* layoutPlugin = NULL;
        try {
            layoutPlugin = dynamic_cast<LayoutModelPlugin*>(layoutBase); }
        catch(std::bad_cast) {
            std::cerr << "Unable to get layout information"; }
        
        // remove the existing layout plugins
        while (layoutPlugin->getNumLayouts())
            layoutPlugin->removeLayout(0);
        
        // add one layout element
        Layout* layout = layoutPlugin->createLayout();
            
        Network* net = (Network*)l->net;
        
        // add the network info to layout info
        if(net) {
            // set id
            if (net->isSetId())
                layout->setId(net->getId());
            else
                layout->setId("SBNE_Layout");
                
            // set dimensions
            Dimensions* dimensinos = new Dimensions();
            if (net->isSetWidth())
                dimensinos->setWidth(net->getWidth());
            else
                dimensinos->setWidth(1024.0);
            if (net->isSetHeight())
                dimensinos->setHeight(net->getHeight());
            else
                dimensinos->setHeight(1024.0);
            layout->setDimensions(dimensinos);
            
            // add compartments glyphs
            CompartmentGlyph* cg = NULL;
            for(constCompartmentIt i = net->compartmentsBegin(); i != net->compartmentsEnd(); ++i) {
                NCompartment* c = *i;
                
                if (c->isSetGlyphId() && !stringCompare(c->getGlyphId(), "default_Compartment_Glyph")) {
                    // create a compartmnet glyph
                    cg = layout->createCompartmentGlyph();
                    
                    // set id
                    cg->setId(c->getGlyphId());
                    
                    // set compartment
                    if (c->isSetId())
                        cg->setCompartmentId(c->getId());
                    
                    // set order
                    if (c->isSetOrder())
                        cg->setOrder(c->getOrder());
                    
                    // set bounding box
                    if (c->isSetBox()) {
                        BoundingBox bb;
                        bb.setX(c->getBox()->x());
                        bb.setY(c->getBox()->y());
                        bb.setWidth(c->getBox()->width());
                        bb.setHeight(c->getBox()->height());
                        if (c->getBox()->isSetId())
                            bb.setId(c->getBox()->getId());
                        cg->setBoundingBox(&bb);
                    }
                }
            }
            
            // add species glyphs
            SpeciesGlyph* sg = NULL;
            for(constSpeciesIt i = net->speciesBegin(); i != net->speciesEnd(); ++i) {
                NSpecies* s = *i;
                
                if (s->isSetGlyphId()) {
                    // create a species glyph
                    sg = layout->createSpeciesGlyph();
                    
                    // set id
                    sg->setId(s->getGlyphId());
                    
                    // set species
                    if (s->isSetId())
                        sg->setSpeciesId(s->getId());
                    
                    // set bounding box
                    if (s->isSetBox()) {
                        BoundingBox bb;
                        bb.setX(s->getBox()->x());
                        bb.setY(s->getBox()->y());
                        bb.setWidth(s->getBox()->width());
                        bb.setHeight(s->getBox()->height());
                        if (s->getBox()->isSetId())
                            bb.setId(s->getBox()->getId());
                        sg->setBoundingBox(&bb);
                    }
                }
            }
            
            // add reaction glyphs
            ReactionGlyph* rg = NULL;
            for(constReactionIt i = net->reactionsBegin(); i != net->reactionsEnd(); ++i) {
                NReaction* r = *i;
                
                if (r->isSetGlyphId()) {
                    // create a reaction glyph
                    rg = layout->createReactionGlyph();
                    
                    // set id
                    rg->setId(r->getGlyphId());
                    
                    // set reaction
                    if (r->isSetId())
                        rg->setReactionId(r->getId());
                    
                    // set bounding box
                    if (r->isSetBox()) {
                        BoundingBox bb;
                        bb.setX(r->getBox()->x());
                        bb.setY(r->getBox()->y());
                        bb.setWidth(r->getBox()->width());
                        bb.setHeight(r->getBox()->height());
                        if (r->getBox()->isSetId())
                            bb.setId(r->getBox()->getId());
                        rg->setBoundingBox(&bb);
                    }
                    
                    // set curve
                    if (r->isSetCurve()) {
                        const LCurve* lCurve = r->getCurve();
                        
                        // get curve elements
                        LineSegment* lineSegment = NULL;
                        for(LCurve::constElementIt j = lCurve->elementsBegin(); j != lCurve->elementsEnd(); ++j) {
                            
                            if ((*j)->isCubicBezier())
                                lineSegment = rg->createCubicBezier();
                            else
                                lineSegment = rg->createLineSegment();
                                
                            // set network line info to layout line info
                            populateLineInfo(*j, lineSegment);
                        }
                    }
                    
                    // add species references
                    SpeciesReferenceGlyph* srg = NULL;
                    for(constSReferenceIt j = r->sReferencesBegin(); j != r->sReferencesEnd(); ++j) {
                        NSpeciesReference* sr = *j;
                        
                        // for species references glyph
                        if (sr->isSetGlyphId()) {
                            // create a reaction glyph
                            srg = rg->createSpeciesReferenceGlyph();
                            
                            // set id
                            srg->setId(sr->getGlyphId());
                            
                            // set species reference
                            if (sr->isSetId())
                                srg->setSpeciesReferenceId(sr->getId());
                            
                            // set species glyph
                            if (sr->getSpecies())
                                if (sr->getSpecies()->isSetGlyphId())
                                    srg->setSpeciesGlyphId(sr->getSpecies()->getGlyphId());
                                    
                            
                            // set role
                            if (sr->isSetRole())
                                srg->setRole(rxnRoleToString(sr->getRole()));
                            
                            // set bounding box
                            if (sr->isSetBox()) {
                                BoundingBox bb;
                                bb.setX(sr->getBox()->x());
                                bb.setY(sr->getBox()->y());
                                bb.setWidth(sr->getBox()->width());
                                bb.setHeight(sr->getBox()->height());
                                if (sr->getBox()->isSetId())
                                    bb.setId(sr->getBox()->getId());
                                srg->setBoundingBox(&bb);
                            }
                            
                            // set species reference curve
                            if (sr->isSetCurve()) {
                                const LCurve* lCurve = sr->getCurve();
                                
                                // get curve elements
                                LineSegment* lineSegment = NULL;
                                for(LCurve::constElementIt k = lCurve->elementsBegin(); k != lCurve->elementsEnd(); ++k) {
                                    
                                    if ((*k)->isCubicBezier())
                                        lineSegment = srg->createCubicBezier();
                                    else
                                        lineSegment = srg->createLineSegment();
                                        
                                    // set network line info to layout line info
                                    populateLineInfo(*k, lineSegment);
                                }
                            }
                        }
                    }
                }
            }
            
            // add text glyphs
            TextGlyph* tg = NULL;
            for(Network::constTextIt i = net->textsBegin(); i != net->textsEnd(); ++i) {
                NText* t = *i;
                
                if (t->isSetGlyphId()) {
                    // create a text glyph
                    tg = layout->createTextGlyph();
                    
                    // set id
                    tg->setId(t->getGlyphId());
                    
                    // set bounding box
                    if (t->isSetBox()) {
                        BoundingBox bb;
                        bb.setX(t->getBox()->x());
                        bb.setY(t->getBox()->y());
                        bb.setWidth(t->getBox()->width());
                        bb.setHeight(t->getBox()->height());
                        if (t->getBox()->isSetId())
                            bb.setId(t->getBox()->getId());
                        tg->setBoundingBox(&bb);
                    }
                    
                    // set graphical object
                    if (t->isSetGraphicalObjectId())
                        tg->setGraphicalObjectId(t->getGraphicalObjectId());
                    
                    // set text
                    if (t->isSetText())
                        tg->setText(t->getText());
                    
                    // set origin of text
                    if (t->isSetOriginOfTextId())
                        tg->setOriginOfTextId(t->getOriginOfTextId());
                }
            }
            
            // add text glyphs
            GraphicalObject* gO = NULL;
            for(Network::constGObjectIt i = net->gObjectsBegin(); i != net->gObjectsEnd(); ++i) {
                NGraphicalObject* o = *i;
                
                if (o->isSetGlyphId()) {
                    // create a graphical object
                    gO = layout->createAdditionalGraphicalObject();
                    
                    // set id
                    gO->setId(o->getGlyphId());
                    
                    // set bounding box
                    if (o->isSetBox()) {
                        BoundingBox bb;
                        bb.setX(o->getBox()->x());
                        bb.setY(o->getBox()->y());
                        bb.setWidth(o->getBox()->width());
                        bb.setHeight(o->getBox()->height());
                        if (o->getBox()->isSetId())
                            bb.setId(o->getBox()->getId());
                        gO->setBoundingBox(&bb);
                    }
                }
            }
        }
        else
            std::cerr << "No network is assgiend to the layoutinfo";
    }
    else
        std::cerr << "Failed to load either document or layout info \n";
    
    return doc;
}

// LayoutInfo

Network* ne_li_getNetwork(LayoutInfo* l) {
    if (l->net)
        return (Network*)(l->net);
    
    return NULL;
}

int ne_li_setNetwork(LayoutInfo* l, Network* net) {
    if (net) {
        l->net = net;
        return 0;
    }
    
    return -1;
}

Network* ne_li_addLayoutFeaturesToNetowrk(LayoutInfo* l) {
    // get network
    Network* net = (Network*)l->net;
    if (net) {
        // set id
        net->setId("SBNE_Layout");
        
        // set default dimensions
        net->setWidth(300.0);
        net->setHeight(300.0);
        
        // compartments
        NCompartment* c = NULL;
        bool isReactionSetCompartment = true;
        
        // check to see if the reactions are assigned to their compartment
        for (constReactionIt rIt = net->reactionsBegin(); rIt != net->reactionsEnd(); ++rIt) {
            if (!(*rIt)->isSetCompartment() && net->getNumCompartments() != 1)
                isReactionSetCompartment = false;
        }
        
        // if no compartment exists, add a temperoray default one
        if (net->getNumCompartments() == 0 || !isReactionSetCompartment) {
            c = new NCompartment();
            c->setId("default_Compartment");
            net->addCompartment(c);
        }
        
        // set comparments glyphs and features
        for (constCompartmentIt it= net->compartmentsBegin(); it != net->compartmentsEnd(); ++it) {
            c = *it;
            
            if (isReactionSetCompartment || stringCompare(c->getId(), "default_Compartment")) {
                c->setMatchWithGlyph(true);
                c->setUsed(true);
                
                // set id
                c->setGlyphId(net->getCompartmentUniqueGlyphId(c->getId()));
                
                // set the intial bounding box of the compartment
                c->setBox(new LBox(0.0, 0.0, 150.0, 150.0));
                
                // set the glyphs of reactions, species, and species references placed in this compartment
                locateCompartmentItems(net, c);
            }
        }
        
        // find the position of the compartments in the network and set the dimensions of the network
        packCompartmentsIntoNetwork(net);
        
        // set layout specified
        net->setLayoutSpecified(true);
    }
    else
        std::cout << "No network is assigned to the layoutinfo\n";
    
    return net;
}

// NetworkElement

bool ne_ne_isSetId(NetworkElement* nE) {
    if (nE)
        return nE->isSetId();
    
    return false;
}

const std::string ne_ne_getId(NetworkElement* nE) {
    if (nE)
        return nE->getId();
    
    return std::string();
}

bool ne_ne_isSetName(NetworkElement* nE) {
    if (nE)
        return nE->isSetName();
    
    return false;
}

const std::string ne_ne_getName(NetworkElement* nE) {
    if (nE)
        return nE->getName();
    
    return std::string();
}

// Network

bool ne_net_isLayoutSpecified(Network* net) {
    if (net)
        return net->isLayoutSpecified();
    
    return false;
}

const size_t ne_net_getNumCompartments(Network* net) {
    if (net)
        return net->getNumCompartments();
    
    return 0;
}

NCompartment* ne_net_getCompartment(Network* net, const unsigned int& index) {
    if (net) {
        if (index >= 0 && index < int(net->getNumCompartments()))
            return net->getCompartments().at(index);
    }
    
    return NULL;
}

compartmentVec ne_net_getCompartments(Network* net) {
    if (net)
        return net->getCompartments();
    
    return compartmentVec(0);
}

const size_t ne_net_getNumSpecies(Network* net) {
    if (net)
        return net->getNumSpecies();
    
    return 0;
}

NSpecies* ne_net_getSpecies(Network* net, const unsigned int& index) {
    if (net) {
        if (index >= 0 && index < int(net->getNumSpecies()))
            return net->getSpecies().at(index);
    }
    
    return NULL;
}

speciesVec ne_net_getSpecies(Network* net) {
    if (net)
        return net->getSpecies();
    
    return speciesVec(0);
}

const size_t ne_net_getNumReactions(Network* net) {
    if (net)
        return net->getNumReactions();
    
    return 0;
}

NReaction* ne_net_getReaction(Network* net, const unsigned int& index) {
    if (net) {
        if (index >= 0 && index < int(net->getNumReactions()))
            return net->getReactions().at(index);
    }
    
    return NULL;
}

reactionVec ne_net_getReactions(Network* net) {
    if (net)
        return net->getReactions();
    
    return reactionVec(0);
}

NText* ne_net_getText(Network* net, NGraphicalObject* gO) {
    NText* text = NULL;
    if (net && gO) {
        text = net->findTextByGraphicalObjectId(gO->getGlyphId());
        if (!text) {
            // create a text
            text = new NText();
            
            // set glyphid
            text->setGlyphId(net->getTextUniqueGlyphId());
            
            // set bounding box
            if (gO->isSetBox()) {
                LBox* bbx = new LBox(gO->getBox()->x(), gO->getBox()->y(), gO->getBox()->width(), gO->getBox()->height());
                text->setBox(bbx);
            }
            
            // set graphical object
            text->setGraphicalObjectId(gO->getGlyphId());
            
            // set text of graphical object
            gO->setText(text);
            
            // add the text to veneer
            net->addText(text);
        }
    }
    
    return text;
}

// NGraphicalObject

bool ne_go_isSetGlyphId(NGraphicalObject* gO) {
    if (gO)
        return gO->isSetGlyphId();
    
    return false;
}

const std::string ne_go_getGlyphId(NGraphicalObject* gO) {
    if (gO)
        return gO->getGlyphId();
    
    return std::string();
}

int ne_go_setGlyphId(Network* net, NGraphicalObject* gO, const std::string& gOId) {
    if (net && gO) {
        switch (gO->getType()) {
            case 0: {
                constCompartmentIt cIt;
                for (cIt = net->compartmentsBegin(); cIt != net->compartmentsEnd(); ++cIt) {
                    if (stringCompare((*cIt)->getGlyphId(), gOId)) {
                        std::cerr << " The enetered Id is assgiend to another compartment glyph\n";
                        return -1;
                    }
                }
                
                if (cIt == net->compartmentsEnd()) {
                    gO->setGlyphId(gOId);
                    return 0;
                }
            }
                
                break;
                
            case 1: {
                constSpeciesIt sIt;
                for (sIt = net->speciesBegin(); sIt != net->speciesEnd(); ++sIt) {
                    if (stringCompare((*sIt)->getGlyphId(), gOId)) {
                        std::cerr << " The enetered Id is assgiend to another species glyph\n";
                        return -1;
                    }
                }
                
                if (sIt == net->speciesEnd()) {
                    gO->setGlyphId(gOId);
                    return 0;
                }
            }
                
                break;
                
            case 2: {
                constReactionIt rIt;
                for (rIt = net->reactionsBegin(); rIt != net->reactionsEnd(); ++rIt) {
                    if (stringCompare((*rIt)->getGlyphId(), gOId)) {
                        std::cerr << " The enetered Id is assgiend to another reaction glyph\n";
                        return -1;
                    }
                }
                
                if (rIt == net->reactionsEnd()) {
                    gO->setGlyphId(gOId);
                    return 0;
                }
            }
                
                break;
                
            case 3: {
                NReaction* r = NULL;
                if (((NSpeciesReference*)gO)->isSetReaction())
                    r = ((NSpeciesReference*)gO)->getReaction();
                
                if (r) {
                    constSReferenceIt sRIt;
                    
                    for (sRIt = r->sReferencesBegin(); sRIt != r->sReferencesEnd(); ++sRIt) {
                        if (stringCompare((*sRIt)->getGlyphId(), gOId)) {
                            std::cerr << " The enetered Id is assgiend to another species reference glyph of its reaction\n";
                            return -1;
                        }
                    }
                    
                    if (sRIt == r->sReferencesEnd()) {
                        gO->setGlyphId(gOId);
                        return 0;
                    }
                }
            }
                
                break;
                
            case 4: {
                Network::constTextIt tIt;
                for (tIt = net->textsBegin(); tIt != net->textsEnd(); ++tIt) {
                    if (stringCompare((*tIt)->getGlyphId(), gOId)) {
                        std::cerr << " The enetered Id is assgiend to another text glyph\n";
                        return -1;
                    }
                }
                
                if (tIt == net->textsEnd()) {
                    gO->setGlyphId(gOId);
                    return 0;
                }
            }
                
                break;
                
            default:
                break;
        }
    }
    
    return -1;
}

bool ne_go_isSetText(NGraphicalObject* gO) {
    if (gO)
        return gO->isSetText();
    
    return false;
}

NText* ne_go_getText(NGraphicalObject* gO) {
    if (gO)
        return gO->getText();
    
    return NULL;
}

int ne_go_setText(NGraphicalObject* gO, NText* t) {
    if (gO && t) {
        gO->setText(t);
        return 0;
    }
    
    return -1;
}

bool ne_go_isSetBoundingBox(NGraphicalObject* gO) {
    if (gO)
        return gO->isSetBox();
    
    return false;
}

LBox* ne_go_getBoundingBox(NGraphicalObject* gO) {
    if (gO)
        return gO->getBox();
    
    return NULL;
}

int ne_go_setBoundingBox(NGraphicalObject* gO, LBox* b) {
    if (gO && b) {
        gO->setBox(b);
        return 0;
    }
    
    return -1;
}

int ne_go_unSetBoundingBox(NGraphicalObject* gO, const bool& remove) {
    if (gO) {
        gO->unSetBox(remove);
        return 0;
    }
    
    return -1;
}

const GrphObjType ne_go_getType(NGraphicalObject* gO) {
    if (gO)
        return gO->getType();
    
    return NUM_GrphObjType;
}

// NSpecies

int ne_spc_updateBoundingBox(NSpecies* s, const double& positionX, const double& positionY, const double& dimensionWidth, const double& dimensionHeight) {
    if (s) {
        if (s->isSetBox()) {
            s->getBox()->setX(positionX);
            s->getBox()->setY(positionY);
            s->getBox()->setWidth(dimensionWidth);
            s->getBox()->setHeight(dimensionHeight);
            

            for (NSpecies::constRMemberIt rMIt = s->rMembersBegin(); rMIt != s->rMembersEnd(); ++rMIt) {
                if ((*rMIt)->reaction) {
                    for (constSReferenceIt sRIt = (*rMIt)->reaction->sReferencesBegin(); sRIt != (*rMIt)->reaction->sReferencesEnd(); ++sRIt) {
                        if ((*sRIt)->isSetSpecies() && (*sRIt)->getSpecies()->isSetId() && stringCompare((*sRIt)->getSpecies()->getId(), s->getId()) && (*sRIt)->isSetCurve()) {
                            LCurve* curve = (*sRIt)->getCurve();
                            LCurve::constElementIt eIt;
                            LPoint boxPoint;
                            SpcSide speciesSide;
                            
                            switch((*sRIt)->getQuadrant()) {
                                case Quad_I_1:
                                case Quad_IV_2:
                                    speciesSide = LEFT_SIDE;
                                    break;
                                
                                case Quad_I_2:
                                case Quad_II_1:
                                    speciesSide = BOTTOM_SIDE;
                                    break;
                                    
                                case Quad_II_2:
                                case Quad_III_1:
                                    speciesSide = RIGHT_SIDE;
                                    break;
                                    
                                case Quad_III_2:
                                case Quad_IV_1:
                                    speciesSide = TOP_SIDE;
                                    break;
                            }
                            
                            // get the species box point of species reference
                            boxPoint = getSReferenceSpeciesBoxPoint(s, *sRIt, speciesSide);
                            
                            // for products
                            if ((*sRIt)->getRole() == 1 || (*sRIt)->getRole() == 3) {
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
                        }
                    }
                }
            }
            
            return 0;
        }
    }
    
    return -1;
}

bool ne_spc_isSetCompartment(NSpecies* s) {
    if (s)
        return s->isSetCompartment();
    
    return false;
}

const std::string ne_spc_getCompartment(NSpecies* s) {
    if (s)
        return s->getCompartment();
    
    return std::string();
}

// NReaction

const size_t ne_rxn_getNumSpeciesReferences(NReaction* r) {
    if (r)
        return r->getNumSpeciesReferences();
    
    return 0;
}

NSpeciesReference* ne_rxn_getSpeciesReference(NReaction* r, const unsigned int& index) {
    if (r) {
        if (index >= 0 && index < int(r->getNumSpeciesReferences()))
            return r->getSpeciesReferences().at(index);
    }
    
    return NULL;
}

SReferenceVec ne_rxn_getSpeciesReferences(NReaction* r) {
    if (r)
        return r->getSpeciesReferences();
    
    return SReferenceVec(0);
}

bool ne_rxn_isSetCurve(NReaction* r) {
    if (r)
        return r->isSetCurve();
    
    return false;
}

LCurve* ne_rxn_getCurve(NReaction* r) {
    if (r)
        return r->getCurve();
    
    return NULL;
}

int ne_rxn_setCurve(NReaction* r, LCurve* c) {
    if (r && c) {
        r->setCurve(c);
        return 0;
    }
    
    return -1;
}

int ne_rxn_unSetCurve(NReaction* r, const bool& remove) {
    if (r) {
        r->unSetCurve(remove);
        return 0;
    }
    
    return -1;
}

bool ne_rxn_isSetExtentBox(NReaction* r) {
    if (r)
        return r->isSetExtentBox();
    
    return false;
}

LBox* ne_rxn_getExtentBox(NReaction* r) {
    if (r) {
        LBox* b = new LBox(r->getExtentBox());
        return b;
    }
    
    return NULL;
}

// NSpeciesReference

bool ne_sr_isSetReaction(NSpeciesReference* sR) {
    if (sR)
        return sR->isSetReaction();
    
    return false;
}

NReaction* ne_sr_getReaction(NSpeciesReference* sR) {
    if (sR)
        return sR->getReaction();
    
    return NULL;
}

bool ne_sr_isSetSpecies(NSpeciesReference* sR) {
    if (sR)
        return sR->isSetReaction();
    
    return false;
}

NSpecies* ne_sr_getSpecies(NSpeciesReference* sR) {
    if (sR)
        return sR->getSpecies();
    
    return NULL;
}

bool ne_sr_isSetRole(NSpeciesReference* sR) {
    if (sR)
        return sR->isSetRole();
    
    return false;
}

const std::string ne_sr_getRoleAsString(NSpeciesReference* sR) {
    if (sR)
        return rxnRoleToString(sR->getRole());
    
    return std::string();
}

bool ne_sr_isSetCurve(NSpeciesReference* sR) {
    if (sR)
        return sR->isSetCurve();
    
    return false;
}

LCurve* ne_sr_getCurve(NSpeciesReference* sR) {
    if (sR)
        return sR->getCurve();
    
    return NULL;
}

// NText

bool ne_gtxt_isSetPlainText(NText* t) {
    if (t)
        return t->isSetText();
    
    return false;
}

const std::string ne_gtxt_getPlainText(NText* t) {
    if (t)
        return t->getText();
    
    return std::string();
}

int ne_gtxt_setPlainText(NText* t, const std::string& plainText) {
    if (t) {
        t->setText(plainText);
        return 0;
    }
    
    return -1;
}

int ne_gtxt_unSetPlainText(NText* t) {
    if (t) {
        t->unSetText();
        return 0;
    }
    
    return -1;
}

// LCurve

const size_t ne_crv_getNumElements(LCurve* c) {
    if (c)
        return c->getNumElements();
    
    return 0;
}

LLineSegment* ne_crv_getElement(LCurve* c, const unsigned int& index) {
    if (c) {
        if (index >= 0 && index < int(c->getNumElements()))
            return c->getListOfElements().at(index);
    }
    
    return NULL;
}

LCurve::listOfElements ne_crv_getElements(LCurve* c) {
    if (c)
        return c->getListOfElements();
    
    return LCurve::listOfElements(0);
}

LLineSegment* ne_crv_addElement(LCurve* c, const  int& index, LPoint* startpoint, LPoint* endpoint, LPoint* basepoint1,  LPoint* basepoint2) {
    LLineSegment* line = NULL;
    if (c && index >= -1 && index <= int(c->getNumElements() && startpoint && endpoint)) {
        if (basepoint1 && basepoint2) {
            line = new LCubicBezier();
            ((LCubicBezier*)line)->setBasePoint1(LPoint(basepoint1->x(), basepoint1->y()));
            ((LCubicBezier*)line)->setBasePoint2(LPoint(basepoint2->x(), basepoint2->y()));
        }
        else
            line = new LLineSegment();
        
        if (startpoint && endpoint) {
            line->setStart(LPoint(startpoint->x(), startpoint->y()));
            line->setEnd(LPoint(endpoint->x(), endpoint->y()));
        }
        
        if (index == -1)
            c->addToListOfElementsEnd(line);
        else
            c->addToListOfElements(line, index);
    }
    
    return line;
}

int ne_crv_removeElement(LCurve* c, const  int& index) {
    if (c) {
        if (0 <= index && index < int(c->getNumElements())) {
            c->removeFromListOfElements(index);
            
            return 0;
        }
    }
    
    return -1;
}

LBox* ne_crv_getExtentBox(LCurve* c) {
    if (c) {
        LBox* b = new LBox(c->getExtentBox());
        return b;
    }
    
    return NULL;
}

// LLineSegment

bool ne_ls_isSetStart(LLineSegment* l) {
    if (l)
        return l->isSetStart();
    
    return false;
}

LPoint* ne_ls_getStart(LLineSegment* l) {
    LPoint* point = NULL;
    if (l)
        point = new LPoint(l->getStart());
    
    return point;
}

int ne_ls_setStart(LLineSegment* l, LPoint* p) {
    if (l && p) {
        l->setStart(LPoint(p->x(), p->y()));
        return 0;
    }
    
    return -1;
}

bool ne_ls_isSetEnd(LLineSegment* l) {
    if (l)
        return l->isSetEnd();
    
    return false;
}

LPoint* ne_ls_getEnd(LLineSegment* l) {
    LPoint* point = NULL;
    if (l)
        point = new LPoint(l->getEnd());
    
    return point;
}

int ne_ls_setEnd(LLineSegment* l, LPoint* p) {
    if (l && p) {
        l->setEnd(LPoint(p->x(), p->y()));
        return 0;
    }
    
    return -1;
}

bool ne_ls_isCubicBezier(LLineSegment* l) {
    if (l)
        return l->isCubicBezier();
    
    return false;
}

// LCubicBezier

bool ne_cb_isSetBasePoint1(LLineSegment* l) {
    if (l && l->isCubicBezier())
        return ((LCubicBezier*)(l))->isSetBasePoint1();
    
    return false;
}

LPoint* ne_cb_getBasePoint1(LLineSegment* l) {
    LPoint* point = NULL;
    if (l && l->isCubicBezier())
        point = new LPoint(((LCubicBezier*)(l))->getBasePoint1());
    
    return point;
}

int ne_cb_setBasePoint1(LLineSegment* l, LPoint* p) {
    if (l && l->isCubicBezier() && p) {
        ((LCubicBezier*)(l))->setBasePoint1(LPoint(p->x(), p->y()));
        return 0;
    }
    
    return -1;
}

bool ne_cb_isSetBasePoint2(LLineSegment* l) {
    if (l && l->isCubicBezier())
        return ((LCubicBezier*)(l))->isSetBasePoint2();
    
    return false;
}

LPoint* ne_cb_getBasePoint2(LLineSegment* l) {
    LPoint* point = NULL;
    if (l && l->isCubicBezier())
        point = new LPoint(((LCubicBezier*)(l))->getBasePoint2());
    
    return point;
}

int ne_cb_setBasePoint2(LLineSegment* l, LPoint* p) {
    if (l && l->isCubicBezier() && p) {
        ((LCubicBezier*)(l))->setBasePoint2(LPoint(p->x(), p->y()));
        return 0;
    }
    
    return -1;
}

// LBox

const double ne_bb_getX(LBox* b) {
    if (b)
        return b->x();
    
    return 0.0;
}

int ne_bb_setX(LBox* b, const double& x) {
    if (b) {
        b->setX(x);
        return 0;
    }
    
    return -1;
}

const double ne_bb_getY(LBox* b) {
    if (b)
        return b->y();
    
    return 0.0;
}

int ne_bb_setY(LBox* b, const double& y) {
    if (b) {
        b->setY(y);
        return 0;
    }
    
    return -1;
}

const double ne_bb_getWidth(LBox* b) {
    if (b)
        return b->width();
    
    return 0.0;
}

int ne_bb_setWidth(LBox* b, const double& width) {
    if (b) {
        b->setWidth(width);
        return 0;
    }
    
    return -1;
}

const double ne_bb_getHeight(LBox* b) {
    if (b)
        return b->height();
    
    return 0.0;
}

int ne_bb_setHeight(LBox* b, const double& height) {
    if (b) {
        b->setHeight(height);
        return 0;
    }
    
    return -1;
}

// LPoint

const double ne_point_getX(LPoint* p) {
    if (p)
        return p->x();
    
    return 0.0;
}

int ne_point_setX(LPoint* p, const double& x) {
    if (p) {
        p->setX(x);
        return 0;
    }
    
    return -1;
}

const double ne_point_getY(LPoint* point) {
    if (point)
        return point->y();
    
    return 0.0;
}

int ne_point_setY(LPoint* p, const double& y) {
    if (p) {
        p->setY(y);
        return 0;
    }
    
    return -1;
}

}
