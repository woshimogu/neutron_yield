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
/// \file B1SteppingAction.cc
/// \brief Implementation of the B1SteppingAction class

#include "B1SteppingAction.hh"
#include "B1EventAction.hh"
#include "B1DetectorConstruction.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
#include "g4root.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1SteppingAction::B1SteppingAction(B1EventAction* eventAction)
: G4UserSteppingAction(),
  fEventAction(eventAction),
  fScoringVolume(0)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1SteppingAction::~B1SteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1SteppingAction::UserSteppingAction(const G4Step* step)
{
  // get volume of the current step
  G4LogicalVolume* volume 
    = step->GetPreStepPoint()->GetTouchableHandle()
      ->GetVolume()->GetLogicalVolume();
  G4String volname = volume->GetName();
  G4double edepStep = step->GetTotalEnergyDeposit();

  G4StepPoint* prestep = step->GetPreStepPoint();
  G4StepPoint* poststep = step->GetPostStepPoint();
  G4ThreeVector pos =
            (step->GetPreStepPoint()->GetPosition() +
             step->GetPostStepPoint()->GetPosition())*0.5;
  G4double  z = pos.z();

  if(1 == step->GetTrack()->GetTrackID())
  {
      G4double kin = step->GetTrack()->GetKineticEnergy();
      G4AnalysisManager::Instance()->FillH1(5, z / CLHEP::mm, kin / CLHEP::MeV);
  }


  if (edepStep <= 0.) return;

  // get stopping power curve
  if ("World" != volname)
  {
      G4AnalysisManager::Instance()->FillH1(4, z / CLHEP::mm, edepStep / CLHEP::MeV);
  }
  // check if we are in scoring volume
  if ("Be_Layer" == volname) {
      fEventAction->AddEdep(edepStep, 0);
  }
  else if ("V_Layer" == volname)
  {
      fEventAction->AddEdep(edepStep, 1);
  }
  else
  {
      return;
  }

  // collect energy deposited in this step
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

