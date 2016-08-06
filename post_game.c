#include "headers/global.h"
// Creating current score to display on the screen
void draw_elements(void);
void exit_game(void);
char * input (void);
char * make_string(char select)
{
    int n=0,r=0,i=0,j=0;
    char hud[20]="";
    char ch[30]="",tmp[30]="";
    switch (select)
    {
    case 'D':
        if(_difficulty==1)
            return("Easy");
        if (_difficulty==2)
            return("Medium");
        if(_difficulty==3)
            return("Hard");

    case 'S':
        n=score;
        strcpy(hud,"  erocS"); //Score is stored in reverse form so later whole string ch is reversed and stored in tmp
        break;


    case 'L':

        n=current_level+1;
        strcpy(hud,"  leveL");
        break;


    }
    if (n==0)
        ch[i++]='0';
    while(n!=0)
    {
        r = n%10;
        ch[i] = r +'0';
        n=n/10;
        i++;
    }

    ch[i]='\0';
    strcat(ch,hud);
    for(i=strlen(ch)-1; i>=0; i--)
        tmp[j++]=ch[i];
    return(tmp);

}

void save_highscore(char str_score[])
{
    int i,j,pos=-1;
    bool play_again=false;
    al_set_target_bitmap(al_get_backbuffer(display));
    al_draw_text(big_font,al_map_rgb(255,0,0), 400, 300,ALLEGRO_ALIGN_CENTRE, "Game Over!!!!!");
    al_draw_text(big_font, al_map_rgb(0,0,255), 400, 400,ALLEGRO_ALIGN_CENTRE, str_score);
    al_flip_display();
    al_rest(1);
    al_clear_to_color(al_map_rgb(0,0,0));
    al_flip_display();

    if (score > player[9].score)
    {
        strcpy(player[9].name,input());
        player[9].score = score;

        for(i=0; i<10; i++)
        {
            for(j=0; j<9; j++)
            {
                if(player[j].score<player[j+1].score)
                {
                    _tmp=player[j];
                    player[j]=player[j+1];
                    player[j+1]=_tmp;
                }
            }
        }

        FILE *fptr=NULL;
        fptr=fopen("data/score.dat","w");
        for(i=0; i<10; i++)
            fprintf(fptr,"%s\n%d\n",player[i].name,player[i].score);
        fclose(fptr);

    }

    else
    {
        level_running=false;
        game_running=false;
    }
    print_highscore();
}

void exit_game(void)
{
    /*Encrypting score file */
    FILE *ptr=fopen("data/score.dat","r");
    FILE *target_ptr=fopen("data/temp_score.dat","w");
    while(!feof(ptr))
    {
        char ch=fgetc(ptr);
        if(ch!='\0')
        {
            fputc(~ch,target_ptr);
        }
    }
    fclose(ptr);
    fclose(target_ptr);
    remove("data/score.dat");
    rename("data/temp_score.dat","data/score.dat");

    al_destroy_font(font);
    al_destroy_font(big_font);
    al_destroy_bitmap(head);
    al_destroy_bitmap(body);
    al_destroy_bitmap(tail);
    al_destroy_bitmap(block_red);
    al_destroy_bitmap(block_green);
    al_destroy_bitmap(block_blue);
    al_destroy_bitmap(block_yellow);
    al_destroy_bitmap(block_gray);
    al_destroy_bitmap(_food);
    al_destroy_bitmap(b_food);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);
    al_destroy_sample(_type);
    al_destroy_sample(navi);
    al_destroy_sample(eat);
    al_destroy_sample(die);
    al_destroy_sample(enter);
    al_destroy_display(display);
}


char * input(void)
{
    int count=0;
    int i=0;
    char name[15]="";
    al_set_target_bitmap(background);
    al_clear_to_color(al_map_rgb(0,0,0));
    al_draw_text(font,al_map_rgb(100,100,100), 640/2+2, 480/4+2,ALLEGRO_ALIGN_CENTRE, "You Got Highscore Enter your name:");
    al_draw_text(font,al_map_rgb(255,255,255), 640/2, 480/4,ALLEGRO_ALIGN_CENTRE, "You Got Highscore Enter your name:");

    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_set_target_bitmap(al_get_backbuffer(display));

    while(1)
    {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue,&ev);
        al_clear_to_color(al_map_rgb(0,0,0));
        al_draw_bitmap(background,0,0,0);
        al_draw_text(font,al_map_rgb(255,255,255), 640/2, 480,ALLEGRO_ALIGN_CENTRE, name);;
        al_flip_display();

        if(ev.type==ALLEGRO_EVENT_KEY_DOWN)
        {

            if(ev.keyboard.keycode==ALLEGRO_KEY_ENTER)
            {
                al_play_sample(enter, 1.0, -1.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
                break;
            }

            if(ev.keyboard.keycode==ALLEGRO_KEY_BACKSPACE && i>0)
            {
                al_play_sample(_type, 1.0, -1.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
                i--;
                name[i]=' ';
                name[i+1]='\0';
            }

            if(ev.keyboard.keycode>=ALLEGRO_KEY_A && ev.keyboard.keycode<=ALLEGRO_KEY_Z)
            {
                al_play_sample(_type, 1.0, -1.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
                for(count=0; count<26; count++)
                {
                    if(ev.keyboard.keycode==ALLEGRO_KEY_A+count)
                    {
                        name[i]='A'+count;
                        name[i+1]='\0';
                        i++;
                    }
                }
            }
        }// End of key detection
    } // End of while loop

    level_running=false;
    game_running=false;

    return(name);
}
