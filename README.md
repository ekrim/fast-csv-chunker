### Fast CSV Chunker

Assumes first row contains the headers. No delimiter type needed... but will break if you have endline symbols as entries. 

TODO:
  - Specify primary key, and keep all rows with the same key in the same shard.

#### Build

```
g++ -std=c++11 -o chunker main.cc
```

#### Usage

```
./chunker <csv_file> <num_chunks>
```

Output:
```
root
  |- chunk_00.csv
  ...
  |- chunk_49.csv
```

