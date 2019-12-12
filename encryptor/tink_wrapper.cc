#include "tink_wrapper.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include "tink/cleartext_keyset_handle.h"
#include "tink/config.h"
#include "tink/json_keyset_reader.h"
#include "tink/keyset_handle.h"
#include "tink/keyset_reader.h"
#include "tink/config/tink_config.h"

#include "tink/aead/aead_key_templates.h"
#include <exception>

namespace 
{
// Helper functions.
// Upon failure each function writes an error message, and terminates.

  // Initializes Tink.
  void InitTink() {
    std::clog << "Initializing Tink...\n";
    auto status = crypto::tink::TinkConfig::Register();
    if (!status.ok()) {
      throw std::runtime_error(std::string("Initialization of Tink failed: ") + status.error_message());
    }
  }

  // Creates a KeysetReader that reads a JSON-formatted keyset
  // from the given file.
  std::unique_ptr<crypto::tink::KeysetReader> GetJsonKeysetReader(
      const std::string& filename) {
    std::clog << "Creating a JsonKeysetReader...\n";
    std::unique_ptr<std::ifstream> keyset_stream(new std::ifstream());
    keyset_stream->open(filename, std::ifstream::in);
    auto keyset_reader_result =
        crypto::tink::JsonKeysetReader::New(std::move(keyset_stream));
    if (!keyset_reader_result.ok()) {
      throw std::runtime_error(std::string("Creation of the reader failed: ") + keyset_reader_result.status().error_message());
    }
    return std::move(keyset_reader_result.ValueOrDie());
  }

  // Creates a KeysetHandle that for a keyset read from the given file,
  // which is expected to contain a JSON-formatted keyset.
  std::unique_ptr<crypto::tink::KeysetHandle> ReadKeyset(
      const std::string& filename) {
     auto keyset_reader = GetJsonKeysetReader(filename);
     auto keyset_handle_result =
         crypto::tink::CleartextKeysetHandle::Read(std::move(keyset_reader));
        
    if (!keyset_handle_result.ok()) {
      throw std::runtime_error(std::string("Reading the keyset failed: ") + keyset_handle_result.status().error_message());
    }
    return std::move(keyset_handle_result.ValueOrDie());
  }

  // Reads the specified file and returns the read content as a string.
  std::string Read(const std::string& filename) {
    std::clog << "Reading the input...\n";
    std::ifstream input_stream;
    input_stream.open(filename, std::ifstream::in);
    if (!input_stream.is_open()) {
      std::clog << "Error opening input file " << filename << std::endl;
      exit(1);
    }
    std::stringstream input;
    input << input_stream.rdbuf();
    input_stream.close();
    return input.str();
  }

  // Writes the given string to the specified file.
  void Write(const std::string& output, const std::string& filename) {
    std::clog << "Writing the output...\n";
    std::ofstream output_stream(filename,
                                std::ofstream::out | std::ofstream::binary);
    if (!output_stream.is_open()) {
      std::clog << "Error opening output file " << filename << std::endl;
      exit(1);
    }
    output_stream << output;
    output_stream.close();
  }

}  // namespace

TinkWrapper::TinkWrapper(const char * path_key_file)
{
  InitTink();
  // Read the keyset.
  auto keyset_handle = ReadKeyset(path_key_file);

  // Get the primitive.
  auto primitive_result = keyset_handle->GetPrimitive<crypto::tink::Aead>();
  if (!primitive_result.ok()) {
    throw std::runtime_error(std::string("Getting AEAD-primitive from the factory failed: ") + primitive_result.status().error_message());
  }

  m_aead = std::move(primitive_result.ValueOrDie());
}

void TinkWrapper::Encrypt(tink_crypt_param_struct* encrypt_params)
{
  if (!m_aead.get())
    throw std::runtime_error(std::string("AEAD is not initialized"));

  auto encrypt_result = m_aead->Encrypt(absl::string_view(encrypt_params->input_str, encrypt_params->input_str_size), absl::string_view(encrypt_params->associated_data, encrypt_params->associated_data_size));

  if (!encrypt_result.ok()) 
    throw std::runtime_error(std::string("Error while encrypting the input: ") + encrypt_result.status().error_message());

  std::string& output = encrypt_result.ValueOrDie();

  if (output.size() > encrypt_params->out_buffer_size - 1)
     throw std::runtime_error(std::string("Encrypted output exceed max size of buffer."));
  
  memcpy(encrypt_params->out_buffer, output.c_str(), output.size());
}

void TinkWrapper::Decrypt(tink_crypt_param_struct* decrypt_params)
{
  if (!m_aead.get())
    throw std::runtime_error(std::string("AEAD is not initialized"));

  auto decrypt_result = m_aead->Decrypt(absl::string_view(decrypt_params->input_str, decrypt_params->input_str_size), absl::string_view(decrypt_params->associated_data, decrypt_params->associated_data_size));
  if (!decrypt_result.ok()) 
    throw std::runtime_error(std::string("Error while decrypting the input: ") + decrypt_result.status().error_message());

  std::string& output = decrypt_result.ValueOrDie();

  if (output.size() > decrypt_params->out_buffer_size - 1)
    throw std::runtime_error(std::string("Encrypted output exceed max size of buffer."));
  
  memcpy(decrypt_params->out_buffer, output.c_str(), output.size());
}