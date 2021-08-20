#ifndef __NE_NETWORK_H_
#define __NE_NETWORK_H_

#ifndef SWIG
#include "sbml/SBMLTypes.h"
#include "sbml/packages/layout/common/LayoutExtensionTypes.h"
#include "sbml/packages/render/common/RenderExtensionTypes.h"
#endif

#include "sbne/ne_core.h"
#include "sbne/layout/ne_layoutPoint.h"
#include "sbne/layout/ne_layoutBox.h"

namespace sbne {

/// @enum GrphObjType
/// @brief Enumeration of values permitted as the value of the "type" attribute of an NGraphicalObject object.
typedef enum {
    GRP_OBJ_CMP, /**< NCompartment */
    GRP_OBJ_SPC, /**< NSpecies */
    GRP_OBJ_RXN, /**< NReaction */
    GRP_OBJ_SPC_RFC, /**< NSpeciesReference */
    GRP_OBJ_TXT, /**< NText */
    NUM_GrphObjType /**< Number of GrphObjTypes*/
} GrphObjType;

/// @enum RxnRoleType
/// @brief Enumeration of values permitted as the value of the "role" attribute of an NSpeciesReference object.
typedef enum {
    RXN_ROLE_SUBSTRATE, /**< substrate */
    RXN_ROLE_PRODUCT, /**< product */
    RXN_ROLE_SIDESUBSTRATE, /**< side substrate  */
    RXN_ROLE_SIDEPRODUCT, /**< side product */
    RXN_ROLE_MODIFIER, /**< modifier */
    RXN_ROLE_ACTIVATOR, /**< activator */
    RXN_ROLE_INHIBITOR, /**< inhibitor */
    NUM_RxnRoleType, /**< Number of RxnRoleTypes */
} RxnRoleType;

typedef enum {
    Quad_I_1,
    Quad_I_2,
    Quad_II_1,
    Quad_II_2,
    Quad_III_1,
    Quad_III_2,
    Quad_IV_1,
    Quad_IV_2,
} CoordSysQuad;

typedef enum {
    TOP_SIDE,
    RIGHT_SIDE,
    BOTTOM_SIDE,
    LEFT_SIDE,
} SpcSide;

typedef enum {
    UP_END_SIDE,
    DOWN_END_SIDE,
    DOWN_START_SIDE,
    UP_START_SIDE,
} RxnSide;

typedef enum {
    END_END,
    START_END,
} RxnEnd;

// decalre the network classes
class NetworkElement;
class Network;
class NGraphicalObject;
class NCompartment;
class NSpecies;
class NReaction;
class NSpeciesReference;
class NText;
class LCurve;
class LLineSegment;
class LCubicBezier;
class ReactionLayer;

// decalre the structs in use

struct spcReferencedSide;
struct referencedEnd;
struct connectedReaction;
struct rxnReferencedSide;
struct reactionMember;
struct radialVacancy;
struct vacancy;
struct circumferenceVacancy;

/// Mutual Containers
// compartment
typedef std::vector<NCompartment*> compartmentVec;

// species
typedef std::vector<NSpecies*> speciesVec;

// reactions
typedef std::vector<NReaction*> reactionVec;

// species references
typedef std::vector<NSpeciesReference*> SReferenceVec;

/// Mutual Iterators
// compartment
typedef compartmentVec::iterator compartmentIt;
typedef compartmentVec::const_iterator constCompartmentIt;

// species
typedef speciesVec::iterator speciesIt;
typedef speciesVec::const_iterator constSpeciesIt;

// reaction
typedef reactionVec::iterator reactionIt;
typedef reactionVec::const_iterator constReactionIt;

// species references
typedef SReferenceVec::iterator sReferenceIt;
typedef SReferenceVec::const_iterator constSReferenceIt;

/// @brief Gets the string equivalent of this RxnRoleType.
/// @param role a RxnRoleType.
/// @return the string equivalent of this RxnRoleType.
std::string rxnRoleToString(RxnRoleType role);

/// @brief Gets the RxnRoleType associated with this SpeciesReferenceRole_t.
/// @param role a SpeciesReferenceRole_t.
/// @return the RxnRoleType associated with this SpeciesReferenceRole_t.
RxnRoleType SBMLRoleToRxnRole(SpeciesReferenceRole_t role);

CoordSysQuad angleToQuadrant(double angle);

Network* networkFromModel(const Model& model, Network* net);

Network* networkFromLayout(const Layout& layout, Network* net);

void getLineInfo(LCurve* c, const LineSegment* line);

/// @class NetworkElement
/// @brief Base class for Network objects.
class NetworkElement{
public:
    
    /// Constructors
    NetworkElement() {
        _isSetId = false;
        _isSetName = false;
    }
    
    /// Functions
    // set the id of network element
    void setId(const std::string& id);
    
    // get the id of network element
    const std::string& getId() const;
    
    // show whether the id of network element is set
    const bool isSetId() const { return _isSetId; }
    
    // set the name of network element
    void setName(const std::string& name);
    
    // get the name of network element
    const std::string& getName() const;
    
    // show whether the name of network element is set
    const bool isSetName() const { return _isSetName; }
    
protected:
    // model info:
    std::string _id;
    std::string _name;
    bool _isSetId;
    bool _isSetName;
};

/// @class Network
/// @brief The equivalent class for SBML [Layout](http://model.caltech.edu/software/libsbml/5.18.0/docs/formatted/cpp-api/class_layout.html) .
class Network : public NetworkElement {
public:
    /// Constructors
    Network() {
        _compartments.clear();
        _species.clear();
        _reactions.clear();
        _texts.clear();
        _gObjects.clear();
        _isSetBox = false;
        _isLayoutSpecified = false;
    }
    
    /// Containers
    // texts
    typedef std::vector<NText*> textVec;
    
    // graphical object
    typedef std::vector<NGraphicalObject*> gObjectVec;
    
    /// Iterators
    // text
    typedef textVec::iterator textIt;
    typedef textVec::const_iterator constTextIt;
    
    // graphical object
    typedef gObjectVec::iterator gObjectIt;
    typedef gObjectVec::const_iterator constGObjectIt;
    
    /// Functions related to beginning and end of containers
    // compartment
    const constCompartmentIt compartmentsBegin() const { return _compartments.begin(); }
    const constCompartmentIt compartmentsEnd() const { return _compartments.end(); }
    
    // species
    const constSpeciesIt speciesBegin() const { return _species.begin(); }
    const constSpeciesIt speciesEnd() const { return _species.end(); }
    
    // reaction
    const constReactionIt reactionsBegin() const { return _reactions.begin(); }
    const constReactionIt reactionsEnd() const { return _reactions.end(); }
    
    // text
    const constTextIt textsBegin() const { return _texts.begin(); }
    const constTextIt textsEnd() const { return _texts.end(); }
    
    // graphical object
    const constGObjectIt gObjectsBegin() const { return _gObjects.begin(); }
    const constGObjectIt gObjectsEnd() const { return _gObjects.end(); }
    
    /// Functions
    // add a new compartment to network
    void addCompartment(NCompartment* c);
    
    // remove a compartment from network using its index
    void removeCompartment(int compartmentIndex);
    
    // set compartmentVec
    void setCompartments(const compartmentVec& cv);
    
    // get compartmentVec
    const compartmentVec& getCompartments() const;
    
    // get total number of compartments
    const size_t getNumCompartments() const;
    
    // find a compartment by its specified Id. Returns NULL if no such compartment exists
    NCompartment* findCompartmentById(const std::string& id);
    
    // find the index of a compartment by its specified Id. Returns -1 if no such compartment exists
    int findCompartmentIndexById(const std::string& id);
    
    // find a compartment by its specified glyph Id. Returns NULL if no such compartment exists
    NCompartment* findCompartmentByGlyphId(const std::string& glyphId);
    
    // find the index of a compartment by its specified glyph Id. Returns -1 if no such compartment exists
    int findCompartmentIndexByGlyphId(const std::string& glyphId);
    
    // generate unique Id for for a new compartment
    std::string getCompartmentUniqueId();
    
    // generate unique glyph Id for for a new compartment
    std::string getCompartmentUniqueGlyphId(const std::string& id);
    
    // add a new species to network
    void addSpecies(NSpecies* s);
    
    // remove a species from network using its index
    void removeSpecies(int speciesIndex);
    
    // set speciesVec
    void setSpecies(const speciesVec& sv);
    
    // get speciesVec
    const speciesVec& getSpecies() const;
    
    // get total number of species
    const size_t getNumSpecies() const;
    
    // find a species by its specified Id. Returns NULL if no such species exists
    NSpecies* findSpeciesById(const std::string& id);
    
    // find the index of a species by its specified Id. Returns -1 if no such species exists
    int findSpeciesIndexById(const std::string& id);
    
    // find a species by its specified glyph Id. Returns NULL if no such species exists
    NSpecies* findSpeciesByGlyphId(const std::string& glyphId);
    
    // find the index of a species by its specified glyph Id. Returns -1 if no such species exists
    int findSpeciesIndexByGlyphId(const std::string& glyphId);
    
    // generate unique Id for for a new species
    std::string getSpeciesUniqueId();
    
    // generate unique glyph Id for for a new species
    std::string getSpeciesUniqueGlyphId(const std::string& id);
    
    // add a new reaction to network
    void addReaction(NReaction* r);
    
    // remove a reaction from network using its index
    void removeReaction(int reactionIndex);
    
    // set reactionVec
    void setReactions(const reactionVec& rv);
    
    // get reactionVec
    const reactionVec& getReactions() const;
    
    // get total number of reactions
    const size_t getNumReactions() const;
    
    // find a reaction by its specified Id. Returns NULL if no such reaction exists
    NReaction* findReactionById(const std::string& id);
    
    // find the index of a reaction by its specified Id. Returns -1 if no such reaction exists
    int findReactionIndexById(const std::string& id);
    
    // find a reaction by its specified glyph Id. Returns NULL if no such reaction exists
    NReaction* findReactionByGlyphId(const std::string& glyphId);
    
    // find the index of a reaction by its specified glyph Id. Returns -1 if no such reaction exists
    int findReactionIndexByGlyphId(const std::string& glyphId);
    
    // generate unique Id for for a new reaction
    std::string getReactionUniqueId();
    
    // generate unique glyph Id for for a new reaction
    std::string getReactionUniqueGlyphId(const std::string& id);
    
    // add a new text to network
    void addText(NText* t);
    
    // remove a text from network using its index
    void removeText(int textIndex);
    
    // set textVec
    void setTexts(const textVec& tv);
    
    // get textVec
    const textVec& getTexts() const;
    
    // get total number of texts
    const size_t getNumTexts() const;
    
    // find a text by its specified Id. Returns NULL if no such text exists
    NText* findTextById(const std::string& id);
    
    // find the index of a text by its specified Id. Returns -1 if no such text exists
    int findTextIndexById(const std::string& id);
    
    // find a text by its specified glyph Id. Returns NULL if no such text exists
    NText* findTextByGlyphId(const std::string& glyphId);
    
    // find the index of a text by its specified glyph Id. Returns -1 if no such text exists
    int findTextIndexByGlyphId(const std::string& glyphId);
    
    // find a text by the id of its specified graphical object. Returns NULL if no such text exists
    NText* findTextByGraphicalObjectId(const std::string& gOId);
    
    // find the index of a text by the id of its specified graphical object. Returns -1 if no such text exists
    int findTextIndexByGraphicalObjectId(const std::string& gOId);
    
    // generate unique glyph id for for a new text
    std::string getTextUniqueGlyphId();
    
    // add a new graphical object to network
    void addGObject(NGraphicalObject* o);
    
    // remove a graphical object from network using its index
    void removeGObject(int gObjectIndex);
    
    // set gObjectVec
    void setGObjects(const gObjectVec& ov);
    
    // get gObjectVec
    const gObjectVec& getGObjects() const;
    
    // get total number of graphical objects
    const size_t getNumGObjects() const;
    
    // find a graphical object by its specified Id. Returns NULL if no such graphical object exists
    NGraphicalObject* findGObjectById(const std::string& id);
    
    // find the index of a graphical object by its specified Id. Returns -1 if no such graphical object exists
    int findGObjectIndexById(const std::string& id);
    
    // find a graphical object by its specified glyph Id. Returns NULL if no such graphical object exists
    NGraphicalObject* findGObjectByGlyphId(const std::string& glyphId);
    
    // find the index of a graphical object by its specified glyph Id. Returns -1 if no such graphical object exists
    int findGObjectIndexByGlyphId(const std::string& glyphId);
    
    // generate unique glyph id for for a new graphical object
    std::string getGObjectUniqueGlyphId();
    
    // find a network element either by its id or glyph id. Returns NULL if no such network element exists
    NGraphicalObject* findNetworkElement(const std::string& id);
    
    // set the bounding box of network
    void setBox(const double& x, const double& y, const double& width, const double& height);
    
    // get the bounding box of network
    const LBox& getBox() const;
    
    // show whether the box of the network is ever updated
    const bool isSetBox() const { return _isSetBox; }
    
    // show whether the additional graphical object is specified (set)
    const bool isLayoutSpecified() const { return _isLayoutSpecified; }
        
    // set whether the layout is specified for network
    void setLayoutSpecified(bool value);
    
protected:
    // model info:
    compartmentVec _compartments;
    speciesVec _species;
    reactionVec _reactions;
    textVec _texts;
    gObjectVec _gObjects;
    LBox _box;
    bool _isSetBox;
    bool _isLayoutSpecified;
};

/// @class NGraphicalObject
/// @brief The equivalent class for SBML [GraphicalObject](http://model.caltech.edu/software/libsbml/5.18.0/docs/formatted/cpp-api/class_graphical_object.html) .
class NGraphicalObject : public NetworkElement {
public:
    /// Constructors
    NGraphicalObject() {
        _box = NULL;
        _text = NULL;
        _isSetGlyphId = false;
        _isMatchWithGlyph = false;
        _isSetObjectRole = false;
        _isSetBox = false;
        _isSetText = false;
        _isUsed = false;
        _isLockedPosition = false;
    }
        
    /// Functions
    // get the type of graphical object as GrphObjType
    GrphObjType getType();
    
    // set the glyph id of graphical object
    void setGlyphId(const std::string& glyphId);
    
    // get the glyph id of graphical object
    const std::string& getGlyphId() const;
    
    // show whether the glyph id of graphical object is set
    const bool isSetGlyphId() const { return _isSetGlyphId; }
    
    // set whether the model element and its layout element glyph matches
    void setMatchWithGlyph(bool value);
    
    // show whether the model element and its layout element glyph matches
    const bool isMatchWithGlyph() const { return _isMatchWithGlyph; }
    
    // set the object role of graphical object
    void setObjectRole(const std::string& role);
    
    // get the object role of graphical object
    const std::string& getObjectRole() const;
    
    // show whether the object role of graphical object is set
    const bool isSetObjectRole() const { return _isSetObjectRole; }
    
    // set the bounding box of graphical object
    void setBox(LBox* box);
    
    // unset the bounding box of graphical object
    void unSetBox(const bool& removeBox);
    
    // get the bounding box of graphical object
    LBox* getBox();
    
    // show whether the bounding box of graphical object is set
    const bool isSetBox() const { return _isSetBox; }
    
    // set the text glyph of graphical object
    void setText(NText* text);
    
    // get the text glyph of graphical object
    NText* getText();
    
    // show whether the text glyph of graphical object is set
    const bool isSetText() const { return _isSetText; }
    
    // set whether the graphical object is already used for a glyph
    void setUsed(bool value);
    
    // show whether the graphical object is already used for a glyph
    const bool isUsed() const { return _isUsed; }
    
    // set whether the graphical object position is locked
    void setLockedPosition(bool value);
    
    // show whether the graphical object position is locked
    const bool isLockedPosition() const { return _isLockedPosition; }
        
protected:
    // model info:
    GrphObjType _type;
    std::string _glyphId;
    std::string _objectRole;
    LBox* _box;
    NText* _text;
    bool _isSetGlyphId;
    bool _isMatchWithGlyph;
    bool _isSetObjectRole;
    bool _isSetBox;
    bool _isSetText;
    bool _isUsed;
    bool _isLockedPosition;
};

/// @class NCompartment
/// @brief The equivalent class for SBML [Compartment](http://model.caltech.edu/software/libsbml/5.18.0/docs/formatted/cpp-api/class_compartment.html) and [CompartmentGlyph](http://model.caltech.edu/software/libsbml/5.18.0/docs/formatted/cpp-api/class_compartment_glyph.html) .
class NCompartment : public NGraphicalObject {
public:
    /// Constructors
    NCompartment() {
        _type = GRP_OBJ_CMP;
        _species.clear();
        _reactions.clear();
        _isSetOrder = false;
    }
    
    /// Functions related to beginning and end of containers
    // species
    const constSpeciesIt speciesBegin() const { return _species.begin(); }
    const constSpeciesIt speciesEnd() const { return _species.end(); }
    
    // reaction
    const constReactionIt reactionsBegin() const { return _reactions.begin(); }
    const constReactionIt reactionsEnd() const { return _reactions.end(); }
    
    /// Functions
    // add a new species to compartment
    void addSpecies(NSpecies* s);
    
    // remove a species from compartment using its index
    void removeSpecies(int speciesIndex);
    
    // set speciesVec
    void setSpecies(const speciesVec& sv);
    
    // get speciesVec
    const speciesVec& getSpecies() const;
    
    // get total number of species
    const size_t getNumSpecies() const;
    
    // find a species by its specified Id. Returns NULL if no such species exists
    NSpecies* findSpeciesById(const std::string& id);
    
    // find the index of a species by its specified Id. Returns -1 if no such species exists
    int findSpeciesIndexById(const std::string& id);
    
    // find a species by its specified glyph Id. Returns NULL if no such species exists
    NSpecies* findSpeciesByGlyphId(const std::string& glyphId);
    
    // find the index of a species by its specified glyph Id. Returns -1 if no such species exists
    int findSpeciesIndexByGlyphId(const std::string& glyphId);
    
    // add a new reaction to compartment
    void addReaction(NReaction* r);
    
    // remove a reaction from compartment using its index
    void removeReaction(int reactionIndex);
    
    // set reactionVec
    void setReactions(const reactionVec& rv);
    
    // get reactionVec
    const reactionVec& getReactions() const;
    
    // get total number of reactions
    const size_t getNumReactions() const;
    
    // find a reaction by its specified Id. Returns NULL if no such reaction exists
    NReaction* findReactionById(const std::string& id);
    
    // find the index of a reaction by its specified Id. Returns -1 if no such reaction exists
    int findReactionIndexById(const std::string& id);
    
    // find a reaction by its specified glyph Id. Returns NULL if no such reaction exists
    NReaction* findReactionByGlyphId(const std::string& glyphId);
    
    // find the index of a reaction by its specified glyph Id. Returns -1 if no such reaction exists
    int findReactionIndexByGlyphId(const std::string& glyphId);
    
    // set the order of compartment
    void setOrder(const double& order);
    
    // get the order of compartment
    const double& getOrder() const;
    
    // show whether the order of compartment is set
    const bool isSetOrder() const { return _isSetOrder; }
    
    // shift the position of a compartment and all its items
    void shiftItems(const double& dx, const double& dy);
        
protected:
    speciesVec _species;
    reactionVec _reactions;
    double _order;
    bool _isSetOrder;
};

/// @class NSpecies
/// @brief The equivalent class for SBML [Species](http://model.caltech.edu/software/libsbml/5.18.0/docs/formatted/cpp-api/class_species.html) and [SpeciesGlyph](http://model.caltech.edu/software/libsbml/5.18.0/docs/formatted/cpp-api/class_species_glyph.html) .
class NSpecies : public NGraphicalObject {
public:
    /// Constructors
    NSpecies() {
        _type = GRP_OBJ_SPC;
        _reactionMembers.clear();
        _rSides.clear();
        _isSetCompartment = false;
        _isPseudoSpecies = false;
    }
    
    /// Containers
    // reaction members
    typedef std::vector<reactionMember*> rMemberVec;
    
    // referenced sides
    typedef std::vector<spcReferencedSide*> rSideVec;
    
    /// Iterators
    // reaction members
    typedef rMemberVec::iterator rMemberIt;
    typedef rMemberVec::const_iterator constRMemberIt;
    
    // referenced sides
    typedef rSideVec::iterator rSideIt;
    typedef rSideVec::const_iterator constRSideIt;
    
    /// Functions related to beginning and end of containers
    // reaction
    const constRMemberIt rMembersBegin() const { return _reactionMembers.begin(); }
    const constRMemberIt rMembersEnd() const { return _reactionMembers.end(); }
    
    // referenced sides
    const constRSideIt rSideBegin() const { return _rSides.begin(); }
    const constRSideIt rSideEnd() const { return _rSides.end(); }
    
    /// Functions
    // set the id of assgined compartment of species
    void setCompartment(const std::string& compartmentId);
    
    // get the id of assgined compartment of species
    const std::string& getCompartment() const;
    
    // show whether the compartment of species is assgiend
    const bool isSetCompartment() const { return _isSetCompartment; }
    
    // add a new reaction member
    void addReactionMember(NReaction* r, const double& angle = 0.0, const bool& isMainReaction = false);
    
    // remove a reaction member from reaction members using its id
    void removeReactionMember(const std::string& reactionId);
    
    // get rMemberVec
    const rMemberVec& getReactionMembers() const;

    // set rMemberVec
    void setReactionMembers(const rMemberVec& rmv);
    
    // get total number of reaction members
    const size_t getNumReactionMembers() const;
    
    // find a reaction member in reaction members by its specified Id. Returns NULL if no such reaction member exists
    reactionMember* findReactionMemberById(const std::string& reactionId);
    
    // find a reaction member in reaction members by its specified glyph Id. Returns NULL if no such reaction member exists
    reactionMember* findReactionMemberByGlyphId(const std::string& reactionGlyphId);
    
    // add a side with zero reference to the species sides
    void addReferencedSide(const SpcSide& side);
    
    // add a new species reference to species side
    void addToReferencedSide(NSpeciesReference* sr, const SpcSide& side);
    
    // remove a reference from species using its side
    void removeReferencedSide(const SpcSide& side);
    
    // remove a speceis reference from a referenced side
    void removeFromReferencedSide(NSpeciesReference* sr, const SpcSide& side);
    
    // get srefenceVec of a referenced side
    SReferenceVec getSideSpeciesReferences(const SpcSide& side);
    
    // set rSideVec
    void setReferencedSide(const rSideVec& rsv);
    
    // get rSideVec
    const rSideVec& getReferencedSides() const;
    
    // get total number of referenced sides
    const size_t getNumRSides() const;
    
    // set whether the species is pseudo
    void setPseudoSpecies(bool value);
    
    // show whether the the species is pseudo
    const bool isPseudoSpecies() const { return _isPseudoSpecies; }
    
protected:
    // model info:
    std::string _compartment;
    rMemberVec _reactionMembers;
    rSideVec _rSides;
    bool _isSetCompartment;
    bool _isPseudoSpecies;
};

/// @class NReaction
/// @brief The equivalent class for SBML [Reaction](http://model.caltech.edu/software/libsbml/5.18.0/docs/formatted/cpp-api/class_reaction.html) and [ReactionGlyph](http://model.caltech.edu/software/libsbml/5.18.0/docs/formatted/cpp-api/class_reaction_glyph.html) .
class NReaction : public NGraphicalObject {
public:
    /// Constructors
    NReaction() {
        _type = GRP_OBJ_RXN;
        _speciesReferences.clear();
        _subSpeciesReferences.clear();
        _species.clear();
        _subSpecies.clear();
        _pseudoSpecies.clear();
        _curve = NULL;
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
        _isSetExtentBox = false;
    }
    
    /// Containers
    // referenced sides
    typedef std::vector<rxnReferencedSide*> rSideVec;
    
    // referenced ends
    typedef std::vector<referencedEnd*> rEndVec;
    
    // connected reactions
    typedef std::vector<connectedReaction*> connectedRVec;
    
    // reaction layers
    typedef std::vector<ReactionLayer*> rLayerVec;
           
    /// Iterators
    // referenced sides
    typedef rSideVec::iterator rSideIt;
    typedef rSideVec::const_iterator constRSideIt;
    
    // referenced ends
    typedef rEndVec::iterator rEndIt;
    typedef rEndVec::const_iterator constREndIt;
    
    // connected reactions
    typedef connectedRVec::iterator connectedRIt;
    typedef connectedRVec::const_iterator constConnectedRIt;
    
    // reaction layers
    typedef rLayerVec::iterator rLayerIt;
    typedef rLayerVec::const_iterator constRLayerIt;
    
    /// Functions related to beginning and end of containers
    // species references
    const constSReferenceIt sReferencesBegin() const { return _speciesReferences.begin(); }
    const constSReferenceIt sReferencesEnd() const { return _speciesReferences.end(); }
    
    // sub species references
    const constSReferenceIt subSReferencesBegin() const { return _subSpeciesReferences.begin(); }
    const constSReferenceIt subSReferencesEnd() const { return _subSpeciesReferences.end(); }
    
    // species
    const constSpeciesIt speciesBegin() const { return _species.begin(); }
    const constSpeciesIt speciesEnd() const { return _species.end(); }
    
    // sub Species
    const constSpeciesIt subSpeciesBegin() const { return _subSpecies.begin(); }
    const constSpeciesIt subSpeciesEnd() const { return _subSpecies.end(); }
    
    // pseudoSpecies
    const constSpeciesIt pseudoSpeciesBegin() const { return _pseudoSpecies.begin(); }
    const constSpeciesIt pseudoSpeciesEnd() const { return _pseudoSpecies.end(); }
    
    // referenced sides
    const constRSideIt rSideBegin() const { return _rSides.begin(); }
    const constRSideIt rSideEnd() const { return _rSides.end(); }
    
    // referenced ends
    const constREndIt rEndsBegin() const { return _rEnds.begin(); }
    const constREndIt rEndsEnd() const { return _rEnds.end(); }
    
    // connected reactions
    const constConnectedRIt connectedRBegin() const { return _connectedReactions.begin(); }
    const constConnectedRIt connectedREnd() const { return _connectedReactions.end(); }
    
    // sub reactions
    const constReactionIt subReactionsBegin() const { return _subReactions.begin(); }
    const constReactionIt subReactionsEnd() const { return _subReactions.end(); }
    
    // reaction layers
    const constRLayerIt rLayersBegin() const { return _reactionLayers.begin(); }
    const constRLayerIt rLayersEnd() const { return _reactionLayers.end(); }
    
    /// Functions
    // set the id of assgined compartment of reaction
    void setCompartment(const std::string& compartmentId);
    
    // get the id of assgined compartment of reaction
    const std::string& getCompartment() const;
    
    // show whether the compartment of reaction is assgiend
    const bool isSetCompartment() const { return _isSetCompartment; }
    
    // add a new species reference to reaction
    void addSpeciesReference(NSpeciesReference* sr);
    
    // remove a species reference from reaction using its index
    void removeSpeciesReference(int sReferenceIndex);
    
    // set sReferenceVec
    void setSpeciesReferences(const SReferenceVec& srv);
    
    // get sReferenceVec
    const SReferenceVec& getSpeciesReferences() const;
    
    // get total number of species references
    const size_t getNumSpeciesReferences() const;
    
    // find a species reference by its specified Id. Returns NULL if no such species reference exists
    NSpeciesReference* findSpeciesReferenceById(const std::string& id);
    
    // find the index of a species reference by its specified Id. Returns -1 if no such species reference exists
    int findSpeciesReferenceIndexById(const std::string& id);
    
    // find a species reference by its specified glyph Id. Returns NULL if no such species reference exists
    NSpeciesReference* findSpeciesReferenceByGlyphId(const std::string& glyphId);
    
    // find the index of a species reference by its specified glyph Id. Returns -1 if no such species reference exists
    int findSpeciesReferenceIndexByGlyphId(const std::string& glyphId);
    
    // generate unique Id for for a new species reference
    std::string getSpeciesReferenceUniqueId();
    
    // generate unique glyph Id for for a new species reference
    std::string getSpeciesReferenceUniqueGlyphId(const std::string& id);
    
    // add a new sub species reference to reaction
    void addSubSpeciesReference(NSpeciesReference* ssr);
    
    // remove a sub species reference from reaction using its index
    void removeSubSpeciesReference(int subSReferenceIndex);
    
    // set sub species references
    void setSubSpeciesReferences(const SReferenceVec& ssrv);
    
    // get sub species references
    const SReferenceVec& getSubSpeciesReference() const;
    
    // get total number of sub species references
    const size_t getNumSubSpeciesReferences() const;
    
    // find a sub species reference by its specified Id. Returns NULL if no such such species reference exists
    NSpeciesReference* findSubSpeciesReferenceById(const std::string& id);
    
    // find the index of a sub species reference by its specified Id. Returns -1 if no such sub species reference exists
    int findSubSpeciesReferenceIndexById(const std::string& id);
    
    // add a new species to reaction
    void addSpecies(NSpecies* s);
    
    // remove a species from reaction using its index
    void removeSpecies(int speciesIndex);
    
    // set speciesVec
    void setSpecies(const speciesVec& sv);
    
    // get speciesVec
    const speciesVec& getSpecies() const;
    
    // get total number of species
    const size_t getNumSpecies() const;
    
    // find a species by its specified Id. Returns NULL if no such species exists
    NSpecies* findSpeciesById(const std::string& id);
    
    // find the index of a species by its specified Id. Returns -1 if no such species exists
    int findSpeciesIndexById(const std::string& id);
    
    // find a species by its specified glyph Id. Returns NULL if no such species exists
    NSpecies* findSpeciesByGlyphId(const std::string& glyphId);
    
    // find the index of a species by its specified glyph Id. Returns -1 if no such species exists
    int findSpeciesIndexByGlyphId(const std::string& glyphId);
    
    // add a new subSpecies to reaction
    void addSubSpecies(NSpecies* ss);
    
    // remove a subSpecies from reaction using its index
    void removeSubSpecies(int subSpeciesIndex);
    
    // set subSpeciesVec
    void setSubSpecies(const speciesVec& ssv);
    
    // get subSpeciesVec
    const speciesVec& getSubSpecies() const;
    
    // get total number of subSpecies
    const size_t getNumSubSpecies() const;
    
    // find a subSpecies by its specified Id. Returns NULL if no such species exists
    NSpecies* findSubSpeciesById(const std::string& id);
    
    // find the index of a subSpecies by its specified Id. Returns -1 if no such species exists
    int findSubSpeciesIndexById(const std::string& id);
    
    // add a new pseudoSpecies to reaction
    void addPseudoSpecies(NSpecies* ps);
    
    // remove a pseudoSpecies from reaction using its index
    void removePseudoSpecies(int pseudoSpeciesIndex);
    
    // set pseudoSpeciesVec
    void setPseudoSpecies(const speciesVec& psv);
    
    // get pseudoSpeciesVec
    const speciesVec& getPseudoSpecies() const;
    
    // get total number of pseudoSpecies
    const size_t getNumPseudoSpecies() const;
    
    // find a pseudoSpecies by its specified Id. Returns NULL if no such species exists
    NSpecies* findPseudoSpeciesById(const std::string& id);
    
    // find the index of a pseudoSpecies by its specified Id. Returns -1 if no such species exists
    int findPseudoSpeciesIndexById(const std::string& id);
    
    // generate an id for a pseudoSpecies using the mutual species
    std::string generatePseudoSpeciesId(const speciesVec& mutualSpecies);
    
    // generate an id for a pseudoSpecies using the mutual species the version number of the pseudoSpecies
    std::string generatePseudoSpeciesId(const speciesVec& mutualSpecies, const unsigned int& version);
    
    // generate a unique id for a pseudoSpecies using the mutual species;
    std::string generatePseudoSpeciesUniqueId(const speciesVec& mutualSpecies);
    
    // set the curve of reaction
    void setCurve(LCurve* curve);
    
    // unset the curve of reaction
    void unSetCurve(const bool& removeCurve);
    
    // get the curve of reaction
    LCurve* getCurve();
    
    // show whether the curve of reaction is set
    const bool isSetCurve() const { return _isSetCurve; }
    
    // add a new species reference to reaction side
    void addReferencedSide(NSpeciesReference* sr, const RxnSide& side);
    
    // remove a speceis reference from a referenced side
    void removeFromReferencedSide(NSpeciesReference* sr, const RxnSide& side);
    
    // get srefenceVec of a referenced side
    SReferenceVec getSideSpeciesReferences(const RxnSide& side);
    
    // set rSideVec
    void setReferencedSide(const rSideVec& rsv);
    
    // get rSideVec
    const rSideVec& getReferencedSides() const;
    
    // get total number of referenced sides
    const size_t getNumRSides() const;
    
    // add a new species reference to reaction end
    void addReferencedEnd(NSpeciesReference* sr, const RxnEnd& end);
    
    // remove a speceis reference from a referenced end
    void removeFromReferencedEnd(NSpeciesReference* sr, const RxnEnd& end);
    
    // get srefenceVec of a referenced end
    SReferenceVec getEndSpeciesReferences(const RxnEnd& end);
    
    // set rEndVec
    void setReferencedEnd(const rEndVec& rev);
    
    // get rEndVec
    const rEndVec& getReferencedEnds() const;
    
    // get total number of referenced ends
    const size_t getNumREnds() const;
    
    // add a new connected reaction
    void addConnectedReaction(NReaction* r);
    
    // add a new connected reaction with a joint species
    void addConnectedReaction(NReaction* r, NSpecies* s);
    
    // set the pseudoSpecies of connected reaction
    void setPseudoSpeciesOfConnectedReaction(NReaction* r, NSpecies* pseudoSpecies);
    
    // remove a reaction from connected reactions using its id
    void removeConnectedReaction(NReaction* r);
    
    // remove a species from a connected reaction using its id
    void removeSpeciesFromConnectedReaction(NReaction* r, NSpecies* s);
    
    // unSet the pseudoSpecies of connected reaction
    void unSetPseudoSpeciesFromConnectedReaction(NReaction* r);
    
    // get connectedRVec
    const connectedRVec& getConnectedReactions() const;

    // set connectedRVec
    void setConnectedReactions(const connectedRVec& crv);
    
    // get total number of connected reactions
    const size_t getNumConnectedReactions() const;
    
    // find a connected reaction in connected reactions by its specified Id. Returns NULL if no such connected reaction exists
    connectedReaction* findConnectedReactionByReactionId(const std::string& reactionId);
    
    // find a connected reaction in connected reactions by its specified glyph Id. Returns NULL if no such connected reaction exists
    connectedReaction* findConnectedReactionByReactionGlyphId(const std::string& reactionGlyphId);
    
    // find all connected reactions in which a species exists using species id
    connectedRVec findConnectedReactionsBySpeciesId(const std::string& speciesId);
    
    // find all connected reactions in which a species exists using species glyph id
    connectedRVec findConnectedReactionsBySpeciesGlyphId(const std::string& speciesId);
    
    // add a new subReaction to reaction
    void addSubReaction(NReaction* sr);
    
    // remove a subReaction from reaction using its index
    void removeSubReaction(int subReactionIndex);
    
    // set subReactionVec
    void setSubReactions(const reactionVec& srv);
    
    // get subReactionVec
    const reactionVec& getSubReactions() const;
    
    // get total number of subReactions
    const size_t getNumSubReactions() const;
    
    // find a subReaction by its specified Id. Returns NULL if no such reaction exists
    NReaction* findSubReactionById(const std::string& id);
    
    // find the index of a subReaction by its specified Id. Returns -1 if no such reaction exists
    int findSubReactionIndexById(const std::string& id);
    
    // find a subReaction by its specified glyph Id. Returns NULL if no such reaction exists
    NReaction* findSubReactionByGlyphId(const std::string& glyphId);
    
    // find the index of a subReaction by its specified glyph Id. Returns -1 if no such reaction exists
    int findSubReactionIndexByGlyphId(const std::string& glyphId);
    
    // add a new reaction layer to reaction
    void addReactionLayer();
    
    // add a species to a vacancy
    int addSpeciesToVacancy(NSpecies* s, double& speciesAngle);
    
    // remove a species from a vacancy
    int removeSpeciesFromVacancy(NSpecies* s, const double& speciesAngle);
    
    // find the layer and vacancy in which this species positioned. Returns -1, -1 if the species is not postioned
    std::pair<int, int> findSpeciesAddress(NSpecies* s);
    
    // find the layer in which this species has occupied a vacancy. Returns -1 if the species has not occupied any vacancy
    int findSpeciesLayer(NSpecies* s);
    
    // transfer a an already located species to its next vacant layer
    int transferSpeciesToNextLayerVacancy(NSpecies* s, double& speciesAngle);
    
    // get number of sub reactions in a vacancy using the species located in that vacancy
    int getNumOccupiedReactionsOfSpecies(NSpecies* s);
    
    // add a sub reaction to a vacancy using joint species angle
    int addSubReactionToVacancy(NReaction* subR, const double& jointSpeciesAngle);
    
    // add a sub reaction to a vacancy using joint species
    int addSubReactionToVacancy(NReaction* subR, NSpecies* jointSpecies);
    
    // remove a sub reaction from a vacancy using joint species angle
    int removeSubReactionFromVacancy(NReaction* subR, const double& jointSpeciesAngle);
    
    // remove a sub reaction from a vacancy using joint species
    int removeSubReactionFromVacancy(NReaction* subR, NSpecies* jointSpecies);
    
    // find the layer and vacancy in which this sub reaction positioned. Returns -1, -1 if the sub reaction is not postioned
    std::pair<int, int> findSubReactionAddress(NReaction* subR);
    
    // find the layer in which this sub reaction has occupied a vacancy. Returns -1 if the sub reaction has not occupied any vacancy
    int findSubReactionLayer(NReaction* subR);
    
    // add a species reference to a circumference vacancy using pseudospecies
    int addSpeciesReferenceToCircumferenceVacancy(NSpeciesReference* sr, NSpecies* pseudoSpecies, const int& numOfTrasferringSteps);

    // remove a species reference from a circumference vacancy
    int removeSpeciesReferenceFromCircumferenceVacancy(NSpeciesReference* sr);
    
    // find the manhatan path for the species reference starting from pseudospecies and moving numRadialTransferringSteps and numAngularTransferringSteps
    std::vector<std::pair<int, int>> getManhatanPathOfSpeciesReference(NSpeciesReference* sr, NSpecies* pseudoSpecies, const int& numRadialTransferringSteps, const int& numAngularTransferringSteps);
    
    // get rLayerVec
    const rLayerVec& getReactionLayers() const;

    // get total number of reaction layers
    const size_t getNumReactionLayers() const;
    
    // set the center point of reaction
    void setCenter(const LPoint& centerP);
    
    // get the center point of reaction
    const LPoint& getCenter() const;
    
    // show whether center point of reaction is set
    const bool isSetCenter() const { return _isSetCenterP; }
    
    // set the radius of reaction
    void setRadius(const double& radius);
    
    // get the radius of reaction
    const double& getRadius() const;
    
    // show whether radius of reaction is set
    const bool isSetRadius() const { return _isSetRadius; }
    
    // set the direction of reaction curve
    //void setDirection(const RxnDir& direction);
    void setDirectionAngle(const double& directionAngle);
    
    // get the direction of reaction curve
    //const RxnDir& getDirection() const;
    const double& getDirectionAngle() const;
    
    // show whether direction angle of reaction is set
    const bool isSetDirectionAngle() const { return _isSetDirectionAngle; }
    
    // set the species start angle of reaction
    void setSpeciesStartAngle(const double& angle);
    
    // get the pecies start angle reaction
    const double& getSpeciesStartAngle() const;
    
    // show whether species start angle reaction is set
    const bool isSetSpeciesStartAngle() const { return _isSetSpeciesStartAngle; }
    
    // shift the position of a main reaction and all its items
    void shiftItems(const double& dx, const double& dy);
    
    // claculate the extents of the reaction and all its items and set the extent box
    void calculateExtents();
    
    // get the extent box
    LBox getExtentBox() ;
    
    // show whether extent box of the reaction is set
    const bool isSetExtentBox() const { return _isSetExtentBox; }
protected:
    // model info:
    std::string _compartment;
    SReferenceVec _speciesReferences;
    SReferenceVec _subSpeciesReferences;
    speciesVec _species;
    speciesVec _subSpecies;
    speciesVec _pseudoSpecies;
    LCurve* _curve;
    rSideVec _rSides;
    rEndVec _rEnds;
    connectedRVec _connectedReactions;
    reactionVec _subReactions;
    rLayerVec _reactionLayers;
    LPoint _centerP;
    double _radius;
    double _directionAngle;
    double _speciesStartAngle;
    LBox _extentBox;
    bool _isSetCompartment;
    bool _isSetCurve;
    bool _isSetCenterP;
    bool _isSetRadius;
    bool _isSetDirectionAngle;
    bool _isSetSpeciesStartAngle;
    bool _isSetExtentBox;
};

/// @class NSpeciesReference
/// @brief The equivalent class for SBML [SpeciesReference](http://model.caltech.edu/software/libsbml/5.18.0/docs/formatted/cpp-api/class_species_reference.html) and [SpeciesReferenceGlyph](http://model.caltech.edu/software/libsbml/5.18.0/docs/formatted/cpp-api/class_species_reference_glyph.html) .
class NSpeciesReference : public NGraphicalObject {
public:
    /// Constructors
    NSpeciesReference() {
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
    }
    
    /// Functions
    // set the species of species reference
    void setSpecies(NReaction* r, NSpecies* s);
    
    // get the species of species reference
    NSpecies* getSpecies() const;
    
    // show whether the species of species reference is set
    const bool isSetSpecies() const { return _isSetSpecies; }
    
    // get the reaction of species reference
    NReaction* getReaction() const;
    
    // show whether the reaction of species reference is set
    const bool isSetReaction() const { return _isSetReaction; }
    
    // set the curve of species reference
    void setCurve(LCurve* s);
    
    // unset the curve of species reference
    void unSetCurve(const bool& removeCurve);
    
    // get the curve of species reference
    LCurve* getCurve() const;
    
    // show whether the curve of species reference is set
    const bool isSetCurve() const { return _isSetCurve; }
    
    // set the role of species reference
    void setRole(const RxnRoleType& role);
    
    // get the role of species reference
    const RxnRoleType& getRole() const;
    
    // show whether the role of species reference is set
    const bool isSetRole() const { return _isSetRole; }
    
    // set the quadrant position of species reference extreme relative to its reaction
    void setQuadrant(const CoordSysQuad& quadrant);
    
    // get the quadrant position of species reference extreme relative to its reaction
    const CoordSysQuad& getQuadrant() const;
    
    // show whether the quadrant position of species reference extreme relative to its reaction
    const bool isSetQuadrant() const { return _isSetQuadrant; }
    
    // set the value of start point of the species reference
    void setStartPoint(const LPoint& startPoint);

    // get the value of start point of the species reference
    const LPoint& getStartPoint() const;

    // show whether the value of start point of the species reference is set
    const bool isSetStartPoint() const { return _isSetStartPoint; }
    
    // set the value of end point of the species reference
    void setEndPoint(const LPoint& endPoint);

    // get the value of end point of the species reference
    const LPoint& getEndPoint() const;

    // show whether the value of end point of the species reference is set
    const bool isSetEndPoint() const { return _isSetEndPoint; }

protected:
    // model info:
    NSpecies* _species;
    NReaction* _reaction;
    LCurve* _curve;
    RxnRoleType _role;
    CoordSysQuad _quadrant;
    LPoint _startPoint;
    LPoint _endPoint;
    bool _isSetSpecies;
    bool _isSetReaction;
    bool _isSetCurve;
    bool _isSetRole;
    bool _isSetQuadrant;
    bool _isSetStartPoint;
    bool _isSetEndPoint;
};

/// @class NText
/// @brief The equivalent class for SBML [TextGlyph](http://model.caltech.edu/software/libsbml/5.18.0/docs/formatted/cpp-api/class_text_glyph.html) .
class NText : public NGraphicalObject {
public:
    /// Constructors
    NText() {
        _type = GRP_OBJ_TXT;
        _isSetGraphicalObjectId = false;
        _isSetText = false;
        _isSetOriginOfTextId = false;
    }
        
    /// Functions
    // set the graphical object of text
    void setGraphicalObjectId(const std::string& graphicalObjectId);
    
    // get the graphical object of text
    const std::string& getGraphicalObjectId() const;
    
    // show whether the graphical object of text is set
    const bool isSetGraphicalObjectId() const { return _isSetGraphicalObjectId; }
    
    // set the text of text
    void setText(const std::string& text);
    
    void unSetText();
    
    // get the text of text
    const std::string& getText() const;
    
    // show whether the text of text is set
    const bool isSetText() const { return _isSetText; }
    
    // set the origin of text of text
    void setOriginOfTextId(const std::string& originOfTextId);
    
    // get the origin of text of text
    const std::string& getOriginOfTextId() const;
    
    // show whether the origin of text of text is set
    const bool isSetOriginOfTextId() const { return _isSetOriginOfTextId; }
    
protected:
    std::string _graphicalObjectId;
    std::string _text;
    std::string _originOfTextId;
    bool _isSetGraphicalObjectId;
    bool _isSetText;
    bool _isSetOriginOfTextId;
};

/// @class LCurve
/// @brief The equivalent class for SBML [Curve](http://model.caltech.edu/software/libsbml/5.18.0/docs/formatted/cpp-api/class_curve.html) .
class LCurve : public NetworkElement {
public:
    /// Constructors
    LCurve() {
        _listOfElements.clear();
    }
    
    /// Containers
    // element
    typedef std::vector<LLineSegment*> listOfElements;
    
    /// Iterators
    // element
    typedef listOfElements::iterator elementIt;
    typedef listOfElements::const_iterator constElementIt;
    
    /// Functions related to beginning and end of containers
    // element
    const constElementIt elementsBegin() const { return _listOfElements.begin(); }
    const constElementIt elementsEnd() const { return _listOfElements.end(); }
    
    /// Functions
    // add a point or cubic bezier to the end of list of elements
    void addToListOfElementsEnd(LLineSegment* l);
    
    // add a point or cubic bezier to the list of elements of curve
    void addToListOfElements(LLineSegment* l, unsigned int index);
    
    // remove an element from the list of elements of curve
    void removeFromListOfElements(unsigned int index);
    
    // remove all elements from the list of elements of  curve
    void clearListOfElements();
    
    // get the list of elements of  curve
    const listOfElements& getListOfElements() const;
    
    // get total number of elements
    const size_t getNumElements() const;
    
    // find an element by its specified Id. Returns NULL if no such element exists
    LLineSegment* findElementById(const std::string& id);
    
    // find the index of an element by its specified Id. Returns -1 if no such element exists
    int findElementIndexById(const std::string& id);
    
    // generate unique ID for a new element
    std::string getElementUniqueId();
    
    // shift position
    void shiftPosition(const double& dx, const double& dy);
    
    // get the extent box of the curve points
    LBox getExtentBox();
    
protected:
    // model info:
    listOfElements _listOfElements;
};

/// @class LLineSegment
/// @brief The equivalent class for SBML [LineSegment](http://model.caltech.edu/software/libsbml/5.18.0/docs/formatted/cpp-api/class_line_segment.html) .
class LLineSegment : public NetworkElement {
public:
    /// Constructors
    LLineSegment() {
        _start = LPoint(0.0, 0.0);
        _end = LPoint(0.0, 0.0);
        _isSetStart = false;
        _isSetEnd = false;
    }

    /// Functions
    // set the start value of layout point
    void setStart(const LPoint& p);

    // get the start value of layout point
    const LPoint& getStart() const;

    // show whether the start point of line segment is set
    const bool isSetStart() const { return _isSetStart; }
    
    // set the end value of layout point
    void setEnd(const LPoint& p);

    // get the end value of layout point
    const LPoint& getEnd() const;

    // show whether the end point of line segment is set
    const bool isSetEnd() const { return _isSetEnd; }

    // show whether it is of type cubic bezier
    virtual bool isCubicBezier() { return false; }

protected:
    // model info:
    LPoint _start;
    LPoint _end;
    bool _isSetStart;
    bool _isSetEnd;
};

/// @class LCubicBezier
/// @brief The equivalent class for SBML [CubicBezier](http://model.caltech.edu/software/libsbml/5.18.0/docs/formatted/cpp-api/class_cubic_bezier.html) .
class LCubicBezier : public LLineSegment {
public:
    /// Constructors
    LCubicBezier() {
        _basePoint1 = LPoint(0.0, 0.0);
        _basePoint2 = LPoint(0.0, 0.0);
        _isSetBasePoint1 = false;
        _isSetBasePoint2 = false;
        _isArcLike = false;
    }

    /// Functions
    // set the base point 1 value of cubic bezier
    void setBasePoint1(const LPoint& p);
    
    // get the base point 1 value of cubic bezier
    const LPoint& getBasePoint1() const;
    
    // show whether the base point 1 value of cubic bezier is set
    const bool isSetBasePoint1() const { return _isSetBasePoint1; }
    
    // set the base point 2 value of cubic bezier
    void setBasePoint2(const LPoint& p);
    
    // get the base point 2 value of cubic bezier
    const LPoint& getBasePoint2() const;
    
    // show whether the base point 2 value of cubic bezier is set
    const bool isSetBasePoint2() const { return _isSetBasePoint2; }
    
    // set whether the cubic bezier is an arc-like cubic bezier or not
    void setArcLike(bool value);
    
    // show whether the cubic bezier is an arc-like cubic bezier
    const bool isArcLike() const {return _isArcLike;}
    
    // show whether it is of type cubic bezier
    virtual bool isCubicBezier() { return true; }

protected:
    // model info:
    LPoint _basePoint1;
    LPoint _basePoint2;
    bool _isSetBasePoint1;
    bool _isSetBasePoint2;
    bool _isArcLike;
};

class ReactionLayer {
public:
    /// Constructors
    ReactionLayer(const unsigned int& layer, const unsigned int& numReactionSpecies, const double& reactionStartAngle);
    
    /// Containers
    // vacancies
    typedef std::vector<vacancy*> vacancyVec;
    
    // cirumference vacancies
    typedef std::vector<circumferenceVacancy*> cVacancyVec;
    
    /// Iterators
    // vacancies
    typedef vacancyVec::iterator vacancyIt;
    typedef vacancyVec::const_iterator constVacancyIt;
    
    // circumference vacancies
    typedef cVacancyVec::iterator cVacancyIt;
    typedef cVacancyVec::const_iterator constCVacancyIt;
    
    /// Functions related to beginning and end of containers
    // vacancies
    const constVacancyIt vacanciesBegin() const { return _vacancies.begin(); }
    const constVacancyIt vacanciesEnd() const { return _vacancies.end(); }
    
    // circumference vacancies
    const constCVacancyIt cVacanciesBegin() const { return _circumferenceVacancies.begin(); }
    const constCVacancyIt cVacanciesEnd() const { return _circumferenceVacancies.end(); }
    
    /// Functions
    // get the layer of reactionlayer
    const unsigned int& getLayer() const;
    
    // add a species to a vacancy. Returns -1 if the vacancy is occupied
    int addToVacancy(NSpecies* s, double& speciesAngle, const bool& useNeighborVacancies);
    
    // remove the species from the vacancy. Returns -1 if the species did not occupy the vacancy
    int removeFromVacancy(NSpecies* s, const double& speciesAngle);
    
    // add a reaction to a vacancy using joint species angle. Returns -1 if the vacancy is fully occupied
    int addToVacancy(NReaction* r, const double& jointSpeciesAngle);
    
    // add a reaction to a vacancy using joint species. Returns -1 if the vacancy is fully occupied
    int addToVacancy(NReaction* r, NSpecies* jointSpecies);
    
    // remove the reaction from the vacancy using joint species angle.  Returns -1 if the reaction did not occupy the vacancy
    int removeFromVacancy(NReaction* r, const double& jointSpeciesAngle);
    
    // remove the reaction from the vacancy using joint species. Returns -1 if the reaction did not occupy the vacancy
    int removeFromVacancy(NReaction* r, NSpecies* jointSpecies);
    
    // swap the species located in the two vacancie of the reaction layer
    int swapSpecies (NSpecies* s1, NSpecies* s2);
    
    // find the vacancy index in which the entered species is located. Returns -1 if the species did not occupy any vacancy
    int findVacancy(NSpecies* s);
    
    // find the vacancy index in which the entered reaction is located. Returns -1 if the reaction did not occupy any vacancy
    int findVacancy(NReaction* r);
    
    // the number of reactions occupying the entered vacancy
    unsigned int numOfOccupiedReactionInVacancy(const unsigned int& vacancyIndex);
    
    // get total number of vacancies
    const size_t getNumVacancies() const;
    
    // add a species reference to a circumference vacancy using its pseduo species and number of transferring steps. Returns -1 if pseudospecies is not located in this layer
    int addToCircumferenceVacancy(NSpeciesReference* sr, NSpecies* pseudoSpecies, const int& numOfTrasferringSteps);
    
    // add a species reference to a circumference vacancy using its vacancy index and number of transferring steps. Returns -1 if it is not possbile to add it
    int addToCircumferenceVacancy(NSpeciesReference* sr, const unsigned int& reactionVacancyIndex, const int& numOfAngualrTrasferringSteps);
    
    // check to see if the adjacent circumference vacancies has enough capacity for a species reference. Returns -1 if adjacent circumference vacancies are fully occupied
    int checkCircumferenceVacancy(const unsigned int& reactionVacancyIndex, const int& numOfAngualrTrasferringSteps);
    
    // remove the species reference from a circumference vacancy. Returns -1 if the species reference did not occupy any circumference vacancy
    int removeFromCircumferenceVacancy(NSpeciesReference* sr);
    
    // add a species reference to a radial vacancy using its vacancy index. Returns -1 if the radial vacancy in question is fully occupied or blocked with a species
    int addToRadialVacancy(NSpeciesReference* sr, const unsigned int& reactionVacancyIndex);
    
    // check to see if the adjacent radial vacancy has enough capacity for a species reference. Returns -1 if adjacent radial vacancy is fully occupied
    int checkRadialVacancy(const unsigned int& reactionVacancyIndex);
    
    // remove the species reference from a radial vacancy. Returns -1 if the species reference did not occupy any circumference vacancy
    int removeFromRadialVacancy(NSpeciesReference* sr);
    
protected:
    unsigned int _layer;
    vacancyVec _vacancies;
    cVacancyVec _circumferenceVacancies;
    double _dAngle;
};

struct spcReferencedSide {
    SpcSide side;
    SReferenceVec speciesReferences;
};

struct reactionMember {
    double angle;
    NReaction* reaction;
    bool isMainReaction;
};

struct rxnReferencedSide {
    RxnSide side;
    SReferenceVec speciesReferences;
};

struct referencedEnd {
    RxnEnd end;
    SReferenceVec speciesReferences;
};

struct connectedReaction {
    speciesVec species;
    NSpecies* pseudoSpecies;
    NReaction* reaction;
};

struct radialVacancy {
    std::vector<std::pair<NSpeciesReference*, unsigned int>> sReferences;
};

struct vacancy {
    double angle;
    NSpecies* species;
    std::vector <NReaction*> reactions;
    radialVacancy rVacancy;
};

struct circumferenceVacancy {
    vacancy* startVacancy;
    vacancy* endVacancy;
    std::vector<std::pair<NSpeciesReference*, unsigned int>> sReferences;
};

}

#endif

