# Reporte de Práctica: Analizador Sintáctico Ascendente LR

**Alumno:** Rizo Serrano Félix David
**Grupo:** 7010
**Materia:** Compiladores 2026-1

---

## 1. Definición Formal de la Gramática

Para la gramática $G = (N, T, P, S)$, definimos los conjuntos resultantes del análisis:

### Símbolos No Terminales ($N$)
$$N = \{ \text{programa}, \text{declaraciones}, \text{declaracion}, \text{tipo}, \text{lista\_var}, \text{sentencias}, \text{sentencia}, \text{expresion} \}$$

### Símbolos Terminales ($T$)
$$T = \{ \textbf{int}, \textbf{float}, \textbf{id}, \textbf{num}, ;, ,, =, \textbf{if}, \textbf{else}, \textbf{while}, +, -, *, /, (, ) \}$$

### Símbolo Inicial ($S$)
$$S = \text{programa}$$

---

## 1.a. Gramática Aumentada $G'$

Para construir el autómata LR, aumentamos la gramática con una nueva producción inicial $S' \to S$.

**Producciones ($P'$):**

1.  $S' \to \text{programa}$
2.  $\text{programa} \to \text{declaraciones} \ \text{sentencias}$
3.  $\text{declaraciones} \to \text{declaraciones} \ \text{declaracion}$
4.  $\text{declaraciones} \to \text{declaracion}$
5.  $\text{declaracion} \to \text{tipo} \ \text{lista\_var} \ \textbf{;}$
6.  $\text{tipo} \to \textbf{int}$
7.  $\text{tipo} \to \textbf{float}$
8.  $\text{lista\_var} \to \text{lista\_var} \ \textbf{,} \ \textbf{id}$
9.  $\text{lista\_var} \to \textbf{id}$
10. $\text{sentencias} \to \text{sentencias} \ \text{sentencia}$
11. $\text{sentencias} \to \text{sentencia}$
12. $\text{sentencia} \to \textbf{id} \ \textbf{=} \ \text{expresion} \ \textbf{;}$
13. $\text{sentencia} \to \textbf{if} \ \textbf{(} \ \text{expresion} \ \textbf{)} \ \text{sentencias} \ \textbf{else} \ \text{sentencias}$
14. $\text{sentencia} \to \textbf{while} \ \textbf{(} \ \text{expresion} \ \textbf{)} \ \text{sentencias}$
15. $\text{expresion} \to \text{expresion} \ \textbf{+} \ \text{expresion}$
16. $\text{expresion} \to \text{expresion} \ \textbf{-} \ \text{expresion}$
17. $\text{expresion} \to \text{expresion} \ \textbf{*} \ \text{expresion}$
18. $\text{expresion} \to \text{expresion} \ \textbf{/} \ \text{expresion}$
19. $\text{expresion} \to \textbf{id}$
20. $\text{expresion} \to \textbf{num}$
21. $\text{expresion} \to \textbf{(} \ \text{expresion} \ \textbf{)}$

---

## 1.c. Conjuntos FIRST ($Primeros$)

Calculamos los tokens que pueden aparecer al inicio de cada No Terminal.

| No Terminal | FIRST | Justificación |
| :--- | :--- | :--- |
| **tipo** | `{ int, float }` | Deriva directamente terminales. |
| **declaracion** | `{ int, float }` | Empieza con `tipo`. |
| **declaraciones**| `{ int, float }` | Empieza con `declaracion`. |
| **lista_var** | `{ id }` | Siempre inicia con un identificador. |
| **expresion** | `{ id, num, ( }` | Puede ser variable, número o paréntesis. |
| **sentencia** | `{ id, if, while }` | Asignación o estructuras de control. |
| **sentencias** | `{ id, if, while }` | Conjunto de sentencias. |
| **programa** | `{ int, float }` | Inicia obligatoriamente con `declaraciones`. |

---

## 1.d. Conjuntos FOLLOW ($Siguientes$)

Calculamos qué tokens pueden aparecer *inmediatamente después* de cada No Terminal. Es crucial para resolver reducciones en SLR.

| No Terminal | FOLLOW | Explicación |
| :--- | :--- | :--- |
| **programa** | `{ $ }` | Fin de archivo. |
| **declaraciones**| `{ id, if, while }` | Lo que sigue en `programa` son `sentencias`. |
| **declaracion** | `{ int, float, id, if, while }` | Puede seguir otra declaración o empezar `sentencias`. |
| **tipo** | `{ id }` | Siempre seguido de `lista_var`. |
| **lista_var** | `{ ;, , }` | Seguido de punto y coma o coma. |
| **sentencias** | `{ $, else }` | Fin de programa o fin de bloque `if`. |
| **sentencia** | `{ id, if, while, $, else }` | Inicio de otra sentencia o fin de bloque. |
| **expresion** | `{ ;, ), +, -, *, /, else }` | Operadores, cierre de paréntesis o fin de instrucción. |

---

## 2. Construcción de la Tabla de Análisis Sintáctico (Resumen)

A continuación se muestra una representación parcial de la Tabla ACTION y GOTO implementada en `ParserLR.cpp`. Esta tabla maneja los desplazamientos (**s**hift) y reducciones (**r**educe) basados en el autómata LR(0) y los conjuntos FOLLOW para desambiguar (SLR).

*(Nota: "s5" significa Desplazar e ir al estado 5. "r3" significa Reducir usando la producción 3).*

| Estado | int | float | id | if | while | ; | = | EOF ($) | ... | GOTO (NoTerm) |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :--- |
| **0** | s5 | s6 | | | | | | | | prog->1, decls->2, tipo->4 |
| **1** | | | | | | | | **ACC** | | |
| **2** | s5 | s6 | s30 | s40 | s50 | | | | | sents->90, sent->91 |
| **3** | r3 | r3 | r3 | r3 | r3 | | | | | (Reducción Declaraciones) |
| **4** | | | s21 | | | | | | | lista_var->22 |
| **5** | | | r5 | | | | | | | (Reducción Tipo -> int) |
| **30** | | | | | | | s31 | | | (Inicio Asignación) |
| **31** | | | s37 | | | | | | | expresion->33 |
| **33** | | | | | | s36 | | | | (+ -> s34) |
| **36** | | | r11 | r11 | r11 | | | r11 | | (Fin Asignación) |
| **90** | | | s30 | s40 | s50 | | | r1 | | (Fin Programa) |

### Conflictos Resueltos
En el estado donde se procesa `expresion -> expresion . + expresion`, existe un conflicto **Shift/Reduce** clásico de las expresiones aritméticas.
* **Decisión:** Se optó por **Shift** para dar precedencia a la continuación de la expresión, permitiendo el encadenamiento de operaciones (asociatividad por derecha en esta implementación simple LR).

---

## 3. Pruebas Realizadas

Se generaron 4 archivos de prueba adicionales para validar el compilador.

### Pruebas Válidas
1.  **test_valido1**: Verifica el uso correcto de `if` y `else` con bloques de sentencias simples.
    * *Resultado:* `Cadena aceptada correctamente.`
2.  **test_valido2**: Verifica el ciclo `while` y expresiones aritméticas complejas.
    * *Resultado:* `Cadena aceptada correctamente.`

### Pruebas Inválidas
1.  **test_invalido1**: Error de sintaxis por falta de punto y coma `;`.
    * *Resultado:* `Error Sintactico en linea 3 Token: 3` (El parser detecta el identificador siguiente cuando esperaba cierre de instrucción).
2.  **test_invalido2**: Error léxico por inclusión de caracter `$` no permitido.
    * *Resultado:* `ERROR LEXICO $ en linea 3` (El programa termina inmediatamente gracias a la corrección en `lexer.ll`).