// Geometry.cc
//// Auteur: Arnaud HUBER for ENL group <huber@cenbg.in2p3.fr>
//// Copyright: 2022 (C) Projet RATP - ENL [LP2IB] - CELIA

#include "AIFIRA2023SimMaterials.hh"
#include "Geometry.hh"
#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4ThreeVector.hh"
#include "G4VisAttributes.hh"
#include "G4Transform3D.hh"
#include "G4UnionSolid.hh"
#include <fstream>
#include <iostream>

#include "G4PVPlacement.hh"
#include "G4UnitsTable.hh"
#include <math.h>

//#ifndef disable_gdml
#include "G4GDMLParser.hh"
//#endif


using namespace CLHEP;

// ***********************
// Constructor
// ***********************
const G4String Geometry::path_bin = "../bin/";


Geometry::Geometry(G4String buildfile){

  // Read keys and values from file buildfile defined in x_blockGeometry.cc
  // Some of these variables may not be needed.
  std::ifstream config;
  config.open(buildfile);
  if ( !config.is_open() )
  G4cout << "Error opening file " << buildfile << G4endl;
  else{
    while( config.is_open() ){
      G4String variable;
      G4String unit;
      G4double value;

      config >> variable;
      if(!config.good()) break;
      //####################### COMMON variables ###########################
      if(variable == "ScintillatorLength"){
        config >> value >> unit;
        ScintillatorLength = value*G4UnitDefinition::GetValueOf(unit);
      }
      if(variable == "ScintillatorWidth"){
        config >> value >> unit;
        ScintillatorWidth = value*G4UnitDefinition::GetValueOf(unit);
      }
      else if(variable == "ScintillatorThickness"){
        config >> value >> unit;
        ScintillatorThickness = value*G4UnitDefinition::GetValueOf(unit);
      }
      else if(variable == "ZnSThickness"){
        config >> value >> unit;
        ZnSThickness = value*G4UnitDefinition::GetValueOf(unit);
      }
      else if(variable == "DetectorLength"){
        config >> value >> unit;
        DetectorLength = value*G4UnitDefinition::GetValueOf(unit);
      }
      else if(variable == "DetectorWidth"){
        config >> value >> unit;
        DetectorWidth = value*G4UnitDefinition::GetValueOf(unit);
      }
      else if(variable == "DetectorThickness"){
        config >> value >> unit;
        DetectorThickness = value*G4UnitDefinition::GetValueOf(unit);
      }
      else if(variable == "AirGap"){
        config >> value >> unit;
        AirGap = value*G4UnitDefinition::GetValueOf(unit);
      }
    }
  }
  config.close();



  G4cout << "\n The Variables that we read in are: "
  << "\n ScintillatorLength = " << ScintillatorLength
  << "\n ScintillatorWidth = " << ScintillatorWidth
  << "\n ScintillatorThickness = " << ScintillatorThickness
  << "\n ZnSThickness = " << ZnSThickness
  << "\n DetectorLength = " << DetectorLength
  << "\n DetectorWidth = " << DetectorWidth
  << "\n DetectorThickness = " << DetectorThickness
  << "\n Air gap = " << AirGap
  << "\n " << G4endl;

}
// ***********************
// Destructor
// ***********************
Geometry::~Geometry(){
  if(scintProp) { delete scintProp; scintProp = 0; }
  if(clear) { delete clear; clear = 0; }
}


G4LogicalVolume *Geometry::GetSc(){

  Material = scintProp->GetMaterial("EJ262");
  //Material = scintProp->GetMaterial("YAG");
  //Material = scintProp->GetMaterial("ZnS");

  //scintillator = scintProp->GetMaterial("Alu");

  G4Box *Box = new G4Box   ("Box",             //its name
  ScintillatorLength/2, ScintillatorWidth/2, ScintillatorThickness/2);    //its size

  LogicalVolume = new G4LogicalVolume(Box, Material, "Sc_Test",0,0,0);

  return LogicalVolume;
}


G4LogicalVolume *Geometry::GetZnS(){

  Material = scintProp->GetMaterial("ZnS");

  G4Box *Box = new G4Box   ("Box",             //its name
  ScintillatorLength/2, ScintillatorWidth/2, ZnSThickness/2);    //its size
  LogicalVolume = new G4LogicalVolume(Box, Material, "ZnS",0,0,0);

  return LogicalVolume;
}


G4LogicalVolume *Geometry::GetPhotocathode(){
  // Materials properties for PMT
  //Material = scintProp->GetMaterial("Vacuum");
  Material = scintProp->GetMaterial("Silicium");

  G4Box *Box = new G4Box   ("Box",             //its name
  //ScintillatorLength/2, 60/2, DetectorThickness/2);    //its size
  //(Fiber_number_per_line*Fiber_width + (Fiber_number_per_line+1)*Fiber_space)/2, (Fiber_number_per_line*Fiber_width + (Fiber_number_per_line+1)*Fiber_space)/2, DetectorThickness/2);    //its size
  DetectorLength/2, DetectorWidth/2, DetectorThickness/2);    //its size

  LogicalVolume = new G4LogicalVolume(Box, Material, "Photocathode",0,0,0);

  return LogicalVolume;
}


G4LogicalVolume *Geometry::GetRoundPhotocathode(){

  Material = scintProp->GetMaterial("Silicium");

  G4Tubs *Tubs = new G4Tubs   ("Tubs",             //its name
  0., (75/2)*mm, (0.1/2)*mm, 0, 360*deg);    //its size

  LogicalVolume = new G4LogicalVolume(Tubs, Material, "Photocathode",0,0,0);

  return LogicalVolume;
}
