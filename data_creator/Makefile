CC = gcc
CFLAGS = -Wall -std=gnu99
OBJECT = data_creator
TEST_FILE = input.txt
OUT_FILE = out.data

all :
	$(CC) $(CFLAGS) $(OBJECT).c -o $(OBJECT)

clean :
	rm -f $(OBJECT)

testing : clean all
	./$(OBJECT) $(TEST_FILE) 10 $(OUT_FILE) 0
	test -s $(OUT_FILE) && echo File does exist
