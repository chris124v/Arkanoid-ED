//Arkanoid Main 

//Christopher Daniel Vargas Villalta, Carnet: 2024108443
//Santiago Espinoza Rendon, Carnet: 2024156530

//Llamamos a las librerias de allegro 
#include <stdio.h>
#include <iostream>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

//Llamamos al .h del juego
#include "Juego.h"

//Usamos esto para evitar errores y definir los fps
using namespace std;
#pragma warning(disable:4996);
#define FPS 60.0

//Variables de la altura y ancho de la pantalla despegable 
int alturaescalada = 1080;
int anchoescalado = 1920;

//Main principal del juego
void main()

{   
    //Mensaje en caso de que no se pueda inicializar el juego
    if (!al_init()) {
        al_show_native_message_box(NULL, "Ventana Emergente", "Error", "No se puede inicializar la Practica de Allegro", NULL, NULL);
        return;
    }

    //Iniciamos los addon propios de allegro 
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_image_addon();
    al_init_primitives_addon();
    al_install_keyboard();
    al_install_mouse();

    //Obtenemos la informacion del monitor donde se esta corriendo el juego
    ALLEGRO_MONITOR_INFO monitor;
    al_get_monitor_info(0, &monitor);
    const int RX = monitor.x2 - monitor.x1;
    const int RY = monitor.y2 - monitor.y1;

    //Inicializamos el mouse en la posicion 0
    int mousex = 0;
    int mousey = 0;

    //Creamos la pantalla en base a las dimensiones del monitior
    ALLEGRO_DISPLAY* pantalla = al_create_display(RX, RY);
    al_set_display_flag(pantalla, ALLEGRO_FULLSCREEN, true);

    //Mensaje en caso de que la pantalla no se pueda desplegar
    if (!pantalla) {
        al_show_native_message_box(NULL, "Error", "Error", "No se pudo crear la ventana", NULL, NULL);
        return;
    }

    //Cola de eventos las diversas acciones que se realizaen en el juego
    ALLEGRO_EVENT_QUEUE* cola_eventos = al_create_event_queue();

    //Timers para los frames y su actualizacion
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);
    ALLEGRO_TIMER* timer2 = al_create_timer(1.0 / FPS);
    
    //Llamamos al fondo y al logo de arkanoid
    ALLEGRO_BITMAP* Fondo = al_load_bitmap("Imagenes/fondo_main.jpg");
    ALLEGRO_BITMAP* Logo = al_load_bitmap("Imagenes/arkanoid.png");
    ALLEGRO_BITMAP* Flecha = al_load_bitmap("Imagenes/Flecha.png");

    //Estos dos if son en caso de que no se encuentren las imagenes
    if (!Fondo) {
        al_show_native_message_box(NULL, "Error", "Error", "No se pudo cargar el fondo", NULL, NULL);
        return;
    }
    if (!Logo || !Flecha) {
        al_show_native_message_box(NULL, "Error", "Error", "No se pudo cargar el logo", NULL, NULL);
        return;
    }

    //Establecemos dos tipos de fuentes de texto con diferentes tamanos 
    ALLEGRO_FONT* font = al_load_ttf_font("Video-Font.ttf", 40, NULL);
    ALLEGRO_FONT* font2 = al_load_ttf_font("Video-Font.ttf", 25, NULL);
    ALLEGRO_FONT* font3 = al_load_ttf_font("Video-Font.ttf", 20, NULL);

    //Limpiamos la pantalla
    al_clear_to_color(al_map_rgb(0, 0, 0));

    //Registramos los eventos de la pantalla, timers, teclado y mouse
    al_register_event_source(cola_eventos, al_get_timer_event_source(timer));
    al_register_event_source(cola_eventos, al_get_timer_event_source(timer2));
    al_register_event_source(cola_eventos, al_get_display_event_source(pantalla));
    al_register_event_source(cola_eventos, al_get_keyboard_event_source());
    al_register_event_source(cola_eventos, al_get_mouse_event_source());

    //Establecemos un booleano para la aparicion del main
    bool creacion = true;

    //Obtenemos el alto y ancho de la pantalla en la que estamos
    int X = al_get_display_width(pantalla);
    int Y = al_get_display_height(pantalla);

    //Establecemos variables para cuando inicializemos el juego 
    bool hecho = true;
    int inicio = 0;
    

    //Inicializamos los timers
    al_start_timer(timer);
    al_start_timer(timer2);

    //Ciclo while que permite la creacion del menu principal
    while (creacion) {

        //Establecemos los eventos
        ALLEGRO_EVENT eventos;

        //Esperamos por los eventos
        al_wait_for_event(cola_eventos, &eventos);

        //En caso de que se cierre la pantalla se cierra el juego 
        if (eventos.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            creacion = false;
        }

        //Establecemos en la cola de eventos la existencia del mouse
        if (eventos.type == ALLEGRO_EVENT_MOUSE_AXES) {
            mousex = eventos.mouse.x;
            mousey = eventos.mouse.y;
        }

        //Inicializamos los timers para el fondo y demas aspectos 
        if (eventos.type == ALLEGRO_EVENT_TIMER) {

            //Si se verifica el timer
            if (eventos.timer.source == timer) {

                //Establecemos primero la pantalla en negro
                al_clear_to_color(al_map_rgb(0, 0, 0));

                //Llamamos al fondo y que se escale en base a la pantalla 
                al_draw_scaled_bitmap(Fondo, 0, 0, anchoescalado, alturaescalada, 0, 0, RX, RY, 0);

                // Dimensiones normales del logo
                int logo_ancho = al_get_bitmap_width(Logo);
                int logo_alto = al_get_bitmap_height(Logo);

                // Acabo vamos a establecer un tamano maximo para el logo
                float tanmax = 0.3; 
                float maxi_ancho= X * tanmax;
                float maxi_altura = Y * tanmax;

                // Aqui tomamos en cuenta que tan grande debe ser basandose en la pantalla
                float tamano_normal_x = maxi_ancho / logo_ancho;
                float tamano_normal_y = maxi_altura / logo_alto;

                // Aqui mantenemos la proporcion minima para que no sea enorme
                float proporcion = min(tamano_normal_x, tamano_normal_y);

                // Finalmente obtenemos el tamano nuevo segun la proporcion y el tamano real
                int ancho_escalado = (int)(logo_ancho * proporcion);
                int alto_escalado = (int)(logo_alto * proporcion);

                // Posicionar el logo centrado en la pantalla
                int logoX = (X - ancho_escalado) / 2; 
                int logoY = (Y - alto_escalado) / 15; 

                // Hacemos el bitmap tomando en ceunta todos los factores
                al_draw_scaled_bitmap(Logo, 0, 0, logo_ancho, logo_alto, logoX, logoY, ancho_escalado, alto_escalado, 0);

                //Imprimimos el texto para las diferentes opciones en el menu principal
                al_draw_text(font, al_map_rgb(255, 255, 255), X / 2, (Y * (250.0 / 720.0)), ALLEGRO_ALIGN_CENTER, "JUGAR");
                al_draw_text(font, al_map_rgb(255, 255, 255), X / 2, (Y * (325.0 / 720.0)), ALLEGRO_ALIGN_CENTER, "REGLAS");
                al_draw_text(font, al_map_rgb(255, 255, 255), X / 2, (Y * (390.0 / 720.0)), ALLEGRO_ALIGN_CENTER, "RESULTADOS");
                al_draw_text(font, al_map_rgb(255, 255, 255), X / 2, (Y * (470.0 / 720.0)), ALLEGRO_ALIGN_CENTER, "SALIR");
                al_draw_text(font2, al_map_rgb(255, 255, 255), 50, Y - al_get_font_line_height(font2) - 50, ALLEGRO_ALIGN_LEFT, "by Santiago and Christopher");

                //Hacemos el flip display para actualizar la pantalla
                al_flip_display();
            }
        }

        //Dependiendo de la posicion del mouse si se posiciona justo encima de jugar cambia el color a cyan y si lo presiona inicializa el juego
        if ((mousex >= X / 2 - 42 && mousex <= X / 2 + 42) && (mousey >= (Y * 255.0 / 720.0) && mousey <= (Y * 290.0 / 720.0))) {

            //Aqui cambiamos el color del texto
            al_draw_text(font, al_map_rgb(0, 255, 255), X / 2, (Y * (250.0 / 720.0)), ALLEGRO_ALIGN_CENTRE, "JUGAR");

            //Aqui en caso de que se presione el mouse
            if (eventos.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {

                //Si se presiona el mouse o "1"
                if (eventos.mouse.button & 1) {

                    //Cerramos el menu principal
                    al_destroy_display(pantalla);

                    //Establecemos las vidas del jugador y el nivel inicial
                    int vida = 3;
                    int nivel = 1;
                    
                    //Mientras las vidas no lleguen a 0
                    while (vida != 0) {

                        //Inicializa el juego con el metodo arkanoid que es el juego en si
                        vida = arkanoid(nivel, vida);

                        //Establece el cambio de nivel
                        nivel = nivel + 1;
                    }

                    //Llamamos nuevamente al main en caso de que se pierdan todas las vidas
                    main();

                    //Hecho ahora como false si se sale 
                    hecho = false;
                }
            }
        }

        //Aqui hacemos lo mismo con instrucciones en caso de que se quiera ver las instrucciones de otros jugadores
        if ((mousex >= X / 2 - 42 && mousex <= X / 2 + 42) && (mousey >= (Y * 330.0 / 720.0) && mousey <= (Y * 390.0 / 720.0))) {

            //Cambiamos el color a cyan
            al_draw_text(font, al_map_rgb(0, 255, 255), X / 2, (Y * (325.0 / 720.0)), ALLEGRO_ALIGN_CENTRE, "REGLAS");

            //Si se presiona salir creacion pasa a ser false saliendo de la interfaz
            if (eventos.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {

                if (eventos.mouse.button & 1) {

                    
                    bool salir = false;

                    //Aqui el ciclo se seguira cumpliendo
                    while (!salir) {

                        al_wait_for_event(cola_eventos, &eventos);
                        if (eventos.type == ALLEGRO_EVENT_TIMER) {
                            
                                //Se dibujan todos los bitmaps, las instrucciones y títulos
                                al_clear_to_color(al_map_rgb(0, 0, 0));
                                al_draw_scaled_bitmap(Fondo, 0, 0, anchoescalado, alturaescalada, 0, 0, RX, RY, 0);
                                al_draw_text(font, al_map_rgb(0, 255, 255), X / 2, (RY * (120.0 / 768.0)), ALLEGRO_ALIGN_CENTRE, "Arkanoid");
                                al_draw_text(font2, al_map_rgb(250, 250, 250), X / 2, (RY * (170.0 / 768.0)), ALLEGRO_ALIGN_CENTRE, "Instrucciones");
                                al_draw_text(font2, al_map_rgb(250, 250, 250), X / 2, (RY* (260.0 / 768.0)), ALLEGRO_ALIGN_CENTRE, "En el juego arkanoid tienes un total de 3 vidas disponibles usalas con sabiduria.");
                                al_draw_text(font2, al_map_rgb(250, 250, 250), X / 2, (RY* (310.0 / 768.0)), ALLEGRO_ALIGN_CENTRE, "Avanzas si logras destruir todos los bloques de un nivel, hay un total de 10 niveles disponibles.");
                                al_draw_text(font2, al_map_rgb(250, 250, 250), X / 2, (RY* (360.0 / 768.0)), ALLEGRO_ALIGN_CENTRE, "Mueres si pierdes todas tus vidas esto se da solo si la bola cruza el limite inferior.");
                                al_draw_text(font2, al_map_rgb(250, 250, 250), X / 2, (RY* (410.0 / 768.0)), ALLEGRO_ALIGN_CENTRE, "Utiliza las flechas para mover la nave (barra) <- y ->");
                                al_draw_text(font2, al_map_rgb(250, 250, 250), X / 2, (RY* (460.0 / 768.0)), ALLEGRO_ALIGN_CENTRE, "PuntosxNivel= 100, PuntosxEnemigo = 10, PuntosxBloque = 5");
                                al_draw_text(font2, al_map_rgb(250, 250, 250), X / 2, (RY * (570.0 / 768.0)), ALLEGRO_ALIGN_CENTRE, "Atras");
                            
                        }

                        //Aqui se realiza el evento que determina la posicion del mouse y se activa cuando el mismo se mueve
                        if (eventos.type == ALLEGRO_EVENT_MOUSE_AXES)
                        {
                            mousex = eventos.mouse.x;
                            mousey = eventos.mouse.y;
                        }

                        //En este caso basandose en la posicion del rectangulo de la opcion atras hace la comparacion
                        if ((mousex >= X / 2 - 42 && mousex <= X / 2 + 42) && (mousey >= (RY * (570.0 / 768.0)) && mousey <= (RY * (620.0 / 768.0)))) {
                            
                            //Se subraya el texto con el color cyan
                            al_draw_text(font2, al_map_rgb(0, 255, 255), X / 2, (Y* (570.0 / 768.0)), ALLEGRO_ALIGN_CENTRE, "Atras");

                            //Si se presiona atras sale al menú principal
                            if (eventos.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
                            {
                                //Esto basicamente lo que indica es que como salir antes era false se invalida el ciclo while
                                if (eventos.mouse.button & 1) {
                                    salir = true;
                                }
                            }
                        }

                        //Esto tambien en caso que se quiera salir con escape en lugar de salir
                        if (eventos.type == ALLEGRO_EVENT_KEY_DOWN) {

                            //Se hace el switch con el teclado 
                            switch (eventos.keyboard.keycode) {

                                //Si se usa escape se sale de juego
                            case ALLEGRO_KEY_ESCAPE:

                                creacion = false;
                            }
                        }

                        //Mediante flip display lo que se logra es hacer el cambio al menu principal con el 
                        al_flip_display();
                    }

                }
            }
        }


        //Aqui hacemos lo mismo con resultados en caso de que se quiera ver los resultados de otros jugadores
        if ((mousex >= X / 2 - 42 && mousex <= X / 2 + 42) && (mousey >= (Y * 395.0 / 720.0) && mousey <= (Y * 450.0 / 720.0))) {

            //Cambiamos el color a cyan
            al_draw_text(font, al_map_rgb(0, 255, 255), X / 2, (Y * (390.0 / 720.0)), ALLEGRO_ALIGN_CENTRE, "RESULTADOS");

            //Si se presiona salir creacion pasa a ser false saliendo de la interfaz
            if (eventos.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
                if (eventos.mouse.button & 1) {

              
                    //Salimos del ciclo while
                    bool salir = false;
                    
                    //Si se cumple que salir es true
                    while (!salir) {

                        //Iniciamos la cola de eventos
                        al_wait_for_event(cola_eventos, &eventos);

                        //En caso de que se inicialice el timer
                        if (eventos.type == ALLEGRO_EVENT_TIMER) {

                            //Verificamos que sea del tipo timer
                            if (eventos.timer.source == timer) {

                                //Creamos fonod negro
                                al_clear_to_color(al_map_rgb(0, 0, 0));

                                //Llamamos al fondo y que se escale en base a la pantalla 
                                al_draw_scaled_bitmap(Fondo, 0, 0, anchoescalado, alturaescalada, 0, 0, RX, RY, 0);

                                //Se dibujan los textos pertinents para el submenu
                                al_draw_text(font, al_map_rgb(0, 255, 255), X / 2, (RY * (100.0 / 768.0)), ALLEGRO_ALIGN_CENTRE, "Arkanoid");
                                al_draw_text(font, al_map_rgb(250, 250, 250), X / 2, (RY * (200.0 / 768.0)), ALLEGRO_ALIGN_CENTRE, "RESULTADOS");

                                //Estas serian las flechas para ver los resultados
                                al_draw_bitmap(Flecha, X / 2 - 200, (RY * (300.0 / 768.0)), NULL);
                                al_draw_bitmap(Flecha, X / 2 - 200, (RY * (500 / 768.0)), ALLEGRO_FLIP_VERTICAL);

                                //Esta funcion permite guardar los resultados de otro jugadores
                                CargarArchivo(X, Y, font3, inicio);

                                //Establecemos un texto para devolvernos
                                al_draw_text(font2, al_map_rgb(250, 250, 250), X / 2 - 300, (RY * (600.0 / 768.0)), ALLEGRO_ALIGN_CENTRE, "Atras");
                                al_flip_display();
                            }
                        }

                        //Se invoca nuevamente al movimiento del mouse 
                        if (eventos.type == ALLEGRO_EVENT_MOUSE_AXES)
                        {
                            mousex = eventos.mouse.x;
                            mousey = eventos.mouse.y;
                        }

                        //Si posicionamos el mouse en la flecha de arriba
                        if ((mousex >= X / 2 - 200 && mousex <= X / 2 - 155) && (mousey >= 380 && mousey <= 490)) {

                            //Pintamos la imagen de la flecha en color cyan
                            al_draw_tinted_bitmap(Flecha, al_map_rgb(0, 255, 255), X / 2 - 200, (RY * (300.0 / 768.0)), NULL);

                            //Movimiento de las flechas para subir
                            if (eventos.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
                            {
                                if (eventos.mouse.button & 1) {
                                    if (inicio >= 1) {
                                        inicio = inicio - 1;
                                    }
                                }
                            }
                        }

                        //Este es el caso de la flecha hacia abajo
                        if ((mousex >= X / 2 - 200 && mousex <= X / 2 - 155) && (mousey >= 620 && mousey <= 720)) {

                            //Pintamos el bitmap en color cyan
                            al_draw_tinted_bitmap(Flecha, al_map_rgb(0, 255, 255), X / 2 - 200, (RY * (500.0 / 768.0)), ALLEGRO_FLIP_VERTICAL);

                            //Esto es para bajar
                            if (eventos.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
                            {
                                if (eventos.mouse.button & 1) {
                                    inicio = inicio + 1;
                                }
                            }
                        }

                        //Caso de salir solo nos pernmite salir de los resultados
                        if ((mousex >= X / 2 - 42 - 300 && mousex <= X / 2 + 42 - 300) && (mousey >= (RY * (6.0 / 768.0)) && mousey <= (RY * (640.0 / 768.0)))) {

                            al_draw_text(font2, al_map_rgb(0, 255, 255), X / 2 - 300, (RY * (600.0 / 768.0)), ALLEGRO_ALIGN_CENTRE, "Atras");

                            if (eventos.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
                            {
                                //Aqui finalizaria el ciclo while
                                if (eventos.mouse.button & 1) {
                                    salir = true;
                                }
                            }
                        }

                        //Esto tambien en caso que se quiera salir con escape en lugar de salir
                        if (eventos.type == ALLEGRO_EVENT_KEY_DOWN) {

                            //Se hace el switch con el teclado 
                            switch (eventos.keyboard.keycode) {

                                //Si se usa escape se sale de juego
                            case ALLEGRO_KEY_ESCAPE:

                                creacion = false;
                            }
                        }

                        //En este al flip display lo que se realiza es pasar directamente al menu principal con los datos ya construidos
                        al_flip_display();
                    }
                }
            }
        }

        //Aqui hacemos lo mismo con salir en caso de que se quiera salir del juego
        if ((mousex >= X / 2 - 42 && mousex <= X / 2 + 42) && (mousey >= (Y * 475.0 / 720.0) && mousey <= (Y * 530.0 / 720.0))) {

            //Cambiamos el color a cyan
            al_draw_text(font, al_map_rgb(0, 255, 255), X / 2, (Y * (470.0 / 720.0)), ALLEGRO_ALIGN_CENTRE, "SALIR");

            //Si se presiona salir creacion pasa a ser false saliendo de la interfaz
            if (eventos.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
                if (eventos.mouse.button & 1) {
                    creacion = false;
                }
            }
        }

        //Esto tambien en caso que se quiera salir con escape en lugar de salir
        if (eventos.type == ALLEGRO_EVENT_KEY_DOWN) {

            //Se hace el switch con el teclado 
            switch (eventos.keyboard.keycode) {
            
            //Si se usa escape se sale de juego
            case ALLEGRO_KEY_ESCAPE:

                creacion = false;
            }
        }

        //Esto para hacer la actualizacion del display
        al_flip_display();
    }

    //Destruimos todos los recursos
    al_destroy_bitmap(Fondo);
    al_destroy_bitmap(Logo);
    al_destroy_bitmap(Flecha);
    al_destroy_font(font);
    al_destroy_font(font2);
    al_destroy_font(font3);
    al_destroy_timer(timer);
    al_destroy_timer(timer2);
    al_destroy_event_queue(cola_eventos);
    al_destroy_display(pantalla);
}
