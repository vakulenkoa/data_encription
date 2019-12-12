#include "tink_api.h"
#include "tink_wrapper.h"

#include <iostream>

std::unique_ptr<TinkWrapper> tink_wrapper;

void InitTink(const char* path_aes_key_file)
{
    if (path_aes_key_file == nullptr)
        throw std::runtime_error("Tink key file was not specified.");

    tink_wrapper.reset(new TinkWrapper(path_aes_key_file));
    std::cout << "Welcome to Tink API." << std::endl; //temp
}

int Encrypt(const char* path_aes_key_file, tink_crypt_param_struct* encrypt_params)
{
    try
    {
        if (tink_wrapper.get() == nullptr)
            InitTink(path_aes_key_file);

        tink_wrapper->Encrypt(encrypt_params);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }

    return 0;
}

int Decrypt(const char* path_aes_key_file, tink_crypt_param_struct* decrypt_params)
{
    try
    {
        if (tink_wrapper.get() == nullptr)
            InitTink(path_aes_key_file);
        tink_wrapper->Decrypt(decrypt_params);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }

    return 0;
}