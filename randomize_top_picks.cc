#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <cstdio>   // needed for io
#include <chrono>
#include <map>
#include <random>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>

using namespace std;

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

int main(int argc, char *argv[]) {

  //std::copy(argv, argv + argc, std::ostream_iterator<char *>(std::cout, "\n"));

  if(argc != 3) {
    std::cout << "command line input missing. usage: ./randomize_top_picks inputfile.dat outputfile.dat" << std::endl;
    return 1;
  }
  bool verbose = true;//false;//true;//
  
  std::ifstream infile(argv[1]);
 
  std::vector<string> header;
  std::vector<std::vector<int>> top_picks; //length of number of groups
  int counter = -1;
  std::vector<int> sel;
  std::vector<int> groups;

  std::vector<int> proj;
  for(int i = 0; i<34; ++i) {
    proj.push_back(0);
  }
  
  std::string line;
  if ( infile.is_open() ) { // always check whether the file is open
    while (  std::getline(infile,line) ) { 
      //int igroup = -1;

      if (line.rfind("#", 0) == 0) {
        header.push_back(line);
        std::string strnum = first_numberstring(line);
        int igroup = atoi(strnum.c_str());
        groups.push_back(igroup);
        if(counter>-1) {
          top_picks.push_back(sel);
        }
        sel.clear();
        counter++;
      } else {
        int isel = atoi(line.c_str());
        sel.push_back(isel);
        proj[isel-1]+=1;
      }
    }
    //push last set of picks
    if(counter<=top_picks.size()) top_picks.push_back(sel);
  }

  
  std::vector<int> shuffled;
  for(int i = 0; i<top_picks.size(); ++i) {
    shuffled.push_back(i);
  }
  std::random_device rd;
  std::mt19937 g(rd());
 
  std::shuffle(shuffled.begin(), shuffled.end(), g);

  if(verbose) {
    std::copy(shuffled.begin(), shuffled.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << '\n';
  }

  

  //output text file
  ofstream fout;
  const char *dir = getenv("PWD");
  char buff[100];
  snprintf(buff, sizeof(buff), "%s/%s", dir,argv[2]);
  std::string outFileName  = buff;

  fout.open(outFileName);

  for(int i = 0; i<shuffled.size(); ++i) {
    int iline = shuffled.at(i);
    fout << header.at(iline) << std::endl;
    std::vector<int> picks = top_picks.at(iline);
    for(int j = 0; j<picks.size(); ++j) {
      fout << picks.at(j) << std::endl;
    }
  }
  
  std::sort(groups.begin(), groups.end());
  if(verbose) {
    std::cout << "groups: " << std::endl;
    std::copy(groups.begin(), groups.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << '\n';
  }
    
  //check which groups are missing
  std::vector<int> missing;
  for(int i = 1; i<35; ++i) {
    if (std::binary_search(groups.begin(), groups.end(), i)) {
      if(verbose) std::cout << "group " << i << " submitted preferences" << std::endl;
    }
    else
      missing.push_back(i);
  }
  
  if(verbose) {
    std::cout << "groups that didn't sumbmit preferences: " << std::endl;
    std::copy(missing.begin(), missing.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << '\n';

    std::cout << "number of groups: " << groups.size() << " + " << missing.size() << " = " << groups.size() +  missing.size() << std::endl;
  }

  //generate random picks for mising groups
  std::vector<double> weights;
  for(int i = 0; i<34; ++i) {
    if(proj[i]==0) weights.push_back(1.);
    else
      weights.push_back(1./(double)proj[i]);
  }
  if(verbose) {
    std::copy(proj.begin(), proj.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << '\n';
    std::copy(weights.begin(), weights.end(), std::ostream_iterator<double>(std::cout, " "));
    std::cout << '\n';
  }
  
  std::discrete_distribution<> d(weights.begin(), weights.end());
  for(int imis : missing) {
    fout << "# group " << imis << " random" << std::endl;
    fout << d(g)+1 << std::endl;
    fout << d(g)+1 << std::endl;
    fout << d(g)+1 << std::endl;
    //int iproj = d(g);
    //std::cout << imis << " proj: " << iproj << "  pop: " << weights[iproj] << std::endl;
  }

  //print project picks
  int np = 0;
  for(int i = 0; i<34; ++i) {
    std::cout << "project " << i+1 << ": " << proj[i] << " picked" << std::endl;
    if(proj[i]>0) np++;
  }
  std::cout << "number of projects that was picked at least once: " << np << std::endl;
    
  fout.close();
  
}
