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
/// \file B1RunAction.hh
/// \brief Definition of the B1RunAction class

#ifndef B1RunAction_h
#define B1RunAction_h 1

#include "G4UserRunAction.hh"
#include "G4Accumulable.hh"
#include "globals.hh"
#include "g4root.hh"

class G4Run;

/// Run action class
///
/// In EndOfRunAction(), it calculates the dose in the selected volume 
/// from the energy deposit accumulated via stepping and event actions.
/// The computed dose is then printed on the screen.
enum{
    proton = 0,
    deuteron = 1,
    tritium = 2,
    neutron = 3,
    heliem3 = 4,
    alpha = 5
};


class B1RunAction : public G4UserRunAction
{
  public:
    B1RunAction();
    virtual ~B1RunAction();

    // virtual G4Run* GenerateRun();
    virtual void BeginOfRunAction(const G4Run*);
    virtual void   EndOfRunAction(const G4Run*);

    void AddEdep (G4double edepBe,G4double edepV);

    inline void AddProjRange (G4double x);

    inline void AddProton(){++fProton;};
    inline void AddDeuteron(){ ++fDeuteron;};
    inline void AddTritium(){++fTritium;};
    inline void AddNeutron(){++fNeutron;};
    inline void AddHe3(){++fHelium3;};
    inline void AddAlpha(){++fAlpha;};

  private:
    void BookHisto();

    G4AnalysisManager*      fAnalysisManager;

    G4Accumulable<G4double> fEdepBe;
    G4Accumulable<G4double> fEdep2Be;
    G4Accumulable<G4double> fEdepV;
    G4Accumulable<G4double> fEdep2V;
    G4Accumulable<G4double> fProjRange;

    G4Accumulable<G4int>    fProton;
    G4Accumulable<G4int>    fDeuteron;
    G4Accumulable<G4int>    fTritium;
    G4Accumulable<G4int>    fNeutron;
    G4Accumulable<G4int>    fHelium3;
    G4Accumulable<G4int>    fAlpha;

};

inline void B1RunAction::AddProjRange (G4double x)
{
    fProjRange  += x;
}


#endif

