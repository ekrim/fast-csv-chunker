### Fast CSV Chunker

This is a tool written in C++11 to split CSV files too large for memory into chunks with a specified number of rows.

__Key grouping for aggregations__

Optionally, a foreign key can be specified such that all entries with the same key end up in the same chunk. This allows aggregations to be performed on each chunk, which is the motivating force behind this work. 

The alternative would be to create a table in SQL with an index on the desired key. Then, chunks could be produced with queries, but to get a chunk while also not splitting apart rows with the same key value would be difficult. 

Grouping matching keys has its drawbacks. The task of parsing the line for the key value is a computational burden. In addition, the resulting chunks will no longer have the specified number of rows, as rows might be added to any of the existing files, even after we have moved on to a new chunk. 

Assumptions:
- First row contains the headers
- Delimiter is ',' (TODO)

TODO:
- Named arg parsing and add delimiter
- Check for quotes (just doing a simple delimiter count now)
- Benchmarking

#### Build

Only dependencies are in the C++ standard library. 
```
g++ -std=c++11 -o chunker main.cc
```

#### Usage

__Case 1__: Simple chunking
```
./chunker <csv_file> <rows_per_chunk> 
```

Output:

```
  .
  ├── chunk_000000.csv
  ├── chunk_000001.csv
  ├── ...
  └── chunk_000049.csv
```

__Case 2__: Chunk with foreign key groupings

```
./chunker <csv_file> <rows_per_chunk> <key>
```

For example, to break the Iris dataset into a separate .csv file for each class, run:

```
./chunker iris.csv 10 species
```

The key is `species`, so for each row, the species is parsed, and the row is added to the file where the first instance of that species was added.
