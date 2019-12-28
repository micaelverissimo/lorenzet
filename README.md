# Lorenzet detector

This is the first event generator for HEP using a generic calorimeter for machine
learning studies.



## Detector construction

It's a composite calorimeter with eletromagnet and hadronic layers. 

- 100cm X 100cm with face (x,y);
- We considere a face of 48 cm as Region of interesti (x,y);
- We always shot the particle in the center of the (x,y) face;
- First EM layer with 15 cm depth (z) and 15 segments (layers) of LAR and Pb; (3 x 96 cells);
- Second EM layer with 30 cm depth (z) and 50 segments (layers) of LAR and Pb; (12 x 12 cells);
- Third EM layer with 3 cm depth (z) and 15 segments (layers) of LAR and Pb; (12 x c cells);
- First HAD layer with 40 cm depth (z) and 2 segments (layers) of Cesium iodide and iron ( 8 x 8 cells); 
- Second HAD layer with 40 cm depth (z) and 2 segments (layers) of Cesium iodide and iron ( 8 x 8 cells); 
- Third HAD layer with 20 cm depth (z) and 1 segments (layers) of Cesium iodide and iron ( 4 x 4 cells); 

For mode information about the detector granularity: https://github.com/jodafons/lorenzet/blob/master/src/SteppingAction.cc


## Requirements

- Geant4 (opengl or qt4 is required for graphic interface, https://github.com/jodafons/geant4.git);
- ROOT (https://github.com/root-project/root.git);
- Pythia8 (for jets generator);
- HEPMC (for hep mc files, https://github.com/jodafons/hepmc.git);
- FastJet (https://github.com/jodafons/fastjet.git).


## Installation



## Usage




