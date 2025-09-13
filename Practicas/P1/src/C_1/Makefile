# Makefile para P2: compila y ejecuta el lexer y el programa principal

# Variables de archivo
LEXER=lexer.ll          # Archivo de definición del lexer en Flex++
LEXER_CPP=Lexer.cpp     # Archivo C++ generado por Flex++
MAIN_CPP=main.cpp       # Archivo principal del programa
COMPILER=compiler       # Nombre del ejecutable final
INPUT=prueba.txt        # Archivo de entrada para pruebas

# Regla por defecto: compilar el ejecutable
all: $(COMPILER)

# Compila el ejecutable a partir de Lexer.cpp y main.cpp
$(COMPILER): $(LEXER_CPP) $(MAIN_CPP)
	g++ $(LEXER_CPP) $(MAIN_CPP) -o $(COMPILER)

# Genera Lexer.cpp a partir de lexer.ll usando flex++
$(LEXER_CPP): $(LEXER)
	flex++ $(LEXER)

# Ejecuta el compilador con el archivo de prueba
run: $(COMPILER)
	./$(COMPILER) $(INPUT)

# Limpia los archivos generados
clean:
	rm -f $(COMPILER) $(LEXER_CPP)
