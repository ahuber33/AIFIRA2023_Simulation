/// AIFIRA2023SimPhysics.hh
//// Auteur: Arnaud HUBER for ENL group <huber@cenbg.in2p3.fr>
//// Copyright: 2022 (C) Projet RATP - ENL [LP2IB] - CELIA

#ifndef  AIFIRA2023SimPhysics_h
#define   AIFIRA2023SimPhysics_h  1

//#include "G4VUserPhysicsList.hh"
#include "G4VModularPhysicsList.hh"
#include "CLHEP/Units/SystemOfUnits.h"
#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
class  AIFIRA2023SimPhysics:  public G4VModularPhysicsList
{      public:
  AIFIRA2023SimPhysics();
  virtual ~AIFIRA2023SimPhysics();

protected:
  virtual void ConstructParticle();
  virtual void ConstructProcess();
  virtual void ConstructOp();
  virtual void SetCuts();
protected:
  //virtual void ConstructBosons();
  //virtual void ConstructLeptons();
  //virtual void ConstructMesons();
  //virtual void ConstructBaryons();
protected:
  //virtual void ConstructGeneral();
  //virtual void ConstructEM();
  //virtual void ConstructAllShortLiveds();

private:
  G4VPhysicsConstructor*  emPhysicsList;
  G4VPhysicsConstructor*  particleList;
  G4VPhysicsConstructor*  raddecayList;
  std::vector<G4VPhysicsConstructor*>  hadronPhys;
};
#endif
