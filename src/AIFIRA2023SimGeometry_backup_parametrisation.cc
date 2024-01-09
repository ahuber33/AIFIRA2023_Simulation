//// AIFIRA2023SimGeometry.cc
//// Auteur: Arnaud HUBER for ENL group <huber@cenbg.in2p3.fr>
//// Copyright: 2022 (C) Projet RATP - ENL [LP2IB] - CELIA

#include "AIFIRA2023SimGeometry.hh"
#include "AIFIRA2023SimRunAction.hh"
#include "AIFIRA2023SimMaterials.hh"
#include "AIFIRA2023SimSteppingAction.hh"
#include "Geometry.hh"
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
#include "G4PVParameterised.hh"
#include "FiberParameterisation.hh"
#include "G4PVReplica.hh"
#include "G4SubtractionSolid.hh"
#include "G4AssemblyVolume.hh"

#include "G4ElectroMagneticField.hh"
#include "G4MagneticField.hh"
#include "G4UniformMagField.hh"
#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4EquationOfMotion.hh"
#include "G4Mag_UsualEqRhs.hh"
#include "G4MagIntegratorStepper.hh"
#include "G4ChordFinder.hh"
#include "G4UniformElectricField.hh"
#include "G4EqMagElectricField.hh"
#include "G4MagIntegratorStepper.hh"
#include "G4MagIntegratorDriver.hh"

#include "G4ExplicitEuler.hh"
#include "G4ImplicitEuler.hh"
#include "G4SimpleRunge.hh"
#include "G4SimpleHeum.hh"
#include "G4ClassicalRK4.hh"
#include "G4HelixExplicitEuler.hh"
#include "G4HelixImplicitEuler.hh"
#include "G4HelixSimpleRunge.hh"
#include "G4CashKarpRKF45.hh"
#include "G4RKG3_Stepper.hh"
#include "G4ConstRK4.hh"
#include "G4NystromRK4.hh"
#include "G4HelixMixedStepper.hh"
#include "G4ExactHelixStepper.hh"
#include "G4InterpolationDriver.hh"
#include "G4IntegrationDriver.hh"
#include "G4VIntegrationDriver.hh"
#include "G4BFieldIntegrationDriver.hh"
#include <G4HelixHeum.hh>

// Newest steppers - from Release 10.3-beta (June 2013)
#include "G4BogackiShampine23.hh"
#include "G4BogackiShampine45.hh"
#include "G4DormandPrince745.hh"
#include "G4DormandPrinceRK56.hh"
#include "G4DormandPrinceRK78.hh"
#include "G4TsitourasRK45.hh"

#include "G4RegionStore.hh"
#include "FastSimModelOpFiber.hh"
#include "SimG4FastSimOpFiberRegion.hh"

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
  Alu = scintProp->GetMaterial("Alu");
  theScint = new Geometry(path_bin+"AIFIRA2023Sim.cfg");


  // ***********************
  // Visualization Colors
  // ***********************
  // Create some colors for visualizations
  invis = new G4VisAttributes( G4Colour(255/255. ,255/255. ,255/255. ));
  invis->SetVisibility(false);

  white = new G4VisAttributes(G4Colour(1,1,1,1.)); // Sets the color (can be looked up online)
  //white->SetForceWireframe(true); // Sets to wire frame mode for coloring the volume
  white->SetForceSolid(true); // Sets to solid mode for coloring the volume
  white->SetVisibility(true); // Makes color visible in visualization

  gray = new G4VisAttributes(G4Colour(0.5,0.5,0.5,0.5));
  //  gray->SetForceWireframe(true);
  gray->SetForceSolid(true);
  gray->SetVisibility(true);

  gray_bis = new G4VisAttributes(G4Colour(0.5,0.5,0.5,0.05));
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

  orange = new G4VisAttributes(G4Colour(1,0.5,0,0.1));
  //  orange->SetForceWireframe(true);
  orange->SetForceSolid(true);
  orange->SetVisibility(true);

  yellow = new G4VisAttributes(G4Colour(1,1,0,0.1));
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

  cyan = new G4VisAttributes(G4Colour(0,1,1,0.1));
  //  cyan->SetForceWireframe(true);
  cyan->SetForceSolid(true);
  cyan->SetVisibility(true);

  blue = new G4VisAttributes(G4Colour(0,0,1,0.5));
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
  EF_Value = theScint->GetEFValue();
  EF_Dist_between_plates = theScint->GetDistBetweenPlates();
  EF_Thickness_plates = theScint->GetEFPlatesThickness();
  EF_Length_plates = theScint->GetEFPlatesLength();
  EF_Width_plates = theScint->GetEFPlatesWidth();
  MF_Value = theScint->GetMFValue();
  MF_Dist_between_plates = theScint->GetMFDistBetweenPlates();
  MF_Thickness_plates = theScint->GetMFPlatesThickness();
  MF_Length_plates = theScint->GetMFPlatesLength();
  MF_Width_plates = theScint->GetMFPlatesWidth();
  Dist_between_plates = theScint->GetDistBetweenPlates();
  Dist_EFPlates_Detector = theScint->GetEFPlatesDetector();
  Dist_pinhole_MFPlates = theScint->GetPinholeMFPlates();
  translation_pinhole = theScint->GetTranslationPinhole();
  ScintillatorThickness = theScint->GetScintillatorThickness();
  ZnSThickness = theScint->GetZnSThickness();
  ZnSLGThickness = theScint->GetZnSLGThickness();
  DetectorThickness = theScint->GetDetectorThickness();
  DetectorTranslation = theScint->GetDetectorTranslation();
  PinholeThickness = theScint->GetPinholeThickness();
  FiberLength = theScint->GetFiberLength();
  FiberWidth = theScint->GetFiberWidth();
  FiberCladdingRatio = theScint->GetFiberCladdingRatio();
  FiberNumberPerLine = theScint->GetFiberNumberPerLine();
  FiberSpace = theScint->GetFiberSpace();
  NbOfFibers = FiberNumberPerLine*FiberNumberPerLine;
  FiberSpacing = FiberWidth+FiberSpace;
  FiberGeometry = theScint->GetFiberGeometry();
  FiberMultiCladding = theScint->GetFiberMultiCladdingOption();
  FiberWidthCore=0;
  if(FiberMultiCladding ==0) FiberWidthCore = FiberWidth-2*FiberCladdingRatio*FiberWidth;
  else if(FiberMultiCladding ==1)
  {
    FiberWidthCore = FiberWidth-4*FiberCladdingRatio*FiberWidth;
    FiberWidthCladding = FiberWidth-2*FiberCladdingRatio*FiberWidth;
  }
  else  G4Exception("AIFIRA2023Sim.cfg",
  "InvalidSetup", FatalException,
  "Fiber multi cladding option not well defined");
  WidthBunchFibers = FiberNumberPerLine*(FiberWidth) + (FiberNumberPerLine +1)*FiberSpace;
  ActivationG4FAST = theScint->GetActivationG4FAST();

  //#########################
  // DEFINE GEOMETRY VOLUMES#
  //#########################

  // Create World Volume
  // This is just a big box to place all other logical volumes inside
  G4Box *SolidWorld = new G4Box("SolidWorld", 1100*cm, 1100*cm, 1100*cm );
  LogicalWorld = new G4LogicalVolume(SolidWorld, VacuumWorld,"LogicalWorld",0,0,0);
  LogicalWorld->SetVisAttributes(invis);

  // Place the world volume: center of world at origin (0,0,0)
  PhysicalWorld = new G4PVPlacement(G4Transform3D(DontRotate,G4ThreeVector(0,0,0)),"World",LogicalWorld,NULL,false,0);


  // Create Holder Volume
  // This is just a big box to count the escaped photons
  G4Box *s_holder = new G4Box("s_holder", 1000*cm, 1000*cm, 1000*cm);

  LogicalHolder = new G4LogicalVolume(s_holder,Vacuum,"logical_holder",0,0,0); //Replace Air with Vacuum (init)

  //G4Box *s_holder;
  G4Box *s_fibersholder = new G4Box("s_fibersholder", (WidthBunchFibers/2)*mm, (WidthBunchFibers/2)*mm, FiberLength/2);
  //G4Tubs *s_fibersholder = new G4Tubs("s_fibersholder", 0.0, 0.6*mm, 50*mm, 0, 360*deg);

  LogicalFibersHolder = new G4LogicalVolume(s_fibersholder,Vacuum,"logical_fibersholder",0,0,0); //Replace Air with Vacuum (init)

  //*********************************
  // Build scint et wrapping volumes*
  //*********************** *********
  //Simply calls functions from Scintillator() class
  LogicalPinhole = theScint->GetPinhole();
  LogicalEFPlates = theScint->GetEFPlates();
  LogicalVolumeEFPlates = theScint->GetVolumeEFPlates();
  LogicalMFPlates = theScint->GetMFPlates();
  LogicalVolumeMFPlates = theScint->GetVolumeMFPlates();
  LogicalSc = theScint->GetScTest();
  LogicalLens = theScint->GetLens();

  //  LogicalZnS = theScint->GetZnS();
  if(FiberGeometry ==0)
  {
    LogicalCoreFiber = theScint->GetCoreRoundFiber();
    LogicalInnerCladdingFiber = theScint->GetInnerCladdingRoundFiber();
    LogicalOuterCladdingFiber = theScint->GetOuterCladdingRoundFiber();
    LogicalOuterCladdingFiber->SetVisAttributes(red);
  }

  else if (FiberGeometry ==1)
  {
    LogicalCoreFiber = theScint->GetCoreSquareFiber();
    LogicalInnerCladdingFiber = theScint->GetCladdingSquareFiber();
  }

  else  G4Exception("AIFIRA2023Sim.cfg",
  "InvalidSetup", FatalException,
  "Fiber Geometry !=0 or 1");


  // Set colors of various block materials
  LogicalPinhole->SetVisAttributes(black);
  LogicalEFPlates->SetVisAttributes(red);
  LogicalVolumeEFPlates->SetVisAttributes(gray);
  LogicalMFPlates->SetVisAttributes(blue);
  LogicalVolumeMFPlates->SetVisAttributes(gray);
  LogicalSc->SetVisAttributes(cyan);
  LogicalCoreFiber->SetVisAttributes(cyan);
  LogicalInnerCladdingFiber->SetVisAttributes(yellow);
  //LogicalZnS->SetVisAttributes(green);
  LogicalHolder->SetVisAttributes(invis);
  LogicalFibersHolder->SetVisAttributes(orange);


  // G4Region* RegEM = new G4Region("EMField");
  // //RegEM->AddRootLogicalVolume(LogicalHolder);
  // RegEM->AddRootLogicalVolume(LogicalVolumeEFPlates);
  // RegEM->AddRootLogicalVolume(LogicalVolumeMFPlates);
  //
  G4Region* FiberRegion = new G4Region("Fiber_Region");
  FiberRegion->AddRootLogicalVolume(LogicalFibersHolder);


  G4RegionStore* regionStore = G4RegionStore::GetInstance();
  if(ActivationG4FAST==1) new FastSimModelOpFiber("FastSimModelOpFiber", FiberRegion, FiberMultiCladding);


  //********************************************
  // Build optical properties and skin surfaces*
  //*********************** ********************

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
  //SSTeflon = new G4LogicalSkinSurface("b_Teflon",LogicalTeflon,OpticalTeflon); // Applies optical Teflon properties to ogical Teflon surface


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

  //SSMylar = new G4LogicalSkinSurface("SSMylar", LogicalMylar,OpticalMylar); // Applies Mylar properties to logical mylar surface
  SSAlu = new G4LogicalSkinSurface("SSAlu", LogicalBoitierAlu,OpticalMylar); // Applies Mylar properties to logical Alu surface


  //***********************
  // Build PMT volumes    *
  //***********************

  // Build the PMT glass structure from PMT class
  LogicalPhotocathode = theScint->GetPhotocathode(); // Call function for PMT glass
  //LogicalPhotocathode->SetVisAttributes(blue); // Set photocathode color to orange


  // Define PMT properties
  G4double indexconst = 1.49; // Index currently set to constant for PMT glass
  G4double reflectconst = 0.;
  std::ifstream ReadPMT;
  //G4String PMTfile = path+"QE_ham_GA0124.txt";
  //G4String PMTfile = path+"9102B_ET_reverse.txt";
  G4String PMTfile = path+"ORCA_II_reverse.cfg";
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
      Photocathode_Value.push_back(1);  //For the GAxxxx file with good QE and 0.65 for collection efficiency
      //Photocathode_Value.push_back(quant_eff*1); //Change 1 if you know ou want to apply a collection efficiency factor !!!
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


  //#########################
  // DEFINE EM FIELD PART   #
  //#########################

  G4double fMinStep     = 1*um; // minimal step of 1 mm is default

  //G4MagneticField *magField = new G4UniformMagField(G4ThreeVector(0.0*tesla, 0.*tesla, 0.0*tesla));
  G4MagneticField *localmagField = new G4UniformMagField(G4ThreeVector(0., MF_Value, 0.0));
  //G4Mag_UsualEqRhs* fEquation = new G4Mag_UsualEqRhs(magField);
  G4Mag_UsualEqRhs* fEquationlocal = new G4Mag_UsualEqRhs(localmagField);



  //G4FieldManager* MagFieldManager = G4TransportationManager::GetTransportationManager()->GetFieldManager();
  // fieldMgr->SetDetectorField(magField);
  // fieldMgr->CreateChordFinder(magField);

  // Update field
  //G4MagIntegratorStepper*fStepper;
  G4MagIntegratorStepper*localfStepperMag;
  //localfStepperMag = new G4ClassicalRK4( fEquationlocal ,8);
  localfStepperMag = new G4DormandPrince745( fEquationlocal,8 );
  //localfStepperMag = new G4NystromRK4(fEquationlocal);
  //localfStepperMag = new G4HelixExplicitEuler( fEquationlocal );
  //localfStepperMag = new G4ExactHelixStepper( fEquationlocal );

  //fFieldManager->SetDetectorField(magField);

  G4FieldManager* LocalMagFieldManager = new G4FieldManager(localmagField);

  G4ChordFinder*fLocalChordFinder = new G4ChordFinder( localmagField, fMinStep, localfStepperMag);

  // Set accuracy parameters
  G4double deltaChord        = 0.25*um; //3.0 mm by default
  G4double epsMax            = 1.0e-1;  // Pure number -- maximum relative integration error
  G4double epsMin            = 2.5e-2;  //
  G4double deltaOneStep      = 0.01*um;
  G4double deltaIntersection = 0.1*um;

  //fChordFinder->SetDeltaChord( deltaChord );
  fLocalChordFinder->SetDeltaChord( deltaChord );
  LocalMagFieldManager->SetAccuraciesWithDeltaOneStep(deltaOneStep);
  LocalMagFieldManager->SetMinimumEpsilonStep(epsMin);
  LocalMagFieldManager->SetMaximumEpsilonStep(epsMax);
  LocalMagFieldManager->SetDeltaIntersection(deltaIntersection);
  LocalMagFieldManager->SetDeltaOneStep(0.01 * um);

  //fFieldManager->CreateChordFinder(magField);
  //fFieldManager->SetChordFinder(fChordFinder);
  LocalMagFieldManager->SetChordFinder(fLocalChordFinder);

  LogicalVolumeMFPlates->SetFieldManager(LocalMagFieldManager, true);





  //G4double fMinStep     = 1*mm; // minimal step of 1 mm is default

  //G4ElectricField* fEMfield = new G4UniformElectricField(G4ThreeVector(0.0,0*kilovolt/m,0.0));
  G4ElectricField* LocalfEMfield = new G4UniformElectricField(
    G4ThreeVector(0.0, EF_Value,0.0));

    G4EqMagElectricField* LocalfEquation = new G4EqMagElectricField(LocalfEMfield);

    //G4FieldManager* fFieldManager = G4TransportationManager::GetTransportationManager()->GetFieldManager();

    G4FieldManager* LocalFieldManager = new G4FieldManager(LocalfEMfield);
    G4MagIntegratorStepper* localfStepper = new G4ExplicitEuler( LocalfEquation, 8 );
    G4MagInt_Driver* fIntgrDriver = new G4MagInt_Driver(fMinStep,
      localfStepper,
      localfStepper->GetNumberOfVariables());
      G4ChordFinder* fChordFinder = new G4ChordFinder(fIntgrDriver);
      LocalFieldManager->SetChordFinder(fChordFinder);

      LogicalVolumeEFPlates->SetFieldManager(LocalFieldManager, true);



      //***********************
      // Various Positioning values
      //***********************

      Z_Position_MFPlates = PinholeThickness/2 + Dist_pinhole_MFPlates + MF_Length_plates/2;
      Z_Position_EFPlates = PinholeThickness/2 + Dist_pinhole_MFPlates + MF_Length_plates + Dist_between_plates + EF_Length_plates/2;
      Z_Position_ZnS = PinholeThickness/2 + Dist_pinhole_MFPlates + MF_Length_plates + Dist_between_plates + EF_Length_plates + Dist_EFPlates_Detector + ZnSThickness/2;
      Z_Position_Sc = PinholeThickness/2 + Dist_pinhole_MFPlates + MF_Length_plates + Dist_between_plates + EF_Length_plates + Dist_EFPlates_Detector + ZnSThickness + ScintillatorThickness/2;
      //Z_Position_ZnSLG = Dist_pinhole_MFPlates + MF_Length_plates + Dist_between_plates + EF_Length_plates + Dist_EFPlates_Detector + ZnSThickness + ScintillatorThickness +ZnSLGThickness/2;
      Z_Position_Fiber = PinholeThickness/2 + Dist_pinhole_MFPlates + MF_Length_plates + Dist_between_plates + EF_Length_plates + Dist_EFPlates_Detector + FiberLength/2;
      //Z_Position_Photocathode = PinholeThickness/2 + Dist_pinhole_MFPlates + MF_Length_plates + Dist_between_plates + EF_Length_plates + Dist_EFPlates_Detector + FiberLength + DetectorThickness/2;
      //Z_Position_Photocathode = PinholeThickness/2 + Dist_pinhole_MFPlates + MF_Length_plates + Dist_between_plates + EF_Length_plates + Dist_EFPlates_Detector + ZnSThickness + ScintillatorThickness+ZnSLGThickness+DetectorThickness/2;
      Z_Position_Photocathode = FiberLength/2 + DetectorThickness/2 + DetectorTranslation;


      //############################
      // DEFINE GEOMETRY PLACEMENTS#
      //############################

      #ifndef disable_gdml

      PhysicalHolder = new G4PVPlacement(G4Transform3D(DontRotate,G4ThreeVector(0, 0, 0)),LogicalHolder, "Vacuum", LogicalWorld,false,0);

      //PhysicalFibersHolder = new G4PVPlacement(G4Transform3D(DontRotate,G4ThreeVector(FiberWidth/2 + FiberSpace - WidthBunchFibers/2, 0, 0)),LogicalFibersHolder, "Holder_Fiber",LogicalHolder,false,0);//USE THAT FOR TP
      PhysicalFibersHolder = new G4PVPlacement(G4Transform3D(DontRotate,G4ThreeVector(0, 0, 0)),LogicalFibersHolder, "Holder_Fiber",LogicalHolder,false,0);//USE THAT FOR DEBUG


      // PhysicalPinhole = new G4PVPlacement(G4Transform3D
      //   (DontRotate,G4ThreeVector(0*mm, 0*mm, 0*mm)), // Set at origin as basis of everything else
      //   LogicalPinhole,"Pinhole",
      //   LogicalHolder,false,0);
      //
      //   PhysicalVolumeMFPlates = new G4PVPlacement(G4Transform3D
      //     (DontRotate,G4ThreeVector(0*mm, translation_pinhole, Z_Position_MFPlates)), // Set at origin as basis of everything else
      //     LogicalVolumeMFPlates,"Volume_MF_Plates",
      //     LogicalHolder,false,0);
      //
      //     PhysicalMFPlates = new G4PVPlacement(G4Transform3D
      //       (DontRotate,G4ThreeVector(0*mm, 0, 0)), // Set at origin as basis of everything else
      //       LogicalMFPlates,"MF_Plates",
      //       LogicalVolumeMFPlates,false,0);
      //
      //       PhysicalVolumeEFPlates = new G4PVPlacement(G4Transform3D
      //         (DontRotate,G4ThreeVector(0*mm,translation_pinhole, Z_Position_EFPlates)), // Set at origin as basis of everything else
      //         LogicalVolumeEFPlates,"Volume_EF_Plates",
      //         LogicalHolder,false,0);
      //
      //         PhysicalEFPlates = new G4PVPlacement(G4Transform3D
      //           (DontRotate,G4ThreeVector(0*mm, 0, 0)), // Set at origin as basis of everything else
      //           LogicalEFPlates,"EF_Plates",
      //           LogicalVolumeEFPlates,false,0);


                // PhysicalZnS = new G4PVPlacement(G4Transform3D
                //   (DontRotate,G4ThreeVector(0*mm, translation_pinhole, Z_Position_ZnS)), // Set at origin as basis of everything else
                //   LogicalZnS,"ZnS",
                //   LogicalHolder,false,0);
                //
                // PhysicalSc = new G4PVPlacement(G4Transform3D
                //   (DontRotate,G4ThreeVector(0*mm, translation_pinhole, Z_Position_Sc)), // Set at origin as basis of everything else
                //   LogicalSc,"Scintillator",
                //   LogicalHolder,false,0);

                // PhysicalZnSLG = new G4PVPlacement(G4Transform3D
                //   (DontRotate,G4ThreeVector(0*mm, translation_pinhole, Z_Position_ZnSLG)), // Set at origin as basis of everything else
                //   LogicalZnSLG,"PMMA",
                //   LogicalHolder,false,0);

G4int n=0;
G4float translation_x=0;
G4float translation_y =0;

for (int i=0; i<FiberNumberPerLine; i++)
{
  for (int j=0; j<FiberNumberPerLine; j++)
  {
    translation_x = (FiberSpace+FiberWidth/2-WidthBunchFibers/2)+j*(FiberWidth + FiberSpace);
    translation_y = (FiberSpace+FiberWidth/2-WidthBunchFibers/2)+i*(FiberWidth + FiberSpace);

    PhysicalCoreFiberBunch[n] = new G4PVPlacement(G4Transform3D
     (DontRotate,G4ThreeVector(translation_x, translation_y, 0*mm)), // Set at origin as basis of everything else
     LogicalCoreFiber,"Core_Fiber",
     LogicalFibersHolder,false,0);

     PhysicalInnerCladdingFiberBunch[n] = new G4PVPlacement(G4Transform3D
      (DontRotate,G4ThreeVector(translation_x, translation_y, 0*mm)), // Set at origin as basis of everything else
      LogicalInnerCladdingFiber,"Inner_Cladding_Fiber",
      LogicalFibersHolder,false,0);

      if(FiberMultiCladding  == 1)
      {
        PhysicalOuterCladdingFiberBunch[n] = new G4PVPlacement(G4Transform3D
         (DontRotate,G4ThreeVector(translation_x, translation_y, 0*mm)), // Set at origin as basis of everything else
         LogicalOuterCladdingFiber,"Outer_Cladding_Fiber",
         LogicalFibersHolder,false,0);
      }

     n++;
  }
}
                // if(FiberGeometry ==0)
                // {
                //   if (FiberMultiCladding ==0)
                //   {
                //     G4VPVParameterisation* InnerCladdingFiberParam =  new FiberParameterisation(
                //       FiberNumberPerLine,   // NoFibers
                //       (-WidthBunchFibers+FiberWidth+2*FiberSpace)/2,  // Z of center of first
                //       FiberSpacing, // Z spacing of centers
                //       FiberWidth,  // Fiber radius/width for round/square fiber
                //       FiberLength);    // final length
                //
                //       new G4PVParameterised("Inner_Cladding_Fiber",       // their name
                //       LogicalInnerCladdingFiber,   // their logical volume
                //       LogicalFibersHolder,       // Mother logical volume
                //       kUndefined,          // Are placed along this axis
                //       NbOfFibers,    // Number of Fibers
                //       InnerCladdingFiberParam,    // The parametrisation
                //       false); // checking overlaps
                //
                //       G4VPVParameterisation* FiberParam =  new FiberParameterisation(
                //         FiberNumberPerLine,   // NoFibers
                //         0,  // Z of center of first
                //         1, // Z spacing of centers -> Not useful HERE
                //         FiberWidthCore,  // Fiber radius/width for round/square fiber
                //         FiberLength);    // final length
                //
                //         new G4PVParameterised("Core_Fiber",       // their name
                //         LogicalCoreFiber,   // their logical volume
                //         LogicalInnerCladdingFiber,       // Mother logical volume
                //         kUndefined,          // Are placed along this axis
                //         1,    // Number of Fibers
                //         FiberParam,    // The parametrisation
                //         false); // checking overlaps
                //       }
                //
                //
                //       if (FiberMultiCladding ==1)
                //       {
                //         G4VPVParameterisation* OuterCladdingFiberParam =  new FiberParameterisation(
                //           FiberNumberPerLine,   // NoFibers
                //           (-WidthBunchFibers+FiberWidth+2*FiberSpace)/2,  // Z of center of first
                //           FiberSpacing, // Z spacing of centers
                //           FiberWidth,  // Fiber radius/width for round/square fiber
                //           FiberLength);    // final length
                //
                //           new G4PVParameterised("Outer_Cladding_Fiber",       // their name
                //           LogicalOuterCladdingFiber,   // their logical volume
                //           LogicalFibersHolder,       // Mother logical volume
                //           kUndefined,          // Are placed along this axis
                //           NbOfFibers,    // Number of Fibers
                //           OuterCladdingFiberParam,    // The parametrisation
                //           false); // checking overlaps
                //
                //
                //           G4VPVParameterisation* InnerCladdingFiberParam =  new FiberParameterisation(
                //             FiberNumberPerLine,   // NoFibers
                //             0,  // Z of center of first
                //             1, // Z spacing of centers
                //             FiberWidthCladding,  // Fiber radius/width for round/square fiber
                //             FiberLength);    // final length
                //
                //             new G4PVParameterised("Inner_Cladding_Fiber",       // their name
                //             LogicalInnerCladdingFiber,   // their logical volume
                //             LogicalOuterCladdingFiber,       // Mother logical volume
                //             kUndefined,          // Are placed along this axis
                //             1,    // Number of Fibers
                //             InnerCladdingFiberParam,    // The parametrisation
                //             false); // checking overlaps
                //
                //
                //             G4VPVParameterisation* FiberParam =  new FiberParameterisation(
                //               FiberNumberPerLine,   // NoFibers
                //               0,  // Z of center of first
                //               1, // Z spacing of centers -> Not useful HERE
                //               FiberWidthCore,  // Fiber radius/width for round/square fiber
                //               FiberLength);    // final length
                //
                //               new G4PVParameterised("Core_Fiber",       // their name
                //               LogicalCoreFiber,   // their logical volume
                //               LogicalInnerCladdingFiber,       // Mother logical volume
                //               kUndefined,          // Are placed along this axis
                //               1,    // Number of Fibers
                //               FiberParam,    // The parametrisation
                //               false); // checking overlaps
                //             }
                //           }
                //
                //           if(FiberGeometry ==1)
                //           {
                //             G4VPVParameterisation* InnerCladdingFiberParam =  new FiberParameterisation(
                //               FiberNumberPerLine,   // NoFibers
                //               (-WidthBunchFibers+FiberWidth)/2,  // Z of center of first
                //               FiberSpacing, // Z spacing of centers
                //               FiberWidth,  // Fiber radius/width for round/square fiber
                //               FiberLength);    // final length
                //
                //               new G4PVParameterised("Inner_Cladding_Fiber",       // their name
                //               LogicalInnerCladdingFiber,   // their logical volume
                //               LogicalFibersHolder,       // Mother logical volume
                //               kUndefined,          // Are placed along this axis
                //               NbOfFibers,    // Number of Fibers
                //               InnerCladdingFiberParam,    // The parametrisation
                //               false); // checking overlaps
                //
                //
                //               G4VPVParameterisation* FiberParam =  new FiberParameterisation(
                //                 FiberNumberPerLine,   // NoFibers
                //                 0,  // Z of center of first
                //                 1, // Z spacing of centers -> Not useful HERE
                //                 FiberWidthCore,  // Fiber radius/width for round/square fiber
                //                 FiberLength);    // final length
                //
                //                 new G4PVParameterised("Core_Fiber",       // their name
                //                 LogicalCoreFiber,   // their logical volume
                //                 LogicalInnerCladdingFiber,       // Mother logical volume
                //                 kUndefined,          // Are placed along this axis
                //                 1,    // Number of Fibers
                //                 FiberParam,    // The parametrisation
                //                 false); // checking overlaps
                //               }


                              // PMT photocathode placement
                              PhysicalPhotocathode = new G4PVPlacement(G4Transform3D
                                //(DontRotate,G4ThreeVector(FiberWidth/2 + FiberSpace - WidthBunchFibers/2, 0, Z_Position_Photocathode)), //USE THAT FOR TP
                                (DontRotate,G4ThreeVector(0, 0, Z_Position_Photocathode)), //USE THAT FOR DEBUG
                                LogicalPhotocathode,"CMOS",
                                LogicalHolder,true,0);

                                #else

                                #endif





                                // Returns world with everything in it and all properties set
                                return PhysicalWorld;
                              }
