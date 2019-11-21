#include "tink_api.h"
#include "tink_wrapper.h"


#include <iostream>

std::unique_ptr<TinkWrapper> tink_wrapper;

void InitTink(const char* path_aes_key_file, const char* secret_prase)
{
    tink_wrapper.reset(new TinkWrapper(path_aes_key_file, secret_prase));
    std::cout << "Welcome to Tink API." << std::endl; //temp
}

int Encrypt(const char* str_to_encrypt, const char* path_aes_key_file, const char* secret_prase, char* out_str, unsigned int max_out_str_size)
{
    try
    {
        if (tink_wrapper.get() == nullptr)
            InitTink(path_aes_key_file, secret_prase);

        tink_wrapper->Encrypt(str_to_encrypt, secret_prase, out_str, max_out_str_size);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return 0;
    }

    return 1;
}

int Decrypt(const char* str_to_decrypt, const char* path_aes_key_file, const char* secret_prase, char* out_str, unsigned int max_out_str_size)
{
    try
    {
        if (tink_wrapper.get() == nullptr)
            InitTink(path_aes_key_file, secret_prase);
        tink_wrapper->Decrypt(str_to_decrypt, secret_prase, out_str, max_out_str_size);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return 0;
    }

    return 1;
}