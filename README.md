# Lorenzet detector

This is the first event generator for HEP using a generic calorimeter for machine
learning studies.



## Detector construction

Detector ROI dimesions:

                (0,0,0)
                   |
       |--------- EM ----------|----------- HAD ----------|
         15cm     30cm     3cm    40cm       40cm    20cm
  +    +--------------------------------------------------+
  |    |      |           |    |         |         |      |   
 48cm  |      |           |    |         |         |      |
  |    |      |           |    |         |         |      |
  +    +--------------------------------------------------+


It's a composite calorimeter with eletromagnet and hadronic layers. 

- 100cm X 100cm with face (x,y);
- We considere a face of 48 cm as Region of interesti (x,y);
- We always shot the particle in the center of the (x,y) face;
- First EM layer with 15 cm depth (z) and 15 segments (layers) of LAR and Pb; (3 x 96 cells);
- Second EM layer with 30 cm depth (z) and 50 segments (layers) of LAR and Pb; (12 x 12 cells);
- Third EM layer with 3 cm depth (z) and 15 segments (layers) of LAR and Pb; (12 x c cells);
- First HAD layer with 40 cm depth (z) and 2 segments (layers) of 


## Requirements

- Geant4 (opengl or qt4 is required for graphic interface, https://github.com/jodafons/geant4.git);
- ROOT (https://github.com/root-project/root.git);
- Pythia8 (for jets generator);
- HEPMC (for hep mc files, https://github.com/jodafons/hepmc.git);
- FastJet (https://github.com/jodafons/fastjet.git).


## Installation



## Usage




