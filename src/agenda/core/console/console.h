/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SRC_AGENDA_CORE_CONSOLE_CONSOLE_H__
#define __SRC_AGENDA_CORE_CONSOLE_CONSOLE_H__
/**
  ******************************************************************************
  * @file      console.h
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
typedef char(* pLastMenu)(void);

typedef enum
{
    CONSOLE_ADD_EVT_START,
    CONSOLE_ADD_EVT_TIME_EXPECTED,
    CONSOLE_ADD_EVT_EVT_EXPECTED,
    CONSOLE_ADD_EVT_STATE_EXPECTED,
    CONSOLE_ADD_EVT_MAX
}ConsoleAddEventState_t;

typedef enum
{
    CONSOLE_SEARCH_EVT_START,
    CONSOLE_SEARCH_EVT_TIME_EXPECTED,
    CONSOLE_SEARCH_EVT_STATE_EXPECTED,
    CONSOLE_SEARCH_EVT_MAX
}ConsoleSearchState_t;
/* Global variable declarations ----------------------------------------------*/
extern uint8_t nextEventString[200];
extern uint8_t EventListString[3000];
extern ConsoleAddEventState_t consoleAddEventState;
extern ConsoleSearchState_t consoleSearchState;

/* Global function prototypes ------------------------------------------------*/
/**
 * @brief  console Init screen
 * @param  pUserInfo pointer to user info
 * @return void
 */
void Console_InitScreen( AgednaUserInfo_t * pUserInfo );
/**
 * @brief  Shows the next event console
 * @param  pAgendaDB pointer to the agenda DB
 * @return void
 */
void Console_NextEventScreen(AgendaDB_t* pAgendaDB);
/**
 * @brief  Shows list of current day events
 * @param  pAgendaDB pointer to the agenda DB
 * @return void
 */
void Console_ListTodayEventsScreen(AgendaDB_t* pAgendaDB);
/**
 * @brief  Shows the add event console
 * @param  pAgendaDB pointer to the agenda DB
 * @return void
 */
void Console_AddEventScreen(AgendaDB_t* pAgendaDB);
/**
 * @brief  Shows the search for event console
 * @param  pAgendaDB pointer to the agenda DB
 * @return void
 */
void Console_SearchScreen(AgendaDB_t* pAgendaDB);
/**
 * @brief  Shows the exit app console
 * @param  void
 * @return void
 */
void Console_ExitScreen(void);
/**
 * @brief  Shows the event reminder console
 * @param  pAgendaDB pointer to the agenda DB
 * @return void
 */
void Console_ReminderScreen( AgendaDB_t * pAgendaDB);
/**
 * @brief  clears the stdin buffer
 * @param  void
 * @return void
 */
void flushstdin(void);
#if AGENDA_DEMO_MODE
void Console_DemoStartScreen(AgendaDB_t *  pAgendaDB);
void Console_DemoEndedScreen(AgendaDB_t* pAgendaDB);
#endif


/* End of file ---------------------------------------------------------------*/
#endif /* __SRC_AGENDA_CORE_CONSOLE_CONSOLE_H__ */
