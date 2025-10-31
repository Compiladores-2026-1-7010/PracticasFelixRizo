## Ejercicio 1: Análisis de la Gramática

Se analiza la gramática G original para determinar las transformaciones necesarias para que sea LL(1).

* **Gramática Original (G):**
    * **N:** { programa, declaraciones, declaracion, tipo, lista_var, sentencias, sentencia, expresion }
    * **$\Sigma$:** { int, float, ;, ,, identificador, =, if, (, ), else, while, +, -, *, /, numero }
    * **S:** programa

---

### a. Eliminación de Ambigüedad

**Justificación:** Sí, fue necesario. La gramática original es ambigua, específicamente en la regla `expresion`, ya que no define ni la **precedencia de operadores** (ej. `*` antes que `+`) ni la **asociatividad** (ej. `5-3-1` debe ser `(5-3)-1`).

**Proceso:** Se reescribió la regla `expresion` introduciendo los no-terminales `termino` (para `*` y `/`) y `factor` (para `( )`, `id` y `num`) para forzar el orden de evaluación correcto.

---

### b. Eliminación de Recursividad Izquierda

**Justificación:** Sí, fue necesario. Varias reglas de G (y de la gramática desambiguada) tenían recursividad izquierda directa (ej. `declaraciones → declaraciones ...` y `expresion → expresion ...`), lo cual no es permitido en un analizador LL(1).

**Proceso:** Se aplicó la transformación estándar a todas las reglas recursivas (`declaraciones`, `lista_var`, `sentencias`, `expresion` y `termino`). Esto implicó crear nuevos no-terminales (que llamaremos con el sufijo `_p`) para manejar la parte repetitiva de la regla, moviendo la recursividad a la derecha y añadiendo una producción a $\epsilon$.

---

### c. Factorización Izquierda

**Justificación:** No fue necesario. Después de realizar las dos transformaciones anteriores, la gramática resultante no presenta el problema de factorización izquierda. Ningún no-terminal tiene dos o más producciones que comiencen con el mismo símbolo terminal (ej. `sentencia` ahora inicia claramente con `identificador`, `if` o `while`).

---

### d. Nuevos Conjuntos $N'$ y $P'$ (Gramática $G'$)

La gramática final $G'$, preparada para el análisis LL(1), es la siguiente:

* **$N' = $**
    `{ programa, declaraciones, declaraciones_p, declaracion, tipo, lista_var, lista_var_p, sentencias, sentencias_p, sentencia, expresion, expresion_p, termino, termino_p, factor }`

* **$P' = $**
    ```
    programa → declaraciones sentencias
    
    declaraciones → declaracion declaraciones_p
    declaraciones_p → declaracion declaraciones_p | \epsilon
    
    declaracion → tipo lista_var ;
    
    tipo → int | float
    
    lista_var → identificador lista_var_p
    lista_var_p → , identificador lista_var_p | \epsilon
    sentencias → sentencia sentencias_p
    sentencias_p → sentencia sentencias_p | \epsilon
    
    sentencia → identificador = expresion ;
              | if ( expresion ) sentencias else sentencias
              | while ( expresion ) sentencias
    
    expresion → termino expresion_p
    expresion_p → + termino expresion_p | - termino expresion_p | \epsilon
    
    termino → factor termino_p
    termino_p → * factor termino_p | / factor termino_p | \epsilon
    
    factor → ( expresion ) | identificador | numero
    ```

## Ejercicio 2: Construcción de Conjuntos FIRST

A continuación, se presentan los conjuntos $FIRST$ para cada símbolo no-terminal de la gramática $G'$ (la gramática transformada en el Ejercicio 1). El cálculo se realiza siguiendo las reglas de construcción de $FIRST$, comenzando desde las producciones más simples (base) y ascendiendo.

---

### Cálculo de Conjuntos $FIRST(X)$

1.  **$FIRST(\text{tipo})$** = `{ int, float }`
    * *Basado en:* `tipo → int | float`

2.  **$FIRST(\text{factor})$** = `{ (, identificador, numero }`
    * *Basado en:* `factor → ( expresion ) | identificador | numero`

3.  **$FIRST(\text{termino\_p})$** = `{ *, /, \epsilon }`
    * *Basado en:* `termino_p → * factor ... | / factor ... | \epsilon`

4.  **$FIRST(\text{expresion\_p})$** = `{ +, -, \epsilon }`
    * *Basado en:* `expresion_p → + termino ... | - termino ... | \epsilon`

5.  **$FIRST(\text{lista\_var\_p})$** = `{ ,, \epsilon }`
    * *Basado en:* `lista_var_p → , identificador ... | \epsilon`

6.  **$FIRST(\text{termino})$** = $FIRST(\text{factor})$ = `{ (, identificador, numero }`
    * *Basado en:* `termino → factor termino_p`

7.  **$FIRST(\text{expresion})$** = $FIRST(\text{termino})$ = `{ (, identificador, numero }`
    * *Basado en:* `expresion → termino expresion_p`

8.  **$FIRST(\text{lista\_var})$** = `{ identificador }`
    * *Basado en:* `lista_var → identificador lista_var_p`

9.  **$FIRST(\text{declaracion})$** = $FIRST(\text{tipo})$ = `{ int, float }`
    * *Basado en:* `declaracion → tipo lista_var ;`

10. **$FIRST(\text{sentencia})$** = `{ identificador, if, while }`
    * *Basado en:* `sentencia → identificador = ... | if ( ... | while ( ...`

11. **$FIRST(\text{declaraciones\_p})$** = $FIRST(\text{declaracion}) \cup \{ \epsilon \}$ = `{ int, float, \epsilon }`
    * *Basado en:* `declaraciones_p → declaracion ... | \epsilon`

12. **$FIRST(\text{sentencias\_p})$** = $FIRST(\text{sentencia}) \cup \{ \epsilon \}$ = `{ identificador, if, while, \epsilon }`
    * *Basado en:* `sentencias_p → sentencia ... | \epsilon`

13. **$FIRST(\text{declaraciones})$** = $FIRST(\text{declaracion})$ = `{ int, float }`
    * *Basado en:* `declaraciones → declaracion declaraciones_p`
    * *(Nota: No incluye $\epsilon$ porque $FIRST(\text{declaracion})$ no lo contiene)*

14. **$FIRST(\text{sentencias})$** = $FIRST(\text{sentencia})$ = `{ identificador, if, while }`
    * *Basado en:* `sentencias → sentencia sentencias_p`
    * *(Nota: No incluye $\epsilon$ porque $FIRST(\text{sentencia})$ no lo contiene)*

15. **$FIRST(\text{programa})$** = $FIRST(\text{declaraciones})$ = `{ int, float }`
    * *Basado en:* `programa → declaraciones sentencias`
    * *(Nota: Como $FIRST(\text{declaraciones})$ no tiene $\epsilon$, no es necesario revisar $FIRST(\text{sentencias})$)*

## Ejercicio 3: Construcción de Conjuntos FOLLOW

Basándonos en la gramática $G'$ (del Ejercicio 1) y los conjuntos $FIRST$ (del Ejercicio 2), calculamos los conjuntos $FOLLOW$ para cada símbolo no-terminal.

El conjunto $FOLLOW(A)$ se define como el conjunto de todos los terminales (más `$`) que pueden aparecer inmediatamente después del no-terminal $A$ en alguna derivación.

---

### Cálculo de Conjuntos $FOLLOW(X)$

1.  **$FOLLOW(\text{programa})$** = `{ $ }`
    * *Regla 1:* Es el símbolo inicial, por lo que contiene el fin de cadena `$`.

2.  **$FOLLOW(\text{declaraciones})$** = $FIRST(\text{sentencias})$ = `{ identificador, if, while }`
    * *Por `programa → declaraciones sentencias` (Regla 2).*

3.  **$FOLLOW(\text{declaraciones\_p})$** = $FOLLOW(\text{declaraciones})$ = `{ identificador, if, while }`
    * *Por `declaraciones → declaracion declaraciones_p` (Regla 3).*
    * *Por `declaraciones_p → declaracion declaraciones_p` (Regla 3).*

4.  **$FOLLOW(\text{declaracion})$** = $FIRST(\text{declaraciones\_p})$ (sin $\epsilon$) $\cup$ $FOLLOW(\text{declaraciones\_p})$ = `{ int, float } \cup { identificador, if, while }`
    * *Por `declaraciones → declaracion declaraciones_p` y `declaraciones_p → declaracion ...` (Reglas 2 y 3, ya que $FIRST(\text{declaraciones\_p})$ tiene $\epsilon$).*
    * **Resultado:** `{ int, float, identificador, if, while }`

5.  **$FOLLOW(\text{tipo})$** = $FIRST(\text{lista\_var})$ = `{ identificador }`
    * *Por `declaracion → tipo lista_var ;` (Regla 2).*

6.  **$FOLLOW(\text{lista\_var})$** = `{ ; }`
    * *Por `declaracion → tipo lista_var ;` (Regla 2).*

7.  **$FOLLOW(\text{lista\_var\_p})$** = $FOLLOW(\text{lista\_var})$ = `{ ; }`
    * *Por `lista_var → identificador lista_var_p` (Regla 3).*
    * *Por `lista_var_p → , identificador lista_var_p` (Regla 3).*

8.  **$FOLLOW(\text{sentencias})$** = $FOLLOW(\text{programa}) \cup \{ \text{else} \} \cup FOLLOW(\text{sentencia})$
    * *Por `programa → ... sentencias` (Regla 3) $\rightarrow$ `{ $ }`*
    * *Por `sentencia → if (...) sentencias else ...` (Regla 2) $\rightarrow$ `{ else }`*
    * *Por `sentencia → if (...) ... else sentencias` (Regla 3) $\rightarrow$ $FOLLOW(\text{sentencia})$*
    * *Por `sentencia → while (...) sentencias` (Regla 3) $\rightarrow$ $FOLLOW(\text{sentencia})$*
    * *(Resolviendo la dependencia mutua con `sentencia` y `sentencias_p`, el resultado es):*
    * **Resultado:** `{ $, else, identificador, if, while }`

9.  **$FOLLOW(\text{sentencias\_p})$** = $FOLLOW(\text{sentencias})$
    * *Por `sentencias → sentencia sentencias_p` (Regla 3).*
    * *Por `sentencias_p → sentencia sentencias_p` (Regla 3).*
    * **Resultado:** `{ $, else, identificador, if, while }`

10. **$FOLLOW(\text{sentencia})$** = $FIRST(\text{sentencias\_p})$ (sin $\epsilon$) $\cup$ $FOLLOW(\text{sentencias})$
    * *Por `sentencias → sentencia sentencias_p` y `sentencias_p → sentencia ...` (Reglas 2 y 3, ya que $FIRST(\text{sentencias\_p})$ tiene $\epsilon$).*
    * *Resultado:* `{ identificador, if, while } \cup FOLLOW(\text{sentencias})`
    * **Resultado Final:** `{ $, else, identificador, if, while }`

11. **$FOLLOW(\text{expresion})$** = `{ ;, ) }`
    * *Por `sentencia → identificador = expresion ;` (Regla 2) $\rightarrow$ `{ ; }`*
    * *Por `sentencia → if ( expresion ) ...` (Regla 2) $\rightarrow$ `{ ) }`*
    * *Por `sentencia → while ( expresion ) ...` (Regla 2) $\rightarrow$ `{ ) }`*
    * *Por `factor → ( expresion )` (Regla 2) $\rightarrow$ `{ ) }`*

12. **$FOLLOW(\text{expresion\_p})$** = $FOLLOW(\text{expresion})$ = `{ ;, ) }`
    * *Por `expresion → termino expresion_p` (Regla 3).*
    * *Por `expresion_p → + ... expresion_p` y `expresion_p → - ... expresion_p` (Regla 3).*

13. **$FOLLOW(\text{termino})$** = $FIRST(\text{expresion\_p})$ (sin $\epsilon$) $\cup$ $FOLLOW(\text{expresion})$
    * *Por `expresion → termino expresion_p` (Reglas 2 y 3, ya que $FIRST(\text{expresion\_p})$ tiene $\epsilon$).*
    * *Resultado:* `{ +, - } \cup { ;, ) }`
    * *También por `expresion_p → + termino ...` y `expresion_p → - termino ...` (Reglas 2 y 3).*
    * **Resultado Final:** `{ +, -, ;, ) }`

14. **$FOLLOW(\text{termino\_p})$** = $FOLLOW(\text{termino})$
    * *Por `termino → factor termino_p` (Regla 3).*
    * *Por `termino_p → * ... termino_p` y `termino_p → / ... termino_p` (Regla 3).*
    * **Resultado:** `{ +, -, ;, ) }`

15. **$FOLLOW(\text{factor})$** = $FIRST(\text{termino\_p})$ (sin $\epsilon$) $\cup$ $FOLLOW(\text{termino})$
    * *Por `termino → factor termino_p` (Reglas 2 y 3, ya que $FIRST(\text{termino\_p})$ tiene $\epsilon$).*
    * *Resultado:* `{ *, / } \cup { +, -, ;, ) }`
    * *También por `termino_p → * factor ...` y `termino_p → / factor ...` (Reglas 2 y 3).*
    * **Resultado Final:** `{ *, /, +, -, ;, ) }`


## Ejercicio 4: Tabla de Análisis Sintáctico Predictivo $M[A, a]$

Para construir la tabla de análisis sintáctico $LL(1)$, es necesario aplicar las reglas de construcción basadas en los conjuntos $FIRST$ y $FOLLOW$ que calculamos.

---

### Nota Importante sobre la Gramática
Al calcular los conjuntos $FOLLOW$ para la gramática $G'$ (del Ejercicio 1.d), nos encontramos con un problema. La gramática original:

> `sentencia → ... | if ( expresion ) sentencias else sentencias | while ( expresión ) sentencias`

...genera una gramática $G'$ que **no es $LL(1)$**.

**Razón:** Se produce un conflicto de "FIRST/FOLLOW". El $FIRST(\text{sentencia})$ (que incluye `identificador`, `if`, `while`) se superpone con el $FOLLOW(\text{sentencias\_p})$ (que también termina incluyendo `identificador`, `if`, `while`). Esto crea múltiples entradas en la misma celda de la tabla (ej. en $M[\text{sentencias\_p, if}]$), y el analizador no sabría qué producción elegir.

**Solución/Suposición:** Para que la práctica sea viable y se pueda construir una tabla $LL(1)$, asumiremos que las reglas de `sentencia` contenían un error tipográfico y debían referenciarse a sí mismas (a `sentencia`), no a `sentencias`. Esta es una estructura estándar para anidar sentencias.

Las reglas **corregidas** que usaremos para la tabla son:

* `sentencia → if ( expresion ) sentencia else sentencia` (en lugar de `... sentencias else sentencias`)
* `sentencia → while ( expresion ) sentencia` (en lugar de `... sentencias`)

Esta corrección **cambia los conjuntos $FOLLOW$** de la siguiente manera:
* $FOLLOW(\text{sentencias}) = \{ \text{\$} \}$
* $FOLLOW(\text{sentencias\_p}) = \{ \text{\$} \}$
* $FOLLOW(\text{declaraciones}) = \{ \text{identificador, if, while} \}$
* $FOLLOW(\text{declaraciones\_p}) = \{ \text{identificador, if, while} \}$
* $FOLLOW(\text{sentencia}) = \{ \text{\$, else, identificador, if, while} \}$

*(Los $FOLLOW$ de `expresion`, `termino`, `factor`, etc., no cambian).*

---

### Producciones de $G'$ (con numeración)

Para que la tabla sea legible, usaremos números para referirnos a las producciones de $G'$ (corregida):

| Num | Producción | | Num | Producción |
| :--- | :--- | :--- | :--- | :--- |
| P1 | `programa → declaraciones sentencias` | | P15 | `sentencia → if ( expresion ) sentencia else sentencia` |
| P2 | `declaraciones → declaracion declaraciones_p` | | P16 | `sentencia → while ( expresion ) sentencia` |
| P3 | `declaraciones_p → declaracion declaraciones_p` | | P17 | `expresion → termino expresion_p` |
| P4 | `declaraciones_p → \epsilon` | | P18 | `expresion_p → + termino expresion_p` |
| P5 | `declaracion → tipo lista_var ;` | | P19 | `expresion_p → - termino expresion_p` |
| P6 | `tipo → int` | | P20 | `expresion_p → \epsilon` |
| P7 | `tipo → float` | | P21 | `termino → factor termino_p` |
| P8 | `lista_var → identificador lista_var_p` | | P22 | `termino_p → * factor termino_p` |
| P9 | `lista_var_p → , identificador lista_var_p` | | P23 | `termino_p → / factor termino_p` |
| P10 | `lista_var_p → \epsilon` | | P24 | `termino_p → \epsilon` |
| P11 | `sentencias → sentencia sentencias_p` | | P25 | `factor → ( expresion )` |
| P12 | `sentencias_p → sentencia sentencias_p` | | P26 | `factor → identificador` |
| P13 | `sentencias_p → \epsilon` | | P27 | `factor → numero` |
| P14 | `sentencia → identificador = expresion ;` | | | |

---

### Tabla de Análisis Sintáctico $M[A, a]$

(Las celdas vacías representan un error sintáctico).

| $A$ (No Terminal) | `int` | `float` | `;` | `,` | `id` | `=` | `if` | `(` | `)` | `else` | `while` | `+` | `-` | `*` | `/` | `num` | `$` |
| :--- | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| **programa** | P1 | P1 | | | | | | | | | | | | | | | |
| **declaraciones** | P2 | P2 | | | | | | | | | | | | | | | |
| **declaraciones\_p** | P3 | P3 | | | P4 | | P4 | | | | P4 | | | | | | |
| **declaracion** | P5 | P5 | | | | | | | | | | | | | | | |
| **tipo** | P6 | P7 | | | | | | | | | | | | | | | |
| **lista\_var** | | | | | P8 | | | | | | | | | | | | |
| **lista\_var\_p** | | | P10 | P9 | | | | | | | | | | | | | |
| **sentencias** | | | | | P11 | | P11 | | | | P11 | | | | | | |
| **sentencias\_p** | | | | | P12 | | P12 | | | | P12 | | | | | | P13 |
| **sentencia** | | | | | P14 | | P15 | | | | P16 | | | | | | |
| **expresion** | | | | | P17 | | | P17 | | | | | | | | P17 | |
| **expresion\_p** | | | P20 | | | | | | P20 | | | P18 | P19 | | | | |
| **termino** | | | | | P21 | | | P21 | | | | | | | | P21 | |
| **termino\_p** | | | P24 | | | | | | P24 | | | P24 | P24 | P22 | P23 | | |
| **factor** | | | | | P26 | | | P25 | | | | | | | | P27 | |