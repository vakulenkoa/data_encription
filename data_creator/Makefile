CC = g++
CFLAGS = -Wall -std=c++11
OBJECT = data_creator
TEST_FILE = input.txt
OUT_FILE = out.data

all :
	$(CC) $(CFLAGS) $(OBJECT).cc -o $(OBJECT)

clean :
	rm -f $(OBJECT)

testing : clean all
	./$(OBJECT) $(TEST_FILE) 10 $(OUT_FILE) 0
	test -s $(OUT_FILE) && echo File does exist
