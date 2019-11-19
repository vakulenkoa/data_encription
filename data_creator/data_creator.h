#ifndef data_creator_h__
#define data_creator_h__

extern int GenerateData(const char* input_file_path, 
                        unsigned long records_number, 
                        const char* output_file_path,
                        unsigned int seed);

#endif  // data_creator_h__