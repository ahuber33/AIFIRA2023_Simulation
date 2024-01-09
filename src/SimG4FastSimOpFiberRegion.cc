#include "SimG4FastSimOpFiberRegion.hh"

// Geant4
#include "G4RegionStore.hh"
#include "G4TransportationManager.hh"
#include "G4VFastSimulationModel.hh"

#include "G4SystemOfUnits.hh"

#include <stdexcept>

SimG4FastSimOpFiberRegion::SimG4FastSimOpFiberRegion() {}

SimG4FastSimOpFiberRegion::~SimG4FastSimOpFiberRegion() {
  if (fModel) delete fModel;
}

void SimG4FastSimOpFiberRegion::create() {
  auto regionStore = G4RegionStore::GetInstance();
  auto region = regionStore->GetRegion("Fiber_Region");

  fModel = new FastSimModelOpFiber("FastSimModelOpFiber",region, FiberMultiCladding);
  //fModel->SetFiberLength(2000.*mm);

  return;
}
