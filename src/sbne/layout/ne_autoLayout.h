#ifndef __NE_AUTOLAYOUT_H_
#define __NE_AUTOLAYOUT_H_

#include "sbne/network/ne_network.h"

#if GRAPHVIZ_INCLUDED
#include "graphviz/gvc.h"
#endif

namespace sbne {
#if GRAPHVIZ_INCLUDED
void locateNetworkItems(Network* net);
#endif

int setCurve(LCurve* curve, LPoint startPoint, const LPoint basePoint1, const LPoint basePoint2, const LPoint endPoint);

void locateCompartmentItems(Network* net, NCompartment* c);

void locateUnusedSpecies(NReaction* r, NSpecies* s, const double& usedSpeciesSAngle, const double& usedSpeciesEAngle, const unsigned int& lastUsedSpeciesIndex, const unsigned int& index);

void locateUsedSpecies(NReaction* r, NSpecies* s, double& usedSpeciesSAngle, double& usedSpeciesEAngle, const unsigned int& index);

void locateSReference(NReaction* r, NSpeciesReference* sr);

void setReactionCurve(NReaction* r);

void locateReactionSpecies(Network* net, NReaction* mR, NReaction* r, std::vector<NSpecies*>& locatedMainReactionSpecies);

void neighborMutualSpecies(NReaction* cR ,std::vector<NSpecies*> mutualSpecies);

void neighborTwoSpecies(NReaction* cR, const unsigned int& index1, const unsigned int& index2);

void swapSpeciesPositions(NSpecies* s1, NSpecies* s2);

void setSubReactionToReaction(NReaction* mR, NReaction* cR, NReaction* r, std::vector<NSpecies*> mutualSpecies);

void identifyMainAndSubReactions(NCompartment* c);

void addReferencedSideToSpecies(NReaction* r, NSpecies* s, double angle);

NSpecies* getPseudoSpecies(NReaction* mR, NReaction* cR, std::vector<NSpecies*> mutualSpecies);

void adjustSpeciesBoxPosition(NSpecies* s, double angle);

const LBox getSpeciesBoxOriginalPosition(NSpecies* s);

const LBox getSpeciesBoxOriginalPosition(NReaction* r, NSpecies* s);

std::vector<NSpecies*> findTheSameSpecies(std::vector<NSpecies*> sv1, std::vector<NSpecies*> sv2);

void packReactionsIntoCompartment(NCompartment* c);

void packCompartmentsIntoNetwork(Network* net);

void locateReactionSpeciesReferences(NReaction* r);

const SpcSide getSReferenceSpeciesSide(const LPoint& speciesBoxPoint, const LPoint& mReactionInnerLayerPoint);

const LPoint getSReferenceSpeciesBoxPoint(NSpecies* s, NSpeciesReference* sr, const SpcSide side);

const LPoint getSReferenceReactionEndPoint(NReaction* r, const double& sRAngle, RxnEnd& rxnEnd);

const LPoint getSReferenceReactionSidePoint(NReaction* r, const double& sRAngle, RxnSide& rxnSide);

void setSpeciesReferenceCurve(NReaction* mR, NReaction* r, NSpeciesReference* sr, NSpecies* pseudoSpecies, SpcSide speciesSide, const RxnEnd rxnEnd, const RxnSide rxnSide);

LLineSegment* extendCurve(LCurve* curve, LLineSegment* l, LPoint& startPoint, const double& dx, const double& dy);

LLineSegment* extendCurve(LCurve* curve, LLineSegment* l, LPoint& startPoint, const LPoint& newPoint);

LLineSegment* extendCurve(LCurve* curve, LLineSegment* l, LPoint& startPoint, const LPoint& newPoint, const LPoint& basePoint1, const LPoint& basePoint2, const bool& isArcLike);

void calculateCurvePointsFeatures(NReaction* mR, NReaction* r, NSpecies* pseudoSpecies, NSpeciesReference* sr, LPoint& pseudoPoint, LPoint& basePoint1, LPoint& basePoint2, LPoint& mReactionOuterLayerPoint, LPoint& mReactionInnerLayerPoint, LPoint& offsetOnLayer, const LPoint& offsetOnReaction, int offsetOnReactionSign, int& numTransferringSteps);

std::vector<std::vector<LPoint*>> convertManhatanPathToPoints(NReaction* mR, NSpecies* species, const std::vector<std::pair<int, int>>& manhatanPath);

}

#endif
