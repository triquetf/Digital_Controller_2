#ifndef _menu_h_

#define _menu_h_



#include <avr/pgmspace.h>

#include "Main.h"



#ifndef PGM_P

#define PGM_P const rom char *

#endif



typedef struct PROGMEM

{

	unsigned char state;

	unsigned char input;

	unsigned char nextstate;

} MENU_NEXTSTATE;



typedef struct PROGMEM

{

	unsigned char state;

	PGM_P pText;

	char (*pFunc)(char input);

} MENU_STATE;



// Menu text

const char Txt_WELCOME[] PROGMEM                           =            "WELCOME !";

const char Txt_SETUP[] PROGMEM                                    =            "SETUP";

const char Txt_CLOCK[] PROGMEM                                  =            "CLOCK";

const char Txt_SENSOR[] PROGMEM                                =   "SENSOR";

const char Txt_ACTUATOR[] PROGMEM                           =   "ACTUATOR";

const char Txt_STORAGE[] PROGMEM                              =   "DATA STORAGE";

const char Txt_MONITORING[] PROGMEM                      =   "MONITORING";

const char Txt_CONTRAST[] PROGMEM                           =   "CONTRAST";

const char Txt_DEBUG[] PROGMEM                                  =   "DEBUG";

const char Txt_DATALOGGER[] PROGMEM                     =   "DATA LOGGER";

const char Txt_PARAMETERS[] PROGMEM                      =   "PARAMETERS";

const char Txt_ON_OFF[] PROGMEM                                =   "ON / OFF";

const char Txt_TRANSFER[] PROGMEM                            =   "TRANSFER OF DATA";

const char Txt_CONTROL[] PROGMEM                             =   "DIGITAL CONTROL";

const char Txt_MANUAL[] PROGMEM                               =   "MANUAL MODE";

const char Txt_AUTO[] PROGMEM                                      =   "AUTOMATIC MODE";

const char Txt_PWM[] PROGMEM                                       =   "PWM GENERATION";

const char Txt_SINUS_MODE[] PROGMEM                     =   "SINUS MODE";

const char Txt_SINUS[] PROGMEM                                    =   "SINUS GENERATION";

const char Txt_Date[] PROGMEM                                       =   "DATE";

const char Txt_TIME[] PROGMEM                                       =   "TIME";

const char Txt_ADC[] PROGMEM                                        =   "ADC";

const char Txt_CURRENT[] PROGMEM                             =   "CURRENT";

const char Txt_THERMOCOUPLE[] PROGMEM              =            "THERMOCOUPLE";

const char Txt_DAC[] PROGMEM                                        =            "DAC";

const char Txt_PWM_SETUP[] PROGMEM                       =   "PWM SETUP";

const char Txt_RELAY[] PROGMEM                                    =   "RELAY";

const char Txt_PWM_1[] PROGMEM                                  =   "1 PWM";

const char Txt_PWM_2[] PROGMEM                                  =   "2 PWM";

const char Txt_PWM_3[] PROGMEM                                  =   "3 PWM";

const char Txt_TRIANGLE[] PROGMEM                             =            "CHOISE OF TRIANGLE";

const char Txt_DENT_SCIE[] PROGMEM                          =   "DENT DE SCIE";

const char Txt_ISOCELE[] PROGMEM                =   "TRIANGLE ISOCELE";

const char Txt_PERIODE[] PROGMEM                =   "PERIODE";

const char Txt_RESOLUTION[] PROGMEM                      =   "RESOLUTION";

const char Txt_SRAM[] PROGMEM                                     =   "SRAM";

const char Txt_EEPROM[] PROGMEM                               =   "EEPROM";

const char Txt_SAMPLING[] PROGMEM                           =   "SAMPLING TIME";

const char Txt_DURATION[] PROGMEM                           =   "DURATION";

const char Txt_PID[] PROGMEM                                          =   "PID";

const char Txt_RECURSION[] PROGMEM                        =   "RECURSION";

const char Txt_SMITH[] PROGMEM                                    =   "SMITH PREDICTOR";

const char Txt_MODEL[] PROGMEM                                  =   "INTERNAL MODEL";

const char Txt_RST[] PROGMEM                                         =   "RST";

const char Txt_P[] PROGMEM                                              =   "GAIN VALUE";

const char Txt_I[] PROGMEM                                                =   "INTEGRAL VALUE";

const char Txt_D[] PROGMEM                                              =   "DERIVATIVE VALUE";

const char Txt_WINDUP[] PROGMEM                                =   "WINDUP VALUE";

const char Txt_MODEL_RECURSION[] PROGMEM=   "DOWNLOAD REC";

const char Txt_MODEL_SETTING[] PROGMEM              =   "ALPHA VALUE";

const char Txt_AMPLITUDE[] PROGMEM                         =            "AMPLITUDE VALUE";









const MENU_NEXTSTATE menu_nextstate[] PROGMEM = {

	//  STATE                       INPUT       NEXT STATE

	// MENUS

	//HIGH LEVEL

	{ST_TXT_WELCOME,                                   ENTER,                              ST_TXT_SETUP},

	

	// MAIN MENU (HORIZONTAL)

	{ST_TXT_SETUP,                                                          RIGHT,                               ST_TXT_DATALOGGER},

	{ST_TXT_DATALOGGER,                                           RIGHT,                               ST_TXT_CONTROL},

	{ST_TXT_CONTROL,                                    RIGHT,                               ST_TXT_PWM},

	//            {ST_TXT_PWM,                                              RIGHT,                               ST_TXT_SINUS},

	//            {ST_TXT_SINUS,                                                          RIGHT,                               ST_TXT_SETUP},

	

	{ST_TXT_PWM,                                              RIGHT,                               ST_TXT_SETUP},

	

	{ST_TXT_DATALOGGER,                                           LEFT,                                  ST_TXT_SETUP},

	{ST_TXT_CONTROL,                                    LEFT,                                  ST_TXT_DATALOGGER},

	{ST_TXT_PWM,                                              LEFT,                                  ST_TXT_CONTROL},

	//            {ST_TXT_SINUS,                                                          LEFT,                                  ST_TXT_PWM},

	//            {ST_TXT_SETUP,                                                          LEFT,                                  ST_TXT_SINUS},

	

	{ST_TXT_SETUP,                                                          LEFT,                                  ST_TXT_PWM},

	

	// MAIN MENU SETUP (VERTICAL)

	{ST_TXT_SETUP,                                                          ENTER,                              ST_TXT_CLOCK},

	{ST_TXT_CLOCK,                                                         DOWN,                                             ST_TXT_SENSOR},

	{ST_TXT_SENSOR,                                                      DOWN,                                             ST_TXT_ACTUATOR},

	{ST_TXT_ACTUATOR,                                  DOWN,                                             ST_TXT_STORAGE},

	{ST_TXT_STORAGE,                                     DOWN,                                             ST_TXT_MONITORING},

	{ST_TXT_MONITORING,                                            DOWN,                                             ST_TXT_CONTRAST},

	{ST_TXT_CONTRAST,                                  DOWN,                                             ST_TXT_DEBUG},

	{ST_TXT_DEBUG,                                                        DOWN,                                             ST_TXT_CLOCK},

	{ST_TXT_CLOCK,                                                         UP,                                                     ST_TXT_DEBUG},

	{ST_TXT_SENSOR,                                                      UP,                                                     ST_TXT_CLOCK},

	{ST_TXT_ACTUATOR,                                  UP,                                                     ST_TXT_SENSOR},

	{ST_TXT_STORAGE,                                     UP,                                                     ST_TXT_ACTUATOR},

	{ST_TXT_MONITORING,                                            UP,                                                     ST_TXT_STORAGE},

	{ST_TXT_CONTRAST,                                  UP,                                                     ST_TXT_MONITORING},

	{ST_TXT_DEBUG,                                                        UP,                                                     ST_TXT_CONTRAST},

	{ST_TXT_SENSOR,                                                      LEFT,                                  ST_TXT_SETUP},

	{ST_TXT_ACTUATOR,                                  LEFT,                                  ST_TXT_SETUP},

	{ST_TXT_STORAGE,                                     LEFT,                                  ST_TXT_SETUP},

	{ST_TXT_MONITORING,                                            LEFT,                                  ST_TXT_SETUP},

	{ST_TXT_CONTRAST,                                  LEFT,                                  ST_TXT_SETUP},

	{ST_TXT_DEBUG,                                                        LEFT,                                  ST_TXT_SETUP},

	

	

	// MAIN MENU DATALOGGER (VERTICAL)

	{ST_TXT_DATALOGGER,                                           ENTER,                              ST_TXT_PARAMETERS_DL},

	{ST_TXT_PARAMETERS_DL,                      DOWN,                                             ST_TXT_ON_OFF},

	{ST_TXT_ON_OFF,                                                       DOWN,                                             ST_TXT_DATA_TRANSFER},

	{ST_TXT_DATA_TRANSFER,                      DOWN,                                             ST_TXT_PARAMETERS_DL},

	{ST_TXT_DATA_TRANSFER,                      UP,                                                     ST_TXT_ON_OFF},

	{ST_TXT_ON_OFF,                                                       UP,                                                     ST_TXT_PARAMETERS_DL},

	{ST_TXT_PARAMETERS_DL,                      UP,                                                     ST_TXT_DATA_TRANSFER},

	{ST_TXT_PARAMETERS_DL,                      LEFT,                                  ST_TXT_SETUP},

	{ST_TXT_ON_OFF,                                                       LEFT,                                  ST_TXT_SETUP},

	{ST_TXT_DATA_TRANSFER,                      LEFT,                                  ST_TXT_SETUP},

	

	// MAIN MENU DIGITAL CONTROL (VERTICAL)

	{ST_TXT_CONTROL,                                    ENTER,                              ST_TXT_PARAMETERS_CTRL},

	{ST_TXT_PARAMETERS_CTRL,  DOWN,                                             ST_TXT_SAMPLING_CTRL},

	{ST_TXT_SAMPLING_CTRL,                      DOWN,                                             ST_TXT_MANUAL_MODE},

	{ST_TXT_MANUAL_MODE,                        DOWN,                                             ST_TXT_AUTO_MODE},

	{ST_TXT_AUTO_MODE,                                             DOWN,                                             ST_TXT_PARAMETERS_CTRL},

	{ST_TXT_PARAMETERS_CTRL,  UP,                                                     ST_TXT_AUTO_MODE},

	{ST_TXT_SAMPLING_CTRL,                      UP,                                                     ST_TXT_PARAMETERS_CTRL},

	{ST_TXT_AUTO_MODE,                                             UP,                                                     ST_TXT_MANUAL_MODE},

	{ST_TXT_MANUAL_MODE,                        UP,                                                     ST_TXT_SAMPLING_CTRL},

	{ST_TXT_PARAMETERS_CTRL,  LEFT,                                  ST_TXT_SETUP},

	{ST_TXT_SAMPLING_CTRL,                      LEFT,                                  ST_TXT_SETUP},

	{ST_TXT_AUTO_MODE,                                             LEFT,                                  ST_TXT_SETUP},

	{ST_TXT_MANUAL_MODE,                        LEFT,                                  ST_TXT_SETUP},

	

	// MAIN MENU PWM GENERATION (VERTICAL)

	{ST_TXT_PWM,                                              ENTER,                                             ST_TXT_MANUAL_MODE_PWM},

	{ST_TXT_MANUAL_MODE_PWM,            DOWN,                                                           ST_TXT_SINUS},

	{ST_TXT_SINUS,                                                          DOWN,                                                           ST_TXT_MANUAL_MODE_PWM},

	{ST_TXT_SINUS,                                                          UP,                                                                    ST_TXT_MANUAL_MODE_PWM},

	{ST_TXT_MANUAL_MODE_PWM,            UP,                                                                    ST_TXT_SINUS},

	{ST_TXT_SINUS,                                                          LEFT,                                                 ST_TXT_SETUP},

	{ST_TXT_MANUAL_MODE_PWM,            LEFT,                                                 ST_TXT_SETUP},

	

	// MAIN MENU SINUS GENERATION (VERTICAL)

	{ST_TXT_SINUS,                                                          ENTER,                              ST_TXT_PARAMETERS_SINUS},

	{ST_TXT_PARAMETERS_SINUS,               DOWN,                                             ST_TXT_ON_OFF_SINUS},

	{ST_TXT_ON_OFF_SINUS,                         DOWN,                                             ST_TXT_PARAMETERS_SINUS},

	{ST_TXT_PARAMETERS_SINUS,               UP,                                                     ST_TXT_ON_OFF_SINUS},

	{ST_TXT_ON_OFF_SINUS,                         UP,                                                     ST_TXT_PARAMETERS_SINUS},

	{ST_TXT_PARAMETERS_SINUS,               LEFT,                                  ST_TXT_SETUP},

	{ST_TXT_ON_OFF_SINUS,                         LEFT,                                  ST_TXT_SETUP},

	

	// SETUP (VERTICAL)

	// CLOCK (VERTICAL)

	{ST_TXT_CLOCK,                                                         ENTER,                              ST_TXT_DATE},

	{ST_TXT_DATE,                                             DOWN,                                             ST_TXT_TIME},

	{ST_TXT_TIME,                                              DOWN,                                             ST_TXT_DATE},

	{ST_TXT_TIME,                                              UP,                                                     ST_TXT_DATE},

	{ST_TXT_DATE,                                             UP,                                                     ST_TXT_TIME},

	//chang�

	{ST_TXT_TIME,                                              LEFT,                                  ST_TXT_CLOCK},

	{ST_TXT_DATE,                                             LEFT,                                  ST_TXT_CLOCK},

	// FUNCTIONS

	{ST_TXT_DATE,                                             ENTER,                              ST_FCT_DATE},

	{ST_TXT_TIME,                                              ENTER,                              ST_FCT_TIME},

	

	// SENSOR (VERTICAL)

	{ST_TXT_SENSOR,                                                      ENTER,                              ST_TXT_ADC},

	{ST_TXT_ADC,                                               DOWN,                                             ST_TXT_CURRENT},

	{ST_TXT_CURRENT,                                     DOWN,                                             ST_TXT_THERMOCOUPLE},

	{ST_TXT_THERMOCOUPLE,                      DOWN,                                             ST_TXT_ADC},

	{ST_TXT_THERMOCOUPLE,                      UP,                                                     ST_TXT_CURRENT},

	{ST_TXT_CURRENT,                                     UP,                                                     ST_TXT_ADC},

	{ST_TXT_ADC,                                               UP,                                                     ST_TXT_THERMOCOUPLE},

	// FUNCTIONS

	{ST_TXT_ADC,                                               ENTER,                              ST_FCT_ADC},

	{ST_TXT_CURRENT,                                     ENTER,                              ST_FCT_CURRENT},

	{ST_TXT_THERMOCOUPLE,                      ENTER,                              ST_FCT_THERMOCOUPLE},

	

	// ACTUATOR (VERTICAL)

	{ST_TXT_ACTUATOR,                                  ENTER,                              ST_TXT_DAC},

	{ST_TXT_DAC,                                               DOWN,                                             ST_TXT_PWM_SETUP},

	{ST_TXT_PWM_SETUP,                               DOWN,                                             ST_TXT_RELAY},

	{ST_TXT_RELAY,                                                          DOWN,                                             ST_TXT_DAC},

	{ST_TXT_DAC,                                               UP,                                                     ST_TXT_RELAY},

	{ST_TXT_PWM_SETUP,                               UP,                                                     ST_TXT_DAC},

	{ST_TXT_RELAY,                                                          UP,                                                     ST_TXT_PWM_SETUP},

	//chang�

	{ST_TXT_DAC,                                               LEFT,                                  ST_TXT_ACTUATOR},

	{ST_TXT_PWM_SETUP,                               LEFT,                                  ST_TXT_ACTUATOR},

	{ST_TXT_RELAY,                                                          LEFT,                                  ST_TXT_ACTUATOR},

	// FUNCTIONS

	{ST_TXT_DAC,                                               ENTER,                              ST_FCT_DAC},

	{ST_TXT_RELAY,                                                          ENTER,                              ST_FCT_RELAY},

	

	{ST_TXT_PWM_SETUP,                               ENTER,                              ST_TXT_PWM_1},

	{ST_TXT_PWM_1,                                                        DOWN,                                             ST_TXT_PWM_1_2},

	{ST_TXT_PWM_1_2,                                     DOWN,                                             ST_TXT_PWM_1_2_3},

	{ST_TXT_PWM_1_2_3,                                DOWN,                                             ST_TXT_PERIODE},

	{ST_TXT_PERIODE,                                      DOWN,                                             ST_TXT_RESOLUTION},

	{ST_TXT_RESOLUTION,                                            DOWN,                                             ST_TXT_PWM_TRIANGLE},

	{ST_TXT_PWM_TRIANGLE,                       DOWN,                                             ST_TXT_PWM_1},

	{ST_TXT_PWM_TRIANGLE,                       UP,                                                     ST_TXT_RESOLUTION},

	{ST_TXT_RESOLUTION,                                            UP,                                                     ST_TXT_PERIODE},

	{ST_TXT_PERIODE,                                      UP,                                                     ST_TXT_PWM_1_2_3},

	{ST_TXT_PWM_1_2_3,                                UP,                                                     ST_TXT_PWM_1_2},

	{ST_TXT_PWM_1_2,                                     UP,                                                     ST_TXT_PWM_1},

	{ST_TXT_PWM_1,                                                        UP,                                                     ST_TXT_PWM_TRIANGLE},

	// FUNCTIONS

	{ST_TXT_PWM_1,                                                        ENTER,                              ST_FCT_PWM_1},

	{ST_TXT_PWM_1_2,                                     ENTER,                              ST_FCT_PWM_1_2},

	{ST_TXT_PWM_1_2_3,                                ENTER,                              ST_FCT_PWM_1_2_3},

	

	{ST_TXT_PWM_TRIANGLE,                       ENTER,                              ST_TXT_DENT_SCIE},

	{ST_TXT_DENT_SCIE,                                 DOWN,                                             ST_TXT_ISOCELE},

	{ST_TXT_ISOCELE,                                      DOWN,                                             ST_TXT_DENT_SCIE},

	{ST_TXT_ISOCELE,                                      UP,                                          ST_TXT_DENT_SCIE},

	{ST_TXT_DENT_SCIE,                                 UP,                                                     ST_TXT_ISOCELE},

	// FUNCTIONS

	{ST_TXT_PERIODE,                                      ENTER,                              ST_FCT_PERIODE},

	{ST_TXT_RESOLUTION,                                            ENTER,                              ST_FCT_RESOLUTION},

	{ST_TXT_DENT_SCIE,                                 ENTER,                              ST_FCT_DENT_SCIE},

	{ST_TXT_ISOCELE,                                      ENTER,                              ST_FCT_ISOCELE},

	

	// STORAGE (VERTICAL)

	{ST_TXT_STORAGE,                                     ENTER,                              ST_TXT_SRAM},

	{ST_TXT_SRAM,                                                           DOWN,                                             ST_TXT_EEPROM},

	{ST_TXT_EEPROM,                                                     DOWN,                                             ST_TXT_SRAM},

	{ST_TXT_EEPROM,                                                     UP,                                                     ST_TXT_SRAM},

	{ST_TXT_SRAM,                                                           UP,                                                     ST_TXT_EEPROM},

	// FUNCTIONS

	{ST_TXT_EEPROM,                                                     ENTER,                              ST_FCT_EEPROM},

	{ST_TXT_SRAM,                                                           ENTER,                              ST_FCT_SRAM},

	

	// MONITORING

	// FUNCTION

	{ST_TXT_MONITORING,                                            ENTER,                              ST_FCT_MONITORING},

	

	// CONTRAST

	// FUNCTION

	{ST_TXT_CONTRAST,                                  ENTER,                              ST_FCT_CONTRAST},

	

	// DEBUG

	// FUNCTION

	{ST_TXT_DEBUG,                                                        ENTER,                              ST_FCT_DEBUG},

	

	

	// DATALOGGER (VERTICAL)

	// PARAMETERS

	{ST_TXT_PARAMETERS_DL,                      ENTER,                              ST_TXT_SAMPLING_TIME},

	{ST_TXT_SAMPLING_TIME,                       DOWN,                                             ST_TXT_DURATION},

	{ST_TXT_DURATION,                                  DOWN,                                             ST_TXT_SAMPLING_TIME},

	{ST_TXT_SAMPLING_TIME,                       UP,                                                     ST_TXT_DURATION},

	{ST_TXT_DURATION,                                  UP,                                                     ST_TXT_SAMPLING_TIME},

	// FUNCTIONS

	{ST_TXT_SAMPLING_TIME,                       ENTER,                              ST_FCT_SAMPLING_TIME},

	{ST_TXT_DURATION,                                  ENTER,                              ST_FCT_DURATION},

	

	// ON/OFF

	// FUNCTIONS

	{ST_TXT_ON_OFF,                                                       ENTER,                              ST_FCT_ON_OFF},

	

	// DATA TRANSFER (USART)

	// FUNCTIONS

	{ST_TXT_DATA_TRANSFER,                      ENTER,                              ST_FCT_DATA_TRANSFER},

	

	

	// DIGITAL CONTROL (VERTICAL)

	// TYPE

	//PID

	{ST_TXT_PARAMETERS_CTRL,  ENTER,                              ST_TXT_PID},

	{ST_TXT_PID,                                                 DOWN,                                             ST_TXT_RECURSION},

	{ST_TXT_RECURSION,                               DOWN,                                             ST_TXT_SMITH},

	{ST_TXT_SMITH,                                                          DOWN,                                             ST_TXT_MODEL},

	{ST_TXT_MODEL,                                                        DOWN,                                             ST_TXT_RST},

	{ST_TXT_RST,                                                 DOWN,                                            ST_TXT_PID},

	{ST_TXT_PID,                                                 UP,                                                     ST_TXT_RST},

	{ST_TXT_RECURSION,                               UP,                                                     ST_TXT_PID},

	{ST_TXT_SMITH,                                                          UP,                                                     ST_TXT_RECURSION},

	{ST_TXT_MODEL,                                                        UP,                                                     ST_TXT_SMITH},

	{ST_TXT_RST,                                                 UP,                                                     ST_TXT_MODEL},

	

	{ST_TXT_PID,                                                 ENTER,                              ST_TXT_P},

	{ST_TXT_P,                                                                    DOWN,                                             ST_TXT_I},

	{ST_TXT_I,                                                                     DOWN,                                             ST_TXT_D},

	{ST_TXT_D,                                                                   DOWN,                                             ST_TXT_WINDUP},

	{ST_TXT_WINDUP,                                                      DOWN,                                             ST_TXT_P},

	{ST_TXT_P,                                                                    UP,                                                     ST_TXT_WINDUP},

	{ST_TXT_I,                                                                     UP,                                                     ST_TXT_P},

	{ST_TXT_D,                                                                   UP,                                                     ST_TXT_I},

	{ST_TXT_WINDUP,                                                      UP,                                                     ST_TXT_D},

	// FUNCTIONS

	{ST_TXT_P,                                                                    ENTER,                              ST_FCT_P},

	{ST_TXT_I,                                                                     ENTER,                              ST_FCT_I},

	{ST_TXT_D,                                                                   ENTER,                              ST_FCT_D},

	{ST_TXT_WINDUP,                                                      ENTER,                              ST_FCT_WINDUP},

	

	// RECURSION

	// FUNCTION

	{ST_TXT_RECURSION,                               ENTER,                              ST_FCT_RECURSION},

	

	// SMITH PREDICTOR

	// FUNCTION

	{ST_TXT_SMITH,                                                          ENTER,                              ST_FCT_SMITH},

	

	// INTERNAL MODEL

	{ST_TXT_MODEL,                                                        ENTER,                              ST_TXT_MODEL_RECURSION},

	{ST_TXT_MODEL_RECURSION,               DOWN,                                             ST_TXT_MODEL_SETTING},

	{ST_TXT_MODEL_SETTING,                      DOWN,                                             ST_TXT_MODEL_RECURSION},

	{ST_TXT_MODEL_RECURSION,               UP,                                                     ST_TXT_MODEL_SETTING},

	{ST_TXT_MODEL_SETTING,                      UP,                                                     ST_TXT_MODEL_RECURSION},

	// FUNCTIONS

	{ST_TXT_MODEL_RECURSION,               ENTER,                              ST_FCT_MODEL},

	{ST_TXT_MODEL_SETTING,                      ENTER,                              ST_FCT_MODEL_SETTING},

	

	// RST

	// FUNCTION

	{ST_TXT_RST,                                                 ENTER,                              ST_FCT_RST},

	

	// SAMPLING TIME

	// FUNCTION

	{ST_TXT_SAMPLING_CTRL,                      ENTER,                              ST_FCT_SAMPLING_CTRL},

	

	// MANUAL MODE

	// FUNCTION

	{ST_TXT_MANUAL_MODE,                        ENTER,                              ST_FCT_MANUAL_MODE},

	

	// AUTOMATIC MODE

	// FUNCTION

	{ST_TXT_AUTO_MODE,                                             ENTER,                              ST_FCT_AUTO_MODE},

	

	

	// PWM GENERATION

	// FUNCTIONS

	{ST_TXT_MANUAL_MODE_PWM,            ENTER,                              ST_FCT_MANUAL_MODE_PWM},

	{ST_TXT_MODE_SINUS,                                            ENTER,                              ST_FCT_MODE_SINUS},

	

	// SINUS GENERATION

	{ST_TXT_PARAMETERS_SINUS,               ENTER,                              ST_TXT_SINUS_AMPLITUDE},

	{ST_TXT_SINUS_AMPLITUDE,   DOWN,                                             ST_TXT_SINUS_PERIODE},

	{ST_TXT_SINUS_PERIODE,                       DOWN,                                             ST_TXT_SINUS_AMPLITUDE},

	{ST_TXT_SINUS_AMPLITUDE,   UP,                                                     ST_TXT_SINUS_PERIODE},

	{ST_TXT_SINUS_PERIODE,                       UP,                                                     ST_TXT_SINUS_AMPLITUDE},

	// FUNCTIONS

	{ST_TXT_SINUS_AMPLITUDE,   ENTER,                              ST_FCT_SINUS_AMPLITUDE},

	{ST_TXT_SINUS_PERIODE,                       ENTER,                              ST_FCT_SINUS_PERIODE},

	{ST_TXT_ON_OFF_SINUS,                         ENTER,                              ST_FCT_ON_OFF_SINUS},

	

	// MAIN

	

	{0,                         0,          0},

};





const MENU_STATE Menu_State[] PROGMEM = {

	//  STATE                               STATE TEXT                  STATE_FUNC

	

	

	{ST_TXT_WELCOME,                                                                Txt_WELCOME,                                                           NULL},

	{ST_TXT_SETUP,                                                                                        Txt_SETUP,                                                                    NULL},

	{ST_TXT_DATALOGGER,                                                                         Txt_DATALOGGER,                                                           NULL},

	{ST_TXT_CONTROL,                                                                 Txt_CONTROL,                                              NULL},

	{ST_TXT_PWM,                                                                           Txt_PWM,                                                                      NULL},

	{ST_TXT_SINUS,                                                                                       Txt_SINUS,                                                                   NULL},

	

	{ST_TXT_CLOCK,                                                                                      Txt_CLOCK,                                                                  NULL},

	{ST_TXT_SENSOR,                                                                                   Txt_SENSOR,                                                               NULL},

	{ST_TXT_ACTUATOR,                                                                Txt_ACTUATOR,                                                          NULL},

	{ST_TXT_STORAGE,                                                                  Txt_STORAGE,                                               NULL},

	{ST_TXT_MONITORING,                                                                         Txt_MONITORING,                                                           NULL},

	{ST_TXT_CONTRAST,                                                                Txt_CONTRAST,                                                          NULL},

	{ST_TXT_DEBUG,                                                                                      Txt_DEBUG,                                                                  NULL},

	

	{ST_TXT_PARAMETERS_DL,                                                   Txt_PARAMETERS,                                                     NULL},

	{ST_TXT_ON_OFF,                                                                                    Txt_ON_OFF,                                                                NULL},

	{ST_TXT_DATA_TRANSFER,                                                    Txt_TRANSFER,                                                           NULL},

	

	{ST_TXT_PARAMETERS_CTRL,                                Txt_PARAMETERS,                                                      NULL},

	{ST_TXT_SAMPLING_CTRL,                                                    Txt_SAMPLING,                                                           NULL},

	{ST_TXT_MANUAL_MODE,                                                     Txt_MANUAL,                                                              NULL},

	{ST_TXT_AUTO_MODE,                                                                           Txt_AUTO,                                                                     NULL},

	

	{ST_TXT_MANUAL_MODE_PWM,                                         Txt_MANUAL,                                                              NULL},

	{ST_TXT_MODE_SINUS,                                                                         Txt_SINUS_MODE,                                                           NULL},

	

	{ST_TXT_PARAMETERS_SINUS,                                            Txt_PARAMETERS,                                                     NULL},

	{ST_TXT_ON_OFF_SINUS,                                                      Txt_ON_OFF,                                                                NULL},

	

	{ST_TXT_DATE,                                                                           Txt_Date,                                                                       NULL},

	{ST_TXT_TIME,                                                                            Txt_TIME,                                                                      NULL},

	

	{ST_TXT_ADC,                                                                            Txt_ADC,                                                                       NULL},

	{ST_TXT_CURRENT,                                                                  Txt_CURRENT,                                               NULL},

	{ST_TXT_THERMOCOUPLE,                                                   Txt_THERMOCOUPLE,                                NULL},

	

	{ST_TXT_DAC,                                                                             Txt_DAC,                                                                       NULL},

	{ST_TXT_PWM_SETUP,                                                             Txt_PWM_SETUP,                                                       NULL},

	{ST_TXT_RELAY,                                                                                        Txt_RELAY,                                                                    NULL},

	

	{ST_TXT_PWM_1,                                                                                     Txt_PWM_1,                                                                 NULL},

	{ST_TXT_PWM_1_2,                                                                  Txt_PWM_2,                                                                 NULL},

	{ST_TXT_PWM_1_2_3,                                                              Txt_PWM_3,                                                                 NULL},

	{ST_TXT_PERIODE,                                                                   Txt_PERIODE,                                                NULL},

	{ST_TXT_RESOLUTION,                                                                          Txt_RESOLUTION,                                                           NULL},

	{ST_TXT_PWM_TRIANGLE,                                                     Txt_TRIANGLE,                                             NULL},

	

	{ST_TXT_DENT_SCIE,                                                               Txt_DENT_SCIE,                                                         NULL},

	{ST_TXT_ISOCELE,                                                                    Txt_ISOCELE,                                                NULL},

	

	{ST_TXT_SRAM,                                                                                        Txt_SRAM,                                                                    NULL},

	{ST_TXT_EEPROM,                                                                                   Txt_EEPROM,                                                               NULL},

	

	{ST_TXT_SAMPLING_TIME,                                                    Txt_SAMPLING,                                                           NULL},

	{ST_TXT_DURATION,                                                                Txt_DURATION,                                                          NULL},

	

	{ST_TXT_PID,                                                                              Txt_PID,                                                                         NULL},

	{ST_TXT_RECURSION,                                                             Txt_RECURSION,                                                       NULL},

	{ST_TXT_SMITH,                                                                                       Txt_SMITH,                                                                   NULL},

	{ST_TXT_MODEL,                                                                                     Txt_MODEL,                                                                 NULL},

	{ST_TXT_RST,                                                                              Txt_RST,                                                                         NULL},

	

	{ST_TXT_P,                                                                                                  Txt_P,                                                                              NULL},

	{ST_TXT_I,                                                                                                   Txt_I,                                                                               NULL},

	{ST_TXT_D,                                                                                                 Txt_D,                                                                             NULL},

	{ST_TXT_WINDUP,                                                                                   Txt_WINDUP,                                                               NULL},

	

	{ST_TXT_MODEL_RECURSION,                                            Txt_MODEL_RECURSION,                              NULL},

	{ST_TXT_MODEL_SETTING,                                                    Txt_MODEL_SETTING,                                NULL},

	

	{ST_TXT_SINUS_AMPLITUDE,                                 Txt_AMPLITUDE,                                                         NULL},

	{ST_TXT_SINUS_PERIODE,                                                     Txt_PERIODE,                                                NULL},

	

	{ST_FCT_DATE,                                                                                         NULL,                                                                             Date},

	{ST_FCT_TIME,                                                                           NULL,                                                                            Time},

	{ST_FCT_ADC,                                                                            NULL,                                                                            Adc},

	{ST_FCT_CURRENT,                                                                  NULL,                                                                            Current},

	{ST_FCT_THERMOCOUPLE,                                                   NULL,                                                                            Thermocouple},

	{ST_FCT_DAC,                                                                            NULL,                                                                            Dac},

	{ST_FCT_RELAY,                                                                                       NULL,                                                                             Relay},

	{ST_FCT_PWM_1,                                                                                     NULL,                                                                             Pwm_1},

	{ST_FCT_PWM_1_2,                                                                  NULL,                                                                            Pwm_1_2},

	{ST_FCT_PWM_1_2_3,                                                             NULL,                                                                            Pwm_1_2_3},

	{ST_FCT_PERIODE,                                                                   NULL,                                                                            Periode},

	{ST_FCT_RESOLUTION,                                                                         NULL,                                                                             Resolution},

	{ST_FCT_DENT_SCIE,                                                              NULL,                                                                            Dent_Scie},

	{ST_FCT_ISOCELE,                                                                   NULL,                                                                            Isocele},

	{ST_FCT_EEPROM,                                                                                  NULL,                                                                             Eeprom},

	{ST_FCT_SRAM,                                                                                        NULL,                                                                             Sram},

	{ST_FCT_MONITORING,                                                                         NULL,                                                                             Monitoring},

	{ST_FCT_CONTRAST,                                                               NULL,                                                                            Contrast},

	{ST_FCT_DEBUG,                                                                                     NULL,                                                                             Debug},

	{ST_FCT_SAMPLING_TIME,                                                    NULL,                                                                            Sampling_Time},

	{ST_FCT_DURATION,                                                               NULL,                                                                            Duration},

	{ST_FCT_ON_OFF,                                                                                    NULL,                                                                             Datalogger_On_Off},

	{ST_FCT_DATA_TRANSFER,                                                   NULL,                                                                            Transfer_Usart},

	{ST_FCT_SAMPLING_CTRL,                                                   NULL,                                                                            Sampling_CTRL},

	{ST_FCT_MANUAL_MODE,                                                     NULL,                                                                            Manual_Mode},

	{ST_FCT_AUTO_MODE,                                                                          NULL,                                                                             Auto_Mode},

	{ST_FCT_P,                                                                                                 NULL,                                                                             Pid_P},

	{ST_FCT_I,                                                                                                  NULL,                                                                             Pid_I},

	{ST_FCT_D,                                                                                                NULL,                                                                             Pid_D},

	{ST_FCT_WINDUP,                                                                                   NULL,                                                                             Pid_WindUp},

	{ST_FCT_RECURSION,                                                            NULL,                                                                            Recursion},

	{ST_FCT_SMITH,                                                                                       NULL,                                                                             Smith},

	{ST_FCT_MODEL,                                                                                     NULL,                                                                             Internal_Model},

	{ST_FCT_MODEL_SETTING,                                                   NULL,                                                                            Internal_Model_Setting},

	{ST_FCT_RST,                                                                              NULL,                                                                            Rst},

	{ST_FCT_MANUAL_MODE_PWM,                                         NULL,                                                                            Pwm_Manual},

	{ST_FCT_MODE_SINUS,                                                                         NULL,                                                                             Pwm_Sinus},

	{ST_FCT_SINUS_AMPLITUDE,                                NULL,                                                                             Sinus_Amplitude},

	{ST_FCT_SINUS_PERIODE,                                                    NULL,                                                                            Sinus_Periode},

	{ST_FCT_ON_OFF_SINUS,                                                      NULL,                                                                            Sinus_On_Off},

	

	

	

	

	

	{0,                                 NULL,                       NULL},

	

};

#endif