#include "Reconstruction_Traj_Lentille.hh"

void Reconstruction_Traj_Lentille()
{
  gStyle->SetOptStat(0);

  float x[NbinsX], ex[NbinsX], y[NbinsX], ey[NbinsX];
  float x_new[1500], y_new[1500], ex_new[1500], ey_new[1500];
  float xE[NbinsX], exE[NbinsX], yE[NbinsX], eyE[NbinsX];
  float xNph[NbinsX], exNph[NbinsX], yNph[NbinsX], eyNph[NbinsX];

  float distance = 7.44;
  string filename = "ZnS_1mm_2.5MeV_SCALED.root";
  //string filename = "test.root";
  Rebin=2;
  float Gain =0.44;
  float NEvent =1000000;

  // cout << "\nStart of 2D Histogram reconstruction !!!" << endl;
  // Pos = Histo_Reconstruction_2D(filename.c_str(), "2D", distance);
  // cout << "End of 2D Histogram reconstruction !!!" << endl;

  // float a = GetNEvent(filename.c_str());

  // Pos->Draw("colz");

  // Pos->Scale(1/a);

  // Pos->SaveAs("ZnS_1mm_2.5MeV_SCALED.root");

  // TH1D* proj = new TH1D("proj", "proj", 2048, 0, 2048);
  // Pos->ProjectionX("proj", 1024, 1024, "");
  // proj->Draw();

  // proj->SaveAs("EJ262_1mm_2.5MeV_SCALED_PROJX.root");


  
  Signal_Analyse(filename, NEvent, Gain);
  Draw_Results(Rebin);


    
}
