#pragma once
#include "api_extern_structs.h"
#include "tink/aead.h"

#include <string>
#include <memory>

class TinkWrapper
{
    public:
        TinkWrapper(const char * path_key_file);
        void Encrypt(tink_crypt_param_struct* encrypt_params);
        void Decrypt(tink_crypt_param_struct* decrypt_params);

    private:
        std::unique_ptr<crypto::tink::Aead> m_aead;
};
