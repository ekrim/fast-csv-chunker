#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <typeinfo>

using std::cout;
using std::endl;
using std::vector;
using std::string;


void readme(){
  cout << "./chunk_csv <csv_file> <num_chunks>" << endl;
}


template <typename T>
void print(vector<T>& vec){
  for (auto const& el : vec){
    cout << el << endl;
  }
}


void parse_cols(vector<string>& col, string all_cols){
  size_t start = 0;
  size_t cnt = 0;
  for (int i = 0; i < all_cols.length(); ++i){
    if (all_cols[i] == ','){
      string col_name(all_cols, start, cnt);
      col.push_back(col_name);
      cnt = 0;
      start = i + 1;
    } else {
      ++cnt;
    }
  }
}


int main(int argc, char **argv){
  if (argc != 3){
    readme(); 
    return 1;
  }

  std::ifstream myfile(argv[1]);

  string line, header;
  
  getline(myfile, header);
  
  vector<string> col_vec;
  parse_cols(col_vec, header);
  
  print(col_vec);

  
  

  while( getline(myfile, line)){}
 
  myfile.close();
  return 0;
}
