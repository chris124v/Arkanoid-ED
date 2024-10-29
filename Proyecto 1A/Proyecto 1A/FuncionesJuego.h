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

void Inicializar_nave(nave& Jugador, int x){

    Jugador.x = x / 2;
    Jugador.y = ResY - 100;
    Jugador.velocidadY = 5;

}

void inicializar_bloque(Ptrbloque& bloques, int i, int x) {

    bloques -> x = x / 2 - ResX / 2 + (i % 11) * ((ResX - 200) / 10) + 100;
    bloques -> y = ((i / 11) + 1) * diametro / 2 * 4; 
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







void colision_bola(nave& nave, bola& balin, enemigo& enem) {


}

void inicializar_bola(Ptrbola& Balin, nave& jugador, int velocidad) {
    
    Balin.x = jugador.x;
    Balin.y = jugador.y - 10;
    

}