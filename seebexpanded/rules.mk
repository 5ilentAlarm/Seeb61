#Build options
#Based off Ferris
BOOTMAGIC_ENABLE = no
BOOTMAGIC_ENABLE = no       # Enable Bootmagic Lite
MOUSEKEY_ENABLE = yes       # Mouse keys
EXTRAKEY_ENABLE = yes       # Audio control and System control
CONSOLE_ENABLE = yes         # Console for debug
COMMAND_ENABLE = no         # Commands for debug and configuration
NKRO_ENABLE = no            # Enable N-Key Rollover
BACKLIGHT_ENABLE = no       # Enable keyboard backlight functionality
RGBLIGHT_ENABLE = no        # Enable keyboard RGB underglow
AUDIO_ENABLE = no           # Audio output
UNICODE_ENABLE = yes

CUSTOM_MATRIX = lite #When using extra hardware between switches and pins, its important to enable this
#Allows the user to create 

NO_USB_STARTUP_CHECK = yes
LTO_ENABLE = yes

SRC += matrix.c
QUANTUM_LIB_SRC += i2c_master.c


ENCODER_ENABLE = yes
