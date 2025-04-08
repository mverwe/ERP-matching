#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <cstdio>   // needed for io
#include <chrono>
#include <map>
#include <random>
#include <string>
#include <algorithm>

#include "ProgressBar.h"

using namespace std;

int main() {

  const int np = 42; //number of projects

  std::random_device rd;  // Used to seed the generator
  std::mt19937 gen(rd()); // Mersenne Twister generator
  //std::uniform_real_distribution<> dis(0.0, 1.0);
  std::normal_distribution dis{0.7, 0.15};

  //---------------------------------------------------
  // generate popularity weight factors for each project
  //--------------------------------------------------- 
  
  std::vector<double> weight_p;
  for(int i = 0; i<np; ++i) {
    //double rnd = 1.;//dis(gen);
    double rnd = dis(gen);
    weight_p.push_back(rnd);
    //std::cout << "i: "<< i << " prob: " << rnd << std::endl;
  }

  auto sum_weights = std::reduce(weight_p.begin(), weight_p.end());
  std::cout << "sum_weights: " << sum_weights << std::endl;
  for(int i = 0; i<np; ++i) {
    std::cout << "i: "<< i << " prob: " << weight_p[i] << " norm: " << weight_p[i] / sum_weights << std::endl;
  }

  
  //std::cout << "\n\n\n" << std::endl;
  //for(double d : weight_p) std::cout << d << std::endl;  

  //---------------------------------------------------
  // generate top 3 pick for each group
  //---------------------------------------------------
  std::discrete_distribution<int> dd{weight_p.begin(), weight_p.end()}; // create distribution
  std::map<int, int> map;
  std::vector<std::vector<int>> top_picks;
  for(int i = 0; i<np; ++i) {
    //for(int i = 0; i<1e4; ++i) {
    std::vector<int> sel;
    for(int j =0; j<3; ++j) {
      std::cout << "group " << i << "  project " << j << std::endl;
      int irnd = dd(gen);
      //if(j==0) sel.push_back(irnd);
      std::cout << "sel.size(): " << sel.size() << std::endl;
      for (auto k : sel)
        std::cout << k << ' ';
      std::cout << std::endl;
      int cnt = std::count(sel.begin(), sel.end(), irnd);
      std::cout << "count: " << cnt << std::endl;
      while(cnt>0) { //prevent picking same project
        //std::cout << irnd <<  "=============================== already present" << std::endl;
        irnd = dd(gen);
        cnt = std::count(sel.begin(), sel.end(), irnd);
      }
      sel.push_back(irnd);
      ++map[irnd];
      std::cout << "i: " << i << " j: " << j << "  irnd: " << irnd << std::endl;
      //sel.push_back(irnd);
    }
    std::cout << "number of picks: " << sel.size() << std::endl;
    top_picks.push_back(sel);
  }

  
  
  // for (const auto& [num, count] : map)
  //   std::cout << num << " generated " << std::setw(4) << count << " times with probablity " << count/(double)(np*3) << std::endl;

  //---------------------------------------------------
  // put top 3 in order for each group
  //---------------------------------------------------
  for (const auto& [key, value] : map)
    std::cout << '[' << key << "] = " << value << "; ";
  std::cout << std::endl;
  
  for(int i = 0; i<np; ++i) {
    std::vector<int> sel = top_picks.at(i);
    std::vector<double> prob;
    for(int j =0; j<3; ++j) {
      //std::cout << "i: " <<i << " j: " << j << " map: " << map[i,j] << std::endl;
      prob.push_back(weight_p.at(sel.at(j)));
    }
  }

  //output text file
  ofstream fout;
  const char *dir = getenv("PWD");
  //std::string outFileName = std::format("%s/top_picks.dat",dir);

  char buff[100];
  snprintf(buff, sizeof(buff), "%s/top_picks_gaus.dat", dir);
  std::string outFileName  = buff;
  
  fout.open(outFileName);
  for(int i = 0; i<np; ++i) {
    std::vector<int> sel = top_picks.at(i);
    fout << "# group " << i << endl;
    for(int j =0; j<3; ++j) {
      fout << sel.at(j) << std::endl;
    }
  }
  fout.close();
  
  
  // std::discrete_distribution<> d({0.04, 0.8, 0.01, 0.03, 0.1, 0.9});
  // std::map<int, int> m;
  // for(int n=0; n<100000; ++n) {
  //   ++m[d(gen)];
  // }
  // for(auto p : m) {
  //   std::cout << p.first+1 << " generated " << p.second << " times\n";
  // } 
 

}
