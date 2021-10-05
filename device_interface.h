#ifndef DEVICE_INTERFACE_H
#define DEVICE_INTERFACE_H

//Dial interface
extern void DIAL_DEVICE_INIT();
extern int DIAL_DEVICE_GET_POSITION();

//Display interface
extern void DISPLAY_DEVICE_INIT();
extern void DISPLAY_DEVICE_WRITE_STRING(char *const s);
extern void DISPLAY_DEVICE_CLEAR();

//Sound interface
extern void MUSIC_INIT();
extern void MUSIC_PLAY_FINAL_SOUND();
extern void MUSIC_PLAY_BEEP_SOUND();
extern void MUSIC_STOP();
extern int MUSIC_IS_PLAYING();

//Debug interface
extern void OUT_PRINT(const char *const s);
#endif