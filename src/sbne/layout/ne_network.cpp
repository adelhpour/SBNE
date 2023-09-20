#include "sbne/layout/ne_network.h"

namespace sbne {

std::string rxnRoleToString(RxnRoleType role) {
    switch(role) {
        case RXN_ROLE_SUBSTRATE:
            return "substrate";
        case RXN_ROLE_PRODUCT:
            return "product";
        case RXN_ROLE_SIDESUBSTRATE:
            return "side substrate";
        case RXN_ROLE_SIDEPRODUCT:
            return "side product";
        case RXN_ROLE_MODIFIER:
            return "modifier";
        case RXN_ROLE_ACTIVATOR:
            return "activator";
        case RXN_ROLE_INHIBITOR:
            return "inhibitor";
        default:
            std::cerr << "Unknown RxnRoleType\n";
            return std::string();
    }
}

RxnRoleType SBMLRoleToRxnRole(SpeciesReferenceRole_t role) {
    switch (role) {
        case SPECIES_ROLE_SUBSTRATE:
            return RXN_ROLE_SUBSTRATE;
        case SPECIES_ROLE_PRODUCT:
            return RXN_ROLE_PRODUCT;
        case SPECIES_ROLE_SIDESUBSTRATE:
            return RXN_ROLE_SIDESUBSTRATE;
        case SPECIES_ROLE_SIDEPRODUCT:
            return RXN_ROLE_SIDEPRODUCT;
        case SPECIES_ROLE_MODIFIER:
            return RXN_ROLE_MODIFIER;
        case SPECIES_ROLE_ACTIVATOR:
            return RXN_ROLE_ACTIVATOR;
        case SPECIES_ROLE_INHIBITOR:
            return RXN_ROLE_INHIBITOR;
        case SPECIES_ROLE_UNDEFINED:
            std::cout << "Cannot convert role SPECIES_ROLE_UNDEFINED" << std::endl;
        default:
            std::cout << " Unknown role\n";
            return RXN_ROLE_SUBSTRATE;
    }
}

CoordSysQuad angleToQuadrant(double angle) {
    while (angle < 0.0)
        angle += 2 * pi;
    while (angle > 2 * pi)
        angle -= 2 * pi;
    
    if (angle >= 0.0 && angle <= pi / 4.0)
        return Quad_I_1;
    else if (angle > pi / 4.0 && angle <= pi / 2.0)
        return Quad_I_2;
    else if (angle > pi / 2.0 && angle <= 3 * pi / 4.0)
        return Quad_II_1;
    else if (angle > 3 * pi / 4.0 && angle <= pi)
        return Quad_II_2;
    else if (angle > pi && angle <= 5 * pi / 4.0)
        return Quad_III_1;
    else if (angle > 5 * pi / 4.0 && angle <= 3 * pi / 2.0)
        return Quad_III_2;
    else if (angle > 3 * pi / 2.0 && angle <= 7 * pi / 4.0)
        return Quad_IV_1;
    else if (angle > 7 * pi / 4.0 && angle < 2 * pi)
        return Quad_IV_2;
    else
        return Quad_I_1;
}

Network* networkFromLayout(const Layout& layout, Network* net) {
    // get id
    if (layout.isSetId())
        net->setId(layout.getId());
                   
    // get name
    if (layout.isSetName())
        net->setName(layout.getName());
    
    // get dimensions
    net->setBox(0.0, 0.0, layout.getDimensions()->getWidth(), layout.getDimensions()->getHeight());
    
    // compartments
    for(int i = 0; i < layout.getNumCompartmentGlyphs(); ++i) {
        const CompartmentGlyph* cg = layout.getCompartmentGlyph(i);
        
        // find the model compartment
        NCompartment* c = net->findCompartmentById(cg->getCompartmentId());
        
        // if the model compartment exist
        if (c)
            c->setMatchWithGlyph(true);
        else {
            c = new NCompartment();
            c->setId(net->getCompartmentUniqueId());
            net->addCompartment(c);
        }
        
        // if compartment is used for another glyph
        if (c->isUsed()) {
            NCompartment* _c = c;
            c = new NCompartment();
            c->setId(_c->getId());
            if (_c->isSetName())
                c->setName(_c->getName());
            net->addCompartment(c);
        }
        c->setUsed(true);
        
        // get id
        if (cg->isSetId())
            c->setGlyphId(cg->getId());
        else
            c->setGlyphId(net->getCompartmentUniqueGlyphId(c->getId()));
        
        // get order
        if (cg->isSetOrder())
            c->setOrder(cg->getOrder());
        
        // get the compartment bounding box
        if (cg->getBoundingBox()) {
            LBox* b = new LBox();
            b->setX(cg->getBoundingBox()->x());
            b->setY(cg->getBoundingBox()->y());
            b->setWidth(cg->getBoundingBox()->width());
            b->setHeight(cg->getBoundingBox()->height());
            if (cg->getBoundingBox()->isSetId())
                b->setId(cg->getBoundingBox()->getId());
            c->setBox(b);
        }
        
        // get object role
        RenderGraphicalObjectPlugin* rgoPlugin = (RenderGraphicalObjectPlugin*) cg->getPlugin("render");
        if (rgoPlugin && rgoPlugin->isSetObjectRole())
            c->setObjectRole(rgoPlugin->getObjectRole());
    }
    
    // if no compartment glyph exists, add a temporary default one
    if (!layout.getNumCompartmentGlyphs()) {
        NCompartment* c = NULL;
        // find the model compartment
        if (net->getNumCompartments()) {
            c = net->getCompartments().at(0);
            c->setMatchWithGlyph(true);
        }
        else {
            c = new NCompartment();
            c->setId("default_Compartment");
            net->addCompartment(c);
        }
        
        // add reactions to the only compartment
        for (constReactionIt rIt = net->reactionsBegin(); rIt != net->reactionsEnd(); ++rIt)
            c->addReaction(*rIt);
        
        // set glyph id
        c->setGlyphId(c->getId() + "_Glyph");
        
        // set the compartment bounding box
        c->setBox(new LBox(net->getBox()));
    }
    
    // species
    for(int i = 0; i < layout.getNumSpeciesGlyphs(); ++i) {
        const SpeciesGlyph* sg = layout.getSpeciesGlyph(i);
        
        // find the model species
        NSpecies* s = net->findSpeciesById(sg->getSpeciesId());
        
        // if the model species exist
        if (s)
            s->setMatchWithGlyph(true);
        else {
            s = new NSpecies();
            s->setId(net->getSpeciesUniqueId());
            net->addSpecies(s);
        }
        
        // if species is used for another glyph
        if (s->isUsed()) {
            NSpecies* _s = s;
            s = new NSpecies();
            s->setId(_s->getId());
            if (_s->isSetName())
                s->setName(_s->getName());
            if (_s->isSetCompartment())
                s->setCompartment(_s->getCompartment());
            net->addSpecies(s);
        }
        s->setUsed(true);
            
        // get id
        if (sg->isSetId())
            s->setGlyphId(sg->getId());
        else
            s->setGlyphId(net->getSpeciesUniqueGlyphId(s->getId()));
        
        // get the species bounding box
        if (sg->getBoundingBox()) {
            LBox* b = new LBox();
            b->setX(sg->getBoundingBox()->x());
            b->setY(sg->getBoundingBox()->y());
            b->setWidth(sg->getBoundingBox()->width());
            b->setHeight(sg->getBoundingBox()->height());
            if (sg->getBoundingBox()->isSetId())
                b->setId(sg->getBoundingBox()->getId());
            s->setBox(b);
        }
        
        // get object role
        RenderGraphicalObjectPlugin* rgoPlugin = (RenderGraphicalObjectPlugin*) sg->getPlugin("render");
        if (rgoPlugin && rgoPlugin->isSetObjectRole())
            s->setObjectRole(rgoPlugin->getObjectRole());
    }

    // reactions
    for(int i = 0; i < layout.getNumReactionGlyphs(); ++i) {
        const ReactionGlyph* rg = layout.getReactionGlyph(i);
        
        // find the model reaction
        NReaction* r = net->findReactionById(rg->getReactionId());
        
        // if the model reaction exist
        if (r)
            r->setMatchWithGlyph(true);
        else {
            r = new NReaction();
            r->setId(net->getReactionUniqueId());
            net->addReaction(r);
        }
        
        // get id
        if (rg->isSetId())
            r->setGlyphId(rg->getId());
        else
            r->setGlyphId(r->getId() + "_Glyph");
        
        // get the reaction bounding box
        if (rg->getBoundingBox()) {
            LBox* b = new LBox();
            b->setX(rg->getBoundingBox()->x());
            b->setY(rg->getBoundingBox()->y());
            b->setWidth(rg->getBoundingBox()->width());
            b->setHeight(rg->getBoundingBox()->height());
            if (rg->getBoundingBox()->isSetId())
                b->setId(rg->getBoundingBox()->getId());
            r->setBox(b);
        }
        
        // get curve
        if (rg->isSetCurve()) {
            const Curve* curve = rg->getCurve();
            LCurve* c = new LCurve();
            LLineSegment* l = NULL;
            
            // get curve elements
            for (int j = 0; j < curve->getNumCurveSegments(); ++j)
                getLineInfo(c, curve->getCurveSegment(j));
            
            r->setCurve(c);
        }
        
        // get species references and their curves
        for(int j = 0; j < rg->getNumSpeciesReferenceGlyphs(); ++j) {
            const SpeciesReferenceGlyph* srg = rg->getSpeciesReferenceGlyph(j);
            NSpeciesReference* sr = NULL;
            
            // find the species glyph
            NSpecies* s = net->findSpeciesByGlyphId(srg->getSpeciesGlyphId());
            
            if (s) {
                // find the species reference glyph
                for (constSReferenceIt sRIt = r->sReferencesBegin(); sRIt < r->sReferencesEnd(); ++sRIt) {
                    if ((*sRIt)->isSetSpecies()) {
                        if ((*sRIt)->getSpecies()->getId() == s->getId()) {
                            (*sRIt)->setSpecies(r, s);
                            sr = *sRIt;
                            sr->setMatchWithGlyph(false);
                        }
                    }
                }
            }
            
            if (!sr) {
                sr = new NSpeciesReference();
                sr->setId(r->getSpeciesReferenceUniqueId());
                r->addSpeciesReference(sr);
            }
            
            // if species reference is used for another glyph
            if (sr->isUsed()) {
                NSpeciesReference* _sr = sr;
                sr = new NSpeciesReference();
                sr->setId(r->getSpeciesReferenceUniqueId());
                sr->setRole(_sr->getRole());
                if (_sr->isSetSpecies())
                    sr->setSpecies(_sr->getReaction(), _sr->getSpecies());
                r->addSpeciesReference(sr);
            }
            sr->setUsed(true);
                
            // get id
            if (srg->isSetId())
                sr->setGlyphId(srg->getId());
            else
                sr->setGlyphId(r->getSpeciesReferenceUniqueGlyphId(sr->getId()));
            
            // get the id of species reference glyph
            if (srg->isSetSpeciesReferenceId())
                sr->setId(srg->getSpeciesReferenceId());
            
            // get role
            if (srg->isSetRole())
                sr->setRole(SBMLRoleToRxnRole(srg->getRole()));
            
            // get the species reference bounding box
            if (srg->getBoundingBox()) {
                LBox* b = new LBox();
                b->setX(srg->getBoundingBox()->x());
                b->setY(srg->getBoundingBox()->y());
                b->setWidth(srg->getBoundingBox()->width());
                b->setHeight(srg->getBoundingBox()->height());
                if (srg->getBoundingBox()->isSetId())
                    b->setId(srg->getBoundingBox()->getId());
                sr->setBox(b);
            }

            // get species referneces curve
            if (srg->isSetCurve()) {
                const Curve* curve = srg->getCurve();
                LCurve* c = new LCurve();
                LLineSegment* l;
                
                // get curve elements
                for (int j = 0; j < curve->getNumCurveSegments(); ++j)
                    getLineInfo(c, curve->getCurveSegment(j));
                
                // set the species reference curve
                sr->setCurve(c);
            }
        }
        
        // get object role
        RenderGraphicalObjectPlugin* rgoPlugin = (RenderGraphicalObjectPlugin*) rg->getPlugin("render");
        if (rgoPlugin && rgoPlugin->isSetObjectRole())
            r->setObjectRole(rgoPlugin->getObjectRole());
    }
    
    // additional graphical objects
    for (int i = 0; i < layout.getNumAdditionalGraphicalObjects(); ++i) {
        const GraphicalObject* gO = layout.getAdditionalGraphicalObject(i);
        
        // create a network graphical object
        NGraphicalObject* o = new NGraphicalObject();
        net->addGObject(o);
        
        // get id
        if (gO->isSetId())
            o->setGlyphId(gO->getId());
        else
            o->setGlyphId(net->getGObjectUniqueGlyphId());
        
        // get the graphical object bounding box
        if (gO->getBoundingBox()) {
            LBox* b = new LBox();
            b->setX(gO->getBoundingBox()->x());
            b->setY(gO->getBoundingBox()->y());
            b->setWidth(gO->getBoundingBox()->width());
            b->setHeight(gO->getBoundingBox()->height());
            if (gO->getBoundingBox()->isSetId())
                b->setId(gO->getBoundingBox()->getId());
            o->setBox(b);
        }
    }
    
    // texts
    for (int i = 0; i < layout.getNumTextGlyphs(); ++i) {
        const TextGlyph* tg = layout.getTextGlyph(i);
        
        // create a network text
        NText* t = new NText();
        net->addText(t);
        
        // get id
        if (tg->isSetId())
            t->setGlyphId(tg->getId());
        else
            t->setGlyphId(net->getTextUniqueGlyphId());
        
        // get the text bounding box
        if (tg->getBoundingBox()) {
            LBox* b = new LBox();
            b->setX(tg->getBoundingBox()->x());
            b->setY(tg->getBoundingBox()->y());
            b->setWidth(tg->getBoundingBox()->width());
            b->setHeight(tg->getBoundingBox()->height());
            if (tg->getBoundingBox()->isSetId())
                b->setId(tg->getBoundingBox()->getId());
            t->setBox(b);
        }
        
        // get graphical object
        if (tg->isSetGraphicalObjectId()) {
            t->setGraphicalObjectId(tg->getGraphicalObjectId());
            NGraphicalObject* o = net->findNetworkElement(tg->getGraphicalObjectId());
            if (o && o->getType() != 4)
                o->addText(t);
        }
        
        // get origin of texts
        if (tg->isSetOriginOfTextId())
            t->setOriginOfTextId(tg->getOriginOfTextId());
        
        // get text
        if (tg->isSetText())
            t->setText(tg->getText());
    }
    
    // enable layout specified in network
    net->setLayoutSpecified(true);
    
    return net;
}

Network* networkFromModel(const Model& model, Network* net) {
    
    // add compartments
    for(int i = 0; i < model.getNumCompartments(); ++i) {
        const Compartment* comp = model.getCompartment(i);
        NCompartment* c = new NCompartment();
        
        // get id
        if (comp->isSetId())
            c->setId(comp->getId());
        else
            c->setId(net->getCompartmentUniqueId());
        
        // get name
        if (comp->isSetName())
            c->setName(comp->getName());
        
        // get metaid
        if (comp->isSetMetaId())
            c->setMetaId(comp->getMetaId());
        
        net->addCompartment(c);
    }
    
    // add species
    for(int i = 0; i < model.getNumSpecies(); ++i) {
        const Species* spc = model.getSpecies(i);
        NSpecies* s = new NSpecies();
        
        // get id
        if (spc->isSetId())
            s->setId(spc->getId());
        else
            s->setId(net->getSpeciesUniqueId());
        
        // get name
        if (spc->isSetName())
            s->setName(spc->getName());
        
        // get metaid
        if (spc->isSetMetaId())
            s->setMetaId(spc->getMetaId());
        
        // get compartment
        if (spc->isSetCompartment()) {
            s->setCompartment(spc->getCompartment());
            NCompartment* c = net->findCompartmentById(spc->getCompartment());
            c->addSpecies(s);
        }
        
        net->addSpecies(s);
    }
    
    // add reactions
    for(int i = 0; i < model.getNumReactions(); ++i) {
        const Reaction* rxn = model.getReaction(i);
        NReaction* r = new NReaction();
        
        // get id
        if (rxn->isSetId())
            r->setId(rxn->getId());
        else
            r->setId(net->getReactionUniqueId());
        
        // get name
        if (rxn->isSetName())
            r->setName(rxn->getName());
        
        // get metaid
        if (rxn->isSetMetaId())
            r->setMetaId(rxn->getMetaId());
        
        // get compartment
        if (rxn->isSetCompartment()) {
            r->setCompartment(rxn->getCompartment());
            NCompartment* c = net->findCompartmentById(rxn->getCompartment());
            c->addReaction(r);
        }
            
        // add reactans
        NSpecies* s;
        for(int j = 0; j < rxn->getNumReactants(); ++j) {
            const SpeciesReference* spc_r = rxn->getReactant(j);
            NSpeciesReference* sr = new NSpeciesReference();
            
            // get id
            if (spc_r->isSetId())
                sr->setId(spc_r->getId());
            else
                sr->setId(r->getSpeciesReferenceUniqueId());
            
            // get name
            if (spc_r->isSetName())
                sr->setName(spc_r->getName());
            
            // get metaid
            if (spc_r->isSetMetaId())
                sr->setMetaId(spc_r->getMetaId());
            
            // get the species reference role
            sr->setRole(RXN_ROLE_SUBSTRATE);
            
            // get the species reference species
            if (spc_r->isSetSpecies() && net->findSpeciesById(spc_r->getSpecies())) {
                s = net->findSpeciesById(spc_r->getSpecies());
                
                // set the species of species reference
                sr->setSpecies(r, s);
                
                // if the reaction does not contain the species
                if (!r->findSpeciesById(spc_r->getSpecies()))
                    r->addSpecies(s);
                
                // if the species is not already a participant in this reaction
                if (!s->findReactionMemberById(r->getId()))
                    s->addReactionMember(r);
            }
                
            r->addSpeciesReference(sr);
        }
        
        // add products
        for(int j = 0; j < rxn->getNumProducts(); ++j) {
            const SpeciesReference* spc_r = rxn->getProduct(j);
            NSpeciesReference* sr = new NSpeciesReference();
            
            // get id
            if (spc_r->isSetId())
                sr->setId(spc_r->getId());
            else
                sr->setId(r->getSpeciesReferenceUniqueId());
            
            // get name
            if (spc_r->isSetName())
                sr->setName(spc_r->getName());
            
            // get metaid
            if (spc_r->isSetMetaId())
                sr->setMetaId(spc_r->getMetaId());
            
            // set the species reference role
            sr->setRole(RXN_ROLE_PRODUCT);
            
            // get the species reference species
            if (spc_r->isSetSpecies() && net->findSpeciesById(spc_r->getSpecies())) {
                s = net->findSpeciesById(spc_r->getSpecies());
                
                // set the species of species reference
                sr->setSpecies(r, s);
                
                // if the reaction does not contain the species
                if (!r->findSpeciesById(spc_r->getSpecies()))
                    r->addSpecies(s);
                
                // if the species is not already a participant in this reaction
                if (!s->findReactionMemberById(r->getId()))
                    s->addReactionMember(r);
            }
            
            r->addSpeciesReference(sr);
        }
        
        // add modifiers
        for(int j = 0; j < rxn->getNumModifiers(); ++j) {
            const ModifierSpeciesReference* spc_r = rxn->getModifier(j);
            NSpeciesReference* sr = new NSpeciesReference();
            
            // get id
            if (spc_r->isSetId())
                sr->setId(spc_r->getId());
            else
                sr->setId(r->getSpeciesReferenceUniqueId());
            
            // get name
            if (spc_r->isSetName())
                sr->setName(spc_r->getName());
            
            // get metaid
            if (spc_r->isSetMetaId())
                sr->setMetaId(spc_r->getMetaId());
            
            // set the species reference role
            sr->setRole(RXN_ROLE_MODIFIER);
            
            // get the species reference species
            if (spc_r->isSetSpecies() && net->findSpeciesById(spc_r->getSpecies())) {
                s = net->findSpeciesById(spc_r->getSpecies());
                
                // set the species of species reference
                sr->setSpecies(r, s);
                
                // if the reaction does not contain the species
                if (!r->findSpeciesById(spc_r->getSpecies()))
                    r->addSpecies(s);
                
                // if the species is not already a participant in this reaction
                if (!s->findReactionMemberById(r->getId()))
                    s->addReactionMember(r);
            }
            
            r->addSpeciesReference(sr);
        }
        
        net->addReaction(r);
    }
    
    return net;
}

void getLineInfo(LCurve* c, const LineSegment* line) {
    const CubicBezier* bezier = dynamic_cast< const CubicBezier* >(line);
    LLineSegment* l = NULL;
    
    // if it is of type cubic bezier
    if (bezier) {
        l = new LCubicBezier();
        
        // get start point
        l->setStart(LPoint(bezier->getStart()->x(), bezier->getStart()->y()));
        
        // get end point
        l->setEnd(LPoint(bezier->getEnd()->x(), bezier->getEnd()->y()));
        
        // get basepoint 1
        ((LCubicBezier*)l)->setBasePoint1(LPoint(bezier->getBasePoint1()->x(), bezier->getBasePoint1()->y()));
        
        // get basepoint 2
        ((LCubicBezier*)l)->setBasePoint2(LPoint(bezier->getBasePoint2()->x(), bezier->getBasePoint2()->y()));
    }
    
    // if it is of type line segment
    else if (line) {
        l = new LLineSegment();
        
        // get start point
        l->setStart(LPoint(line->getStart()->x(), line->getStart()->y()));
        
        // get end point
        l->setEnd(LPoint(line->getEnd()->x(), line->getEnd()->y()));
    }
    
    // get id
    if (line->isSetId())
        l->setId(line->getId());
    else
        l->setId(c->getElementUniqueId());
    
    c->addToListOfElementsEnd(l);
}


//--CLASS NetworkElement--

NetworkElement::NetworkElement(const NetworkElement& nE) {
    _isSetId = false;
    _isSetName = false;
    _isSetMetaId = false;
    
    // id
    if (nE.isSetId())
        setId(getId());
    
    // name
    if (nE.isSetName())
        setName(nE.getName());
    
    // metaid
    if (nE.isSetMetaId())
        setMetaId(nE.getMetaId());
}

void NetworkElement::setId(const std::string& id) {
    _id = id;
    _isSetId = true;
}

const std::string& NetworkElement::getId() const {
    return _id;
}

void NetworkElement::setName(const std::string& name) {
    _name = name;
    _isSetName = true;
}

const std::string& NetworkElement::getName() const {
    return _name;
}

void NetworkElement::setMetaId(const std::string& metaid) {
    _metaid = metaid;
    _isSetMetaId = true;
}

const std::string& NetworkElement::getMetaId() const {
    return _metaid;
}

//--CLASS Network--

Network::Network(const Network& n) : NetworkElement(n) {
    _compartments.clear();
    _species.clear();
    _reactions.clear();
    _texts.clear();
    _gObjects.clear();
    _isSetBox = false;
    _isLayoutSpecified = false;
    
    // compartments
    if (n.getNumCompartments())
        setCompartments(n.getCompartments());
    
    // species
    if (n.getNumSpecies())
        setSpecies(n.getSpecies());
    
    // reactions
    if (n.getNumReactions())
        setReactions(n.getReactions());
    
    // texts
    if (n.getNumTexts())
        setTexts(n.getTexts());
    
    // graphcialobjects
    if (n.getNumGObjects())
        setGObjects(n.getGObjects());
}

void Network::addCompartment(NCompartment* c) {
    if (c)
        _compartments.push_back(c);
    else
        std::cout << "No compartment to add\n";
}

void Network::removeCompartment(int compartmentIndex) {
    if (compartmentIndex >= _compartments.size() || compartmentIndex < 0)
        std::cerr << "the entered compartment index is not within the compartmentVec range\n";
    else {
        // set the iterator to the desired compartment
        constCompartmentIt _it = compartmentsBegin();
        for (int i = 0; i < compartmentIndex; ++i)
            ++_it;
        // remove the desired compartment from the compartmentVec
        _compartments.erase(_it);
    }
}

void Network::setCompartments(const compartmentVec& cv) {
    _compartments = cv;
}

const compartmentVec& Network::getCompartments() const {
    return _compartments;
}

const size_t Network::getNumCompartments() const {
    return _compartments.size();
}

NCompartment* Network::findCompartmentById(const std::string& id) {
    // search in compartments to find the desired one
    for (constCompartmentIt i = compartmentsBegin(); i != compartmentsEnd(); ++i) {
        if (stringCompare((*i)->getId(), id))
            return *i;
    }
    // not found
    return NULL;
}

int Network::findCompartmentIndexById(const std::string& id) {
    int _compartmentIndex = 0;

    // search in compartments to find the desired one
    for (constCompartmentIt i = compartmentsBegin(); i != compartmentsEnd(); ++i) {
        if (stringCompare((*i)->getId(), id))
            return _compartmentIndex;
        ++_compartmentIndex;
    }
    // not found
    return -1;
}

NCompartment* Network::findCompartmentByGlyphId(const std::string& glyphId) {
    // search in compartments to find the desired one using its glyph id
    for (constCompartmentIt i = compartmentsBegin(); i != compartmentsEnd(); ++i) {
        if (stringCompare((*i)->getGlyphId(), glyphId))
            return *i;
    }
    // not found
    return NULL;
}

int Network::findCompartmentIndexByGlyphId(const std::string& glyphId) {
    int _compartmentIndex = 0;

    // search in compartments to find the desired one using its glyph id
    for (constCompartmentIt i = compartmentsBegin(); i != compartmentsEnd(); ++i) {
        if (stringCompare((*i)->getGlyphId(), glyphId))
            return _compartmentIndex;
        ++_compartmentIndex;
    }
    // not found
    return -1;
}

std::string Network::getCompartmentUniqueId() {
    size_t k = 0;
    NCompartment* c = NULL;

    // create a new compartment id and search in the exisiting ones to ensure it has not already used
    while (1) {
        ++k;
        std::stringstream ss;
        ss << "Compartment_" << k;
        c = findCompartmentById(ss.str());
        if (!c)
            return ss.str();
    }
}

std::string Network::getCompartmentUniqueGlyphId(const std::string& id) {
    size_t k = 0;
    NCompartment* c = NULL;

    // create a new compartment glyph id and search in the exisiting ones to ensure it has not already used
    while (1) {
        ++k;
        std::stringstream ss;
        ss << id << "_Glyph_" << k;
        c = findCompartmentByGlyphId(ss.str());
        if (!c)
            return ss.str();
    }
}

void Network::addSpecies(NSpecies* s) {
    if (s)
        _species.push_back(s);
    else
        std::cout << "No species to add to network\n";
}

void Network::removeSpecies(int speciesIndex) {
    if (speciesIndex >= _species.size() || speciesIndex < 0)
        std::cerr << "the entered species index is not within the speciesVec range\n";
    else {
        // set the iterator to the desired species
        constSpeciesIt _it = speciesBegin();
        for (int i = 0; i < speciesIndex; ++i)
            ++_it;
        // remove the desired species from the speciesVec
        _species.erase(_it);
    }
}

void Network::setSpecies(const speciesVec& sv) {
    _species = sv;
}

const speciesVec& Network::getSpecies() const {
    return _species;
}

const size_t Network::getNumSpecies() const {
    return _species.size();
}

NSpecies* Network::findSpeciesById(const std::string& id) {
    // search in species to find the desired one
    for (constSpeciesIt i = speciesBegin(); i != speciesEnd(); ++i) {
        if (stringCompare((*i)->getId(), id))
            return *i;
    }
    // not found
    return NULL;
}

int Network::findSpeciesIndexById(const std::string& id) {
    int _speciesIndex = 0;

    // search in species to find the desired one
    for (constSpeciesIt i = speciesBegin(); i != speciesEnd(); ++i) {
        if (stringCompare((*i)->getId(), id))
            return _speciesIndex;
        ++_speciesIndex;
    }
    // not found
    return -1;
}

NSpecies* Network::findSpeciesByGlyphId(const std::string& glyphId) {
    // search in species to find the desired one using its glyph id
    for (constSpeciesIt i = speciesBegin(); i != speciesEnd(); ++i) {
        if (stringCompare((*i)->getGlyphId(), glyphId))
            return *i;
    }
    // not found
    return NULL;
}

int Network::findSpeciesIndexByGlyphId(const std::string& glyphId) {
    int _speciesIndex = 0;

    // search in species to find the desired one using its glyph id
    for (constSpeciesIt i = speciesBegin(); i != speciesEnd(); ++i) {
        if (stringCompare((*i)->getGlyphId(), glyphId))
            return _speciesIndex;
        ++_speciesIndex;
    }
    // not found
    return -1;
}

std::string Network::getSpeciesUniqueId() {
    size_t k = 0;
    NSpecies* s = NULL;

    // create a new species id and search in the exisiting ones to ensure it has not already used
    while (1) {
        ++k;
        std::stringstream ss;
        ss << "Species_" << k;
        s = findSpeciesById(ss.str());
        if (!s)
            return ss.str();
    }
}

std::string Network::getSpeciesUniqueGlyphId(const std::string& id) {
    size_t k = 0;
    NSpecies* s = NULL;

    // create a new species glyph id and search in the exisiting ones to ensure it has not already used
    while (1) {
        ++k;
        std::stringstream ss;
        ss << id << "_Glyph_" << k;
        s = findSpeciesByGlyphId(ss.str());
        if (!s)
            return ss.str();
    }
}

void Network::addReaction(NReaction* r) {
    if (r)
        _reactions.push_back(r);
    else
        std::cout << "No reaction to add\n";
}

void Network::removeReaction(int reactionIndex) {
    if (reactionIndex >= _reactions.size() || reactionIndex < 0)
        std::cerr << "the entered reaction index is not within the reactionVec range\n";
    else {
        // set the iterator to the desired reaction
        constReactionIt _it = reactionsBegin();
        for (int i = 0; i < reactionIndex; ++i)
            ++_it;
        // remove the desired reaction from the reactionVec
        _reactions.erase(_it);
    }
}

void Network::setReactions(const reactionVec& rv) {
    _reactions = rv;
}

const reactionVec& Network::getReactions() const {
    return _reactions;
}

const size_t Network::getNumReactions() const {
    return _reactions.size();
}

NReaction* Network::findReactionById(const std::string& id) {
    // search in reactions to find the desired one
    for (constReactionIt i = reactionsBegin(); i != reactionsEnd(); ++i) {
        if (stringCompare((*i)->getId(), id))
            return *i;
    }
    // not found
    return NULL;
}

int Network::findReactionIndexById(const std::string& id) {
    int _reactionIndex = 0;

    // search in reactions to find the desired one
    for (constReactionIt i = reactionsBegin(); i != reactionsEnd(); ++i) {
        if (stringCompare((*i)->getId(), id))
            return _reactionIndex;
        ++_reactionIndex;
    }
    // not found
    return -1;
}

NReaction* Network::findReactionByGlyphId(const std::string& glyphId) {
    // search in reactions to find the desired one using its glyph id
    for (constReactionIt i = reactionsBegin(); i != reactionsEnd(); ++i) {
        if (stringCompare((*i)->getGlyphId(), glyphId))
            return *i;
    }
    // not found
    return NULL;
}

int Network::findReactionIndexByGlyphId(const std::string& glyphId) {
    int _reactionIndex = 0;

    // search in reactions to find the desired one using its glyph id
    for (constReactionIt i = reactionsBegin(); i != reactionsEnd(); ++i) {
        if (stringCompare((*i)->getGlyphId(), glyphId))
            return _reactionIndex;
        ++_reactionIndex;
    }
    // not found
    return -1;
}

std::string Network::getReactionUniqueId() {
    size_t k = 0;
    NReaction* r = NULL;

    // create a new reaction id and search in the exisiting ones to ensure it has not already used
    while (1) {
        ++k;
        std::stringstream ss;
        ss << "Reaction_" << k;
        r = findReactionById(ss.str());
        if (!r)
            return ss.str();
    }
}

std::string Network::getReactionUniqueGlyphId(const std::string& id) {
    size_t k = 0;
    NReaction* r = NULL;

    // create a new reaction glyph id and search in the exisiting ones to ensure it has not already used
    while (1) {
        ++k;
        std::stringstream ss;
        ss << id << "_Glyph_" << k;
        r = findReactionByGlyphId(ss.str());
        if (!r)
            return ss.str();
    }
}

void Network::addText(NText* t) {
    if (t)
        _texts.push_back(t);
    else
        std::cout << "No text to add\n";
}

void Network::removeText(int textIndex) {
    if (textIndex >= _texts.size() || textIndex < 0)
        std::cerr << "the entered text index is not within the textVec range\n";
    else {
        // set the iterator to the desired text
        constTextIt _it = textsBegin();
        for (int i = 0; i < textIndex; ++i)
            ++_it;
        // remove the desired text from the textVec
        _texts.erase(_it);
    }
}

void Network::setTexts(const textVec& tv) {
    _texts = tv;
}

const Network::textVec& Network::getTexts() const {
    return _texts;
}

const size_t Network::getNumTexts() const {
    return _texts.size();
}

NText* Network::findTextByGlyphId(const std::string& glyphId) {
    // search in texts to find the desired one using its glyph id
    for (constTextIt i = textsBegin(); i != textsEnd(); ++i) {
        if (stringCompare((*i)->getGlyphId(), glyphId))
            return *i;
    }
    // not found
    return NULL;
}

int Network::findTextIndexByGlyphId(const std::string& glyphId) {
    int _textIndex = 0;

    // search in texts to find the desired one using its glyph id
    for (constTextIt i = textsBegin(); i != textsEnd(); ++i) {
        if (stringCompare((*i)->getGlyphId(), glyphId))
            return _textIndex;
        ++_textIndex;
    }
    // not found
    return -1;
}

NText* Network::findTextByGraphicalObjectId(const std::string& gOId) {
    // search in texts to find the desired one using its graphcial object id
    for (constTextIt i = textsBegin(); i != textsEnd(); ++i) {
        if ((*i)->isSetGraphicalObjectId() && stringCompare((*i)->getGraphicalObjectId(), gOId))
            return *i;
    }
    // not found
    return NULL;
}

int Network::findTextIndexByGraphicalObjectId(const std::string& gOId) {
    int _textIndex = 0;

    // search in texts to find the desired one using its graphical object id
    for (constTextIt i = textsBegin(); i != textsEnd(); ++i) {
        if ((*i)->isSetGraphicalObjectId() && stringCompare((*i)->getGraphicalObjectId(), gOId))
            return _textIndex;
        ++_textIndex;
    }
    // not found
    return -1;
}

NText* Network::findTextByOriginOfTextId(const std::string& id) {
    // search in texts to find the desired one using its origin of text id
    for (constTextIt i = textsBegin(); i != textsEnd(); ++i) {
        if ((*i)->isSetOriginOfTextId() && stringCompare((*i)->getOriginOfTextId(), id))
            return *i;
    }
    // not found
    return NULL;
}

int Network::findTextIndexByOriginOfTextId(const std::string& id) {
    int _textIndex = 0;

    // search in texts to find the desired one using its origin of text id
    for (constTextIt i = textsBegin(); i != textsEnd(); ++i) {
        if ((*i)->isSetOriginOfTextId() && stringCompare((*i)->getOriginOfTextId(), id))
            return _textIndex;
        ++_textIndex;
    }
    // not found
    return -1;
}

std::string Network::getTextUniqueGlyphId() {
    size_t k = 0;
    NText* t = NULL;

    // create a new text glyph id and search in the exisiting ones to ensure it has not already used
    while (1) {
        ++k;
        std::stringstream ss;
        ss << "TextGlyph_" << k;
        t = findTextByGlyphId(ss.str());
        if (!t)
            return ss.str();
    }
}

void Network::addGObject(NGraphicalObject* o) {
    if (o)
        _gObjects.push_back(o);
    else
        std::cout << "No graphical object to add\n";
}

void Network::removeGObject(int gObjectIndex) {
    if (gObjectIndex >= _gObjects.size() || gObjectIndex < 0)
        std::cerr << "the entered graphcial object index is not within the gObjectVec range\n";
    else {
        // set the iterator to the desired graphical object
        constGObjectIt _it = gObjectsBegin();
        for (int i = 0; i < gObjectIndex; ++i)
            ++_it;
        // remove the desired graphcial object from the gObjectVec
        _gObjects.erase(_it);
    }
}

void Network::setGObjects(const gObjectVec& ov) {
    _gObjects = ov;
}

const Network::gObjectVec& Network::getGObjects() const {
    return _gObjects;
}

const size_t Network::getNumGObjects() const {
    return _gObjects.size();
}

NGraphicalObject* Network::findGObjectById(const std::string& id) {
    // search in graphical objects to find the desired one
    for (constGObjectIt i = gObjectsBegin(); i != gObjectsEnd(); ++i) {
        if (stringCompare((*i)->getId(), id))
            return *i;
    }
    // not found
    return NULL;
}

int Network::findGObjectIndexById(const std::string& id) {
    int _gObjectIndex = 0;

    // search in graphical objects to find the desired one
    for (constGObjectIt i = gObjectsBegin(); i != gObjectsEnd(); ++i) {
        if (stringCompare((*i)->getId(), id))
            return _gObjectIndex;
        ++_gObjectIndex;
    }
    // not found
    return -1;
}

NGraphicalObject* Network::findGObjectByGlyphId(const std::string& glyphId) {
    // search in graphical objects to find the desired one using its glyph id
    for (constGObjectIt i = gObjectsBegin(); i != gObjectsEnd(); ++i) {
        if (stringCompare((*i)->getGlyphId(), glyphId))
            return *i;
    }
    // not found
    return NULL;
}

int Network::findGObjectIndexByGlyphId(const std::string& glyphId) {
    int _gObjectIndex = 0;

    // search in graphical objects to find the desired one using its glyph id
    for (constGObjectIt i = gObjectsBegin(); i != gObjectsEnd(); ++i) {
        if (stringCompare((*i)->getGlyphId(), glyphId))
            return _gObjectIndex;
        ++_gObjectIndex;
    }
    // not found
    return -1;
}

std::string Network::getGObjectUniqueGlyphId() {
    size_t k = 0;
    NGraphicalObject* t = NULL;

    // create a new graphical object id and search in the exisiting ones to ensure it has not already used
    while (1) {
        ++k;
        std::stringstream ss;
        ss << "GraphicalObject_" << k;
        t = findGObjectByGlyphId(ss.str());
        if (!t)
            return ss.str();
    }
}

NGraphicalObject* Network::findNetworkElement(const std::string& id) {
    // search in compartments to find the desired one
    for (constCompartmentIt cIt = compartmentsBegin(); cIt != compartmentsEnd(); ++cIt) {
       if (stringCompare((*cIt)->getId(), id) || stringCompare((*cIt)->getGlyphId(), id))
           return *cIt;
    }
    
    // search in species to find the desired one
    for (constSpeciesIt sIt = speciesBegin(); sIt != speciesEnd(); ++sIt) {
       if (stringCompare((*sIt)->getId(), id) || stringCompare((*sIt)->getGlyphId(), id))
           return *sIt;
    }
    
    // search in reactions to find the desired one
    for (constReactionIt rIt = reactionsBegin(); rIt != reactionsEnd(); ++rIt) {
       if (stringCompare((*rIt)->getId(), id) || stringCompare((*rIt)->getGlyphId(), id))
           return *rIt;
        
        for (constSReferenceIt sRIt = (*rIt)->sReferencesBegin(); sRIt != (*rIt)->sReferencesEnd(); ++ sRIt) {
            if (stringCompare((*sRIt)->getId(), id) || stringCompare((*sRIt)->getGlyphId(), id))
                return *sRIt;
        }
    }
    
    // search in texts to find the desired one
    for (constTextIt tIt = textsBegin(); tIt != textsEnd(); ++tIt) {
       if (stringCompare((*tIt)->getGlyphId(), id))
           return *tIt;
    }
    
    // search in graphical objects to find the desired one
    for (constGObjectIt gOIt = gObjectsBegin(); gOIt != gObjectsEnd(); ++gOIt) {
       if (stringCompare((*gOIt)->getId(), id) || stringCompare((*gOIt)->getGlyphId(), id))
           return *gOIt;
    }
    
    // not found
    return NULL;
}

void Network::setBox(const double& x, const double& y, const double& width, const double& height) {
    _box.setX(x);
    _box.setY(y);
    _box.setWidth(width);
    _box.setHeight(height);
    _isSetBox = true;
}

const LBox& Network::getBox() const {
    return _box;
}

void Network::setLayoutSpecified (bool value) {
    _isLayoutSpecified = value;
}

//--CLASS NGraphicalObject--

NGraphicalObject::NGraphicalObject(Network* net, NGraphicalObject& gO) : NetworkElement(gO) {
    _box = NULL;
    _texts.clear();
    _isSetGlyphId = false;
    _isMatchWithGlyph = false;
    _isSetObjectRole = false;
    _isSetBox = false;
    _isUsed = false;
    _isLockedPosition = false;
    
    // boundingbox
    if (gO.isSetBox())
        setBox(new LBox(*(gO.getBox())));
    
    // text
    NText* text = NULL;
    for (constTextIt tIt = gO.textsBegin(); tIt != gO.textsEnd(); ++tIt) {
        text = new NText(*(*tIt));
        text->setGlyphId(net->getTextUniqueGlyphId());
        net->addText(text);
        addText(text);
    }
    
    // obejectrole
    if (gO.isSetObjectRole())
        setObjectRole(gO.getObjectRole());
}

GrphObjType NGraphicalObject::getType() {
    return _type;
}

void NGraphicalObject::setGlyphId(const std::string& glyphId) {
    _glyphId = glyphId;
    _isSetGlyphId = true;
}

const std::string& NGraphicalObject::getGlyphId() const {
    return _glyphId;
}

void NGraphicalObject::setMatchWithGlyph(bool value) {
    _isMatchWithGlyph = value;
}

void NGraphicalObject::setObjectRole(const std::string& role) {
    _objectRole = role;
    _isSetObjectRole = true;
}

const std::string& NGraphicalObject::getObjectRole() const {
    return _objectRole;
}

void NGraphicalObject::setBox(LBox* box) {
    if (box) {
        _box = box;
        _isSetBox = true;
    }
    else
        std::cout << "No bounding box to set\n";
}

void NGraphicalObject::unSetBox(const bool& removeBox) {
    _isSetBox = false;
    if (removeBox) {
        delete _box;
        _box = NULL;
    }
}

LBox* NGraphicalObject::getBox() {
    return _box;
}

void NGraphicalObject::addText(NText* t) {
    if (t)
        _texts.push_back(t);
    else
        std::cout << "No text to add\n";
}

void NGraphicalObject::removeText(int textIndex) {
    if (textIndex >= _texts.size() || textIndex < 0)
        std::cerr << "the entered text index is not within the textVec range\n";
    else {
        // set the iterator to the desired text
        constTextIt _it = textsBegin();
        for (int i = 0; i < textIndex; ++i)
            ++_it;
        // remove the desired text from the textVec
        _texts.erase(_it);
    }
}

void NGraphicalObject::setTexts(const textVec& tv) {
    _texts = tv;
}

const NGraphicalObject::textVec& NGraphicalObject::getTexts() const {
    return _texts;
}

const size_t NGraphicalObject::getNumTexts() const {
    return _texts.size();
}

NText* NGraphicalObject::findTextByGlyphId(const std::string& glyphId) {
    // search in texts to find the desired one using its glyph id
    for (constTextIt i = textsBegin(); i != textsEnd(); ++i) {
        if (stringCompare((*i)->getGlyphId(), glyphId))
            return *i;
    }
    // not found
    return NULL;
}

int NGraphicalObject::findTextIndexByGlyphId(const std::string& glyphId) {
    int _textIndex = 0;

    // search in texts to find the desired one using its glyph id
    for (constTextIt i = textsBegin(); i != textsEnd(); ++i) {
        if (stringCompare((*i)->getGlyphId(), glyphId))
            return _textIndex;
        ++_textIndex;
    }
    // not found
    return -1;
}

void NGraphicalObject::setUsed(bool value) {
    _isUsed = value;
}

void NGraphicalObject::setLockedPosition(bool value) {
    _isLockedPosition = value;
}

//--CLASS NCompartment--

NCompartment::NCompartment(NCompartment& c) : NGraphicalObject(c) {
    _type = GRP_OBJ_CMP;
    _species.clear();
    _reactions.clear();
    _isSetOrder = false;
    
    // species
    if (c.getNumSpecies())
        setSpecies(c.getSpecies());
    
    // reactions
    if (c.getNumReactions())
        setReactions(c.getReactions());
    
    // order
    if (c.isSetOrder())
        setOrder(c.getOrder());
}

void NCompartment::addSpecies(NSpecies* s) {
    if (s)
        _species.push_back(s);
    else
        std::cout << "No species to add to compartment\n";
}

void NCompartment::removeSpecies(int speciesIndex) {
    if (speciesIndex >= _species.size() || speciesIndex < 0)
        std::cerr << "the entered species index is not within the speciesVec range\n";
    else {
        // set the iterator to the desired species
        constSpeciesIt _it = speciesBegin();
        for (int i = 0; i < speciesIndex; ++i)
            ++_it;
        // remove the desired species from the speciesVec
        _species.erase(_it);
    }
}

void NCompartment::setSpecies(const speciesVec& sv) {
    _species = sv;
}

const speciesVec& NCompartment::getSpecies() const {
    return _species;
}

const size_t NCompartment::getNumSpecies() const {
    return _species.size();
}

NSpecies* NCompartment::findSpeciesById(const std::string& id) {
    // search in species to find the desired one
    for (constSpeciesIt i = speciesBegin(); i != speciesEnd(); ++i) {
        if (stringCompare((*i)->getId(), id))
            return *i;
    }
    // not found
    return NULL;
}

int NCompartment::findSpeciesIndexById(const std::string& id) {
    int _speciesIndex = 0;

    // search in species to find the desired one
    for (constSpeciesIt i = speciesBegin(); i != speciesEnd(); ++i) {
        if (stringCompare((*i)->getId(), id))
            return _speciesIndex;
        ++_speciesIndex;
    }
    // not found
    return -1;
}

NSpecies* NCompartment::findSpeciesByGlyphId(const std::string& glyphId) {
    // search in species to find the desired one using its glyph id
    for (constSpeciesIt i = speciesBegin(); i != speciesEnd(); ++i) {
        if (stringCompare((*i)->getGlyphId(), glyphId))
            return *i;
    }
    // not found
    return NULL;
}

int NCompartment::findSpeciesIndexByGlyphId(const std::string& glyphId) {
    int _speciesIndex = 0;

    // search in species to find the desired one using its glyph id
    for (constSpeciesIt i = speciesBegin(); i != speciesEnd(); ++i) {
        if (stringCompare((*i)->getGlyphId(), glyphId))
            return _speciesIndex;
        ++_speciesIndex;
    }
    // not found
    return -1;
}

void NCompartment::addReaction(NReaction* r) {
    if (r)
        _reactions.push_back(r);
    else
        std::cout << "No reaction to add\n";
}

void NCompartment::removeReaction(int reactionIndex) {
    if (reactionIndex >= _reactions.size() || reactionIndex < 0)
        std::cerr << "the entered reaction index is not within the reactionVec range\n";
    else {
        // set the iterator to the desired reaction
        constReactionIt _it = reactionsBegin();
        for (int i = 0; i < reactionIndex; ++i)
            ++_it;
        // remove the desired reaction from the reactionVec
        _reactions.erase(_it);
    }
}

void NCompartment::setReactions(const reactionVec& rv) {
    _reactions = rv;
}

const reactionVec& NCompartment::getReactions() const {
    return _reactions;
}

const size_t NCompartment::getNumReactions() const {
    return _reactions.size();
}

NReaction* NCompartment::findReactionById(const std::string& id) {
    // search in reactions to find the desired one
    for (constReactionIt i = reactionsBegin(); i != reactionsEnd(); ++i) {
        if (stringCompare((*i)->getId(), id))
            return *i;
    }
    // not found
    return NULL;
}

int NCompartment::findReactionIndexById(const std::string& id) {
    int _reactionIndex = 0;

    // search in reactions to find the desired one
    for (constReactionIt i = reactionsBegin(); i != reactionsEnd(); ++i) {
        if (stringCompare((*i)->getId(), id))
            return _reactionIndex;
        ++_reactionIndex;
    }
    // not found
    return -1;
}

NReaction* NCompartment::findReactionByGlyphId(const std::string& glyphId) {
    // search in reactions to find the desired one using its glyph id
    for (constReactionIt i = reactionsBegin(); i != reactionsEnd(); ++i) {
        if (stringCompare((*i)->getGlyphId(), glyphId))
            return *i;
    }
    // not found
    return NULL;
}

int NCompartment::findReactionIndexByGlyphId(const std::string& glyphId) {
    int _reactionIndex = 0;

    // search in reactions to find the desired one using its glyph id
    for (constReactionIt i = reactionsBegin(); i != reactionsEnd(); ++i) {
        if (stringCompare((*i)->getGlyphId(), glyphId))
            return _reactionIndex;
        ++_reactionIndex;
    }
    // not found
    return -1;
}

void NCompartment::setOrder(const double& order) {
    _order = order;
    _isSetOrder = true;
}

const double& NCompartment::getOrder() const {
    return _order;
}

void NCompartment::shiftItems(const double& dx, const double& dy) {
    for (constReactionIt rIt = reactionsBegin(); rIt != reactionsEnd(); ++rIt) {
        if ((*rIt)->getNumConnectedReactions() == 0 && (*rIt)->isMatchWithGlyph())
            (*rIt)->shiftItems(dx, dy);
    }
}

//--CLASS NSpecies--

NSpecies::NSpecies(NSpecies& s) : NGraphicalObject(s) {
    _type = GRP_OBJ_SPC;
    _reactionMembers.clear();
    _rSides.clear();
    _isSetCompartment = false;
    _isPseudoSpecies = false;
    
    // compartment
    if (s.isSetCompartment())
        setCompartment(s.getCompartment());
}

void NSpecies::setCompartment(const std::string& compartmentId) {
    _compartment = compartmentId;
    _isSetCompartment = true;
}

const std::string& NSpecies::getCompartment() const {
    return _compartment;
}

void NSpecies::addReactionMember(NReaction* r, const double& angle, const bool& isMainReaction) {
    reactionMember* rMember = findReactionMemberById(r->getId());
    if (!rMember) {
        rMember = new reactionMember();
        rMember->angle = angle;
        rMember->reaction = r;
        rMember->isMainReaction = isMainReaction;
        _reactionMembers.push_back(rMember);
    }
    else {
        rMember->angle = angle;
        rMember->isMainReaction = isMainReaction;
    }
}

void NSpecies::removeReactionMember(const std::string& reactionId) {
    for (constRMemberIt rMIt = rMembersBegin(); rMIt != rMembersEnd(); ++rMIt) {
        if (stringCompare((*rMIt)->reaction->getId(), reactionId)) {
            _reactionMembers.erase(rMIt);
            break;
        }
    }
}

const NSpecies::rMemberVec& NSpecies::getReactionMembers() const {
    return _reactionMembers;
}

void NSpecies::setReactionMembers(const rMemberVec& rmv) {
    _reactionMembers = rmv;
}

const size_t NSpecies::getNumReactionMembers() const {
    return _reactionMembers.size();
}

reactionMember* NSpecies::findReactionMemberById(const std::string& reactionId) {
    // search in reaction members to find the desired one
    for (constRMemberIt rMIt = rMembersBegin(); rMIt != rMembersEnd(); ++rMIt) {
        if (stringCompare((*rMIt)->reaction->getId(), reactionId))
            return *rMIt;
    }
    // not found
    return NULL;
}

reactionMember* NSpecies::findReactionMemberByGlyphId(const std::string& reactionGlyphId) {
    // search in reaction members to find the desired one using the reacion glyph id
    for (constRMemberIt rMIt = rMembersBegin(); rMIt != rMembersEnd(); ++rMIt) {
        if (stringCompare((*rMIt)->reaction->getGlyphId(), reactionGlyphId))
            return *rMIt;
    }
    // not found
    return NULL;
}

void NSpecies::addReferencedSide(const SpcSide& side) {
    bool isAlreadyReferenced = false;
    
    // search in referenced sides to find the one with the same side
    for (constRSideIt rSIt = rSideBegin(); rSIt != rSideEnd(); ++rSIt) {
        if ((*rSIt)->side == side)
            isAlreadyReferenced = true;
    }
    // not found, add a new one
    if (!isAlreadyReferenced) {
        spcReferencedSide* rSide = new spcReferencedSide();
        rSide->side = side;
        rSide->speciesReferences.clear();
        _rSides.push_back(rSide);
    }
}

void NSpecies::addToReferencedSide(NSpeciesReference* sr, const SpcSide& side) {
    // search in referenced sides to find the one with the same side
    constRSideIt rSIt;
    for (rSIt = rSideBegin(); rSIt != rSideEnd(); ++rSIt) {
        if ((*rSIt)->side == side)
            break;
    }
    // not found, add a new one
    if (rSIt == rSideEnd()) {
        addReferencedSide(side);
        rSIt = rSideEnd() - 1;
    }
    
    // add species reference to the referenced side
    (*rSIt)->speciesReferences.push_back(sr);
}

void NSpecies::removeReferencedSide(const SpcSide& side) {
    // search in referenced sides to find the one with the same side
    for (constRSideIt rSIt = rSideBegin(); rSIt != rSideEnd(); ++rSIt) {
        if ((*rSIt)->side == side) {
            _rSides.erase(rSIt);
            break;
        }
    }
}

void NSpecies::removeFromReferencedSide(NSpeciesReference* sr, const SpcSide& side) {
    // search in referenced sides to find the one with the same side
    for (constRSideIt rSIt = rSideBegin(); rSIt != rSideEnd(); ++rSIt) {
        if ((*rSIt)->side == side) {
            for (int i = 0; i < (*rSIt)->speciesReferences.size(); ++i) {
                if (stringCompare(sr->getId(), (*rSIt)->speciesReferences.at(i)->getId()))
                    (*rSIt)->speciesReferences.at(i) = NULL;
            }
        }
    }
}

SReferenceVec NSpecies::getSideSpeciesReferences(const SpcSide& side) {
    SReferenceVec srv;
    
    // search in referenced sides to find the one with the same side
    for (constRSideIt rSIt = rSideBegin(); rSIt != rSideEnd(); ++rSIt) {
        if ((*rSIt)->side == side) {
            srv =  (*rSIt)->speciesReferences;
            break;
        }
    }
    // not referenced
    return srv;
}

void NSpecies::setReferencedSide(const rSideVec& rsv) {
    _rSides = rsv;
}

const NSpecies::rSideVec& NSpecies::getReferencedSides() const {
    return _rSides;
}

const size_t NSpecies::getNumRSides() const {
    return _rSides.size();
}

void NSpecies::setPseudoSpecies(bool value) {
    _isPseudoSpecies = value;
}

//--CLASS NReaction--

NReaction::NReaction(NReaction& r) : NGraphicalObject(r) {
    _type = GRP_OBJ_RXN;
    _speciesReferences.clear();
    _subSpeciesReferences.clear();
    _species.clear();
    _subSpecies.clear();
    _pseudoSpecies.clear();
    _curve = NULL;
    _extentBox = new LBox();
    _rSides.clear();
    _rEnds.clear();
    _connectedReactions.clear();
    _subReactions.clear();
    _reactionLayers.clear();
    _directionAngle = 0.0;
    _speciesStartAngle = 0.0;
    _isSetCompartment = false;
    _isSetCurve = false;
    _isSetCenterP = false;
    _isSetRadius = false;
    _isSetDirectionAngle = false;
    _isSetSpeciesStartAngle = false;
    
    // compartment
    if (r.isSetCompartment())
        setCompartment(r.getCompartment());
    
    // speciesreferences
    if (r.getNumSpeciesReferences())
        setSpeciesReferences(r.getSpeciesReferences());
    
    // curve
    if (r.isSetCurve())
        setCurve(new LCurve(*(r.getCurve())));
}

void NReaction::setCompartment(const std::string& compartmentId) {
    _compartment = compartmentId;
    _isSetCompartment = true;
}

const std::string& NReaction::getCompartment() const {
    return _compartment;
}

const std::string NReaction::findCompartment() const {
    if (isSetCompartment())
        return getCompartment();
    
    std::string compartment;
    for (constSReferenceIt sRIt = sReferencesBegin(); sRIt != sReferencesEnd(); ++sRIt) {
        if ((*sRIt)->isSetSpecies() && (*sRIt)->getSpecies()->isSetCompartment()) {
            if (sRIt - sReferencesBegin() == 0)
                compartment = (*sRIt)->getSpecies()->getCompartment();
            else if (!stringCompare(compartment, (*sRIt)->getSpecies()->getCompartment())) {
                compartment.clear();
                break;
            }
        }
    }
    
    return compartment;
}

void NReaction::addSpeciesReference(NSpeciesReference* sr) {
    if (sr)
        _speciesReferences.push_back(sr);
    else
        std::cout << "No species reference to add\n";
}

void NReaction::removeSpeciesReference(int sReferenceIndex) {
    if (sReferenceIndex >= _speciesReferences.size() || sReferenceIndex < 0)
        std::cerr << "the entered species reference index is not within the sReferenceVec range\n";
    else {
        // set the iterator to the desired species reference
        constSReferenceIt _it = sReferencesBegin();
        for (int i = 0; i < sReferenceIndex; ++i)
            ++_it;
        // remove the desired species reference from the sReferenceVec
        _speciesReferences.erase(_it);
    }
}

void NReaction::setSpeciesReferences(const SReferenceVec& srv) {
    _speciesReferences = srv;
}

const SReferenceVec& NReaction::getSpeciesReferences() const {
    return _speciesReferences;
}

const size_t NReaction::getNumSpeciesReferences() const {
    return _speciesReferences.size();
}

NSpeciesReference* NReaction::findSpeciesReferenceById(const std::string& id) {
    // search in species references to find the desired one
    for (constSReferenceIt i = sReferencesBegin(); i != sReferencesEnd(); ++i) {
        if (stringCompare((*i)->getId(), id))
            return *i;
    }
    // not found
    return NULL;
}

int NReaction::findSpeciesReferenceIndexById(const std::string& id) {
    int _sReferenceIndex = 0;
    
    // search in species references to find the desired one
    for (constSReferenceIt i = sReferencesBegin(); i != sReferencesEnd(); ++i) {
        if (stringCompare((*i)->getId(), id))
            return _sReferenceIndex;
        ++_sReferenceIndex;
    }
    // not found
    return -1;
}

NSpeciesReference* NReaction::findSpeciesReferenceByGlyphId(const std::string& glyphId) {
    // search in species references to find the desired one using its glyph id
    for (constSReferenceIt i = sReferencesBegin(); i != sReferencesEnd(); ++i) {
        if (stringCompare((*i)->getGlyphId(), glyphId))
            return *i;
    }
    // not found
    return NULL;
}

int NReaction::findSpeciesReferenceIndexByGlyphId(const std::string& glyphId) {
    int _sReferenceIndex = 0;
    
    // search in species references to find the desired one using its glyph id
    for (constSReferenceIt i = sReferencesBegin(); i != sReferencesEnd(); ++i) {
        if (stringCompare((*i)->getGlyphId(), glyphId))
            return _sReferenceIndex;
        ++_sReferenceIndex;
    }
    // not found
    return -1;
    
}

std::string NReaction::getSpeciesReferenceUniqueId() {
    size_t k = 0;
    NSpeciesReference* sr = NULL;
    
    // create a new species reference id and search in the exisiting ones to ensure it has not already used
    while (1) {
        ++k;
        std::stringstream ss;
        ss << getId() << "_SpeciesReference_" << k;
        sr = findSpeciesReferenceById(ss.str());
        if (!sr)
            return ss.str();
    }
}

std::string NReaction::getSpeciesReferenceUniqueGlyphId(const std::string& id) {
    size_t k = 0;
    NSpeciesReference* sr = NULL;
    
    // create a new species reference id and search in the exisiting ones to ensure it has not already used
    while (1) {
        ++k;
        std::stringstream ss;
        ss << id << "_Glyph_" << k;
        sr = findSpeciesReferenceById(ss.str());
        if (!sr)
            return ss.str();
    }
}

void NReaction::addSubSpeciesReference(NSpeciesReference* ssr) {
    if (ssr)
        _subSpeciesReferences.push_back(ssr);
    else
        std::cout << "No sub species reference to add\n";
}

void NReaction::removeSubSpeciesReference(int subSReferenceIndex) {
    if (subSReferenceIndex >= _subSpeciesReferences.size() || subSReferenceIndex < 0)
        std::cerr << "the entered sub species reference index is not within the sub species references range\n";
    else {
        // set the iterator to the desired sub species reference
        constSReferenceIt _it = subSReferencesBegin();
        for (int i = 0; i < subSReferenceIndex; ++i)
            ++_it;
        // remove the desired sub species reference from the sub species references
        _subSpeciesReferences.erase(_it);
    }
}

void NReaction::setSubSpeciesReferences(const SReferenceVec& ssrv) {
    _subSpeciesReferences = ssrv;
}

const SReferenceVec& NReaction::getSubSpeciesReference() const {
    return _subSpeciesReferences;
}

const size_t NReaction::getNumSubSpeciesReferences() const {
    return _subSpeciesReferences.size();
}

NSpeciesReference* NReaction::findSubSpeciesReferenceById(const std::string& id) {
    // search in sub species references to find the desired one
    for (constSReferenceIt i = subSReferencesBegin(); i != subSReferencesEnd(); ++i) {
        if (stringCompare((*i)->getId(), id))
            return *i;
    }
    // not found
    return NULL;
}

int NReaction::findSubSpeciesReferenceIndexById(const std::string& id) {
    int _subSReferenceIndex = 0;
    
    // search in sub species references to find the desired one
    for (constSReferenceIt i = subSReferencesBegin(); i != subSReferencesEnd(); ++i) {
        if (stringCompare((*i)->getId(), id))
            return _subSReferenceIndex;
        ++_subSReferenceIndex;
    }
    // not found
    return -1;
}
                               
void NReaction::addSpecies(NSpecies* s) {
    if (s)
        _species.push_back(s);
    else
        std::cout << "No species to add to reaction\n";
}

void NReaction::removeSpecies(int speciesIndex) {
    if (speciesIndex >= _species.size() || speciesIndex < 0)
        std::cerr << "the entered species index is not within the speciesVec range\n";
    else {
        // set the iterator to the desired species
        constSpeciesIt _it = speciesBegin();
        for (int i = 0; i < speciesIndex; ++i)
            ++_it;
        // remove the desired species from the speciesVec
        _species.erase(_it);
    }
}

void NReaction::setSpecies(const speciesVec& sv) {
    _species = sv;
}

const speciesVec& NReaction::getSpecies() const {
    return _species;
}

const size_t NReaction::getNumSpecies() const {
    return _species.size();
}

NSpecies* NReaction::findSpeciesById(const std::string& id) {
    // search in species to find the desired one
    for (constSpeciesIt i = speciesBegin(); i != speciesEnd(); ++i) {
        if (stringCompare((*i)->getId(), id))
            return *i;
    }
    // not found
    return NULL;
}

int NReaction::findSpeciesIndexById(const std::string& id) {
    int _speciesIndex = 0;

    // search in species to find the desired one
    for (constSpeciesIt i = speciesBegin(); i != speciesEnd(); ++i) {
        if (stringCompare((*i)->getId(), id))
            return _speciesIndex;
        ++_speciesIndex;
    }
    // not found
    return -1;
}

NSpecies* NReaction::findSpeciesByGlyphId(const std::string& glyphId) {
    // search in species to find the desired one using its glyph id
    for (constSpeciesIt i = speciesBegin(); i != speciesEnd(); ++i) {
        if (stringCompare((*i)->getGlyphId(), glyphId))
            return *i;
    }
    // not found
    return NULL;
}

int NReaction::findSpeciesIndexByGlyphId(const std::string& glyphId) {
    int _speciesIndex = 0;

    // search in species to find the desired one using its glyph id
    for (constSpeciesIt i = speciesBegin(); i != speciesEnd(); ++i) {
        if (stringCompare((*i)->getGlyphId(), glyphId))
            return _speciesIndex;
        ++_speciesIndex;
    }
    // not found
    return -1;
}

void NReaction::addSubSpecies(NSpecies* ss) {
    if (ss)
        _subSpecies.push_back(ss);
    else
        std::cout << "No subSpecies to add to reaction\n";
}

void NReaction::removeSubSpecies(int subSpeciesIndex) {
    if (subSpeciesIndex >= _subSpecies.size() || subSpeciesIndex < 0)
        std::cerr << "the entered subSpecies index is not within the subSpeciesVec range\n";
    else {
        // set the iterator to the desired subSpecies
        constSpeciesIt _it = subSpeciesBegin();
        for (int i = 0; i < subSpeciesIndex; ++i)
            ++_it;
        // remove the desired subSpecies from the subSpeciesVec
        _subSpecies.erase(_it);
    }
}

void NReaction::setSubSpecies(const speciesVec& ssv) {
    _subSpecies = ssv;
}

const speciesVec& NReaction::getSubSpecies() const {
    return _subSpecies;
}

const size_t NReaction::getNumSubSpecies() const {
    return _subSpecies.size();
}

NSpecies* NReaction::findSubSpeciesById(const std::string& id) {
    // search in subSpecies to find the desired one
    for (constSpeciesIt i = subSpeciesBegin(); i != subSpeciesEnd(); ++i) {
        if (stringCompare((*i)->getId(), id))
            return *i;
    }
    // not found
    return NULL;
}

int NReaction::findSubSpeciesIndexById(const std::string& id) {
    int _subSpeciesIndex = 0;
    
    // search in subSpecies to find the desired one
    for (constSpeciesIt i = subSpeciesBegin(); i != subSpeciesEnd(); ++i) {
        if (stringCompare((*i)->getId(), id))
            return _subSpeciesIndex;
        ++_subSpeciesIndex;
    }
    // not found
    return -1;
}

void NReaction::addPseudoSpecies(NSpecies* ps) {
    if (ps)
        _pseudoSpecies.push_back(ps);
    else
        std::cout << "No pseudoSpecies to add to reaction\n";
}

void NReaction::removePseudoSpecies(int pseudoSpeciesIndex) {
    if (pseudoSpeciesIndex >= _pseudoSpecies.size() || pseudoSpeciesIndex < 0)
        std::cerr << "the entered pseudoSpecies index is not within the pseudoSpeciesVec range\n";
    else {
        // set the iterator to the desired pseudoSpecies
        constSpeciesIt _it = pseudoSpeciesBegin();
        for (int i = 0; i < pseudoSpeciesIndex; ++i)
            ++_it;
        // remove the desired pseudoSpecies from the pseudoSpeciesVec
        _pseudoSpecies.erase(_it);
    }
}

void NReaction::setPseudoSpecies(const speciesVec& psv) {
    _pseudoSpecies = psv;
}

const speciesVec& NReaction::getPseudoSpecies() const {
    return _pseudoSpecies;
}

const size_t NReaction::getNumPseudoSpecies() const {
    return _pseudoSpecies.size();
}

NSpecies* NReaction::findPseudoSpeciesById(const std::string& id) {
    // search in pseudoSpecies to find the desired one
    for (constSpeciesIt i = pseudoSpeciesBegin(); i != pseudoSpeciesEnd(); ++i) {
        if (stringCompare((*i)->getId(), id))
            return *i;
    }
    // not found
    return NULL;
}

int NReaction::findPseudoSpeciesIndexById(const std::string& id) {
    int _pseudoSpeciesIndex = 0;
    
    // search in pseudoSpecies to find the desired one
    for (constSpeciesIt i = pseudoSpeciesBegin(); i != pseudoSpeciesEnd(); ++i) {
        if (stringCompare((*i)->getId(), id))
            return _pseudoSpeciesIndex;
        ++_pseudoSpeciesIndex;
    }
    // not found
    return -1;
}

std::string NReaction::generatePseudoSpeciesId(const speciesVec& mutualSpecies) {
    std::stringstream ss;
    ss << getId() << "_";
    for (int i = 0; i < mutualSpecies.size(); ++i)
        ss << mutualSpecies.at(i)->getId() << "_";
    ss << "PseudoSpecies";
    return ss.str();
}

std::string NReaction::generatePseudoSpeciesId(const speciesVec& mutualSpecies, const unsigned int& version) {
    std::string pseudoSpeciesId = generatePseudoSpeciesId(mutualSpecies);
    if (version != 0) {
        std::stringstream ss;
        ss << pseudoSpeciesId << "_" << version;
        pseudoSpeciesId = ss.str();
    }
    
    return pseudoSpeciesId;
}

std::string NReaction::generatePseudoSpeciesUniqueId(const speciesVec& mutualSpecies) {
    std::string pseudoSpeciesId = generatePseudoSpeciesId(mutualSpecies);
    std::string pseudoSpeciesUniqueId = pseudoSpeciesId;
    size_t k = 0;

    while (findPseudoSpeciesById(pseudoSpeciesUniqueId)) {
        ++k;
        std::stringstream ss;
        ss << pseudoSpeciesId << "_" << k;
        pseudoSpeciesUniqueId = ss.str();
    }
    
    return pseudoSpeciesUniqueId;
}

void NReaction::setCurve(LCurve* curve) {
    if (curve) {
        _curve = curve;
        _isSetCurve = true;
    }
    else
        std::cout << "No curve to set\n";
}

void NReaction::unSetCurve(const bool& removeCurve) {
    _isSetCurve = false;
    if (removeCurve) {
        delete _curve;
        _curve = NULL;
    }
}

LCurve* NReaction::getCurve() {
    return _curve;
}

void NReaction::addReferencedSide(NSpeciesReference* sr, const RxnSide& side) {
    // search in referenced sides to find the one with the same side
    constRSideIt rSIt;
    for (rSIt = rSideBegin(); rSIt != rSideEnd(); ++rSIt) {
        if ((*rSIt)->side == side)
            break;
    }
    // not found, add a new one
    if (rSIt == rSideEnd()) {
        rxnReferencedSide* rSide = new rxnReferencedSide();
        rSide->side = side;
        _rSides.push_back(rSide);
        rSIt = rSideEnd() - 1;
    }
    
    // add species reference to the referenced side
    (*rSIt)->speciesReferences.push_back(sr);
}

void NReaction::removeFromReferencedSide(NSpeciesReference* sr, const RxnSide& side) {
    // search in referenced sides to find the one with the same side
    for (constRSideIt rSIt = rSideBegin(); rSIt != rSideEnd(); ++rSIt) {
        if ((*rSIt)->side == side) {
            for (int i = 0; i < (*rSIt)->speciesReferences.size(); ++i) {
                if (stringCompare(sr->getId(), (*rSIt)->speciesReferences.at(i)->getId()))
                    (*rSIt)->speciesReferences.at(i) = NULL;
            }
        }
    }
}

SReferenceVec NReaction::getSideSpeciesReferences(const RxnSide& side) {
    SReferenceVec srv;
    
    // search in referenced sides to find the one with the same side
    for (constRSideIt rSIt = rSideBegin(); rSIt != rSideEnd(); ++rSIt) {
        if ((*rSIt)->side == side) {
            srv =  (*rSIt)->speciesReferences;
            break;
        }
    }
    // not referenced
    return srv;
}

void NReaction::setReferencedSide(const rSideVec& rsv) {
    _rSides = rsv;
}

const NReaction::rSideVec& NReaction::getReferencedSides() const {
    return _rSides;
}

const size_t NReaction::getNumRSides() const {
    return _rSides.size();
}

void NReaction::addReferencedEnd(NSpeciesReference* sr, const RxnEnd& end) {
    // search in referenced ends to find the one with the same end
    constREndIt rEIt;
    for (rEIt = rEndsBegin(); rEIt != rEndsEnd(); ++rEIt) {
        if ((*rEIt)->end == end)
            break;
    }
    // not found, add a new one
    if (rEIt == rEndsEnd()) {
        referencedEnd* rEnd = new referencedEnd();
        rEnd->end = end;
        rEnd->speciesReferences.resize(0);
        _rEnds.push_back(rEnd);
        rEIt = rEndsEnd() - 1;
    }
    
    // add species reference to the referenced end
    (*rEIt)->speciesReferences.push_back(sr);
}

void NReaction::removeFromReferencedEnd(NSpeciesReference* sr, const RxnEnd& end) {
    // search in referenced ends to find the one with the same end
    for (constREndIt rEIt = rEndsBegin(); rEIt != rEndsEnd(); ++rEIt) {
        if ((*rEIt)->end == end) {
            for (int i = 0; i < (*rEIt)->speciesReferences.size(); ++i) {
                if (stringCompare(sr->getId(), (*rEIt)->speciesReferences.at(i)->getId()))
                    (*rEIt)->speciesReferences.at(i) = NULL;
            }
        }
    }
}

SReferenceVec NReaction::getEndSpeciesReferences(const RxnEnd& end) {
    SReferenceVec srv;
    
    // search in referenced ends to find the one with the same end
    for (constREndIt rEIt = rEndsBegin(); rEIt != rEndsEnd(); ++rEIt) {
        if ((*rEIt)->end == end) {
            srv =  (*rEIt)->speciesReferences;
            break;
        }
    }
    // not referenced
    return srv;
}

void NReaction::setReferencedEnd(const rEndVec& rev) {
    _rEnds = rev;
}

const NReaction::rEndVec& NReaction::getReferencedEnds() const {
    return _rEnds;
}

const size_t NReaction::getNumREnds() const {
    return _rEnds.size();
}

void NReaction::addConnectedReaction(NReaction* r) {
    if (r) {
        connectedReaction* cReaction = findConnectedReactionByReactionId(r->getId());
        
        if (!cReaction) {
            cReaction = new connectedReaction();
            cReaction->reaction = r;
            cReaction->species.clear();
            cReaction->pseudoSpecies = NULL;
            _connectedReactions.push_back(cReaction);
        }
    }
    else
        std::cout << "No reaction to add\n";
}

void NReaction::addConnectedReaction(NReaction* r, NSpecies* s) {
    if (r && s) {
        connectedReaction* cReaction = findConnectedReactionByReactionId(r->getId());
        
        if (!cReaction) {
            cReaction = new connectedReaction();
            cReaction->reaction = r;
            cReaction->species.push_back(s);
            cReaction->pseudoSpecies = NULL;
            _connectedReactions.push_back(cReaction);
        }
        else {
            bool isSpeciesFound = false;
            for (constSpeciesIt sIt = cReaction->species.begin(); sIt != cReaction->species.end(); ++sIt)
                if (stringCompare((*sIt)->getId(), s->getId()))
                    isSpeciesFound = true;
            
            if (!isSpeciesFound)
                cReaction->species.push_back(s);
        }
    }
    else if (!r)
        std::cout << "No reaction to add\n";
    else
        std::cout << "No species to add\n";
}

void NReaction::setPseudoSpeciesOfConnectedReaction(NReaction* r, NSpecies* pseudoSpecies) {
    for (constConnectedRIt cRIt = connectedRBegin(); cRIt != connectedREnd(); ++cRIt) {
        if (stringCompare(r->getId(), (*cRIt)->reaction->getId()))
            (*cRIt)->pseudoSpecies = pseudoSpecies;
    }
}

void NReaction::removeConnectedReaction(NReaction* r) {
    for (constConnectedRIt cRIt = connectedRBegin(); cRIt != connectedREnd(); ++cRIt) {
        if (stringCompare(r->getId(), (*cRIt)->reaction->getId())) {
            _connectedReactions.erase(cRIt);
            break;
        }
    }
}

void NReaction::removeSpeciesFromConnectedReaction(NReaction* r, NSpecies* s) {
    for (constConnectedRIt cRIt = connectedRBegin(); cRIt != connectedREnd(); ++cRIt) {
        if (stringCompare(r->getId(), (*cRIt)->reaction->getId())) {
            for (constSpeciesIt sIt = (*cRIt)->species.begin(); sIt != (*cRIt)->species.end(); ++sIt) {
                if (stringCompare(s->getId(), (*sIt)->getId())) {
                    (*cRIt)->species.erase(sIt);
                    if ((*cRIt)->species.size() == 0)
                        _connectedReactions.erase(cRIt);
                    break;
                }
            }
        }
    }
}

void NReaction::unSetPseudoSpeciesFromConnectedReaction(NReaction* r) {
    for (constConnectedRIt cRIt = connectedRBegin(); cRIt != connectedREnd(); ++cRIt) {
        if (stringCompare(r->getId(), (*cRIt)->reaction->getId()))
            (*cRIt)->pseudoSpecies = NULL;
    }
}

const NReaction::connectedRVec& NReaction::getConnectedReactions() const {
    return _connectedReactions;
}

void NReaction::setConnectedReactions(const connectedRVec& crv) {
    _connectedReactions = crv;
}

const size_t NReaction::getNumConnectedReactions() const {
    return _connectedReactions.size();
}

connectedReaction* NReaction::findConnectedReactionByReactionId(const std::string& reactionId) {
    // search in connected reactions to find the desired one
    for (constConnectedRIt cRIt = connectedRBegin(); cRIt != connectedREnd(); ++cRIt) {
        if (stringCompare((*cRIt)->reaction->getId(), reactionId))
            return *cRIt;
    }
    // not found
    return NULL;
}

connectedReaction* NReaction::findConnectedReactionByReactionGlyphId(const std::string& reactionGlyphId) {
    // search in connected reactions to find the desired one using its glyph id
    for (constConnectedRIt cRIt = connectedRBegin(); cRIt != connectedREnd(); ++cRIt) {
        if (stringCompare((*cRIt)->reaction->getGlyphId(), reactionGlyphId))
            return *cRIt;
    }
    // not found
    return NULL;
}

NReaction::connectedRVec NReaction::findConnectedReactionsBySpeciesId(const std::string& speciesId) {
    connectedRVec cRV(0);
    
    // search in connected reactions to find the desired species
    for (constConnectedRIt cRIt = connectedRBegin(); cRIt != connectedREnd(); ++cRIt) {
        for (constSpeciesIt sIt = (*cRIt)->species.begin(); sIt != (*cRIt)->species.end(); ++sIt) {
            if (stringCompare((*sIt)->getId(), speciesId))
                cRV.push_back(*cRIt);
        }
    }
    
    return cRV;
}

NReaction::connectedRVec NReaction::findConnectedReactionsBySpeciesGlyphId(const std::string& speciesGlyphId) {
    connectedRVec cRV(0);
    
    // search in connected reactions to find the desired species
    for (constConnectedRIt cRIt = connectedRBegin(); cRIt != connectedREnd(); ++cRIt) {
        for (constSpeciesIt sIt = (*cRIt)->species.begin(); sIt != (*cRIt)->species.end(); ++sIt) {
            if (stringCompare((*sIt)->getGlyphId(), speciesGlyphId))
                cRV.push_back(*cRIt);
        }
    }
    
    return cRV;
}

void NReaction::addSubReaction(NReaction* sr) {
    if (sr)
        _subReactions.push_back(sr);
    else
        std::cout << "No subReaction to add\n";
}

void NReaction::removeSubReaction(int subReactionIndex) {
    if (subReactionIndex >= _subReactions.size() || subReactionIndex < 0)
        std::cerr << "the entered subReaction index is not within the subReactionVec range\n";
    else {
        // set the iterator to the desired subReaction
        constReactionIt _it = subReactionsBegin();
        for (int i = 0; i < subReactionIndex; ++i)
            ++_it;
        // remove the desired subReaction from the subReactionVec
        _subReactions.erase(_it);
    }
}

void NReaction::setSubReactions(const reactionVec& srv) {
    _subReactions = srv;
}

const reactionVec& NReaction::getSubReactions() const {
    return _subReactions;
}

const size_t NReaction::getNumSubReactions() const {
    return _subReactions.size();
}

NReaction* NReaction::findSubReactionById(const std::string& id) {
    // search in subReactions to find the desired one
    for (constReactionIt i = subReactionsBegin(); i != subReactionsEnd(); ++i) {
        if (stringCompare((*i)->getId(), id))
            return *i;
    }
    // not found
    return NULL;
}

int NReaction::findSubReactionIndexById(const std::string& id) {
    int _subReactionIndex = 0;

    // search in subReactions to find the desired one
    for (constReactionIt i = subReactionsBegin(); i != subReactionsEnd(); ++i) {
        if (stringCompare((*i)->getId(), id))
            return _subReactionIndex;
        ++_subReactionIndex;
    }
    // not found
    return -1;
}

NReaction* NReaction::findSubReactionByGlyphId(const std::string& glyphId) {
    // search in subReactions to find the desired one using its glyph id
    for (constReactionIt i = subReactionsBegin(); i != subReactionsEnd(); ++i) {
        if (stringCompare((*i)->getGlyphId(), glyphId))
            return *i;
    }
    // not found
    return NULL;
}

int NReaction::findSubReactionIndexByGlyphId(const std::string& glyphId) {
    int _subReactionIndex = 0;

    // search in subReactions to find the desired one using its glyph id
    for (constReactionIt i = subReactionsBegin(); i != subReactionsEnd(); ++i) {
        if (stringCompare((*i)->getGlyphId(), glyphId))
            return _subReactionIndex;
        ++_subReactionIndex;
    }
    // not found
    return -1;
}

void NReaction::addReactionLayer() {
    ReactionLayer* rLayer = new ReactionLayer(getNumReactionLayers(), getNumSpecies(), getSpeciesStartAngle());
    _reactionLayers.push_back(rLayer);
}

int NReaction::addSpeciesToVacancy(NSpecies* s, double& speciesAngle) {
    constRLayerIt rLIt;
    
    bool useNeighborVacancies = false;
    if (getNumConnectedReactions() == 0)
        useNeighborVacancies = true;
    
    // add it to the already existing layers
    for (rLIt = rLayersBegin(); rLIt != rLayersEnd(); ++rLIt) {
        if ((*rLIt)->addToVacancy(s, speciesAngle, useNeighborVacancies) == 0)
            return rLIt - rLayersBegin();
    }
    
    // add it to a new layer
    addReactionLayer();
    rLIt = rLayersEnd() - 1;
    if ((*rLIt)->addToVacancy(s, speciesAngle, true) == 0)
        return rLIt - rLayersBegin();
    
    return -1;
}

int NReaction::removeSpeciesFromVacancy(NSpecies* s, const double& speciesAngle) {
    
    for (constRLayerIt rLIt = rLayersBegin(); rLIt != rLayersEnd(); ++rLIt) {
        if ((*rLIt)->removeFromVacancy(s, speciesAngle) == 0)
            return rLIt - rLayersBegin();
    }
    
    return -1;
}

std::pair<int, int> NReaction::findSpeciesAddress(NSpecies* s) {
    std::pair<int, int> address;
    address.first = -1;
    address.second = -1;
    int vacancyIndex = -1;
    for (constRLayerIt rLIt = rLayersBegin(); rLIt != rLayersEnd(); ++rLIt) {
        vacancyIndex = (*rLIt)->findVacancy(s);
        if (vacancyIndex != -1) {
            address.first = rLIt - rLayersBegin();
            address.second = vacancyIndex;
            return address;
        }
    }
    
    return address;
}

int NReaction::findSpeciesLayer(NSpecies* s) {
    for (constRLayerIt rLIt = rLayersBegin(); rLIt != rLayersEnd(); ++rLIt) {
        if ((*rLIt)->findVacancy(s) != -1)
            return rLIt - rLayersBegin();
    }
    
    return -1;
}

int NReaction::transferSpeciesToNextLayerVacancy(NSpecies* s, double& speciesAngle) {
    constRLayerIt rLIt;
    
    // remove the species in the previous layers
    int layer = removeSpeciesFromVacancy(s, speciesAngle);
    if (layer != -1) {
        do {
            ++layer;
            
            // if the species used to be located in the last layer
            if (layer == rLayersEnd() - rLayersBegin())
                addReactionLayer();
            
            // set the layer iterator
            rLIt = rLayersBegin() + layer;
            
        } while ((*rLIt)->addToVacancy(s, speciesAngle, false));
    }
    
    return layer;
}

int NReaction::getNumOccupiedReactionsOfSpecies(NSpecies* s) {
    for (constRLayerIt rLIt = rLayersBegin(); rLIt != rLayersEnd(); ++rLIt) {
        int vacantIndex = (*rLIt)->findVacancy(s);
        if (vacantIndex != -1) {
            return (*rLIt)->numOfOccupiedReactionInVacancy(vacantIndex);
        }
    }
    
    return -1;
}

int NReaction::addSubReactionToVacancy(NReaction* subR, const double& jointSpeciesAngle) {
    constRLayerIt rLIt;
    // add it to the already existing layers
    for (rLIt = rLayersBegin(); rLIt != rLayersEnd(); ++rLIt) {
        if ((*rLIt)->addToVacancy(subR, jointSpeciesAngle) == 0)
            return rLIt - rLayersBegin();
    }
    
    // add it to a new layer
    addReactionLayer();
    rLIt = rLayersEnd() - 1;
    if ((*rLIt)->addToVacancy(subR, jointSpeciesAngle) == 0)
        return rLIt - rLayersBegin();
    
    return -1;
}

int NReaction::addSubReactionToVacancy(NReaction* subR, NSpecies* jointSpecies) {
    constRLayerIt rLIt;
    // add it to the already existing layers
    for (rLIt = rLayersBegin(); rLIt != rLayersEnd(); ++rLIt) {
        if ((*rLIt)->addToVacancy(subR, jointSpecies) == 0)
            return rLIt - rLayersBegin();
    }
    
    // add it to a new layer
    addReactionLayer();
    rLIt = rLayersEnd() - 1;
    if ((*rLIt)->addToVacancy(subR, jointSpecies) == 0)
        return rLIt - rLayersBegin();
    
    return -1;
}

int NReaction::removeSubReactionFromVacancy(NReaction* subR, const double& jointSpeciesAngle) {
    
    for (constRLayerIt rLIt = rLayersBegin(); rLIt != rLayersEnd(); ++rLIt) {
        if ((*rLIt)->removeFromVacancy(subR, jointSpeciesAngle) == 0)
            return rLIt - rLayersBegin();
    }
    
    return -1;
}

int NReaction::removeSubReactionFromVacancy(NReaction* subR, NSpecies* jointSpecies) {
    
    for (constRLayerIt rLIt = rLayersBegin(); rLIt != rLayersEnd(); ++rLIt) {
        if ((*rLIt)->removeFromVacancy(subR, jointSpecies) == 0)
            return rLIt - rLayersBegin();
    }
    
    return -1;
}

std::pair<int, int> NReaction::findSubReactionAddress(NReaction* subR) {
    std::pair<int , int> address;
    address.first = -1;
    address.second = -1;
    int vacancyIndex = -1;
    for (constRLayerIt rLIt = rLayersBegin(); rLIt != rLayersEnd(); ++rLIt) {
        vacancyIndex = (*rLIt)->findVacancy(subR);
        if (vacancyIndex != -1) {
            address.first = rLIt - rLayersBegin();
            address.second = vacancyIndex;
            return address;
        }
    }
    
    return address;
}

int NReaction::findSubReactionLayer(NReaction* subR) {
    for (constRLayerIt rLIt = rLayersBegin(); rLIt != rLayersEnd(); ++rLIt) {
        if ((*rLIt)->findVacancy(subR) != -1)
            return rLIt - rLayersBegin();
    }
    
    return -1;
}

int NReaction::addSpeciesReferenceToCircumferenceVacancy(NSpeciesReference* sr, NSpecies* pseudoSpecies, const int& numOfAngularTrasferringSteps) {
    int vacantIndex;
    for (constRLayerIt rLIt = rLayersBegin(); rLIt != rLayersEnd(); ++rLIt) {
        vacantIndex = (*rLIt)->addToCircumferenceVacancy(sr, pseudoSpecies, numOfAngularTrasferringSteps);
        if (vacantIndex != -1)
            return vacantIndex;
    }
    
    return vacantIndex;
}

int NReaction::removeSpeciesReferenceFromCircumferenceVacancy(NSpeciesReference* sr) {
    for (constRLayerIt rLIt = rLayersBegin(); rLIt != rLayersEnd(); ++rLIt) {
        if ((*rLIt)->removeFromCircumferenceVacancy(sr) == 0)
            return 0;
    }
    
    return -1;
}


std::vector<std::pair<int , int>> NReaction::getManhatanPathOfSpeciesReference(NSpeciesReference* sr, NSpecies* pseudoSpecies, const int& numAngularTransferringSteps, const int& numRadialTransferringSteps) {
    std::pair<int , int> pathStep;
    std::vector<std::pair<int , int>> manhatanPath(0);
    std::pair<int, int> address = findSpeciesAddress(pseudoSpecies);
    std::vector <std::pair<int, int>> addressesInReaction;
    addressesInReaction.push_back(address);
    int remainedATSteps = numAngularTransferringSteps;
    int remainedRTSteps = numRadialTransferringSteps;
    int aTStep = 1;
    if (numAngularTransferringSteps < 0)
        aTStep*= -1;
    bool isPathAdded = true;
    bool reachedImpasse = false;
    
    while (!reachedImpasse) {
        while ((remainedATSteps || remainedRTSteps) && !reachedImpasse) {
            address = addressesInReaction.back();
            if (isPathAdded) {
                isPathAdded = false;
                switch (manhatanPath.size() % 2) {
                    case 0:
                        if (remainedATSteps && (*(rLayersBegin() + address.first))->checkCircumferenceVacancy(address.second, aTStep) != -1) {
                            address.second += aTStep;
                                
                            while (address.second >= int((*(rLayersBegin() + address.first))->getNumVacancies()))
                                address.second -= (*(rLayersBegin() + address.first))->getNumVacancies();
                            while (address.second < 0)
                                address.second += (*(rLayersBegin() + address.first))->getNumVacancies();
                            
                            addressesInReaction.push_back(address);
                            pathStep.first = aTStep;
                            manhatanPath.push_back(pathStep);
                            isPathAdded = true;
                            remainedATSteps -= aTStep;
                        }
                        
                        if (!manhatanPath.size() && !isPathAdded) {
                            pathStep.first = 0;
                            manhatanPath.push_back(pathStep);
                            isPathAdded = true;
                        }
                        break;
                        
                    case 1:
                        if (remainedRTSteps && (*(rLayersBegin() + address.first - 1))->checkRadialVacancy(address.second) != -1) {
                            address.first -= 1;
                            
                            addressesInReaction.push_back(address);
                            pathStep.first = 1;
                            manhatanPath.push_back(pathStep);
                            isPathAdded = true;
                            --remainedRTSteps;
                        }
                }
            }
            else {
                reachedImpasse = true;
                switch (manhatanPath.size() % 2) {
                    case 0:
                        if (remainedRTSteps && (*(rLayersBegin() + address.first - 1))->checkRadialVacancy(address.second) != -1) {
                            address.first -= 1;
                            
                            addressesInReaction.push_back(address);
                            ++manhatanPath.back().first;
                            isPathAdded = true;
                            reachedImpasse = false;
                            --remainedRTSteps;
                        }
                        break;
                        
                    case 1:
                        if (remainedATSteps && (*(rLayersBegin() + address.first))->checkCircumferenceVacancy(address.second, aTStep) != -1 && manhatanPath.back().first) {
                            address.second += aTStep;
                            while (address.second >= int((*(rLayersBegin() + address.first))->getNumVacancies()))
                                address.second -= (*(rLayersBegin() + address.first))->getNumVacancies();
                            while (address.second < 0)
                                address.second += (*(rLayersBegin() + address.first))->getNumVacancies();
                            
                            addressesInReaction.push_back(address);
                            manhatanPath.back().first += aTStep;
                            isPathAdded = true;
                            reachedImpasse = false;
                            remainedATSteps -= aTStep;
                        }
                }
            }
        }
        
        if (!remainedATSteps && !remainedRTSteps)
            break;
        else if (reachedImpasse && manhatanPath.size() && !(manhatanPath.size() == 1 && !manhatanPath.back().first)) {
            if (manhatanPath.size() % 2 == 0) {
                while (manhatanPath.back().first) {
                    ++remainedRTSteps;
                    --manhatanPath.back().first;
                    addressesInReaction.pop_back();
                }
                manhatanPath.pop_back();
                isPathAdded = false;
            }
            else {
                while (manhatanPath.back().first) {
                    remainedATSteps += aTStep;
                    manhatanPath.back().first -= aTStep;
                    addressesInReaction.pop_back();
                }
                if (manhatanPath.size() != 1)
                    manhatanPath.pop_back();
                else
                    isPathAdded = true;
            }
            
            reachedImpasse = false;
        }
    }
    
    if (reachedImpasse) {
        manhatanPath.clear();
        pathStep.first = numAngularTransferringSteps;
        manhatanPath.push_back(pathStep);
        pathStep.first = numRadialTransferringSteps;
        manhatanPath.push_back(pathStep);
    }
    
    address = findSpeciesAddress(pseudoSpecies);
    for (int i = 0; i < manhatanPath.size(); ++i) {
        if (i % 2) {
            for (int j = 0; j < manhatanPath.at(i).first; ++j) {
                manhatanPath.at(i).second = (*(rLayersBegin() + address.first))->addToRadialVacancy(sr, address.second);
                address.first -= 1;
            }
        }
        else {
            manhatanPath.at(i).second = (*(rLayersBegin() + address.first))->addToCircumferenceVacancy(sr, address.second, manhatanPath.at(i).first);
            address.second += manhatanPath.at(i).first;
            while (address.second >= int((*(rLayersBegin() + address.first))->getNumVacancies()))
                address.second -= (*(rLayersBegin() + address.first))->getNumVacancies();
            while (address.second < 0)
                address.second += (*(rLayersBegin() + address.first))->getNumVacancies();
        }
    }
    
    return manhatanPath;
}

const NReaction::rLayerVec& NReaction::getReactionLayers() const {
    return _reactionLayers;
}

const size_t NReaction::getNumReactionLayers() const {
    return _reactionLayers.size();
}

void NReaction::setCenter(const LPoint& centerP) {
    _centerP = centerP;
    _isSetCenterP = true;
}

const LPoint& NReaction::getCenter() const {
    return _centerP;
}

void NReaction::setRadius(const double& radius) {
    _radius = radius;
    _isSetRadius = true;
}

const double& NReaction::getRadius() const {
    return _radius;
}

void NReaction::setDirectionAngle(const double& directionAngle) {
    _directionAngle = directionAngle;
    _isSetDirectionAngle = true;
}

const double& NReaction::getDirectionAngle() const {
    return _directionAngle;
}

void NReaction::setSpeciesStartAngle(const double& angle) {
    _speciesStartAngle = angle;
    _isSetSpeciesStartAngle = true;
}

const double& NReaction::getSpeciesStartAngle() const {
    return _speciesStartAngle;
}

void NReaction::shiftItems(const double& dx, const double& dy) {
    LPoint point;
    
    // for reaction
    // center
    if (isSetCenter()) {
        point = getCenter();
        point.shift(dx, dy);
        setCenter(point);
    }
    // curve
    if (isSetCurve())
        getCurve()->shiftPosition(dx, dy);
    
    // for subReactions
    for (constReactionIt sRIt = subReactionsBegin(); sRIt != subReactionsEnd(); ++sRIt) {
        // center
        if ((*sRIt)->isSetCenter()) {
            point = (*sRIt)->getCenter();
            point.shift(dx, dy);
            (*sRIt)->setCenter(point);
        }
        // curve
        if ((*sRIt)->isSetCurve())
            (*sRIt)->getCurve()->shiftPosition(dx, dy);
        
        // sub species references
        for (constSReferenceIt sSRIt = (*sRIt)->sReferencesBegin(); sSRIt != (*sRIt)->sReferencesEnd(); ++sSRIt) {
            if ((*sSRIt)->isSetCurve())
                (*sSRIt)->getCurve()->shiftPosition(dx, dy);
        }
    }
    
    // for species
    for (constSpeciesIt sIt = speciesBegin(); sIt != speciesEnd(); ++sIt) {
        if ((*sIt)->isSetBox())
            (*sIt)->getBox()->shiftPosition(dx, dy);
    }
    
    // for subSpecies
    for (constSpeciesIt sSIt = subSpeciesBegin(); sSIt != subSpeciesEnd(); ++sSIt) {
        if ((*sSIt)->isSetBox())
            (*sSIt)->getBox()->shiftPosition(dx, dy);
    }
    
    // for pseudoSpecies
    for (constSpeciesIt pSIt = pseudoSpeciesBegin(); pSIt != pseudoSpeciesEnd(); ++pSIt) {
        if ((*pSIt)->isSetBox())
            (*pSIt)->getBox()->shiftPosition(dx, dy);
    }
    
    // for species references
    for (constSReferenceIt sRIt = sReferencesBegin(); sRIt != sReferencesEnd(); ++sRIt) {
        if ((*sRIt)->isSetCurve())
            (*sRIt)->getCurve()->shiftPosition(dx, dy);
    }
}

void NReaction::calculateExtents() {
    LPoint minP(INT_MAX, INT_MAX);
    LPoint maxP(INT_MIN, INT_MIN);
    LBox* curveBox;
    
    // for reaction
    // center
    if (isSetCenter()) {
        minP = pointMin(getCenter(), minP);
        maxP = pointMax(getCenter(), maxP);
    }
    // curve
    if (isSetCurve()) {
        curveBox = getCurve()->getExtentBox();
        minP = pointMin(curveBox->upperLeftCorner(), minP);
        maxP = pointMax(curveBox->bottomRightCorner(), maxP);
    }
    
    // for subReactions
    for (constReactionIt sRIt = subReactionsBegin(); sRIt != subReactionsEnd(); ++sRIt) {
        // center
        if ((*sRIt)->isSetCenter()) {
            minP = pointMin((*sRIt)->getCenter(), minP);
            maxP = pointMax((*sRIt)->getCenter(), maxP);
        }
        // curve
        if ((*sRIt)->isSetCurve()) {
            curveBox = (*sRIt)->getCurve()->getExtentBox();
            minP = pointMin(curveBox->upperLeftCorner(), minP);
            maxP = pointMax(curveBox->bottomRightCorner(), maxP);
        }
    }
    
    // for species
    for (constSpeciesIt sIt = speciesBegin(); sIt != speciesEnd(); ++sIt) {
        if ((*sIt)->isSetBox()) {
            minP = pointMin((*sIt)->getBox()->upperLeftCorner(), minP);
            maxP = pointMax((*sIt)->getBox()->bottomRightCorner(), maxP);
        }
    }
    
    // for subSpecies
    for (constSpeciesIt sSIt = subSpeciesBegin(); sSIt != subSpeciesEnd(); ++sSIt) {
        if ((*sSIt)->isSetBox()) {
            minP = pointMin((*sSIt)->getBox()->upperLeftCorner(), minP);
            maxP = pointMax((*sSIt)->getBox()->bottomRightCorner(), maxP);
        }
    }
    
    // for pseudoSpecies
    for (constSpeciesIt pSIt = pseudoSpeciesBegin(); pSIt != pseudoSpeciesEnd(); ++pSIt) {
        if ((*pSIt)->isSetBox()) {
            minP = pointMin((*pSIt)->getBox()->upperLeftCorner(), minP);
            maxP = pointMax((*pSIt)->getBox()->bottomRightCorner(), maxP);
        }
    }
    
    // for species references
    for (constSReferenceIt sRIt = sReferencesBegin(); sRIt != sReferencesEnd(); ++sRIt) {
        if ((*sRIt)->isSetCurve()) {
            curveBox = (*sRIt)->getCurve()->getExtentBox();
            minP = pointMin(curveBox->upperLeftCorner(), minP);
            maxP = pointMax(curveBox->bottomRightCorner(), maxP);
        }
    }
    
    // for sub species references
    for (constSReferenceIt sRIt = subSReferencesBegin(); sRIt != subSReferencesEnd(); ++sRIt) {
        if ((*sRIt)->isSetCurve()) {
            curveBox = (*sRIt)->getCurve()->getExtentBox();
            minP = pointMin(curveBox->upperLeftCorner(), minP);
            maxP = pointMax(curveBox->bottomRightCorner(), maxP);
        }
    }
    
    _extentBox->setX(minP.x() - 0.1 * (maxP.x() - minP.x()));
    _extentBox->setY(minP.y() - 0.1 * (maxP.y() - minP.y()));
    if (1.2 * (maxP.x() - minP.x()) > 150.0)
        _extentBox->setWidth(1.2 * (maxP.x() - minP.x()));
    else {
        shiftItems(60.0, 0.0);
        _extentBox->setWidth(150.0);
    }
        
    
    if (1.2 * (maxP.y() - minP.y()) > 150.0)
        _extentBox->setHeight(1.2 * (maxP.y() - minP.y()));
    else {
        shiftItems(0.0, 60.0);
        _extentBox->setHeight(150.0);
    }
}

LBox* NReaction::getExtentBox() {
    calculateExtents();
    return _extentBox;
}

//--CLASS NSpeciesReference--

NSpeciesReference::NSpeciesReference(NSpeciesReference& sR) : NGraphicalObject(sR) {
    _type = GRP_OBJ_SPC_RFC;
    _species = NULL;
    _reaction = NULL;
    _curve = NULL;
    _startPoint = LPoint(0.0, 0.0);
    _endPoint = LPoint(0.0, 0.0);
    _isSetSpecies = false;
    _isSetReaction = false;
    _isSetCurve = false;
    _isSetRole = false;
    _isSetQuadrant = false;
    _isSetStartPoint = false;
    _isSetEndPoint = false;
    
    // curve
    if (sR.isSetCurve())
        setCurve(new LCurve(*(sR.getCurve())));
    
    // role
    if (sR.isSetRole())
        setRole(sR.getRole());
}

void NSpeciesReference::setSpecies(NReaction* r, NSpecies* s) {
    if (r && s) {
        _species = s;
        _reaction = r;
        _isSetSpecies = true;
        _isSetReaction = true;
    }
    else if (!s)
        std::cout << "No species to set\n";
    else if (!r)
        std::cout << "No reaction to set\n";
    
}

NSpecies* NSpeciesReference::getSpecies() const {
    return _species;
}

NReaction* NSpeciesReference::getReaction() const {
    return _reaction;
}

void NSpeciesReference::setCurve(LCurve* curve) {
    if (curve) {
        _curve = curve;
        _isSetCurve = true;
    }
    else
        std::cout << "No curve to set\n";
}

void NSpeciesReference::unSetCurve(const bool& removeCurve) {
    _isSetCurve = false;
    if (removeCurve) {
        delete _curve;
        _curve = NULL;
    }
}

LCurve* NSpeciesReference::getCurve() const {
    return _curve;
}

void NSpeciesReference::setRole(const RxnRoleType& role) {
    _role = role;
    _isSetRole = true;
}

const RxnRoleType& NSpeciesReference::getRole() const {
    return _role;
}

void NSpeciesReference::setQuadrant(const CoordSysQuad& quadrant) {
    _quadrant = quadrant;
    _isSetQuadrant = true;
}

const CoordSysQuad& NSpeciesReference::getQuadrant() const {
    return _quadrant;
}

void NSpeciesReference::setStartPoint(const LPoint& startPoint) {
    _startPoint = startPoint;
    _isSetStartPoint = true;
}

const LPoint& NSpeciesReference::getStartPoint() const {
    return _startPoint;
}

void NSpeciesReference::setEndPoint(const LPoint& endPoint) {
    _endPoint = endPoint;
    _isSetEndPoint = true;
}

const LPoint& NSpeciesReference::getEndPoint() const {
    return _endPoint;
}

//--CLASS NText--

NText::NText(NText& t) : NGraphicalObject(t) {
    _type = GRP_OBJ_TXT;
    _isSetGraphicalObjectId = false;
    _isSetText = false;
    _isSetOriginOfTextId = false;
    
    // graphical object id
    if (t.isSetGraphicalObjectId())
        setGraphicalObjectId(t.getGraphicalObjectId());
    
    // text
    if (t.isSetText())
        setText(t.getText());
    
    // origin of text
    if (t.isSetOriginOfTextId())
        setOriginOfTextId(t.getOriginOfTextId());
}

void NText::setGraphicalObjectId(const std::string& graphicalObjectId) {
    _graphicalObjectId = graphicalObjectId;
    _isSetGraphicalObjectId = true;
}

const std::string& NText::getGraphicalObjectId() const {
    return _graphicalObjectId;
}

void NText::setText(const std::string& text) {
    _text = text;
    _isSetText = true;
}

void NText::unSetText() {
    _text.clear();
    _isSetText = false;
}

const std::string& NText::getText() const {
    return _text;
}

void NText::setOriginOfTextId(const std::string& originOfTextId) {
    _originOfTextId = originOfTextId;
    _isSetOriginOfTextId = true;
}

const std::string& NText::getOriginOfTextId() const {
    return _originOfTextId;
}

//--CLASS LCurve--

LCurve::LCurve(const LCurve& c) : NetworkElement(c) {
    _listOfElements.clear();
    _extentBox = new LBox();
    
    // elements
    LLineSegment* line = NULL;
    for (constElementIt eIt = c.elementsBegin(); eIt != c.elementsEnd(); ++eIt) {
        if (!(*eIt)->isCubicBezier())
            line = new LLineSegment(*(*eIt));
        else
            line = new LCubicBezier(*((LCubicBezier*)(*eIt)));
        addToListOfElementsEnd(line);
    }
}

void LCurve::addToListOfElementsEnd(LLineSegment* l) {
    if (l)
        _listOfElements.push_back(l);
    else
        std::cout << "No line segment to add\n";
}

void LCurve::addToListOfElements(LLineSegment* l, unsigned int index) {
    if (l)
        _listOfElements.insert(_listOfElements.begin() + index, l);
    else
        std::cout << "No line segment to add\n";
}

void LCurve::removeFromListOfElements(unsigned int index) {
    constElementIt _it = elementsBegin();
    for (int i = 0; i < index; ++i)
        ++_it;
    _listOfElements.erase(_it);
}

void LCurve::clearListOfElements() {
    _listOfElements.clear();
}

const LCurve::listOfElements& LCurve::getListOfElements() const {
    return _listOfElements;
}

const size_t LCurve::getNumElements() const {
    return _listOfElements.size();
}

LLineSegment* LCurve::findElementById(const std::string& id) {
    // search in the elements to find the one with the same id
    for (constElementIt i = elementsBegin(); i != elementsEnd(); ++i) {
        if (stringCompare((*i)->getId(), id))
            return *i;
    }
    // not found
    return NULL;
}

int LCurve::findElementIndexById(const std::string& id) {
    int _elementIndex = 0;

    // search in the elements to find the one with the same id
    for (constElementIt i = elementsBegin(); i != elementsEnd(); ++i) {
        if (stringCompare((*i)->getId(), id))
            return _elementIndex;
        ++_elementIndex;
    }
    // not found
    return -1;
}

std::string LCurve::getElementUniqueId() {
    size_t k = 0;
    LLineSegment* l = NULL;

    // create a new line segment id and search in the exisiting ones to ensure it has not already used
    while (1) {
        ++k;
        std::stringstream ss;
        ss << "LineSegment_" << k;
        l = findElementById(ss.str());
        if (!l)
            return ss.str();
    }
}

void LCurve::shiftPosition(const double& dx, const double& dy) {
    LPoint* point = NULL;
    
    for (constElementIt eIt = elementsBegin(); eIt != elementsEnd(); ++eIt) {
        // for start point
        if ((*eIt)->isSetStart()) {
            point = (*eIt)->getStart();
            point->shift(dx, dy);
        }
        
        // for end point
        if ((*eIt)->isSetEnd()) {
            point = (*eIt)->getEnd();
            point->shift(dx, dy);
        }
        
        // if it is a cubic bezier
        if ((*eIt)->isCubicBezier()) {
            // for basepoint1
            if (((LCubicBezier*)(*eIt))->isSetBasePoint1()) {
                point = ((LCubicBezier*)(*eIt))->getBasePoint1();
                point->shift(dx, dy);
            }
            
            // for basepoint2
            if (((LCubicBezier*)(*eIt))->isSetBasePoint2()) {
                point = ((LCubicBezier*)(*eIt))->getBasePoint2();
                point->shift(dx, dy);
            }
        }
    }
}

LBox* LCurve::getExtentBox() {
    LPoint minP(INT_MAX, INT_MAX);
    LPoint maxP(INT_MIN, INT_MIN);
    
    for (constElementIt eIt = elementsBegin(); eIt != elementsEnd(); ++eIt) {
        // for start point
        if ((*eIt)->isSetStart()) {
            minP = pointMin(*(*eIt)->getStart(), minP);
            maxP = pointMax(*(*eIt)->getStart(), maxP);
        }
        
        // for end point
        if ((*eIt)->isSetEnd()) {
            minP = pointMin(*(*eIt)->getEnd(), minP);
            maxP = pointMax(*(*eIt)->getEnd(), maxP);
        }
        
        // if it is a cubic bezier
        if ((*eIt)->isCubicBezier()) {
            // if it is an arc-like bezier
            if (((LCubicBezier*)(*eIt))->isArcLike()) {
                // for the zenith of the arc
                if (((LCubicBezier*)(*eIt))->isSetBasePoint1() && ((LCubicBezier*)(*eIt))->isSetBasePoint2()) {
                    LPoint arcCenter = 0.125 * *(*eIt)->getStart() + 0.375 * *((LCubicBezier*)(*eIt))->getBasePoint1() + 0.375 * *((LCubicBezier*)(*eIt))->getBasePoint2() + 0.125 * *(*eIt)->getEnd();
                    minP = pointMin(arcCenter, minP);
                    maxP = pointMax(arcCenter, maxP);
                }
            }
            else {
                // for basepoint1
                if (((LCubicBezier*)(*eIt))->isSetBasePoint1()) {
                    minP = pointMin(*((LCubicBezier*)(*eIt))->getBasePoint1(), minP);
                    maxP = pointMax(*((LCubicBezier*)(*eIt))->getBasePoint1(), maxP);
                }
                
                // for basepoint2
                if (((LCubicBezier*)(*eIt))->isSetBasePoint2()) {
                    minP = pointMin(*((LCubicBezier*)(*eIt))->getBasePoint2(), minP);
                    maxP = pointMax(*((LCubicBezier*)(*eIt))->getBasePoint2(), maxP);
                }
            }
        }
    }
    
    *_extentBox = LBox(minP, maxP);
    return _extentBox;
}

//--CLASS LLineSegment--

LLineSegment::LLineSegment(LLineSegment& line) : NetworkElement(line) {
    // start
    if (line.isSetStart())
        setStart(*line.getStart());
    else {
        _start = new LPoint(0.0, 0.0);
        _isSetStart = false;
    }
    
    // end
    if (line.isSetEnd())
        setEnd(*line.getEnd());
    else {
        _end = new LPoint(0.0, 0.0);
        _isSetEnd = false;
    }
}

void LLineSegment::setStart(const LPoint& p) {
    *_start = p;
    _isSetStart = true;
}

LPoint* LLineSegment::getStart() {
    return _start;
}

void LLineSegment::setEnd(const LPoint& p) {
    *_end = p;
    _isSetEnd = true;
}

LPoint* LLineSegment::getEnd() {
    return _end;
}

//--CLASS LCubicBezier--

LCubicBezier::LCubicBezier(LCubicBezier& cb) : LLineSegment(cb) {
    _isArcLike = false;
    
    // basepoint1
    if (cb.isSetBasePoint1())
        setBasePoint1(*cb.getBasePoint1());
    else {
        _basePoint1 = new LPoint(0.0, 0.0);
        _isSetBasePoint1 = false;
    }
    
    // basepoint2
    if (cb.isSetBasePoint2())
        setBasePoint2(*cb.getBasePoint2());
    else {
        _basePoint2 = new LPoint(0.0, 0.0);
        _isSetBasePoint2 = false;
    }
}

void LCubicBezier::setBasePoint1(const LPoint& p) {
    *_basePoint1 = p;
    _isSetBasePoint1 = true;
}


LPoint* LCubicBezier::getBasePoint1() {
    return _basePoint1;
}

void LCubicBezier::setBasePoint2(const LPoint& p) {
    *_basePoint2 = p;
    _isSetBasePoint2 = true;
}

LPoint* LCubicBezier::getBasePoint2() {
    return _basePoint2;
}

void LCubicBezier::setArcLike(bool value) {
    _isArcLike = value;
}

//--CLASS ReactionLayer--
ReactionLayer::ReactionLayer(const unsigned int& layer, const unsigned int& numReactionSpecies, const double& speciesStartAngle) {
    _layer = layer;
    _dAngle = pi / numReactionSpecies;
    
    vacancy* v;
    circumferenceVacancy* cV;
    double angle = speciesStartAngle;
        
    for (int i = 0; i < 2 * numReactionSpecies; ++i) {
        v = new vacancy();
        
        // set angle
        if (angle >= 2 * pi)
            angle -= 2 * pi;
        v->angle = angle;
        angle += _dAngle;
    
        // set species
        v->species = NULL;
        
        // set reactions
        v->reactions.clear();
        
        // set radial vacancy
        v->rVacancy.sReferences.clear();
        
        // add vacancy to the reaction vacancies
        _vacancies.push_back(v);
    }
    
    for (int i = 0; i < 2 * numReactionSpecies; ++i) {
        /// the first part
        cV = new circumferenceVacancy();
        
        // set start vacancy
        cV->startVacancy = _vacancies.at(i);
        
        // set end vacancy
        if (i != 2 * numReactionSpecies - 1)
            cV->endVacancy = _vacancies.at(i + 1);
        else
            cV->endVacancy = _vacancies.at(0);
        
        // set species references
        cV->sReferences.clear();
        
        // add vacancy to the reaction vacancies
        _circumferenceVacancies.push_back(cV);
    }
}

const unsigned int& ReactionLayer::getLayer() const {
    return _layer;
}

int ReactionLayer::addToVacancy(NSpecies* s, double& speciesAngle, const bool& useNeighborVacancies) {
    double bottomAngle;
    double upperAngle;
    double _speciesAngle = speciesAngle;
    if (std::floor(_speciesAngle) < 0)
        _speciesAngle += 2 * pi;
    if (_speciesAngle > 2 * pi)
        _speciesAngle -= 2 * pi;
    
    constVacancyIt vIt = vacanciesBegin();
    if (_layer % 2 && !s->isPseudoSpecies())
        ++vIt;
    
    while (vIt < vacanciesEnd()) {
        if (s->isPseudoSpecies()) {
            bottomAngle = (*vIt)->angle - 0.5 * _dAngle;
            upperAngle = (*vIt)->angle + 0.5 * _dAngle;
        }
        else {
            bottomAngle = (*vIt)->angle - _dAngle;
            upperAngle = (*vIt)->angle + _dAngle;
        }
        
        if ((_speciesAngle >= bottomAngle && _speciesAngle < upperAngle) || (_speciesAngle >= bottomAngle + 2 * pi && _speciesAngle < upperAngle + 2 * pi) || (_speciesAngle >= bottomAngle - 2 * pi && _speciesAngle < upperAngle - 2 * pi)) {
            
            // if the vacancy is occupied, check the adjacent vacancies
            if ((*vIt)->species  && useNeighborVacancies) {
                
                // for pseudospecies
                if (s->isPseudoSpecies()) {
                    // put it to the previous vacancy
                    // the current vacancy is not the first one in the layer
                    if (vIt != vacanciesBegin()) {
                        if (!(*(vIt - 1))->species)
                            --vIt;
                    }
                    // the current vacancy is the first one in the layer
                    else if (!(*(vacanciesEnd() - 1))->species)
                        vIt = vacanciesEnd() - 1;
                    
                    // the species angle is closer to the next vacancy
                    if ((upperAngle - speciesAngle) > (speciesAngle - bottomAngle)) {
                        // the current vacancy is not the last one in the layer
                        if (vIt + 1 != vacanciesEnd()) {
                            if (!(*(vIt + 1))->species)
                                ++vIt;
                        }
                        // the current vacancy is the last one in the layer
                        else if (!(*vacanciesBegin())->species)
                            vIt = vacanciesBegin();
                    }
                }
                // for species
                else {
                    // put it to the previous vacancy
                    // the current vacancy is not the first one in the layer
                    if ((_layer % 2 == 0 && vIt != vacanciesBegin()) || (_layer % 2 == 1 && vIt != vacanciesBegin() + 1)) {
                        if (!(*(vIt - 2))->species)
                            vIt = vIt - 2;
                    }
                    // the current vacancy is the first one in the layer
                    else if (_layer % 2 == 0 && !(*(vacanciesEnd() - 2))->species)
                        vIt = vacanciesEnd() - 2;
                    else if (_layer % 2 == 1 && !(*(vacanciesEnd() - 1))->species)
                        vIt = vacanciesEnd() - 1;
                    
                    // the species angle is closer to the next vacancy
                    if ((upperAngle - speciesAngle) > (speciesAngle - bottomAngle)) {
                        // the current vacancy is not the last one in the layer
                        if ((_layer % 2 == 0 && vIt + 2 != vacanciesEnd()) || (_layer % 2 == 1 && vIt + 1 != vacanciesEnd())) {
                            if (!(*(vIt + 2))->species)
                                vIt = vIt + 2;
                        }
                        // the current vacancy is the last one in the layer
                        else if (_layer % 2 == 0 && !(*vacanciesBegin())->species)
                            vIt = vacanciesBegin();
                        else if (_layer % 2 == 1 && !(*vacanciesBegin() + 1)->species)
                            vIt = vacanciesBegin() + 1;
                    }
                }
            }
            
            if (!(*vIt)->species) {
                (*vIt)->species = s;
                if (!s->isUsed())
                    speciesAngle = (*vIt)->angle;
                
                return 0;
            }
            
            break;
        }
        else {
            if (s->isPseudoSpecies())
                ++vIt;
            else
                vIt = vIt + 2;
        }
    }
    
    // the layer is already occupied
    return -1;
}

int ReactionLayer::removeFromVacancy(NSpecies* s, const double& speciesAngle) {
       double bottomAngle;
       double upperAngle;
       double _speciesAngle = speciesAngle;
       if (std::floor(_speciesAngle) < 0)
           _speciesAngle += 2 * pi;
       if (_speciesAngle > 2 * pi)
           _speciesAngle -= 2 * pi;
       
       for (constVacancyIt vIt = vacanciesBegin(); vIt != vacanciesEnd(); ++vIt) {
           bottomAngle = (*vIt)->angle - 0.5 * _dAngle;
           upperAngle = (*vIt)->angle + 0.5 * _dAngle;
               
           if ((_speciesAngle >= bottomAngle && _speciesAngle < upperAngle) || (_speciesAngle >= bottomAngle + 2 * pi && _speciesAngle < upperAngle + 2 * pi)) {
               
               if ((*vIt)->species) {
                   if (stringCompare(s->getId(), (*vIt)->species->getId())) {
                       (*vIt)->species = NULL;
                       return 0;
                   }
               }
               
               break;
           }
       }
       
       // is not occupied by this species
       return -1;
   }

int ReactionLayer::addToVacancy(NReaction* r, const double& jointSpeciesAngle) {
    double bottomAngle;
    double upperAngle;
    
    for (constVacancyIt vIt = vacanciesBegin(); vIt != vacanciesEnd(); ++vIt) {
        bottomAngle = (*vIt)->angle - 0.5 * _dAngle;
        if (bottomAngle < int(0))
            bottomAngle += 2 * pi;
        upperAngle = (*vIt)->angle + 0.5 * _dAngle;
        if (upperAngle >= 2 * pi)
            upperAngle -= 2 * pi;
        
        if ((jointSpeciesAngle >= bottomAngle && jointSpeciesAngle < upperAngle) || (jointSpeciesAngle + 2 * pi >= bottomAngle && jointSpeciesAngle < upperAngle) || (jointSpeciesAngle + 2 * pi >= bottomAngle && jointSpeciesAngle < upperAngle)) {
            if ((*vIt)->reactions.size() < 3) {
                (*vIt)->reactions.push_back(r);
                return 0;
            }
            
            break;
        }
    }
    
    // is already fully occupied
    return -1;
}

int ReactionLayer::addToVacancy(NReaction* r, NSpecies* jointSpecies) {
    for (constVacancyIt vIt = vacanciesBegin(); vIt != vacanciesEnd(); ++vIt) {
        if ((*vIt)->species) {
            if (stringCompare(jointSpecies->getId(), (*vIt)->species->getId())) {
                if ((*vIt)->reactions.size() < 3) {
                    (*vIt)->reactions.push_back(r);
                    return 0;
                }
                
                break;
            }
        }
    }
    
    // is either already fully occupied or could not find the species
    return -1;
}

int ReactionLayer::removeFromVacancy(NReaction* r, const double& jointSpeciesAngle) {
    double bottomAngle;
    double upperAngle;
    
    for (constVacancyIt vIt = vacanciesBegin(); vIt != vacanciesEnd(); ++vIt) {
        bottomAngle = (*vIt)->angle - 0.5 * _dAngle;
        if (bottomAngle < int(0))
            bottomAngle += 2 * pi;
        upperAngle = (*vIt)->angle + 0.5 * _dAngle;
        if (upperAngle >= 2 * pi)
            upperAngle -= 2 * pi;
        
        if ((jointSpeciesAngle >= bottomAngle && jointSpeciesAngle < upperAngle) || (jointSpeciesAngle + 2 * pi >= bottomAngle && jointSpeciesAngle < upperAngle) || (jointSpeciesAngle + 2 * pi >= bottomAngle && jointSpeciesAngle < upperAngle)) {
            for (int i = 0; i < (*vIt)->reactions.size(); ++i) {
                if (stringCompare(r->getId(), (*vIt)->reactions.at(i)->getId())) {
                    (*vIt)->reactions.erase((*vIt)->reactions.begin() + i);
                    return 0;
                }
            }
            
            break;
        }
    }
    
    // is not occupied by this reaction
    return -1;
}

int ReactionLayer::removeFromVacancy(NReaction* r, NSpecies* jointSpecies) {
    for (constVacancyIt vIt = vacanciesBegin(); vIt != vacanciesEnd(); ++vIt) {
        if ((*vIt)->species) {
            if (stringCompare(jointSpecies->getId(), (*vIt)->species->getId())) {
                for (int i = 0; i < (*vIt)->reactions.size(); ++i) {
                    if (stringCompare(r->getId(), (*vIt)->reactions.at(i)->getId())) {
                        (*vIt)->reactions.erase((*vIt)->reactions.begin() + i);
                        return 0;
                    }
                }
                
                break;
            }
        }
    }
    
    // is not occupied by either this reaction or this species
    return -1;
}

int ReactionLayer::swapSpecies(NSpecies* s1, NSpecies* s2) {
    int s1VacancyIndex = -1;
    int s2VacancyIndex = -1;
    
    for (constVacancyIt vIt = vacanciesBegin(); vIt != vacanciesEnd(); ++vIt) {
        if ((*vIt)->species) {
            if (stringCompare(s1->getId(), (*vIt)->species->getId()))
                s1VacancyIndex = vIt - vacanciesBegin();
            else if (stringCompare(s2->getId(), (*vIt)->species->getId()))
                s2VacancyIndex = vIt - vacanciesBegin();
        }
    }
    
    if (s1VacancyIndex != -1 && s2VacancyIndex != -1) {
        _vacancies.at(s1VacancyIndex)->species = s2;
        _vacancies.at(s2VacancyIndex)->species = s1;
        return 0;
    }
    
    return -1;
}

int ReactionLayer::findVacancy(NSpecies* s) {
    for (constVacancyIt vIt = vacanciesBegin(); vIt != vacanciesEnd(); ++vIt) {
        if ((*vIt)->species) {
            if (stringCompare(s->getId(), (*vIt)->species->getId()))
                return (vIt - vacanciesBegin());
        }
    }
    
    // is not occupied by this species
    return -1;
}

int ReactionLayer::findVacancy(NReaction* r) {
    for (constVacancyIt vIt = vacanciesBegin(); vIt != vacanciesEnd(); ++vIt) {
        for (int i = 0; i < (*vIt)->reactions.size(); ++i) {
            if (stringCompare(r->getId(), (*vIt)->reactions.at(i)->getId()))
                return (vIt - vacanciesBegin());
        }
    }
    
    // is not occupied by this reaction
    return -1;
}

unsigned int ReactionLayer::numOfOccupiedReactionInVacancy(const unsigned int& vacancyIndex) {
    return _vacancies.at(vacancyIndex)->reactions.size();
}

const size_t ReactionLayer::getNumVacancies() const {
    return _vacancies.size();
}

int ReactionLayer::addToCircumferenceVacancy(NSpeciesReference* sr, NSpecies* pseudoSpecies, const int& numOfAngualrTrasferringSteps) {
    constCVacancyIt cVIt, _cVIt;
    for (cVIt = cVacanciesBegin(); cVIt != cVacanciesEnd(); ++cVIt) {
        if ((*cVIt)->startVacancy && (*cVIt)->startVacancy->species &&  stringCompare((*cVIt)->startVacancy->species->getId(), pseudoSpecies->getId()))
            break;
    }
    
    if (cVIt != cVacanciesEnd()) {
        int signStep = 1;
        if (numOfAngualrTrasferringSteps < 0)
            signStep *= -1;
        int vacantIndex;
        bool isAlreadyOccupied = true;
        for (vacantIndex = 0; isAlreadyOccupied ; ++vacantIndex) {
            isAlreadyOccupied = false;
            for (int i = 0; i != numOfAngualrTrasferringSteps && !isAlreadyOccupied; i += signStep) {
                _cVIt = cVIt + i;
                if (signStep == -1)
                    --_cVIt;
                                    
                while (_cVIt - cVacanciesBegin() >= cVacanciesEnd() - cVacanciesBegin())
                    _cVIt -= cVacanciesEnd() - cVacanciesBegin();
                
                while (_cVIt - cVacanciesBegin() < 0)
                    _cVIt += cVacanciesEnd() - cVacanciesBegin();
                
                for (int j = 0; j < (*_cVIt)->sReferences.size(); ++j) {
                    if (vacantIndex == (*_cVIt)->sReferences.at(j).second) {
                        isAlreadyOccupied = true;
                        break;
                    }
                }
            }
        }
        
        --vacantIndex;
        std::pair<NSpeciesReference*, unsigned int> sRPair;
        for (int i = 0; i != numOfAngualrTrasferringSteps; i += signStep) {
            _cVIt = cVIt + i;
            if (signStep == -1)
                --_cVIt;
            while (_cVIt - cVacanciesBegin() >= cVacanciesEnd() - cVacanciesBegin())
                _cVIt -= cVacanciesEnd() - cVacanciesBegin();
            
            while (_cVIt - cVacanciesBegin() < 0)
                _cVIt += cVacanciesEnd() - cVacanciesBegin();
            
            sRPair.first = sr;
            sRPair.second = vacantIndex;
            (*_cVIt)->sReferences.push_back(sRPair);
        }
        
        return vacantIndex;
    }
    
    return -1;
}

int ReactionLayer::addToCircumferenceVacancy(NSpeciesReference* sr, const unsigned int& reactionVacancyIndex, const int& numOfAngualrTrasferringSteps) {
    constCVacancyIt cVIt, _cVIt;
    cVIt = cVacanciesBegin() + reactionVacancyIndex;
    
    if (cVIt >= cVacanciesBegin() && cVIt < cVacanciesEnd()) {
        int signStep = 1;
        if (numOfAngualrTrasferringSteps < 0)
            signStep *= -1;
        int vacantIndex;
        bool isAlreadyOccupied = true;
        //for (vacantIndex = 0; isAlreadyOccupied && vacantIndex < 5 ; ++vacantIndex) {
        for (vacantIndex = 0; isAlreadyOccupied ; ++vacantIndex) {
            isAlreadyOccupied = false;
            for (int i = 0; i != numOfAngualrTrasferringSteps && !isAlreadyOccupied; i += signStep) {
                _cVIt = cVIt + i;
                if (signStep == -1)
                    --_cVIt;
                                    
                while (_cVIt - cVacanciesBegin() >= cVacanciesEnd() - cVacanciesBegin())
                    _cVIt -= cVacanciesEnd() - cVacanciesBegin();
                
                while (_cVIt - cVacanciesBegin() < 0)
                    _cVIt += cVacanciesEnd() - cVacanciesBegin();
                
                for (int j = 0; j < (*_cVIt)->sReferences.size(); ++j) {
                    if (vacantIndex == (*_cVIt)->sReferences.at(j).second) {
                        isAlreadyOccupied = true;
                        break;
                    }
                }
            }
        }
        
        --vacantIndex;
        std::pair<NSpeciesReference*, unsigned int> sRPair;
        for (int i = 0; i != numOfAngualrTrasferringSteps; i += signStep) {
            _cVIt = cVIt + i;
            if (signStep == -1)
                --_cVIt;
            while (_cVIt - cVacanciesBegin() >= cVacanciesEnd() - cVacanciesBegin())
                _cVIt -= cVacanciesEnd() - cVacanciesBegin();
            
            while (_cVIt - cVacanciesBegin() < 0)
                _cVIt += cVacanciesEnd() - cVacanciesBegin();
            
            sRPair.first = sr;
            sRPair.second = vacantIndex;
            (*_cVIt)->sReferences.push_back(sRPair);
        }
        
        //std::cout << " cVacantIndex " << vacantIndex << std::endl;
        
        return vacantIndex;
    }
    
    //std::cout << " cVacantIndexNotFound " << std::endl;
    return -1;
}

int ReactionLayer::checkCircumferenceVacancy(const unsigned int& reactionVacancyIndex, const int& numOfAngualrTrasferringSteps) {
    constCVacancyIt cVIt, _cVIt;
    cVIt = cVacanciesBegin() + reactionVacancyIndex;
    
    if (cVIt >= cVacanciesBegin() && cVIt < cVacanciesEnd()) {
        int signStep = 1;
        if (numOfAngualrTrasferringSteps < 0)
            signStep *= -1;
        int vacantIndex;
        bool isAlreadyOccupied = true;
        for (vacantIndex = 0; isAlreadyOccupied && vacantIndex < 5 ; ++vacantIndex) {
            isAlreadyOccupied = false;
            for (int i = 0; i != numOfAngualrTrasferringSteps && !isAlreadyOccupied; i += signStep) {
                _cVIt = cVIt + i;
                if (signStep == -1)
                    --_cVIt;
                                    
                while (_cVIt - cVacanciesBegin() >= cVacanciesEnd() - cVacanciesBegin())
                    _cVIt -= cVacanciesEnd() - cVacanciesBegin();
                
                while (_cVIt - cVacanciesBegin() < 0)
                    _cVIt += cVacanciesEnd() - cVacanciesBegin();
                
                for (int j = 0; j < (*_cVIt)->sReferences.size(); ++j) {
                    if (vacantIndex == (*_cVIt)->sReferences.at(j).second) {
                        isAlreadyOccupied = true;
                        break;
                    }
                }
            }
        }
        
        if (vacantIndex != 5)
            return --vacantIndex;
    }
    
    return -1;
}

int ReactionLayer::removeFromCircumferenceVacancy(NSpeciesReference* sr) {
    bool isRemoved = false;
    for (constCVacancyIt cVIt = cVacanciesBegin(); cVIt != cVacanciesEnd(); ++cVIt) {
        for (int i = 0; i < (*cVIt)->sReferences.size(); ++i) {
            if ((stringCompare((*cVIt)->sReferences.at(i).first->getId(), sr->getId())) && (stringCompare((*cVIt)->sReferences.at(i).first->getReaction()->getId(), sr->getReaction()->getId()))) {
                (*cVIt)->sReferences.erase((*cVIt)->sReferences.begin() + i);
                isRemoved = true;
                break;
            }
        }
    }
    
    if (isRemoved)
        return 0;
    else
        return -1;
}

int ReactionLayer::addToRadialVacancy(NSpeciesReference* sr, const unsigned int& reactionVacancyIndex) {
    constVacancyIt vIt = vacanciesBegin() + reactionVacancyIndex;
    
    //std::cout << " vIt " << vIt - vacanciesBegin() << " num " << vacanciesEnd() - vacanciesBegin() << std::endl;
    //if ((*vIt)->species)
        //std::cout << " isPS " << (*vIt)->species->isPseudoSpecies() << std::endl;
    
    //if (vIt >= vacanciesBegin() && vIt < vacanciesEnd() && (!(*vIt)->species || ((*vIt)->species && (*vIt)->species->isPseudoSpecies()))) {
    if (vIt >= vacanciesBegin() && vIt < vacanciesEnd()) {
        int vacantIndex;
        bool isAlreadyOccupied = true;
        //for (vacantIndex = 0; isAlreadyOccupied && vacantIndex < 5 ; ++vacantIndex) {
        for (vacantIndex = 0; isAlreadyOccupied; ++vacantIndex) {
            isAlreadyOccupied = false;
            for (int i = 0; i < (*vIt)->rVacancy.sReferences.size(); ++i) {
                if (vacantIndex == (*vIt)->rVacancy.sReferences.at(i).second) {
                    isAlreadyOccupied = true;
                    break;
                }
            }
        }
        
        --vacantIndex;
        std::pair<NSpeciesReference*, unsigned int> sRPair;
        sRPair.first = sr;
        sRPair.second = vacantIndex;
        (*vIt)->rVacancy.sReferences.push_back(sRPair);
        
        //std::cout << " rVacantIndex " << vacantIndex << std::endl;
        return vacantIndex;
    }
    
    //std::cout << " rVacantIndexNotFound " << std::endl;
    return -1;
}

int ReactionLayer::checkRadialVacancy(const unsigned int& reactionVacancyIndex) {
    constVacancyIt vIt = vacanciesBegin() + reactionVacancyIndex;
    
    if (vIt >= vacanciesBegin() && vIt < vacanciesEnd() && (!(*vIt)->species || ((*vIt)->species && (*vIt)->species->isPseudoSpecies()))) {
        int vacantIndex;
        bool isAlreadyOccupied = true;
        for (vacantIndex = 0; isAlreadyOccupied && vacantIndex < 5 ; ++vacantIndex) {
            isAlreadyOccupied = false;
            for (int i = 0; i < (*vIt)->rVacancy.sReferences.size(); ++i) {
                if (vacantIndex == (*vIt)->rVacancy.sReferences.at(i).second) {
                    isAlreadyOccupied = true;
                    break;
                }
            }
        }
        
        if (vacantIndex != 5)
            return vacantIndex;
    }
    
    return -1;
}

int ReactionLayer::removeFromRadialVacancy(NSpeciesReference* sr) {
    for (constVacancyIt vIt = vacanciesBegin(); vIt != vacanciesEnd(); ++vIt) {
        for (int i = 0; i < (*vIt)->rVacancy.sReferences.size(); ++i) {
            if (stringCompare((*vIt)->rVacancy.sReferences.at(i).first->getId(), sr->getId()) && (stringCompare((*vIt)->rVacancy.sReferences.at(i).first->getReaction()->getId(), sr->getReaction()->getId()))) {
                (*vIt)->rVacancy.sReferences.erase((*vIt)->rVacancy.sReferences.begin() + i);
                return 0;
            }
        }
    }
    
    return -1;
}

}
