#CC = gcc
CFLAGS = -Wall #-std=gnu99 -fpic
OBJECT = launcher
TEST_FILE = input.txt
OUT_FILE = out.data

DATA_CREATOR_DIR = data_creator
ENCRYPTOR_DIR = encryptor
DEPLOY_DIR=$(HOME)/bin/data_encryption

TINK_DIR = $(CURDIR)/tink
TINK_BASE_INCLUDE_DIR = $(TINK_DIR)/include
TINK_INCLUDE_DIR_LIST = -I$(TINK_BASE_INCLUDE_DIR) -I$(TINK_BASE_INCLUDE_DIR)/__generated -I$(TINK_BASE_INCLUDE_DIR)/__third_party/com_google_absl/src -I$(TINK_BASE_INCLUDE_DIR)/__third_party/com_google_protobuf/src/src -I$(TINK_BASE_INCLUDE_DIR)/__third_party/rapidjson/src -I$(TINK_BASE_INCLUDE_DIR)/boringssl/src/src/include

#.PHONY: $(DATA_CREATOR)
#$(DATA_CREATOR):

all :
	cd $(DATA_CREATOR_DIR) && $(MAKE)
	cd $(ENCRYPTOR_DIR) && $(MAKE)
	$(CC) -c $(CFLAGS) $(OBJECT).c
	$(CC) -o $(OBJECT) $(OBJECT).o -L$(CURDIR)/$(DATA_CREATOR_DIR) -ldata_creator -L$(CURDIR)/$(ENCRYPTOR_DIR) -lencryptor -L/usr/lib64 -lpthread -lstdc++ -L$(TINK_DIR)/lib -ltink -lm -lrt

clean :
	rm -f $(OBJECT)
	rm -f $(OBJECT).o


deploy: all
	cd $(DATA_CREATOR_DIR) && $(MAKE) deploy
	cd $(ENCRYPTOR_DIR) && $(MAKE)
	rm -f $(DEPLOY_DIR)/$(OBJECT)
	mv $(OBJECT) $(DEPLOY_DIR)

#testing : clean all
#	./$(OBJECT) $(TEST_FILE) 10 $(OUT_FILE) 0
#	test -s $(OUT_FILE) && echo File does exist
