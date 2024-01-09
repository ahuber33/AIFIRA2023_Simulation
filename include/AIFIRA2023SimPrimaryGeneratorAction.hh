/// AIFIRA2023SimPrimaryGeneratorAction.hh
//// Auteur: Arnaud HUBER for ENL group <huber@cenbg.in2p3.fr>
//// Copyright: 2022 (C) Projet RATP - ENL [LP2IB] - CELIA

#ifndef AIFIRA2023SimPrimaryGeneratorAction_h
#define AIFIRA2023SimPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"
#include "G4ParticleGun.hh"
#include "G4GeneralParticleSource.hh"

class G4Event;

class AIFIRA2023SimPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  AIFIRA2023SimPrimaryGeneratorAction();
  ~AIFIRA2023SimPrimaryGeneratorAction();

public:
  void GeneratePrimaries(G4Event* anEvent);
  void SetEnergy(G4double en){particleGun->SetParticleEnergy(en);};

  G4double GetEnergy(){return particleGun->GetParticleEnergy();};

private:
  G4ParticleGun* particleGun;
  G4GeneralParticleSource *particleSource;
  G4double IncidentE;
};

#endif
