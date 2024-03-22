void Plot_Degradation()
{
    TFile *file1 = new TFile("Degradation_ZnS-1mm_Fit_Signal_bis.root");
    TFile *file2 = new TFile("Degradation_ZnS-1mm_sigmaY_bis.root");

    TGraphErrors *a = (TGraphErrors *)file1->Get("Evolution of Gv Fit Signal integral during irradiation");
    TGraphErrors *b = (TGraphErrors *)file2->Get("Evolution of #sigma Y during irradiation");
    

    cout << a->GetN() << endl;

    // for (int i = 1; i < a->GetN(); i++)
    // {
    //     //cout << "i = " << i << endl;
    //     // if (a->GetErrorY(i) >1000) a->RemovePoint(i);
    //     //cout << a->GetPointY(i) << endl;
    //     if (a->GetPointY(i) < 5000000)
    //     {
    //         //cout << "REMOVE " << a->GetPointY(i) << endl;
    //         a->RemovePoint(i);
            
    //     }
        
    // }

    cout << a->GetN() << endl;
    a->Draw("AP");
    //b->Draw("Psame");
    b->SetMarkerColor(kRed);
}
