Práctica 0: Sistema de procesamiento de Lenguaje

Preprocesamiento del primer programa (programa.c)

Comando:
<pre>cpp programa.c programa.i</pre>

¿Qué ocurre cuando se invoca el comando cpp con esos argumentos?
El comando cpp realiza el preprocesamiento del programa fuente programa.c y genera el archivo programa.i. Se eliminan los comentarios, se expanden las macros y se incluyen los archivos de cabecera.

¿Qué similitudes encuentra entre los archivos programa.c y programa.i?
El código esencial es el mismo: la lógica y las funciones escritas por el usuario permanecen. La diferencia es que programa.i incluye las cabeceras y ya no contiene macros ni comentarios.

¿Qué pasa con las macros y los comentarios del código fuente original en programa.i?
Los comentarios se eliminan. Cada invocación de macro se reemplaza por su texto de sustitución, según lo definido en #define.

Compare el contenido de programa.i con el de stdio.h e indique de forma general las similitudes entre ambos archivos.
programa.i contiene parte del contenido de stdio.h, incluyendo las funciones, tipos y constantes definidas en la biblioteca estándar de C.

¿A qué etapa corresponde este proceso?
Al preprocesamiento del código fuente.

---

Compilación a ensamblador

Comando:
<pre> gcc -Wall -S programa.i </pre>

¿Para qué sirve la opción -Wall?
Activa todas las advertencias que el compilador puede detectar sobre código sospechoso o inseguro.

¿Qué le indica a gcc la opción -S?
Detenerse después de compilar a ensamblador; no genera código objeto ni ejecutable.

¿Qué contiene el archivo de salida y cuál es su extensión?
Contiene el código en lenguaje ensamblador con extensión .s

¿A qué etapa corresponde este comando?
A la compilación, del lenguaje C a ensamblador.

---

Ensamblado a código objeto

Comando:
<pre>as programa.s -o programa.o</pre>

Antes de revisarlo, indique cuál es su hipótesis sobre lo que debe contener el archivo con extensión .o
Es el programa objeto, ya ensamblado y listo para enlazar.

Diga de forma general qué contiene el archivo programa.o y por qué se visualiza de esa manera
Contiene código máquina y símbolos extraños, ilegible directamente, porque está en formato binario.

¿Qué programa se invoca con as?
El ensamblador (as).

¿A qué etapa corresponde la llamada a este programa?
A la etapa de ensamblado.

---

Enlazado a ejecutable

Comando:
<pre>ld -o ejecutable -dynamic-linker /usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2 \
   /usr/lib/x86_64-linux-gnu/Scrt1.o \
   /usr/lib/x86_64-linux-gnu/crti.o \
   programa.o \
   -lc \
   /usr/lib/x86_64-linux-gnu/crtn.o \
   /usr/lib/gcc/x86_64-linux-gnu/13/crtendS.o \
   /usr/lib/gcc/x86_64-linux-gnu/13/crtbeginS.o</pre>


En caso de que ld mande errores, proponga una solución
Si faltan archivos, verificar sus rutas exactas en el sistema o usar gcc programa.o -o ejecutable para que el compilador gestione automáticamente el enlazado.

Describa el resultado obtenido
El enlazador combina los archivos objeto con las bibliotecas del sistema y genera un ejecutable listo para correr.

Resultado de ejecutar ./ejecutable
Hola Mundo !
Resultado : 28.274401

---

Activando la macro PI

Cambio en programa.c:
#define PI 3.1415926535897

Generar nuevo archivo preprocesado:
cpp programa.c programa2.i

¿Cambia en algo la ejecución final?
Sí, cambia el resultado del área, más preciso:

Hola Mundo !
Resultado : 28.274334

Esto confirma que las macros afectan directamente los cálculos y que el preprocesador sustituyó correctamente la macro.

---

Segundo programa con 4 directivas distintas

Código:

<pre>#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define RADIO 5
#undef PI
#define PI 3.1415926535897

#ifdef DEBUG
#define LOG(x) printf("DEBUG: %s\n", x)
#else
#define LOG(x)
#endif

int main(void) {
    LOG("Inicio del programa");
    printf("Hola, mundo!\n");

    double area = PI * pow(RADIO, 2);
    printf("Área del círculo de radio %d: %f\n", RADIO, area);
    printf("Esta es la línea %d del archivo %s\n", __LINE__, __FILE__);
    return 0;
}
</pre>


Explicación de las directivas:

<pre>#define: Define macros o constantes. Aquí define RADIO y PI para el cálculo del área.
#undef: Elimina definiciones anteriores de macros. Aquí permite redefinir PI sin conflicto.
#ifdef / #else: Incluye código solo si una macro está definida. Aquí activa la macro LOG() solo si DEBUG está definido.
#include: Incluye archivos de cabecera. Aquí <math.h> para usar pow() y funciones matemáticas.</pre>



Conclusión:
El proceso de pasar de un código fuente en C a un ejecutable involucra múltiples etapas: preprocesamiento, compilación, ensamblado y enlazado. Cada etapa transforma el código de manera específica y es fundamental para entender cómo funciona un compilador. Las directivas del preprocesador permiten controlar la inclusión de código, definir constantes y macros, y condicionar la compilación, demostrando cómo pequeñas modificaciones en el código fuente pueden afectar el resultado final del programa.
