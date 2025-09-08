#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define RADIO 5          // Define una constante
#undef PI
#define PI 3.1415926535897 // Redefine PI si estaba definida

#ifdef DEBUG
#define LOG(x) printf("DEBUG: %s\n", x) // Macro condicional para depuración
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
