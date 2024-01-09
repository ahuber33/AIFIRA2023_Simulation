//// FiberParametrisation.cc
//// Auteur: Arnaud HUBER for ENL group <huber@cenbg.in2p3.fr>
//// Copyright: 2022 (C) Projet RATP

#include "FiberParameterisation.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FiberParameterisation::FiberParameterisation(
        G4int    noFibers,
        G4double start,          //  Z of center of first
        G4double spacing,        //  Z spacing of centers
        G4double widthFiber,
        G4double length)
{
   fNoFibers =  noFibers;
   fStart     =  start;
   fLength  =  length/2;
   fSpacing    =  spacing;
   fWidth = widthFiber/2;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FiberParameterisation::~FiberParameterisation()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FiberParameterisation::ComputeTransformation
(const G4int copyNo, G4VPhysicalVolume* physVol) const
{
  // Note: copyNo will start with zero!
  G4int columns = copyNo/fNoFibers;
  G4int lines = copyNo %fNoFibers;
  G4double Yposition = fStart + lines * fSpacing;
  G4double Xposition= fStart + columns* fSpacing;
  G4ThreeVector origin(Xposition,Yposition,0);
  physVol->SetTranslation(origin);
  physVol->SetRotation(nullptr);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FiberParameterisation::ComputeDimensions
(G4Box& trackerFiber, const G4int copyNo, const G4VPhysicalVolume*) const
{
  // Note: copyNo will start with zero!
  trackerFiber.SetXHalfLength(fWidth);
  trackerFiber.SetYHalfLength(fWidth);
  trackerFiber.SetZHalfLength(fLength);
}


void FiberParameterisation::ComputeDimensions
(G4Tubs& trackerFiber, const G4int copyNo, const G4VPhysicalVolume*) const
{
  // Note: copyNo will start with zero!
  trackerFiber.SetInnerRadius(0);
  trackerFiber.SetOuterRadius(fWidth);
  trackerFiber.SetZHalfLength(fLength);
  trackerFiber.SetStartPhiAngle(0.*deg);
  trackerFiber.SetDeltaPhiAngle(360.*deg);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
