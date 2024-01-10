/// AIFIRA2023SimGeometry.hh
//// Auteur: Arnaud HUBER for ENL group <huber@cenbg.in2p3.fr>
//// Copyright: 2022 (C) Projet RATP - ENL [LP2IB] - CELIA

#ifndef AIFIRA2023SimGeometry_h
#define AIFIRA2023SimGeometry_h  1

#include "G4MaterialPropertiesTable.hh"

class Geometry;
class AIFIRA2023SimMaterials;
class G4VPhysicalVolume;

#include "G4VUserDetectorConstruction.hh"
#include "G4LogicalVolume.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "CLHEP/Units/SystemOfUnits.h"

class  AIFIRA2023SimGeometry:  public G4VUserDetectorConstruction
{
public:
  AIFIRA2023SimGeometry();
  ~AIFIRA2023SimGeometry();

public:
  G4VPhysicalVolume* Construct();
  G4LogicalVolume* GetLWorld() {return LogicalWorld;}

private:
  static const G4String path_bin;
  static const G4String path;
  // Classes for building various components
  //     LogicalHolder *holder;
  AIFIRA2023SimMaterials *scintProp;
  Geometry *theScint;
  G4Material *Vacuum;
  G4Material *VacuumWorld;
  G4Material *Air;
  G4Material *Alu;

  // Colors for visualizations
  G4VisAttributes *invis;
  G4VisAttributes *white;
  G4VisAttributes *gray;
  G4VisAttributes *gray_bis;
  G4VisAttributes *black;
  G4VisAttributes *black_bis;
  G4VisAttributes *red;
  G4VisAttributes *red_hot;
  G4VisAttributes *orange;
  G4VisAttributes *yellow;
  G4VisAttributes *green;
  G4VisAttributes *green_hot;
  G4VisAttributes *cyan;
  G4VisAttributes *blue;
  G4VisAttributes *magenta;

  // Logical Volumes
  G4LogicalVolume *LogicalWorld;
  G4LogicalVolume *LogicalHolder;
  G4LogicalVolume *LogicalSc;
  G4LogicalVolume *LogicalZnS;
  G4LogicalVolume *LogicalPM;
  G4LogicalVolume *LogicalPhotocathode;

  // Physical volumes
  G4VPhysicalVolume *PhysicalWorld;
  G4VPhysicalVolume *PhysicalHolder;
  G4VPhysicalVolume *PhysicalSc;
  G4VPhysicalVolume *PhysicalZnS;
  G4VPhysicalVolume *PhysicalPM;
  G4VPhysicalVolume *PhysicalPhotocathode;

  // Optical surfaces
  G4OpticalSurface *OpticalTeflon;
  G4OpticalSurface *OpticalMylar;
  G4OpticalSurface *OpticalPMT;
  G4OpticalSurface *OpticalAlu;

  // Material Properties Table
  G4MaterialPropertiesTable *TeflonMPT;
  G4MaterialPropertiesTable *MylarMPT;
  G4MaterialPropertiesTable *AluMPT;
  G4MaterialPropertiesTable *PMT_MPT;

  // // Skin surfaces
  G4LogicalSkinSurface *SSTeflon;
  G4LogicalSkinSurface *SSMylar;
  G4LogicalSkinSurface *SSAlu;
  G4LogicalSkinSurface *SSPhotocathode;
  // G4LogicalSkinSurface *SSScintillateur;

  // Dimension values
  G4double ScintillatorThickness;
  G4double AirGap;
  G4double ZnSThickness;
  G4double DetectorThickness;
  
  // Dimensions PLACEMENTS
  G4double Z_Position_ZnS;
  G4double Z_Position_Sc;
  G4double Z_Position_Photocathode;
  

};
#endif
