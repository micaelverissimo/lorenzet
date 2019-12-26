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
/// \file eventgenerator/HepMC/HepMCEx01/src/HepMCG4AsciiReader.cc
/// \brief Implementation of the HepMCG4AsciiReader class
//
//

#include "jets/JetReader.hh"
#include "jets/JetReaderMessenger.hh"

#include <iostream>
#include <fstream>

JetReader::JetReader()
  :  m_filename("hepmc_input.root"), 
     m_verbose(0),
     m_evt(0)
{
  m_messenger= new JetReaderMessenger(this);
}


JetReader::~JetReader()
{
  release();
  m_f->Close();
  delete m_f;
  delete m_messenger;
}



void JetReader::Initialize()
{
  // Get the ttree from the root file
  G4cout << "Open the root file: " << m_filename << G4end;
  m_f = new TFile( m_filename , "read" )
  m_ttree = (TTree*)m_f->Get("t");
  m_evt=0;
  G4cout << "Link all branches..." << G4endl;
  link( m_ttree );
  G4cout << "Allocate..." << G4endl;
  alloc();
  G4cout << "Done" << G4endl;
}







template <class T> 
void JetReader::InitBranch(TTree* fChain, std::string branch_name, T* param, bool message){

  std::string bname = branch_name;
  if (fChain->GetAlias(bname.c_str()))
     bname = std::string(fChain->GetAlias(bname.c_str()));

  if (!fChain->FindBranch(bname.c_str()) && message) {
    G4cout << "[WARNING] unknown branch " << bname << G4endl;
    return;  
  }
  fChain->SetBranchStatus(bname.c_str(), 1.);
  fChain->SetBranchAddress(bname.c_str(), param);
}



void JetReader::link(TTree *t){
  
  InitBranch( t, "jet_orig_e"		,      &m_jet_orig_e		);
  InitBranch( t, "jet_orig_et"	,      &m_jet_orig_et		);
  InitBranch( t, "jet_orig_eta"	,      &m_jet_orig_eta	);
  InitBranch( t, "jet_orig_phi"	,      &m_jet_orig_phi	);
  InitBranch( t, "jet_px"				,      &m_jet_px				);
  InitBranch( t, "jet_py"				,      &m_jet_py				);
  InitBranch( t, "jet_pz"				,      &m_jet_pz				);
  InitBranch( t, "jet_electron_photon_e_frac",&m_jet_electron_photon_e_frac);

  InitBranch( t, "s_pdg_id"			,      &m_s_pdg_id			);
  InitBranch( t, "s_orig_e"			,      &m_s_orig_e			);
  InitBranch( t, "s_orig_et"		,      &m_s_orig_et			);
  InitBranch( t, "s_orig_eta"		,      &m_s_orig_eta		);
  InitBranch( t, "s_orig_phi"		,      &m_s_orig_phi		);
  InitBranch( t, "s_px"					,      &m_s_px					);
  InitBranch( t, "s_py"					,      &m_s_py					);
  InitBranch( t, "s_px"					,      &m_s_pz					);
  InitBranch( t, "s_prod_x"			,      &m_s_prod_x			);
  InitBranch( t, "s_prod_y"			,      &m_s_prod_y			);
  InitBranch( t, "s_prod_z"			,      &m_s_prod_z			);
}




void JetReader::clear(){
 
  m_jet_orig_e		= 0.0;
  m_jet_orig_et	  = 0.0;
  m_jet_orig_eta	= 0.0;
  m_jet_orig_phi	= 0.0;
  m_jet_px				= 0.0;
  m_jet_py				= 0.0;
  m_jet_pz				= 0.0;
  m_jet_n_part    = 0.0;
  m_jet_electron_photon_e_frac = 0.0;

  m_s_orig_e		->clear();
  m_s_orig_et		->clear();
  m_s_orig_eta	->clear();
  m_s_orig_phi	->clear();
  m_s_pdg_id		->clear();
  m_s_px				->clear();
  m_s_py				->clear();
  m_s_pz				->clear();
  m_s_prod_x		->clear();
  m_s_prod_y		->clear();
  m_s_prod_z		->clear();
}


void JetReader::alloc(){

  m_s_orig_e		= new std::vector<float>();
  m_s_orig_et		= new std::vector<float>();
  m_s_orig_eta	= new std::vector<float>();
  m_s_orig_phi	= new std::vector<float>();
  m_s_pdg_id		= new std::vector<float>();
  m_s_px				= new std::vector<float>();
  m_s_py				= new std::vector<float>();
  m_s_pz				= new std::vector<float>();
  m_s_prod_x		= new std::vector<float>();
  m_s_prod_y		= new std::vector<float>();
  m_s_prod_z		= new std::vector<float>();
}



void JetReader::release(){

  delete m_s_orig_e		;
  delete m_s_orig_et	;
  delete m_s_orig_eta	;
  delete m_s_orig_phi	;
  delete m_s_pdg_id		;
  delete m_s_px				;
  delete m_s_py				;
  delete m_s_pz				;
  delete m_s_prod_x		;
  delete m_s_prod_y		;
  delete m_s_prod_z		;
}




// Call by geant
void JetReader::GeneratePrimaryVertex( G4Event* anEvent )
{
  // clear the old event one;
  clear();

  // Check if we have an event inside of the root three
  if ( m_evt <  m_ttree->GetEntries() ){

    m_ttree->GetEntry(m_evt);
    Convert( anEvent );
    m_evt++;

  }else{
    G4cout << "JetReader: no generated particles. run terminated..." << G4endl;
    G4RunManager::GetRunManager()->AbortRun();
  }
    

}





G4bool JetReader::CheckVertexInsideWorld(const G4ThreeVector& pos) const
{
  G4Navigator* navigator= G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
  G4VPhysicalVolume* world= navigator->GetWorldVolume();
  G4VSolid* solid= world-> GetLogicalVolume()->GetSolid();
  EInside qinside= solid->Inside(pos);
  if( qinside != kInside) return false;
  else return true;
}





void JetReader::Convert( G4Event* g4event )
{

  for ( unsigned int i=0; i < m_jet_n_part; ++i )
  {
    G4LorentzVector xvtx( m_s_prod_x->at(i), m_s_prod_y->at(i), m_s_prod_z->at(i), 0.0);
    G4PrimaryVertex* g4vtx= new G4PrimaryVertex(xvtx.x()*mm, xvtx.y()*mm, xvtx.z()*mm, xvtx.t()*mm/c_light);
    
    if (! CheckVertexInsideWorld(xvtx.vect()*mm)) continue;

    G4int pdgcode= m_s_pdg_id->at(i);
    G4LorentzVector p( m_s_px->at(i), m_s_py->at(i), m_s_pz->at(i),  m_s_orig_e->at(i) );
    G4PrimaryParticle* g4prim = new G4PrimaryParticle(pdgcode, p.x()*GeV, p.y()*GeV, p.z()*GeV);
    g4vtx->SetPrimary(g4prim);
    g4event->AddPrimaryVertex(g4vtx);
  }


}


