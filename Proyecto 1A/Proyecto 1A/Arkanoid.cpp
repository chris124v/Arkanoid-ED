#include <stdio.h>
#include <iostream>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include "Juego.h"

using namespace std;
#pragma warning(disable:4996);
#define FPS 60.0

void main()
{
    if (!al_init()) {
        al_show_native_message_box(NULL, "Ventana Emergente", "Error", "No se puede inicializar la Practica de Allegro", NULL, NULL);
        return;
    }

    al_init_font_addon();
    al_init_ttf_addon();
    al_init_image_addon();
    al_init_primitives_addon();
    al_install_keyboard();
    al_install_mouse();

    ALLEGRO_MONITOR_INFO monitor;
    al_get_monitor_info(0, &monitor);
    const int RX = monitor.x2 - monitor.x1;
    const int RY = monitor.y2 - monitor.y1;

    int mousex = 0;
    int mousey = 0;

    ALLEGRO_DISPLAY* pantalla = al_create_display(RX, RY);
    al_set_display_flag(pantalla, ALLEGRO_FULLSCREEN, true);

    if (!pantalla) {
        al_show_native_message_box(NULL, "Error", "Error", "No se pudo crear la ventana", NULL, NULL);
        return;
    }

    ALLEGRO_EVENT_QUEUE* cola_eventos = al_create_event_queue();
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);
    ALLEGRO_TIMER* timer2 = al_create_timer(1.0 / FPS);

    ALLEGRO_BITMAP* Fondo = al_load_bitmap("Imagenes/fondo_main.jpg");
    ALLEGRO_BITMAP* Logo = al_load_bitmap("Imagenes/arkanoid.png");

    if (!Fondo) {
        al_show_native_message_box(NULL, "Error", "Error", "No se pudo cargar el fondo", NULL, NULL);
        return;
    }
    if (!Logo) {
        al_show_native_message_box(NULL, "Error", "Error", "No se pudo cargar el logo", NULL, NULL);
        return;
    }

    ALLEGRO_FONT* font = al_load_ttf_font("Video-Font.ttf", 40, NULL);
    ALLEGRO_FONT* font2 = al_load_ttf_font("Video-Font.ttf", 25, NULL);

    al_clear_to_color(al_map_rgb(0, 0, 0));

    al_register_event_source(cola_eventos, al_get_timer_event_source(timer));
    al_register_event_source(cola_eventos, al_get_timer_event_source(timer2));
    al_register_event_source(cola_eventos, al_get_display_event_source(pantalla));
    al_register_event_source(cola_eventos, al_get_keyboard_event_source());
    al_register_event_source(cola_eventos, al_get_mouse_event_source());

    bool creacion = true;
    int X = al_get_display_width(pantalla);
    int Y = al_get_display_height(pantalla);

    bool hecho = true;
    int a = 0;
    int inicio = 0;
    bool modo;

    al_start_timer(timer);
    al_start_timer(timer2);

    while (creacion) {
        ALLEGRO_EVENT eventos;
        al_wait_for_event(cola_eventos, &eventos);

        if (eventos.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            creacion = false;
        }

        if (eventos.type == ALLEGRO_EVENT_MOUSE_AXES) {
            mousex = eventos.mouse.x;
            mousey = eventos.mouse.y;
        }

        if (eventos.type == ALLEGRO_EVENT_TIMER) {
            if (eventos.timer.source == timer) {
                al_clear_to_color(al_map_rgb(0, 0, 0));

                float scale = 1.3;
                int ancho = al_get_bitmap_width(Logo) * scale;
                int alto = al_get_bitmap_height(Logo) * scale;
                int logoX = (X - ancho) / 2;
                int logoY = Y / 15;

                al_draw_bitmap(Fondo, 0, 0, 0);
                al_draw_scaled_bitmap(Logo, 0, 0, al_get_bitmap_width(Logo), al_get_bitmap_height(Logo), logoX, logoY, ancho, alto, 0);

                al_draw_text(font, al_map_rgb(255, 255, 255), X / 2, (Y * (250.0 / 720.0)), ALLEGRO_ALIGN_CENTER, "JUGAR");
                al_draw_text(font, al_map_rgb(255, 255, 255), X / 2, (Y * (325.0 / 720.0)), ALLEGRO_ALIGN_CENTER, "REGLAS");
                al_draw_text(font, al_map_rgb(255, 255, 255), X / 2, (Y * (390.0 / 720.0)), ALLEGRO_ALIGN_CENTER, "RESULTADOS");
                al_draw_text(font, al_map_rgb(255, 255, 255), X / 2, (Y * (470.0 / 720.0)), ALLEGRO_ALIGN_CENTER, "SALIR");
                al_draw_text(font2, al_map_rgb(255, 255, 255), 50, Y - al_get_font_line_height(font2) - 50, ALLEGRO_ALIGN_LEFT, "by Santiago and Christopher");
                al_flip_display();
            }
        }

        if ((mousex >= X / 2 - 42 && mousex <= X / 2 + 42) && (mousey >= (Y * 255.0 / 720.0) && mousey <= (Y * 290.0 / 720.0))) {
            al_draw_text(font, al_map_rgb(0, 255, 255), X / 2, (Y * (250.0 / 720.0)), ALLEGRO_ALIGN_CENTRE, "JUGAR");

            if (eventos.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
                if (eventos.mouse.button & 1) {
                    al_destroy_display(pantalla);
                    int vida = 3;
                    int nivel = 1;
                    modo = true;

                    while (vida != 0) {
                        vida = arkanoid(nivel, vida);
                        nivel = nivel + 1;
                    }

                    main();
                    hecho = false;
                }
            }
        }

        if ((mousex >= X / 2 - 42 && mousex <= X / 2 + 42) && (mousey >= (Y * 475.0 / 720.0) && mousey <= (Y * 530.0 / 720.0))) {
            al_draw_text(font, al_map_rgb(0, 255, 255), X / 2, (Y * (470.0 / 720.0)), ALLEGRO_ALIGN_CENTRE, "SALIR");
            if (eventos.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
                if (eventos.mouse.button & 1) {
                    creacion = false;
                }
            }
        }

        if (eventos.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch (eventos.keyboard.keycode) {
            case ALLEGRO_KEY_ESCAPE:
                creacion = false;
            }
        }
        al_flip_display();
    }

    al_destroy_bitmap(Fondo);
    al_destroy_bitmap(Logo);
    al_destroy_font(font);
    al_destroy_font(font2);
    al_destroy_timer(timer);
    al_destroy_timer(timer2);
    al_destroy_event_queue(cola_eventos);
    al_destroy_display(pantalla);
}
