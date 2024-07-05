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

#include "game.h"

void _menu_update(void *arg);
void _menu_wait(void *arg);
void Initial(void *arg);
void _cal_axis(void *arg);
void _ChangeState(void *arg);
game_t *game;

int main (void) {
    game = (game_t *)malloc(10 * sizeof(game_t));
    Initial(game);
    game->ChangeState(game);
    printf("Game Over!!!\n");
    scanf("%hhd", &game->exit);
    free(game);
    return 0;
}
void Initial(void *arg) {
    game_t *G = arg;
    G->X               = 30;
    G->Y               = 14;
    G->CountUpdate     = 1;
    G->Snake[0][0]     = (G->X/2); // 15
    G->Snake[0][1]     = (G->Y/2); // 7
    G->Snake[1][0]     = (G->X/2);
    G->Snake[1][1]     = (G->Y/2)-1;
    G->Snake[2][0]     = (G->X/2);
    G->Snake[2][1]     = (G->Y/2)-2;
    G->SnakeOld[0][0] = G->Snake[0][0]; 
    G->SnakeOld[0][1] = G->Snake[0][1]; 
    G->SnakeOld[1][0] = G->Snake[1][0];
    G->SnakeOld[1][1] = G->Snake[1][1];
    G->SnakeOld[2][0] = G->Snake[2][0];
    G->SnakeOld[2][1] = G->Snake[2][1];
    G->menu_update     = &_menu_update;
    G->menu_wait       = &_menu_wait;
    G->cal_axis        = &_cal_axis;
    G->ChangeState     = &_ChangeState;
    G->Idx             = UPDATE;
}

menu_t menu_tab[] = {
    {_menu_update},
    {_menu_wait}
};
void _menu_update(void *arg){
    game_t *G = arg;

    while (G->CountUpdate) {
        system("@cls||clear");
        for (int YIndex = 0; YIndex < G->Y; YIndex++) {
            for (int XIndex = 0; XIndex < G->X; XIndex++) {
                if(!YIndex) {
                    printf("_");
                }
                if (G->Snake[0][0] + 3 >= G->X 
                    || G->Snake[0][0] <= 0
                    || G->Snake[0][1] + 1 >= G->Y
                    || G->Snake[0][1] <= 0) {
                    return;
                }else{
                    if((YIndex == G->Snake[2][1]) && (XIndex == G->Snake[2][0])) {
                        if((G->Snake[2][1] > G->SnakeOld[2][1]) 
                            || G->Snake[2][1] < G->SnakeOld[2][1]
                            || G->Snake[2][1] == G->SnakeOld[2][1]){
                            printf("2");
                        }
                    }else if((YIndex == G->Snake[1][1]) && (XIndex == G->Snake[1][0])) {
                        if((G->Snake[1][1] > G->SnakeOld[1][1]) 
                            || G->Snake[1][1] < G->SnakeOld[1][1]
                            || G->Snake[1][1] == G->SnakeOld[1][1]){
                            printf("1");
                        }
                    }else if((YIndex == G->Snake[0][1]) && (XIndex == G->Snake[0][0])) {
                        if((G->Snake[0][1] > G->SnakeOld[0][1]) 
                            || G->Snake[0][1] < G->SnakeOld[0][1]
                            || G->Snake[0][1] == G->SnakeOld[0][1]){
                            printf("0");
                        }
                    }else if((YIndex == G->Snake[0][1]) && (XIndex+1 == G->X)){
                        printf("|");
                    }else if((YIndex && !XIndex) || (YIndex && (XIndex+1 == G->X))) {
                        if ((YIndex != G->Snake[0][1]) || (YIndex && !XIndex))
                            printf("|");
                    }else if(YIndex && (YIndex + 1 != G->Y)){
                        printf(" ");
                    }
                }
                
                if((YIndex + 1 == G->Y) && (XIndex+1 != G->X) && (XIndex+2 != G->X)) {
                    printf("X");
                }
            }
            printf("\n");
        }
        //sleep(1);
        G->CountUpdate--;
        G->cal_axis(G);
    }
    G->Idx = WAIT;
    G->ChangeState(G);
}
void _menu_wait(void *arg){
    game_t *G = arg;
    static uint8_t Move4 = 3;
    static uint8_t Move8 = 3;
    static uint8_t Move6 = 3;
    static uint8_t Move2 = 3;
    scanf("%d", &G->Move);
    if((G->Move == 2) || (G->Move == 4) || (G->Move == 8) || (G->Move == 6)) {
        G->Idx = UPDATE;
        G->CountUpdate = 1;
        if(G->Move == 2) {
            if((G->Snake[0][0] > G->Snake[1][0] && G->Snake[1][0] > G->Snake[2][0])
                || (G->Snake[0][0] < G->Snake[1][0] && G->Snake[1][0] < G->Snake[2][0])
                || (G->Snake[0][1] > G->Snake[1][1] && G->Snake[1][0] < G->Snake[2][0])
                || (G->Snake[0][1] > G->Snake[1][1] && G->Snake[1][0] > G->Snake[2][0])){
               if(Move2 == 3) {
                    Move2 = 2;
                    Move4 = 3;
                    Move6 = 3;
                    Move8 = 3;
                    if(G->Snake[0][0] < G->Snake[1][0] && G->Snake[1][0] < G->Snake[2][0]){
                        G->Snake[2][0]--;
                        G->Snake[1][0]--;
                        G->Snake[0][1]++;
                    }else{
                        G->Snake[2][0]++;
                        G->Snake[1][0]++;
                        G->Snake[0][1]++;
                    }
                }else if(Move2 == 2) {
                    Move2 = 1;
                    if(G->Snake[0][1] > G->Snake[1][1] && G->Snake[1][0] < G->Snake[2][0]){
                        G->Snake[2][0]--;
                        G->Snake[1][1]++;
                        G->Snake[0][1]++;                       
                    }else{
                        G->Snake[2][0]++;
                        G->Snake[1][1]++;
                        G->Snake[0][1]++;
                    }
                }
            }else if(G->Snake[0][0] == G->Snake[1][0] && G->Snake[1][0] == G->Snake[2][0]){
                G->Snake[2][1]++;
                G->Snake[1][1]++;
                G->Snake[0][1]++;
                Move2 = 3;
            }
        }
        else if(G->Move == 8) {
            if((G->Snake[0][0] > G->Snake[1][0] && G->Snake[1][0] > G->Snake[2][0])
                || (G->Snake[0][1] < G->Snake[1][1] && G->Snake[1][0] > G->Snake[2][0])
                || (G->Snake[0][0] < G->Snake[1][0] && G->Snake[1][0] < G->Snake[2][0])
                || (G->Snake[0][1] < G->Snake[1][1] && G->Snake[1][0] < G->Snake[2][0])){
               if(Move8 == 3) {
                    Move8 = 2;
                    Move2 = 3;
                    Move4 = 3;
                    Move6 = 3;
                    if(G->Snake[0][0] < G->Snake[1][0] && G->Snake[1][0] < G->Snake[2][0]){
                        G->Snake[2][0]--;
                        G->Snake[1][0]--;
                        G->Snake[0][1]--;                        
                    }else{
                        G->Snake[2][0]++;
                        G->Snake[1][0]++;
                        G->Snake[0][1]--;
                    }
                }else if(Move8 == 2) {
                    Move8 = 1;
                    if(G->Snake[0][1] < G->Snake[1][1] && G->Snake[1][0] < G->Snake[2][0]){
                        G->Snake[2][0]--;
                        G->Snake[1][1]--;
                        G->Snake[0][1]--;                        
                    }else{
                        G->Snake[2][0]++;
                        G->Snake[1][1]--;
                        G->Snake[0][1]--;
                    }
                }
            }else if(G->Snake[0][1] <= G->Snake[1][1] && G->Snake[1][1] <= G->Snake[2][1]){
                G->Snake[2][1]--;
                G->Snake[1][1]--;
                G->Snake[0][1]--;
                Move8 = 3;
            }
        }
        else if(G->Move == 4) {
            if((G->Snake[0][1] > G->Snake[1][1] && G->Snake[1][1] > G->Snake[2][1])
                || (G->Snake[0][0] < G->Snake[1][0] && G->Snake[1][1] > G->Snake[2][1])
                || (G->Snake[0][1] < G->Snake[1][1] && G->Snake[1][1] < G->Snake[2][1])
                || (G->Snake[0][0] < G->Snake[1][0] && G->Snake[1][1] < G->Snake[2][1])){
               if(Move4 == 3) {
                    Move4 = 2;
                    Move2 = 3;
                    Move6 = 3;
                    Move8 = 3;
                    if(G->Snake[0][1] < G->Snake[1][1] && G->Snake[1][1] < G->Snake[2][1]){
                        G->Snake[2][1]--;
                        G->Snake[1][1]--;
                        G->Snake[0][0]--;                        
                    }else{
                        G->Snake[2][1]++;
                        G->Snake[1][1]++;
                        G->Snake[0][0]--;
                    }
                }else if(Move4 == 2) {
                    Move4 = 1;
                    if(G->Snake[0][0] < G->Snake[1][0] && G->Snake[1][1] < G->Snake[2][1]){
                        G->Snake[2][1]--;
                        G->Snake[1][0]--;
                        G->Snake[0][0]--;                    
                    }else{
                        G->Snake[2][1]++;
                        G->Snake[1][0]--;
                        G->Snake[0][0]--;
                    }
                }
            }else if(G->Snake[0][1] == G->Snake[1][1] && G->Snake[1][1] == G->Snake[2][1]){
                G->Snake[2][0]--;
                G->Snake[1][0]--;
                G->Snake[0][0]--;
                Move4 = 3;
            }
        }
        else if(G->Move == 6) {
            if((G->Snake[0][1] > G->Snake[1][1] && G->Snake[1][1] > G->Snake[2][1])
                || (G->Snake[0][0] > G->Snake[1][0] && G->Snake[1][1] > G->Snake[2][1])
                || (G->Snake[0][1] < G->Snake[1][1] && G->Snake[1][1] < G->Snake[2][1])
                || (G->Snake[0][0] > G->Snake[1][0] && G->Snake[1][1] < G->Snake[2][1])){
               if(Move6 == 3) {
                    Move6 = 2;
                    Move2 = 3;
                    Move4 = 3;
                    Move8 = 3;
                    if(G->Snake[0][1] < G->Snake[1][1] && G->Snake[1][1] < G->Snake[2][1]){
                        G->Snake[2][1]--;
                        G->Snake[1][1]--;
                        G->Snake[0][0]++;         
                    }else{
                        G->Snake[2][1]++;
                        G->Snake[1][1]++;
                        G->Snake[0][0]++;
                    }
                }else if(Move6 == 2) {
                    Move6 = 1;
                    if(G->Snake[0][0] > G->Snake[1][0] && G->Snake[1][1] < G->Snake[2][1]){
                        G->Snake[2][1]--;
                        G->Snake[1][0]++;
                        G->Snake[0][0]++;
                    }else{
                        G->Snake[2][1]++;
                        G->Snake[1][0]++;
                        G->Snake[0][0]++;
                    }
                }
            }else if(G->Snake[0][1] == G->Snake[1][1] && G->Snake[1][1] == G->Snake[2][1]){
                G->Snake[2][0]++;
                G->Snake[1][0]++;
                G->Snake[0][0]++;
                Move6 = 3;
            }
        }
    } else {
        G->Idx = WAIT;
    }
    G->ChangeState(G);
}

void _cal_axis(void *arg){
    game_t *G = arg;
    G->SnakeOld[0][0] = G->Snake[0][0]; 
    G->SnakeOld[0][1] = G->Snake[0][1]; 
    G->SnakeOld[1][0] = G->Snake[1][0];
    G->SnakeOld[1][1] = G->Snake[1][1];
    G->SnakeOld[2][0] = G->Snake[2][0];
    G->SnakeOld[2][1] = G->Snake[2][1];
}
void _ChangeState(void *arg) {
    game_t *G = arg;
    menu_tab[G->Idx].PointerFunction(G);
}



