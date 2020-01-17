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


#include "G4UIdirectory.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "hepmc/HepMCReaderMessenger.hh"
#include "hepmc/HepMCReader.hh"


HepMCReaderMessenger::HepMCReaderMessenger(HepMCReader* agen)
  : m_gen(agen)
{
  m_dir= new G4UIdirectory("/generator/hepmcAscii/");
  m_dir->SetGuidance("Reading HepMC event from an Ascii file");

  m_verbose= new G4UIcmdWithAnInteger("/generator/hepmcAscii/verbose", this);
  m_verbose-> SetGuidance("Set verbose level");
  m_verbose-> SetParameterName("verboseLevel", false, false);
  m_verbose-> SetRange("verboseLevel>=0 && verboseLevel<=1");

  m_open= new G4UIcmdWithAString("/generator/hepmcAscii/open", this);
  m_open-> SetGuidance("(re)open data file (HepMC Ascii format)");
  m_open-> SetParameterName("input ascii file", true, true);
}


HepMCReaderMessenger::~HepMCReaderMessenger()
{
  delete m_verbose;
  delete m_open;
  delete m_dir;
}


void HepMCReaderMessenger::SetNewValue(G4UIcommand* command, G4String newValues)
{
  if (command==m_verbose) {
    int level= m_verbose-> GetNewIntValue(newValues);
    m_gen-> SetVerboseLevel(level);
  } else if (command==m_open) {
    m_gen-> SetFileName(newValues);
    G4cout << "HepMC Ascii inputfile: " << m_gen->GetFileName() << G4endl;
    m_gen->Initialize();
  }
}


G4String HepMCReaderMessenger::GetCurrentValue(G4UIcommand* command)
{
  G4String cv;
  if (command == m_verbose) {
    cv= m_verbose->ConvertToString(m_gen-> GetVerboseLevel());
  } else  if (command == m_open) {
    cv= m_gen->GetFileName();
  }
  return cv;
}
