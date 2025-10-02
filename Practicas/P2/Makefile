# Compilador a utilizar (g++ o clang++)
CXX=g++
# Nombre del ejecutable final
EXE=analyzer
# Banderas de compilación para depuración y estándar C++14
CXXDEBUG= -g -Wall
CXXSTD=-std=c++14
CXXFLAGS= $(CXXDEBUG) $(CXXSTD)

# Lista de archivos fuente .cpp (Lexer.cpp se genera automáticamente)
CPPOBJ= main Parser Lexer
OBJS = $(addsuffix .o, $(CPPOBJ))

# El objetivo principal que se ejecuta por defecto
all: $(EXE)

# Regla para enlazar los archivos objeto y crear el ejecutable
$(EXE): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(EXE) $(OBJS)

# Regla para compilar cada archivo .cpp en un archivo .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Regla para generar Lexer.cpp a partir de lexer.ll usando flex
Lexer.cpp: lexer.ll
	flex++ -o Lexer.cpp lexer.ll

# El archivo Lexer.o depende de Lexer.cpp
Lexer.o: Lexer.cpp

# Regla para limpiar los archivos generados (objetos, ejecutable, Lexer.cpp)
clean:
	rm -f $(OBJS) $(EXE) Lexer.cpp

# Phony targets no representan archivos reales
.PHONY: all clean