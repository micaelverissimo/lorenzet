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

#ifndef RunData_h
#define RunData_h 1

#include "G4Run.hh"
#include "globals.hh"
#include <vector>
#include <map>
#include <string>

#include "action/CaloCell.hh"


class RunData : public G4Run
{
  public:

    RunData();
    virtual ~RunData();

    // Fill event into the persistent ntuple
    void FillPerEvent();
    // Reset all variables usage to store the pulses/cells energy
    void Reset();
    
    void SetTotalEnergy(G4double e){TotalEnergy = e;};
    G4double GetTotalEnergy(){return TotalEnergy;};
  
    
    void Add(G4Step *step);
  
  private:

    void CreateCaloCells();

    std::vector<CaloCell> m_cells;

    std::map<std::string, std::vector<G4double>> m_container;

    G4double TotalEnergy;
  
};

  
  

#endif

