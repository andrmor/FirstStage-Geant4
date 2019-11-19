#include "DetectorConstruction.hh"
#include "SessionManager.hh"
#include "G4SystemOfUnits.hh"

#include "G4Element.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "G4Transform3D.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  G4NistManager * man = G4NistManager::Instance();
  SessionManager & SM = SessionManager::getInstance();

  // Materials
  G4Material * matVacuum = man->FindOrBuildMaterial("G4_Galactic");

  std::vector<G4int> natoms;
  std::vector<G4String> elements;
  elements.push_back("C");     natoms.push_back(5);
  elements.push_back("H");     natoms.push_back(8);
  elements.push_back("O");     natoms.push_back(2);
  G4double density = 1.18*g/cm3;
  G4Material * matPMMA = man->ConstructNewMaterial("PMMA", elements, natoms, density);

  SM.dummyMat = new G4Material("Dummy", 1.0, 1.008*g/mole, 1.0e-25*g/cm3);

  // Geometry
  G4double cylRadius = 75.0*mm;
  G4double cylLength = SM.PMMAlength*mm;

  G4Box             * solidWorld = new G4Box("World", 450.0*mm, 300.0*mm, 2.0*cylRadius);
  G4LogicalVolume   * logicWorld = new G4LogicalVolume(solidWorld, matVacuum, "World");
  G4VPhysicalVolume * physWorld = new G4PVPlacement(0, G4ThreeVector(0.0, 0.0, 0.0), logicWorld, "World", 0, false, 0);
  logicWorld->SetVisAttributes(G4VisAttributes(G4Colour(0.0, 1.0, 0.0)));
  //logicWorld->SetVisAttributes(G4VisAttributes::Invisible);

  G4VSolid          * solidPmma = new G4Tubs("Cyl", 0, cylRadius, 0.5*cylLength, 0, 360.0*deg);
  G4LogicalVolume   * logicPmma = new G4LogicalVolume(solidPmma, matPMMA, "Cyl");
  G4RotationMatrix * rot = new CLHEP::HepRotation(90*deg, 90*deg, 0);
  new G4PVPlacement(rot, G4ThreeVector(SM.CylShift*mm + 0.5*cylLength, 0, 0), logicPmma, "Target", logicWorld, false, 0);  // Shifted!!!
  logicPmma->SetVisAttributes(G4VisAttributes(G4Colour(0.0, 1.0, 1.0)));

  G4VSolid          * solidDummy = new G4Box("Dummy", 225*mm, 275.0*mm, 0.5*mm);
  G4LogicalVolume   * logicDummy = new G4LogicalVolume(solidDummy, SM.dummyMat, "Dummy");
  new G4PVPlacement(0, G4ThreeVector(200.0*mm, 0, -100.5*mm), logicDummy, "Dummy", logicWorld, false, 0);
  logicPmma->SetVisAttributes(G4VisAttributes(G4Colour(0.0, 0.0, 1.0)));

  return physWorld;
}

