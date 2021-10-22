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
/// \file B1RunAction.cc
/// \brief Implementation of the B1RunAction class

#include "B1RunAction.hh"
#include "B1PrimaryGeneratorAction.hh"
#include "B1DetectorConstruction.hh"
// #include "B1Run.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4AccumulableManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1RunAction::B1RunAction()
: G4UserRunAction(),
  fEdepBe(0.),
  fEdep2Be(0.),
  fEdepV(0.),
  fEdep2V(0.),
  fProjRange(0.),
  fDeuteron(0),
  fProton(0),
  fTritium(0),
  fNeutron(0),
  fHelium3(0),
  fAlpha(0),
  fAnalysisManager(nullptr)
{ 
  // add new units for dose
  // 
  const G4double milligray = 1.e-3*gray;
  const G4double microgray = 1.e-6*gray;
  const G4double nanogray  = 1.e-9*gray;  
  const G4double picogray  = 1.e-12*gray;
   
  new G4UnitDefinition("milligray", "milliGy" , "Dose", milligray);
  new G4UnitDefinition("microgray", "microGy" , "Dose", microgray);
  new G4UnitDefinition("nanogray" , "nanoGy"  , "Dose", nanogray);
  new G4UnitDefinition("picogray" , "picoGy"  , "Dose", picogray); 

  // Register accumulable to the accumulable manager
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->RegisterAccumulable(fEdepBe);
  accumulableManager->RegisterAccumulable(fEdep2Be);
  accumulableManager->RegisterAccumulable(fEdepV);
  accumulableManager->RegisterAccumulable(fEdep2V);
  accumulableManager->RegisterAccumulable(fProjRange);

  accumulableManager->RegisterAccumulable(fProton);
  accumulableManager->RegisterAccumulable(fDeuteron);
  accumulableManager->RegisterAccumulable(fTritium);
  accumulableManager->RegisterAccumulable(fNeutron);
  accumulableManager->RegisterAccumulable(fHelium3);
  accumulableManager->RegisterAccumulable(fAlpha);
  //G4AnalysisManager
  //BookHisto();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1RunAction::~B1RunAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1RunAction::BeginOfRunAction(const G4Run*) {
    // inform the runManager to save random number seed
    G4RunManager::GetRunManager()->SetRandomNumberStore(false);

    if (!fAnalysisManager) { BookHisto(); }
    // reset accumulables to their initial values
    G4AccumulableManager *accumulableManager = G4AccumulableManager::Instance();
    accumulableManager->Reset();
    /*const B1PrimaryGeneratorAction *generatorAction
            = static_cast<const B1PrimaryGeneratorAction *>
            (G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());*/
    //if (generatorAction) {
     //   G4double energy = generatorAction->GetParticleGun()->GetParticleEnergy();
    //    G4String filename;
    //    filename += "pBe" ;
     //   filename += G4BestUnit(energy,"Energy");
     //   filename += ".o";
        if (fAnalysisManager->IsActive()) {
            fAnalysisManager->OpenFile("pBe");
        }
    //}

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1RunAction::EndOfRunAction(const G4Run* run)
{
  G4int nofEvents = run->GetNumberOfEvent();
  if (nofEvents == 0) return;

  // Merge accumulables 
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Merge();

  // Compute average deposited energy = averaged energy deposit in a run and its variance
  //
  G4double edepBe  = fEdepBe.GetValue() / nofEvents;
  G4double edepV = fEdepV.GetValue() / nofEvents;
  G4double projRange = fProjRange.GetValue() / nofEvents;

  G4double nproton    = (G4double)fProton.GetValue() / (G4double)nofEvents;
  G4double ndeuteron  = (G4double)fDeuteron.GetValue() / (G4double)nofEvents;
  G4double ntriton    = (G4double)fTritium.GetValue() / (G4double)nofEvents;
  G4double nneutron   = (G4double)fNeutron.GetValue() / (G4double)nofEvents;
  G4double nhe3       = (G4double)fHelium3.GetValue() / (G4double)nofEvents;
  G4double nalpha     = (G4double)fAlpha.GetValue() / (G4double)nofEvents;

  if(IsMaster())
  {
      G4double binWidth = fAnalysisManager->GetH1Width(4);
      G4double fac = (MeV/mm)/((G4double)nofEvents * binWidth);
      fAnalysisManager->ScaleH1(4, fac);

      fac = (MeV)/((G4double)nofEvents);
      fAnalysisManager->ScaleH1(5, fac);
  }

  // Run conditions
  //  note: There is no primary generator action object for "master"
  //        run manager for multi-threaded mode.
  const B1PrimaryGeneratorAction* generatorAction
   = static_cast<const B1PrimaryGeneratorAction*>
     (G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());
  G4String runCondition;
  if (generatorAction)
  {
    const G4GeneralParticleSource* particleGun = generatorAction->GetParticleGun();
    runCondition += particleGun->GetParticleDefinition()->GetParticleName();
    runCondition += " of ";
    G4double particleEnergy = particleGun->GetParticleEnergy();
    runCondition += G4BestUnit(particleEnergy,"Energy");
  }



  // Print
  //  
  if (IsMaster()) {
    G4cout
     << G4endl
     << "--------------------End of Global Run-----------------------";
      const B1DetectorConstruction* det
              = static_cast<const B1DetectorConstruction*>
              (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
      if(det){
          G4cout << G4endl
          << " The Thick of " << det->GetTargetMaterialName() <<" Target is " << G4BestUnit(det->GetRange(),"Length");
      }
  }
  else {
    G4cout
     << G4endl
     << "--------------------End of Local Run------------------------";
  }


  G4cout
     << G4endl
     << " The run consists of " << nofEvents << " "<< runCondition
     << G4endl
     << " 平均在Be靶内的沉积能量 "
     << G4BestUnit(edepBe,"Energy")
     << G4endl
     << " 平均在V靶内的沉积能量 "
     << G4BestUnit(edepV,"Energy")
     << G4endl
     << " Averaged projected range "
     << G4BestUnit(projRange,"Length")
     << G4endl
     << " Secondary proton per proton "
     << nproton
     << G4endl
     << " Secondary deuteron per proton "
     << ndeuteron
     << G4endl
     << " Secondary triton per proton "
     << ntriton
     << G4endl
     << " Secondary neutron per proton "
     << nneutron
     << G4endl
     << " Secondary He3 per proton "
     << nhe3
     << G4endl
     << " Secondary alpha per proton "
     << nalpha
     << G4endl
     << "------------------------------------------------------------"
     << G4endl
     << G4endl;

    if (fAnalysisManager->IsActive() )
    {

        fAnalysisManager->Write();
        fAnalysisManager->CloseFile();

        G4cout << "\n----> Histograms and ntuples are saved\n" << G4endl;
        delete fAnalysisManager;
        fAnalysisManager = 0;
    }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1RunAction::AddEdep(G4double edepBe, G4double edepV)
{

    /*在Be内的能量沉积*/
        fEdepBe  += edepBe;
        fEdep2Be += edepBe * edepBe;
    /*在V内的能量沉积*/
        fEdepV  += edepV;
        fEdep2V += edepV * edepV;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1RunAction::BookHisto()
{
    // Create or get analysis manager
    // The choice of analysis technology is done via selection of a namespace
    // in HistoManager.hh
    fAnalysisManager = G4AnalysisManager::Instance();
    fAnalysisManager->SetVerboseLevel(1);
    fAnalysisManager->SetActivation(true);
    fAnalysisManager->SetFirstHistoId(0);
    fAnalysisManager->SetFirstNtupleId(0);
    fAnalysisManager->SetFirstNtupleColumnId(0);
    fAnalysisManager->SetNtupleMerging(true);

    // Define histograms start values
    const G4int kMaxHisto = 4;
    const G4String id[] = { "h0", "h1", "h2", "h3", "h4" ,"h5"};
    const G4String title[] =
            { "Edep(MeV) in Breyllium Layer",                    //0
              "Edep(MeV) in Vanadium Layer",                     //1
              "Edep(MeV) in the target",                         //2
              "projectile range" ,                               //3
              "Stopping power curve",                            //4
              "Primary proton energy depth profile"              //5
            };
    // Default values (to be reset via /analysis/h1/set command)
    const G4int nbins = 200;
    const G4double vmin[] = {0., 0., 0., 0., 0., 0.};
    const G4double vmax[] = {13., 7., 13., 1.3, 1.3, 1.3};

    // Create all histograms as inactivated
    // as we have not yet set nbins, vmin, vmax
    fAnalysisManager->CreateH1(id[0], title[0], nbins, vmin[0], vmax[0]);
    fAnalysisManager->CreateH1(id[1], title[1], nbins, vmin[1], vmax[1]);
    fAnalysisManager->CreateH1(id[2], title[2], nbins, vmin[2], vmax[2]);
    fAnalysisManager->CreateH1(id[3], title[3], nbins, vmin[3], vmax[3]);
    fAnalysisManager->CreateH1(id[4], title[4], nbins, vmin[4], vmax[4]);
    fAnalysisManager->CreateH1(id[5], title[5], nbins, vmin[5], vmax[5]);

    //Set title
    fAnalysisManager->SetH1XAxisTitle(0,"Energy(MeV)");
    fAnalysisManager->SetH1YAxisTitle(0,"Counts");

    fAnalysisManager->SetH1XAxisTitle(1,"Energy(MeV)");
    fAnalysisManager->SetH1YAxisTitle(1,"Counts");

    fAnalysisManager->SetH1XAxisTitle(2,"Energy(MeV)");
    fAnalysisManager->SetH1YAxisTitle(2,"Counts");

    fAnalysisManager->SetH1XAxisTitle(3,"Depth(mm)");
    fAnalysisManager->SetH1YAxisTitle(3,"Counts");

    fAnalysisManager->SetH1XAxisTitle(4,"Depth(mm)");
    fAnalysisManager->SetH1YAxisTitle(4,"MeV/mm");

    fAnalysisManager->SetH1XAxisTitle(5,"Depth(mm)");
    fAnalysisManager->SetH1YAxisTitle(5,"Proton Energy(MeV)");

    //Set Ntuple
    fAnalysisManager->CreateNtuple("N1","Generation position of proton");
    fAnalysisManager->CreateNtupleDColumn("x");
    fAnalysisManager->CreateNtupleDColumn("y");
    fAnalysisManager->CreateNtupleDColumn("z");
    fAnalysisManager->FinishNtuple();

    fAnalysisManager->CreateNtuple("N2","Generation position of deuterium");
    fAnalysisManager->CreateNtupleDColumn("x");
    fAnalysisManager->CreateNtupleDColumn("y");
    fAnalysisManager->CreateNtupleDColumn("z");
    fAnalysisManager->FinishNtuple();

    fAnalysisManager->CreateNtuple("N3","Generation position of tritium");
    fAnalysisManager->CreateNtupleDColumn("x");
    fAnalysisManager->CreateNtupleDColumn("y");
    fAnalysisManager->CreateNtupleDColumn("z");
    fAnalysisManager->FinishNtuple();

    fAnalysisManager->CreateNtuple("N4","Generation position of neutron");
    fAnalysisManager->CreateNtupleDColumn("x");
    fAnalysisManager->CreateNtupleDColumn("y");
    fAnalysisManager->CreateNtupleDColumn("z");
    fAnalysisManager->FinishNtuple();

    fAnalysisManager->CreateNtuple("N5","Generation position of He3");
    fAnalysisManager->CreateNtupleDColumn("x");
    fAnalysisManager->CreateNtupleDColumn("y");
    fAnalysisManager->CreateNtupleDColumn("z");
    fAnalysisManager->FinishNtuple();

    fAnalysisManager->CreateNtuple("N6","Generation position of Alpha");
    fAnalysisManager->CreateNtupleDColumn("x");
    fAnalysisManager->CreateNtupleDColumn("y");
    fAnalysisManager->CreateNtupleDColumn("z");
    fAnalysisManager->FinishNtuple();

}