// This script will select a specific number of random events from an ntuple

#include<cstdio>
#include<cstdlib>
#include<iostream>
#include<vector>

#include </cvmfs/sft.cern.ch/lcg/releases/LCG_104c_ATLAS_2/ROOT/6.28.10/x86_64-el9-gcc13-opt/include/TROOT.h>
#include </cvmfs/sft.cern.ch/lcg/releases/LCG_104c_ATLAS_2/ROOT/6.28.10/x86_64-el9-gcc13-opt/include/TTree.h>
#include </cvmfs/sft.cern.ch/lcg/releases/LCG_104c_ATLAS_2/ROOT/6.28.10/x86_64-el9-gcc13-opt/include/TFile.h>
#include </cvmfs/sft.cern.ch/lcg/releases/LCG_104c_ATLAS_2/ROOT/6.28.10/x86_64-el9-gcc13-opt/include/TRandom3.h>
#include </cvmfs/sft.cern.ch/lcg/releases/LCG_104c_ATLAS_2/ROOT/6.28.10/x86_64-el9-gcc13-opt/include/TLorentzVector.h>
#include </cvmfs/sft.cern.ch/lcg/releases/LCG_104c_ATLAS_2/ROOT/6.28.10/x86_64-el9-gcc13-opt/include/TVector3.h>
#include </cvmfs/sft.cern.ch/lcg/releases/LCG_104c_ATLAS_2/ROOT/6.28.10/x86_64-el9-gcc13-opt/include/TH1D.h>
#include </cvmfs/sft.cern.ch/lcg/releases/LCG_104c_ATLAS_2/ROOT/6.28.10/x86_64-el9-gcc13-opt/include/TAttMarker.h>


int main() {
    TFile f("/eos/user/s/svenetia/taucp_ntuples_with_hadhad/mc/hadhad/mc20e/nom/user.svenetia.Ztt_pythia02.mc20_13TeV.802365.Py8_DYtt_60M250_hadhad.PHYS.r13145_p6266.w_0_Zt/user.svenetia.46611689._000001.ZttHadHad.root"); // read input file
    TTree *input_tree = (TTree*)f.Get("NOMINAL");        // "Get" the NOMINAL tree from the file and assign it to local variable "input_tree"

    Int_t n_taus;                                        // Initialize variable storing the number of taus (integer)
    Int_t tau0_matched_n_charged_pion;
    Int_t tau0_matched_n_neutral_pion;
    UInt_t tau0_matched_isHadTau;
    Int_t tau1_matched_n_charged_pion;
    Int_t tau1_matched_n_neutral_pion;
    UInt_t tau1_matched_isHadTau;

    TLorentzVector* tau1_p4 = new TLorentzVector();     // test: create pointer tau1_p4 that points to a TLoretnzVector
    
    // Initialize TLorentzVector for each p4 needed for psi truth calc
    TLorentzVector* tau0_matched_vis_p4 = new TLorentzVector();             // truth variables
    TLorentzVector* tau0_matched_vis_neutral_p4 = new TLorentzVector();
    TLorentzVector* tau1_matched_vis_p4 = new TLorentzVector();
    TLorentzVector* tau1_matched_vis_neutral_p4 = new TLorentzVector();

    TLorentzVector* tau0_charged_tracks_p4 = new TLorentzVector();          // reco variables
    TLorentzVector* tau0_pi0s_p4 = new TLorentzVector();
    TLorentzVector* tau1_charged_tracks_p4 = new TLorentzVector();
    TLorentzVector* tau1_pi0s_p4 = new TLorentzVector();

    input_tree->SetBranchAddress("n_taus", &n_taus);     // test: Assign new "n_taus" variables to its address (&n_taus) in the input_tree
   
    input_tree->SetBranchAddress("tau0_matched_n_charged_pion", &tau0_matched_n_charged_pion); 
    input_tree->SetBranchAddress("tau0_matched_n_neutral_pion", &tau0_matched_n_neutral_pion);
    input_tree->SetBranchAddress("tau0_matched_isHadTau", &tau0_matched_isHadTau);
    input_tree->SetBranchAddress("tau1_matched_n_charged_pion", &tau1_matched_n_charged_pion);
    input_tree->SetBranchAddress("tau1_matched_n_neutral_pion", &tau1_matched_n_neutral_pion);
    input_tree->SetBranchAddress("tau1_matched_isHadTau", &tau1_matched_isHadTau);

    input_tree->SetBranchAddress("tau1_p4", &tau1_p4);	// test

    // Point to all p4s needed for psi truth
    input_tree->SetBranchAddress("tau0_matched_vis_p4", &tau0_matched_vis_p4);                      // truth variables
    input_tree->SetBranchAddress("tau0_matched_vis_neutral_p4", &tau0_matched_vis_neutral_p4);
    input_tree->SetBranchAddress("tau1_matched_vis_p4", &tau1_matched_vis_p4);
    input_tree->SetBranchAddress("tau1_matched_vis_neutral_p4", &tau1_matched_vis_neutral_p4);

    input_tree->SetBranchAddress("tau0_charged_tracks_p4", &tau0_charged_tracks_p4);                      // reco variables
    input_tree->SetBranchAddress("tau0_pi0s_p4", &tau0_pi0s_p4);
    input_tree->SetBranchAddress("tau1_charged_tracks_p4", &tau1_charged_tracks_p4);
    input_tree->SetBranchAddress("tau1_pi0s_p4", &tau1_pi0s_p4);

    TRandom3 rand;                                       // Initialize random number to 
    // std::cout << "Initialized random number" << std::endl;

    std::vector<float> psi_truth;
    std::vector<float> psi_reco_vec;

    TH1* h1 = nullptr;
    TH1* h2 = nullptr;

    h1 = new TH1D("h1", "Psi Truth", 30, -3.14, 3.14);
    h2 = new TH1D("h2", "Psi Reco", 30, -3.14, 3.14);

    TFile fout1("psi_truth.root", "recreate");
    TFile fout2("psi_reco.root", "recreate");

    std::cout << "Starting loop..." << std::endl;

    int nEntries = input_tree->GetEntries();

    for ( int randCounter = 0 ; randCounter < nEntries ; randCounter++ ) { // iterate from 0 to the max number of events you want to select
        // int i = rand.Uniform(input_tree->GetEntries());     // chooses a random number from the entries in the input tree

        input_tree->GetEvent(randCounter);                            // get the information for the specific event

        // truth selections
        if ((tau0_matched_n_charged_pion==1) && (tau0_matched_n_neutral_pion==1) && (tau0_matched_isHadTau==1) && (tau1_matched_n_charged_pion==1) && (tau1_matched_n_neutral_pion==1) && (tau1_matched_isHadTau==1))
        {

        //std::cout << "Index for this event is: " << randCounter << " and number of taus is: " << n_taus << std::endl; // print branch information for that index

        // implement TRUTH psi calculation

        TLorentzVector had_tau0_vis;            // pulls from matched_vis
        TLorentzVector had_tau1_vis;
        
        TLorentzVector tau0_neut_pion;          // pulls from matched_vis_neutral
        TLorentzVector tau1_neut_pion;

        TLorentzVector tau0_char_pion;          // calculated
        TLorentzVector tau1_char_pion;

        // Need to first subtract neutral pion from total p4 to get charged contribution for correct filling
        // This formula only works for truth because reco needs to fill from 'tau0_charged_tracks_p4'

        had_tau0_vis.SetPtEtaPhiE(tau0_matched_vis_p4->Pt(), tau0_matched_vis_p4->Eta(), tau0_matched_vis_p4->Phi(), tau0_matched_vis_p4->E());
        had_tau1_vis.SetPtEtaPhiE(tau1_matched_vis_p4->Pt(), tau1_matched_vis_p4->Eta(), tau1_matched_vis_p4->Phi(), tau1_matched_vis_p4->E());
        tau0_neut_pion.SetPtEtaPhiE(tau0_matched_vis_neutral_p4->Pt(), tau0_matched_vis_neutral_p4->Eta(), tau0_matched_vis_neutral_p4->Phi(), tau0_matched_vis_neutral_p4->E());
        tau1_neut_pion.SetPtEtaPhiE(tau1_matched_vis_neutral_p4->Pt(), tau1_matched_vis_neutral_p4->Eta(), tau1_matched_vis_neutral_p4->Phi(), tau1_matched_vis_neutral_p4->E());

        tau0_char_pion=had_tau0_vis - tau0_neut_pion;
        tau1_char_pion=had_tau1_vis - tau1_neut_pion;

        const TLorentzVector COM = tau0_char_pion + tau1_char_pion + tau0_neut_pion + tau1_neut_pion;

        const double upsilon0 = tau0_char_pion.E()/(tau0_char_pion+tau0_neut_pion).E();
        const double upsilon1 = tau1_char_pion.E()/(tau1_char_pion+tau1_neut_pion).E();

        // std::cout<<"upsilon0: " << upsilon0 << std::endl;
        // std::cout<<"upsilon1: " <<upsilon1 << std::endl;

        // compute_psi_hh_basic

        TLorentzVector beam;
	    beam.SetPxPyPzE(0., 0., 6.5e6, 6.5e6);
	    
	    const TVector3 boostVector = COM.BoostVector();

        tau0_char_pion.Boost((-1.)*boostVector);
        tau1_char_pion.Boost((-1.)*boostVector);
        beam.Boost((-1.)*boostVector); 

        // rotate system and obtain psi
        
        double phi_tau0_char_pion = tau0_char_pion.Phi();
        //std::cout << "phi_vz (tau0_char_pion phi): " << phi_vz << "\n" << std::endl;
        double theta_tau0_char_pion = tau0_char_pion.Theta();

        tau0_char_pion.RotateZ((-1.0) * phi_tau0_char_pion);
        tau0_char_pion.RotateY((-1.0) * theta_tau0_char_pion);

        tau1_char_pion.RotateZ((-1.0) * phi_tau0_char_pion);
        tau1_char_pion.RotateY((-1.0) * theta_tau0_char_pion);

        beam.RotateZ((-1.0) * phi_tau0_char_pion);
        beam.RotateY((-1.0) * theta_tau0_char_pion);
        beam.RotateZ((-1.0) * tau1_char_pion.Phi());

        tau0_char_pion.RotateZ((-1.0) * tau1_char_pion.Phi());
        tau1_char_pion.RotateZ((-1.0) * tau1_char_pion.Phi());

        double psi = beam.Phi();

        // perform phase shift if necessary
        if ((upsilon0 < 0.5 && upsilon1 > 0.5) || (upsilon0 > 0.5 && upsilon1 < 0.5))
        {
            // std::cout << "Upsilon condition met" <<"\n"<<std::endl;
            psi += (M_PI / 2);

        }

        // std::cout << "check" << std::endl;
        // correct phase to be witin -pi to pi
        while (psi > M_PI) 
        {
            psi -= 2 * M_PI;
        }
        while (psi < -M_PI) 
        {
            psi += 2 * M_PI;
        }
        
        // std::cout << "Psi: " << psi << "\n" << std::endl;

        psi_truth.push_back(psi);
        h1->Fill(psi);

        //std::cout<<"psi_truth: " << psi_truth.size() <<std::endl;




        // *** IMPLEMENTING RECO PSI CALCULATION **
        
        TLorentzVector tau0_neut_pion_reco;          // pulls from pi0_p4
        TLorentzVector tau1_neut_pion_reco;

        TLorentzVector tau0_char_pion_reco;          // pulls from charged_tracks
        TLorentzVector tau1_char_pion_reco;

        // Need to first subtract neutral pion from total p4 to get charged contribution for correct filling
        // This formula only works for truth because reco needs to fill from 'tau0_charged_tracks_p4'

        tau0_neut_pion_reco.SetPtEtaPhiE(tau0_pi0_p4->Pt(), tau0_pi0_p4->Eta(), tau0_pi0_p4->Phi(), tau0_pi0_p4->E());
        tau1_neut_pion_reco.SetPtEtaPhiE(tau1_pi0_p4->Pt(), tau1_pi0_p4->Eta(), tau1_pi0_p4->Phi(), tau1_pi0_p4->E());

        tau0_char_pion_reco.SetPtEtaPhiE(tau0_charged_tracks_p4->Pt(), tau0_charged_tracks_p4->Eta(), tau0_charged_tracks_p4->Phi(), tau0_charged_tracks_p4->E());
        tau1_char_pion_reco.SetPtEtaPhiE(tau1_charged_tracks_p4->Pt(), tau1_charged_tracks_p4->Eta(), tau1_charged_tracks_p4->Phi(), tau1_charged_tracks_p4->E());

        const TLorentzVector COM_reco = tau0_char_pion_reco + tau1_char_pion_reco + tau0_neut_pion_reco + tau1_neut_pion_reco;

        const double upsilon0_reco = tau0_char_pion_reco.E()/(tau0_char_pion_reco+tau0_neut_pion_reco).E();
        const double upsilon1_reco = tau1_char_pion_reco.E()/(tau1_char_pion_reco+tau1_neut_pion_reco).E();

        // std::cout<<"upsilon0: " << upsilon0 << std::endl;
        // std::cout<<"upsilon1: " <<upsilon1 << std::endl;

        // compute_psi_hh_basic

        TLorentzVector beam_reco;
	    beam_reco.SetPxPyPzE(0., 0., 6.5e6, 6.5e6);
	    
	    const TVector3 boostVector_reco = COM_reco.BoostVector();

        tau0_char_pion_reco.Boost((-1.)*boostVector_reco);
        tau1_char_pion_reco.Boost((-1.)*boostVector_reco);
        beam_reco.Boost((-1.)*boostVector_reco); 

        // rotate system and obtain psi
        
        double phi_tau0_char_pion_reco = tau0_char_pion_reco.Phi();
        //std::cout << "phi_vz (tau0_char_pion phi): " << phi_vz << "\n" << std::endl;
        double theta_tau0_char_pion_reco = tau0_char_pion_reco.Theta();

        tau0_char_pion_reco.RotateZ((-1.0) * phi_tau0_char_pion_reco);
        tau0_char_pion_reco.RotateY((-1.0) * theta_tau0_char_pion_reco);

        tau1_char_pion_reco.RotateZ((-1.0) * phi_tau0_char_pion_reco);
        tau1_char_pion_reco.RotateY((-1.0) * theta_tau0_char_pion_reco);

        beam_reco.RotateZ((-1.0) * phi_tau0_char_pion_reco);
        beam_reco.RotateY((-1.0) * theta_tau0_char_pion_reco);
        beam_reco.RotateZ((-1.0) * tau1_char_pion_reco.Phi());

        tau0_char_pion_reco.RotateZ((-1.0) * tau1_char_pion_reco.Phi());
        tau1_char_pion_reco.RotateZ((-1.0) * tau1_char_pion_reco.Phi());

        double psi_reco = beam_reco.Phi();

        // perform phase shift if necessary
        if ((upsilon0_reco < 0.5 && upsilon1_reco > 0.5) || (upsilon0_reco > 0.5 && upsilon1_reco < 0.5))
        {
            // std::cout << "Upsilon condition met" <<"\n"<<std::endl;
            psi_reco += (M_PI / 2);

        }

        // std::cout << "check" << std::endl;
        // correct phase to be witin -pi to pi
        while (psi_reco > M_PI) 
        {
            psi_reco -= 2 * M_PI;
        }
        while (psi_reco < -M_PI) 
        {
            psi_reco += 2 * M_PI;
        }
        
        // std::cout << "Psi: " << psi << "\n" << std::endl;

        psi_reco_vec.push_back(psi_reco);
        h1->Fill(psi_reco);

        //std::cout<<"psi_truth: " << psi_truth.size() <<std::endl;


        }

    }

    // Printing all psi values

    /*
    for ( float counter = 0 ; counter < psi_truth.size() ; counter++ ) {
        std::cout << psi_truth[counter] << std::endl;
    }
    */

    // Drawing Histograms

    //  Truth Psi

    fout1.cd();
    
    std::cout << "writing truth psi..." << std::endl;
    h1->GetXaxis()->SetTitle("Truth Psi");
    h1->GetYaxis()->SetTitle("Entries");
    //h1->SetMarkerStyle();

    h1->Write();
    std::cout << "closing truth psi..." << std::endl;
    fout1.Close();
    std::cout << "Done." << std::endl;

    // Reco Psi

    fout2.cd();
    
    std::cout << "writing reco psi..." << std::endl;
    h2->GetXaxis()->SetTitle("Reco Psi");
    h2->GetYaxis()->SetTitle("Entries");
    //h1->SetMarkerStyle();

    h2->Write();
    std::cout << "closing reco psi..." << std::endl;
    fout2.Close();
    std::cout << "Done." << std::endl;

    return 0;
    }
