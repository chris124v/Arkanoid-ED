#pragma once

#include "FuncionesJuego.h"

using namespace std;
#pragma warning(disable:4996)
#define FPS 60.0

int arkanoid(int nivel, int vidas) {

    ALLEGRO_MONITOR_INFO monitor;
    al_get_monitor_info(0, &monitor);
    const int pantallaAncho = monitor.x2 - monitor.x1;
    const int pantallaAlto = monitor.y2 - monitor.y1;

    // Dimensiones fijas del área de juego
    const int RX = 800; // Ancho fijo del área de juego
    const int RY = 1100; // Altura fija del área de juego

    al_set_new_display_flags(ALLEGRO_FULLSCREEN); // Pantalla completa
    ALLEGRO_DISPLAY* pantalla = al_create_display(pantallaAncho, pantallaAlto);
    al_set_window_title(pantalla, "Arkanoid");

    if (!pantalla) {
        al_show_native_message_box(NULL, "Ventana Emergente", "Error", "No se puede crear la pantalla", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return 0;
    }

    ALLEGRO_FONT* fuente1 = al_load_font("Video-Font.ttf", 40, NULL);
    ALLEGRO_FONT* fuente2 = al_load_font("Video-Font.ttf", 30, NULL);
    if (!fuente1 || !fuente2) {
        al_show_native_message_box(pantalla, "Error", "Carga de Fuente", "No se pudo cargar las fuentes.", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return 0;
    }

    ALLEGRO_BITMAP* bloque = al_load_bitmap("Imagenes/Celeste_1.png");
    ALLEGRO_BITMAP* fondo = al_load_bitmap("Imagenes/fondo_juego.png");
    ALLEGRO_BITMAP* nave2 = al_load_bitmap("Imagenes/Nave_2.png");
    ALLEGRO_BITMAP* fondo2 = al_load_bitmap("Imagenes/fondo_main.jpg");

    if (!bloque || !fondo || !nave2) {
        al_show_native_message_box(pantalla, "Error", "Carga de Imagen", "No se pudo cargar una o más imágenes.", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return 0;
    }
    enum Direccion { NINGUNA, IZQUIERDA, DERECHA };
    enum Direccion Dir = NINGUNA;

    // Factor de escala para reducir el tamaño de la imagen al 50%
    const float escalaFactor = 0.5;

    // Calcula el ancho y alto escalados del bloque
    const int anchoBloqueEscalado = al_get_bitmap_width(bloque) * escalaFactor;
    const int altoBloqueEscalado = al_get_bitmap_height(bloque) * escalaFactor;

    ALLEGRO_EVENT_QUEUE* cola_eventos = al_create_event_queue();
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);
    al_register_event_source(cola_eventos, al_get_timer_event_source(timer));
    al_register_event_source(cola_eventos, al_get_keyboard_event_source());

    bool hecho = true;
    int puntos = 0;
    char buffer[20];
    int salida = 3;

    nave jugador;
    Ptrbola bola = NULL; // Aquí ya lo tienes
    Ptrbloque bloques = NULL;
    Ptrenemigo enemigos = NULL;
    int probabilidadEnemigos = 2;
    int maxEnemigosPorNivel = 1;  // Maximum number of enemies per level
    int enemigosGenerados = 0;

    // Calcula la posición para centrar el área de juego en pantalla completa
    int centroX = (pantallaAncho - RX) / 2;
    int centroY = (pantallaAlto - RY) / 2;

    Inicializar_nave(jugador, RX);
    inicializar_bola(bola, jugador, 5);
    formacion_bloques(bloques, nivel, bloque);

    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_flip_display();
    al_rest(1);
    al_start_timer(timer);

    while (hecho) {
        ALLEGRO_EVENT eventos;
        al_wait_for_event(cola_eventos, &eventos);

        if (!quedanBloques(bloques)) {
            nivel++;

            if (maxEnemigosPorNivel > 5) {
                maxEnemigosPorNivel++;
            }

            else {

            }

            al_clear_to_color(al_map_rgb(0, 0, 0));
            if (nivel == 1) {
                al_draw_text(fuente1, al_map_rgb(255, 255, 20), pantallaAncho / 2, pantallaAlto / 2, ALLEGRO_ALIGN_CENTRE, "Nivel 1");
            }
            else {
                al_draw_text(fuente1, al_map_rgb(255, 255, 20), pantallaAncho / 2, pantallaAlto / 2 - 30, ALLEGRO_ALIGN_CENTRE, "¡Nivel Completado!");
                char mensajeNivel[20];
                sprintf(mensajeNivel, "Nivel %d", nivel);
                al_draw_text(fuente1, al_map_rgb(255, 255, 255), pantallaAncho / 2, pantallaAlto / 2 + 30, ALLEGRO_ALIGN_CENTRE, mensajeNivel);
            }

            al_flip_display();
            al_rest(2);

            formacion_bloques(bloques, nivel, bloque);
            
            puntos += 100;
            continue;
        }

        if (eventos.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch (eventos.keyboard.keycode) {

            case ALLEGRO_KEY_ESCAPE:
                hecho = false;
                salida = 0;
                break;

            case ALLEGRO_KEY_LEFT:
                Dir = IZQUIERDA;
                break;

            case ALLEGRO_KEY_RIGHT:
                Dir = DERECHA;
                break;
            }
        }

        // Manejo de teclas liberadas
        if (eventos.type == ALLEGRO_EVENT_KEY_UP) {
            switch (eventos.keyboard.keycode) {
            case ALLEGRO_KEY_LEFT:
                if (Dir == IZQUIERDA)
                    Dir = NINGUNA;
                break;
            case ALLEGRO_KEY_RIGHT:
                if (Dir == DERECHA)
                    Dir = NINGUNA;
                break;
            }
        }

        if (eventos.type == ALLEGRO_EVENT_TIMER) {

            // Baja la probabilidad de aparición de enemigos al 2%
            if (enemigosGenerados < maxEnemigosPorNivel && rand() % 100 < probabilidadEnemigos) {
                generar_enemigos(enemigos, 1, nivel);
                enemigosGenerados++;  // Increment the counter for each generated enemy
            }

            mover_enemigos(enemigos, bloques, centroX, centroY, bloque);


      
            // Movimiento del jugador
            switch (Dir) {

            case IZQUIERDA:
                if (jugador.x >= centroX - RX / 2 - 120) // Límite izquierdo
                    jugador.x -= jugador.velocidadY;
                break;

            case DERECHA:
                if (jugador.x <= centroX + RX / 2 - 300) // Límite derecho
                    jugador.x += jugador.velocidadY;
                break;
            }

            al_clear_to_color(al_map_rgb(0, 0, 0));

            al_draw_bitmap(fondo2, 0, 0, 0);

            // Dibujar fondo centrado sin cambiar su proporción vertical
            al_draw_scaled_bitmap(fondo, 0, 0, al_get_bitmap_width(fondo), al_get_bitmap_height(fondo), centroX, centroY, RX, RY, 0);

            // Ajuste de posición para los bloques y otros elementos
            Ptrbloque tempBloque = bloques;
            while (tempBloque != NULL) {
                if (tempBloque->estado) {

                    al_draw_scaled_bitmap(
                        bloque,                           // Bitmap original del bloque
                        0, 0,                             // Coordenadas origen en la imagen (0, 0 en este caso)
                        al_get_bitmap_width(bloque),      // Ancho de la imagen original
                        al_get_bitmap_height(bloque),     // Alto de la imagen original
                        tempBloque->x + centroX,          // Posición X de destino en pantalla
                        tempBloque->y + centroY,          // Posición Y de destino en pantalla
                        al_get_bitmap_width(bloque) / 2,  // Ancho escalado (ajústalo según necesidad)
                        al_get_bitmap_height(bloque) / 2, // Alto escalado (ajústalo según necesidad)
                        0                                 // Sin banderas de dibujo adicionales
                    );

                    // Dibuja el contorno de la hitbox del bloque para depuración
                    al_draw_rectangle(
                        tempBloque->x + centroX,
                        tempBloque->y + centroY,
                        tempBloque->x + centroX + anchoBloqueEscalado,
                        tempBloque->y + centroY + altoBloqueEscalado,
                        al_map_rgb(255, 0, 0), // Color rojo para la hitbox
                        1 // Grosor de la línea
                    );
                }

                tempBloque = tempBloque->Siguiente;
            }

            // Dibujar enemigos en amarillo y limitar su movimiento al área del fondo
            // Dibujar enemigos en amarillo y limitar su movimiento al área del fondo

            Ptrenemigo tempEnemigo = enemigos;

            while (tempEnemigo != NULL) {

                if (tempEnemigo->estado) {

                    // Rebote en los bordes del fondo
                    if (tempEnemigo->x < centroX + diametro + 15 || tempEnemigo->x > centroX + RX - diametro - 20) {
                        tempEnemigo->velocidadX = -tempEnemigo->velocidadX;
                    }

                    if (tempEnemigo->y < centroY + diametro + 15 || tempEnemigo->y > centroY + RY) {
                        tempEnemigo->velocidadY = -tempEnemigo->velocidadY;
                    }

                    // Dibujar el enemigo en amarillo
                    al_draw_filled_circle(tempEnemigo->x, tempEnemigo->y, diametro / 2, al_map_rgb(255, 255, 0));

                    // Dibuja la hitbox del enemigo como un contorno para depuración
                    al_draw_rectangle(
                        tempEnemigo->x - diametro / 2,            // X inicial (izquierda)
                        tempEnemigo->y - diametro / 2,            // Y inicial (arriba)
                        tempEnemigo->x + diametro / 2,            // X final (derecha)
                        tempEnemigo->y + diametro / 2,            // Y final (abajo)
                        al_map_rgb(255, 0, 0),                    // Color rojo para el contorno de la hitbox
                        1                                         // Grosor del contorno
                    );
                }
                tempEnemigo = tempEnemigo->Siguiente;
            }

            // Actualizar la posición de la bola y manejar colisiones
            colision_bola(bola, bloques, jugador, enemigos, vidas, centroX, centroY, bloque);

            if (bola->estado) { // Verifica si la bola está activa

                // Rebote en los bordes del fondo
                if (bola->x - bola->radio < centroX + 25) { // Límite izquierdo
                    bola->x = centroX + bola->radio + 25; // Ajustar la posición para no salir
                    bola->velocidadX = -bola->velocidadX; // Rebote en el borde izquierdo
                }
                else if (bola->x + bola->radio > centroX + RX - 25) { // Límite derecho
                    bola->x = centroX + RX - bola->radio - 25; // Ajustar la posición para no salir
                    bola->velocidadX = -bola->velocidadX; // Rebote en el borde derecho
                }

                if (bola->y - bola->radio < centroY + 30) { // Límite superior
                    bola->y = centroY + bola->radio + 30; // Ajustar la posición para no salir
                    bola->velocidadY = -bola->velocidadY; // Rebote en el borde superior
                }

                else if (bola->y + bola->radio > centroY + RY) { // Límite inferior
                    vidas--; // Resta una vida si la bola sale por el borde inferior
                    bola->x = jugador.x + centroX; // Reposiciona la bola en la zona centrada
                    bola->y = jugador.y + centroY - 10; // Reposiciona por encima de la nave
                    bola->velocidadY = -fabs(bola->velocidadY); // Rebote hacia arriba
                }

                // Actualizar la posición de la bola
                bola->x += bola->velocidadX;
                bola->y += bola->velocidadY;

                // Dibujar la bola
                al_draw_filled_circle(bola->x, bola->y, bola->radio, al_map_rgb(255, 0, 0)); // Dibuja la bola en rojo

                // Dibuja la hitbox de la bola como un contorno para depuración
                al_draw_circle(bola->x, bola->y, bola->radio, al_map_rgb(255, 0, 0), 1); // Contorno rojo para la hitbox
            }

            


    

            dibujar_nave(jugador, nave2, centroX, centroY);

            // Mostrar estadísticas en la esquina superior derecha de la pantalla completa
            sprintf(buffer, "Puntos: %d", puntos);
            al_draw_text(fuente1, al_map_rgb(255, 255, 255), pantallaAncho - 100, 10, ALLEGRO_ALIGN_RIGHT, buffer);

            sprintf(buffer, "Vidas: %d", vidas);
            al_draw_text(fuente1, al_map_rgb(255, 255, 255), pantallaAncho - 100, 50, ALLEGRO_ALIGN_RIGHT, buffer);

            al_flip_display();
        }
    }

    // Limpiar recursos al terminar el juego
    al_destroy_timer(timer);
    al_destroy_font(fuente1);
    al_destroy_font(fuente2);
    al_destroy_display(pantalla);
    al_destroy_bitmap(bloque);
    al_destroy_bitmap(fondo);
    al_destroy_event_queue(cola_eventos);

    return salida;
}