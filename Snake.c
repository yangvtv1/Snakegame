/*
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above
 *   copyright notice, this list of conditions and the following disclaimer
 *   in the documentation and/or other materials provided with the
 *   distribution.
 * * Neither the name of the  nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */

#include "Snake.h"

void _menu_update(void *arg);
void _menu_wait(void *arg);
void Initial(void *arg);
void _Move(void *arg);
int _FindPos(void *arg, int _PosX, int _PosY);
void _ChangeState(void *arg);
void _menu_fruit(void *arg);
game_t *game;

int main (void) {
    game = (game_t *)malloc(10 * sizeof(game_t));
    Initial(game);
    game->ChangeState(game);
    //  while(1){
    //     game->menu_fruit(game);
    //  }

    printf("Game Over!!!\n");
    scanf("%hhd", &game->exit);
    free(game);
    return 0;
}
void Initial(void *arg) {
    game_t *G = arg;
    G->X                 = 31;
    G->Y                 = 16;
    G->CountUpdate       = 1;
    G->Fruit             = 0;
    G->Point             = 0;
    G->i                 = 0;
    for(int i = 0; i < BURN_INIT; i++){
        G->axis[i]    = (axis_t *)malloc(sizeof(axis_t));
    }
    for(int i = 0; i < BURN_INIT; i++){
        G->axis[i]->x    = (G->X/2)-i;
        G->axis[i]->y    = (G->Y/2);
        //printf("%d %d %d %c\n", i, G->axis[i]->x, G->axis[i]->y, G->Position[i]);
    }
    G->LenCurrent      = LEN_BURN;
    G->menu_update     = &_menu_update;
    G->menu_wait       = &_menu_wait;
    G->menu_fruit      = &_menu_fruit;
    G->FMove           = &_Move;
    G->FindPos         = &_FindPos; 
    G->ChangeState     = &_ChangeState;
    G->Idx             = UPDATE;
}
menu_t menu_tab[] = {
    {_menu_update},
    {_menu_wait}
};
void _menu_update(void *arg){
    game_t *G = arg;
    G->menu_fruit(G);
    int index;
    if(G->Move == UP || G->Move == LEFT)
        index = G->LenCurrent;
    else if(G->Move == DOWN || G->Move == RIGHT)
        index = 0;
        
    while (G->CountUpdate) {
        system("@cls||clear");
        for (int YIndex = 0; YIndex < G->Y + 2; YIndex++) {
            for (int XIndex = 0; XIndex < G->X; XIndex++) {
                if(!YIndex) {
                    if((!YIndex && !XIndex) || (!YIndex && XIndex == 1)){
                        printf(" ");
                    }else{
                        printf("#");
                    }
                }

                if (G->axis[0]->x + 1 >= G->X || G->axis[0]->x <= 0 || G->axis[0]->y + 1 >= G->Y || G->axis[0]->y <= 0) {
                    return;
                }else{
                    if((G->axis[0]->y == G->FruitY) && (G->axis[0]->x == G->FruitX) && G->Fruit) {
                        G->Fruit = 0;
                        G->Point++;
                        //G->LenCurrent++;
                        //G->axis[G->LenCurrent]    = (axis_t *)malloc(sizeof(axis_t));
                        //for(int i = G->LenCurrent; i >= 1; i--){
                            //memcpy(G->axis[i], G->axis[i - 1], sizeof(G->axis[i]));
                        //}
                        
                    }else if(YIndex == G->FruitY && XIndex == G->FruitX && G->Fruit){
                        printf("@");
                    }else if(G->FindPos(G, XIndex, YIndex) == EOK){
                        printf("%d", G->Position);
                    }else if((XIndex == G->X/2) && (YIndex == G->Y)){
                        printf("%d", G->Point);
                    }else if(((YIndex == G->axis[0]->y) && (XIndex+1 == G->X))
                             || (YIndex == G->axis[0]->y && XIndex == 1)){
                        printf("X");
                    }else if((YIndex && !XIndex) || (YIndex && (XIndex+1 == G->X))
                             || (YIndex && XIndex == 1)) {
                        if ((YIndex != G->axis[0]->y) || (YIndex && !XIndex))
                            if(YIndex && !XIndex) {
                                printf("%02d", YIndex);
                            }else if((YIndex && XIndex) || (YIndex && XIndex == 1)){
                                printf("X");
                            }
                    }else if(YIndex && (YIndex + 1 != G->Y)){
                        if((YIndex == G->Y + 1) && (XIndex+1 != G->X)){
                            printf("X");
                        }else{
                            printf(" ");
                        }
                    }
                }
                
                if((YIndex + 1 == G->Y) && (XIndex+1 != G->X) && (XIndex+2 != G->X)) {
                    if(XIndex != 1)
                        printf("X");
                }
            }
            printf("\n");
        }
        G->CountUpdate--;
    }
    G->Idx = WAIT;
    G->ChangeState(G);
}
void _menu_wait(void *arg){
    game_t *G = arg;
    scanf("%d", &G->Move);
    if((G->Move == DOWN) || (G->Move == LEFT) || (G->Move ==UP) || (G->Move == RIGHT)) {
        G->Idx = UPDATE;
        G->CountUpdate = 1;
       G->FMove(G);
    } else {
        G->Idx = WAIT;
    }
    G->ChangeState(G);
}
void _Move(void *arg){
    game_t *G = arg;
    uint8_t dir = 0;
    for(int i = (G->LenCurrent - 1); i >= 0; i--){
        memcpy(G->axis[i+1], G->axis[i], sizeof(G->axis[i]));
    }
    if(G->Move == DOWN)
        G->axis[0]->y++;
    else if(G->Move == UP)
        G->axis[0]->y--;
    else if(G->Move == LEFT)
        G->axis[0]->x--;
    else if(G->Move == RIGHT)
        G->axis[0]->x++;
}

int _FindPos(void *arg, int _PosX, int _PosY){
    game_t *G = arg;
    int ret = ERR;
    for(int i = G->LenCurrent; i >= 0; i--){
        if(_PosX == G->axis[i]->x && _PosY == G->axis[i]->y){
            G->Position = i;
            ret = EOK;
        }
    }
    return ret;    
}

void _menu_fruit(void *arg){
    game_t *G = arg;
    static int count = 0;
    char getbuf[11]="";
    uint8_t axisX = 0;
    uint8_t axisY = 0;
    if(!G->Fruit){
        G->Fruit = rand();
        sprintf(getbuf, "%d", G->Fruit);
        axisX = (getbuf[0]-0x30)*10 + (getbuf[1]-0x30);
        axisY = axisX;
        
        for(int x = 0; x < 10; x++) {
            if(!axisX || axisX == 1){
                axisX += 2;
            }else if(axisX >= (G->X - 1)){
                axisX -= G->X;
            }else{
                break;
            }
        }

         for(int y = 0; y < 10; y++) {    
            if(!axisY){
                axisY++;
            }else if((axisY >= G->Y) || (axisY == G->Y - 1)){
                if(axisY == G->Y - 1) axisY = 1;
                else axisY -= G->Y;
            }else{
                break;
            }
        }
        G->FruitX = axisX;
        G->FruitY = axisY;
    }
}
void _ChangeState(void *arg) {
    game_t *G = arg;
    menu_tab[G->Idx].PointerFunction(G);
}
