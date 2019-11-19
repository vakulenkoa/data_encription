#include "data_creator.h"

//#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

void OutputFlush(FILE* ws, const char* buffer, size_t buf_size);
void ReportProgress(unsigned long record_number, const unsigned long report_step_number);
size_t IncrementHash(size_t av_records_on_same_hash_count);
void SetHash(char* buffer,  size_t buffer_offset, unsigned int rec_hash);
int SetRecord(FILE* read_stream, char* buffer, const size_t buffer_offset, const size_t record_size, const unsigned long file_size);

int GenerateData(const char* input_file_path, unsigned long records_number, const char* output_file_path, unsigned int seed)
{
	//init some constatnts
	const size_t _record_size = 80; //record size in bytes
	const unsigned long _records_buffer_size = 10000; //number of records in buffer
	const size_t _avarge_same_hash_records = 7; //avarage number of records with same search key
	const size_t _report_every_n_persent = 5; //report every 5 percent

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
	const long input_file_size = ftell(input_fl);

	FILE* output_fl = fopen(output_file_path, "wb");
	if (output_fl == NULL)
	{
		printf("Could not open file %s", output_file_path);
		return 3;
	}

	//seed for randomize
	srand(seed == 0? (size_t )(time(NULL)) : seed);

	//calulate constants for generation		
	size_t record_hash = 0x01010101;//AAAA
	unsigned long report_step_number = records_number * _report_every_n_persent / 100;// report every %_report_every_n_persent% percents of progress
	const unsigned long full_record_size = sizeof(unsigned int)/*hash size*/ + _record_size;
	size_t buffer_offset = 0;

	//allocate memory for buffer with generated info
	char* records_buffer = (char *)malloc(full_record_size * _records_buffer_size);

	//create records
	for (unsigned long rec_num = 0; rec_num < records_number; ++rec_num)
	{
		ReportProgress(rec_num, report_step_number);

		SetHash(records_buffer,  buffer_offset, record_hash);

		if(SetRecord(input_fl, records_buffer, buffer_offset, _record_size, input_file_size) != 0)
		{
			printf("Error during \'%s\' file reading", input_file_path);
			return 1;
		}

		if ((rec_num + 1) % _records_buffer_size == 0)
		{
			OutputFlush(output_fl, records_buffer, full_record_size * _records_buffer_size);
			buffer_offset = 0; //start to fill buffer again
		}
		else
		{
			buffer_offset += full_record_size; //increment buffer pos
		}
		
		//randomly increment hash
		record_hash += IncrementHash(_avarge_same_hash_records);
	}

	//if something still in buffer, let's flush it
	if (records_number % _records_buffer_size != 0)
		OutputFlush(output_fl, records_buffer, (records_number % _records_buffer_size) * full_record_size);

	free(records_buffer);
	fclose(output_fl);
	fclose(input_fl);

	printf("Process complete.\n");
	return 0;
}

void SetHash(char* buffer,  size_t buffer_offset, unsigned int rec_hash)
{
	*((unsigned int *)(buffer + buffer_offset)) = rec_hash;
}

int SetRecord(FILE* read_stream, char* buffer, const size_t buffer_offset, const size_t record_size, const unsigned long file_size)
{
	long in_fl_offset = rand() % (file_size - record_size);
	fseek(read_stream, in_fl_offset, SEEK_SET);		
	if (fread(buffer + buffer_offset + sizeof(unsigned int), sizeof(char), record_size, read_stream) != record_size)
		return 1; //error

	return 0;
}

void ReportProgress(const unsigned long record_number, const unsigned long report_step_number)
{
	if (report_step_number > 0 && (record_number % report_step_number) == 0 && record_number != 0)
		printf("%lu percents of records processed.\n", (record_number / report_step_number) * 5);
}

size_t IncrementHash(size_t av_records_on_same_hash_count)
{
	//for this time record_hash simply increment by one to simulate data records sorted by sk in file
	return rand() % av_records_on_same_hash_count == 0 ? 1 : 0;
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