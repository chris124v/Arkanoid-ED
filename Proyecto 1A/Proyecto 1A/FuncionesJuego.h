//Funciones del juego implementadas posteriormente en el juego.h

//Christopher Daniel Vargas Villalta, Carnet: 2024108443
//Santiago Espinoza Rendon, Carnet: 2024156530

#pragma once

//Liberias propios de C++
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <windows.h>
#include <stdlib.h>
#include <cstdlib>
#include <time.h>
#include <algorithm> 

//Librerias de allegro 
#include <allegro5/allegro.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

//Inlcude del juego
#include "Juego.h"

//Esto lo hacemos para evitar problemas con los archivos
using namespace std;
#pragma warning(disable:4996);  

//Establecemos el area en donde se realizara el juego
const int ResX = 800;
const int ResY = 1100;
const int diametro = 32;

//Estas constantes nos serviran para establecer los tamanos de la nave o barra
const int nave_ancho = 100;  
const int nave_alto = 35;

//Este seria el struct de la bola
typedef struct bola {

    int x;              // Posicion en el eje x
    int y;              // Posicion en el eje y
    int radio;          // Radio de la bola
    float velocidadX;   // Velocidad en el eje x
    float velocidadY;   // Velocidad en el eje Y        
    bool estado;        //Estado para saber si esta viva o no
    bola* Siguiente;    //Establecemos un puntero como lista enlaza

}*Ptrbola;

//Struct de la nave 
typedef struct nave {
    float x;         // Posicion en el eje x
    float y;         // Posicion en el eje y
    int velocidadX;  //velocidad lateral de la nave
    bool estado;     //Estado de la nave para saber si esta viva
};

//Struct del bloque
typedef struct bloque {

    float x;         // Posicion en el eje x
    float y;         // Posicion en el eje y
    bool estado;     // Estado del bloque para ver si se elimino
    int resistencia; // Nivel de resistencia del bloque
    bloque* Siguiente; // Puntero al siguiente bloque en la lista

}*Ptrbloque;

//Struct del enemigo que aparece
typedef struct enemigo {

    float x;         // Posicion en el eje x
    float y;         // Posicion en el eje y
    float velocidadX; // Velocidad en el eje x
    float velocidadY; // Velocidad en el eje y
    int tipo;         //Tipo de enemigo que va a aparecer
    bool estado;      //Estado para ver si esta vivo o no
    enemigo* Siguiente; //Establecemos la lista enlazada

}*Ptrenemigo;

//En esta funcion inicializamos la nave 
void Inicializar_nave(nave& Jugador, int x) {

    
    Jugador.x = ResX / 2 - 40;  //Posicion en x segun el area
    Jugador.y = ResY - 300;     //Posicion en y segun el area
    Jugador.velocidadX = 5;     //Rapidez de movimiento lateral
    Jugador.estado = true; // Activa la nave para que se muestre

};

//Funcion de inicializar el bloque
void inicializar_bloque(Ptrbloque& bloques, int i, int x) {

    bloques->y = ((x / 11) + 1) * diametro / 2 * 4; //Establecemos la posicion en el y segun el diametro del juego
    bloques->estado = true; //Inicialmente el bloque esta activo o vivo
    bloques->resistencia = rand() % 5 + 1; // Asignar un nivel de resistencia aleatorio entre 1 y 5
    bloques->Siguiente = NULL; //Establecemos el null de la lista enlazada
}

//Funcion de inicializar el enemigo 
void inicializar_enemigo(Ptrenemigo& enemigo, int i, int t, int x) {

    enemigo->x = x / 2 - ResX / 2 + (i % 11) * ((ResX - 200) / 10) + 100;  //Establece posicion en x del enemigo
    enemigo->y = ((i / 11) + 1) * diametro / 2 * 4; //Posicion en y segun el area
    enemigo->velocidadX = 5;   //Velocididad del enemigo en x 
    enemigo->velocidadY = 5;   //Velocidad del enemigo en Y
    enemigo->tipo = t;         //Tipo del enemigo lo definimos despues
    enemigo->estado = true;     //Inicialmente esta vivo
    enemigo->Siguiente = NULL;  //Establecemos la lista enlazada

}

//Funcion para dibujar la nave con un bitmap, usamos parametros posteriormente utilizables en juego
void dibujar_nave(nave& jugador, ALLEGRO_BITMAP* nave_bitmap, int centroX, int centroY) {

    //Si la nave esta activa
    if (jugador.estado) {  

        // Determinamos el tamano original de la nave
        int bitmap_ancho = al_get_bitmap_width(nave_bitmap);
        int bitmap_alto = al_get_bitmap_height(nave_bitmap);

        // Se dibuja el bitmap de la nave con las especificaciones del tamano, tambien tomamos las dimensiones de la nave que se iniciliazaron como constantes
        al_draw_scaled_bitmap(nave_bitmap, 0, 0, bitmap_ancho, bitmap_alto, jugador.x + centroX, jugador.y + centroY, nave_ancho, nave_alto, 0);
    }
}

//Funcion para formar los bloques segun 10 niveles con diferentes patrones de bloques
void formacion_bloques(Ptrbloque& bloques, int nivel, ALLEGRO_BITMAP* bloques2) {

    //Constantes para escalar los bloques y hacer la generacion
    const float escalar_b = 1.15;
    const int ancho_b = al_get_bitmap_width(bloques2) * escalar_b;
    const int alto_b = al_get_bitmap_height(bloques2) * escalar_b;
    const int bloques_filas = 8; //Bloques por cada fila

    int centrar_f = (ResX - (bloques_filas * ancho_b)) / 2; //Centrar horizontalmente la fila de bloques
    int yInicial = 100; //Posicion en vertical inicial
    int filasLimite = (nivel == 3) || (nivel == 9) ? 7 : 5; //Ajustamos el numero de filas segun cada nivel

    // Ciclo para crear bloques segun filas y columnas
    for (int i = 0; i < bloques_filas * filasLimite; i++) {

        //Calculamos la fila y la columna para cada bloque
        int fila = i / bloques_filas;
        int columna = i % bloques_filas;
        int xInicial = centrar_f;
        int bloques_en_fila = bloques_filas;

        //Estos serian dos casos especificos de formaciones para los bloques
        
        //  Nivel 5 de piramide
        if (nivel == 5) {

            bloques_en_fila = bloques_filas - fila;  // Disminuye los bloques en cada fila
            xInicial = (ResX - (bloques_en_fila * ancho_b)) / 2; //Basicamente la cantidad de bloques en cada fila
            if (columna >= bloques_en_fila) continue;  // Salta las columnas fuera del límite
        }

        // Nivel 7 alternacion de bloques
        if (nivel == 7) {

            bloques_en_fila = (bloques_filas + 1) / 2;  // Mitad de los bloques en alternancia
            xInicial = (ResX - ((bloques_en_fila * 2 - 1) * ancho_b)) / 2;  // Asegura que los bloques no se desborden
            if (columna % 2 != 0) continue;  // Desactiva bloques en posiciones pares
        }

        // Crear y posicionar el bloque como tal seria como la inicializacion
        Ptrbloque nuevoBloque = new bloque;

        //Establecemos posiciones x y y del nuevo bloque basandonos en los niveles, filas y columnas
        nuevoBloque->x = xInicial + (nivel == 7 ? (columna / 2) * 2 * ancho_b : columna * ancho_b);
        nuevoBloque->y = yInicial + fila * alto_b;

        nuevoBloque->estado = true;
        nuevoBloque->resistencia = rand() % 5 + 1;
        nuevoBloque->Siguiente = NULL;

        // Configuración general para cada nivel

        // Nivel 1 filas completas
        if (nivel == 1) {
            
        }

        //Nivel 2 en zigzag
        else if (nivel == 2) {

            if (columna % 2 == 0) nuevoBloque->y += alto_b / 2; //Desplazamos los bloques hacia abajo

        }

        // Nivel 3 alternando filas y dejando las filas pares vacias y la ultima fila activa
        else if (nivel == 3) {
            
            if (fila % 2 != 0) {
                nuevoBloque->estado = false;
            }
        }

        // Nivel 4 diagonal mas ancha con tres bloques de ancho en cada escalon
        else if (nivel == 4) {
            
            if ((columna != fila) && (columna != fila + 1) && (columna != fila + 2) && (columna != fila + 3)) {
                nuevoBloque->estado = false;
            }
        }

        //Saltamos a nivel 6 porque ya hicimos el 5 que era especifico
        else if (nivel == 6) {

            // Ajustamos cada bloque para que sea del tamano de la piramide
            if ((fila == 0 && (columna < 2 || columna > 5)) ||
                (fila == 1 && (columna < 1 || columna > 6)) ||
                (fila == 3 && (columna < 1 || columna > 6)) ||
                (fila == 4 && (columna < 2 || columna > 5))) {
                nuevoBloque->estado = false;
            }
        }

        //Pasamos al nivel 8 porque ya hicimos el 7
        else if (nivel == 8) {
            
            //Este corresponde a un patron disperso por columnas
            if ((columna < 2 || columna > 5) && fila < 3) nuevoBloque->estado = false;
        }

        // Nivel 9 filas y columnas alternas de bloques
        if (nivel == 9) {
            bool esBloqueActivo = (fila % 2 == 0) ? (columna % 2 == 0) : (columna % 2 != 0);
            if (!esBloqueActivo) continue;
        }

        // Nivel 1o (ultim)
        if (nivel == 10) {
            if (columna < 2 || columna > 5) {  // Bloques activos solo en bordes
                nuevoBloque->estado = (fila % 3 != 0);  // Espaciado irregular cada tercera fila
            }
            else {
                nuevoBloque->estado = true;
            }
        }

        // Anadir el nuevo bloque a la lista

        if (nuevoBloque->estado) {  // Solo anadimos bloques activos o que esten vivos
            
            //Verificamos si la lista esta vacia anadimos este bloque
            if (!bloques) {
                bloques = nuevoBloque;
            }

            //Si no se ha anadido lo recorremos y agregamos al final como si de una cola se tratase
            else {
                Ptrbloque temp = bloques;
                while (temp->Siguiente) temp = temp->Siguiente;
                temp->Siguiente = nuevoBloque;
            }
        }

        else {
            delete nuevoBloque;  // Libera memoria de los bloques inactivos
        }
    }
}

//Funcion de inicializar la bola, tomamos como parametros la bola, nave y el radio de la bola
void inicializar_bola(Ptrbola& bola, nave& jugador, int radio) {

    //Creamos una nueva bola
    bola = new struct bola; 

    //Aqui establecemos la posicion x y y de la bola para que aparezca un poco mas arriba de la nave
    bola->x = jugador.x + 600; 
    bola->y = jugador.y - 100; 

    //Este seria el radio de la bola
    bola->radio = 10; 

    //Finalmente estarian velocidades aleatorias para cada pasada del juego
    bola->velocidadX = rand() % 2 + 1;
    bola->velocidadY = - (rand() % 2 + 1) ; 
    
    bola->estado = true; // La bola esta viva
    bola->Siguiente = NULL; // Lista enlazada
}

//Funcion de la colision de la bola, toma como parametros: Bola, bloques, jugador, enemigos, vidas, puntos, coordenadas x y y, tamano actual de los bloques.
void colision_bola(Ptrbola& balin, Ptrbloque& bloques, nave& jugador, Ptrenemigo& enemigos, int& vidas, int centroX, int centroY, ALLEGRO_BITMAP* bloques2, int& puntos, int& bloques_elim, int&enemigos_elim) {

    // Este seria el estandar del tamano del bloque para la hitbox de colision
    const int anchoBloqueEscalado = al_get_bitmap_width(bloques2) * 1.15;
    const int altoBloqueEscalado = al_get_bitmap_height(bloques2) * 1.15;

    // Radio de la bola
    const int radioBola = balin->radio;

    // Limitar el movimiento a los bordes designados

    //Aqui establecemos el movimiento de la bola segun los bordes

    /*
    Este seria el de la parte izquierda, basicamente si la posicion de la
    bola menos el radio de la bola es menor o igual al centroX que es el borde
    designado va a invertir la velocidad en x de la bola, este proceso se repite 
    para todos los bordes
    */

    //Caso de la izquierda
    if (balin->x - radioBola <= centroX) {
        balin->x = centroX + radioBola; // Ajustar la posicion de la bola
        balin->velocidadX = -balin->velocidadX; // Cambiar direccion segun pega
    }

    //Caso de la derecha
    if (balin->x + radioBola >= centroX + ResX) {
        balin->x = centroX + ResX - radioBola; // Ajustar la posicion
        balin->velocidadX = -balin->velocidadX; // Cambiar direccion
    }

    //Caso del borde superior
    if (balin->y - radioBola <= centroY) {
        balin->y = centroY + radioBola; // Ajustar la posición para no salir
        balin->velocidadY = -balin->velocidadY; // Cambiar dirección
    }

    //Este seria el caso del borde inferior en donde si la bola baja se reducen las vidas
    if (balin->y + radioBola >= centroY + ResY) {

        vidas--;  // Resta una vida
        balin->x = jugador.x + centroX;  // Reposiciona en la zona centrada
        balin->y = jugador.y + centroY - 10; // Reposicionar justo encima de la nave
        balin->velocidadY = -fabs(balin->velocidadY); // Invertir direccion

    }

    // Verificar colision con bloques llamamos a la struct
    Ptrbloque bloqueActual = bloques;

    //Recorrer la lista enlazada bloques hasta llegar NULL
    while (bloqueActual != NULL) {

        //Si el bloque aun se encuentra vivo
        if (bloqueActual->estado) {

            // Aca determinamos si la bola choca con el bloque actual considerando todos los dados
            if (balin->x + radioBola >= bloqueActual->x + centroX && balin->x - radioBola <= bloqueActual->x + centroX + anchoBloqueEscalado &&
                balin->y + radioBola >= bloqueActual->y + centroY &&
                balin->y - radioBola <= bloqueActual->y + centroY + altoBloqueEscalado) {


                // En caso de que se detecte una colision se va a reducir la resistencia del bloque para que pueda ser destruido
                bloqueActual->resistencia--;

                //En caso de que la resistencia llegue a 0
                if (bloqueActual->resistencia <= 0) {

                    //El bloque con el que pego desaparece
                    bloqueActual->estado = false; 

                    //Por cada bloque eliminado se suma 5 a la puntuacion general
                    puntos += 5;
                    bloques_elim++;

                }

                

                // Rebote de la bola al chocar con el bloque segun las dimensiones
                float choqueX = (balin->x + radioBola) - (bloqueActual->x + centroX);
                float choqueY = (balin->y + radioBola) - (bloqueActual->y + centroY);

                //Revisamos con fabs que es valor absoluto donde se da el choque
                if (fabs(choqueX) < fabs(choqueY)) {
                    balin->velocidadY = -balin->velocidadY; // Cambiar direccion vertical
                    balin->y += (balin->velocidadY > 0) ? -1 : 1; // Ajustar posicion de la bola y su velocidad ajustamos un poco la velocidad
                }

                //Aqui se realiza lo mismo pero para el eje horizontal
                else {
                    balin->velocidadX = -balin->velocidadX; // Cambiar direccion horizontal
                    balin->x += (balin->velocidadX > 0) ? -1 : 1; // Ajustamos la posicion
                }

                break; 
            }
        }

        // Avanzar al siguiente bloque
        bloqueActual = bloqueActual->Siguiente;
    }

    // Esta seria la colision pero con la nave 
    if (jugador.estado) {

        // Calcular las posiciones de la hitbox de la nave segun su posicion
        float nave_izqui = jugador.x + centroX;
        float nave_dere = nave_izqui + nave_ancho;
        float nave_superior = jugador.y + centroY;
        float nave_inf = nave_superior + nave_alto;

        // Verificar si la bola esta en la hitbox de la nave, anadimos +10 para si pegue justo en la nave
        if (balin->x + radioBola >= nave_izqui && balin->x - radioBola <= nave_dere && balin->y + radioBola >= nave_superior + 10 &&
            balin->y - radioBola <= nave_inf) {

            //En caso de que si haya una colision en el eje y
            balin->velocidadY = -balin->velocidadY; // Se cambia la direccion en vertical
            balin->y += (balin->velocidadY > 0) ? -1 : 1; // Ajustamos la posicion de la bola segun velocidad

            // Determina si la colision es en el borde izquierdo o derecho de la nave
            if (balin->x < nave_izqui + nave_ancho / 4) { // Colision en el borde izquierdo
                balin->velocidadX = -abs(balin->velocidadX); // Rebote hacia la izquierda
            }

            //Esta seria la del derecho
            else if (balin->x > nave_dere - nave_ancho / 4) { // Colision en el borde derecho
                balin->velocidadX = abs(balin->velocidadX); // Rebote hacia la derecha
            }

            // Invertir la dirección vertical de la bola, usamos abs para el valor absoluto
            balin->velocidadY = -abs(balin->velocidadY);

        }
    }

    // Este seria el apartado de como colisiona con los enemigos

    Ptrenemigo enemigoActual = enemigos;

    // Se recorre toda la lista enlazada 
    while (enemigoActual != NULL) {

        //En caso de que el enemigo este vivo
        if (enemigoActual->estado) {

            //Establecemos una hitbox que tome en cuenta la dimensiones del enemigo
            float enemigo_izqui = enemigoActual->x - diametro / 2;
            float enemigo_dere = enemigoActual->x + diametro / 2;
            float enemigo_arri = enemigoActual->y - diametro / 2;
            float enemigo_abajo = enemigoActual->y + diametro / 2;

            // Verificar si la bola colisiona con la hitbox del enemigo segun los datos establecidos
            if (balin->x + radioBola >= enemigo_izqui && balin->x - radioBola <= enemigo_dere &&
                balin->y + radioBola >= enemigo_arri &&
                balin->y - radioBola <= enemigo_abajo) {

                // Desactivar el enemigo o eliminarlo
                enemigoActual->estado = false;

                //Por cada enemigo eliminado se suman 10 puntos
                puntos += 10;
                enemigos_elim++;

                // Determinar desde que direccion vino la colision
                float cayoX = balin->x - enemigoActual->x;
                float cayoY = balin->y - enemigoActual->y;

                // Ajustar la dirección de rebote basado en el punto de impacto
                if (fabs(cayoX) > fabs(cayoY)) {

                    // Colision horizontal
                    balin->velocidadX = -balin->velocidadX;
                }

                //Colision en el caso vertical 
                else {
                    // Colision vertical
                    balin->velocidadY = -balin->velocidadY;
                }

                // Ajustamos la posicion del balin segun la velocidad
                balin->x += balin->velocidadX;
                balin->y += balin->velocidadY;

                break; 
            }
        }

        //Pasamos al siguiente enemigo
        enemigoActual = enemigoActual->Siguiente;
    }

    // Actualizar posición de la bola
    balin->x += balin->velocidadX;
    balin->y += balin->velocidadY;
}


//Funcion para generar los enemigos
void generar_enemigos(Ptrenemigo& enemigos, int cantidad, int nivel, Ptrbloque bloques, int posicionNaveY, int centroY, int centroX) {

    // Parametros del area jugable basados en los valores proporcionados

    const int margenSuperior = centroY + diametro + 300;            // Borde superior justo debajo de los bloques
    const int margenInferior = posicionNaveY - 50;                 // Borde inferior justo encima de la nave
    const int margenIzquierdo = centroX + diametro + 15;           // Borde izquierdo de la zona jugable
    const int margenDerecho = centroX + ResX - diametro - 20;      // Borde derecho de la zona jugable
    const int anchoEnemigo = diametro;                             // Ancho del enemigo
    const int altoEnemigo = diametro;                              // Altura del enemigo

    // Generar enemigos dentro de los limites definidos
    for (int i = 0; i < cantidad; ++i) {

        //Establecemos un nuevo enemigo
        Ptrenemigo nuevoEnemigo = new enemigo;

        // Seleccion aleatoria del tipo de enemigo (0 = Adherido, 1 = Rebote, 2 = Lineal)
        int tipo = rand() % 4; 

        //Llamamos a inicializar enemigo
        inicializar_enemigo(nuevoEnemigo, i, tipo, ResX);

        // Generamos posicion aleatoria dentro de la zona delimitada
        int xPos = margenIzquierdo + (rand() % (margenDerecho - margenIzquierdo - anchoEnemigo));
        int yPos = margenSuperior + (rand() % (margenInferior - margenSuperior - altoEnemigo));

        // Enemigo inicia vivo
        nuevoEnemigo->x = xPos;
        nuevoEnemigo->y = yPos;
        nuevoEnemigo->estado = true;                  
        nuevoEnemigo->Siguiente = NULL;

        // Configurar el movimiento basado en el tipo de enemigo mediante 

        switch (tipo) {

        case 0: // Izquierda y derecha
            nuevoEnemigo->velocidadX = (rand() % 3 + 1) * (rand() % 2 == 0 ? -1 : 1); // Establecemos velocidad ranodm en x
            nuevoEnemigo->velocidadY = 0; //No hay movimiento en y
            break;

        case 1: // Rebote
            nuevoEnemigo->velocidadX = (rand() % 3 + 1) * (rand() % 2 == 0 ? -1 : 1); // Velocidad aleatoria en ambos casos
            nuevoEnemigo->velocidadY = (rand() % 3 + 1) * (rand() % 2 == 0 ? -1 : 1); 
            break;

        case 2: // Movimiento en ambas direcciones
            nuevoEnemigo->velocidadX = (rand() % 3 + 1) * (rand() % 2 == 0 ? -1 : 1); // Velocidad aleatoria en ambos casos
            nuevoEnemigo->velocidadY = (rand() % 3 + 1) * (rand() % 2 == 0 ? -1 : 1); 
            break;

        case 3: // Arriba y abajo
            nuevoEnemigo->velocidadX = 0; // No permite movimiento en X
            nuevoEnemigo->velocidadY = (rand() % 3 + 1) * (rand() % 2 == 0 ? -1 : 1); 
            break;
        }

        // Anadir el nuevo enemigo a la lista de enemigos si no hay
        if (!enemigos) {
            enemigos = nuevoEnemigo;
        }

        //Anadir los enemigos a la lista enlazada al final como si fuera un cola
        else {
            Ptrenemigo Aux = enemigos;
            while (Aux->Siguiente) Aux = Aux->Siguiente;
            Aux->Siguiente = nuevoEnemigo;
        }
    }
}

//Funcion para mover los enemigos
void mover_enemigos(Ptrenemigo& enemigos, Ptrbloque& bloques, int centroX, int centroY, ALLEGRO_BITMAP* bloques2) {
   
    // Definir el tamano escalado de los bloques
    const int anchoBloqueEscalado = al_get_bitmap_width(bloques2) / 2 - 5;
    const int altoBloqueEscalado = al_get_bitmap_height(bloques2) / 2 - 5;

    // Radio del enemigo 
    const int radioEnemigo = diametro / 2;

    //Inicializamos la lista de enemigos
    Ptrenemigo Aux = enemigos;
    while (Aux != nullptr) {

        // Limitar el movimiento a los bordes del area del fondo centrado
        if (Aux->x <= centroX) {
            Aux->x = centroX;
            Aux->velocidadX = abs(Aux->velocidadX); // Mover hacia la derecha usando abs
        }
        if (Aux->x + diametro >= centroX + ResX) {
            Aux->x = centroX + ResX - diametro;
            Aux->velocidadX = -abs(Aux->velocidadX); // Mover hacia la izquierda usando abs
        }

        //Caso para mover hacia abajo
        if (Aux->y <= centroY) {
            Aux->y = centroY;
            Aux->velocidadY = abs(Aux->velocidadY); // Mover hacia abajo
        }

        //Caso para mover hacia arriba
        if (Aux->y + diametro >= centroY + ResY) {
            Aux->y = centroY + ResY - diametro;
            Aux->velocidadY = -abs(Aux->velocidadY); // Mover hacia arriba
        }

        // Verificar colision con bloques, ajustando por el radio del enemigo
        Ptrbloque bloqueActual = bloques;

        //Mientras no se hayan recorrido todos los bloques
        while (bloqueActual != NULL) {

            //Si estan activos
            if (bloqueActual->estado) {

                // Verifica colision considerando el radio del enemigo
                if (Aux->x + radioEnemigo >= bloqueActual->x + centroX &&
                    Aux->x - radioEnemigo <= bloqueActual->x + centroX + anchoBloqueEscalado &&
                    Aux->y + radioEnemigo >= bloqueActual->y + centroY &&
                    Aux->y - radioEnemigo <= bloqueActual->y + centroY + altoBloqueEscalado) {

                   
                    // Rebote del enemigo al chocar con el bloque
                    if (Aux->y < bloqueActual->y + centroY || Aux->y > bloqueActual->y + centroY + altoBloqueEscalado) {
                        Aux->velocidadY = -Aux->velocidadY;  // Cambiar direccion vertical
                    }
                    else {
                        Aux->velocidadX = -Aux->velocidadX;  // Cambiar direccion horizontal
                    }
                    break; 
                }
            }

            //Sigue recorriendo la lista enlazada
            bloqueActual = bloqueActual->Siguiente;
        }

        // Actualizar posicion del enemigo
        Aux->x += Aux->velocidadX;
        Aux->y += Aux->velocidadY;

        //Sigue recorriendo la lista
        Aux = Aux->Siguiente;
    }
}

//Funcion que revisa la lista enlazada de bloques
bool quedanBloques(Ptrbloque bloques) {

    //Establecemos un auxiliar para recorrer la lista enlazada
    Ptrbloque Aux = bloques;

    //Mientras no se llegue al final
    while (Aux != NULL) {

        //Si el bloque esta activo
        if (Aux->estado) {
            return true; //Retorna true
        }

        //Seguimos recorriendo la lista enlazada
        Aux = Aux->Siguiente;
    }

    //Esto en caso de que no queden bloques activos
    return false;
}

//Funcion creararchivo que crea un archivo con un puntaje general y el nombre
void CrearArchivo(char* puntaje, char* nombre, char* enem_elim, char* b_elim)
{
    FILE* archivo;
    archivo = fopen("resultados.txt", "a"); //Abre el archivo llamado resultados

    //Si no se puede encontrar se indica
    if (NULL == archivo) {
        fprintf(stderr, "No se pudo crear archivo %s.\n", "resultados.txt");
        exit(-1);
    }

    //En caso contrario escribe nombre y puntaje en el archivo
    else {
        fprintf(archivo, "Nombre:%s\n", nombre);
        fprintf(archivo, "Puntaje: %s\n", puntaje);
        fprintf(archivo, "Enemigos Eliminados: %s\n", enem_elim);
        fprintf(archivo, "Bloques Eliminados: %s\n", b_elim);
        fprintf(archivo, "\n\n");
    }

    //Cerramos el archivo
    fclose(archivo);
}

//Se carga el archivo en memoria secundaria a pantalla con el puntaje y nombre escritos en el display
void CargarArchivo(int x, int y, ALLEGRO_FONT* fuente, int inicio)
{   
    //Establecemos limites para nombre y puntaje
    char nombre[40];
    char puntaje[30];
    char enemigos_eliminados[30];
    char bloques_eliminados[30];
    int i = 0;
    int e = 0;

    FILE* archivo;

    //Abre el archivo en tipo read
    archivo = fopen("resultados.txt", "r");

    //Si el archivo es diferente de null
    if (archivo != NULL) {
        
        //Recorremos hasta el final del archivo
        while (!feof(archivo)) {

            //Leemos nombre y puntaje
            fscanf(archivo, "Nombre:%s\n", nombre);
            fscanf(archivo, "Puntaje: %s\n", puntaje);
            fscanf(archivo, "Enemigos eliminados:%s\n", enemigos_eliminados);
            fscanf(archivo, "Bloques elimnados:%s\n", bloques_eliminados);

            //Leemos tanto nombre como puntaje y lo imprimimos
            if (inicio <= i && i < inicio + 4) {
                al_draw_text(fuente, al_map_rgb(250, 250, 250), x / 2, (y * (300.0 / 768.0)) + 75 * e, ALLEGRO_ALIGN_RIGHT, "Nombre: ");
                al_draw_text(fuente, al_map_rgb(250, 250, 250), x / 2, (y * (300.0 / 768.0)) + 75 * e, ALLEGRO_ALIGN_LEFT, nombre);
                al_draw_text(fuente, al_map_rgb(250, 250, 250), x / 2, (y * (330.0 / 768.0)) + 75 * e, ALLEGRO_ALIGN_RIGHT, "Puntaje: ");
                al_draw_text(fuente, al_map_rgb(250, 250, 250), x / 2, (y * (330.0 / 768.0)) + 75 * e, ALLEGRO_ALIGN_LEFT, puntaje);
                al_draw_text(fuente, al_map_rgb(250, 250, 250), x / 2, (y * (360.0 / 768.0)) + 75 * e, ALLEGRO_ALIGN_RIGHT, "Enemigos Eliminados: ");
                al_draw_text(fuente, al_map_rgb(250, 250, 250), x / 2, (y * (360.0 / 768.0)) + 75 * e, ALLEGRO_ALIGN_LEFT, enemigos_eliminados);
                al_draw_text(fuente, al_map_rgb(250, 250, 250), x / 2, (y * (390.0 / 768.0)) + 75 * e, ALLEGRO_ALIGN_RIGHT, "Bloques Eliminados: ");
                al_draw_text(fuente, al_map_rgb(250, 250, 250), x / 2, (y * (390.0 / 768.0)) + 75 * e, ALLEGRO_ALIGN_LEFT, bloques_eliminados);
                e++;
            }

            //Se hace para todos los existentes
            i++;
        }

        //Cerramos el archivo
        fclose(archivo);
    }

}



