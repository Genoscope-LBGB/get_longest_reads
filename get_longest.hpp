#include <vector> // std::vector
#include <iostream> // std::cout
#include <fstream> // std::ifstream
#include <string> // std::string
#include <algorithm> // std::sort()
#include <map> // std::array
#include <iomanip> // std::setw()
#include <unordered_set> // std::set

void usage();
void get_cmd_line_args(int, char*, std::string*, unsigned int*, unsigned int*); 
std::string detect_file_type(std::string);
bool sort_by_second_element(const std::pair<std::string, int>&, const std::pair<std::string,int>&);
void remove_short_sequences(unsigned int, std::vector<std::pair<std::string, int>>&, std::unordered_set<std::string>&, unsigned int);
void get_fasta_lengths(std::ifstream&, unsigned int, std::string); 
void get_fastq_lengths(std::ifstream&, unsigned int, std::string, unsigned int coverage); 
void write_fastq(std::ifstream&, unsigned int, std::vector<std::pair<std::string, int>>&, std::string, std::unordered_set<std::string>&, unsigned int);