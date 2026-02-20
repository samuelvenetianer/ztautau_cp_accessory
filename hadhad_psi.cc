// This script will select a specific number of random events from an ntuple

#include<cstdio>
#include<cstdlib>
#include<iostream>
#include<vector>
#include<iterator>
#include<algorithm> 
#include<fstream>

#include </cvmfs/sft.cern.ch/lcg/releases/LCG_104c_ATLAS_2/ROOT/6.28.10/x86_64-el9-gcc13-opt/include/TROOT.h>
#include </cvmfs/sft.cern.ch/lcg/releases/LCG_104c_ATLAS_2/ROOT/6.28.10/x86_64-el9-gcc13-opt/include/TTree.h>
#include </cvmfs/sft.cern.ch/lcg/releases/LCG_104c_ATLAS_2/ROOT/6.28.10/x86_64-el9-gcc13-opt/include/TFile.h>
#include </cvmfs/sft.cern.ch/lcg/releases/LCG_104c_ATLAS_2/ROOT/6.28.10/x86_64-el9-gcc13-opt/include/TRandom3.h>
#include </cvmfs/sft.cern.ch/lcg/releases/LCG_104c_ATLAS_2/ROOT/6.28.10/x86_64-el9-gcc13-opt/include/TLorentzVector.h>
#include </cvmfs/sft.cern.ch/lcg/releases/LCG_104c_ATLAS_2/ROOT/6.28.10/x86_64-el9-gcc13-opt/include/TVector3.h>
#include </cvmfs/sft.cern.ch/lcg/releases/LCG_104c_ATLAS_2/ROOT/6.28.10/x86_64-el9-gcc13-opt/include/TH1D.h>
#include </cvmfs/sft.cern.ch/lcg/releases/LCG_104c_ATLAS_2/ROOT/6.28.10/x86_64-el9-gcc13-opt/include/TAttMarker.h>

#include <DictOutput.cxx>

float hadhadTruthPsi(
                TLorentzVector* tau0_track,  // tau0_matched_vis_p4
                TLorentzVector* tau1_track,  // tau1_matched_vis_p4
                TLorentzVector* tau0_neut,   // tau0_matched_vis_neutral_p4
                TLorentzVector* tau1_neut    // tau1_matched_vis_neutral_p4
                ){

    // implement TRUTH psi calculation

    // std::cout << "Calculating truth level psi..." << std::endl;

    TLorentzVector had_tau0_vis;
    TLorentzVector had_tau1_vis;
    TLorentzVector tau0_char_pion;
    TLorentzVector tau1_char_pion;
    TLorentzVector tau0_neut_pion;
    TLorentzVector tau1_neut_pion;

    // Need to first subtract neutral pion from total p4 to get charged contribution for correct filling
    // This formula only works for truth because reco needs to fill from 'tau0_charged_tracks_p4'

    had_tau0_vis.SetPtEtaPhiE(tau0_track->Pt(), tau0_track->Eta(), tau0_track->Phi(), tau0_track->E());
    had_tau1_vis.SetPtEtaPhiE(tau1_track->Pt(), tau1_track->Eta(), tau1_track->Phi(), tau1_track->E());
    tau0_neut_pion.SetPtEtaPhiE(tau0_neut->Pt(), tau0_neut->Eta(), tau0_neut->Phi(), tau0_neut->E());
    tau1_neut_pion.SetPtEtaPhiE(tau1_neut->Pt(), tau1_neut->Eta(), tau1_neut->Phi(), tau1_neut->E());

    tau0_char_pion=had_tau0_vis - tau0_neut_pion;
    tau1_char_pion=had_tau1_vis - tau1_neut_pion;

    const TLorentzVector COM = tau0_char_pion + tau1_char_pion + tau0_neut_pion + tau1_neut_pion;

    const double upsilon0 = tau0_char_pion.E()/(tau0_char_pion+tau0_neut_pion).E();
    const double upsilon1 = tau1_char_pion.E()/(tau1_char_pion+tau1_neut_pion).E();

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
        psi += (M_PI / 2);
    }
    // correct phase to be witin -pi to pi
    while (psi > M_PI) 
    {
        psi -= 2 * M_PI;
    }
    while (psi < -M_PI) 
    {
        psi += 2 * M_PI;
    }
    
    // std::cout << "Truth Level Psi: " << psi << "\n" << std::endl;

    return psi;

}

float hadhadRecoPsi(
            std::vector<TLorentzVector>* tau0_track,  // tau0_charged_tracks_p4
            std::vector<TLorentzVector>* tau1_track,  // tau1_charged_tracks_p4
            std::vector<TLorentzVector>* tau0_neut,   // tau0_pi0s_p4
            std::vector<TLorentzVector>* tau1_neut    // tau1_pi0s_p4
            ){

    TLorentzVector tau0_neut_pion_reco;          // pulls from pi0_p4
    TLorentzVector tau1_neut_pion_reco;
    TLorentzVector tau0_char_pion_reco;          // pulls from charged_tracks
    TLorentzVector tau1_char_pion_reco;

    tau0_char_pion_reco.SetPtEtaPhiE((*tau0_track)[0].Pt(), (*tau0_track)[0].Eta(), (*tau0_track)[0].Phi(), (*tau0_track)[0].E());
    tau1_char_pion_reco.SetPtEtaPhiE((*tau1_track)[0].Pt(), (*tau1_track)[0].Eta(), (*tau1_track)[0].Phi(), (*tau1_track)[0].E());

    tau0_neut_pion_reco.SetPtEtaPhiE((*tau0_neut)[0].Pt(), (*tau0_neut)[0].Eta(), (*tau0_neut)[0].Phi(), (*tau0_neut)[0].E());
    tau1_neut_pion_reco.SetPtEtaPhiE((*tau1_neut)[0].Pt(), (*tau1_neut)[0].Eta(), (*tau1_neut)[0].Phi(), (*tau1_neut)[0].E());

    const TLorentzVector COM_reco = tau0_char_pion_reco + tau1_char_pion_reco + tau0_neut_pion_reco + tau1_neut_pion_reco;

    const double upsilon0_reco = tau0_char_pion_reco.E()/(tau0_char_pion_reco+tau0_neut_pion_reco).E();
    const double upsilon1_reco = tau1_char_pion_reco.E()/(tau1_char_pion_reco+tau1_neut_pion_reco).E();

    // compute_psi_hh_basic

    
    TLorentzVector beam_reco;
    beam_reco.SetPxPyPzE(0., 0., 6.5e6, 6.5e6);
    
    const TVector3 boostVector_reco = COM_reco.BoostVector();

    tau0_char_pion_reco.Boost((-1.)*boostVector_reco);
    tau1_char_pion_reco.Boost((-1.)*boostVector_reco);
    beam_reco.Boost((-1.)*boostVector_reco); 

    // rotate system and obtain psi

    double phi_tau0_char_pion_reco = tau0_char_pion_reco.Phi();
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
        psi_reco += (M_PI / 2);

    }

    // correct phase to be witin -pi to pi

    while (psi_reco > M_PI) 
    {
        psi_reco -= 2 * M_PI;
    }
    while (psi_reco < -M_PI) 
    {
        psi_reco += 2 * M_PI;
    }
    
    // std::cout << "Reco Level Psi: " << psi_reco << "\n" << std::endl;

    return psi_reco;
}

int main() {

    // Select sample (select number of events above for loop)

    // polarized
    /*
    TFile f("/eos/user/s/svenetia/taucp_ntuples_with_hadhad/mc/hadhad/mc20e/nom/user.svenetia.Ztt_pythia02.mc20_13TeV.802365.Py8_DYtt_60M250_hadhad.PHYS.r13145_p6266.w_0_Zt/user.svenetia.46611689._000001.ZttHadHad.root"); // read input file
    TFile fout1("psiTruthPol.root", "recreate"); 
    TFile fout2("psiRecoPol.root", "recreate");
    string truthFilestem = "./psiTruthPol.csv";
    string recoFilestem = "./psiRecoPol.csv";    
    */

    // unpolarized
    
    TFile f("/eos/user/s/svenetia/taucp_ntuples_with_hadhad/mc/hadhad/mc20e/nom/user.svenetia.Ztt_test02.mc20_13TeV.602984.PhPy8_Ztt_UnPol_had30had20.PHYS.r13145_p6490.w_0_Zt/user.svenetia.46373438._000001.ZttHadHad.root");
    TFile fout1("psiTruthUnPol.root", "recreate"); 
    TFile fout2("psiRecoUnPol.root", "recreate");
    string truthFilestem = "./psiTruthUnPol.csv";
    string recoFilestem = "./psiRecoUnPol.csv";
    

    // ********************************************************************************************************************* //

    // Initialized variables

    TTree *input_tree = (TTree*)f.Get("NOMINAL");        // "Get" the NOMINAL tree from the file and assign it to local variable "input_tree"

    Int_t n_taus;                                       
    Int_t tau0_matched_n_charged_pion;
    Int_t tau0_matched_n_neutral_pion;
    UInt_t tau0_matched_isHadTau;
    Int_t tau1_matched_n_charged_pion;
    Int_t tau1_matched_n_neutral_pion;
    UInt_t tau1_matched_isHadTau;
    UInt_t tau0_n_charged_tracks;
    UInt_t tau1_n_charged_tracks;
    UInt_t tau0_pantau_decay_mode;
    UInt_t tau1_pantau_decay_mode;
    
    // Initialize TLorentzVector for each p4 needed for psi truth calc
    TLorentzVector* tau0_matched_vis_p4 = new TLorentzVector();             // truth variables
    TLorentzVector* tau0_matched_vis_neutral_p4 = new TLorentzVector();
    TLorentzVector* tau1_matched_vis_p4 = new TLorentzVector();
    TLorentzVector* tau1_matched_vis_neutral_p4 = new TLorentzVector();

    std::vector<TLorentzVector>* tau0_charged_tracks_p4 = new std::vector<TLorentzVector>();          // reco variables
    std::vector<TLorentzVector>* tau0_pi0s_p4 = new std::vector<TLorentzVector>();
    std::vector<TLorentzVector>* tau1_charged_tracks_p4 = new std::vector<TLorentzVector>();
    std::vector<TLorentzVector>* tau1_pi0s_p4 = new std::vector<TLorentzVector>();

    // Point to location in tree

    // Selections
    input_tree->SetBranchAddress("tau0_matched_n_charged_pion", &tau0_matched_n_charged_pion); 
    input_tree->SetBranchAddress("tau0_matched_n_neutral_pion", &tau0_matched_n_neutral_pion);
    input_tree->SetBranchAddress("tau0_matched_isHadTau", &tau0_matched_isHadTau);
    input_tree->SetBranchAddress("tau1_matched_n_charged_pion", &tau1_matched_n_charged_pion);
    input_tree->SetBranchAddress("tau1_matched_n_neutral_pion", &tau1_matched_n_neutral_pion);
    input_tree->SetBranchAddress("tau1_matched_isHadTau", &tau1_matched_isHadTau);
    input_tree->SetBranchAddress("tau0_n_charged_tracks", &tau0_n_charged_tracks); 
    input_tree->SetBranchAddress("tau1_n_charged_tracks", &tau1_n_charged_tracks);
    input_tree->SetBranchAddress("n_taus", &n_taus); 
    input_tree->SetBranchAddress("tau0_pantau_decay_mode", &tau0_pantau_decay_mode); 
    input_tree->SetBranchAddress("tau1_pantau_decay_mode", &tau1_pantau_decay_mode);

    // Truth Variables
    input_tree->SetBranchAddress("tau0_matched_vis_p4", &tau0_matched_vis_p4);                      
    input_tree->SetBranchAddress("tau0_matched_vis_neutral_p4", &tau0_matched_vis_neutral_p4);
    input_tree->SetBranchAddress("tau1_matched_vis_p4", &tau1_matched_vis_p4);
    input_tree->SetBranchAddress("tau1_matched_vis_neutral_p4", &tau1_matched_vis_neutral_p4);

    // Reco Variables
    input_tree->SetBranchAddress("tau0_charged_tracks_p4", &tau0_charged_tracks_p4);                      
    input_tree->SetBranchAddress("tau0_pi0s_p4", &tau0_pi0s_p4);
    input_tree->SetBranchAddress("tau1_charged_tracks_p4", &tau1_charged_tracks_p4);
    input_tree->SetBranchAddress("tau1_pi0s_p4", &tau1_pi0s_p4);

    TRandom3 rand;  // Random number for choosing a selection of events                              

    std::vector<float> psi_truth_vec; // for csv export
    std::vector<float> psi_reco_vec;  // for csv export

    // Initialize histograms
    TH1* h1 = nullptr; 
    TH1* h2 = nullptr;
    h1 = new TH1D("h1", "Psi Truth", 30, -3.14, 3.14);
    h2 = new TH1D("h2", "Psi Reco", 30, -3.14, 3.14);

    std::cout << "Starting loop..." << std::endl;

    // select number of events
    int nEntries = input_tree->GetEntries(); // all events
    // int nEntries = 50;                    // selection of events - if want random, make required change in "main" block

    for ( int randCounter = 0 ; randCounter < nEntries ; randCounter++ ) { // iterate from 0 to the max number of events you want to select

        input_tree->GetEvent(randCounter);                            // get the information for the specific event
        
        // if selecting random events, uncomment below
        /*
        int i = rand.Uniform(input_tree->GetEntries());     // chooses a random number from the entries in the input tree
        input_tree->GetEvent(i);   
        */

        // truth selections
        if ((tau0_matched_n_charged_pion==1) && (tau0_matched_n_neutral_pion==1) && (tau0_matched_isHadTau==1) && (tau1_matched_n_charged_pion==1) && (tau1_matched_n_neutral_pion==1) && (tau1_matched_isHadTau==1)){

        // Calculate truth and reco psi
        float psi_truth_result = hadhadTruthPsi(
                                        tau0_matched_vis_p4,
                                        tau1_matched_vis_p4,
                                        tau0_matched_vis_neutral_p4,
                                        tau1_matched_vis_neutral_p4
                                        );

        psi_truth_vec.push_back(psi_truth_result);
        h1->Fill(psi_truth_result);

        float psi_reco_result = hadhadRecoPsi(
                                        tau0_charged_tracks_p4,
                                        tau1_charged_tracks_p4,
                                        tau0_pi0s_p4,
                                        tau1_pi0s_p4
                                        );

        psi_reco_vec.push_back(psi_reco_result);
        h2->Fill(psi_reco_result);

        }

    }

    // Helpful print statements for debugging

    /*
    for ( float counter = 0 ; counter < psi_reco_vec.size() ; counter++ ) {
        std::cout << psi_reco_vec[counter] << std::endl;
    }
    std::cout << "Length of truth psi: " << psi_truth_vec.size() << std::endl;
    std::cout << "Length of reco psi: " << psi_reco_vec.size() << std::endl;
	*/

    std::ofstream reco_output_file(truthFilestem);
    std::ostream_iterator<float> reco_output_iterator(reco_output_file, "\n");
    std::copy(std::begin(psi_reco_vec), std::end(psi_reco_vec), reco_output_iterator);

    std::ofstream truth_output_file(recoFilestem);
    std::ostream_iterator<float> truth_output_iterator(truth_output_file, "\n");
    std::copy(std::begin(psi_truth_vec), std::end(psi_truth_vec), truth_output_iterator);

    // Drawing Histograms

    std::cout << "Plotting truth psi!" << std::endl;

    fout1.cd();

    std::cout << "writing..." << std::endl;
    h1->GetXaxis()->SetTitle("Truth Psi");
    h1->GetYaxis()->SetTitle("Entries");
    h1->Write();
    std::cout << "closing..." << std::endl;
    fout1.Close();
    std::cout << "Done." << std::endl;

    // Reco Psi

    std::cout << "Plotting reco psi!" << std::endl;
    
    fout2.cd();
    
    std::cout << "writing reco psi..." << std::endl;
    h2->GetXaxis()->SetTitle("Reco Psi");
    h2->GetYaxis()->SetTitle("Entries");
    h2->Write();
    std::cout << "closing reco psi..." << std::endl;
    fout2.Close();
    std::cout << "Done." << std::endl;

    return 0;
    }
