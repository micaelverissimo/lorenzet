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


#ifndef JETREADER_H
#define JETREADER_H

#include "jets/JetInterface.hh"

class JetReaderMessenger;

class JetMCReader : public JetInterface {
  
 
  public:

    JetReader();
    ~JetReader();
  
    // set/get methods
    void SetFileName(G4String name);
    G4String GetFileName() const;
  
    void SetVerboseLevel(G4int i);
    G4int GetVerboseLevel() const; 
  
    void Initialize();

  protected:

    G4String               m_filename;
    G4int                  m_verbose;
    JetReaderMessenger    *m_messenger;
  
 
  
    double        m_jet_orig_e		;
    double        m_jet_orig_et	  ;
    double        m_jet_orig_eta	;
    double        m_jet_orig_phi	;
    double        m_jet_px				;
    double        m_jet_py				;
    double        m_jet_pz				;
    unsigned int  m_jet_n_part    ;
    
    std::vector<float>  *m_jet_electron_photon_e_frac;

    std::vector<float>  *m_s_orig_e				;
    std::vector<float>  *m_s_orig_et			;
    std::vector<float>  *m_s_orig_eta			;
    std::vector<float>  *m_s_orig_phi			;
    std::vector<int>    *m_s_pdg_id			  ;
    std::vector<float>  *m_s_px						;
    std::vector<float>  *m_s_py						;
    std::vector<float>  *m_s_pz						;
    std::vector<float>  *m_s_prod_x				;
    std::vector<float>  *m_s_prod_y				;
    std::vector<float>  *m_s_prod_z				;



  private:

    void InitBranch(TTree* fChain, std::string branch_name, T* param, bool message);
    void link(TTree *t);
    void clear();


    G4bool CheckVertexInsideWorld(const G4ThreeVector& pos) const;
    void Convert( G4Event* g4event );


    


};


inline void JetReader::SetFileName(G4String name)
{
  m_filename= name;
}

inline G4String JetReader::GetFileName() const
{
  return m_filename;
}

inline void JetReader::SetVerboseLevel(G4int i)
{
  m_verbose= i;
}

inline G4int JetReader::GetVerboseLevel() const
{
  return m_verbose;
}



#endif
