//
// Created by mogu on 10/22/21.
//

#include "B1DetectorMessenger.hh"
#include "B1DetectorConstruction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

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
/// \file B2aDetectorMessenger.cc
/// \brief Implementation of the B2aDetectorMessenger class

#include "B1DetectorMessenger.hh"
#include "B1DetectorConstruction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorMessenger::B1DetectorMessenger(B1DetectorConstruction* Det)
        : G4UImessenger(),
          fDetectorConstruction(Det)
{
    fDetDirectory = new G4UIdirectory("/det/");
    fDetDirectory->SetGuidance("Detector construction control");

    fTargMatCmd = new G4UIcmdWithAString("/det/setTargetMaterial",this);
    fTargMatCmd->SetGuidance("Select Material of the Target.");
    fTargMatCmd->SetParameterName("choice",false);
    fTargMatCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

    fRangeCmd = new G4UIcmdWithADoubleAndUnit("/det/range",this);
    fRangeCmd->SetGuidance("Define a target thick");
    fRangeCmd->SetParameterName("Range",false);
    fRangeCmd->SetRange("Range>0.");
    fRangeCmd->SetUnitCategory("Length");
    fRangeCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorMessenger::~B1DetectorMessenger()
{
    delete fTargMatCmd;
    delete fRangeCmd;
    delete fDetDirectory;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1DetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
    if( command == fRangeCmd ) {
        fDetectorConstruction
                ->SetRange(fRangeCmd->GetNewDoubleValue(newValue));
    }
    else if(command == fTargMatCmd){
        fDetectorConstruction->SetTargetMaterial(newValue);
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

