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


std::string first_numberstring(std::string const & str) {
  char const* digits = "0123456789";
  std::size_t const n = str.find_first_of(digits);
  if (n != std::string::npos)
    {
      std::size_t const m = str.find_first_not_of(digits, n);
      return str.substr(n, m != std::string::npos ? m-n : m);
    }
  return std::string();
}

int main(int argc, char *argv[]) {

    std::copy(argv, argv + argc, std::ostream_iterator<char *>(std::cout, "\n"));

    if(argc != 2) {
        std::cout << "command line input missing. usage: ./visualize_survey inputfile.dat" << std::endl;
    return 1;
    }

    std::string instr(argv[1]);
    std::ifstream infile(instr.c_str());

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

  
  TH1D *hPick1= new TH1D("hPick1","hPick1",50,0.5,50.5);
  TH1D *hPick2= new TH1D("hPick2","hPick2",50,0.5,50.5);
  TH1D *hPick3= new TH1D("hPick3","hPick3",50,0.5,50.5);
  TH1D *hPick4= new TH1D("hPick4","hPick4",50,0.5,50.5);
  TH1D *hPick5= new TH1D("hPick5","hPick5",50,0.5,50.5);

  for(int i = 0; i<top_picks.size(); ++i) {
    std::vector<int> sel = top_picks.at(i);
    //std::cout << "group " << groupnr.at(i) << " header: " << header[i] << std::endl;
    if (header[i].find("random") != std::string::npos) {
      std::cout << "group " << groupnr.at(i) << " header: " << header[i] << std::endl;
      continue;
    }
    for(int j = 0; j<sel.size(); ++j) {
      if(j==0) hPick1->Fill(sel.at(j));
      else if(j==1) hPick2->Fill(sel.at(j));
      else if(j==2) hPick3->Fill(sel.at(j));
      else if(j==3) hPick4->Fill(sel.at(j));
      else if(j==4) hPick5->Fill(sel.at(j));

      //if(j==0) std::cout << "pick 1: " << sel.at(j) << std::endl;

      //print when open project (19 and 25) appears
        if(sel.at(j)==19 || sel.at(j)==25) {
            std::cout << "===" << std::endl;
            std::cout << "group " << groupnr.at(i) << " header: " << header[i] << std::endl;
            std::cout << "pick " << j+1 << ": " << sel.at(j) << std::endl;
            std::cout << "===" << std::endl;
        }
    }
  }

  TFile *fout = new TFile("survey_picks.root","RECREATE");
  hPick1->Write();
  hPick2->Write();
  hPick3->Write();
  hPick4->Write();
  hPick5->Write();

  fout->Write();
  fout->Close();
  
  return 0;
}