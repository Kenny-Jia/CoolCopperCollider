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

int singlebkg(TTree *tree, const char *hitname) {
    tree->ResetBranchAddresses();

    std::cout << "Processing " << hitname << std::endl;
    int hitcnt = 0;
    
    vector<dd4hep::sim::Geant4Calorimeter::Hit*> *hits = new vector<dd4hep::sim::Geant4Calorimeter::Hit*>;

    tree -> SetBranchAddress(hitname, &hits);

    std::cout << "open up branch " << hitname << std::endl;
    for (int i = 0; i < tree->GetEntries(); ++i) {
	tree -> GetEntry(i);
	std::cout << "Processing event " << i << std::endl;
	std::cout << "hit number " << hits -> size() << std::endl;
	for (size_t j = 0; j < hits -> size(); ++j) {
	    dd4hep::sim::Geant4Calorimeter::Hit* hit = (*hits)[j];
	    double energy = hit -> energyDeposit;
	    hitcnt++;
	    /*
	    if (energy != 0 ){
	        hitcnt++;
	    }
	    */
	}
    }
    std::cout << hitcnt << std::endl;
    delete hits;
    return hitcnt;
}
int singlebkg_tracker(TTree *tree, const char *hitname) {
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
	        hitcnt++;
	    }
	}
    }
    std::cout << hitcnt << std::endl;
    delete hits;
    return hitcnt;
}

void all_hits(const char *inputFile_sp, const char *inputFile_spw) {
    gSystem -> Load("libDDG4Plugins.so");
    gSystem -> Load("libDDG4.so");
    gSystem -> Load("libDDCore.so");
    TFile *file_sp = new TFile(inputFile_sp);
    TFile *file_spw = new TFile(inputFile_spw);
    TTree *tree_sp = (TTree*) file_sp -> Get("EVENT");
    TTree *tree_spw = (TTree*) file_spw -> Get("EVENT");

    int ecalbarrel_sp = singlebkg(tree_sp, "ECalBarrelHits");    
    int ecalendcap_sp = singlebkg(tree_sp, "ECalEndcapHits");    
    int hcalbarrel_sp = singlebkg(tree_sp, "HCalBarrelHits");    
    int hcalendcap_sp = singlebkg(tree_sp, "HCalEndcapHits");    
    int muonbarrel_sp = singlebkg(tree_sp, "MuonBarrelHits");    
    int muonendcap_sp = singlebkg(tree_sp, "MuonEndcapHits");    
    int lumical_sp = singlebkg(tree_sp, "LumiCalHits");
    int beamcal_sp = singlebkg(tree_sp, "BeamCalHits");
    int sivertexbarrel_sp = singlebkg_tracker(tree_sp, "SiVertexBarrelHits");    
    int sivertexendcap_sp = singlebkg_tracker(tree_sp, "SiVertexEndcapHits");    
    int sitrackerbarrel_sp = singlebkg_tracker(tree_sp, "SiTrackerBarrelHits");    
    int sitrackerendcap_sp = singlebkg_tracker(tree_sp, "SiTrackerEndcapHits");    
	
    int ecalbarrel_spw = singlebkg(tree_spw, "ECalBarrelHits");    
    int ecalendcap_spw = singlebkg(tree_spw, "ECalEndcapHits");    
    int hcalbarrel_spw = singlebkg(tree_spw, "HCalBarrelHits");    
    int hcalendcap_spw = singlebkg(tree_spw, "HCalEndcapHits");    
    int muonbarrel_spw = singlebkg(tree_spw, "MuonBarrelHits");    
    int muonendcap_spw = singlebkg(tree_spw, "MuonEndcapHits");    
    int lumical_spw = singlebkg(tree_spw, "LumiCalHits");
    int beamcal_spw = singlebkg(tree_spw, "BeamCalHits");
    int sivertexbarrel_spw = singlebkg_tracker(tree_spw, "SiVertexBarrelHits");    
    int sivertexendcap_spw = singlebkg_tracker(tree_spw, "SiVertexEndcapHits");    
    int sitrackerbarrel_spw = singlebkg_tracker(tree_spw, "SiTrackerBarrelHits");    
    int sitrackerendcap_spw = singlebkg_tracker(tree_spw, "SiTrackerEndcapHits");    
	
    TCanvas *c1 = new TCanvas("c1", "Comparison Bar Chart", 2400, 1800);

    c1 -> SetLogy(1);
    c1 -> SetGrid();
    TH1F *h_sp = new TH1F("h_sp", "", 12, 0, 12);
    h_sp -> SetMinimum(1);
    h_sp -> SetBinContent(1, ecalbarrel_sp);
    h_sp -> SetBinContent(2, ecalendcap_sp);
    h_sp -> SetBinContent(3, hcalbarrel_sp);
    h_sp -> SetBinContent(4, hcalendcap_sp);
    h_sp -> SetBinContent(5, muonbarrel_sp);
    h_sp -> SetBinContent(6, muonendcap_sp);
    h_sp -> SetBinContent(7, lumical_sp);
    h_sp -> SetBinContent(8, beamcal_sp);
    h_sp -> SetBinContent(9, sivertexbarrel_sp);
    h_sp -> SetBinContent(10, sivertexendcap_sp);
    h_sp -> SetBinContent(11, sitrackerbarrel_sp);
    h_sp -> SetBinContent(12, sitrackerendcap_sp);
    h_sp -> SetFillColor(kRed);
    h_sp -> SetFillStyle(1001);  
    h_sp -> SetBarWidth(0.8);
    h_sp -> SetBarOffset(0.1);
    h_sp -> SetStats(0);

    TH1F *h_spw = new TH1F("h_spw", "", 12, 0, 12);
    h_spw -> SetBinContent(1, ecalbarrel_spw);
    h_spw -> SetBinContent(2, ecalendcap_spw);
    h_spw -> SetBinContent(3, hcalbarrel_spw);
    h_spw -> SetBinContent(4, hcalendcap_spw);
    h_spw -> SetBinContent(5, muonbarrel_spw);
    h_spw -> SetBinContent(6, muonendcap_spw);
    h_spw -> SetBinContent(7, lumical_spw);
    h_spw -> SetBinContent(8, beamcal_spw);
    h_spw -> SetBinContent(9, sivertexbarrel_spw);
    h_spw -> SetBinContent(10, sivertexendcap_spw);
    h_spw -> SetBinContent(11, sitrackerbarrel_spw);
    h_spw -> SetBinContent(12, sitrackerendcap_spw);
    h_spw -> SetFillStyle(1001);  
    h_spw -> SetFillColor(kWhite);
    h_spw -> SetBarWidth(0.8);
    h_spw -> SetBarOffset(0.1);
    h_spw -> SetStats(0);
 
    h_sp -> GetXaxis() -> SetBinLabel(1, "ECalBarrel");
    h_sp -> GetXaxis() -> SetBinLabel(2, "ECalEndcap");
    h_sp -> GetXaxis() -> SetBinLabel(3, "HCalBarrel");
    h_sp -> GetXaxis() -> SetBinLabel(4, "HCalEndcap");
    h_sp -> GetXaxis() -> SetBinLabel(5, "MuonBarrel");
    h_sp -> GetXaxis() -> SetBinLabel(6, "MuonEndcap");
    h_sp -> GetXaxis() -> SetBinLabel(7, "LumiCal");
    h_sp -> GetXaxis() -> SetBinLabel(8, "BeamCal");
    h_sp -> GetXaxis() -> SetBinLabel(9, "SiVertexBarrel");
    h_sp -> GetXaxis() -> SetBinLabel(10, "SiVertexEndcap");
    h_sp -> GetXaxis() -> SetBinLabel(11, "SiTrackerBarrel");
    h_sp -> GetXaxis() -> SetBinLabel(12, "SiTrackerEndcap");

    h_sp -> GetYaxis() -> SetTitle("Number of Hits");

    h_sp -> Draw("B");
    h_spw -> Draw("SAME B");
    TH1F* h_spw_copy = new TH1F(*h_spw);
    h_spw_copy -> SetFillStyle(3353);
    h_spw_copy -> SetFillColor(kRed);
    h_spw_copy -> Draw("SAME B");
    
    TLegend *leg = new TLegend(0.75, 0.75, 0.9, 0.9);
    leg -> AddEntry(h_sp, "5 Spoilers", "f");
    leg -> AddEntry(h_spw, "5 Spoilers + Wall", "f");
    leg -> Draw("SAME");

    c1 -> Update();
    c1 -> SaveAs("SubDetector.png");

    file_sp -> Close();
}
