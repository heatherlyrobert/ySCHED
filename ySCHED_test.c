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
   /*> sprintf (t, ".valid %s %s", a_sched->beg, a_sched->end);                       <*/
   /*> ySCHED_valid (t);                                                              <*/
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
   /*> if (x_sched->valid != 'y') {                                                   <* 
    *>    return 0;                                                                   <* 
    *> }                                                                              <*/
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

char
ysched_test__normal     (tSCHED *a_sched, char *r_note, char a_hour, char a_minute)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    1;
   /*---(based on setdate)---------------*/
   if      (a_sched->wks [mySCHED.s_woy]  == '_') {
      if (r_note != NULL)  *r_note = 'W';
      rc = 0;
   }
   else if (a_sched->mos [mySCHED.s_month]  == '_') {
      if (r_note != NULL)  *r_note = 'M';
      rc = 0;
   }
   else if (a_sched->dow [mySCHED.s_dow]  == '_' || a_sched->dys [mySCHED.s_day]  == '_') {
      if (r_note != NULL)  *r_note = 'D';
      rc = 0;
   }
   /*---(hours)--------------------------*/
   else if (a_hour   != YSCHED_ANY && a_sched->hrs [a_hour    ]  == '_') {
      if (r_note != NULL)  *r_note = 'h';
      rc = 0;
   }
   else if (a_hour   == YSCHED_ANY && strchr (a_sched->hrs, '1') == NULL) {
      if (r_note != NULL)  *r_note = 'h';
      rc = 0;
   }
   /*---(minutes)------------------------*/
   else if (a_minute != YSCHED_ANY && a_sched->min [a_minute  ]  == '_') {
      if (r_note != NULL)  *r_note = 'm';
      rc = 0;
   }
   else if (a_minute == YSCHED_ANY && strchr (a_sched->min, '1') == NULL) {
      if (r_note != NULL)  *r_note = 'm';
      rc = 0;
   }
   /*---(mark success)-------------------*/
   a_sched->result = rc;
   /*---(complete)-----------------------*/
   return rc;
}

char
ysched_test__after      (tSCHED *a_sched, char *r_note, char a_hour, char a_minute)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    1;
   char        b           =  '_';
   char        c           =  '_';
   /*---(default)------------------------*/
   a_sched->result = 0;
   /*---(years)--------------------------*/
   c = a_sched->yrs [mySCHED.s_year ];
   if (mySCHED.s_year  == 0)  b = c; else b = a_sched->yrs [mySCHED.s_year  - 1];
   DEBUG_YSCHED  yLOG_complex ("year"      , "%2d, %c, %c, %s", mySCHED.s_year  , b, c, a_sched->yrs);
   if (b == '1')  { a_sched->result = 1; return 1; }
   if (c == '_')  { if (r_note != NULL)  *r_note = 'Y';  return 0; }
   /*---(week of year)-------------------*/
   /*> c = a_sched->wks [mySCHED.s_woy  ];                                                                   <* 
    *> if (mySCHED.s_woy   == 0)  b = c; else b = a_sched->wks [mySCHED.s_woy   - 1];                        <* 
    *> DEBUG_YSCHED  yLOG_complex ("weeks"     , "%2d, %c, %c, %s", mySCHED.s_woy   , b, c, a_sched->wks);   <* 
    *> if (b == '1')  { a_sched->result = 1; return 1; }                                                     <* 
    *> if (c == '_')  { if (r_note != NULL)  *r_note = 'W';  return 0; }                                     <*/
   /*---(months)-------------------------*/
   c = a_sched->mos [mySCHED.s_month];
   if (mySCHED.s_month == 0)  b = c; else b = a_sched->mos [mySCHED.s_month - 1];
   DEBUG_YSCHED  yLOG_complex ("months"    , "%2d, %c, %c, %s", mySCHED.s_month , b, c, a_sched->mos);
   if (b == '1')  { a_sched->result = 1; return 1; }
   if (c == '_')  { if (r_note != NULL)  *r_note = 'M';  return 0; }
   /*---(day of week)--------------------*/
   /*> c = a_sched->doy [mySCHED.s_dow  ];                                              <* 
    *> if (mySCHED.s_dow   == 0)  b = c; else b = a_sched->doy [mySCHED.s_dow  ] - 1;   <* 
    *> if (b == '1')  return 1;                                                         <* 
    *> if (c == '_')  return 0;                                                         <*/
   /*---(days)---------------------------*/
   c = a_sched->dys [mySCHED.s_day  ];
   if (mySCHED.s_day   == 0)  b = c; else b = a_sched->dys [mySCHED.s_day   - 1];
   DEBUG_YSCHED  yLOG_complex ("days"      , "%2d, %c, %c, %s", mySCHED.s_day   , b, c, a_sched->dys);
   if (b == '1')  { a_sched->result = 1; return 1; }
   if (c == '_')  { if (r_note != NULL)  *r_note = 'D';  return 0; }
   /*---(hours)--------------------------*/
   c = a_sched->hrs [a_hour ];
   if (a_hour   == 0)  b = c; else b = a_sched->hrs [a_hour  - 1];
   DEBUG_YSCHED  yLOG_complex ("hours"     , "%2d, %c, %c, %s", a_hour          , b, c, a_sched->hrs);
   if (a_hour   == YSCHED_ANY && strchr (a_sched->hrs, '1') == NULL) {
      return 0;
   } else {
      if (b == '1')  { a_sched->result = 1; return 1; }
      if (c == '_')  { if (r_note != NULL)  *r_note = 'h';  return 0; }
   }
   /*---(minutes)------------------------*/
   c = a_sched->min [a_minute];
   if (a_minute == 0)  b = c; else b = a_sched->min [a_minute  - 1];
   DEBUG_YSCHED  yLOG_complex ("minutes"   , "%2d, %c, %c, %s", a_minute        , b, c, a_sched->min);
   if (a_minute == YSCHED_ANY && strchr (a_sched->min, '1') == NULL) {
      return 0;
   } else {
      if (b == '1')  { a_sched->result = 1; return 1; }
      if (c == '_')  { if (r_note != NULL)  *r_note = 'm';  return 0; }
   }
   /*---(mark success)-------------------*/
   a_sched->result = rc;
   /*---(complete)-----------------------*/
   return rc;
}

char       /*--> test for a specific timing ----------------------------------*/
ySCHED_test             (void *a_sched, char a_hour, char a_minute, char *r_note)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tSCHED     *x_sched     = NULL;
   /*---(header)-------------------------*/
   DEBUG_YSCHED  yLOG_enter   (__FUNCTION__);
   /*---(default)------------------------*/
   if (r_note != NULL)  *r_note = '-';
   /*---(defense)------------------------*/
   DEBUG_YSCHED  yLOG_point   ("a_sched"   , a_sched);
   --rce;  if (a_sched == NULL) {
      DEBUG_YSCHED  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   x_sched = (tSCHED *) a_sched;
   /*---(range checks)-------------------*/
   DEBUG_YSCHED  yLOG_value   ("a_hour"    , a_hour);
   --rce;  if (a_hour < 0 || a_hour > 23) {
      DEBUG_YSCHED  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YSCHED  yLOG_value   ("a_minute"  , a_minute);
   --rce;  if (a_minute < 0 || a_minute > 59) {
      if (a_minute != YSCHED_ANY) {
         DEBUG_YSCHED  yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      DEBUG_YSCHED  yLOG_note    ("a_minute shown as YSCHED_ANY");
   }
   /*---(check update)-------------------*/
   DEBUG_YSCHED  yLOG_info    ("raw"       , x_sched->raw);
   DEBUG_YSCHED  yLOG_complex ("year"      , "%3d =? %3d", x_sched->year  , mySCHED.s_year);
   DEBUG_YSCHED  yLOG_complex ("month"     , "%3d =? %3d", x_sched->month , mySCHED.s_month);
   DEBUG_YSCHED  yLOG_complex ("day"       , "%3d =? %3d", x_sched->day   , mySCHED.s_day);
   if      (x_sched->year   != mySCHED.s_year)    rc = ysched_update (a_sched);
   else if (x_sched->month  != mySCHED.s_month)   rc = ysched_update (a_sched);
   else if (x_sched->day    != mySCHED.s_day)     rc = ysched_update (a_sched);
   DEBUG_YSCHED  yLOG_value   ("update"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_YSCHED  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check for already tested)-------*/
   DEBUG_YSCHED  yLOG_complex ("hour"      , "%3d =? %3d", x_sched->hour  , a_hour);
   DEBUG_YSCHED  yLOG_complex ("minute"    , "%3d =? %3d", x_sched->minute, a_minute);
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
   rc = 1;
   /*---(check details)------------------*/
   switch (x_sched->eval) {
   case '·'  : rc = ysched_test__normal (x_sched, r_note, a_hour, a_minute); break;
   case '¼'  : rc = ysched_test__after  (x_sched, r_note, a_hour, a_minute); break;
   }
   /*---(based on setdate)---------------*/
   /*> if      (x_sched->wks [mySCHED.s_woy]  == '_') {                                           <* 
    *>    if (r_note != NULL)  *r_note = 'W';                                                     <* 
    *>    rc = 0;                                                                                 <* 
    *> }                                                                                          <* 
    *> else if (x_sched->mos [mySCHED.s_month]  == '_') {                                         <* 
    *>    if (r_note != NULL)  *r_note = 'M';                                                     <* 
    *>    rc = 0;                                                                                 <* 
    *> }                                                                                          <* 
    *> else if (x_sched->dow [mySCHED.s_dow]  == '_' || x_sched->dys [mySCHED.s_day]  == '_') {   <* 
    *>    if (r_note != NULL)  *r_note = 'D';                                                     <* 
    *>    rc = 0;                                                                                 <* 
    *> }                                                                                          <*/
   /*---(hours)--------------------------*/
   /*> else if (a_hour   != YSCHED_ANY && x_sched->hrs [a_hour    ]  == '_') {        <* 
    *>    if (r_note != NULL)  *r_note = 'h';                                         <* 
    *>    rc = 0;                                                                     <* 
    *> }                                                                              <* 
    *> else if (a_hour   == YSCHED_ANY && strchr (x_sched->hrs, '1') == NULL) {       <* 
    *>    if (r_note != NULL)  *r_note = 'h';                                         <* 
    *>    rc = 0;                                                                     <* 
    *> }                                                                              <*/
   /*---(minutes)------------------------*/
   /*> else if (a_minute != YSCHED_ANY && x_sched->min [a_minute  ]  == '_') {        <* 
    *>    if (r_note != NULL)  *r_note = 'm';                                         <* 
    *>    rc = 0;                                                                     <* 
    *> }                                                                              <* 
    *> else if (a_minute == YSCHED_ANY && strchr (x_sched->min, '1') == NULL) {       <* 
    *>    if (r_note != NULL)  *r_note = 'm';                                         <* 
    *>    rc = 0;                                                                     <* 
    *> }                                                                              <*/
   /*---(mark success)-------------------*/
   /*> x_sched->result = rc;                                                          <*/
   /*---(complete)-----------------------*/
   DEBUG_YSCHED  yLOG_exit    (__FUNCTION__);
   return rc;
}

char
ySCHED_teste            (void *a_sched, long a_epoch)
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
   rc = ySCHED_test (a_epoch, x_broke.tm_hour, x_broke.tm_min, NULL);
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
