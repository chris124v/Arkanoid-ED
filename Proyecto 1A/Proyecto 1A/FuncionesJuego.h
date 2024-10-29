//


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
    bool estado;
    int codigo;
    bloque* Siguiente;

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
    bloques->Siguiente = NULL;

};

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

void formacion_bloques(Ptrbloque& bloques, int nivel, int anchoVentana) {
    const int espacio = 10;  // Espacio entre bloques
    const int numBloquesPorFila = 7;  // Cantidad de bloques en una fila
    int xInicial = (anchoVentana - (numBloquesPorFila * diametro + (numBloquesPorFila - 1) * espacio)) / 2;
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





void colision_bola(nave& nave, bola& balin, enemigo& enem) {


}

void inicializar_bola(Ptrbola& Balin, nave& jugador, int velocidad) {




}