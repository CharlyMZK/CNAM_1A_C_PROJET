#include "dessine.h"
#include <string.h>

// -- Struct init
GC      gc;
Display *display;
Window  win, root;

// -- Params init
int largeur_fenetre;
int hauteur_fenetre;
int     screen;

// -- Externs definition
extern void player_play(int x, int y);
extern void draw_win();
extern void draw_win_menu();
extern void draw_win_board_size();
extern void drop_stone(int x, int y);
extern void test_clicked(int coord,float taille_case);
extern void mouse_clicked(int bouton, int x, int y);
extern void key_pressed(KeySym code, char c, int x_souris, int y_souris);

/**
 * largeur fenetre
 */
double width_win()
{
	return largeur_fenetre;
}

/**
 * hauteur fenetre
 */
double height_win()
{
	return hauteur_fenetre;
}

/**
 * initialisation fenetre
 * w: largeur
 * h:  hauteur
 * titre: titre fenetre
 * r,g,b couleur de fond
 */
void init_win(int w, int h, char* message)
{
	if ((display = XOpenDisplay ("")) == NULL)
	{
		fprintf (stderr, "Can't open Display\n");
		exit (1);
	}

	largeur_fenetre = w;
	hauteur_fenetre = h;
	// recupere param pour creation fenetre
	gc = DefaultGC (display, screen);
	screen = DefaultScreen (display);
	root = RootWindow (display, screen);
	int rr = 255;
	int gg = 178;
	int bb = 102;
	unsigned long col_pixel = rr<<16|gg<<8|bb;
	unsigned long black_pixel = BlackPixel (display, screen);
	// creation fenetre
	win = XCreateSimpleWindow (display, root, 0, 0, largeur_fenetre+largeur_fenetre, hauteur_fenetre+(hauteur_fenetre*0.3), 5, black_pixel, col_pixel);
	// filtre les evenements
	XSelectInput (display, win, ExposureMask|ButtonPressMask|KeyPressMask);
	// titre fenetre
	XStoreName (display, win, message);
	XMapWindow (display, win);
}

/**
 * boucle evenementielle
 */
void event_loop()
{
	char buffer[8];
	KeySym touche;
	int nb;
	XEvent ev;
	draw_win_menu();
	//draw_win_menu();
	while(1)
	{
		XNextEvent (display, &ev);
		switch (ev.type)
		{
		case Expose :
			draw_win_menu();
			break;
		case ButtonPress:
			mouse_clicked(ev.xbutton.button,ev.xbutton.x,ev.xbutton.y);
			break;
		case KeyPress:
			if ( ev.xkey.keycode == 0x09 ) // ESC : on sort
				exit(0);
			nb = XLookupString((XKeyEvent*)&ev, buffer, 20, &touche, 0);
			buffer[nb] = 0;
			key_pressed(touche, buffer[0],ev.xbutton.x,ev.xbutton.y);
			break;
		default :
			break;
		}
	}
}

/**
 * vide fenetre
 */
void clear_win()
{
	XClearWindow(display, win);
}

/**
 *  change couleur de trace en RVB
 *  r: %rouge [0,1]
 *  v: %vert [0,1]
 *  b: %bleu [0,1]
 */
void color(float r, float v, float b)
{
	int rr = 255*r;
	int vv = 255*v;
	int bb = 255*b;
	XSetForeground (display, gc, 256*256*rr+256*vv+bb);
}

/**
 * trace une ligne
 * x0,y0 point depart
 * x1,y1 point arrivee
 */
void line(int x0, int y0, int x1, int y1)
{
	XDrawLine(display,win,gc,x0,y0,x1,y1);
}

/**
 * trace un rectangle
 * x,y position
 * w,h taille
 */
void rectangle(int x, int y, int w, int h)
{
	XDrawRectangle(display,win,gc,x,y,w,h);
}

/**
 * trace un rectangle plein
 * x,y position
 * w,h taille
 */
void filled_rectangle(int x, int y, int w, int h)
{
	XFillRectangle(display,win,gc,x,y,w,h);
}

/**
 * trace un cercle
 * x,y position
 * r rayon
 */
void circle(int x, int y, int r)
{
	XDrawArc(display,win,gc,x-r,y-r,2*r,2*r,0,360*64);
}

/**
 * trace un disque
 * x,y position
 * r rayon
 */
void filled_circle(int x, int y, int r)
{
	XFillArc(display,win,gc,x-r,y-r,2*r,2*r,0,360*64);
}

/**
 * trace un pixel a la position x,y
 */
void pixel(int x, int y)
{
	XDrawPoint(display,win,gc,x,y);
}

/**
 * affiche une chaine de caractere
 */
void string(int x, int y, char* chaine)
{
	XDrawString(display,win,gc,x,y, chaine, strlen(chaine));
}

/**
 * Affiche le tour du joueur
 */
void draw_player_turn(int cell_size, int turn){
	// -- Refresh du rectange
	color(255,178,102);
	filled_rectangle(width_win()+cell_size+5,0,300,30);
	// -- Marquage du joueur
	color(0,0,0);
	if(turn == 0){
		string(width_win()+cell_size+20,20,"Tour de noir");
	}else{
		string(width_win()+cell_size+20,20,"Tour de blanc");
	}
}

/*
 * Affiche les informations de la partie chargée
 */
void draw_loaded_game(int cell_size, int turn, char* date, char* won){
	// -- Refresh du rectange
	color(255,178,102);
	filled_rectangle(width_win()+cell_size+5,0,300,30);
	// -- Marquage du joueur
	color(0,0,0);
	string(width_win()+cell_size+20,20,"Affichage d'une partie chargee");

	color(255,178,102);
	filled_rectangle(width_win()+cell_size+5,50,300,30);
	color(0,0,0);
	string(width_win()+cell_size+20,70,date);

  color(255,178,102);
  filled_rectangle(width_win()+cell_size+5,100,300,30);

  color(0,0,0);
  string(width_win()+cell_size+20,120, won);
}

/**
 * Dessine le menu permettant de choisir le mode de jeu
 */
void draw_win_menu(){
	// - Vide la fenetre
	clear_win();
	// -- Bouton joueur contre joueur
	color(255,178,102);
	filled_rectangle(50,50,200,30);
	color(0,0,0);
	rectangle(50,50,200,30);
	string(70,70,"Joueur vs joueur");
	// -- Bouton joueur contre bot
	color(255,178,102);
	filled_rectangle(50,100,200,30);
	color(0,0,0);
	rectangle(50,100,200,30);
	string(70,120,"Joueur vs bot");
	// -- Charger une partie
	color(255,178,102);
	filled_rectangle(50,150,200,30);
	color(0,0,0);
	rectangle(50,150,200,30);
	string(70,170,"Charger une partie");

}

/**
 * Dessine le menu permettant de choisir la taille du plateau
 */
void draw_win_board_size(){
	// - Vide la fenetre
	clear_win();
	// -- 19x19
	color(255,178,102);
	filled_rectangle(50,50,200,30);
	color(0,0,0);
	rectangle(50,50,200,30);
	string(70,70,"19x19");
	// -- 13x13
	color(255,178,102);
	filled_rectangle(50,100,200,30);
	color(0,0,0);
	rectangle(50,100,200,30);
	string(70,120,"13x13");
	// -- 9x9
	color(255,178,102);
	filled_rectangle(50,150,200,30);
	color(0,0,0);
	rectangle(50,150,200,30);
	string(70,170,"9x9");

}

/**
 * Dessine le menu permettant de choisir le handicap du joueur 2
 */
void draw_menu_handicap(){
	// - Vide la fenetre
	clear_win();
	// -- Bouton joueur contre joueur
	color(255,178,102);
	filled_rectangle(50,50,200,30);
	color(0,0,0);
	rectangle(50,50,200,30);
	string(70,70,"6 tours de handicap");
	// -- Bouton joueur contre bot
	color(255,178,102);
	filled_rectangle(50,100,200,30);
	color(0,0,0);
	rectangle(50,100,200,30);
	string(70,120,"3 tours de handicap");
	// -- Charger une partie
	color(255,178,102);
	filled_rectangle(50,150,200,30);
	color(0,0,0);
	rectangle(50,150,200,30);
	string(70,170,"0 tours de handicap");
}

/**
 * Dessine les hoshis
 */
void draw_hoshi(int board_size,int cell_size) {
 	int i,j;
	color(0,0,0);
	switch(board_size){
		case 9:
			for(i=2; i <= 6; i+=4){
				for(j=2; j <= 6; j+=4){
					int x = cell_size + (j * cell_size);
					int y = cell_size + (i * cell_size);
					filled_circle(x,y,cell_size/6);
				}
			}
			break;
		case 13:
			for(i=3; i <= 9; i+=3){
				for(j=3; j <= 9; j+=3){
					int x = cell_size + (j * cell_size);
					int y = cell_size + (i * cell_size);
					filled_circle(x,y,cell_size/6);
				}
			}
		break;
		case 19:
			for(i=3; i <= 15; i+=6){
				for(j=3; j <= 15; j+=6){
					int x = cell_size + (j * cell_size);
					int y = cell_size + (i * cell_size);
					filled_circle(x,y,cell_size/6);
				}
			}
			break;
	}
}
