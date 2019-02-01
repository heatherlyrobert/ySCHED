/*============================----beg-of-source---============================*/
#include   "ySCHED.h"
#include   "ySCHED_priv.h"




/*====================------------------------------------====================*/
/*===----                              dates                           ----===*/
/*====================------------------------------------====================*/
static void      o___DATES___________________o (void) {;}

char       /*--> clear all data fields ---------------------------------------*/
ySCHED_resetdate   (void)
{
   /*---(header)-------------------------*/
   DEBUG_YSCHED  yLOG_senter  (__FUNCTION__);
   mySCHED.nwks  =  mySCHED.ndys  = mySCHED.fdow  = 0;
   mySCHED.cdate =  mySCHED.cset              = 0;
   mySCHED.cyrs  =  mySCHED.cwks  = mySCHED.cmos  = 0;
   mySCHED.cdys  =  mySCHED.cdow  = mySCHED.coff  = 0;
   DEBUG_YSCHED  yLOG_sexit   (__FUNCTION__);
   return 0;
}

char       /*--> set the data for parsing ------------------------------------*/
ysched_date__defense    (int a_year, int a_month, int a_day)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   /*---(header)-------------------------*/
   DEBUG_YSCHED  yLOG_enter   (__FUNCTION__);
   /*---(reset date fields)--------------*/
   ySCHED_resetdate ();
   /*---(check max ranges)---------------*/
   DEBUG_YSCHED  yLOG_complex ("a_year"   , "%d (0 to 1000)", a_year);
   --rce;  if (a_year < 0 || a_year > 1000) {
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
   mySCHED.cyrs            = a_year;
   mySCHED.cmos            = a_month;
   mySCHED.cdys            = a_day;
   /*---(complete)-----------------------*/
   DEBUG_YSCHED  yLOG_exit    (__FUNCTION__);
   return 0;
}

char       /*--> set the data for parsing ------------------------------------*/
ysched_date__current    (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        t           [200];
   long        now         = 0;
   /*---(header)-------------------------*/
   DEBUG_YSCHED  yLOG_enter   (__FUNCTION__);
   /*---(get current date)---------------*/
   now              = time      (NULL);
   mySCHED.cbroke   = localtime (&now);
   strftime (t, 100, "%a, %b %d, %Y", mySCHED.cbroke);
   DEBUG_YSCHED  yLOG_info    ("current"   , t);
   /*---(adjust to requsted)-------------*/
   mySCHED.cbroke->tm_year = mySCHED.cyrs + 100;
   DEBUG_YSCHED  yLOG_value   ("tm_year"   , mySCHED.cbroke->tm_year);
   mySCHED.cbroke->tm_mon  = mySCHED.cmos - 1;
   DEBUG_YSCHED  yLOG_value   ("tm_mon"    , mySCHED.cbroke->tm_mon);
   mySCHED.cbroke->tm_mday = mySCHED.cdys;
   DEBUG_YSCHED  yLOG_value   ("tm_mday"   , mySCHED.cbroke->tm_mday);
   /*---(get the format version)---------*/
   mySCHED.cdate           = mktime    (mySCHED.cbroke);
   mySCHED.cbroke          = localtime (&mySCHED.cdate);
   strftime (t, 100, "%a, %b %d, %Y", mySCHED.cbroke);
   DEBUG_YSCHED  yLOG_info    ("specified" , t);
   /*---(defenses : day)-----------------*/
   --rce;  if (mySCHED.cbroke->tm_mon != mySCHED.cmos - 1) {
      DEBUG_YSCHED  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(calculate the offsets)----------*/
   mySCHED.coff            = (mySCHED.cdate - now) / (60 * 60 * 24);
   DEBUG_YSCHED  yLOG_value   ("coff"      , mySCHED.coff);
   mySCHED.cset            = mySCHED.cdate;
   DEBUG_YSCHED  yLOG_value   ("cset"      , mySCHED.cset);
   /*---(work out the current time)----------*/
   mySCHED.cdow            = mySCHED.cbroke->tm_wday;
   if (mySCHED.cdow == 0) mySCHED.cdow = 7;        /* put sunday at the end                   */
   DEBUG_YSCHED  yLOG_value   ("cdow"      , mySCHED.cdow);
   strftime (t, 10, "%V", mySCHED.cbroke);  /* first more than half week of the year   */
   mySCHED.cwks            = atoi(t);
   DEBUG_YSCHED  yLOG_value   ("cwks"      , mySCHED.cwks);
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
   long        now         = 0;
   /*---(header)-------------------------*/
   DEBUG_YSCHED  yLOG_enter   (__FUNCTION__);
   /*---(work out the dow for 1st)-----------*/
   mySCHED.cbroke->tm_mday = 1;                           /* set  day to the 1st  */
   mySCHED.cdate           = mktime    (mySCHED.cbroke);      /* make epock format    */
   mySCHED.cbroke          = localtime (&mySCHED.cdate);      /* break it down again  */
   mySCHED.fdow            = mySCHED.cbroke->tm_wday;         /* get day of week      */
   if (mySCHED.fdow == 0) mySCHED.fdow = 7;                   /* put sunday at end    */
   strftime(t, 100, "%a, %b %d, %Y", mySCHED.cbroke);
   DEBUG_YSCHED  yLOG_info    ("begin"     , t);
   DEBUG_YSCHED  yLOG_value   ("fdow"      , mySCHED.fdow);
   /*> DEBUG_YSCHED  printf ("%02dday, %02dmon, %01dfdow\n", mySCHED.cbroke->tm_mday, mySCHED.cbroke->tm_mon, mySCHED.fdow);   <*/
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
   char        t           [200];
   long        now         = 0;
   /*---(header)-------------------------*/
   DEBUG_YSCHED  yLOG_enter   (__FUNCTION__);
   /*---(work out the max days)--------------*/
   ++mySCHED.cbroke->tm_mon;                   /* go to the next month            */
   if (mySCHED.cbroke->tm_mon > 11) {          /* if moved to jan, increase year  */
      mySCHED.cbroke->tm_mon = 0;
      ++mySCHED.cbroke->tm_year;
   }
   mySCHED.cbroke->tm_mday = 1;                           /* set  day to the 1st  */
   mySCHED.cdate           = mktime    (mySCHED.cbroke);      /* make epoch format    */
   mySCHED.cdate           = mySCHED.cdate - (60 * 60 * 24);  /* subtract a full day  */
   mySCHED.cbroke          = localtime(&mySCHED.cdate);       /* break it down        */
   mySCHED.ndys            = mySCHED.cbroke->tm_mday;         /* the day is now max   */
   strftime (t, 100, "%a, %b %d, %Y", mySCHED.cbroke);
   DEBUG_YSCHED  yLOG_info    ("end"       , t);
   DEBUG_YSCHED  yLOG_value   ("ndys"      , mySCHED.ndys);
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
   long        now         = 0;
   /*---(header)-------------------------*/
   DEBUG_YSCHED  yLOG_enter   (__FUNCTION__);
   /*---(work out the max weeks)-------------*/
   mySCHED.cbroke->tm_year = mySCHED.cyrs + 101;
   mySCHED.cbroke->tm_mon  = 0;
   mySCHED.cbroke->tm_mday = 1;
   mySCHED.cdate           = mktime(mySCHED.cbroke);
   mySCHED.cdate           = mySCHED.cdate - (60 * 60 * 24);
   mySCHED.cbroke          = localtime(&mySCHED.cdate);
   int xdow = mySCHED.cbroke->tm_wday;
   if (xdow == 0) xdow = 7;
   strftime (t, 100, "%a, %b %d, %Y", mySCHED.cbroke);
   DEBUG_YSCHED  yLOG_info    ("year end"  , t);
   strftime(t, 10, "%V", mySCHED.cbroke);  /* first more than half week of the year   */
   mySCHED.nwks            = atoi(t);
   DEBUG_YSCHED  yLOG_value   ("nwks"      , mySCHED.nwks);
   /*> DEBUG_YSCHED  printf("%1ddow, %s, %02dnwks\n", mySCHED.cbroke->tm_wday, t, mySCHED.nwks);   <*/
   xdow = mySCHED.cbroke->tm_wday;
   if (xdow == 0) xdow = 7;
   if (xdow <  4)  mySCHED.cdate           = mySCHED.cdate - (xdow * 60 * 60 * 24);
   mySCHED.cbroke          = localtime(&mySCHED.cdate);
   strftime(t, 100, "%a, %b %d, %Y", mySCHED.cbroke);
   DEBUG_YSCHED  yLOG_info    ("year end*" , t);
   strftime(t, 10, "%V", mySCHED.cbroke);  /* first more than half week of the year   */
   mySCHED.nwks            = atoi(t);
   DEBUG_YSCHED  yLOG_value   ("nwks"      , mySCHED.nwks);
   /*> DEBUG_YSCHED  printf("%1ddow, %s, %02dnwks\n", mySCHED.cbroke->tm_wday, t, mySCHED.nwks);   <*/
   /*---(complete)-----------------------*/
   DEBUG_YSCHED  yLOG_exit    (__FUNCTION__);
   return 0;
}

char       /*--> set the data for parsing ------------------------------------*/
ySCHED_setdate     (int a_year, int a_month, int a_day)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_YSCHED  yLOG_enter   (__FUNCTION__);
   /*---(defenses)-----------------------*/
   rc = ysched_date__defense (a_year, a_month, a_day);
   --rce;  if (rc < 0) {
      ySCHED_resetdate ();
      DEBUG_YSCHED  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(get current date)---------------*/
   rc = ysched_date__current ();
   --rce;  if (rc < 0) {
      ySCHED_resetdate ();
      DEBUG_YSCHED  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(work out the dow for 1st)-----------*/
   rc = ysched_date__dow_1st ();
   --rce;  if (rc < 0) {
      ySCHED_resetdate ();
      DEBUG_YSCHED  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(work out the max days)--------------*/
   rc = ysched_date__max_days ();
   --rce;  if (rc < 0) {
      ySCHED_resetdate ();
      DEBUG_YSCHED  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(work out the max weeks)-------------*/
   rc = ysched_date__max_weeks ();
   --rce;  if (rc < 0) {
      ySCHED_resetdate ();
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
   /*---(locals)-------------------------*/
   tSCHED   *x_sched   = NULL;
   /*---(check location)-----------------*/
   if (a_sched == NULL)  x_sched = &sched;
   else                  x_sched = a_sched;
   /*---(effective)----------------------*/
   if (x_sched->eff[100 + mySCHED.coff] == '_') {
      return FAILED;
   }
   /*---(based on setdate)---------------*/
   if (x_sched->wks[mySCHED.cwks]  == '_') {
      return FAILED;
   }
   if (x_sched->mos[mySCHED.cmos]  == '_') {
      return FAILED;
   }
   if (x_sched->dow[mySCHED.cdow]  == '_' || x_sched->dys[mySCHED.cdys]  == '_') {
      return FAILED;
   }
   /*---(hours)--------------------------*/
   if (a_hour   != ySCHED_ANY && x_sched->hrs[a_hour    ]  == '_') {
      return FAILED;
   }
   if (a_hour   == ySCHED_ANY && strchr(x_sched->hrs, '1') == NULL) {
      return FAILED;
   }
   /*---(minutes)------------------------*/
   if (a_minute != ySCHED_ANY && x_sched->min[a_minute  ]  == '_') {
      return FAILED;
   }
   if (a_minute == ySCHED_ANY && strchr(x_sched->min, '1') == NULL) {
      return FAILED;
   }
   /*---(complete)-----------------------*/
   /*> return x_sched->dur;                                                           <*/
   return 1;
}
