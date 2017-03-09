CC = cc
TARGET = testlib
OBJECTS = main.o ebcdic.c messages.c

all: $(TARGET)

%.o: %.c
	$(CC) -g -c -o $@ $<

clean: clean-obj clean-bin

clean-obj:
	rm -rf *.o
	
clean-bin:
	rm -rf $(TARGET)
	
$(TARGET): $(OBJECTS)
	$(CC) -g -o $(TARGET) $(OBJECTS)
