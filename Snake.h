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

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>
#include <stdbool.h>

#define LOGA(fmt,...)  printf("[%s][%d]: "fmt"", __FUNCTION__, __LINE__, ##__VA_ARGS__);

#define UP                              8
#define DOWN                            2
#define LEFT                            4
#define RIGHT                           6

#define BURN_INIT                       3
#define EOK  1
#define ERR  0


typedef struct {
    void (*PointerFunction)(void*);
}menu_t;

typedef struct{
    uint32_t x;
    uint32_t y;
}axis_t;

typedef struct {
    void (*menu_update)(void *);
    void (*menu_wait)(void *);
    void (*menu_fruit)(void *);
    void (*Initial)(void *);
    void (*FMove)(void *);
    void (*MenuGame)(void *);
    int  (*FindPos)(void *, int , int );
    void (*ChangeState)(void *);
    axis_t   *axis[100];
    uint32_t X;
    uint32_t Y;
    uint8_t  Idx;
    uint32_t CountUpdate;
    uint32_t Move;
    char     exit;
    uint32_t Fruit;
    uint8_t  FruitX;
    uint8_t  FruitY;
    uint32_t Point;
    uint8_t  i;
    uint16_t LenCurrent;
    uint32_t Position;
    uint8_t  StartGame;
    uint32_t Choose;
    bool     DetectOutFlag;
    bool     WelcomeFlag;
    bool     StateFlag;
}game_t;

typedef enum {
    UPDATE,
    WAIT,
    INITAL,
    MENU
}game_enum;
