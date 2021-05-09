#include "sbne/layout/ne_autoLayout.h"

namespace sbne {

void locateCompartmentItems(Network* net, NCompartment* c) {
   
   // add all the species and reaction to the default compartment
   if (net->getNumCompartments() == 1 || stringCompare(c->getId(), "default_Compartment")) {
       c->setSpecies(net->getSpecies());
       c->setReactions(net->getReactions());
   }
   
   // identify the main reactions and sub reactions
   identifyMainAndSubReactions(c);
   
   // reactions
   NReaction* r = NULL;
   NReaction* cR = NULL;
   NReaction* mR = NULL;
   speciesVec locatedMainReactionSpecies;
   for (constReactionIt rIt = c->reactionsBegin(); rIt != c->reactionsEnd(); ++rIt) {
       r = *rIt;
       
       if (r->getNumSpecies() > 0) {
           // main reactions
           if (r->getNumConnectedReactions() == 0) {
               r->setMatchWithGlyph(true);
               r->setUsed(true);
               
               // set it as the main reaction
               mR = r;
               
               // empty the list of already located speceis of the main reaction
               locatedMainReactionSpecies.clear();

               // set id
               r->setGlyphId(net->getReactionUniqueGlyphId(r->getId()));
               
               // set the default radius of main reaction
               r->setRadius((4 * cReactionRadius + 3 * maxSpeciesBoxWidth) / (std::sqrt(2 * (1 - std::cos(2 * pi / r->getNumSpecies())))));

               // set the default center point of main reaction
               r->setCenter(LPoint(2 * r->getRadius(), 2 * r->getRadius()));
               
               // species
               locateReactionSpecies(net, mR, r, locatedMainReactionSpecies);
           }
           
           // for sub reactions
           else {
               // add sub reaction to the main reaction
               mR->addSubReaction(r);
               
               r->setMatchWithGlyph(true);
               r->setUsed(true);
               
               // set id
               r->setGlyphId(net->getReactionUniqueGlyphId(r->getId()));
               
               // set the radius of reaction
               r->setRadius(cReactionRadius);
               
               connectedReaction* cReaction = r->getConnectedReactions().at(0);
               cR = cReaction->reaction;
               
               std::vector<NSpecies*> mutualSpecies = findTheSameSpecies(r->getSpecies(), locatedMainReactionSpecies);
               
               // put the mutual reactions near each other
               neighborMutualSpecies(cR, cReaction->species);
               
               // set the position of side reaction on a species side
               setSubReactionToReaction(mR, cR, r, mutualSpecies);
               
               // species
               locateReactionSpecies(net, mR, r, locatedMainReactionSpecies);
           }
       }
   }
   
   // reaction curve and species references
   for (constReactionIt rIt = c->reactionsBegin(); rIt != c->reactionsEnd(); ++rIt) {
       
       if ((*rIt)->getNumSpecies() > 0) {
           locateReactionSpeciesReferences(*rIt);
           setReactionCurve(*rIt);
       }
   }
       
   // find the position of the main reactions in the compartment and set the dimensions of the compartment
   packReactionsIntoCompartment(c);
}

void locateUnusedSpecies(NReaction* r, NSpecies* s, const double& usedSpeciesSAngle, const double& usedSpeciesEAngle, const unsigned int& lastUsedSpeciesIndex, const unsigned int& index) {
   LPoint centerP = r->getCenter();
   double radius = r->getRadius();
   unsigned int numSpecies = r->getNumSpecies();
   LBox* speciesBox = new LBox();
   
   // calcuate the angle in the reaction circle
   double angle = usedSpeciesEAngle + (((2 * pi - (usedSpeciesEAngle - usedSpeciesSAngle))) / double(numSpecies - lastUsedSpeciesIndex)) * (index - lastUsedSpeciesIndex);
   
   while (angle >= 2 * pi)
       angle -= 2 * pi;
   
   // set speceis as a reactant of the reaction
   s->addReactionMember(r, angle, true);
   
   // add species to the reaction's vacancy
   r->addSpeciesToVacancy(s, angle);
   
   // set the position
   speciesBox->setX(centerP.x() + radius * std::cos(angle));
   speciesBox->setY(centerP.y() - radius * std::sin(angle));
   
   // set dimensions
   speciesBox->setWidth(minSpeciesBoxWidth);
   speciesBox->setHeight(minSpeciesBoxHeight);
   
   // set species box;
   s->setBox(speciesBox);
   
   addReferencedSideToSpecies(r, s, angle);
   
   if (r->findSpeciesIndexById(s->getId()) == 0)
       s->setLockedPosition(true);
}

void locateUsedSpecies(NReaction* r, NSpecies* s, double& usedSpeciesSAngle, double& usedSpeciesEAngle, const unsigned int& index) {
   
   // set angle
   LBox oSpeciesBox = getSpeciesBoxOriginalPosition(r, s);
   double dy = r->getCenter().y() - oSpeciesBox.y();
   double dx = oSpeciesBox.x() - r->getCenter().x();
   double angle = atan2(dy, dx);
   
   while (std::floor(angle) < 0)
       angle += 2 * pi;
   while (angle > 2 * pi)
       angle -= 2 * pi;
   
   // set used species start angle and end angle
   if (index == 0) {
       usedSpeciesSAngle = angle;
       usedSpeciesEAngle = angle;
   }
   else {
       if (angle > usedSpeciesEAngle) {
           if (usedSpeciesEAngle - (angle - 2 * pi) > angle - usedSpeciesSAngle)
               usedSpeciesEAngle = angle;
           else
               usedSpeciesSAngle = angle - 2 * pi;
       }
           
       if (angle < usedSpeciesSAngle) {
           if ((angle + 2 * pi) - usedSpeciesSAngle > usedSpeciesSAngle - angle)
               usedSpeciesSAngle = angle;
           else
               usedSpeciesEAngle = angle + 2 * pi;
       }
   }
   
   // set the start angle of the speceis of the reaction
   r->setSpeciesStartAngle(usedSpeciesSAngle);
   
   // add species to the reaction's vacancy
   r->addSpeciesToVacancy(s, angle);
   
   // set speceis as a reactant of the reaction
   s->addReactionMember(r, angle, false);
}

void locateSReference(NReaction* r, NSpeciesReference* sr) {
   LPoint centerP = r->getCenter();
   double radius = r->getRadius();
   NSpecies* s = sr->getSpecies();
   LPoint boxPoint, reactionEndPoint, reactionSidePoint;
   SpcSide speciesSide;
   RxnEnd rxnEnd;
   RxnSide rxnSide;
   
   // set line segment points
   if (s->findReactionMemberById(r->getId())) {
       // get main reaction and pseudo species
       NReaction* mR = NULL;
       NSpecies* pseudoSpecies = NULL;
       for (NReaction::constConnectedRIt cRIt = r->connectedRBegin(); cRIt != r->connectedREnd(); ++cRIt) {
           if ((*cRIt)->reaction->getNumConnectedReactions() == 0) {
               mR = (*cRIt)->reaction;
               pseudoSpecies = (*cRIt)->pseudoSpecies;
               break;
           }
       }
       
       if (mR) {
            // if the reaction is not the main reaction
           if (!stringCompare(r->getId(), mR->getId())) {
               // add the species reference as a sub speceis reference to the main reaction
               mR->addSubSpeciesReference(sr);
               
               // if the species is located in this reaction for the first time, there is no need for pseudospecies
               for (NSpecies::constRMemberIt rMIt = s->rMembersBegin(); rMIt != s->rMembersEnd(); ++rMIt) {
                   if ((*rMIt)->isMainReaction && stringCompare((*rMIt)->reaction->getId(), r->getId()))
                       pseudoSpecies = NULL;
               }
           }
       }
       
       // set quadrant of species reference
       double sRAngle = 0.0;
       if (pseudoSpecies) {
           LPoint pSPoint = getSpeciesBoxOriginalPosition(pseudoSpecies).upperLeftCorner();
           sRAngle = atan2(r->getCenter().y() - pSPoint.y(), pSPoint.x() - r->getCenter().x());
           while (std::floor(sRAngle) < 0)
               sRAngle += 2 * pi;
           while (sRAngle > 2 * pi)
               sRAngle -= 2 * pi;
       }
       else
           sRAngle = s->findReactionMemberById(r->getId())->angle;
       sr->setQuadrant(angleToQuadrant(sRAngle));
       
       switch(sr->getQuadrant()) {
           case Quad_I_1:
               speciesSide = LEFT_SIDE;
               rxnEnd = END_END;
               rxnSide = UP_END_SIDE;
               break;
               
           case Quad_I_2:
               speciesSide = BOTTOM_SIDE;
               rxnEnd = END_END;
               rxnSide = UP_END_SIDE;
               break;
               
           case Quad_II_1:
               speciesSide = BOTTOM_SIDE;
               rxnEnd = START_END;
               rxnSide = UP_START_SIDE;
               break;
           
           case Quad_II_2:
               speciesSide = RIGHT_SIDE;
               rxnEnd = START_END;
               rxnSide = UP_START_SIDE;
               break;
               
           case Quad_III_1:
               speciesSide = RIGHT_SIDE;
               rxnEnd = START_END;
               rxnSide = DOWN_START_SIDE;
               break;
               
           case Quad_III_2:
               speciesSide = TOP_SIDE;
               rxnEnd = START_END;
               rxnSide = DOWN_START_SIDE;
               break;
               
           case Quad_IV_1:
               speciesSide = TOP_SIDE;
               rxnEnd = END_END;
               rxnSide = DOWN_END_SIDE;
               break;
               
           case Quad_IV_2:
               speciesSide = LEFT_SIDE;
               rxnEnd = END_END;
               rxnSide = DOWN_END_SIDE;
               break;
       }

       // get the species box point of species reference
       boxPoint = getSReferenceSpeciesBoxPoint(s,sr, speciesSide);
       
       // for substrates and products
       if (sr->getRole() < 4) {
           reactionEndPoint = getSReferenceReactionEndPoint(r, sRAngle, rxnEnd);
           r->addReferencedEnd(sr, rxnEnd);
       }
       // for modifiers, activators, and inhibitors
       else {
           reactionSidePoint = getSReferenceReactionSidePoint(r, sRAngle, rxnSide);
           r->addReferencedSide(sr, rxnSide);
       }
       
       // set start and end points of a species reference
       // for substrates
       if (sr->getRole() == 0 || sr->getRole() == 2) {
           sr->setStartPoint(boxPoint);
           sr->setEndPoint(reactionEndPoint);
       }
       // for products
       else if (sr->getRole() == 1 || sr->getRole() == 3) {
           sr->setStartPoint(reactionEndPoint);
           sr->setEndPoint(boxPoint);
       }
       // for modifiers, activators, and inhibitors
       else {
           sr->setStartPoint(boxPoint);
           sr->setEndPoint(reactionSidePoint);
       }
       
       // set the curve of the species reference
       setSpeciesReferenceCurve(mR, r, sr, pseudoSpecies, speciesSide, rxnEnd, rxnSide);
   }
}

void setReactionCurve(NReaction* r) {
   LPoint sPoint = r->getCenter();
   LPoint ePoint = r->getCenter();
   int maxNumLEndings = 0;
   
   if (r->getNumSpecies() == 1) {
       if (r->getEndSpeciesReferences(START_END).size() || r->getSideSpeciesReferences(UP_START_SIDE).size() || r->getSideSpeciesReferences(DOWN_START_SIDE).size())
           sPoint += LPoint(-3 * lLendingSize * std::cos(r->getDirectionAngle()), 3 * lLendingSize * std::sin(r->getDirectionAngle()));
       else
           ePoint += LPoint(3 * lLendingSize * std::cos(r->getDirectionAngle()), -3 * lLendingSize * std::sin(r->getDirectionAngle()));
   }
   // both left and right sides
   else if (r->getEndSpeciesReferences(START_END).size() && r->getEndSpeciesReferences(END_END).size()) {
       sPoint += LPoint(- 3 * lLendingSize * std::cos(r->getDirectionAngle()), 3 * lLendingSize * std::sin(r->getDirectionAngle()));
       ePoint += LPoint(3 * lLendingSize * std::cos(r->getDirectionAngle()), -3 * lLendingSize * std::sin(r->getDirectionAngle()));
   }
   else {
       // start side
       if (r->getEndSpeciesReferences(START_END).size() && (r->getSideSpeciesReferences(UP_END_SIDE).size() || r->getSideSpeciesReferences(UP_START_SIDE).size() || r->getSideSpeciesReferences(DOWN_END_SIDE).size() || r->getSideSpeciesReferences(DOWN_START_SIDE).size()))
           sPoint += LPoint(-3 * lLendingSize * std::cos(r->getDirectionAngle()), 3 * lLendingSize * std::sin(r->getDirectionAngle()));
       else if (r->getSideSpeciesReferences(UP_START_SIDE).size() || r->getSideSpeciesReferences(DOWN_START_SIDE).size()) {
           maxNumLEndings = std::max(r->getSideSpeciesReferences(UP_START_SIDE).size(), r->getSideSpeciesReferences(DOWN_START_SIDE).size());
           maxNumLEndings = std::min(maxNumLEndings, 3);
           sPoint += LPoint(- maxNumLEndings * lLendingSize * std::cos(r->getDirectionAngle()), maxNumLEndings * lLendingSize * std::sin(r->getDirectionAngle()));
       }
       
       // end side
       if (r->getEndSpeciesReferences(END_END).size() && (r->getSideSpeciesReferences(UP_END_SIDE).size() || r->getSideSpeciesReferences(UP_START_SIDE).size() || r->getSideSpeciesReferences(DOWN_END_SIDE).size() || r->getSideSpeciesReferences(DOWN_START_SIDE).size()))
           ePoint += LPoint(3 * lLendingSize * std::cos(r->getDirectionAngle()), -3 * lLendingSize * std::sin(r->getDirectionAngle()));
       else if (r->getSideSpeciesReferences(UP_END_SIDE).size() || r->getSideSpeciesReferences(DOWN_END_SIDE).size()) {
           maxNumLEndings = std::max(r->getSideSpeciesReferences(UP_END_SIDE).size(), r->getSideSpeciesReferences(DOWN_END_SIDE).size());
           maxNumLEndings = std::min(maxNumLEndings, 3);
           ePoint += LPoint(maxNumLEndings * lLendingSize * std::cos(r->getDirectionAngle()), -maxNumLEndings * lLendingSize * std::sin(r->getDirectionAngle()));
       }
   }
   
   LCurve* curve = new LCurve();
   LLineSegment* l = new LLineSegment();
   l->setId(curve->getElementUniqueId());
   l->setStart(sPoint);
   l->setEnd(ePoint);
   curve->addToListOfElementsEnd(l);
   r->setCurve(curve);
}

void locateReactionSpecies(Network* net, NReaction* mR, NReaction* r, std::vector<NSpecies*>& locatedMainReactionSpecies) {
   
   // sort the species to put the used ones at the beginning of the list
   std::deque<NSpecies*> sd;
   for (constSpeciesIt sIt = r->speciesBegin(); sIt != r->speciesEnd(); ++sIt) {
       if ((*sIt)->isUsed())
           sd.push_front(*sIt);
       else
           sd.push_back(*sIt);
   }
   speciesVec sv(sd.begin(), sd.end());
   r->setSpecies(sv);
    
   // locate the species
   NSpecies* s = NULL;
   unsigned int lastUsedSpeciesIndex = 0;
   double usedSpeciesSAngle = 0.0;
   double usedSpeciesEAngle = 0.0;
   for (constSpeciesIt sIt = r->speciesBegin(); sIt != r->speciesEnd(); ++sIt) {
       s = *sIt;
       
       // if it is already displayed in another reaction
       if (s->isUsed()) {
           s->setLockedPosition(true);
           locateUsedSpecies(r, s, usedSpeciesSAngle, usedSpeciesEAngle, sIt - r->speciesBegin());
           lastUsedSpeciesIndex = sIt - r->speciesBegin();
       }
       // if it is not already displayed in another reaction
       else {
           s->setMatchWithGlyph(true);
           s->setUsed(true);
           
           // set id
           s->setGlyphId(net->getSpeciesUniqueGlyphId(s->getId()));
           
           // add species to main reaction as a sub species
           if (!stringCompare(mR->getId(), r->getId()))
               mR->addSubSpecies(s);
           
           // set the species bounding box
           locateUnusedSpecies(r, s, usedSpeciesSAngle, usedSpeciesEAngle, lastUsedSpeciesIndex, sIt - r->speciesBegin());
           
           // add to the list of located species of the main reaction
           locatedMainReactionSpecies.push_back(s);
       }
       
   }
}

void neighborMutualSpecies(NReaction* cR, std::vector<NSpecies*> mutualSpecies) {
   
   for (int degreeSubReaction = 2; degreeSubReaction <= cR->getNumSpecies() - 1; ++degreeSubReaction) {
       if (degreeSubReaction == mutualSpecies.size()) {
           std::vector<unsigned int> mSpeciesIndices;
           for (constSpeciesIt sIt = mutualSpecies.begin(); sIt != mutualSpecies.end(); ++sIt)
               mSpeciesIndices.push_back( cR->findSpeciesIndexById((*sIt)->getId()));
               std::sort(mSpeciesIndices.begin(), mSpeciesIndices.end());
           
           if (cR->getNumSpecies() >= degreeSubReaction + 2) {
               for (int i = 0; i < degreeSubReaction - 1; ++i)
                   neighborTwoSpecies(cR, mSpeciesIndices.at(i), mSpeciesIndices.at(i + 1));
           }
       }
   }
}

void neighborTwoSpecies(NReaction* cR, const unsigned int& index1, const unsigned int& index2) {
   speciesVec sv = cR->getSpecies();
   NSpecies* s;
   
   for (int i = 0; i < cR->getNumSpecies() - 1; ++i) {
       for (int j = i + 1; j < cR->getNumSpecies(); ++j) {
           if (i == index1 && j == index2) {
               if ((j - i) >= 2 && ((j + 1) % cR->getNumSpecies()) != i) {
                   swapSpeciesPositions(cR->getSpecies().at(i + 1), cR->getSpecies().at(j));
                   cR->getReactionLayers().front()->swapSpecies(cR->getSpecies().at(i + 1), cR->getSpecies().at(j));
                   
                   s = cR->getSpecies().at(i + 1);
                   sv.erase (sv.begin() + i + 1);
                   sv.insert(sv.begin() + i + 1, cR->getSpecies().at(j));
                   sv.erase (sv.begin() + j);
                   sv.insert(sv.begin() + j, s);
                   cR->setSpecies(sv);
               }
           }
       }
   }
}

void swapSpeciesPositions(NSpecies* s1, NSpecies* s2) {
   if (!s1->isLockedPosition() && !s2->isLockedPosition()) {
       // swap the bouding boxes
       LBox* sBox = s1->getBox();
       s1->setBox(s2->getBox());
       s2->setBox(sBox);
       s2->setLockedPosition(true);
       
       // swap the refrenced sides
       SpcSide side;
       for (NSpecies::constRSideIt rSIt1 = s1->rSideBegin(); rSIt1 != s1->rSideEnd(); ++rSIt1) {
           for (NSpecies::constRSideIt rSIt2 = s2->rSideBegin(); rSIt2 != s2->rSideEnd(); ++rSIt2) {
               side = (*rSIt2)->side;
               (*rSIt2)->side = (*rSIt1)->side;
               (*rSIt1)->side = side;
           }
       }
       
       // swap reaction members
       double angle = 0.0;
       for (NSpecies::constRMemberIt rMIt1 = s1->rMembersBegin(); rMIt1 != s1->rMembersEnd(); ++rMIt1) {
           for (NSpecies::constRMemberIt rMIt2 = s2->rMembersBegin(); rMIt2 != s2->rMembersEnd(); ++rMIt2) {
               if (stringCompare((*rMIt1)->reaction->getId(), (*rMIt2)->reaction->getId())) {
                   angle = (*rMIt1)->angle;
                   (*rMIt1)->angle = (*rMIt2)->angle;
                   (*rMIt2)->angle = angle;
               }
           }
       }
   }
}

void setSubReactionToReaction(NReaction* mR, NReaction* cR, NReaction* r, std::vector<NSpecies*> mutualSpecies) {
   NSpecies* s = NULL;
   SpcSide speciesSide;
   double sAngle = 0.0;
   double pSpeciesAngle = 0.0;
   int numOccupiedReactionsOfSpecies = 0;
   //CoordSysQuad quadrant;
   
   std::vector<NSpecies*> theSameSpecies = findTheSameSpecies( cR->getSpecies(), mutualSpecies);
   
   // if the mutual speceis are all present in the connected reaction
   if (theSameSpecies.size() == mutualSpecies.size()) {
       s = getPseudoSpecies(mR, cR, mutualSpecies);
       sAngle = s->findReactionMemberById(cR->getId())->angle;
       numOccupiedReactionsOfSpecies = cR->getNumOccupiedReactionsOfSpecies(s);
   }
   
   // if the mutual species are present in other reactions, too
   else {
       s = getPseudoSpecies(mR, mR, mutualSpecies);
       sAngle = s->findReactionMemberById(mR->getId())->angle;
       numOccupiedReactionsOfSpecies = mR->getNumOccupiedReactionsOfSpecies(s);
   }
   
   if (s->isPseudoSpecies()) {
       switch (numOccupiedReactionsOfSpecies) {
           case 0:
               pSpeciesAngle = pi + sAngle;
               break;
               
           case 1:
               pSpeciesAngle = 3.0 * pi / 2.0 + sAngle;
               break;
               
           case 2:
               pSpeciesAngle = pi / 2.0 + sAngle;
               break;
               
           default:
               break;
       }
   }
   else {
       switch (numOccupiedReactionsOfSpecies) {
           case 0:
               pSpeciesAngle = 3.0 * pi / 2.0 + sAngle;
               break;
               
           case 1:
               pSpeciesAngle = pi / 2.0 + sAngle;
               break;
               
           case 2:
               pSpeciesAngle = pi + sAngle;
               break;
               
           default:
               break;
       }
   }
   
   switch (angleToQuadrant(pSpeciesAngle)) {
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
           
       default:
           break;
   }
   
   if (s->isPseudoSpecies()) {
       // locate reaction position
       LPoint sPoint = getSpeciesBoxOriginalPosition(s).upperLeftCorner();
       r->setCenter(LPoint(sPoint.x() + r->getRadius() * std::cos(pSpeciesAngle - pi), sPoint.y() - r->getRadius() * std::sin(pSpeciesAngle - pi)));
       r->setDirectionAngle(pSpeciesAngle - pi);
       
       // set pseudoSpecies of the connected reaction
       r->setPseudoSpeciesOfConnectedReaction(cR, s);
       
       // if connected reaction is not the main reaction
       if (!stringCompare(mR->getId(), cR->getId())) {
           r->addConnectedReaction(mR);
           r->setPseudoSpeciesOfConnectedReaction(mR, s);
       }
       
       // add reaction member to the pseudo speices
       s->addReactionMember(r, pSpeciesAngle, false);
   }
   else {
       // locate reaction position
       LBox* sBox = s->getBox();
       
       if (r->getNumSpecies() == 1) {
           switch (speciesSide) {
               case TOP_SIDE:
                   r->setDirectionAngle(pi / 2.0);
                   r->setCenter(LPoint(sBox->x() + 0.5 * sBox->width(), sBox->y() - r->getRadius()));
                   break;
               case LEFT_SIDE:
                   r->setDirectionAngle(pi);
                   r->setCenter(LPoint(sBox->x() - r->getRadius(), sBox->y() + 0.5 * sBox->height()));
                   break;
               case RIGHT_SIDE:
                   r->setDirectionAngle(0.0);
                   r->setCenter(LPoint(sBox->x() + sBox->width() + r->getRadius(), sBox->y() + 0.5 * sBox->height()));
                   break;
               case BOTTOM_SIDE:
                   r->setDirectionAngle(3 * pi / 2.0);
                   r->setCenter(LPoint(sBox->x() + 0.5 * sBox->width(), sBox->y() + sBox->height() + r->getRadius()));
           }
       }
       else {
           r->setCenter(LPoint(sBox->center().x() + (0.5 * sBox->width() + r->getRadius()) * std::cos(pSpeciesAngle - pi), sBox->center().y() - (0.5 * sBox->height() + r->getRadius()) * std::sin(pSpeciesAngle - pi)));
           r->setDirectionAngle(pSpeciesAngle - pi);
       }
   }
   
   // add sub reaction to the main reaction vacancy
   mR->addSubReactionToVacancy(r, s);
   
   // if reaction is added to the connected reaction, add it to its vacancy
   if (theSameSpecies.size() == mutualSpecies.size() && !stringCompare(cR->getId(), mR->getId()))
       cR->addSubReactionToVacancy(r, s);
}

void identifyMainAndSubReactions(NCompartment* c) {
   reactionVec rv, subRV, sortedRV, possibleMRV;
   rv = c->getReactions();
   constReactionIt rIt1, rIt2, subRIt, pMRIt;
   NReaction* r;
   bool isSubReaction;
   bool reactionAlreadyExists;
   bool possibleMainReactionAlreadyExists;
   bool ispMRVChanged;
   
   // separate main and sub reactions
   while (rv.size()) {
       // sort the reactions based on their number of species
       std::sort(rv.begin(), rv.end(), [](NReaction* a, NReaction* b) -> bool { return a->getNumSpecies() > b->getNumSpecies(); });
       
       rIt1 = rv.begin();
   
       while ((*rIt1)->getNumConnectedReactions() != 0 && rIt1 != rv.end())
           ++rIt1;
       
       // for a main reaction
       subRV.clear();
       possibleMRV.clear();
       if (rIt1 != rv.end()) {
           r = *rIt1;
           rv.erase(rIt1);
           for (rIt2 = rv.begin(); rIt2 != rv.end(); ++rIt2) {
               for(constSpeciesIt sIt1 = r->speciesBegin(); sIt1 != r->speciesEnd(); sIt1++) {
                   for(constSpeciesIt sIt2 = (*rIt2)->speciesBegin(); sIt2 != (*rIt2)->speciesEnd(); sIt2++) {
                       if (stringCompare((*sIt1)->getId(), (*sIt2)->getId())) {
                           if (!(*rIt2)->findConnectedReactionsBySpeciesId((*sIt1)->getId()).size())
                               (*rIt2)->addConnectedReaction(r, *sIt1);
                           
                           for (subRIt = subRV.begin(); subRIt != subRV.end(); ++subRIt) {
                               if (stringCompare((*rIt2)->getId(), (*subRIt)->getId()))
                                   break;
                           }
                           
                           if (subRIt == subRV.end())
                               subRV.push_back(*rIt2);
                       }
                   }
               }
           }
           
           // add the main reaction to the sorted reaction vector
           sortedRV.push_back(r);
       }
       else
           break;
       
       // for a possible main reaction
       ispMRVChanged = false;
       while(1) {
           rIt1 = rv.begin();
           while ((*rIt1)->getNumConnectedReactions() != 0 && rIt1 != rv.end())
               ++rIt1;
           
           if (rIt1 != rv.end()) {
               r = *rIt1;
               isSubReaction = false;
               for (rIt2 = subRV.begin(); rIt2 != subRV.end(); ++rIt2) {
                   for(constSpeciesIt sIt1 = r->speciesBegin(); sIt1 != r->speciesEnd(); sIt1++) {
                       for(constSpeciesIt sIt2 = (*rIt2)->speciesBegin(); sIt2 != (*rIt2)->speciesEnd(); sIt2++) {
                           if (stringCompare((*sIt1)->getId(), (*sIt2)->getId())) {
                               if (!r->findConnectedReactionsBySpeciesId((*sIt2)->getId()).size())
                                   r->addConnectedReaction(*rIt2, *sIt2);
                               isSubReaction = true;
                           }
                       }
                   }
               }
               
               // if it is a sub reation
               if (isSubReaction) {
                   subRV.push_back(r);
                   
                   for (pMRIt = possibleMRV.begin(); pMRIt != possibleMRV.end(); ++pMRIt) {
                       if (stringCompare((*pMRIt)->getId(), r->getId())) {
                           possibleMRV.erase(pMRIt);
                           ispMRVChanged = true;
                           break;
                       }
                   }
               }
               else {
                   rv.erase(rIt1);
                   rv.push_back(r);
                   
                   possibleMainReactionAlreadyExists = false;
                   for (pMRIt = possibleMRV.begin(); pMRIt != possibleMRV.end(); ++pMRIt) {
                       if (stringCompare((*pMRIt)->getId(), r->getId())) {
                           possibleMainReactionAlreadyExists = true;
                           break;
                       }
                   }
                   
                   if (!possibleMainReactionAlreadyExists)
                       possibleMRV.push_back(r);
                   else {
                       if (pMRIt == possibleMRV.end() - 1) {
                           if (ispMRVChanged)
                               ispMRVChanged = false;
                           else
                               break;
                       }
                   }
               }
           }
           else
               break;
       }
       
       // add sub reactions to the sorted reaction vector and remove them from reaction vector
       subRIt = subRV.begin();
       while (subRIt != subRV.end()) {
           if ((*subRIt)->getNumConnectedReactions() == 1 && (*subRIt)->getConnectedReactions().at(0)->species.size() == 1) {
               for (rIt1 = sortedRV.begin(); rIt1 != sortedRV.end(); ++rIt1) {
                   if (stringCompare((*subRIt)->getConnectedReactions().at(0)->reaction->getId(), (*rIt1)->getId()))
                       break;
               }
               
               if (rIt1 != sortedRV.end()) {
                   sortedRV.push_back(*subRIt);
                   
                   for (rIt1 = rv.begin(); rIt1 != rv.end();) {
                       if (stringCompare((*subRIt)->getId(), (*rIt1)->getId())) {
                           rv.erase(rIt1);
                           break;
                       }
                       else
                           ++rIt1;
                   }
                   
                   subRV.erase(subRIt);
               }
               else
                   ++subRIt;
           }
           else
               ++subRIt;
       }
       
       for (subRIt = subRV.begin(); subRIt != subRV.end(); ++subRIt) {
           sortedRV.push_back(*subRIt);
           for (rIt1 = rv.begin(); rIt1 != rv.end();) {
               if (stringCompare((*subRIt)->getId(), (*rIt1)->getId())) {
                   rv.erase(rIt1);
                   break;
               }
               else
                   ++rIt1;
           }
       }
   }
   
   // set the compartments sorted reactions
   c->setReactions(sortedRV);
}

void addReferencedSideToSpecies(NReaction* r, NSpecies* s, double angle) {
   while (std::floor(angle) < 0)
       angle += 2 * pi;
   while (angle > 2 * pi)
       angle -= 2 * pi;
   
   // adjust the species box position
   if (!s->isLockedPosition())
       adjustSpeciesBoxPosition(s, angle);
   
   // add a referenced side to species
   if ((angle >= 0.0 && angle <= pi / 4.0) || (angle > 7 * pi / 4.0 && angle < 2 * pi))
       s->addReferencedSide(LEFT_SIDE);
   else if (angle > pi / 4.0 && angle <= 3 * pi / 4.0)
       s->addReferencedSide(BOTTOM_SIDE);
   else if (angle > 3 * pi / 4.0 && angle <= 5 * pi / 4.0)
       s->addReferencedSide(RIGHT_SIDE);
   else if (angle > 5 * pi / 4.0 && angle <= 7 * pi / 4.0)
       s->addReferencedSide(TOP_SIDE);
}

void adjustSpeciesBoxPosition(NSpecies* s, double angle) {
   while (std::floor(angle) < 0)
       angle += 2 * pi;
   while (angle > 2 * pi)
       angle -= 2 * pi;
   
   LBox* speciesBox = s->getBox();
   
   // adjust the box position
   if ((angle >= 0.0 && angle <= pi / 4.0) || (angle > 7 * pi / 4.0 && angle < 2 * pi))
       speciesBox->setY(speciesBox->y() - 0.5 * speciesBox->height());
   else if (angle > pi / 4.0 && angle <= 3 * pi / 4.0) {
       speciesBox->setX(speciesBox->x() - 0.5 * speciesBox->width());
       speciesBox->setY(speciesBox->y() - speciesBox->height());
   }
   else if (angle > 3 * pi / 4.0 && angle <= 5 * pi / 4.0) {
       speciesBox->setX(speciesBox->x() - speciesBox->width());
       speciesBox->setY(speciesBox->y() - 0.5 * speciesBox->height());
   }
   else if (angle > 5 * pi / 4.0 && angle <= 7 * pi / 4.0)
       speciesBox->setX(speciesBox->x() - 0.5 * speciesBox->width());
   
   s->setBox(speciesBox);
}

const LBox getSpeciesBoxOriginalPosition(NSpecies* s) {
   double angle = 0.0;
   for (NSpecies::constRMemberIt rMIt = s->rMembersBegin(); rMIt != s->rMembersEnd(); ++rMIt) {
       if ((*rMIt)->isMainReaction)
           angle = (*rMIt)->angle;
   }
   
   while (std::floor(angle) < 0)
       angle += 2 * pi;
   while (angle > 2 * pi)
       angle -= 2 * pi;
   
   LBox speciesBox = *(s->getBox());
   
   // get the original position of the box
   if ((angle >= 0.0 && angle <= pi / 4.0) || (angle > 7 * pi / 4.0 && angle < 2 * pi))
       speciesBox.setY(speciesBox.y() + 0.5 * speciesBox.height());
   else if (angle > pi / 4.0 && angle <= 3 * pi / 4.0) {
       speciesBox.setX(speciesBox.x() + 0.5 * speciesBox.width());
       speciesBox.setY(speciesBox.y() + speciesBox.height());
   }
   else if (angle > 3 * pi / 4.0 && angle <= 5 * pi / 4.0) {
       speciesBox.setX(speciesBox.x() + speciesBox.width());
       speciesBox.setY(speciesBox.y() + 0.5 * speciesBox.height());
   }
   else if (angle > 5 * pi / 4.0 && angle <= 7 * pi / 4.0)
       speciesBox.setX(speciesBox.x() + 0.5 * speciesBox.width());
   
   return speciesBox;
}

const LBox getSpeciesBoxOriginalPosition(NReaction* r, NSpecies* s) {
   LBox speciesBox = *(s->getBox());
   double dy = r->getCenter().y() - speciesBox.y();
   double dx = speciesBox.x() - r->getCenter().x();
   double angle = atan2(dy, dx);
   
   while (std::floor(angle) < 0)
       angle += 2 * pi;
   while (angle > 2 * pi)
       angle -= 2 * pi;
   
   switch (angleToQuadrant(angle)) {
       case Quad_I_1:
       case Quad_IV_2:
           speciesBox.setY(speciesBox.y() + 0.5 * speciesBox.height());
           break;
           
       case Quad_I_2:
       case Quad_II_1:
           speciesBox.setX(speciesBox.x() + 0.5 * speciesBox.width());
           speciesBox.setY(speciesBox.y() + speciesBox.height());
           break;
           
       case Quad_II_2:
       case Quad_III_1:
           speciesBox.setX(speciesBox.x() + speciesBox.width());
           speciesBox.setY(speciesBox.y() + 0.5 * speciesBox.height());
           break;
       
       case Quad_III_2:
       case Quad_IV_1:
           speciesBox.setX(speciesBox.x() + 0.5 * speciesBox.width());
           break;
   }
   
   return speciesBox;
}

std::vector<NSpecies*> findTheSameSpecies(std::vector<NSpecies*> sv1, std::vector<NSpecies*> sv2) {
   std::vector<NSpecies*> theSameSpecies;
   for (int i = 0; i < sv1.size(); ++i) {
       for (int j = 0; j < sv2.size(); ++j) {
           if (stringCompare(sv1.at(i)->getId(), sv2.at(j)->getId()))
               theSameSpecies.push_back(sv1.at(i));
       }
   }
   
   return theSameSpecies;
}

void packReactionsIntoCompartment(NCompartment* c) {
   std::vector<LBox*> mainReactionBoxes;
   LBox compartmentBox, * reactionBox;
   for (constReactionIt rIt = c->reactionsBegin(); rIt != c->reactionsEnd(); ++rIt) {
       if ((*rIt)->getNumConnectedReactions() == 0 && (*rIt)->isMatchWithGlyph()) {
           reactionBox = new LBox((*rIt)->getExtentBox());
           mainReactionBoxes.push_back(reactionBox);
       }
   }
   
   compartmentBox = packBoxesIntoOptimalBox(c->getBox()->upperLeftCorner(), mainReactionBoxes);
   
   c->getBox()->setX(compartmentBox.x());
   c->getBox()->setY(compartmentBox.y());
   if (compartmentBox.width() > c->getBox()->width())
       c->getBox()->setWidth(compartmentBox.width());
   if (compartmentBox.height() > c->getBox()->height())
       c->getBox()->setHeight(compartmentBox.height());
   
   unsigned int boxIndex = 0;
   LPoint shiftDistance(0.0, 0.0);
   for (constReactionIt rIt = c->reactionsBegin(); rIt != c->reactionsEnd(); ++rIt) {
       if ((*rIt)->getNumConnectedReactions() == 0 && (*rIt)->isMatchWithGlyph()) {
           shiftDistance = mainReactionBoxes.at(boxIndex)->upperLeftCorner() - (*rIt)->getExtentBox().upperLeftCorner();
           if (shiftDistance.mag() > 0.0000001)
               (*rIt)->shiftItems(shiftDistance.x(), shiftDistance.y());
           ++boxIndex;
       }
   }
}

void packCompartmentsIntoNetwork(Network* net) {
   std::vector<LBox*> compartmentBoxes;
   LBox networkBox, * compartmentBox;
   
   for (constCompartmentIt cIt = net->compartmentsBegin(); cIt != net->compartmentsEnd(); ++cIt) {
       if ((*cIt)->isMatchWithGlyph()) {
           compartmentBox = new LBox(*(*cIt)->getBox());
           compartmentBoxes.push_back(compartmentBox);
       }
   }
   
   networkBox = packBoxesIntoOptimalBox(LPoint(0.0, 0.0), compartmentBoxes);
   
   if (networkBox.width() > net->getWidth())
       net->setWidth(networkBox.width());
   if (networkBox.height() > net->getHeight())
       net->setHeight(networkBox.height());
   
   unsigned int boxIndex = 0;
   LPoint shiftDistance(0.0, 0.0);
   for (constCompartmentIt cIt = net->compartmentsBegin(); cIt != net->compartmentsEnd(); ++cIt) {
       if ((*cIt)->isMatchWithGlyph()) {
           shiftDistance = compartmentBoxes.at(boxIndex)->upperLeftCorner() - (*cIt)->getBox()->upperLeftCorner();
           if (shiftDistance.mag() > 0.0000001)
               (*cIt)->shiftItems(shiftDistance.x(), shiftDistance.y());
           ++boxIndex;
       }
   }
}

void locateReactionSpeciesReferences(NReaction* r) {
   NSpeciesReference* sr = NULL;
   for (constSReferenceIt sRIt = r->sReferencesBegin(); sRIt != r->sReferencesEnd(); ++sRIt) {
       sr = *sRIt;
       
       sr->setMatchWithGlyph(true);
       sr->setUsed(true);
       
       // set id
       sr->setGlyphId(r->getSpeciesReferenceUniqueGlyphId(sr->getId()));
       
       // set curve
       locateSReference(r, sr);
   }
}

const SpcSide getSReferenceSpeciesSide(const LPoint& speciesBoxPoint, const LPoint& mReactionInnerLayerPoint) {
   
   // calculate species angle
   double dy = mReactionInnerLayerPoint.y() - speciesBoxPoint.y();
   double dx = speciesBoxPoint.x() - mReactionInnerLayerPoint.x();
   double sAngle = atan2(dy, dx);

   while (std::floor(sAngle) < 0)
       sAngle += 2 * pi;
   while (sAngle > 2 * pi)
       sAngle -= 2 * pi;
   
   // find the species side using the quadrant in which species is located relative to the mreacion inner layer point
   switch (angleToQuadrant(sAngle)) {
       case Quad_I_1:
       case Quad_IV_2:
           return LEFT_SIDE;
           
       case Quad_I_2:
       case Quad_II_1:
           return BOTTOM_SIDE;
           
       case Quad_II_2:
       case Quad_III_1:
           return RIGHT_SIDE;
           
       case Quad_III_2:
       case Quad_IV_1:
           return TOP_SIDE;
           
       default:
           return TOP_SIDE;
   }
}

const LPoint getSReferenceSpeciesBoxPoint(NSpecies* s, NSpeciesReference* sr, const SpcSide side) {
   LBox* speciesBox = s->getBox();
   LPoint boxPoint;
   int alreadyOccupiedIndex = -1;
   int firstVacantIndex = -1;
   int speciesReferenceIndex;
   
   for (int i = 0; i < (s->getSideSpeciesReferences(side)).size(); ++i) {
       if (s->getSideSpeciesReferences(side).at(i)) {
           if (stringCompare(sr->getId(), s->getSideSpeciesReferences(side).at(i)->getId()) && stringCompare(sr->getReaction()->getId(), s->getSideSpeciesReferences(side).at(i)->getReaction()->getId())) {
               alreadyOccupiedIndex = i;
               break;
           }
       }
       else {
           firstVacantIndex = i;
           break;
       }
   }
   
   if (alreadyOccupiedIndex != -1)
       speciesReferenceIndex = alreadyOccupiedIndex;
   else if (firstVacantIndex != -1)
       speciesReferenceIndex = firstVacantIndex;
   else
       speciesReferenceIndex = (s->getSideSpeciesReferences(side)).size();
   
   unsigned int sideRow = 0;
   switch(side) {
       case LEFT_SIDE:
           /// species box point
           sideRow = speciesReferenceIndex / 3;
           switch (speciesReferenceIndex % 3) {
               case 0:
                   boxPoint = LPoint(speciesBox->x() - (sideRow * (1 + 1.0 / 3.0) + 1.0 / 3.0) * lLendingSize, speciesBox->y() + 0.5 * speciesBox->height());
                   break;
               case 1:
                   boxPoint = LPoint(speciesBox->x() - (sideRow * (1 + 1.0 / 3.0) + 1.0 / 3.0) * lLendingSize, speciesBox->y() + 0.5 * speciesBox->height() - 1.2 * lLendingSize);
                   break;
               case 2:
                   boxPoint = LPoint(speciesBox->x() - (sideRow * (1 + 1.0 / 3.0) + 1.0 / 3.0) * lLendingSize, speciesBox->y() + 0.5 * speciesBox->height() + 1.2 * lLendingSize);
                   break;
           }
           break;
           
       case BOTTOM_SIDE:
           /// species box point
           sideRow = speciesReferenceIndex / 5;
           switch (speciesReferenceIndex % 5) {
               case 0:
                   boxPoint = LPoint(speciesBox->x() + 0.5 * speciesBox->width(), speciesBox->y() + speciesBox->height() + (sideRow * (1 + 1.0 / 3.0) + 1.0 / 3.0) * lLendingSize);
                   break;
               case 1:
                   boxPoint = LPoint(speciesBox->x() + 0.5 * speciesBox->width() + 1.2 * lLendingSize, speciesBox->y() + speciesBox->height() + (sideRow * (1 + 1.0 / 3.0) + 1.0 / 3.0) * lLendingSize);
                   break;
               case 2:
                   boxPoint = LPoint(speciesBox->x() + 0.5 * speciesBox->width() - 1.2 * lLendingSize, speciesBox->y() + speciesBox->height() + (sideRow * (1 + 1.0 / 3.0) + 1.0 / 3.0) * lLendingSize);
                   break;
               case 3:
                   boxPoint = LPoint(speciesBox->x() + 0.5 * speciesBox->width() + 2.4 * lLendingSize, speciesBox->y() + speciesBox->height() + (sideRow * (1 + 1.0 / 3.0) + 1.0 / 3.0) * lLendingSize);
                   break;
               case 4:
                   boxPoint = LPoint(speciesBox->x() + 0.5 * speciesBox->width() - 2.4 * lLendingSize, speciesBox->y() + speciesBox->height() + (sideRow * (1 + 1.0 / 3.0) + 1.0 / 3.0) * lLendingSize);
                   break;
           }
           break;
           
       case RIGHT_SIDE:
           /// species box point
           sideRow = speciesReferenceIndex / 3;
           switch (speciesReferenceIndex % 3) {
               case 0:
                   boxPoint = LPoint(speciesBox->x() + speciesBox->width() + (sideRow * (1 + 1.0 / 3.0) + 1.0 / 3.0) * lLendingSize, speciesBox->y() + 0.5 * speciesBox->height());
                   break;
               case 1:
                   boxPoint = LPoint(speciesBox->x() + speciesBox->width() + (sideRow * (1 + 1.0 / 3.0) + 1.0 / 3.0) * lLendingSize, speciesBox->y() + 0.5 * speciesBox->height() + 1.2 * lLendingSize);
                   break;
               case 2:
                   boxPoint = LPoint(speciesBox->x() + speciesBox->width() + (sideRow * (1 + 1.0 / 3.0) + 1.0 / 3.0) * lLendingSize, speciesBox->y() + 0.5 * speciesBox->height() - 1.2 * lLendingSize);
                   break;
           }
           break;
           
       case TOP_SIDE:
           /// species box point
           sideRow = speciesReferenceIndex / 5;
           switch (speciesReferenceIndex % 5) {
               case 0:
                   boxPoint =  LPoint(speciesBox->x() + 0.5 * speciesBox->width(), speciesBox->y() - (sideRow * (1 + 1.0 / 3.0) + 1.0 / 3.0) * lLendingSize);
                   break;
               case 1:
                   boxPoint = LPoint(speciesBox->x() + 0.5 * speciesBox->width() - 1.2 * lLendingSize, speciesBox->y() - (sideRow * (1 + 1.0 / 3.0) + 1.0 / 3.0) * lLendingSize);
                   break;
               case 2:
                   boxPoint = LPoint(speciesBox->x() + 0.5 * speciesBox->width() + 1.2 * lLendingSize, speciesBox->y() - (sideRow * (1 + 1.0 / 3.0) + 1.0 / 3.0) * lLendingSize);
                   break;
               case 3:
                   boxPoint = LPoint(speciesBox->x() + 0.5 * speciesBox->width() - 2.4 * lLendingSize, speciesBox->y() - (sideRow * (1 + 1.0 / 3.0) + 1.0 / 3.0) * lLendingSize);
                   break;
               case 4:
                   boxPoint = LPoint(speciesBox->x() + 0.5 * speciesBox->width() + 2.4 * lLendingSize, speciesBox->y() - (sideRow * (1 + 1.0 / 3.0) + 1.0 / 3.0) * lLendingSize);
                   break;
           }
           break;
   }
   
   return boxPoint;
}

const LPoint getSReferenceReactionEndPoint(NReaction* r, const double& sRAngle, RxnEnd& rxnEnd) {
   LPoint centerP = r->getCenter();
   LPoint reactionEndPoint;
   
   switch (angleToQuadrant(sRAngle - r->getDirectionAngle())) {
       case Quad_I_1:
       case Quad_I_2:
       case Quad_IV_1:
       case Quad_IV_2:
           rxnEnd = END_END;
           reactionEndPoint = LPoint(centerP.x() + 3 * lLendingSize * std::cos(r->getDirectionAngle()), centerP.y() - 3 * lLendingSize * std::sin(r->getDirectionAngle()));
           break;
           
       case Quad_II_1:
       case Quad_II_2:
       case Quad_III_1:
       case Quad_III_2:
           rxnEnd = START_END;
           reactionEndPoint = LPoint(centerP.x() - 3 * lLendingSize * std::cos(r->getDirectionAngle()), centerP.y() + 3 * lLendingSize * std::sin(r->getDirectionAngle()));
           break;

       default:
           break;
   }
   
   return reactionEndPoint;
}

const LPoint getSReferenceReactionSidePoint(NReaction* r, const double& sRAngle, RxnSide& rxnSide) {
   LPoint centerP = r->getCenter();
   LPoint reactionSidePoint;
   
   switch (angleToQuadrant(sRAngle - r->getDirectionAngle())) {
       case Quad_I_1:
       case Quad_I_2:
           rxnSide = UP_END_SIDE;
           switch (r->getSideSpeciesReferences(rxnSide).size() % 3) {
               case 0:
                   reactionSidePoint = LPoint(centerP.x() + 0.5 * lLendingSize * std::cos(r->getDirectionAngle()) - 0.2 * lLendingSize * std::sin(r->getDirectionAngle()), centerP.y() - 0.2 * lLendingSize * std::cos(r->getDirectionAngle()) - 0.5 * lLendingSize * std::sin(r->getDirectionAngle()));
                   break;
               case 1:
                   reactionSidePoint = LPoint(centerP.x() + 1.5 * lLendingSize * std::cos(r->getDirectionAngle()) - 0.2 * lLendingSize * std::sin(r->getDirectionAngle()), centerP.y() - 0.2 * lLendingSize * std::cos(r->getDirectionAngle()) - 1.5 * lLendingSize * std::sin(r->getDirectionAngle()));
                   break;
               case 2:
                   reactionSidePoint = LPoint(centerP.x() + 2.5 * lLendingSize * std::cos(r->getDirectionAngle()) - 0.2 * lLendingSize * std::sin(r->getDirectionAngle()), centerP.y() - 0.2 * lLendingSize * std::cos(r->getDirectionAngle()) - 2.5 * lLendingSize * std::sin(r->getDirectionAngle()));
                   break;
           }
           break;
           
       case Quad_II_1:
       case Quad_II_2:
           rxnSide = UP_START_SIDE;
           switch (r->getSideSpeciesReferences(rxnSide).size() % 3) {
               case 0:
                   reactionSidePoint = LPoint(centerP.x() - 0.5 * lLendingSize * std::cos(r->getDirectionAngle()) - 0.2 * lLendingSize * std::sin(r->getDirectionAngle()), centerP.y() - 0.2 * lLendingSize * std::cos(r->getDirectionAngle()) + 0.5 * lLendingSize * std::sin(r->getDirectionAngle()));
                   break;
               case 1:
                   reactionSidePoint = LPoint(centerP.x() - 1.5 * lLendingSize * std::cos(r->getDirectionAngle()) - 0.2 * lLendingSize * std::sin(r->getDirectionAngle()), centerP.y() - 0.2 * lLendingSize * std::cos(r->getDirectionAngle()) + 1.5 * lLendingSize * std::sin(r->getDirectionAngle()));
                   break;
               case 2:
                   reactionSidePoint = LPoint(centerP.x() - 2.5 * lLendingSize * std::cos(r->getDirectionAngle()) - 0.2 * lLendingSize * std::sin(r->getDirectionAngle()), centerP.y() - 0.2 * lLendingSize * std::cos(r->getDirectionAngle()) + 2.5 * lLendingSize * std::sin(r->getDirectionAngle()));
                   break;
           }
           break;
           
       case Quad_III_1:
       case Quad_III_2:
           rxnSide = DOWN_START_SIDE;
           switch (r->getSideSpeciesReferences(rxnSide).size() % 3) {
               case 0:
                   reactionSidePoint = LPoint(centerP.x() - 0.5 * lLendingSize * std::cos(r->getDirectionAngle()) + 0.2 * lLendingSize * std::sin(r->getDirectionAngle()), centerP.y() + 0.2 * lLendingSize * std::cos(r->getDirectionAngle()) + 0.5 * lLendingSize * std::sin(r->getDirectionAngle()));
                   break;
               case 1:
                   reactionSidePoint = LPoint(centerP.x() - 1.5 * lLendingSize * std::cos(r->getDirectionAngle()) + 0.2 * lLendingSize * std::sin(r->getDirectionAngle()), centerP.y() + 0.2 * lLendingSize * std::cos(r->getDirectionAngle()) + 1.5 * lLendingSize * std::sin(r->getDirectionAngle()));
                   break;
               case 2:
                   reactionSidePoint = LPoint(centerP.x() - 2.5 * lLendingSize * std::cos(r->getDirectionAngle()) + 0.2 * lLendingSize * std::sin(r->getDirectionAngle()), centerP.y() + 0.2 * lLendingSize * std::cos(r->getDirectionAngle()) + 2.5 * lLendingSize * std::sin(r->getDirectionAngle()));
                   break;
           }
           break;
           
       case Quad_IV_1:
       case Quad_IV_2:
           rxnSide = DOWN_END_SIDE;
           switch (r->getSideSpeciesReferences(rxnSide).size() % 3) {
               case 0:
                   reactionSidePoint = LPoint(centerP.x() + 0.5 * lLendingSize * std::cos(r->getDirectionAngle()) + 0.2 * lLendingSize * std::sin(r->getDirectionAngle()), centerP.y() + 0.2 * lLendingSize * std::cos(r->getDirectionAngle()) - 0.5 * lLendingSize * std::sin(r->getDirectionAngle()));
                   break;
               case 1:
                   reactionSidePoint = LPoint(centerP.x() + 1.5 * lLendingSize * std::cos(r->getDirectionAngle()) + 0.2 * lLendingSize * std::sin(r->getDirectionAngle()), centerP.y() + 0.2 * lLendingSize * std::cos(r->getDirectionAngle()) - 1.5 * lLendingSize * std::sin(r->getDirectionAngle()));
                   break;
               case 2:
                   reactionSidePoint = LPoint(centerP.x() + 2.5 * lLendingSize * std::cos(r->getDirectionAngle()) + 0.2 * lLendingSize * std::sin(r->getDirectionAngle()), centerP.y() + 0.2 * lLendingSize * std::cos(r->getDirectionAngle()) - 2.5 * lLendingSize * std::sin(r->getDirectionAngle()));
                   break;
           }
           break;
   }
   
   return reactionSidePoint;
}

LLineSegment* extendCurve(LCurve* curve, LLineSegment* l, LPoint& startPoint, const double& dx, const double& dy) {
   
   // shift the position of start point
   startPoint.shift(dx, dy);
   
   // set the end point of previous line segment
   l->setEnd(startPoint);
   
   // add line segment to the curve
   curve->addToListOfElementsEnd(l);
   
   // create a new line segment
   l = new LLineSegment();
   
   // set id
   l->setId(curve->getElementUniqueId());
   
   // set the start point of line segment
   l->setStart(startPoint);
   
   return l;
}

LLineSegment* extendCurve(LCurve* curve, LLineSegment* l, LPoint& startPoint, const LPoint& newPoint) {
   return extendCurve(curve, l, startPoint, newPoint.x() - startPoint.x(), newPoint.y() - startPoint.y());
}

LLineSegment* extendCurve(LCurve* curve, LLineSegment* l, LPoint& startPoint, const LPoint& newPoint, const LPoint& basePoint1, const LPoint& basePoint2, const bool& isArcLike) {
   
   delete l;
   
   // create a cubic bezier (turn previous line segment to cubic bezier)
   l = new LCubicBezier();
   
   // set id
   l->setId(curve->getElementUniqueId());
   
   // set the start point of cubic bezier
   l->setStart(startPoint);
   
   // set the base point 1 of cubic bezier
   ((LCubicBezier*)l)->setBasePoint1(basePoint1);
   
   // set the base point 2 of cubic bezier
   ((LCubicBezier*)l)->setBasePoint2(basePoint2);
   
   // set the start point of cubic bezier
   l->setEnd(newPoint);
   
   // if it is an arc-like cubic bezier
   if (isArcLike)
       ((LCubicBezier*)l)->setArcLike(true);
   
   // add the cubic bezier to the curve
   curve->addToListOfElementsEnd(l);
   
   // shift the position of start point
   startPoint = newPoint;
   
   // create a new line segment
   l = new LLineSegment();
   
   // set id
   l->setId(curve->getElementUniqueId());
   
   // set the start point of line segment
   l->setStart(startPoint);
   
   return l;
}

void setSpeciesReferenceCurve(NReaction* mR, NReaction* r, NSpeciesReference* sr, NSpecies* pseudoSpecies, SpcSide speciesSide, const RxnEnd rxnEnd, const RxnSide rxnSide) {
   
   // species reference start and end point
   LPoint startPoint = sr->getStartPoint();
   LPoint endPoint = sr->getEndPoint();
   
   // create a new curve
   LCurve* curve = new LCurve();
   
   // create a new line segment
   LLineSegment* l = new LLineSegment();
   
   // set id
   l->setId(curve->getElementUniqueId());
   
   // set the start point of the first line segment
   l->setStart(startPoint);
   
   if (pseudoSpecies) {
       LPoint pseudoPoint, basePoint1, basePoint2, mReactionOuterLayerPoint, mReactionInnerLayerPoint;
       LPoint offsetOnReaction(0.0, 0.0), offsetOnLayer(0.0, 0.0);
       std::vector <NSpeciesReference*> srv;
       unsigned int sReferenceIndex = 0;
       int offsetOnReactionSign = 1;
       int numAngularTransferringSteps = 0;
       
       // for products and side products
       if (sr->getRole() == 1 || sr->getRole() == 3) {
           // calculate the offset on reaction
           srv = r->getEndSpeciesReferences(rxnEnd);
           for (int i = 0; i < srv.size(); ++i) {
               if (stringCompare(srv.at(i)->getId(), sr->getId())) {
                   sReferenceIndex = i;
                   break;
               }
           }
           
           switch (sReferenceIndex) {
               case 0:
                   offsetOnReaction = LPoint(0.0, 0.0);
                   break;
               case 1:
                   offsetOnReaction = LPoint(-0.3 * lLendingSize * std::sin(r->getDirectionAngle()), -0.3 * lLendingSize * std::cos(r->getDirectionAngle()));
                   offsetOnReactionSign *= -1;
                   break;
               case 2:
                   offsetOnReaction = LPoint(0.3 * lLendingSize * std::sin(r->getDirectionAngle()), 0.3 * lLendingSize * std::cos(r->getDirectionAngle()));
                   break;
           }
           
           // calculate the magnitude of pseudospecies point, base point1 of the transferring curve, base point 2 of the transferring curve, main reacion outer layer point, main reacion inner layer point
           calculateCurvePointsFeatures(mR, r, pseudoSpecies, sr, pseudoPoint, basePoint1, basePoint2, mReactionOuterLayerPoint, mReactionInnerLayerPoint, offsetOnLayer, offsetOnReaction, offsetOnReactionSign, numAngularTransferringSteps);
           
           // get species side to which the species reference enters
           speciesSide = getSReferenceSpeciesSide(sr->getSpecies()->getBox()->center(), mReactionInnerLayerPoint);
           
           // get the box point where the species reference enters the species side and update it on the species reference
           endPoint = getSReferenceSpeciesBoxPoint(sr->getSpecies(), sr, speciesSide);
           sr->setEndPoint(endPoint);
           
           // set the curve segments
           l = extendCurve(curve, l, startPoint, startPoint + offsetOnReaction);
           
           l = extendCurve(curve, l, startPoint, pseudoPoint + offsetOnReaction);

           if (numAngularTransferringSteps) {
               l = extendCurve(curve, l, startPoint, pseudoPoint + offsetOnReaction + offsetOnLayer);
               
               l = extendCurve(curve, l, startPoint, mReactionOuterLayerPoint, basePoint1, basePoint2, true);
           }
           
           l = extendCurve(curve, l, startPoint, mReactionInnerLayerPoint);
       }
       
       // for substrates and side substrates
       else if (sr->getRole() == 0 || sr->getRole() == 2) {
           // calculate the offset on reaction
           srv = r->getEndSpeciesReferences(rxnEnd);
           for (int i = 0; i < srv.size(); ++i) {
               if (stringCompare(srv.at(i)->getId(), sr->getId())) {
                   sReferenceIndex = i;
                   break;
               }
           }
           
           switch (sReferenceIndex) {
               case 0:
                   offsetOnReaction = LPoint(0.0, 0.0);
                   break;
               case 1:
                   offsetOnReaction = LPoint(-0.3 * lLendingSize * std::sin(r->getDirectionAngle()), -0.3 * lLendingSize * std::cos(r->getDirectionAngle()));
                   offsetOnReactionSign *= -1;
                   break;
               case 2:
                   offsetOnReaction = LPoint(0.3 * lLendingSize * std::sin(r->getDirectionAngle()), 0.3 * lLendingSize * std::cos(r->getDirectionAngle()));
                   break;
           }
           
           // calculate the magnitude of pseudospecies point, base point1 of the transferring curve, base point 2 of the transferring curve, main reacion outer layer point, main reacion inner layer point
           calculateCurvePointsFeatures(mR, r, pseudoSpecies, sr, pseudoPoint, basePoint1, basePoint2, mReactionOuterLayerPoint, mReactionInnerLayerPoint, offsetOnLayer, offsetOnReaction, offsetOnReactionSign, numAngularTransferringSteps);
           
           // get species side to which the species reference enters
           speciesSide = getSReferenceSpeciesSide(sr->getSpecies()->getBox()->center(), mReactionInnerLayerPoint);
           
           // get the box point where the species reference enters the species side and update it on the species reference and the first line segment
           startPoint = getSReferenceSpeciesBoxPoint(sr->getSpecies(), sr, speciesSide);
           sr->setStartPoint(startPoint);
           l->setStart(startPoint);
           
           // set the curve segments
           l = extendCurve(curve, l, startPoint, mReactionInnerLayerPoint);
           
           if (numAngularTransferringSteps) {
                l = extendCurve(curve, l, startPoint, mReactionOuterLayerPoint);
               
               l = extendCurve(curve, l, startPoint, pseudoPoint + offsetOnReaction + offsetOnLayer, basePoint2, basePoint1, true);
           }
           
           l = extendCurve(curve, l, startPoint, pseudoPoint + offsetOnReaction);
           
           l = extendCurve(curve, l, startPoint, endPoint + offsetOnReaction);
       }
       // for modifiers, activators, and inhibitors
       else {
           // calculate the offset on reaction
           srv = r->getSideSpeciesReferences(rxnSide);
           for (int i = 0; i < srv.size(); ++i) {
               if (stringCompare(srv.at(i)->getId(), sr->getId())) {
                   sReferenceIndex = i;
                   break;
               }
           }
           
           switch (sReferenceIndex) {
               case 0:
                   offsetOnReaction = LPoint(1.2 * lLendingSize * std::sin(r->getDirectionAngle()), 1.2 * lLendingSize * std::cos(r->getDirectionAngle()));
                   break;
               case 1:
                   offsetOnReaction = LPoint(1.5 * lLendingSize * std::sin(r->getDirectionAngle()), 1.5 * lLendingSize * std::cos(r->getDirectionAngle()));
                   break;
               case 2:
                   offsetOnReaction = LPoint(1.8 * lLendingSize * std::sin(r->getDirectionAngle()), 1.8 * lLendingSize * std::cos(r->getDirectionAngle()));
                   break;
           }
           
           // calculate the magnitude of pseudospecies point, base point1 of the transferring curve, base point 2 of the transferring curve, main reacion outer layer point, main reacion inner layer point
           calculateCurvePointsFeatures(mR, r, pseudoSpecies, sr, pseudoPoint, basePoint1, basePoint2, mReactionOuterLayerPoint, mReactionInnerLayerPoint, offsetOnLayer, offsetOnReaction, offsetOnReactionSign, numAngularTransferringSteps);
           
           // get species side to which the species reference enters
           speciesSide = getSReferenceSpeciesSide(sr->getSpecies()->getBox()->center(), mReactionInnerLayerPoint);
           
           // get the box point where the species reference enters the species side and update it on the species reference and the first line segment
           startPoint = getSReferenceSpeciesBoxPoint(sr->getSpecies(), sr, speciesSide);
           sr->setStartPoint(startPoint);
           l->setStart(startPoint);
           
           // set the curve segments
           l = extendCurve(curve, l, startPoint, mReactionInnerLayerPoint);
           
           if (numAngularTransferringSteps) {
                l = extendCurve(curve, l, startPoint, mReactionOuterLayerPoint);
               
               l = extendCurve(curve, l, startPoint, pseudoPoint + offsetOnReaction + offsetOnLayer, basePoint2, basePoint1, true);
           }
           
           l = extendCurve(curve, l, startPoint, pseudoPoint + offsetOnReaction);
           
           l = extendCurve(curve, l, startPoint, endPoint + offsetOnReaction);
       }
   }
   
   // set the end point of the last line segment
   l->setEnd(endPoint);
              
   // add line segment to the curve
   curve->addToListOfElementsEnd(l);
   
   // set curve to the species reference
   sr->setCurve(curve);
   
   // add to the reference side of species
   sr->getSpecies()->addToReferencedSide(sr, speciesSide);
}

void calculateCurvePointsFeatures(NReaction* mR, NReaction* r, NSpecies* pseudoSpecies, NSpeciesReference* sr, LPoint& pseudoPoint, LPoint& basePoint1, LPoint& basePoint2, LPoint& mReactionOuterLayerPoint, LPoint& mReactionInnerLayerPoint, LPoint& offsetOnLayer, const LPoint& offsetOnReaction, int offsetOnReactionSign, int& numAngularTransferringSteps) {
   
   // calculate the angle difference between two vacancies
   double dAngle = 2 * pi / mR->getNumSpecies();
   
   // calculate pseudoSpecies Box point
   pseudoPoint = getSpeciesBoxOriginalPosition(pseudoSpecies).upperLeftCorner();
   
   // calculate pseudoSpecies angle relative to the main reaction
   double pSAngle = pseudoSpecies->findReactionMemberById(mR->getId())->angle;
   
   // get species Box point
   LPoint speciesPoint = sr->getSpecies()->getBox()->center();
   
   // get the angle of the pseudoSpecies to which the reaction of the species is connected
   double sAngle = pSAngle;
   NReaction* jointR  = NULL;
   NReaction::constConnectedRIt jCRIt;
   for (NSpecies::constRMemberIt rMIt = sr->getSpecies()->rMembersBegin(); rMIt != sr->getSpecies()->rMembersEnd(); ++rMIt) {
       if ((*rMIt)->isMainReaction) {
           if (!stringCompare((*rMIt)->reaction->getId(), mR->getId())) {
               jointR = (*rMIt)->reaction;
               
               while (!stringCompare(jointR->getId(), mR->getId())) {
                   jCRIt = jointR->connectedRBegin();
                   jointR = (*jCRIt)->reaction;
                   if ((*jCRIt)->pseudoSpecies)
                       break;
                   else
                        jointR = (*jCRIt)->reaction;
               }
           }
           
           break;
       }
   }
   
   if (!jointR)
       sAngle = sr->getSpecies()->findReactionMemberById(mR->getId())->angle;
   else if ((*jCRIt)->pseudoSpecies)
       sAngle = (*jCRIt)->pseudoSpecies->findReactionMemberById(mR->getId())->angle;
   else if ((*jCRIt)->species.at(0) && ((*jCRIt)->species.at(0)->findReactionMemberById(mR->getId())))
           sAngle = (*jCRIt)->species.at(0)->findReactionMemberById(mR->getId())->angle;
   
   // calculate number of steps the curve has to trasnfer from the pseudo species box point to reach close to the species
   double positionAngle = pSAngle;
   if (pSAngle - sAngle - 0.5 * dAngle > 0.000001) {
       if ((sAngle + 2 * pi - pSAngle) - (pSAngle - sAngle) > 0.000001) {
           while (positionAngle - sAngle - 0.5 * dAngle > 0.000001) {
               positionAngle -= 0.5 * dAngle;
               --numAngularTransferringSteps;
           }
       }
       else {
           while (sAngle + 2 * pi - positionAngle - 0.5 * dAngle > 0.000001) {
               positionAngle += 0.5 * dAngle;
               ++numAngularTransferringSteps;
           }
       }
           
   }
   else if (sAngle - pSAngle - 0.5 * dAngle > 0.000001) {
       if ((pSAngle + 2 * pi - sAngle) - (sAngle - pSAngle) > 0.000001) {
           while (sAngle - positionAngle - 0.5 * dAngle > 0.000001) {
               positionAngle += 0.5 * dAngle;
               ++numAngularTransferringSteps;
           }
       }
       else {
           while (positionAngle + 2 * pi - sAngle - 0.5 * dAngle > 0.000001) {
               positionAngle -= 0.5 * dAngle;
               --numAngularTransferringSteps;
           }
       }
   }
   
   // calculate the radial distance between the reaction layer and species layer of the species reference
   unsigned int numRadialTransferringSteps = mR->findSubReactionLayer(r);
   if (stringCompare(mR->getId(), r->getId()) && jointR)
       numRadialTransferringSteps = mR->findSubReactionLayer(r) - mR->findSubReactionLayer(jointR);
   
   std::vector<std::pair<int , int>> manhatanPath = mR->getManhatanPathOfSpeciesReference(sr, pseudoSpecies, numAngularTransferringSteps, numRadialTransferringSteps);
   
   std::vector<std::vector<LPoint*>> points = convertManhatanPathToPoints(mR, pseudoSpecies, manhatanPath);
   
   // calculate the path offset of the species reference curve on the main reaction outer layer
   unsigned int vacantIndex = 0;
   if (numAngularTransferringSteps)
       vacantIndex = mR->addSpeciesReferenceToCircumferenceVacancy(sr, pseudoSpecies, numAngularTransferringSteps);
   offsetOnLayer = LPoint(-lLendingSize * (1 + 0.5 * vacantIndex) * std::cos(pSAngle), lLendingSize * (1 + 0.5 * vacantIndex) * std::sin(pSAngle));
   
   // calculate the magnitude of mreactionouterlayerpoint, basepoint1, and basepoint2
   double mReactionLayerAngle = pSAngle;
   mReactionOuterLayerPoint = pseudoPoint + offsetOnReaction;
   if (numAngularTransferringSteps) {
       // calculate updated value of pseudopoint, pseudoRadius, and pseudoangle with the offset
       LPoint _pseudoPoint = pseudoPoint + offsetOnReaction + offsetOnLayer;
       double _pSR = LPoint(_pseudoPoint.x() - mR->getCenter().x(), mR->getCenter().y() - _pseudoPoint.y()).mag();
       double _pSAngle = atan2(mR->getCenter().y() - _pseudoPoint.y(), _pseudoPoint.x() - mR->getCenter().x());
       while (std::floor(_pSAngle) < 0)
           _pSAngle += 2 * pi;
       while (_pSAngle > 2 * pi)
           _pSAngle -= 2 * pi;
       
       double theta = numAngularTransferringSteps * 0.5 * dAngle;
       if (std::floor(theta) < 0)
           theta += std::min(0.1 * dAngle, 8 * lLendingSize/ _pSR);
       else
           theta -= std::min(0.1 * dAngle, 8 * lLendingSize/ _pSR);
       
       double phi = _pSAngle;
       mReactionLayerAngle = phi + theta;
       mReactionOuterLayerPoint = mR->getCenter() + LPoint(_pSR * std::cos(mReactionLayerAngle), - _pSR * std::sin(mReactionLayerAngle));
       
       double c = 0.0;
       switch (angleToQuadrant(phi + 0.5 * theta)) {
           case Quad_I_1:
           case Quad_II_2:
           case Quad_III_1:
           case Quad_IV_2:
               c = (mR->getCenter().x() + _pSR * std::cos(phi + 0.5 * theta) - 0.5 * _pseudoPoint.x() - 0.5 * mReactionOuterLayerPoint.x()) / (0.75 * std::cos(phi + 0.5 * theta) * std::sin(0.5 * std::abs(theta)));
               break;
               
           case Quad_I_2:
           case Quad_II_1:
           case Quad_III_2:
           case Quad_IV_1:
               c = (mR->getCenter().y() - _pSR * std::sin(phi + 0.5 * theta) - 0.5 * _pseudoPoint.y() - 0.5 * mReactionOuterLayerPoint.y()) / (-0.75 * std::sin(phi + 0.5 * theta) * std::sin(0.5 * std::abs(theta)));
       }
       if (std::floor(theta) < 0) {
           phi += theta;
           theta = std::abs(theta);
           basePoint1 = LPoint(_pseudoPoint.x() + c * std::sin(theta + phi), _pseudoPoint.y() + c * std::cos(theta + phi));
           basePoint2 = LPoint(mReactionOuterLayerPoint.x() - c * std::sin(phi), mReactionOuterLayerPoint.y() - c * std::cos(phi));
       }
       else {
           basePoint1 = LPoint(_pseudoPoint.x() - c * std::sin(phi), _pseudoPoint.y() - c * std::cos(phi));
           basePoint2 = LPoint(mReactionOuterLayerPoint.x() + c * std::sin(theta + phi), mReactionOuterLayerPoint.y() + c * std::cos(theta + phi));
       }
   }
   
   mReactionLayerAngle = atan2(mR->getCenter().y() - mReactionOuterLayerPoint.y(), mReactionOuterLayerPoint.x() - mR->getCenter().x());
   mReactionInnerLayerPoint = mReactionOuterLayerPoint - LPoint((numRadialTransferringSteps * (2 * cReactionRadius + 2.5 * maxSpeciesBoxWidth) + 0.5 * cReactionRadius) * std::cos(mReactionLayerAngle), -(numRadialTransferringSteps * (2 * cReactionRadius + 2.5 * maxSpeciesBoxWidth) + 0.5 * cReactionRadius) * std::sin(mReactionLayerAngle));
       
   // set the quadrant of species reference
   double sAngleToInnerLayerPoint = atan2(mReactionInnerLayerPoint.y() - speciesPoint.y(), speciesPoint.x() - mReactionInnerLayerPoint.x());
   
   while (std::floor(sAngleToInnerLayerPoint) < 0)
       sAngleToInnerLayerPoint += 2 * pi;
   while (sAngleToInnerLayerPoint > 2 * pi)
       sAngleToInnerLayerPoint -= 2 * pi;
   sr->setQuadrant(angleToQuadrant(sAngleToInnerLayerPoint));
}

NSpecies* getPseudoSpecies(NReaction* mR, NReaction* cR, std::vector<NSpecies*> mutualSpecies) {
   double angleMR = 0.0, dxMR = 0.0, dyMR = 0.0;
   double angleMRMax = INT_MIN, angleMRMin = INT_MAX;
   LBox oSpeciesBoxMR;
   
   // find the relative angle toward main and connected reaction
   double speciesAngleMR;
   for (int i = 0; i < mutualSpecies.size(); ++i) {
       oSpeciesBoxMR = getSpeciesBoxOriginalPosition(mR, mutualSpecies.at(i));
       dyMR = mR->getCenter().y() - oSpeciesBoxMR.y();
       dxMR = oSpeciesBoxMR.x() - mR->getCenter().x();
       speciesAngleMR = atan2(dyMR, dxMR);
       if (std::floor(speciesAngleMR) < 0)
           speciesAngleMR +=  2 * pi;
       else if ((int (speciesAngleMR) - 2 * pi) >= 0)
           speciesAngleMR -=  2 * pi;
       angleMR += speciesAngleMR;
       angleMRMin = std::min(angleMRMin, speciesAngleMR);
       angleMRMax = std::max(angleMRMax, speciesAngleMR);
   }
   angleMR /= mutualSpecies.size();
   if ((angleMRMax - angleMRMin) - pi > 0.00001)
       angleMR -= pi;
   while (std::floor(angleMR) < 0)
       angleMR += 2 * pi;
   while (angleMR > 2 * pi)
       angleMR -= 2 * pi;
   
   if (mutualSpecies.size() == 1) {
       if (mR->findSpeciesLayer(mutualSpecies.at(0)) == -1) {
           if (!stringCompare(mR->getId(), cR->getId()))
               mR->addSpeciesToVacancy(mutualSpecies.at(0), angleMR);
       }
       
       if (cR->getNumOccupiedReactionsOfSpecies(mutualSpecies.at(0)) < 3)
           return mutualSpecies.at(0);
   }
   
   std::string pseudoSpeciesId = cR->generatePseudoSpeciesId(mutualSpecies);
   NSpecies* pseudoSpecies = cR->findPseudoSpeciesById(pseudoSpeciesId);
   unsigned int version = 0;
   while (pseudoSpecies && cR->getNumOccupiedReactionsOfSpecies(pseudoSpecies) == 3) {
       ++version;
       pseudoSpeciesId = cR->generatePseudoSpeciesId(mutualSpecies, version);
       pseudoSpecies = cR->findPseudoSpeciesById(pseudoSpeciesId);
   }
   
   if (!pseudoSpecies) {
       pseudoSpecies = new NSpecies();
       
       // set as pseudo species
       pseudoSpecies->setPseudoSpecies(true);
       
       // set id
       pseudoSpecies->setId(pseudoSpeciesId);
       
       // add the pseudo species to main reaction
       int mRLayer = mR->addSpeciesToVacancy(pseudoSpecies, angleMR);
       
       // find the distance between pseudo species and main reaction center
       double distanceMR = mR->getRadius() + mRLayer * (2 * cReactionRadius + 2.5 * maxSpeciesBoxWidth);
      
       // find the layer of main reaction, in which the connected reaction is located
       int cRLayerInMR = mR->findSubReactionLayer(cR);
       
       // if connected reaction is located in one of main reaction layers and is not main reaction itself
       double distanceCR = 0.0;
       double dyCR = 0.0, dxCR = 0.0, rCR = 0.0, angleCR = 0.0;
       int cRLayer = 0;
       if (cRLayerInMR != -1) {
           dyCR = cR->getCenter().y() - (mR->getCenter().y() - distanceMR * std::sin(angleMR));
           dxCR = (mR->getCenter().x() + distanceMR * std::cos(angleMR)) - cR->getCenter().x();
           rCR = std::sqrt(dxCR * dxCR + dyCR * dyCR);
           angleCR = atan2(dyCR, dxCR);
           while (std::floor(angleCR) < 0)
               angleCR += 2 * pi;
           while (angleCR > 2 * pi)
               angleCR -= 2 * pi;
           
           double _angleCR = angleCR;
           cRLayer = cR->addSpeciesToVacancy(pseudoSpecies, _angleCR);
           
           while (rCR < cR->getRadius() + cRLayer * (2 * cR->getRadius() + 2.5 * maxSpeciesBoxWidth)) {
               cR->removeSpeciesFromVacancy(pseudoSpecies, _angleCR);
               
               mRLayer = mR->transferSpeciesToNextLayerVacancy(pseudoSpecies, angleMR);
               
               // calculate the new distance between the modified location of the pseudo species and main reaction center
               distanceMR = mR->getRadius() + mRLayer * (2 * cR->getRadius() + 2.5 * maxSpeciesBoxWidth);
               
               dyCR = cR->getCenter().y() - (mR->getCenter().y() - distanceMR * std::sin(angleMR));
               dxCR = (mR->getCenter().x() + distanceMR * std::cos(angleMR)) - cR->getCenter().x();
               rCR = std::sqrt(dxCR * dxCR + dyCR * dyCR);
               angleCR = atan2(dyCR, dxCR);
               while (std::floor(angleCR) < 0)
                   angleCR += 2 * pi;
               while (angleCR > 2 * pi)
                   angleCR -= 2 * pi;
               
               _angleCR = angleCR;
               cRLayer = cR->addSpeciesToVacancy(pseudoSpecies, _angleCR);
           }
           
           while (rCR > cR->getRadius() + (cRLayer + 1) * (2 * cR->getRadius() + 2.5 * maxSpeciesBoxWidth))
               cRLayer = cR->transferSpeciesToNextLayerVacancy(pseudoSpecies, _angleCR);
           
           // find the distance between pseudo species and connected reaction center
           distanceCR = cR->getRadius() + cRLayer * (2 * cR->getRadius() + 2.5 * maxSpeciesBoxWidth);
           
           // add reaction member
           pseudoSpecies->addReactionMember(cR, angleCR, true);
           pseudoSpecies->addReactionMember(mR, angleMR, false);
           
           // add pseudo species to the connected reaction
           cR->addPseudoSpecies(pseudoSpecies);
       }
       else {
           // add reaction member
           pseudoSpecies->addReactionMember(mR, angleMR, true);
           
           // add pseudo species to the connected reaction
           mR->addPseudoSpecies(pseudoSpecies);
           
           distanceCR = distanceMR;
           angleCR = angleMR;
       }
       
       // set bounding box
       LBox* pseudoSpeciesBox = new LBox();
       pseudoSpeciesBox->setX(cR->getCenter().x() + distanceCR * std::cos(angleCR));
       pseudoSpeciesBox->setY(cR->getCenter().y() - distanceCR * std::sin(angleCR));
       pseudoSpeciesBox->setWidth(minSpeciesBoxWidth);
       pseudoSpeciesBox->setHeight(minSpeciesBoxHeight);
       pseudoSpecies->setBox(pseudoSpeciesBox);
        
       // add referenced side
       addReferencedSideToSpecies(cR, pseudoSpecies, angleCR);
   }
   
   return pseudoSpecies;
}

std::vector<std::vector<LPoint*>> convertManhatanPathToPoints(NReaction* mR, NSpecies* species, const std::vector<std::pair<int, int>>& manhatanPath) {
   std::vector<std::vector<LPoint*>> points;
   std::pair<int, int> speciesAddress = mR->findSpeciesAddress(species);
   if (speciesAddress.first != -1) {
       std::vector<LPoint*> pathPoint;
       LPoint positionPoint, newPositionPoint, basePoint1, basePoint2;
       double positionRadius, positionAngle;
       double c, theta;
       double dAngle = 2 * pi / mR->getNumSpecies();
       
       for (int i = 0; i < manhatanPath.size(); ++i) {
           positionRadius = mR->getRadius() + speciesAddress.first * (2 * cReactionRadius + 2.5 * maxSpeciesBoxWidth);
           positionAngle = speciesAddress.second * (0.5 * dAngle);
           positionPoint = mR->getCenter() + LPoint(positionRadius * std::cos(positionAngle), -positionRadius * std::sin(positionAngle));
       
           if (i % 2 == 0 && manhatanPath.at(i).first) {
               speciesAddress.second += manhatanPath.at(i).first;
               positionPoint += LPoint(-lLendingSize * (1 + 0.5 * manhatanPath.at(i).second) * std::cos(positionAngle), lLendingSize * (1 + 0.5 * manhatanPath.at(i).second) * std::sin(positionAngle));
               pathPoint.push_back(new LPoint(positionPoint.x(), positionPoint.y()));
               points.push_back(pathPoint);
               pathPoint.clear();
               
               positionRadius = LPoint(positionPoint.x() - mR->getCenter().x(), mR->getCenter().y() - positionPoint.y()).mag();
               positionAngle = atan2(mR->getCenter().y() - positionPoint.y(), positionPoint.x() - mR->getCenter().x());
               while (std::floor(positionAngle) < 0)
                   positionAngle += 2 * pi;
               while (positionAngle > 2 * pi)
                   positionAngle -= 2 * pi;
               
               theta = manhatanPath.at(i).first * 0.5 * dAngle;
               if (std::floor(theta) < 0)
                   theta += std::min(0.1 * dAngle, 8 * lLendingSize/ positionRadius);
               else
                   theta -= std::min(0.1 * dAngle, 8 * lLendingSize/ positionRadius);
               
               newPositionPoint = mR->getCenter() + LPoint(positionRadius * std::cos(positionAngle + theta), - positionRadius * std::sin(positionAngle + theta));
               
               c = 0.0;
               switch (angleToQuadrant(positionAngle + 0.5 * theta)) {
                   case Quad_I_1:
                   case Quad_II_2:
                   case Quad_III_1:
                   case Quad_IV_2:
                       c = (mR->getCenter().x() + positionRadius * std::cos(positionAngle + 0.5 * theta) - 0.5 * positionPoint.x() - 0.5 * newPositionPoint.x()) / (0.75 * std::cos(positionAngle + 0.5 * theta) * std::sin(0.5 * std::abs(theta)));
                       break;
                       
                   case Quad_I_2:
                   case Quad_II_1:
                   case Quad_III_2:
                   case Quad_IV_1:
                       c = (mR->getCenter().y() - positionRadius * std::sin(positionAngle + 0.5 * theta) - 0.5 * positionPoint.y() - 0.5 * newPositionPoint.y()) / (-0.75 * std::sin(positionAngle + 0.5 * theta) * std::sin(0.5 * std::abs(theta)));
               }
               
               if (std::floor(theta) < 0) {
                   positionAngle += theta;
                   theta = std::abs(theta);
                   basePoint1 = LPoint(positionPoint.x() + c * std::sin(theta + positionAngle), positionPoint.y() + c * std::cos(theta + positionAngle));
                   basePoint2 = LPoint(newPositionPoint.x() - c * std::sin(positionAngle), newPositionPoint.y() - c * std::cos(positionAngle));
               }
               else {
                   basePoint1 = LPoint(positionPoint.x() - c * std::sin(positionAngle), positionPoint.y() - c * std::cos(positionAngle));
                   basePoint2 = LPoint(newPositionPoint.x() + c * std::sin(theta + positionAngle), newPositionPoint.y() + c * std::cos(theta + positionAngle));
               }
               
               positionPoint = newPositionPoint;
               pathPoint.push_back(new LPoint(positionPoint.x(), positionPoint.y()));
               pathPoint.push_back(new LPoint(basePoint1.x(), basePoint1.y()));
               pathPoint.push_back(new LPoint(basePoint2.x(), basePoint2.y()));
               points.push_back(pathPoint);
               pathPoint.clear();
           }
           else if (i % 2 == 1) {
               speciesAddress.first -= manhatanPath.at(i).first;
               positionPoint += LPoint(-lLendingSize * (1 + 0.5 * manhatanPath.at(i).second) * std::sin(positionAngle), lLendingSize * (1 + 0.5 * manhatanPath.at(i).second) * std::cos(positionAngle));
               pathPoint.push_back(new LPoint(positionPoint.x(), positionPoint.y()));
               points.push_back(pathPoint);
               pathPoint.clear();
               
               positionRadius = LPoint(positionPoint.x() - mR->getCenter().x(), mR->getCenter().y() - positionPoint.y()).mag();
               positionAngle = atan2(mR->getCenter().y() - positionPoint.y(), positionPoint.x() - mR->getCenter().x());
               while (std::floor(positionAngle) < 0)
                   positionAngle += 2 * pi;
               while (positionAngle > 2 * pi)
                   positionAngle -= 2 * pi;
               
               positionPoint -= manhatanPath.at(i).first * LPoint((2 * cReactionRadius + 2.5 * maxSpeciesBoxWidth) * std::cos(positionAngle), (2 * cReactionRadius + 2.5 * maxSpeciesBoxWidth) * std::sin(positionAngle));
               
               pathPoint.push_back(new LPoint(positionPoint.x(), positionPoint.y()));
               points.push_back(pathPoint);
               pathPoint.clear();
           }
       }
   }
   
   return points;
}

}
