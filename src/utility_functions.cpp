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
        case SCANNER:
          scannerUpdate();
          break;
        default:
          break;
      }
    }
  }
}

// Increment the Index and reset at the end
void incrementIndex(int *program_index, uint16_t *total_steps, int *increment_by)
{
  if (dir == FORWARD)
  {
    *program_index = *program_index + *increment_by;
    if (*program_index >= *total_steps)
    {
      *program_index = 0;
    }
  }
  if (dir == REVERSE)
  {
    *program_index = *program_index - *increment_by;
    if (*program_index <= 0)
    {
      *program_index = *total_steps;
    }
  }
}

void setSlope(float s) {
  slope = s;
}

void setInterval(uint8_t i) {
  interval = i;
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

  if(msg.fullMatch("/Program/theater_chase"))
  {
    theaterChase();
    Serial.println("Activated the program Theater Chase");
  }

  if(msg.fullMatch("/Program/scanner"))
  {
    scanner();
    Serial.println("Activated the program Scanner");
  }
}

void changeValue(OSCMessage &msg, int addrOffset )
{

  if (msg.fullMatch("/Variable/delta"))
  {
    float value = msg.getFloat(0);
    delta = (int)value;
    update = true;
  }

  if (msg.fullMatch("/Variable/slope"))
  {
    float value = msg.getFloat(0);
    Serial.println(value);
    setSlope(value);
    update = true;
  }

  if (msg.fullMatch("/Variable/interval"))
  {
    float value = msg.getFloat(0);
    setInterval((int)value);
    update = true;
  }

  if (msg.fullMatch("/Variable/value1"))
  {
    uint8_t value = (uint8_t)msg.getFloat(0);
    value1 = value;
    update = true;
  }
  if (msg.fullMatch("/Variable/value2"))
  {
    uint8_t value = (uint8_t)msg.getFloat(0);
    value2 = value;
    update = true;
  }
  if (msg.fullMatch("/Variable/saturation1"))
  {
    uint8_t saturation = (uint8_t)msg.getFloat(0);
    saturation1 = saturation;
    update = true;
  }
  if (msg.fullMatch("/Variable/saturation2"))
  {
    uint8_t saturation = (uint8_t)msg.getFloat(0);
    saturation2 = saturation;
    update = true;
  }
  if (msg.fullMatch("/Variable/hue1"))
  {
    uint8_t hue = (uint8_t)msg.getFloat(0);
    hue1 = hue;
    update = true;
  }
  if (msg.fullMatch("/Variable/hue2"))
  {
    uint8_t hue = (uint8_t)msg.getFloat(0);
    hue2 = hue;
    update = true;
  }

  if (msg.match("/Variable/lamp"))
  {
    float lamp = msg.getFloat(0);
    activateDeactivateLamp(uint8_t(lamp));
    update = true;
  }

  if (msg.fullMatch("/Variable/color1"))
  {
    uint8_t x = (uint8_t)msg.getFloat(0);
    uint8_t y = (uint8_t)msg.getFloat(1);

    saturation1 = x;
    hue1 = y;

    update = true;
  }

  if (msg.fullMatch("/Variable/color2"))
  {
    uint8_t x = (uint8_t)msg.getFloat(0);
    uint8_t y = (uint8_t)msg.getFloat(1);

    saturation2 = x;
    hue2 = y;

    update = true;
  }

  //Random interval for blink
  if (msg.fullMatch("/Variable/blinkrandomtime"))
  {
    float value = msg.getFloat(0);
    setBlinkRandomTime((int)value);
    update = true;
  }

  if (msg.fullMatch("/Variable/palette"))
  {
    int x = (int)msg.getFloat(0);
    if (x > -1) {
      setActivePalette(x);
    }
    else {
      setActivePalette(0);
    }
    update = true;
  }

  if (msg.fullMatch("/Variable/chase_mode"))
  {
    uint8_t x = (uint8_t)msg.getFloat(0);
    if (x > -1)
    {
      if (x == 1) {
        c_mode = REGULAR;
      }
      else if (x == 2) {
        c_mode = RAINBOW_CHASE;
      }
    }
    update = true;
  }

  if (msg.fullMatch("/Variable/pixel_distance"))
  {
    uint8_t x = (uint8_t)msg.getFloat(0);
    pixel_distance = x;
    update = true;
  }

  if (msg.fullMatch("/Variable/pixel_width"))
  {
    uint8_t x = (uint8_t)msg.getFloat(0);
    pixel_width = x;
    update = true;
  }

  if (msg.fullMatch("/Variable/direction"))
  {
    uint8_t x = (uint8_t)msg.getFloat(0);
    Serial.print("Direction: ");
    Serial.println(x);
    if (x == 1) {
      dir = FORWARD;
    }
    if (x == 2) {
      dir = REVERSE;
    }
    update = true;
  }

  if (msg.fullMatch("/Variable/stopstart"))
  {
    update = 1-update;
    FastLED.delay(10);
    Serial.print("Update: ");
    Serial.println(update);
  }

  if (msg.fullMatch("/Variable/tail_length"))
  {
    uint8_t x = (uint8_t)msg.getFloat(0);
    tail_length = x;
    update = true;
  }

}
