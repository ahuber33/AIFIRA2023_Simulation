#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "Analyse.hh"

using namespace std;

void Analyse_bis()
{

    TFile* file1 = new TFile("BdF_ORCA_4ms_CLEAN.root");
    TH2D* a = (TH2D*)file1->Get("BdF");
    //a->Draw("colz");

    //new TCanvas;
    TFile* file = new TFile("BdF_ORCA_500ms_CLEAN.root");
    TH2D* h = (TH2D*)file->Get("BdF");
    //h->Draw("colz");

    vector<float> Value;
    float Mean = 0;

    TH2D* final = (TH2D *)a->Clone("BdF");
    
    

    for (int i = 1; i <= h->GetNbinsX(); i++)
    {
        for (int j = 1; j <= h->GetNbinsY(); j++)
        {
            final->SetBinContent(i, j, 1.452*a->GetBinContent(i,j) - h->GetBinContent(i,j));
            //final->SetBinContent(i, j, 1.452*a->GetBinContent(i,j));


            if (j <100 || j>1900)
            {
                Value.push_back(final->GetBinContent(i, j));
            }
               //Value.push_back(final->GetBinContent(i, j));
            
        }

    }

    //final->Draw("colz");
    //final->SaveAs("BdF_ORCA_500ms_CLEAN_bis.root", "recreate");

    Mean = CalculMoyenne(Value);
    float EcartType = CalculEcartType(Value);
    cout << "n = " << Value.size() << endl;
    cout << "Mean = " << Mean << endl;
    cout << "sigma = " << EcartType << endl;

}