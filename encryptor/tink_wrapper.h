#pragma once

#include <string>
#include <memory>

#include "tink/aead.h"

class TinkWrapper
{
    public:
        TinkWrapper(const char * path_key_file, const char* secret_prase);
        //void SetSecretWord(const char* secret_phrase); //TODO: is it need?
        void Encrypt(const char* str_to_encrypt, const char* secret_phrase, char* out_str, size_t max_buffer_length);
        void Decrypt(const char* str_to_decrypt, const char* secret_phrase, char* out_str, size_t max_buffer_length);

    private:
        //std::string m_secret_word; // TODO: is it need? 

        std::unique_ptr<crypto::tink::Aead> m_aead;
};

//using TinkWrapperPtr=std::unique_ptr<TinkWrapper>;