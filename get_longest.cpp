/* Extract the longest 30x reads from a fastq file
* Usage: get_longest -f fastq -g genome_size
*/

#include "get_longest.hpp"


void usage()
{
	std::cout << std::endl << "Extract the longest 30x reads from a fastq file" << std::endl;
	std::cout << "Usage: get_longest -f fastq -g genome_size (in Mb) -o output_file [-c coverage]";
	exit(1);
}


// Parses command line arguments and defines input_file and genome_size parameters
void get_cmd_line_args(int argc, char *argv[], std::string *input_file, unsigned int *genome_size, std::string *output_file, unsigned int *coverage) 
{
	int i = 0;

	while(i < argc) 
	{
		std::string current_arg = argv[i];
		if(current_arg == "-h") 
		{
			usage();
		}
		else if(current_arg == "-f")
		{
			*input_file = std::string(argv[i+1]);
		}
		else if(current_arg == "-g")
		{
			*genome_size = atoi(argv[i+1]);
		}
		else if(current_arg == "-o")
		{
			*output_file = argv[i+1];
		}		
		else if(current_arg == "-c")
		{
			*coverage = atoi(argv[i+1]);
		}
		i++;
	}
}


// Detects if the input file is in fasta or fastq format
std::string detect_file_type(std::string input_file) 
{
	std::string line;

	std::ifstream sequence_file(input_file.c_str());
	getline(sequence_file, line);
	sequence_file.close();

	if(line[0] == '>') 
	{
		return("fasta");
	}
	else if(line[0] == '@') 
	{
		return("fastq");
	}
	else 
	{
		return("error");
	}
}


// Reads a fastq file and computes sequence sizes
void get_fastq_lengths(std::ifstream &sequence_file, unsigned int genome_size, std::string output_file, unsigned int coverage) 
{
	std::string line;
	std::string sequence_name;
	std::string sequence;
	std::vector<std::pair<std::string, int>> sequence_lengths;

	std::cout << "\nParsing fastq..." << "\n" << std::flush;
	sequence_file.seekg(0, std::ios::beg);
	while(getline(sequence_file, sequence_name)) 
	{
		getline(sequence_file, sequence);
		sequence_lengths.emplace_back(sequence_name, sequence.size());
		getline(sequence_file, line);
		getline(sequence_file, line);
	}

	std::cout << "Sorting..." << "\n" << std::flush;
	std::sort(sequence_lengths.begin(), sequence_lengths.end(), sort_by_second_element);

	std::cout << "Removing short sequences..." << "\n" << std::flush;
	std::unordered_set<std::string> longest_sequences;
	remove_short_sequences(genome_size, sequence_lengths, longest_sequences, coverage);

	std::cout << "Writing file..." << "\n" << std::flush;
	write_fastq(sequence_file, genome_size, sequence_lengths, output_file, longest_sequences, coverage);
}


// Makes it possible to sort a vector of pairs by second element
bool sort_by_second_element(const std::pair<std::string, int> &a, const std::pair<std::string,int> &b)
{ 
    return (a.second > b.second); 
} 


void remove_short_sequences(unsigned int genome_size, std::vector<std::pair<std::string, int>> &sequence_lengths, std::unordered_set<std::string> &longest_sequences, unsigned int coverage)
{
	double wanted_number_of_bases = double(genome_size) * 1000000 * coverage;
	double current_number_of_bases = 0;

	for(unsigned int i = 0; i < sequence_lengths.size(); ++i)
	{
		if(current_number_of_bases < wanted_number_of_bases)
		{
			current_number_of_bases += sequence_lengths[i].second;
			longest_sequences.emplace(sequence_lengths[i].first);
		}
		else 
		{
			break;
		}
	}
}


void write_fastq(std::ifstream &sequence_file, unsigned int genome_size, std::vector<std::pair<std::string, int>> &sequence_lengths, std::string output_file, std::unordered_set<std::string> &longest_sequences, unsigned int coverage)
{
	std::string sequence_name;
	std::string sequence;
	std::string comments;
	std::string quality;

	double wanted_number_of_bases = double(genome_size) * 1000000 * coverage;
	double current_number_of_bases = 0;

	std::ofstream out;
	out.open(output_file);

	sequence_file.clear();
	sequence_file.seekg(0, std::ios::beg);
	while(getline(sequence_file, sequence_name) && current_number_of_bases < wanted_number_of_bases)
	{
		getline(sequence_file, sequence);
		getline(sequence_file, comments);
		getline(sequence_file, quality);

		if(longest_sequences.count(sequence_name) > 0)
		{
			out << sequence_name << "\n" << sequence << "\n" << comments << "\n" << quality << "\n";
			current_number_of_bases += sequence.size();
		}
	}
	out.close();
}


int main(int argc, char *argv[]) 
{
	std::string input_file;
	std::string output_file;
	unsigned int genome_size = 0;
	unsigned int coverage = 30;

	// If no input file has been given, print usage and exit
	if(argc < 5) 
	{
		usage();
	}

	// Parse command line arguments to determine the input file and genome_size values
	get_cmd_line_args(argc, argv, &input_file, &genome_size, &output_file, &coverage);

	// Determine if file is a fasta or fastq
	std::string file_type = detect_file_type(input_file);

	// Compute metrics depending of file type
	std::ifstream sequence_file(input_file.c_str());
	if(file_type == "fasta") 
	{
		std::cout << std::endl << "File needs to be in fastq format. Exiting." << std::endl;
		exit(-1);	
	}
	else if(file_type == "fastq") 
	{
		get_fastq_lengths(sequence_file, genome_size, output_file, coverage);
	}
	else 
	{
		std::cout << std::endl << "Unable to determine if file is a fasta or fastq. Exiting." << std::endl;
		exit(-1);
	}
	sequence_file.close();

	return(0);
}
