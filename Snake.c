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
void _MoveDown(void *arg);
void _MoveUp(void *arg);
void _MoveLeft(void *arg);
void _MoveRight(void *arg);
uint8_t _ProcessMove(void *arg);
void _JumpStep(void *arg);
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
    G->X                = 31;
    G->Y                = 16;
    G->CountUpdate      = 1;
    G->Fruit            = 0;
    G->Move2            = 3;
    G->Move4            = 3;
    G->Move6            = 3;
    G->Move8            = 3;
    G->Point            = 0;
    G->i                = 0;
    G->TurnUpFlag       = false;
    for(int i = 0; i < BURN_INIT; i++){
        G->axis[i]    = (axis_t *)malloc(sizeof(axis_t));
    }
    for(int i = 0; i < BURN_INIT; i++){
        G->axis[i]->x    = (G->X/2)-i;
        G->axis[i]->y    = (G->Y/2);
        //printf("%d %d %d %c\n", i, G->axis[i]->x, G->axis[i]->y, G->Position[i]);
    }
    G->menu_update     = &_menu_update;
    G->menu_wait       = &_menu_wait;
    G->menu_fruit      = &_menu_fruit;
    G->ProcessMove     = &_ProcessMove;
    G->MoveDown        = &_MoveDown;
    G->MoveUp          = &_MoveUp;
    G->MoveLeft        = &_MoveLeft;
    G->MoveRight       = &_MoveRight;
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
                    }else if(YIndex == G->FruitY && XIndex == G->FruitX && G->Fruit){
                        printf("@");
                    }else if(((YIndex == G->axis[LEN_BURN - G->i]->y) && (XIndex == G->axis[LEN_BURN - G->i]->x))
                          || ((YIndex == G->axis[G->i]->y) && (XIndex == G->axis[G->i]->x))
                          ){
                        if(G->i != LEN_BURN){
                            if((G->axis[LEN_BURN - G->i]->x < G->axis[LEN_BURN - G->i - 1]->x) 
                            && (G->axis[LEN_BURN - G->i]->y == G->axis[LEN_BURN - G->i - 1]->y)){
                                if(G->TurnUpFlag){
                                    printf("%d", G->i);                                  
                                }else{
                                    printf("%d", LEN_BURN - G->i);
                                }
                            }else if(G->axis[G->i]->x > G->axis[G->i + 1]->x 
                                     && G->TurnUpFlag){
                                printf("%d", G->i);
                            }else if((G->axis[G->i]->x < G->axis[G->i + 1]->x) 
                                 && (G->axis[G->i]->y == G->axis[G->i + 1]->y)){
                                printf("%d", G->i);
                            }else if((G->axis[LEN_BURN - G->i]->y < G->axis[LEN_BURN - G->i - 1]->y) 
                                 && (G->axis[LEN_BURN - G->i]->x == G->axis[LEN_BURN - G->i - 1]->x)){
                                    printf("%d", LEN_BURN - G->i);
                            }else if((G->axis[G->i]->y < G->axis[G->i + 1]->y) 
                                 && (G->axis[G->i]->x == G->axis[G->i + 1]->x)){
                                printf("%d", G->i);
                            }
                        }else if((G->axis[LEN_BURN - G->i]->x > G->axis[LEN_BURN - G->i + 1]->x) 
                              && (G->axis[LEN_BURN - G->i]->y == G->axis[LEN_BURN - G->i + 1]->y)){
                            printf("%d", LEN_BURN - G->i);
                        }else if((G->axis[G->i]->x < G->axis[G->i - 1]->x) && (G->TurnUpFlag)){
                            G->TurnUpFlag = false;
                            printf("%d", 2);
                        }else if((G->axis[G->i]->x > G->axis[G->i - 1]->x)  
                              && (G->axis[G->i]->y == G->axis[G->i - 1]->y)){
                            printf("%d", G->i);
                        }else if((G->axis[LEN_BURN - G->i]->y > G->axis[LEN_BURN - G->i + 1]->y) 
                              && (G->axis[LEN_BURN - G->i]->x == G->axis[LEN_BURN - G->i + 1]->x)){
                            printf("%d", LEN_BURN - G->i);
                        }else if((G->axis[G->i]->y > G->axis[G->i - 1]->y) 
                              && (G->axis[G->i]->x == G->axis[G->i - 1]->x)){
                            printf("%d", G->i);
                        }
                        if(G->i < LEN_BURN)
                            G->i++;                  
                    }else if((XIndex == G->X/2) && (YIndex == G->Y)){
                        printf("%d.%d", G->Point, G->i);
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
        if(G->Move == DOWN) G->MoveDown(G);
        else if(G->Move == UP) G->MoveUp(G);
        else if(G->Move == LEFT) G->MoveLeft(G);
        else if(G->Move == RIGHT) G->MoveRight(G);
    } else {
        G->Idx = WAIT;
    }
    if(G->i) G->i = 0;
    G->ChangeState(G);
}
void _MoveDown(void *arg){
    game_t *G = arg;
    if(G->ProcessMove(G) == EOK){
        if(G->Move2 == 3) {
            _JumpStep(G);
            for(int i = 0; i < BURN_INIT - 1; i++){
                if(G->axis[i]->x < G->axis[i+1]->x){
                    if(i == BURN_INIT - 2){
                        for(int j = 1; j <= BURN_INIT - 1; j++){
                            G->axis[j]->x--;
                        }
                        G->axis[0]->y++;
                    }
                }else{
                    for(int j = 1; j <= BURN_INIT - 1; j++){
                        G->axis[j]->x++;
                    }
                    G->axis[0]->y++;                    
                    break;
                }
            }
        }else if(G->Move2 == 2) {
            G->Move2 = 1;
            for(int i = 0; i < BURN_INIT - 1; i++){
                if((G->axis[i]->y > G->axis[i+1]->y) && (G->axis[i+1]->x < G->axis[i+2]->x)){
                    G->axis[BURN_INIT-1]->x--;
                    for(int j = 0; j < BURN_INIT - 1; j++){
                        G->axis[j]->y++;
                    }
                    break;
                }else{
                    for(int j = 0; j < BURN_INIT - 1; j++){
                            G->axis[j]->y++;
                    }
                    G->axis[BURN_INIT-1]->x++;
                    break;
                }
            }
        }
    }else{
        for(int i = 0; i < BURN_INIT - 1; i++){
            if(G->axis[i]->x == G->axis[i+1]->x){
                if(i+1 == BURN_INIT-1){
                    for(int j = 0; j < BURN_INIT; j++){
                        G->axis[j]->y++;
                    }
                }
            }
        }
        G->Move2=3;
    }
}
void _MoveUp(void *arg){
    game_t *G = arg;
    if(_ProcessMove(G) == EOK){
        if(G->Move8 == 3) {
            _JumpStep(G);
            for(int i = 0; i < BURN_INIT - 1; i++){
                if(G->axis[i]->x < G->axis[i+1]->x){
                    if(i == BURN_INIT - 2){
                        for(int j = 1; j <= BURN_INIT - 1; j++){
                            G->axis[j]->x--;
                        }
                        G->axis[0]->y--;
                    }
                }else{
                    G->TurnUpFlag = true;
                    for(int j = 1; j <= BURN_INIT - 1; j++){
                        G->axis[j]->x++;
                    }
                    G->axis[0]->y--;                    
                    break;
                }
            }
        }else if(G->Move8 == 2) {
            G->Move8 = 1;
            for(int i = 0; i < BURN_INIT - 1; i++){
                if((G->axis[i]->y < G->axis[i+1]->y) && (G->axis[i+1]->x < G->axis[i+2]->x)){
                    G->axis[BURN_INIT-1]->x--;
                    for(int j = 0; j < BURN_INIT - 1; j++){
                        G->axis[j]->y--;
                    }
                    break;
                }else{
                    for(int j = 0; j < BURN_INIT - 1; j++){
                            G->axis[j]->y--;
                    }
                    G->axis[BURN_INIT-1]->x++;
                    break;
                }
            }
        }
    }else {
        for(int i = 0; i < BURN_INIT - 1; i++){
            if(G->axis[i]->x == G->axis[i+1]->x){
                if(i+1 == BURN_INIT-1){
                    for(int j = 0; j < BURN_INIT; j++){
                        G->axis[j]->y--;
                    }
                }
            }
        }
        G->Move8 = 3;
    }
}
void _MoveLeft(void *arg){
    game_t *G = arg;
    if(G->ProcessMove(G) == EOK){
        if(G->Move4 == 3) {
            _JumpStep(G);
            for(int i = 0; i < BURN_INIT - 1; i++){
                if(G->axis[i]->y < G->axis[i+1]->y){
                    if(i == BURN_INIT - 2){
                        for(int j = 1; j <= BURN_INIT - 1; j++){
                            G->axis[j]->y--;
                        }
                        G->axis[0]->x--;
                    }
                }else{
                    for(int j = 1; j <= BURN_INIT - 1; j++){
                        G->axis[j]->y++;
                    }
                    G->axis[0]->x--;                    
                    break;
                }
            }
        }else if(G->Move4 == 2) {
            G->Move4 = 1;
            for(int i = 0; i < BURN_INIT - 1; i++){
                if((G->axis[i]->x < G->axis[i+1]->x) && (G->axis[i+1]->y < G->axis[i+2]->y)){
                    G->axis[BURN_INIT-1]->y--;
                    for(int j = 0; j < BURN_INIT - 1; j++){
                        G->axis[j]->x--;
                    }
                    break;
                }else{
                    for(int j = 0; j < BURN_INIT - 1; j++){
                            G->axis[j]->x--;
                    }
                    G->axis[BURN_INIT-1]->y++;
                    break;
                }
            }
        }
    }else{
        for(int i = 0; i < BURN_INIT - 1; i++){
            if(G->axis[i]->y == G->axis[i+1]->y){
                if(i+1 == BURN_INIT-1){
                    for(int j = 0; j < BURN_INIT; j++){
                        G->axis[j]->x--;
                    }
                }
            }
        }
        G->Move4 = 3;
    }
}
void _MoveRight(void *arg){
    game_t *G = arg;
    if(G->ProcessMove(G) == EOK){
        if(G->Move6 == 3) {
             _JumpStep(G);
            for(int i = 0; i < BURN_INIT - 1; i++){
                if(G->axis[i]->y < G->axis[i+1]->y){
                    if(i == BURN_INIT - 2){
                        for(int j = 1; j <= BURN_INIT - 1; j++){
                            G->axis[j]->y--;
                        }
                        G->axis[0]->x++;
                    }
                }else{
                    for(int j = 1; j <= BURN_INIT - 1; j++){
                        G->axis[j]->y++;
                    }
                    G->axis[0]->x++;                    
                    break;
                }
            }
        }else if(G->Move6 == 2) {
            G->Move6 = 1;
            for(int i = 0; i < BURN_INIT - 1; i++){
                if((G->axis[i]->x > G->axis[i+1]->x) && (G->axis[i+1]->y < G->axis[i+2]->y)){
                    G->axis[BURN_INIT-1]->y--;
                    for(int j = 0; j < BURN_INIT - 1; j++){
                        G->axis[j]->x++;
                    }
                    break;
                }else{
                    for(int j = 0; j < BURN_INIT - 1; j++){
                            G->axis[j]->x++;
                    }
                    G->axis[BURN_INIT-1]->y++;
                    break;
                }
            }
        }
    }else{
        for(int i = 0; i < BURN_INIT - 1; i++){
            if(G->axis[i]->y == G->axis[i+1]->y){
                if(i+1 == BURN_INIT-1){
                    for(int j = 0; j < BURN_INIT; j++){
                        G->axis[j]->x++;
                    }
                }
            }
        }
        G->Move6 = 3;
    }
}
uint8_t _ProcessMove(void *arg){
    game_t *G = arg;
    uint8_t ret = ERR;
    uint32_t index = 0;
    for(index = 0; index < BURN_INIT - 1; index++){
        if(G->Move == DOWN || G->Move == UP){
            if((G->axis[index]->x > G->axis[index + 1]->x) || (G->axis[index]->x < G->axis[index + 1]->x)){
                if(index == BURN_INIT - 2){
                    ret = EOK;
                    break;
                }
            }
        }else if(G->Move == LEFT || G->Move == RIGHT){
            if((G->axis[index]->y > G->axis[index + 1]->y) || (G->axis[index]->y < G->axis[index + 1]->y)){
                if(index == BURN_INIT - 2){
                    ret = EOK;
                    break;
                }
            }
        }
    }
    return ret;
}
void _JumpStep(void *arg){
    game_t *G = arg;
    if(G->Move == DOWN){
        G->Move2 = 2;
        G->Move4 = 3;
        G->Move6 = 3;
        G->Move8 = 3;
    }else if(G->Move == UP){
        G->Move8 = 2;
        G->Move2 = 3;
        G->Move4 = 3;
        G->Move6 = 3;       
    }else if(G->Move == LEFT){
        G->Move4 = 2;
        G->Move2 = 3;
        G->Move6 = 3;
        G->Move8 = 3;      
    }else if(G->Move == RIGHT){
        G->Move6 = 2;
        G->Move2 = 3;
        G->Move4 = 3;
        G->Move8 = 3;
    }
}
void _menu_fruit(void *arg){
    game_t *G = arg;
    static int count = 0;
    char getbuf[11]="";
    uint8_t axisX = 0;
    uint8_t axisY = 0;
    if(/*count++<=1*/!G->Fruit){
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
        //printf("G->FruitX=%d, G->FruitY=%d\n", G->FruitX, G->FruitY);
    }
    //if(count>=10)count=11;
}
void _ChangeState(void *arg) {
    game_t *G = arg;
    menu_tab[G->Idx].PointerFunction(G);
}



