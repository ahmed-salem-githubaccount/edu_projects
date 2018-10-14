/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SRC_AGENDA_CORE_SCREEN_FSM_FSM_REMINDER_H__
#define __SRC_AGENDA_CORE_SCREEN_FSM_FSM_REMINDER_H__
/**
  ******************************************************************************
  * @file      reminder_screen.h
  * @author    Ahmed
  * @version   
  * @date      Aug 26, 2018
  * @brief     
  ******************************************************************************
  */

/* Includes ("...") ----------------------------------------------------------*/
#include "fsm.h"
/* Includes (<...>) ----------------------------------------------------------*/
/* Global defines-------------------------------------------------------------*/
/* Global macros -------------------------------------------------------------*/
/* Global constants declarations ---------------------------------------------*/
/* Global typedef ------------------------------------------------------------*/
/* Global variable declarations ----------------------------------------------*/
/* Global function prototypes ------------------------------------------------*/
/**
 * @brief  Transition function to Reminder event state
 * @param  state pointer to current state
 * @param  pData passed data to the state
 * @return void
 */
void fsm_TransitionToReminderState(ScreenStatePtr_t state, void * pData);
/* End of file ---------------------------------------------------------------*/
#endif /* __SRC_AGENDA_CORE_SCREEN_FSM_FSM_REMINDER_H__ */
