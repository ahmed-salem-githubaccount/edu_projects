/**
  ******************************************************************************
  * @file     agenda_interface.c
  * @author   Ahmed
  * @version
  * @date     Aug 22, 2018
  * @brief    
  ******************************************************************************
  */

/* Includes ("...") ----------------------------------------------------------*/
#include "agenda_interface.h"
#include "agenda_config.h"
#include "time.h"
#include "utils.h"
#include "db.h"
#include "main.h"
#include "fsm.h"
/* Includes (<...>) ----------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <inttypes.h>
#include <unistd.h>
#include <pthread.h>
/* Private defines ------------- ----------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* To be called inside agendaTask */
//static remindersCB_t remindersCB = NULL;
/* Private function prototypes -----------------------------------------------*/
/* Global constants definitions ----------------------------------------------*/
/* Global variable definitions -----------------------------------------------*/
int eventDueID = 0;
/* Module functions implementation -------------------------------------------*/

void * agendaTask(void * vargp)
{
    char** str_list;
    char* endptr;
    int nextEventHH;
    int nextEventMM;
    int nextEventSS;
    int timeDiff;
    int currentTimeInSec = 0;
    AgendaDB_t *  pAgendaDB = (AgendaDB_t*)vargp;
    AgendaEvent_t * agendaNextEvent = &pAgendaDB->agendaNextEvent;

    while( 1 )
    {
        /* Thread is executed every 1sec*/
        sleep( 1 );

        do
        {
#if AGENDA_DEMO_MODE
            if( !agendaDemoStarted)
            {
                //Don't continue
                break;
            }
#endif

            DB_QueryNextEventToday(agendaNextEvent);

#if AGENDA_DEMO_MODE
           if(!getCurrentTimeinSec(&currentTimeInSec))
           {
               fsm_HandleEvent( EVT_END_DEMO, pAgendaDB );
           }
#else
           getCurrentTimeinSec(&currentTimeInSec);
#endif

            if( agendaNextEvent->eventPresent )
            {
                str_list = str_split( agendaNextEvent->time, ':' );  // split the time string
                nextEventHH = strtoumax( str_list[0], &endptr, 10 );
                nextEventMM = strtoumax( str_list[1], &endptr, 10 );
                nextEventSS = strtoumax( str_list[2], &endptr, 10 );

                timeDiff = ( ( nextEventHH * 60 * 60 ) + ( nextEventMM * 60 ) + nextEventSS ) - currentTimeInSec;
                if( timeDiff < AGENDA_REMINDER_1_IN_SEC && ( !agendaNextEvent->firstReminderFired ) ) // if event created and due time is less than 10 minutes
                {
                    agendaNextEvent->firstReminderFired = true;
                    agendaNextEvent->eventDueInMintues = timeDiff / 60;
                    fsm_HandleEvent( EVT_REMINDER, pAgendaDB ); // trigger reminder for this event
                }
                else if( timeDiff == AGENDA_REMINDER_1_IN_SEC )
                {
                    agendaNextEvent->firstReminderFired = true;
                    agendaNextEvent->eventDueInMintues = timeDiff / 60;
                    fsm_HandleEvent( EVT_REMINDER, pAgendaDB ); // trigger reminder for this event
                }
                else if( timeDiff == AGENDA_REMINDER_3_IN_SEC )
                {
                    eventDueID = agendaNextEvent->id;
                    agendaNextEvent->eventDueInMintues = timeDiff / 60;
                    fsm_HandleEvent( EVT_REMINDER, pAgendaDB ); // trigger reminder for this event
                }
            }

        }while(0);
    }
    return (void*) 0;
}

/*bool agendaAddEntry(AgendaEntry_t * agendaEntry)
{

}

bool agendaRetrieveEntry(AgendaEntry_t * agendaEntry)
{

}

bool agendaDeleteEntry( AgendaEntry_t * agendaEntry )
{

}

void agendaListEntries(struct tm date)
{

}



/*
 * to be called by application in init phase
 * */
/*void agendaRegisterRemindersCB(remindersCB_t cb)
{
    if (NULL != cb)
    {
        remindersCB = cb;
    }
}*/
/* End of file ---------------------------------------------------------------*/
/* agenda_interface.c */
