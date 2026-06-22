#include "Arduino.h"
// --- App Toggles ---
#define ENABLE_MOWER
#define ENABLE_CAR
#define ENABLE_VENTILATION
#define ENABLE_HEAT_PUMP
#define ENABLE_MUSIC
#define ENABLE_WASTE
#define ENABLE_LIGHTS
#define ENABLE_ENERGY
#define ENABLE_evcc

#ifdef ENABLE_LIGHTS
// Comment out the lines below to disable specific lamps, but only disable in
// order from the last one to the first one.
#define LAMP1_ENABLED
#define LAMP2_ENABLED
#define LAMP3_ENABLED
#define LAMP4_ENABLED
#define LAMP5_ENABLED
#define LAMP6_ENABLED

// Uncomment the lines below if the lamps are on/off switches, comment out if they are dimmable.
#define LAMP1_ON_OFF
// #define LAMP2_ON_OFF
// #define LAMP3_ON_OFF
// #define LAMP4_ON_OFF
#define LAMP5_ON_OFF
// #define LAMP6_ON_OFF

// No need to change anything below this line unless you know what you're doing.
#ifdef LAMP1_ENABLED
extern const char* lamp1;
extern const char* lamp1type;
#endif
#ifdef LAMP2_ENABLED
extern const char* lamp2;
extern const char* lamp2type;
#endif
#ifdef LAMP3_ENABLED
extern const char* lamp3;
extern const char* lamp3type;
#endif
#ifdef LAMP4_ENABLED
extern const char* lamp4;
extern const char* lamp4type;
#endif
#ifdef LAMP5_ENABLED
extern const char* lamp5;
extern const char* lamp5type;
#endif
#ifdef LAMP6_ENABLED
extern const char* lamp6;
extern const char* lamp6type;
#endif

#ifdef LAMP6_ENABLED
extern const int lamp1Co[4];
extern const int lamp2Co[4];
extern const int lamp3Co[4];
extern const int lamp4Co[4];
extern const int lamp5Co[4];
extern const int lamp6Co[4];
#else
#ifdef LAMP5_ENABLED
extern const int lamp1Co[4];
extern const int lamp2Co[4];
extern const int lamp3Co[4];
extern const int lamp4Co[4];
extern const int lamp5Co[4];
#else
#ifdef LAMP4_ENABLED
extern const int lamp1Co[4];
extern const int lamp2Co[4];
extern const int lamp3Co[4];
extern const int lamp4Co[4];
#else
#ifdef LAMP3_ENABLED
extern const int lamp1Co[4];
extern const int lamp2Co[4];
extern const int lamp3Co[4];
#else
#ifdef LAMP2_ENABLED
extern const int lamp1Co[4];
extern const int lamp2Co[4];
#else
#ifdef LAMP1_ENABLED
extern const int lamp1Co[4];
#endif
#endif
#endif
#endif
#endif
#endif
#endif