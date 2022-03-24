#ELEC2204 - MIPs Processor
#James Stockton


# make file commands
#	main - compiles main program
#	test - compiles test program
#	clean - removes compiled files

#compiler constants
CC = gcc
CFLAGS = -lstdc++
TARGET = main
TEST = $(TARGET).test

BUILD = build
INCLUDES = src



test:
	$(CC) $(TEST).cpp $(INCLUDES)/*.cpp $(CFLAGS) -o $(BUILD)/$(TEST).out

runtest:
	$(BUILD)/$(TEST).out

main:
	$(CC) $(TARGET).cpp $(INCLUDES)/*.cpp $(CFLAGS) -o $(BUILD)/$(TARGET).out

runmain:
	$(BUILD)/$(TARGET).out

clean:
	rm -f build/*.out