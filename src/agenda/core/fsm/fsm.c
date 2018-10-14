/**
  ******************************************************************************
  * @file     fsm.c
  * @author   Ahmed
  * @version
  * @date     Aug 24, 2018
  * @brief    
  ******************************************************************************
  */

/* Includes ("...") ----------------------------------------------------------*/
#include "utils.h"
#include "db.h"
#include "console.h"
#include "fsm.h"
#include "fsm_next_event.h"
#include "agenda_interface.h"
#include "main.h"
/* Includes (<...>) ----------------------------------------------------------*/
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static ScreenState_t currentState;
static ScreenState_t lastState;
/* Private function prototypes -----------------------------------------------*/
static void fsm_DefaultEventHandle( UNUSED_PARAM ScreenStatePtr_t state, void * pData);
static void screen_fsm_DefaultExit( UNUSED_PARAM ScreenStatePtr_t state, void * pData );
static void screen_fsm_StateTransition( UNUSED_PARAM ScreenStatePtr_t state, void *pData );
#if AGENDA_DEMO_MODE
static void fsm_demoStopmHandler( UNUSED_PARAM ScreenStatePtr_t state, AgendaDB_t *  pAgendaDB);
#endif
/* Global constants definitions ----------------------------------------------*/
/* Global variable definitions -----------------------------------------------*/
/* Module functions implementation -------------------------------------------*/

void fsm_Init(AgendaDB_t * pDB)
{
    fsm_RestoreDefault(&currentState);

    // Transition to Default state
    fsm_TransitionToNextEventState( &currentState,&pDB->agendaNextEvent );
}


void fsm_HandleEvent( EVT_t event, void * pData )
{
    assert( event < EVT_CNT );
    currentState.eventHandler[event]( &currentState, pData );
}


void fsm_RestoreDefault( ScreenStatePtr_t state )
{
    uint8_t index = 0;

    // Set all event handlers to DefaultHandleButtonEvent
    for( index = 0; index < EVT_CNT; index++ )
    {
        state->eventHandler[index] = fsm_DefaultEventHandle;
    }
    state->exitState = screen_fsm_DefaultExit;
    state->id = STATE_ID_DEFAULT;
    state->stateTransition = screen_fsm_StateTransition;

#if AGENDA_DEMO_MODE
    state->eventHandler[EVT_END_DEMO] = fsm_demoStopmHandler;
#endif
}

StateId_t fsm_getCurrentStateID(void)
{
    return currentState.id;
}

void fsm_RestoreLastState(void * pData)
{
    if( STATE_ID_REMINDER == lastState.id )
    {
        fsm_TransitionToNextEventState( &currentState, pData );
    }
    else
    {
        lastState.stateTransition(&currentState, pData);
    }

}

void fsm_SaveLastState(void)
{
    lastState = currentState;
}

/**
 * @brief Default function handling all events
 * @param state - current state
 */
static void fsm_DefaultEventHandle( UNUSED_PARAM ScreenStatePtr_t state, void * pData)
{
    //Do nothing
}

#if AGENDA_DEMO_MODE
static void fsm_demoStopmHandler( UNUSED_PARAM ScreenStatePtr_t state, AgendaDB_t *  pAgendaDB)
{
    agendaDemoStarted = false;
   /* load the Demo end screen */
    Console_DemoEndedScreen(pAgendaDB);
    /* Go to Next event state */
    fsm_TransitionToNextEventState( state, pAgendaDB );
}
#endif


/**
 * @brief NULL pattern implementation for Exit
 * @param state - current state
 */
static void screen_fsm_DefaultExit( UNUSED_PARAM ScreenStatePtr_t state, void * pData )
{
}

/**
 * @brief NULL pattern implementation for state transition
 * @param state - current state
 */
static void screen_fsm_StateTransition( UNUSED_PARAM ScreenStatePtr_t state, void * pData )
{
}

/* End of file ---------------------------------------------------------------*/
/* fsm.c */
