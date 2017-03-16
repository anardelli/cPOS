CC = cc
TARGET = testlib
OBJECTS = example.o ebcdic.o messages.o
# OBJECTS = ebcdic.o messages.o main.o

all: $(TARGET)

%.o: %.c
	$(CC) -Wall -g -c -o $@ $<

clean: clean-obj clean-bin

clean-obj:
	rm -rf *.o
	
clean-bin:
	rm -rf $(TARGET)
	
$(TARGET): $(OBJECTS)
	$(CC) -Wall -g -o $(TARGET) $(OBJECTS)
