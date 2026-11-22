#ifndef lights_h
#define lights_h

#include "Arduino.h"
#include "config.h"

class Lights {
private:
public:
  int lamp1co[4];
  int lamp2co[4];
  int lamp3co[4];
  int lamp4co[4];
  int lamp5co[4];
  int lamp6co[4];

  bool lamp1enabled = LOW;
  bool lamp1state = LOW;
  int lamp1brightness = 0;
  bool lamp1OnOff = LOW;
  String lamp1name = "";

  bool lamp2enabled = LOW;
  bool lamp2state = LOW;
  int lamp2brightness = 0;
  bool lamp2OnOff = LOW;
  String lamp2name = "";

  bool lamp3enabled = LOW;
  bool lamp3state = LOW;
  int lamp3brightness = 0;
  bool lamp3OnOff = LOW;
  String lamp3name = "";

  bool lamp4enabled = LOW;
  bool lamp4state = LOW;
  int lamp4brightness = 0;
  bool lamp4OnOff = LOW;
  String lamp4name = "";

  bool lamp5enabled = LOW;
  bool lamp5state = LOW;
  int lamp5brightness = 0;
  bool lamp5OnOff = LOW;
  String lamp5name = "";

  bool lamp6enabled = LOW;
  bool lamp6state = LOW;
  int lamp6brightness = 0;
  bool lamp6OnOff = LOW;
  String lamp6name = "";

  bool lamp1change = LOW;
  bool lamp2change = LOW;
  bool lamp3change = LOW;
  bool lamp4change = LOW;
  bool lamp5change = LOW;
  bool lamp6change = LOW;

  void reconfigureCoordinates();
};

#endif