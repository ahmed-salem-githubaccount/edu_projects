/**
  ******************************************************************************
  * @file     db.c
  * @author   embedded
  * @version
  * @date     Aug 22, 2018
  * @brief    
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2018 BLOKS. GmbH</center></h2>
  *
  *
  ******************************************************************************
  */

/* Includes ("...") ----------------------------------------------------------*/
#include "sqlite3.h"
#include "agenda.h"
#include "agenda_config.h"
#include "main.h"
#include "console.h"
#include "utils.h"
/* Includes (<...>) ----------------------------------------------------------*/
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <inttypes.h>

/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void DB_ErrHandling(int errorCode);
static int DB_NextEventTodayCB(void *arg, int Columncount, char **data, char **columns);
static int DB_EventListTodayCB(void *arg, int Columncount, char **data, char **columns);
static int DB_ReadUserInfoCB(void *arg, int Columncount, char **data, char **columns);
/* Global constants definitions ----------------------------------------------*/
/* Global variable definitions -----------------------------------------------*/

static sqlite3 *db;

/* Module functions implementation -------------------------------------------*/


void DB_Open(char * dbName)
{
    assert(dbName != NULL);
    assert(&db != NULL);

    DB_ErrHandling( sqlite3_open( dbName, &db ) );
}

void DB_Close()
{
    assert(db != NULL);

    DB_ErrHandling( sqlite3_close( db ) );
}

void DB_CreateAgnedaTable(void)
{
    assert(db != NULL);

    char *sql;
#if AGENDA_DEMO_MODE
    sql = "CREATE TABLE if not exists Agenda(" \
                    "id             INTEGER PRIMARY KEY AUTOINCREMENT," \
                    "Event          TEXT               NOT NULL," \
                    "state          TEXT               NOT NULL," \
                    "evtDate        TEXT               NOT NULL," \
                    "evtTime        TEXT               NOT NULL," \
                    "evtTimeInSec   INT                NOT NULL)" \
                    ";";
#else
    sql = "CREATE TABLE if not exists Agenda(" \
                     "id             INTEGER PRIMARY KEY AUTOINCREMENT," \
                     "Event          TEXT               NOT NULL," \
                     "state          TEXT               NOT NULL," \
                     "evtDate        TEXT               NOT NULL," \
                     "evtTime        TEXT               NOT NULL)" \
                     ";";
#endif

    DB_ErrHandling( sqlite3_exec( db, sql, (void*) 0, (void*) 0, (void*) 0 ) );
}

void DB_CreateUserInfoTable(void)
{
    assert(db != NULL);

    char *sql;

    sql = "CREATE TABLE if not exists UserInfo(" \
                    "id              INTEGER PRIMARY KEY AUTOINCREMENT," \
                    "name            TEXT               NOT NULL," \
                    "age             TEXT               NULL," \
                    "email           TEXT               NULL" \
                    ");";

    DB_ErrHandling( sqlite3_exec( db, sql, (void*) 0, (void*) 0, (void*) 0 ) );
}


void DB_ReadUserInfo(const AgednaUserInfo_t * pUserInfo )
{
    assert(db != NULL);

    char *sql;

    sql = "SELECT * from UserInfo ;";

    DB_ErrHandling( sqlite3_exec( db, sql, DB_ReadUserInfoCB, (void *)pUserInfo, (void*) 0 ) );
}

void DB_UpdateUserInfo(const AgednaUserInfo_t * pUserInfo )
{
    assert(db != NULL);

    char sql[1000] = { 0 };
    char sqlValues[1000] = { 0 };

    /*Create SQL statement*/
    strcpy( sql, "INSERT INTO UserInfo (name,age,email) " );
    /* Build the rest if SQL statement*/
    strcat( sqlValues, "VALUES ('" );
    strcat( sqlValues, pUserInfo->name );
    strcat( sqlValues, "'" );
    strcat( sqlValues, "," );
    strcat( sqlValues, "'" );
    strcat( sqlValues, pUserInfo->age );
    strcat( sqlValues, "'" );
    strcat( sqlValues, "," );
    strcat( sqlValues, "'" );
    strcat( sqlValues, pUserInfo->email );
    strcat( sqlValues, "');" );
    //SQL complete statement
    strcat( sql, sqlValues ); //ex."INSERT INTO UserInfo (name,age,email) VALUES ('John', '50', 'john@xx.com'); ";


    /* Execute SQL statement */
    DB_ErrHandling( sqlite3_exec( db, sql, NULL, NULL, NULL ) );
}

void DB_QueryNextEventToday(AgendaEvent_t * agendaNextEvent)
{
    assert(db != NULL);

    /* clear the event present flag*/
    agendaNextEvent->eventPresent = false;

#if AGENDA_DEMO_MODE
    char sql[256] = {0};
    char timeInSec[32] = { 0 };

    /*Build the SQL statement*/
    sprintf( timeInSec, "%d", currentTimeInSec);
    strcpy(sql, "SELECT * from Agenda WHERE evtTimeInSec >= ");
    strcat( sql, timeInSec );
    strcat( sql, " AND state = 'undone'  ORDER BY evtTimeInSec ASC Limit 1;");
#else
    char *sql;
    sql = "SELECT * from Agenda WHERE evtTime >= time('now', 'localtime') AND date(evtDate) = date('now', 'localtime')" \
                    " AND state = 'undone'  ORDER BY evtTime ASC Limit 1;";
#endif

    /* Clear the Next event string */
    strcpy( nextEventString, "No More scheduled events for today.. Have fun ;)\n" );
    /* Execute SQL statement */
    DB_ErrHandling( sqlite3_exec( db, sql, DB_NextEventTodayCB, (void *)agendaNextEvent, (void*) 0 ) );
}

void DB_EventListToday(void)
{
    assert(db != NULL);
    assert(EventListString != NULL);

    char *sql;

#if AGENDA_DEMO_MODE
    sql = "SELECT * from Agenda ORDER BY evtTime ASC ;";
#else
    sql = "SELECT * from Agenda WHERE date(evtDate) = date('now', 'localtime') ORDER BY evtTime ASC ;";
#endif


    /* Clear the event list string */
    strcpy( EventListString, "" );
    /* Execute SQL statement */
    DB_ErrHandling( sqlite3_exec( db, sql, DB_EventListTodayCB, EventListString, (void*) 0 ) );
}


void DB_AddEvent(AgendaEvent_t * agendaNewEvent)
{
    char sql[1000] = {0};
    char sqlValues[1000] = {0};

#if AGENDA_DEMO_MODE
    char timeInSec[16] = {0};
    sprintf( timeInSec, "%d", agendaNewEvent->evtTimeInSec );

    /*Create SQL statement*/
    // values
    strcat( sqlValues, "VALUES ('" );
    strcat( sqlValues, agendaNewEvent->event );
    strcat( sqlValues, "'" );
    strcat( sqlValues, "," );
    strcat( sqlValues, "'" );
    strcat( sqlValues, agendaNewEvent->state );
    strcat( sqlValues, "'" );
    strcat( sqlValues, "," );
    strcat( sqlValues, "'" );
    strcat( sqlValues, agendaNewEvent->date );
    strcat( sqlValues, "'" );
    strcat( sqlValues, "," );
    strcat( sqlValues, "'" );
    strcat( sqlValues, agendaNewEvent->time );
    strcat( sqlValues, "'" );
    strcat( sqlValues, "," );
    strcat( sqlValues, timeInSec );
    strcat( sqlValues, ");" );

    strcpy( sql, "INSERT INTO Agenda (Event,state,evtDate,evtTime,evtTimeInSec) " );

#else

    /*Create SQL statement*/
    // values
    strcat( sqlValues, "VALUES ('" );
    strcat( sqlValues, agendaNewEvent->event );
    strcat( sqlValues, "'" );
    strcat( sqlValues, "," );
    strcat( sqlValues, "'" );
    strcat( sqlValues, agendaNewEvent->state );
    strcat( sqlValues, "'" );
    strcat( sqlValues, "," );
    strcat( sqlValues, "'" );
    strcat( sqlValues, agendaNewEvent->date );
    strcat( sqlValues, "'" );
    strcat( sqlValues, "," );
    strcat( sqlValues, "'" );
    strcat( sqlValues, agendaNewEvent->time );
    strcat( sqlValues, "'" );
    strcat( sqlValues, ");" );

    strcpy( sql, "INSERT INTO Agenda (Event,state,evtDate,evtTime) " );
#endif

    strcat( sql, sqlValues );

    /* ex. sql = "INSERT INTO Agenda (Event,state,date,time) "  \
             "VALUES ('eat breakfast', 'undone', '2018-08-24', '20:15:00'); "

     /* Execute SQL statement */
    DB_ErrHandling( sqlite3_exec( db, sql, NULL, NULL, NULL ) );

}

void DB_updateEventState(int evtId, char state[7] )
{
    sqlite3_stmt *compiled_statement;
    char *sql;

    // Prepare the parametrized SQL statement to update player #99.
    sql = "update Agenda set state=? WHERE id=?;";
    DB_ErrHandling( sqlite3_prepare_v2( db, sql, -1, &compiled_statement, NULL ) );

    // Bind the values to the parameters (see https://sqlite.org/c3ref/bind_blob.html).
    DB_ErrHandling( sqlite3_bind_text( compiled_statement, 1, state, -1, NULL ) );
    DB_ErrHandling( sqlite3_bind_int( compiled_statement, 2, evtId ) );

    // Evaluate the prepared SQL statement.
    DB_ErrHandling( sqlite3_step( compiled_statement ) );

    // Destroy the prepared statement object.
    DB_ErrHandling( sqlite3_finalize( compiled_statement ) );

}

#if AGENDA_DEMO_MODE
void DB_updateAllEventsState(char state[7] )
{
    sqlite3_stmt *compiled_statement;
    char *sql;

    // Prepare the parametrized SQL statement to update player #99.
    sql = "update Agenda set state=?";
    DB_ErrHandling( sqlite3_prepare_v2( db, sql, -1, &compiled_statement, NULL ) );

    // Bind the values to the parameters (see https://sqlite.org/c3ref/bind_blob.html).
    DB_ErrHandling( sqlite3_bind_text( compiled_statement, 1, state, -1, NULL ) );

    // Evaluate the prepared SQL statement.
    DB_ErrHandling( sqlite3_step( compiled_statement ) );

    // Destroy the prepared statement object.
    DB_ErrHandling( sqlite3_finalize( compiled_statement ) );

}
#endif

bool DB_SearchForEventByTime(char * pTime, AgendaEvent_t * evt)
{
    sqlite3_stmt *compiled_statement;
    char *sql;
    int evtID = 0;
    const unsigned char *evtName, *evtState, *evtDate, *evtTime = 0;

#if AGENDA_DEMO_MODE
    sql = "SELECT * from Agenda WHERE evtTime=?;";
#else
    sql = "SELECT * from Agenda WHERE evtTime=? AND date(evtDate) = date('now', 'localtime');";
#endif
    DB_ErrHandling( sqlite3_prepare_v2( db, sql, -1, &compiled_statement, NULL ) );
    // Bind the values to the parameters (see https://sqlite.org/c3ref/bind_blob.html).
    DB_ErrHandling( sqlite3_bind_text( compiled_statement, 1, pTime, -1, NULL ) );
    // Evaluate the prepared SQL statement.
    DB_ErrHandling( sqlite3_step( compiled_statement ) );

    evtID = sqlite3_column_int(compiled_statement, 0);
    evtName = sqlite3_column_text(compiled_statement, 1);
    evtState = sqlite3_column_text(compiled_statement, 2);
    evtDate = sqlite3_column_text(compiled_statement, 3);
    evtTime = sqlite3_column_text(compiled_statement, 4);

    if(!evtID || !evt || !evtState|| !evtDate|| !evtTime)
    {
        return false;
    }

    /*  Copy retrieved data*/
    evt->id = evtID;
    strcpy( evt->event, evtName );
    strcpy( evt->state, evtState );
    strcpy( evt->date, evtDate );
    strcpy( evt->time, evtTime );

    // Destroy the prepared statement object.
    DB_ErrHandling( sqlite3_finalize( compiled_statement ) );

    return true;

}

/* This CB will be called for every result row */
static int DB_EventListTodayCB(void *arg, int Columncount, char **data, char **columns)
{
    char tempStr[256]={0};
    char * eventListString = (char *) arg;

    strcat( eventListString, "\n>> " );
    sprintf(tempStr, "%-64s",data[1]);      // to have fixed width for the event field
    strcat( eventListString, tempStr );
    strcat( eventListString, "   " );
    strcat( eventListString, "State: " );
    sprintf(tempStr, "%-6s",data[2]);
    strcat( eventListString, tempStr );
    strcat( eventListString, "   " );
#if !AGENDA_DEMO_MODE
    strcat( eventListString, "Date: " );
    strcat( eventListString, data[3] );
    strcat( eventListString, "   " );
#endif
    strcat( eventListString, "Time: " );
    strcat( eventListString, data[4] );
    strcat( eventListString, "\n" );

    return 0;
}


static int DB_NextEventTodayCB(void *arg, int Columncount, char **data, char **columns)
{
    char* endptr;
    AgendaEvent_t * agendaNextEvent = (AgendaEvent_t *)arg;

    /*  Assign Agenda Next event variable to be used in Agenda Thread */
    strcpy( agendaNextEvent->event, data[1] );
    strcpy( agendaNextEvent->date, data[3] );
    strcpy( agendaNextEvent->time, data[4] );
#if AGENDA_DEMO_MODE
    agendaNextEvent->evtTimeInSec = strtoumax( data[5], &endptr, 10 );;
#endif
    agendaNextEvent->id = strtoumax( data[0], &endptr, 10 );
    agendaNextEvent->eventPresent = true;

    /* Build the next event string
     * ex. Go to the supermarket Date: 01-02-2018 Time 19:15:00
     */
    strcpy( nextEventString, "\n>> " );
    strcpy( nextEventString, data[1] );
    strcat( nextEventString, "   " );
#if !AGENDA_DEMO_MODE
    strcat( nextEventString, "Date: " );
    strcat( nextEventString, data[3] );
    strcat( nextEventString, "    " );
#endif
    strcat( nextEventString, "Time: " );
    strcat( nextEventString, data[4] );
    strcat( nextEventString, "\n" );

    return 0;
}


static int DB_ReadUserInfoCB(void *arg, int Columncount, char **data, char **columns)
{
    AgednaUserInfo_t * pUserInfo = (AgednaUserInfo_t *)arg;

    strcpy( pUserInfo->name, data[1] );
    strcpy( pUserInfo->age, data[2] );
    strcpy( pUserInfo->email, data[3] );
    pUserInfo->userInfoPresent = true;

    return 0;
}


static void DB_ErrHandling(int errorCode)
{
    if( errorCode == SQLITE_OK || errorCode == SQLITE_DONE || errorCode == SQLITE_ROW )
    {
        // Do nothing
    }
    else
    {
        printf( "Error #%d: %s\n", errorCode, sqlite3_errmsg( db ) );
        sqlite3_close( db );
        exit( errorCode );
    }
}

/* End of file ---------------------------------------------------------------*/
/* db.c */
