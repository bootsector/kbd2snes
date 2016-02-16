/*
 * KBD2SNES - PS/2 Keyboard Adapter for SNES
 * Copyright (c) 2016 Bruno Freitas - bruno@brunofreitas.com
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <avr/pgmspace.h>

#include "kbdhandler.h"
#include "snes.h"
#include "ringbuffer.h"

#define NUM_KBD_PROFILES 2

// Profile template (all 256 positions as SNES_NO_ACTION):
// {SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION},

const uint8_t PROGMEM keys_lut[NUM_KBD_PROFILES][256] = {
		// Code:   0x00,           0x01,           0x02,           0x03,           0x04,           0x05,           0x06,           0x07,           0x08,           0x09,           0x0A,           0x0B,           0x0C,           0x0D,           0x0E,           0x0F,           0x10,           0x11,           0x12,           0x13,           0x14,           0x15,           0x16,           0x17,           0x18,           0x19,           0x1A,           0x1B,           0x1C,           0x1D,           0x1E,           0x1F,           0x20,           0x21,           0x22,           0x23,           0x24,           0x25,           0x26,           0x27,           0x28,           0x29,           0x2A,           0x2B,           0x2C,           0x2D,           0x2E,           0x2F,           0x30,           0x31,           0x32,           0x33,           0x34,           0x35,           0x36,           0x37,           0x38,           0x39,           0x3A,           0x3B,           0x3C,           0x3D,           0x3E,           0x3F,           0x40,           0x41,           0x42,           0x43,           0x44,           0x45,           0x46,           0x47,           0x48,           0x49,           0x4A,           0x4B,           0x4C,           0x4D,           0x4E,           0x4F,           0x50,           0x51,           0x52,           0x53,           0x54,           0x55,           0x56,           0x57,           0x58,           0x59,           0x5A,           0x5B,           0x5C,           0x5D,           0x5E,           0x5F,           0x60,           0x61,           0x62,           0x63,           0x64,           0x65,           0x66,           0x67,           0x68,           0x69,           0x6A,           0x6B,           0x6C,           0x6D,           0x6E,           0x6F,           0x70,           0x71,           0x72,           0x73,           0x74,           0x75,           0x76,           0x77,           0x78,           0x79,           0x7A,           0x7B,           0x7C,           0x7D,           0x7E,           0x7F,           0x80,           0x81,           0x82,           0x83,           0x84,           0x85,           0x86,           0x87,           0x88,           0x89,           0x8A,           0x8B,           0x8C,           0x8D,           0x8E,           0x8F,           0x90,           0x91,           0x92,           0x93,           0x94,           0x95,           0x96,           0x97,           0x98,           0x99,           0x9A,           0x9B,           0x9C,           0x9D,           0x9E,           0x9F,           0xA0,           0xA1,           0xA2,           0xA3,           0xA4,           0xA5,           0xA6,           0xA7,           0xA8,           0xA9,           0xAA,           0xAB,           0xAC,           0xAD,           0xAE,           0xAF,           0xB0,           0xB1,           0xB2,           0xB3,           0xB4,           0xB5,           0xB6,           0xB7,           0xB8,           0xB9,           0xBA,           0xBB,           0xBC,           0xBD,           0xBE,           0xBF,           0xC0,           0xC1,           0xC2,           0xC3,           0xC4,           0xC5,           0xC6,           0xC7,           0xC8,           0xC9,           0xCA,           0xCB,           0xCC,           0xCD,           0xCE,           0xCF,           0xD0,           0xD1,           0xD2,           0xD3,           0xD4,           0xD5,           0xD6,           0xD7,           0xD8,           0xD9,           0xDA,           0xDB,           0xDC,           0xDD,           0xDE,           0xDF,           0xE0,           0xE1,           0xE2,           0xE3,           0xE4,           0xE5,           0xE6,           0xE7,           0xE8,           0xE9,           0xEA,           0xEB,           0xEC,           0xED,           0xEE,           0xEF,           0xF0,           0xF1,           0xF2,           0xF3,           0xF4,           0xF5,           0xF6,           0xF7,           0xF8,           0xF9,           0xFA,           0xFB,           0xFC,           0xFD,           0xFE,           0xFF
		{SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION,    SNES_SELECT, SNES_NO_ACTION,     SNES_START, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION,      SNES_DOWN,      SNES_LEFT,        SNES_UP, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION,     SNES_RIGHT, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION,      SNES_LEFT, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION,      SNES_DOWN,        SNES_UP, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION,     SNES_RIGHT, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION,     SNES_START, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION,         SNES_Y, SNES_NO_ACTION,         SNES_X, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION,         SNES_B,         SNES_A,         SNES_R, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION,         SNES_L, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION},
		{SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION,    SNES_SELECT, SNES_NO_ACTION,     SNES_START, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION,         SNES_Y,         SNES_X,         SNES_L, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION,         SNES_A,         SNES_B,         SNES_R, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION,     SNES_START, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION},
};

const uint8_t PROGMEM ext_keys_lut[NUM_KBD_PROFILES][256] = {
		// Code:   0x00,           0x01,           0x02,           0x03,           0x04,           0x05,           0x06,           0x07,           0x08,           0x09,           0x0A,           0x0B,           0x0C,           0x0D,           0x0E,           0x0F,           0x10,           0x11,           0x12,           0x13,           0x14,           0x15,           0x16,           0x17,           0x18,           0x19,           0x1A,           0x1B,           0x1C,           0x1D,           0x1E,           0x1F,           0x20,           0x21,           0x22,           0x23,           0x24,           0x25,           0x26,           0x27,           0x28,           0x29,           0x2A,           0x2B,           0x2C,           0x2D,           0x2E,           0x2F,           0x30,           0x31,           0x32,           0x33,           0x34,           0x35,           0x36,           0x37,           0x38,           0x39,           0x3A,           0x3B,           0x3C,           0x3D,           0x3E,           0x3F,           0x40,           0x41,           0x42,           0x43,           0x44,           0x45,           0x46,           0x47,           0x48,           0x49,           0x4A,           0x4B,           0x4C,           0x4D,           0x4E,           0x4F,           0x50,           0x51,           0x52,           0x53,           0x54,           0x55,           0x56,           0x57,           0x58,           0x59,           0x5A,           0x5B,           0x5C,           0x5D,           0x5E,           0x5F,           0x60,           0x61,           0x62,           0x63,           0x64,           0x65,           0x66,           0x67,           0x68,           0x69,           0x6A,           0x6B,           0x6C,           0x6D,           0x6E,           0x6F,           0x70,           0x71,           0x72,           0x73,           0x74,           0x75,           0x76,           0x77,           0x78,           0x79,           0x7A,           0x7B,           0x7C,           0x7D,           0x7E,           0x7F,           0x80,           0x81,           0x82,           0x83,           0x84,           0x85,           0x86,           0x87,           0x88,           0x89,           0x8A,           0x8B,           0x8C,           0x8D,           0x8E,           0x8F,           0x90,           0x91,           0x92,           0x93,           0x94,           0x95,           0x96,           0x97,           0x98,           0x99,           0x9A,           0x9B,           0x9C,           0x9D,           0x9E,           0x9F,           0xA0,           0xA1,           0xA2,           0xA3,           0xA4,           0xA5,           0xA6,           0xA7,           0xA8,           0xA9,           0xAA,           0xAB,           0xAC,           0xAD,           0xAE,           0xAF,           0xB0,           0xB1,           0xB2,           0xB3,           0xB4,           0xB5,           0xB6,           0xB7,           0xB8,           0xB9,           0xBA,           0xBB,           0xBC,           0xBD,           0xBE,           0xBF,           0xC0,           0xC1,           0xC2,           0xC3,           0xC4,           0xC5,           0xC6,           0xC7,           0xC8,           0xC9,           0xCA,           0xCB,           0xCC,           0xCD,           0xCE,           0xCF,           0xD0,           0xD1,           0xD2,           0xD3,           0xD4,           0xD5,           0xD6,           0xD7,           0xD8,           0xD9,           0xDA,           0xDB,           0xDC,           0xDD,           0xDE,           0xDF,           0xE0,           0xE1,           0xE2,           0xE3,           0xE4,           0xE5,           0xE6,           0xE7,           0xE8,           0xE9,           0xEA,           0xEB,           0xEC,           0xED,           0xEE,           0xEF,           0xF0,           0xF1,           0xF2,           0xF3,           0xF4,           0xF5,           0xF6,           0xF7,           0xF8,           0xF9,           0xFA,           0xFB,           0xFC,           0xFD,           0xFE,           0xFF
		{SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION,      SNES_LEFT, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION,      SNES_DOWN, SNES_NO_ACTION,     SNES_RIGHT,        SNES_UP, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION},
		{SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION,      SNES_LEFT, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION,      SNES_DOWN, SNES_NO_ACTION,     SNES_RIGHT,        SNES_UP, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION, SNES_NO_ACTION},
};

static int kbd_extended_flag = 0;
static int kbd_key_profile = 0;

static void keyboard_ignore_next_keys(int keys) {
	while(keys--) {
		while(!rb_hasitem());

		rb_get();
	}
}

static void keyboard_make(uint8_t key) {
	if(kbd_extended_flag) {
		switch(key) {
			case 0x12: // PRNT SCRN (Make) E0,12,E0,7C
				keyboard_ignore_next_keys(2);
				break;
			default:
				snes_press(pgm_read_byte(&ext_keys_lut[kbd_key_profile][key]));
		}
	} else {
		switch(key) {
			case 0x05: // F1 - Rotate between keyboard profiles
				snes_reset_output();
				kbd_key_profile = (kbd_key_profile + 1) % NUM_KBD_PROFILES;
				break;
			default:
				snes_press(pgm_read_byte(&keys_lut[kbd_key_profile][key]));
		}
	}
}

static void keyboard_break(void) {
	while(!rb_hasitem());

	uint8_t key = rb_get();

	if(kbd_extended_flag) {
		switch(key) {
			case 0x7C: // PRNT SCRN (Break) E0,F0,7C,E0,F0,12
				keyboard_ignore_next_keys(3);
				break;
			default:
				snes_release(pgm_read_byte(&ext_keys_lut[kbd_key_profile][key]));
		}

	} else {
		switch(key) {
			case 0x05: // F1 key release. Do nothing!
				snes_release(SNES_NO_ACTION);
				break;
			default:
				snes_release(pgm_read_byte(&keys_lut[kbd_key_profile][key]));
		}
	}
}

static void keyboard_extended(void) {
	while(!rb_hasitem());

	kbd_extended_flag = 1;

	uint8_t key = rb_get();

	switch(key) {
		case 0xF0:
			keyboard_break();
			break;
		default:
			keyboard_make(key);
			break;
	}
}

void keyboard_handle_key(uint8_t key) {
	kbd_extended_flag = 0;

	switch(key) {
		case 0xF0:
			keyboard_break();
			break;
		case 0xE0:
			keyboard_extended();
			break;
		case 0xE1: // PAUSE (Make) E1,14,77,E1,F0,14,F0,77
			keyboard_ignore_next_keys(7);
			break;
		default:
			keyboard_make(key);
			break;
	}
}
