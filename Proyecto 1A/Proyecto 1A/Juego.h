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

    // Inicialización del sistema de audio y códecs
    if (!al_install_audio()) {
        fprintf(stderr, "Error: No se pudo inicializar el sistema de audio.\n");
        return 0;
    }
    if (!al_init_acodec_addon()) {
        fprintf(stderr, "Error: No se pudo inicializar los códecs de audio.\n");
        return 0;
    }
    if (!al_reserve_samples(50)) { // Reserva un solo canal si solo necesitas la música
        fprintf(stderr, "Error: No se pudo reservar el canal de audio.\n");
        return 0;
    }

    // Carga de música de fondo
    ALLEGRO_SAMPLE* Musica_Arkanoid = al_load_sample("Sonidos/Musica_Arkanoid.wav");
    ALLEGRO_SAMPLE* Game_Over = al_load_sample("Sonidos/Game_Over.mp3");
    ALLEGRO_SAMPLE* Niveles = al_load_sample("Sonidos/Niveles.mp3");
    ALLEGRO_SAMPLE* Puntos = al_load_sample("Sonidos/Puntos.mp3");
    ALLEGRO_SAMPLE* Rebote_Bola = al_load_sample("Sonidos/Rebote_Bola.wav");
    ALLEGRO_SAMPLE* Rebote_Enemigos = al_load_sample("Sonidos/Rebote_Enemigos.wav");

    // Verificar si alguno de los samples no se pudo cargar
    if (!Musica_Arkanoid || !Game_Over || !Niveles || !Puntos || !Rebote_Bola || !Rebote_Enemigos) {
        fprintf(stderr, "Error: No se pudo cargar uno o más archivos de sonido.\n");
        return 0;
    }

    // Reproducir la música en bucle
    al_play_sample(Musica_Arkanoid, 0.3, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);

    //Establecemos los tipos de fuentes a cargar con su tamano
    ALLEGRO_FONT* fuente1 = al_load_font("Video-Font.ttf", 40, NULL);
    ALLEGRO_FONT* fuente2 = al_load_font("Video-Font.ttf", 30, NULL);
    ALLEGRO_FONT* fuente3 = al_load_font("Video-Font.ttf", 20, NULL);

    //Este mensaje se da en caso de que no se puedan cargar las fuentes
    if (!fuente1 || !fuente2) {
        al_show_native_message_box(pantalla, "Error", "Carga de Fuente", "No se pudo cargar las fuentes.", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return 0;
    }

    //Aqui cargamos la imagenes del juego
    ALLEGRO_BITMAP* bloque = al_load_bitmap("Imagenes/Gris.png");
    ALLEGRO_BITMAP* bloque2 = al_load_bitmap("Imagenes/Verde.png");
    ALLEGRO_BITMAP* bloque3 = al_load_bitmap("Imagenes/Naranja.png");
    ALLEGRO_BITMAP* bloque4 = al_load_bitmap("Imagenes/Rojo.png");
    ALLEGRO_BITMAP* bloque5 = al_load_bitmap("Imagenes/Azul.png");
    ALLEGRO_BITMAP* bloque6 = al_load_bitmap("Imagenes/Amarillo.png");
    ALLEGRO_BITMAP* bloque7 = al_load_bitmap("Imagenes/Celeste.png");
    ALLEGRO_BITMAP* fondo = al_load_bitmap("Imagenes/fondo_juego.png");
    ALLEGRO_BITMAP* fondorep = al_load_bitmap("Imagenes/replica.png");
    ALLEGRO_BITMAP* nave2 = al_load_bitmap("Imagenes/Nave_2.png");
    ALLEGRO_BITMAP* fondos = al_load_bitmap("Imagenes/fondo_main.jpg");
    ALLEGRO_BITMAP* bolas = al_load_bitmap("Imagenes/Bola.png");
    ALLEGRO_BITMAP* enemigo1 = al_load_bitmap("Imagenes/Enemigo1.png");
    ALLEGRO_BITMAP* enemigo2 = al_load_bitmap("Imagenes/Enemigo2.png");
    ALLEGRO_BITMAP* enemigo3 = al_load_bitmap("Imagenes/Enemigo3.png");

    //Mensaje en caso de que no se puedan cargar las imagenes
    if (!bloque || !fondo || !nave2 || !fondos || !bolas || !enemigo1 || !enemigo2 || !enemigo3) {
        al_show_native_message_box(pantalla, "Error", "Carga de Imagen", "No se pudo cargar una o mas imagenes.", NULL, ALLEGRO_MESSAGEBOX_ERROR);
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

    //Establecemos las variables del juego
    bool hecho = true;
    bool juego_in = false;
    int puntos = 0;
    int bloques_elim = 0;
    int enemigos_elim = 0;

    //Buffer para el nombre
    char buffer[50];

    //Salida del juego
    int salida = 0;

    //Inicializamos el jugador, bola, enemigos y bloques
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

            enem_nivel++;

            // Resetea el contador de enemigos generados para el nuevo nivel
            enemigos_genera = 0;

            // Pausar la música de fondo
            al_stop_samples(); // Pausa todos los sonidos (incluyendo Musica_Arkanoid)
            al_play_sample(Niveles, 0.5, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL); // Reproducir sonido de cambio de nivel

            // Mensaje de transicion de nivel
            al_clear_to_color(al_map_rgb(0, 0, 0));

            //Establecemos un char para ir por los 10 niveles
            char mensajeNivel[10];
            sprintf(mensajeNivel, "Nivel %d", nivel);

            //Establecemos un mensaje de cambio de nivel
            al_draw_text(fuente1, al_map_rgb(0, 255, 255), pantallaAncho / 2, pantallaAlto / 2 + 10, ALLEGRO_ALIGN_CENTRE, mensajeNivel);
            al_flip_display();

            //Cambia al siguiente nivel
            al_rest(2);

            // Limpieza completa de enemigos
            while (enemigos != NULL) {
                Ptrenemigo temp = enemigos;
                enemigos = enemigos->Siguiente;
                delete temp;
            }
            enemigos = NULL;

            // Reiniciar la bola
            if (bola != NULL) {
                delete bola;
            }

            inicializar_bola(bola, jugador, 10);

            // Ajustar posición inicial de la bola
            bola->x = jugador.x + centroX;
            bola->y = jugador.y - 20;
            bola->velocidadX = 2;
            bola->velocidadY = -2;

            // Generación de nueva formacion de bloques y suma de puntos
            formacion_bloques(bloques, nivel, bloque);


            //Suma de puntos de 100 por cada nivel
            puntos += 100;

            // Reanudar la música de fondo
            al_play_sample(Musica_Arkanoid, 0.5, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL); // Reanudar Musica_Arkanoid en loop

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
                if (!juego_in) {
                    juego_in = true;  // Inicia el juego al primer movimiento
                }
                break;
            
            //Si se presiona la tecla derecha se va a la derecha
            case ALLEGRO_KEY_RIGHT:
                Dir = DERECHA;
                if (!juego_in) {
                    juego_in = true;  // Inicia el juego al primer movimiento
                }
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

            //Aqui utilizamos la variable juego in para si el juego no se ha iniciado o no se ha movido la nave no aparezca nada
            if (!juego_in) {
                al_flip_display();
                
                //Aqui imprimimos una imagen
                al_draw_scaled_bitmap(fondorep, 0, 0, al_get_bitmap_width(fondorep), al_get_bitmap_height(fondorep), 0, 0,  pantallaAncho, pantallaAlto, 0); 
            }

            //Cuando si se mueve la nave se inicializa el juego
            else {
                // Baja la probabilidad de aparicion de enemigos al 2%
                if (enemigos_genera < enem_nivel && rand() % 100 < proba_enem) {
                    generar_enemigos(enemigos, 4, nivel, bloques, ResY - 300, centroY, centroX);
                    enemigos_genera++; //Se incrementan los enemigos por 1 en cada nivel
                }

                //Llamamos la funcion de mover los enemigos en el area del juego
                mover_enemigos(enemigos, bloques, centroX, centroY, bloque, jugador, puntos, Rebote_Enemigos, Puntos);



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
                al_draw_bitmap(fondos, 0, 0, 0);

                // Dibujar fondo centrado sin cambiar su proporcion vertical
                al_draw_scaled_bitmap(fondo, 0, 0, al_get_bitmap_width(fondo), al_get_bitmap_height(fondo), centroX, centroY, RX, RY, 0);

                //Inicialiamos la lista enlazada de bloques
                Ptrbloque bloquesss = bloques;

                //Llamamos a todos los bitmaps para las hileras de bloques
                ALLEGRO_BITMAP* hileras[7] = { bloque, bloque2, bloque3, bloque4, bloque5, bloque6, bloque7 };

                // Factor de escala para el tamano real de los bloques
                float factorEscala = 1.15;
                int bloquesPorFila = 8; // Numero de bloques por cada fila
                int alto_b = al_get_bitmap_height(bloque) * factorEscala; // Altura escalada de cada bloque

                // Recorre todos los bloques y dibuja cada uno
                while (bloquesss != NULL) {
                    if (bloquesss->estado) {
                        // Calcular la fila del bloque usando la posición y su altura
                        int fila = (bloquesss->y - 100) / alto_b;  // Suponiendo que la posición inicial en y es 100
                        int hilera = fila % 7;  // Ciclo entre las 7 hileras de imágenes

                        // Selecciona la imagen correspondiente a la hilera
                        ALLEGRO_BITMAP* bloqueActual = hileras[hilera];

                        // Dibujar el bloque escalado con el factor de escala ajustado
                        al_draw_scaled_bitmap(bloqueActual, 0, 0, al_get_bitmap_width(bloqueActual), al_get_bitmap_height(bloqueActual), bloquesss->x + centroX, bloquesss->y + centroY, al_get_bitmap_width(bloqueActual) * factorEscala, al_get_bitmap_height(bloqueActual) * factorEscala, 0);
                    }

                    // Avanza al siguiente bloque en la lista
                    bloquesss = bloquesss->Siguiente;
                }


                //Aqui iniciamos la lista con los enemigos
                Ptrenemigo Aux_enem = enemigos;

                //Recorremos toda la lista con el auxiliar
                while (Aux_enem != NULL) {

                    //Si el enemigo es true osea esta activo
                    if (Aux_enem->estado) {

                        // Rebote en los bordes laterales del juego, subimos un poco el borde para que sea realista
                        if (Aux_enem->x < centroX + diametro + 15 || Aux_enem->x > centroX + RX - diametro - 20) {
                            Aux_enem->velocidadX = -Aux_enem->velocidadX; //Invertimos la velocidad
                        }

                        //Aqui en el caso del borde superior
                        if (Aux_enem->y < centroY + diametro + 15 || Aux_enem->y > centroY + RY) {
                            Aux_enem->velocidadY = -Aux_enem->velocidadY; //Alternamos la velocidad vertical
                        }

                        //Construimos un bitmap de la imagen del enemigo esto para las imagenes y sus variaciones
                        ALLEGRO_BITMAP* imagen_enem = NULL;

                        // Seleccionar la imagen basada en el tipo de enemigo
                        switch (Aux_enem->tipo) {

                        case 0:
                            imagen_enem = enemigo1;
                            break;
                        case 1:
                            imagen_enem = enemigo2;
                            break;
                        case 2:
                            imagen_enem = enemigo3;
                            break;

                        }

                        // Si la imagen correspondiente no esta definida, saltar este enemigo
                        if (!imagen_enem) {
                            Aux_enem = Aux_enem->Siguiente;
                            continue;
                        }

                        // Verificar las dimensiones de la imagen
                        int ancho_e = al_get_bitmap_width(imagen_enem);
                        int alto_e = al_get_bitmap_height(imagen_enem);

                        //Validacion en caso de imagen erronea
                        if (ancho_e <= 0 || alto_e <= 0) {
                            std::cerr << "Error: Dimensiones de la imagen de enemigo no válidas." << std::endl;
                            Aux_enem = Aux_enem->Siguiente;
                            continue;
                        }

                        // Escala de un 50 por ciento
                        float tam = 1.5;
                        float escala = (diametro * tam) / ancho_e;

                        // Dibujamos la imagen escalada segun corresponda
                        al_draw_scaled_bitmap(imagen_enem, 0, 0, ancho_e, alto_e, Aux_enem->x - ((diametro * tam) / 2), Aux_enem->y - ((diametro * tam) / 2), ancho_e * escala, alto_e * escala, 0);

                    }

                    //Seguimos recorriendo la lista enlazada para cada enemigo
                    Aux_enem = Aux_enem->Siguiente;


                }

                // Llamamos a la funcion de colision con la bola
                colision_bola(bola, bloques, jugador, enemigos, vidas, centroX, centroY, bloque, puntos, bloques_elim, enemigos_elim, Rebote_Bola);



                // Verifica si la bola est activa

                if (bola->estado) {

                    // Rebote en los bordes laterales
                    if (bola->x - bola->radio < centroX + 25) { // Limite izquierdo
                        bola->x = centroX + bola->radio + 25; // Ajustar la posicion 
                        bola->velocidadX = -bola->velocidadX; // Rebote en el borde izquierdo
                    }
                    else if (bola->x + bola->radio > centroX + RX - 25) { // Limite derecho
                        bola->x = centroX + RX - bola->radio - 25; // Ajustar la posicion 
                        bola->velocidadX = -bola->velocidadX; // Rebote en el borde derecho
                    }

                    if (bola->y - bola->radio < centroY + 30) { // Limite superior
                        bola->y = centroY + bola->radio + 30; // Ajustar la posicion para no salir
                        bola->velocidadY = -bola->velocidadY; // Rebote en el borde superior
                    }

                    else if (bola->y + bola->radio > centroY + RY) { // Limite inferior
                        vidas--; // Resta una vida si la bola sale por el borde inferior
                        bola->x = jugador.x + centroX; // Reposiciona la bola en la zona centrada
                        bola->y = jugador.y + centroY - 10; // Reposiciona por encima de la nave
                        bola->velocidadY = -fabs(bola->velocidadY); // Rebote hacia arriba
                    }


                    // Actualizar la posicion de la bola
                    bola->x += bola->velocidadX;
                    bola->y += bola->velocidadY;

                    // Escala para ajustar el radio
                    float escala = (2.0 * bola->radio) / al_get_bitmap_width(bolas);

                    // Dibujar la imagen de la bola centrada 
                    al_draw_scaled_bitmap(bolas, 0, 0, al_get_bitmap_width(bolas), al_get_bitmap_height(bolas), bola->x - bola->radio, bola->y - bola->radio, al_get_bitmap_width(bolas) * escala, al_get_bitmap_height(bolas) * escala, 0);


                }

                //Llamamos a dibujar nave
                dibujar_nave(jugador, nave2, centroX, centroY);

                // Mostrar estadisticas en la esquina superior derecha de la pantalla completa

                sprintf(buffer, "Puntos: %d", puntos);
                al_draw_text(fuente1, al_map_rgb(255, 255, 255), pantallaAncho - 100, 20, ALLEGRO_ALIGN_RIGHT, buffer);

                sprintf(buffer, "Vidas: %d", vidas);
                al_draw_text(fuente1, al_map_rgb(255, 255, 255), pantallaAncho - 100, 70, ALLEGRO_ALIGN_RIGHT, buffer);

                sprintf(buffer, "Enemigos Eliminados: %d", enemigos_elim);
                al_draw_text(fuente2, al_map_rgb(255, 255, 255), 20, 40, ALLEGRO_ALIGN_LEFT, buffer);

                sprintf(buffer, "Bloques Eliminados: %d", bloques_elim);
                al_draw_text(fuente2, al_map_rgb(255, 255, 255), 20, 90, ALLEGRO_ALIGN_LEFT, buffer);

                //En caso de que se llegue a 0 vidas
                if (vidas <= 0) {

                    // Limpiar pantalla 
                    al_clear_to_color(al_map_rgb(0, 0, 0));
                    al_flip_display();

                    // Establecer variables para salir del juego
                    hecho = false;
                    salida = 1;

                    // Pausar la música de fondo
                    al_stop_samples(); // Pausa todos los sonidos (incluyendo Musica_Arkanoid)
                    al_play_sample(Game_Over, 0.5, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL); // Reproducir sonido de cambio de nivel
                }

                // En caso de que hecho no se cumpla y salida sea 1
                if (!hecho && salida == 1) {

                    //Establecemos diversas variables para puntaje y demas
                    int seguir = true;
                    char puntaje[60] = "Puntaje: ";
                    char enemigos_elim2[30] = "Enemigos Eliminados: ";
                    char bloques_elim2[30] = "Bloques Eliminados: ";

                    sprintf(buffer, "%d", puntos);  // Asegurarse de usar los puntos actuales
                    strcat_s(puntaje, 60, buffer);

                    strcpy(enemigos_elim2, "Enemigos Eliminados: ");  // Reinicia la cadena para evitar duplicados
                    sprintf(buffer, "%d", enemigos_elim);  // Guarda los enemigos
                    strcat_s(enemigos_elim2, 30, buffer);

                    strcpy(bloques_elim2, "Bloques Eliminados: ");  // Reinicia la cadena para evitar duplicados
                    sprintf(buffer, "%d", bloques_elim);  // Guarda los bloques
                    strcat_s(bloques_elim2, 30, buffer);

                    //Inicializacion del nombre
                    char nombre[40] = { '_' };
                    int pos = 0;

                    // Limpiar la pantalla antes de entrar al bucle de nombre
                    al_clear_to_color(al_map_rgb(0, 0, 0));
                    al_flip_display();

                    //Iniciamos el ciclo seguir
                    while (seguir) {

                        //Llamamos a los eventos de allegro
                        ALLEGRO_EVENT evento;
                        al_wait_for_event(cola_eventos, &evento);

                        //Esto es para los resultados en pantalla
                        if (evento.type == ALLEGRO_EVENT_KEY_CHAR) {

                            //Mientras la personas no escriba mas de 39 caracteres
                            if (pos < 39) {

                                //Establecemos los eventos de teclado
                                if (evento.keyboard.keycode == ALLEGRO_KEY_BACKSPACE && pos > 0) {
                                    nombre[--pos] = '\0';
                                }

                                //Si se da enter se guarda lo que se escribio
                                else if (evento.keyboard.keycode == ALLEGRO_KEY_ENTER) {

                                    // Guardar el archivo antes de salir
                                    if (pos == 0) nombre[0] = '?';
                                    CrearArchivo(buffer, nombre, enemigos_elim2, bloques_elim2);

                                    // Limpiar la pantalla 
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

                                    // Volver al main
                                    return 0;
                                }

                                //Capturamos los espacios
                                else if (evento.keyboard.keycode == ALLEGRO_KEY_SPACE) {
                                    nombre[pos++] = '_';
                                }

                                //Se capturan los caracteres visibles del teclado
                                else if (evento.keyboard.unichar >= 32 && evento.keyboard.unichar <= 126) {
                                    nombre[pos++] = evento.keyboard.unichar;
                                }
                            }
                        }

                        // Dibujamos en pantalla game over y los resultados de la persona
                        al_clear_to_color(al_map_rgb(0, 0, 0));
                        al_draw_text(fuente1, al_map_rgb(0, 255, 255), pantallaAncho / 2, pantallaAlto / 2 - 250, ALLEGRO_ALIGN_CENTRE, "Game Over!");
                        al_draw_text(fuente2, al_map_rgb(255, 255, 255), pantallaAncho / 2 - 500, pantallaAlto / 2 - 150, ALLEGRO_ALIGN_CENTRE, puntaje);
                        al_draw_text(fuente2, al_map_rgb(255, 255, 255), pantallaAncho / 2 - 100, pantallaAlto / 2 - 150, ALLEGRO_ALIGN_CENTRE, enemigos_elim2);
                        al_draw_text(fuente2, al_map_rgb(255, 255, 255), pantallaAncho / 2 + 400, pantallaAlto / 2 - 150, ALLEGRO_ALIGN_CENTRE, bloques_elim2);
                        al_draw_text(fuente2, al_map_rgb(0, 255, 255), pantallaAncho / 2, pantallaAlto / 2, ALLEGRO_ALIGN_CENTRE, nombre);
                        al_draw_text(fuente2, al_map_rgb(255, 255, 255), pantallaAncho / 2, pantallaAlto / 2 + 200, ALLEGRO_ALIGN_CENTRE, "Presione Enter para volver al menu");

                        al_flip_display();
                    }
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
    al_destroy_bitmap(fondorep);
    al_destroy_event_queue(cola_eventos);

    // Destruir todos los samples de audio cargados
    al_destroy_sample(Musica_Arkanoid);
    al_destroy_sample(Game_Over);
    al_destroy_sample(Niveles);
    al_destroy_sample(Puntos);
    al_destroy_sample(Rebote_Bola);
    al_destroy_sample(Rebote_Enemigos);

    al_uninstall_audio();
    
    //Devuelve la salida
    return salida;
}

