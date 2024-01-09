/// AIFIRA2023SimTrackingAction.hh
//// Auteur: Arnaud HUBER for ENL group <huber@cenbg.in2p3.fr>
//// Copyright: 2022 (C) Projet RATP - ENL [LP2IB] - CELIA

#ifndef AIFIRA2023SimTrackingAction_h
#define AIFIRA2023SimTrackingAction_h 1

#include "G4UserTrackingAction.hh"
#include "G4Track.hh"

class AIFIRA2023SimTrackingAction : public G4UserTrackingAction
{
public:
  AIFIRA2023SimTrackingAction();
  ~AIFIRA2023SimTrackingAction();

  void PreUserTrackingAction(const G4Track*);
  void PostUserTrackingAction(const G4Track*);
};
#endif
