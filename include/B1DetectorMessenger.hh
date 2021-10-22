//
// Created by mogu on 10/22/21.
//

#ifndef B1_B1DETECTORMESSENGER_HH
#define B1_B1DETECTORMESSENGER_HH

#include "globals.hh"
#include "G4UImessenger.hh"

class B1DetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;

class B1DetectorMessenger : public G4UImessenger
{
public:
    B1DetectorMessenger(B1DetectorConstruction* );
    virtual ~B1DetectorMessenger();

    virtual void SetNewValue(G4UIcommand*, G4String);

    B1DetectorConstruction*  fDetectorConstruction;
    G4UIdirectory*           fDetDirectory;

    G4UIcmdWithAString*      fTargMatCmd;
    G4UIcmdWithADoubleAndUnit* fRangeCmd;


};


#endif //B1_B1DETECTORMESSENGER_HH
