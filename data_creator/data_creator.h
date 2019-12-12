#ifndef data_creator_h__
#define data_creator_h__

extern int GenerateData(const char* input_file_path, 
                        const unsigned long records_number, 
                        const char* output_file_path, 
                        const unsigned int seed, // in case seed is not 0, random seed is used. this is for determinate behavior
                        const char* path_aes_key_file //if this parameter is NULL then no encryption performs
                        );

#endif  // data_creator_h__