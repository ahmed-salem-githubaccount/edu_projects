/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SRC_UTILS_UTILS_H__
#define __SRC_UTILS_UTILS_H__
/**
  ******************************************************************************
  * @file      utils.h
  * @author    Ahmed
  * @version   
  * @date      Aug 22, 2018
  * @brief     
  ******************************************************************************
  */

/* Includes ("...") ----------------------------------------------------------*/
#include "agenda_config.h"
/* Includes (<...>) ----------------------------------------------------------*/
#include <stdbool.h>
#include <time.h>
/* Global defines-------------------------------------------------------------*/
#define GCC_PACKED_STRUCT __attribute__((packed))
// this way we tell the compiler( GCC ) what we want to ignore unused parameter warning
#define UNUSED_PARAM                        __attribute__( ( unused ) )

/* Global macros -------------------------------------------------------------*/
/* Global constants declarations ---------------------------------------------*/
/* Global typedef ------------------------------------------------------------*/
/* Global variable declarations ----------------------------------------------*/
#if AGENDA_DEMO_MODE
int currentTimeInSec;
#endif
/* Global function prototypes ------------------------------------------------*/
/**
 * @brief  returns current system time
 * @param  OUT timeinfo buffer
 * @return void
 */
void getCurrentTime(struct tm ** timeinfo);
/**
 * @brief  returns current system time in seconds
 * @param  OUT pcurrentTimeInSec buffer
 * @return true if OK, false if not OK
 */
bool getCurrentTimeinSec(int * pcurrentTimeInSec);
/**
 * @brief  splits a string using the passed delimiter
 * @param  a_str string to be splitted
 * @return pointer to the resulted strings
 */
char** str_split(char* a_str, const char a_delim);
/**
 * @brief  check the time format and modify it if necessary
 * @param  time pointer to the time string
 * @return fasle if error happened ot true in case of success
 */
bool checkAndFormatTime(char * time);
/**
 * @brief  returns current date in the format that the app expects
 * @param  OUT pDate pointer to the date string
 * @return void
 */
void getCurrentDateFormatted(char * pDate);
/**
 * @brief  returns current time in the format that the app expects
 * @param  OUT pTime pointer to the time string
 * @return void
 */
void getCurrentTimeFormatted(char * pTime);
/**
 * @brief  removed trailing new line from a string
 * @param  IN/OUT buffer pointer to the string
 * @return void
 */
void removeTrailingNewline( char * buffer);
/**
 * @brief  convert all upper case chars in a string to lower case ones
 * @param  IN/OUT pStr pointer to the string
 * @return void
 */
void stringToLower(char * pStr);
#if AGENDA_DEMO_MODE
int ConvertFormattedTimetoSec(char * ptime);
#endif
/* End of file ---------------------------------------------------------------*/
#endif /* __SRC_UTILS_UTILS_H__ */
