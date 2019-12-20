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

#include "RunData.hh"
#include "Analysis.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"


RunData::RunData() : G4Run()//, fNumCells(4815)
{

  // set printing event number per each event
  G4RunManager::GetRunManager()->SetPrintProgress(1);     

  // Create analysis manager
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  
  // Create directories 
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetFirstHistoId(1);
  analysisManager->CreateNtuple("fancy_tree", "Edep and TrackL");


  // Create all cells branches
  int total_bins = 504 + 3;  // 3 overflow bins for the three calo layers
  for (int i = 0; i < total_bins; ++i) {
    std::stringstream out; out << i;
    analysisManager->CreateNtupleDColumn("cell_" + out.str());
  }

  //m_deposit_x = new std::vector<G4double>();
  //m_deposit_y = new std::vector<G4double>();
  //m_deposit_z = new std::vector<G4double>();
  //m_deposit_energy = new std::vector<G4double>();


  // Create the total energy branch
  analysisManager->CreateNtupleDColumn("TotalEnergy");
  analysisManager->CreateNtupleDColumn("deposit_x"      , m_deposit_x);
  analysisManager->CreateNtupleDColumn("deposit_y"      , m_deposit_y);
  analysisManager->CreateNtupleDColumn("deposit_z"      , m_deposit_z);
  analysisManager->CreateNtupleDColumn("deposit_energy" , m_deposit_energy);
  analysisManager->FinishNtuple();



  for ( G4int i=0; i < kNumCells; i++) {
    m_cell_energy[i] = 0.;
  }  
}


RunData::~RunData()
{;}



void RunData::FillPerEvent()
{
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  for (int i = 0; i < kNumCells; ++i) {
    analysisManager->FillNtupleDColumn(i, m_cell_energy[i]);
  }
  analysisManager->FillNtupleDColumn(kNumCells, GetTotalEnergy());

   
  // Fill all simulated points
  for(unsigned int i = 0; i < m_deposit.size(); ++i){
    m_deposit_x.push_back( m_deposit.at(i).x );
    m_deposit_y.push_back( m_deposit.at(i).y );
    m_deposit_z.push_back( m_deposit.at(i).z );
    m_deposit_energy.push_back( m_deposit.at(i).energy );
  }
  

  /*
  for(unsigned int i = 0; i < m_points.size(); ++i){

    analysisManager->FillNtupleDColumn(kNumCells+1, m_deposit.at(i).x      );
    analysisManager->FillNtupleDColumn(kNumCells+2, m_deposit.at(i).y      );
    analysisManager->FillNtupleDColumn(kNumCells+3, m_deposit.at(i).z      );
    analysisManager->FillNtupleDColumn(kNumCells+4, m_deposit.at(i).energy );

    //analysisManager->FillNtupleDColumn(kNumCells+1, m_deposit_x);
    //analysisManager->FillNtupleDColumn(kNumCells+2, m_deposit_y);
    //analysisManager->FillNtupleDColumn(kNumCells+3, m_deposit_z);
    //analysisManager->FillNtupleDColumn(kNumCells+4, m_deposit_energy);
  
  }*/

  analysisManager->AddNtupleRow();  
}


void RunData::Reset()
{ 
  for ( G4int i=0; i<kNumCells; i++) {
    m_cell_energy[i] = 0.;
  } 

  m_deposit.clear();
  m_deposit_x.clear();
  m_deposit_y.clear();
  m_deposit_z.clear();
  m_deposit_energy.clear();
}

