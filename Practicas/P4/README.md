# Práctica 4: Analizadores sintácticos con Yacc/Bison

**Materia:** Compiladores

**Alumno:** David Rizo

## Descripción General

Esta práctica tiene como objetivo implementar analizadores sintácticos utilizando la herramienta Bison (Yacc) en conjunto con Flex. El trabajo se divide en dos partes principales:

1. **Calculadora Extendida:** Modificación de una calculadora básica para soportar flotantes, multilínea, más operadores y números negativos.

2. **Analizador Sintáctico C_1:** Implementación desde cero de un lexer y parser para una gramática tipo C, incluyendo manejo de errores y pruebas.

## Parte 1: Calculadora Extendida (Ejercicios 1-4)

Se modificaron los archivos `calculadora.yy` y `lexer.ll` para agregar las siguientes funcionalidades:

### Cambios Implementados

1. **Soporte a Flotantes (Ejercicio 1):**

   * Se cambió la unión de Bison (`%union`) para usar `double` en lugar de `struct`.

   * Se actualizó la expresión regular en Flex para detectar decimales (`{DIG}+(\.{DIG}+)?`).

2. **Lectura Multilínea (Ejercicio 2):**

   * Se modificó la gramática para aceptar una lista recursiva de expresiones (`input`).

   * Se añadió el token `EOL` para manejar saltos de línea y permitir múltiples operaciones en un solo archivo.

3. **Nuevas Operaciones (Ejercicio 3):**

   * Se agregaron los tokens y reglas para la **Resta** (`-`) y la **División** (`/`).

   * Se implementó control de errores para división por cero.

4. **Números Negativos (Ejercicio 4):**

   * Se definió la precedencia del operador "menos unario" (`%prec MENOS_UNARIO`) para distinguir entre una resta (`5-5`) y un número negativo (`-5`).

### Compilación y Ejecución

Desde la carpeta `src/`:

```
flex++ lexer.ll
bison calculadora.yy
g++ Parser.cpp Lexer.cpp -o calculadora
./calculadora prueba.txt
```

## Parte 2: Analizador Sintáctico C_1 (Ejercicio 3 y Extras)

Se creó una nueva carpeta `C_1` para alojar un compilador que reconoce una gramática simplificada del lenguaje C.

### Características

* **Lexer Adaptado:** Se reutilizó la lógica de la Práctica 2, adaptándola para trabajar con los tokens generados por Bison.

* **Gramática:** Soporte para declaraciones de variables (`int`, `float`), asignaciones, y estructuras de control (`if`, `if-else`, `while`).

* **Manejo de Errores:** Se implementó un reporte de errores explícito que indica el **número de línea** donde ocurre el fallo sintáctico.

### Archivos de Prueba (Extra 14)

| **Archivo** | **Tipo** | **Descripción** |
| `test_ok.txt` | Válido | Declaraciones, asignaciones y bloque `if-else`. |
| `test_valido_2.txt` | Válido | Uso de `float` y ciclo `while`. |
| `test_error.txt` | Inválido | Error de sintaxis: falta de punto y coma (`;`). |
| `test_invalido_2.txt` | Inválido | Error de sintaxis: falta de paréntesis en estructura `while`. |

### Compilación y Ejecución

Desde la carpeta `src/C_1/`:

```
flex++ lexer.ll
bison Parser.yy
g++ Parser.cpp Lexer.cpp main.cpp -o compilador

# Ejecutar pruebas
./compilador test_ok.txt
./compilador test_error.txt
```