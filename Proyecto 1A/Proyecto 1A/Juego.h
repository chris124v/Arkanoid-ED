//Este serie el archivo del juego como tal en donde implementamos la parte grafica

//Christopher Daniel Vargas Villalta, Carnet: 2024108443
//Santiago Espinoza Rendon, Carnet: 2024156530

#pragma once

//Llamamos a las funciones del juego
#include "FuncionesJuego.h"

//Usamos esto para evitar problemas de archivos
using namespace std;
#pragma warning(disable:4996)
#define FPS 60.0

//Juego principal de arkanoid 
int arkanoid(int nivel, int vidas) {

    //LLamamos al monitor para obtener su informacion
    ALLEGRO_MONITOR_INFO monitor;
    al_get_monitor_info(0, &monitor);

    //Definimos el ancho y alto de la pantalla segun el monitor
    const int pantallaAncho = monitor.x2 - monitor.x1;
    const int pantallaAlto = monitor.y2 - monitor.y1;

    // Dimensiones fijas del area de juego
    const int RX = 800; 
    const int RY = 1100; 

    //Obtenemos la pantallla o display y lo acomodamos segun el monitor.
    ALLEGRO_DISPLAY* pantalla = al_create_display(pantallaAncho, pantallaAlto);

    //Lo designamos como pantalla en fullscreen
    al_set_display_flag(pantalla, ALLEGRO_FULLSCREEN, true);

    //Este seria el nombre de la pantalla
    al_set_window_title(pantalla, "Arkanoid");

    //Mensaje en caso de que la pantalla no se pueda inicializar
    if (!pantalla) {
        al_show_native_message_box(NULL, "Ventana Emergente", "Error", "No se puede crear la pantalla", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return 0;
    }

    //Establecemos los tipos de fuentes a cargar con su tamano
    ALLEGRO_FONT* fuente1 = al_load_font("Video-Font.ttf", 40, NULL);
    ALLEGRO_FONT* fuente2 = al_load_font("Video-Font.ttf", 30, NULL);

    //Este mensaje se da en caso de que no se puedan cargar las fuentes
    if (!fuente1 || !fuente2) {
        al_show_native_message_box(pantalla, "Error", "Carga de Fuente", "No se pudo cargar las fuentes.", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return 0;
    }

    //Aqui cargamos la imagenes del juego
    ALLEGRO_BITMAP* bloque = al_load_bitmap("Imagenes/Celeste_1.png");
    ALLEGRO_BITMAP* fondo = al_load_bitmap("Imagenes/fondo_juego.png");
    ALLEGRO_BITMAP* nave2 = al_load_bitmap("Imagenes/Nave_2.png");
    ALLEGRO_BITMAP* fondo2 = al_load_bitmap("Imagenes/fondo_main.jpg");

    //Mensaje en caso de que no se puedan cargar las imagenes
    if (!bloque || !fondo || !nave2 ) {
        al_show_native_message_box(pantalla, "Error", "Carga de Imagen", "No se pudo cargar una o más imágenes.", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return 0;
    }

    //Establecemos las direcciones de la nave
    enum Direccion { NINGUNA, IZQUIERDA, DERECHA };
    enum Direccion Dir = NINGUNA; //Definimos una direccion para que no se mueva

    //Aqui inicialiamos la cola de eventos 
    ALLEGRO_EVENT_QUEUE* cola_eventos = al_create_event_queue();

    //Inicializamos los timers
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);

    //Registramos en la cola de eventos el timer y el teclado
    al_register_event_source(cola_eventos, al_get_timer_event_source(timer));
    al_register_event_source(cola_eventos, al_get_keyboard_event_source());

    //Establecemos las variables del juegp
    bool hecho = true;
    int puntos = 0;

    //Buffer para el nombre
    char buffer[20];

    //Salida del juego
    int salida = 0;

    //Inicializamos el jugador, bola, enemigos y bloqq
    nave jugador;
    Ptrbola bola = NULL; 
    Ptrbloque bloques = NULL;
    Ptrenemigo enemigos = NULL;

    //Probabilidades por nivel
    int proba_enem = 2;
    int enem_nivel = 1;  
    int enemigos_genera = 0;

    // Calcula la posición para centrar el área de juego en pantalla completa
    int centroX = (pantallaAncho - RX) / 2;
    int centroY = (pantallaAlto - RY) / 2;

    //Llamamos a las funciones para inicializar la bola, nave y bloques
    Inicializar_nave(jugador, RX);
    inicializar_bola(bola, jugador, 5);
    formacion_bloques(bloques, nivel, bloque);

    //Pone en negro la pantalla
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_flip_display();
    al_rest(1);

    //Inicializa los timers
    al_start_timer(timer);

    //Ciclo while mientras se cumpla hecho
    while (hecho) {

        //Iniciamos los eventos
        ALLEGRO_EVENT eventos;

        //Esperamos los eventos
        al_wait_for_event(cola_eventos, &eventos);

        //Si ya no quedan bloques
        if (!quedanBloques(bloques)) {

            //Subimos el nivel
            nivel++;

            //Subimos los enemigos por cada nivel
            if (enem_nivel > 5) {
                enem_nivel++;
            }

            // Resetea el contador de enemigos generados para el nuevo nivel
            enemigos_genera = 0;

            // Mensaje de transicion de nivel
            al_clear_to_color(al_map_rgb(0, 0, 0));

            //Establecemos un char para ir por los 10 niveles
            char mensajeNivel[10];
            sprintf(mensajeNivel, "Nivel %d", nivel);

            //Establecemos un mensaje de cambio de nivel
            al_draw_text(fuente1, al_map_rgb(0, 255, 255), pantallaAncho / 2, pantallaAlto / 2 + 40, ALLEGRO_ALIGN_CENTRE, mensajeNivel);
            al_flip_display();

            //Cambia al siguiente nivel
            al_rest(2);

            // Generación de nueva formacion de bloques y suma de puntos
            formacion_bloques(bloques, nivel, bloque);

            //Suma de puntos de 100 por cada nivel
            puntos += 100;

            continue;
        }

        
        //Esto es en caso de que se presione una tecla
        if (eventos.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch (eventos.keyboard.keycode) {

            //Si se presiona escape salimos del juego
            case ALLEGRO_KEY_ESCAPE:
                hecho = false;
                salida = 0;
                break;
            
            //Si se presiona la tecla izquierda la nave va a la izquierda
            case ALLEGRO_KEY_LEFT:
                Dir = IZQUIERDA;
                break;
            
            //Si se presiona la tecla derecha se va a la derecha
            case ALLEGRO_KEY_RIGHT:
                Dir = DERECHA;
                break;
            }
        }

        // Manejo de teclas liberadas
        if (eventos.type == ALLEGRO_EVENT_KEY_UP) {

            //Esto seria el caso de ninguno en caso de que se liberen las flechas
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

        //Aqui iniciamos los eventos del timer
        if (eventos.type == ALLEGRO_EVENT_TIMER) {

            // Baja la probabilidad de aparicion de enemigos al 2%
            if (enemigos_genera < enem_nivel && rand() % 100 < proba_enem) {
                generar_enemigos(enemigos, 2, nivel, bloques, ResY - 300, centroY, centroX);
                enemigos_genera++; //Se incrementan los enemigos por 1 en cada nivel
            }

            //Llamamos la funcion de mover los enemigos en el area del juego
            mover_enemigos(enemigos, bloques, centroX, centroY, bloque);


      
            // Movimiento del jugador
            switch (Dir) {

            case IZQUIERDA:
                if (jugador.x >= centroX - RX / 2 - 120) // Limite izquierdo
                    jugador.x -= jugador.velocidadX;
                break;

            case DERECHA:
                if (jugador.x <= centroX + RX / 2 - 300) // Limite derecho
                    jugador.x += jugador.velocidadX;
                break;
            }

            //Esto es en caso de que se presione escape para salir del juego
            if (eventos.type == ALLEGRO_EVENT_KEY_DOWN) {
                switch (eventos.keyboard.keycode) {
                case ALLEGRO_KEY_ESCAPE:
                    hecho = false;
                }
            }

            //Flip display para actualizar frames
            al_flip_display();

            //Cambiamos todo a color negro
            al_clear_to_color(al_map_rgb(0, 0, 0));

            //Dibujamos el fondo de estrellas
            al_draw_bitmap(fondo2, 0, 0, 0);

            // Dibujar fondo centrado sin cambiar su proporcion vertical
            al_draw_scaled_bitmap(fondo, 0, 0, al_get_bitmap_width(fondo), al_get_bitmap_height(fondo), centroX, centroY, RX, RY, 0);

            //Establecemos la lista enlazada de bloques
            Ptrbloque bloquesss = bloques;

            //Recorre todos los bloques
            while (bloquesss != NULL) {

                //Si esta activo
                if (bloquesss->estado) {

                    //Dibujamos los bloques con los factores de posicion
                    al_draw_scaled_bitmap( bloque, 0, 0, al_get_bitmap_width(bloque), al_get_bitmap_height(bloque), bloquesss->x + centroX, bloquesss->y + centroY, al_get_bitmap_width(bloque) / 2, al_get_bitmap_height(bloque) / 2, 0 );

                }

                //Seguimos recorriendo la lista enlazada de los bloques
                bloquesss = bloquesss->Siguiente;
            }

            
            Ptrenemigo Aux_enem = enemigos;

            while (Aux_enem != NULL) {

                if (Aux_enem->estado) {

                    // Rebote en los bordes del fondo
                    if (Aux_enem->x < centroX + diametro + 15 || Aux_enem->x > centroX + RX - diametro - 20) {
                        Aux_enem->velocidadX = -Aux_enem->velocidadX;
                    }

                    if (Aux_enem->y < centroY + diametro + 15 || Aux_enem->y > centroY + RY) {
                        Aux_enem->velocidadY = -Aux_enem->velocidadY;
                    }

                    // Dibujar el enemigo en amarillo
                    al_draw_filled_circle(Aux_enem->x, Aux_enem->y, diametro / 2, al_map_rgb(255, 255, 0));

                    // Dibuja la hitbox del enemigo como un contorno para depuración
                    al_draw_rectangle(
                        Aux_enem->x - diametro / 2,            // X inicial (izquierda)
                        Aux_enem->y - diametro / 2,            // Y inicial (arriba)
                        Aux_enem->x + diametro / 2,            // X final (derecha)
                        Aux_enem->y + diametro / 2,            // Y final (abajo)
                        al_map_rgb(255, 0, 0),                    // Color rojo para el contorno de la hitbox
                        1                                         // Grosor del contorno
                    );

                }

                Aux_enem = Aux_enem->Siguiente;

     
            }

            // Actualizar la posición de la bola y manejar colisiones
            colision_bola(bola, bloques, jugador, enemigos, vidas, centroX, centroY, bloque, puntos);

            

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

                
            }

            dibujar_nave(jugador, nave2, centroX, centroY);

            // Mostrar estadísticas en la esquina superior derecha de la pantalla completa
            sprintf(buffer, "Puntos: %d", puntos);
            al_draw_text(fuente1, al_map_rgb(255, 255, 255), pantallaAncho - 100, 20, ALLEGRO_ALIGN_RIGHT, buffer);

            sprintf(buffer, "Vidas: %d", vidas);
            al_draw_text(fuente1, al_map_rgb(255, 255, 255), pantallaAncho - 100, 70, ALLEGRO_ALIGN_RIGHT, buffer);

            if (vidas <= 0) {
                // Limpiar pantalla y establecer variables
                al_clear_to_color(al_map_rgb(0, 0, 0));
                al_flip_display();

                // Establecer variables para salir del juego
                hecho = false;
                salida = 1;
            }

            // Mover esta sección fuera del bucle de eventos del timer
            if (!hecho && salida == 1) {
                int seguir = true;
                char puntaje[20] = "Puntaje: ";
                sprintf(buffer, "%d", puntos);  // Asegurarse de usar los puntos actuales
                strcat_s(puntaje, 20, buffer);
                char nombre[40] = { '_' };
                int pos = 0;

                // Limpiar la pantalla antes de entrar al bucle de nombre
                al_clear_to_color(al_map_rgb(0, 0, 0));
                al_flip_display();

                while (seguir) {
                    ALLEGRO_EVENT evento;
                    al_wait_for_event(cola_eventos, &evento);

                    if (evento.type == ALLEGRO_EVENT_KEY_CHAR) {
                        if (pos < 39) {
                            if (evento.keyboard.keycode == ALLEGRO_KEY_BACKSPACE && pos > 0) {
                                nombre[--pos] = '\0';
                            }
                            else if (evento.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                                // Guardar el archivo antes de salir
                                if (pos == 0) nombre[0] = '?';
                                CrearArchivo(buffer, nombre);

                                // Limpiar la pantalla una última vez
                                al_clear_to_color(al_map_rgb(0, 0, 0));
                                al_flip_display();

                                // Limpiar recursos
                                al_destroy_timer(timer);
                                al_destroy_font(fuente1);
                                al_destroy_font(fuente2);
                                al_destroy_display(pantalla);
                                al_destroy_bitmap(bloque);
                                al_destroy_bitmap(fondo);
                                al_destroy_event_queue(cola_eventos);

                                return 0; // Volver al main
                            }
                            else if (evento.keyboard.keycode == ALLEGRO_KEY_SPACE) {
                                nombre[pos++] = '_';
                            }
                            else if (evento.keyboard.unichar >= 32 && evento.keyboard.unichar <= 126) {
                                nombre[pos++] = evento.keyboard.unichar;
                            }
                        }
                    }

                    // Redibujar la pantalla
                    al_clear_to_color(al_map_rgb(0, 0, 0));
                    al_draw_text(fuente1, al_map_rgb(255, 255, 20), pantallaAncho / 2, pantallaAlto / 2 - 250,
                        ALLEGRO_ALIGN_CENTRE, "FIN DEL JUEGO");
                    al_draw_text(fuente2, al_map_rgb(255, 255, 255), pantallaAncho / 2, pantallaAlto / 2 - 150,
                        ALLEGRO_ALIGN_CENTRE, puntaje);
                    al_draw_text(fuente2, al_map_rgb(255, 255, 20), pantallaAncho / 2, pantallaAlto / 2,
                        ALLEGRO_ALIGN_CENTRE, nombre);
                    al_draw_text(fuente2, al_map_rgb(255, 255, 255), pantallaAncho / 2, pantallaAlto / 2 + 200,
                        ALLEGRO_ALIGN_CENTRE, "Presione Enter para volver al menu");
                    al_flip_display();
                }
            }

            
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

