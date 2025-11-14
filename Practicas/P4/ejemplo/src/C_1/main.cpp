#include <iostream>
#include <fstream>
#include "Parser.hpp"
#include "Lexer.hpp"

using namespace std;

int main(int argc, char *argv[]) {
    if(argc < 2) {
        cout << "Faltan argumentos. Uso: ./parser archivo_entrada" << endl;
        return 1;
    }
    ifstream in(argv[1]);
    if(!in.is_open()) {
        cout << "No se puede abrir el archivo." << endl;
        return 1;
    }

    calc::Lexer lexer(&in);
    calc::Parser parser(&lexer);

    parser.parse();
    
    return 0;
}