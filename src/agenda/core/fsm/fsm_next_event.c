/**
  ******************************************************************************
  * @file     fsm_next_event.c
  * @author   Ahmed
  * @version
  * @date     Aug 24, 2018
  * @brief    
  ******************************************************************************
  */

/* Includes ("...") ----------------------------------------------------------*/
#include "utils.h"
#include "db.h"
#include "agenda_config.h"
#include "agenda.h"
#include "console.h"
#include "fsm.h"
#include "fsm_add_event.h"
#include "fsm_list_events.h"
#include "fsm_next_event.h"
#include "fsm_reminder.h"
#include "fsm_search.h"
#include "main.h"
/* Includes (<...>) ----------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void stateExit( UNUSED_PARAM ScreenStatePtr_t state, AgendaDB_t *  pAgendaDB );
static void addEventHandler(ScreenStatePtr_t state, AgendaDB_t *  pAgendaDB);
static void listEventsHandler(ScreenStatePtr_t state, AgendaDB_t *  pAgendaDB);
static void searchHandler(ScreenStatePtr_t state, AgendaDB_t *  pAgendaDB);
static void screenReloadHandler(ScreenStatePtr_t state, AgendaDB_t *  pAgendaDB);
static void agendaExitHandler(ScreenStatePtr_t state, AgendaDB_t *  pAgendaDB);
static void eventReminderHandler(ScreenStatePtr_t state, AgendaDB_t *  pAgendaDB);
static void userInputHandler(ScreenStatePtr_t state, AgendaDB_t *  pAgendaDB);
#if AGENDA_DEMO_MODE
static void demoStartHandler(ScreenStatePtr_t state, AgendaDB_t *  pAgendaDB);
#endif
/* Global constants definitions ----------------------------------------------*/
/* Global variable definitions -----------------------------------------------*/
/* Module functions implementation -------------------------------------------*/

void fsm_TransitionToNextEventState(ScreenStatePtr_t state, void * pData)
{
    AgendaDB_t *  pAgendaDB = (AgendaDB_t*)pData;

    /* Save last state */
    fsm_SaveLastState();

    /* Cleanup */
    state->exitState(state, pAgendaDB);
    fsm_RestoreDefault(state);

    /* init the state */
    state->id = STATE_ID_DEFAULT;
    state->exitState = stateExit;
    state->stateTransition = fsm_TransitionToNextEventState;

    state->eventHandler[EVT_BACK_OR_EXIT_AGENDA] = agendaExitHandler;
    state->eventHandler[EVT_ADD_EVENT] = addEventHandler;
    state->eventHandler[EVT_LIST_EVENTS] = listEventsHandler;
    state->eventHandler[EVT_SEARCH] = searchHandler;
    state->eventHandler[EVT_CONSOLE_SCREEN_RELOAD] = screenReloadHandler;
    state->eventHandler[EVT_REMINDER] = eventReminderHandler;
    state->eventHandler[EVT_USER_INPUT_RECEIVED] = userInputHandler;
#if AGENDA_DEMO_MODE
    state->eventHandler[EVT_START_DEMO] = demoStartHandler;
#endif

    /* Console screen */
    DB_QueryNextEventToday(&pAgendaDB->agendaNextEvent);
    Console_NextEventScreen(pAgendaDB);
}

static void stateExit( UNUSED_PARAM ScreenStatePtr_t state, AgendaDB_t *  pAgendaDB )
{
    system("clear");
}


static void agendaExitHandler(ScreenStatePtr_t state, AgendaDB_t *  pAgendaDB)
{
    char userInput = 0;
    printf("Are you Sure you want to Exit your Agenda?(y/n): ");
    fflush(stdin);
    scanf("%c", &userInput);
    flushstdin();
    userInput = tolower(userInput);
    if(userInput == 'y')
    {
        appExit();
    }
    else
    {
        fsm_TransitionToNextEventState(state, pAgendaDB);
    }
}

static void addEventHandler(ScreenStatePtr_t state, AgendaDB_t *  pAgendaDB)
{
    // Make the transition to add event state
    fsm_TransitionToAddEventState(state, pAgendaDB);
}

static void listEventsHandler(ScreenStatePtr_t state, AgendaDB_t *  pAgendaDB)
{
    // Make the transition to list events state
    fsm_TransitionToListEventsState( state, pAgendaDB);
}

static void searchHandler(ScreenStatePtr_t state, AgendaDB_t *  pAgendaDB)
{
    // Make the transition to search state
    fsm_TransitionToSearchState(state, pAgendaDB);
}

static void screenReloadHandler(ScreenStatePtr_t state, AgendaDB_t *  pAgendaDB)
{
    // Make the transition to same state
    /* Console screen */
    Console_NextEventScreen(pAgendaDB);
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

#if AGENDA_DEMO_MODE
static void demoStartHandler(ScreenStatePtr_t state, AgendaDB_t *  pAgendaDB)
{
    /* Start the Demo */
    DB_updateAllEventsState("undone");
    agendaDemoStarted = true;
    Console_DemoStartScreen(pAgendaDB);

    /* Console screen */
    DB_QueryNextEventToday(&pAgendaDB->agendaNextEvent);
    screenReloadHandler(state, pAgendaDB);
}
#endif

/* End of file ---------------------------------------------------------------*/
/* fsm_next_event.c */
