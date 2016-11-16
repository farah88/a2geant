
#ifndef A2DetCherenkov_h
#define A2DetCherenkov_h 1

#include "A2Detector.hh"

class A2DetCherenkov : public A2Detector
{
public:
  
  A2DetCherenkov();
  ~A2DetCherenkov();

  G4VPhysicalVolume* Construct(G4LogicalVolume *MotherLogic);
  void CheckOverlapAndAbort(G4VPhysicalVolume* vol, const G4String& location);
  void MakeCherenkov();

  private:
  G4bool fIsCheckOverlap;

  G4LogicalVolume *fAirBoxLogic;
  G4LogicalVolume *fAlBoxLogic;
  G4LogicalVolume *fbackAlBoxLogic;
  G4LogicalVolume *finnerboxvolumeLogic;
  G4LogicalVolume *fsmallboxLogic;
  G4LogicalVolume *ftriAlLogic;
  G4LogicalVolume *finnertriLogic;

  G4LogicalVolume *fbackwindowdownLogic;
  G4LogicalVolume *fbackwindowrightLogic;
  G4LogicalVolume *fbackwindowupLogic;
  G4LogicalVolume *fbackwindowleftLogic;

  G4LogicalVolume *fbackwindowMylardownLogic;
  G4LogicalVolume *fbackwindowMylarrightLogic;
  G4LogicalVolume *fbackwindowMylarupLogic;
  G4LogicalVolume *fbackwindowMylarleftLogic;

  G4LogicalVolume *fbackwindowTetlardownLogic;
  G4LogicalVolume *fbackwindowTetlarrightLogic;
  G4LogicalVolume *fbackwindowTetlarupLogic;
  G4LogicalVolume *fbackwindowTetlarleftLogic;

  G4LogicalVolume *frontwindowupperLogic;
  G4LogicalVolume *frontwindowlowerLogic;

  G4LogicalVolume *fnoseupLogic;
  G4LogicalVolume *fnosedownLogic;
  G4LogicalVolume *fnoseinnerupLogic;
  G4LogicalVolume *fnoseinnerdownLogic;

  G4VPhysicalVolume *fAirBoxPhysi;
  G4VPhysicalVolume *fAlBoxPhysi;
  G4VPhysicalVolume *fbackAlBoxPhysi;
  G4VPhysicalVolume *finnerboxvolumePhysi;
  G4VPhysicalVolume *fsmallboxPhysi;
  G4VPhysicalVolume *ftriAlPhysi;
  G4VPhysicalVolume *finnertriPhysi;

  G4VPhysicalVolume *fbackwindowdownPhysi;
  G4VPhysicalVolume *fbackwindowrightPhysi;
  G4VPhysicalVolume *fbackwindowupPhysi;
  G4VPhysicalVolume *fbackwindowleftPhysi;

  G4VPhysicalVolume *fbackwindowMylardownPhysi;
  G4VPhysicalVolume *fbackwindowMylarrightPhysi;
  G4VPhysicalVolume *fbackwindowMylarupPhysi;
  G4VPhysicalVolume *fbackwindowMylarleftPhysi;

  G4VPhysicalVolume *fbackwindowTetlardownPhysi;
  G4VPhysicalVolume *fbackwindowTetlarrightPhysi;
  G4VPhysicalVolume *fbackwindowTetlarupPhysi;
  G4VPhysicalVolume *fbackwindowTetlarleftPhysi;

  G4VPhysicalVolume *frontwindowupperPhysi;
  G4VPhysicalVolume *frontwindowlowerPhysi;

  G4VPhysicalVolume *fnoseupPhysi;
  G4VPhysicalVolume *fnosedownPhysi;
  G4VPhysicalVolume *fnoseinnerupPhysi;
  G4VPhysicalVolume *fnoseinnerdownPhysi;

  G4LogicalVolume *ffrontwindow_noseup_mylarLogic;
  G4VPhysicalVolume *ffrontwindow_noseup_mylarPhysi;
  G4LogicalVolume *ffrontwindow_nosedown_mylarLogic;
  G4VPhysicalVolume *ffrontwindow_nosedown_mylarPhysi;

  G4LogicalVolume *ffrontwindow_nose_upperhalf_tetlarLogic;
  G4VPhysicalVolume *ffrontwindow_nose_upperhalf_tetlarPhysi;
  G4LogicalVolume *ffrontwindow_nose_lowerhalf_tetlarLogic;
  G4VPhysicalVolume *ffrontwindow_nose_lowerhalf_tetlarPhysi;

  G4LogicalVolume *fnoseframeupLogic;
  G4LogicalVolume *fnoseframedownLogic;
  G4VPhysicalVolume *fnoseframeupPhysi;
  G4VPhysicalVolume *fnoseframedownPhysi;

  G4LogicalVolume *fnoseframeinnerupLogic;
  G4LogicalVolume *fnoseframeinnerdownLogic;
  G4VPhysicalVolume *fnoseframeinnerupPhysi;
  G4VPhysicalVolume *fnoseframeinnerdownPhysi;

 G4LogicalVolume *fmirrorLogic;
 G4VPhysicalVolume *fmirrorPhysi;

};

#endif
