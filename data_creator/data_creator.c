#include "data_creator.h"

//#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

void OutputFlush(FILE* ws, const char* buffer, size_t buf_size);

int GenerateData(const char* input_file_path, unsigned long records_number, const char* output_file_path, unsigned int seed)
{
	//init some constatnts
	const size_t _record_size = 80; //record size in bytes
	const unsigned long _flush_record_size = 10000; //number of records in buffer
	const size_t _records_to_hash_dispersion = 3; //possible deviation from avarage number
	const size_t _avarge_same_hash_records = 7; //avarage number of records with same search key

	if (records_number == 0)
	{
		printf("Record number should be grather then 0.");
		return 1;
	}

	//read input file
	FILE *input_fl = fopen(input_file_path, "rb");
	if (input_fl == NULL)
	{
		printf("Could not open file %s", input_file_path);
		return 2;
	}

	//check input file size
	fseek(input_fl, 0L, SEEK_END);
	long input_file_size = ftell(input_fl);

	FILE* output_fl = fopen(output_file_path, "wb");
	if (output_fl == NULL)
	{
		printf("Could not open file %s", output_file_path);
		return 3;
	}

	//seed for randomize
	srand(seed == 0? (size_t )(time(NULL)) : seed);

	const unsigned long full_record_size = _record_size + sizeof(unsigned int);
	char* record_data = (char *)malloc(full_record_size * _flush_record_size);
	unsigned int record_hash = 0, curr_hash_rec_num = 0;
	unsigned long report_step_number = records_number / 20;// every 5 %

	//create records
	for (unsigned long rn = 0; rn < records_number; ++rn, --curr_hash_rec_num)
	{
		if (report_step_number > 0 && (rn % report_step_number) == 0 && rn != 0)
			printf("%lu percents of records processed.\n", (rn / report_step_number) * 5);

		if (curr_hash_rec_num == 0)
		{
			//multiple records can have the same search key, curr_hash_rec_num determines how many records to cetain sk  
			curr_hash_rec_num = _avarge_same_hash_records + (rand() % (2 * _records_to_hash_dispersion) - _records_to_hash_dispersion);
			//for this time record_hash simply increment by one to simulate data records sorted by sk in file
			++record_hash;
		}

		unsigned int rec_offset = (rn % _flush_record_size) * full_record_size;
		*((int *)(record_data + rec_offset)) = record_hash;

		long in_fl_offset = rand() % (input_file_size - _record_size);
		fseek(input_fl, in_fl_offset, SEEK_SET);		
		if (fread(record_data + rec_offset + sizeof(unsigned int), sizeof(char), _record_size, input_fl) != _record_size)
		{
			printf("Error during \'%s\' file reading", input_file_path);
			return 1;
		}

		if ((rn + 1) % _flush_record_size == 0 && rn != 0)
			OutputFlush(output_fl, record_data, full_record_size * _flush_record_size);
	}

	//if something still in buffer, let's flush it
	if (records_number % _flush_record_size != 0)
		OutputFlush(output_fl, record_data, (records_number % _flush_record_size) * full_record_size);

	free(record_data);
	fclose(output_fl);
	fclose(input_fl);

	printf("Process complete.\n");
	return 0;
}

void OutputFlush(FILE* ws, const char* buffer, size_t buf_size)
{
	if (fwrite(buffer, sizeof(char), buf_size, ws) != buf_size)
	{
		printf("Error during data writing.\n");
		exit(3);
	}
	fflush(ws);
}