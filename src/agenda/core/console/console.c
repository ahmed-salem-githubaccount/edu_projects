/**
  ******************************************************************************
  * @file     console.c
  * @author   ahmed
  * @version
  * @date     Aug 24, 2018
  * @brief    
  ******************************************************************************
  */

/* Includes ("...") ----------------------------------------------------------*/
#include "db.h"
#include "agenda_config.h"
#include "agenda.h"
#include "agenda_interface.h"
#include "utils.h"
#include "console.h"
#include "fsm.h"
#include "main.h"
/* Includes (<...>) ----------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <unistd.h>
#include <pthread.h>
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void Console_ScreenHeader(AgednaUserInfo_t * pUserInfo);
/* Global constants definitions ----------------------------------------------*/
/* Global variable definitions -----------------------------------------------*/
uint8_t nextEventString[200] = {0};
uint8_t EventListString[3000] = {0};
ConsoleAddEventState_t consoleAddEventState = CONSOLE_ADD_EVT_START;
ConsoleSearchState_t consoleSearchState = CONSOLE_SEARCH_EVT_START;
/* Module functions implementation -------------------------------------------*/
void Console_InitScreen( AgednaUserInfo_t * pUserInfo )
{
    char userInput = 0;
    /* Screen header */
    Console_ScreenHeader(pUserInfo);

    printf("        ***Personal Info***        \n");
    printf("To skip  Entering your personal Info Press [ESC] and any other Key to continue ");
    scanf("%c", &userInput);

    if(CONSOLE_USER_INPUT_BACK_EXIT == userInput)
    {
        printf("you didn't tell me your name :(.. Next time I will ask again\n");
        sleep(2);
        flushstdin();
        return;
    }

    /* Cleanup stdin buffer*/
    if('\n' != userInput)
    {
        flushstdin();

    }
    /* Ask the user for his info*/

    printf("Please Enter your name: ");
    fgets(pUserInfo->name, sizeof(pUserInfo->name), stdin);
    removeTrailingNewline(pUserInfo->name);

    printf("Please Enter your age: ");
    fgets(pUserInfo->age, sizeof(pUserInfo->age), stdin);
    removeTrailingNewline(pUserInfo->age);

    printf("Please Enter your email: ");
    fgets(pUserInfo->email, sizeof(pUserInfo->email), stdin);
    removeTrailingNewline(pUserInfo->email);

    DB_UpdateUserInfo(pUserInfo);

    printf("Thank you! your data is saved\n\r");
    sleep(2);
}

void Console_NextEventScreen(AgendaDB_t* pAgendaDB)
{
    /* Screen header */
    Console_ScreenHeader(&pAgendaDB->userInfo);

    /* Next Event for today */
    printf("Next Event:\n");
    printf("%s", nextEventString);
    printf("\n\n");
#if AGENDA_DEMO_MODE
    printf( "[%c]Add event   [%c]List all events   [%c]Search   [%c]Start Demo   [ESC]Exit\n\r",
               CONSOLE_USER_INPUT_ADD_EVENT, CONSOLE_USER_INPUT_LIST_EVENTS,
               CONSOLE_USER_INPUT_SEARCH, CONSOLE_USER_INPUT_START_DEMO);
#else
    printf( "[%c]Add event   [%c]List all events   [%c]Search   [ESC]Exit\n\r",
               CONSOLE_USER_INPUT_ADD_EVENT, CONSOLE_USER_INPUT_LIST_EVENTS,
               CONSOLE_USER_INPUT_SEARCH);
#endif
}

void Console_ReminderScreen( AgendaDB_t * pAgendaDB)
{
    /* Screen header */
    Console_ScreenHeader(&pAgendaDB->userInfo);
    printf("\n        ***Reminder***        \n");

    /* Next Event for today */
    printf("Hey %s, your next Event due is in %d minutes:\n\n", pAgendaDB->userInfo.name, pAgendaDB->agendaNextEvent.eventDueInMintues);
    printf("%s", nextEventString);
    printf("\n\n");

    printf( "[%c]Dismiss event   [ESC]Back\n\r", CONSOLE_USER_INPUT_DISMISS_REMINDER);
}

void Console_ListTodayEventsScreen(AgendaDB_t* pAgendaDB)
{
    /* Screen header */
    Console_ScreenHeader(&pAgendaDB->userInfo);

    /* List Today's Events */
    printf( "\nToday's Events:\n" );
    printf("%s", EventListString);
    printf("\n\n");

    printf( "[%c]Add event   [%c]Search   [ESC]Back\n",CONSOLE_USER_INPUT_ADD_EVENT,
            CONSOLE_USER_INPUT_SEARCH);
}

void Console_AddEventScreen(AgendaDB_t* pAgendaDB)
{
    char state = 0;
    AgendaEvent_t * pAgendaNewEvent = &pAgendaDB->agendaNewEvent;
    char * userInputString = NULL;

    switch( consoleAddEventState )
    {
        case CONSOLE_ADD_EVT_START:
            /* Screen header */
            Console_ScreenHeader( &pAgendaDB->userInfo );
            printf( "[ESC]Back\n\n\r" );
            /* Build the date string */
            getCurrentDateFormatted( pAgendaDB->agendaNewEvent.date );

            /* Next state preparation*/
            consoleAddEventState = CONSOLE_ADD_EVT_TIME_EXPECTED;
            printf( "\n\rEvent Time(HH:MM): " );
            break;
        case CONSOLE_ADD_EVT_TIME_EXPECTED:
            /* Check the time string*/
            userInputString = appGetUserInputString();
            if( !checkAndFormatTime( userInputString ) )
            {
                printf( "\n\n>> Failed, Time entered is not correct..\n\n" );
                sleep( 3 );
                consoleAddEventState = CONSOLE_ADD_EVT_START;
                /* Console screen */
                Console_AddEventScreen(pAgendaDB);
                return;
            }

            strcpy( pAgendaNewEvent->time, userInputString );
#if AGENDA_DEMO_MODE
            pAgendaNewEvent->evtTimeInSec = ConvertFormattedTimetoSec( pAgendaNewEvent->time );
#endif
            /* Next state preparation*/
            consoleAddEventState = CONSOLE_ADD_EVT_EVT_EXPECTED;
            printf( "\n\rWhat is this event about?(max.64 character): " );
            break;

        case CONSOLE_ADD_EVT_EVT_EXPECTED:
            userInputString = appGetUserInputString();
            strcpy( pAgendaNewEvent->event, userInputString );

            /* Next state preparation*/
            consoleAddEventState = CONSOLE_ADD_EVT_STATE_EXPECTED;
            printf( "\n\rWhat is the event state(done, undone)?: " );
            break;

        case CONSOLE_ADD_EVT_STATE_EXPECTED:
            userInputString = appGetUserInputString();
            if( !strcmp( "done", userInputString ) || !strcmp( "undone", userInputString ) )
            {
                strcpy( pAgendaNewEvent->state, userInputString );
                /*Call DB function*/
                DB_AddEvent( pAgendaNewEvent );

                consoleAddEventState = CONSOLE_ADD_EVT_START;
                Console_ScreenHeader( &pAgendaDB->userInfo );
                printf( "\n>> Event added!\n\n" );
                printf( "[%c]Add another event   [ESC]Back\n\r", CONSOLE_USER_INPUT_ADD_EVENT );
            }
            else
            {
                printf( "\n\n>> Failed, State entered is not correct.\n\n" );
                sleep( 3 );
                consoleAddEventState = CONSOLE_ADD_EVT_START;
                /* Console screen */
                Console_AddEventScreen(pAgendaDB);
                return;
            }
            break;
    }
}

void Console_SearchScreen( AgendaDB_t* pAgendaDB )
{
    char tempStr[256] = {0};
    char evtStr[256] = {0};
    char userInput = 0;
    char * userInputString = NULL;
    AgendaEvent_t * evt = &pAgendaDB->agendaSearchEvent;

    switch(consoleSearchState)
    {
        case CONSOLE_SEARCH_EVT_START:
            /* Screen header */
            Console_ScreenHeader( &pAgendaDB->userInfo );
            printf( "[ESC]Back\n\n\r" );
            /* Next state preparation*/
            consoleSearchState = CONSOLE_SEARCH_EVT_TIME_EXPECTED;
            printf( "\n\rPlease Enter Time(<HH:MM> or <now> for current time): " );
            break;

        case CONSOLE_SEARCH_EVT_TIME_EXPECTED:
            userInputString = appGetUserInputString();
            if( !strcmp( userInputString, "now" ) )
            {
                /*Get current time*/
                getCurrentTimeFormatted( userInputString );
            }
            else if( !checkAndFormatTime( userInputString ) )
            {
                printf( "\n\n>> Failed, Time entered is not correct..\n\n" );
                sleep( 3 );
                consoleSearchState = CONSOLE_SEARCH_EVT_START;
                Console_SearchScreen(pAgendaDB);
                return;
            }

            /* Search for the time in DB*/
            if( !DB_SearchForEventByTime( userInputString, evt ) )
            {
                printf( "\n\n>> Failed, event not found..\n\n" );
                sleep( 3 );
                consoleSearchState = CONSOLE_SEARCH_EVT_START;
                Console_SearchScreen(pAgendaDB);
                return;
            }

            /* Format Console string */
            strcpy( evtStr, "\n>> " );
            sprintf( tempStr, "%-64s", evt->event );      // to have fixed width for the event field
            strcat( evtStr, tempStr );
            strcat( evtStr, "   " );
            strcat( evtStr, "State: " );
            sprintf( tempStr, "%-6s", evt->state );
            strcat( evtStr, tempStr );
            strcat( evtStr, "   " );
#if !AGENDA_DEMO_MODE
            strcat( evtStr, "Date: " );
            strcat( evtStr, evt->date );
            strcat( evtStr, "   " );
#endif
            strcat( evtStr, "Time: " );
            strcat( evtStr, evt->time );
            strcat( evtStr, "\n" );

            printf( "\nEvent Found:\n" );
            printf( "%s\n", evtStr );
            /* Check event state*/
            if( !strcmp( evt->state, "done" ) )
            {
                printf( "Chill, This event is already done :)\n\n" );
                /*Prepare for exit state*/
                consoleSearchState = CONSOLE_SEARCH_EVT_START;
                printf( "[%c]Add event   [%c]List all events   [%c]Search   [ESC]Back\n\r",
                CONSOLE_USER_INPUT_ADD_EVENT,
                        CONSOLE_USER_INPUT_LIST_EVENTS,
                        CONSOLE_USER_INPUT_SEARCH );
            }
            else
            {
                /* Prepare for Next state*/
                consoleSearchState = CONSOLE_SEARCH_EVT_STATE_EXPECTED;
                sleep( 3 ); //wait for 3sec

                /* Screen header */
                Console_ScreenHeader( &pAgendaDB->userInfo );
                printf( "\nEvent Found:\n" );
                printf( "%s\n", evtStr );
                printf( "\n\rThis Event is not yet Done, Did you already do this event?(y/n): " );
            }
            break;

        case CONSOLE_SEARCH_EVT_STATE_EXPECTED:
            userInputString = appGetUserInputString();
            stringToLower(userInputString);
            if( !strcmp(userInputString, "yes") || !strcmp(userInputString, "y"))
            {
                DB_updateEventState( evt->id, "done" );
                printf( "\n\r>> Event state updated to <done> !\n\n" );
            }
            /*Prepare for exit state*/
            consoleSearchState = CONSOLE_SEARCH_EVT_START;
            printf( "\n\n[%c]Add event   [%c]List all events   [%c]Search   [ESC]Back\n\r",
            CONSOLE_USER_INPUT_ADD_EVENT, CONSOLE_USER_INPUT_LIST_EVENTS, CONSOLE_USER_INPUT_SEARCH );
            break;
    }
}


#if AGENDA_DEMO_MODE
void Console_DemoStartScreen(AgendaDB_t *  pAgendaDB)
{
    char userInput = 0;

    /* Screen header */
    Console_ScreenHeader(&pAgendaDB->userInfo);

    printf( "Demo has Started!... All events states are reset to undone)\n" );
    printf( "press any Key to Continue..." );
    getchar();
}

void Console_DemoEndedScreen(AgendaDB_t* pAgendaDB)
{
    char userInput = 0;

    /* Screen header */
    Console_ScreenHeader(&pAgendaDB->userInfo);

    printf( "Demo has Ended!... I wish you liked the grandma Agenda :)\n\n" );
    sleep(3);
}
#endif


static void Console_ScreenHeader(AgednaUserInfo_t * pUserInfo)
{
    system("clear");
    printf( "\nWelcome %s\n\r", pUserInfo->name );
    printf( "________________________________________\n\n\r" );
}

void flushstdin(void)
{
    char c;
    while ( (c != '\n') && c != EOF )
    {
        c = getchar();
    }
}


/* End of file ---------------------------------------------------------------*/
/* console.c */
