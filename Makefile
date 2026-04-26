CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

TARGET = soc_estimator
SRCS = main.cpp battery.cpp csv_reader.cpp
OBJS = $(SRCS:.cpp=.o)

all: output $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

output:
	mkdir -p output

clean:
	rm -f $(OBJS) $(TARGET)
	rm -f output/soc_log.csv

run: all
	./$(TARGET) data/battery_data.csv
