#include "Riostream.h"

void QC3_Gas_Leak_Test()
{
//////////////////// Define the Canvas /////////////////
  gROOT->Reset();
  c1 = new TCanvas("c1", "Accumulated Charge vs. Time", 510, 510, 1000, 1000);

 TString dir = gSystem->UnixPathName(__FILE__);//boh, don't touch(?)
  //dir.ReplaceAll("test.cpp","");//boh, don't touch(?)
  // dir.ReplaceAll("/./","/");//boh, don't touch(?)
  const Int_t n=60;//number of points
  Double_t x[n];//in col2
  Double_t y[n];//in col1
  Double_t ey[n];
  Double_t ex[n];
  Double_t temp[n];
  Double_t f = 3600.0;
  //in contain the values of x and y
  ifstream fiume;
fstream in_err;
  fiume.open("QC3_GE11-X-S-CERN-0001.txt");//insert name of the file
  Int_t i=0;
  //This read the values contained in flux.txt
  for(int i=0; i<n; i++)// giro sulle righe
    {
      
      fiume  >> temp[i]  >> y[i];

      x[i] = temp[i] / f;

      
    ex[i]=1/f;
    ey[i]=0.05;
    cout <<" "<<i<<" "<< "x: " << x[i] << " - y: " << y[i] << "ex: " << ex[i] << " - ey: " << ey[i] << endl;

    if (!fiume.good())
      {
      break;
      }
    }
  fiume.close();



  
  //////////////////// Define the TGraphErrors /////////////////
  gr = new TGraphErrors(n,x,y,ex,ey);

  //////////////////// Sigmoid Curve Fit /////////////////
  float a,b, a_err, b_err;
  //TF1 *f1 = new TF1("f1", "TMath::Exp([0]-(x/[1]))", 0, 2);
  TF1 *f1 = new TF1("f1", "[0]*(TMath::Exp(-(x/[1])))", 0, 2);
  f1->SetLineColor(2);
   
  f1->SetParameter(0,20);
  f1->SetParameter(1,100);
  
  f1->SetParName(0, "P_{0} (mbar)");
  f1->SetParName(1, "#tau (hr)");

  gr->Fit(f1, "r", "");
  gStyle->SetOptFit();
  
  a = f1->GetParameter(0);
  b = f1->GetParameter(1);
  a_err = f1->GetParError(0);
  b_err = f1->GetParError(1);
  
  //cout << "parameter a = " << a << endl;
  //cout << "parameter b = " << b << endl;
  // cout << "error parameter a = " << a_err << endl;
  // cout << "error parameter b = " << b_err << endl;
 
  
   //////////////////// Define Residual Plot /////////////////
   
  Double_t x1[n], y1[n];
  Int_t n = 60;
  for (Int_t i=0;i<n;i++){
    x1[i] = x[i];
    y1[i] = y[i]-f1->Eval(x[i]);

    Double_t f1_err[n], ex1[n], ey1[n]; 
    
   f1_err[i] = (f1->Eval(x[i])) * sqrt(((a_err/a)*(a_err/a)) + ((b_err/(b*b))*(b_err/(b*b))));

   ex1[i] = .0;
   ey1[i] = sqrt((ey[i]*ey[i]) + (f1_err[i]*f1_err[i]));
    
    // cout << "fit error = " << f1_err << endl;

    //cout << "fit = " << ey1  << endl;
  
    
  }

  TGraphErrors* gr1 = new TGraphErrors(n,x1,y1,ex1,ey1);
  //TGraph* gr1 = new TGraph(n,x1,y1);
 
  
   //////////////////// Upper plot will be in pad1  ////////////////////
   
   //TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);

TPad *pad1 = new TPad("pad1", "pad1", 0.05,0.36,0.95,1);
//pad1->SetTopMargin(1);
   pad1->SetBottomMargin(0.02); // Upper and lower plot are joined
   pad1->SetGridx();         // Vertical grid
   pad1->Draw();             // Draw the upper pad: pad1
   pad1->cd();               // pad1 becomes the current pad

   gr->SetMarkerColor(4);
   gr->SetMarkerStyle(21);
   gr->Draw("Ap");

   TPaveText *pt = new TPaveText(0.02378513,25.60673,0.5236178,29.59902,"br");
   //TPaveText *pt = new TPaveText(0,21,0.5,25,"br");
   //TPaveText *pt = new TPaveText(200,10000,500,1.960301,"br");
   pt->AddText("GE1/1 Detector Production (LS2)");
   pt->AddText("GE1/1-X-L-CERN-0001 Detector");
   pt->AddText("Gas: CO_{2}");
   pt->AddText("P_{m}( t ) = P_{0} #times  exp( - t / #tau )");
   
   pt->SetBorderSize(0);
   pt->SetLineColor(1);
   pt->SetLineStyle(1);
   pt->SetLineWidth(1);
   pt->SetFillColor(1);
   pt->SetFillStyle(0);
   pt->SetTextSize(0.04);
   pt->SetTextFont(42);
   pt->SetTextAlign(11);
   pt->Draw();   
   
   TGaxis *axis = new TGaxis( -5, 20, -5, 220, 20,220,510,"");
   axis->SetLabelFont(43); // Absolute font size in pixel (precision 3)
   axis->SetLabelSize(15);
   axis->Draw();

   TLegend *leg = new TLegend(0.1374386,0.05030801,0.3873271,0.2444174,NULL,"brNDC");
   //TLegend *leg = new TLegend(0.1374386,0.04549538,0.5178492,0.2460216,NULL,"brNDC");
   //TLegend *leg = new TLegend(0.1636546,0.7022587,0.5441767,0.9024641,NULL,"brNDC");
   leg->SetBorderSize(0);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(0);
  
   TLegendEntry *entry=leg->AddEntry("gr","Data","lep");
   entry->SetLineColor(4);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(4);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry->SetTextFont(52);
   entry->SetTextSize(.03);
   entry->SetTextColor(1);

   
   entry=leg->AddEntry("f1","Curve Fitting","l");
   entry->SetLineColor(2);
   entry->SetLineStyle(1);
   entry->SetLineWidth(2);
   entry->SetMarkerColor(2);
   entry->SetMarkerStyle(4);
   entry->SetMarkerSize(1);
   entry->SetTextFont(52);
   entry->SetTextSize(.03);
   entry->SetTextColor(1);

   leg->Draw();
   
   
   //////////////////// lower plot will be in pad  ////////////////////
   
   c1->cd();          // Go back to the main canvas before defining pad2

   //TPad *pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.3);

 TPad *pad2 = new TPad("pad2", "pad2", 0.05,0.05,0.95,0.36);
   
   pad2->SetTopMargin(1);
   pad2->SetBottomMargin(0.2);
   pad2->SetGridx(); // vertical grid
   pad2->Draw();
   pad2->cd();       // pad2 becomes the current pad
   //h2->SetStats(0);          // No statistics on upper plot
   //h2->Draw();               // Draw h1

 
    gr1->SetMarkerColor(4);
    gr1->SetMarkerStyle(21);
    gr1->Draw("Ap");

    TLine *line2 = new TLine(0,0,1.03,0);
    line2->SetLineWidth(2);
    line2->SetLineColor(kRed);
    line2->SetLineStyle(2);
    line2->Draw();
  
    c1->cd(0);
    line2->Draw();
    

   //////////////////// gr Settings /////////////////

    gr->SetTitle(" ");
    
    gr->GetXaxis()->SetLabelSize(0.);
    /*
    gr->GetXaxis()->SetTitle("HV_{Eff} (V)");
    gr->GetXaxis()->SetLabelFont(42);
    gr->GetXaxis()->SetLabelSize(0.038);
    gr->GetXaxis()->SetTitleSize(0.042);
    gr->GetXaxis()->SetTitleOffset(1.0);
    gr->GetXaxis()->SetTitleFont(42);
    */    
    // gr->GetXaxis()->SetRangeUser(6300,7900);

    gr->GetXaxis()->SetLimits(0,1.03);
        
    gr->GetYaxis()->SetTitle("Manifold Pressure P_{m} (mbar)");
    gr->GetYaxis()->SetLabelFont(42);
    gr->GetYaxis()->SetLabelSize(0.038);
    gr->GetYaxis()->SetTitleSize(0.042);
    gr->GetYaxis()->SetTitleOffset(1.0);
    gr->GetYaxis()->SetTitleFont(42);
    gr->GetYaxis()->SetDecimals();

    gr->GetYaxis()->SetRangeUser(15,30);


 
 //////////////////// gr1 Settings /////////////////

    gr1->SetTitle(" ");
  
    gr1->GetXaxis()->SetTitle("Time t (hr)");
    gr1->GetXaxis()->SetLabelFont(42);
    gr1->GetXaxis()->SetLabelSize(0.078);
    gr1->GetXaxis()->SetTitleSize(0.085);
    gr1->GetXaxis()->SetTitleOffset(1.1);
    gr1->GetXaxis()->SetTitleFont(42);
    //gr1->GetXaxis()->SetRangeUser(6500,7700);

    gr1->GetXaxis()->SetLimits(0,1.03);
    
    gr1->GetYaxis()->SetLabelFont(42);
    gr1->GetYaxis()->SetLabelSize(0.078);
    gr1->GetYaxis()->SetTitle("Residual (a.u.)");
    gr1->GetYaxis()->SetTitleSize(0.085);
    gr1->GetYaxis()->SetTitleOffset(0.50);
    gr1->GetYaxis()->SetTitleFont(42);
    gr1->GetYaxis()->SetNdivisions(506);

    gr1->GetYaxis()->SetDecimals(3);
    gr1->GetYaxis()->SetRangeUser(-1,1);


    gStyle->SetGridStyle(2);
    TGaxis::SetMaxDigits(4);
    
    c1->Update();

    gSystem->ProcessEvents();
    TImage *img = TImage::Create();
  //img->FromPad(c, 10, 10, 300, 200);
    img->FromPad(c1);
    img->WriteImage("QC3_Gas_Leak_Test.png");
   //delete mg;
   //delete c2;
   //delete img;  


}
