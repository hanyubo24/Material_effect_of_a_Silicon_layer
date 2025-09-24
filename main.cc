#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"

#include <G4RunManagerFactory.hh>
#include <G4UImanager.hh>
#include <G4UIterminal.hh>
#include <G4UIExecutive.hh>
#include <G4VisExecutive.hh>
#include <G4PhysListFactory.hh>


void PrintUsage()
{
  G4cerr << " Usage: " << G4endl;
  G4cerr << " si_cyl -n 1000 -p e- -pmin 80 -pmax 80 -o myOutput.csv -z -14.0 -ui" << G4endl;
  G4cerr << "        -r 240.0 [mm], ranging from 240 mm to 348 mm " << G4endl;
  G4cerr << "        -th 300.0 [um], thickness of the silicon" << G4endl;
 
 }


int main(int argc, char** argv) 
{

  if (argc > 1 && G4String(argv[1]) == "--h") {
    PrintUsage();
    return 1;
  }

  G4String macro;
  G4String session;
  G4bool verboseBestUnits = true;

  G4double pMin = 10 *MeV;
  G4double pMax = 1000 *MeV;
  G4String particleName = "pi-";

  G4String outFileName = "Silicon_material.csv"; 
  G4int nEvent = 10; 
  G4double gunZPos = 0.0 * cm; 
  std::vector<G4double> radii ={240.0};
  G4double thickness = 300.0 ; 
  G4bool show_gui=false;
#ifdef G4MULTITHREADED
  G4int nThreads = 0;
#endif
  for (G4int i = 1; i < argc; i = i + 2) {
    if (G4String(argv[i]) == "-m")
      macro = argv[i + 1];
    else if (G4String(argv[i]) == "-u")
      session = argv[i + 1];
    else if (G4String(argv[i]) == "-n")
      nEvent = G4UIcommand::ConvertToInt(argv[i + 1]);
    else if (G4String(argv[i]) == "-z")
      gunZPos = std::stod(argv[i + 1]) * CLHEP::cm;
    else if (G4String(argv[i]) == "-p")
        particleName = argv[i + 1];
    else if (G4String(argv[i]) == "-r")
        radii.push_back(std::stod(argv[i+1]));
    else if (G4String(argv[i]) == "-pmin")
        pMin = std::stod(argv[i + 1]) * MeV;
    else if (G4String(argv[i]) == "-pmax")
        pMax = std::stod(argv[i + 1]) * MeV;
    else if (G4String(argv[i]) == "-th")
        thickness = std::stod(argv[i + 1]) ;
    else if (G4String(argv[i]) == "-o")  
        outFileName = argv[i + 1];
    else if (G4String(argv[i]) == "-ui")  
        show_gui=true;
#ifdef G4MULTITHREADED
    else if (G4String(argv[i]) == "-t") {
      nThreads = G4UIcommand::ConvertToInt(argv[i + 1]);
    }
#endif
    else {
      PrintUsage();
      return 1;
    }
  }

  // Detect interactive mode (if no macro provided) and define UI session
  //
  G4UIExecutive* ui = nullptr;
  if (!macro.size()) {
    ui = new G4UIExecutive(argc, argv, session);
  }

    auto* runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);
#ifdef G4MULTITHREADED
  if (nThreads > 0) {
    runManager->SetNumberOfThreads(nThreads);
  }
#endif
    
    // Physics list
    G4PhysListFactory factory;
    auto* phys = factory.GetReferencePhysList("FTFP_BERT");
    runManager->SetUserInitialization(phys);
   
     std::cout << " >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   Radii: ";
     for (const auto& r : radii) {
        std::cout << r << " "<< "  thickness "<< thickness;
       
     }
    std::cout << std::endl; 
    auto detConstruction = new DetectorConstruction(radii, thickness);
    runManager->SetUserInitialization(detConstruction);
    
    // Actions
    auto actionInitialization = new ActionInitialization(detConstruction, particleName, pMin, pMax, outFileName, gunZPos);
    runManager->SetUserInitialization(actionInitialization);
    runManager-> Initialize();
 
    // Visualization
    auto* visManager = new G4VisExecutive();
    visManager->Initialize();
    
    auto* UImanager = G4UImanager::GetUIpointer();

    G4RunManager::GetRunManager()->BeamOn(nEvent);
    if (show_gui){
         UImanager->ApplyCommand("/control/execute macros/init_vis.mac");
         ui->SessionStart();
         delete ui;

    }
  delete visManager;
  delete runManager;
    
}
