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
  cout << "./chunk_csv <csv_file> <rows_per_chunk>" << endl;
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
  
  unsigned rows_per_chunk = atoi(argv[2]);
  string input_fn = argv[1];

  std::ifstream input_file(input_fn);

  string line, header;
  vector<string> col_vec;
  
  // get and process headers
  getline(input_file, header); 
  parse_cols(col_vec, header);
  print(col_vec);

  
  unsigned row_cnt = 0;
  unsigned chunk_cnt = 0;
  char output_fn[50];
  std::ofstream output_file;

  while( getline(input_file, line) ){
    if (row_cnt == 0){
      sprintf(output_fn, "chunk_%06d.csv", chunk_cnt);
      cout << output_fn << endl;
      output_file.open(output_fn);
      output_file << header << endl;
    }
   
    output_file << line << endl;

    if (row_cnt == rows_per_chunk-1){
      ++chunk_cnt;
      row_cnt = 0;
      output_file.close();
    } else {
      row_cnt += 1;
    }
  }
 
  input_file.close();
  return 0;
}
