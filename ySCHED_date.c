/*============================----beg-of-source---============================*/
#include   "ySCHED.h"
#include   "ySCHED_priv.h"




/*====================------------------------------------====================*/
/*===----                              dates                           ----===*/
/*====================------------------------------------====================*/
static void      o___DATES___________________o (void) {;}

char       /*--> clear all data fields ---------------------------------------*/
ysched_resetdate        (void)
{
   /*---(header)-------------------------*/
   DEBUG_YSCHED  yLOG_senter  (__FUNCTION__);
   mySCHED.s_epoch = mySCHED.s_off   = 0;
   mySCHED.s_year  = mySCHED.s_month = mySCHED.s_day   = 0;
   mySCHED.s_doy   = mySCHED.s_woy   = mySCHED.s_dow   = 0;
   mySCHED.s_wiy   = mySCHED.s_dim   = mySCHED.s_fdow  = 0;
   DEBUG_YSCHED  yLOG_sexit   (__FUNCTION__);
   return 0;
}

char       /*--> set the data for parsing ------------------------------------*/
ysched_date__defense    (cint a_year, cint a_month, cint a_day)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   /*---(header)-------------------------*/
   DEBUG_YSCHED  yLOG_enter   (__FUNCTION__);
   /*---(reset date fields)--------------*/
   ysched_resetdate ();
   /*---(check max ranges)---------------*/
   DEBUG_YSCHED  yLOG_complex ("a_year"   , "%d (0 to 999)", a_year);
   --rce;  if (a_year < 0 || a_year > 999) {
      DEBUG_YSCHED  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YSCHED  yLOG_complex ("a_month"  , "%d (0 to 12)", a_month);
   --rce;  if (a_month < 1 || a_month > 12) {
      DEBUG_YSCHED  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YSCHED  yLOG_complex ("a_day"    , "%d (0 to 31)", a_day);
   --rce;  if (a_day < 1 || a_day > 31) {
      DEBUG_YSCHED  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(store supplied values)----------*/
   DEBUG_YSCHED  yLOG_note    ("save values");
   mySCHED.s_year            = a_year;
   mySCHED.s_month            = a_month;
   mySCHED.s_day            = a_day;
   /*---(complete)-----------------------*/
   DEBUG_YSCHED  yLOG_exit    (__FUNCTION__);
   return 0;
}

char       /*--> set the data for parsing ------------------------------------*/
ysched_date__current    (long a_now)
{
   /*---(design)-------------------------*/
   /*  included parameter a_now for strict unit testing
    */
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        t           [200];
   long        x_now       = 0;
   /*---(header)-------------------------*/
   DEBUG_YSCHED  yLOG_enter   (__FUNCTION__);
   /*---(get current date)---------------*/
   if (a_now >= 0)  x_now = a_now;
   else             x_now = time (NULL);
   mySCHED.s_broke   = localtime (&x_now);
   /*---(clear small elements)-----------*/
   mySCHED.s_broke->tm_sec  = 0;
   mySCHED.s_broke->tm_min  = 0;
   mySCHED.s_broke->tm_hour = 0;
   strftime (t, 100, "%a, %b %d, %Y", mySCHED.s_broke);
   DEBUG_YSCHED  yLOG_info    ("current"   , t);
   /*---(adjust to requsted)-------------*/
   mySCHED.s_broke->tm_year = mySCHED.s_year + 100;  /* from 1900 */
   DEBUG_YSCHED  yLOG_value   ("tm_year"   , mySCHED.s_broke->tm_year);
   mySCHED.s_broke->tm_mon  = mySCHED.s_month - 1;
   DEBUG_YSCHED  yLOG_value   ("tm_mon"    , mySCHED.s_broke->tm_mon);
   mySCHED.s_broke->tm_mday = mySCHED.s_day;
   DEBUG_YSCHED  yLOG_value   ("tm_mday"   , mySCHED.s_broke->tm_mday);
   /*---(get the format version)---------*/
   mySCHED.s_epoch          = mktime   (mySCHED.s_broke);
   mySCHED.s_broke          = localtime (&mySCHED.s_epoch);
   strftime (t, 100, "%a, %b %d, %Y", mySCHED.s_broke);
   DEBUG_YSCHED  yLOG_info    ("specified" , t);
   /*---(defenses : day)-----------------*/
   --rce;  if (mySCHED.s_broke->tm_mon != mySCHED.s_month - 1) {
      DEBUG_YSCHED  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(calculate the offsets)----------*/
   mySCHED.s_off            = (mySCHED.s_epoch - x_now) / (60 * 60 * 24);
   if (mySCHED.s_off < -99)  mySCHED.s_off = -999;
   if (mySCHED.s_off > 365)  mySCHED.s_off =  999;
   DEBUG_YSCHED  yLOG_value   ("s_off"      , mySCHED.s_off);
   /*---(work out the current time)----------*/
   mySCHED.s_dow            = mySCHED.s_broke->tm_wday;
   if (mySCHED.s_dow == 0) mySCHED.s_dow = 7;        /* put sunday at the end                   */
   DEBUG_YSCHED  yLOG_value   ("s_dow"      , mySCHED.s_dow);
   /*---(complete)-----------------------*/
   DEBUG_YSCHED  yLOG_exit    (__FUNCTION__);
   return 0;
}

char       /*--> set the data for parsing ------------------------------------*/
ysched_date__dow_1st    (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        t           [200];
   tTIME      *x_broke     = NULL;
   long        x_epoch     =    0;
   /*---(header)-------------------------*/
   DEBUG_YSCHED  yLOG_enter   (__FUNCTION__);
   /*---(work out the dow for 1st)-----------*/
   x_broke          = localtime (&(mySCHED.s_epoch));
   x_broke->tm_mday = 1;                        /* set  day to the 1st  */
   x_epoch          = mktime    (x_broke);      /* make epoch format    */
   x_broke          = localtime(&x_epoch);       /* break it down        */
   mySCHED.s_fdow   = x_broke->tm_wday;         /* get day of week      */
   if (mySCHED.s_fdow == 0) mySCHED.s_fdow = 7;                   /* put sunday at end    */
   strftime(t, 100, "%a, %b %d, %Y", x_broke);
   DEBUG_YSCHED  yLOG_info    ("begin"     , t);
   DEBUG_YSCHED  yLOG_value   ("s_fdow"      , mySCHED.s_fdow);
   /*---(complete)-----------------------*/
   DEBUG_YSCHED  yLOG_exit    (__FUNCTION__);
   return 0;
}

char       /*--> set the data for parsing ------------------------------------*/
ysched_date__max_days   (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        t           [200] = "";
   tTIME      *x_broke     = NULL;
   long        x_epoch     =    0;
   /*---(header)-------------------------*/
   DEBUG_YSCHED  yLOG_enter   (__FUNCTION__);
   /*---(max day of month)---------------*/
   x_broke          = localtime (&(mySCHED.s_epoch));
   ++x_broke->tm_mon;                   /* go to the next month            */
   if (x_broke->tm_mon > 11) {          /* if moved to jan, increase year  */
      x_broke->tm_mon = 0;
      ++x_broke->tm_year;
   }
   x_broke->tm_mday = 1;                           /* set  day to the 1st  */
   x_epoch          = mktime    (x_broke);      /* make epoch format    */
   x_epoch          = x_epoch - (60 * 60 * 24);  /* subtract a full day  */
   x_broke          = localtime(&x_epoch);       /* break it down        */
   mySCHED.s_dim    = x_broke->tm_mday;         /* the day is now max   */
   DEBUG_YSCHED  yLOG_value   ("s_dim"      , mySCHED.s_dim);
   /*---(max day of year)----------------*/
   x_broke          = localtime (&(mySCHED.s_epoch));
   ++x_broke->tm_year;                  /* go to the next year             */
   x_broke->tm_mon  = 0;                           /* set month to jan     */
   x_broke->tm_mday = 1;                           /* set  day to the 1st  */
   x_epoch          = mktime    (x_broke);      /* make epoch format    */
   x_epoch          = x_epoch - (60 * 60 * 24);  /* subtract a full day  */
   x_broke          = localtime(&x_epoch);       /* break it down        */
   strftime(t, 10, "%j", x_broke);
   mySCHED.s_diy            = atoi(t);
   DEBUG_YSCHED  yLOG_value   ("s_diy"      , mySCHED.s_diy);
   /*---(day in year)--------------------*/
   x_broke          = localtime (&(mySCHED.s_epoch));
   strftime(t, 10, "%j", x_broke);
   mySCHED.s_doy            = atoi(t);
   DEBUG_YSCHED  yLOG_value   ("s_doy"      , mySCHED.s_doy);
   /*---(complete)-----------------------*/
   DEBUG_YSCHED  yLOG_exit    (__FUNCTION__);
   return 0;
}

char       /*--> set the data for parsing ------------------------------------*/
ysched_date__max_weeks  (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        t           [200];
   tTIME      *x_broke     = NULL;
   long        x_epoch     =    0;
   int         x_dow       =    0;
   /*---(header)-------------------------*/
   DEBUG_YSCHED  yLOG_enter   (__FUNCTION__);
   /*---(work out current week)--------------*/
   x_broke          = localtime (&(mySCHED.s_epoch));
   strftime(t, 10, "%W", x_broke);  /* first more than half week of the year   */
   mySCHED.s_woy            = atoi(t);
   DEBUG_YSCHED  yLOG_value   ("s_woy"      , mySCHED.s_woy);
   /*---(work out the max weeks)-------------*/
   x_broke          = localtime (&(mySCHED.s_epoch));
   ++x_broke->tm_year;
   x_broke->tm_mon  = 0;
   x_broke->tm_mday = 1;
   x_epoch          = mktime (x_broke);
   x_epoch          = x_epoch - (60 * 60 * 24);
   x_broke          = localtime (&x_epoch);
   strftime(t, 10, "%W", x_broke);  /* first more than half week of the year   */
   mySCHED.s_wiy            = atoi(t);
   DEBUG_YSCHED  yLOG_value   ("s_wiy"      , mySCHED.s_wiy);
   /*> DEBUG_YSCHED  printf("%1ddow, %s, %02dnwks¦", mySCHED.s_broke->tm_wday, t, mySCHED.s_wiy);   <*/
   /*---(complete)-----------------------*/
   DEBUG_YSCHED  yLOG_exit    (__FUNCTION__);
   return 0;
}

char       /*--> set the data for parsing ------------------------------------*/
ySCHED_setdate     (short a_year, char a_month, char a_day)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_YSCHED  yLOG_enter   (__FUNCTION__);
   /*---(defenses)-----------------------*/
   rc = ysched_date__defense (a_year, a_month, a_day);
   --rce;  if (rc < 0) {
      ysched_resetdate ();
      DEBUG_YSCHED  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(get current date)---------------*/
   rc = ysched_date__current (-1);
   --rce;  if (rc < 0) {
      ysched_resetdate ();
      DEBUG_YSCHED  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(work out the dow for 1st)-----------*/
   rc = ysched_date__dow_1st ();
   --rce;  if (rc < 0) {
      ysched_resetdate ();
      DEBUG_YSCHED  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(work out the max days)--------------*/
   rc = ysched_date__max_days ();
   --rce;  if (rc < 0) {
      ysched_resetdate ();
      DEBUG_YSCHED  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(work out the max weeks)-------------*/
   rc = ysched_date__max_weeks ();
   --rce;  if (rc < 0) {
      ysched_resetdate ();
      DEBUG_YSCHED  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_YSCHED  yLOG_exit    (__FUNCTION__);
   return 0;
}

char       /*--> test for a specific timing ----------------------------------*/
ySCHED_test        (tSCHED *a_sched, int a_hour, int a_minute)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   tSCHED     *x_sched     = NULL;
   /*---(check location)-----------------*/
   if (a_sched == NULL)  x_sched = &g_sched;
   else                  x_sched = a_sched;
   /*---(effective)----------------------*/
   if (x_sched->eff [100 + mySCHED.s_off] == '_') {
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
   if (a_hour   != ySCHED_ANY && x_sched->hrs [a_hour    ]  == '_') {
      return 0;
   }
   if (a_hour   == ySCHED_ANY && strchr (x_sched->hrs, '1') == NULL) {
      return 0;
   }
   /*---(minutes)------------------------*/
   if (a_minute != ySCHED_ANY && x_sched->min [a_minute  ]  == '_') {
      return 0;
   }
   if (a_minute == ySCHED_ANY && strchr (x_sched->min, '1') == NULL) {
      return 0;
   }
   /*---(complete)-----------------------*/
   /*> return x_sched->dur;                                                           <*/
   return 1;
}

