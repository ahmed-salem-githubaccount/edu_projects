/**
  ******************************************************************************
  * @file     utils.c
  * @author   Ahmed
  * @version
  * @date     Aug 25, 2018
  * @brief    
  ******************************************************************************
  */

/* Includes ("...") ----------------------------------------------------------*/
#include "utils.h"
#include "agenda_config.h"
#include "fsm.h"
/* Includes (<...>) ----------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <inttypes.h>
#include <unistd.h>
#include <ctype.h>
/* Private defines -----------------------------------------------------------*/
#if AGENDA_DEMO_MODE
#define NUMBER_OF_SECONDS_IN_24HOURS        (86400)
#endif
/* Private macros ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Global constants definitions ----------------------------------------------*/
/* Global variable definitions -----------------------------------------------*/
#if AGENDA_DEMO_MODE
int currentTimeInSec = 0;
#endif
/* Module functions implementation -------------------------------------------*/

void getCurrentTime(struct tm ** timeinfo)
{
    time_t rawtime;

    time( &rawtime );
    * timeinfo = localtime( &rawtime );
}

bool getCurrentTimeinSec(int * pcurrentTimeInSec)
{
    bool state = true;

#if AGENDA_DEMO_MODE
    if( currentTimeInSec < NUMBER_OF_SECONDS_IN_24HOURS)
    {
        /* Applying speed factor for Demo purpose */
        currentTimeInSec += AGENDA_TIME_FACTOR;
    }
    else
    {
        currentTimeInSec = 0;
        state = false;
    }
#else
    int currentTimeInSec = 0;
    time_t rawtime;
    struct tm * timeinfo;

    time( &rawtime );
    timeinfo = localtime( &rawtime );
    currentTimeInSec = ((timeinfo->tm_hour * 60 * 60) + (timeinfo->tm_min * 60) + timeinfo->tm_sec);
#endif

    * pcurrentTimeInSec = currentTimeInSec;
    return state;
}



void getCurrentDateFormatted(char * pDate)
{
    char tempStr[16]= {0};
    struct tm * timeinfo;

    getCurrentTime(&timeinfo);
    sprintf(tempStr, "%04d",(timeinfo->tm_year + 1900));
    strcpy(pDate, tempStr);
    strcat(pDate, "-");
    sprintf(tempStr, "%02d",(timeinfo->tm_mon + 1));
    strcat(pDate, tempStr);
    strcat(pDate, "-");
    sprintf(tempStr, "%02d",(timeinfo->tm_mday));
    strcat(pDate, tempStr);
}

void getCurrentTimeFormatted(char * pTime)
{
    char tempStr[16]= {0};
    struct tm * timeinfo;

    getCurrentTime(&timeinfo);
    sprintf(tempStr, "%02d",(timeinfo->tm_hour));
    strcpy(pTime, tempStr);
    strcat(pTime, "-");
    sprintf(tempStr, "%02d",(timeinfo->tm_min + 1));
    strcat(pTime, tempStr);
    strcat(pTime, "-");
    sprintf(tempStr, "%02d",(timeinfo->tm_sec));
    strcat(pTime, tempStr);
}

bool checkAndFormatTime(char * ptime)
{
    char** str_list;
    char* endptr;
    int nextEventHH;
    int nextEventMM;
    char tempStr[16]= {0};

    if(!strlen(ptime))
    {
        return false;
    }

    str_list = str_split( ptime, ':' );  // split the time string
    if( !str_list[0] || !str_list[1] )
    {
        return false;
    }
    nextEventHH = strtoumax( str_list[0], &endptr, 10 );
    nextEventMM = strtoumax( str_list[1], &endptr, 10 );

    if( nextEventHH > 23 || nextEventMM > 59 )
    {
        return false;
    }

    sprintf( tempStr, "%02d", nextEventHH );
    strcpy( ptime, tempStr );
    strcat( ptime, ":" );
    sprintf( tempStr, "%02d", nextEventMM );
    strcat( ptime, tempStr );
    strcat( ptime, ":00" );    // added this part for as a DB requirement

    return true;
}

#if AGENDA_DEMO_MODE
int ConvertFormattedTimetoSec(char * ptime)
{
    char** str_list;
    char* endptr;
    int nextEventHH;
    int nextEventMM;
    char tempStr[16]= {0};

    if(!strlen(ptime))
    {
        return false;
    }

    strcpy(tempStr, ptime);
    str_list = str_split( tempStr, ':' );  // split the time string
    if( !str_list[0] || !str_list[1] )
    {
        return false;
    }
    nextEventHH = strtoumax( str_list[0], &endptr, 10 );
    nextEventMM = strtoumax( str_list[1], &endptr, 10 );

    if( nextEventHH > 23 || nextEventMM > 59 )
    {
        return false;
    }

    return ((nextEventHH * 60 * 60) + (nextEventMM * 60));
}
#endif


char** str_split(char* a_str, const char a_delim)
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}

void removeTrailingNewline( char * buffer)
{
    buffer[strcspn(buffer, "\n")] = '\0';
}

void stringToLower(char * pStr)
{
    for ( ; *pStr; ++pStr) *pStr = tolower(*pStr);
}

/* End of file ---------------------------------------------------------------*/
/* utils.c */
