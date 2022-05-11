#ELEC2204 - MIPs Processor
#James Stockton


# make file commands
#	test - compiles test program
#	runtest - run test program
#	main - compiles main program
#	runmain - run main program
#	clean - removes compiled files

#compiler constants
CC = gcc
CFLAGS = -lstdc++ -Os
TARGET = main
TEST = $(TARGET).test

BUILD = build
SOURCE = src
INCLUDES = $(wildcard $(SOURCE)/*.cpp)


all: $(BUILD)/$(TARGET).out

test: $(BUILD)/$(TEST).out


run: all
	$(BUILD)/$(TARGET).out

runtest: test
	$(BUILD)/$(TEST).out


clean:
	rm -f build/*.out


$(BUILD)/$(TEST).out: $(INCLUDES) $(TEST).cpp
	$(CC) $(TEST).cpp $(INCLUDES) $(CFLAGS) -o $(BUILD)/$(TEST).out

$(BUILD)/$(TARGET).out: $(INCLUDES) $(TARGET).cpp
	$(CC) $(TARGET).cpp $(INCLUDES) $(CFLAGS) -o $(BUILD)/$(TARGET).out