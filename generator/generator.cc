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
/// \file eventgenerator/HepMC/HepMCEx02/HepMCEx02.cc
/// \brief Main program of the eventgenerator/HepMC/HepMCEx02 example
//
//
//
// 
// --------------------------------------------------------------
//      GEANT 4 - example of HepMC-interface
// --------------------------------------------------------------

#include "G4Types.hh"

#include "G4RunManager.hh"
#include "G4UImanager.hh"

#include "DetectorConstruction.hh"
#include "FTFP_BERT.hh"
#include "PrimaryGeneratorAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

int main(int argc, char** argv)
{
  G4UIExecutive* ui = nullptr;
  if ( argc == 1 ) {
    ui = new G4UIExecutive(argc, argv);
  }

  G4RunManager* runManager= new G4RunManager;

  // User Initialization classes (mandatory)
  G4VUserDetectorConstruction* detector = new DetectorConstruction;
  runManager-> SetUserInitialization(detector);
  
  G4VUserPhysicsList* physics = new FTFP_BERT();
  runManager->SetUserInitialization(physics);
  runManager->Initialize();

  G4VUserPrimaryGeneratorAction* gen_action = new PrimaryGeneratorAction();
  runManager->SetUserAction(gen_action);
  
  G4UserEventAction* event_action = new EventAction();
  runManager->SetUserAction(event_action);
  
  G4UserSteppingAction* stepping_action = new SteppingAction();
  runManager->SetUserAction(stepping_action);


  G4VisManager* visManager= new G4VisExecutive();
  visManager->Initialize();


  G4UImanager* UImanager = G4UImanager::GetUIpointer();  

  if (!ui) { // batch mode
    visManager-> SetVerboseLevel("quiet");
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command+fileName);    
  } else {  
    ui-> SessionStart();
    delete ui;
  }


  delete visManager;
  delete runManager;
}

