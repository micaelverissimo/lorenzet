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


#ifndef H02_PRIMARY_GENERATOR_ACTION_H
#define H02_PRIMARY_GENERATOR_ACTION_H

#include <map>
#include "globals.hh"
#include "G4VUserPrimaryGeneratorAction.hh"

class G4Event;
class G4VPrimaryGenerator;
class PrimaryGeneratorMessenger;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
  public:

    PrimaryGeneratorAction();
    ~PrimaryGeneratorAction();
  
    virtual void GeneratePrimaries(G4Event* anEvent);
  
    void SetGenerator(G4VPrimaryGenerator* gen);
    void SetGenerator(G4String genname);
  
    G4VPrimaryGenerator* GetGenerator() const;
    G4String GetGeneratorName() const;
  
  private:

    G4VPrimaryGenerator *m_particleGun;
    G4VPrimaryGenerator *m_hepmcAscii;
    G4VPrimaryGenerator *m_pythiaGen;
  
    G4VPrimaryGenerator                       *m_currentGenerator;
    G4String                                   m_currentGeneratorName;
    std::map<G4String, G4VPrimaryGenerator*>   m_gentypeMap;
    PrimaryGeneratorMessenger                 *m_messenger;

};

inline void PrimaryGeneratorAction::SetGenerator(G4VPrimaryGenerator* gen)
{
  m_currentGenerator= gen;
}

inline void PrimaryGeneratorAction::SetGenerator(G4String genname)
{
  std::map<G4String, G4VPrimaryGenerator*>::iterator pos = m_gentypeMap.find(genname);

  if(pos != m_gentypeMap.end()) {
    m_currentGenerator= pos->second;
    m_currentGeneratorName= genname;
  }
}

inline G4VPrimaryGenerator* PrimaryGeneratorAction::GetGenerator() const
{
  return m_currentGenerator;
}

inline G4String PrimaryGeneratorAction::GetGeneratorName() const
{
  return m_currentGeneratorName;
}

#endif
