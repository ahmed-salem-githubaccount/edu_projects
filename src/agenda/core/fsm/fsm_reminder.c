/**
  ******************************************************************************
  * @file     fsm_reminder.c
  * @author   Ahmed
  * @version
  * @date     Aug 26, 2018
  * @brief    
  ******************************************************************************
  */

/* Includes ("...") ----------------------------------------------------------*/
#include "utils.h"
#include "agenda.h"
#include "agenda_config.h"
#include "db.h"
#include "console.h"
#include "fsm.h"
#include "fsm_next_event.h"
#include "fsm_reminder.h"
/* Includes (<...>) ----------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void stateExit( UNUSED_PARAM ScreenStatePtr_t state, AgendaDB_t *  pAgendaDB );
static void agendaExitHandler(ScreenStatePtr_t state, AgendaDB_t *  pAgendaDB);
static void screeReloadHandler(ScreenStatePtr_t state, AgendaDB_t *  pAgendaDB);
static void eventReminderHandler(ScreenStatePtr_t state, AgendaDB_t *  pAgendaDB);
static void eventDismissHandler(ScreenStatePtr_t state, AgendaDB_t *  pAgendaDB);
/* Global constants definitions ----------------------------------------------*/
/* Global variable definitions -----------------------------------------------*/
/* Module functions implementation -------------------------------------------*/

void fsm_TransitionToReminderState(ScreenStatePtr_t state, void * pData)
{
    AgendaDB_t * pAgendaDB = (AgendaDB_t *)pData;
    /* Save last state */
    fsm_SaveLastState();

    /* Cleanup */
    state->exitState(state, pAgendaDB);
    fsm_RestoreDefault(state);

    /* init the state */
    state->id = STATE_ID_REMINDER;
    state->exitState = stateExit;
    state->stateTransition = fsm_TransitionToReminderState;

    state->eventHandler[EVT_BACK_OR_EXIT_AGENDA] = agendaExitHandler;
    state->eventHandler[EVT_CONSOLE_SCREEN_RELOAD] = screeReloadHandler;
    state->eventHandler[EVT_REMINDER] = eventReminderHandler;
    state->eventHandler[EVT_DISMISS_REMINDER] = eventDismissHandler;

    /* Console screen */
    Console_ReminderScreen(pAgendaDB);
}

static void stateExit( UNUSED_PARAM ScreenStatePtr_t state, AgendaDB_t *  pAgendaDB )
{
    system("clear");
}

static void agendaExitHandler(ScreenStatePtr_t state, AgendaDB_t *  pAgendaDB)
{
    /* Back to last state */
    fsm_RestoreLastState(pAgendaDB);
}

static void screeReloadHandler(ScreenStatePtr_t state, AgendaDB_t *  pAgendaDB)
{
    Console_ReminderScreen(pAgendaDB);
}

static void eventReminderHandler(ScreenStatePtr_t state, AgendaDB_t *  pAgendaDB)
{
    /* Console screen */
    Console_ReminderScreen(pAgendaDB);
}

static void eventDismissHandler(ScreenStatePtr_t state, AgendaDB_t *  pAgendaDB)
{
    /* Mark the event as Done*/
    DB_updateEventState(eventDueID, "done");

    /* Back to last state */
    /* cleanup */
    memset( &pAgendaDB->agendaNextEvent, 0, sizeof( AgendaEvent_t ) );
    fsm_RestoreLastState(pAgendaDB);
}


/* End of file ---------------------------------------------------------------*/
/* fsm_reminder.c */
