/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SRC_AGENDA_CORE_FSM_FSM_ADD_EVENT_H__
#define __SRC_AGENDA_CORE_FSM_FSM_ADD_EVENT_H__
/**
  ******************************************************************************
  * @file      add_event_menu.h
  * @author    Ahmed
  * @version   
  * @date      Aug 24, 2018
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
 * @brief  Transition function to add event state
 * @param  state pointer to current state
 * @param  pData passed data to the state
 * @return void
 */
void fsm_TransitionToAddEventState(ScreenStatePtr_t state, void* pData);
/* End of file ---------------------------------------------------------------*/
#endif /* __SRC_AGENDA_CORE_FSM_FSM_ADD_EVENT_H__ */
