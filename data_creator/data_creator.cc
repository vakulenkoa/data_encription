#include <iostream>
#include <string.h>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <limits>
#include <string>

namespace
{
	size_t _record_size = 80;
	unsigned long _flush_record_size = 10000;
	size_t _records_to_hash_dispersion = 3;
	size_t _avarge_same_hash_records = 7;
}

void OutputFlush(std::ofstream& os, const char* buffer, size_t buf_size)
{
	os.write(buffer, buf_size);
	os.flush();
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

	unsigned long full_record_size = _record_size + sizeof(unsigned int);
	char* record_data = new char[full_record_size * _flush_record_size];
	unsigned int record_hash = 0, curr_hash_rec_num = 0;
	unsigned int report_step_number = records_number / 20;// every 5 %

	//create records
	for (unsigned long rn = 0; rn < records_number; ++rn, --curr_hash_rec_num)
	{
		if (rn % report_step_number == 0 && rn != 0)
			std::cout << ((rn / report_step_number) * 5) << " % of records processed." << std::endl;

		if (curr_hash_rec_num == 0)
		{
			//multiple records can have the same search key, curr_hash_rec_num determines how many records to cetain sk  
			curr_hash_rec_num = _avarge_same_hash_records + (rand() % (2 * _records_to_hash_dispersion) - _records_to_hash_dispersion);
			//for this time record_hash simply increment by one to simulate data records sorted by sk in file
			++record_hash;
		}

		unsigned int rec_offset = (rn % _flush_record_size) * full_record_size;
		memcpy((void *)(record_data + rec_offset), &record_hash, sizeof(unsigned int));

		std::streampos in_fl_offset = rand() % (input_file_size - _record_size);
		input_fl.seekg(in_fl_offset);
		input_fl.read(record_data + rec_offset + sizeof(unsigned int), _record_size);

		if ((rn + 1) % _flush_record_size == 0 && rn != 0)
			OutputFlush(output_fl, record_data, full_record_size);

	}
	
	//if something still in buffer, let's flush it
	if (records_number % _flush_record_size != 0)
		OutputFlush(output_fl, record_data, (records_number % _flush_record_size) * full_record_size);

	delete[] record_data;
	output_fl.close();
	input_fl.close();

	std::cout << "Process complete." << std::endl;
	return 0;
}
