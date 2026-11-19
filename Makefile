CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

TARGET = soc_estimator
SRCS = main.cpp battery.cpp csv_reader.cpp
OBJS = $(SRCS:.cpp=.o)

TEST_TARGET = tests/test_battery
GOLDEN = tests/golden/soc_log.csv

.PHONY: all clean run test

all: output $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

main.o battery.o: battery.h
main.o csv_reader.o: csv_reader.h

$(TEST_TARGET): tests/test_battery.cpp battery.o battery.h
	$(CXX) $(CXXFLAGS) -I. -o $@ tests/test_battery.cpp battery.o

output:
	mkdir -p output

clean:
	rm -f $(OBJS) $(TARGET) $(TEST_TARGET)
	rm -f output/soc_log.csv

run: all
	./$(TARGET) data/battery_data.csv

test: all $(TEST_TARGET)
	./$(TARGET) data/battery_data.csv > /dev/null
	diff -u $(GOLDEN) output/soc_log.csv
	./$(TEST_TARGET)
