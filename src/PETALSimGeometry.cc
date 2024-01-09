// GEANT4 Nuclear experiment PETAL $

#include "AIFIRA2023SimGeometry.hh"


#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4Element.hh"
#include "G4ElementTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

#include "G4Box.hh"
#include "G4Sphere.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"
#include "G4PVReplica.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"
#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4SDManager.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include <G4LogicalBorderSurface.hh>
#include <G4OpticalSurface.hh>
#include <G4IntersectionSolid.hh>
#include "G4GeometryTolerance.hh"
#include "G4UserLimits.hh"
#include "G4Polycone.hh"
#include "G4Polyhedra.hh"
#include "G4GeometryTolerance.hh"
#include "G4NistManager.hh"
#include "G4SubtractionSolid.hh"

#include "G4ios.hh"
#include <G4UnionSolid.hh>
#include <G4UIcmdWithAString.hh>
#include <globals.hh>
#include <stdlib.h>


AIFIRA2023SimGeometry::AIFIRA2023SimGeometry()
{
experimentalHall_log=0;
MS_IP_log=protective_MS_log=phosphor_MS_log=support_MS_log=magnetic_MS_log=0;
SR_IP_log=protective_SR_log=phosphor_SR_log=support_SR_log=magnetic_SR_log=0;
TR_IP_log=phosphor_TR_log=support_TR_log=magnetic_TR_log=0;
ND_IP_log=phosphor_ND_log=support_ND_log=magnetic_ND_log=0;
filtre_PEHD_log=filtre_mylar_log=filtre_al_log=filtre_ti_log=filtre_fe_log=0;
filtre_cu_log=filtre_mo_log=filtre_ag_log=filtre_sn_log=filtre_ta_log=0;
filtre_au_log=filtre_pb1_log=filtre_pb2_log=filtre_pb3_log=filtre_pb4_log=0;
filtre_pb5_log=filtre_pb6_log=filtre_al_back_log=filtre_pb_back_log=0;
blindage_1_log=blindage_2_log=blindage_3_log=blindage_4_log=blindage_5_log=blindage_6_log=0;
}

AIFIRA2023SimGeometry::~AIFIRA2023SimGeometry()
{

}

inline G4Material* GetMat(G4String const& name)
{ return G4Material::GetMaterial( name ); }

G4VPhysicalVolume* AIFIRA2023SimGeometry::Construct()
{
  // **************************  COLOURS  ******************************

	 G4Colour red		(1.,0.,0.);	// Define red color
	 G4Colour green		(0.,1.,0.);	// Define green color
	 G4Colour blue		(0.,0.,1.);	// Define blue color
	 G4Colour violet	(1.,0.,1.);
	 G4Colour turquoise	(0.,1.,1.);
	 G4Colour white		(1.,1.,1.);
	 G4Colour yellow	(1.,1.,0.);
	 G4Colour orange	(1.,0.5,0.);
	 G4Colour grey		(0.5,0.5,0.5);
	 G4Colour black        (0.0, 0.0, 0.0);
	 G4Colour brown         (0.7, 0.4, 0.1);

		 G4VisAttributes attred(red);   // Define a red visualization attribute
		 G4VisAttributes attgreen(green);
		 G4VisAttributes attblue(blue);
		 G4VisAttributes attviolet(violet);
		 G4VisAttributes attturq(turquoise);
		 G4VisAttributes attwhite(white);
		 G4VisAttributes attyellow(yellow);
		 G4VisAttributes attorange(orange);
		 G4VisAttributes attgrey(grey);

// ********************    END OF COLOURS    *************************
// ************************** MATERIALS ******************************
	G4String name, symbol;
	G4double a, z, dens;		// , fractionmass, pressure, temperature
	G4int ncomp, natoms;
	G4double abundance, fractionmass;

	G4double density;
	G4int ncfiltre_ti_logomponents,nelements;
	G4int nprot,nnucl;
	G4double Mmol;

	const G4double epsilon = 1*nm;

////////////////////////////////////////////////////////////////////////////
//------- ELEMENTS --------------
////////////////////////////////////////////////////////////////////////////

	G4Element* H  = new G4Element(name="H",  symbol="H", z=1, a= 1.000*g/mole);
	G4Element* D  = new G4Element(name="D",  symbol="D", z=1.,a= 2.01*g/mole);
	G4Element* Li = new G4Element(name="Li",  symbol="Li", z=3, a=6.941*g/mole);
	G4Element* C  = new G4Element(name="C",  symbol="C", z=6, a=12.000*g/mole);
	G4Element* N  = new G4Element(name="N",  symbol="N", z=7, a=14.010*g/mole);
    	G4Element* O  = new G4Element(name="O",  symbol="O", z=8, a=16.000*g/mole);
    	G4Element* F  = new G4Element(name="F",  symbol="F", z=9, a=18.998*g/mole);
	G4Element* Al = new G4Element(name="Al", symbol="Al",z=13,a=26.98*g/mole);//
    	G4Element* Si = new G4Element(name="Si", symbol="Si",z=14,a=28.090*g/mole);
    	G4Element* Ar = new G4Element(name="Ar",  symbol="Ar", z=18, a=39.048*g/mole);
    	G4Element* Ca = new G4Element(name="Ca", symbol="Ca",z=20,a= 40.078*g/mole);
	G4Element* Mn = new G4Element(name="Mn", symbol="Mn",z=25,a=54.938*g/mole);
	G4Element* Ti = new G4Element(name="Ti", symbol="Ti",z=22,a=47.867*g/mole);//
    	G4Element* Fe = new G4Element(name="Fe", symbol="Fe",z=26,a=55.850*g/mole);//
    	G4Element* Ni = new G4Element(name="Ni", symbol="Ni",z=28,a= 58.6934*g/mole);
	G4Element* Cu = new G4Element(name="Cu", symbol="Cu",z=29,a=63.000*g/mole);//
	G4Element* Zn = new G4Element(name="Zn", symbol="Zn",z=30,a=65.409*g/mole);
	G4Element* Br = new G4Element(name="Br", symbol="Br",z=35,a= 79.904*g/mole);
    	G4Element* Mo = new G4Element(name="Mo", symbol="Mo",z=42,a= 95.94*g/mole);//
	G4Element* Ag = new G4Element(name="Ag", symbol="Ag",z=47,a= 107.86*g/mole);//
	G4Element* Sn = new G4Element(name="Sn", symbol="Sn",z=50,a= 118.71*g/mole);//
	G4Element* I  = new G4Element(name="I", symbol="I",z=53,a= 126.90447*g/mole);
	G4Element* Ba = new G4Element(name="Ba", symbol="Ba",z=56,a= 137.327*g/mole);
	G4Element* Gd = new G4Element(name="Gd", symbol="Gd",z=64,a= 157.25*g/mole);
	G4Element* Ta = new G4Element(name="Ta", symbol="Ta",z=73,a= 180.94*g/mole);//
	G4Element* W  = new G4Element(name="W", symbol="W",z=74,a= 183.85*g/mole);
	G4Element* Au = new G4Element(name="Au", symbol="Au",z=79,a= 196.96*g/mole);//
	G4Element* Pb = new G4Element(name="Pb", symbol="Pb",z=82,a= 207.2*g/mole);//

    //////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
//------- MATERIALS --------------
////////////////////////////////////////////////////////////////////////////

 //Canon Brem
  G4Material* Aluminium = new G4Material ("Al",z=13.,a=26.98*g/mole,dens=2.7*g/cm3);
  G4Material* Titane 	= new G4Material ("Ti",z=22.,a=47.867*g/mole,dens=4.51*g/cm3);
  G4Material* Fer 	= new G4Material ("Fe",z=26.,a=55.850*g/mole,dens=7.874*g/cm3);
  G4Material* Cuivre 	= new G4Material ("Cu",z=29.,a=63*g/mole,dens=8.96*g/cm3);
  G4Material* Molybdene = new G4Material ("Mo",z=42.,a=95.94*g/mole,dens=10.22*g/cm3);
  G4Material* Argent    = new G4Material ("Ag",z=47.,a=107.86*g/mole,dens=10.5*g/cm3);
  G4Material* Etain     = new G4Material ("Sn",z=50.,a=118.71*g/mole,dens=7.29*g/cm3);
  G4Material* Tantale 	= new G4Material ("Ta",z=73.,a=180.94*g/mole,dens=16.4*g/cm3);
  G4Material* Or 	= new G4Material ("Au",z=79.,a=196.96*g/mole,dens=19.3*g/cm3);
  G4Material* Plomb 	= new G4Material ("Pb",z=82.,a=207.2*g/mole,dens=11.35*g/cm3);
  G4Material* Tungstene     = new G4Material ("W",z=74.,a=183.84*g/mole,dens=19.3*g/cm3);
  G4Material* Nickel = new G4Material ("Ni", z=28, a= 58.6934*g/mole, dens=8.9*g/cm3);

  G4Material* Plastic = new G4Material(name="Plastic",dens=0.965 *g/cm3, ncomp=2);//PEHD
    Plastic ->AddElement(H, natoms=2);
    Plastic ->AddElement(C, natoms=1);

  G4Material* Mylar= new G4Material(name="Mylar",dens=1.39 *g/cm3, ncomp=3);
    Mylar ->AddElement(C, natoms=62.5);
    Mylar ->AddElement(H, natoms=4.2);
    Mylar ->AddElement(O, natoms=33.3);

    G4Material* Denal = new G4Material(name="Denal",dens=17.6 *g/cm3, ncomp=3);
    Denal ->AddElement(W, natoms=96);
    Denal ->AddElement(Ni, natoms=3);
    Denal ->AddElement(Fe, natoms=1);

 //Air
  G4Material* Air 		= new G4Material("Air", dens= 1.29*mg/cm3, 2);
  Air->AddElement(N, 70*perCent);
  Air->AddElement(O, 30*perCent);

 // Vacuum
	G4Material *vacuum=0;
	if (G4Material::GetMaterial("Vacuum",false) == 0) {
		vacuum = new G4Material(name="Vacuum",density=1e-15*g/cm3,ncomp=1,kStateGas, STP_Temperature,1e-15*bar);
		vacuum->AddMaterial(Air,100.*perCent);
		G4cout << "creating Material Vacuum" << G4endl;
	}

    // Ethenone
    G4Material* Ethenone = new G4Material(name="Ethenone",dens=1.66 *g/cm3, ncomp=3);
    Ethenone ->AddElement(C, natoms=2);
    Ethenone ->AddElement(H, natoms=2);
    Ethenone ->AddElement(O, natoms=1);
    G4Material* EthenoneSR = new G4Material(name="EthenoneSR",dens=1.273 *g/cm3, ncomp=3);
    EthenoneSR ->AddElement(C, natoms=2);
    EthenoneSR ->AddElement(H, natoms=2);
    EthenoneSR ->AddElement(O, natoms=1);

    //BaFBr0.85I0.15
       G4Material* mat_phosphor_MS = new G4Material(name="mat_phosphor_MS",dens=3.31 *g/cm3, ncomp=4);
    mat_phosphor_MS  ->AddElement(Ba, natoms=100);
    mat_phosphor_MS  ->AddElement(F, natoms=100);
    mat_phosphor_MS  ->AddElement(Br, natoms=85);
    mat_phosphor_MS  ->AddElement(I, natoms=15);
    G4Material* mat_phosphor_TR = new G4Material(name="mat_phosphor_TR",dens=2.85 *g/cm3, ncomp=4);
    mat_phosphor_TR  ->AddElement(Ba, natoms=100);
    mat_phosphor_TR  ->AddElement(F, natoms=100);
    mat_phosphor_TR  ->AddElement(Br, natoms=85);
    mat_phosphor_TR  ->AddElement(I, natoms=15);

        //BaFBr
       G4Material* mat_phosphor_SR = new G4Material(name="mat_phosphor_SR",dens=3.1 *g/cm3, ncomp=3);
    mat_phosphor_SR  ->AddElement(Ba, natoms=1);
    mat_phosphor_SR  ->AddElement(F, natoms=1);
    mat_phosphor_SR  ->AddElement(Br, natoms=1);

        //BaFBrGd0.88O1.3
       G4Material* mat_phosphor_ND = new G4Material(name="mat_phosphor_ND",dens=3.3 *g/cm3, ncomp=5);
    mat_phosphor_ND  ->AddElement(Ba, natoms=1);
    mat_phosphor_ND  ->AddElement(F, natoms=1);
    mat_phosphor_ND  ->AddElement(Br, natoms=1);
    mat_phosphor_ND  ->AddElement(Gd, natoms=0.88);
    mat_phosphor_ND  ->AddElement(O, natoms=1.3);

        //ZnMn2Fe5NO40H15C10
    G4Material* mat_magnetic_MS_TR = new G4Material(name="mat_magnetic_MS_TR",dens=2.77 *g/cm3, ncomp=7);
    mat_magnetic_MS_TR  ->AddElement(Zn, natoms=1);
    mat_magnetic_MS_TR  ->AddElement(Mn, natoms=2);
    mat_magnetic_MS_TR  ->AddElement(Fe, natoms=5);
    mat_magnetic_MS_TR  ->AddElement(N, natoms=1);
    mat_magnetic_MS_TR  ->AddElement(O, natoms=40);
    mat_magnetic_MS_TR  ->AddElement(H, natoms=15);
    mat_magnetic_MS_TR  ->AddElement(C, natoms=10);
    G4Material* mat_magnetic_SR = new G4Material(name="mat_magnetic_SR",dens=3.1 *g/cm3, ncomp=7);
    mat_magnetic_SR  ->AddElement(Zn, natoms=1);
    mat_magnetic_SR  ->AddElement(Mn, natoms=2);
    mat_magnetic_SR  ->AddElement(Fe, natoms=5);
    mat_magnetic_SR  ->AddElement(N, natoms=1);
    mat_magnetic_SR  ->AddElement(O, natoms=40);
    mat_magnetic_SR  ->AddElement(H, natoms=15);
    mat_magnetic_SR  ->AddElement(C, natoms=10);

// Print all the materials defined.
  //
  G4cout << G4endl << "The materials defined are : " << G4endl << G4endl;
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;


//  **************************************************************************************
//    --------------------------------------- VOLUMES ------------------------------
//  **************************************************************************************

G4cout << "Constructing ...experimental hall.." << G4endl;

//   *********************************  WORLD VOLUME  ************************************
//------------------------------ experimental hall

  G4Sphere * experimentalHall_box = new G4Sphere("expHall_box",0.*mm,50.*cm,0*deg,360*deg,0*deg,360*deg);
  //G4Tubs* experimentalHall_box = new G4Tubs("expHall_box",0*mm,100*mm,500*mm,0*deg,360*deg);
  G4LogicalVolume * experimentalHall_log = new G4LogicalVolume(experimentalHall_box, vacuum,"experimentalHall_log",0,0,0);
  G4PVPlacement* experimentalHall_phys = new G4PVPlacement(0,G4ThreeVector(0,0,0), experimentalHall_log, "expHall_P",0,false,999);
  experimentalHall_log->SetVisAttributes(G4VisAttributes::GetInvisible());

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////
// CANON BREMSSTRAHLUNG
///////////////////////////////////////////////////////////////

  G4cout << "Constructing ... CANON BREMSSTRAHLUNG... in progress" << G4endl;

// epaisseurs

const G4double ep_PEHD=15*mm;
const G4double ep_mylar=0.25*mm;
const G4double ep_al=0.09*mm;
const G4double ep_ti=0.125*mm;
const G4double ep_fe=0.125*mm;
const G4double ep_cu=0.1*mm;
const G4double ep_mo=0.1*mm;
const G4double ep_ag=0.15*mm;
const G4double ep_sn=0.5*mm;
const G4double ep_ta=0.5*mm;
const G4double ep_au=1.5*mm;
const G4double ep_pb1=1*mm;
const G4double ep_pb2=2*mm;
const G4double ep_pb3=3*mm;
const G4double ep_pb4=4*mm;
const G4double ep_pb5=6*mm;
const G4double ep_pb6=6*mm;
const G4double ep_al_back=3*mm;
const G4double ep_pb_back=8*mm;

const G4double rayon_filtre=10*mm;

// filtres

	G4Tubs* filtre_PEHD = new G4Tubs("filtre_PEHD",0*mm,7.5*mm,ep_PEHD/2.,0*deg,360*deg);
        filtre_PEHD_log = new G4LogicalVolume(filtre_PEHD,Plastic,"filtre_PEHD_log", 0,0,0);
	filtre_PEHD_log->SetVisAttributes(white);

        G4Tubs* filtre_mylar = new G4Tubs("filtre_mylar",0*mm,rayon_filtre,ep_mylar/2.,0*deg,360*deg);
        filtre_mylar_log = new G4LogicalVolume(filtre_mylar,Mylar,"filtre_mylar_log", 0,0,0);
	filtre_mylar_log->SetVisAttributes(green);

        G4Tubs* filtre_al = new G4Tubs("filtre_al",0*mm,rayon_filtre,ep_al/2.,0*deg,360*deg);
        filtre_al_log = new G4LogicalVolume(filtre_al,Aluminium,"filtre_al_log", 0,0,0);
	filtre_al_log->SetVisAttributes(red);

        G4Tubs* filtre_ti = new G4Tubs("filtre_ti",0*mm,rayon_filtre,ep_ti/2.,0*deg,360*deg);
        filtre_ti_log = new G4LogicalVolume(filtre_ti,Titane,"filtre_ti_log", 0,0,0);
	filtre_ti_log->SetVisAttributes(red);

        G4Tubs* filtre_fe = new G4Tubs("filtre_fe",0*mm,rayon_filtre,ep_fe/2.,0*deg,360*deg);
        filtre_fe_log = new G4LogicalVolume(filtre_fe,Fer,"filtre_fe_log", 0,0,0);
	filtre_fe_log->SetVisAttributes(red);

        G4Tubs* filtre_cu = new G4Tubs("filtre_cu",0*mm,rayon_filtre,ep_cu/2.,0*deg,360*deg);
        filtre_cu_log = new G4LogicalVolume(filtre_cu,Cuivre,"filtre_cu_log", 0,0,0);
	filtre_cu_log->SetVisAttributes(red);

        G4Tubs* filtre_mo = new G4Tubs("filtre_mo",0*mm,rayon_filtre,ep_mo/2.,0*deg,360*deg);
        filtre_mo_log = new G4LogicalVolume(filtre_mo,Aluminium,"filtre_mo_log", 0,0,0);
	filtre_mo_log->SetVisAttributes(red);

        G4Tubs* filtre_ag = new G4Tubs("filtre_ag",0*mm,rayon_filtre,ep_ag/2.,0*deg,360*deg);
        filtre_ag_log = new G4LogicalVolume(filtre_ag,Argent,"filtre_ag_log", 0,0,0);
	filtre_ag_log->SetVisAttributes(red);

        G4Tubs* filtre_sn = new G4Tubs("filtre_sn",0*mm,rayon_filtre,ep_sn/2.,0*deg,360*deg);
        filtre_sn_log = new G4LogicalVolume(filtre_sn,Etain,"filtre_sn_log", 0,0,0);
	filtre_sn_log->SetVisAttributes(red);

        G4Tubs* filtre_ta = new G4Tubs("filtre_ta",0*mm,rayon_filtre,ep_ta/2.,0*deg,360*deg);
        filtre_ta_log = new G4LogicalVolume(filtre_ta,Tantale,"filtre_ta_log", 0,0,0);
	filtre_ta_log->SetVisAttributes(red);

        G4Tubs* filtre_au = new G4Tubs("filtre_au",0*mm,rayon_filtre,ep_au/2.,0*deg,360*deg);
        filtre_au_log = new G4LogicalVolume(filtre_au,Or,"filtre_alu_log", 0,0,0);
	filtre_au_log->SetVisAttributes(red);

        G4Tubs* filtre_pb1 = new G4Tubs("filtre_pb1",0*mm,rayon_filtre,ep_pb1/2.,0*deg,360*deg);
        filtre_pb1_log = new G4LogicalVolume(filtre_pb1,Plomb,"filtre_pb1_log", 0,0,0);
	filtre_pb1_log->SetVisAttributes(red);

        G4Tubs* filtre_pb2 = new G4Tubs("filtre_pb2",0*mm,rayon_filtre,ep_pb2/2.,0*deg,360*deg);
        filtre_pb2_log = new G4LogicalVolume(filtre_pb2,Plomb,"filtre_pb2_log", 0,0,0);
	filtre_pb2_log->SetVisAttributes(red);

        G4Tubs* filtre_pb3 = new G4Tubs("filtre_pb3",0*mm,rayon_filtre,ep_pb3/2.,0*deg,360*deg);
        filtre_pb3_log = new G4LogicalVolume(filtre_pb3,Plomb,"filtre_pb3_log", 0,0,0);
	filtre_pb3_log->SetVisAttributes(red);

        G4Tubs* filtre_pb4 = new G4Tubs("filtre_pb4",0*mm,rayon_filtre,ep_pb4/2.,0*deg,360*deg);
        filtre_pb4_log = new G4LogicalVolume(filtre_pb4,Plomb,"filtre_pb4_log", 0,0,0);
	filtre_pb4_log->SetVisAttributes(red);

        G4Tubs* filtre_pb5 = new G4Tubs("filtre_pb5",0*mm,rayon_filtre,ep_pb5/2.,0*deg,360*deg);
        filtre_pb5_log = new G4LogicalVolume(filtre_pb5,Plomb,"filtre_pb5_log", 0,0,0);
	filtre_pb5_log->SetVisAttributes(red);

        G4Tubs* filtre_pb6 = new G4Tubs("filtre_pb6",0*mm,rayon_filtre,ep_pb6/2.,0*deg,360*deg);
        filtre_pb6_log = new G4LogicalVolume(filtre_pb6,Plomb,"filtre_pb6_log", 0,0,0);
	filtre_pb6_log->SetVisAttributes(red);

	G4Tubs* filtre_al_back = new G4Tubs("filtre_al_back",0*mm,15*mm,ep_al_back/2.,0*deg,360*deg);
        filtre_al_back_log = new G4LogicalVolume(filtre_al_back,Aluminium,"filtre_al_back_log", 0,0,0);
	filtre_al_back_log->SetVisAttributes(grey);

	G4Tubs* filtre_pb_back = new G4Tubs("filtre_pb_back",0*mm,15*mm,ep_pb_back/2.,0*deg,360*deg);
        filtre_pb_back_log = new G4LogicalVolume(filtre_pb_back,Plomb,"filtre_pb_back_log", 0,0,0);
	filtre_pb_back_log->SetVisAttributes(violet);

	// structure simplifiee du canon et blindage

	G4Tubs* blindage_1 = new G4Tubs("blindage_1",7.5*mm,25*mm,7.5*mm,0*deg,360*deg);
        blindage_1_log = new G4LogicalVolume(blindage_1,Plomb,"blindage_1_log", 0,0,0);
	blindage_1_log->SetVisAttributes(violet);

	G4Tubs* blindage_2 = new G4Tubs("blindage_2",7.5*mm,15*mm,10*mm,0*deg,360*deg);
        blindage_2_log = new G4LogicalVolume(blindage_2,Aluminium,"blindage_2_log", 0,0,0);
	blindage_2_log->SetVisAttributes(grey);

	G4Tubs* blindage_3 = new G4Tubs("blindage_3",15*mm,25*mm,38*mm,0*deg,360*deg);
        blindage_3_log = new G4LogicalVolume(blindage_3,Plomb,"blindage_3_log", 0,0,0);
	blindage_3_log->SetVisAttributes(violet);

	G4Tubs* blindage_4 = new G4Tubs("blindage_4",10*mm,15*mm,22.5*mm,0*deg,360*deg);
        blindage_4_log = new G4LogicalVolume(blindage_4,Aluminium,"blindage_4_log", 0,0,0);
	blindage_4_log->SetVisAttributes(grey);

	G4Tubs* blindage_5 = new G4Tubs("blindage_5",25*mm,47.55*mm,41.5*mm,0*deg,360*deg);
        blindage_5_log = new G4LogicalVolume(blindage_5,Plastic,"blindage_5_log", 0,0,0);
	blindage_5_log->SetVisAttributes(white);

	G4Tubs* blindage_6 = new G4Tubs("blindage_6",0*mm,47.55*mm,1.5*mm,0*deg,360*deg);
        blindage_6_log = new G4LogicalVolume(blindage_6,Plastic,"blindage_6_log", 0,0,0);
	blindage_6_log->SetVisAttributes(white);





 G4cout << "Constructing ... CANON BREMSSTRAHLUNG ... done" << G4endl;

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
/// 	MS-IP
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

G4cout << "Constructing ...MS_IP detector.." << G4endl;

const G4double protective_thickness_MS		=  9*um;
const G4double phosphor_thickness_MS	       =  115*um;
const G4double support_thickness_MS              =  190*um;
const G4double magnetic_thickness_MS            =  160*um;
G4double Z_MS=protective_thickness_MS+phosphor_thickness_MS+support_thickness_MS+magnetic_thickness_MS;

const G4double X_MS	       =  40*cm;
const G4double Y_MS            =  40*cm;
//const G4double d_stenope_ms_1=  4.5*m;

G4cout << "Computed tolerance = "
         << G4GeometryTolerance::GetInstance()->GetSurfaceTolerance()/cm
         << " cm" << G4endl;

// D�finition du volume MS_IP + placement
//G4Box* MS_IP = new G4Box("MS_IP",X_MS/2,Y_MS/2,Z_MS/2);
G4Tubs* MS_IP = new G4Tubs("MS_IP",0*mm,rayon_filtre,Z_MS/2.,0*deg,360*deg);
MS_IP_log  = new G4LogicalVolume(MS_IP, vacuum,"MS_IP_log",0,0,0);
MS_IP_log->SetVisAttributes(G4VisAttributes::GetInvisible());
//MS_IP_log->SetVisAttributes(yellow);

// inside the MS_IP
//
// protective layer
//
//G4VSolid * protective_MS  = new G4Box("protective_MS",X_MS/2,Y_MS/2,protective_thickness_MS/2);
G4VSolid * protective_MS = new G4Tubs("protective_MS",0*mm,rayon_filtre,protective_thickness_MS/2.,0*deg,360*deg);
protective_MS_log   = new G4LogicalVolume(protective_MS,Ethenone,"protective_MS_log",0,0,0);
protective_MS_phys  = new G4PVPlacement(0,G4ThreeVector(0.,0.,+Z_MS/2-protective_thickness_MS/2),protective_MS_log,"protective_MS_phys",MS_IP_log,false,100);
protective_MS_log->SetVisAttributes(blue);
//
// phosphor layer
//
//G4VSolid * phosphor_MS  = new G4Box("phosphor_MS",X_MS/2,Y_MS/2,phosphor_thickness_MS/2);
G4VSolid * phosphor_MS = new G4Tubs("phosphor_MS",0*mm,rayon_filtre,phosphor_thickness_MS/2.,0*deg,360*deg);
phosphor_MS_log   = new G4LogicalVolume(phosphor_MS,mat_phosphor_MS,"phosphor_MS_log",0,0,0);//mat_phosphor_MS
phosphor_MS_phys  = new G4PVPlacement(0,G4ThreeVector(0.,0.,+Z_MS/2-protective_thickness_MS-phosphor_thickness_MS/2),phosphor_MS_log,"phosphor_MS_phys",MS_IP_log,false,101);
phosphor_MS_log->SetVisAttributes(yellow);
//
// support layer
//
//G4VSolid * support_MS  = new G4Box("support_MS",X_MS/2,Y_MS/2,support_thickness_MS/2);
G4VSolid * support_MS = new G4Tubs("support_MS",0*mm,rayon_filtre,support_thickness_MS/2.,0*deg,360*deg);
support_MS_log   = new G4LogicalVolume(support_MS,Ethenone,"support_MS_log",0,0,0);
support_MS_phys  = new G4PVPlacement(0,G4ThreeVector(0.,0.,+Z_MS/2-protective_thickness_MS-phosphor_thickness_MS-support_thickness_MS/2),support_MS_log,"support_MS_phys",MS_IP_log,false,102);
support_MS_log->SetVisAttributes(blue);
//
// magnetic layer
//
//G4VSolid * magnetic_MS  = new G4Box("magnetic_MS",X_MS/2,Y_MS/2,magnetic_thickness_MS/2);
G4VSolid * magnetic_MS = new G4Tubs("magnetic_MS",0*mm,rayon_filtre,magnetic_thickness_MS/2.,0*deg,360*deg);
magnetic_MS_log   = new G4LogicalVolume(magnetic_MS,mat_magnetic_MS_TR,"magnetic_MS_log",0,0,0);
magnetic_MS_phys  = new G4PVPlacement(0,G4ThreeVector(0.,0.,+Z_MS/2-protective_thickness_MS-phosphor_thickness_MS-support_thickness_MS-magnetic_thickness_MS/2),magnetic_MS_log,"magnetic_MS_phys",MS_IP_log,false,103);
magnetic_MS_log->SetVisAttributes(blue);

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
/// 	Fin de MS-IP
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
/// 	SR-IP
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

G4cout << "Constructing ...SR_IP detector.." << G4endl;

const G4double protective_thickness_SR		=  6*um;
const G4double phosphor_thickness_SR	       =  120*um;
const G4double support_thickness_SR              =  188*um;
const G4double magnetic_thickness_SR            =  160*um;
G4double Z_SR=protective_thickness_SR+phosphor_thickness_SR+support_thickness_SR+magnetic_thickness_SR;

const G4double X_SR	       =  10*cm;
const G4double Y_SR            =  10*cm;
const G4double d_stenope_SR_1=  4.5*m;

G4cout << "Computed tolerance = "
         << G4GeometryTolerance::GetInstance()->GetSurfaceTolerance()/cm
         << " cm" << G4endl;

// D�finition du volume SR_IP + placement
G4Box* SR_IP = new G4Box("SR_IP",X_SR/2,Y_SR/2,Z_SR/2);
SR_IP_log  = new G4LogicalVolume(SR_IP, vacuum,"SR_IP_log",0,0,0);
SR_IP_log->SetVisAttributes(G4VisAttributes::GetInvisible());
//SR_IP_log->SetVisAttributes(yellow);

// inside the SR_IP
//
// protective layer
//
G4VSolid * protective_SR  = new G4Box("protective_SR",X_SR/2,Y_SR/2,protective_thickness_SR/2);
protective_SR_log   = new G4LogicalVolume(protective_SR,EthenoneSR,"protective_SR_log",0,0,0);
protective_SR_phys  = new G4PVPlacement(0,G4ThreeVector(0.,0.,+Z_SR/2-protective_thickness_SR/2),protective_SR_log,"protective_SR_phys",SR_IP_log,false,100);
protective_SR_log->SetVisAttributes(turquoise);
//
// phosphor layer
//
G4VSolid * phosphor_SR  = new G4Box("phosphor_SR",X_SR/2,Y_SR/2,phosphor_thickness_SR/2);
phosphor_SR_log   = new G4LogicalVolume(phosphor_SR,mat_phosphor_SR,"phosphor_SR_log",0,0,0);
phosphor_SR_phys  = new G4PVPlacement(0,G4ThreeVector(0.,0.,+Z_SR/2-protective_thickness_SR-phosphor_thickness_SR/2),phosphor_SR_log,"phosphor_SR_phys",SR_IP_log,false,101);
phosphor_SR_log->SetVisAttributes(green);
//
// support layer
//
G4VSolid * support_SR  = new G4Box("support_SR",X_SR/2,Y_SR/2,support_thickness_SR/2);
support_SR_log   = new G4LogicalVolume(support_SR,EthenoneSR,"support_SR_log",0,0,0);
support_SR_phys  = new G4PVPlacement(0,G4ThreeVector(0.,0.,+Z_SR/2-protective_thickness_SR-phosphor_thickness_SR-support_thickness_SR/2),support_SR_log,"support_SR_phys",SR_IP_log,false,102);
support_SR_log->SetVisAttributes(orange);
//
// magnetic layer
//
G4VSolid * magnetic_SR  = new G4Box("magnetic_SR",X_SR/2,Y_SR/2,magnetic_thickness_SR/2);
magnetic_SR_log   = new G4LogicalVolume(magnetic_SR,mat_magnetic_SR,"magnetic_SR_log",0,0,0);
magnetic_SR_phys  = new G4PVPlacement(0,G4ThreeVector(0.,0.,+Z_SR/2-protective_thickness_SR-phosphor_thickness_SR-support_thickness_SR-magnetic_thickness_SR/2),magnetic_SR_log,"magnetic_SR_phys",SR_IP_log,false,103);
magnetic_SR_log->SetVisAttributes(grey);

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
/// 	Fin de SR-IP
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
/// 	ND-IP
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

G4cout << "Constructing ...ND_IP detector.." << G4endl;

const G4double protective_thickness_ND		=  6*um;
const G4double phosphor_thickness_ND	       =  135*um;
const G4double support_thickness_ND              =  188*um;
const G4double magnetic_thickness_ND            =  160*um;
G4double Z_ND=protective_thickness_ND+phosphor_thickness_ND+support_thickness_ND+magnetic_thickness_ND;


G4cout << "Computed tolerance = "
         << G4GeometryTolerance::GetInstance()->GetSurfaceTolerance()/cm
         << " cm" << G4endl;

// D�finition du volume ND_IP + placement
G4Box* ND_IP = new G4Box("ND_IP",X_SR/2,Y_SR/2,Z_ND/2);
ND_IP_log  = new G4LogicalVolume(ND_IP, vacuum,"ND_IP_log",0,0,0);
ND_IP_log->SetVisAttributes(G4VisAttributes::GetInvisible());
//ND_IP_log->SetVisAttributes(yellow);

// inside the ND_IP
//
// protective layer
//
G4VSolid * protective_ND  = new G4Box("protective_ND",X_SR/2,Y_SR/2,protective_thickness_ND/2);
protective_ND_log   = new G4LogicalVolume(protective_ND,EthenoneSR,"protective_ND_log",0,0,0);
protective_ND_phys  = new G4PVPlacement(0,G4ThreeVector(0.,0.,+Z_ND/2-protective_thickness_ND/2),protective_ND_log,"protective_ND_phys",ND_IP_log,false,100);
protective_ND_log->SetVisAttributes(turquoise);
//
// phosphor layer
//
G4VSolid * phosphor_ND  = new G4Box("phosphor_ND",X_SR/2,Y_SR/2,phosphor_thickness_ND/2);
phosphor_ND_log   = new G4LogicalVolume(phosphor_ND,mat_phosphor_ND,"phosphor_ND_log",0,0,0);
phosphor_ND_phys  = new G4PVPlacement(0,G4ThreeVector(0.,0.,+Z_ND/2-protective_thickness_ND-phosphor_thickness_ND/2),phosphor_ND_log,"phosphor_ND_phys",ND_IP_log,false,101);
phosphor_ND_log->SetVisAttributes(green);
//
// support layer
//
G4VSolid * support_ND  = new G4Box("support_ND",X_SR/2,Y_SR/2,support_thickness_ND/2);
support_ND_log   = new G4LogicalVolume(support_ND,EthenoneSR,"support_ND_log",0,0,0);
support_ND_phys  = new G4PVPlacement(0,G4ThreeVector(0.,0.,+Z_ND/2-protective_thickness_ND-phosphor_thickness_ND-support_thickness_ND/2),support_ND_log,"support_ND_phys",ND_IP_log,false,102);
support_ND_log->SetVisAttributes(orange);
//
// magnetic layer
//
G4VSolid * magnetic_ND  = new G4Box("magnetic_ND",X_SR/2,Y_SR/2,magnetic_thickness_ND/2);
magnetic_ND_log   = new G4LogicalVolume(magnetic_ND,mat_magnetic_SR,"magnetic_ND_log",0,0,0);
magnetic_ND_phys  = new G4PVPlacement(0,G4ThreeVector(0.,0.,+Z_ND/2-protective_thickness_ND-phosphor_thickness_ND-support_thickness_ND-magnetic_thickness_ND/2),magnetic_ND_log,"magnetic_ND_phys",ND_IP_log,false,103);
magnetic_ND_log->SetVisAttributes(grey);

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
/// 	Fin de ND-IP
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
/// 	TR-IP
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

G4cout << "Constructing ...TR_IP detector.." << G4endl;

const G4double protective_thickness_TR		=  0*um;
const G4double phosphor_thickness_TR	       =  50*um;
const G4double support_thickness_TR              =  250*um;
const G4double magnetic_thickness_TR            =  160*um;
G4double Z_TR=protective_thickness_TR+phosphor_thickness_TR+support_thickness_TR+magnetic_thickness_TR;

const G4double X_TR	       =  10*cm;
const G4double Y_TR            =  10*cm;
const G4double d_stenope_TR_1=  4.5*m;

G4cout << "Computed tolerance = "
         << G4GeometryTolerance::GetInstance()->GetSurfaceTolerance()/cm
         << " cm" << G4endl;

// D�finition du volume TR_IP + placement
G4Box* TR_IP = new G4Box("TR_IP",X_TR/2,Y_TR/2,Z_TR/2);
TR_IP_log  = new G4LogicalVolume(TR_IP, vacuum,"TR_IP_log",0,0,0);
TR_IP_log->SetVisAttributes(G4VisAttributes::GetInvisible());
//TR_IP_log->SetVisAttributes(yellow);

// inside the TR_IP
//
// protective layer
//
//G4VSolid * protective_TR  = new G4Box("protective_TR",X_TR/2,Y_TR/2,protective_thickness_TR/2);
//protective_TR_log   = new G4LogicalVolume(protective_TR,EthenoneTR,"protective_TR_log",0,0,0);
//protective_TR_phys  = new G4PVPlacement(0,G4ThreeVector(0.,0.,+Z_TR/2-protective_thickness_TR/2),protective_TR_log,"protective_TR_phys",TR_IP_log,false,100);
//protective_TR_log->SetVisAttributes(turquoise);
//
// phosphor layer
//
G4VSolid * phosphor_TR  = new G4Box("phosphor_TR",X_TR/2,Y_TR/2,phosphor_thickness_TR/2);
phosphor_TR_log   = new G4LogicalVolume(phosphor_TR,mat_phosphor_TR,"phosphor_TR_log",0,0,0);
phosphor_TR_phys  = new G4PVPlacement(0,G4ThreeVector(0.,0.,+Z_TR/2-protective_thickness_TR-phosphor_thickness_TR/2),phosphor_TR_log,"phosphor_TR_phys",TR_IP_log,false,101);
phosphor_TR_log->SetVisAttributes(green);
//
// support layer
//
G4VSolid * support_TR  = new G4Box("support_TR",X_TR/2,Y_TR/2,support_thickness_TR/2);
support_TR_log   = new G4LogicalVolume(support_TR,Ethenone,"support_TR_log",0,0,0);
support_TR_phys  = new G4PVPlacement(0,G4ThreeVector(0.,0.,+Z_TR/2-protective_thickness_TR-phosphor_thickness_TR-support_thickness_TR/2),support_TR_log,"support_TR_phys",TR_IP_log,false,102);
support_TR_log->SetVisAttributes(orange);
//
// magnetic layer
//
G4VSolid * magnetic_TR  = new G4Box("magnetic_TR",X_TR/2,Y_TR/2,magnetic_thickness_TR/2);
magnetic_TR_log   = new G4LogicalVolume(magnetic_TR,mat_magnetic_MS_TR,"magnetic_TR_log",0,0,0);
magnetic_TR_phys  = new G4PVPlacement(0,G4ThreeVector(0.,0.,+Z_TR/2-protective_thickness_TR-phosphor_thickness_TR-support_thickness_TR-magnetic_thickness_TR/2),magnetic_TR_log,"magnetic_TR_phys",TR_IP_log,false,103);
magnetic_TR_log->SetVisAttributes(grey);

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
/// 	Fin de TR-IP
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
/// 	Placements
///     !!!!! replica number must be sorted !!!
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

const G4double pos_ref=-25.00*mm;

const G4double pos_PEHD=6.25*mm+pos_ref;
const G4double pos_al=-21.295*mm+pos_ref;
const G4double pos_ti=-22.3765*mm+pos_ref;
const G4double pos_fe=-23.4755*mm+pos_ref;
const G4double pos_cu=-24.562*mm+pos_ref;
const G4double pos_mo=-25.636*mm+pos_ref;
const G4double pos_ag=-26.735*mm+pos_ref;
const G4double pos_sn=-28.034*mm+pos_ref;
const G4double pos_ta=-29.508*mm+pos_ref;
const G4double pos_au=-31.482*mm+pos_ref;
const G4double pos_pb1=-33.706*mm+pos_ref;
const G4double pos_pb2=-36.18*mm+pos_ref;
const G4double pos_pb3=-39.654*mm+pos_ref;
const G4double pos_pb4=-44.128*mm+pos_ref;
const G4double pos_pb5=-50.102*mm+pos_ref;
const G4double pos_pb6=-57.076*mm+pos_ref;
const G4double pos_al_back=-67.5*mm+pos_ref;
const G4double pos_pb_back=-73.0*mm+pos_ref;

const G4double pos_blindage1=6.5*mm+pos_ref;
const G4double pos_blindage2=-11*mm+pos_ref;
const G4double pos_blindage3=-39*mm+pos_ref;
const G4double pos_blindage4=-43.5*mm+pos_ref;
const G4double pos_blindage5=-31.5*mm+pos_ref;
const G4double pos_blindage6av=15.5*mm+pos_ref;
const G4double pos_blindage6ar=-78.5*mm+pos_ref;

G4cout << pos_ref << G4endl;
G4cout << pos_PEHD << G4endl;
G4cout << pos_al << G4endl;
G4cout << pos_ti << G4endl;
G4cout << pos_fe << G4endl;
G4cout << pos_cu << G4endl;
G4cout << pos_mo << G4endl;
G4cout << pos_ag << G4endl;
G4cout << pos_sn << G4endl;
G4cout << pos_ta << G4endl;
G4cout << pos_au << G4endl;
G4cout << pos_pb1 << G4endl;
G4cout << pos_pb2 << G4endl;
G4cout << pos_pb3 << G4endl;
G4cout << pos_pb4 << G4endl;
G4cout << pos_pb5 << G4endl;
G4cout << pos_pb6 << G4endl;
G4cout << pos_al_back << G4endl;
G4cout << pos_pb_back << G4endl;

const G4double pos_ip0=-21.827*mm+pos_ref;
const G4double pos_ip1=-22.926*mm+pos_ref;
const G4double pos_ip2=-24.025*mm+pos_ref;
const G4double pos_ip3=-25.099*mm+pos_ref;
const G4double pos_ip4=-26.173*mm+pos_ref;
const G4double pos_ip5=-27.297*mm+pos_ref;
const G4double pos_ip6=-28.771*mm+pos_ref;
const G4double pos_ip7=-30.245*mm+pos_ref;
const G4double pos_ip8=-32.719*mm+pos_ref;
const G4double pos_ip9=-34.693*mm+pos_ref;
const G4double pos_ip10=-37.667*mm+pos_ref;
const G4double pos_ip11=-41.641*mm+pos_ref;
const G4double pos_ip12=-46.615*mm+pos_ref;
const G4double pos_ip13=-53.589*mm+pos_ref;
const G4double pos_ip14=-60.563*mm+pos_ref;

const G4double pos_mylar_f1=pos_al+ep_al/2.+ep_mylar/2.;
const G4double pos_mylar_b1=pos_al-ep_al/2.-ep_mylar/2.;
const G4double pos_mylar_f2=pos_ti+ep_ti/2.+ep_mylar/2.;
const G4double pos_mylar_b2=pos_ti-ep_ti/2.-ep_mylar/2.;
const G4double pos_mylar_f3=pos_fe+ep_fe/2.+ep_mylar/2.;
const G4double pos_mylar_b3=pos_fe-ep_fe/2.-ep_mylar/2.;
const G4double pos_mylar_f4=pos_cu+ep_cu/2.+ep_mylar/2.;
const G4double pos_mylar_b4=pos_cu-ep_cu/2.-ep_mylar/2.;
const G4double pos_mylar_f5=pos_mo+ep_mo/2.+ep_mylar/2.;
const G4double pos_mylar_b5=pos_mo-ep_mo/2.-ep_mylar/2.;
const G4double pos_mylar_f6=pos_ag+ep_ag/2.+ep_mylar/2.;
const G4double pos_mylar_b6=pos_ag-ep_ag/2.-ep_mylar/2.;
const G4double pos_mylar_f7=pos_sn+ep_sn/2.+ep_mylar/2.;
const G4double pos_mylar_b7=pos_sn-ep_sn/2.-ep_mylar/2.;
const G4double pos_mylar_f8=pos_ta+ep_ta/2.+ep_mylar/2.;
const G4double pos_mylar_b8=pos_ta-ep_ta/2.-ep_mylar/2.;
const G4double pos_mylar_f9=pos_au+ep_au/2.+ep_mylar/2.;
const G4double pos_mylar_b9=pos_au-ep_au/2.-ep_mylar/2.;
const G4double pos_mylar_f10=pos_pb1+ep_pb1/2.+ep_mylar/2.;
const G4double pos_mylar_b10=pos_pb1-ep_pb1/2.-ep_mylar/2.;
const G4double pos_mylar_f11=pos_pb2+ep_pb2/2.+ep_mylar/2.;
const G4double pos_mylar_b11=pos_pb2-ep_pb2/2.-ep_mylar/2.;
const G4double pos_mylar_f12=pos_pb3+ep_pb3/2.+ep_mylar/2.;
const G4double pos_mylar_b12=pos_pb3-ep_pb3/2.-ep_mylar/2.;
const G4double pos_mylar_f13=pos_pb4+ep_pb4/2.+ep_mylar/2.;
const G4double pos_mylar_b13=pos_pb4-ep_pb4/2.-ep_mylar/2.;
const G4double pos_mylar_f14=pos_pb5+ep_pb5/2.+ep_mylar/2.;
const G4double pos_mylar_b14=pos_pb5-ep_pb5/2.-ep_mylar/2.;
const G4double pos_mylar_f15=pos_pb6+ep_pb6/2.+ep_mylar/2.;
const G4double pos_mylar_b15=pos_pb6-ep_pb6/2.-ep_mylar/2.;

// placement des filtres

filtre_PEHD_phys= new G4PVPlacement(0,  G4ThreeVector(0,0,pos_PEHD),  filtre_PEHD_log,  "filtre_PEHD_0",   experimentalHall_log, false,  999);
filtre_al_phys= new G4PVPlacement(0,  G4ThreeVector(0,0,pos_al),  filtre_al_log,  "filtre_al_0",   experimentalHall_log, false,  999);
filtre_ti_phys= new G4PVPlacement(0,  G4ThreeVector(0,0,pos_ti),  filtre_ti_log,  "filtre_ti_0",   experimentalHall_log, false,  999);
filtre_fe_phys= new G4PVPlacement(0,  G4ThreeVector(0,0,pos_fe),  filtre_fe_log,  "filtre_fe_0",   experimentalHall_log, false,  999);
filtre_cu_phys= new G4PVPlacement(0,  G4ThreeVector(0,0,pos_cu),  filtre_cu_log,  "filtre_cu_0",   experimentalHall_log, false,  999);
filtre_mo_phys= new G4PVPlacement(0,  G4ThreeVector(0,0,pos_mo),  filtre_mo_log,  "filtre_mo_0",   experimentalHall_log, false,  999);
filtre_ag_phys= new G4PVPlacement(0,  G4ThreeVector(0,0,pos_ag),  filtre_ag_log,  "filtre_ag_0",   experimentalHall_log, false,  999);
filtre_sn_phys= new G4PVPlacement(0,  G4ThreeVector(0,0,pos_sn),  filtre_sn_log,  "filtre_sn_0",   experimentalHall_log, false,  999);
filtre_ta_phys= new G4PVPlacement(0,  G4ThreeVector(0,0,pos_ta),  filtre_ta_log,  "filtre_ta_0",   experimentalHall_log, false,  999);
filtre_au_phys= new G4PVPlacement(0,  G4ThreeVector(0,0,pos_au),  filtre_au_log,  "filtre_au_0",   experimentalHall_log, false,  999);
filtre_pb1_phys= new G4PVPlacement(0,  G4ThreeVector(0,0,pos_pb1),  filtre_pb1_log,  "filtre_pb_1",   experimentalHall_log, false,  999);
filtre_pb2_phys= new G4PVPlacement(0,  G4ThreeVector(0,0,pos_pb2),  filtre_pb2_log,  "filtre_pb_2",   experimentalHall_log, false,  999);
filtre_pb3_phys= new G4PVPlacement(0,  G4ThreeVector(0,0,pos_pb3),  filtre_pb3_log,  "filtre_pb_3",   experimentalHall_log, false,  999);
filtre_pb4_phys= new G4PVPlacement(0,  G4ThreeVector(0,0,pos_pb4),  filtre_pb4_log,  "filtre_pb_4",   experimentalHall_log, false,  999);
filtre_pb5_phys= new G4PVPlacement(0,  G4ThreeVector(0,0,pos_pb5),  filtre_pb5_log,  "filtre_pb_5",   experimentalHall_log, false,  999);
filtre_pb6_phys= new G4PVPlacement(0,  G4ThreeVector(0,0,pos_pb6),  filtre_pb6_log,  "filtre_pb_6",   experimentalHall_log, false,  999);
filtre_al_back_phys= new G4PVPlacement(0,  G4ThreeVector(0,0,pos_al_back),  filtre_al_back_log,  "filtre_al_back",   experimentalHall_log, false,  999);
filtre_pb_back_phys= new G4PVPlacement(0,  G4ThreeVector(0,0,pos_pb_back),  filtre_pb_back_log,  "filtre_pb_back",   experimentalHall_log, false,  999);

// placement des IPs

MS_IP_phys = new G4PVPlacement(0,  G4ThreeVector(0,0,pos_ip0),  MS_IP_log,  "MS_IP_0",   experimentalHall_log, false,  0);
MS_IP_phys = new G4PVPlacement(0,  G4ThreeVector(0,0,pos_ip1),  MS_IP_log,  "MS_IP_1",   experimentalHall_log, false,  1);
MS_IP_phys = new G4PVPlacement(0,  G4ThreeVector(0,0,pos_ip2),  MS_IP_log,  "MS_IP_2",   experimentalHall_log, false,  2);
MS_IP_phys = new G4PVPlacement(0,  G4ThreeVector(0,0,pos_ip3),  MS_IP_log,  "MS_IP_3",   experimentalHall_log, false,  3);
MS_IP_phys = new G4PVPlacement(0,  G4ThreeVector(0,0,pos_ip4),  MS_IP_log,  "MS_IP_4",   experimentalHall_log, false,  4);
MS_IP_phys = new G4PVPlacement(0,  G4ThreeVector(0,0,pos_ip5),  MS_IP_log,  "MS_IP_5",   experimentalHall_log, false,  5);
MS_IP_phys = new G4PVPlacement(0,  G4ThreeVector(0,0,pos_ip6),  MS_IP_log,  "MS_IP_6",   experimentalHall_log, false,  6);
MS_IP_phys = new G4PVPlacement(0,  G4ThreeVector(0,0,pos_ip7),  MS_IP_log,  "MS_IP_7",   experimentalHall_log, false,  7);
MS_IP_phys = new G4PVPlacement(0,  G4ThreeVector(0,0,pos_ip8),  MS_IP_log,  "MS_IP_8",   experimentalHall_log, false,  8);
MS_IP_phys = new G4PVPlacement(0,  G4ThreeVector(0,0,pos_ip9),  MS_IP_log,  "MS_IP_9",   experimentalHall_log, false,  9);
MS_IP_phys = new G4PVPlacement(0,  G4ThreeVector(0,0,pos_ip10),  MS_IP_log,  "MS_IP_10",   experimentalHall_log, false,  10);
MS_IP_phys = new G4PVPlacement(0,  G4ThreeVector(0,0,pos_ip11),  MS_IP_log,  "MS_IP_11",   experimentalHall_log, false,  11);
MS_IP_phys = new G4PVPlacement(0,  G4ThreeVector(0,0,pos_ip12),  MS_IP_log,  "MS_IP_12",   experimentalHall_log, false,  12);
MS_IP_phys = new G4PVPlacement(0,  G4ThreeVector(0,0,pos_ip13),  MS_IP_log,  "MS_IP_13",   experimentalHall_log, false,  13);
MS_IP_phys = new G4PVPlacement(0,  G4ThreeVector(0,0,pos_ip14),  MS_IP_log,  "MS_IP_14",   experimentalHall_log, false,  14);

// placement des films plastiques

filtre_f1_phys= new G4PVPlacement(0,  G4ThreeVector(0,0,pos_mylar_f1),  filtre_mylar_log,  "filtre_f1",   experimentalHall_log, false,  999);
filtre_f2_phys= new G4PVPlacement(0,  G4ThreeVector(0,0,pos_mylar_f2),  filtre_mylar_log,  "filtre_f2",   experimentalHall_log, false,  999);
filtre_f3_phys= new G4PVPlacement(0,  G4ThreeVector(0,0,pos_mylar_f3),  filtre_mylar_log,  "filtre_f3",   experimentalHall_log, false,  999);
filtre_f4_phys= new G4PVPlacement(0,  G4ThreeVector(0,0,pos_mylar_f4),  filtre_mylar_log,  "filtre_f4",   experimentalHall_log, false,  999);
filtre_f5_phys= new G4PVPlacement(0,  G4ThreeVector(0,0,pos_mylar_f5),  filtre_mylar_log,  "filtre_f5",   experimentalHall_log, false,  999);
filtre_f6_phys= new G4PVPlacement(0,  G4ThreeVector(0,0,pos_mylar_f6),  filtre_mylar_log,  "filtre_f6",   experimentalHall_log, false,  999);
filtre_f7_phys= new G4PVPlacement(0,  G4ThreeVector(0,0,pos_mylar_f7),  filtre_mylar_log,  "filtre_f7",   experimentalHall_log, false,  999);
filtre_f8_phys= new G4PVPlacement(0,  G4ThreeVector(0,0,pos_mylar_f8),  filtre_mylar_log,  "filtre_f8",   experimentalHall_log, false,  999);
filtre_f9_phys= new G4PVPlacement(0,  G4ThreeVector(0,0,pos_mylar_f9),  filtre_mylar_log,  "filtre_f9",   experimentalHall_log, false,  999);
filtre_f10_phys= new G4PVPlacement(0,  G4ThreeVector(0,0,pos_mylar_f10),  filtre_mylar_log,  "filtre_f10",   experimentalHall_log, false,  999);
filtre_f11_phys= new G4PVPlacement(0,  G4ThreeVector(0,0,pos_mylar_f11),  filtre_mylar_log,  "filtre_f11",   experimentalHall_log, false,  999);
filtre_f12_phys= new G4PVPlacement(0,  G4ThreeVector(0,0,pos_mylar_f12),  filtre_mylar_log,  "filtre_f12",   experimentalHall_log, false,  999);
filtre_f13_phys= new G4PVPlacement(0,  G4ThreeVector(0,0,pos_mylar_f13),  filtre_mylar_log,  "filtre_f13",   experimentalHall_log, false,  999);
filtre_f14_phys= new G4PVPlacement(0,  G4ThreeVector(0,0,pos_mylar_f14),  filtre_mylar_log,  "filtre_f14",   experimentalHall_log, false,  999);
filtre_f15_phys= new G4PVPlacement(0,  G4ThreeVector(0,0,pos_mylar_f15),  filtre_mylar_log,  "filtre_f15",   experimentalHall_log, false,  999);

filtre_b1_phys= new G4PVPlacement(0,  G4ThreeVector(0,0,pos_mylar_b1),  filtre_mylar_log,  "filtre_b1",   experimentalHall_log, false,  999);
filtre_b2_phys= new G4PVPlacement(0,  G4ThreeVector(0,0,pos_mylar_b2),  filtre_mylar_log,  "filtre_b2",   experimentalHall_log, false,  999);
filtre_b3_phys= new G4PVPlacement(0,  G4ThreeVector(0,0,pos_mylar_b3),  filtre_mylar_log,  "filtre_b3",   experimentalHall_log, false,  999);
filtre_b4_phys= new G4PVPlacement(0,  G4ThreeVector(0,0,pos_mylar_b4),  filtre_mylar_log,  "filtre_b4",   experimentalHall_log, false,  999);
filtre_b5_phys= new G4PVPlacement(0,  G4ThreeVector(0,0,pos_mylar_b5),  filtre_mylar_log,  "filtre_b5",   experimentalHall_log, false,  999);
filtre_b6_phys= new G4PVPlacement(0,  G4ThreeVector(0,0,pos_mylar_b6),  filtre_mylar_log,  "filtre_b6",   experimentalHall_log, false,  999);
filtre_b7_phys= new G4PVPlacement(0,  G4ThreeVector(0,0,pos_mylar_b7),  filtre_mylar_log,  "filtre_b7",   experimentalHall_log, false,  999);
filtre_b8_phys= new G4PVPlacement(0,  G4ThreeVector(0,0,pos_mylar_b8),  filtre_mylar_log,  "filtre_b8",   experimentalHall_log, false,  999);
filtre_b9_phys= new G4PVPlacement(0,  G4ThreeVector(0,0,pos_mylar_b9),  filtre_mylar_log,  "filtre_b9",   experimentalHall_log, false,  999);
filtre_b10_phys= new G4PVPlacement(0,  G4ThreeVector(0,0,pos_mylar_b10),  filtre_mylar_log,  "filtre_b10",   experimentalHall_log, false,  999);
filtre_b11_phys= new G4PVPlacement(0,  G4ThreeVector(0,0,pos_mylar_b11),  filtre_mylar_log,  "filtre_b11",   experimentalHall_log, false,  999);
filtre_b12_phys= new G4PVPlacement(0,  G4ThreeVector(0,0,pos_mylar_b12),  filtre_mylar_log,  "filtre_b12",   experimentalHall_log, false,  999);
filtre_b13_phys= new G4PVPlacement(0,  G4ThreeVector(0,0,pos_mylar_b13),  filtre_mylar_log,  "filtre_b13",   experimentalHall_log, false,  999);
filtre_b14_phys= new G4PVPlacement(0,  G4ThreeVector(0,0,pos_mylar_b14),  filtre_mylar_log,  "filtre_b14",   experimentalHall_log, false,  999);
filtre_b15_phys= new G4PVPlacement(0,  G4ThreeVector(0,0,pos_mylar_b15),  filtre_mylar_log,  "filtre_b15",   experimentalHall_log, false,  999);

// placement du blindage

blindage_1_phys= new G4PVPlacement(0,  G4ThreeVector(0,0,pos_blindage1),  blindage_1_log,  "blindage_1",   experimentalHall_log, false,  999);
blindage_2_phys= new G4PVPlacement(0,  G4ThreeVector(0,0,pos_blindage2),  blindage_2_log,  "blindage_2",   experimentalHall_log, false,  999);
blindage_3_phys= new G4PVPlacement(0,  G4ThreeVector(0,0,pos_blindage3),  blindage_3_log,  "blindage_3",   experimentalHall_log, false,  999);
blindage_4_phys= new G4PVPlacement(0,  G4ThreeVector(0,0,pos_blindage4),  blindage_4_log,  "blindage_4",   experimentalHall_log, false,  999);
blindage_5_phys= new G4PVPlacement(0,  G4ThreeVector(0,0,pos_blindage5),  blindage_5_log,  "blindage_5",   experimentalHall_log, false,  999);
blindage_6av_phys= new G4PVPlacement(0,  G4ThreeVector(0,0,pos_blindage6av),  blindage_6_log,  "blindage_6av",   experimentalHall_log, false,  999);
blindage_6ar_phys= new G4PVPlacement(0,  G4ThreeVector(0,0,pos_blindage6ar),  blindage_6_log,  "blindage_6ar",   experimentalHall_log, false,  999);

 return experimentalHall_phys;
}
