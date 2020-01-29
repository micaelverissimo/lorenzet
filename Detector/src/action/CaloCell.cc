
#include "CaloCell.h"


CaloCell::CaloCell( float samples, float eta, float phi, float deta, float dphi, float rmin, float rmax ):
  m_samples( samples, 0.0 ),
  m_eta(eta),
  m_phi(phi),
  m_deta(deta),
  m_dphi(dphi),
  m_rmin(rmin),
  m_rmax(rmax)
{;}


CaloCell::~CaloCell()
{;}


float CaloCell::eta()
{
  return m_eta;
}

float CaloCell::phi()
{
  return m_phi;
}

float CaloCell::deltpaEta()
{
  return m_deta;
}

float CaloCell::deltaPhi()
{
  return m_dphi;
}

float CaloCell::rmin()
{
  return m_rmin;
}

float CaloCell::rmax()
{
  return m_rmax;
}

void clear()
{
  // Zeroize all samples inside of the pulse energy
  for(unsigned s=0; s<m_samples.size();++s)
    m_samples[s]=0.0;
}


bool Add( G4int sample, G4double energy )
{
  if(sample<m_samples.size())
    m_samples[sample]+=energy;
  else
    return false;
  // return true if the sample it is inside of the
  // smaple size.
  return true;
}




