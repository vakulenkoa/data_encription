#include <iostream>
#include <string.h>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <functional>
#include <limits>
#include <string>

namespace
{
	size_t _record_size = 80;
}

unsigned int HashData(char* record_data)
{
	std::string str_data(record_data);
	return std::hash<std::string>()(str_data) % std::numeric_limits<unsigned int>::max();
}

int main(int argc, char* argv[])
{
	//check args
	if (argc < 4 || argc > 5 || (strcmp("help", argv[1]) == 0))
	{
		std::cout << "The program accept 3 - 4 arguments only." << std::endl;
		std::cout << "First one: path to input text file." << std::endl;
		std::cout << "Second one: number of records to create." << std::endl;
		std::cout << "Third one: path to output data." << std::endl;
		std::cout << "Forth one (optional): seed number (0-4294967296)." << std::endl;
		std::cout << "For example: ./data_creator input_data.txt 10000 out_data.dat 890990897" << std::endl;
		return 1;
	}

	unsigned long records_number = std::stoul(argv[2]);

	size_t seed_number;
	if (argc == 5)
		seed_number = std::stoul(argv[4]);
	else
		seed_number = static_cast<size_t>(time(NULL));

	//read input file
	std::ifstream input_fl;
	input_fl.open(argv[1], std::ios::binary | std::ios::ate);
	if (!input_fl.is_open())
	{
		std::cout << "Unable to open " << argv[1] << " file." << std::endl;
		return 2;
	}

	size_t input_file_size = static_cast<size_t>(input_fl.tellg());
	srand(seed_number);

	std::ofstream output_fl;
	output_fl.open(argv[3], std::ios::binary);
	if (!output_fl.is_open())
	{
		std::cout << "Unable to open " << argv[1] << " file to write." << std::endl;
		return 3;
	}

	char* record_data = new char[_record_size + 1];
	record_data[_record_size] = '\0';
	unsigned int record_hash;
	for (unsigned long rn = 0; rn < records_number; ++rn)
	{
		std::streampos in_fl_offset = rand() % (input_file_size - _record_size);
		input_fl.seekg(in_fl_offset);
		input_fl.read(record_data, _record_size);
		record_hash = HashData(record_data);
		output_fl.write(reinterpret_cast<char *>(&record_hash), sizeof(record_hash));
		output_fl.write(record_data, _record_size);
	}

	delete[] record_data;
	output_fl.close();
	input_fl.close();

	std::cout << "Process complete." << std::endl;
	return 0;
}