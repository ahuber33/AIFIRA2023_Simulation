/// AIFIRA2023SimTrackInformation.cc
//// Auteur: Arnaud HUBER for ENL group <huber@cenbg.in2p3.fr>
//// Copyright: 2022 (C) Projet RATP - ENL [LP2IB] - CELIA

#include "AIFIRA2023SimTrackInformation.hh"
#include "G4VProcess.hh"

using namespace CLHEP;

G4Allocator<AIFIRA2023SimTrackInformation> TrackInformationAllocator;
AIFIRA2023SimTrackInformation::AIFIRA2023SimTrackInformation(){}
AIFIRA2023SimTrackInformation::AIFIRA2023SimTrackInformation(const G4Track *aTrack)
{
  //set new TrackInformation for each photon at scintillation
  if(!aTrack->GetUserInformation()){
    BirthPosition = aTrack->GetVertexPosition();
    BirthLambda = 1240/(aTrack->GetTotalEnergy()/eV);
    DistBeforeWLS = 0;
    DistAfterWLS = 0;
    ReflectBeforeWLS = 0;
    ReflectAfterWLS = 0;
    Reflections = 0;
    WLSCount = 0;
    TotalInternalReflections = 0;
    Rayleigh =0;
  }

  else{
    AIFIRA2023SimTrackInformation *anInfo = (AIFIRA2023SimTrackInformation*)(aTrack->GetUserInformation());

    BirthPosition = aTrack->GetVertexPosition();

    if(WLSCount == 0){
      DistBeforeWLS = aTrack->GetTrackLength()/mm;
    }

    if(WLSCount > 0){
      DistBeforeWLS = aTrack->GetTrackLength()/mm;
    }

    ReflectBeforeWLS = anInfo->GetReflectBeforeWLS();
    ReflectAfterWLS = anInfo->GetReflectAfterWLS();
    Reflections = anInfo->GetReflections();
    WLSCount = anInfo->GetWLSCount()+1;
//G4cout<< "WLS count = " << anInfo->GetWLSCount() << G4endl;

  }
}

void AIFIRA2023SimTrackInformation::CountReflections(){
  Reflections++;

  if(WLSCount==0)
    ReflectBeforeWLS++;
  else
    if(WLSCount > 0){
      ReflectAfterWLS++;
    }

}

void AIFIRA2023SimTrackInformation::CountTotalInternalReflections(){

    TotalInternalReflections++;
}

void AIFIRA2023SimTrackInformation::CountRayleighScattering(){

    Rayleigh++;
}


AIFIRA2023SimTrackInformation::~AIFIRA2023SimTrackInformation(){}
