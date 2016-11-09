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
#include "TMath.h"

A2DetCherenkov::A2DetCherenkov()
{

}
A2DetCherenkov::~A2DetCherenkov()
{

}


G4VPhysicalVolume* A2DetCherenkov::Construct(G4LogicalVolume* MotherLogical){
fMotherLogic=MotherLogical;

// Al frame of Cherenkov detector, consisting of large box, triangular shaped trapezoid at top right corner in xz-plane
    G4Box* centralbox=new G4Box("centralbox",60.0*cm,60.0*cm,36.0*cm);

    std::vector<G4TwoVector> vertices_tri;

    vertices_tri.push_back(G4TwoVector(-10.5*cm,-60.0*cm));
    vertices_tri.push_back(G4TwoVector(-10.5*cm,-60.0*cm));
    vertices_tri.push_back(G4TwoVector(-10.5*cm,60.0*cm));
    vertices_tri.push_back(G4TwoVector(-10.5*cm,60.0*cm));
    vertices_tri.push_back(G4TwoVector(10.5*cm,-60.0*cm));
    vertices_tri.push_back(G4TwoVector(-10.5*cm,-60.0*cm));
    vertices_tri.push_back(G4TwoVector(-10.5*cm,60.0*cm));
    vertices_tri.push_back(G4TwoVector(10.5*cm,60.0*cm));


    G4GenericTrap* tri= new G4GenericTrap("tri",22.5*cm,vertices_tri);

    G4RotationMatrix* rm = new G4RotationMatrix();
    rm->rotateZ(0.*deg);

    //unite the centralbox and the triangle
    G4UnionSolid* Albox = new G4UnionSolid("Albox", centralbox, tri, rm,G4ThreeVector(70.5*cm,0.*cm,13.5*cm));

    //subtract the inner volume of the box to get the 6mm thick Al frame
    G4Box* centralinnerbox=new G4Box("centralinnerbox",59.8*cm,59.8*cm,35.6*cm);//thickness of Al frame is 6mm (front plate) or 2mm (rest)

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

    G4double zhalflength_innertri= (45.*cm - 0.2*cm - 0.2*cm/(TMath::Sin(TMath::ATan(21./45.))))/2.;

    G4GenericTrap* innertri= new G4GenericTrap("innertri",zhalflength_innertri,vertices_innertri);//22.02676

    G4Box* smallbox=new G4Box("smallbox",0.1*cm,59.8*cm,zhalflength_innertri);//thickness of Al frame is 2mm 22.16338

    G4UnionSolid* centralinner_And_smallbox = new G4UnionSolid("centralinner_And_smallbox", centralinnerbox, smallbox, rm,G4ThreeVector(59.9*cm,0.*cm,(13.5-0.2)*cm));
    G4UnionSolid* inner_vol = new G4UnionSolid("inner_vol", centralinner_And_smallbox, innertri, rm,G4ThreeVector(70.5*cm,0.*cm,(13.5-0.2)*cm));

    G4SubtractionSolid* Alboxframe = new G4SubtractionSolid("Alboxframe", Albox, inner_vol, rm,G4ThreeVector(0.0*cm,0.*cm,0.2*cm));

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

    G4UnionSolid* backwindow_left_right_tocutfromAlframe = new G4UnionSolid("backwindow_left_right_tocutfromAlframe", backwindow_left_tocutfromAlframe, backwindow_right_tocutfromAlframe, rm,G4ThreeVector(0.0*cm,0.*cm, 0.*cm));

    G4UnionSolid* backwindow_left_right_mylar = new G4UnionSolid("backwindow_left_right_mylar", backwindow_left_mylar, backwindow_right_mylar, rm,G4ThreeVector(0.0*cm,0.*cm, 0.*cm));

    G4UnionSolid* backwindow_left_right_tetlar = new G4UnionSolid("backwindow_left_right_tetlar", backwindow_left_tetlar, backwindow_right_tetlar, rm,G4ThreeVector(0.0*cm,0.*cm, 0.*cm));

    G4UnionSolid* backwindow_up_down_tocutfromAlframe = new G4UnionSolid("backwindow_up_down_tocutfromAlframe", backwindow_up_tocutfromAlframe, backwindow_down_tocutfromAlframe, rm,G4ThreeVector(0.0*cm,0.*cm, 0.*cm));

    G4UnionSolid* backwindow_up_down_mylar = new G4UnionSolid("backwindow_up_down_mylar", backwindow_up_mylar, backwindow_down_mylar, rm,G4ThreeVector(0.0*cm,0.*cm, 0.*cm));

    G4UnionSolid* backwindow_up_down_tetlar = new G4UnionSolid("backwindow_up_down_tetlar", backwindow_up_tetlar, backwindow_down_tetlar, rm,G4ThreeVector(0.0*cm,0.*cm, 0.*cm));

    G4UnionSolid* backwindow_tocutfromAlframe = new G4UnionSolid("backwindow_tocutfromAlframe", backwindow_left_right_tocutfromAlframe , backwindow_up_down_tocutfromAlframe, rm,G4ThreeVector(0.0*cm,0.*cm, 0.*cm));

    G4UnionSolid* backwindow_mylar = new G4UnionSolid("backwindow_mylar", backwindow_left_right_mylar , backwindow_up_down_mylar, rm,G4ThreeVector(0.0*cm,0.*cm, 0.*cm));

    G4UnionSolid* backwindow_tetlar = new G4UnionSolid("backwindow_tetlar", backwindow_left_right_tetlar, backwindow_up_down_tetlar, rm,G4ThreeVector(0.0*cm,0.*cm, 0.*cm));



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

    G4UnionSolid* nose_frame = new G4UnionSolid("nose_frame", nose_frame_upperhalf, nose_frame_lowerhalf, rm,G4ThreeVector(0.0*cm,0.*cm, 0.*cm));

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

    G4UnionSolid* hex_noseframe_inner = new G4UnionSolid("hex_noseframe_inner", hex_noseframe_upperhalf_inner, hex_noseframe_lowerhalf_inner, rm,G4ThreeVector(0.0*cm,0.*cm, 0.*cm));
    G4UnionSolid* frontwindow_atAlBox = new G4UnionSolid("frontwindow_atAlBox", frontwindow_atAlBox_upperhalf, frontwindow_atAlBox_lowerhalf, rm,G4ThreeVector(0.0*cm,0.*cm, 0.*cm));


    //G4UnionSolid*  noseframe_inner = Make_inner_hexagon_large(2*cm, 2*cm, "noseframe_inner");


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

    G4UnionSolid* hex_nose_solid = new G4UnionSolid("hex_nose_solid", hex_nose_upperhalf, hex_nose_lowerhalf, rm,G4ThreeVector(0.0*cm,0.*cm, 0.*cm));

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

    G4UnionSolid* hex_nose_solid_inner = new G4UnionSolid("hex_nose_solid_inner", hex_nose_upperhalf_inner, hex_nose_lowerhalf_inner, rm,G4ThreeVector(0.0*cm,0.*cm, 0.*cm));


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

        G4UnionSolid* frontwindow_nose_mylar = new G4UnionSolid("frontwindow_nose_mylar",frontwindow_nose_upperhalf_mylar, frontwindow_nose_lowerhalf_mylar, 0,G4ThreeVector(0.0*cm,0.*cm, 0.*cm));
        G4UnionSolid* frontwindow_nose_tetlar = new G4UnionSolid("frontwindow_nose_tetlar",frontwindow_nose_upperhalf_tetlar, frontwindow_nose_lowerhalf_tetlar, 0,G4ThreeVector(0.0*cm,0.*cm, 0.*cm));





 //Colours with thier corresponding materials used in the visualization:
 G4VisAttributes* AlVisAtt= new G4VisAttributes(G4Colour(0.8,0.8,0.8)); //(grey)
 G4VisAttributes* CUVisAtt= new G4VisAttributes(G4Colour(0.8,0.6,0.2)); //copper (brown)
 G4VisAttributes* CyanVisAtt= new G4VisAttributes(G4Colour(.5,.2,0.0)); // Mylar foil window
 G4VisAttributes* MagentaVisAtt= new G4VisAttributes(G4Colour(1.0,0.0,1.0)); //Tedlar foil
 G4VisAttributes* BlueVisAtt= new G4VisAttributes(G4Colour(0.0,0.0,1.0)); //PVC nose
 G4VisAttributes* GreenVisAtt= new G4VisAttributes(G4Colour(0.0,1.0,0.0)); //Cherenkov gas
 G4VisAttributes* RedVisAtt= new G4VisAttributes(G4Colour(1.0, 0.0, 0.0)); //Al box
 G4VisAttributes* WhiteVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0)); //


//placement of all objects:
    fAlBoxLogic=new G4LogicalVolume(Albox,fNistManager->FindOrBuildMaterial("G4_Al"),"Albox");
     fAlBoxPhysi=new G4PVPlacement(0, G4ThreeVector(-15.5*cm,0.0*cm,133*cm), fAlBoxLogic, "Albox", fMotherLogic, false, 0,true);
     fAlBoxLogic->SetVisAttributes(RedVisAtt);
    // fAlBoxLogic->SetVisAttributes(G4VisAttributes::Invisible);

    finnerboxvolumeLogic=new G4LogicalVolume(inner_vol,fNistManager->FindOrBuildMaterial("Cherenkovgas"),"inner_vol");
    finnerboxvolumePhysi=new G4PVPlacement(0, G4ThreeVector(0.0*cm,0.0*cm,0.2*cm), finnerboxvolumeLogic, "inner_vol", fAlBoxLogic, false, 0,true);
    finnerboxvolumeLogic->SetVisAttributes(GreenVisAtt);
   //     finnerboxvolumeLogic->SetVisAttributes(G4VisAttributes::Invisible);

    frontwindowLogic=new G4LogicalVolume(frontwindow_atAlBox,fNistManager->FindOrBuildMaterial("Cherenkovgas"),"frontwindow_atAlBox");
    frontwindowPhysi=new G4PVPlacement(0, G4ThreeVector(15.5*cm,0.0*cm,-35.7*cm), frontwindowLogic, "frontwindow_atAlBox", fAlBoxLogic, false, 0,true);
    frontwindowLogic->SetVisAttributes(GreenVisAtt);


     fbackwindowLogic=new G4LogicalVolume(backwindow_tocutfromAlframe,fNistManager->FindOrBuildMaterial("Cherenkovgas"),"backwindow_tocutfromAlframe");
  fbackwindowPhysi=new G4PVPlacement(0, G4ThreeVector(15.5*cm,0.0*cm,35.894*cm), fbackwindowLogic, "backwindow_tocutfromAlframe", fAlBoxLogic, false, 0,true);//35.9
    fbackwindowLogic->SetVisAttributes(GreenVisAtt);
//fbackwindowLogic->SetVisAttributes(G4VisAttributes::Invisible);

     fbackwindowMylarLogic=new G4LogicalVolume(backwindow_mylar,fNistManager->FindOrBuildMaterial("Mylar"),"backwindow_mylar");
    fbackwindowMylarPhysi=new G4PVPlacement(0, G4ThreeVector(15.5*cm,0.0*cm,35.9915*cm), fbackwindowMylarLogic, "backwindow_mylar",fAlBoxLogic, false, 0, true);//0.915*mm fbackwindowLogic
    fbackwindowMylarLogic->SetVisAttributes(CyanVisAtt);
//    fbackwindowMylarLogic->SetVisAttributes(G4VisAttributes::Invisible);

    fbackwindowTetlarLogic=new G4LogicalVolume(backwindow_tetlar,fNistManager->FindOrBuildMaterial("Tedlar"),"backwindow_tetlar");
 fbackwindowTetlarPhysi=new G4PVPlacement(0, G4ThreeVector(15.5*cm,0.0*cm,35.9975*cm), fbackwindowTetlarLogic, "backwindow_tetlar", fAlBoxLogic, false, 0,true);//0.975*mm
    fbackwindowTetlarLogic->SetVisAttributes(MagentaVisAtt);
//    fbackwindowTetlarLogic->SetVisAttributes(G4VisAttributes::Invisible);


//place the nose of the Cherenkov detector

//first place the front window:

   fnoseframeLogic=new G4LogicalVolume(nose_frame,fNistManager->FindOrBuildMaterial("PVC"),"nose_frame");
 fnoseframePhysi=new G4PVPlacement(0, G4ThreeVector(0.0*cm,0.0*cm,96*cm), fnoseframeLogic, "nose_frame", fMotherLogic, false, 0,true);
   fnoseframeLogic->SetVisAttributes(AlVisAtt);

    fnoseframeinnerLogic=new G4LogicalVolume(hex_noseframe_inner,fNistManager->FindOrBuildMaterial("Cherenkovgas"),"hex_noseframe_inner");
     fnoseframeinnerPhysi=new G4PVPlacement(0, G4ThreeVector(0.0*cm,0.0*cm,0.0*cm), fnoseframeinnerLogic, "hex_noseframe_inner", fnoseframeLogic, false, 0,true);
    fnoseframeinnerLogic->SetVisAttributes(GreenVisAtt);


    fnoseLogic=new G4LogicalVolume(hex_nose_solid,fNistManager->FindOrBuildMaterial("PVC"),"hex_nose_solid");
     fnosePhysi=new G4PVPlacement(0, G4ThreeVector(0.0*cm,0.0*cm,77.5*cm), fnoseLogic, "hex_nose_solid", fMotherLogic, false, 0,true);
     fnoseLogic->SetVisAttributes(BlueVisAtt);
     //fnoseLogic->SetVisAttributes(G4VisAttributes::Invisible);

     fnoseinnerLogic=new G4LogicalVolume(hex_nose_solid_inner,fNistManager->FindOrBuildMaterial("Cherenkovgas"),"hex_nose_solid_inner");
  fnoseinnerPhysi=new G4PVPlacement(0, G4ThreeVector(0.0*cm,0.0*cm,0.12*mm/2.0), fnoseinnerLogic, "hex_nose_solid_inner", fnoseLogic, false, 0,true);//thickness_nose/2.0
     fnoseinnerLogic->SetVisAttributes(GreenVisAtt);

     ffrontwindow_nose_mylarLogic=new G4LogicalVolume(frontwindow_nose_mylar,fNistManager->FindOrBuildMaterial("Mylar"),"frontwindow_nose_mylar");
  ffrontwindow_nose_mylarPhysi=new G4PVPlacement(0, G4ThreeVector(0.0*cm,0.0*cm,-17.4915*cm), ffrontwindow_nose_mylarLogic, "frontwindow_nose_mylar", fnoseLogic, false, 0,true);//thickness_nose/2.0
     ffrontwindow_nose_mylarLogic->SetVisAttributes(CyanVisAtt);

     ffrontwindow_nose_tetlarLogic=new G4LogicalVolume(frontwindow_nose_tetlar,fNistManager->FindOrBuildMaterial("Tedlar"),"frontwindow_nose_tetlar");
  ffrontwindow_nose_tetlarPhysi=new G4PVPlacement(0, G4ThreeVector(0.0*cm,0.0*cm,-17.4975*cm), ffrontwindow_nose_tetlarLogic, "frontwindow_nose_tetlar", fnoseLogic, false, 0,true);//thickness_nose/2.0
     ffrontwindow_nose_tetlarLogic->SetVisAttributes(MagentaVisAtt);

return fMyPhysi;
}
