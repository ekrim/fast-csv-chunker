#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <typeinfo>
#include <memory>
#include <map>

using std::cout;
using std::endl;
using std::vector;
using std::string;


const string HORZ_LINE(50, '-');
const char CHUNK_PREF[] = "chunk_";


void readme(){
  cout << "./chunk_csv <csv_file> <rows_per_chunk> <primary_key(opt)>" << endl;
}


template <typename T>
void print(vector<T>& vec){
  for (auto const& el : vec){
    cout << el << endl;
  }
}


/**
Args:
  col: empty vector to store column names
  all_cols: string which is the first line of the csv, containing
    column names and delimiters
  primary_key: string which is the column name of the primary key

Returns:
  col_idx: index of the column containing the primary key
  
*/
unsigned parse_cols(vector<string>& col, string& all_cols, string& primary_key){
  size_t start = 0;
  size_t L_cnt = 0;
  unsigned delim_cnt = 0;
  int col_idx = -1;
  for (int i = 0; i < all_cols.length(); ++i){
    if (all_cols[i] == ',' || i == all_cols.length()-1){
      L_cnt = i == all_cols.length()-1 ? L_cnt + 1 : L_cnt;
      string col_name(all_cols, start, L_cnt);
      if ( col_name.compare(primary_key) == 0){
        col_idx = delim_cnt;
      }
      col.push_back(col_name);
      ++delim_cnt;
      L_cnt = 0;
      start = i + 1;
    } else {
      ++L_cnt;
    }
  }
  
  return col_idx;
}


// return entry from column idx
string get_entry_by_index(string& row, int idx, char delim){
  unsigned delim_cnt = 0;
  unsigned last_delim_idx = 0;
  
  for (int i = 0; i < row.length(); ++i){
    if (row[i] == delim){
      if (delim_cnt == idx){
        return row.substr(last_delim_idx, i-last_delim_idx);
      } 
      ++delim_cnt;
      last_delim_idx = i + 1;
    }
  }
  return row.substr(last_delim_idx, row.length()-last_delim_idx);
  
}


int main(int argc, char **argv){
  string primary_key;
  if (argc == 3){
    primary_key = "";
  } else if (argc == 4){
    primary_key = argv[3];
  } else {
    readme(); 
    return 1;
  }
  
  string input_fn = argv[1];
  unsigned rows_per_chunk = atoi(argv[2]);
 
  std::ifstream input_file(input_fn);

  string line, header;
  vector<string> col_vec;
  
  // read and process header
  getline(input_file, header); 
  int col_idx = parse_cols(col_vec, header, primary_key);
  cout << "column names:" << endl << HORZ_LINE << endl;
  print(col_vec);
  cout << HORZ_LINE << endl;

  unsigned row_cnt = 0;
  unsigned chunk_cnt = 0;
  bool make_new_chunk = true;
  char output_fn[50];
  //vector<std::unique_ptr<std::ofstream>> output_file_vec;
  vector<std::ofstream*> output_file_vec;

  std::map<string, unsigned> keys_to_chunks;
  string this_key;

  while( getline(input_file, line) ){
    if (make_new_chunk){
      sprintf(output_fn, "%s%06d.csv", CHUNK_PREF, chunk_cnt);
      cout << output_fn << endl;

      // making new ofstream that will persist
      std::ofstream *p_new_ofstream = new std::ofstream(output_fn);
      
      output_file_vec.push_back(p_new_ofstream);
   
      //output_file_vec[chunk_cnt].open(output_fn);
      *output_file_vec[chunk_cnt] << header << endl;
      make_new_chunk = false;
    }
    
    // if we are not grouping by a primary key, add the row to the 
    // current ofstream
    if (col_idx == -1){
      *output_file_vec[chunk_cnt] << line << endl;
      ++row_cnt;

    // if we are grouping by a primary key, see which ofstream this 
    // row belongs to and add it there
    } else { 
      this_key = get_entry_by_index(line, col_idx, ',');
      if (keys_to_chunks.count(this_key) == 0){
        keys_to_chunks[this_key] = chunk_cnt;
      } 
      if (keys_to_chunks[this_key] == chunk_cnt){
        ++row_cnt;
      }
      *output_file_vec[keys_to_chunks[this_key]] << line << endl;
    }

    if (row_cnt >= rows_per_chunk-1){
      make_new_chunk = true;
      ++chunk_cnt;
      row_cnt = 0;
    }
  }
  
  for (auto p : output_file_vec){
    (*p).close();
    delete p;
    p = NULL;
  }  
 
  input_file.close();

  return 0;
}
