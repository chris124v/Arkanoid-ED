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
    const int RX = 700; // Ancho fijo del área de juego
    const int RY = 800; // Altura fija del área de juego

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

    ALLEGRO_BITMAP* bloque = al_load_bitmap("Imagenes/Gris_1.png");
    ALLEGRO_BITMAP* fondo = al_load_bitmap("Imagenes/fondo_juego.png");
    if (!bloque || !fondo) {
        al_show_native_message_box(pantalla, "Error", "Carga de Imagen", "No se pudo cargar una o más imágenes.", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return 0;
    }

    ALLEGRO_EVENT_QUEUE* cola_eventos = al_create_event_queue();
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);
    al_register_event_source(cola_eventos, al_get_timer_event_source(timer));
    al_register_event_source(cola_eventos, al_get_keyboard_event_source());

    bool hecho = true;
    int puntos = 0;
    char buffer[20];
    int salida = 3;

    nave jugador;
    Ptrbola bola = new struct bola;
    Ptrbloque bloques = NULL;
    Ptrenemigo enemigos = NULL;
    int probabilidadEnemigos = 2;

    // Calcula la posición para centrar el área de juego en pantalla completa
    int centroX = (pantallaAncho - RX) / 2;
    int centroY = (pantallaAlto - RY) / 2;

    Inicializar_nave(jugador, RX);
    inicializar_bola(bola, jugador, 5);
    formacion_bloques(bloques, nivel);

    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_flip_display();
    al_rest(1);
    al_start_timer(timer);

    while (hecho) {
        ALLEGRO_EVENT eventos;
        al_wait_for_event(cola_eventos, &eventos);

        if (!quedanBloques(bloques)) {
            nivel++;
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

            formacion_bloques(bloques, nivel);
            inicializar_bola(bola, jugador, 5);
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
                jugador.x -= jugador.velocidadY;
                break;
            case ALLEGRO_KEY_RIGHT:
                jugador.x += jugador.velocidadY;
                break;
            }
        }

        if (eventos.type == ALLEGRO_EVENT_TIMER) {
            // Baja la probabilidad de aparición de enemigos al 2%
            if (rand() % 100 < probabilidadEnemigos) {
                generar_enemigos(enemigos, 1);
            }
            mover_enemigos(enemigos, centroX, centroY);

            // Ajusta la colisión de la bola para rebotar en los bordes del área del fondo centrado
            if (bola->x - bola->radio <= centroX || bola->x + bola->radio >= centroX + RX) {
                bola->velocidadX = -bola->velocidadX; // Rebote en los bordes laterales
            }
            if (bola->y - bola->radio <= centroY || bola->y + bola->radio >= centroY + RY) {
                bola->velocidadY = -bola->velocidadY; // Rebote en los bordes superior e inferior
            }

            colision_bola(*bola, bloques, jugador, enemigos, vidas, centroX, centroY);

            if (vidas == 0) {
                salida = 0;
                hecho = false;
            }

            al_clear_to_color(al_map_rgb(0, 0, 0));

            // Dibujar fondo centrado sin cambiar su proporción vertical
            al_draw_scaled_bitmap(fondo, 0, 0, al_get_bitmap_width(fondo), al_get_bitmap_height(fondo), centroX, centroY, RX, RY, 0);

            // Ajuste de posición para los bloques y otros elementos
            Ptrbloque tempBloque = bloques;
            while (tempBloque != NULL) {
                if (tempBloque->estado) {
                    al_draw_bitmap(bloque, tempBloque->x + centroX, tempBloque->y + centroY, 0);
                }
                tempBloque = tempBloque->Siguiente;
            }

            // Dibujar enemigos en amarillo y limitar su movimiento al área del fondo
            Ptrenemigo tempEnemigo = enemigos;
            while (tempEnemigo != NULL) {
                if (tempEnemigo->estado) {
                    // Rebote en los bordes del fondo
                    if (tempEnemigo->x < centroX || tempEnemigo->x > centroX + RX - diametro) {
                        tempEnemigo->velocidadX = -tempEnemigo->velocidadX;
                    }
                    if (tempEnemigo->y < centroY || tempEnemigo->y > centroY + RY - diametro) {
                        tempEnemigo->velocidadY = -tempEnemigo->velocidadY;
                    }

                    // Dibujar el enemigo en amarillo
                    al_draw_filled_circle(tempEnemigo->x, tempEnemigo->y, diametro / 2, al_map_rgb(255, 255, 0));
                }
                tempEnemigo = tempEnemigo->Siguiente;
            }

            // Dibujar la nave
            al_draw_filled_rectangle(jugador.x + centroX, jugador.y + centroY, jugador.x + centroX + 80, jugador.y + centroY + 20, al_map_rgb(255, 255, 255));

            // Dibujar la bola en rojo
            al_draw_filled_circle(bola->x + centroX, bola->y + centroY, bola->radio, al_map_rgb(255, 0, 0));

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

