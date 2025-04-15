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

int main() {

  std::ifstream infile("/Users/mverweij/wrk/UU/ExperimentalDesign/ProjectAllocation/top_picks_gaus.dat");

  std::vector<string> header;
  std::vector<std::vector<int>> top_picks; //length of number of groups
  int counter = -1;
  std::vector<int> sel;

  std::string line;
  if ( infile.is_open() ) { // always check whether the file is open
    while (  std::getline(infile,line) ) { 
      int igroup = -1;

      if (line.rfind("#", 0) == 0) {
        header.push_back(line);
        std::string strnum = first_numberstring(line);
        igroup = atoi(strnum.c_str());
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

  
  std::vector<int> shuffled;
  for(int i = 0; i<top_picks.size(); ++i) {
    shuffled.push_back(i);
  }
  std::random_device rd;
  std::mt19937 g(rd());
 
  std::shuffle(shuffled.begin(), shuffled.end(), g);

  std::copy(shuffled.begin(), shuffled.end(), std::ostream_iterator<int>(std::cout, " "));
  std::cout << '\n';


  //output text file
  ofstream fout;
  const char *dir = getenv("PWD");
  char buff[100];
  snprintf(buff, sizeof(buff), "%s/top_picks_shuffle.dat", dir);
  std::string outFileName  = buff;

  fout.open(outFileName);

  for(int i = 0; i<shuffled.size(); ++i) {
    int iline = shuffled.at(i);
    fout << header.at(iline) << std::endl;
    std::vector<int> sel = top_picks.at(iline);
    for(int j = 0; j<sel.size(); ++j) {
      fout << sel.at(j) << std::endl;
    }
  }
  fout.close();
  
}
