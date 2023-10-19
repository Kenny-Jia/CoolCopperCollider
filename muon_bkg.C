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

void muon_bkg(const char *inputFile) {
    gSystem -> Load("libDDG4Plugins.so");
    gSystem -> Load("libDDG4.so");
    gSystem -> Load("libDDCore.so");
    TFile *file = new TFile(inputFile);
    TTree *tree = (TTree*) file -> Get("EVENT");

    vector<dd4hep::sim::Geant4Tracker::Hit*> *hits = 0;
    vector<dd4hep::sim::Geant4Calorimeter::Hit*> *muonbarrelhits = 0;
    vector<dd4hep::sim::Geant4Particle*> *mcParticles = 0;
    
    tree -> SetBranchAddress("MuonBarrelHits", &muonbarrelhits);
    tree -> SetBranchAddress("MCParticles", &mcParticles);

    TH1D *muonbarrel = new TH1D("muonbarrel", "Muon Barrel Hits energy spectrum", 30, 0, 1.5);
    TH1D *mcmuon_pe = new TH1D("mcmuon_pe", "MC Muon energy spectrum", 30, 0, 200000);
    TH1D *mcmuon_ps = new TH1D("mcmuon_ps", "Mc Muon energy spectrum", 30, 0, 200000);
    TH2D *muonbar_x_y = new TH2D("muonbar_x_y", "Muon Barrel Hits X_Y position", 250, -8000, 8000, 250, -8000, 8000);
    TH2D *mc_s_x_y = new TH2D("mc_s_x_y", "MC muons starting X_Y position", 250, -15000, 15000, 250, -15000, 15000);
    TH2D *mc_e_x_y = new TH2D("mc_e_x_y", "MC muons ending X_Y position", 250, -15000, 15000, 250, -15000, 15000);
    for (int i = 0; i < tree->GetEntries(); ++i) {
	tree -> GetEntry(i);

	for (size_t j = 0; j < muonbarrelhits -> size(); ++j) {
	    dd4hep::sim::Geant4Calorimeter::Hit* muonbarrelhit = (*muonbarrelhits)[j];
	    double energy = muonbarrelhit -> energyDeposit;
	    ROOT::Math::XYZVector pos = muonbarrelhit -> position;
	    //ROOT::Math::XYZVector dir = muonbarrelhit -> momentum;
	    double x = pos.X();
	    double y = pos.Y();
	    double z = pos.Z();
	    //double px = dir.X();
	    //double py = dir.Y();
	    //double pz = dir.Z();
	    //std::cout << "x: " << x << "; y: " << y << "; z: " << z << std::endl;
	    //std::cout << "px: " << px << "; py: " << py << "; pz: " << pz << std::endl;
	    
	    //std::cout << energy << std::endl;

	    if (energy != 0 ){
		muonbarrel -> Fill(energy);
		muonbar_x_y -> Fill(x, y);
	    }
	}
	for (size_t k = 0; k < mcParticles -> size(); ++k) {
	    dd4hep::sim::Geant4Particle* mcParticle = (*mcParticles)[k];
	    if (TMath::Abs(mcParticle -> pdgID) != 13) {
		continue;
	    }
	    double mc_pex = mcParticle -> pex;
	    double mc_pey = mcParticle -> pey;
	    double mc_pez = mcParticle -> pez;
	    double mc_psx = mcParticle -> psx;
	    double mc_psy = mcParticle -> psy;
	    double mc_psz = mcParticle -> psz;
	    double mc_pe = TMath::Sqrt(mc_pex * mc_pex + mc_pey * mc_pey + mc_pez * mc_pez);
	    double mc_ps = TMath::Sqrt(mc_psx * mc_psx + mc_psy * mc_psy + mc_psz * mc_psz);

	    double mc_vex = mcParticle -> vex;
	    double mc_vey = mcParticle -> vey;
	    double mc_vez = mcParticle -> vez;
	    double mc_vsx = mcParticle -> vsx;
	    double mc_vsy = mcParticle -> vsy;
	    double mc_vsz = mcParticle -> vsz;
	    mcmuon_pe -> Fill(mc_pe);
	    mcmuon_ps -> Fill(mc_ps);
	    //std::cout << "ps = " << mc_ps << "; pe = " << mc_pe << ";" << std::endl;
	    mc_e_x_y -> Fill(mc_vex, mc_vey);
	    mc_s_x_y -> Fill(mc_vsx, mc_vsy);

	}
    }
    TCanvas *totalcanvas = new TCanvas("totalcanvas", "Canvas", 1400, 1400, 1400, 1400);
    totalcanvas -> SetWindowSize(1204,1228);
    totalcanvas -> SetCanvasSize(1200,1200);
    totalcanvas -> SetLogy();
    mcmuon_pe -> SetLineColor(kPink);
    mcmuon_ps -> SetLineColor(kBlue);
    TLegend *legend = new TLegend(0.15, 0.15, 0.35, 0.35);
    legend -> AddEntry(mcmuon_ps, "starting", "l");
    legend -> AddEntry(mcmuon_pe, "ending", "l");
    mcmuon_pe -> Draw("HIST");
    mcmuon_ps-> Draw("same");
    legend -> Draw("same");
    totalcanvas -> SaveAs("energy_spectrum.png");
   /* 
    TCanvas *canvas = new TCanvas("canvas", "Canvas", 1400, 1400, 1400, 1400);
    canvas -> SetWindowSize(1204,1228);
    canvas -> SetCanvasSize(1200,1200);
    muonbar_x_y -> Draw("COL");
    canvas -> SaveAs("MuonBarrel_x_y.png");

    TCanvas *c1 = new TCanvas("c1", "Canvas", 1400, 1400, 1400, 1400);
    c1 -> SetWindowSize(1204,1228);
    c1 -> SetCanvasSize(1200,1200);
    mc_e_x_y -> Draw("COL");
    c1-> SaveAs("Muon_e_x_y.png");

    TCanvas *c2 = new TCanvas("c2", "Canvas", 1400, 1400, 1400, 1400);
    c2 -> SetWindowSize(1204,1228);
    c2 -> SetCanvasSize(1200,1200);
    mc_s_x_y -> Draw("COL");
    c2-> SaveAs("Muon_s_x_y.png");

 */   
    file -> Close();
}
