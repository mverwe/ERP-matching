#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <cstdio>   // needed for io
#include <chrono>
#include <map>
#include <random>
#include <string>

#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TParameter.h"
#include "TTree.h"

#include "ProgressBar.h"

using namespace std;

int run(double prob1 = 40., double prob2 = 35., double prob3 = 25., int trial = 0, std::string strfile = "/Users/mverweij/wrk/UU/ExperimentalDesign/ERP-matching/top_picks_shuffle.dat");

std::string first_numberstring(std::string const & str)
{
  char const* digits = "0123456789";
  std::size_t const n = str.find_first_of(digits);
  if (n != std::string::npos)
    {
      std::size_t const m = str.find_first_not_of(digits, n);
      return str.substr(n, m != std::string::npos ? m-n : m);
    }
  return std::string();
}

//int main(double prob1 = 40., double prob2 = 35., double prob3 = 25.) {

int main(int argc, char *argv[]) {

  std::copy(argv, argv + argc, std::ostream_iterator<char *>(std::cout, "\n"));

  if(argc != 3) {
    std::cout << "command line input missing. usage: ./matchmaking inputfile.dat 10" << std::endl;
    return 1;
  }

  std::string instr(argv[1]);
  
  int ntrials = atoi(argv[2]);//10000;
  for(int i = 0; i<ntrials; ++i) {
    //run(40.,35.,25.,i);
    //run(50.,30.,20.,i);
    // run(60.,20.,10.,i);
    //run(70.,15.,5.,i);
    run(58.,26.,16.,i,instr); //based on output of run(70.,15.,5.,i);
  }
 
  return 0;
}

int run(double prob1, double prob2, double prob3, int trial, std::string strfile) {

  //Assign probability for 1st, 2nd, 3rd. Deal with random assignment later?
  double prob[3] = {prob1, prob2, prob3};//{40.,35.,25.};
   
  std::ifstream infile(strfile.c_str());
  //std::ifstream infile("/Users/mverweij/wrk/UU/ExperimentalDesign/ProjectAllocation/top_picks_gaus.dat");

  std::string line;

  std::vector<std::vector<int>> top_picks; //length of number of groups
  int counter = -1;
  std::vector<int> sel;
  std::vector<int> groupnr;
  std::vector<string> header;
  //sel.reserve(3);
  if ( infile.is_open() ) { // always check whether the file is open
    while (  std::getline(infile,line) ) { 
      int igroup = -1;

      if (line.rfind("#", 0) == 0) {
        header.push_back(line);
        std::string strnum = first_numberstring(line);
        igroup = atoi(strnum.c_str());
        groupnr.push_back(igroup);
        if(counter>-1) {
          top_picks.push_back(sel);
        }
        sel.clear();
        counter++;
      } else {
        int isel = atoi(line.c_str());
        sel.push_back(isel);
      }
    }
    //push last set of picks
    if(counter<=top_picks.size()) top_picks.push_back(sel);
  }

  
  TH1D *hTopPicks = new TH1D("hTopPicks","hTopPicks",50,0.5,50.5);

  for(int i = 0; i<top_picks.size(); ++i) {
    std::vector<int> sel = top_picks.at(i);
    for(int j = 0; j<sel.size(); ++j) {
      hTopPicks->Fill(sel.at(j));
    }
  }
 
  //Need to pick project for each group

  std::random_device rd;  // Used to seed the generator
  std::mt19937 gen(rd()); // Mersenne Twister generator
  std::uniform_real_distribution<> dis(0., 100.);

  TH1D *hRndNum = new TH1D("hRndNum","hRndNum",100,0.,100.);
  
  TH1D *hSelProj = new TH1D("hSelProj","hSelProj",5,-0.5,4.5);
  TH1D *hSelPicks = new TH1D("hSelPicks","hSelPicks",50,0.5,50.5);
  
  TH1D *hRndProj = new TH1D("hRndProj","hRndProj",5,-0.5,4.5);
  TH1D *hRndPicks = new TH1D("hRndPicks","hRndPicks",50,0.5,50.5);

  TH1D *hAvgHappiness = new TH1D("hAvgHappiness","hAvgHappiness",100,0.,100.);
  TH2D *hAvgHapFrac1 = new TH2D("hAvgHapFrac1","hAvgHapFrac1",100,0.,100.,40,0.,1.);
  TH2D *hAvgHapFrac2 = new TH2D("hAvgHapFrac2","hAvgHapFrac2",100,0.,100.,40,0.,1.);
  TH2D *hAvgHapFrac3 = new TH2D("hAvgHapFrac3","hAvgHapFrac3",100,0.,100.,40,0.,1.);
  TH2D *hAvgHapFrac4 = new TH2D("hAvgHapFrac4","hAvgHapFrac4",100,0.,100.,40,0.,1.);
  

  TH1D *hProb = new TH1D("hProb","hProb",5,-0.5,4.5);
  for(int i = 0; i<4; ++i)
    hProb->SetBinContent(i+1,prob[i]);
  
  std::vector<int> picked; //should be length of number of available projects
  picked.reserve(top_picks.size());
  std::vector<int> selproj;//bookkeep if group got Nth pick or random. should be length of the number of groups
  selproj.reserve(top_picks.size());
  std::vector<int> match; //selected project for group of students
  match.reserve(top_picks.size());
  
  for(int i = 0; i<top_picks.size(); ++i) {
    picked.push_back(0);
    match.push_back(-1);
  }
  
  std::srand(std::time(0)); // use current time as seed for random generator
  
  for(int i = 0; i<top_picks.size(); ++i) { //loop over student groups
    std::vector<int> sel = top_picks.at(i);
    double rnd = dis(gen);
    hRndNum->Fill(rnd);
    int isel = -1;
    if(rnd<prob[0])      isel = 0;
    else if(rnd<(prob[0]+prob[1])) isel = 1;
    else                 isel = 2;
    hRndProj->Fill(isel);

    int psel = sel.at(isel);
    
    hRndPicks->Fill(psel);

    if(picked[psel]>0) {
      double rnd = dis(gen);
      if(rnd<(prob[0]+prob[1])) isel = 1;
      else                      isel = 2;
      psel = sel.at(isel);

      if(picked[psel]>0) {
        isel = 2;

        if(picked[psel]>0) {
          //need to select a project randomly
          isel = 3;
          while(picked[psel]>0) {
            psel = std::rand() % picked.size();
          }
        }
      }
    }
    selproj.push_back(isel);

    match[i] = psel;
    picked[psel]++;
    hSelProj->Fill(isel);
    hSelPicks->Fill(psel);
  }

  int happiness = 0;
  int score[4] = {100,80,70,20};//{100,80,60,10};
  for(int i = 0; i<selproj.size(); ++i) {
    happiness+=score[selproj.at(i)];
  }

  double avghap = (double)happiness/(double)selproj.size();
  
  hAvgHappiness->Fill(avghap);
  
  double ngroups = (double)top_picks.size();
  hAvgHapFrac1->Fill(avghap,hSelProj->GetBinContent(1)/ngroups);
  hAvgHapFrac2->Fill(avghap,hSelProj->GetBinContent(2)/ngroups);
  hAvgHapFrac3->Fill(avghap,hSelProj->GetBinContent(3)/ngroups);
  hAvgHapFrac4->Fill(avghap,hSelProj->GetBinContent(4)/ngroups);

  TParameter<double> *pAvgHap = new TParameter<double>;
  pAvgHap->SetVal((double)happiness/(double)selproj.size());

  TTree *treeMatch = new TTree("treeMatch","treeMatch");
  treeMatch->Branch("header",&header);
  treeMatch->Branch("groupnr", &groupnr);
  treeMatch->Branch("match",&match);
  treeMatch->Fill();
  
  TFile *fout = new TFile(Form("matchmaking_gaus_%.0f_%.0f_%.0f_%d.root",prob1,prob2,prob3,trial),"RECREATE");
  hProb->Write();
  hTopPicks->Write();
  hSelProj->Write();
  hSelPicks->Write();
  hRndProj->Write();
  hRndPicks->Write();
  hRndNum->Write();
  hAvgHappiness->Write();
  pAvgHap->Write("pAvgHap");
  hAvgHapFrac1->Write();
  hAvgHapFrac2->Write();
  hAvgHapFrac3->Write();
  hAvgHapFrac4->Write();

  treeMatch->Write();

  fout->Write();
  fout->Close();

  delete hProb;
  delete hTopPicks;
  delete hSelProj;
  delete hSelPicks;
  delete hRndProj;
  delete hRndPicks;
  delete hRndNum;
  delete hAvgHappiness;
  delete pAvgHap;
  delete hAvgHapFrac1;
  delete hAvgHapFrac2;
  delete hAvgHapFrac3;
  delete hAvgHapFrac4;
  
  return 0;
}


