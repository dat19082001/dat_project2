# Trình biên dịch và cờ biên dịch
CXX = g++
CXXFLAGS = -Wall -g

# Tệp nguồn và đối tượng
SOURCES = main.cpp Product.cpp Book.cpp MusicDisc.cpp Invoice.cpp
OBJECTS = $(SOURCES:.cpp=.o)
TARGET = main

# Mục tiêu chính: build file thực thi
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

# Quy tắc chung để tạo file .o từ .cpp
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Mục tiêu chạy chương trình
run: $(TARGET)
	./$(TARGET)

# Mục tiêu chạy GDB để debug
debug: $(TARGET)
	gdb ./$(TARGET)

# Dọn sạch
clean:
	rm -f $(TARGET) $(OBJECTS)
