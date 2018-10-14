/**
  ******************************************************************************
  * @file     fsm_add_event.c
  * @author   Ahmed
  * @version
  * @date     Aug 24, 2018
  * @brief    
  ******************************************************************************
  */

/* Includes ("...") ----------------------------------------------------------*/
#include "console.h"
#include "fsm.h"
#include "fsm_next_event.h"
#include "fsm_reminder.h"
#include "utils.h"
/* Includes (<...>) ----------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void agendaExitHandler(ScreenStatePtr_t state, AgendaDB_t *  pAgendaDB);
static void screenReloadHandler(ScreenStatePtr_t state, AgendaDB_t *  pAgendaDB);
static void eventReminderHandler(ScreenStatePtr_t state, AgendaDB_t *  pAgendaDB);
static void addEventHandler(ScreenStatePtr_t state, AgendaDB_t *  pAgendaDB);
static void userInputHandler(ScreenStatePtr_t state, AgendaDB_t *  pAgendaDB);
static void stateExit( UNUSED_PARAM ScreenStatePtr_t state, AgendaDB_t *  pAgendaDB );
/* Global constants definitions ----------------------------------------------*/
/* Global variable definitions -----------------------------------------------*/
/* Module functions implementation -------------------------------------------*/

void fsm_TransitionToAddEventState(ScreenStatePtr_t state, void * pData)
{

    AgendaDB_t *  pAgendaDB = (AgendaDB_t*)pData;
    /* Save last state */
    fsm_SaveLastState();

    /* Cleanup */
    state->exitState(state, pAgendaDB);
    fsm_RestoreDefault(state);

    /* init the state */
    state->id = STATE_ID_ADD_EVENT;
    state->exitState = stateExit;
    state->stateTransition = fsm_TransitionToAddEventState;
    state->eventHandler[EVT_BACK_OR_EXIT_AGENDA] = agendaExitHandler;
    state->eventHandler[EVT_CONSOLE_SCREEN_RELOAD] = screenReloadHandler;
    state->eventHandler[EVT_REMINDER] = eventReminderHandler;
    state->eventHandler[EVT_ADD_EVENT] = addEventHandler;
    state->eventHandler[EVT_USER_INPUT_RECEIVED] = userInputHandler;

    /* Console screen */
    Console_AddEventScreen(pAgendaDB);
}

static void addEventHandler(ScreenStatePtr_t state, AgendaDB_t *  pAgendaDB)
{
    /* Console screen */
    Console_AddEventScreen(pAgendaDB);
}

static void agendaExitHandler(ScreenStatePtr_t state, AgendaDB_t *  pAgendaDB)
{
    consoleAddEventState = CONSOLE_ADD_EVT_START;
    /* Back to last state */
    fsm_RestoreLastState(pAgendaDB);
}


static void screenReloadHandler(ScreenStatePtr_t state, AgendaDB_t *  pAgendaDB)
{
    consoleAddEventState = CONSOLE_ADD_EVT_START;
    /* Console screen */
    Console_AddEventScreen(pAgendaDB);
}

static void eventReminderHandler(ScreenStatePtr_t state, AgendaDB_t *  pAgendaDB)
{
    // Make the transition to reminder state
    fsm_TransitionToReminderState(state, pAgendaDB);
}

static void userInputHandler(ScreenStatePtr_t state, AgendaDB_t *  pAgendaDB)
{
    /* Console screen */
    Console_AddEventScreen(pAgendaDB);
}


static void stateExit( UNUSED_PARAM ScreenStatePtr_t state, AgendaDB_t *  pAgendaDB)
{
    consoleAddEventState = CONSOLE_ADD_EVT_START;
    system("clear");
}
/* End of file ---------------------------------------------------------------*/
/* fsm_add_event.c */
