/// FiberParametrisation.hh
//// Auteur: Arnaud HUBER for ENL group <huber@cenbg.in2p3.fr>
//// Copyright: 2022 (C) Projet RATP - ENL [LP2IB] - CELIA

#ifndef FiberParameterisation_h
#define FiberParameterisation_h 1

#include "globals.hh"
#include "G4VPVParameterisation.hh"

class G4VPhysicalVolume;
class G4Box;

// Dummy declarations to get rid of warnings ...
class G4Trd;
class G4Trap;
class G4Cons;
class G4Orb;
class G4Sphere;
class G4Ellipsoid;
class G4Torus;
class G4Para;
class G4Hype;
class G4Tubs;
class G4Polycone;
class G4Polyhedra;

///  A parameterisation that describes a series of boxes along Z.
///
///  The boxes have equal width, & their lengths are a linear equation.
///  They are spaced an equal distance apart, starting from given location.

class FiberParameterisation : public G4VPVParameterisation
{
  public:

    FiberParameterisation(G4int    noFibers,
                              G4double startZ,
                              G4double spacing,
                              G4double widthFiber,
                              G4double length);

    ~FiberParameterisation() override;

    void ComputeTransformation (const G4int copyNo,
                                G4VPhysicalVolume* physVol) const override;

    void ComputeDimensions (G4Box & trackerLayer, const G4int copyNo,
                            const G4VPhysicalVolume* physVol) const override;

    void ComputeDimensions (G4Tubs & trackerLayer, const G4int copyNo,
                            const G4VPhysicalVolume* physVol) const override;

  private:  // Dummy declarations to get rid of warnings ...

    //void ComputeDimensions (G4Box&,const G4int,
      //                      const G4VPhysicalVolume*) const override {}
    void ComputeDimensions (G4Trd&,const G4int,
                            const G4VPhysicalVolume*) const override {}
    void ComputeDimensions (G4Trap&,const G4int,
                            const G4VPhysicalVolume*) const override {}
    void ComputeDimensions (G4Cons&,const G4int,
                            const G4VPhysicalVolume*) const override {}
    void ComputeDimensions (G4Sphere&,const G4int,
                            const G4VPhysicalVolume*) const override {}
    void ComputeDimensions (G4Orb&,const G4int,
                            const G4VPhysicalVolume*) const override {}
    void ComputeDimensions (G4Ellipsoid&,const G4int,
                            const G4VPhysicalVolume*) const override {}
    void ComputeDimensions (G4Torus&,const G4int,
                            const G4VPhysicalVolume*) const override {}
    void ComputeDimensions (G4Para&,const G4int,
                            const G4VPhysicalVolume*) const override {}
    void ComputeDimensions (G4Hype&,const G4int,
                            const G4VPhysicalVolume*) const override {}
    void ComputeDimensions (G4Polycone&,const G4int,
                            const G4VPhysicalVolume*) const override {}
    void ComputeDimensions (G4Polyhedra&,const G4int,
                            const G4VPhysicalVolume*) const override {}

  private:

    G4int    fNoFibers;
    G4double fStart;
    G4double fLength;        //  Length fiber
    G4double fSpacing;          //  The distance between the Fibers' center
    G4double fWidth;        //  Case of Squared Fiber

};


#endif
