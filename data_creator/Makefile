CC = gcc
CFLAGS = -Wall -std=gnu99 -fpic
OBJECT = data_creator
TEST_FILE = input.txt
OUT_FILE = out.data
DEPLOY_DIR=$(HOME)/bin/data_encryption

all :
	$(CC) -c $(CFLAGS) $(OBJECT).c
	$(CC) -shared -o lib$(OBJECT).so $(OBJECT).o

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
