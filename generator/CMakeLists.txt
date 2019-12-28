#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(generator)

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
include(${Geant4_USE_FILE})

#----------------------------------------------------------------------------
# Find HepMC (required package)
find_package(HepMC REQUIRED)



#----------------------------------------------------------------------------
# Find ROOT (required package)
find_package(ROOT COMPONENTS EG Eve Geom Gui GuiHtml GenVector Hist Physics Matrix Graf RIO Tree Gpad RGL MathCore)
include(${ROOT_USE_FILE})

#----------------------------------------------------------------------------
# Locate sources and headers for this project
message(${HEPMC_INCLUDE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/include 
                    ${Geant4_INCLUDE_DIR}
                    ${HEPMC_INCLUDE_DIR}
                    ${ROOT_INCLUDE_DIRS})
file(GLOB sources 
     ${PROJECT_SOURCE_DIR}/src/*.cc
     ${PROJECT_SOURCE_DIR}/src/hepmc/*.cc
     ${PROJECT_SOURCE_DIR}/src/jets/*.cc
  )

file(GLOB headers 
     ${PROJECT_SOURCE_DIR}/include/*.hh
     ${PROJECT_SOURCE_DIR}/include/hepmc/*.hh
     ${PROJECT_SOURCE_DIR}/include/jets/*.hh
     )

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
add_executable(generator generator.cc ${sources} ${headers})
target_link_libraries(generator ${Geant4_LIBRARIES} ${HEPMC_LIBRARIES} ${HEPMC_FIO_LIBRARIES} ${ROOT_LIBRARIES} )













