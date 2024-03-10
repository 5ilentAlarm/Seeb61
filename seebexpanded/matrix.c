/*
    Ararat Mesrobian
    2023
    ararat.mesrobian@gmail.com


    Based on the code from the Ferris split keyboard. 
*/

#include "matrix.h"
#include "debug.h"
#include "wait.h"
#include "i2c_master.h"

extern i2c_status_t mcp23017_status; //Check if MCP23017 is connected
#define I2C_TIMEOUT 1000 //set delay period during I2C transactions

#define I2C_ADDR 0b00100000
#define I2C_ADDR_WRITE ((I2C_ADDR << 1) | I2C_WRITE)
#define I2C_ADDR_READ ((I2C_ADDR << 1) | I2C_READ)

//IO Register addresses to send through I2C to expander to communicate with the different pins
#define IODIRA 0x00  // i/o direction register
#define IODIRB 0x01
#define GPPUA 0x0C  // GPIO pull-up resistor register
#define GPPUB 0x0D
#define GPIOA 0x12  // general purpose i/o port register (write modifies OLAT)
#define GPIOB 0x13

bool i2c_initialized = 0; //boolean to communicate that the protocol is set or not
i2c_status_t mcp23017_status = I2C_ADDR;

uint8_t init_mcp23017(void)
{
    print("starting init"); //for debugging, need to enable console
    mcp23017_status = I2C_ADDR;

    //Init i2c system
    if(i2c_initialized == 0) //if its not on
    {
        i2c_init(); //turn it on, uses normal pro micro pins for I2C
        i2c_initialized = true; //turn it on
        wait_ms(I2C_TIMEOUT);
    }

    uint8_t buf[] = {IODIRA, 0b11111111, 0b11110000};
    mcp23017_status = i2c_transmit(I2C_ADDR_WRITE, buf, sizeof(buf), I2C_TIMEOUT); //will return I2C_STATUS_SUCCESS if good
    if(!mcp23017_status) //if succeeded, 
    {
        uint8_t pullup_buf[] = {GPPUA, 0b11111111, 0b11110000};
        mcp23017_status = i2c_transmit(I2C_ADDR_WRITE, pullup_buf, sizeof(pullup_buf), I2C_TIMEOUT);
    }
    return mcp23017_status;
}

static matrix_row_t matrix[MATRIX_ROWS];

static matrix_row_t read_cols(uint8_t row);
static void init_cols(void);
static void unselect_rows(void);
static void select_row(uint8_t row);

static uint8_t mcp23017_reset_loop;

void matrix_init_custom(void)
{
    mcp23017_status = init_mcp23017();

    unselect_rows();
    init_cols();

    for(uint8_t i = 0; i < MATRIX_ROWS; i++)
    {
        matrix[i] = 0; //all keys in the matrix will be off
    }
}

void matrix_power_up(void)
{
    mcp23017_status = init_mcp23017();

    unselect_rows();
    init_cols();

    //initialize matrix state: all keys off
    for(uint8_t i = 0; i < MATRIX_ROWS; i++)
    {
        matrix[i] = 0; //on power up all will be off
    }
}

//read the row and detects change
static inline bool store_matrix_row(matrix_row_t current_matrix[], uint8_t index)
{
    matrix_row_t temp = read_cols(index);
    if(current_matrix[index] != temp)
    {
        current_matrix[index] = temp;
        return true;
    }
    return false;
}

bool matrix_scan_custom(matrix_row_t current_matrix[])
{
    if(mcp23017_status)
    {
        if(++mcp23017_reset_loop == 0)
        {
            dprint("trying to reset expander\n");
            mcp23017_status = init_mcp23017();
            if(mcp23017_status)
            {
                dprint("Expander not responding\n");
            }
            else
            {
                dprint("Expander connected\n");
            }
        }
    }

    bool changed = false;

    for(uint8_t i = 0; i < MATRIX_ROWS_PER_SIDE; i++)
    {
        uint8_t left_index = i;
        uint8_t right_index = i + MATRIX_ROWS_PER_SIDE; //Matrix_rows_per_side is splitting keys connected to the pro micro vs. expander
        select_row(left_index);
        select_row(right_index);

        changed |= store_matrix_row(current_matrix, left_index);
        changed |= store_matrix_row(current_matrix, right_index);

        unselect_rows();
    }

    return changed;
}

static void init_cols(void)
{
    pin_t matrix_col_pins_mcu[MATRIX_COLS_PER_SIDE] = MATRIX_COL_PINS_MCU;
    for(int pin_index = 0; pin_index < MATRIX_COLS_PER_SIDE; pin_index++)
    {
        pin_t pin = matrix_col_pins_mcu[pin_index];
        setPinInput(pin);
        writePinHigh(pin);
    }
}

static matrix_row_t read_cols(uint8_t row)
{
    if(row < MATRIX_ROWS_PER_SIDE)
    {
        pin_t matrix_col_pins_mcu[MATRIX_COLS_PER_SIDE] = MATRIX_COL_PINS_MCU;
        matrix_row_t current_row_value = 0;

        for(uint8_t col_index = 0; col_index < MATRIX_COLS_PER_SIDE; col_index++)
        {
            uint8_t pin_state = readPin(matrix_col_pins_mcu[col_index]);
            current_row_value |= pin_state ? 0 : (MATRIX_ROW_SHIFTER << col_index);
        }
        return current_row_value;
    }
    else
    {
        if(mcp23017_status) //if there was an error
        {
            return 0;
        }
        else
        {
            uint8_t buf[] = {GPIOA};
            mcp23017_status = i2c_transmit(I2C_ADDR_WRITE, buf, sizeof(buf), I2C_TIMEOUT);
            uint8_t data[] = {0};
            if(!mcp23017_status)
            {
                mcp23017_status = i2c_receive(I2C_ADDR_READ, data, sizeof(data), I2C_TIMEOUT);
                data[0] = ~(data[0]);
            }
            return data[0];
        }
    }
}

static void unselect_rows(void)
{
    pin_t matrix_row_pins_mcu[MATRIX_ROWS_PER_SIDE] = MATRIX_ROW_PINS_MCU;
    for(int pin_index = 0; pin_index < MATRIX_ROWS_PER_SIDE; pin_index++)
    {
        pin_t pin = matrix_row_pins_mcu[pin_index];
        setPinInput(pin);
        writePinLow(pin);
    }
}

static void select_row(uint8_t row)
{
    if(row < MATRIX_ROWS_PER_SIDE)
    {
        pin_t matrix_row_pins_mcu[MATRIX_ROWS_PER_SIDE] = MATRIX_ROW_PINS_MCU;
        pin_t pin = matrix_row_pins_mcu[row];
        setPinOutput(pin);
        writePinLow(pin);
    }
    else
    {
        if(mcp23017_status) //error
        {}
        else
        {
            uint8_t buf[] = {GPIOB, 0xFF & ~(1 << (row - MATRIX_ROWS_PER_SIDE))};
            mcp23017_status = i2c_transmit(I2C_ADDR_WRITE, buf, sizeof(buf), I2C_TIMEOUT);
        }
    }
}