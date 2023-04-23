/*
  Copyright (C) 2023 Julian Metzler

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <Arduino.h>

#define PIN_SDB 18
#define PIN_CS 5
#define PIN_SCK 19
#define PIN_MOSI 21
#define PIN_MISO 22

void is31fl3743_init();
void is31fl3743_write(uint8_t page, uint8_t addr, uint8_t* data, uint16_t dataLen);
void is31fl3743_read(uint8_t page, uint8_t addr, uint8_t* data, uint16_t dataLen);
void is31fl3743_reset();
void is31fl3743_setGlobalBrightness(uint8_t brightness);
void is31fl3743_setPixelScalingFactor(uint8_t pixel, uint8_t scalingFactor);
void is31fl3743_setPixelRGB(uint8_t x, uint8_t y, uint32_t color);