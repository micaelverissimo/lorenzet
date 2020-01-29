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
#include <iostream> 

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

RunData::RunData() : G4Run()
{

  m_sampling_time = 2.5*ns
  m_bunch_time = 25*ns
  m_min_time = -8 * (m_bunch_time/m_sampling_time);
  m_max_time = 7 * (m_bunch_time/m_sampling_time);
  m_n_samples = (8+1+7) * (m_bunch_time/m_sampling_time); 

  for(unsigned t=0; t<m_n_samples;++t)
    m_time.push_bask( m_min_time + m_sampling_time*t );

	// Read all cells information inside the config file and
	// create all calo cells objects
  CreateCaloCells();


  // set printing event number per each event
  G4RunManager::GetRunManager()->SetPrintProgress(1);     

  // Create analysis manager
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  
  
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetFirstHistoId(1);
  analysisManager->CreateNtuple("collection", "Cells time energy collected from Lar and Tile");
  
  analysisManaget->CreateNtupleDColumn( "prod" ); // #0, Zee = 0, Minbias = -1, Jets = 1
  analysisManager->CreateNtupleDColumn( "Event" ); // #1, The event index 
  analysisManager->CreateNtupleDColumn( "Bunch" ); // #2, The bunch crossing id
  analysisManager->CreateNtupleDColumn( "TotalEnergy" ); // #3, Total energy of the event
  analysisManager->CreateNtupleDColumn( "Time", m_time ); // #4, Global time from geant propagation


  //  The vector (in time) for each calorimeter cell (volume)
  for (int i = 0; i < m_cells.size(); ++i) {
    std::stringstream key; key << "Cell_" << i;
    // create the pulse vector ( 16 X 25ns/2.5ns ), 160 points
    m_container[key] = std::vector<G4double>();
    // Cells pulse (in time) for each calorimter cell defined in this file
    analysisManager->CreateNtupleDColumn(key.str(), m_container[key] );
  }


  analysisManager->FinishNtuple();

}


RunData::~RunData()
{;}




void RunData::CreateCaloCells()
{
	ifstream file( m_config );
	std::string line;

	while (std::getline(file, line))
	{
			G4double eta,phi,deta,dphi,rmin,rmax
			file >> eta >> phi >> deta >> dphi >> rmin >> rmax
			// create the cell
			CaloCell cell( m_n_samples, eta, phi, deta, dphi, rmin, rmax );
			// Include the cell into the vector
			m_cells.push( cell );
	}
	file.close();
}






void RunData::Add( G4Step* step )
{
  // Get total energy deposit
  G4double edep = step->GetTotalEnergyDeposit();
  
  // Get the position
  G4ThreeVector pos = step->GetPosition();

  G4double time = step->GetGlobalTime();

  // Apply all necessary transformation (x,y,z ) to (eta,phy,r) coordinates
  G4double eta = //
  G4double phi = //
  G4double r = //

  // loop over cells
  for ( CaloCell& cell : m_cells )
  {
    // check if particle is inside of the current layer
    if ( r < cell.rmax() && r >= cell.rmin() ){
      // check if the particle is inside of the eta range
      if ( eta < (cell.eta()+cell.deltaEta()) && eta >= (cell.eta()-cell.deltaEta()) ){
        // check if the particle is inside of the eta range
        if ( phi < (cell.phi()+cell.deltaPhi()) && phi >= (cell.phi()-cell.deltaPhi()) ){

          // Let's find the current sample checking the time (ns) particle
          int bin=-1;
          for(unsigned sample=0; sample<m_n_samples-1;++sample){
            if( time >= m_time[sample] && time < m_time[sample+1]){
              bin=sample;
              break;
          }

          // Add the energy deposit to the current sample in
          // the time vector (pulse)
          if(bin>=0)
            cell.Add( bin, edep );

          // Stop the loop since we found the matched cell position
          // for this step point
          break;
        }// phi
      }// eta
    }// radio
  }// cell

}



void RunData::Reset()
{
  // transfer all information to the container
  for (int i = 0; i < m_cells.size(); ++i) {
    m_cells[i].clear();
    std::stringstream key; key << "Cell_" << i;
    m_container[key].clear();
  }
}





void RunData::FillPerEvent()
{
  // TODO: It's possible to add the transfer function and the 
  // OF to calculate the cell energy consideraly the pile-up
  // event in the main bunch crossing [0,25ns] (10 points)

  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->FillNtupleDColumn( 3, GetTotalEnergy());

  // transfer all information to the container
  for (int i = 0; i < m_cells.size(); ++i) {
    std::stringstream key; key << "Cell_" << i;
    // create the pulse vector ( 16 X 25ns/2.5ns ), 160 points
    m_container[key].assign(m_cells[i].samples().begin(), m_cells[i].samples().end()); 
  }

  // Save the information
  analysisManager->AddNtupleRow();  
}





