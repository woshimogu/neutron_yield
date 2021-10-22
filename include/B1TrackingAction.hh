//
// Created by mogu on 8/19/21.
//

#ifndef B1_B1TRACKINGACTION_HH
#define B1_B1TRACKINGACTION_HH 1


#include "G4UserTrackingAction.hh"
#include "globals.hh"

class B1DetectorConstruction;
class B1RunAction;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class B1TrackingAction : public G4UserTrackingAction {

public:
    B1TrackingAction(B1RunAction* runAction);
    virtual ~B1TrackingAction();

    virtual void PreUserTrackingAction(const G4Track*);
    virtual void PostUserTrackingAction(const G4Track*);

private:
    B1DetectorConstruction* fDetector;
    B1RunAction*            fRunAction;
};



#endif //B1_B1TRACKINGACTION_HH
