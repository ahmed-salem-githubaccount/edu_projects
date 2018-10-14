/**
  ******************************************************************************
  * @file     fsm_search.c
  * @author   Ahmed
  * @version
  * @date     Aug 24, 2018
  * @brief    
  ******************************************************************************
  */

/* Includes ("...") ----------------------------------------------------------*/
#include "utils.h"
#include "console.h"
#include "fsm.h"
#include "fsm_add_event.h"
#include "fsm_list_events.h"
#include "fsm_next_event.h"
#include "fsm_reminder.h"
/* Includes (<...>) ----------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void eventReminderHandler(ScreenStatePtr_t state, AgendaDB_t *  pAgendaDB);
static void stateExit( UNUSED_PARAM ScreenStatePtr_t state, AgendaDB_t *  pAgendaDB );
static void agendaExitHandler(ScreenStatePtr_t state, AgendaDB_t *  pAgendaDB);
static void addEventHandler(ScreenStatePtr_t state, AgendaDB_t *  pAgendaDB);
static void searchHandler(ScreenStatePtr_t state, AgendaDB_t *  pAgendaDB);
static void screenReloadHandler(ScreenStatePtr_t state, AgendaDB_t *  pAgendaDB);
static void listEventsHandler(ScreenStatePtr_t state, AgendaDB_t *  pAgendaDB);
static void userInputHandler(ScreenStatePtr_t state, AgendaDB_t *  pAgendaDB);
/* Global constants definitions ----------------------------------------------*/
/* Global variable definitions -----------------------------------------------*/
/* Module functions implementation -------------------------------------------*/


void fsm_TransitionToSearchState(ScreenStatePtr_t state, void * pData)
{
    AgendaDB_t * pAgendaDB  = (AgendaDB_t *)pData;
    /* Save last state */
    fsm_SaveLastState();

    /* Cleanup */
    state->exitState(state, pAgendaDB);
    fsm_RestoreDefault(state);

    /* init the state */
    state->id = STATE_ID_SEARCH;
    state->exitState = stateExit;
    state->stateTransition = fsm_TransitionToSearchState;
    state->eventHandler[EVT_REMINDER] = eventReminderHandler;
    state->eventHandler[EVT_BACK_OR_EXIT_AGENDA] = agendaExitHandler;
    state->eventHandler[EVT_ADD_EVENT] = addEventHandler;
    state->eventHandler[EVT_SEARCH] = searchHandler;
    state->eventHandler[EVT_CONSOLE_SCREEN_RELOAD] = screenReloadHandler;
    state->eventHandler[EVT_LIST_EVENTS] = listEventsHandler;
    state->eventHandler[EVT_USER_INPUT_RECEIVED] = userInputHandler;

    Console_SearchScreen(pAgendaDB);
}

static void stateExit( UNUSED_PARAM ScreenStatePtr_t state, AgendaDB_t *  pAgendaDB )
{
    consoleSearchState = CONSOLE_SEARCH_EVT_START;
    system("clear");
}

static void eventReminderHandler(ScreenStatePtr_t state, AgendaDB_t *  pAgendaDB)
{
    fsm_TransitionToReminderState(state, pAgendaDB);
}

static void agendaExitHandler(ScreenStatePtr_t state, AgendaDB_t *  pAgendaDB)
{
    consoleSearchState = CONSOLE_SEARCH_EVT_START;
    /* Back to last state */
    fsm_RestoreLastState(pAgendaDB);
}

static void addEventHandler(ScreenStatePtr_t state, AgendaDB_t *  pAgendaDB)
{
    fsm_TransitionToAddEventState(state, pAgendaDB);
}

static void searchHandler(ScreenStatePtr_t state, AgendaDB_t *  pAgendaDB)
{
    fsm_TransitionToSearchState(state, pAgendaDB);
}

static void screenReloadHandler(ScreenStatePtr_t state, AgendaDB_t *  pAgendaDB)
{
    consoleSearchState = CONSOLE_SEARCH_EVT_START;
    Console_SearchScreen(pAgendaDB);
}

static void listEventsHandler(ScreenStatePtr_t state, AgendaDB_t *  pAgendaDB)
{
    fsm_TransitionToListEventsState( state, pAgendaDB);
}

static void userInputHandler(ScreenStatePtr_t state, AgendaDB_t *  pAgendaDB)
{
    Console_SearchScreen(pAgendaDB);
}
/* End of file ---------------------------------------------------------------*/
/* fsm_search.c */
