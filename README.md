# data_encription
This is test project. The main purpose of this is to use tink to encrypt/decrypt data.

BUILD:
1) git https://github.com/vakulenkoa/data_encription %PROJECT_FOLDER%
2) extract tink.tar.gz to %PROJECT_FOLDER% /tink
3) cd %PROJECT_FOLDER% and make deploy
4) cd $(HOME)/bin/data_encryption
5) Example of run: ./launcher -g -k aes128_gcm_test_keyset_json.txt -i input.txt -o encrypted.out -s 333 -q 10000000
Note: if you add -k option with key file then result will be encripted otherwise it will be plain text.

#data_creator
This app allows to create unencrepted test data.

#encryptor
Tink wrapper library

#unit_tests
Testing, but currently not even compiled