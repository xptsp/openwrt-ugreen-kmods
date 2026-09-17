# src/Makefile
TARGET = disk-monitor
OBJS = disk_monitor.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -std=c++11 -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)
