#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <sys/time.h>
#include <time.h>
#include "hsm_event.h"
#include "dial_hsm.h"
#include "top_hsm.h"
#include "utils_hsm.h"


static unsigned dial_position = 0;

int DIAL_DEVICE_GET_POSITION(){
    return dial_position;
}

void DIAL_DEVICE_INIT(){
    return;
}

void UPDATE_DIAL_POSITION(const unsigned p){
    dial_position = p;
}

void INCREMENT_DIAL_POSITION(){
    dial_position++;
}
void DECREMENT_DIAL_POSITION(){
    dial_position--;
}

void print_event(HsmEvent * event){
    printw("%s <--\n", GET_EVENT_NAME(*event));
    refresh();
}

void delay(int milisec){
    struct timeval start, end, diff;
    int time_passed;
    gettimeofday(&start, NULL);
    do{
        gettimeofday(&end, NULL);
        timersub(&end, &start, &diff);
        time_passed = (int)(diff.tv_sec*1000) + (int)(diff.tv_usec/1000);
    }while(time_passed < milisec);
}

int main(){
    Top_hsm top;
    INIT_TOP_HSM(&top, &print_event);
    int N=10;
    char c, cmd,cmds[N];
    unsigned nticks[N];
    struct timeval start, end, diff;
    initscr();
    noecho();
    scrollok(stdscr, TRUE);
    keypad(stdscr, TRUE);
    do{
        erase();
        refresh();
        printw("Ready to receive commands. Commands list:\n\t 'i' -> Increment Dial\n\t 'd'-> Decrement Dial.\n\t 'c' -> Run comands passed and ask for more\n\t 'q' -> Execute previous comands and exit simulator.\nEnter first command.\n");
        refresh();
        int i = 0;
        cmd = getch();
        while((i<N) && (cmd != 'q') && (cmd != 'c')){
            gettimeofday(&start, NULL);
            cmds[i]=cmd;
            cmd = getch();
            gettimeofday(&end, NULL);
            timersub(&end, &start, &diff);
            nticks[i] = (unsigned)(diff.tv_sec*1000) + (unsigned)(diff.tv_usec/1000);
            i++;
        };
        if((cmd != 'q') || (i>0)){
            erase();
            refresh();
            if(!(i<N)){
                printw("Many commands. Taking a simulation step.\nPress Enter.\n");
                refresh();
                c = getch();
                while(c != 0xa){
                    c = getch();
                }
            }else{
                if(cmd != 'c'){
                    printw("Simulation Step. Press Enter\n");
                    c = getch();
                    while(c != 0xa){
                        c = getch();
                    }
                }else{
                    printw("Simulation step...\n");
                }
            }
            refresh();
            for(int j=0;j<i;j++){
                for(int k=0; k<(int)nticks[j]; k++){
                    TOP_RUN_ALL_HSMS(&top);
                    delay(1);
                }
                if(cmds[j] == 'i'){
                    INCREMENT_DIAL_POSITION();
                    printw("--> INCREMENT_DIAL\n");
                    refresh();
                }
                if(cmds[j] == 'd'){
                    DECREMENT_DIAL_POSITION();
                    printw("--> DECREMENT_DIAL\n");
                    refresh();
                }
            }
            refresh();
            printw("Simulation step done.\n");
            refresh();
            getch();
        }
    }while(cmd != 'q');
    printw("Done.\n");
    refresh();
    delay(1200);
    endwin();
    return 0;
}