#CC = gcc
CFLAGS = -Wall 
#-std=gnu99 -fpic
OBJECT = tink

ENCRYPTOR_DIR=/home/userdev/data_encription/encryptor
DEPLOY_DIR=$(HOME)/bin/data_encryption

#.PHONY: $(DATA_CREATOR)
#$(DATA_CREATOR):

TARGETS = $(OBJECT)

all : 
	#$(info var is $(ENCRYPTOR_DIR))
	$(CC) -c -I	$(ENCRYPTOR_DIR) $(CFLAGS) $(OBJECT).c
	$(CC) -o $(OBJECT) $(OBJECT).o -L/usr/lib64 -lcunit -L$(ENCRYPTOR_DIR) -ltink_api

clean :
	rm -f $(OBJECT)
	rm -f $(OBJECT).o

check : all
	$(CURDIR)/$(OBJECT)

#deploy: all
#	cd $(DATA_CREATOR_DIR) && $(MAKE) deploy
#	rm -f $(DEPLOY_DIR)/$(OBJECT)
#	mv $(OBJECT) $(DEPLOY_DIR)

#testing : clean all
#	./$(OBJECT) $(TEST_FILE) 10 $(OUT_FILE) 0
#	test -s $(OUT_FILE) && echo File does exist
