/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SRC_MAIN_H__
#define __SRC_MAIN_H__
/**
  ******************************************************************************
  * @file      main.h
  * @author    embedded
  * @version   
  * @date      Aug 24, 2018
  * @brief     
  ******************************************************************************
  */

/* Includes ("...") ----------------------------------------------------------*/
/* Includes (<...>) ----------------------------------------------------------*/
#include <stdbool.h>
/* Global defines-------------------------------------------------------------*/
/* Global macros -------------------------------------------------------------*/
/* Global constants declarations ---------------------------------------------*/
/* Global typedef ------------------------------------------------------------*/
typedef struct
{
    union
    {
        char singleChar;
        char str[64];
    }u;
    int size;
}UserInput_t;

/* Global variable declarations ----------------------------------------------*/
#if AGENDA_DEMO_MODE
extern bool agendaDemoStarted;
#endif
/* Global function prototypes ------------------------------------------------*/
/**
 * @brief  Do necessary cleanup and exit the program
 * @param  void
 * @return void
 */
void appExit(void);
/**
 * @brief  Returns Console user input string
 * @param  void
 * @return char * user input string
 */
char * appGetUserInputString(void);
/* End of file ---------------------------------------------------------------*/
#endif /* __SRC_MAIN_H__ */
