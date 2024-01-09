// AIFIRA2023SimGeometry_test.cc
//// Auteur: Arnaud HUBER for ENL group <huber@cenbg.in2p3.fr>
/// Copyright: 2017 (C) Projet BADGE - CARMELEC -P2R

#include "AIFIRA2023SimGeometry.hh"
#include "AIFIRA2023SimRunAction.hh"
#include "AIFIRA2023SimMaterials.hh"
#include "AIFIRA2023SimSteppingAction.hh"
#include "Scintillator.hh"
#include "Coupling.hh"
#include "PMT.hh"
#include "GdL.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4UnitsTable.hh"
#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4Element.hh"
#include "G4ElementTable.hh"
#include "G4Box.hh"
#include "G4Orb.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4SDManager.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4MaterialTable.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4RunManager.hh"
#include "G4Transform3D.hh"
#include "G4SurfaceProperty.hh"
#include "globals.hh"
#include <fstream>
#include <iostream>

using namespace CLHEP;

const G4String AIFIRA2023SimGeometry::path_bin = "../bin/";
const G4String AIFIRA2023SimGeometry::path = "../simulation_input_files/";

// Constructor
AIFIRA2023SimGeometry::AIFIRA2023SimGeometry(){}

// Destructor
AIFIRA2023SimGeometry::~AIFIRA2023SimGeometry()
{
}

// Main Function: Builds Full block, coupling, and PMT geometries
G4VPhysicalVolume* AIFIRA2023SimGeometry::Construct( ){
  // Initialize scint classes
  scintProp = new AIFIRA2023SimMaterials(path_bin+"Materials.cfg");
  Vacuum = scintProp->GetMaterial("Vacuum");
  VacuumWorld = scintProp->GetMaterial("VacuumWorld");
  Air = scintProp->GetMaterial("Air");
  theScint = new Scintillator(path_bin+"AIFIRA2023Sim.cfg");


  // ***********************
  // Visualization Colors
  // ***********************
  // Create some colors for visualizations
  invis = new G4VisAttributes( G4Colour(255/255. ,255/255. ,255/255. ));
  invis->SetVisibility(false);

  white = new G4VisAttributes(G4Colour(1,1,1,0.1)); // Sets the color (can be looked up online)
  //white->SetForceWireframe(true); // Sets to wire frame mode for coloring the volume
  white->SetForceSolid(true); // Sets to solid mode for coloring the volume
  white->SetVisibility(true); // Makes color visible in visualization

  gray = new G4VisAttributes(G4Colour(0.5,0.5,0.5,0.5));
  //  gray->SetForceWireframe(true);
   gray->SetForceSolid(true);
  gray->SetVisibility(true);

  gray_bis = new G4VisAttributes(G4Colour(0.5,0.5,0.5,0.25));
  //  gray->SetForceWireframe(true);
  gray_bis->SetForceSolid(true);
  gray_bis->SetVisibility(true);

  black = new G4VisAttributes(G4Colour(0,0,0,0.7));
  //  black->SetForceWireframe(true);
  black->SetForceSolid(true);
  black->SetVisibility(true);


  black_bis = new G4VisAttributes(G4Colour(0,0,0,0.4));
  //  black->SetForceWireframe(true);
  black_bis->SetForceSolid(true);
  black_bis->SetVisibility(true);

  red = new G4VisAttributes(G4Colour(1,0,0,0.5));
  //  red->SetForceWireframe(true);
  red->SetForceSolid(true);
  red->SetVisibility(true);


  red_hot = new G4VisAttributes(G4Colour(1,0,0,1));
  //  red->SetForceWireframe(true);
  red_hot->SetForceSolid(true);
  red_hot->SetVisibility(true);

  orange = new G4VisAttributes(G4Colour(1,0.5,0,0.3));
  //  orange->SetForceWireframe(true);
  orange->SetForceSolid(true);
  orange->SetVisibility(true);

  yellow = new G4VisAttributes(G4Colour(1,1,0,0.19));
  //  yellow->SetForceWireframe(true);
  yellow->SetForceSolid(true);
  yellow->SetVisibility(true);

  green = new G4VisAttributes(G4Colour(0,1,0,0.35));
  //  green->SetForceWireframe(true);
  green->SetForceSolid(true);
  green->SetVisibility(true);

  green_hot = new G4VisAttributes(G4Colour(0,1,0,1));
  //  green_hot->SetForceWireframe(true);
  green_hot->SetForceSolid(true);
  green_hot->SetVisibility(true);

  cyan = new G4VisAttributes(G4Colour(0,1,1,0.55));
  //  cyan->SetForceWireframe(true);
  cyan->SetForceSolid(true);
  cyan->SetVisibility(true);

  blue = new G4VisAttributes(G4Colour(0,0,1,0.3));
  //  blue->SetForceWireframe(true);
  blue->SetForceSolid(true);
  blue->SetVisibility(true);

  magenta = new G4VisAttributes(G4Colour(1,0,1,0.85));
  //  magenta->SetForceWireframe(true);
  //magenta->SetForceSolid(true);
  magenta->SetVisibility(true);

  // Define common rotations
  G4RotationMatrix DontRotate;
  DontRotate.rotateX(0.0*deg);
  G4RotationMatrix Flip;
  Flip.rotateZ(0*deg);
  Flip.rotateX(180*deg);
  Flip.rotateY(0*deg);


// ***********************
// Various dimensions
// ***********************
//Scint
SNMW_ScintStepHeight = theScint->GetSNMW_ScintStepHeight();
SNMW_ScintStepWidth = theScint->GetSNMW_ScintStepWidth();
SNMW_ScintBodyHeight = theScint->GetSNMW_ScintBodyHeight();
SNMW_ScintBodyWidth = theScint->GetSNMW_ScintBodyWidth();
SNMW_FullScintHeight = theScint->GetSNMW_FullScintHeight();
SNMW_CouplingSphereRadius = theScint->GetSNMW_CouplingRadius();
SNMW_CouplingSphereDepth = theScint->GetSNMW_CouplingDepth();
// Wrapping
TeflonThickness = theScint->GetTeflonThickness();
TeflonGap = theScint->GetAirGapTeflon();
MylarThickness = theScint->GetMylarThickness();
MylarGap = theScint->GetAirGapMylar();


  //#########################
  // DEFINE GEOMETRY VOLUMES#
  //#########################

  // Create World Volume
  // This is just a big box to place all other logical volumes inside
  G4Box *SolidWorld = new G4Box("SolidWorld", 110*cm, 110*cm, 110*cm );
  LogicalWorld = new G4LogicalVolume(SolidWorld, VacuumWorld,"LogicalWorld",0,0,0);
  LogicalWorld->SetVisAttributes(invis);

  // Place the world volume: center of world at origin (0,0,0)
  PhysicalWorld = new G4PVPlacement(G4Transform3D
				    (DontRotate,G4ThreeVector(0,0,0)),
				    "PhysicalWorld",LogicalWorld,NULL,false,0);



  // Create Holder Volume
  // This is just a big box to count the escaped photons
  //G4Box *s_holder;
  G4Box *s_holder = new G4Box("s_holder", 100*cm, 100*cm, 100*cm );

  LogicalHolder = new G4LogicalVolume(s_holder,Air,"logical_holder",0,0,0); //Replace Air with Vacuum (init)

  // Place the holder volume: center of world at origin (0,0,0)
  PhysicalHolder = new G4PVPlacement(G4Transform3D
				    (DontRotate,G4ThreeVector(0,0,0)),
				    LogicalHolder, "Air",LogicalWorld,false,0);

  LogicalHolder->SetVisAttributes(invis);


  //*********************************
  // Build scint et wrapping volumes*
  //*********************** *********
  //Simply calls functions from Scintillator() class
  LogicalGM_LND = theScint->GetGM_LND();
  LogicalSc = theScint->GetSNMW_8InchesScint();
  LogicalMylar = theScint->GetSNMW_8InchesMylar();
  LogicalTeflon = theScint->GetSNMW_Teflon();

  // Set colors of various block materials
  LogicalGM_LND->SetVisAttributes(red);
	LogicalSc->SetVisAttributes(cyan);
  LogicalTeflon->SetVisAttributes(gray);
  LogicalMylar->SetVisAttributes(magenta);




  // Define Teflon properties
// Properties are read in from data file
std::ifstream Read_teflon;
G4String teflon_file = path+"teflon.dat";
std::vector<G4double> Teflon_Energy;
std::vector<G4double> Teflon_Reflectivity;
std::vector<G4double> Teflon_Zero;
G4double wavelength; // x values
G4double teflon_ref_coeff; // y values
Read_teflon.open(teflon_file);
if (Read_teflon.is_open()){
  while(!Read_teflon.eof()){
    G4String filler; // This just skips the coma and space in data files
    Read_teflon >> wavelength >> filler >> teflon_ref_coeff;
    Teflon_Energy.push_back((1240/wavelength)*eV);
    Teflon_Reflectivity.push_back(1.*teflon_ref_coeff);
    Teflon_Zero.push_back(1e-6);
  }
}
else
  G4cout << "Error opening file: " << teflon_file << G4endl; // throw an error if file is not found
Read_teflon.close();

// Define Teflon optical boundary surface properties
OpticalTeflon = new G4OpticalSurface("OpticalTeflon");
OpticalTeflon->SetModel(unified); // Either glisur (GEANT3 model) or unified surface model: set references online
OpticalTeflon->SetPolish(0); // Set level of polish
OpticalTeflon->SetFinish(groundfrontpainted); // Sets finish to one of 6 options: see references online - based off polished or grounded
OpticalTeflon->SetType(dielectric_metal); // Sets boundary type: dielectric-dielectric or dielectric-metal
// Define Teflon material properties
TeflonMPT = new G4MaterialPropertiesTable(); // Creates table to populate with various material properties: reflectivity, ...
TeflonMPT->AddProperty("REFLECTIVITY",Teflon_Energy, Teflon_Reflectivity);
TeflonMPT->AddProperty("SPECULARLOBECONSTANT",Teflon_Energy, Teflon_Zero);
TeflonMPT->AddProperty("SPECULARSPIKECONSTANT", Teflon_Energy, Teflon_Zero);
TeflonMPT->AddProperty("BACKSCATTERCONSTANT", Teflon_Energy, Teflon_Zero);
// Geometrical implementation of boundary surface
OpticalTeflon->SetMaterialPropertiesTable(TeflonMPT);
SSTeflon = new G4LogicalSkinSurface("b_Teflon",LogicalTeflon,OpticalTeflon); // Applies optical Teflon properties to ogical Teflon surface

// Define Mylar properties
std::ifstream Read_mylar;
G4String mylar_file = path+"mylar.dat";
std::vector<G4double> Mylar_Energy;
std::vector<G4double> Mylar_Reflectivity;
std::vector<G4double> Mylar_Zero;
std::vector<G4double> Mylar_Un;
G4double mylar_ref_coeff;
Read_mylar.open(mylar_file);
if (Read_mylar.is_open()){
  while(!Read_mylar.eof()){
    G4String filler;
    Read_mylar >> wavelength >> filler >> mylar_ref_coeff;
    Mylar_Energy.push_back((1240/wavelength)*eV);
    Mylar_Reflectivity.push_back(1.*mylar_ref_coeff);
    Mylar_Zero.push_back(0.0);
    Mylar_Un.push_back(1.0);
  }
}
else
  G4cout << "Error opening file: " << mylar_file << G4endl;
Read_mylar.close();

// Define Mylar optical boundary surface properties
OpticalMylar = new G4OpticalSurface("OpticalMylar");
OpticalMylar->SetModel(glisur);
OpticalMylar->SetFinish(polished);
OpticalMylar->SetType(dielectric_metal);

// Define Mylar material properties
MylarMPT = new G4MaterialPropertiesTable();
MylarMPT->AddProperty("REFLECTIVITY", Mylar_Energy, Mylar_Reflectivity);
MylarMPT->AddProperty("SPECULARLOBECONSTANT", Mylar_Energy, Mylar_Un);
MylarMPT->AddProperty("SPECULARSPIKECONSTANT", Mylar_Energy, Mylar_Un);
//MylarMPT->AddProperty("DIFFUSELOBECONSTANT",mylar_energy,realzero,mylar_entries);
MylarMPT->AddProperty("BACKSCATTERCONSTANT", Mylar_Energy, Mylar_Zero);

// Geometrical implementation of boundary surface
OpticalMylar->SetMaterialPropertiesTable(MylarMPT);
SSMylar = new G4LogicalSkinSurface("SSMylar",LogicalMylar,OpticalMylar); // Applies Mylar properties to logical mylar surface



//***********************
// Build coupling medium
//***********************
CouplingGlue = new Coupling();  // Initialize coupling class
LogicalGlue = CouplingGlue->GetSNMW_8InchesSphericalCoupling(); // Call function for building glue
LogicalGlue->SetVisAttributes(red); // Set glue color to red


//***********************
// Build PMT volumes
//***********************
// Initialize PMT class object for creating R6594-03 HPK PMT
R6594 = new PMT();

// Build the PMT glass structure from PMT class
LogicalPM = R6594->Get8InchesPMTGlass(); // Call function for PMT glass
LogicalPM->SetVisAttributes(yellow); // Set glass color to yellow

// Build the photocathode from PMT class
LogicalPhotocathode = R6594->Get8InchesPhotocathode(); // Call function for PMT photocathode
LogicalPhotocathode->SetVisAttributes(orange); // Set photocathode color to orange


// Define PMT properties
G4double indexconst = 1.49; // Index currently set to constant for PMT glass
G4double reflectconst = 0.;
std::ifstream ReadPMT;
//G4String PMTfile = path+"QE_ham_GA0124.txt";
G4String PMTfile = path+"9102B_ET_reverse.txt";
std::vector<G4double> Photocathode_Energy;
std::vector<G4double> Photocathode_Value;
std::vector<G4double> Photocathode_Index;
std::vector<G4double> Photocathode_Reflectivity;
G4double quant_eff;
ReadPMT.open(PMTfile);
if (ReadPMT.is_open()){
  while(!ReadPMT.eof()){
    G4String filler;
    ReadPMT >> wavelength >> filler >> quant_eff;
    //PMTdetect[PMTentries] = 0.79*quant_eff*.65;  //0.79 = correction factor to adjust QE to lower value of 34% for R6594 and 0.59 = 25%
    //PMTdetect1[PMTentries] = 1; // Use only if you want %100 QE
    Photocathode_Energy.push_back((1240/wavelength)*eV);
    Photocathode_Value.push_back(quant_eff*0.57);  //For the GAxxxx file with good QE and 0.65 for collection efficiency
    Photocathode_Index.push_back(indexconst);
    Photocathode_Reflectivity.push_back(reflectconst);
  }
}
else
  G4cout << "Error opening file: " << PMTfile << G4endl;
ReadPMT.close();

// Define PMT optical boundary surface properties
OpticalPMT = new G4OpticalSurface("OpticalPMT");
OpticalPMT->SetModel(glisur);
OpticalPMT->SetFinish(polished);
OpticalPMT->SetType(dielectric_metal);

// Define PMT material properties
PMT_MPT = new G4MaterialPropertiesTable();
PMT_MPT->AddProperty("EFFICIENCY",Photocathode_Energy, Photocathode_Value);
PMT_MPT->AddProperty("REFLECTIVITY",Photocathode_Energy, Photocathode_Reflectivity);
PMT_MPT->AddProperty("RINDEX", Photocathode_Energy, Photocathode_Index);

// Geometrical implementation of boundary surface
OpticalPMT->SetMaterialPropertiesTable(PMT_MPT);
SSPhotocathode = new G4LogicalSkinSurface("SSVikuiti",LogicalPhotocathode,OpticalPMT);



  //############################
  // DEFINE GEOMETRY PLACEMENTS#
  //############################

  #ifndef disable_gdml


  //***********************
// Various Positioning values
//***********************
G4double translation_visu = 0.0*mm;
G4double GlueZpos = SNMW_ScintStepHeight/2 + SNMW_ScintBodyHeight - SNMW_CouplingSphereDepth + SNMW_CouplingSphereRadius;
//G4double LightGuideZpos = ScintStepHeight/2 + ScintBodyHeight; // Need to change glue and PMT values if used

G4cout << "Teflon Zpos = " << (TeflonThickness+TeflonGap)/2
      << "\nMylar Zpos = " << (TeflonThickness+TeflonGap)/2
      << "\nGlue Zpos = " << GlueZpos << G4endl;



  // PhysicalDolphy_GM = new G4PVPlacement(G4Transform3D
	//     (DontRotate,G4ThreeVector(0*mm,0.*mm,0.*mm)), // Set at origin as basis of everything else
	//     LogicalDolphy_GM,"GM_LND",
	//     LogicalHolder,false,0);


	PhysicalSc = new G4PVPlacement(G4Transform3D
	    (DontRotate,G4ThreeVector(0*mm,0.*mm,0.*mm)), // Set at origin as basis of everything else
	    LogicalSc,"Scintillator",
	    LogicalHolder,false,0);


  PhysicalTeflon = new G4PVPlacement(G4Transform3D
      (DontRotate,G4ThreeVector(0*mm,0.*mm,(TeflonThickness+TeflonGap)/2)), // Set at origin as basis of everything else
	    LogicalTeflon,"Teflon",
	    LogicalHolder,false,0);

  PhysicalMylar = new G4PVPlacement(G4Transform3D
      (DontRotate,G4ThreeVector(0*mm,0.*mm, (TeflonThickness+TeflonGap)/2)), // Set at origin as basis of everything else
      LogicalMylar,"Mylar",
      LogicalHolder,false,0);

      // Glue placement
  PhysicalGlue = new G4PVPlacement(G4Transform3D
     (Flip,G4ThreeVector(0,0,220*mm)),
     LogicalGlue,"Glue",
     LogicalHolder,false,0);


      // PMT placement
  PhysicalPM = new G4PVPlacement(G4Transform3D
     (Flip,G4ThreeVector(0,0,220*mm)),
     LogicalPM,"PM",
     LogicalHolder,false,0);

     // PMT photocathode placement
   PhysicalPhotocathode = new G4PVPlacement(G4Transform3D
     (Flip,G4ThreeVector(0,0,220*mm)),
     LogicalPhotocathode,"Photocathode",LogicalHolder,true,0);



#else

#endif





  // Returns world with everything in it and all properties set
  return PhysicalWorld;
}
