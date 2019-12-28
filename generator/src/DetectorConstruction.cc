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

#include "DetectorConstruction.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4AutoDelete.hh"
#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"


G4ThreadLocal
G4GlobalMagFieldMessenger* DetectorConstruction::m_magFieldMessenger = 0;


DetectorConstruction::DetectorConstruction()
 : G4VUserDetectorConstruction(),
   m_absorberPV(0),
   m_gapPV(0),
   m_checkOverlaps(true)
{;}


DetectorConstruction::~DetectorConstruction()
{;}


G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Define materials
  DefineMaterials();
  // Define volumes
  return DefineVolumes();
}


void DetectorConstruction::DefineMaterials()
{
  // Lead material defined using NIST Manager
  G4NistManager* nistManager = G4NistManager::Instance();
  nistManager->FindOrBuildMaterial("G4_Pb");
  nistManager->FindOrBuildMaterial("G4_Fe");
  nistManager->FindOrBuildMaterial("G4_CESIUM_IODIDE");

  // Liquid argon material
  G4double a;  // mass of a mole;
  G4double z;  // z=mean number of protons;
  G4double density;
  // The argon by NIST Manager is a gas with a different density
  new G4Material("liquidArgon", z=18., a= 39.95*g/mole, density= 1.390*g/cm3);

  // Vacuum
  new G4Material("Galactic", z=1., a=1.01*g/mole,density= universe_mean_density,kStateGas, 2.73*kelvin, 3.e-18*pascal);

  // Print materials
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}


G4VPhysicalVolume* DetectorConstruction::DefineVolumes()
{
  // Geometry parameters
  G4int nofLayers        = 80;
  G4double absoThickness = 2.*mm;
  G4double gapThickness  = 4.*mm;
  G4double calorSizeXY   = 100.*cm;

  G4double layerThickness = absoThickness + gapThickness;
  G4double calorThickness = nofLayers * layerThickness; //48 cm
  G4double worldSizeXY    = 10 * calorSizeXY;
  G4double worldSizeZ     = 10 * calorThickness;

  // Get materials
  G4Material* defaultMaterial  = G4Material::GetMaterial("Galactic");
  G4Material* absorberMaterial = G4Material::GetMaterial("G4_Pb");
  G4Material* gapMaterial      = G4Material::GetMaterial("liquidArgon");
  G4Material* caesium_material = G4Material::GetMaterial("G4_CESIUM_IODIDE");
  G4Material* iron_material    = G4Material::GetMaterial("G4_Fe");



  if ( ! defaultMaterial || ! absorberMaterial || ! gapMaterial ) {
    G4ExceptionDescription msg;
    msg << "Cannot retrieve materials already defined.";
    G4Exception("DetectorConstruction::DefineVolumes()",
      "MyCode0001", FatalException, msg);
  }

  //
  // World
  //
  G4VSolid* worldS = new G4Box("World",           // its name
                               worldSizeXY/2, worldSizeXY/2, worldSizeZ/2); // its size

  G4LogicalVolume* worldLV = new G4LogicalVolume(
                                 worldS,           // its solid
                                 defaultMaterial,  // its material
                                 "World");         // its name

  G4VPhysicalVolume* worldPV = new G4PVPlacement(
                 0,                // no rotation
                 G4ThreeVector(),  // at (0,0,0)
                 worldLV,          // its logical volume
                 "World",          // its name
                 0,                // its mother  volume
                 false,            // no boolean operation
                 0,                // copy number
                 m_checkOverlaps);  // checking overlaps

  //
  // Calorimeter
  //
  G4VSolid* calorimeterS =  new G4Box("Calorimeter",     // its name
                                      calorSizeXY/2, calorSizeXY/2, calorThickness/2); // its size

  G4LogicalVolume* calorLV = new G4LogicalVolume( calorimeterS,     // its solid
                                                  defaultMaterial,  // its material
                                                  "Calorimeter");   // its name



  new G4PVPlacement(
                 0,                // no rotation
                 G4ThreeVector(),  // at (0,0,0)
                 calorLV,          // its logical volume
                 "Calorimeter",    // its name
                 worldLV,          // its mother  volume
                 false,            // no boolean operation
                 0,                // copy number
                 m_checkOverlaps);  // checking overlaps

  //
  // Layer
  //
  G4VSolid* layerS = new G4Box("Layer",           // its name
                               calorSizeXY/2, calorSizeXY/2, layerThickness/2); // its size

  G4LogicalVolume* layerLV = new G4LogicalVolume(
                                                  layerS,           // its solid
                                                  defaultMaterial,  // its material
                                                  "Layer");         // its name

  new G4PVReplica(
                 "Layer",          // its name
                 layerLV,          // its logical volume
                 calorLV,          // its mother
                 kZAxis,           // axis of replication
                 nofLayers,        // number of replica
                 layerThickness);  // witdth of replica

  //
  // Absorber
  //
  G4VSolid* absorberS
    = new G4Box("Abso",            // its name
                 calorSizeXY/2, calorSizeXY/2, absoThickness/2); // its size

  G4LogicalVolume* absorberLV = new G4LogicalVolume(
                 absorberS,        // its solid
                 absorberMaterial, // its material
                 "Abso");          // its name

  m_absorberPV = new G4PVPlacement(
                 0,                // no rotation
                 G4ThreeVector(0., 0., -gapThickness/2), // its position
                 absorberLV,       // its logical volume
                 "Abso",           // its name
                 layerLV,          // its mother  volume
                 false,            // no boolean operation
                 0,                // copy number
                 m_checkOverlaps);  // checking overlaps

  //
  // Gap
  //
  G4VSolid* gapS
    = new G4Box("Gap",             // its name
                 calorSizeXY/2, calorSizeXY/2, gapThickness/2); // its size

  G4LogicalVolume* gapLV
    = new G4LogicalVolume(
                 gapS,             // its solid
                 gapMaterial,      // its material
                 "Gap");           // its name

  m_gapPV = new G4PVPlacement(
                 0,                // no rotation
                 G4ThreeVector(0., 0., absoThickness/2), // its position
                 gapLV,            // its logical volume
                 "Gap",            // its name
                 layerLV,          // its mother  volume
                 false,            // no boolean operation
                 0,                // copy number
                 m_checkOverlaps);  // checking overlaps

  //
  // print parameters
  //
  G4cout
    << G4endl
    << "------------------------------------------------------------" << G4endl
    << "---> The calorimeter is " << nofLayers << " layers of: [ "
    << absoThickness/mm << "mm of " << absorberMaterial->GetName()
    << " + "
    << gapThickness/mm << "mm of " << gapMaterial->GetName() << " ] " << G4endl
    << "------------------------------------------------------------" << G4endl;


  //
  // TileCal
  //
  G4double tile_calorThickness = 100.*cm;

  G4VSolid* tile_calorimeterS = new G4Box("TileCal",     // its name
                 calorSizeXY/2, calorSizeXY/2, tile_calorThickness/2); // its size

  G4LogicalVolume* tile_calorLV = new G4LogicalVolume(
                 tile_calorimeterS,     // its solid
                 defaultMaterial,  // its material
                 "TileCal");   // its name


  new G4PVPlacement(
                 0,                // no rotation
                 G4ThreeVector(0,0, calorThickness/2. + tile_calorThickness/2.),  // at (0,0, 74cm)
                 tile_calorLV,          // its logical volume
                 "TileCal",    // its name
                 worldLV,          // its mother  volume
                 false,            // no boolean operation
                 0,                // copy number
                 m_checkOverlaps);  // checking overlaps


  //
  // TileCal Layer //96 cm
  //
  G4int tile_nofLayers = 5;
  G4double tile_absoThickness  = tile_calorThickness/(2*tile_nofLayers); // 10 cm
  G4double tile_gapThickness   =  tile_calorThickness/(2*tile_nofLayers); // 10 cm
  G4double tile_layerThickness = tile_absoThickness + tile_gapThickness; // 20 cm
  
  
  G4VSolid* tile_layerS = new G4Box("TileCalLayer",           // its name
                                    calorSizeXY/2, calorSizeXY/2, tile_layerThickness/2); // its size

  G4LogicalVolume* tile_layerLV = new G4LogicalVolume(
                                      tile_layerS,           // its solid
                                      defaultMaterial,  // its material
                                      "TileCalLayer");         // its name


  new G4PVReplica(
                 "TileCalLayer",          // its name
                 tile_layerLV,          // its logical volume
                 tile_calorLV,          // its mother
                 kZAxis,           // axis of replication
                 tile_nofLayers,        // number of replica
                 tile_layerThickness);  // witdth of replica

  //
  // TileCal Absorber
  //
  G4VSolid* tile_absorberS
    = new G4Box("TileCalAbso",            // its name
                 calorSizeXY/2, calorSizeXY/2, tile_absoThickness/2); // its size

  G4LogicalVolume* tile_absorberLV
    = new G4LogicalVolume(
                 tile_absorberS,        // its solid
                 iron_material, // its material
                 "TileCalAbso");          // its name

  new G4PVPlacement(
                 0,                // no rotation
                 G4ThreeVector(0., 0., -tile_gapThickness/2), // its position
                 tile_absorberLV,       // its logical volume
                 "TileCalAbso",           // its name
                 tile_layerLV,          // its mother  volume
                 false,            // no boolean operation
                 0,                // copy number
                 m_checkOverlaps);  // checking overlaps


  //
  // TileCal Gap
  //
  G4VSolid* tile_gapS
   = new G4Box("TileCalGap",             // its name
                calorSizeXY/2, calorSizeXY/2, tile_gapThickness/2); // its size

  G4LogicalVolume* tile_gapLV
   = new G4LogicalVolume(
                tile_gapS,             // its solid
                caesium_material,      // its material
                "TileCalGap");           // its name

  new G4PVPlacement(
                0,                // no rotation
                G4ThreeVector(0., 0., tile_absoThickness/2), // its position
                tile_gapLV,            // its logical volume
                "TileCalGap",            // its name
                tile_layerLV,          // its mother  volume
                false,            // no boolean operation
                0,                // copy number
                m_checkOverlaps);  // checking overlaps

  //
  // Visualization attributes
  //
  worldLV->SetVisAttributes (G4VisAttributes::Invisible);

  G4VisAttributes* simpleBoxVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  simpleBoxVisAtt->SetVisibility(true);
  calorLV->SetVisAttributes(simpleBoxVisAtt);

  //
  // Always return the physical World
  //
  return worldPV;
}


void DetectorConstruction::ConstructSDandField()
{
  // Create global magnetic field messenger.
  // Uniform magnetic field is then created automatically if
  // the field value is not zero.
  G4ThreeVector fieldValue = G4ThreeVector();
  m_magFieldMessenger = new G4GlobalMagFieldMessenger(fieldValue);
  m_magFieldMessenger->SetVerboseLevel(1);

  // Register the field messenger for deleting
  G4AutoDelete::Register(m_magFieldMessenger);
}

