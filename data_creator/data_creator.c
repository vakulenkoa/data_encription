#include "data_creator.h"
#include "../encryptor/tink_api.h"

#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#define RECORD_SIZE 80
#define OUT_BUFF_SIZE RECORD_SIZE + 40

void OutputFlush(FILE* ws, const char* buffer, size_t buf_size);
void ReportProgress(const size_t record_number, const size_t report_step_number);
unsigned int IncrementHash(const unsigned int av_records_on_same_hash_count);
void SetHash(char* buffer, const char* rec_hash);
int SetRecord(FILE* read_stream, char* buffer, const size_t record_size, const unsigned long file_size);
int EncryptRecord(const char* record_str, char* out_buffer, const char*  path_aes_key_file, const char* secret_key);

int GenerateData(const char* input_file_path, const unsigned long records_number, const char* output_file_path, const unsigned int seed, const char* path_aes_key_file)
{
	//init some constatnts
	const unsigned long _records_buffer_size = 10000; //number of records in buffer
	const size_t _avarge_same_hash_records = 7; //avarage number of records with same search key
	const size_t _report_every_n_persent = 5; //report every 5 percent

	//Fill encrypt data struct init values
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
	char record_hash[5];
	*((unsigned int *)record_hash) = 0x01010101;//AAAA
	record_hash[4] = '\0';

	size_t report_step_number = records_number * _report_every_n_persent / 100;// report every %_report_every_n_persent% percents of progress
	const unsigned int full_record_size = sizeof(unsigned int)/*hash size*/ + RECORD_SIZE;
	unsigned int buffer_offset = 0;

	//allocate memory for buffer with generated info
	char rec_buf[OUT_BUFF_SIZE];
	char* records_buffer = (char *)malloc(full_record_size * _records_buffer_size);

	clock_t time_start_counter = clock();
	//create records
	for (unsigned long rec_num = 0; rec_num < records_number; ++rec_num)
	{
		ReportProgress(rec_num, report_step_number);

		SetHash(records_buffer + buffer_offset, record_hash);

		if(SetRecord(input_fl, rec_buf, RECORD_SIZE, input_file_size) != 0)
		{
			printf("Error during \'%s\' file reading", input_file_path);
			return 1;
		}

		if (path_aes_key_file != NULL) //encrypt
		{
			if(EncryptRecord(rec_buf, records_buffer + buffer_offset + sizeof(unsigned int), path_aes_key_file, record_hash))
			{
				printf("Encryption error.");
				return 4;
			}
		}
		else //place as is
		{
			memcpy(records_buffer + buffer_offset + sizeof(unsigned int), rec_buf, RECORD_SIZE);
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
		*((unsigned int *)record_hash) += IncrementHash(_avarge_same_hash_records);
	}

	//if something still in buffer, let's flush it
	if (records_number % _records_buffer_size != 0)
		OutputFlush(output_fl, records_buffer, (records_number % _records_buffer_size) * full_record_size);

	printf("Working time (in clocks): %ld\n", clock() - time_start_counter);

	free(records_buffer);
	fclose(output_fl);
	fclose(input_fl);

	printf("Process complete.\n");
	return 0;
}

int EncryptRecord(const char* record_str, char* out_buffer, const char*  path_aes_key_file, const char* secret_key)
{
	struct tink_crypt_param_struct _encrypt_data = (struct tink_crypt_param_struct)  { .input_str = record_str, .input_str_size = RECORD_SIZE, 
	.associated_data = secret_key, .associated_data_size = sizeof(unsigned int), .out_buffer = out_buffer, .out_buffer_size = OUT_BUFF_SIZE };

	return Encrypt(path_aes_key_file, &_encrypt_data);
}

//void SetHash(char* buffer,  const size_t buffer_offset, unsigned int rec_hash)
void SetHash(char* buffer, const char* rec_hash)
{
	*((unsigned int *)(buffer)) = *((unsigned int *)rec_hash);
}

int SetRecord(FILE* read_stream, char* buffer, const size_t record_size, const unsigned long file_size)
{
	long in_fl_offset = rand() % (file_size - record_size);
	fseek(read_stream, in_fl_offset, SEEK_SET);		
	if (fread(buffer, sizeof(char), record_size, read_stream) != record_size)
		return 1; //error

	return 0;
}

void ReportProgress(const size_t record_number, const size_t report_step_number)
{
	if (report_step_number > 0 && (record_number % report_step_number) == 0 && record_number != 0)
		printf("%lu percents of records processed.\n", (record_number / report_step_number) * 5);
}

unsigned int IncrementHash(unsigned int av_records_on_same_hash_count)
{
	//for this time record_hash simply increment by one to simulate data records sorted by sk in file
	return rand() % av_records_on_same_hash_count == 0 ? 1 : 0;
}

void OutputFlush(FILE* ws, const char* buffer, const size_t buf_size)
{
	if (fwrite(buffer, sizeof(char), buf_size, ws) != buf_size)
	{
		printf("Error during data writing.\n");
		exit(3);
	}
	fflush(ws);
}