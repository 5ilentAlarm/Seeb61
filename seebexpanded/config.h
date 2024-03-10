// Copyright 2023 Sebastian (@SebastianMn4345)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

/*
 * Feature disable options
 *  These options are also useful to firmware size reduction.
 */

/* disable debug print */
//#define NO_DEBUG

/* disable print */
//#define NO_PRINT

/* disable action features */
//#define NO_ACTION_LAYER
//#define NO_ACTION_TAPPING
//#define NO_ACTION_ONESHOT
#pragma once

/* key matrix size */
//Seems that for split keyboard, a doubled number of either rows or columns is needed, This situation warrants a doubled number of columns.
#define MATRIX_ROWS 10
#define MATRIX_COLS 14

#define MATRIX_ROWS_PER_SIDE (MATRIX_ROWS / 2)
//#define MATRIX_COLS_PER_SIDE (MATRIX_COLS / 2)
#define MATRIX_COLS_PER_SIDE MATRIX_COLS //2x the columns, scanning is done using a "virtual" column stack-up

// wiring
#define MATRIX_ROW_PINS_MCU \
    { D4, C6, D7, E6, B4 }
#define MATRIX_COL_PINS_MCU \
    { B6, B5, B2, B3, D3, F7, B1 }
#define MATRIX_ROW_PINS_MCP \
    { B0, B1, B2, B3, B4 }
#define MATRIX_COL_PINS_MCP \
    { A0, A1, A2, A3, A4, A5, A6 }

/* COL2ROW, ROW2COL*/
//#define DIODE_DIRECTION COL2ROW
#define DIODE_DIRECTION COL2ROW

//Defining the pins for the encoder
#define ENCODERS_PAD_A { F5 }
#define ENCODERS_PAD_B { F6 }