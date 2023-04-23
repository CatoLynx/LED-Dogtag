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

#include "is31fl3743.h"
#include <SPI.h>

SPIClass spi(HSPI);

void is31fl3743_init() {
  pinMode(PIN_SDB, OUTPUT);
  pinMode(PIN_CS, OUTPUT);
  digitalWrite(PIN_SDB, HIGH);
  digitalWrite(PIN_CS, HIGH);
  spi.begin(PIN_SCK, PIN_MISO, PIN_MOSI);

  is31fl3743_reset();

  uint8_t data = 0b00001001; // No shutdown, No OSD, all 11 drivers enabled
  is31fl3743_write(2, 0x00, &data, 1); // Write to configuration register at 0x00

  data = 0b00110011; // 0° phase shift, 2k SW pulldown when off, 2k CS pullup when off
  is31fl3743_write(2, 0x02, &data, 1); // Write to pullup/down register at 0x02
}

void is31fl3743_write(uint8_t page, uint8_t addr, uint8_t* data, uint16_t dataLen) {
  uint8_t firstByte = 0b01010000;
  firstByte |= (page & 0x0F);

  digitalWrite(PIN_CS, LOW);
  spi.beginTransaction(SPISettings());

  spi.write(firstByte);
  spi.write(addr);
  for (uint32_t i = 0; i < dataLen; i++) {
    spi.write(data[i]);
  }

  spi.endTransaction();
  digitalWrite(PIN_CS, HIGH);
}

void is31fl3743_read(uint8_t page, uint8_t addr, uint8_t* data, uint16_t dataLen) {
  uint8_t firstByte = 0b11010000;
  firstByte |= (page & 0x0F);

  digitalWrite(PIN_CS, LOW);
  spi.beginTransaction(SPISettings());

  spi.write(firstByte);
  spi.write(addr);
  for (uint32_t i = 0; i < dataLen; i++) {
    data[i] = spi.transfer(0x00);
  }

  spi.endTransaction();
  digitalWrite(PIN_CS, HIGH);
}

void is31fl3743_reset() {
  uint8_t data = 0xAE; // Reset
  is31fl3743_write(2, 0x2F, &data, 1);
}

void is31fl3743_setGlobalBrightness(uint8_t brightness) {
  is31fl3743_write(2, 0x01, &brightness, 1);
}

void is31fl3743_setPixelScalingFactor(uint8_t pixel, uint8_t scalingFactor) {
  // pixel is the 0-based index of the pixel register to write
  is31fl3743_write(1, pixel+1, &scalingFactor, 1);
}

void is31fl3743_setPixelRGB(uint8_t x, uint8_t y, uint32_t color) {
  // color: uint32_t, 00RRGGBB
  uint8_t i = y * 6 + x;
  // Color order: BGR
  uint8_t data[3] = { (color & 0xFF), ((color >> 8) & 0xFF), ((color >> 16) & 0xFF) };
  is31fl3743_write(0, i*3+1, data, 3); // Write to PWM registers
}