#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "Analyse.hh"

using namespace std;

void Analyse()
{
    gStyle->SetOptStat(0);
    path = "/mnt/hgfs/shared/Simulations/AIFIRA2023_Simulation/Resultats/Analyse/Data/CAMERA_AIFIRA/ZnS-1mm/Linearite/10ms/Linearite_";
    //path = "/mnt/hgfs/shared/Simulations/AIFIRA2023_Simulation/Resultats/Analyse/Data/CAMERA_AIFIRA/ZnS-1mm/Degradation/Film_degradation_";
    //path = "/mnt/hgfs/shared/Simulations/AIFIRA2023_Simulation/Resultats/Analyse/Data/ORCA/EJ262-0.1mm/irradiation_5ms/";
    //path = "/mnt/hgfs/shared/Simulations/AIFIRA2023_Simulation/Resultats/Analyse/Data/ORCA/ZnS-1mm/ZnS_1mm_integration_camera_1s";
    //path = "/mnt/hgfs/shared/Simulations/AIFIRA2023_Simulation/Resultats/Analyse/Data/ORCA/ZnS-1mm/ZnS_1mm_temps_integration_5s_temps_irradiation_10ms";

    N_files = 1980;
    BitCodage = 8;
    Rebin =2; // ATTENTION : Pour l'analyse ORCA, il faut penser à changer le rebinning effectué pour la CAMERA AIFIRA

    //BdF_Analyse(path, N_files);
    // TCanvas *c1 = new TCanvas;
    // c1->SetGrayscale();
    // BdF->Draw("colz");


    // for (int i=0; i<=10; i++)
    // {
    //     filename = path + to_string(i) + ".txt";
         filename = path + "1797.txt";
    //     //filename = path + "EJ262_0.1mm_temps_integration_500ms_temps_irradiation_10ms.txt";
         Signal_Analyse(filename);
         Draw_Results(Rebin);
    //     // string png = to_string(i) + ".png";
    //     // c1->SaveAs(png.c_str());
    //      //c1->SaveAs("EJ262_0.1mm_temps_integration_500ms_temps_irradiation_1ms_1.png");
    //     // c1->Modified();
    //     // c1->Update();
    // }

   //Analyse_Degradation(path, 14, 5417);



    // TFile *file_BdF = new TFile("BdF_ORCA_500ms_CLEAN.root"); // BdF ORCA
    // BdF_Mean = (TH2D *)file_BdF->Get("BdF");

    // string name = "Signal";

    // Signal = Plot_from_CSV(filename.c_str(), name.c_str());
    // Signal_true = (TH2D *)Signal->Clone("Signal_true");
    // // Signal_true->SetName("Signal_true");
    // ComputeBdFSubstraction(Signal_true, BdF_Mean);
    // Signal_true->Draw("colz");
    // // new TCanvas;
    // // BdF_Mean->Draw("colz");

    vector<float> Value;
    float Mean = 0;

    for (int i = 1; i <= 500; i++)
    {
        for (int j = 1; j <= 500; j++)
        {
            //if (i<800 && j >800 && j<1000)
            //
                Value.push_back(Signal_true->GetBinContent(i, j));
                //Value.push_back(BdF_Mean->GetBinContent(i, j));
            //}
        }
    }

    // // for (int i=0; i<Value.size(); i++)

    Mean = CalculMoyenne(Value);
    float EcartType = CalculEcartType(Value);
    cout << "n = " << Value.size() << endl;
    cout << "Mean = " << Mean << endl;
    cout << "sigma = " << EcartType << endl;

}