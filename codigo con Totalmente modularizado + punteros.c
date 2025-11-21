#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define N 5
#define MAX 50
#define BUF 200

/* ---------- VALIDACIONES --------- */
int tiene_letras(const char *s) {
    while (*s) {
        if (isalpha((unsigned char)*s)) return 1;
        s++;
    }
    return 0;
}

void leer_nombre(char *dest, const char *msg) {
    char buf[BUF];
    while (1) {
        printf("%s", msg);
        if (!fgets(buf, BUF, stdin)) continue;

        char *nl = strchr(buf, '\n');
        if (nl) *nl = '\0';

        if (!tiene_letras(buf)) {
            printf("Inválido: debe contener letras.\n");
            continue;
        }

        strncpy(dest, buf, MAX - 1);
        dest[MAX - 1] = '\0';
        return;
    }
}

int leer_entero(const char *msg) {
    char buf[BUF];
    int v;
    while (1) {
        printf("%s", msg);
        if (!fgets(buf, BUF, stdin)) continue;
        char extra[2];

        if (sscanf(buf, "%d %1s", &v, extra) == 1) return v;

        printf("Inválido: entero requerido.\n");
    }
}

float leer_float(const char *msg) {
    char buf[BUF];
    float v;
    while (1) {
        printf("%s", msg);
        if (!fgets(buf, BUF, stdin)) continue;
        char extra[2];

        if (sscanf(buf, "%f %1s", &v, extra) == 1) return v;

        printf("Inválido: número decimal incorrecto.\n");
    }
}

/* ---------- OPERACIONES --------- */
int buscar_producto(char nombres[][MAX], const char *key) {
    for (int i = 0; i < N; i++)
        if (strcmp(nombres[i], key) == 0)
            return i;
    return -1;
}

void ingresar_producto(int idx, char nombres[][MAX],
                       float *t, int *r, int *d) {
    printf("\n--- Producto %d ---\n", idx + 1);
    leer_nombre(nombres[idx], "Nombre: ");
    t[idx] = leer_float("Tiempo (h): ");
    r[idx] = leer_entero("Recursos requeridos: ");
    d[idx] = leer_entero("Demanda: ");
}

void mostrar_todo(char nombres[][MAX], float *t, int *r, int *d) {
    printf("\n--- Lista de Productos ---\n");
    for (int i = 0; i < N; i++) {
        printf("%d) %s\n", i + 1, nombres[i]);
        printf("   T: %.2f | R: %d | D: %d\n",
               t[i], r[i], d[i]);
    }
}

void editar_producto(char nombres[][MAX], float *t, int *r, int *d) {
    char key[MAX];
    leer_nombre(key, "Producto a editar: ");

    int idx = buscar_producto(nombres, key);
    if (idx == -1) {
        printf("No existe.\n");
        return;
    }

    printf("\n1) Nombre\n2) Tiempo\n3) Recursos\n4) Demanda\n");
    int op = leer_entero("Opción: ");

    if (op == 1) leer_nombre(nombres[idx], "Nuevo nombre: ");
    else if (op == 2) t[idx] = leer_float("Nuevo tiempo: ");
    else if (op == 3) r[idx] = leer_entero("Nuevos recursos: ");
    else if (op == 4) d[idx] = leer_entero("Nueva demanda: ");
}

void eliminar_producto(char nombres[][MAX], float *t, int *r, int *d) {
    char key[MAX];
    leer_nombre(key, "Producto a eliminar: ");

    int idx = buscar_producto(nombres, key);
    if (idx == -1) {
        printf("No existe.\n");
        return;
    }

    for (int i = idx; i < N - 1; i++) {
        strcpy(nombres[i], nombres[i + 1]);
        t[i] = t[i + 1];
        r[i] = r[i + 1];
        d[i] = d[i + 1];
    }

    strcpy(nombres[N - 1], "ELIMINADO");
    t[N - 1] = 0;
    r[N - 1] = 0;
    d[N - 1] = 0;

    printf("Eliminado correctamente.\n");
}

void calcular_totales(float *t, int *r, int *d) {
    double T = 0;
    long R = 0;

    for (int i = 0; i < N; i++) {
        T += (*(t + i)) * (*(d + i));
        R += (*(r + i)) * (*(d + i));

    }

    printf("\nTiempo total: %.2f\n", T);
    printf("Recursos totales: %ld\n", R);

    double dispT = leer_float("Tiempo disponible: ");
    long dispR = leer_entero("Recursos disponibles: ");

    printf(dispT >= T ? "Tiempo suficiente.\n" : "Tiempo insuficiente.\n");
    printf(dispR >= R ? "Recursos suficientes.\n" : "Recursos insuficientes.\n");
}

/* ---------- MAIN PRINCIPAL ---------- */
int main() {
    char nombres[N][MAX];
    float tiempo[N];
    int recursos[N];
    int demanda[N];

    for (int i = 0; i < N; i++)
        ingresar_producto(i, nombres, tiempo, recursos, demanda);

    while (1) {
        printf("\n1) Mostrar productos\n2) Editar\n3) Eliminar\n4) Totales\n5) Salir\n");
        int op = leer_entero("Opción: ");

        if (op == 1) mostrar_todo(nombres, tiempo, recursos, demanda);
        else if (op == 2) editar_producto(nombres, tiempo, recursos, demanda);
        else if (op == 3) eliminar_producto(nombres, tiempo, recursos, demanda);
        else if (op == 4) calcular_totales(tiempo, recursos, demanda);
        else if (op == 5) break;
    }

    return 0;
}

