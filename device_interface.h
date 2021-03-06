#ifndef DEVICE_INTERFACE_H
#define DEVICE_INTERFACE_H

//Dial interface
extern void DIAL_DEVICE_INIT();
extern int DIAL_DEVICE_GET_POSITION();

//Display interface
extern void DISPLAY_DEVICE_INIT();
extern void DISPLAY_DEVICE_WRITE_STRING(char *s);

#endif