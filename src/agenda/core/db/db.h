/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SRC_DB_DB_H__
#define __SRC_DB_DB_H__
/**
  ******************************************************************************
  * @file      db.h
  * @author    embedded
  * @version   
  * @date      Aug 22, 2018
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
#include "agenda.h"
#include "agenda_config.h"
#include "agenda_interface.h"
/* Includes (<...>) ----------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
/* Global defines-------------------------------------------------------------*/
/* Global macros -------------------------------------------------------------*/
/* Global constants declarations ---------------------------------------------*/
/* Global typedef ------------------------------------------------------------*/
/* Global variable declarations ----------------------------------------------*/
/* Global function prototypes ------------------------------------------------*/
/**
 * @brief Create Agenda Table in DB
 * @param  void
 * @return void
 */
void DB_CreateAgnedaTable(void);
/**
 * @brief Create User Info Table in DB
 * @param  void
 * @return void
 */
void DB_CreateUserInfoTable(void);
/**
 * @brief Get The next undone event
 * @param  void
 * @return agendaNextEvent pointer to next event  buffer
 */
void DB_QueryNextEventToday(AgendaEvent_t * agendaNextEvent);
/**
 * @brief  List all events from agenda table
 * @param  void
 * @return void
 */
void DB_EventListToday(void);
/**
 * @brief  Adds new event to agenda table in DB
 * @param  agendaNewEvent Pointer to new event buffer
 * @return void
 */
void DB_AddEvent(AgendaEvent_t * agendaNewEvent);
/**
 * @brief  Open DB
 * @param  dbName name of the DB to be opened
 * @return void
 */
void DB_Open(char * dbName);
/**
 * @brief  close DB
 * @param  void
 * @return void
 */
void DB_Close(void);
/**
 * @brief  read and return user info from agenda table in DB
 * @param  pUserInfo pointer to userinfo buffer
 * @return void
 */
void DB_ReadUserInfo(const AgednaUserInfo_t * pUserInfo );
/**
 * @brief  update user info in DB
 * @param  pUserInfo pointer to userinfo buffer
 * @return void
 */
void DB_UpdateUserInfo(const AgednaUserInfo_t * pUserInfo );
/**
 * @brief  Update any event state in DB
 * @param  evtId event ID to be updated
 * @param  state 'done' or 'undone'
 * @return void
 */
void DB_updateEventState(int evtId, char state[7] );
/**
 * @brief  Search for an event in DB by time
 * @param  pTime pointer to time buffer
 * @param  evt   OUT pointer to the retrieved event
 * @return void
 */
bool DB_SearchForEventByTime(char * pTime, AgendaEvent_t * evt);
#if AGENDA_DEMO_MODE
/**
 * @brief  Update all events state to done or undone
 * @param  state 'done' or 'undone'
 * @return void
 */
void DB_updateAllEventsState(char state[7] );
#endif
/* End of file ---------------------------------------------------------------*/
#endif /* __SRC_DB_DB_H__ */
