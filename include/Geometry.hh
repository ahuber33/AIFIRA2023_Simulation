/// Geometry.hh
//// Auteur: Arnaud HUBER for ENL group <huber@cenbg.in2p3.fr>
//// Copyright: 2022 (C) Projet RATP - ENL [LP2IB] - CELIA

// This class is used to create a plastic Geometry.

#ifndef Geometry_h
#define Geometry_h

#include "G4LogicalVolume.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "CLHEP/Units/SystemOfUnits.h"
#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Polyhedra.hh"
#include <G4Polycone.hh>
#include "G4Sphere.hh"
#include "G4Trap.hh"
#include "G4Trd.hh"
#include "G4SubtractionSolid.hh"

class AIFIRA2023SimMaterials;
class Geometry

{
public:

  //constructor, builds from keys specified in buildfile
  Geometry(G4String buildfile);
  ~Geometry();
  //  void Construct();

public:

  // Functions to call this geometry in x_blockGeometry.cc
  G4LogicalVolume *GetSc();
  G4LogicalVolume *GetZnS();
  G4LogicalVolume *GetPhotocathode();
  G4LogicalVolume *GetRoundPhotocathode();


  // Functions that can be called to return various scint dimensions

  //****************COMMON********************
  // wrapping
  G4double GetAirGap(){return AirGap;}
  // Position of Detector
  G4double GetScintillatorThickness(){return ScintillatorThickness;}
  G4double GetZnSThickness(){return ZnSThickness;}
  G4double GetDetectorThickness(){return DetectorThickness;}

private:


  Geometry *theScint;
  AIFIRA2023SimMaterials* scintProp;

  static const G4String path_bin;


  // Materials
  G4Material *Material;

  // Logical Volumes
  G4LogicalVolume *LogicalVolume;

  G4double ScintillatorLength;
  G4double ScintillatorWidth;
  G4double ScintillatorThickness;
  G4double ZnSThickness;
  G4double DetectorLength;
  G4double DetectorWidth;
  G4double DetectorThickness;
  // Physical Dimensions
  // wrapping
  G4double AirGap;
  
  // Other
  G4VisAttributes *clear;

};
#endif
