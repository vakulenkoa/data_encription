#ifndef api_extern_structs_h__
#define api_extern_structs_h__

#ifdef __cplusplus
extern "C" {
#endif

struct tink_crypt_param_struct
{
    const char* input_str;
    const unsigned int input_str_size;
    const char* associated_data; 
    const unsigned int associated_data_size;
    char* out_buffer;
    const unsigned int out_buffer_size;
};

#ifdef __cplusplus
}
#endif

#endif  // api_extern_structs_h__