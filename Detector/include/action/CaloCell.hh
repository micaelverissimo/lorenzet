#ifndef CaloCell_h
#define CaloCell_h 1




#include "globals.hh"
#include <vector>


class CaloCell{
  

  public:

    CaloCell( G4double samples, G4double eta, G4double phi, G4double deta, 
              G4double dphi, G4double rmin, G4double rmax );

    ~CaloCell();
    
    // Eta/phi cell centers
    G4double eta();
    G4double phi();

    // Eta/phi delta dimensions
    G4double deltaEta();
    G4double deltaPhi();

    // R min/max for the cell layer
    G4double rmin();
    G4double rmax();

    
    bool Add( G4int sample , G4double energy);

    void clear();
    
    std::vector<G4double> samples(){return m_samples;};

  private:
    G4double m_eta;
    G4double m_phi;
    G4double m_deta;
    G4double m_dphi;
    G4double m_rmin;
    G4double m_rmax;

    std::vector<G4double> m_samples;
    
};

#endif
