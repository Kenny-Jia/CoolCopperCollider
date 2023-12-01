#include <DDG4/DDG4Dict.h>
#include <DDG4/Geant4DataDump.h> 
#include <TVector.h>
#include <TTree.h>
#include <TBranch.h>
#include <TLeaf.h>
#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TLorentzVector.h>
#include <TLatex.h>
#include <time.h>
#include <variant>

void singlebkg(TTree *tree, const char *hitname, TH1D* histo) {
    tree->ResetBranchAddresses();
    int hitcnt = 0;
    
    vector<dd4hep::sim::Geant4Calorimeter::Hit*> *hits = new vector<dd4hep::sim::Geant4Calorimeter::Hit*>; 

    tree -> SetBranchAddress(hitname, &hits);

    for (int i = 0; i < tree->GetEntries(); ++i) {
	tree -> GetEntry(i);
	for (size_t j = 0; j < hits -> size(); ++j) {
	    dd4hep::sim::Geant4Calorimeter::Hit* hit = (*hits)[j];
	    double energy = hit -> energyDeposit;
	    if (energy != 0 ){
		auto truthhits = static_cast<std::vector<dd4hep::sim::Geant4HitData::MonteCarloContrib>>(hit->truth);
		for (auto& mcContrib : truthhits) {
		    double time = mcContrib.time;
		    double deposit = mcContrib.deposit;
		    if (deposit != 0) {
			hitcnt++;
		        histo->Fill(time - 14.44);
		    }
		}
		continue;
	    }
	    
        }
    }
    std::cout << hitcnt << std::endl;
    delete hits;
}



void singlebkg_tracker(TTree *tree, const char *hitname, TH1D* histo) {
    tree->ResetBranchAddresses();
    int hitcnt = 0;
    
    vector<dd4hep::sim::Geant4Tracker::Hit*> *hits = new vector<dd4hep::sim::Geant4Tracker::Hit*>; 

    tree -> SetBranchAddress(hitname, &hits);

    for (int i = 0; i < tree->GetEntries(); ++i) {
	tree -> GetEntry(i);
	for (size_t j = 0; j < hits -> size(); ++j) {
	    dd4hep::sim::Geant4Tracker::Hit* hit = (*hits)[j];
	    double energy = hit -> energyDeposit;
	    if (energy != 0 ){
		auto truthhit = static_cast<dd4hep::sim::Geant4HitData::MonteCarloContrib>(hit->truth);
		double time = truthhit.time;
		double deposit = truthhit.deposit;
	    	if (deposit != 0) {
		    hitcnt++;
		    histo->Fill(time - 14.44);
	        }
	    }
	}
    }
    std::cout << hitcnt << std::endl;
    delete hits;
}

void singlefile(TTree *tree, TH1D *ECalBarrelHitsHisto, TH1D *ECalEndcapHitsHisto, TH1D *HCalBarrelHitsHisto, TH1D *HCalEndcapHitsHisto, TH1D *MuonBarrelHitsHisto, TH1D *MuonEndcapHitsHisto, TH1D *LumiCalHitsHisto, TH1D *BeamCalHitsHisto, TH1D *SiVertexBarrelHitsHisto, TH1D *SiVertexEndcapHitsHisto, TH1D *SiTrackerBarrelHitsHisto, TH1D *SiTrackerEndcapHitsHisto, double scale) {

    TH1D *ECalBarrelHitsHisto_tmp = new TH1D("ECalBarrelHitsHisto_tmp", "histo", 40, 0, 37.8);
    TH1D *ECalEndcapHitsHisto_tmp = new TH1D("ECalEndcapHitsHisto_tmp", "histo", 40, 0, 37.8);
    TH1D *HCalBarrelHitsHisto_tmp = new TH1D("HCalBarrelHitsHisto_tmp", "histo", 40, 0, 37.8);
    TH1D *HCalEndcapHitsHisto_tmp = new TH1D("HCalEndcapHitsHisto_tmp", "histo", 40, 0, 37.8);
    TH1D *MuonBarrelHitsHisto_tmp = new TH1D("MuonBarrelHitsHisto_tmp", "histo", 40, 0, 37.8);
    TH1D *MuonEndcapHitsHisto_tmp = new TH1D("MuonEndcapHitsHisto_tmp", "histo", 40, 0, 37.8);
    TH1D *LumiCalHitsHisto_tmp = new TH1D("LumiCalHitsHisto_tmp", "histo", 40, 0, 37.8);
    TH1D *BeamCalHitsHisto_tmp = new TH1D("BeamCalHitsHisto_tmp", "histo", 40, 0, 37.8);
    TH1D *SiVertexBarrelHitsHisto_tmp = new TH1D("SiVertexBarrelHitsHisto_tmp", "histo", 40, 0, 37.8);
    TH1D *SiVertexEndcapHitsHisto_tmp = new TH1D("SiVertexEndcapHitsHisto_tmp", "histo", 40, 0, 37.8);
    TH1D *SiTrackerBarrelHitsHisto_tmp = new TH1D("SiTrackerBarrelHitsHisto_tmp", "histo", 40, 0, 37.8);
    TH1D *SiTrackerEndcapHitsHisto_tmp = new TH1D("SiTrackerEndcapHitsHisto_tmp", "histo", 40, 0, 37.8);

    singlebkg(tree, "ECalBarrelHits", ECalBarrelHitsHisto_tmp);    
    singlebkg(tree, "ECalEndcapHits", ECalEndcapHitsHisto_tmp);    
    singlebkg(tree, "HCalBarrelHits", HCalBarrelHitsHisto_tmp);    
    singlebkg(tree, "HCalEndcapHits", HCalEndcapHitsHisto_tmp);    
    singlebkg(tree, "MuonBarrelHits", MuonBarrelHitsHisto_tmp);    
    singlebkg(tree, "MuonEndcapHits", MuonEndcapHitsHisto_tmp);    
    singlebkg(tree, "LumiCalHits", LumiCalHitsHisto_tmp);
    singlebkg(tree, "BeamCalHits", BeamCalHitsHisto_tmp);
    singlebkg_tracker(tree, "SiVertexBarrelHits", SiVertexBarrelHitsHisto_tmp);    
    singlebkg_tracker(tree, "SiVertexEndcapHits", SiVertexEndcapHitsHisto_tmp);    
    singlebkg_tracker(tree, "SiTrackerBarrelHits", SiTrackerBarrelHitsHisto_tmp);    
    singlebkg_tracker(tree, "SiTrackerEndcapHits", SiTrackerEndcapHitsHisto_tmp);    

    
    ECalBarrelHitsHisto_tmp -> Scale(scale); 
    ECalEndcapHitsHisto_tmp -> Scale(scale);
    HCalBarrelHitsHisto_tmp -> Scale(scale); 
    HCalEndcapHitsHisto_tmp -> Scale(scale);
    MuonBarrelHitsHisto_tmp -> Scale(scale);  
    MuonEndcapHitsHisto_tmp -> Scale(scale);
    LumiCalHitsHisto_tmp -> Scale(scale);
    BeamCalHitsHisto_tmp -> Scale(scale);
    SiVertexBarrelHitsHisto_tmp -> Scale(scale);
    SiVertexEndcapHitsHisto_tmp -> Scale(scale);
    SiTrackerBarrelHitsHisto_tmp -> Scale(scale);
    SiTrackerEndcapHitsHisto_tmp -> Scale(scale);


    /*
    if (scale == 0) {
	ECalBarrelHitsHisto_tmp -> Scale(0.074888); 
	ECalEndcapHitsHisto_tmp -> Scale(0.083186);
	HCalBarrelHitsHisto_tmp -> Scale(0.076266); 
	HCalEndcapHitsHisto_tmp -> Scale(0.084266);
	MuonBarrelHitsHisto_tmp -> Scale(0.081251);  
	MuonEndcapHitsHisto_tmp -> Scale(0.083438);
	LumiCalHitsHisto_tmp -> Scale(0.085353);
	BeamCalHitsHisto_tmp -> Scale(0.09877);
	SiVertexBarrelHitsHisto_tmp -> Scale(0.127273);
	SiVertexEndcapHitsHisto_tmp -> Scale(0.103387);
	SiTrackerBarrelHitsHisto_tmp -> Scale(0.069084);
	SiTrackerEndcapHitsHisto_tmp -> Scale(0.07531);
    }
    if (scale == 1) {
        ECalBarrelHitsHisto_tmp -> Scale(0.08869); 
	ECalEndcapHitsHisto_tmp -> Scale(0.09328);
	HCalBarrelHitsHisto_tmp -> Scale(0.085301); 
	HCalEndcapHitsHisto_tmp -> Scale(0.087916);
	MuonBarrelHitsHisto_tmp -> Scale(0.084786);  
	MuonEndcapHitsHisto_tmp -> Scale(0.089236);
	LumiCalHitsHisto_tmp -> Scale(0.08996);
	BeamCalHitsHisto_tmp -> Scale(0.064375);
	SiVertexBarrelHitsHisto_tmp -> Scale(0.016835);
	SiVertexEndcapHitsHisto_tmp -> Scale(0.064577);
	SiTrackerBarrelHitsHisto_tmp -> Scale(0.098367);
	SiTrackerEndcapHitsHisto_tmp -> Scale(0.089648);
    }
*/   
    ECalBarrelHitsHisto -> Add(ECalBarrelHitsHisto_tmp);
    ECalEndcapHitsHisto -> Add(ECalEndcapHitsHisto_tmp);
    HCalBarrelHitsHisto -> Add(HCalBarrelHitsHisto_tmp);
    HCalEndcapHitsHisto -> Add(HCalEndcapHitsHisto_tmp);
    MuonBarrelHitsHisto -> Add(MuonBarrelHitsHisto_tmp);
    MuonEndcapHitsHisto -> Add(MuonEndcapHitsHisto_tmp);
    LumiCalHitsHisto -> Add(LumiCalHitsHisto_tmp);
    BeamCalHitsHisto -> Add(BeamCalHitsHisto_tmp);
    SiVertexBarrelHitsHisto -> Add(SiVertexBarrelHitsHisto_tmp);
    SiVertexEndcapHitsHisto -> Add(SiVertexEndcapHitsHisto_tmp);
    SiTrackerBarrelHitsHisto -> Add(SiTrackerBarrelHitsHisto_tmp);
    SiTrackerEndcapHitsHisto -> Add(SiTrackerEndcapHitsHisto_tmp);

    delete ECalBarrelHitsHisto_tmp; 
    delete ECalEndcapHitsHisto_tmp;
    delete HCalBarrelHitsHisto_tmp; 
    delete HCalEndcapHitsHisto_tmp;
    delete MuonBarrelHitsHisto_tmp;  
    delete MuonEndcapHitsHisto_tmp;
    delete LumiCalHitsHisto_tmp;
    delete BeamCalHitsHisto_tmp;
    delete SiVertexBarrelHitsHisto_tmp;
    delete SiVertexEndcapHitsHisto_tmp;
    delete SiTrackerBarrelHitsHisto_tmp;
    delete SiTrackerEndcapHitsHisto_tmp;
}

void time_all() {
    gSystem -> Load("libDDG4Plugins.so");
    gSystem -> Load("libDDG4.so");
    gSystem -> Load("libDDCore.so");
    TFile *file_n_sp = new TFile("e-_spoilers_old.root");
    //TFile *file_n_sp = new TFile("C3SiD_e-_4vect_5spoilers.root");
    TFile *file_p_sp = new TFile("e+_spoilers_old.root");
    //TFile *file_p_sp = new TFile("C3SiD_e+_4vect_5spoilers.root");
    //TFile *file_n_spw = new TFile("C3SiD_e-_4vect_5sp+wall.root");
    //TFile *file_p_spw = new TFile("C3SiD_e-_4vect_5sp+wall.root");
    TTree *tree_n_sp = (TTree*) file_n_sp -> Get("EVENT"); 
    TTree *tree_p_sp = (TTree*) file_p_sp -> Get("EVENT"); 
    //TTree *tree_n_spw = (TTree*) file_n_spw -> Get("EVENT"); 
    //TTree *tree_p_spw = (TTree*) file_p_spw -> Get("EVENT"); 

    TH1D *ECalBarrelHitsHisto = new TH1D("ECalBarrelHitsHisto", "Hit time distribution", 40, 0, 37.8);
    TH1D *ECalEndcapHitsHisto = new TH1D("ECalEndcapHitsHisto", "histo", 40, 0, 37.8);
    TH1D *HCalBarrelHitsHisto = new TH1D("HCalBarrelHitsHisto", "histo", 40, 0, 37.8);
    TH1D *HCalEndcapHitsHisto = new TH1D("HCalEndcapHitsHisto", "histo", 40, 0, 37.8);
    TH1D *MuonBarrelHitsHisto = new TH1D("MuonBarrelHitsHisto", "histo", 40, 0, 37.8);
    TH1D *MuonEndcapHitsHisto = new TH1D("MuonEndcapHitsHisto", "Hit time distribution", 40, 0, 37.8);
    TH1D *LumiCalHitsHisto = new TH1D("LumiCalHitsHisto", "histo", 40, 0, 37.8);
    TH1D *BeamCalHitsHisto = new TH1D("BeamCalHitsHisto", "histo", 40, 0, 37.8);
    TH1D *SiVertexBarrelHitsHisto = new TH1D("SiVertexBarrelHitsHisto", "histo", 40, 0, 37.8);
    TH1D *SiVertexEndcapHitsHisto = new TH1D("SiVertexEndcapHitsHisto", "histo", 40, 0, 37.8);
    TH1D *SiTrackerBarrelHitsHisto = new TH1D("SiTrackerBarrelHitsHisto", "histo", 40, 0, 37.8);
    TH1D *SiTrackerEndcapHitsHisto = new TH1D("SiTrackerEndcapHitsHisto", "histo", 40, 0, 37.8);

    double scale_n_sp = 2.0*1312/30292;
    double scale_p_sp = 2.3*1312/33482;
    //double scale_n_spw = 0.26*75/3474;
    //double scale_p_spw = 0.34*75/4724;

    singlefile(tree_n_sp, ECalBarrelHitsHisto, ECalEndcapHitsHisto, HCalBarrelHitsHisto, HCalEndcapHitsHisto, MuonBarrelHitsHisto, MuonEndcapHitsHisto, LumiCalHitsHisto, BeamCalHitsHisto, SiVertexBarrelHitsHisto, SiVertexEndcapHitsHisto, SiTrackerBarrelHitsHisto, SiTrackerEndcapHitsHisto, scale_n_sp);
    singlefile(tree_p_sp, ECalBarrelHitsHisto, ECalEndcapHitsHisto, HCalBarrelHitsHisto, HCalEndcapHitsHisto, MuonBarrelHitsHisto, MuonEndcapHitsHisto, LumiCalHitsHisto, BeamCalHitsHisto, SiVertexBarrelHitsHisto, SiVertexEndcapHitsHisto, SiTrackerBarrelHitsHisto, SiTrackerEndcapHitsHisto, scale_p_sp);
    //singlefile(tree_n_spw, ECalBarrelHitsHisto, ECalEndcapHitsHisto, HCalBarrelHitsHisto, HCalEndcapHitsHisto, MuonBarrelHitsHisto, MuonEndcapHitsHisto, LumiCalHitsHisto, BeamCalHitsHisto, SiVertexBarrelHitsHisto, SiVertexEndcapHitsHisto, SiTrackerBarrelHitsHisto, SiTrackerEndcapHitsHisto, scale_n_spw);
    //singlefile(tree_p_spw, ECalBarrelHitsHisto, ECalEndcapHitsHisto, HCalBarrelHitsHisto, HCalEndcapHitsHisto, MuonBarrelHitsHisto, MuonEndcapHitsHisto, LumiCalHitsHisto, BeamCalHitsHisto, SiVertexBarrelHitsHisto, SiVertexEndcapHitsHisto, SiTrackerBarrelHitsHisto, SiTrackerEndcapHitsHisto, scale_p_spw);

    TCanvas *c1 = new TCanvas("c1", "time", 1900, 1800);

    c1 -> SetLogy(1);

    TPad *pad1 = new TPad("pad1", "pad1", 0, 0, 1, 1); 
    //TPad *pad2 = new TPad("pad1", "pad1", 0.9, 0, 1, 1); 
    pad1 -> SetLogy(1);
    pad1 -> Draw();
    //pad2 -> Draw();
    pad1 -> cd();

    MuonEndcapHitsHisto -> SetMinimum(1); 
    ECalBarrelHitsHisto -> SetStats(0); 
    ECalEndcapHitsHisto -> SetStats(0);
    HCalBarrelHitsHisto -> SetStats(0);
    HCalEndcapHitsHisto -> SetStats(0);
    MuonBarrelHitsHisto -> SetStats(0);  
    MuonEndcapHitsHisto -> SetStats(0); 
    LumiCalHitsHisto -> SetStats(0); 
    BeamCalHitsHisto -> SetStats(0);
    SiVertexBarrelHitsHisto -> SetStats(0);
    SiVertexEndcapHitsHisto -> SetStats(0);
    SiTrackerBarrelHitsHisto -> SetStats(0);
    SiTrackerEndcapHitsHisto -> SetStats(0);

    ECalBarrelHitsHisto -> SetLineColor(kGreen); 
    ECalEndcapHitsHisto -> SetLineColor(kBlue);
    HCalBarrelHitsHisto -> SetLineColor(kYellow);
    HCalEndcapHitsHisto -> SetLineColor(kMagenta);
    MuonBarrelHitsHisto -> SetLineColor(kRed);
    MuonEndcapHitsHisto -> SetLineColor(kBlack); 
    LumiCalHitsHisto -> SetLineColor(kCyan);  
    BeamCalHitsHisto -> SetLineColor(kTeal-5);
    SiVertexBarrelHitsHisto -> SetLineColor(kBlue-6);
    SiVertexEndcapHitsHisto -> SetLineColor(kGreen-8);
    SiTrackerBarrelHitsHisto -> SetLineColor(kOrange+8);
    SiTrackerEndcapHitsHisto -> SetLineColor(kPink-7);
  
    ECalBarrelHitsHisto -> SetLineWidth(4); 
    ECalEndcapHitsHisto -> SetLineWidth(4);
    HCalBarrelHitsHisto -> SetLineWidth(4);
    HCalEndcapHitsHisto -> SetLineWidth(4);
    MuonBarrelHitsHisto -> SetLineWidth(4);  
    MuonEndcapHitsHisto -> SetLineWidth(4); 
    LumiCalHitsHisto -> SetLineWidth(4);  
    BeamCalHitsHisto -> SetLineWidth(4);
    SiVertexBarrelHitsHisto -> SetLineWidth(4);
    SiVertexEndcapHitsHisto -> SetLineWidth(4);
    SiTrackerBarrelHitsHisto -> SetLineWidth(4);
    SiTrackerEndcapHitsHisto -> SetLineWidth(4);

    MuonEndcapHitsHisto -> Draw("Hist"); 
    MuonBarrelHitsHisto -> Draw("same Hist");  
    ECalBarrelHitsHisto -> Draw("same Hist"); 
    ECalEndcapHitsHisto -> Draw("same Hist");
    HCalBarrelHitsHisto -> Draw("same Hist");
    HCalEndcapHitsHisto -> Draw("same Hist");
    LumiCalHitsHisto -> Draw("same Hist");  
    BeamCalHitsHisto -> Draw("same Hist");
    SiVertexBarrelHitsHisto -> Draw("same Hist");
    SiVertexEndcapHitsHisto -> Draw("same Hist");
    SiTrackerBarrelHitsHisto -> Draw("same Hist");
    SiTrackerEndcapHitsHisto -> Draw("same Hist");
     
    ECalBarrelHitsHisto -> SetMarkerStyle(22); 
    ECalEndcapHitsHisto -> SetMarkerStyle(23);
    HCalBarrelHitsHisto -> SetMarkerStyle(24);
    HCalEndcapHitsHisto -> SetMarkerStyle(25);
    MuonBarrelHitsHisto -> SetMarkerStyle(21);  
    MuonEndcapHitsHisto -> SetMarkerStyle(20); 
    LumiCalHitsHisto -> SetMarkerStyle(26);  
    BeamCalHitsHisto -> SetMarkerStyle(27);
    SiVertexBarrelHitsHisto -> SetMarkerStyle(28);
    SiVertexEndcapHitsHisto -> SetMarkerStyle(29);
    SiTrackerBarrelHitsHisto -> SetMarkerStyle(30);
    SiTrackerEndcapHitsHisto -> SetMarkerStyle(31);
    
    ECalBarrelHitsHisto -> SetMarkerSize(2); 
    ECalEndcapHitsHisto -> SetMarkerSize(2);
    HCalBarrelHitsHisto -> SetMarkerSize(2);
    HCalEndcapHitsHisto -> SetMarkerSize(2);
    MuonBarrelHitsHisto -> SetMarkerSize(2);
    MuonEndcapHitsHisto -> SetMarkerSize(2); 
    LumiCalHitsHisto -> SetMarkerSize(2);  
    BeamCalHitsHisto -> SetMarkerSize(2);
    SiVertexBarrelHitsHisto -> SetMarkerSize(2);
    SiVertexEndcapHitsHisto -> SetMarkerSize(2);
    SiTrackerBarrelHitsHisto -> SetMarkerSize(2);
    SiTrackerEndcapHitsHisto -> SetMarkerSize(2);
 
    ECalBarrelHitsHisto -> SetMarkerColor(kGreen); 
    ECalEndcapHitsHisto -> SetMarkerColor(kBlue);
    HCalBarrelHitsHisto -> SetMarkerColor(kYellow);
    HCalEndcapHitsHisto -> SetMarkerColor(kMagenta);
    MuonBarrelHitsHisto -> SetMarkerColor(kRed);
    MuonEndcapHitsHisto -> SetMarkerColor(kBlack); 
    LumiCalHitsHisto -> SetMarkerColor(kCyan);  
    BeamCalHitsHisto -> SetMarkerColor(kTeal-5);
    SiVertexBarrelHitsHisto -> SetMarkerColor(kBlue-6);
    SiVertexEndcapHitsHisto -> SetMarkerColor(kGreen-8);
    SiTrackerBarrelHitsHisto -> SetMarkerColor(kOrange+8);
    SiTrackerEndcapHitsHisto -> SetMarkerColor(kPink-7);

    MuonEndcapHitsHisto -> Draw("same Hist P"); 
    MuonBarrelHitsHisto -> Draw("same Hist P");  
    ECalBarrelHitsHisto -> Draw("same Hist P"); 
    ECalEndcapHitsHisto -> Draw("same Hist P");
    HCalBarrelHitsHisto -> Draw("same Hist P");
    HCalEndcapHitsHisto -> Draw("same Hist P");
    LumiCalHitsHisto -> Draw("same Hist P");  
    BeamCalHitsHisto -> Draw("same Hist P");
    SiVertexBarrelHitsHisto -> Draw("same Hist P");
    SiVertexEndcapHitsHisto -> Draw("same Hist P");
    SiTrackerBarrelHitsHisto -> Draw("same Hist P");
    SiTrackerEndcapHitsHisto -> Draw("same Hist P");
    
    //pad2 -> cd();
    TLegend *leg = new TLegend(0.75, 0.5, 0.95, 0.95);
    //TLegend *leg = new TLegend(0.1, 0.1, 0.9, 0.9);
    leg -> AddEntry(MuonEndcapHitsHisto, "MuonEndcapHits", "lep");
    leg -> AddEntry(MuonBarrelHitsHisto, "MuonBarrelHits", "lep");
    leg -> AddEntry(ECalBarrelHitsHisto, "ECalBarrelHits", "lep");
    leg -> AddEntry(ECalEndcapHitsHisto, "ECalEndcapHits", "lep");
    leg -> AddEntry(HCalBarrelHitsHisto, "HCalBarrelHits", "lep");
    leg -> AddEntry(HCalEndcapHitsHisto, "HCalEndcapHits", "lep");
    leg -> AddEntry(LumiCalHitsHisto, "LumiCalHits", "lep");
    leg -> AddEntry(BeamCalHitsHisto, "BeamCalHits", "lep");
    leg -> AddEntry(SiVertexBarrelHitsHisto, "SiVertexBarrelHits", "lep");
    leg -> AddEntry(SiVertexEndcapHitsHisto, "SiVertexEndcapHits", "lep");
    leg -> AddEntry(SiTrackerBarrelHitsHisto, "SiTrackerBarrelHits", "lep");
    leg -> AddEntry(SiTrackerEndcapHitsHisto, "SiTrackerEndcapHits", "lep");
    leg -> Draw("SAME");

    c1 -> Update();
    c1 -> SaveAs("Time.png");

    file_n_sp -> Close();
    file_p_sp -> Close();
    //file_n_spw -> Close();
    //file_p_spw -> Close();
}
