/*============================----beg-of-source---============================*/

#include   "ySCHED.h"
#include   "ySCHED_priv.h"

char
ysched_test_update     (tSCHED *a_sched, long a_epoch)
{
   /*---(locals)-----------+-----+-----+-*/
   char        z           =  '-';
   char        t           [LEN_HUND]  = "";
   /*---(defense)------------------------*/
   if (a_sched->epoch > a_epoch)                 z = 'y';
   if (a_sched->epoch + DAY2SEC <= a_epoch)      z = 'y';
   if (z == '-')  return 0;
   /*---(update validity)----------------*/
   sprintf (t, ".valid %s %s", a_sched->beg, a_sched->end);
   ySCHED_valid (t);
   /*---(update parsing)-----------------*/
   /*> ySCHED_parse (a_sched, a_sched->raw);                                          <*/
   /*---(complete)-----------------------*/
   return 0;
}

char       /*--> test for a specific timing ----------------------------------*/
ySCHED_testepoch        (tSCHED *a_sched, long a_epoch)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   tSCHED     *x_sched     = NULL;
   tTIME       x_broke;
   long        x_now       =    0;
   char        x_hour      =    0;
   char        x_min       =    0;
   /*---(header)-------------------------*/
   DEBUG_YSCHED  yLOG_enter   (__FUNCTION__);
   /*---(check location)-----------------*/
   if (a_sched == NULL)  x_sched = &g_curr;
   else                  x_sched = a_sched;
   /*---(update)-------------------------*/
   ysched_test_update (a_sched, a_epoch);
   /*---(get current date)---------------*/
   if (a_epoch >= 0)  x_now = a_epoch;
   else               x_now = time (NULL);
   localtime_r (&x_now, &x_broke);
   /*---(clear small elements)-----------*/
   x_min  = x_broke.tm_min;
   x_hour = x_broke.tm_hour;
   /*---(effective)----------------------*/
   if (x_sched->valid != 'y') {
      return 0;
   }
   /*---(based on setdate)---------------*/
   if (x_sched->wks [mySCHED.s_woy]  == '_') {
      return 0;
   }
   if (x_sched->mos [mySCHED.s_month]  == '_') {
      return 0;
   }
   if (x_sched->dow [mySCHED.s_dow]  == '_' || x_sched->dys [mySCHED.s_day]  == '_') {
      return 0;
   }
   /*---(hours)--------------------------*/
   if (x_hour != YSCHED_ANY && x_sched->hrs [x_hour]  == '_') {
      return 0;
   }
   if (x_hour == YSCHED_ANY && strchr (x_sched->hrs, '1') == NULL) {
      return 0;
   }
   /*---(minutes)------------------------*/
   if (x_min != YSCHED_ANY && x_sched->min [x_min]  == '_') {
      return 0;
   }
   if (x_min == YSCHED_ANY && strchr (x_sched->min, '1') == NULL) {
      return 0;
   }
   /*---(complete)-----------------------*/
   /*> return x_sched->dur;                                                           <*/
   return 1;
}

char       /*--> test for a specific timing ----------------------------------*/
ySCHED_test_by_time     (void *a_sched, int a_hour, int a_minute)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    1;
   tSCHED     *x_sched     = NULL;
   /*---(header)-------------------------*/
   DEBUG_YSCHED  yLOG_enter   (__FUNCTION__);
   /*---(check location)-----------------*/
   --rce;  if (a_sched == NULL) {
      DEBUG_YSCHED  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   x_sched = (tSCHED *) a_sched;
   --rce;  if (a_hour < 0 || a_hour > 23) {
      DEBUG_YSCHED  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (a_minute < 0 || a_minute > 59) {
      DEBUG_YSCHED  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check update)-------------------*/
   if      (x_sched->year   != mySCHED.s_year)    ysched_update (a_sched);
   else if (x_sched->month  != mySCHED.s_month)   ysched_update (a_sched);
   else if (x_sched->day    != mySCHED.s_day)     ysched_update (a_sched);
   /*---(check for already tested)-------*/
   if (x_sched->hour == a_hour) {
      if (x_sched->minute == a_minute) {
         DEBUG_YSCHED  yLOG_note    ("same as last test");
         DEBUG_YSCHED  yLOG_exit    (__FUNCTION__);
         return 2;
      }
   }
   /*---(mark test)----------------------*/
   x_sched->hour   = a_hour;
   x_sched->minute = a_minute;
   x_sched->result = 0;
   /*---(effective)----------------------*/
   if (x_sched->valid  != 'y') {
      rc = 0;
   }
   /*---(based on setdate)---------------*/
   else if (x_sched->wks [mySCHED.s_woy]  == '_') {
      rc = 0;
   }
   else if (x_sched->mos [mySCHED.s_month]  == '_') {
      rc = 0;
   }
   else if (x_sched->dow [mySCHED.s_dow]  == '_' || x_sched->dys [mySCHED.s_day]  == '_') {
      rc = 0;
   }
   /*---(hours)--------------------------*/
   else if (a_hour   != YSCHED_ANY && x_sched->hrs [a_hour    ]  == '_') {
      rc = 0;
   }
   else if (a_hour   == YSCHED_ANY && strchr (x_sched->hrs, '1') == NULL) {
      rc = 0;
   }
   /*---(minutes)------------------------*/
   else if (a_minute != YSCHED_ANY && x_sched->min [a_minute  ]  == '_') {
      rc = 0;
   }
   else if (a_minute == YSCHED_ANY && strchr (x_sched->min, '1') == NULL) {
      rc = 0;
   }
   /*---(mark success)-------------------*/
   x_sched->result = rc;
   /*---(complete)-----------------------*/
   DEBUG_YSCHED  yLOG_exit    (__FUNCTION__);
   return rc;
}

char
ySCHED_test_by_epoch    (void *a_sched, long a_epoch)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        t           [200];
   long        x_now       = 0;
   tTIME       x_broke;
   /*---(header)-------------------------*/
   DEBUG_YSCHED  yLOG_enter   (__FUNCTION__);
   /*---(get current date)---------------*/
   if (a_epoch >= 0)  x_now = a_epoch;
   else               x_now = time (NULL);
   localtime_r (&x_now, &x_broke);
   /*---(check update)-------------------*/
   --rce;  if (x_broke.tm_year - 100 != mySCHED.s_year) {
      DEBUG_YSCHED  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (x_broke.tm_mon != mySCHED.s_month) {
      DEBUG_YSCHED  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (x_broke.tm_mday != mySCHED.s_day) {
      DEBUG_YSCHED  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(run details)--------------------*/
   rc = ySCHED_test_by_time (a_epoch, x_broke.tm_hour, x_broke.tm_min);
   DEBUG_YSCHED  yLOG_value   ("driver"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_YSCHED  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_YSCHED  yLOG_exit    (__FUNCTION__);
   return 0;
}


/*====================------------------------------------====================*/
/*===----                         unit testing                         ----===*/
/*====================------------------------------------====================*/
static void      o___UNITTEST________________o (void) {;};

char*
ysched_test__unit       (char *a_question)
{
   /*---(defense)------------------------*/
   snprintf (unit_answer, LEN_RECD, "TEST unit        : question unknown");
   /*---(simple)-------------------------*/
   if        (strcmp (a_question, "last"         ) == 0) {
      sprintf(unit_answer, "TEST last        : %2d  %2d  %2d", g_curr->hour, g_curr->minute, g_curr->result);
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}


char       /*----: set up program urgents/debugging --------------------------*/
ysched__unit_quiet     (void)
{
   yLOGS_begin ("ySCHED" , YLOG_SYS, YLOG_QUIET);
   ysched_date_reset  ();
   ysched_error_reset ();
   return 0;
}

char       /*----: set up program urgents/debugging --------------------------*/
ysched__unit_loud     (void)
{
   yLOGS_begin ("ySCHED" , YLOG_SYS, YLOG_NOISE);
   yURG_name  ("kitchen"      , YURG_ON);
   yURG_name  ("ysched"       , YURG_ON);
   DEBUG_YSCHED  yLOG_info     ("ySCHED"    , ySCHED_version   ());
   ysched_date_reset  ();
   ysched_error_reset ();
   return 0;
}

char       /*----: stop logging ----------------------------------------------*/
ysched__unit_end   (void)
{
   ysched_purge  ();
   yLOGS_end     ();
   return 0;
}


/*============================----end-of-source---============================*/
