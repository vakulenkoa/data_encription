# data_encription
This is test project. The main purpose of this is to use tink to encrypt/decrypt data.

BUILD:
1) git https://github.com/vakulenkoa/data_encription %PROJECT_FOLDER%
2) extract tink.tar.gz to %PROJECT_FOLDER% /tink
3) cd %PROJECT_FOLDER% and make deploy
4) export LD_LIBRARY_PATH=$(HOME)/bin/data_encryption:$LD_LIBRARY_PATH
5) cd $(HOME)/bin/data_encryption
6) Example of run: ./launcher -e -k aes128_gcm_test_keyset_json.txt -p "AAAAAAAA"

#data_creator
This app allows to create unencrepted test data.

#encryptor
Tink wrapper library

#unit_tests
Testing, but currently not even compiled