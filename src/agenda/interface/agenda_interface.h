/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SRC_AGENDA_INTERFACE_AGENDA_INTERFACE_H__
#define __SRC_AGENDA_INTERFACE_AGENDA_INTERFACE_H__
/**
  ******************************************************************************
  * @file      agenda_interface.h
  * @author    Ahmed
  * @version   
  * @date      Aug 22, 2018
  * @brief     
  ******************************************************************************
  */

/* Includes ("...") ----------------------------------------------------------*/
#include "agenda_config.h"
/* Includes (<...>) ----------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
/* Global defines-------------------------------------------------------------*/
/* Global macros -------------------------------------------------------------*/
/* Global constants declarations ---------------------------------------------*/
/* Global typedef ------------------------------------------------------------*/

typedef struct
{
#define AGENDA_USERINFO_NAME_STRING_SIZE        (32)
#define AGENDA_USERINFO_AGE_STRING_SIZE         (4)
#define AGENDA_USERINFO_EMAIL_STRING_SIZE       (32)
    char name[32];
    char age[4];
    char email[32];
    bool userInfoPresent;
}AgednaUserInfo_t;

typedef struct _AgendaEvent
{
#define AGENDA_EVENT_TIME_STRING_SIZE       (16)
#define AGENDA_EVENT_DATE_STRING_SIZE       (16)
#define AGENDA_EVENT_EVENT_STRING_SIZE      (64)
#define AGENDA_EVENT_STATE_STRING_SIZE      (8)

    int  id;
    char date[16];
    char time[16];
    char event[128];
    char state[8];
    int  eventDueInMintues; // event Due compared to current system time
    bool eventPresent;
    bool isEventDismissed;
    bool firstReminderFired;
#if AGENDA_DEMO_MODE
    int evtTimeInSec;
#endif
}AgendaEvent_t;


typedef struct DB
{
    AgendaEvent_t agendaNextEvent;
    AgendaEvent_t agendaNewEvent;
    AgendaEvent_t agendaSearchEvent;
    AgednaUserInfo_t userInfo;
}AgendaDB_t;

/* Global variable declarations ----------------------------------------------*/
extern int eventDueID;
/* Global function prototypes ------------------------------------------------*/
/**
 * @brief  Agenda Task. This function will check reminders and update next event value
 *         Must be called cyclicly with period of 1ms
 * @param  vargp  passed argument
 * @return void
 */
void * agendaTask(void * vargp);

/* End of file ---------------------------------------------------------------*/
#endif /* __SRC_AGENDA_INTERFACE_AGENDA_INTERFACE_H__ */
