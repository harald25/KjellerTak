#include "utility_functions.h"

void updateLEDs()
{
  if(update)
  {
    if ((millis() - last_update) > interval) // time to update
    {
      last_update = millis();
      switch (active_program)
      {
        case NONE:
          break;
        case CUSTOM_LAMP:
          customLampUpdate();
          break;
        case BLINK:
          blinkUpdate();
          break;
        case CYCLE:
          cycleUpdate();
          break;
        case THEATER_CHASE:
          theaterChaseUpdate();
          break;
        default:
          break;
      }
    }
  }
}

// Increment the Index and reset at the end
void incrementIndex(int *program_index1, uint16_t *total_steps1, int *increment_by1)
{
  *program_index1 = *program_index1 + *increment_by1;

  if (*program_index1 >= *total_steps1)
  {
    *program_index1 = 0;
  }
}

void setSlope(float s) {
  slope = s;
}

void setInterval(uint8_t i) {
  interval = i;
}

void setHue1(uint8_t h) {
  hue1 = h;
}

void setSaturation1(uint8_t s) {
  saturation1 = s;
}

void setValue1(uint8_t v) {
  value1 = v;
}

void setColor1(uint8_t x, uint8_t y)
{
  saturation1 = x;
  hue1 = y;
}

void setBlinkRandomTime(uint32_t x)
{
  blink_random_time = x;
}

void setActivePalette(int x) {
  active_palette = blink_palette_array[x];
}

void changeLEDProgram(OSCMessage &msg, int addrOffset )
{
  update = true;

  if(msg.fullMatch("/Program/custom_lamp"))
  {
    customLamp();
    Serial.println("Activated the program Custom Lamp");
  }

  if(msg.fullMatch("/Program/blink"))
  {
    blink();
    Serial.println("Activated the program Blink");
  }

  if(msg.fullMatch("/Program/cycle"))
  {
    cycle();
    Serial.println("Activated the program Cycle");
  }
}

void changeValue(OSCMessage &msg, int addrOffset )
{
  update = true;

  if (msg.fullMatch("/Variable/delta"))
  {
    float value = msg.getFloat(0);
    delta = (int)value;
  }

  if (msg.fullMatch("/Variable/slope"))
  {
    float value = msg.getFloat(0);
    Serial.println(value);
    setSlope(value);
  }

  if (msg.fullMatch("/Variable/interval"))
  {
    float value = msg.getFloat(0);
    setInterval((int)value);
  }

  if (msg.fullMatch("/Variable/value1"))
  {
    float value = msg.getFloat(0);
    setValue1((int)value);
  }
  if (msg.fullMatch("/Variable/saturation1"))
  {
    float saturation = msg.getFloat(0);
    setSaturation1((int)saturation);
  }
  if (msg.fullMatch("/Variable/hue1"))
  {
    float hue = msg.getFloat(0);
    setHue1((int)hue);
  }

  if (msg.match("/Variable/lamp"))
  {
    float lamp = msg.getFloat(0);
    activateDeactivateLamp(uint8_t(lamp));
  }

  if (msg.fullMatch("/Variable/color1"))
  {
    float x = msg.getFloat(0);
    float y = msg.getFloat(1);
    setColor1((int)x,(int)y);
  }

  //Random interval for blink
  if (msg.fullMatch("/Variable/blinkrandomtime"))
  {
    float value = msg.getFloat(0);
    setBlinkRandomTime((int)value);
  }

  if (msg.fullMatch("/Variable/palette"))
  {
    int x = (int)msg.getFloat(0);
    if (x > -1) {
      setActivePalette(x);
    }
  }
}
