# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -std=c++11

# Allegro libraries path
ALLEGRO_LIBS = -LC:/Users/karmi/OneDrive/Desktop/projekt/packages/Allegro.5.2.9/build/native/v142/x64/lib \
               -lallegro -lallegro_primitives -lallegro_font -lallegro_ttf

# Allegro include directory
ALLEGRO_INCLUDE = -IC:/Users/karmi/OneDrive/Desktop/projekt/packages/Allegro.5.2.9/build/native/include

# Source files
SRCS = Funkcje_sterowania.cpp Funkcje_wyswietlajace.cpp Funkcje_gra.cpp main.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Header files directory
INCDIR = .

# Executable name
TARGET = myprogram

# Rules
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(ALLEGRO_LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(ALLEGRO_INCLUDE) -I$(INCDIR) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)