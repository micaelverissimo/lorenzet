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


#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "HepMCReader.hh"
#include "PrimaryGeneratorAction.hh"
#include "PrimaryGeneratorMessenger.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleGunMessenger.hh"


PrimaryGeneratorAction::PrimaryGeneratorAction(): G4VUserPrimaryGeneratorAction()
{
  // default generator is particle gun.
  auto gun  = new G4ParticleGun();
  //m_particleGunMessenger = new G4ParticleGunMessenger( gun );
  m_currentGenerator = m_particleGun = gun;

  m_currentGeneratorName= "particleGun";
  m_hepmcAscii= new HepMCReader();

  m_gentypeMap["particleGun"] = m_particleGun;
  m_gentypeMap["hepmcAscii"]  = m_hepmcAscii;

  m_messenger= new PrimaryGeneratorMessenger(this);
}


PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete m_messenger;
  //delete m_particleGunMessenger;
}


void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  if(m_currentGenerator)
    m_currentGenerator->GeneratePrimaryVertex(anEvent);
  else
    G4Exception("PrimaryGeneratorAction::GeneratePrimaries",
                "InvalidSetup", FatalException,
                "Generator is not instanciated.");
}



