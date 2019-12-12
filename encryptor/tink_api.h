#ifndef tink_api_h__
#define tink_api_h__

#include "api_extern_structs.h"

#ifdef __cplusplus
extern "C" {
#endif

int Encrypt(const char* path_aes_key_file, struct tink_crypt_param_struct* encrypt_params);
int Decrypt(const char* path_aes_key_file, struct tink_crypt_param_struct* decrypt_params);

#ifdef __cplusplus
}
#endif

#endif  // tink_api_h__