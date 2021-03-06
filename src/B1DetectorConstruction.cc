//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file B1DetectorConstruction.cc
/// \brief Implementation of the B1DetectorConstruction class

#include "B1DetectorConstruction.hh"
#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Tubs.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4UserLimits.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"




//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::B1DetectorConstruction()
: G4VUserDetectorConstruction(),
  fScoringVolume(nullptr),
  frange(0.1* mm),
  fLogicTarget(nullptr)
{
    G4NistManager* nist = G4NistManager::Instance();
    fTargetMaterial = nist->FindOrBuildMaterial("G4_Be");
    fMessenger = new B1DetectorMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::~B1DetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* B1DetectorConstruction::Construct()
{
    G4GeometryManager::GetInstance()->OpenGeometry();
    G4PhysicalVolumeStore::GetInstance()->Clean();
    G4LogicalVolumeStore::GetInstance()->Clean();
    G4SolidStore::GetInstance()->Clean();

  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();

  G4double density, fractionmass;
  G4int ncomponents;
  // Envelope parameters
  //
  G4double env_sizeXY = 40*cm, env_sizeZ = 40*cm;


  // Option to switch on/off checking of volumes overlaps
  //
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;

  G4bool checkOverlaps = true;

  //     
  // World
  //
  G4double world_sizeXY = 1.2*env_sizeXY;
  G4double world_sizeZ  = 1.2*env_sizeZ;
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_Galactic");
  
  G4Box* solidWorld =    
    new G4Box("World",                       //its name
       0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);     //its size
      
  G4LogicalVolume* logicWorld =                         
    new G4LogicalVolume(solidWorld,          //its solid
                        world_mat,           //its material
                        "World");            //its name
                                   
  G4VPhysicalVolume* physWorld = 
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking

 //Be Target
  G4double BeTargetRadius = 2.5 * cm;
  G4double startAngle = 0.*degree;
  G4double endAngle = 360.*degree;
    G4Tubs* solidBeTarget =
          new G4Tubs("Be_Layer", 0,BeTargetRadius,frange/2., startAngle,endAngle);

    fLogicTarget =
            new G4LogicalVolume(solidBeTarget,           //its solid
                                fTargetMaterial,                      //its material
                                "Be_Layer");       //its name;
    G4cout        << G4endl
                  <<  "za hui shi" << fLogicTarget->GetMaterial()->GetName() << G4endl;
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(0,0,frange/2.),       //at (0,0,0)
                      fLogicTarget,            //its logical volume
                      "Be_Layer",               //its name
                      logicWorld,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking


    // Step limits only approach to charged particles!
    G4double max_step =  frange / 2000.;
    G4cout << "frange " << frange / CLHEP::mm << G4endl;
    G4cout << " step in run " << max_step / CLHEP::mm << G4endl;
    G4UserLimits *step_limit_target = new G4UserLimits();
    step_limit_target->SetMaxAllowedStep(max_step);
    fLogicTarget->SetUserLimits(step_limit_target);

    return physWorld;
}


void B1DetectorConstruction::SetRange(G4double range)
{
    frange = range;
    //G4RunManager::GetRunManager()->PhysicsHasBeenModified();
    G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void B1DetectorConstruction::SetTargetMaterial(G4String materialName)
{
    G4String a = materialName;
    G4NistManager* nistManager = G4NistManager::Instance();

    G4Material* pttoMaterial =
            nistManager->FindOrBuildMaterial(materialName);

    if (fTargetMaterial != pttoMaterial) {
        if ( pttoMaterial ) {
            fTargetMaterial = pttoMaterial;
            if (fLogicTarget) {
                fLogicTarget->SetMaterial(fTargetMaterial);
                G4RunManager::GetRunManager()->PhysicsHasBeenModified();
            }
            G4cout
                    << G4endl
                    << "----> The target is made of " << materialName << G4endl;
        }
        else {
            G4cout
                    << G4endl
                    << "-->  WARNING from SetTargetMaterial : "
                    << materialName << " not found" << G4endl;
        }
    }
}