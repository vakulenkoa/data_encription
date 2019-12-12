CC = gcc
CFLAGS = -Wall -std=gnu99 -fPIC
OBJECT = data_creator
TEST_FILE = input.txt
OUT_FILE = out.data
DEPLOY_DIR=$(HOME)/bin/data_encryption
ENCRYPTOR_DIR = $(CURDIR)/../encryptor
TINK_DIR = $(CURDIR)/../tink

all :
	cd $(ENCRYPTOR_DIR) && $(MAKE)
	$(CC) -c $(CFLAGS) $(OBJECT).c
	$(CC) -shared -o lib$(OBJECT).so $(OBJECT).o -L$(ENCRYPTOR_DIR) -lencryptor
	#$(CC) -shared -o lib$(OBJECT).so $(OBJECT).o -L$(ENCRYPTOR_DIR) -lencryptor -L/usr/lib64 -lpthread -lstdc++ -L$(TINK_DIR)/lib -ltink -lm -lrt

clean :
	rm -f lib$(OBJECT).so
	rm -f $(OBJECT).o

deploy: all
	mkdir -p $(DEPLOY_DIR)
	rm -f $(DEPLOY_DIR)/lib$(OBJECT).so
	mv lib$(OBJECT).so $(DEPLOY_DIR)

#testing : clean all
#	./$(OBJECT) $(TEST_FILE) 10 $(OUT_FILE) 0
#	test -s $(OUT_FILE) && echo File does exist
