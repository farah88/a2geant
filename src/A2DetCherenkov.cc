#include "A2DetCherenkov.hh"
#include "G4Box.hh"
#include "G4Trd.hh"
#include "G4Trap.hh"
#include "G4IntersectionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4PVPlacement.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4VisAttributes.hh"
#include "G4SubtractionSolid.hh"
#include "G4GenericTrap.hh"
#include "G4Sphere.hh"
#include "G4Ellipsoid.hh"
#include "TMath.h"

A2DetCherenkov::A2DetCherenkov()
{
    fIsCheckOverlap=kFALSE;
}
A2DetCherenkov::~A2DetCherenkov()
{

}


//______________________________________________________________________________
void A2DetCherenkov::CheckOverlapAndAbort(G4VPhysicalVolume* vol, const G4String& location)
{
    // Check if the volume 'vol' has overlaps and abort if that is the case.
    // Use 'location' in the error message to indicate the origin of the
    // problem.

    // check for overlaps
    if (vol->CheckOverlaps())
    {
        G4cout << location << ": Overlap in volume " << vol->GetName() << " detected!" << G4endl;
        exit(1);
    }
}


G4VPhysicalVolume* A2DetCherenkov::Construct(G4LogicalVolume* MotherLogical){
    fMotherLogic=MotherLogical;

    //make the Cherenkov detector
    MakeCherenkov();

    //check for overlaps
    if (fIsCheckOverlap){
        CheckOverlapAndAbort(fAirBoxPhysi, "A2DetCherenkov::Construct()");
        CheckOverlapAndAbort(fnoseupPhysi, "A2DetCherenkov::Construct()");
        CheckOverlapAndAbort(fnosedownPhysi, "A2DetCherenkov::Construct()");
    }

    return fMyPhysi;
}

void A2DetCherenkov::MakeCherenkov(){
    //Colours with thier corresponding materials used in the visualization:
    G4VisAttributes* GrayVisAtt= new G4VisAttributes(G4Colour(0.8,0.8,0.8)); //frame for nose(gry)
    G4VisAttributes* BrownVisAtt= new G4VisAttributes(G4Colour(0.8,0.6,0.2)); //(brown)
    G4VisAttributes* CyanVisAtt= new G4VisAttributes(G4Colour(0.0, 1.0, 1.0)); // Mylar foil window
    G4VisAttributes* MagentaVisAtt= new G4VisAttributes(G4Colour(1.0,0.0,1.0)); //Tedlar foil
    G4VisAttributes* BlueVisAtt= new G4VisAttributes(G4Colour(0.0,0.0,1.0)); //PVC nose
    G4VisAttributes* GreenVisAtt= new G4VisAttributes(G4Colour(0.0,1.0,0.0)); //Cherenkov gas
    G4VisAttributes* RedVisAtt= new G4VisAttributes(G4Colour(1.0, 0.0, 0.0)); //Al box
    G4VisAttributes* WhiteVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0)); //

// Al frame of Cherenkov detector, consisting of large box, triangular shaped trapezoid at top right corner in xz-plane
    G4Box* airbox=new G4Box("airbox",72.5*cm,60.0*cm,36.0*cm);// the main box part of Cherenkov will be placed this box of air

    fAirBoxLogic=new G4LogicalVolume(airbox,fNistManager->FindOrBuildMaterial("G4_AIR"),"airbox");
    fAirBoxPhysi=new G4PVPlacement(0, G4ThreeVector(-3.0*cm,0.0*cm,133.*cm), fAirBoxLogic, "airbox", fMotherLogic, false, 0);
//    fAirBoxLogic->SetVisAttributes(RedVisAtt);
    fAirBoxLogic->SetVisAttributes(G4VisAttributes::Invisible);

    //CheckOverlapAndAbort(fAirBoxLogic, "A2DetCherenkov::Construct()");//if (fIsCheckOverlap)

    G4Box* centralbox=new G4Box("centralbox",60.0*cm,60.0*cm,35.9*cm);//box does not contain the Al backplate of 2mm thickness

    fAlBoxLogic=new G4LogicalVolume(centralbox,fNistManager->FindOrBuildMaterial("G4_Al"),"centralbox");
    fAlBoxPhysi=new G4PVPlacement(0, G4ThreeVector(-12.5*cm,0.0*cm,-0.1*cm), fAlBoxLogic, "centralbox", fAirBoxLogic, false, 0);
    fAlBoxLogic->SetVisAttributes(RedVisAtt);
    //fAlBoxLogic->SetVisAttributes(G4VisAttributes::Invisible);

    //the back plate made of Al
    G4Box* backAlbox=new G4Box("backAlbox",72.5*cm,60.0*cm,0.1*cm);//Al backplate of 2mm thickness in z direction

    fbackAlBoxLogic=new G4LogicalVolume(backAlbox,fNistManager->FindOrBuildMaterial("G4_Al"),"backAlbox");
    fbackAlBoxPhysi=new G4PVPlacement(0, G4ThreeVector(0.0*cm,0.0*cm,35.9*cm), fbackAlBoxLogic, "backAlbox", fAirBoxLogic, false, 0);
    fbackAlBoxLogic->SetVisAttributes(RedVisAtt);
    //fbackAlBoxLogic->SetVisAttributes(G4VisAttributes::Invisible);

    //the inner volume of the box which is filled with Cherenkov gas
    G4Box* centralinnerbox=new G4Box("centralinnerbox",59.8*cm,59.8*cm,35.6*cm);//thickness of Al frame is 6mm (front plate) or 2mm (rest)

    finnerboxvolumeLogic=new G4LogicalVolume(centralinnerbox,fNistManager->FindOrBuildMaterial("A2_Cherenkovgas"),"centralinnerbox");
    finnerboxvolumePhysi=new G4PVPlacement(0, G4ThreeVector(0.0*cm,0.0*cm,0.3*cm), finnerboxvolumeLogic, "centralinnerbox", fAlBoxLogic, false, 0);
    finnerboxvolumeLogic->SetVisAttributes(GreenVisAtt);
    //finnerboxvolumeLogic->SetVisAttributes(G4VisAttributes::Invisible);

    //need a small box to cut the Al frame on the left side (view from target) where the triangular shape is located
    G4double zhalflength_innertri= (45.*cm - 0.2*cm - 0.2*cm/(TMath::Sin(TMath::ATan(21./45.))))/2.;
    G4Box* smallbox=new G4Box("smallbox",0.1*cm,59.8*cm,zhalflength_innertri);

    fsmallboxLogic=new G4LogicalVolume(smallbox,fNistManager->FindOrBuildMaterial("A2_Cherenkovgas"),"smallbox");
    fsmallboxPhysi=new G4PVPlacement(0, G4ThreeVector(59.9*cm,0.0*cm,13.6*cm), fsmallboxLogic, "smallbox", fAlBoxLogic, false, 0);
    fsmallboxLogic->SetVisAttributes(GreenVisAtt);
    //   fsmallboxLogic->SetVisAttributes(G4VisAttributes::Invisible);



//now the triangular shaped trapezoid which is located on the left side
    std::vector<G4TwoVector> vertices_tri;

    vertices_tri.push_back(G4TwoVector(-10.5*cm,-60.0*cm));
    vertices_tri.push_back(G4TwoVector(-10.5*cm,-60.0*cm));
    vertices_tri.push_back(G4TwoVector(-10.5*cm,60.0*cm));
    vertices_tri.push_back(G4TwoVector(-10.5*cm,60.0*cm));
    vertices_tri.push_back(G4TwoVector(10.5*cm,-60.0*cm));
    vertices_tri.push_back(G4TwoVector(-10.5*cm,-60.0*cm));
    vertices_tri.push_back(G4TwoVector(-10.5*cm,60.0*cm));
    vertices_tri.push_back(G4TwoVector(10.5*cm,60.0*cm));

    G4GenericTrap* tri= new G4GenericTrap("tri",22.4*cm,vertices_tri);

    ftriAlLogic=new G4LogicalVolume(tri,fNistManager->FindOrBuildMaterial("G4_Al"),"tri");
    ftriAlPhysi=new G4PVPlacement(0, G4ThreeVector(58.*cm,0.0*cm,13.4*cm), ftriAlLogic, "tri", fAirBoxLogic, false, 0);
    ftriAlLogic->SetVisAttributes(RedVisAtt);

    G4RotationMatrix* rm = new G4RotationMatrix();
    rm->rotateZ(0.*deg);

    //subtract the inner volume of the triangle to get the 2mm and 6mm thick Al frame of the triangle
    std::vector<G4TwoVector> vertices_innertri;

    vertices_innertri.push_back(G4TwoVector(-10.5*cm,-59.8*cm));
    vertices_innertri.push_back(G4TwoVector(-10.5*cm,-59.8*cm));
    vertices_innertri.push_back(G4TwoVector(-10.5*cm,59.8*cm));
    vertices_innertri.push_back(G4TwoVector(-10.5*cm,59.8*cm));
    vertices_innertri.push_back(G4TwoVector(10.279326*cm,-59.8*cm));
    vertices_innertri.push_back(G4TwoVector(-10.5*cm,-59.8*cm));
    vertices_innertri.push_back(G4TwoVector(-10.5*cm,59.8*cm));
    vertices_innertri.push_back(G4TwoVector(10.279326*cm,59.8*cm));

    G4GenericTrap* innertri= new G4GenericTrap("innertri",(zhalflength_innertri+(0.2*cm/(TMath::Sin(TMath::ATan(21./45.))))/2.),vertices_innertri);

    finnertriLogic=new G4LogicalVolume(innertri,fNistManager->FindOrBuildMaterial("A2_Cherenkovgas"),"innertri");
    finnertriPhysi=new G4PVPlacement(0, G4ThreeVector(0.0*cm,0.0*cm,0.0*cm), finnertriLogic, "innertri", ftriAlLogic, false, 0);
    finnertriLogic->SetVisAttributes(GreenVisAtt);

    //now the backside of the Cherenkov detector (septagonal shape is divided into 4 parts in order to use G4GenericTrap):
    std::vector<G4TwoVector> vertices_backwindow_left;

    vertices_backwindow_left.push_back(G4TwoVector(-38.5*cm,54.0*cm));
    vertices_backwindow_left.push_back(G4TwoVector(0.0*cm,0.0*cm));
    vertices_backwindow_left.push_back(G4TwoVector(-30.5*cm,-53.5*cm));
    vertices_backwindow_left.push_back(G4TwoVector(-70.0*cm,0.0*cm));
    vertices_backwindow_left.push_back(G4TwoVector(-38.5*cm,54.0*cm));
    vertices_backwindow_left.push_back(G4TwoVector(0.0*cm,0.0*cm));
    vertices_backwindow_left.push_back(G4TwoVector(-30.5*cm,-53.5*cm));
    vertices_backwindow_left.push_back(G4TwoVector(-70.0*cm,0.0*cm));

    G4GenericTrap* backwindow_left_tocutfromAlframe= new G4GenericTrap("backwindow_left_tocutfromAlframe",1.88/2.*mm,vertices_backwindow_left);
    G4GenericTrap* backwindow_left_mylar= new G4GenericTrap("backwindow_left_mylar",0.07/2.*mm,vertices_backwindow_left);
    G4GenericTrap* backwindow_left_tetlar= new G4GenericTrap("backwindow_left_tetlar",0.05/2.*mm,vertices_backwindow_left);

    std::vector<G4TwoVector> vertices_backwindow_up;

    vertices_backwindow_up.push_back(G4TwoVector(-38.5*cm,54.0*cm));
    vertices_backwindow_up.push_back(G4TwoVector(33.5*cm,54.0*cm));
    vertices_backwindow_up.push_back(G4TwoVector(0.0*cm,0.0*cm));
    vertices_backwindow_up.push_back(G4TwoVector(0.0*cm,0.0*cm));
    vertices_backwindow_up.push_back(G4TwoVector(-38.5*cm,54.0*cm));
    vertices_backwindow_up.push_back(G4TwoVector(33.5*cm,54.0*cm));
    vertices_backwindow_up.push_back(G4TwoVector(0.0*cm,0.0*cm));
    vertices_backwindow_up.push_back(G4TwoVector(0.0*cm,0.0*cm));

    G4GenericTrap* backwindow_up_tocutfromAlframe= new G4GenericTrap("backwindow_up_tocutfromAlframe",1.88/2.*mm,vertices_backwindow_up);
    G4GenericTrap* backwindow_up_mylar= new G4GenericTrap("backwindow_up_mylar",0.07/2.*mm,vertices_backwindow_up);
    G4GenericTrap* backwindow_up_tetlar= new G4GenericTrap("backwindow_up_tetlar",0.05/2.*mm,vertices_backwindow_up);

    std::vector<G4TwoVector> vertices_backwindow_right;

    vertices_backwindow_right.push_back(G4TwoVector(33.5*cm,54.0*cm));
    vertices_backwindow_right.push_back(G4TwoVector(61.0*cm,19.5*cm));
    vertices_backwindow_right.push_back(G4TwoVector(61.0*cm,-19.5*cm));
    vertices_backwindow_right.push_back(G4TwoVector(0.0*cm,0.0*cm));
    vertices_backwindow_right.push_back(G4TwoVector(33.5*cm,54.0*cm));
    vertices_backwindow_right.push_back(G4TwoVector(61.0*cm,19.5*cm));
    vertices_backwindow_right.push_back(G4TwoVector(61.0*cm,-19.5*cm));
    vertices_backwindow_right.push_back(G4TwoVector(0.0*cm,0.0*cm));

    G4GenericTrap* backwindow_right_tocutfromAlframe= new G4GenericTrap("backwindow_right_tocutfromAlframe",1.88/2.*mm,vertices_backwindow_right);
    G4GenericTrap* backwindow_right_mylar= new G4GenericTrap("backwindow_right_mylar",0.07/2.*mm,vertices_backwindow_right);
    G4GenericTrap* backwindow_right_tetlar= new G4GenericTrap("backwindow_right_tetlar",0.05/2.*mm,vertices_backwindow_right);

    std::vector<G4TwoVector> vertices_backwindow_down;

    vertices_backwindow_down.push_back(G4TwoVector(0.0*cm,0.0*cm));
    vertices_backwindow_down.push_back(G4TwoVector(61.0*cm,-19.5*cm));
    vertices_backwindow_down.push_back(G4TwoVector(42.5*cm,-53.5*cm));
    vertices_backwindow_down.push_back(G4TwoVector(-30.5*cm,-53.5*cm));
    vertices_backwindow_down.push_back(G4TwoVector(0.0*cm,0.0*cm));
    vertices_backwindow_down.push_back(G4TwoVector(61.0*cm,-19.5*cm));
    vertices_backwindow_down.push_back(G4TwoVector(42.5*cm,-53.5*cm));
    vertices_backwindow_down.push_back(G4TwoVector(-30.5*cm,-53.5*cm));

    G4GenericTrap* backwindow_down_tocutfromAlframe= new G4GenericTrap("backwindow_down_tocutfromAlframe",1.88/2.*mm,vertices_backwindow_down);
    G4GenericTrap* backwindow_down_mylar= new G4GenericTrap("backwindow_down_mylar",0.07/2.*mm,vertices_backwindow_down);
    G4GenericTrap* backwindow_down_tetlar= new G4GenericTrap("backwindow_down_tetlar",0.05/2.*mm,vertices_backwindow_down);

    //backwindow which is cut from Al frame and is part of the inner volume filled with Cherenkov gas
    fbackwindowleftLogic=new G4LogicalVolume(backwindow_left_tocutfromAlframe,fNistManager->FindOrBuildMaterial("A2_Cherenkovgas"),"backwindow_left_tocutfromAlframe");
    fbackwindowleftPhysi=new G4PVPlacement(0, G4ThreeVector(3.0*cm,0.0*cm,-0.06*mm), fbackwindowleftLogic, "backwindow_left_tocutfromAlframe", fbackAlBoxLogic, false, 0);
    fbackwindowleftLogic->SetVisAttributes(GreenVisAtt);

    fbackwindowupLogic=new G4LogicalVolume(backwindow_up_tocutfromAlframe,fNistManager->FindOrBuildMaterial("A2_Cherenkovgas"),"backwindow_up_tocutfromAlframe");
    fbackwindowupPhysi=new G4PVPlacement(0, G4ThreeVector(3.0*cm,0.0*cm,-0.06*mm), fbackwindowupLogic, "backwindow_up_tocutfromAlframe", fbackAlBoxLogic, false, 0);
    fbackwindowupLogic->SetVisAttributes(GreenVisAtt);

    fbackwindowrightLogic=new G4LogicalVolume(backwindow_right_tocutfromAlframe,fNistManager->FindOrBuildMaterial("A2_Cherenkovgas"),"backwindow_right_tocutfromAlframe");
    fbackwindowrightPhysi=new G4PVPlacement(0, G4ThreeVector(3.0*cm,0.0*cm,-0.06*mm), fbackwindowrightLogic, "backwindow_right_tocutfromAlframe", fbackAlBoxLogic, false, 0);
    fbackwindowrightLogic->SetVisAttributes(GreenVisAtt);

    fbackwindowdownLogic=new G4LogicalVolume(backwindow_down_tocutfromAlframe,fNistManager->FindOrBuildMaterial("A2_Cherenkovgas"),"backwindow_down_tocutfromAlframe");
    fbackwindowdownPhysi=new G4PVPlacement(0, G4ThreeVector(3.0*cm,0.0*cm,-0.06*mm), fbackwindowdownLogic, "backwindow_down_tocutfromAlframe", fbackAlBoxLogic, false, 0);
    fbackwindowdownLogic->SetVisAttributes(GreenVisAtt);

    //Mylar backwindow
    fbackwindowMylarleftLogic=new G4LogicalVolume(backwindow_left_mylar,fNistManager->FindOrBuildMaterial("G4_MYLAR"),"backwindow_left_mylar");
    fbackwindowMylarleftPhysi=new G4PVPlacement(0, G4ThreeVector(3.0*cm,0.0*cm,0.915*mm), fbackwindowMylarleftLogic, "backwindow_left_mylar", fbackAlBoxLogic, false, 0);
    fbackwindowMylarleftLogic->SetVisAttributes(CyanVisAtt);

    fbackwindowMylarupLogic=new G4LogicalVolume(backwindow_up_mylar,fNistManager->FindOrBuildMaterial("G4_MYLAR"),"backwindow_up_mylar");
    fbackwindowMylarupPhysi=new G4PVPlacement(0, G4ThreeVector(3.0*cm,0.0*cm,0.915*mm), fbackwindowMylarupLogic, "backwindow_up_mylar", fbackAlBoxLogic, false, 0);
    fbackwindowMylarupLogic->SetVisAttributes(CyanVisAtt);

    fbackwindowMylarrightLogic=new G4LogicalVolume(backwindow_right_mylar,fNistManager->FindOrBuildMaterial("G4_MYLAR"),"backwindow_right_mylar");
    fbackwindowMylarrightPhysi=new G4PVPlacement(0, G4ThreeVector(3.0*cm,0.0*cm,0.915*mm), fbackwindowMylarrightLogic, "backwindow_right_mylar", fbackAlBoxLogic, false, 0);
    fbackwindowMylarrightLogic->SetVisAttributes(CyanVisAtt);

    fbackwindowMylardownLogic=new G4LogicalVolume(backwindow_down_mylar,fNistManager->FindOrBuildMaterial("G4_MYLAR"),"backwindow_down_mylar");
    fbackwindowMylardownPhysi=new G4PVPlacement(0, G4ThreeVector(3.0*cm,0.0*cm,0.915*mm), fbackwindowMylardownLogic, "backwindow_down_mylar", fbackAlBoxLogic, false, 0);
    fbackwindowMylardownLogic->SetVisAttributes(CyanVisAtt);


    //Tedlar backwindow
    fbackwindowTetlarleftLogic=new G4LogicalVolume(backwindow_left_tetlar,fNistManager->FindOrBuildMaterial("A2_Tedlar"),"backwindow_left_tetlar");
    fbackwindowTetlarleftPhysi=new G4PVPlacement(0, G4ThreeVector(3.0*cm,0.0*cm,0.975*mm), fbackwindowTetlarleftLogic, "backwindow_left_tetlar", fbackAlBoxLogic, false, 0);
    fbackwindowTetlarleftLogic->SetVisAttributes(MagentaVisAtt);

    fbackwindowTetlarupLogic=new G4LogicalVolume(backwindow_up_tetlar,fNistManager->FindOrBuildMaterial("A2_Tedlar"),"backwindow_up_tetlar");
    fbackwindowTetlarupPhysi=new G4PVPlacement(0, G4ThreeVector(3.0*cm,0.0*cm,0.975*mm), fbackwindowTetlarupLogic, "backwindow_up_tetlar", fbackAlBoxLogic, false, 0);
    fbackwindowTetlarupLogic->SetVisAttributes(MagentaVisAtt);

    fbackwindowTetlarrightLogic=new G4LogicalVolume(backwindow_right_tetlar,fNistManager->FindOrBuildMaterial("A2_Tedlar"),"backwindow_right_tetlar");
    fbackwindowTetlarrightPhysi=new G4PVPlacement(0, G4ThreeVector(3.0*cm,0.0*cm,0.975*mm), fbackwindowTetlarrightLogic, "backwindow_right_tetlar", fbackAlBoxLogic, false, 0);
    fbackwindowTetlarrightLogic->SetVisAttributes(MagentaVisAtt);

    fbackwindowTetlardownLogic=new G4LogicalVolume(backwindow_down_tetlar,fNistManager->FindOrBuildMaterial("A2_Tedlar"),"backwindow_down_tetlar");
    fbackwindowTetlardownPhysi=new G4PVPlacement(0, G4ThreeVector(3.0*cm,0.0*cm,0.975*mm), fbackwindowTetlardownLogic, "backwindow_down_tetlar", fbackAlBoxLogic, false, 0);//0.975*mm
    fbackwindowTetlardownLogic->SetVisAttributes(MagentaVisAtt);
//    fbackwindowTetlarLogic->SetVisAttributes(G4VisAttributes::Invisible);


    //nose of the Cherenkov detector: (the x axis goes from right to left in the drawing!!!)

    //First the 2cm frame on which the nose is mounted is made

    std::vector<G4TwoVector> vertices_nose_frame_upperhalf;

    vertices_nose_frame_upperhalf.push_back(G4TwoVector(18.5*cm,32.0*cm));
    vertices_nose_frame_upperhalf.push_back(G4TwoVector(40.5*cm,0.0*cm));
    vertices_nose_frame_upperhalf.push_back(G4TwoVector(-43.0*cm,0.0*cm));
    vertices_nose_frame_upperhalf.push_back(G4TwoVector(-21.5*cm,37.0*cm));
    vertices_nose_frame_upperhalf.push_back(G4TwoVector(18.5*cm,32.0*cm));
    vertices_nose_frame_upperhalf.push_back(G4TwoVector(40.5*cm,0.0*cm));
    vertices_nose_frame_upperhalf.push_back(G4TwoVector(-43.0*cm,0.0*cm));
    vertices_nose_frame_upperhalf.push_back(G4TwoVector(-21.5*cm,37.0*cm));

    G4GenericTrap* nose_frame_upperhalf= new G4GenericTrap("nose_frame_upperhalf",2./2.*cm,vertices_nose_frame_upperhalf);

    std::vector<G4TwoVector> vertices_nose_frame_lowerhalf;

    vertices_nose_frame_lowerhalf.push_back(G4TwoVector(40.5*cm,0.0*cm));
    vertices_nose_frame_lowerhalf.push_back(G4TwoVector(17.5*cm,-38.5*cm));
    vertices_nose_frame_lowerhalf.push_back(G4TwoVector(-21.5*cm,-33.0*cm));
    vertices_nose_frame_lowerhalf.push_back(G4TwoVector(-43.0*cm,0.0*cm));
    vertices_nose_frame_lowerhalf.push_back(G4TwoVector(40.5*cm,0.0*cm));
    vertices_nose_frame_lowerhalf.push_back(G4TwoVector(17.5*cm,-38.5*cm));
    vertices_nose_frame_lowerhalf.push_back(G4TwoVector(-21.5*cm,-33.0*cm));
    vertices_nose_frame_lowerhalf.push_back(G4TwoVector(-43.0*cm,0.0*cm));

    G4GenericTrap* nose_frame_lowerhalf= new G4GenericTrap("nose_frame_lowerhalf",2./2.*cm,vertices_nose_frame_lowerhalf);

   fnoseframeupLogic=new G4LogicalVolume(nose_frame_upperhalf,fNistManager->FindOrBuildMaterial("G4_POLYVINYL_CHLORIDE"),"nose_frame_upperhalf");
   fnoseframeupPhysi=new G4PVPlacement(0, G4ThreeVector(0.0*cm,0.0*cm,96*cm), fnoseframeupLogic, "nose_frame_upperhalf", fMotherLogic, false, 0);
   fnoseframeupLogic->SetVisAttributes(GrayVisAtt);

   fnoseframedownLogic=new G4LogicalVolume(nose_frame_lowerhalf,fNistManager->FindOrBuildMaterial("G4_POLYVINYL_CHLORIDE"),"nose_frame_lowerhalf");
   fnoseframedownPhysi=new G4PVPlacement(0, G4ThreeVector(0.0*cm,0.0*cm,96*cm), fnoseframedownLogic, "nose_frame_lowerhalf", fMotherLogic, false, 0);
   fnoseframedownLogic->SetVisAttributes(GrayVisAtt);

    //make the inner volume of the frame which contains the Cherenkov gas

    G4double thickness_nose_PVC= 5*mm;//this is not the correct value!!!!! Needs to be replaced
    G4double thickness_frame=2.0*cm+thickness_nose_PVC;//in xy-frame

    //calculate new Points of the two inner hexagons, using first incorrect coordinate system (x axis going from left to right)
    G4double point1x_newhex = (-40.5*cm + thickness_frame/TMath::Sin(TMath::ATan(38.5/22.0)));
    G4double point1y_newhex = 0.0*cm;

    G4double point4x_newhex = (43.0*cm - thickness_frame/TMath::Sin(TMath::ATan(33./21.5)));
    G4double point4y_newhex = 0.0*cm;

    G4double point2x_newhex = (point1y_newhex-32./22.*point1x_newhex)/(-32./18.5-32/22.);
    G4double point2y_newhex = -32./18.5*point2x_newhex;

    G4double point3x_newhex = (point4y_newhex+37./21.5*point4x_newhex)/(74./21.5);
    G4double point3y_newhex = 37./21.5*point3x_newhex;

    G4double point5x_newhex = (point4y_newhex-33./21.5*point4x_newhex)/(-33./21.5-33./21.5);
    G4double point5y_newhex = -33./21.5*point5x_newhex;

    G4double point6x_newhex = (point1y_newhex+38.5/23.*point1x_newhex)/(38.5/17.5+38.5/23.);
    G4double point6y_newhex = 38.5/17.5*point6x_newhex;

    std::vector<G4TwoVector> vertices_hexframe_upperhalf_inner;

    vertices_hexframe_upperhalf_inner.push_back(G4TwoVector(-1.*point1x_newhex,point1y_newhex));
    vertices_hexframe_upperhalf_inner.push_back(G4TwoVector(-1.*point4x_newhex,point4y_newhex));
    vertices_hexframe_upperhalf_inner.push_back(G4TwoVector(-1.*point3x_newhex,point3y_newhex));
    vertices_hexframe_upperhalf_inner.push_back(G4TwoVector(-1.*point2x_newhex,point2y_newhex));
    vertices_hexframe_upperhalf_inner.push_back(G4TwoVector(-1.*point1x_newhex,point1y_newhex));
    vertices_hexframe_upperhalf_inner.push_back(G4TwoVector(-1.*point4x_newhex,point4y_newhex));
    vertices_hexframe_upperhalf_inner.push_back(G4TwoVector(-1.*point3x_newhex,point3y_newhex));
    vertices_hexframe_upperhalf_inner.push_back(G4TwoVector(-1.*point2x_newhex,point2y_newhex));


    G4GenericTrap* hex_noseframe_upperhalf_inner= new G4GenericTrap("hex_noseframe_upperhalf_inner",(2*cm)/2.,vertices_hexframe_upperhalf_inner);//
    G4GenericTrap* frontwindow_atAlBox_upperhalf= new G4GenericTrap("frontwindow_atAlBox_upperhalf",(6*mm)/2.,vertices_hexframe_upperhalf_inner);//

    std::vector<G4TwoVector> vertices_hexframe_lowerhalf_inner;

    vertices_hexframe_lowerhalf_inner.push_back(G4TwoVector(-1.*point1x_newhex,point1y_newhex));
    vertices_hexframe_lowerhalf_inner.push_back(G4TwoVector(-1.*point6x_newhex,point6y_newhex));
    vertices_hexframe_lowerhalf_inner.push_back(G4TwoVector(-1.*point5x_newhex,point5y_newhex));
    vertices_hexframe_lowerhalf_inner.push_back(G4TwoVector(-1.*point4x_newhex,point4y_newhex));
    vertices_hexframe_lowerhalf_inner.push_back(G4TwoVector(-1.*point1x_newhex,point1y_newhex));
    vertices_hexframe_lowerhalf_inner.push_back(G4TwoVector(-1.*point6x_newhex,point6y_newhex));
    vertices_hexframe_lowerhalf_inner.push_back(G4TwoVector(-1.*point5x_newhex,point5y_newhex));
    vertices_hexframe_lowerhalf_inner.push_back(G4TwoVector(-1.*point4x_newhex,point4y_newhex));

    G4GenericTrap* hex_noseframe_lowerhalf_inner= new G4GenericTrap("hex_noseframe_lowerhalf_inner",(2*cm)/2.,vertices_hexframe_lowerhalf_inner);
    G4GenericTrap* frontwindow_atAlBox_lowerhalf= new G4GenericTrap("frontwindow_atAlBox_lowerhalf",(6*mm)/2.,vertices_hexframe_lowerhalf_inner);//

    fnoseframeinnerupLogic=new G4LogicalVolume(hex_noseframe_upperhalf_inner,fNistManager->FindOrBuildMaterial("A2_Cherenkovgas"),"hex_noseframe_upperhalf_inner");
    fnoseframeinnerupPhysi=new G4PVPlacement(0, G4ThreeVector(0.0*cm,0.0*cm,0.0*cm), fnoseframeinnerupLogic, "hex_noseframe_upperhalf_inner", fnoseframeupLogic, false, 0);
    fnoseframeinnerupLogic->SetVisAttributes(GreenVisAtt);

    fnoseframeinnerdownLogic=new G4LogicalVolume(hex_noseframe_lowerhalf_inner,fNistManager->FindOrBuildMaterial("A2_Cherenkovgas"),"hex_noseframe_lowerhalf_inner");
    fnoseframeinnerdownPhysi=new G4PVPlacement(0, G4ThreeVector(0.0*cm,0.0*cm,0.0*cm), fnoseframeinnerdownLogic, "hex_noseframe_lowerhalf_inner", fnoseframedownLogic, false, 0);
    fnoseframeinnerdownLogic->SetVisAttributes(GreenVisAtt);

    //this is the front window at the Al box which represents the Cherenkov gas volume
    frontwindowupperLogic=new G4LogicalVolume(frontwindow_atAlBox_upperhalf,fNistManager->FindOrBuildMaterial("A2_Cherenkovgas"),"frontwindow_atAlBox_upperhalf");
    frontwindowupperPhysi=new G4PVPlacement(0, G4ThreeVector(15.5*cm,0.0*cm,-35.6*cm), frontwindowupperLogic, "frontwindow_atAlBox_upperhalf", fAlBoxLogic, false, 0);
    frontwindowupperLogic->SetVisAttributes(GreenVisAtt);

    frontwindowlowerLogic=new G4LogicalVolume(frontwindow_atAlBox_lowerhalf,fNistManager->FindOrBuildMaterial("A2_Cherenkovgas"),"frontwindow_atAlBox_lowerhalf");
    frontwindowlowerPhysi=new G4PVPlacement(0, G4ThreeVector(15.5*cm,0.0*cm,-35.6*cm), frontwindowlowerLogic, "frontwindow_atAlBox_lowerhalf", fAlBoxLogic, false, 0);
    frontwindowlowerLogic->SetVisAttributes(GreenVisAtt);


    //now the nose itself
    G4double distance=2.0*cm;//distance from outer nose frame to the beginning of the nose

    //calculate new Points of the two inner hexagons, using first incorrect coordinate system (x axis going from left to right)
    G4double point1x_nose = (-40.5*cm + distance/TMath::Sin(TMath::ATan(38.5/22.0)));
    G4double point1y_nose = 0.0*cm;

    G4double point4x_nose = (43.0*cm - distance/TMath::Sin(TMath::ATan(33./21.5)));
    G4double point4y_nose = 0.0*cm;

    G4double point2x_nose = (point1y_nose-32./22.*point1x_nose)/(-32./18.5-32/22.);
    G4double point2y_nose = -32./18.5*point2x_nose;

    G4double point3x_nose = (point4y_nose+37./21.5*point4x_nose)/(74./21.5);
    G4double point3y_nose = 37./21.5*point3x_nose;

    G4double point5x_nose = (point4y_nose-33./21.5*point4x_nose)/(-33./21.5-33./21.5);
    G4double point5y_nose = -33./21.5*point5x_nose;

    G4double point6x_nose = (point1y_nose+38.5/23.*point1x_nose)/(38.5/17.5+38.5/23.);
    G4double point6y_nose = 38.5/17.5*point6x_nose;

    std::vector<G4TwoVector> vertices_hex_nose_upperhalf;

    vertices_hex_nose_upperhalf.push_back(G4TwoVector(24.5*cm,0.0*cm));
    vertices_hex_nose_upperhalf.push_back(G4TwoVector(-28.0*cm,0.0*cm));
    vertices_hex_nose_upperhalf.push_back(G4TwoVector(-12.5*cm,24.0*cm));
    vertices_hex_nose_upperhalf.push_back(G4TwoVector(10.5*cm,20.0*cm));
    vertices_hex_nose_upperhalf.push_back(G4TwoVector(-1.*point1x_nose,point1y_nose));
    vertices_hex_nose_upperhalf.push_back(G4TwoVector(-1.*point4x_nose,point4y_nose));
    vertices_hex_nose_upperhalf.push_back(G4TwoVector(-1.*point3x_nose,point3y_nose));
    vertices_hex_nose_upperhalf.push_back(G4TwoVector(-1.*point2x_nose,point2y_nose));

    G4GenericTrap* hex_nose_upperhalf= new G4GenericTrap("hex_nose_upperhalf",35./2.*cm,vertices_hex_nose_upperhalf);

    std::vector<G4TwoVector> vertices_hex_nose_lowerhalf;

    vertices_hex_nose_lowerhalf.push_back(G4TwoVector(24.5*cm,0.0*cm));
    vertices_hex_nose_lowerhalf.push_back(G4TwoVector(9.8*cm,-23.5*cm));
    vertices_hex_nose_lowerhalf.push_back(G4TwoVector(-14.0*cm,-19.5*cm));
    vertices_hex_nose_lowerhalf.push_back(G4TwoVector(-28.0*cm,0.0*cm));
    vertices_hex_nose_lowerhalf.push_back(G4TwoVector(-1.*point1x_nose,point1y_nose));
    vertices_hex_nose_lowerhalf.push_back(G4TwoVector(-1.*point6x_nose,point6y_nose));
    vertices_hex_nose_lowerhalf.push_back(G4TwoVector(-1.*point5x_nose,point5y_nose));
    vertices_hex_nose_lowerhalf.push_back(G4TwoVector(-1.*point4x_nose,point4y_nose));

    G4GenericTrap* hex_nose_lowerhalf= new G4GenericTrap("hex_nose_lowerhalf",35./2.*cm,vertices_hex_nose_lowerhalf);

    fnoseupLogic=new G4LogicalVolume(hex_nose_upperhalf,fNistManager->FindOrBuildMaterial("G4_POLYVINYL_CHLORIDE"),"hex_nose_upperhalf");
    fnoseupPhysi=new G4PVPlacement(0, G4ThreeVector(0.0*cm,0.0*cm,77.5*cm), fnoseupLogic, "hex_nose_upperhalf", fMotherLogic, false, 0);
    fnoseupLogic->SetVisAttributes(BlueVisAtt);
    //fnoseupLogic->SetVisAttributes(G4VisAttributes::Invisible);

    fnosedownLogic=new G4LogicalVolume(hex_nose_lowerhalf,fNistManager->FindOrBuildMaterial("G4_POLYVINYL_CHLORIDE"),"hex_nose_lowerhalf");
    fnosedownPhysi=new G4PVPlacement(0, G4ThreeVector(0.0*cm,0.0*cm,77.5*cm), fnosedownLogic, "hex_nose_lowerhalf", fMotherLogic, false, 0);
    fnosedownLogic->SetVisAttributes(BlueVisAtt);
    //fnosedownLogic->SetVisAttributes(G4VisAttributes::Invisible);



    //inner volume of the nose of the Cherenkov detector:

    G4double point1x_smallnewhex = (-24.5*cm + thickness_nose_PVC/TMath::Sin(TMath::ATan(23.5/14.7)));
    G4double point1y_smallnewhex = 0.0*cm;

    G4double point4x_smallnewhex = (28.0*cm - thickness_nose_PVC/TMath::Sin(TMath::ATan(19.5/14.0)));
    G4double point4y_smallnewhex = 0.0*cm;

    G4double point2x_smallnewhex = (point1y_smallnewhex  - 20./14.*point1x_smallnewhex)/(-20./10.5-20./14.);
    G4double point2y_smallnewhex = -20./10.5*point2x_smallnewhex;

    G4double point3x_smallnewhex = (point4y_smallnewhex+24./15.5*point4x_smallnewhex)/(24./12.5+24./15.5);
    G4double point3y_smallnewhex = 24./12.5*point3x_smallnewhex;

    G4double point5x_smallnewhex = (point4y_smallnewhex-19.5/14.*point4x_smallnewhex)/(-19.5/14.-19.5/14.);
    G4double point5y_smallnewhex = -19.5/14.0*point5x_smallnewhex;

    G4double point6x_smallnewhex = (point1y_smallnewhex+23.5/14.7*point1x_smallnewhex)/(23.5/9.8+23.5/14.7);
    G4double point6y_smallnewhex = 23.5/9.8*point6x_smallnewhex;

    std::vector<G4TwoVector> vertices_hex_nose_upperhalf_inner;

    vertices_hex_nose_upperhalf_inner.push_back(G4TwoVector(-1.*point1x_smallnewhex,point1y_smallnewhex));
    vertices_hex_nose_upperhalf_inner.push_back(G4TwoVector(-1.*point4x_smallnewhex,point4y_smallnewhex));
    vertices_hex_nose_upperhalf_inner.push_back(G4TwoVector(-1.*point3x_smallnewhex,point3y_smallnewhex));
    vertices_hex_nose_upperhalf_inner.push_back(G4TwoVector(-1.*point2x_smallnewhex,point2y_smallnewhex));
    vertices_hex_nose_upperhalf_inner.push_back(G4TwoVector(-1.*point1x_newhex,point1y_newhex));//same as the base of the nose frame inner volume
    vertices_hex_nose_upperhalf_inner.push_back(G4TwoVector(-1.*point4x_newhex,point4y_newhex));
    vertices_hex_nose_upperhalf_inner.push_back(G4TwoVector(-1.*point3x_newhex,point3y_newhex));
    vertices_hex_nose_upperhalf_inner.push_back(G4TwoVector(-1.*point2x_newhex,point2y_newhex));


    G4GenericTrap* hex_nose_upperhalf_inner= new G4GenericTrap("hex_nose_upperhalf_inner",(35.*cm-0.12*mm)/2.,vertices_hex_nose_upperhalf_inner);//

    std::vector<G4TwoVector> vertices_hex_nose_lowerhalf_inner;

    vertices_hex_nose_lowerhalf_inner.push_back(G4TwoVector(-1.*point1x_smallnewhex,point1y_smallnewhex));
    vertices_hex_nose_lowerhalf_inner.push_back(G4TwoVector(-1.*point6x_smallnewhex,point6y_smallnewhex));
    vertices_hex_nose_lowerhalf_inner.push_back(G4TwoVector(-1.*point5x_smallnewhex,point5y_smallnewhex));
    vertices_hex_nose_lowerhalf_inner.push_back(G4TwoVector(-1.*point4x_smallnewhex,point4y_smallnewhex));
    vertices_hex_nose_lowerhalf_inner.push_back(G4TwoVector(-1.*point1x_newhex,point1y_newhex));
    vertices_hex_nose_lowerhalf_inner.push_back(G4TwoVector(-1.*point6x_newhex,point6y_newhex));
    vertices_hex_nose_lowerhalf_inner.push_back(G4TwoVector(-1.*point5x_newhex,point5y_newhex));
    vertices_hex_nose_lowerhalf_inner.push_back(G4TwoVector(-1.*point4x_newhex,point4y_newhex));

    G4GenericTrap* hex_nose_lowerhalf_inner= new G4GenericTrap("hex_nose_lowerhalf_inner",(35.*cm-0.12*mm)/2.,vertices_hex_nose_lowerhalf_inner);

    fnoseinnerupLogic=new G4LogicalVolume(hex_nose_upperhalf_inner,fNistManager->FindOrBuildMaterial("A2_Cherenkovgas"),"hex_nose_upperhalf_inner");
    fnoseinnerupPhysi=new G4PVPlacement(0, G4ThreeVector(0.0*cm,0.0*cm,0.12*mm/2.0), fnoseinnerupLogic, "hex_nose_upperhalf_inner", fnoseupLogic, false, 0);
    fnoseinnerupLogic->SetVisAttributes(GreenVisAtt);

    fnoseinnerdownLogic=new G4LogicalVolume(hex_nose_lowerhalf_inner,fNistManager->FindOrBuildMaterial("A2_Cherenkovgas"),"hex_nose_lowerhalf_inner");
    fnoseinnerdownPhysi=new G4PVPlacement(0, G4ThreeVector(0.0*cm,0.0*cm,0.12*mm/2.0), fnoseinnerdownLogic, "hex_nose_lowerhalf_inner", fnosedownLogic, false, 0);
    fnoseinnerdownLogic->SetVisAttributes(GreenVisAtt);


    //front window at the nose (hexagonal shape is divided into two parts at the x-axis)
    std::vector<G4TwoVector> vertices_frontwindow_nose_upperhalf;

    vertices_frontwindow_nose_upperhalf.push_back(G4TwoVector(-1.*point1x_smallnewhex,point1y_smallnewhex));
    vertices_frontwindow_nose_upperhalf.push_back(G4TwoVector(-1.*point4x_smallnewhex,point4y_smallnewhex));
    vertices_frontwindow_nose_upperhalf.push_back(G4TwoVector(-1.*point3x_smallnewhex,point3y_smallnewhex));
    vertices_frontwindow_nose_upperhalf.push_back(G4TwoVector(-1.*point2x_smallnewhex,point2y_smallnewhex));
    vertices_frontwindow_nose_upperhalf.push_back(G4TwoVector(-1.*point1x_smallnewhex,point1y_smallnewhex));
    vertices_frontwindow_nose_upperhalf.push_back(G4TwoVector(-1.*point4x_smallnewhex,point4y_smallnewhex));
    vertices_frontwindow_nose_upperhalf.push_back(G4TwoVector(-1.*point3x_smallnewhex,point3y_smallnewhex));
    vertices_frontwindow_nose_upperhalf.push_back(G4TwoVector(-1.*point2x_smallnewhex,point2y_smallnewhex));

    G4GenericTrap* frontwindow_nose_upperhalf_mylar= new G4GenericTrap("frontwindow_nose_upperhalf_mylar",0.07/2.*mm,vertices_frontwindow_nose_upperhalf);
    G4GenericTrap* frontwindow_nose_upperhalf_tetlar= new G4GenericTrap("frontwindow_nose_upperhalf_tetlar",0.05/2.*mm,vertices_frontwindow_nose_upperhalf);


    std::vector<G4TwoVector> vertices_frontwindow_nose_lowerhalf;

    vertices_frontwindow_nose_lowerhalf.push_back(G4TwoVector(-1.*point1x_smallnewhex,point1y_smallnewhex));
    vertices_frontwindow_nose_lowerhalf.push_back(G4TwoVector(-1.*point6x_smallnewhex,point6y_smallnewhex));
    vertices_frontwindow_nose_lowerhalf.push_back(G4TwoVector(-1.*point5x_smallnewhex,point5y_smallnewhex));
    vertices_frontwindow_nose_lowerhalf.push_back(G4TwoVector(-1.*point4x_smallnewhex,point4y_smallnewhex));
    vertices_frontwindow_nose_lowerhalf.push_back(G4TwoVector(-1.*point1x_smallnewhex,point1y_smallnewhex));
    vertices_frontwindow_nose_lowerhalf.push_back(G4TwoVector(-1.*point6x_smallnewhex,point6y_smallnewhex));
    vertices_frontwindow_nose_lowerhalf.push_back(G4TwoVector(-1.*point5x_smallnewhex,point5y_smallnewhex));
    vertices_frontwindow_nose_lowerhalf.push_back(G4TwoVector(-1.*point4x_smallnewhex,point4y_smallnewhex));

    G4GenericTrap* frontwindow_nose_lowerhalf_mylar= new G4GenericTrap("frontwindow_nose_lowerhalf_tetlar",0.07/2.*mm,vertices_frontwindow_nose_lowerhalf);
    G4GenericTrap* frontwindow_nose_lowerhalf_tetlar= new G4GenericTrap("frontwindow_nose_lowerhalf_tetlar",0.05/2.*mm,vertices_frontwindow_nose_lowerhalf);

    ffrontwindow_noseup_mylarLogic=new G4LogicalVolume(frontwindow_nose_upperhalf_mylar,fNistManager->FindOrBuildMaterial("G4_MYLAR"),"frontwindow_nose_upperhalf_mylar");
    ffrontwindow_noseup_mylarPhysi=new G4PVPlacement(0, G4ThreeVector(0.0*cm,0.0*cm,-17.4915*cm), ffrontwindow_noseup_mylarLogic, "frontwindow_nose_upperhalf_mylar", fnoseupLogic, false, 0);//thickness_nose/2.0
    ffrontwindow_noseup_mylarLogic->SetVisAttributes(CyanVisAtt);

    ffrontwindow_nosedown_mylarLogic=new G4LogicalVolume(frontwindow_nose_lowerhalf_mylar,fNistManager->FindOrBuildMaterial("G4_MYLAR"),"frontwindow_nose_lowerhalf_mylar");
    ffrontwindow_nosedown_mylarPhysi=new G4PVPlacement(0, G4ThreeVector(0.0*cm,0.0*cm,-17.4915*cm), ffrontwindow_nosedown_mylarLogic, "frontwindow_nose_lowerhalf_mylar", fnosedownLogic, false, 0);//thickness_nose/2.0
    ffrontwindow_nosedown_mylarLogic->SetVisAttributes(CyanVisAtt);

    ffrontwindow_nose_upperhalf_tetlarLogic=new G4LogicalVolume(frontwindow_nose_upperhalf_tetlar,fNistManager->FindOrBuildMaterial("A2_Tedlar"),"frontwindow_nose_upperhalf_tetlar");
    ffrontwindow_nose_upperhalf_tetlarPhysi=new G4PVPlacement(0, G4ThreeVector(0.0*cm,0.0*cm,-17.4975*cm), ffrontwindow_nose_upperhalf_tetlarLogic, "frontwindow_nose_upperhalf_tetlar", fnoseupLogic, false, 0);//thickness_nose/2.0
    ffrontwindow_nose_upperhalf_tetlarLogic->SetVisAttributes(MagentaVisAtt);

    ffrontwindow_nose_lowerhalf_tetlarLogic=new G4LogicalVolume(frontwindow_nose_lowerhalf_tetlar,fNistManager->FindOrBuildMaterial("A2_Tedlar"),"frontwindow_nose_lowerhalf_tetlar");
    ffrontwindow_nose_lowerhalf_tetlarPhysi=new G4PVPlacement(0, G4ThreeVector(0.0*cm,0.0*cm,-17.4975*cm), ffrontwindow_nose_lowerhalf_tetlarLogic, "frontwindow_nose_lowerhalf_tetlar", fnosedownLogic, false, 0);//thickness_nose/2.0
    ffrontwindow_nose_lowerhalf_tetlarLogic->SetVisAttributes(MagentaVisAtt);

// now the mirror of the Cherenkov detector

    G4double  mirrorRadius = 140.*cm, mirrorThickness = 3*mm;
    G4Sphere * MirrorSphere = new G4Sphere("MirrorSpere", mirrorRadius, mirrorRadius+mirrorThickness,0,360*deg,0,20*deg);

    G4RotationMatrix* rm_mirror = new G4RotationMatrix();
    rm_mirror->rotateZ(60.*deg);

    fmirrorLogic=new G4LogicalVolume(MirrorSphere,fNistManager->FindOrBuildMaterial("A2_Pyrex"),"MirrorSphere");
    fmirrorPhysi=new G4PVPlacement(rm_mirror, G4ThreeVector(0.0*cm,0.0*cm,-140.0*cm), fmirrorLogic, "MirrorSphere", finnerboxvolumeLogic, false, 0);
    fmirrorLogic->SetVisAttributes(BlueVisAtt);
}
