//********************************************************************************************************
// RooFit program to fit f2' invariant mass 
//
//     details of file specific: copied the file fit_dikaon_mass_f2prime_v2.cpp and editing is going 
//      date : 28 November 2018 v2
//*******************************************************************************************************
#include <TLatex.h>
#include <TString.h>
#include <TLegend.h>

void fit_dikaon_mass_f2prime_v2(){


  gSystem->Load("libRooFit");
  using namespace RooFit;

  TChain *ch = new TChain("tree");
  ch->Add("../sel_Bs2f2pmm_2016*root");
  TTree *tr = ch;

  int nentries_ = tr->GetEntries();
  cout << "\n=> total entries in the 13 TeV data tree = " << nentries_ << endl;

  ///RooRealVar  Bmass("Bmass","m(B_{s}) (GeV/c^{2})", 5.0, 5.7);
  RooRealVar Bmass("Bmass","#it{m(KK#mu#mu) (GeV/c^{2})}", 5.1, 5.7);
  RooRealVar Mumumass("Mumumass","#it{m(#mu#mu) (GeV/c^{2})}", 2.95, 3.3);
  RooRealVar Mumumasserr("Mumumasserr","Error of M^{#mu#mu}", 0., 10.);
  //RooRealVar Phimass("Phimass","#it{m(KK) (GeV/c^{2})}", 1.04, 1.2);
  //RooRealVar Phimass("Phimass","#it{m(KK) (GeV/c^{2})}", 1., 2.1);
  RooRealVar Phimass("Phimass","#it{m(KK) (GeV/c^{2})}", 1.3, 1.8);
  RooRealVar Kmpt("Kmpt","K- pt", 0., 200.);
  RooRealVar Kppt("Kppt","K+ pt", 0., 200.);
  RooRealVar Kmtrkdcasigbs("Kmtrkdcasigbs","K- trk DCA/sigma", 0., 1000.);
  RooRealVar Kptrkdcasigbs("Kptrkdcasigbs","K+ trk DCA/sigma", 0., 1000.);
  RooRealVar Bpt("Bpt","Bs pt", 0., 500.);
  RooRealVar Bvtxcl("Bvtxcl","B vtx cl.", 0., 1.);
  RooRealVar Bctau("Bctau","B ctau", -10., 10.);
  RooRealVar Bcosalphabs2d("Bcosalphabs2d","Bcosalphabs2d", 0., 1.);
  RooRealVar Blxysig("Blxysig","Blxy sig.", 0., 1000.);
  ////  RooRealVar Q2("Q2","dimuon inv mass squared", 0.5, 20.);
  RooRealVar Triggers("Triggers","",0,1);


  RooArgSet vars(Bmass,Mumumass,Phimass,Kmpt,Kppt,Kmtrkdcasigbs, Kptrkdcasigbs);
  vars.add(RooArgSet(Blxysig,Triggers,Mumumasserr,Bpt,Bvtxcl,Bctau,Bcosalphabs2d));

  ////RooDataSet data("data","dataset with Bmass", ch, RooArgSet(Q2, Bmass, Mumumass, Mumumasserr, Triggers));  
  RooDataSet data("data","dataset with Bmass", ch, vars);  


  ////TString decayMode[2] = {"bs2jpsiphi","bs2jpsif2p"};

  
  TCut c1 = "Bmass > 5.1 && Bmass < 5.7";
  //TCut c2 = "( Mumumass > (3.096-3*Mumumasserr) && Mumumass < 3.096+3*Mumumasserr )";
  TCut c2 = "Mumumass > 2.95 && Mumumass < 3.3";
  ////TCut c3 = "( Mumumass < (3.686 - 3.5*Mumumasserr) || Mumumass > (3.686 + 3.5*Mumumasserr) )";
  TCut c3 = "Phimass>1.3 && Phimass<1.8";
  /////if (decayMode=decayMode[1]) TCut c3 = "Phimass>1.4 && Phimass<1.7";
  ///if (decayMode=decayMode[0]) TCut c3 = "Phimass>1.01 && Phimass<1.03";
  //TCut c3 = "Phimass>1.01 && Phimass<1.03";

  TCut c4 = "Kmpt> 0.8 && Kppt> 0.8";
  TCut c5 = "Kmtrkdcasigbs > 0.8 && Kptrkdcasigbs > 0.8";
  TCut c6 = "Bpt>10";
  TCut c7 = "Bvtxcl>0.02";
  TCut c8 = "abs(Bctau)>0.01";
  TCut c9 = "Bcosalphabs2d>0.9987";
  TCut c10 = "Blxysig>8.5";
  TCut c11 = "Triggers==1";

  TCut cutTotal = c1 && c2 && c3 && c4 && c5 && c6 && c7 && c8 && c9 && c10 && c11;

  RooDataSet *redData = (RooDataSet*)data.reduce(cutTotal);
  ///RooDataSet *redData = (RooDataSet*)data.reduce(c1&&c2);
 

  RooRealVar  mean("mean","mean of the gaussians", 1.525, 1.4, 1.7);
  //RooRealVar  mean("mean","mean of the gaussians", 1.0, 2.1);
  RooRealVar  sigma("sigma","sigma of the gaussian", 0.0, 1.);
  //RooRealVar  sigma2("sigma2","sigma of the gaussian2", 0.2, 0., 1.);
  RooRealVar  width("width","width", 0.05, 0., 2.);
  //RooGaussian  sigG("sigG","signal component-1", Phimass, mean, sigma);
  RooVoigtian  sigG("sigG","signal component-1", Phimass, mean, width, sigma);
  //RooBreitWigner  sigG("sigG","signal component-1", Phimass, mean, sigma);
  //RooGaussian  sigG2("sigG2","signal component-2", Phimass, mean, sigma2);
  //RooRealVar sigM_frac("sigM_frac","fraction of Gaussians", 0.8, 0., 1.);
  //RooAddPdf sigG("sigG","sigG", RooArgList(sigG1,sigG2), RooArgList(sigM_frac) );
  
  //RooRealVar lambda("lambda","slope", -1.5, -20., 0.);
  //RooExponential bkgE("bkgE","exponential PDF", Phimass, lambda);
  //RooExponential bkgC("bkgC","exponential PDF", Phimass, lambda);

  /*RooRealVar a0("a0", "constant", -10., 10.);
  RooRealVar a1("a1", "linear", -10., 10.);
  RooRealVar a2("a2", "quadratic", 10., 10.);*/
  RooRealVar a0("a0", "constant", -.52);//, 10.);
  RooRealVar a1("a1", "linear", -.01);//, 1.);
  RooRealVar a2("a2", "quadratic", -.028);//, 2.);
  RooChebychev bkgC("bkgC", "Background", Phimass, RooArgSet(a0, a1, a2));
  //RooChebychev bkgC("bkgC", "Background", Phimass, RooArgSet(a0, a1));
  //RooRealVar a0("a0", "constant", 10000);//, 1000, 1.e5);
  //RooRealVar a1("a1", "linear", -5000);//, -1.e5, -1.);
  //RooPolynomial bkgC("bkgC", "Background", Phimass, RooArgSet(a0, a1));
  //RooPolynomial bkgC("bkgC", "Background", Phimass, RooArgSet(a0));

  RooRealVar nsigG("nsigG","signal events", 2000, 0, 1E7);
  //RooRealVar nbkgE("nbkgE","number of exponential bkg events", 8000, 0, 1E7);
  RooRealVar nbkgC("nbkgC","number of chebychev pol. bkg events", 8000, 0, 1E7);
  //RooRealVar nbkgG1("nbkgG1","number of gaussian1 bkg events", 1000, 0, 1E7);
  //RooRealVar nbkgG2("nbkgG2","number of gaussian2 bkg events", 2000, 0, 1E7);
  
  //RooAddPdf model("model","g+e+g+g", RooArgList(sigG,bkgExpo,bkgGau1,bkgGau2), RooArgList(nsigG,nbkgE,nbkgG1,nbkgG2) );
  //RooAddPdf model("model","g+e+g+g", RooArgList(sigG,bkgC,bkgGau1,bkgGau2), RooArgList(nsigG,nbkgC,nbkgG1,nbkgG2) );
  RooAddPdf model("model","g+c", RooArgList(sigG,bkgC), RooArgList(nsigG,nbkgC) );
  //RooAddPdf model("model","g+e", RooArgList(sigG,bkgE), RooArgList(nsigG,nbkgE) );
  model.Print("t");
  RooFitResult* fitres = model.fitTo(*redData, Extended(true), Save(true), Minos(true) );

  
  RooPlot *xframe = Phimass.frame(Title("KK invariant mass"),Bins(30) ); //50
  redData->plotOn(xframe,RooFit::Name("data"));
  model.plotOn(xframe,RooFit::Name("pdf"));
  double chi2_new = xframe->chiSquare();
  xframe->SetTitle("");
  xframe->SetMinimum(0);


  //data.plotOn(xframe);
  //redData->statOn(xframe);
  //data.statOn(xframe,Layout(0.55,0.99,0.8));

  //data.plotOn(xframe, Cut("Bmass > 5 && Bmass < 5.56"));
  //model.plotOn(xframe);
  //model.plotOn(xframe, RooFit::Name("sig"), Components("sigG"), LineStyle(kDashed), LineColor(kRed), FillColor(kGreen), FillStyle(3008), DrawOption("F") );
  model.plotOn(xframe, RooFit::Name("sig"), Components("sigG"), FillStyle(3008), FillColor(kGreen), DrawOption("F") );
  model.plotOn(xframe, RooFit::Name("bkg"), Components("bkgC"), LineStyle(kDotted), LineColor(kRed) );
  //model.plotOn(xframe, Components("bkgE"), LineStyle(kDotted), LineColor(kRed) );
  //model.plotOn(xframe, RooFit::Name("bkg"), Components("bkgE"), LineStyle(kDotted), LineColor(kRed) );


  TCanvas *c = new TCanvas("c","c", 600,600);
  gPad->SetLeftMargin(0.15);
  xframe->GetYaxis()->SetTitleOffset(1.7);
  xframe->Draw();

  
  TLegend *leg =new TLegend(0.22,0.40,0.40,0.58,NULL,"brNDC");
  leg->AddEntry("data"," Data "," PE ");
  leg->AddEntry("pdf"," fit "," L ");
  leg->AddEntry("sig"," Signal "," F ");
  leg->AddEntry("bkg"," Background "," L");
  leg->SetLineColor(0);
  leg->SetFillColor(0);
  leg->SetTextSize(0.03);
  leg->Draw();

  TPaveText* paveText = new TPaveText(0.62,0.70,0.80,0.88,"NDC");
  paveText->SetBorderSize(0.0);
  paveText->SetFillColor(kWhite);
  paveText->SetFillStyle(0);
  paveText->SetTextSize(0.02);
  paveText->AddText(Form("Sig. Yield  = %.0f #pm %.0f", nsigG.getVal(), nsigG.getError()));
  paveText->AddText(Form("Bkg. Yield  = %.0f #pm %.0f evts", nbkgC.getVal(), nbkgC.getError()));
  //paveText->AddText(Form("Bkg. Yield  = %.0f #pm %.0f", nbkgE.getVal(), nbkgE.getError()));
  paveText->AddText(Form("mean  = %.4f #pm %.4f GeV/c^{2}" , mean.getVal() , mean.getError()));
  paveText->AddText(Form("sigma = %.4f #pm %.4f GeV/c^{2}" , sigma.getVal(), sigma.getError()));
  paveText->AddText(Form("width = %.4f #pm %.4f GeV/c^{2}" , width.getVal(), width.getError()));
  paveText->AddText(Form("a_{0} = %.4f #pm %.4f GeV/c^{2}" , a0.getVal(), a0.getError()));
  paveText->AddText(Form("a_{1} = %.4f #pm %.4f GeV/c^{2}" , a1.getVal(), a1.getError()));
  paveText->AddText(Form("a_{2} = %.4f #pm %.4f GeV/c^{2}" , a2.getVal(), a2.getError()));
  paveText->AddText(Form("#chi^{2}/dof = %.2f", chi2_new));
  //paveText->AddText(Form("slope = %.5f #pm %.5f " , lambda.getVal(), lambda.getError()));                                  
  //paveText->AddText(Form("sigma1 = %.5f #pm %.5f GeV", sigma1.getVal(), sigma1.getError()));
  //paveText->AddText(Form("sigma2 = %.5f #pm %.5f GeV", sigma2.getVal(), sigma2.getError()));
  //paveText->AddText(Form("frac = %.5f #pm %.5f ", sigM_frac.getVal(), sigM_frac.getError()));


  //double s1 = sigma.getVal();
  //double s2 = sigma2.getVal();
  //double f = sigM_frac.getVal();

  //double effSig = sqrt(f*f*s1*s1 + (1-f)*(1-f)*s2*s2);
  //cout << "Sigma (total) = " << effSig << endl;
  
  if (fitres != NULL){
    if ((fitres->covQual() == 3) && (fitres->status() == 0)){
      if (paveText != NULL) paveText->AddText("Fit Status: GOOD");
    } else {
      if (paveText != NULL) paveText->AddText("Fit Status: BAD");
    }
  }

 
  paveText->Draw();


  TLatex *mark = new TLatex();
  mark->SetNDC(true);

  TString dataset[8] = {"2016B","2016C","2016D","2016E","2016F","2016G","2016H","2016B-H"} ;
  double     lumi[8] = {5.753,2.615,4.281,4.027,3.119,7.658,8.378,35.831} ;
  TString plotpath="./charmo";


  double startY = 0.92;
  mark->SetTextFont(42);
  mark->SetTextSize(0.035);
  mark->DrawLatex(0.15,startY,"#bf{CMS} #it{preliminary}");
  mark->DrawLatex(0.75,startY,"#scale[0.8]{xxx fb^{-1} (13 TeV)}");
  //////mark->DrawLatex(0.65,startY,Form("#scale[0.8]{%s, %.3f fb^{-1} (13 TeV)}", dataset[0].Data(), lumi[0]));
  mark->Draw();


  c->Update();
  //c->SaveAs("Bs2JPSiPhi_13TeV_woCDF_w_TRG_OFF_2016B.png");
  //c->SaveAs("Bs2JPSiPhi_13TeV_woCDF_w_TRG_OFF_2016B.pdf");
  //c->SaveAs("Bs2JPSiPhi_13TeV_woCDF_w_TRG_ON_2016G_RUN_278820_JPSI.png");                                                
  //c->SaveAs("Bs2JPSiPhi_13TeV_woCDF_w_TRG_ON_2016G_RUN_278820_JPSI.pdf");
  ////  c->SaveAs(TString::Format("%s/Bs2JPSiPhi_13TeV_%s_JPSI.png",plotpath.Data(),dataset[0].Data()));                       
  ////c->SaveAs(TString::Format("%s/Bs2JPSiPhi_13TeV_%s_JPSI.pdf",plotpath.Data(),dataset[0].Data()));                       
  //c->SaveAs("Bs2f2pJPSi_13tev_2016FGH.png");
  ///////////////  c->SaveAs("dikaon_[1.2-1.9]_Bs2f2pJPSi_13tev_2016FGH.pdf");
  //  c->SaveAs("F2P_13tev_2016FGH_fullDIMU_BW+LP.png");                                                                           
  //c->SaveAs("F2P_13tev_2016FGH_fullDIMU_BW+LP.pdf");   

  ///c->Update();
  ///c->SetLogy();
  ///c->SaveAs("Bs2f2pmm_13tev_2016FGH_log.pdf");
  ///c->SaveAs("Bs2f2pmm_13tev_2016FGH_log.png");
  //c->SaveAs("Bs2JPSiPhi_13TeV_woCDF_w_TRG_ON_2016B_log.png");
  //c->SaveAs("Bs2JPSiPhi_13TeV_woCDF_w_TRG_ON_2016B_log.pdf");
} 
