#CC = gcc
CFLAGS = -Wall #-std=gnu99 -fpic
OBJECT = launcher
TEST_FILE = input.txt
OUT_FILE = out.data

DATA_CREATOR_DIR = data_creator
DEPLOY_DIR=$(HOME)/bin/data_encryption

#.PHONY: $(DATA_CREATOR)
#$(DATA_CREATOR):

all : $(TARGETS)
	echo $(CURDIR)
	cd $(DATA_CREATOR_DIR) && $(MAKE)
	$(CC) -c $(CFLAGS) $(OBJECT).c
	$(CC) -o $(OBJECT) $(OBJECT).o -L$(CURDIR)/$(DATA_CREATOR_DIR) -ldata_creator

clean :
	rm -f $(OBJECT)
	rm -f $(OBJECT).o


deploy: all
	cd $(DATA_CREATOR_DIR) && $(MAKE) deploy
	rm -f $(DEPLOY_DIR)/$(OBJECT)
	mv $(OBJECT) $(DEPLOY_DIR)

#testing : clean all
#	./$(OBJECT) $(TEST_FILE) 10 $(OUT_FILE) 0
#	test -s $(OUT_FILE) && echo File does exist
