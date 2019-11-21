#ifndef tink_api_h__
#define tink_api_h__

#ifdef __cplusplus
extern "C" {
#endif

//int InitTink(const char* path_aes_key_file, const char* secret_prase);
int Encrypt(const char* str_to_encrypt, const char* path_aes_key_file, const char* secret_prase, char* out_str, unsigned int max_out_str_size);
int Decrypt(const char* str_to_decrypt, const char* path_aes_key_file, const char* secret_prase, char* out_str, unsigned int max_out_str_size);
//int Decrypt(const char* str_to_encrypt, char* out_str_to_decrypt);

#ifdef __cplusplus
}
#endif

#endif  // tink_api_h__