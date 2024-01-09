/// AIFIRA2023SimTrackInformation.hh
//// Auteur: Arnaud HUBER for ENL group <huber@cenbg.in2p3.fr>
//// Copyright: 2022 (C) Projet RATP - ENL [LP2IB] - CELIA

//This class stores the statistics for each photon

#ifndef AIFIRA2023SimTrackInformation_h
#define AIFIRA2023SimTrackInformation_h 1

#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4Allocator.hh"
#include "G4Track.hh"
#include "G4VUserTrackInformation.hh"
#include "CLHEP/Units/SystemOfUnits.h"

class AIFIRA2023SimTrackInformation : public G4VUserTrackInformation
{
public:
  AIFIRA2023SimTrackInformation();
  AIFIRA2023SimTrackInformation(const G4Track *aTrack);
  ~AIFIRA2023SimTrackInformation();

  inline void *operator new(size_t);
  inline void operator delete(void *aTrackInfo);

private:
  G4ThreeVector BirthPosition;
  G4double BirthLambda;
  G4int Reflections;
  G4int ReflectBeforeWLS;
  G4int ReflectAfterWLS;
  G4double DistBeforeWLS;
  G4double DistAfterWLS;
  G4double TotalTrackLength;
  G4int WLSCount;
  G4int TotalInternalReflections;
  G4int Rayleigh;
  G4double TimeBeforeWLS;
  G4double my_time;
  G4double my_time2;

public:

  G4ThreeVector GetBirthPosition() const {return BirthPosition;}
  G4double GetBirthLambda() const {return BirthLambda;}
  G4double GetDistBeforeWLS() const {return DistBeforeWLS;}
  G4double GetDistAfterWLS() const {return DistAfterWLS;}
  G4double GetTotalTrackLength() const {return TotalTrackLength;}
  G4double GetTimeBeforeWLS() const {return TimeBeforeWLS;}

  G4int GetReflections() {return Reflections;}
  G4int GetReflectBeforeWLS() const {return ReflectBeforeWLS;}
  G4int GetReflectAfterWLS() const {return ReflectAfterWLS;}
  G4double GetTrackLength() const {return DistBeforeWLS;}
  G4int GetWLSCount() const {return WLSCount;}
  G4int GetTotalInternalReflections() const {return TotalInternalReflections;}
  G4int GetRayleigh() const {return Rayleigh;}

  void CountWLS(){WLSCount++;}

  void CountReflections();
  void CountTotalInternalReflections();
  void CountRayleighScattering();


  void SetDistBeforeWLS(G4double dist){DistBeforeWLS = dist;}
  void Print() const {;}
};

extern G4Allocator<AIFIRA2023SimTrackInformation> TrackInformationAllocator;

inline void* AIFIRA2023SimTrackInformation::operator new(size_t)
{ void* aTrackInfo;
  aTrackInfo = (void*)TrackInformationAllocator.MallocSingle();
  return aTrackInfo;
}

inline void AIFIRA2023SimTrackInformation::operator delete(void *aTrackInfo)
{ TrackInformationAllocator.FreeSingle((AIFIRA2023SimTrackInformation*)aTrackInfo);}

#endif
