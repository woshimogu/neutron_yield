void plotHisto()
{
    //gROOT->Reset();
    // Draw histos filled by Geant4 simulation
    TFile* f = new TFile("pNsource.root");
    /*TCanvas* c1 = new TCanvas("c1", "  ");
    TCanvas* c2 = new TCanvas("c2", "  ");
    TCanvas* c3 = new TCanvas("c3", "  ");*/
    TCanvas* c4 = new TCanvas("c4", "  ");
    TCanvas* c5 = new TCanvas("c5", "  ");
    /*1->cd();
    TH1D* hist1 = (TH1D*)f->Get("h0");
    hist1->Draw("HIST");
    c2->cd();
    TH1D* hist2 = (TH1D*)f->Get("h1");
    hist2->Draw("HIST");
    c3->cd();
    TH1D* hist3 = (TH1D*)f->Get("h2");
    hist3->Draw("HIST");*/
    c4->cd();
    TH1D* hist4 = (TH1D*)f->Get("h4");
    hist4->Draw("HIST");
    c5->cd();
    TH1D* hist5 = (TH1D*)f->Get("h5");
    hist5->Draw("HIST");

}  
