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

#include "SteppingAction.hh"
#include "RunData.hh"
#include "DetectorConstruction.hh"
#include "G4Step.hh"
#include "G4RunManager.hh"


SteppingAction::SteppingAction(
                      const DetectorConstruction* detectorConstruction)
  : G4UserSteppingAction(),
    fDetConstruction(detectorConstruction)
{
}


SteppingAction::~SteppingAction()
{ 
}


int SteppingAction::WhichZBin(double zpos){
  if (zpos < -150.) return 0;
  else if (zpos < 197.) return 1;
  else return 2;
}



// define 480 mm X 480 mm X 480 mm as RoI
int SteppingAction::WhichXYbin(double xpos, double ypos, int zbin){

  int xbin = -1;
  int ybin = -1;
  int nbins1x = 3;
  int nbins2x = 12;
  int nbins3x = 12;
  int nbins1y = 96;
  int nbins2y = 12;
  int nbins3y = 6;
  int nbinsx[]={nbins1x,nbins2x,nbins3x};
  int nbinsy[]={nbins1y,nbins2y,nbins3y};
  for (int i=1; i<=nbinsx[zbin]; i++){
    if ((xpos < -240 + i*480/nbinsx[zbin]) && (xpos > -240)){
      xbin = i - 1;
      break;
    }
  }
  for (int i=1; i<=nbinsy[zbin]; i++){
    if ((ypos < -240 +i*480/nbinsy[zbin]) && (ypos > -240)){
      ybin = i - 1;
      break;
    }
  }

  int lvl1 = nbins1x * nbins1y;
  int lvl2 = nbins2x * nbins2y;
  int lvl3 = nbins3x * nbins3y;

  if ((xbin == -1) || (ybin == -1)) {
    return lvl1 + lvl2 + lvl3 + zbin;
  }

  if (zbin == 0) {
    return xbin * nbins1y + ybin;
  } 
  else if (zbin == 1) {
    return lvl1 + (xbin * nbins2y + ybin);
  }
  else {
    return (lvl1 + lvl2) + (xbin * nbins3y + ybin);
  }


}

void SteppingAction::UserSteppingAction(const G4Step* step)
{
  
  // energy deposit
  G4double edep = step->GetTotalEnergyDeposit();

  G4StepPoint* point1 = step->GetPreStepPoint();
  G4ThreeVector pos1 = point1->GetPosition();

  int mybin = WhichXYbin(pos1.x(),pos1.y(),WhichZBin(pos1.z()));
  
  RunData* runData = static_cast<RunData*>
    (G4RunManager::GetRunManager()->GetNonConstCurrentRun());

  runData->AddCell(mybin, edep); 
  //runData->AddDeposit(pos1.x(), pos1.y(), pos1.z(), edep);
}

