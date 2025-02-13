This repository contains a tool to easily extract the longest reads from a fasta/fastq file. It is used like this:
```
get_longest -f fastq_file_path -g genome_size (in Mb) -o output_file [-c coverage]
```
By default, `-c` is set to 30.

# Build
`get_longest` is written in C++ so you will need a compiler which supports C++11. To build the executable:
```
git clone https://github.com/Genoscope-LBGB/get_longest_reads
cd get_longest_reads
make
``` 
