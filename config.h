#include "Arduino.h"
/////////////////////////////////////// App Toggles
#define ENABLE_MOWER
#define ENABLE_CAR
#define ENABLE_VENTILATION
#define ENABLE_HEAT_PUMP
#define ENABLE_MUSIC
#define ENABLE_WASTE
#define ENABLE_LIGHTS
#define ENABLE_ENERGY
#define ENABLE_evcc
#define ENABLE_PRICES
#define ENABLE_RELAYS

/////////////////////////////////////// RELAYS
#ifdef ENABLE_RELAYS
// Comment out the lines below to disable specific relays, but only disable in
// order from the last one to the first one.
#define RELAY1_ENABLED
#define RELAY2_ENABLED
#define RELAY3_ENABLED
// #define RELAY4_ENABLED
// #define RELAY5_ENABLED
// #define RELAY6_ENABLED

// No need to change anything below this line unless you know what you're doing.
extern const char *relay1;
extern const char *relay1type;
extern const char *relay2;
extern const char *relay2type;
extern const char *relay3;
extern const char *relay3type;
extern const char *relay4;
extern const char *relay4type;
extern const char *relay5;
extern const char *relay5type;
extern const char *relay6;
extern const char *relay6type;

#ifdef RELAY6_ENABLED
extern const int relay1Co[4];
extern const int relay2Co[4];
extern const int relay3Co[4];
extern const int relay4Co[4];
extern const int relay5Co[4];
extern const int relay6Co[4];
#else
#ifdef RELAY5_ENABLED
extern const int relay1Co[4];
extern const int relay2Co[4];
extern const int relay3Co[4];
extern const int relay4Co[4];
extern const int relay5Co[4];
#else
#ifdef RELAY4_ENABLED
extern const int relay1Co[4];
extern const int relay2Co[4];
extern const int relay3Co[4];
extern const int relay4Co[4];
#else
#ifdef RELAY3_ENABLED
extern const int relay1Co[4];
extern const int relay2Co[4];
extern const int relay3Co[4];
#else
#ifdef RELAY2_ENABLED
extern const int relay1Co[4];
extern const int relay2Co[4];
#else
#ifdef RELAY1_ENABLED
extern const int relay1Co[4];
#endif
#endif
#endif
#endif
#endif
#endif
#endif