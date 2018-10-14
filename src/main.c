/**
  ******************************************************************************
  * @file     main.c
  * @author   Ahmed
  * @version
  * @date     Aug 22, 2018
  * @brief    
  ******************************************************************************
  */

/* Includes ("...") ----------------------------------------------------------*/
#include "agenda.h"
#include "sqlite3.h"
#include "agenda_config.h"
#include "db.h"
#include "console.h"
#include "fsm.h"
#include "main.h"
#include "agenda_interface.h"
#include "utils.h"
/* Includes (<...>) ----------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <pthread.h>
#include <string.h>
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Global constants definitions ----------------------------------------------*/
/* Global variable definitions -----------------------------------------------*/
#if AGENDA_DEMO_MODE
bool agendaDemoStarted = false;
#define AGENDA_DB_PATH          "./DBs/agenda_demo.db"
#else
#define AGENDA_DB_PATH          "./DBs/agenda.db"
#endif

static UserInput_t appUserInput = {0};
/* Module functions implementation -------------------------------------------*/



int main()
{
    AgendaDB_t agendaDB = {
        0 };

    char userInput = 0;
    pthread_t thread1_id;
    //  pthread_t thread2_id;

    /* Open database */
    DB_Open( AGENDA_DB_PATH );

    /* Create Tables in data base if not exist */
    DB_CreateAgnedaTable();
    DB_CreateUserInfoTable();

    /* Read User info */
    DB_ReadUserInfo( &agendaDB.userInfo );
    if( !agendaDB.userInfo.userInfoPresent )
    {
        /* Enter Console config screen */
        Console_InitScreen( &agendaDB.userInfo );
    }

    // init state machine
    fsm_Init( &agendaDB );
    /* Create Agenda Thread*/
    pthread_create( &thread1_id, NULL, agendaTask, &agendaDB );

    /********** Show Screen **************/
    while( 1 )
    {
        /* Wait for user input*/
        fgets( appUserInput.u.str, sizeof( appUserInput.u.str ), stdin );
        removeTrailingNewline( appUserInput.u.str );

        /*check the user input single or multi char*/
        appUserInput.size = strlen( appUserInput.u.str );
        if( 1 == appUserInput.size )
        {
            /*User input control character*/
            appUserInput.u.singleChar = tolower(appUserInput.u.singleChar);
            switch( appUserInput.u.singleChar )
            {
                case CONSOLE_USER_INPUT_ADD_EVENT:
                    fsm_HandleEvent( EVT_ADD_EVENT, &agendaDB );
                    break;
                case CONSOLE_USER_INPUT_LIST_EVENTS:
                    fsm_HandleEvent( EVT_LIST_EVENTS, &agendaDB );
                    break;
                case CONSOLE_USER_INPUT_SEARCH:
                    fsm_HandleEvent( EVT_SEARCH, &agendaDB );
                    break;
                case CONSOLE_USER_INPUT_BACK_EXIT:
                    fsm_HandleEvent( EVT_BACK_OR_EXIT_AGENDA, &agendaDB );
                    break;
                case CONSOLE_USER_INPUT_DISMISS_REMINDER:
                    fsm_HandleEvent( EVT_DISMISS_REMINDER, &agendaDB );
                    break;
#if AGENDA_DEMO_MODE
                case CONSOLE_USER_INPUT_START_DEMO:
                    fsm_HandleEvent( EVT_START_DEMO, &agendaDB );
                    break;
#endif
                default:
                    /*user Input char, reroute to current state*/
                     fsm_HandleEvent( EVT_USER_INPUT_RECEIVED, &agendaDB );
                    break;
            }
        }
        else if( appUserInput.size > 1 )
        {
            /*user Input string, reroute to current state*/
            fsm_HandleEvent( EVT_USER_INPUT_RECEIVED, &agendaDB );
        }
    }

    return 0;
}


void appExit(void)
{
    DB_Close();
    exit(0);
}

char * appGetUserInputString(void)
{
    return appUserInput.u.str;
}



/* End of file ---------------------------------------------------------------*/
/* main.c */
