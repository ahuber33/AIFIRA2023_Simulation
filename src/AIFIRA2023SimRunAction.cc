/// AIFIRA2023SimRunAction.cc
//// Auteur: Arnaud HUBER for ENL group <huber@cenbg.in2p3.fr>
//// Copyright: 2022 (C) Projet RATP - ENL [LP2IB] - CELIA

#include "AIFIRA2023SimRunAction.hh"
#include "Randomize.hh"
#include <time.h>
#include "G4Run.hh"
#include "G4UImanager.hh"
#include "G4VVisManager.hh"
#include "G4ios.hh"
#include "TFile.h"
#include "TBranch.h"
#include "TTree.h"
#include "TH2I.h"
#include "TH1D.h"
#include "TAxis.h"
#include <fstream>
#include <iostream>

AIFIRA2023SimRunAction::AIFIRA2023SimRunAction(char* suff):suffixe(suff){}
AIFIRA2023SimRunAction::~AIFIRA2023SimRunAction(){}


//-----------------------------------------------------
//  BeginOfRunAction:  used to calculate the start time and
//  to set up information in the run tree.
//-----------------------------------------------------
void AIFIRA2023SimRunAction::BeginOfRunAction(const G4Run* aRun){

  G4String fileName = suffixe+".root";

  start = time(NULL);     //start the timer clock to calculate run times

  Tree_Optical = new TTree("Optical","Optical_Information");
  Tree_TP = new TTree("TP","TP Information");  //Tree to access position of energy deposition


  //*****************************INFORMATION FROM OPTICAL PART************************************
  RunBranch = Tree_Optical->Branch("IncidentE", &StatsOptical.IncidentE, "IncidentE/F");
  //RunBranch = Tree_Optical->Branch("DepositZnS", &StatsOptical.DepositZnS, "DepositZnS/F");
  RunBranch = Tree_Optical->Branch("DepositSc", &StatsOptical.DepositSc, "DepositSc/F");
  // RunBranch = Tree_Optical->Branch("ScintillationZnS", &StatsOptical.ScintillationZnS, "ScintillationZnS/I");
  // RunBranch = Tree_Optical->Branch("CerenkovZnS", &StatsOptical.CerenkovZnS, "CerenkovZnS/I");
  RunBranch = Tree_Optical->Branch("ScintillationSc", &StatsOptical.ScintillationSc, "ScintillationSc/I");
  // RunBranch = Tree_Optical->Branch("CerenkovSc", &StatsOptical.CerenkovSc, "CerenkovSc/I");
  // RunBranch = Tree_Optical->Branch("BulkAbsZnS", &StatsOptical.BulkAbsZnS, "BulkAbsZnS/I");
   RunBranch = Tree_Optical->Branch("BulkAbsSc", &StatsOptical.BulkAbsSc, "BulkAbsSc/I");
  // RunBranch = Tree_Optical->Branch("Absorbed", &StatsOptical.Absorbed, "Absorbed/I");
   RunBranch = Tree_Optical->Branch("Escaped", &StatsOptical.Escaped, "Escaped/I");
   RunBranch = Tree_Optical->Branch("Failed", &StatsOptical.Failed, "Failed/I");
  //RunBranch = Tree_Optical->Branch("WLS", &StatsOptical.WLS, "WLS/I");
  RunBranch = Tree_Optical->Branch("Detected", &StatsOptical.Detected, "Detected/I");
  RunBranch = Tree_Optical->Branch("DetectorPositionX", "vector<float>", &StatsOptical.DetectorPositionX);
  RunBranch = Tree_Optical->Branch("DetectorPositionY", "vector<float>", &StatsOptical.DetectorPositionY);
  //RunBranch = Tree_Optical->Branch("PositionZ", "vector<float>", &StatsOptical.PositionZ);
  RunBranch = Tree_Optical->Branch("MomentumX", "vector<float>", &StatsOptical.MomentumX);
  RunBranch = Tree_Optical->Branch("MomentumY", "vector<float>", &StatsOptical.MomentumY);
  RunBranch = Tree_Optical->Branch("MomentumZ", "vector<float>", &StatsOptical.MomentumZ);
  //RunBranch = Tree_Optical->Branch("BirthLambda", "vector<float>", &StatsOptical.BirthLambda);
  //RunBranch = Tree_Optical->Branch("Time", "vector<float>", &StatsOptical.Time);
  //RunBranch = Tree_Optical->Branch("Energy_pe", "vector<float>", &StatsOptical.Energy_pe);
  // RunBranch = Tree_Optical->Branch("Rayleigh", "vector<float>", &StatsOptical.Rayleigh);
  // RunBranch = Tree_Optical->Branch("Total_Reflections", "vector<float>", &StatsOptical.Total_Reflections);
  // RunBranch = Tree_Optical->Branch("Wrap_Reflections", "vector<float>", &StatsOptical.Wrap_Reflections);
  //RunBranch = Tree_Optical->Branch("TotalLength", "vector<float>", &StatsOptical.TotalLength);
  RunBranch = Tree_Optical->Branch("Angle_creation", "vector<float>", &StatsOptical.Angle_creation);
  RunBranch = Tree_Optical->Branch("Angle_detection", "vector<float>", &StatsOptical.Angle_detection);
  //RunBranch = Tree_Optical->Branch("Final_state_photon", "vector<int>", &StatsOptical.FinalState);


  //*****************************INFORMATION FROM THE TP**************************************
  //RunBranch = Tree_TP->Branch("ParticuleID", &StatsTP.ParticuleID, "ParticuleID/I" );
  RunBranch = Tree_TP->Branch("E_start", &StatsTP.E_start, "E_start/F" );
  RunBranch = Tree_TP->Branch("E_dep", &StatsTP.E_dep, "E_dep/F" );
  //RunBranch = Tree_TP->Branch("Charge", &StatsTP.Charge, "Charge/F" );
  RunBranch = Tree_TP->Branch("PositionX", &StatsTP.PositionX, "PositionX/F" );
  RunBranch = Tree_TP->Branch("PositionY", &StatsTP.PositionY, "PositionY/F" );
  RunBranch = Tree_TP->Branch("PositionZ", &StatsTP.PositionZ, "PositionZ/F" );
  //RunBranch = Tree_TP->Branch("Time", &StatsTP.Time, "Time/F" );
  //RunBranch = Tree_TP->Branch("TotalLength", &StatsTP.TotalLength, "TotalLength/F" );
  //RunBranch = Tree_TP->Branch("InteractionDepth", &StatsTP.InteractionDepth, "InteractionDepth/F" );



  //set the random seed to the CPU clock
  //G4Random::setTheEngine(new CLHEP::HepJamesRandom);
  G4long seed = time(NULL);
  G4Random::setTheSeed(seed);
  //G4Random::setTheSeed(1655805950);
  G4cout << "seed = " << seed << G4endl;

  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;

  if (G4VVisManager::GetConcreteInstance()){
    G4UImanager* UI = G4UImanager::GetUIpointer();
    UI->ApplyCommand("/vis/scene/notifyHandlers");
  }


}  //end BeginOfRunAction


//-----------------------------------------------------
//  EndOfRunAction:  used to calculate the end time and
//  to write information to the run tree.
//-----------------------------------------------------
void AIFIRA2023SimRunAction::EndOfRunAction(const G4Run*aRun){

  //update the temp root file
  G4String fileName = suffixe+".root";
  f = new TFile(fileName,"update");
  Tree_Optical->Write();
  Tree_TP->Write();
  f->Close();

  if (G4VVisManager::GetConcreteInstance()){
    G4UImanager::GetUIpointer()->ApplyCommand("/vis/viewer/update");
  }

  //display run time and write to file Rntime.out
  time_t end = time(NULL);
  G4int elapsed = end-start;
  G4cout << "Run Completed in " << elapsed/3600
  << ":" << (elapsed%3600)/60 << ":"
  << ((elapsed%3600)%60) << G4endl;

  // Output the time in the file Runtime.out
  std::ofstream timeout;
  timeout.open("Runtime.out",std::ios::app);
  timeout << "Run " << aRun->GetRunID()
  << ": " <<elapsed/3600
  << ":" <<(elapsed%3600)/60
  << ":" <<((elapsed%3600)%60) << G4endl;
  timeout.close();

  G4cout<<"Leaving Run Action"<<G4endl;
}

//---------------------------------------------------------
//  For each event update the statistics in the Run tree
//---------------------------------------------------------

void AIFIRA2023SimRunAction::UpdateStatisticsOptical(RunTallyOptical aRunTallyOptical){
  StatsOptical = aRunTallyOptical;
  Tree_Optical->Fill();
}

void AIFIRA2023SimRunAction::UpdateStatisticsTP(RunTallyTP aRunTallyTP){
  StatsTP = aRunTallyTP;
  Tree_TP->Fill();
}
