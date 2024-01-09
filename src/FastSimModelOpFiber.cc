#include "FastSimModelOpFiber.hh"

#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4ProcessManager.hh"
#include "G4OpProcessSubType.hh"
#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4EventManager.hh"
#include "AIFIRA2023SimEventAction.hh"
#include "Geometry.hh"

FastSimModelOpFiber::FastSimModelOpFiber(G4String name, G4Region* envelope, G4double CladdingIndex)
: G4VFastSimulationModel(name,envelope, CladdingIndex) {
  fOpBoundaryProc = NULL;
  fCoreMaterial = NULL;
  fProcAssigned = false;
  fSafety = 3;
  fSafetyFiberCore = 3;
  fTrkLength = 0.;
  fTrkLengthBis = 0.;
  fTrkLengthTer = 0.;
  fNtransport = 0.;
  fTransportUnit = 0.;
  fFiberAxis = G4ThreeVector(0);
  fKill = false;
  fNtotIntRefl = 0;
  fTrackId = 0;
  CoreTrackLength =0;
  FiberMultiCladding = CladdingIndex;
  fSquareGeometry = true;

  DefineCommands();
}

FastSimModelOpFiber::~FastSimModelOpFiber() {}

G4bool FastSimModelOpFiber::IsApplicable(const G4ParticleDefinition& type) {
  return &type == G4OpticalPhoton::OpticalPhotonDefinition();
}

G4bool FastSimModelOpFiber::ModelTrigger(const G4FastTrack& fasttrack) {
  const G4Track* track = fasttrack.GetPrimaryTrack();

  G4EventManager *evtman = G4EventManager::GetEventManager();
  AIFIRA2023SimEventAction *evtac = (AIFIRA2023SimEventAction*)evtman->GetUserEventAction();

  getCoreMaterial(track);

  auto matPropTable = fCoreMaterial->GetMaterialPropertiesTable();

  G4TouchableHandle theTouchable = track->GetTouchableHandle();
  auto fiberPos = theTouchable->GetHistory()->GetTopTransform().Inverse().TransformPoint(G4ThreeVector(0.,0.,0.));

  fFiberAxis = theTouchable->GetHistory()->GetTopTransform().Inverse().TransformAxis(G4ThreeVector(0.,0.,1.));
  fTrkLength = track->GetTrackLength();
  G4Tubs* tubs = static_cast<G4Tubs*>(theTouchable->GetSolid());
  G4Box* box = static_cast<G4Box*>(theTouchable->GetSolid());
  G4double fiberLen = 2.*box->GetZHalfLength();
  if(fiberLen==0)
  {
    fiberLen = 2.*tubs->GetZHalfLength();
    fSquareGeometry=false;
  }

  if ( !matPropTable ) return false;

  if ( matPropTable->GetProperty(kABSLENGTH) ) {

    double attLength = matPropTable->GetProperty(kABSLENGTH)->Value( track->GetDynamicParticle()->GetTotalMomentum() );

    if(evtac->GetPhotonCreationAngle() >20.4 && evtac->GetPhotonCreationAngle() <159.6)
    {
      //attLength = 1.05*attLength; //ROUND SINGLE CLADDING
      attLength = 1.1*attLength; //ROUND MULTI CLADDING
      //attLength = 1.08*attLength; //SQUARE AIR INDEX = 1
      //attLength = 1.02*attLength; //SQUARE AIR INDEX = 1.49
      if(fSquareGeometry==true) return false;
    }

    if(evtac->GetPhotonCreationAngle() >27. && evtac->GetPhotonCreationAngle() <153)
    {
      //53 for square fiber && 51 for round fiber single cladding
      fKill = true;
      return true;
    }


    // if(evtac->GetPhotonCreationAngle() >40. && evtac->GetPhotonCreationAngle() <140)
    // {
    //   fKill=true;
    //   return true;
    // }

    if ( !checkTotalInternalReflection(track) )
    {
      //G4cout << "Previous status is not internal reflection" << G4endl;
      return false; // nothing to do if the previous status is not total internal reflection
    }


    // G4cout << "Fiber Length = " << fiberLen << G4endl;
    // G4cout << "Fiber pos = " << fiberPos << G4endl;
    // G4cout << "Fiber axis = " << fFiberAxis << G4endl;
    // G4cout << "F Track Length = " << fTrkLength << G4endl;
    // G4cout << "F Track Length bis = " << fTrkLengthBis << G4endl;
    // G4cout << "Track length considered = " << fTrkLength - fTrkLengthBis << G4endl;
    // G4cout << "Track length considered in core fiber = " << CoreTrackLength << G4endl;
    //G4cout << "Fiber length = " << fiberLen << G4endl;


    if ( fTrkLength==0. ) { // kill stopped particle
      fKill = true;
      //G4cout << "Track Length = 0" << G4endl;
      return true;
    }


    auto delta = track->GetMomentumDirection() * (fTrkLength-fTrkLengthBis);
    // G4cout << "Momentum Direction = " << track->GetMomentumDirection() << G4endl;
    // G4cout << "delta = " << delta << G4endl;
    fTransportUnit = delta.dot(fFiberAxis);
    //G4cout << "fTransportUnit = " << fTransportUnit << G4endl;

    if ( fTransportUnit < 0. ) { // kill backward propagation
      fKill = true;
      //G4cout << "fTransportUnit <0" << G4endl;
      return true;
    }

    auto fiberEnd = fiberPos + fFiberAxis*fiberLen/2.;
    //G4cout << "fiberEnd = " << fiberEnd << G4endl;
    auto toEnd = fiberEnd - track->GetPosition();
    //G4cout << "toEnd = " << toEnd << G4endl;
    double toEndAxis = toEnd.dot(fFiberAxis);
    //G4cout << "toEndAxis = " << toEndAxis << G4endl;
    double maxTransport = std::floor(toEndAxis/fTransportUnit);
    //G4cout << "maxTransport = " << maxTransport << G4endl;
    fNtransport = maxTransport - fSafety;
    //fNtransport = maxTransport;
    //G4cout << "fNtransport = " << fNtransport << G4endl;

    //G4cout << "Track Length fast simulated = " << (fTrkLength-fTrkLengthBis)*fNtransport << G4endl;

    if ( fNtransport <= 0. ) { // require at least n = fSafety of total internal reflections at the end
      reset();
      //G4cout << "FNtransport <= 0" << G4endl;
      return false;
    }

    if(evtac->GetPhotonCreationAngle() >50. && evtac->GetPhotonCreationAngle() <130)
    {
      //53 for square fiber && 51 for round fiber single cladding
      return false;
    }




    evtac->SetTrackLengthFastSimulated((fTrkLength-fTrkLengthBis)*fNtransport);
    double nInteractionLength = (fTrkLength-fTrkLengthBis)*fNtransport/attLength;
    //G4cout << "nInteractionLength = " << nInteractionLength << G4endl;
    double nInteractionLengthLeft = -std::log( G4UniformRand() );
    //G4cout << "nInteractionLengthLeft = " << nInteractionLengthLeft << G4endl;

    if ( nInteractionLength > nInteractionLengthLeft ) { // OpAbsorption
      fKill = true;
      //G4cout << "nInteractionLength > nInteractionLengthLeft" << G4endl;
      evtac->CountBulkAbsSc();
      return true;
    }
  }

  fKill = false;

  return true;
}



void FastSimModelOpFiber::DoIt(const G4FastTrack& fasttrack, G4FastStep& faststep) {
  auto track = fasttrack.GetPrimaryTrack();

  if (fKill) {
    //G4cout << "Particule killed" << G4endl;
    faststep.KillPrimaryTrack();
    reset();

    return;
  }

  double velocity = CalculateVelocityForOpticalPhoton(track);
  double timeUnit = (fTrkLength-fTrkLengthBis)/velocity;
  //G4cout << "Time Unit = " << timeUnit << G4endl;
  auto posShift = fTransportUnit*fNtransport*fFiberAxis;
  //G4cout << "posShift = " << posShift << G4endl;
  double timeShift = timeUnit*fNtransport;
  //G4cout << "Time Shift = " << timeShift << G4endl;

  faststep.ProposePrimaryTrackFinalPosition( track->GetPosition() + posShift, false );
  //G4cout << "Proposal final position = " << track->GetPosition() + posShift << G4endl;
  faststep.ProposePrimaryTrackFinalTime( track->GetGlobalTime() + timeShift );
  faststep.ProposePrimaryTrackFinalKineticEnergy( track->GetKineticEnergy() );
  faststep.ProposePrimaryTrackFinalMomentumDirection( track->GetMomentumDirection(), false );
  faststep.ProposePrimaryTrackFinalPolarization( track->GetPolarization(), false );

  reset();

  return;
}

bool FastSimModelOpFiber::checkTotalInternalReflection(const G4Track* track) {
  if (!fProcAssigned) { // locate OpBoundaryProcess only once
    setOpBoundaryProc(track);
  }

  G4EventManager *evtman = G4EventManager::GetEventManager();
  AIFIRA2023SimEventAction *evtac = (AIFIRA2023SimEventAction*)evtman->GetUserEventAction();

  if ( fOpBoundaryProc->GetStatus()==G4OpBoundaryProcessStatus::FresnelReflection ) {
    if(evtac->GetAirIndex()==1)
    {
      if(evtac->GetPhotonCreationAngle() <20.4 || evtac->GetPhotonCreationAngle() >22) //USE ONLY IF AIR INDEX IS 1
      {
        reset();
      }
    }
    if(evtac->GetAirIndex()>1.)
    {
      reset();
    }
  }

  if(track->GetCurrentStepNumber() ==1)
  {
    //G4cout << "Step Number = " << track->GetCurrentStepNumber() << G4endl;
    reset();
  }

  if ( track->GetTrackStatus()==fStopButAlive || track->GetTrackStatus()==fStopAndKill ) return false;

  if ( fOpBoundaryProc->GetStatus()==G4OpBoundaryProcessStatus::TotalInternalReflection ) {
    //G4cout << "fTrackId = " << fTrackId << G4endl;
    //G4cout << "GetTrackID = " << track->GetTrackID() << G4endl;
    fNtotIntRefl++;
    if ( fTrackId != track->GetTrackID() ) { // reset everything if when encountered a different track
      //G4cout << "RESET ici !!!" << G4endl;
      reset();
    }
  }

  if ( fOpBoundaryProc->GetStatus()==G4OpBoundaryProcessStatus::FresnelReflection )
  {
    //G4cout << "fTrackId = " << fTrackId << G4endl;
    //G4cout << "GetTrackID = " << track->GetTrackID() << G4endl;
    fNtotIntRefl++;
    if ( fTrackId != track->GetTrackID() )
    { // reset everything if when encountered a different track
      //G4cout << "RESET ici !!!" << G4endl;
      reset();
    }
  }


  fTrackId = track->GetTrackID();
  //G4cout << "Track ID = " << fTrackId << G4endl;

  if(fNtotIntRefl ==1) fTrkLengthBis = track->GetTrackLength();
  //G4cout << "TrackLength Bis = " << fTrkLengthBis << G4endl;

  if (fNtotIntRefl >1 && fNtotIntRefl <4 && track->GetVolume()->GetName() == "Inner_Cladding_Fiber")
  {
    AddCoreTrackLength(track->GetStepLength());
    //G4cout << "Track Length in Core Fiber = " << CoreTrackLength << G4endl;
  }


  if ( fNtotIntRefl > fSafety && track->GetNextVolume()->GetName() == "Core_Fiber") { // require at least n = fSafety of total internal reflections at the beginning
    //G4cout << "N reflection tot > Safety = 2 && Next volume Core_Fiber" << G4endl;
    return true;
  }

  return false;
}

void FastSimModelOpFiber::setOpBoundaryProc(const G4Track* track) {
  G4ProcessManager* pm = track->GetDefinition()->GetProcessManager();
  auto postStepProcessVector = pm->GetPostStepProcessVector();

  for (int np = 0; np < postStepProcessVector->entries(); np++) {
    auto theProcess = (*postStepProcessVector)[np];

    if ( theProcess->GetProcessType()!=fOptical || theProcess->GetProcessSubType()!=G4OpProcessSubType::fOpBoundary ) continue;

    fOpBoundaryProc = (G4OpBoundaryProcess*)theProcess;
    fProcAssigned = true;

    break;
  }

  return;
}

G4double FastSimModelOpFiber::CalculateVelocityForOpticalPhoton(const G4Track* track) {
  G4double velocity = CLHEP::c_light;
  G4bool update_groupvel = false;
  G4MaterialPropertyVector* groupvel = nullptr;

  // check if previous step is in the same volume
  // and get new GROUPVELOCITY table if necessary
  if ( (fCoreMaterial!=nullptr) && (groupvel==nullptr) ) {
    if ( fCoreMaterial->GetMaterialPropertiesTable() != nullptr ) {
      groupvel = fCoreMaterial->GetMaterialPropertiesTable()->GetProperty("GROUPVEL");
    }
    update_groupvel = true;
  }

  if ( groupvel != nullptr ) {
    // light velocity = c/(rindex+d(rindex)/d(log(E_phot)))
    // values stored in GROUPVEL material properties vector
    G4double current_momentum = track->GetDynamicParticle()->GetTotalMomentum();
    if ( update_groupvel ) {
      velocity = groupvel->Value(current_momentum);
    }
  }

  return velocity;
}

void FastSimModelOpFiber::getCoreMaterial(const G4Track* track) {
  auto physVol = track->GetVolume();
  auto logVol = physVol->GetLogicalVolume();

  if ( logVol->GetNoDaughters()==0 ) {
    fCoreMaterial = logVol->GetMaterial();
  } else {
    auto corePhys = logVol->GetDaughter(0);
    fCoreMaterial = track->GetNextMaterial();
  }
}

void FastSimModelOpFiber::reset() {
  fTrkLength = 0.;
  fTrkLengthBis = 0.;
  fTrkLengthTer = 0.;
  fNtransport = 0.;
  fTransportUnit = 0.;
  fFiberAxis = G4ThreeVector(0);
  fKill = false;
  fNtotIntRefl = 0;
  fTrackId = 0;
  fSafetyFiberCore=3;
  CoreTrackLength=0;
  fSquareGeometry=true;
  //G4cout << "Passage par la fonction Reset() " << G4endl;
}

void FastSimModelOpFiber::DefineCommands() {
  fMessenger = new G4GenericMessenger(this, "/DRsim/fastOp/", "fast Op transport model control");
  G4GenericMessenger::Command& safetyCmd = fMessenger->DeclareProperty("safety",fSafety,"min number of total internal reflection");
  safetyCmd.SetParameterName("safety",true);
  safetyCmd.SetDefaultValue("2.");
}
