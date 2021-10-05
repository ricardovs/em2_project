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
typedef enum Device_sound{
    DEVICE_SOUND_C,  //Do
    DEVICE_SOUND_D,  //Re
    DEVICE_SOUND_E   //Mi
}Device_sound;

extern void MUSIC_DEVICE_INIT();
extern void MUSIC_DEVICE_PLAY();
extern void MUSIC_DEVICE_STOP();
extern void MUSIC_DEVICE_SET_SOUND(const Device_sound);
extern int MUSIC_DEVICE_IS_PLAYING();

//Debug interface
extern void OUT_PRINT(const char *const s);
#endif