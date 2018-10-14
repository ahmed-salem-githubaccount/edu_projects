/**
  ******************************************************************************
  * @file     fsm_list_events.c
  * @author   Ahmed
  * @version
  * @date     Aug 24, 2018
  * @brief    
  ******************************************************************************
  */

/* Includes ("...") ----------------------------------------------------------*/
#include "console.h"
#include "fsm.h"
#include "fsm_add_event.h"
#include "fsm_list_events.h"
#include "fsm_next_event.h"
#include "fsm_reminder.h"
#include "fsm_search.h"
#include "utils.h"
#include "db.h"
/* Includes (<...>) ----------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void stateExit( UNUSED_PARAM ScreenStatePtr_t state, AgendaDB_t *  pAgendaDB );
static void addEventHandler(ScreenStatePtr_t state, AgendaDB_t *  pAgendaDB);
static void searchHandler(ScreenStatePtr_t state, AgendaDB_t *  pAgendaDB);
static void screenReloadHandler(ScreenStatePtr_t state, AgendaDB_t *  pAgendaDB);
static void eventReminderHandler(ScreenStatePtr_t state, AgendaDB_t *  pAgendaDB);
static void agendaExitHandler(ScreenStatePtr_t state, AgendaDB_t *  pAgendaDB);
static void userInputHandler(ScreenStatePtr_t state, AgendaDB_t *  pAgendaDB);
/* Global constants definitions ----------------------------------------------*/
/* Global variable definitions -----------------------------------------------*/
/* Module functions implementation -------------------------------------------*/
void fsm_TransitionToListEventsState(ScreenStatePtr_t state, void * pData)
{
    AgendaDB_t *  pAgendaDB = (AgendaDB_t*)pData;
    /* Save last state */
    fsm_SaveLastState();

    /* Cleanup */
    state->exitState( state, pAgendaDB );
    fsm_RestoreDefault( state );

    /* init the state */
    state->id = STATE_ID_LIST_EVENTS;
    state->exitState = stateExit;
    state->stateTransition = fsm_TransitionToListEventsState;

    state->eventHandler[EVT_BACK_OR_EXIT_AGENDA] = agendaExitHandler;
    state->eventHandler[EVT_ADD_EVENT] = addEventHandler;
    state->eventHandler[EVT_SEARCH] = searchHandler;
    state->eventHandler[EVT_CONSOLE_SCREEN_RELOAD] = screenReloadHandler;
    state->eventHandler[EVT_REMINDER] = eventReminderHandler;
    state->eventHandler[EVT_USER_INPUT_RECEIVED] = userInputHandler;

    /* Console screen */
    DB_EventListToday();
    Console_ListTodayEventsScreen(pAgendaDB);
}

static void stateExit( UNUSED_PARAM ScreenStatePtr_t state, AgendaDB_t *  pAgendaDB )
{
    system("clear");
}


static void agendaExitHandler(ScreenStatePtr_t state, AgendaDB_t *  pAgendaDB )
{
    fsm_TransitionToNextEventState(state, pAgendaDB);
}

static void addEventHandler(ScreenStatePtr_t state, AgendaDB_t *  pAgendaDB)
{
    // Make the transition to add event state
    fsm_TransitionToAddEventState(state, pAgendaDB);
}

static void searchHandler(ScreenStatePtr_t state, AgendaDB_t *  pAgendaDB)
{
    // Make the transition to search state
    fsm_TransitionToSearchState(state, pAgendaDB);
}

static void screenReloadHandler(ScreenStatePtr_t state, AgendaDB_t *  pAgendaDB)
{
    DB_EventListToday();
    Console_ListTodayEventsScreen(pAgendaDB);
}

static void eventReminderHandler(ScreenStatePtr_t state, AgendaDB_t *  pAgendaDB)
{
    // Make the transition to reminder state
    fsm_TransitionToReminderState(state, pAgendaDB);
}

static void userInputHandler(ScreenStatePtr_t state, AgendaDB_t *  pAgendaDB)
{
    printf( "\n\n>> command not supported..\n\n" );
    sleep( 2 );
    screenReloadHandler(state, pAgendaDB);
}
/* End of file ---------------------------------------------------------------*/
/* fsm_list_events.c */
