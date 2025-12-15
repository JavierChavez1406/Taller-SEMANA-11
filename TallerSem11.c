#include <stdio.h>

#define MAX 10

typedef struct {
    char titulo[101];
    char autor[51];
    int año;  
} DatosLibro;

typedef struct {
    int id;
    DatosLibro datos;  
    int estado;       
} Libro;

int iguales(const char *a, const char *b) {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i]) return 0;
        i++;
    }
    return (a[i] == b[i]);
}

int leerEntero(const char *mensaje, int *valor) {
    int leido;
    printf("%s", mensaje);
    leido = scanf("%d", valor);
    if (leido != 1) {
        scanf("%*s"); 
        return 0;
    }
    return 1;
}

int leerTextoTitulo(char *texto) {
    printf("Titulo: ");
    scanf(" %100[^\n]", texto);
    return 1;
}

int leerTextoAutor(char *texto) {
    printf("Autor: ");
    scanf(" %50[^\n]", texto);
    return 1;
}

int buscarPorID(Libro libros[], int numLibros, int idBuscado) {
    int i;
    for (i = 0; i < numLibros; i++) {
        if (libros[i].id == idBuscado) return i;
    }
    return -1;
}

int buscarPorTitulo(Libro libros[], int numLibros, const char *tituloBuscado) {
    int i;
    for (i = 0; i < numLibros; i++) {
        if (iguales(libros[i].datos.titulo, tituloBuscado)) return i;
    }
    return -1;
}

int estadoTxt(int estado) {
    if (estado == 0) printf("Disponible");
    else printf("Prestado");
    return 1;
}

int agregar(Libro libros[], int numLibros) {
    Libro libroNuevo;

    if (numLibros >= MAX) {
        printf("No se pueden agregar mas de %d libros.\n", MAX);
        return numLibros;
    }

    while (1) {
        if (!leerEntero("ID (unico): ", &libroNuevo.id)) {
            printf("ID invalido.\n");
            continue;
        }
        if (buscarPorID(libros, numLibros, libroNuevo.id) != -1) {
            printf("Ese ID ya existe.\n");
            continue;
        }
        break;
    }

    leerTextoTitulo(libroNuevo.datos.titulo);
    leerTextoAutor(libroNuevo.datos.autor);

    while (!leerEntero("Año: ", &libroNuevo.datos.año)) {
        printf("Año invalido.\n");
    }

    while (1) {
        if (!leerEntero("Estado (0=Disponible,1=Prestado): ", &libroNuevo.estado)) {
            printf("Estado invalido.\n");
            continue;
        }
        if (libroNuevo.estado == 0 || libroNuevo.estado == 1) break;
        printf("Solo 0 o 1.\n");
    }

    libros[numLibros] = libroNuevo;
    printf("Libro agregado.\n");
    return numLibros + 1;
}

int listar(Libro libros[], int numLibros) {
    int i;

    if (numLibros == 0) {
        printf("No hay libros.\n");
        return 0;
    }

    printf("\n%-5s | %-30s | %-20s | %-4s | %-10s\n",
           "ID", "Titulo", "Autor", "Año", "Estado");
    printf("--------------------------------------------------------------------------\n");

    for (i = 0; i < numLibros; i++) {
        printf("%-5d | %-30.30s | %-20.20s | %-4d | ",
               libros[i].id, libros[i].datos.titulo,
               libros[i].datos.autor, libros[i].datos.año);
        estadoTxt(libros[i].estado);
        printf("\n");
    }
    return 1;
}

int verLibro(Libro libro) {
    printf("\nID: %d\n", libro.id);
    printf("Titulo: %s\n", libro.datos.titulo);
    printf("Autor: %s\n", libro.datos.autor);
    printf("Año: %d\n", libro.datos.año);
    printf("Estado: ");
    estadoTxt(libro.estado);
    printf("\n");
    return 1;
}

int buscar(Libro libros[], int numLibros) {
    int opcion;

    if (numLibros == 0) {
        printf("No hay libros.\n");
        return 0;
    }

    if (!leerEntero("Buscar por (1=ID, 2=Titulo): ", &opcion)) {
        printf("Opcion invalida.\n");
        return 0;
    }

    if (opcion == 1) {
        int idBuscado, indice;
        if (!leerEntero("ID: ", &idBuscado)) { printf("ID invalido.\n"); return 0; }
        indice = buscarPorID(libros, numLibros, idBuscado);
        if (indice == -1) { printf("No encontrado.\n"); return 0; }
        return verLibro(libros[indice]);
    }

    if (opcion == 2) {
        char tituloBuscado[101];
        int indice;
        printf("Titulo (exacto): ");
        scanf(" %100[^\n]", tituloBuscado);
        indice = buscarPorTitulo(libros, numLibros, tituloBuscado);
        if (indice == -1) { printf("No encontrado.\n"); return 0; }
        return verLibro(libros[indice]);
    }

    printf("Opcion invalida.\n");
    return 0;
}

int cambiarEstado(Libro libros[], int numLibros) {
    int idLibro, indice;

    if (!leerEntero("ID del libro: ", &idLibro)) { printf("ID invalido.\n"); return 0; }

    indice = buscarPorID(libros, numLibros, idLibro);
    if (indice == -1) { printf("No encontrado.\n"); return 0; }

    libros[indice].estado = 1 - libros[indice].estado;
    printf("Nuevo estado: ");
    estadoTxt(libros[indice].estado);
    printf("\n");
    return 1;
}

int eliminar(Libro libros[], int numLibros) {
    int idEliminar, indice, i;

    if (!leerEntero("ID a eliminar: ", &idEliminar)) { printf("ID invalido.\n"); return numLibros; }

    indice = buscarPorID(libros, numLibros, idEliminar);
    if (indice == -1) { printf("No encontrado.\n"); return numLibros; }

    for (i = indice; i < numLibros - 1; i++) {
        libros[i] = libros[i + 1];
    }

    printf("Eliminado.\n");
    return numLibros - 1;
}

int main() {
    Libro libros[MAX];
    int numLibros = 0;
    int opcion;

    do {
        printf("\n--- MENU BIBLIOTECA ---\n");
        printf("1. Agregar\n");
        printf("2. Listar\n");
        printf("3. Buscar\n");
        printf("4. Cambiar estado\n");
        printf("5. Eliminar\n");
        printf("0. Salir\n");

        if (!leerEntero("Opcion: ", &opcion)) {
            printf("Opcion invalida.\n");
            opcion = -1;
            continue;
        }

        if (opcion == 1) numLibros = agregar(libros, numLibros);
        else if (opcion == 2) listar(libros, numLibros);
        else if (opcion == 3) buscar(libros, numLibros);
        else if (opcion == 4) cambiarEstado(libros, numLibros);
        else if (opcion == 5) numLibros = eliminar(libros, numLibros);
        else if (opcion == 0) printf("Saliendo...\n");
        else printf("Opcion invalida.\n");

    } while (opcion != 0);

    return 0;
}
