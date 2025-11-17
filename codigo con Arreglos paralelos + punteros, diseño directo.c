#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define N 5
#define MAX 50
#define BUF 200

int contiene_letras(const char *s) {
    while (*s) {
        if (isalpha((unsigned char)*s)) return 1;
        s++;
    }
    return 0;
}

void leer_cadena_validada(char *dest, const char *msg) {
    char buf[BUF];
    while (1) {
        printf("%s", msg);
        if (!fgets(buf, BUF, stdin)) continue;
        char *nl = strchr(buf, '\n');
        if (nl) *nl = '\0';

        if (strlen(buf) == 0) {
            printf("Inválido: vacío.\n");
            continue;
        }
        if (!contiene_letras(buf)) {
            printf("Inválido: debe contener letras.\n");
            continue;
        }
        strncpy(dest, buf, MAX - 1);
        dest[MAX - 1] = '\0';
        return;
    }
}

int leer_entero_validado(const char *msg) {
    char buf[BUF];
    int v;
    while (1) {
        printf("%s", msg);
        if (!fgets(buf, BUF, stdin)) continue;
        char extra[2];

        if (sscanf(buf, "%d %1s", &v, extra) == 1) return v;

        printf("Inválido: se esperaba ENTERO.\n");
    }
}

float leer_float_validado(const char *msg) {
    char buf[BUF];
    float v;
    while (1) {
        printf("%s", msg);
        if (!fgets(buf, BUF, stdin)) continue;
        char extra[2];

        if (sscanf(buf, "%f %1s", &v, extra) == 1) return v;

        printf("Inválido: número decimal inválido.\n");
    }
}

int buscar(char nombres[][MAX], const char *key) {
    for (int i = 0; i < N; i++)
        if (strcmp(nombres[i], key) == 0)
            return i;
    return -1;
}

void mostrar(char nombres[][MAX], float *t, int *r, int *d) {
    printf("\n--- Productos ---\n");
    for (int i = 0; i < N; i++) {
        printf("%d) %s\n", i + 1, nombres[i]);
        printf("   Tiempo: %.2f | Recursos: %d | Demanda: %d\n",
               *(t + i), *(r + i), *(d + i));
    }
}

void inicializar(char nombres[][MAX], float *ti, int *re, int *de) {
    for (int i = 0; i < N; i++) {
        printf("\nProducto %d:\n", i + 1);
        leer_cadena_validada(nombres[i], "Nombre: ");
        *(ti + i) = leer_float_validado("Tiempo (horas): ");
        *(re + i) = leer_entero_validado("Recursos: ");
        *(de + i) = leer_entero_validado("Demanda: ");
    }
}

void editar(char nombres[][MAX], float *t, int *r, int *d) {
    char key[MAX];
    leer_cadena_validada(key, "Nombre del producto a editar: ");
    int idx = buscar(nombres, key);

    if (idx == -1) {
        printf("No existe.\n");
        return;
    }

    printf("1) Nombre\n2) Tiempo\n3) Recursos\n4) Demanda\n");
    int op = leer_entero_validado("Opción: ");

    if (op == 1) leer_cadena_validada(nombres[idx], "Nuevo nombre: ");
    else if (op == 2) *(t + idx) = leer_float_validado("Nuevo tiempo: ");
    else if (op == 3) *(r + idx) = leer_entero_validado("Nuevos recursos: ");
    else if (op == 4) *(d + idx) = leer_entero_validado("Nueva demanda: ");
}

void eliminar(char nombres[][MAX], float *t, int *r, int *d) {
    char key[MAX];
    leer_cadena_validada(key, "Nombre del producto a eliminar: ");

    int idx = buscar(nombres, key);
    if (idx == -1) {
        printf("No existe.\n");
        return;
    }

    for (int i = idx; i < N - 1; i++) {
        strcpy(nombres[i], nombres[i + 1]);
        *(t + i) = *(t + i + 1);
        *(r + i) = *(r + i + 1);
        *(d + i) = *(d + i + 1);
    }

    strcpy(nombres[N - 1], "ELIMINADO");
    *(t + N - 1) = 0;
    *(r + N - 1) = 0;
    *(d + N - 1) = 0;

    printf("Eliminado.\n");
}

void totales(float *t, int *r, int *d) {
    double T = 0;
    long R = 0;

    for (int i = 0; i < N; i++) {
        T += (*(t + i)) * (*(d + i));
        R += (*(r + i)) * (*(d + i));
    }

    printf("\nTiempo total: %.2f\n", T);
    printf("Recursos totales: %ld\n", R);

    double dispT = leer_float_validado("Tiempo disponible: ");
    long dispR = leer_entero_validado("Recursos disponibles: ");

    if (dispT >= T) printf("Tiempo suficiente.\n");
    else printf("Tiempo insuficiente.\n");

    if (dispR >= R) printf("Recursos suficientes.\n");
    else printf("Recursos insuficientes.\n");
}

int main() {
    char nombres[N][MAX];
    float tiempo[N];
    int recursos[N];
    int demanda[N];

    inicializar(nombres, tiempo, recursos, demanda);

    while (1) {
        printf("\n1) Mostrar\n2) Editar\n3) Eliminar\n4) Totales\n5) Salir\n");
        int op = leer_entero_validado("Opción: ");

        if (op == 1) mostrar(nombres, tiempo, recursos, demanda);
        else if (op == 2) editar(nombres, tiempo, recursos, demanda);
        else if (op == 3) eliminar(nombres, tiempo, recursos, demanda);
        else if (op == 4) totales(tiempo, recursos, demanda);
        else if (op == 5) break;
    }

    return 0;
}
