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


SteppingAction::SteppingAction( bool save_deposit)
  : G4UserSteppingAction(),
    m_saveDeposit(save_deposit)
{;}


SteppingAction::~SteppingAction()
{;}



// Detector ROI dimesions
//                (0,0,0)
//                   |
//       |--------- EM ----------|----------- HAD ----------|
//         15cm     30cm     3cm    40cm       40cm    20cm
//  +    +--------------------------------------------------+
//  |    |      |           |    |         |         |      |   
// 48cm  |      |           |    |         |         |      |
//  |    |      |           |    |         |         |      |
//  +    +--------------------------------------------------+
//
int SteppingAction::WhichZBin(double zpos){

  // 480mm + 1000mm = 1480mm
  int offset = 240;
  zpos = zpos+offset;

  // Total of 80 layers (abs+gap)
  // 25 layers (EM1) 15 cm
  // 50 layers (EM2) 30 cm
  // 15 layers (EM3) 3 cm
  if (zpos < 150.) return 0;
  else if (zpos < 450.) return 1;
  else if (zpos < 480.) return 2;


  // Total of 5 layers (abs+gap) 
  // 2 layers (HAD) 40 cm
  // 2 layers (HAD) 40 cm
  // 2 layers (HAD) 20 cm
  else if (zpos < 880.) return 3;
  else if (zpos < 1280.) return 4;
  else return 5;
}



// define 480 mm X 480 mm X 480 mm as RoI
int SteppingAction::WhichXYbin(double xpos, double ypos, int zbin){

  // Eletromagnetic 
  int xbin = -1;
  int ybin = -1;
  // Eletromagnetic 
  int nbins1x = 3;
  int nbins1y = 96;
  // Eletromagnetic 
  int nbins2x = 12;
  int nbins2y = 12;
  // Eletromagnetic 
  int nbins3x = 12;
  int nbins3y = 6;

  // TileCal
  int nbins4x = 8;
  int nbins4y = 8;
  // TileCal
  int nbins5x = 8;
  int nbins5y = 8;
  // TileCal
  int nbins6x = 4;
  int nbins6y = 4;



  int nbinsx[]={nbins1x,nbins2x,nbins3x,nbins4x,nbins5x,nbins6x};
  int nbinsy[]={nbins1y,nbins2y,nbins3y,nbins4y,nbins5y,nbins6y};
  

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

  // Eletromagnetic Layers
  int lvl1 = nbins1x * nbins1y;
  int lvl2 = nbins2x * nbins2y;
  int lvl3 = nbins3x * nbins3y;

  // Hadronic Layers
  int lvl4 = nbins4x * nbins4y;
  int lvl5 = nbins5x * nbins5y;
  int lvl6 = nbins6x * nbins6y;


  // Deposit outside of the RoI
  if ((xbin == -1) || (ybin == -1)) { // Outlier bins 
    return lvl1 + lvl2 + lvl3 + lvl4 + lvl5 + lvl6 + zbin;
  }

  if (zbin == 0) { // Eletromagnetic layer 1
    return xbin * nbins1y + ybin;
  } 
  else if (zbin == 1) { // Eletromagnetic layer 2
    return lvl1 + (xbin * nbins2y + ybin);
  }
  else if (zbin == 2) { // Eletromagnetic layer 3
    return (lvl1 + lvl2) + (xbin * nbins3y + ybin);
  }
  else if (zbin == 3) { // Hadronic layer 1
    return (lvl1 + lvl2 + lvl3) + (xbin * nbins4y + ybin);
  }
  else if (zbin == 4) { // Hadronic layer 2
    return (lvl1 + lvl2 + lvl3 + lvl4) + (xbin * nbins5y + ybin);
  }
  else{ // Hadronic layer 3
    return (lvl1 + lvl2 + lvl3 + lvl4 + lvl5) + (xbin * nbins6y + ybin);
  }



}

void SteppingAction::UserSteppingAction(const G4Step* step)
{
  
  // energy deposit
  G4double edep = step->GetTotalEnergyDeposit();

  G4StepPoint* point1 = step->GetPreStepPoint();
  G4ThreeVector pos1 = point1->GetPosition();

  int mybin = WhichXYbin(pos1.x(),pos1.y(),WhichZBin(pos1.z()));
  
  RunData* runData = static_cast<RunData*> (G4RunManager::GetRunManager()->GetNonConstCurrentRun());

  runData->AddCell(mybin, edep); 
  if (m_saveDeposit)
    runData->AddDeposit(pos1.x(), pos1.y(), pos1.z(), edep);
}

