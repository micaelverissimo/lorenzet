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


#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcommand.hh"
#include "G4UIdirectory.hh"
#include "G4UIparameter.hh"
#include "PrimaryGeneratorMessenger.hh"
#include "PrimaryGeneratorAction.hh"

PrimaryGeneratorMessenger::PrimaryGeneratorMessenger( PrimaryGeneratorAction* genaction )
  : G4UImessenger(),
    m_primaryAction(genaction)
{
  m_dir= new G4UIdirectory("/generator/");
  m_dir-> SetGuidance("Control commands for primary generator");


  m_select= new G4UIcmdWithAString("/generator/select", this);
  m_select-> SetGuidance("Select generator type");
  m_select-> SetParameterName("generator_type", false, false);
  m_select-> SetCandidates("particleGun hepmcAscii jetReader");
  m_select-> SetDefaultValue("particleGun");
}


PrimaryGeneratorMessenger::~PrimaryGeneratorMessenger()
{
  delete m_select;
  delete m_dir;
}


void PrimaryGeneratorMessenger::SetNewValue(G4UIcommand* command, G4String newValues)
{
  if ( command == m_select) {
    m_primaryAction->SetGenerator(newValues);
    G4cout << "current generator type: " << m_primaryAction-> GetGeneratorName() << G4endl;
  }
}


G4String PrimaryGeneratorMessenger::GetCurrentValue(G4UIcommand* command)
{
  G4String cv, st;
  if (command == m_select) {
    cv= m_primaryAction-> GetGeneratorName();
  }

 return cv;
}
