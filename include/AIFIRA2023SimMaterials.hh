/// AIFIRA2023SimMaterials.hh
//// Auteur: Arnaud HUBER for ENL group <huber@cenbg.in2p3.fr>
//// Copyright: 2022 (C) Projet RATP - ENL [LP2IB] - CELIA

// This class is used to create a plastic scintillator.

#ifndef AIFIRA2023SimMaterials_h
#define AIFIRA2023SimMaterials_h


#include "G4MaterialPropertiesTable.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4OpticalSurface.hh"
#include "CLHEP/Units/SystemOfUnits.h"

class G4Material;

class AIFIRA2023SimMaterials
{
public:

  AIFIRA2023SimMaterials(G4String prop_buildfile);
  ~AIFIRA2023SimMaterials();
  void Construct();

public:
  G4Material *GetMaterial(G4String);


private:

  static const G4String path;

  G4Element* elementH;
  G4Element* elementC;
  G4Element* elementO;
  G4Element* elementF;
  G4Element* elementTi;
  G4Element* elementSi;
  G4Element* elementB;
  G4Element* elementNa;
  G4Element* elementAl;
  G4Element* elementN;
  G4Element* elementFe;
  G4Element* elementNi;
  G4Element* elementCr;
  G4Element* elementMn;
  G4Element* elementCu;
  G4Element* elementSn;
  G4Element* elementSb;
  G4Element* elementMg;
  G4Element* elementZn;
  G4Element* elementCl;
  G4Element* elementK;
  G4Element* elementPb;
  G4Element* elementLa;
  G4Element* elementBr;
  G4Element* elementS;
  G4Element* elementLu;
  G4Element* elementY;
  G4Element* elementCs;
  G4Element* elementI;
  G4Element* elementGd;
  G4Element* elementGa;

  G4Material *SiO2;
  G4Material *Silicium;
  G4Material *B2O3;
  G4Material *Na2O;
  G4Material *Al2O3;
  G4Material *Air;
  G4Material *Fer;
  G4Material *Inox;
  G4Material *Cuivre;
  G4Material *Carbon;
  G4Material *Kapton;
  G4Material *Acier;
  G4Material *N2;
  G4Material *H2O;
  G4Material *Polypropylene;
  G4Material *Compensation;
  G4Material *Papier;
  G4Material *Nylon;
  G4Material *teflon;
  G4Material *mousse;
  G4Material *Neoprene;
  G4Material *Mica;
  G4Material *Plomb_Antimoine;
  G4Material *LaBr3;
  G4Material *bs_glass;
  G4Material *polysty;
  G4Material *Ti02;
  G4Material *coating;
  G4Material *scintillator;
  G4Material *Vacuum;
  G4Material *VacuumWorld;
  G4Material *plastic;
  G4Material *mylar;
  G4Material *grease;
  G4Material *cargille;
  G4Material *PMMA;
  G4Material *FP;
  G4Material *Alu;
  G4Material *RTV;
  G4Material *ZnS;
  G4Material *NoWaSH;
  G4Material *lens_glass;
  G4Material *EJ214;
  G4Material *EJ212;
  G4Material *EJ260;
  G4Material *EJ262;
  G4Material *LYSO;
  G4Material *CsITl;
  G4Material *LuAG;
  G4Material *GAGG;
  G4Material *YAG;

  G4MaterialPropertiesTable *bs_glassMPT;
  G4MaterialPropertiesTable *vacMPT;
  G4MaterialPropertiesTable *scintMPT;
  G4MaterialPropertiesTable *wlsMPT;
  G4MaterialPropertiesTable *plasticMPT;
  G4MaterialPropertiesTable *greaseMPT;
  G4MaterialPropertiesTable *cargilleMPT;
  G4MaterialPropertiesTable *LaBr3MPT;
  G4MaterialPropertiesTable *PMMAMPT;
  G4MaterialPropertiesTable *FPMPT;
  G4MaterialPropertiesTable *ZnSMPT;
  G4MaterialPropertiesTable *NoWaSHMPT;
  G4MaterialPropertiesTable *lens_glassMPT;
  G4MaterialPropertiesTable *EJ212MPT;
  G4MaterialPropertiesTable *EJ214MPT;
  G4MaterialPropertiesTable *EJ260MPT;
  G4MaterialPropertiesTable *EJ262MPT;
  G4MaterialPropertiesTable *LYSOMPT;
  G4MaterialPropertiesTable *CsITlMPT;
  G4MaterialPropertiesTable *LuAGMPT;
  G4MaterialPropertiesTable *YAGMPT;
  G4MaterialPropertiesTable *GAGGMPT;

  G4OpticalSurface* PMMAOpSurface;


  G4double scintAbsorbconst;
  G4double scintIndexconst;
  G4double lightyield;
  G4double lightyieldZnS;
  G4double pWavelength;
  G4double ratio;
  G4float Air_Index;


};
#endif
