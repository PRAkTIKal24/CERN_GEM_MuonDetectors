//take the data from gain.txt and make the quadratic fit
//gain.txt contains columns: x,,y,ey [THE ERROR IS 0.5% ON THE READING CURRENT]
#include "Riostream.h"
Double_t fitf(Double_t *x, Double_t *par)
{
Double_t fitval =TMath::Exp(par[0]-(x[0]/par[1]));
   return fitval;
}
void fit(){//the function name has to be the same of the script

  TString dir = gSystem->UnixPathName(__FILE__);//boh, don't touch(?)
  dir.ReplaceAll("fit.C","");//boh, don't touch(?)
  dir.ReplaceAll("/./","/");//boh, don't touch(?)
  const Int_t n=60;//number of points
  Double_t x[n];//in col2
  Double_t y[n];//in col1
  Double_t ey[n];
  Double_t ex[n];
  //in contain the values of x and y
  ifstream fiume;
fstream in_err;
  fiume.open("GE11_X_S_CERN_0010.txt");//insert name of the file
  Int_t i=0;
  //This read the values contained in flux.txt
  for(int i=0; i<n; i++)// giro sulle righe
    {
    fiume  >> x[i] >> y[i] ;
    ex[i]=1;
    ey[i]=0.05;
    //cout <<" "<<i<<" "<< "x: " << x[i] << " - y: " << y[i] << "ex: " << ex[i] << " - ey: " << ey[i] << endl;

    if (!fiume.good())
      {
      break;
      }
    }
  fiume.close();


  TCanvas *screen1 = new TCanvas("screen1", "Gas Leakage Test", 0,0,800,600);
  TGraphErrors *graph = new TGraphErrors(n,x,y,ex,ey);//add eventual error bars
  TMultiGraph *mg = new TMultiGraph;
  mg -> SetTitle("Pressure drop between in gas pipe and out gas pipe; Time(s) ; Pressure(mbar); ");//title;titleX;TitleY
  mg-> Add(graph);
  screen1 -> cd();
  graph->SetMarkerStyle(7);
  graph->SetMarkerColor(1);
  mg->Draw("ap");

  //descripiot
  //TPaveText *pt = new TPaveText(0.02378513,25.60673,0.5236178,29.59902,"br");
  TPaveText *pt = new TPaveText(0,2,0,5);
  //TPaveText *pt = new TPaveText(200,10000,500,1.960301,"br");
  pt->AddText("Triple GEM Outgassing Prototype");
  pt->AddText("Gas: CO_{2}");
  pt->AddText("P_{m} = exp( A - t / #tau )+B");

  pt->SetBorderSize(0);
  pt->SetLineColor(1);
  pt->SetLineStyle(1);
  pt->SetLineWidth(1);
  pt->SetFillColor(1);
  pt->SetFillStyle(0);
  pt->SetTextSize(0.03);
  pt->SetTextFont(42);
  pt->SetTextAlign(11);
  pt->Draw();




  //range x (a,,b)
  Double_t a=60;
  Double_t b=3600;
  TF1 *f1 = new TF1("f1", fitf, a, b,2);
  f1->SetLineColor(2);
  f1->SetLineWidth(1);
  f1->SetParameter(0, 30);
  f1->SetParameter(1,22600);
  //f1->SetParameter(2,1);
  f1->SetParName(0, "A");
  f1->SetParName(1, "#tau(s)");
  //f1->SetParName(2, "Offset(mbar)");
  graph->Fit(f1, "", "", a, b);



}
