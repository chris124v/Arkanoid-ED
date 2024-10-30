//Archivo de tipo.h del juego para probar el juego

#pragma once

#include "FuncionesJuego.h"

using namespace std;
#pragma warning(disable:4996);
#define FPS 60.0

int arkanoid(int nivel, int vidas) {
    ALLEGRO_MONITOR_INFO monitor;
    al_get_monitor_info(0, &monitor);
    const int RX = monitor.x2 - monitor.x1;
    const int RY = monitor.y2 - monitor.y1;

    al_set_new_display_flags(ALLEGRO_WINDOWED | ALLEGRO_RESIZABLE);
    ALLEGRO_DISPLAY* pantalla = al_create_display(RX, RY);
    al_set_window_title(pantalla, "Arkanoid");

    if (!pantalla) {
        al_show_native_message_box(NULL, "Ventana Emergente", "Error", "No se puede crear la pantalla", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return 0;
    }

    ALLEGRO_FONT* fuente1;
    ALLEGRO_FONT* fuente2;
    ALLEGRO_KEYBOARD_STATE teclado;
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);

    ALLEGRO_EVENT_QUEUE* cola_eventos = al_create_event_queue();
    ALLEGRO_BITMAP* bloque = al_load_bitmap("Imagenes/bloque.png");
    ALLEGRO_BITMAP* fondo = al_load_bitmap("Imagenes/fondo.png");

    ALLEGRO_SAMPLE* musica = al_load_sample("Musica/musica.wav");
    ALLEGRO_SAMPLE* rebote = al_load_sample("Musica/rebote.wav");
    ALLEGRO_SAMPLE* romper_bloque = al_load_sample("Musica/romper_bloque.wav");

    fuente1 = al_load_font("pixel.ttf", 40, NULL);
    fuente2 = al_load_font("pixel.ttf", 30, NULL);

    al_register_event_source(cola_eventos, al_get_timer_event_source(timer));
    al_register_event_source(cola_eventos, al_get_keyboard_event_source());

    bool hecho = true;
    bool dibujar = true;
    int puntos = 0;
    char buffer[20];
    int salida = 3;

    nave jugador;
    Ptrbola bola = new struct bola;
    Ptrbloque bloques = NULL;
    Ptrenemigo enemigos = NULL;  // Lista de enemigos
    int probabilidadEnemigos = 5; // Probabilidad de aparición (0-100)

    // Inicialización de la nave, bola, y bloques
    Inicializar_nave(jugador, RX);
    inicializar_bola(bola, jugador, 5);
    formacion_bloques(bloques, nivel);

    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_flip_display();
    al_rest(1);

    al_play_sample(musica, 0.3, 0, 1, ALLEGRO_PLAYMODE_LOOP, NULL);

    // Iniciar temporizador
    al_start_timer(timer);

    while (hecho) {
        ALLEGRO_EVENT eventos;
        al_wait_for_event(cola_eventos, &eventos);

        // Verifica si todos los bloques fueron destruidos y pasa al siguiente nivel
        if (!quedanBloques(bloques)) {
            nivel++;

            // Mensaje de transición de nivel
            al_clear_to_color(al_map_rgb(0, 0, 0));
            if (nivel == 1) {
                al_draw_text(fuente1, al_map_rgb(255, 255, 20), RX / 2, RY / 2, ALLEGRO_ALIGN_CENTRE, "Nivel 1");
            }
            else {
                al_draw_text(fuente1, al_map_rgb(255, 255, 20), RX / 2, RY / 2 - 30, ALLEGRO_ALIGN_CENTRE, "¡Nivel Completado!");
                char mensajeNivel[20];
                sprintf(mensajeNivel, "Nivel %d", nivel);
                al_draw_text(fuente1, al_map_rgb(255, 255, 255), RX / 2, RY / 2 + 30, ALLEGRO_ALIGN_CENTRE, mensajeNivel);
            }
            al_flip_display();
            al_rest(2); // Pausa de 2 segundos

            // Inicializa bloques y bola para el nuevo nivel
            formacion_bloques(bloques, nivel);
            inicializar_bola(bola, jugador, 5);
            puntos += 100;
            continue;
        }

        // Control de la nave con teclado
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

        // Eventos del temporizador para mover bola y verificar colisiones
        if (eventos.type == ALLEGRO_EVENT_TIMER) {
            // Generación aleatoria de enemigos
            if (rand() % 100 < probabilidadEnemigos) {
                generar_enemigos(enemigos, 1); // Genera un enemigo con baja probabilidad
            }

            // Mover la bola, verificar colisiones y mover enemigos
            mover_enemigos(enemigos);
            colision_bola(*bola, bloques, jugador, enemigos, vidas);

            if (vidas == 0) {
                salida = 0;
                hecho = false;
            }

            // Dibuja todos los elementos en pantalla
            if (dibujar && al_is_event_queue_empty(cola_eventos)) {
                al_clear_to_color(al_map_rgb(0, 0, 0));
                al_draw_bitmap(fondo, 0, 0, 0);

                // Dibujar bloques
                Ptrbloque tempBloque = bloques;
                while (tempBloque != NULL) {
                    if (tempBloque->estado) {
                        al_draw_bitmap(bloque, tempBloque->x, tempBloque->y, 0);
                    }
                    tempBloque = tempBloque->Siguiente;
                }

                // Dibujar enemigos
                Ptrenemigo tempEnemigo = enemigos;
                while (tempEnemigo != NULL) {
                    if (tempEnemigo->estado) {
                        al_draw_filled_circle(tempEnemigo->x, tempEnemigo->y, diametro / 2, al_map_rgb(255, 0, 0)); // Ejemplo de representación
                    }
                    tempEnemigo = tempEnemigo->Siguiente;
                }

                // Dibujar la nave
                al_draw_filled_rectangle(jugador.x, jugador.y, jugador.x + 80, jugador.y + 20, al_map_rgb(255, 255, 255));

                // Dibujar la bola
                al_draw_filled_circle(bola->x, bola->y, bola->radio, al_map_rgb(255, 0, 0));

                // Dibuja estadísticas
                sprintf(buffer, "Puntos: %d", puntos);
                al_draw_text(fuente1, al_map_rgb(255, 255, 255), 10, 10, 0, buffer);

                sprintf(buffer, "Vidas: %d", vidas);
                al_draw_text(fuente1, al_map_rgb(255, 255, 255), RX - 100, 10, 0, buffer);

                al_flip_display();
                dibujar = false;
            }
        }
    }

    // Si el jugador pierde, muestra "Fin del juego"
    if (salida == 0) {
        int continuar = true;
        char nombre[40];
        int pos = 0;

        while (continuar) {
            ALLEGRO_EVENT eventoos;
            al_wait_for_event(cola_eventos, &eventoos);
            al_clear_to_color(al_map_rgb(0, 0, 0));

            if (eventoos.type == ALLEGRO_EVENT_KEY_CHAR) {
                if (pos < 40) {
                    if (eventoos.keyboard.keycode == ALLEGRO_KEY_BACKSPACE && pos > 0) {
                        nombre[--pos] = '\0';
                    }
                    else if (eventoos.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                        continuar = false;
                    }
                    else if (eventoos.keyboard.keycode != ALLEGRO_KEY_ESCAPE) {
                        nombre[pos++] = eventoos.keyboard.unichar;
                    }
                }
            }

            nombre[pos] = '|';
            al_draw_text(fuente1, al_map_rgb(255, 255, 20), RX / 2, RY / 2 - 50, ALLEGRO_ALIGN_CENTRE, "FIN DEL JUEGO");
            al_draw_text(fuente2, al_map_rgb(255, 255, 255), RX / 2, RY / 2, ALLEGRO_ALIGN_CENTRE, nombre);
            al_draw_text(fuente2, al_map_rgb(255, 255, 255), RX / 2, RY / 2 + 100, ALLEGRO_ALIGN_CENTRE, "Presione Enter para finalizar");
            al_flip_display();
        }

        // Guardar puntaje
        if (pos > 0) {
            nombre[pos] = '\0';
        }
        else {
            strcpy(nombre, "Jugador");
        }
        CrearArchivo(buffer, nombre);
    }
    else {
        // Si no perdió, permite continuar
        int seguir = true;
        while (seguir) {
            ALLEGRO_EVENT eventoos;
            al_wait_for_event(cola_eventos, &eventoos);
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_text(fuente1, al_map_rgb(255, 255, 20), RX / 2, RY / 2, ALLEGRO_ALIGN_CENTRE, "Presione Enter para continuar");
            if (eventoos.type == ALLEGRO_EVENT_KEY_UP && eventoos.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                seguir = false;
            }
            al_flip_display();
        }
    }

    // Destruir recursos de Allegro
    al_destroy_sample(musica);
    al_destroy_timer(timer);
    al_destroy_font(fuente1);
    al_destroy_font(fuente2);
    al_destroy_display(pantalla);
    al_destroy_bitmap(bloque);
    al_destroy_bitmap(fondo);
    al_destroy_event_queue(cola_eventos);

    return salida;
}
