#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include "API2024Parte2.h"

#define MAX_U32 4294967295 //(2^32)-1

void ColorGroupInit (ColorGroup CG, Grafo G, color c) {
    // Asumimos que, para los grafos con los que se va a trabajar, 
    // cada grupo de colores no tendra más de (4N)/10 vértices más una cierta tolerancia 
    CG->Vertices = malloc((((NumeroDeVertices(G) * 4) / 10) + TOLERANCIA) * sizeof(u32));
    if (!CG->Vertices) {
        fprintf(stderr, "ERROR: Fallo al asignar memoria.\n");
        fprintf(stderr, "Cerrando el programa ...\n");
        exit(1);
    }
    CG->Longitud = 0;
    CG->Color = c;
} 

void ColorGroupDestroy (ColorGroup CG) {
    free(CG->Vertices);
    CG->Vertices = NULL;
    free(CG);
}

// Función auxiliar para verificar si un arreglo es biyectivo. Costo: O(N)
static bool verificarBiyectividad(u32* Orden, u32 n) {
    bool *vistos = calloc(n, sizeof(bool));
    if (!vistos) {
        return false; // Error: No se pudo asignar memoria
    }

    for (u32 i = 0; i < n; ++i) {
        if (Orden[i] >= n || vistos[Orden[i]]) {
            free(vistos);
            return false; // Orden no es biyectiva
        }
        vistos[Orden[i]] = true;
    }

    free(vistos);
    return true;
}

static void mergeCG(ColorGroup arr[], int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    // Crear arreglos temporales de punteros a struct ColorGroupSt
    ColorGroup *L = (ColorGroup *)malloc(n1 * sizeof(ColorGroup));
    ColorGroup *R = (ColorGroup *)malloc(n2 * sizeof(ColorGroup));
    if (L == NULL || R == NULL) {
        fprintf(stderr, "Error: No se pudo asignar memoria para los arreglos temporales\n");
        exit(EXIT_FAILURE);
    }

    // Copiar datos a los arreglos temporales L[] y R[]
    for (int i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    // Combinar los arreglos temporales de vuelta en arr[l..r]
    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i]->Longitud <= R[j]->Longitud) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }        

    // Copiar los elementos restantes de L[], si hay alguno
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    // Copiar los elementos restantes de R[], si hay alguno
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }

    // Liberar memoria
    free(L);
    free(R);
}

// Merge Sort para Grupo de Colores. Los ordena en orden asendente según la frecuencia del color
static void mergeSortCG(ColorGroup arr[], int l, int r) {
    if (l < r) {
        // Encuentra el punto medio
        int m = l + (r - l) / 2;

        // Ordena las primeras y segundas mitades
        mergeSortCG(arr, l, m);
        mergeSortCG(arr, m + 1, r);

        // Combina las mitades ordenadas
        mergeCG(arr, l, m, r);
    }
}

static void mergeColDesc(color arr[], int l, int m, int r, u32 Valores[]) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    // Crear arreglos temporales
    u32 L[n1], R[n2];

    // Copiar datos a los arreglos temporales L[] y R[]
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    // Mezclar los arreglos temporales de nuevo al arreglo original arr[]
    i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (Valores[L[i] - 1] >= Valores[R[j] - 1]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // Copiar los elementos restantes de L[], si hay alguno
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    // Copiar los elementos restantes de R[], si hay alguno
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

// Merge Sort para colores. Ordena arr en orden descendente según Valores
static void mergeSortColDesc(color arr[], int l, int r, u32 Valores[]) {
    if (l < r) {
        // Encuentra el punto medio del arreglo
        int m = l + (r - l) / 2;

        // Ordena la primera mitad
        mergeSortColDesc(arr, l, m, Valores);

        // Ordena la segunda mitad
        mergeSortColDesc(arr, m + 1, r, Valores);

        // Mezcla las dos mitades ordenadas
        mergeColDesc(arr, l, m, r, Valores);
    }
}

// Función auxiliar para obtener el mayor color de un grafo. Cuesta O(N)
static color getMaxColor(Grafo G) {
    u32 num_ver = NumeroDeVertices(G);
    color Max_color = 0;
    for (u32 i = 0; i < num_ver; ++i) {
        color c = Color(i, G);
        Max_color = (c > Max_color) ? c : Max_color;
    }
    return Max_color;
}

/* La idea es la siguiente: Primero corroboramos que Orden sea efectivamente una biyección, esto nos cuesta O(N). Antes 
de entrar al bucle principal del algoritmo, las dos llamadas a calloc nos cuestan O(Grado(G)) + O(N) respectivamente.
Una vez dentro del bucle, para cada vértice primero obtenemos el arreglo con los colores usados por sus vecinos que es 
O(Grado(v, G)), luego decidimos el color que va a tener v que es O(Grado(v, G)) y reseteamos a 0 el arreglo de colores usados
para el siguiente vértice lo cual también nos cuesta O(Grado(v, G)). TOTAL: O(M) por el lema del apretón de manos.
Por lo tanto la complejidad computacional de Greedy está dada por: O(N) +  O(M) = O(M) */

u32 Greedy(Grafo G, u32* Orden) {
    u32 num_vert = NumeroDeVertices(G);

    // Verificar que Orden provea un orden válido de los elementos {0, 1, ..., n − 1}
    if (!verificarBiyectividad(Orden, num_vert)) {
        return MAX_U32; // Error: Orden no es biyectiva
    }

    // Colorear los vértices siguiendo el orden dado en el arreglo Orden
    u32 v; // Indice del vértice a colorear
    u32 num_vec; // Cantidad de vecinos de v
    color Max_color = 0; // Color máximo utilizado
    bool *colores_usados = calloc(Delta(G) + 2, sizeof(bool)); // Arreglo con los colores usados de los vecinos de v
    color *Colores = calloc(num_vert, sizeof(color)); // Arreglo para guardar los colores del grafo
    if (!colores_usados || !Colores) {
        return MAX_U32; // No se pudo asignar memoria
    }

    for (u32 i = 0; i < num_vert; ++i) {
        v = Orden[i];
        num_vec = Grado (v, G);

        // Seteamos en false
        for (u32 l = 0; l < num_vec + 2; ++l) {
            colores_usados[l] = false;
        }

        // Rellenamos colores_usados
        for (u32 j = 0; j < num_vec; ++j) {  
            u32 w = Vecino(j, v, G); // Variable para los vecinos de v
            if (Colores[w] != 0) { // Si el color del vecino w es 0, no lo contamos
                colores_usados[Colores[w] - 1] = true;
            }
        }

        // Obtenemos el color de v y actualizamos el color máximo
        u32 k = 0;
        while (colores_usados[k]) {
            ++k;
        }
        Colores[v] = k + 1;
        Max_color = (Colores[v] > Max_color) ? Colores[v] : Max_color; // Actualizo el color máximo
    }
    ImportarColores(Colores, G);
    free(colores_usados);
    free(Colores);
    return Max_color;
}

// Función para ordenar los vértices según los criterios especificados
// Como la complejidad de la función mergeSort en este caso es  menor a O(#cantidad_colores log #cantidad_colores) lo
// cual es más chico que O(N), el algoritmo tiene complejidad O(N) para los fines prácticos de este laboratorio.
char GulDukat(Grafo G, u32* Orden) {
    u32 num_ver = NumeroDeVertices(G);

    // Definir, inicializar y calcular las funciones m(x) y M(x)
    // También creamos un arreglo para guardar la suma de M(x) + m(x)
    // Finalmente, ordenamos los arreglos en orden descendente
    u32 Max_color = getMaxColor(G);
    u32 m[Max_color];
    u32 M[Max_color];
    u32 D = Delta(G);
    for (u32 x = 0; x < Max_color; ++x) {
        m[x] = D; // Inicializar con un valor grande
        M[x] = 0; // Inicializar con un valor pequeño
    }
    color c;
    u32 grado;
    for (u32 i = 0; i < num_ver; ++i) {
        c = Color(i, G) - 1;
        grado = Grado(i, G);
        if (grado < m[c]) {
            m[c] = grado;
        }
        if (grado > M[c]) {
            M[c] = grado;
        }
    }

    //Generamos arreglos para ordenar los colores con Merge Sort en base a las condiciones pedidas
    u32 tamDiv4 = Max_color/4;
    color Divisibles4[tamDiv4];
    for (color i = 1; i <= tamDiv4; ++i) {
        Divisibles4[i - 1] = i * 4;
    }
    mergeSortColDesc(Divisibles4, 0, tamDiv4 - 1, M);
    u32 tamPar = (Max_color % 4 <= 1) ? tamDiv4 : tamDiv4 + 1; 
    color Pares[tamPar];
    u32 Mmasm[Max_color]; // Arreglo para guardar M(x) + m(x)
    for (u32 i = 0; i < Max_color; ++i) {
        Mmasm[i] = M[i] + m[i];
    }
    for (color i = 0; i < tamPar; ++i) {
        Pares[i] = i * 4 + 2;
    }
    mergeSortColDesc(Pares, 0, tamPar - 1, Mmasm);
    u32 tamImpar = (Max_color % 2 == 0) ? Max_color/2 : Max_color/2 + 1;
    color Impares[tamImpar];
    for (color i = 1; i <= tamImpar; ++i) {
        Impares[i - 1] = 2 * i - 1;
    }
    mergeSortColDesc(Impares, 0, tamImpar - 1, m);

    // Inicializamos los grupos de colores
    ColorGroup *GrupoColores = malloc(Max_color * sizeof(struct ColorGroupSt));
    if (!GrupoColores) {
        return '1'; // Error: No se pudo asignar memoria
    }
    for (u32 i = 0; i < Max_color; ++i) {
        GrupoColores[i] = malloc(sizeof(struct ColorGroupSt));
        if (!GrupoColores[i]) {
            return '1'; // Error: No se pudo asignar memoria
        }
        ColorGroupInit(GrupoColores[i], G, (i + 1));
    }
    for (u32 i = 0; i < num_ver; ++i) {
        c = Color(i, G) - 1;
        GrupoColores[c]->Vertices[GrupoColores[c]->Longitud] = i;
        ++GrupoColores[c]->Longitud;
    }

    // La comlejidad computacional de todos los bucles siguientes es O(N) puesto que en cada uno reviso todos los vértices 1 vez
    // Agregamos los vértices que tengan colores que sean divisibles por 4, ordenados entre si de acuerdo con M(x)
    u32 vertices_agregados = 0;
    for (u32 i = 0; i < tamDiv4; ++i) {
        for (u32 j = 0; j < GrupoColores[Divisibles4[i] - 1]->Longitud; ++j) {
            Orden[vertices_agregados] = GrupoColores[Divisibles4[i] - 1]->Vertices[j];
            ++vertices_agregados;    
        }
    }
    // Agregamos los vértices que tengan colores que sean divisibles por 2 y no po 4, ordenados entre si de acuerdo con M(x) + m(x)
    for (u32 i = 0; i < tamPar; ++i) {
        for (u32 j = 0; j < GrupoColores[Pares[i] - 1]->Longitud; ++j) {
            Orden[vertices_agregados] = GrupoColores[Pares[i] - 1]->Vertices[j];
            ++vertices_agregados;    
        }
    }
    // Agregamos los vértices que tengan colores impares, ordenados entre si de acuerdo con m(x)
    for (u32 i = 0; i < tamImpar; ++i) {
        for (u32 j = 0; j < GrupoColores[Impares[i] - 1]->Longitud; ++j) {
            Orden[vertices_agregados] = GrupoColores[Impares[i] - 1]->Vertices[j];
            ++vertices_agregados;    
        }
    }

    // Liberamos memoria y retornamos
    for (u32 i = 0; i < Max_color; ++i) {
        ColorGroupDestroy(GrupoColores[i]);
    }
    free(GrupoColores);
    return '0';
}

// Función para ordenar los vértices según los criterios especificados
// Como la complejidad de la función mergeSort en este caso es  menor a O(#cantidad_colores log #cantidad_colores) lo
// cual rápidamente es más chico que O(N) para grafos grandes, el algoritmo tiene complejidad O(N) para los fines 
// prácticos de este laboratorio.
char ElimGarak(Grafo G, u32* Orden) {
    u32 num_ver = NumeroDeVertices(G);
    color *Colores = malloc(num_ver * sizeof(color));
    if (!Colores) {
        return '1'; // Error: No se pudo asignar memoria
    }
    ExtraerColores(G, Colores);
    u32 Max_color = getMaxColor(G);
    ColorGroup *GrupoColores = malloc(Max_color * sizeof(struct ColorGroupSt));
    if (!GrupoColores) {
        return '1'; // Error: No se pudo asignar memoria
    }
    
    // Inicializamos los grupos de colores
    for (u32 i = 0; i < Max_color; ++i) {
        GrupoColores[i] = malloc(sizeof(struct ColorGroupSt));
        if (!GrupoColores[i]) {
            return '1'; // Error: No se pudo asignar memoria
        }
        ColorGroupInit(GrupoColores[i], G, (i + 1));
    }

    // Colocamos los vértices de colores 1 y 2 en sus respectivos lugares
    // Para el resto de vértices, los cargamos en sus respectivos grupos de colores
    u32 index1 = num_ver - 1; // Indice para los vértices de color 1
    u32 index2 = num_ver - 2; // Indice para los vértices de color 2
    for (u32 v = 0; v < num_ver; ++v) {
        if (Colores[v] == 1) {
            // Acomodo el arreglo adecuadamente si hay un vértice de color 2 donde debería colocar el siguiente vértice de color 1
            // También chequeo que efectivamente halla puesto un vértice en esa posición y que no sea el primer vértice de color 1
            if (Colores[Orden[index1]] == 2 && index1 != index2 && index1 != num_ver - 1) {
                u32 temp = Orden[index1];
                Orden[index1] = v;
                Orden[index2] = temp;
                --index2;
            }
            else{
                Orden[index1] = v;
            }
            --index1;
        }
        else if (Colores[v] == 2) {
            Orden[index2] = v;
            --index2;
        }
        // En cualquier otro caso, actualizamos GrupoColores y el color máximo
        else {
            GrupoColores[Colores[v] - 1]->Vertices[GrupoColores[Colores[v] - 1]->Longitud] = v;
            ++GrupoColores[Colores[v] - 1]->Longitud;
        }
        if (index1 == index2 - 1) { // Acomodo si index1 se adelantó
            --index2;
        }
    }
    
    // Ordenamos los Grupos según la frecuencia de color utilziando MergeSort empezando desde el color 3
    if (Max_color > 2) {
        mergeSortCG(GrupoColores, 2, Max_color - 1);
    }
    // Si no hay más colores termina acá
    else {
        for (u32 i = 0; i < Max_color; ++i) {
            ColorGroupDestroy(GrupoColores[i]);
        }
        free(GrupoColores);
        return '0'; // No hay vértices de colores mayores a 2
    }

    // Agregamos el resto de colores a Orden
    // Notar que es O(N) puesto que recorro una vez todos los vértices excepto los que tienen color 1 y 2
    for (u32 i = Max_color - 1; i > 1; --i) {
        for (u32 j = 0; j < GrupoColores[i]->Longitud; ++j) {
            Orden[index2] = GrupoColores[i]->Vertices[j];
            --index2;
        }   
    }

    // Liberamos memoria y retornamos
    free(Colores);
    for (u32 i = 0; i < Max_color; ++i) {
            ColorGroupDestroy(GrupoColores[i]);
        }
    free(GrupoColores);
    return '0';
}
