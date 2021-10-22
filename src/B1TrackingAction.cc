//
// Created by mogu on 8/19/21.
//

#include "B1TrackingAction.hh"
#include "B1DetectorConstruction.hh"
#include "B1RunAction.hh"
#include "G4Track.hh"
#include "g4root.hh"
#include "G4Deuteron.hh"
#include "G4Triton.hh"
#include "G4Neutron.hh"
#include "G4He3.hh"
#include "G4Alpha.hh"
#include "G4Proton.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1TrackingAction::B1TrackingAction(B1RunAction* runAction)
        :G4UserTrackingAction(),fRunAction(runAction)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1TrackingAction::~B1TrackingAction()
{}

void B1TrackingAction::PreUserTrackingAction(const G4Track * track)
{
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    /*secondary particle*/
    if (1 != track->GetTrackID()) {
        G4ThreeVector vertex = track->GetVertexPosition();
        G4double x = vertex.x();
        G4double y = vertex.y();
        G4double z = vertex.z();
        const G4ParticleDefinition* pd = track->GetDefinition();
        if(pd == G4Proton::Proton()){
            fRunAction->AddProton();
            //G4cout << "Find a secondary proton" << G4endl;
            analysisManager->FillNtupleDColumn(0,0,x);
            analysisManager->FillNtupleDColumn(0,1,y);
            analysisManager->FillNtupleDColumn(0,2,z);
            analysisManager->AddNtupleRow(0);
            //G4cout << analysisManager->GetNtuple(0)->entries() << G4endl;;
        }
        else if(pd == G4Deuteron::Deuteron()){
            fRunAction->AddDeuteron();
            analysisManager->FillNtupleDColumn(deuteron,0,x);
            analysisManager->FillNtupleDColumn(deuteron,1,y);
            analysisManager->FillNtupleDColumn(deuteron,2,z);
            analysisManager->AddNtupleRow(deuteron);
        }
        else if(pd == G4Triton::Triton()){
            fRunAction->AddTritium();
            analysisManager->FillNtupleDColumn(tritium,0,x);
            analysisManager->FillNtupleDColumn(tritium,1,y);
            analysisManager->FillNtupleDColumn(tritium,2,z);
            analysisManager->AddNtupleRow(tritium);
        }
        else if(pd == G4Neutron::Neutron()){
            fRunAction->AddNeutron();
            analysisManager->FillNtupleDColumn(neutron,0,x);
            analysisManager->FillNtupleDColumn(neutron,1,y);
            analysisManager->FillNtupleDColumn(neutron,2,z);
            analysisManager->AddNtupleRow(neutron);
        }
        else if(pd == G4He3::He3()){
            fRunAction->AddHe3();
            analysisManager->FillNtupleDColumn(heliem3,0,x);
            analysisManager->FillNtupleDColumn(heliem3,1,y);
            analysisManager->FillNtupleDColumn(heliem3,2,z);
            analysisManager->AddNtupleRow(heliem3);
        }
        else if(pd == G4Alpha::Alpha()){
            fRunAction->AddAlpha();
            analysisManager->FillNtupleDColumn(alpha,0,x);
            analysisManager->FillNtupleDColumn(alpha,1,y);
            analysisManager->FillNtupleDColumn(alpha,2,z);
            analysisManager->AddNtupleRow(alpha);
        }
        else{
            return;
        }

    }

}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1TrackingAction::PostUserTrackingAction(const G4Track* track)
{
    // extract Projected Range of primary particle
    if (track->GetTrackID() == 1) {
        G4double z = track->GetPosition().z();
        if(z > 0.0){
            fRunAction->AddProjRange(z);
        }
        G4AnalysisManager::Instance()->FillH1(3, z / CLHEP::mm);
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
