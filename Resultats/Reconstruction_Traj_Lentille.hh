//float pitch = 6.5e-3; //ORCA
float pitch = 20e-3; //CMOS
//float pitch = 50e-3; //IP
float PosX = 30;
float PosY = 20;
int NbinsX=PosX/pitch;
int NbinsY=PosY/pitch;

float mean, sigma, Resolution, Chi2, Ndf, Chi2Ndf, Eval1, Eval2, MeanEval, Eval_Energy, NEvents =0;
TF1* fit_gaus_Energie = new TF1("fit_gaus_Energie", "gaus", 0, 150);
TF1*fitX;
TF1*fitX1;
TCanvas* c1;
TGraphErrors* Nph;
TGraphErrors* gResolution;



TH2F* Histo_Reconstruction_2D(const char* filename, const char* name, float d)
{
  
  TH2F* h = new TH2F(name, name, NbinsX, -PosX/2, PosX/2, NbinsY, -PosY/2, PosY/2);
  TFile* file = new TFile(filename);
  std::vector<float>* x=0;
  std::vector<float>* y=0;
  std::vector<float>* px=0;
  std::vector<float>* py=0;
  float xnew, ynew=0;
  TTree *Tree = (TTree*)file->Get("Optical");
  Tree->SetBranchAddress("DetectorPositionX", &x);
  Tree->SetBranchAddress("DetectorPositionY", &y);
  Tree->SetBranchAddress("MomentumX", &px);
  Tree->SetBranchAddress("MomentumY", &py);
  const int Entries = Tree->GetEntries();
  
  
  for(int i=0; i<Entries; i++)
  //for(int i=0; i<1; i++)
    {
      x->clear();
      y->clear();
      px->clear();
      py->clear();
      
      Tree->GetEntry(i);
      
      for(int j=0; j<(x->size()); j++)
	{	  
	  xnew = x->at(j) - px->at(j)*d;
	  ynew = y->at(j) - py->at(j)*d;
	  // cout << "\nx[" << j << "] = " << x->at(j) << endl;
	  // cout << "y[" << j << "] = " << y->at(j) << endl;
	  // cout << "px[" << j << "] = " << px->at(j) << endl;
	  // cout << "py[" << j << "] = " << py->at(j) << endl;
	  // cout << "xnew[" << j << "] = " << xnew << endl;
	  // cout << "ynew[" << j << "] = " << ynew << endl;
	  h->Fill(xnew, ynew);
	}
    }

  file->Close();

  return h;

}



TH2F* Histo_Reconstruction_Energie_Position(const char* filename, const char* name, float d)
{
  TH2F* h = new TH2F(name, name, NbinsX, -PosX/2, PosX/2, 2000, 0, 200);
  TFile* file = new TFile(filename);
  std::vector<float>* x=0;
  std::vector<float>* px=0;
  float xnew=0;
  float E=0;
  TTree *Tree = (TTree*)file->Get("Optical");
  Tree->SetBranchAddress("DetectorPositionX", &x);
  Tree->SetBranchAddress("MomentumX", &px);
  Tree->SetBranchAddress("IncidentE", &E);
  const int Entries = Tree->GetEntries();

  
  for(int i=0; i<Entries; i++)
    {
      x->clear();
      px->clear();
      
      Tree->GetEntry(i);
      
      for(int j=0; j<(x->size()); j++)
	{	  
	  xnew = x->at(j) - px->at(j)*d;
	  h->Fill(xnew,E/1000);
	}
    }

  file->Close();

  return h;

}


TH1F* Histo_Reconstruction_Estart(const char* filename)
{
  TFile* file = new TFile(filename);
  TTree *Tree = (TTree*)file->Get("Optical");
  float E=0;
  Tree->SetBranchAddress("IncidentE", &E);
  const int Entries = Tree->GetEntries();
  TH1F* h = new TH1F("E_start", "E_start", 2000, 0, 200);


  for(int i=0; i<Entries; i++)
  {
    Tree->GetEntry(i);
    h->Fill(E/1000);
  }

  return h;
  
}



void Draw_ProjY_Test(TH2F* EPos, TH1D* ProjY,  Int_t bin)
{
  EPos->ProjectionY("ProjY", bin, bin, "");

  new TCanvas;
  ProjY->Draw();
  ProjY->Fit(fit_gaus_Energie, "R");
  mean = fit_gaus_Energie->GetParameter(1);
  sigma = fit_gaus_Energie->GetParameter(2);
  Resolution = sigma/mean;
  Chi2 = fit_gaus_Energie->GetChisquare();
  Ndf = fit_gaus_Energie->GetNDF();
  Chi2Ndf = Chi2/Ndf;

  cout << "Mean = " << mean << endl;
  cout << "Sigma = " << sigma << endl;
  cout << "Resolution = " << Resolution << endl;
  cout << "Chi2/Ndf = " << Chi2Ndf << endl;
}



void Draw_ProjX_Test(TH2F* EPos, TH1D* ProjX,  TH1F* Estart, Int_t bin)
{
  EPos->ProjectionX("ProjX", bin, bin, "");
  fitX = new TF1("fitX", "gaus", 0, ProjX->GetMean(1) + 2*ProjX->GetRMS(1));
  fitX->SetParameter(1, ProjX->GetMean(1));
  fitX->SetParameter(2, ProjX->GetRMS(1));
  
  new TCanvas;
  ProjX->Draw();
  ProjX->Fit(fitX, "QR");
  fitX1 = new TF1("fitX1", "gaus", fitX->GetParameter(1)-3*fitX->GetParameter(2), fitX->GetParameter(1)+0.5*fitX->GetParameter(2));
  ProjX->Fit(fitX1, "R");

  Eval_Energy = fitX1->Eval(fitX1->GetParameter(1))/2;
  NEvents = Estart->Integral(bin, bin);
  cout << "Eval = " << Eval_Energy << endl;
  cout << "Nevents = " << NEvents << endl;
  cout << "Nph = " << Eval_Energy/NEvents << endl;
  cout << "GetMean = " << ProjX->GetMean(1) << endl;;
}




TH2F *Histo_Parabole_Optique(const char* filename, const char* name)
{
  TFile* file = new TFile(filename);
  TTree *Tree = (TTree*)file->Get("Optical");
  std::vector<float>* x=0;
  std::vector<float>* y=0;
  Tree->SetBranchAddress("DetectorPositionX", &x);
  Tree->SetBranchAddress("DetectorPositionY", &y);
  //Tree->SetBranchAddress("PositionX", &x);
  //Tree->SetBranchAddress("PositionY", &y);
  const int Entries = Tree->GetEntries();
  TH2F* h = new TH2F(name, name, NbinsX, -PosX/2, PosX/2, NbinsY, -PosY/2, PosY/2);
  //TH2F* h = new TH2F(name, name, 1000, -20, 120, 1000, -20, 120);

  for(int i=0; i<Entries; i++)
  {
    x->clear();
    y->clear();
    Tree->GetEntry(i);
    for(int j=0; j<(x->size()); j++)
      {
	h->Fill(x->at(j),y->at(j));
      }
  }

  //file->Close();
  
  return h;
}



void Draw_Results(TH1F* Estart, TH2F* Pos, TH2F* EPos, TGraphErrors* gResolution, TGraphErrors* Nph)
{

  c1 = new TCanvas("RESULTS", "RESULTS", 0, 0, 3500, 1800);
  TPad *pad1 = new TPad("pad1", "", 0, 0.66, 0.31, 1);
  TPad *pad2 = new TPad("pad2", "", 0.33, 0.66, 0.64, 1);
  TPad *pad3 = new TPad("pad3", "", 0.66, 0.66, 0.99, 1);
  TPad *pad4 = new TPad("pad4", "", 0., 0., 0.49, 0.65);
  TPad *pad5 = new TPad("pad5", "", 0.51, 0., 0.99, 0.65);
  //TPad *pad6 = new TPad("pad6", "", 0.31, 0., 0.6, 0.24);

  pad1->Draw();
  pad1->SetGridx();
  pad1->SetGridy();
  pad2->Draw();
  pad2->SetLogz();
  pad3->Draw();
  pad3->SetLogz();
  pad4->Draw();
  pad4->SetGridx();
  pad4->SetGridy();
  pad5->Draw();
  pad5->SetLogy();
  pad5->SetGridx();
  pad5->SetGridy();
  //pad6->Draw();


  pad1->cd();
  Estart->Draw();
  Estart->SetTitle("");
  Estart->GetXaxis()->SetRangeUser(0, 120);
  Estart->GetXaxis()->SetTitle("Incident Energy [MeV]");
  Estart->GetYaxis()->SetTitle("dN/dE");

  pad2->cd();
  Pos->Draw("colz");
  Pos->SetTitle("");
  Pos->GetXaxis()->SetTitle("X Position [mm]");
  Pos->GetYaxis()->SetTitle("Y Position [mm]");

  pad3->cd();
  EPos->Draw("colz");
  EPos->SetTitle("");
  EPos->GetYaxis()->SetRangeUser(0, 120);
  EPos->GetXaxis()->SetTitle("X Position [mm]");
  EPos->GetYaxis()->SetTitle("Incident Energy [MeV]");

  pad4->cd();
  gResolution->Draw("APE2");
  gResolution->SetTitle("");
  gResolution->GetYaxis()->SetLimits(0, 0.5);
  gResolution->GetYaxis()->SetRangeUser(0, 0.5);
  gResolution->GetXaxis()->SetLimits(0, 100);
  gResolution->GetXaxis()->SetTitle("Incident Energy [MeV]");
  gResolution->GetYaxis()->SetTitle("Energy Resolution");
  gResolution->GetYaxis()->SetDecimals(2);
  gResolution->SetFillColor(kRed);
  gResolution->SetFillStyle(3002);
  gResolution->SetLineColor(kRed);
  gResolution->SetMarkerColor(kRed);
  gResolution->SetLineWidth(2);

  pad5->cd();
  Nph->Draw("APE2");
  Nph->SetTitle("");
  Nph->GetYaxis()->SetRangeUser(0.01, 10);
  Nph->GetXaxis()->SetRangeUser(0, 100);
  Nph->GetXaxis()->SetTitle("Incident Energy [MeV]");
  Nph->GetYaxis()->SetTitle("Nph [photons/pixel/particles_{detected}]");
  Nph->SetFillColor(kRed);
  Nph->SetMarkerColor(kRed);
  Nph->SetFillStyle(3001);
  Nph->SetLineColor(kRed);
  Nph->SetLineWidth(3);


}



void Save_Results(string filename_out, string filename_out_Resolution, string filename_out_Nph)
{

  c1->SaveAs(filename_out.c_str());
  
  TFile file_out_Nph(filename_out_Nph.c_str(), "RECREATE");
  Nph->Write();
  file_out_Nph.Close();

  TFile file_out_Resolution(filename_out_Resolution.c_str(), "RECREATE");
  gResolution->Write();
  file_out_Resolution.Close();


}
