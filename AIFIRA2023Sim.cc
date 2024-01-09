/// AIFIRA2023Sim.cc
/// Auteur: Arnaud HUBER for ENL group <huber@cenbg.in2p3.fr>
/// Copyright: 2024 (C) Projet RATP - ENL [LP2IB] - CELIA


#include "G4LogicalSkinSurface.hh"
#include "G4OpticalSurface.hh"
#include "G4ThreeVector.hh"
#include "G4MaterialPropertiesTable.hh"
#include "Randomize.hh"
#include "time.h"
#include "G4Timer.hh"
#include "G4UIterminal.hh"
#include "AIFIRA2023SimSteppingAction.hh"
#include "G4RunManager.hh"
#include "G4UImanager.hh"
//#include "G4UIGAG.hh"
#include "AIFIRA2023SimGeometry.hh"
#include "AIFIRA2023SimPhysics.hh"
#include "AIFIRA2023SimPrimaryGeneratorAction.hh"
#include "AIFIRA2023SimRunAction.hh"
#include "AIFIRA2023SimEventAction.hh"
#include "AIFIRA2023SimTrackingAction.hh"
//
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "G4SteppingVerbose.hh"


// #ifdef G4VIS_USE
// #include "AIFIRA2023SimVisManager.hh"
 //#endif

int main(int argc,char** argv){


  char* suff = argv[1];

  //Use SteppingVerbose with Unit
//G4int precision = 4;
//G4SteppingVerbose::UseBestUnit(precision);


  // Construct the default run manager
  G4RunManager* runManager = new G4RunManager;


  // set mandatory initialization classes
  AIFIRA2023SimGeometry* OptGeom = new AIFIRA2023SimGeometry;

  G4cout<<"Geometry given to AIFIRA2023Sim.cc"<<G4endl;

  // initialize the geometry
  runManager->SetUserInitialization(OptGeom);
  G4cout<<"Geometry set in AIFIRA2023Sim.cc given to Runman"<<G4endl;

  // initialize the physics
  runManager->SetUserInitialization(new AIFIRA2023SimPhysics);

// #ifdef G4VIS_USE
//   // visualization manager
//   G4VisManager* visManager = new AIFIRA2023SimVisManager;
//   visManager->Initialize();
// #endif

  // set mandatory user action class
  runManager->SetUserAction(new AIFIRA2023SimPrimaryGeneratorAction);


  // set Run Event and Stepping action classes
  runManager->SetUserAction(new AIFIRA2023SimRunAction(suff));
  G4cout<<"Initialized new Run Action"<<G4endl;

  runManager->SetUserAction(new AIFIRA2023SimEventAction(suff));
  G4cout<<"Initialized new EventAction"<<G4endl;
  runManager->SetUserAction(new AIFIRA2023SimSteppingAction);
  G4cout<<"Initialized new SteppingAction"<<G4endl;
  runManager->SetUserAction(new AIFIRA2023SimTrackingAction);
  G4cout<<"Initialized new Tracking Action"<<G4endl;


//#ifdef G4VIS_USE
      G4VisManager* visManager = new G4VisExecutive;
      visManager->Initialize();
//#endif

  // Initialize G4 kernel
  runManager->Initialize();


  G4cout<<"Initialized new Run Manager"<<G4endl;

  // get the pointer to the User Interface manager
  G4UImanager* UI = G4UImanager::GetUIpointer();
  char movefile[100];


  G4UIExecutive *ui = 0;

  if (argc==4)   // batch mode
    {
      G4cout << "Batch MODE" << G4endl;
      G4String command = "/control/execute ";
      G4String fileName = argv[3];
      UI->ApplyCommand(command+fileName);
      UI->ApplyCommand("control/suppressAbortion");

      char startcommand[100];
      sprintf(startcommand,"/run/beamOn %s",argv[2]);
      UI->ApplyCommand(startcommand);
      // G4cout << "3" << G4endl;
      // sprintf(writefile,"/control/shell mv %s.root ../Resultats/",argv[2]);
      // UI->ApplyCommand(writefile);

      sprintf(movefile,"/control/shell mv %s.root ../Resultats", argv[1]);
      UI->ApplyCommand(movefile);
      G4cout << "Output saved to file " << argv[1] << ".root" << G4endl;

    }

  else           //define visualization and UI terminal for interactive mode
    {

      G4cout << "Interactive MODE" << G4endl;

//#ifdef G4UI_USE
      ui = new G4UIExecutive(argc,argv);
      UI->ApplyCommand("/control/execute vis.mac");
      ui->SessionStart();
      delete ui;
//#endif

      sprintf(movefile,"/control/shell mv %s.root ../Resultats", argv[1]);
      UI->ApplyCommand(movefile);
      G4cout << "Output saved to file " << argv[1] << ".root" << G4endl;
    }

#ifdef G4VIS_USE
      delete visManager;
#endif


  /*
  //execute visualization macro
  UI->ApplyCommand("/control/execute vis.mac");
  UI->ApplyCommand("/control/execute vrml.mac");

  UI->ApplyCommand("/control/suppressAbortion");
  G4UIsession *session = new G4UIterminal();
  if(argc == 1)
    //starts G4Terminal
    session->SessionStart();

  else if(argc == 3){
    //automatically executes the specified number of runs and saves to output
    char writefile[100],startcommand[100], movefile[100];
    sprintf(startcommand,"/run/beamOn %s",argv[1]);
      UI->ApplyCommand(startcommand);
      sprintf(writefile,"/control/shell mv %s.root ../Resultats/",argv[2]);
      UI->ApplyCommand(writefile);
      //sprintf(movefile,"/control/shell mv %s.root ../Resultats", argv[2]);
      //UI->ApplyCommand(movefile);
      G4cout << "Output saved to file " << argv[2] << ".root" << G4endl;
  }

  delete session;
  */


  delete runManager;
  return 0;
}
