// main18.cc is a part of the PYTHIA event generator.
// Copyright (C) 2019 Torbjorn Sjostrand.
// PYTHIA is licenced under the GNU GPL v2 or later, see COPYING for details.
// Please respect the MCnet Guidelines, see GUIDELINES for details.

// This is a simple test program.
// It illustrates how to write an event filter.
// No new functionality is involved - all could be done in the main program
// - but the division of tasks may be more convenient for recurrent cuts.


// NOTE: Pythia always use GeV and mm (time also in mm)

#include <algorithm>
#include <string>
#include <random>

#include "Pythia8/Pythia.h"
//#include "Pythia8/HeavyIons.h"


// ROOT includes
#include "TTree.h"
#include "TFile.h"
#include "TVector3.h"

#include "fastjet/PseudoJet.hh"
#include "fastjet/JetDefinition.hh"
#include "fastjet/ClusterSequence.hh"

using namespace Pythia8;

// Initialize random generator
namespace{
std::random_device rd;
std::mt19937 eng(rd());

std::vector<float>* nvf(){
  return new std::vector<float>();
}
std::vector<int>* nvi(){
  return new std::vector<int>();
}
//==========================================================================
// Printer for particle
std::ostream& operator<< ( std::ostream& out, const Particle& pt ){
  int prec = 3;
  bool useFixed = false;
	out << "    "
       << "     id  name            status     mothers   daughters     colou"
       << "rs      p_x        p_y        p_z        c_x        c_y        c_z         e          m \n";
  out << setw(11) << pt.id() << "  " << left
       << setw(18) << pt.nameWithStatus(18) << right << setw(4)
       << pt.status() << setw(6) << pt.mother1() << setw(6)
       << pt.mother2() << setw(6) << pt.daughter1() << setw(6)
       << pt.daughter2() << setw(6) << pt.col() << setw(6) << pt.acol()
       << setw(8+prec) << pt.px() << setw(8+prec) << pt.py()
       << setw(8+prec) << pt.pz()
       << setw(8+prec) << pt.xProd() << setw(8+prec) << pt.yProd()
       << setw(8+prec) << pt.zProd()
       << setw(8+prec) << pt.e()
       << setw(8+prec) << pt.m();
  return out;
}

static std::size_t level(1);

std::ostream& operator<< ( std::ostream& out, const std::vector<fastjet::PseudoJet>& pt );
std::ostream& operator<< ( std::ostream& out, const fastjet::PseudoJet& pt );

std::ostream& operator<< ( std::ostream& out, const std::vector<fastjet::PseudoJet>& pt ){
  for( auto s : pt ){
    out << s;
  }
  return out;
}

//==========================================================================
// Printer for PseudoJet
std::ostream& operator<< ( std::ostream& out, const fastjet::PseudoJet& s ){
  for( std::size_t i = 0; i < level; ++i ){
    out << "-";
  }
  out << "|x: " << s.px() << " |y: " << s.py() << " |z: " << s.pz() << " |eta: " << s.eta() << " |phi: " << s.phi() << " |E: " << s.E() << " |pt: " << s.pt() << " |Et: " << s.Et() << " |abs(eta): " << std::abs(s.eta()) << endl;
  if ( s.has_constituents()){
    ++::level;
    const auto& c = s.constituents();
    if ( c.size() > 1 ){
      out  << c;
    }
    --::level;
  }
  return out;
}


//==========================================================================
// Printer for particle
inline
std::ostream& operator<< ( std::ostream& out, const Particle* pt ){
  return operator<<( out, *pt );
}

//==========================================================================
// Printer for particle vector
std::ostream& operator<< ( std::ostream& out, const std::vector< Particle* >& vec )
{
  int prec = 3;
  bool useFixed = false;
	out << "    no    "
      << "     id  name            status     mothers   daughters     colou"
      << "rs      p_x        p_y        p_z         e          m \n";
  // Basic line for a particle, always printed.
  for ( std::size_t i = 0; i < vec.size(); ++i ){
    const auto& pt = *vec[i];
    out << setw(6) << i << setw(11) << pt.id() << "  " << left
        << setw(18) << pt.nameWithStatus(18) << right << setw(4)
        << pt.status() << setw(6) << pt.mother1() << setw(6)
        << pt.mother2() << setw(6) << pt.daughter1() << setw(6)
        << pt.daughter2() << setw(6) << pt.col() << setw(6) << pt.acol()
        << setw(8+prec) << pt.px() << setw(8+prec) << pt.py()
        << setw(8+prec) << pt.pz() << setw(8+prec) << pt.e()
        << setw(8+prec) << pt.m() << "\n";
  }
  return out;
}
}

//==========================================================================

// The ParticleFilter class.

// The constructor takes the following arguments
// select = 1 : keep only final particles.
//        = 2 : keep only final visible particles (i.e. not neutrinos).
//        = 3 : keep only final charged particles.
// etaMax (default = 50) : keep only particles with pseudorapidity
//        |eta| < etaMax.
// pTminCharged (default = 0) : keep a charged particle only if
//        its transverse momentum pT < pTminCharged.
// pTminNeutral (default = 0) : keep a neutral particle only if
//        its transverse momentum pT < pTminNeutral.

// Main methods:
// filter( event) takes an event record as input and analyzes it.
// size() returns the number of particles kept.
// index(i) returns the index in the full event of the i'th kept particle.
// particlePtr(i) returns a pointer to the i'th kept particle.
// particleRef(i) returns a reference to the i'th kept particle.
// list() gives a listing of the kept particles only.

class ParticleFilter {

public:

  // Constructor sets properties of filter.
  ParticleFilter( int selectIn, double etaMaxIn = 50.,
    double pTminChargedIn = 0., double pTminNeutralIn = 0.)
    : select(selectIn), etaMax(etaMaxIn), pTminCharged(pTminChargedIn),
    pTminNeutral(pTminNeutralIn) {}

  // Analysis of each new event to find acceptable particles.
  void filter(Event& event);

  // Return size of array, and index of a particle.
  int size()       const {return keptPtrs.size();}
  int index(int i) const {return keptIndx[i];}

  // Return pointer or reference to a particle.
  Particle* particlePtr(int i) {return  keptPtrs[i];}
  Particle& particleRef(int i) {return *keptPtrs[i];}

  // List kept particles only.
  void list(ostream& os = cout);

  std::vector<Particle*>& getParticlesRef() { return keptPtrs; }

private:

  // Filter properties, set by constructor.
  int    select;
  double etaMax, pTminCharged, pTminNeutral;

  // Kept particle indices and pointers, referring to original event.
  vector<int>       keptIndx;
  vector<Particle*> keptPtrs;

};

//--------------------------------------------------------------------------

// The filter method.

void ParticleFilter::filter(Event& event) {

  // Reset arrays in preparation for new event.
  keptIndx.resize(0);
  keptPtrs.resize(0);

  // Loop over all particles in the event record.
  for (int i = 0; i < event.size(); ++i) {

    // Skip if particle kind selection criteria not fulfilled.
    if (!event[i].isFinal()) continue;
    if (select == 2 && !event[i].isVisible()) continue;
    bool isCharged = event[i].isCharged();
    if (select == 3 && !isCharged) continue;

    // Skip if too large pseudorapidity.
    if (abs(event[i].eta()) > etaMax) continue;

    // Skip if too small pT.
    if       (isCharged && event[i].pT() < pTminCharged) continue;
    else if (!isCharged && event[i].pT() < pTminNeutral) continue;

    // Add particle to vectors of indices and pointers.
    keptIndx.push_back( i );
    keptPtrs.push_back( &event[i] );

  // End of particle loop. Done.
  }

}

//--------------------------------------------------------------------------

// The list method: downscaled version of Event::list.

void ParticleFilter::list(ostream& os) {

  // Header.
  os << "\n --------  PYTHIA Event Listing  (filtered)  ------------------"
     << "-----------------------------------------------------------------"
     << "----\n \n    no        id   name            status     mothers  "
     << " daughters     colours      p_x        p_y        p_z         e  "
     << "        m \n";

  // At high energy switch to scientific format for momenta.
  double eSum = 0.;
  for (int iKept = 0; iKept < size(); ++iKept) eSum += keptPtrs[iKept]->e();
  bool useFixed = (eSum < 1e5);

  // Listing of kept particles in event.
  for (int iKept = 0; iKept < size(); ++iKept) {
    int i = keptIndx[iKept];
    Particle& pt = *keptPtrs[iKept];

    // Basic line for a particle, always printed.
    os << setw(6) << i << setw(10) << pt.id() << "   " << left
       << setw(18) << pt.nameWithStatus(18) << right << setw(4)
       << pt.status() << setw(6) << pt.mother1() << setw(6)
       << pt.mother2() << setw(6) << pt.daughter1() << setw(6)
       << pt.daughter2() << setw(6) << pt.col() << setw(6) << pt.acol()
       << ( (useFixed) ? fixed : scientific ) << setprecision(3)
       << setw(11) << pt.px() << setw(11) << pt.py() << setw(11)
       << pt.pz() << setw(11) << pt.e() << setw(11) << pt.m() << "\n";
  }

  // Listing finished.
  os << "\n --------  End PYTHIA Event Listing  ----------------------------"
     << "-------------------------------------------------------------------"
     << endl;
}

struct JetData {
  fastjet::PseudoJet fastjet_pseudojet;
  std::vector<Particle*> pythia_particles;
  fastjet::PseudoJet fastjet_rotated_pseudojet;
  std::vector<Particle*> pythia_rotated_particles;
};

class NoJetError {};

//==========================================================================
// Create jets
JetData getJet(std::vector<Particle*>& particles, const double pt_filter, const double eta_filter){

  // Make one-to-one correspondence between fastjet PseudoJet and Particle
  std::vector<fastjet::PseudoJet> input_particles;
  std::map< std::size_t, Particle* > local_barcode_map;
  std::size_t barcode(0);
  for ( auto part : particles ) {
    fastjet::PseudoJet substruct_cand(part->px(),part->py(),part->pz(),part->e());
    substruct_cand.set_user_index( barcode );
    input_particles.push_back( substruct_cand );
    local_barcode_map.insert( std::pair< std::size_t, Particle* >( barcode++, part ) );
  }

  // Clusterize hadrons
  fastjet::JetDefinition jet_def(fastjet::antikt_algorithm,0.4);
  fastjet::ClusterSequence clust_seq(input_particles,jet_def);
  // Apply filters
  std::vector<fastjet::PseudoJet> inclusive_jets = sorted_by_pt( clust_seq.inclusive_jets() );
  inclusive_jets.erase(std::remove_if(inclusive_jets.begin(),
                            inclusive_jets.end(),
                            [pt_filter,eta_filter](fastjet::PseudoJet& j){return (std::abs(j.eta()) > eta_filter) || (j.pt() < pt_filter);}
                                     ),
                       inclusive_jets.end());

  if ( inclusive_jets.empty() ){
    throw NoJetError();
  }

  //cout << "All jets!" << endl;
  //std::size_t i(0);
  //for( const auto& jet : inclusive_jets ){
  //  cout << "---------------------------- JET(" << i++ << ") --------------------------" << endl;
  //  cout << jet << endl;
  //}


  // Pick a jet
  std::uniform_int_distribution<> distr(0,inclusive_jets.size()-1);
  const auto idx = distr(eng);
  //std::cout << "Random index is: " << idx << endl;
  auto& chosen_jet = inclusive_jets[ idx ];

  //std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ CHOSEN JET ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ is: \n" << chosen_jet;

  // Map back from the PseudoJet to particles
  const auto& substructs = chosen_jet.constituents();
  std::vector<Particle*> jet_pythia_particles;
  jet_pythia_particles.reserve( substructs.size() );
  for ( auto& substruct : substructs ){
    jet_pythia_particles.push_back( local_barcode_map[ substruct.user_index() ] );
  }

  return JetData{chosen_jet, jet_pythia_particles};
}

void getParticlesToTarget( JetData &jet_data,
    const Vec4 &center,
    const Vec4 &pd )
{
  // Get jet momentum
  const auto& jet = jet_data.fastjet_pseudojet;
  Vec4 pj(jet.px(), jet.py(), jet.pz());
  // Obtain angle between jet and target direction
  const double theta = Pythia8::theta(pd, pj);
  // Perpendicular plane
  const auto u = cross3(pj, pd);

  //cout << "===========================================================================================================================================" << endl;
  //cout << "Original jet:" << pj;
  pj.rotaxis( theta, u );
  //cout << "PROOF: " << endl;
  //cout << "Rotated jet:" << pj;
  jet_data.fastjet_rotated_pseudojet.reset_momentum( pj.px(), pj.py(), pj.pz(), jet.e() );

  for ( const auto orig_part : jet_data.pythia_particles ){
    auto* part = new Particle( *orig_part );
    //cout << "===========================================================================================================================================" << endl;
    //cout << "Before rotation: " << endl;
    //cout << part << endl;
    // Rotate particle flight direction
    auto p = part->p();
    p.rotaxis( theta, u );
    part->p( p );
    // Rotate production vertice and recenter particle
    if ( part->hasVertex() ) {
      auto vProd = part->vProd();
      vProd.rotaxis( theta, u );
      part->vProd( vProd + center);
    } else {
      part->vProd( center );
    }
    jet_data.pythia_rotated_particles.push_back( part );
    //cout << "After rotation (" << theta << "):" << endl;
    //cout << part << endl;
  }
}

//==========================================================================
// Use the ParticleFilter method to plot some event properties.
int main() {

  // Generator.
  Pythia pythia;

  // Some information concerning where is our target
  Vec4 targetDirection(0.,0.,1.);
  Vec4 newCenter(0.,0.,-1500); // Position in mm

  // Read in commands from external file.
  pythia.readFile("jet_maker.cmnd");


  // Number of events to generate, to list, to allow aborts.
  int nEvent      = pythia.mode("Main:numberOfEvents");
  int nAbort      = pythia.mode("Main:timesAllowErrors");
  int nList       = 1;

  // Filter values
  int    select   = 2; // Visible particles (no neutrinos)
  double etaMax = 2.5;
  double ptfilter = 17.; // GeV

  // Initialization for LHC.
  pythia.init();

  // Declare Event Filter according to specification.
  ParticleFilter filter( select, etaMax + .6 );

  // Initialize output file
  TFile file("pythia.root","recreate" );
  Event *evtToWrite(nullptr);
  TTree tree("t","Pythia8 event tree");
  // Jet variables/branches
  double jet_orig_eta, jet_orig_phi, jet_orig_e, jet_orig_et, jet_electron_photon_e_frac, jet_px, jet_py, jet_pz;
  unsigned int jet_n_part;
  tree.Branch("jet_orig_eta", &jet_orig_eta, "jet_orig_eta/D"); tree.Branch("jet_orig_phi", &jet_orig_phi, "jet_orig_phi/D");
  tree.Branch("jet_px", &jet_px, "jet_px/D"); tree.Branch("jet_py", &jet_py, "jet_py/D"); tree.Branch("jet_pz", &jet_pz, "jet_pz/D");
  tree.Branch("jet_orig_e", &jet_orig_e, "jet_orig_e/D"); tree.Branch("jet_orig_et", &jet_orig_et, "jet_orig_et/D"); tree.Branch("jet_n_part", &jet_n_part, "jet_n_part/i");
  tree.Branch("jet_electron_photon_e_frac", &jet_electron_photon_e_frac, "jet_electron_photon_e_frac/D");
  // Substructure variables/branches
  std::vector<int> *s_pdg_id(nvi());
  std::vector<float> *s_px(nvf()), *s_py(nvf()), *s_pz(nvf()),
                     *s_orig_eta(nvf()), *s_orig_phi(nvf()),
                     *s_prod_x(nvf()), *s_prod_y(nvf()), *s_prod_z(nvf()), *s_prod_t(nvf()),
                     *s_orig_e(nvf()), *s_orig_et(nvf());
  tree.Branch("s_pdg_id", &s_pdg_id);
  tree.Branch("s_px", &s_px); tree.Branch("s_py", &s_py); tree.Branch("s_pz", &s_pz);
  tree.Branch("s_prod_x", &s_prod_x); tree.Branch("s_prod_y", &s_prod_y); tree.Branch("s_prod_z", &s_prod_z);  tree.Branch("s_prod_t", &s_prod_t);
  tree.Branch("s_orig_eta", &s_orig_eta); tree.Branch("s_orig_phi", &s_orig_phi);
  tree.Branch("s_orig_e", &s_orig_e); tree.Branch("s_orig_et", &s_orig_et);


  // Begin event loop.
  int iAbort = 0;
  for (int iEvent = 0; iEvent < nEvent; ++iEvent) {

    // Generate events. Quit if too many failures.
    if (!pythia.next()) {
      if (++iAbort < nAbort) continue;
      cout << " Event generation aborted prematurely, owing to error!\n";
      break;
    }

    double weight = pythia.info.mergingWeight();
    double evtweight = pythia.info.weight();
    weight *= evtweight;

    // Do not print zero-weight events.
    if ( weight == 0. ) continue;

    // Find final charged particles with |eta| < 3 and pT > 1 GeV.
    filter.filter( pythia.event );

    // List first few events, both complete and after filtering.
    //if (iEvent < nList) {
    //  pythia.info.list();
    //  pythia.process.list();
    //  pythia.event.list();
    //  filter.list();
    //}

    // Retrieve jet
    try {
      auto jet_data = getJet( filter.getParticlesRef(), ptfilter, etaMax );
      // Rotate it
      getParticlesToTarget( jet_data, newCenter, targetDirection );
      // Fill jet data
      const auto& jet = jet_data.fastjet_pseudojet;
      //const auto& jet_rotated = jet_data.fastjet_rotated_pseudojet;
      const auto& jet_rotated = jet_data.fastjet_pseudojet;
      jet_orig_eta = jet.eta(); jet_orig_phi = jet.phi();
      jet_px = jet_rotated.px(); jet_py = jet_rotated.py(); jet_pz = jet_rotated.pz();
      jet_orig_e = jet.e(); jet_orig_et = jet.Et();
      jet_electron_photon_e_frac = 0.;
      jet_n_part = jet_data.pythia_particles.size();
      // Fill substruct
      for ( std::size_t idx = 0; idx < jet_data.pythia_particles.size(); ++idx ){
        // Shorthands
        const auto& orig_part = jet_data.pythia_particles[idx];
        const auto& rotated_part = jet_data.pythia_rotated_particles[idx];
        const auto pdg_id = rotated_part->id();
        // Obtain substruct data
        s_pdg_id->push_back( pdg_id );
        s_px->push_back( rotated_part->px() ); s_py->push_back( rotated_part->py() ); s_pz->push_back( rotated_part->pz() );
        s_prod_x->push_back( rotated_part->xProd() ); s_prod_y->push_back( rotated_part->yProd() ); 
        s_prod_z->push_back( rotated_part->zProd() ); s_prod_t->push_back( rotated_part->tProd() );
        s_orig_eta->push_back( orig_part->eta()); s_orig_phi->push_back( orig_part->phi() );
        s_orig_e->push_back( orig_part->e() ); s_orig_et->push_back( orig_part->eT() );
        if ( pdg_id == 22 || abs(pdg_id) == 11 ){
          jet_electron_photon_e_frac += orig_part->e();
        }
      }
      jet_electron_photon_e_frac /= jet_orig_e;

      // Write root file
      tree.Fill();

      // Clear vectors
      s_pdg_id->clear();
      s_px->clear(); s_py->clear(); s_pz->clear();
      s_prod_x->clear(); s_prod_y->clear(); s_prod_z->clear();  s_prod_t->clear();
      s_orig_eta->clear(); s_orig_phi->clear();
      s_orig_e->clear(); s_orig_et->clear();
      for( auto p : jet_data.pythia_rotated_particles ){
        delete p;
      }
    } catch ( NoJetError ){
      //cout << "No inclusive jet in this event, regenerating..." << endl;
      --iEvent;
    }

    // End of event loop.
  }

  // Final statistics.
  pythia.stat();

  tree.Print();
  tree.Write();

  file.Write();

  // Done.
  return 0;
}
