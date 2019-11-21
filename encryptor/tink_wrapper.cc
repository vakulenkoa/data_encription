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
      // std::clog << "Initialization of Tink failed: "
      //           << status.error_message() << std::endl;
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
      // std::clog << "Creation of the reader failed: "
      //           << keyset_reader_result.status().error_message() << std::endl;
      // exit(1);
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
      // std::clog << "Reading the keyset failed: "
      //           << keyset_handle_result.status().error_message() << std::endl;
      // exit(1);
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



// int main(int argc, char** argv) {
//   if (argc != 6) {
//     std::clog << "Usage: " << argv[0]
//          << "  keyset-file operation input-file associated-data output-file\n";
//     exit(1);
//   }

//   std::string keyset_filename(argv[1]);
//   std::string operation(argv[2]);
//   std::string input_filename(argv[3]);
//   std::string associated_data(argv[4]);
//   std::string output_filename(argv[5]);
//   if (!(operation == "encrypt" || operation == "decrypt")) {
//     std::clog << "Unknown operation '" << operation << "'.\n"
//               << "Expected 'encrypt' or 'decrypt'.\n";
//     exit(1);
//   }
//   std::clog << "Using keyset from file " << keyset_filename
//             << " to AEAD-" << operation
//             << " file "<< input_filename
//             << " with associated data '" << associated_data << "'.\n"
//             << "The resulting output will be written to file "
//             << output_filename << std::endl;

//   // Init Tink;
//   InitTink();

//   // Read the keyset.
//   auto keyset_handle = ReadKeyset(keyset_filename);

//   // Get the primitive.
//   auto primitive_result = keyset_handle->GetPrimitive<crypto::tink::Aead>();
//   if (!primitive_result.ok()) {
//     std::clog << "Getting AEAD-primitive from the factory failed: "
//               << primitive_result.status().error_message() << std::endl;
//     exit(1);
//   }
//   std::unique_ptr<crypto::tink::Aead> aead =
//       std::move(primitive_result.ValueOrDie());

//   // Read the input.
//   std::string input = Read(input_filename);

//   // Compute the output.
//   std::clog << operation << "ing...\n";
//   std::string output;
//   if (operation == "encrypt") {
//     auto encrypt_result = aead->Encrypt(input, associated_data);
//     if (!encrypt_result.ok()) {
//       std::clog << "Error while encrypting the input:"
//                 << encrypt_result.status().error_message() << std::endl;
//       exit(1);
//     }
//     output = encrypt_result.ValueOrDie();
//   } else {  // operation == "decrypt"
//     auto decrypt_result = aead->Decrypt(input, associated_data);
//     if (!decrypt_result.ok()) {
//       std::clog << "Error while decrypting the input:"
//                 << decrypt_result.status().error_message() << std::endl;
//       exit(1);
//     }
//     output = decrypt_result.ValueOrDie();
//   }

//   // Write the output to the output file.
//   Write(output, output_filename);

//   std::clog << "All done.\n";
//   return 0;
// }

TinkWrapper::TinkWrapper(const char * path_key_file, const char* secret_prase)
{
  //m_secret_word = secret_prase;

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

// void TinkWrapper::SetSecretWord(const char* secret_phrase)
// {
//    m_secret_word = secret_prase;
// }

void TinkWrapper::Encrypt(const char* str_to_encrypt, const char* secret_phrase, char* out_str, size_t max_buffer_length)
{
  if (!m_aead.get())
    throw std::runtime_error(std::string("AEAD is not initialized"));

  auto encrypt_result = m_aead->Encrypt(str_to_encrypt, secret_phrase);

  if (!encrypt_result.ok()) 
    throw std::runtime_error(std::string("Error while encrypting the input: ") + encrypt_result.status().error_message());

  std::string& output = encrypt_result.ValueOrDie();

  if (output.size() > max_buffer_length)
    throw std::runtime_error(std::string("Encrypted output exceed max size of buffer."));
  
  memcpy(out_str, output.c_str(), output.size());
}

void TinkWrapper::Decrypt(const char* str_to_decrypt, const char* secret_phrase, char* out_str, size_t max_buffer_length)
{
  if (!m_aead.get())
    throw std::runtime_error(std::string("AEAD is not initialized"));

  auto decrypt_result = m_aead->Decrypt(str_to_decrypt, secret_phrase);
  if (!decrypt_result.ok()) 
    throw std::runtime_error(std::string("Error while decrypting the input: ") + decrypt_result.status().error_message());

  std::string& output = decrypt_result.ValueOrDie();

  if (output.size() > max_buffer_length)
    throw std::runtime_error(std::string("Encrypted output exceed max size of buffer."));
  
  memcpy(out_str, output.c_str(), output.size());
}
