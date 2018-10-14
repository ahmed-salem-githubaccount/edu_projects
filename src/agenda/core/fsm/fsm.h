/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SRC_AGENDA_CORE_SCREEN_FSM_FSM_H__
#define __SRC_AGENDA_CORE_SCREEN_FSM_FSM_H__
/**
  ******************************************************************************
  * @file      screen_fsm.h
  * @author    Ahmed
  * @version   
  * @date      Aug 24, 2018
  * @brief     
  ******************************************************************************
  */

/* Includes ("...") ----------------------------------------------------------*/
#include "agenda_config.h"
#include "agenda_interface.h"
/* Includes (<...>) ----------------------------------------------------------*/
/* Global defines-------------------------------------------------------------*/
/* Global macros -------------------------------------------------------------*/
/* Global constants declarations ---------------------------------------------*/
/* Global typedef ------------------------------------------------------------*/
typedef enum _EVT_t
{
    EVT_NONE,
    EVT_ADD_EVENT,
    EVT_LIST_EVENTS,
    EVT_SEARCH,
    EVT_BACK_OR_EXIT_AGENDA,
    EVT_CONSOLE_SCREEN_RELOAD,
    EVT_REMINDER,
    EVT_DISMISS_REMINDER,
    EVT_USER_INPUT_RECEIVED,
#if AGENDA_DEMO_MODE
    EVT_START_DEMO,
    EVT_END_DEMO,
#endif
    EVT_CNT
} EVT_t;

typedef enum _StateId
{
    STATE_ID_DEFAULT,
    STATE_ID_ADD_EVENT,
    STATE_ID_LIST_EVENTS,
    STATE_ID_SEARCH,
    STATE_ID_REMINDER,
    STATE_ID_COUNT
} StateId_t;

typedef struct _ScreenState* ScreenStatePtr_t;
typedef void (*HandleEventCallback_t)( ScreenStatePtr_t, void* pData );
typedef void (*ExitStateCallback_t)( ScreenStatePtr_t, void* pData );
typedef void (*StateTransitionCallback_t)( ScreenStatePtr_t, void* pData );

typedef struct _ScreenState
{
    HandleEventCallback_t       eventHandler[EVT_CNT];
    ExitStateCallback_t         exitState;
    StateTransitionCallback_t   stateTransition;
    StateId_t                   id;
} ScreenState_t;

/* Global variable declarations ----------------------------------------------*/
/* Global function prototypes ------------------------------------------------*/
/**
 * @brief  Restore default values for FSM buffers
 * @param  state  fsm current state pointer
 * @return void
 */
void fsm_RestoreDefault( ScreenStatePtr_t state );
/**
 * @brief  Init function for fsm. This function will make the transition to the next event state
 * @param  pDB  Pointer to Agenda table in DB
 * @return void
 */
void fsm_Init(AgendaDB_t * pDB);
/**
 * @brief Returns current FSM state ID
 * @param  void
 * @return StateId_t FSM state ID
 */
StateId_t fsm_getCurrentStateID(void);
/**
 * @brief screen fsm events handling. events will be routed to the correct handler
 * @param event  the raised event
 * @param pData  Passed argument
 * @return void
 */
void fsm_HandleEvent( EVT_t event, void * pData  );
/**
 * @brief Rstore FSM last state
 * @param pData  Passed argument
 * @return void
 */
void fsm_RestoreLastState(void * pData);
/**
 * @brief Save last state so that it can be restored later
 * @param  void
 * @return void
 */
void fsm_SaveLastState(void);
/* End of file ---------------------------------------------------------------*/
#endif /* __SRC_AGENDA_CORE_SCREEN_FSM_FSM_H__ */
