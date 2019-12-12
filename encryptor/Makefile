CXX = g++
CPPFLAGS = -Wall -std=c++11 -fPIC

TINK_WR_OBJ = tink_wrapper
TINK_API_OBJ = tink_api
ENCRYPTOR = encryptor
LIB_ENCRYPTOR = libencryptor.a

TINK_DIR = $(CURDIR)/../tink
TINK_BASE_INCLUDE_DIR = $(TINK_DIR)/include
TINK_INCLUDE_DIR_LIST = -I$(TINK_BASE_INCLUDE_DIR) -I$(TINK_BASE_INCLUDE_DIR)/__generated -I$(TINK_BASE_INCLUDE_DIR)/__third_party/com_google_absl/src -I$(TINK_BASE_INCLUDE_DIR)/__third_party/com_google_protobuf/src/src -I$(TINK_BASE_INCLUDE_DIR)/__third_party/rapidjson/src -I$(TINK_BASE_INCLUDE_DIR)/boringssl/src/src/include
DEPLOY_DIR=$(HOME)/bin/data_encryption

TARGETS = $(TINK_API_OBJ).cc $(TINK_WR_OBJ).cc
OBJECTS=$(TINK_API_OBJ).o $(TINK_WR_OBJ).o

#.PHONY: $(DATA_CREATOR)
#$(DATA_CREATOR):

all :
	$(CXX) -c $(TINK_INCLUDE_DIR_LIST) $(CPPFLAGS) $(TARGETS)
	#$(CXX) -shared -o $(ENCRYPTOR) $(TINK_WR_OBJ).o $(TINK_API_OBJ).o  -L/usr/lib64 -L/lib64 -L$(TINK_DIR)/lib -ltink -lpthread 
	ar cr $(LIB_ENCRYPTOR) $(OBJECTS)

clean :
	rm -f $(LIB_ENCRYPTOR)
	rm -f $(OBJECTS)


deploy: all
	mkdir -p $(DEPLOY_DIR)
	cp -n aes128_gcm_test_keyset_json.txt $(DEPLOY_DIR)

#testing : clean all
#	./$(OBJECT) $(TEST_FILE) 10 $(OUT_FILE) 0
#	test -s $(OUT_FILE) && echo File does exist