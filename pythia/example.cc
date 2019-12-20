// main03.cc is a part of the PYTHIA event generator.
// Copyright (C) 2019 Torbjorn Sjostrand.
// PYTHIA is licenced under the GNU GPL v2 or later, see COPYING for details.
// Please respect the MCnet Guidelines, see GUIDELINES for details.

// This is a simple test program.
// It illustrates how different processes can be selected and studied.
// All input is specified in the main03.cmnd file.
// Also illustrated output to be plotted by Python/Matplotlib/pyplot.


#include "Pythia8/Pythia.h"
#include "Pythia8Plugins/HepMC2.h"
#include <unistd.h>


using namespace Pythia8;

//==========================================================================

int main() {



  // Declare sample cross section for output.
  double sigmaTemp  = 0.;
  // Cross section an error.
  double sigmaTotal  = 0.;


  double errorTotal  = 0.;


  // Generator.
  Pythia pythia;

  // Shorthand for the event record in pythia.
  //Event& event = pythia.event;

  // Read in commands from external file.
  pythia.readFile("my_main03.cmnd");

   
  // Interface for conversion from Pythia8::Event to HepMC one.
  HepMC::Pythia8ToHepMC ToHepMC;
  // Specify file where HepMC events will be stored.
  HepMC::IO_GenEvent ascii_io("my_LHE.out", std::ios::out);
  // Switch off warnings for parton-level events.
  ToHepMC.set_print_inconsistency(false);
  ToHepMC.set_free_parton_exception(false);
  // Do not store cross section information, as this will be done manually.
  ToHepMC.set_store_pdf(false);
  ToHepMC.set_store_proc(false);
  ToHepMC.set_store_xsec(false);


  // Extract settings to be used in the main program.
  int nEvent = pythia.mode("Main:numberOfEvents");
  int nAbort = pythia.mode("Main:timesAllowErrors");

  // Initialize.
  pythia.init();

  
  // Begin event loop.
  int iAbort = 0;
  for (int iEvent = 0; iEvent < nEvent; ++iEvent) {

    // Generate events. Quit if many failures.
    if (!pythia.next()) {
      if (++iAbort < nAbort) continue;
      cout << " Event generation aborted prematurely, owing to error!\n";
      break;
    }
    // Analyse event 

    // Dump the event in LHE file
    // Get event weight(s).
    double weight = pythia.info.mergingWeight();
    double evtweight = pythia.info.weight();
    weight *= evtweight;

    // Do not print zero-weight events.
    if ( weight == 0. ) continue;
    // Construct new empty HepMC event.
    HepMC::GenEvent* hepmcevt = new HepMC::GenEvent();


    // Set event weight
    hepmcevt->weights().push_back(weight);
    // Fill HepMC event
    ToHepMC.fill_next_event( pythia, hepmcevt );
    // Add the weight of the current event to the cross section.
    sigmaTotal += weight;
    sigmaTemp  += weight;
    errorTotal += pow2(weight);
    // Report cross section to hepmc
    HepMC::GenCrossSection xsec;
    xsec.set_cross_section( sigmaTotal*1e9, pythia.info.sigmaErr()*1e9 );
    hepmcevt->set_cross_section( xsec );
    // Write the HepMC event to file. Done with it.
    ascii_io << hepmcevt;
    delete hepmcevt;

    // End of event loop.
  }

  // Final statistics. Normalize and output histograms.
  pythia.stat();

  //double sigma = pythia.info.sigmaGen();

  // Done.
  return 0;
}
