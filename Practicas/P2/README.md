# Práctica 2: Análisis de la Gramática

## 1. Las partes de nuestra gramática (N, Σ, S)

Ok, primero lo primero. Nuestra gramática se divide en tres partes clave:

* **N (No-terminales):** Básicamente, son los conceptos o "bloques de construcción" de nuestro lenguaje, como `expresion` o `sentencia`. No son código real, sino las ideas que lo forman.
    * $N = \{ \text{programa, declaraciones, declaracion, tipo, lista\_var, sentencias, sentencia, expresion} \}$

* **Σ (Terminales):** Estas son las palabras y símbolos que sí existen en el código, como `if`, `while`, `+`, `(`, etc. Son las piezas del LEGO con las que construimos todo.
    * $\Sigma = \{ \text{int, float, if, else, while, identificador, numero, +, -, *, /, =, (, ), ;, ,, eof} \}$

* **S (Símbolo Inicial):** Es el punto de partida. En nuestro caso, todo empieza con `programa`.
    * $S = \text{programa}$

---

## 2. Arreglando la ambigüedad

El primer problema es que la gramática es **ambigua**. No sabe qué hacer con algo como `a = 3 + 5 * 2;`. ¿Suma primero o multiplica? Para que entienda el orden correcto (primero `*` y `/`, luego `+` y `-`), tuvimos que reescribir las reglas.

La solución fue crear "niveles" con nuevos no-terminales (`termino` y `factor`) para forzar la precedencia de los operadores.

* **Gramática Original (Ambigua):**
    ```
    expresion → expresion + expresion | expresion * expresion ...
    ```

* **Gramática Corregida (Sin Ambigüedad):**
    ```
    expresion → expresion + termino | expresion - termino | termino
    termino → termino * factor | termino / factor | factor
    factor → ( expresion ) | identificador | numero
    ```

---

## 3. Eliminando la recursividad por la izquierda

Nuestro tipo de analizador (descenso recursivo) no se lleva bien con reglas como `expresion → expresion + termino`, donde el mismo no-terminal aparece al inicio. Esto lo confunde y crea bucles infinitos.

Para arreglarlo, aplicamos una transformación estándar que básicamente "voltea" la regla para que ya no empiece con el mismo símbolo.

* **Ejemplo de transformación para `expresion`:**
    * `expresion → termino expresion'`
    * `expresion' → + termino expresion' | - termino expresion' | ε`

Hicimos esto para todas las reglas que tenían este problema (`declaraciones`, `lista_var`, `sentencias`, etc.).

---

## 4. Revisando la factorización

Después de los arreglos, checamos si había otro problema: que una regla tuviera dos o más opciones que empezaran con el mismo token. Por ejemplo, si `sentencia` tuviera dos caminos que inician con `identificador`.

Afortunadamente, al revisar nuestra gramática, vimos que cada alternativa empieza con algo diferente (`if`, `while`, `identificador`, `int`, `float`...). Así que nos salvamos de este paso, **no hizo falta factorizar**.

---

## 5. Nuevos Conjuntos N y P (Gramática Final)

Tras aplicar todas las transformaciones, la gramática final, lista para ser implementada, queda definida por los siguientes conjuntos:

* **Nuevo N (No-terminales):**
    * $N = \{ \text{programa, declaraciones, declaraciones', declaracion, tipo, lista\_var, lista\_var', sentencias, sentencias', sentencia, expresion, expresion', termino, termino', factor} \}$

* **Nuevas P (Producciones):**
    * `programa → declaraciones sentencias`
    * `declaraciones → declaracion declaraciones'`
    * `declaraciones' → declaracion declaraciones' | ε`
    * `declaracion → tipo lista_var ;`
    * `tipo → int | float`
    * `lista_var → identificador lista_var'`
    * `lista_var' → , identificador lista_var' | ε`
    * `sentencias → sentencia sentencias'`
    * `sentencias' → sentencia sentencias' | ε`
    * `sentencia → identificador = expresion ; | if ( expresion ) sentencias else sentencias | while ( expresion ) sentencias`
    * `expresion → termino expresion'`
    * `expresion' → + termino expresion' | - termino expresion' | ε`
    * `termino → factor termino'`
    * `termino' → * factor termino' | / factor termino' | ε`
    * `factor → ( expresion ) | identificador | numero`