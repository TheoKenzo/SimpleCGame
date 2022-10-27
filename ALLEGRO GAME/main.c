#include <allegro.h>

/*
RECORDE

HI 1554
*/

//funções
int colidir(int Ax, int Ay,int Bx , int By, int Aw, int Ah, int Bw, int Bh);
void sair();
void control();

//objetos

struct obj{
    int wx, wy, x, y, w, h;
};

struct obj
 p  = {0, 0, 400, 300, 31, 31},
 ed = {0, 0, 800, 350, 31, 31},
 bloco[13][26];
 ;

//Variáveis Globais
int sai    =   0;
int width  = 800;
int height = 600;
int nTile  =   0;
int dir    =   0;
int grv    =   4;
int caindo =   1;
int vly    =   0;
int i      =   0;
int j      =   0;
int pulando=   0;
int vup    =  15;
int pLimit =   0;
int timer  =   0;
int rnd    =   0;
int rnd1   =   0;
int speed  =  10;

BITMAP *buffer, *imagem, *fundo, *enemy[2];

SAMPLE *mufundo, *pular, *run, *pontos;

int main() {

    //sons
    mufundo = load_sample("fundo.wav");
    pular   = load_sample("pulo.wav");
    run     = load_sample("run.wav");
    pontos  = load_sample("pontos.wav");

    //Iniciação
 allegro_init();
    install_keyboard();
 set_color_depth(32);
 set_window_title("MTEC");
 set_close_button_callback(sair);
 install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL);
 set_gfx_mode(GFX_AUTODETECT_WINDOWED, width, height, 0, 0);

 //Variáveis Locais
 buffer    = create_bitmap(width, height);
 imagem    = load_bitmap("sprites.bmp", NULL);
 fundo     = load_bitmap("fundo.bmp", NULL);
 enemy[0]  = load_bitmap("eny_1.bmp", NULL);
 enemy[1]  = load_bitmap("eny_2.bmp", NULL);

 //alinhamento da matriz
 for(i = 0; i < 13; i++){
        for(j = 0; j < 26; j++){
            bloco[i][j].y  = 380;
            bloco[i][j].x  = j*31;
            bloco[i][j].w  = 31;
            bloco[i][j].h  = 31;
            bloco[i][j].wy = 155;
            bloco[i][j].wx = 244;
        }
 }

 play_sample(mufundo, 255, 128, 1000, 1);

 while (!(sai || key[KEY_ESC]))
 {
     control();

     for(i = 0; i < 13; i++){
            for(j = 0; j < 26; j++){
                masked_blit(imagem, buffer, bloco[i][j].wx, bloco[i][j].wy, bloco[i][j].x, bloco[i][j].y, bloco[i][j].w, bloco[i][j].h);
                if (colidir(p.x, p.y + 20, bloco[i][j].x, bloco[i][j].y, 34, 40, bloco[i][j].w, 10)){
                        p.y = bloco[i][j].y - p.h;
                        caindo = 0;
                }
            }
     }

     masked_blit(imagem, buffer, p.wx + nTile*31, p.wy + dir*31, p.x, p.y, p.w, p.h);
     masked_blit(enemy[rnd], buffer, ed.wx, ed.wy, ed.x, ed.y, ed.w, ed.h);

     ed.x -= speed;

     if(ed.x < -112){
        rnd = rand()% 2;
        ed.x = (rand()%20)*100 + width;
     }

    rectfill(buffer, 0, 550, width, height, 0x2e2e2e);
    textprintf(buffer, font, 400, 560, makecol(255, 255, 255), "Pontos: %d", timer);
  draw_sprite(screen, buffer, 0, 0);
  rest(70);

  clear(buffer);
        draw_sprite(buffer, fundo, 0, 0);

        timer++;

        if(timer % 100 == 0){
            play_sample(pontos, 255, 128, 1000, 0);
        }

        if(timer % 10 == 0 && timer < 1000){
            speed++;
        }

    if(p.x + p.w > ed.x && ed.x + ed.w > p.x && p.y + p.h > ed.y && ed.y + ed.h > p.y){
        rest(2000);
        return 0;
    }
 }

 //Finalização
 destroy_bitmap(buffer);
 destroy_bitmap(imagem);
 destroy_sample(pular);
 destroy_sample(mufundo);

 return 0;
}
END_OF_MAIN();

void control(){
    if(pulando || caindo){
        if(key[KEY_RIGHT]||key[KEY_D]) {p.x += 20; dir = 1; nTile++;}
        if(key[KEY_LEFT]||key[KEY_A])  {p.x -= 20; dir = 4; nTile++;}
    }

    else if(key[KEY_LEFT]||key[KEY_A])       {p.x -= 10; dir = 3; nTile++; rest(90); play_sample(run, 200, 128, 1000, 0);}
    else if(key[KEY_RIGHT]||key[KEY_D]) {p.x += 10; dir = 0; nTile++; rest(90); play_sample(run, 200, 128, 1000, 0);}
    else if(key[KEY_UP] && !pulando && !vly || key[KEY_SPACE] && !pulando && !vly || key[KEY_W] && !pulando && !vly){
            pLimit = p.y;
            pulando = 1;
            rest(50);
            play_sample(pular, 255, 128, 1000, 0);
         }
    else nTile = 0;

    if(nTile < 0) nTile = 0;
    if(nTile > 3) nTile = 0;

    if(pulando && p.y > pLimit - 10){
        p.y   -= vly+100;
        vly    = vup;
        caindo = 1;
        pulando = 0;
    }else if(caindo){
              pulando = 0;
              vly    += grv;
              p.y    += vly;
           }else{
                pulando = 0;
                vly     = 0;
            }
    caindo  =   1;
}

int colidir(int Ax, int Ay, int Bx, int By, int Aw, int Ah, int Bw, int Bh){
    if(Ax + Aw > Bx && Ax < Bx + Bw && Ay + Ah > By && Ay < By + Bh)
    return 1;
    return 0;
}

void sair(){sai= 1;}
END_OF_FUNCTION(sair);
