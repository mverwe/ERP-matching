#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <istream>
#include <stdio.h>
#include <stdlib.h>
#include <cstdio>   // needed for io
#include <chrono>
#include <map>
#include <random>
#include <string>

using namespace std;

class CSVRow
{
    public:
        std::string_view operator[](std::size_t index) const
        {
            return std::string_view(&m_line[m_data[index] + 1], m_data[index + 1] -  (m_data[index] + 1));
        }
        std::size_t size() const
        {
            return m_data.size() - 1;
        }
        void readNextRow(std::istream& str)
        {
            std::getline(str, m_line);

            m_data.clear();
            m_data.emplace_back(-1);
            std::string::size_type pos = 0;
            while((pos = m_line.find(',', pos)) != std::string::npos)
            {
                m_data.emplace_back(pos);
                ++pos;
            }
            // This checks for a trailing comma with no data after it.
            pos   = m_line.size();
            m_data.emplace_back(pos);
        }
    private:
        std::string         m_line;
        std::vector<int>    m_data;
};

std::istream& operator>>(std::istream& str, CSVRow& data)
{
    data.readNextRow(str);
    return str;
}

class CSVIterator
{   
    public:
        typedef std::input_iterator_tag     iterator_category;
        typedef CSVRow                      value_type;
        typedef std::size_t                 difference_type;
        typedef CSVRow*                     pointer;
        typedef CSVRow&                     reference;

        CSVIterator(std::istream& str)  :m_str(str.good()?&str:nullptr) { ++(*this); }
        CSVIterator()                   :m_str(nullptr) {}

        // Pre Increment
        CSVIterator& operator++()               {if (m_str) { if (!((*m_str) >> m_row)){m_str = nullptr;}}return *this;}
        // Post increment
        CSVIterator operator++(int)             {CSVIterator    tmp(*this);++(*this);return tmp;}
        CSVRow const& operator*()   const       {return m_row;}
        CSVRow const* operator->()  const       {return &m_row;}

        bool operator==(CSVIterator const& rhs) {return ((this == &rhs) || ((this->m_str == nullptr) && (rhs.m_str == nullptr)));}
        bool operator!=(CSVIterator const& rhs) {return !((*this) == rhs);}
    private:
        std::istream*       m_str;
        CSVRow              m_row;
};

class CSVRange
{
    std::istream&   stream;
    public:
        CSVRange(std::istream& str)
            : stream(str)
        {}
        CSVIterator begin() const {return CSVIterator{stream};}
        CSVIterator end()   const {return CSVIterator{};}
};

int main() {

  std::ifstream infile("/Users/mverweij/wrk/UU/ExperimentalDesign/ProjectAllocation/ERP_test/ERPtest_April8_2025_07.08.csv");

  // std::string line;
  


  //output text file
  ofstream fout;
  const char *dir = getenv("PWD");
  char buff[100];
  snprintf(buff, sizeof(buff), "%s/top_picks_csv.dat", dir);
  std::string outFileName  = buff;
  
  fout.open(outFileName);

  
  int counter = -1;
  int igroup = 0;
  for(auto& row: CSVRange(infile)) {
    counter++;
    std::cout << "top 3 of group " << counter << " email contactperson: " << row[18] << std::endl;
    std::cout << "1: " << row[19] << "\n";
    std::cout << "1: " << row[20] << "\n";
    std::cout << "1: " << row[21] << "\n";
    if(counter>2) {
      fout << "# group " << igroup << " " << row[18] << std::endl;
      fout << "1: " << row[19] << "\n";
      fout << "1: " << row[20] << "\n";
      fout << "1: " << row[21] << "\n";
      igroup++;
    }
  }
  fout.close();
      
}
