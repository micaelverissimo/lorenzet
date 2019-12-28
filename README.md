# Lorenzet detector

This is the first event generator for HEP using a generic calorimeter for machine
learning studies.



## Detector construction:

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


![2D shower in the calorimeter](/docs/lateral_view_jets.pdf)


## Requirements:

- Geant4 (opengl or qt4 is required for graphic interface, https://github.com/jodafons/geant4.git);
- ROOT (https://github.com/root-project/root.git);
- Pythia8 (for jets generator);
- HEPMC (for hep mc files, https://github.com/jodafons/hepmc.git);
- FastJet (https://github.com/jodafons/fastjet.git).


## Installation:

Use this example to build your hep installation in your local machine.
The path for each HEP package depends of your local installation


```bash
# ROOT
source ~/.bin/root/build_root/bin/thisroot.sh
# geant4
cd ~/.bin/geant4_10.5/build
source geant4.sh
cd ~
# hepmc
export HEPMC_INCLUDE=/Users/jodafons/.bin/HepMC/HepMC-2.06.09/HepMC
export HEPMC_LIBRARY=/Users/jodafons/.bin/HepMC/build/lib
export DYLD_LIBRARY_PATH=$DYLD_LIBRARY_PATH:$HEPMC_LIBRARY
# pythia
export PYTHIA8_INCLUDE=/Users/jodafons/.bin/pythia8244/include
export PYTHIA8_LIBRARY=/Users/jodafons/.bin/pythia8244/lib
#fastjet
export FASTJET_INCLUDE=/Users/jodafons/.bin/fastjet-3.3.3/include
export FASTJET_LIBRARY=/Users/jodafons/.bin/fastjet-3.3.3/lib
# ROOT extra envs
export ROOT_INCLUDE=/Users/jodafons/.bin/root/build_root/include
export ROOT_LIBRARY=/Users/jodafons/.bin/root/build_root/lib
```

### Compile the simulator package:

The simulator will use this envirements to locate all necessary infomation to build and
link all dependencies. After you install all dependencies and set all necessary envirementes 
just compile the simulator package using the follow commands.


```bash
mkdir build && cd build
cmake ..
make
```

### Compile the pythia jet maker:

This sub package is used to create jet patcicles using pythia and dump this information
inside a ttree. This root file will be used by the gnerator to read all jets and propagate
this particles throut out the calorimeter.

```bash
make jet_maker
```


## Usage:



