/// AIFIRA2023SimVisManager.hh
//// Auteur: Arnaud HUBER for ENL group <huber@cenbg.in2p3.fr> 
//// Copyright: 2022 (C) Projet RATP - ENL [LP2IB] - CELIA

#ifndef AIFIRA2023SimVisManager_h
#define AIFIRA2023SimVisManager_h 1

#include "G4VisManager.hh"


class AIFIRA2023SimVisManager: public G4VisManager {

public:

  AIFIRA2023SimVisManager ();

private:

  void RegisterGraphicsSystems ();

};

#endif
