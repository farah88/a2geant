
#ifndef A2DetCherenkov_h
#define A2DetCherenkov_h 1

#include "A2Detector.hh"

class A2DetCherenkov : public A2Detector
{
public:
  
  A2DetCherenkov();
  ~A2DetCherenkov();

  G4VPhysicalVolume* Construct(G4LogicalVolume *MotherLogic);

  //G4UnionSolid* Make_inner_hexagon_large(G4double thickness, G4double thickness_z, G4String name);

  private:
  G4LogicalVolume *fAlBoxLogic;
  G4LogicalVolume *finnerboxvolumeLogic;

  G4LogicalVolume *fbackwindowLogic;
  G4LogicalVolume *fbackwindowMylarLogic;
  G4LogicalVolume *fbackwindowTetlarLogic;

  G4LogicalVolume *frontwindowLogic;

  G4LogicalVolume *fnoseLogic;
  G4LogicalVolume *fnoseinnerLogic;

  G4VPhysicalVolume *fAlBoxPhysi;
  G4VPhysicalVolume *finnerboxvolumePhysi;

  G4VPhysicalVolume *fbackwindowPhysi;
  G4VPhysicalVolume *fbackwindowMylarPhysi;
  G4VPhysicalVolume *fbackwindowTetlarPhysi;

  G4VPhysicalVolume *frontwindowPhysi;

  G4VPhysicalVolume *fnosePhysi;
  G4VPhysicalVolume *fnoseinnerPhysi;

  G4LogicalVolume *ffrontwindow_nose_mylarLogic;
  G4VPhysicalVolume *ffrontwindow_nose_mylarPhysi;
  G4LogicalVolume *ffrontwindow_nose_tetlarLogic;
  G4VPhysicalVolume *ffrontwindow_nose_tetlarPhysi;

  G4LogicalVolume *fnoseframeLogic;
  G4VPhysicalVolume *fnoseframePhysi;

  G4LogicalVolume *fnoseframeinnerLogic;
  G4VPhysicalVolume *fnoseframeinnerPhysi;

};

#endif
