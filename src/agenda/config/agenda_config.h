/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SRC_AGENDA_CONFIG_AGENDA_CONFIG_H__
#define __SRC_AGENDA_CONFIG_AGENDA_CONFIG_H__
/**
  ******************************************************************************
  * @file      agenda_config.h
  * @author    Ahmed
  * @version   
  * @date      Aug 24, 2018
  * @brief     
  ******************************************************************************
  */

/* Includes ("...") ----------------------------------------------------------*/
/* Includes (<...>) ----------------------------------------------------------*/
/* Global defines-------------------------------------------------------------*/
/* User Inputs to be used by Agenda Core MUST be lower case*/
#define CONSOLE_USER_INPUT_ADD_EVENT            'a'
#define CONSOLE_USER_INPUT_LIST_EVENTS          'l'
#define CONSOLE_USER_INPUT_SEARCH               's'
#define CONSOLE_USER_INPUT_DISMISS_REMINDER     'd'
#define CONSOLE_USER_INPUT_SNOOZE_REMINDER      'z'
/*Reminder periods for events in sec */
#define AGENDA_REMINDER_1_IN_SEC            (10 * 60)   // 10minutes before event due time
#define AGENDA_REMINDER_3_IN_SEC            (0 * 60)    // event due time
/* Enables/Disables Agenda Demo */
#define AGENDA_DEMO_MODE                    0   // This flag if true, then the agenda will not consider the current date and only the current time

#if (AGENDA_DEMO_MODE == 1)
#define AGENDA_TIME_FACTOR                  (200)   // make the day shorter for Demo purpose. 200 and 100 are the optimal value.. any other highr values are not tested and not guaranteed
#define CONSOLE_USER_INPUT_START_DEMO       ('o')
#else
#define AGENDA_TIME_FACTOR                  (1)
#endif

/* Global macros -------------------------------------------------------------*/
/* Global constants declarations ---------------------------------------------*/
/* Global typedef ------------------------------------------------------------*/
/* Global variable declarations ----------------------------------------------*/
/* Global function prototypes ------------------------------------------------*/
/* End of file ---------------------------------------------------------------*/
#endif /* __SRC_AGENDA_CONFIG_AGENDA_CONFIG_H__ */
