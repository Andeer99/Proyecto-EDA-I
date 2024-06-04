#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <unistd.h>

typedef struct Adoptante {
    int id;
    char nombre[50];
    char direccion[100];
    char correo[50];
    char telefono[20];
    char contrasena[20];
} Adoptante;

typedef struct Nodo {
    Adoptante adoptantito;
    struct Nodo *siguiente;
} Nodo;

typedef struct Cola {
    Nodo *frente;
    Nodo *final;
} Cola;

typedef struct Mascota {
    int id;
    char nombre[50];
    char especie[20];
    char raza[30];
    int edad;
    char tamano[20];
    float peso;
    char descripcion[200];
    struct Mascota *siguiente;
} Mascota;

typedef struct Lista {
    Mascota *cabeza;
} Lista;

void inicializarCola(Cola *cola) {
    cola->frente = NULL;
    cola->final = NULL;
}

void inicializarLista(Lista *lista) {
    lista->cabeza = NULL;
}

int estaVacia(Cola *cola) {
    return cola->frente == NULL;
}

void encolar(Cola *cola, Adoptante nuevoAdoptante) {
    Nodo *nuevoNodo = (Nodo *)malloc(sizeof(Nodo));
    nuevoNodo->adoptantito = nuevoAdoptante;
    nuevoNodo->siguiente = NULL;
    if (estaVacia(cola)) {
        cola->frente = nuevoNodo;
        cola->final = nuevoNodo;
    } else {
        cola->final->siguiente = nuevoNodo;
        cola->final = nuevoNodo;
    }
}

Adoptante desencolar(Cola *cola) {
    if (estaVacia(cola)) {
        printf("La fila virtual está vacía\n");
        exit(1);
    }

    Nodo *temp = cola->frente;
    Adoptante adoptante = temp->adoptantito;
    cola->frente = cola->frente->siguiente;
    if (cola->frente == NULL) {
        cola->final = NULL;
    }
    free(temp);
    return adoptante;
}

void agregarMascota(Lista *lista, Mascota nuevaMascota) {
    Mascota *nuevoNodo = (Mascota *)malloc(sizeof(Mascota));
    *nuevoNodo = nuevaMascota;
    nuevoNodo->siguiente = lista->cabeza;
    lista->cabeza = nuevoNodo;
}

void mostrarLista(Lista *lista) {
    Mascota *actual = lista->cabeza;
    while (actual != NULL) {
        printf("Mascota ID: %d, Nombre: %s, Especie: %s, Raza: %s, Edad: %d, Tamaño: %s, Peso: %.2f, Descripción: %s\n",
                actual->id, actual->nombre, actual->especie, actual->raza, actual->edad, actual->tamano, actual->peso, actual->descripcion);
                actual = actual->siguiente;
    }
    printf("\n");
}

void lectorDeArchivosMascotas(Lista *listaMascotas, const char *nombreArchivo) {
    FILE *archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
        perror("Error al abrir el archivo de mascotas");
        return;
    }
    char lector[300];
    while (fgets(lector, 300, archivo) != NULL) {
        Mascota nuevaMascota;
        sscanf(lector, "%d %s %s %s %d %s %f %[^\n]", &nuevaMascota.id, nuevaMascota.nombre, nuevaMascota.especie, nuevaMascota.raza,
               &nuevaMascota.edad, nuevaMascota.tamano, &nuevaMascota.peso, nuevaMascota.descripcion);
        agregarMascota(listaMascotas, nuevaMascota);
    }
    fclose(archivo);
}

void mostrarMenu() {
    printf("A. Registro de adoptante.\n");
    printf("B. Atender a un adoptante.\n");
    printf("C. Cerrar el sistema\n");
    printf("D. Registro de mascotas\n");
}

void lectorDeArchivosAdoptantes(Cola *colaAdoptantes, const char *nombreArchivo) {
    FILE *archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
        perror("Error al abrir el archivo de adoptantes");
        return;
    }
    char lector[200];
    while (fgets(lector, 200, archivo) != NULL) {
        Adoptante nuevoAdoptante;
        sscanf(lector, "%d %s %s %s %s %s", &nuevoAdoptante.id, nuevoAdoptante.nombre, nuevoAdoptante.direccion,
               nuevoAdoptante.correo, nuevoAdoptante.telefono, nuevoAdoptante.contrasena);
        encolar(colaAdoptantes, nuevoAdoptante);
    }
    fclose(archivo);
}

int contadorRegistrosDiarios = 1;

void generarID(char fecha[], Adoptante adoptante, char id[]) {
    char nombreInicial = adoptante.nombre[0];
    char apellidoPaterno[50] = {0};
    char apellidoMaterno[50] = {0};
    char *token;
    char nombreCopia[50];

    strcpy(nombreCopia, adoptante.nombre);

    token = strtok(nombreCopia, " ");
    if (token != NULL) token = strtok(NULL, " ");
    if (token != NULL) strcpy(apellidoPaterno, token);
    token = strtok(NULL, " ");
    if (token != NULL) strcpy(apellidoMaterno, token);

    char ultimaLetraApellidoPaterno = apellidoPaterno[0];
    if (strlen(apellidoPaterno) > 0) {
        ultimaLetraApellidoPaterno = apellidoPaterno[strlen(apellidoPaterno) - 1];
    }

    char terceraLetraApellidoMaterno = apellidoMaterno[0];
    if (strlen(apellidoMaterno) >= 3) {
        terceraLetraApellidoMaterno = apellidoMaterno[2];
    } else if (strlen(apellidoMaterno) > 0) {
        terceraLetraApellidoMaterno = apellidoMaterno[strlen(apellidoMaterno) - 1];
    }

    sprintf(id, "%s%02d%c%c%c", fecha, contadorRegistrosDiarios, nombreInicial, ultimaLetraApellidoPaterno, terceraLetraApellidoMaterno);
    contadorRegistrosDiarios++;
}

void limpiarBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void menuA(Cola *colaAdoptantes, char id[]) {
    char op[3];
    char fecha[20];
    Adoptante nuevoAdoptante;

    do {
        system("clear");
        printf("Registro de adoptantes\n");
        printf("Anota la fecha de registro en el siguiente formato aammdd: ");
        limpiarBuffer();
        fgets(fecha, sizeof(fecha), stdin);
        fecha[strcspn(fecha, "\n")] = '\0';
        printf("Nombre y apellidos: ");
        fgets(nuevoAdoptante.nombre, sizeof(nuevoAdoptante.nombre), stdin);
        nuevoAdoptante.nombre[strcspn(nuevoAdoptante.nombre, "\n")] = '\0';
        printf("\nDirección: ");
        fgets(nuevoAdoptante.direccion, sizeof(nuevoAdoptante.direccion), stdin);
        nuevoAdoptante.direccion[strcspn(nuevoAdoptante.direccion, "\n")] = '\0';
        printf("\nCorreo: ");
        fgets(nuevoAdoptante.correo, sizeof(nuevoAdoptante.correo), stdin);
        nuevoAdoptante.correo[strcspn(nuevoAdoptante.correo, "\n")] = '\0';
        printf("\nTeléfono: ");
        fgets(nuevoAdoptante.telefono, sizeof(nuevoAdoptante.telefono), stdin);
        nuevoAdoptante.telefono[strcspn(nuevoAdoptante.telefono, "\n")] = '\0';
        printf("\nContraseña: ");
        fgets(nuevoAdoptante.contrasena, sizeof(nuevoAdoptante.contrasena), stdin);
        nuevoAdoptante.contrasena[strcspn(nuevoAdoptante.contrasena, "\n")] = '\0';
        printf("\n¿Son correctos los datos? (si/no): ");
        fgets(op, sizeof(op), stdin);
        op[strcspn(op, "\n")] = '\0';
    } while (strcmp(op, "si") != 0 && strcmp(op, "SI") != 0 && strcmp(op, "Si") != 0);

    generarID(fecha, nuevoAdoptante, id);
    printf("ID generado: %s\n", id);
    printf("Ese ID y la contraseña indicada se necesitará para hacer la adopción de la mascota.\n");
    printf("Escriba la palabra CONTINUAR para seguir con las acciones: ");
    char continuar[10];
    limpiarBuffer();
    do {
        fgets(continuar, sizeof(continuar), stdin);
        limpiarBuffer();
        
    } while (strcmp(continuar, "CONTINUAR") != 0);

    encolar(colaAdoptantes, nuevoAdoptante);
    system("clear");
}

void mostrarCola(Cola *colaAdoptantes) {
    Nodo *actual = colaAdoptantes->frente;
    while (actual != NULL) {
        printf("ID: %s, Nombre: %s\n", actual->adoptantito.id, actual->adoptantito.nombre);
        actual = actual->siguiente;
    }
    printf("\n");
}

void atencionAdoptante(Cola *colaAdoptantes, Lista *listaMascotas) {
    system("clear");
    printf("Atención de adoptante\n");
    if (estaVacia(colaAdoptantes)) {
        printf("No hay adoptantes en la cola.\n");
        return;
    }

    Adoptante atendido = colaAdoptantes->frente->adoptantito;
    char contrasena[20];
    int intentos = 0;
    while (intentos < 3) {
        printf("Ingrese su contraseña: ");
        fgets(contrasena, sizeof(contrasena), stdin);
        contrasena[strcspn(contrasena, "\n")] = '\0';

        if (strcmp(contrasena, atendido.contrasena) == 0) {
            int opcionAdoptante;
            do {
                system("clear");
                printf("MENÚ DE ADOPTANTE\n");
                printf("1. Mostrar mascotas de todas las especies\n");
                printf("2. Mostrar mascotas por especie\n");
                printf("3. Buscar una mascota en específico por nombre\n");
                printf("4. Buscar mascotas indicando especie y un intervalo de edad\n");
                printf("5. Salir\n");
                printf("Seleccione una opción: ");
                limpiarBuffer();
                scanf("%d", &opcionAdoptante);

                switch (opcionAdoptante) {
                    case 1:
                        mostrarMascotasTodas(listaMascotas);
                        break;
                    case 2:
                        mostrarMascotasPorEspecie(listaMascotas);
                        break;
                    case 3:
                        buscarMascotaPorNombre(listaMascotas);
                        break;
                    case 4:
                        buscarMascotasPorEspecieYEdad(listaMascotas);
                        break;
                    case 5:
                        printf("Saliendo...\n");
                        break;
                    default:
                        printf("Opción no válida. Intente de nuevo.\n");
                        break;
                }
            } while (opcionAdoptante != 5);
            return;
        } else {
            intentos++;
            printf("Contraseña incorrecta. Intentos restantes: %d\n", 3 - intentos);
        }
    }

    printf("Es necesario que el adoptante se registre de nuevo.\n");
    desencolar(colaAdoptantes);
}

void mostrarMascotasTodas(Lista *listaMascotas) {
    Mascota *actual = listaMascotas->cabeza;
    int index = 1;
    while (actual != NULL) {
        printf("Mascota %d\n", index);
        printf("Especie: %s\nNombre: %s\nEdad: %d\nTamaño: %s\nPeso: %.2f\nDescripción: %s\n\n",
               actual->especie, actual->nombre, actual->edad, actual->tamano, actual->peso, actual->descripcion);
        printf("Ingrese S para siguiente, A para anterior, M para adoptar, Q para salir: ");
        char opcion;
        scanf(" %c", &opcion);
        limpiarBuffer();

        switch (opcion) {
            case 'S':
            case 's':
                if (actual->siguiente != NULL) {
                    actual = actual->siguiente;
                    index++;
                } else {
                    printf("No hay registros posteriores. Ingrese S para continuar.\n");
                    char temp;
                    scanf(" %c", &temp);
                    limpiarBuffer();
                }
                break;
            case 'A':
            case 'a':
                printf("No hay registros previos. Ingrese A para continuar.\n");
                scanf(" %c", &opcion);
                limpiarBuffer();
                break;
            case 'M':
            case 'm':
                adoptarMascota(actual);
                return;
            case 'Q':
            case 'q':
                return;
            default:
                printf("Opción no válida. Intente de nuevo.\n");
                break;
        }
    }
}

void mostrarMascotasPorEspecie(Lista *listaMascotas) {
    Mascota *actual = listaMascotas->cabeza;
    char especies[100][20];
    int num_especies = 0;

    // Recolectar especies únicas
    while (actual != NULL) {
        int i;
        for (i = 0; i < num_especies; i++) {
            if (strcmp(especies[i], actual->especie) == 0) {
                break;
            }
        }
        if (i == num_especies) {
            strcpy(especies[num_especies], actual->especie);
            num_especies++;
        }
        actual = actual->siguiente;
    }

    // Mostrar especies numeradas
    printf("Especies disponibles:\n");
    for (int i = 0; i < num_especies; i++) {
        printf("%d. %s\n", i + 1, especies[i]);
    }

    // Pedir al usuario que seleccione una especie
    int opcion;
    printf("Seleccione el número de la especie que desea ver: ");
    scanf("%d", &opcion);
    limpiarBuffer();

    if (opcion < 1 || opcion > num_especies) {
        printf("Opción no válida.\n");
        return;
    }

    char especieSeleccionada[20];
    strcpy(especieSeleccionada, especies[opcion - 1]);
    system("clear");

    // Mostrar mascotas de la especie seleccionada
    actual = listaMascotas->cabeza;
    int index = 1;
    while (actual != NULL) {
        if (strcmp(actual->especie, especieSeleccionada) == 0) {
            printf("Mascota %d\n", index);
            printf("Especie: %s\nNombre: %s\nEdad: %d\nTamaño: %s\nPeso: %.2f\nDescripción: %s\n\n",
            actual->especie, actual->nombre, actual->edad, actual->tamano, actual->peso, actual->descripcion);
            printf("Ingrese S para siguiente, A para anterior, M para adoptar, Q para salir: ");
            char opcion;
            scanf(" %c", &opcion);
            limpiarBuffer();

            switch (opcion) {
                case 'S':
                case 's':
                    if (actual->siguiente != NULL) {
                        actual = actual->siguiente;
                        index++;
                    } else {
                        printf("No hay registros posteriores. Ingrese S para continuar.\n");
                        char temp;
                        scanf(" %c", &temp);
                        limpiarBuffer();
                    }
                    break;
                case 'A':
                case 'a':
                    printf("No hay registros previos. Ingrese A para continuar.\n");
                    scanf(" %c", &opcion);
                    limpiarBuffer();
                    break;
                case 'M':
                case 'm':
                    adoptarMascota(actual);
                    return;
                case 'Q':
                case 'q':
                    return;
                default:
                    printf("Opción no válida. Intente de nuevo.\n");
                    break;
            }
        }
        actual = actual->siguiente;
    }
}

void buscarMascotaPorNombre(Lista *listaMascotas) {
    char nombre[50];
    printf("Ingrese el nombre de la mascota que desea buscar: ");
    fgets(nombre, sizeof(nombre), stdin);
    nombre[strcspn(nombre, "\n")] = '\0';

    Mascota *actual = listaMascotas->cabeza;
    int encontrado = 0;

    while (actual != NULL) {
        if (strcmp(actual->nombre, nombre) == 0) {
            printf("Mascota encontrada:\n");
            printf("Especie: %s\nNombre: %s\nEdad: %d\nTamaño: %s\nPeso: %.2f\nDescripción: %s\n\n",
                   actual->especie, actual->nombre, actual->edad, actual->tamano, actual->peso, actual->descripcion);
            printf("Ingrese S para siguiente, A para anterior, M para adoptar, Q para salir: ");
            char opcion;
            scanf(" %c", &opcion);
            limpiarBuffer();

            switch (opcion) {
                case 'S':
                case 's':
                    while (actual != NULL && strcmp(actual->nombre, nombre) != 0) {
                        actual = actual->siguiente;
                    }
                    if (actual == NULL) {
                        printf("No hay más mascotas con ese nombre.\n");
                    }
                    break;
                case 'A':
                case 'a':
                    printf("No hay registros previos.\n");
                    break;
                case 'M':
                case 'm':
                    adoptarMascota(actual);
                    return;
                case 'Q':
                case 'q':
                    return;
                default:
                    printf("Opción no válida. Intente de nuevo.\n");
                    break;
            }
            encontrado = 1;
        }
        actual = actual->siguiente;
    }

    if (!encontrado) {
        printf("No se encontraron mascotas con el nombre '%s'.\n", nombre);
    }
}

void buscarMascotasPorEspecieYEdad(Lista *listaMascotas) {
    char especie[20];
    int edadMin, edadMax;
    
    printf("Ingrese la especie de la mascota: ");
    fgets(especie, sizeof(especie), stdin);
    especie[strcspn(especie, "\n")] = '\0';
    
    printf("Ingrese el intervalo de edad (mínimo y máximo) separados por un espacio: ");
    scanf("%d %d", &edadMin, &edadMax);
    limpiarBuffer();

    Mascota *actual = listaMascotas->cabeza;
    int encontrado = 0;

    while (actual != NULL) {
        if (strcmp(actual->especie, especie) == 0 && actual->edad >= edadMin && actual->edad <= edadMax) {
            printf("Mascota encontrada:\n");
            printf("Especie: %s\nNombre: %s\nEdad: %d\nTamaño: %s\nPeso: %.2f\nDescripción: %s\n\n",
                   actual->especie, actual->nombre, actual->edad, actual->tamano, actual->peso, actual->descripcion);
            printf("Ingrese S para siguiente, A para anterior, M para adoptar, Q para salir: ");
            char opcion;
            limpiarBuffer();
            scanf(" %c", &opcion);

            switch (opcion) {
                case 'S':
                case 's':
                    while (actual != NULL && (strcmp(actual->especie, especie) != 0 || actual->edad < edadMin || actual->edad > edadMax)) {
                        actual = actual->siguiente;
                    }
                    if (actual == NULL) {
                        printf("No hay más mascotas con esos criterios.\n");
                    }
                    break;
                case 'A':
                case 'a':
                    printf("No hay registros previos.\n");
                    break;
                case 'M':
                case 'm':
                    adoptarMascota(actual);
                    return;
                case 'Q':
                case 'q':
                    return;
                default:
                    printf("Opción no válida. Intente de nuevo.\n");
                    break;
            }
            encontrado = 1;
        }
        actual = actual->siguiente;
    }

    if (!encontrado) {
        printf("No se encontraron mascotas de la especie '%s' en el intervalo de edad %d-%d.\n", especie, edadMin, edadMax);
    }
}

void adoptarMascota(Mascota *mascota) {
    printf("Datos de la mascota seleccionada:\n");
    printf("Especie: %s\nNombre: %s\nEdad: %d\nTamaño: %s\nPeso: %.2f\nDescripción: %s\n",
    mascota->especie, mascota->nombre, mascota->edad, mascota->tamano, mascota->peso, mascota->descripcion);
    printf("Escriba CONFIRMAR para adoptar la mascota o CANCELAR para regresar: ");
    char opcion[10];
    fgets(opcion, sizeof(opcion), stdin);
    limpiarBuffer();

    if (strcmp(opcion, "CONFIRMAR") == 0) {
        printf("¡Felicitaciones por la adopción! Pase al área de resguardo de mascotas.\n");
        sleep(3);
    } else {
        printf("Está bien, puede seguir revisando la lista de mascotas.\n");
    }
}

int main() {
    setlocale(LC_ALL, "");
    Cola colaAdoptantes;
    Lista listaMascotas;
    inicializarCola(&colaAdoptantes);
    inicializarLista(&listaMascotas);

    lectorDeArchivosAdoptantes(&colaAdoptantes, "adoptantes.txt");
    lectorDeArchivosMascotas(&listaMascotas, "mascotas.txt");

    system("clear");
    char opcion[2];

    do {
        mostrarMenu();
        mostrarCola(&colaAdoptantes);
        printf("Seleccione una opción: ");
        fgets(opcion, sizeof(opcion), stdin);
        opcion[strcspn(opcion, "\n")] = '\0';

        switch (opcion[0]) {
            case 'A':
            case 'a':
                menuA(&colaAdoptantes, opcion);
                break;
            case 'B':
            case 'b':
                atencionAdoptante(&colaAdoptantes, &listaMascotas);
                break;
            case 'C':
            case 'c':
                system("clear");
                printf("Cerrando el sistema... \n");
                break;
            case 'D':
            case 'd':
                system("clear");
                printf("Registro de mascotas desde el archivo: \n");
                mostrarLista(&listaMascotas);
                break;
            default:
                system("clear");
                printf("Opción no válida, por favor intente nuevamente.\n");
        }
    } while (opcion[0] != 'C' && opcion[0] != 'c');

    system("clear");
    return 0;
}
