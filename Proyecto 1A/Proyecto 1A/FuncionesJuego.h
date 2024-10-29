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
    bool Estado;
    int color;       // Color de la bola (opcional, según Allegro)
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
    bool Estado;
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
    enemigo* Siguiente;
}*Ptrenemigo;

void Inicializarnave(nave& Jugador, int x){
    Jugador.x = x / 2;
    Jugador.y = ResY - 100;
    Jugador.velocidadY = 5;
}

void inicializarbloque(Ptrbloque& bloques) {


};