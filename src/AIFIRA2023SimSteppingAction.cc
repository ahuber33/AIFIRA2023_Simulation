/// AIFIRA2023SimSteppingAction.cc
//// Auteur: Arnaud HUBER for ENL group <huber@cenbg.in2p3.fr>
//// Copyright: 2022 (C) Projet RATP - ENL [LP2IB] - CELIA

#include "AIFIRA2023SimSteppingAction.hh"
#include "G4DynamicParticle.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4ProcessManager.hh"
#include "G4Track.hh"
#include "G4ParticleDefinition.hh"
#include "G4OpticalPhoton.hh"
#include "AIFIRA2023SimRunAction.hh"
#include "G4RunManager.hh"
#include "G4EventManager.hh"
#include "AIFIRA2023SimEventAction.hh"
#include <iostream>
#include <fstream>
#include "G4Material.hh"
#include "G4Trajectory.hh"
#include "AIFIRA2023SimTrackInformation.hh"
#include "AIFIRA2023SimGeometry.hh"
#include "G4StepPoint.hh"
#include "TRandom3.h"
#include "TF1.h"
#include "G4EmConfigurator.hh"
#include "G4IonFluctuations.hh"
#include "G4UniversalFluctuation.hh"
#include "G4ionIonisation.hh"
#include "G4IonParametrisedLossModel.hh"
#include "G4LossTableManager.hh"
#include "G4BraggIonGasModel.hh"
#include "G4BetheBlochIonGasModel.hh"

#include "G4ParallelWorldProcess.hh"
#include "G4TransportationManager.hh"



using namespace CLHEP;

const G4String AIFIRA2023SimSteppingAction::path = "../simulation_input_files/";

AIFIRA2023SimSteppingAction::AIFIRA2023SimSteppingAction()
{}

  AIFIRA2023SimSteppingAction::~AIFIRA2023SimSteppingAction(){}
  void AIFIRA2023SimSteppingAction::UserSteppingAction(const G4Step *aStep){




    //###################################
    // Déclaration of functions/variables
    //###################################
    G4Track* theTrack = aStep->GetTrack();
    AIFIRA2023SimTrackInformation *trackInformation = (AIFIRA2023SimTrackInformation*)theTrack->GetUserInformation();
    G4String partname = aStep->GetTrack()->GetDefinition()->GetParticleName();
    G4int partID = aStep->GetTrack()->GetDefinition()->GetPDGEncoding();
    AIFIRA2023SimRunAction *runac = (AIFIRA2023SimRunAction*)(G4RunManager::GetRunManager()->GetUserRunAction());
    G4EventManager *evtman = G4EventManager::GetEventManager();
    AIFIRA2023SimEventAction *evtac = (AIFIRA2023SimEventAction*)evtman->GetUserEventAction();
    AIFIRA2023SimTrackInformation* info = (AIFIRA2023SimTrackInformation*)(aStep->GetTrack()->GetUserInformation());
    G4String endproc = aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
    G4int Parent_ID = aStep->GetTrack()->GetParentID();
    G4int StepNo = aStep->GetTrack()->GetCurrentStepNumber();

    G4double x = aStep->GetTrack()->GetPosition().x();
    G4double y = aStep->GetTrack()->GetPosition().y();
    G4double z = aStep->GetTrack()->GetPosition().z();
    G4double zpre = aStep->GetPreStepPoint()->GetPosition().z();
    G4double px = aStep->GetPreStepPoint()->GetMomentumDirection().x();
    G4double py = aStep->GetPreStepPoint()->GetMomentumDirection().y();
    G4double pz = aStep->GetPreStepPoint()->GetMomentumDirection().z();
    G4double r = sqrt(x*x + y*y);
    G4double angle = acos((z-zpre)/aStep->GetStepLength());
    G4String VolumeNamePreStep = aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName();
    G4String VolumeNamePostStep = aStep->GetPostStepPoint()->GetPhysicalVolume()->GetName();
    //G4int CopyNo = theTrack->GetTouchableHandle()->GetCopyNumber();


    G4double time = aStep->GetPostStepPoint()->GetGlobalTime()/ns;
    G4ThreeVector polarisation = theTrack->GetPolarization();
    //G4double my_dist_after = aStep->GetTrack()->GetTrackLength()/mm;

    //#######################################################################
    //#######################################################################
    //##########################START OPTICAL PART###########################
    //#######################################################################
    //#######################################################################
    //G4cout << "x = " << x << G4endl;
    //G4cout << "y = " << y << G4endl;
    //G4cout << "z = " << z << G4endl;
    //G4cout << "z pre = " << zpre << G4endl;
    // G4cout << "px = " << px << G4endl;
    // G4cout << "py = " << py << G4endl;
    // G4cout << "pz = " << pz << G4endl;
    // G4cout << "polarisation = " << polarisation << G4endl;
    //G4cout << "angle = " << angle/deg << G4endl;
    // G4cout << "Time = " << time << " ns" << G4endl;

    if(partname=="opticalphoton" && VolumeNamePostStep == "Lens")
    {
      evtac->FillPhotonLensPositionX(x);
      evtac->FillPhotonLensPositionY(y);
      // G4cout << "Position X Lens = " << x << G4endl;
      // G4cout << "Position Y Lens = " << y << G4endl;
    }


    if(partname=="opticalphoton" && VolumeNamePostStep == "Vacuum" && z >0)
    {
      if(VolumeNamePreStep == "Core_Fiber" || VolumeNamePreStep == "Inner_Cladding_Fiber" || VolumeNamePreStep == "Outer_Cladding_Fiber" || VolumeNamePreStep == "Holder_Fiber")
      {
        evtac->FillPhotonExitLightPositionX(x);
        evtac->FillPhotonExitLightPositionY(y);
        // G4cout << "Position X Exit Light = " << x << G4endl;
        // G4cout << "Position Y Exit Light = " << y << G4endl;
      }
    }


    if((partname=="opticalphoton" && aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "Holder_Fiber")
    && (evtac->GetAirIndex()<1))
    {
      G4MaterialPropertyVector* rindex= theTrack->GetVolume()->GetLogicalVolume()->GetMaterial()->GetMaterialPropertiesTable()->GetProperty("RINDEX");
      G4float Index = (*rindex)[0];
      //G4cout << "HOLDER Fiber " << G4endl;
      //G4cout << "TEST = " <<  Index << G4endl;
      evtac->SetAirIndex(Index);
      //G4cout << "Test 2 =" << evtac->GetAirIndex() << G4endl;
    }

    if(StepNo==1 && partname == "opticalphoton")
    {
      //G4cout << "px = " << px << G4endl;
      //G4cout << "py = " << py << G4endl;
      //G4cout << "pz = " << pz << G4endl;
      //G4cout << "ANGLE = " << angle/deg << G4endl;
      //evtac->FillFiberAngleCreation(angle/deg);
      evtac->SetPhotonCreationAngle(angle/deg);
      evtac->SetTrackLengthFastSimulated(0);
      //if(angle/deg>20.4 && angle/deg <20.7)G4cout << "HERE" << G4endl;
      if (angle/deg >20 && angle/deg <160) theTrack->SetTrackStatus(fStopAndKill);
    }

    if(0){                       //set to 1 to ignore generated photons
      if(theTrack->GetDefinition()->GetParticleName()=="opticalphoton")
      theTrack->SetTrackStatus(fStopAndKill);
    }

    //The following lines are for debugging purposes
    if(partname == "opticalphoton" && aStep->GetTrack()->GetUserInformation() ==0) G4cout << "WARNING! No user info attached to photon" << G4endl;

    G4OpBoundaryProcessStatus boundaryStatus=Undefined;
    static G4OpBoundaryProcess* boundary=NULL;

    //find the boundary process only once
    if(!boundary)
    {
      G4ProcessManager* pm = aStep->GetTrack()->GetDefinition()->GetProcessManager();
      G4int nprocesses = pm->GetProcessListLength();
      G4ProcessVector* pv = pm->GetProcessList();
      G4int i;
      for( i = 0; i < nprocesses; i++){
        if((*pv)[i]->GetProcessName()=="OpBoundary")
        {
          boundary = (G4OpBoundaryProcess*)(*pv)[i];
          break;
        }
      }
    }


    if(partname =="opticalphoton")
    { //ALL the code in relation with Optical needs to be here !!!!
      boundaryStatus = boundary->GetStatus();
      //G4cout << "BirthLambda = " << info->GetBirthLambda() << G4endl;
      //G4cout << "Time =" << aStep->GetPostStepPoint()->GetGlobalTime()/ns << G4endl;

      // G4bool valid;
      // G4int hNavId = G4ParallelWorldProcess::GetHypNavigatorID();
      // auto iNav = G4TransportationManager::GetTransportationManager()->GetActiveNavigatorsIterator();
      // G4ThreeVector normal = (iNav[hNavId])->GetLocalExitNormal(&valid);
      // float angle_normal = acos(px*normal.x() + py*normal.y() + pz*normal.z())/deg;
      // G4cout << "Normal Surface = " << normal << G4endl;
      // G4cout << "angle_normal = " << angle_normal << G4endl;

      //evtac->AddPhotonTrajectoryNStep();
      //evtac->FillPhotonTrajectoryX(x);
      //evtac->FillPhotonTrajectoryY(y);
      //evtac->FillPhotonTrajectoryZ(z);

      if(endproc == "OpAbsorption")
      {
        if(aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "ZnS") {evtac->CountBulkAbsZnS();}
        if(aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "Core_Fiber") {evtac->CountBulkAbsSc();}
        if(aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "Vacuum") {evtac->CountBulkAbsSc();}
        //G4cout << "Photon BulkAbsorbed" << G4endl;
        //G4cout << "N bulk abs Sc = " << evtac->GetBulkAbsSc() << G4endl;
        //G4cout << "N bulk abs ZnS = " << evtac->GetBulkAbsZnS() << G4endl;
        //evtac->FillPhotonTrajectoryNStep(evtac->GetPhotonTrajectoryNStep());
        evtac->ClearPhotonTrajectoryNStep();
        //evtac->FillPhotonFinalState(1);
        //evtac->FillFiberAngleCreation(evtac->GetPhotonCreationAngle());
      }


      if(endproc == "OpRayleigh")
      {
        ((AIFIRA2023SimTrackInformation*)(aStep->GetTrack()->GetUserInformation()))->CountRayleighScattering();
        //G4cout << "Rayleigh scattering" << G4endl;
        //G4cout << "Number of scattering = " << ((ENLOpticalSimTrackInformation*) (aStep->GetTrack()->GetUserInformation()))->GetRayleigh() << G4endl;
      }

      // If WLS -> Use this !!!
      /*
      else if (endproc == "OpWLS")
      {
      evtac->CountWLS();
      my_dist_after = my_dist_after + aStep->GetTrack()->GetTrackLength()/mm;
      //G4cout << "Count WLS true = " << info->GetWLSCount() << G4endl;
      //G4cout << "Distance d = " << d << G4endl;

      //{
      if (info->GetWLSCount() == 0)
      {
      Length_Track = aStep->GetTrack()->GetTrackLength()/mm;
      //G4cout << "Count WLS = " << info->GetWLSCount() << G4endl;
    }

    if (info->GetWLSCount() > 0)
    {
    Length_Track += aStep->GetTrack()->GetTrackLength()/mm;
    //G4cout << "Count WLS = " << info->GetWLSCount() << G4endl;
  }
  //G4cout << "Track Length = " << aStep->GetTrack()->GetTrackLength()/mm << G4endl;
  //G4cout << "Total Length Track (for different OpWLS) = " << Total_Length_Track << G4endl;
}
*/

else if(partname == "opticalphoton" && endproc != "Transportation" && endproc != "OpAbsorption" && endproc !="G4FSMP")
G4cout << endproc << G4endl;


if(aStep->GetPostStepPoint()->GetStepStatus()==fGeomBoundary){

  //G4cout << "Boundary Status = " << boundaryStatus << G4endl;

  switch(boundaryStatus){
    case Detection:
    {
      evtac->CountDetected();
      evtac->FillPhotonDetectorPositionX(x);
      evtac->FillPhotonDetectorPositionY(y);
      evtac->FillPhotonPositionZ(z);
      evtac->FillPhotonMomentumX(px);
      evtac->FillPhotonMomentumY(py);
      evtac->FillPhotonMomentumZ(pz);
      //G4cout << "Px = " << px << G4endl;
      //G4cout << "Py = " << py << G4endl;
      //G4cout << "Pz = " << pz << G4endl;
      evtac->FillBirthLambda(info->GetBirthLambda());
      evtac->FillPhotonTime(aStep->GetPostStepPoint()->GetGlobalTime()/ns);
      evtac->FillEnergype(aStep->GetTotalEnergyDeposit()/eV);
      evtac->FillRayleigh(((AIFIRA2023SimTrackInformation*) (aStep->GetTrack()->GetUserInformation()))->GetRayleigh());
      evtac->FillTotalReflections(((AIFIRA2023SimTrackInformation*) (aStep->GetTrack()->GetUserInformation()))->GetTotalInternalReflections());
      evtac->FillWrapReflecions(((AIFIRA2023SimTrackInformation*)(aStep->GetTrack()->GetUserInformation()))->GetReflections());
      evtac->FillPhotonTotalLength(aStep->GetTrack()->GetTrackLength()/mm + evtac->GetTrackLengthFastSimulated());
      evtac->FillFiberAngleDetection(angle/deg);
      evtac->FillFiberAngleCreation(evtac->GetPhotonCreationAngle());
      if(r >0.5)
      {
        //evtac->FillFiberAngleCreation(evtac->GetPhotonCreationAngle());
        //evtac->FillFiberAngleDetection(angle/deg);
        //G4cout << "ICI " << G4endl;
      }

      //G4cout << "angle detection = " << angle/deg << G4endl;
      // G4cout << "Lamda = " << info->GetBirthLambda() << " nm " << G4endl;
      // G4cout << "[STEP] Track Length fast simulated = " << evtac->GetTrackLengthFastSimulated() << G4endl;
      // G4cout << "[STEP] Track Length = " << evtac->GetTrackLengthFastSimulated()+aStep->GetTrack()->GetTrackLength()/mm << G4endl;
      //G4cout << "Photon detecté" << G4endl;
      //G4cout << "N detecté = " << evtac->GetDetected() << G4endl;

      evtac->FillPhotonTrajectoryNStep(evtac->GetPhotonTrajectoryNStep());
      evtac->ClearPhotonTrajectoryNStep();
      evtac->FillPhotonFinalState(4);

      break;
      case Absorption:    // used to get the number TRANSMITTED!!

      //if (theTrack->GetNextVolume()->GetName()=="Photocathode")
      if (theTrack->GetNextVolume()->GetName()=="CMOS")
      {
        evtac->CountFailed();
        //G4cout << "Photon failed" << G4endl;
        //G4cout << "N failed = " << evtac->GetFailed() << G4endl;

        //evtac->FillPhotonTrajectoryNStep(evtac->GetPhotonTrajectoryNStep());
        //evtac->ClearPhotonTrajectoryNStep();
        //evtac->FillPhotonFinalState(3);
        //evtac->FillFiberAngleCreation(evtac->GetPhotonCreationAngle());

      }
      else{  // if not bulk, transmitted, or detected...it must be surface!
        evtac->CountAbsorbed();
        //G4cout << "Photon surface absorbed" << G4endl;
        //G4cout << "N absorbed = " << evtac->GetAbsorbed() << G4endl;

        //evtac->FillPhotonTrajectoryNStep(evtac->GetPhotonTrajectoryNStep());
        //evtac->ClearPhotonTrajectoryNStep();
        //evtac->FillPhotonFinalState(0);
        //evtac->FillFiberAngleCreation(evtac->GetPhotonCreationAngle());
      }

      break;
      case Undefined: G4cout<<"Undefined Boundary Process!"<<G4endl;
      break;
      case NoRINDEX:
      {
        evtac->CountEscaped();
        //G4cout << "count escaped" << G4endl;
        //G4cout << "N escaped = " << evtac->GetEscaped() << G4endl;

        //evtac->FillPhotonTrajectoryNStep(evtac->GetPhotonTrajectoryNStep());
        //evtac->ClearPhotonTrajectoryNStep();
        //evtac->FillPhotonFinalState(2);
        //evtac->FillFiberAngleCreation(evtac->GetPhotonCreationAngle());
      }
      break;

      // if we have any kind of reflections, count them
      case LambertianReflection:
      {
        //G4cout << "Reflection L" << G4endl;
        break;
      }
      case FresnelRefraction:
      {
        //G4cout << "Fresnel Refraction" << G4endl;
        break;
      }
      case FresnelReflection:
      {
        //G4cout << "Fresnel Reflection" << G4endl;
        break;
      }
      case LobeReflection:
      {
        //G4cout << "Reflection Lobe" << G4endl;
        break;
      }
      case SpikeReflection:
      {
        ((AIFIRA2023SimTrackInformation*)(aStep->GetTrack()->GetUserInformation()))->CountReflections();
        //G4cout << "Reflection" << G4endl;
        break;}
        case TotalInternalReflection:
        {
          ((AIFIRA2023SimTrackInformation*)(aStep->GetTrack()->GetUserInformation()))->CountTotalInternalReflections();
          //G4cout << "Reflection totale" << G4endl;
          break;
        }
        default:
        break;
      }
    }
  }


  if(partname == "opticalphoton" && aStep->GetTrack()->GetCreatorProcess()->GetProcessName() == "Scintillation" && StepNo ==1)
  {
    if(aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "ZnS")
    {
      evtac->CountScintillationZnS();
      //G4cout << " Photon Scintillation from ZnS!!!" << G4endl;
    }

    if(aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "Scintillator"
    || aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "Core_Fiber")
    {
      evtac->CountScintillationSc();
      //G4cout << " Photon Scintillation from Sc!!!" << G4endl;
    }
    //G4cout << "n sc = " << evtac->GetCountScintillation() << G4endl;

  }

  if(partname == "opticalphoton" && aStep->GetTrack()->GetCreatorProcess()->GetProcessName() == "Cerenkov" && StepNo ==1)
  {
    //G4cout << " Photon Cerenkov !!!" << G4endl;
    if(aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "ZnS"){evtac->CountCerenkovZnS();}
    if(aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "Scintillator"){evtac->CountCerenkovSc();}
    //G4cout << "n cerenkov = " << evtac->GetCountCerenkov() << G4endl;
    //G4cout << " Birth = " << info->GetBirthLambda() << G4endl;
  }


}

//#######################################################################
//#######################################################################
//############################END OPTICAL PART###########################
//#######################################################################
//#######################################################################



//#######################################################################
//#######################################################################
//###########################START EM INFOS PART#########################
//#######################################################################
//#######################################################################

if (Parent_ID ==0 && StepNo==1)
{
  evtac->SetEstartTP(aStep->GetPreStepPoint()->GetKineticEnergy()/MeV);
  evtac->SetIncidentE(aStep->GetPreStepPoint()->GetKineticEnergy()/keV);
  evtac->SetParticuleID(partID);
  evtac->SetCharge(aStep->GetPostStepPoint()->GetCharge());
}


if (((aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "Scintillator") || (aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "ZnS") || (aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "Core_Fiber"))
&& ((aStep->GetPostStepPoint()->GetPhysicalVolume()->GetName() == "Scintillator") || (aStep->GetPostStepPoint()->GetPhysicalVolume()->GetName() == "ZnS") || (aStep->GetPostStepPoint()->GetPhysicalVolume()->GetName() == "Core_Fiber"))
&& partname != "opticalphoton")
{
  evtac->AddTrackLength(aStep->GetTrack()->GetStepLength()/mm);
  evtac->AddEdepTP(aStep->GetTotalEnergyDeposit()/keV);
}

//Be careful here !!! If Zns in here, put ZnS. If not, put Scintillator or Core_Fiber!!!!
if(Parent_ID ==0 && aStep->GetPostStepPoint()->GetPhysicalVolume()->GetName() == "Scintillator" && evtac->GetTPPositionZ()==0)
{
  evtac->SetTPPositionX(x);
  evtac->SetTPPositionY(y);
  evtac->SetTPPositionZ(z);
  evtac->SetTPTime(aStep->GetPostStepPoint()->GetGlobalTime()/ns);

  //if(x <-25){G4cout << "HERE X" << G4endl;}
  //if(y <-10){G4cout << "HERE Y" << G4endl;}
}

if(aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "ZnS" && partname != "opticalphoton") {evtac->AddEdepZnS(aStep->GetTotalEnergyDeposit()/keV);}
if(aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "Scintillator" && partname != "opticalphoton") {evtac->AddEdepSc(aStep->GetTotalEnergyDeposit()/keV);}
if(aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "Core_Fiber" && partname != "opticalphoton") {evtac->AddEdepSc(aStep->GetTotalEnergyDeposit()/keV);}

if(aStep->GetPostStepPoint()->GetPhysicalVolume()->GetName() == "World") {theTrack->SetTrackStatus(fStopAndKill);}


//G4cout << "Charge = " << aStep->GetPostStepPoint()->GetCharge() << G4endl;
//G4cout<< "Charge 2 = " << aStep->GetTrack()->GetDefinition()->GetPDGCharge()<<G4endl;

}
