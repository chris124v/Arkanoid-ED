//Funciones del juego implementadas posteriormente 

#pragma once

#include <stdio.h>
#include <iostream>
#include <math.h>
#include <windows.h>
#include <stdlib.h>
#include <cstdlib>
#include <time.h>
#include <algorithm> 

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

const int ResX = 700;
const int ResY = 800;
const int diametro = 32;

typedef struct bola {

    int codigo;
    float x;         // Posición en el eje X
    float y;         // Posición en el eje Y
    float radio;     // Radio de la bola
    float velocidadX; // Velocidad en el eje X
    float velocidadY; // Velocidad en el eje Y
    bool estado;
    int color;
    bola* Siguiente;

}*Ptrbola;

typedef struct nave {
    float x;         // Posición en el eje X
    float y;         // Posición en el eje Y
    int velocidadY;
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

    Jugador.x = x / 2;
    Jugador.y = ResY - 100;
    Jugador.velocidadY = 5;

};

void inicializar_bloque(Ptrbloque& bloques, int i, int x) {

    bloques->y = ((i / 11) + 1) * diametro / 2 * 4;
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

void formacion_bloques(Ptrbloque& bloques, int nivel) {

    const int espacio = 10;  // Espacio entre bloques
    const int numBloquesPorFila = 7;  // Cantidad de bloques en una fila
    int xInicial = (ResX - (numBloquesPorFila * diametro + (numBloquesPorFila - 1) * espacio)) / 2;
    int yInicial = 50;

    for (int i = 0; i < numBloquesPorFila * 5; i++) {
        Ptrbloque nuevoBloque = new bloque;
        nuevoBloque->x = xInicial + (i % numBloquesPorFila) * (diametro + espacio);
        nuevoBloque->y = yInicial + (i / numBloquesPorFila) * (diametro + espacio);
        nuevoBloque->estado = true;
        nuevoBloque->codigo = i;
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

        // Añadir el nuevo bloque a la lista de bloques
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

void colision_bola(bola& balin, Ptrbloque& bloques, nave& jugador, Ptrenemigo& enemigos, int& vidas) {
    // Colisión con las paredes laterales
    if (balin.x - balin.radio <= 0 || balin.x + balin.radio >= ResX) {
        balin.velocidadX = -balin.velocidadX;  // Rebote horizontal
    }

    // Colisión con la pared superior
    if (balin.y - balin.radio <= 0) {
        balin.velocidadY = -balin.velocidadY;  // Rebote vertical
    }

    // Colisión con la pared inferior (pierde una vida)
    if (balin.y + balin.radio >= ResY) {
        vidas--;  // Resta una vida
        balin.x = jugador.x;
        balin.y = jugador.y - 10;
        balin.velocidadY = -fabs(balin.velocidadY);
    }

    // Colisión con bloques
    Ptrbloque bloqueActual = bloques;
    while (bloqueActual != NULL) {
        if (bloqueActual->estado) { // Solo verifica bloques activos
            if (balin.x + balin.radio >= bloqueActual->x &&
                balin.x - balin.radio <= bloqueActual->x + diametro &&
                balin.y + balin.radio >= bloqueActual->y &&
                balin.y - balin.radio <= bloqueActual->y + diametro) {

                bloqueActual->resistencia--;
                if (bloqueActual->resistencia == 0) {
                    bloqueActual->estado = false;
                }
                if (balin.y < bloqueActual->y || balin.y > bloqueActual->y + diametro) {
                    balin.velocidadY = -balin.velocidadY;
                }
                else {
                    balin.velocidadX = -balin.velocidadX;
                }
                break;
            }
        }
        bloqueActual = bloqueActual->Siguiente;
    }

    // Colisión con enemigos
    Ptrenemigo enemigoActual = enemigos;
    while (enemigoActual != NULL) {
        if (enemigoActual->estado) { // Solo verifica enemigos activos
            if (balin.x + balin.radio >= enemigoActual->x &&
                balin.x - balin.radio <= enemigoActual->x + diametro &&
                balin.y + balin.radio >= enemigoActual->y &&
                balin.y - balin.radio <= enemigoActual->y + diametro) {

                // Desactivar el enemigo
                enemigoActual->estado = false;
                // Aquí puedes agregar lógica para aumentar puntaje o efectos visuales
                balin.velocidadY = -balin.velocidadY; // Rebote al colisionar con el enemigo
                break; // Salir después de colisionar con un enemigo
            }
        }
        enemigoActual = enemigoActual->Siguiente;
    }

    // Colisión con la nave
    if (balin.y + balin.radio >= jugador.y && balin.x >= jugador.x && balin.x <= jugador.x + diametro) {
        if (balin.x < jugador.x + diametro / 2) {
            balin.velocidadX = -fabs(balin.velocidadX);
        }
        else {
            balin.velocidadX = fabs(balin.velocidadX);
        }
        if (balin.x < jugador.x + diametro / 4) {
            balin.velocidadY = -fabs(balin.velocidadY) * 0.8;
        }
        else if (balin.x > jugador.x + 3 * diametro / 4) {
            balin.velocidadY = -fabs(balin.velocidadY) * 0.8;
        }
        else {
            balin.velocidadY = -fabs(balin.velocidadY);
        }
    }
}



void inicializar_bola(Ptrbola& Balin, nave& jugador, int velocidad) {

    // Inicializar la posición de la bola encima de la nave
    Balin->x = jugador.x;
    Balin->y = jugador.y - Balin->radio - 10; // Colocar la bola a 10 pixeles por encima de la nave

    // Inicializar la velocidad de la bola
    Balin->velocidadX = velocidad;
    Balin->velocidadY = -velocidad; // La bola se mueve inicialmente hacia arriba

    Balin->estado = true; // Establecer el estado de la bola a activa
}

void generar_enemigos(Ptrenemigo& enemigos, int cantidad) {

    for (int i = 0; i < cantidad; ++i) {
        Ptrenemigo nuevoEnemigo = new enemigo;
        nuevoEnemigo->tipo = rand() % 3; // Tipos de enemigos: 0 = Adherido, 1 = Rebote, 2 = Lineal
        nuevoEnemigo->x = rand() % (ResX - diametro); // Posición inicial aleatoria en X
        nuevoEnemigo->y = rand() % (ResY / 2); // Posición inicial aleatoria en Y (parte superior de la pantalla)
        nuevoEnemigo->estado = true; // Inicia activo
        nuevoEnemigo->Siguiente = nullptr;

        // Configurar el movimiento basado en el tipo de enemigo
        switch (nuevoEnemigo->tipo) {
        case 0: // Adherido
            nuevoEnemigo->velocidadX = 2; // Velocidad para simular caminata
            nuevoEnemigo->velocidadY = 2; // Velocidad para simular caminata
            break;

        case 1: // Rebote
            nuevoEnemigo->velocidadX = (rand() % 3 + 1) * (rand() % 2 == 0 ? -1 : 1); // Velocidad aleatoria en X
            nuevoEnemigo->velocidadY = (rand() % 3 + 1) * (rand() % 2 == 0 ? -1 : 1); // Velocidad aleatoria en Y
            break;

        case 2: // Lineal (Arriba-Abajo-Izquierda-Derecha)
            nuevoEnemigo->velocidadX = (rand() % 2 == 0 ? 2 : 0); // Puede moverse en X o Y
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

void mover_enemigos(Ptrenemigo& enemigos) {

    Ptrenemigo temp = enemigos;
    while (temp != nullptr) {
        switch (temp->tipo) {
        case 0: // Adherido
            // Movimiento adherido, pegado a paredes o bloques
            if (temp->x <= 0 || temp->x + diametro >= ResX) temp->velocidadX = -temp->velocidadX;
            if (temp->y <= 0 || temp->y + diametro >= ResY) temp->velocidadY = -temp->velocidadY;
            break;

        case 1: // Rebote
            // Movimiento de rebote en los bordes de la pantalla
            if (temp->x <= 0 || temp->x + diametro >= ResX) temp->velocidadX = -temp->velocidadX;
            if (temp->y <= 0 || temp->y + diametro >= ResY) temp->velocidadY = -temp->velocidadY;
            break;

        case 2: // Lineal (Arriba-Abajo-Izquierda-Derecha)
            // Movimiento lineal con cambio de dirección aleatorio
            if (temp->x <= 0 || temp->x + diametro >= ResX) temp->velocidadX = -temp->velocidadX;
            if (temp->y <= 0 || temp->y + diametro >= ResY) temp->velocidadY = -temp->velocidadY;

            // Cambiar dirección al azar cada cierto tiempo
            if (rand() % 100 < 2) { // Probabilidad de 2% de cambiar de dirección
                temp->velocidadX = (rand() % 2 == 0 ? 2 : -2);
                temp->velocidadY = (rand() % 2 == 0 ? 2 : -2);
            }
            break;
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