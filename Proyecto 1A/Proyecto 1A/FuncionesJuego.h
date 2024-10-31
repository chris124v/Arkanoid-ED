//Funciones del juego implementadas posteriormente 

#pragma once
#define NOMINMAX

#include <stdio.h>
#include <iostream>
#include <math.h>
#include <windows.h>
#include <stdlib.h>
#include <cstdlib>
#include <time.h>
#include <algorithm> 

#undef min
#undef max

#include <allegro5/allegro.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include "Juego.h"
using namespace std;
#pragma warning(disable:4996);  //Se desactiva alarma en el manejo de archivos

const int ResX = 800;
const int ResY = 1100;
const int diametro = 32;
const int NAVE_ANCHO = 100;  // Ancho deseado de la nave
const int NAVE_ALTO = 35;

typedef struct bola {

    int x;         // Posición en el eje X
    int y;         // Posición en el eje Y
    int radio;     // Radio de la bola
    float velocidadX; // Velocidad en el eje X
    float velocidadY; // Velocidad en el eje Y
    int codigo;
    bool estado;
    bola* Siguiente;

}*Ptrbola;

typedef struct nave {
    float x;         // Posición en el eje X
    float y;         // Posición en el eje Y
    int velocidadY;
    bool estado;
};

typedef struct bloque {
    float x;         // Posición en el eje X
    float y;         // Posición en el eje Y
    bool estado;     // Estado del bloque (activo/inactivo)
    int codigo;      // Código del bloque
    int resistencia; // Nivel de resistencia del bloque
    bloque* Siguiente; // Puntero al siguiente bloque en la lista
}*Ptrbloque;

typedef struct enemigo {

    float x;         // Posición en el eje X
    float y;         // Posición en el eje Y
    float velocidadX; // Velocidad en el eje X
    float velocidadY; // Velocidad en el eje Y
    int codigo;
    int tipo;
    bool estado;
    enemigo* Siguiente;

}*Ptrenemigo;

void Inicializar_nave(nave& Jugador, int x) {

    Jugador.x = ResX / 2 - 40;
    Jugador.y = ResY - 300;
    Jugador.velocidadY = 5;
    Jugador.estado = true; // Activa la nave para que se muestre

};

void inicializar_bloque(Ptrbloque& bloques, int i, int x) {

    bloques->y = ((x / 11) + 1) * diametro / 2 * 4;
    bloques->estado = true;
    bloques->codigo = i;
    bloques->resistencia = rand() % 5 + 1; // Asignar un nivel de resistencia aleatorio entre 1 y 5
    bloques->Siguiente = NULL;
}

void inicializar_enemigo(Ptrenemigo& enemigo, int i, int t, int x) {

    enemigo->x = x / 2 - ResX / 2 + (i % 11) * ((ResX - 200) / 10) + 100;
    enemigo->y = ((i / 11) + 1) * diametro / 2 * 4;
    enemigo->velocidadX = 5;
    enemigo->velocidadY = 5;
    enemigo->codigo = i;
    enemigo->tipo = t;
    enemigo->estado = true;
    enemigo->Siguiente = NULL;

}

void dibujar_nave(nave& jugador, ALLEGRO_BITMAP* nave_bitmap, int centroX, int centroY) {

    if (jugador.estado) {  // Verifica si la nave está activa
        // Obtener dimensiones originales del bitmap
        int bitmap_ancho = al_get_bitmap_width(nave_bitmap);
        int bitmap_alto = al_get_bitmap_height(nave_bitmap);

        // Dibujar el bitmap escalado en la posición de la nave
        al_draw_scaled_bitmap(nave_bitmap,
            0, 0,                          // Coordenadas fuente X,Y
            bitmap_ancho, bitmap_alto,     // Ancho y alto fuente
            jugador.x + centroX,           // Posición destino X
            jugador.y + centroY,           // Posición destino Y
            NAVE_ANCHO, NAVE_ALTO,         // Ancho y alto destino
            0                              // Flags
        );
    }
}

void formacion_bloques(Ptrbloque& bloques, int nivel, ALLEGRO_BITMAP* bloques2) {
    // Factor de escala y dimensiones escaladas del bloque
    const float escalaFactor = 0.5;
    const int anchoBloque = al_get_bitmap_width(bloques2) * escalaFactor;
    const int altoBloque = al_get_bitmap_height(bloques2) * escalaFactor;

    // Define el espacio entre los bloques
    const int espacioHorizontal = 0; // Ajusta según necesidad
    const int espacioVertical = 0;   // Ajusta según necesidad

    // Número de bloques por fila
    const int numBloquesPorFila = 7;

    // Posición inicial de los bloques para centrarlos en el área de juego
    int xInicial = (ResX - (numBloquesPorFila * anchoBloque + (numBloquesPorFila - 1) * espacioHorizontal)) / 2;
    int yInicial = 50;

    for (int i = 0; i < numBloquesPorFila * 5; i++) {
        Ptrbloque nuevoBloque = new bloque;
        nuevoBloque->x = xInicial + (i % numBloquesPorFila) * (anchoBloque + espacioHorizontal);
        nuevoBloque->y = yInicial + (i / numBloquesPorFila) * (altoBloque + espacioVertical);

        nuevoBloque->estado = true;
        nuevoBloque->resistencia = rand() % 5 + 1; // Resistencia aleatoria entre 1 y 5
        nuevoBloque->Siguiente = NULL;

        // Condiciones para las diferentes formaciones
        if (nivel == 1) {
            // Nivel 1: Filas completas de bloques
        }
        else if (nivel == 2) {
            // Nivel 2: Zigzag
            if (i % 2 == 0) nuevoBloque->y += diametro / 2;
        }
        else if (nivel == 3) {
            // Nivel 3: Pirámide
            if (i < 3 || (i >= 8 && i < 11) || (i >= 18 && i < 21)) nuevoBloque->estado = false;
        }
        else if (nivel == 4) {
            // Nivel 4: Diamante
            if (i < 2 || i > 11 && i < 14 || i > 27) nuevoBloque->estado = false;
        }
        else if (nivel == 5) {
            // Nivel 5: X en el centro
            if (i % 3 != 0) nuevoBloque->estado = false;
        }
        else if (nivel == 6) {
            // Nivel 6: Dos filas de bloques dobles en el centro
            if (i / numBloquesPorFila < 2 || i / numBloquesPorFila > 3) nuevoBloque->estado = false;
        }
        else if (nivel == 7) {
            // Nivel 7: Checkerboard
            if ((i / numBloquesPorFila) % 2 == 0) {
                if (i % 2 == 1) nuevoBloque->estado = false;
            }
            else {
                if (i % 2 == 0) nuevoBloque->estado = false;
            }
        }
        else if (nivel == 8) {
            // Nivel 8: Columna central
            if (i % numBloquesPorFila != numBloquesPorFila / 2) nuevoBloque->estado = false;
        }
        else if (nivel == 9) {
            // Nivel 9: Filas alternadas (una sí, una no)
            if (i / numBloquesPorFila % 2 == 1) nuevoBloque->estado = false;
        }
        else if (nivel == 10) {
            // Nivel 10: Bloques en las esquinas
            if (i < numBloquesPorFila || i >= numBloquesPorFila * 4) nuevoBloque->estado = true;
            else nuevoBloque->estado = false;
        }
        else if (nivel == 11) {
            // Nivel 11: Hileras diagonales
            if ((i % numBloquesPorFila + i / numBloquesPorFila) % 2 != 0) nuevoBloque->estado = false;
        }
        else if (nivel == 12) {
            // Nivel 12: Bloques en forma de "U"
            if (i / numBloquesPorFila == 0 || i / numBloquesPorFila == 4 || (i % numBloquesPorFila == 0 || i % numBloquesPorFila == numBloquesPorFila - 1)) {
                nuevoBloque->estado = true;
            }
            else {
                nuevoBloque->estado = false;
            }
        }
        else if (nivel == 13) {
            // Nivel 13: Esquinas y centro
            if ((i % numBloquesPorFila == 0 || i % numBloquesPorFila == numBloquesPorFila - 1) && (i / numBloquesPorFila == 0 || i / numBloquesPorFila == 4)) {
                nuevoBloque->estado = true;
            }
            else {
                nuevoBloque->estado = false;
            }
        }
        else if (nivel == 14) {
            // Nivel 14: Flecha hacia abajo
            if (i < 3 || (i >= 8 && i < 10) || i == 16) nuevoBloque->estado = false;
        }
        else if (nivel == 15) {
            // Nivel 15: Espiral hacia adentro
            if (i % 4 == 0 || i % 5 == 0) nuevoBloque->estado = false;
        }
        else if (nivel == 16) {
            // Nivel 16: Triángulo inverso
            if (i < 2 || (i >= 4 && i < 7) || (i >= 11 && i < 16)) nuevoBloque->estado = true;
            else nuevoBloque->estado = false;
        }
        else if (nivel == 17) {
            // Nivel 17: Zigzag denso
            if (i % 3 != 1) nuevoBloque->estado = true;
            else nuevoBloque->estado = false;
        }
        else if (nivel == 18) {
            // Nivel 18: Tres columnas
            if (i % numBloquesPorFila == 2 || i % numBloquesPorFila == 4) nuevoBloque->estado = true;
            else nuevoBloque->estado = false;
        }
        else if (nivel == 19) {
            // Nivel 19: Hileras de dos en dos
            if ((i / numBloquesPorFila) % 2 != 0) nuevoBloque->estado = false;
        }
        else if (nivel == 20) {
            // Nivel 20: Marco de bloques
            if (i / numBloquesPorFila == 0 || i / numBloquesPorFila == 4 || i % numBloquesPorFila == 0 || i % numBloquesPorFila == numBloquesPorFila - 1) {
                nuevoBloque->estado = true;
            }
            else {
                nuevoBloque->estado = false;
            }
        }

        // Añadir el nuevo bloque a la lista
        if (!bloques) {
            bloques = nuevoBloque;
        }
        else {
            Ptrbloque temp = bloques;
            while (temp->Siguiente) temp = temp->Siguiente;
            temp->Siguiente = nuevoBloque;
        }
    }
}

void inicializar_bola(Ptrbola& bola, nave& jugador, int radio) {

    bola = new struct bola; // Asignar memoria para la nueva bola
    bola->x = jugador.x; // Posición inicial en la nave
    bola->y = jugador.y - 10; // Colocarla justo encima de la nave
    bola->radio = 7; // Establecer el radio de la bola
    bola->velocidadX = (rand() % 3 + 1) * (rand() % 2 == 0 ? -1 : 1); // Velocidad aleatoria en X; // Velocidad inicial en X
    bola->velocidadY = (rand() % 3 + 1) * (rand() % 2 == 0 ? -1 : 1);; // Velocidad inicial en Y (hacia arriba)
    bola->codigo = 0; // Puedes asignar un código o cualquier otro valor que necesites
    bola->estado = true; // La bola está activa
    bola->Siguiente = NULL; // Inicializar el puntero siguiente
}

void colision_bola(Ptrbola& balin, Ptrbloque& bloques, nave& jugador, Ptrenemigo& enemigos, int& vidas, int centroX, int centroY, ALLEGRO_BITMAP* bloques2) {

    // Definir el tamaño escalado de los bloques
    const int anchoBloqueEscalado = al_get_bitmap_width(bloques2) / 2 - 5;
    const int altoBloqueEscalado = al_get_bitmap_height(bloques2) / 2 - 5;

    // Radio de la bola
    const int radioBola = balin->radio;

    // Limitar el movimiento a los bordes del área centrada
    if (balin->x - radioBola <= centroX) {
        balin->x = centroX + radioBola; // Ajustar la posición para no salir
        balin->velocidadX = -balin->velocidadX; // Cambiar dirección
    }
    if (balin->x + radioBola >= centroX + ResX) {
        balin->x = centroX + ResX - radioBola; // Ajustar la posición para no salir
        balin->velocidadX = -balin->velocidadX; // Cambiar dirección
    }
    if (balin->y - radioBola <= centroY) {
        balin->y = centroY + radioBola; // Ajustar la posición para no salir
        balin->velocidadY = -balin->velocidadY; // Cambiar dirección
    }
    if (balin->y + radioBola >= centroY + ResY) {
        vidas--;  // Resta una vida
        balin->x = jugador.x + centroX;  // Reposiciona en la zona centrada
        balin->y = jugador.y + centroY - 10; // Reposicionar justo encima de la nave
        balin->velocidadY = -fabs(balin->velocidadY); // Invertir dirección
    }

    // Verificar colisión con bloques
    Ptrbloque bloqueActual = bloques;
    while (bloqueActual != NULL) {
        if (bloqueActual->estado) {
            // Verifica colisión considerando el radio de la bola
            if (balin->x + radioBola >= bloqueActual->x + centroX &&
                balin->x - radioBola <= bloqueActual->x + centroX + anchoBloqueEscalado &&
                balin->y + radioBola >= bloqueActual->y + centroY &&
                balin->y - radioBola <= bloqueActual->y + centroY + altoBloqueEscalado) {

                // Colisión detectada, manejar interacción
                bloqueActual->resistencia--;
                if (bloqueActual->resistencia <= 0) {
                    bloqueActual->estado = false; // Desactivar bloque si resistencia llega a 0
                }

                // Rebote de la bola al chocar con el bloque
                float deltaX = (balin->x + radioBola) - (bloqueActual->x + centroX);
                float deltaY = (balin->y + radioBola) - (bloqueActual->y + centroY);

                if (fabs(deltaX) < fabs(deltaY)) {
                    balin->velocidadY = -balin->velocidadY; // Cambiar dirección vertical
                    balin->y += (balin->velocidadY > 0) ? -1 : 1; // Ajustar posición para evitar el "pegado"
                }
                else {
                    balin->velocidadX = -balin->velocidadX; // Cambiar dirección horizontal
                    balin->x += (balin->velocidadX > 0) ? -1 : 1; // Ajustar posición para evitar el "pegado"
                }
                break; // Termina la comprobación una vez que se detecta una colisión
            }
        }
        bloqueActual = bloqueActual->Siguiente; // Avanzar al siguiente bloque
    }

    // Actualizar posición de la bola
    balin->x += balin->velocidadX;
    balin->y += balin->velocidadY;
}





void generar_enemigos(Ptrenemigo& enemigos, int cantidad, int nivel) {
    // Limitar la cantidad de enemigos generados a un máximo de 4 o 5
    // Máximo 5 enemigos por nivel

    for (int i = 0; i < cantidad; ++i) {

        Ptrenemigo nuevoEnemigo = new enemigo;
        int tipo = rand() % 3; // Selección aleatoria del tipo de enemigo (0 = Adherido, 1 = Rebote, 2 = Lineal)
        inicializar_enemigo(nuevoEnemigo, i, tipo, ResX);

        // Posición inicial aleatoria en la parte superior de la pantalla
        nuevoEnemigo->x = (ResX / 1.2) + (rand() % (ResX / 2 - diametro)); // Spawn en la mitad derecha // X entre 0 y el ancho de pantalla menos el diámetro
        nuevoEnemigo->y = rand() % (ResY / 2);        // Y entre 0 y la mitad de la pantalla
        nuevoEnemigo->estado = true;                  // Enemigo inicia activo
        nuevoEnemigo->Siguiente = nullptr;

        // Configurar el movimiento basado en el tipo de enemigo
        switch (tipo) {
        case 0: // Adherido
            nuevoEnemigo->velocidadX = 2; // Velocidad baja en X
            nuevoEnemigo->velocidadY = 2; // Velocidad baja en Y
            break;

        case 1: // Rebote
            nuevoEnemigo->velocidadX = (rand() % 3 + 1) * (rand() % 2 == 0 ? -1 : 1); // Velocidad aleatoria en X
            nuevoEnemigo->velocidadY = (rand() % 3 + 1) * (rand() % 2 == 0 ? -1 : 1); // Velocidad aleatoria en Y
            break;

        case 2: // Lineal (Arriba-Abajo-Izquierda-Derecha)
            nuevoEnemigo->velocidadX = (rand() % 2 == 0 ? 2 : 0); // Solo en X o solo en Y
            nuevoEnemigo->velocidadY = (nuevoEnemigo->velocidadX == 0 ? 2 : 0);
            break;
        }

        // Añadir el nuevo enemigo a la lista de enemigos
        if (!enemigos) {
            enemigos = nuevoEnemigo;
        }
        else {
            Ptrenemigo temp = enemigos;
            while (temp->Siguiente) temp = temp->Siguiente;
            temp->Siguiente = nuevoEnemigo;
        }
    }
}

void mover_enemigos(Ptrenemigo& enemigos, Ptrbloque& bloques, int centroX, int centroY, ALLEGRO_BITMAP* bloques2) {
   
    // Definir el tamaño escalado de los bloques
    const int anchoBloqueEscalado = al_get_bitmap_width(bloques2) / 2 - 5;
    const int altoBloqueEscalado = al_get_bitmap_height(bloques2) / 2 - 5;

    // Radio del enemigo (mitad del diámetro)
    const int radioEnemigo = diametro / 2;

    Ptrenemigo temp = enemigos;
    while (temp != nullptr) {

        // Limitar el movimiento a los bordes del área del fondo centrado
        if (temp->x <= centroX) {
            temp->x = centroX;
            temp->velocidadX = abs(temp->velocidadX); // Mover hacia la derecha
        }
        if (temp->x + diametro >= centroX + ResX) {
            temp->x = centroX + ResX - diametro;
            temp->velocidadX = -abs(temp->velocidadX); // Mover hacia la izquierda
        }
        if (temp->y <= centroY) {
            temp->y = centroY;
            temp->velocidadY = abs(temp->velocidadY); // Mover hacia abajo
        }
        if (temp->y + diametro >= centroY + ResY) {
            temp->y = centroY + ResY - diametro;
            temp->velocidadY = -abs(temp->velocidadY); // Mover hacia arriba
        }

        // Verificar colisión con bloques, ajustando por el radio del enemigo
        Ptrbloque bloqueActual = bloques;

        while (bloqueActual != NULL) {

            if (bloqueActual->estado) {

                // Verifica colisión considerando el radio del enemigo
                if (temp->x + radioEnemigo >= bloqueActual->x + centroX &&
                    temp->x - radioEnemigo <= bloqueActual->x + centroX + anchoBloqueEscalado &&
                    temp->y + radioEnemigo >= bloqueActual->y + centroY &&
                    temp->y - radioEnemigo <= bloqueActual->y + centroY + altoBloqueEscalado) {

                    // Colisión detectada, manejar interacción
                    bloqueActual->resistencia--;

                    if (bloqueActual->resistencia == 0) {
                        bloqueActual->estado = false; // Desactivar bloque si resistencia llega a 0
                    }

                    // Rebote del enemigo al chocar con el bloque
                    if (temp->y < bloqueActual->y + centroY || temp->y > bloqueActual->y + centroY + altoBloqueEscalado) {
                        temp->velocidadY = -temp->velocidadY;  // Cambiar dirección vertical
                    }
                    else {
                        temp->velocidadX = -temp->velocidadX;  // Cambiar dirección horizontal
                    }
                    break; // Termina la comprobación una vez que se detecta una colisión
                }
            }
            bloqueActual = bloqueActual->Siguiente;
        }

        // Actualizar posición del enemigo
        temp->x += temp->velocidadX;
        temp->y += temp->velocidadY;

        temp = temp->Siguiente;
    }
}


void CrearArchivo(char* puntaje, char* nombre)//Se crea la función CrearArchivo que guarda el nombre y el puntaje en un archivo en memoria secundaria
{
    FILE* archivo;
    archivo = fopen("resultados.txt", "a");

    if (NULL == archivo) {
        fprintf(stderr, "No se pudo crear archivo %s.\n", "resultados.txt");
        exit(-1);
    }
    else {
        fprintf(archivo, "Nombre:%s\n", nombre);
        fprintf(archivo, "Puntaje: %s\n", puntaje);
        fprintf(archivo, "\n\n");
    }
    fclose(archivo);
}

void CargarArchivo(int x, int y, ALLEGRO_FONT* fuente, int inicio)//Se carga el archivo en memoria secundaria a pantalla con el puntaje y nombre escritos en el display
{
    char nombre[40];
    char puntaje[10];
    int i = 0;
    int e = 0;
    FILE* archivo;
    archivo = fopen("resultados.txt", "r");
    if (archivo != NULL) {
        while (!feof(archivo)) {
            fscanf(archivo, "Nombre:%s\n", nombre);
            fscanf(archivo, "Puntaje: %s\n", puntaje);
            if (inicio <= i && i < inicio + 4) {
                al_draw_text(fuente, al_map_rgb(250, 250, 250), x / 2, (y * (300.0 / 768.0)) + 75 * e, ALLEGRO_ALIGN_RIGHT, "Nombre: ");
                al_draw_text(fuente, al_map_rgb(250, 250, 250), x / 2, (y * (300.0 / 768.0)) + 75 * e, ALLEGRO_ALIGN_LEFT, nombre);
                al_draw_text(fuente, al_map_rgb(250, 250, 250), x / 2, (y * (330.0 / 768.0)) + 75 * e, ALLEGRO_ALIGN_RIGHT, "Puntaje: ");
                al_draw_text(fuente, al_map_rgb(250, 250, 250), x / 2, (y * (330.0 / 768.0)) + 75 * e, ALLEGRO_ALIGN_LEFT, puntaje);
                e++;
            }
            i++;
        }
        fclose(archivo);
    }

}

bool quedanBloques(Ptrbloque bloques) {
    Ptrbloque temp = bloques;
    while (temp != NULL) {
        if (temp->estado) {
            return true; // Aún quedan bloques activos
        }
        temp = temp->Siguiente;
    }
    return false; // No quedan bloques activos
}
