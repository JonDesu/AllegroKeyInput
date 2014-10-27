#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

enum KEYS{W, S, A, D}; // W = array[0] S = array[1] A = array[2] D = array[3]
const float FPS = 60;
const int WIDTH = 640;
const int HEIGHT = 480;

int main(void)
{
	bool done = false;
	bool redraw = true;

	int pos_x = WIDTH / 2;
	int pos_y = HEIGHT / 2;
	int count = 0;

	float arrow_x = WIDTH / 2.0;
	float arrow_y = HEIGHT / 2.0;

	bool keys[4] = { false, false, false, false }; //each index can either be true or false (1 or 0);

	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_BITMAP *img_home = NULL;
	ALLEGRO_BITMAP *img_arrow = NULL;
	ALLEGRO_EVENT_QUEUE *ev_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_FONT *font18 = NULL;

	if (!al_init())return -1; //initialize ALLEGRO engine
	if (!(display = al_create_display(WIDTH, HEIGHT)))return -1; //assign new display and check for nullness
	if (!al_init_image_addon())return -1; //enable images
	if(!(timer = al_create_timer(1.0 / FPS))) return -1; //enable timer to track FPS

	ev_queue = al_create_event_queue(); //assign new event queue
	img_home = al_load_bitmap("home.png"); //load background image
	img_arrow = al_load_bitmap("arrow.png"); //load arrow image
	//font18 = al_load_font("TerminusTTF-4.39.ttf", 18, 0);

	al_init_primitives_addon();
	//al_init_font_addon();
	//al_init_ttf_addon();
	al_install_keyboard(); //load keyboard
	al_install_mouse(); //load mouse
	al_register_event_source(ev_queue, al_get_keyboard_event_source()); //register keyboard as event source
	al_register_event_source(ev_queue, al_get_display_event_source(display));//register display as event source
	al_register_event_source(ev_queue, al_get_mouse_event_source()); //register mouse as event source
	al_register_event_source(ev_queue, al_get_timer_event_source(timer)); //register timer as event source
	al_draw_bitmap(img_home, 0, 0, 0); //draw background
	al_hide_mouse_cursor(display); 
	al_flip_display();
	al_start_timer(timer);

	while (!done)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(ev_queue, &ev); //wait for something in the event queue

		if (ev.type == ALLEGRO_EVENT_MOUSE_AXES || ev.type == ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY)
		{
			arrow_x = ev.mouse.x;
			arrow_y = ev.mouse.y;
		}

		if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_W:
				keys[W] = true;
				break;
			case ALLEGRO_KEY_S:
				keys[S] = true;
				break;
			case ALLEGRO_KEY_A:
				keys[A] = true;
				break;
			case ALLEGRO_KEY_D:
				keys[D] = true;
				break;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_UP)
		{
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_W:
				keys[W] = false;
				break;
			case ALLEGRO_KEY_S:
				keys[S] = false;
				break;
			case ALLEGRO_KEY_A:
				keys[A] = false;
				break;
			case ALLEGRO_KEY_D:
				keys[D] = false;
				break;
			case ALLEGRO_KEY_ESCAPE:
				done = true; //exit while loop
				break;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_TIMER) //only update every 60th of a second..
		{
			pos_y -= keys[W] * 10; //since keys[x] can either be true or false (1 or 0), we can update position accordingly
			pos_y += keys[S] * 10;
			pos_x -= keys[A] * 10;
			pos_x += keys[D] * 10;
			redraw = true; //we are good to render a new frame
		}
		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) done = true; //exit while loop
		
		if (redraw && al_is_event_queue_empty(ev_queue)) //ensure ev queue is empty && 60th of second has passed..
		{
			redraw = false;
			al_draw_bitmap(img_home, 0, 0, 0); //redraw map
			al_draw_bitmap(img_arrow, arrow_x, arrow_y, 0); //redraw arrow
			al_draw_filled_circle(pos_x, pos_y, 30, al_map_rgb(176, 23, 31)); //redraw circle
			al_flip_display(); //brings back buffer to front buffer
		}
	}

	al_destroy_display(display);

	return 0;
}