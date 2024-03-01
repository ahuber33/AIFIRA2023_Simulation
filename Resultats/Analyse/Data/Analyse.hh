#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "TROOT.h"
#include <TStyle.h>
#include "TF1.h"
#include "TMath.h"

string path;
string filename;
int BitCodage = 16; // Par défaut 16 bits
int N_files = 0;
int Rebin=1;

TH2D *BdF;
TH2D *BdF_Mean;
TH2D *Signal;
TH2D *Signal_true;

TH1D *projX;
TH1D *projY;
TH1D *projX_true;
TH1D *projY_true;

TF2 *fit;
TF2 *fit_bis;
TF2 *fit_signal;
TF1 *fit1X;
TF1 *fit2X;
TF1 *fitX;
TF1 *fit1Y;
TF1 *fit2Y;
TF1 *fitY;
bool flag_BdF;
float Max_flag_Signal = 55; // 60 pour CAMERA AIFIRA
int BinXMax = 0;
int BinYMax = 0;

TCanvas *c1;
TPad *pad1;
TPad *pad2;
TPad *pad3;
TPad *pad4;
TPad *pad5;
TPad *pad6;
TPad *pad7;

TH2D *Plot_from_CSV(const char *filename, const char *name)
{
    vector<vector<float>> tableauCSV;
    ifstream fichier(filename);

    if (!fichier.is_open())
    {
        cerr << "Erreur : Impossible d'ouvrir le fichier CSV." << endl;
        abort;
    }

    // Lire le fichier ligne par ligne
    string ligne;

    while (getline(fichier, ligne))
    {
        stringstream ss(ligne);
        vector<float> ligneCSV;
        float valeur = 0;

        while (ss >> valeur)
        {
            ligneCSV.push_back(valeur);
            if (ss.peek() == '\t')
                ss.ignore();
        }
        // Ajouter la ligne au tableau
        tableauCSV.push_back(ligneCSV);
    }

    int vectorYsize = tableauCSV.size();
    int vectorXsize = tableauCSV[1].size();
    // cout << "Taille vecteur X = " << vectorXsize << endl;
    // cout << "Taille vecteur Y = " << vectorYsize << endl;

    TH2D *histo = new TH2D(name, name, vectorXsize, 0.5, vectorXsize - 0.5, vectorYsize, 0.5, vectorYsize - 0.5);

    for (int i = 0; i != vectorYsize; i++)
    {
        for (int j = 0; j != vectorXsize; j++)
        {
            histo->SetBinContent(j + 1, vectorYsize - i, tableauCSV[i][j]);
            if (tableauCSV[i][j] == 0)
            {
                histo->SetBinContent(j + 1, tableauCSV.size() - i, 0.001);
            }
        }
    }

    return histo;
}

float CalculMoyenne(vector<float> Values)
{
    float Somme = 0.;

    for (float Value : Values)
    {
        Somme += Value;
    }

    return Somme / Values.size();
}

float CalculEcartType(vector<float> Values)
{
    // Calcul de la moyenne
    float Mean = CalculMoyenne(Values);

    // Calcul de la somme des carrés des écarts par rapport à la moyenne
    float SommeCarreesEcarts = 0.;
    for (float Value : Values)
    {
        float ecart = Value - Mean;
        SommeCarreesEcarts += ecart * ecart;
    }

    // Calcul de la Variance
    float Variance = SommeCarreesEcarts / Values.size();

    // Calcul de l'ecart type
    float EcartType = sqrt(Variance);

    return EcartType;
}

void ComputeTH2Sum(TH2D *Main, TH2D *temp)
{
    for (int i = 0; i <= Main->GetNbinsX(); i++)
    {
        for (int j = 0; j <= Main->GetNbinsY(); j++)
        {

            Main->SetBinContent(i, j, Main->GetBinContent(i, j) + temp->GetBinContent(i, j));
        }
    }
}

bool Compute_flag_BdF_or_Signal(TH2D *temp)
{
    bool flag = true; // on part du principe que c'est du BdF
    float value = 0;

    for (int i = 0; i <= temp->GetNbinsX(); i++)
    {
        for (int j = 0; j <= 500; j++)
        //for (int j = 0; j <= temp->GetNbinsY(); j++)
        {
            if (temp->GetBinContent(i, j) > value)
                value = temp->GetBinContent(i, j);
        }
    }

    if (value > Max_flag_Signal)
        flag = false;
    if (flag == false)
        cout << "SIGNAL for  [" << temp->GetName() << "] = " << flag << " with a max @ " << value << endl;

    return flag;
}

void ComputeTH2Mean(TH2D *Main, float N_files)
{
    for (int i = 0; i <= Main->GetNbinsX(); i++)
    {
        for (int j = 0; j <= Main->GetNbinsY(); j++)
        {

            Main->SetBinContent(i, j, Main->GetBinContent(i, j) / N_files);
        }
    }
}

void BdF_Analyse(string path, int N_total_files)
{
    string filename = path + "1711.txt";

    BdF = Plot_from_CSV(filename.c_str(), "BdF");

    float N_BdF_files = 1;
    for (int i = 1712; i <= N_total_files; i++)
    {
        filename = path + to_string(i) + ".txt";
        string name = "BdF_" + to_string(i);
        // cout << "filename = " << filename << endl;
        TH2D *temp = Plot_from_CSV(filename.c_str(), name.c_str());
        flag_BdF = Compute_flag_BdF_or_Signal(temp);
        if (flag_BdF == true)
        {
            ComputeTH2Sum(BdF, temp);
            N_BdF_files++;
        }
        // cout << "N BdF files = " << N_BdF_files << endl;
        delete temp;
    }
    ComputeTH2Mean(BdF, N_BdF_files);

    BdF->SaveAs("BdF_temp.root", "recreate");
}

void FindMaxSignalBins(TH2D *temp, int BitCodage)
{
    float value = 0;

    cout << "Nbins = " << temp->GetNbinsX() << endl;

    if (BitCodage == 8)
    {
        for (int i = 350/Rebin; i <= 380/Rebin; i++)
        {
            for (int j = 260/Rebin; j <= 300/Rebin; j++)
            {
                if (temp->GetBinContent(i, j) > value)
                {
                    value = temp->GetBinContent(i, j);
                    BinXMax = i;
                    BinYMax = j;
                }
            }
        }
    }

    else
    {
        for (int i = 0; i <= temp->GetNbinsX(); i++)
        {
            for (int j = 0; j <= temp->GetNbinsY(); j++)
            {
                if (temp->GetBinContent(i, j) > value)
                {
                    value = temp->GetBinContent(i, j);
                    BinXMax = i;
                    BinYMax = j;
                }
            }
        }
    }
    cout << "Max = " << value << endl;
}

void ComputeBdFSubstraction(TH2D *Signal, TH2D *BdF)
{
    if (Signal->GetNbinsX() != BdF->GetNbinsX() || Signal->GetNbinsY() != BdF->GetNbinsY())
    {
        cerr << "Erreur : Impossible d'effectuer la soustraction car les binnings des 2 fichiers sont différents" << endl;
        abort;
    }

    for (int i = 0; i <= Signal->GetNbinsX(); i++)
    {
        for (int j = 0; j <= Signal->GetNbinsY(); j++)
        {

            Signal->SetBinContent(i, j, Signal->GetBinContent(i, j) - BdF->GetBinContent(i, j));
        }
    }
}

void Signal_Analyse(string filename)
{
    TFile *file_BdF = new TFile("BdF_temp.root"); // BdF CAMERA AIFIRA
    //TFile *file_BdF = new TFile("BdF_ORCA_1s_CLEAN.root"); // BdF ORCA
    //TFile *file_BdF = new TFile("BdF_ORCA_4ms_CLEAN.root"); // BdF ORCA
    BdF_Mean = (TH2D *)file_BdF->Get("BdF");

    string name = "Signal";

    Signal = Plot_from_CSV(filename.c_str(), name.c_str());
    Signal_true = (TH2D *)Signal->Clone("Signal_true");
    // Signal_true->SetName("Signal_true");
    ComputeBdFSubstraction(Signal_true, BdF_Mean);
    Signal_true->Rebin2D(Rebin, Rebin);
    FindMaxSignalBins(Signal_true, BitCodage);
    cout << "Bin X [Max] = " << BinXMax << endl;
    cout << "Bin Y [Max] = " << BinYMax << endl;

    projX = new TH1D("projX", "projX", Signal->GetNbinsX(), 0, float(Signal_true->GetNbinsX()) - 0.5);
    projY = new TH1D("projY", "projY", Signal->GetNbinsY(), 0, float(Signal_true->GetNbinsY()) - 0.5);
    projX_true = new TH1D("projX_true", "projX_true", Signal_true->GetNbinsX(), 0, float(Signal_true->GetNbinsX()) - 0.5);
    projY_true = new TH1D("projY_true", "projY_true", Signal_true->GetNbinsY(), 0, float(Signal_true->GetNbinsY()) - 0.5);

    fit1X = new TF1("fit1X", "[0] * TMath::Gaus(x, [1], [2])", 0, 2048);
    fit2X = new TF1("fit2X", "[0] * TMath::Gaus(x, [1], [2])", 0, 2048);
    fitX = new TF1("fitX", "[0] * TMath::Gaus(x, [1], [2]) + [3] *TMath::Gaus(x, [4], [5])", 0, 2048);
    fit1Y = new TF1("fit1Y", "[0] * TMath::Gaus(x, [1], [2])", 0, 2048);
    fit2Y = new TF1("fit2Y", "[0] * TMath::Gaus(x, [1], [2])", 0, 2048);
    fitY = new TF1("fitY", "[0] * TMath::Gaus(x, [1], [2]) + [3] *TMath::Gaus(x, [4], [5])", 0, 2048);


    // for (int i=0; i<Signal_true->GetNbinsX(); i++)
    // {
    //     for (int j=0; j<Signal_true->GetNbinsY(); j++)
    //     {
    //         if (i <836 || i >904) Signal_true->SetBinContent(i, j, 0);
    //         if(j<984 || j>1060) Signal_true->SetBinContent(i, j, 0);
    //     }
    // }
}

void CreatePad(const char *name)
{
    c1 = new TCanvas(name, name, 0, 10, 2000, 1800);
    c1->cd();
    pad1 = new TPad("pad1", "", 0.05, 0.62, 0.38, 0.95);
    pad2 = new TPad("pad2", "", 0.62, 0.62, 0.95, 0.95);
    pad3 = new TPad("pad3", "", 0.05, 0.27, 0.38, 0.60);
    pad4 = new TPad("pad4", "", 0.6, 0.27, 0.95, 0.60);
    pad5 = new TPad("pad5", "", 0.05, 0., 0.4, 0.24);
    pad6 = new TPad("pad6", "", 0.6, 0., 0.95, 0.24);
    pad7 = new TPad("pad7", "", 0., 0.95, 1, 1);

    pad1->Draw();
    pad2->Draw();
    pad3->Draw();
    pad4->Draw();
    pad5->Draw();
    pad6->Draw();
    pad7->Draw();
}

void Draw_Results(int Rebin)
{
    CreatePad("Test");

    float largeur_fenetre =5;

    pad7->cd();
    TLatex *txt = new TLatex(0.15, 0.5, filename.c_str());
    txt->SetTextColor(kBlack);
    txt->SetTextSize(0.25);
    txt->Draw("");

    // PAD 1 -> RAW SIGNAL
    pad1->cd();
    Signal->Draw("colz");
    Signal->SetNameTitle("Raw Signal", "Raw Signal");
    Signal->GetZaxis()->SetRangeUser(-1, pow(2, BitCodage));
    Signal->GetXaxis()->SetTitle("X Pixels");
    Signal->GetYaxis()->SetTitle("Y Pixels");
    pad1->Update();

    // PAD 2 -> MEAN BDF
    pad2->cd();
    BdF_Mean->Draw("colz");
    BdF_Mean->SetNameTitle("Mean BdF", "Mean BdF");
    BdF_Mean->GetZaxis()->SetRangeUser(-1, pow(2, BitCodage));
    BdF_Mean->GetXaxis()->SetTitle("X Pixels");
    BdF_Mean->GetYaxis()->SetTitle("Y Pixels");
    pad2->SetLogz();
    pad2->Update();

    // PAD 5 -> X PROJECTION
    pad5->cd();
    Signal->ProjectionX("projX", BinYMax, BinYMax, "");
    Signal_true->ProjectionX("projX_true", BinYMax, BinYMax, "");
    projX->Draw();
    projX->SetMaximum(pow(2, BitCodage));
    projX->SetMinimum(0);
    projX->SetNameTitle("ProjX @ Maximum Signal", "ProjX @ Maximum Signal");
    projX->SetLineColor(kBlack);
    projX_true->Draw("same");
    projX_true->SetLineColor(kCyan);
    //projX_true->Rebin(Rebin);
    fit1X->SetParameter(1, BinXMax*Rebin);
    fit1X->SetParLimits(1, (BinXMax-10)*Rebin, (BinXMax+10)*Rebin);
    fit1X->SetParLimits(2, 0.1, 50);
    fit1X->SetParameter(2, 15);
    projX_true->Fit("fit1X", "QN", "");
    fitX->SetParameter(0, fit1X->GetParameter(0));
    fitX->FixParameter(1, fit1X->GetParameter(1));
    fitX->SetParameter(2, fit1X->GetParameter(2));
    fitX->SetParLimits(2, 3, 50);
    fitX->SetParameter(3, fit1X->GetParameter(0) / 10);
    fitX->SetParLimits(3, fit1X->GetParameter(0) / 1000, fit1X->GetParameter(0) * 10);
    fitX->FixParameter(4, fit1X->GetParameter(1));
    fitX->SetParameter(5, fit1X->GetParameter(2) * 10);
    fitX->SetParLimits(5, fit1X->GetParameter(2)*1, 1000);
    projX_true->Fit("fitX", "Q", "", fit1X->GetParameter(1) - largeur_fenetre * fit1X->GetParameter(2), fit1X->GetParameter(1) + largeur_fenetre * fit1X->GetParameter(2));
    fit1X->SetParameters(fitX->GetParameter(0), fitX->GetParameter(1), fitX->GetParameter(2));
    fit2X->SetParameters(fitX->GetParameter(3), fitX->GetParameter(4), fitX->GetParameter(5));
    fit1X->Draw("same");
    fit2X->Draw("same");
    fit1X->SetLineColor(kGreen + 1);
    fit2X->SetLineColor(kGray);
    fit1X->SetLineStyle(7);
    fit2X->SetLineStyle(7);

    projX->GetXaxis()->SetTitle("X Pixels");
    projX->GetYaxis()->SetTitle("Gray Value");

    string s_integral_X = Form("RAW Signal = %g Gv", projX->Integral());
    TLatex *t_integral_X = new TLatex(BinXMax * 1.2, pow(2, BitCodage) * 0.9, s_integral_X.c_str());
    t_integral_X->SetTextColor(kBlack);
    t_integral_X->Draw("same");

    string s_integral_X_true = Form("True Signal = %g Gv", projX_true->Integral());
    TLatex *t_integral_X_true = new TLatex(BinXMax * 1.2, pow(2, BitCodage) * 0.8, s_integral_X_true.c_str());
    t_integral_X_true->SetTextColor(kCyan);
    t_integral_X_true->Draw("same");

    string s_integral_X_fit = Form("Fit [2 gaus] = %g Gv", fitX->Integral(0, Signal->GetNbinsX()));
    TLatex *t_integral_X_fit = new TLatex(BinXMax * 1.2, pow(2, BitCodage) * 0.7, s_integral_X_fit.c_str());
    t_integral_X_fit->SetTextColor(kRed);
    t_integral_X_fit->Draw("same");

    string s_integral_X_fit_chi2 = Form("#Chi^{2}/Ndf = %g ", fitX->GetChisquare() / fitX->GetNDF());
    TLatex *t_integral_X_fit_chi2 = new TLatex(BinXMax * 1.2, pow(2, BitCodage) * 0.65, s_integral_X_fit_chi2.c_str());
    t_integral_X_fit_chi2->SetTextColor(kRed);
    t_integral_X_fit_chi2->Draw("same");

    string s_integral_X_fit1 = Form("Signal gaus = %g Gv", fit1X->Integral(0, Signal->GetNbinsX()));
    TLatex *t_integral_X_fit1 = new TLatex(BinXMax * 1.2, pow(2, BitCodage) * 0.55, s_integral_X_fit1.c_str());
    t_integral_X_fit1->SetTextColor(kGreen + 1);
    t_integral_X_fit1->Draw("same");

    string s_integral_X_fit1_sig = Form("#sigma = %g pixels", fit1X->GetParameter(2));
    TLatex *t_integral_X_fit1_sig = new TLatex(BinXMax * 1.2, pow(2, BitCodage) * 0.5, s_integral_X_fit1_sig.c_str());
    t_integral_X_fit1_sig->SetTextColor(kGreen + 1);
    t_integral_X_fit1_sig->Draw("same");

    string s_integral_X_fit2 = Form("Noise gaus = %g Gv", fit2X->Integral(0, Signal->GetNbinsX()));
    TLatex *t_integral_X_fit2 = new TLatex(BinXMax * 1.2, pow(2, BitCodage) * 0.4, s_integral_X_fit2.c_str());
    t_integral_X_fit2->SetTextColor(kGray);
    t_integral_X_fit2->Draw("same");

    string s_integral_X_fit2_sig = Form("#sigma = %g pixels", fit2X->GetParameter(2));
    TLatex *t_integral_X_fit2_sig = new TLatex(BinXMax * 1.2, pow(2, BitCodage) * 0.35, s_integral_X_fit2_sig.c_str());
    t_integral_X_fit2_sig->SetTextColor(kGray);
    t_integral_X_fit2_sig->Draw("same");

    // cout << "\nX PROJECTION :" << endl;
    // cout << "Chi2 [/NdF] fit 1X = " << fit1X->GetChisquare() << " [" << fit1X->GetNDF() << "]" << endl;
    // cout << "Integral HISTO = " << projX_true->Integral() << endl;
    // cout << "Integral FIT = " << fitX->Integral(0, Signal->GetNbinsX()) << endl;
    // cout << "\n" << endl;

    // PAD 6 -> Y PROJECTION
    pad6->cd();
    Signal->ProjectionY("projY", BinXMax, BinXMax, "");
    Signal_true->ProjectionY("projY_true", BinXMax, BinXMax, "");
    projY->Draw();
    projY->SetMaximum(pow(2, BitCodage));
    projY->SetMinimum(0);
    projY->SetNameTitle("ProjY @ Maximum Signal", "ProjY @ Maximum Signal");
    projY->SetLineColor(kBlack);
    projY_true->Draw("same");
    projY_true->SetLineColor(kCyan);
    //projY_true->Rebin(Rebin);
    fit1Y->SetParameter(1, BinYMax*Rebin);
    fit1X->SetParLimits(1, (BinYMax-10)*Rebin, (BinYMax+10)*Rebin);
    fit1Y->SetParameter(2, 5);
    fit1Y->SetParLimits(2, 1, 50);
    projY_true->Fit("fit1Y", "N");
    fitY->SetParameter(0, fit1Y->GetParameter(0));
    fitY->FixParameter(1, fit1Y->GetParameter(1));
    fitY->SetParameter(2, fit1Y->GetParameter(2));
    fitY->SetParLimits(2, 3, 50);
    fitY->SetParameter(3, fit1Y->GetParameter(0) / 100);
    fitY->SetParLimits(3, fit1Y->GetParameter(0) / 10000, fit1Y->GetParameter(0) * 10);
    fitY->FixParameter(4, fit1Y->GetParameter(1));
    fitY->SetParameter(5, fit1Y->GetParameter(2) * 10);
    fitY->SetParLimits(5, fit1Y->GetParameter(2), 1000);
    projY_true->Fit("fitY", "", "", fit1Y->GetParameter(1) - largeur_fenetre * fit1Y->GetParameter(2), fit1Y->GetParameter(1) + largeur_fenetre * fit1Y->GetParameter(2));
    fit1Y->SetParameters(fitY->GetParameter(0), fitY->GetParameter(1), fitY->GetParameter(2));
    fit2Y->SetParameters(fitY->GetParameter(3), fitY->GetParameter(4), fitY->GetParameter(5));
    fit1Y->Draw("same");
    fit2Y->Draw("same");
    fit1Y->SetLineColor(kGreen + 1);
    fit2Y->SetLineColor(kGray);
    fit1Y->SetLineStyle(7);
    fit2Y->SetLineStyle(7);

    projY->GetXaxis()->SetTitle("Y Pixels");
    projY->GetYaxis()->SetTitle("Gray Value");

    string s_integral_Y = Form("RAW Signal = %g Gv", projY->Integral());
    TLatex *t_integral_Y = new TLatex(BinYMax * 1.2, pow(2, BitCodage) * 0.9, s_integral_Y.c_str());
    t_integral_Y->SetTextColor(kBlack);
    t_integral_Y->Draw("same");

    string s_integral_Y_true = Form("True Signal = %g Gv", projY_true->Integral());
    TLatex *t_integral_Y_true = new TLatex(BinYMax * 1.2, pow(2, BitCodage) * 0.8, s_integral_Y_true.c_str());
    t_integral_Y_true->SetTextColor(kCyan);
    t_integral_Y_true->Draw("same");

    string s_integral_Y_fit = Form("Fit [2 gaus] = %g Gv", fitY->Integral(0, Signal->GetNbinsY()));
    TLatex *t_integral_Y_fit = new TLatex(BinYMax * 1.2, pow(2, BitCodage) * 0.7, s_integral_Y_fit.c_str());
    t_integral_Y_fit->SetTextColor(kRed);
    t_integral_Y_fit->Draw("same");

    string s_integral_Y_fit_chi2 = Form("#Chi^{2}/Ndf = %g ", fitY->GetChisquare() / fitY->GetNDF());
    TLatex *t_integral_Y_fit_chi2 = new TLatex(BinYMax * 1.2, pow(2, BitCodage) * 0.65, s_integral_Y_fit_chi2.c_str());
    t_integral_Y_fit_chi2->SetTextColor(kRed);
    t_integral_Y_fit_chi2->Draw("same");

    string s_integral_Y_fit1 = Form("Signal gaus = %g Gv", fit1Y->Integral(0, Signal->GetNbinsY()));
    TLatex *t_integral_Y_fit1 = new TLatex(BinYMax * 1.2, pow(2, BitCodage) * 0.55, s_integral_Y_fit1.c_str());
    t_integral_Y_fit1->SetTextColor(kGreen + 1);
    t_integral_Y_fit1->Draw("same");

    string s_integral_Y_fit_sig = Form("#sigma = %g pixels", fit1Y->GetParameter(2));
    TLatex *t_integral_Y_fit_sig = new TLatex(BinYMax * 1.2, pow(2, BitCodage) * 0.5, s_integral_Y_fit_sig.c_str());
    t_integral_Y_fit_sig->SetTextColor(kGreen + 1);
    t_integral_Y_fit_sig->Draw("same");

    string s_integral_Y_fit2 = Form("Noise gaus = %g Gv", fit2Y->Integral(0, Signal->GetNbinsY()));
    TLatex *t_integral_Y_fit2 = new TLatex(BinYMax * 1.2, pow(2, BitCodage) * 0.4, s_integral_Y_fit2.c_str());
    t_integral_Y_fit2->SetTextColor(kGray);
    t_integral_Y_fit2->Draw("same");

    string s_integral_Y_fit2_sig = Form("#sigma = %g pixels", fit2Y->GetParameter(2));
    TLatex *t_integral_Y_fit2_sig = new TLatex(BinYMax * 1.2, pow(2, BitCodage) * 0.35, s_integral_Y_fit2_sig.c_str());
    t_integral_Y_fit2_sig->SetTextColor(kGray);
    t_integral_Y_fit2_sig->Draw("same");

    // cout << "\nY PROJECTION :" << endl;
    // cout << "Chi2 [/NdF] fit 1Y = " << fit1Y->GetChisquare() << " [" << fit1X->GetNDF() << "]" << endl;
    // cout << "Integral HISTO = " << projY_true->Integral() << endl;
    // cout << "Integral FIT = " << fitY->Integral(0, Signal->GetNbinsY()) << endl;
    // cout << "\n" << endl;

    // PAD 4 -> TGRAPH2D with FIT
    pad4->cd();
    fit = new TF2("fit", "([0]*TMath::Gaus(x,[1],[2]) + [3]*TMath::Gaus(x,[4],[5])) * ([6]*TMath::Gaus(y,[7],[8]) + [9]*TMath::Gaus(y,[10],[11]))", fit1X->GetParameter(1) - largeur_fenetre * fit1X->GetParameter(2), fit1X->GetParameter(1) + largeur_fenetre * fit1X->GetParameter(2), fit1Y->GetParameter(1) - largeur_fenetre * fit1Y->GetParameter(2), fit1Y->GetParameter(1) + largeur_fenetre * fit1Y->GetParameter(2));
    //TGraph2D *test = new TGraph2D(Signal_true);
    Signal_true->Draw("surf2");
    Signal_true->SetMaximum(pow(2, BitCodage));
    Signal_true->SetMinimum(0);
    Signal_true->SetNameTitle("True Signal", "True Signal");
    //Signal_true->Rebin2D(Rebin, Rebin);
    fit->SetParameter(0, fitX->GetParameter(0)/1);
    fit->FixParameter(1, fitX->GetParameter(1));
    fit->FixParameter(2, fitX->GetParameter(2));
    fit->SetParameter(3, fitX->GetParameter(3)/1);
    fit->FixParameter(4, fitX->GetParameter(4));
    fit->FixParameter(5, fitX->GetParameter(5));
    //fit->FixParameter(6, fitX->GetParameter(0));
    fit->SetParameter(6, fitY->GetParameter(0/1));
    fit->FixParameter(7, fitY->GetParameter(1));
    fit->FixParameter(8, fitY->GetParameter(2));
    fit->SetParameter(9, fitY->GetParameter(3)/1);
    fit->FixParameter(10, fitY->GetParameter(4));
    fit->FixParameter(11, fitY->GetParameter(5));

    Signal_true->Fit("fit", "QNR");
    fit->SetMaximum(pow(2, BitCodage));
    fit->SetMinimum(0);
    // fit->SetNpy(400);
    fit->Draw("surf1 same");

    float Min_integral_X = fit->GetParameter(1) - 3 * fit->GetParameter(2);
    float Max_integral_X = fit->GetParameter(1) + 3 * fit->GetParameter(2);
    float Min_integral_Y = fit->GetParameter(6) - 3 * fit->GetParameter(7);
    float Max_integral_Y = fit->GetParameter(6) + 3 * fit->GetParameter(7);

    fit_signal = new TF2("fit_signal", "([0]*TMath::Gaus(x,[1],[2])) * ([3]*TMath::Gaus(y,[4],[5]))", 0, 2048, 0, 2048);
    fit_signal->FixParameter(0, fit->GetParameter(0));
    fit_signal->FixParameter(1, fit->GetParameter(1));
    fit_signal->FixParameter(2, fit->GetParameter(2));
    fit_signal->FixParameter(3, fit->GetParameter(6));
    fit_signal->FixParameter(4, fit->GetParameter(7));
    fit_signal->FixParameter(5, fit->GetParameter(8));

    Signal_true->GetXaxis()->SetTitle("X Pixels");
    Signal_true->GetYaxis()->SetTitle("Y Pixels");
    Signal_true->GetZaxis()->SetTitle("Gray Value");


    string s_integral_histo = Form("Integral histo 3#sigma = %g Gv", Signal_true->Integral(Min_integral_X/Rebin, Max_integral_X/Rebin, Min_integral_Y/Rebin, Max_integral_Y/Rebin));
    TLatex *t_integral_histo = new TLatex(-0.5, 0.9, s_integral_histo.c_str());
    t_integral_histo->SetTextColor(kBlack);
    t_integral_histo->Draw("same");

    string s_integral_fit = Form("Integral fit = %g Gv", fit->Integral(0, Signal->GetNbinsX(), 0, Signal->GetNbinsY()));
    TLatex *t_integral_fit = new TLatex(-0.5, 0.8, s_integral_fit.c_str());
    t_integral_fit->SetTextColor(kBlack);
    t_integral_fit->Draw("same");

    string s_chi_2D = Form("#Chi^{2}/Ndf = %g ", fit->GetChisquare() / fit->GetNDF());
    TLatex *t_chi_2D = new TLatex(-0.5, 0.7, s_chi_2D.c_str());
    t_chi_2D->SetTextColor(kBlack);
    t_chi_2D->Draw("same");

    string s_integral_fit_signal = Form("Integral fit Signal = %g Gv", fit_signal->Integral(fit1X->GetParameter(1) - largeur_fenetre * fit1X->GetParameter(2), fit1X->GetParameter(1) + largeur_fenetre * fit1X->GetParameter(2), fit1Y->GetParameter(1) - largeur_fenetre * fit1Y->GetParameter(2), fit1Y->GetParameter(1) + largeur_fenetre * fit1Y->GetParameter(2)));
    TLatex *t_integral_fit_signal = new TLatex(-0.5, 0.5, s_integral_fit_signal.c_str());
    t_integral_fit_signal->SetTextColor(kBlack);
    t_integral_fit_signal->Draw("same");

    string s_sig_2DX = Form("#sigma_{X}^{Signal} = %g pixels", fit->GetParameter(2));
    TLatex *t_sig_2DX = new TLatex(-0.5, 0.4, s_sig_2DX.c_str());
    t_sig_2DX->SetTextColor(kBlack);
    t_sig_2DX->Draw("same");

    string s_sig_2DY = Form("#sigma_{Y}^{Signal} = %g pixels", fit->GetParameter(8));
    TLatex *t_sig_2DY = new TLatex(-0.5, 0.25, s_sig_2DY.c_str());
    t_sig_2DY->SetTextColor(kBlack);
    t_sig_2DY->Draw("same");

    //cout << "Eval = " << fit->Eval(BinXMax, BinYMax) << endl;

    // PAD 3 -> TRUE SIGNAL
    pad3->cd();
    //pad3->SetRightMargin(0.2);
    pad3->SetLogz();
    fit_bis = new TF2("fit_bis", "([0]*TMath::Gaus(x,[1],[2]) + [3]*TMath::Gaus(x,[4],[5])) * ([6]*TMath::Gaus(y,[7],[8]) + [9]*TMath::Gaus(y,[10],[11]))", fit1X->GetParameter(1) - 7 * fit1X->GetParameter(2), fit1X->GetParameter(1) + 7 * fit1X->GetParameter(2), fit1Y->GetParameter(1) - 7 * fit1Y->GetParameter(2), fit1Y->GetParameter(1) + 7 * fit1Y->GetParameter(2));
    fit_bis->FixParameter(0, fit->GetParameter(0));
    fit_bis->FixParameter(1, fit->GetParameter(1));
    fit_bis->FixParameter(2, fit->GetParameter(2));
    fit_bis->FixParameter(3, fit->GetParameter(3));
    fit_bis->FixParameter(4, fit->GetParameter(4));
    fit_bis->FixParameter(5, fit->GetParameter(5));
    fit_bis->FixParameter(6, fit->GetParameter(6));
    fit_bis->FixParameter(7, fit->GetParameter(7));
    fit_bis->FixParameter(8, fit->GetParameter(8));
    fit_bis->FixParameter(9, fit->GetParameter(9));
    fit_bis->FixParameter(10, fit->GetParameter(10));
    fit_bis->FixParameter(11, fit->GetParameter(11));
    Signal_true->Draw("colz");
    Signal_true->GetXaxis()->SetRangeUser(fit->GetParameter(1) - 10 * fit->GetParameter(2), fit->GetParameter(1) + 10 * fit->GetParameter(2));
    Signal_true->GetYaxis()->SetRangeUser(fit->GetParameter(7) - 10 * fit->GetParameter(8), fit->GetParameter(7) + 10 * fit->GetParameter(8));
    Signal_true->GetZaxis()->SetRangeUser(0.1, pow(2, BitCodage));
    Signal_true->SetNameTitle("True Signal", "True Signal");
    fit_bis->Draw("same");
    // fit_bis->SetContourLevel(100, 0);
    Signal_true->GetXaxis()->SetTitle("X Pixels");
    Signal_true->GetYaxis()->SetTitle("Y Pixels");
    // Signal_true->GetZaxis()->SetTitle("Gray Value");
}

void Analyse_Degradation(string path, int start, int end)
{
    const int n = end - start + 1;

    float Integral_Fit_Signal[n];
    float E_Integral_Fit_Signal[n];
    float Integral_Fit[n];
    float E_Integral_Fit[n];
    float Sigma_X[n];
    float E_Sigma_X[n];
    float Sigma_Y[n];
    float E_Sigma_Y[n];
    float Chi2ndf[n];
    float Time[n];
    float E_Time[n];
    int temp = 0;

    for (int i = start; i <= end; i += 50)
    {
        cout << i << " START" << endl;
        filename = path + to_string(i) + ".txt";
        Signal_Analyse(filename);

        Signal_true->ProjectionX("projX_true", BinYMax, BinYMax, "");
        fit1X->SetParameter(1, BinXMax);
        fit1X->SetParameter(2, 10);
        projX_true->Fit("fit1X", "QN", "");
        fitX->SetParameter(0, fit1X->GetParameter(0));
        fitX->FixParameter(1, fit1X->GetParameter(1));
        fitX->SetParameter(2, fit1X->GetParameter(2));
        // fitX->SetParLimits(2, fit1X->GetParameter(2) - 5, fit1X->GetParameter(2) + 5);
        fitX->SetParameter(3, fit1X->GetParameter(0) / 100);
        fitX->SetParLimits(3, fit1X->GetParameter(0) / 10000, fit1X->GetParameter(0) * 10);
        fitX->FixParameter(4, fit1X->GetParameter(1));
        fitX->SetParameter(5, fit1X->GetParameter(2) * 10);
        fitX->SetParLimits(5, 0, 500);
        projX_true->Fit("fitX", "QN", "", fit1X->GetParameter(1) - 5 * fit1X->GetParameter(2), fit1X->GetParameter(1) + 5 * fit1X->GetParameter(2));
        fit1X->SetParameters(fitX->GetParameter(0), fitX->GetParameter(1), fitX->GetParameter(2));
        fit2X->SetParameters(fitX->GetParameter(3), fitX->GetParameter(4), fitX->GetParameter(5));

        // Y PROJECTION
        Signal_true->ProjectionY("projY_true", BinXMax, BinXMax, "");
        fit1Y->SetParameter(1, BinYMax);
        fit1Y->SetParameter(2, 10);
        projY_true->Fit("fit1Y", "QN");
        fitY->FixParameter(0, fit1Y->GetParameter(0));
        fitY->FixParameter(1, fit1Y->GetParameter(1));
        fitY->SetParameter(2, fit1Y->GetParameter(2));
        // fitY->SetParLimits(2, fit1Y->GetParameter(2) - 5, fit1Y->GetParameter(2) + 5);
        fitY->SetParameter(3, fit1Y->GetParameter(0) / 100);
        fitY->SetParLimits(3, fit1Y->GetParameter(0) / 10000, fit1Y->GetParameter(0) * 10);
        fitY->FixParameter(4, fit1Y->GetParameter(1));
        fitY->SetParameter(5, fit1Y->GetParameter(2) * 10);
        fitY->SetParLimits(5, 0, 500);
        projY_true->Fit("fitY", "QN", "", fit1Y->GetParameter(1) - 5 * fit1Y->GetParameter(2), fit1Y->GetParameter(1) + 5 * fit1Y->GetParameter(2));
        fit1Y->SetParameters(fitY->GetParameter(0), fitY->GetParameter(1), fitY->GetParameter(2));
        fit2Y->SetParameters(fitY->GetParameter(3), fitY->GetParameter(4), fitY->GetParameter(5));

        // TGRAPH2D with FIT
        fit = new TF2("fit", "([0]*TMath::Gaus(x,[1],[2]) + [3]*TMath::Gaus(x,[4],[5])) * ([6]*TMath::Gaus(y,[7],[8]) + [9]*TMath::Gaus(y,[10],[11]))", fit1X->GetParameter(1) - 5 * fit1X->GetParameter(2), fit1X->GetParameter(1) + 5 * fit1X->GetParameter(2), fit1Y->GetParameter(1) - 5 * fit1Y->GetParameter(2), fit1Y->GetParameter(1) + 5 * fit1Y->GetParameter(2));
        fit->SetParameter(0, fitX->GetParameter(0));
        fit->FixParameter(1, fitX->GetParameter(1));
        fit->FixParameter(2, fitX->GetParameter(2));
        fit->SetParameter(3, fitX->GetParameter(3));
        fit->FixParameter(4, fitX->GetParameter(4));
        fit->FixParameter(5, fitX->GetParameter(5));
        fit->SetParameter(6, fitY->GetParameter(0));
        fit->FixParameter(7, fitY->GetParameter(1));
        fit->FixParameter(8, fitY->GetParameter(2));
        fit->SetParameter(9, fitY->GetParameter(3));
        fit->FixParameter(10, fitY->GetParameter(4));
        fit->FixParameter(11, fitY->GetParameter(5));

        Signal_true->Fit("fit", "QN");

        float Min_integral_X = fit->GetParameter(1) - 3 * fit->GetParameter(2);
        float Max_integral_X = fit->GetParameter(1) + 3 * fit->GetParameter(2);
        float Min_integral_Y = fit->GetParameter(6) - 3 * fit->GetParameter(7);
        float Max_integral_Y = fit->GetParameter(6) + 3 * fit->GetParameter(7);

        fit_signal = new TF2("fit_signal", "([0]*TMath::Gaus(x,[1],[2])) * ([3]*TMath::Gaus(y,[4],[5]))", 0, Signal->GetNbinsX(), 0, Signal->GetNbinsY());
        fit_signal->FixParameter(0, fit->GetParameter(0));
        fit_signal->FixParameter(1, fit->GetParameter(1));
        fit_signal->FixParameter(2, fit->GetParameter(2));
        fit_signal->FixParameter(3, fit->GetParameter(6));
        fit_signal->FixParameter(4, fit->GetParameter(7));
        fit_signal->FixParameter(5, fit->GetParameter(8));

        Integral_Fit_Signal[temp] = fit_signal->Integral(0, Signal->GetNbinsX(), 0, Signal->GetNbinsY());
        E_Integral_Fit_Signal[temp] = sqrt(Integral_Fit_Signal[temp]);
        Integral_Fit[temp] = fit->Integral(0, Signal->GetNbinsX(), 0, Signal->GetNbinsY());
        E_Integral_Fit[temp] = sqrt(Integral_Fit[temp]);
        Sigma_X[temp] = fit->GetParameter(2);
        E_Sigma_X[temp] = fit->GetParError(2);
        Sigma_Y[temp] = fit->GetParameter(8);
        E_Sigma_Y[temp] = fit->GetParError(8);
        Chi2ndf[temp] = fit->GetChisquare() / fit->GetNDF();
        Time[temp] = 0.084 * temp * 50;
        E_Time[temp] = 0.084 / 2;
        temp++;
    }

    new TCanvas;
    TGraphErrors *g_Fit_Signal = new TGraphErrors(temp, Time, Integral_Fit_Signal, E_Time, E_Integral_Fit_Signal);
    g_Fit_Signal->Draw("");
    g_Fit_Signal->SetLineColor(kBlack);
    g_Fit_Signal->SetLineWidth(3);
    g_Fit_Signal->SetMarkerColor(kBlack);
    g_Fit_Signal->SetName("Evolution of Gv Fit Signal integral during irradiation");
    g_Fit_Signal->GetXaxis()->SetTitle("Time [s]");
    g_Fit_Signal->GetYaxis()->SetTitle("Gray Value integrated");
    g_Fit_Signal->SaveAs("Degradation_ZnS-1mm_Fit_Signal_bis.root", "create");

    new TCanvas;
    TGraphErrors *g_fit = new TGraphErrors(temp, Time, Integral_Fit, E_Time, E_Integral_Fit);
    g_fit->Draw("");
    g_fit->SetLineColor(kBlack);
    g_fit->SetLineWidth(3);
    g_fit->SetMarkerColor(kBlack);
    g_fit->SetName("Evolution of Gv fit function integral during irradiation");
    g_fit->GetXaxis()->SetTitle("Time [s]");
    g_fit->GetYaxis()->SetTitle("Gray Value integrated");
    g_fit->SaveAs("Degradation_ZnS-1mm_fit_bis.root", "create");

    new TCanvas;
    TGraphErrors *g_sigmaX = new TGraphErrors(temp, Time, Sigma_X, E_Time, E_Sigma_X);
    g_sigmaX->Draw("");
    g_sigmaX->SetLineColor(kBlack);
    g_sigmaX->SetLineWidth(3);
    g_sigmaX->SetMarkerColor(kBlack);
    g_sigmaX->SetName("Evolution of #sigma X during irradiation");
    g_sigmaX->GetXaxis()->SetTitle("Time [s]");
    g_sigmaX->GetYaxis()->SetTitle("Sigma X [pixels]");
    g_sigmaX->SaveAs("Degradation_ZnS-1mm_sigmaX_bis.root", "create");

    new TCanvas;
    TGraphErrors *g_sigmaY = new TGraphErrors(temp, Time, Sigma_Y, E_Time, E_Sigma_Y);
    g_sigmaY->Draw("");
    g_sigmaY->SetLineColor(kBlack);
    g_sigmaY->SetLineWidth(3);
    g_sigmaY->SetMarkerColor(kBlack);
    g_sigmaY->SetName("Evolution of #sigma Y during irradiation");
    g_sigmaY->GetXaxis()->SetTitle("Time [s]");
    g_sigmaY->GetYaxis()->SetTitle("Sigma Y [pixels]");
    g_sigmaY->SaveAs("Degradation_ZnS-1mm_sigmaY_bis.root", "create");

    new TCanvas;
    TGraph *g_Chi = new TGraph(temp, Time, Chi2ndf);
    g_Chi->Draw("");
    g_Chi->SetLineColor(kBlack);
    g_Chi->SetLineWidth(3);
    g_Chi->SetMarkerColor(kBlack);
    g_Chi->SetName("Evolution of #Chi^{2} during irradiation");
    g_Chi->GetXaxis()->SetTitle("Time [s]");
    g_Chi->GetYaxis()->SetTitle("Chi2/Ndf Value");
    g_Chi->SaveAs("Degradation_ZnS-1mm_Chi2_bis.root", "create");
}
