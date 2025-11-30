CC = g++
CFLAGS = -Wall -Wextra -pedantic
TARGET = Main
DEP1 = Transaction
DEP2 = Spreadsheet
DEP3 = Utils
 
all:	$(TARGET)

$(TARGET):	$(TARGET).cpp $(DEP1).o $(DEP2).o $(DEP3).o 
	$(CC) $(CFLAGS) $(DEP1).o $(DEP2).o $(DEP3).o -o $(TARGET) $(TARGET).cpp

$(DEP1).o:	$(DEP1).cpp
	$(CC) $(CFLAGS) -c $(DEP1).cpp

$(DEP2).o:	$(DEP2).cpp
	$(CC) $(CFLAGS) -c $(DEP2).cpp

$(DEP3).o:	$(DEP3).cpp
	$(CC) $(CFLAGS) -c $(DEP3).cpp

clean:
	rm $(TARGET) *.o