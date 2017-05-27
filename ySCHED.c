/*============================----beg-of-source---============================*/

#include   "ySCHED.h"
#include   "ySCHED_priv.h"


tLOCAL    its;
tSCHED    sched;
char      global    [500];
char      gloout    [100];
char      effective [500];
char      effout    [100];
int       offset = 0;

char      ySCHED_ver   [500];

#define   MAX_FIELDS      10
struct {
   char        label       [15];
   int         min;
   int         max;
} s_fields [MAX_FIELDS] = {
   /* 12345678901234   123   123   */
   { "minutes"       ,   0 ,  59   },
   { "hours"         ,   0 ,  23   },
   { "days"          ,   1 ,  31   },
   { "months"        ,   1 ,  12   },
   { "daysofweek"    ,   1 ,   7   },
   { "weeks"         ,   1 ,  54   },
   { "years"         ,   0 ,  50   },
   { "---end---"     ,   0 ,   0   },
   { "---end---"     ,   0 ,   0   },
   { "---end---"     ,   0 ,   0   },
};

struct {
   char        field       [15];
   char        label       [10];
   int         min;
   int         max;
   char        rev;
   char        inv;
} s_const [100] = {
   /* 12345678901234    12345678901234   123   123    1     1   */
   /*--- years -------------------------------------------------*/
   { "years"         , "*"             ,   0 , 999,  '-',  '-'   },
   /*--- weeks of the year -------------------------------------*/
   { "weeks"         , "*"             ,   0 ,  54,  '-',  '-'   },
   { "weeks"         , "Lw"            ,   0 ,   0,  'y',  '-'   },
   /*--- months : singles --------------------------------------*/
   { "months"        , "*"             ,   1 ,  12,  '-',  '-'   },
   { "months"        , "Jan"           ,   1 ,   1,  '-',  '-'   },
   { "months"        , "Feb"           ,   2 ,   2,  '-',  '-'   },
   { "months"        , "Mar"           ,   3 ,   3,  '-',  '-'   },
   { "months"        , "Apr"           ,   4 ,   4,  '-',  '-'   },
   { "months"        , "May"           ,   5 ,   5,  '-',  '-'   },
   { "months"        , "Jun"           ,   6 ,   6,  '-',  '-'   },
   { "months"        , "Jul"           ,   7 ,   7,  '-',  '-'   },
   { "months"        , "Aug"           ,   8 ,   8,  '-',  '-'   },
   { "months"        , "Sep"           ,   9 ,   9,  '-',  '-'   },
   { "months"        , "Oct"           ,  10 ,  10,  '-',  '-'   },
   { "months"        , "Nov"           ,  11 ,  11,  '-',  '-'   },
   { "months"        , "Dec"           ,  12 ,  12,  '-',  '-'   },
   /*--- months : ranges ---------------------------------------*/
   { "months"        , "1q"            ,   1 ,   3,  '-',  '-'   },
   { "months"        , "2q"            ,   4 ,   6,  '-',  '-'   },
   { "months"        , "3q"            ,   7 ,   9,  '-',  '-'   },
   { "months"        , "4q"            ,  10 ,  12,  '-',  '-'   },
   { "months"        , "Lq"            ,   0 ,   2,  'y',  '-'   },
   { "months"        , "1h"            ,   1 ,   6,  '-',  '-'   },
   { "months"        , "2h"            ,   7 ,  12,  '-',  '-'   },
   { "months"        , "Spring"        ,   3 ,   5,  '-',  '-'   },
   { "months"        , "Summer"        ,   6 ,   8,  '-',  '-'   },
   { "months"        , "Autumn"        ,   9 ,  11,  '-',  '-'   },
   { "months"        , "Winter"        ,   3 ,  11,  '-',  'y'   },
   /*--- days --------------------------------------------------*/
   { "days"          , "*"             ,   1 ,  31,  '-',  '-'   },
   { "days"          , "1w"            ,   1 ,   7,  '-',  '-'   },
   { "days"          , "2w"            ,   8 ,  14,  '-',  '-'   },
   { "days"          , "3w"            ,  15 ,  21,  '-',  '-'   },
   { "days"          , "4w"            ,  22 ,  28,  '-',  '-'   },
   { "days"          , "5w"            ,  29 ,  35,  '-',  '-'   },
   { "days"          , "Lw"            ,   0 ,   6,  'y',  '-'   },
   { "days"          , "Ld"            ,   0 ,   0,  'y',  '-'   },
   /*--- days of the week : singles ----------------------------*/
   { "daysofweek"    , "*"             ,   1 ,   7,  '-',  '-'   },
   { "daysofweek"    , "Mo"            ,   1 ,   1,  '-',  '-'   },
   { "daysofweek"    , "Tu"            ,   2 ,   2,  '-',  '-'   },
   { "daysofweek"    , "We"            ,   3 ,   3,  '-',  '-'   },
   { "daysofweek"    , "Th"            ,   4 ,   4,  '-',  '-'   },
   { "daysofweek"    , "Fr"            ,   5 ,   5,  '-',  '-'   },
   { "daysofweek"    , "Sa"            ,   6 ,   6,  '-',  '-'   },
   { "daysofweek"    , "Su"            ,   7 ,   7,  '-',  '-'   },
   /*--- days of the week : ranges -----------------------------*/
   { "daysofweek"    , "W"             ,   1 ,   5,  '-',  '-'   },
   { "daysofweek"    , "Work"          ,   1 ,   5,  '-',  '-'   },
   { "daysofweek"    , "E"             ,   6 ,   7,  '-',  '-'   },
   { "daysofweek"    , "Off"           ,   6 ,   7,  '-',  '-'   },
   /*--- hours : ranges ----------------------------------------*/
   { "hours"         , "*"             ,   0 ,  23,  '-',  '-'   },
   { "hours"         , "B"             ,   8 ,  16,  '-',  '-'   },
   { "hours"         , "Work"          ,   8 ,  16,  '-',  '-'   },
   { "hours"         , "O"             ,   8 ,  16,  '-',  'y'   },
   { "hours"         , "Off"           ,   8 ,  16,  '-',  'y'   },
   { "hours"         , "Batch"         ,   5 ,  22,  '-',  'y'   },
   /*--- hours : extended --------------------------------------*/
   { "hours"         , "Light"         ,   7 ,  18,  '-',  '-'   },
   { "hours"         , "Dark"          ,   7 ,  18,  '-',  'y'   },
   /*--- hours : special ---------------------------------------*/
   { "hours"         , "Midnight"      ,   2 ,  22,  '-',  'y'   },
   { "hours"         , "Graveyard"     ,   2 ,   4,  '-',  '-'   },
   { "hours"         , "Dawn"          ,   5 ,   7,  '-',  '-'   },
   { "hours"         , "Morning"       ,   8 ,  10,  '-',  '-'   },
   { "hours"         , "Midday"        ,  11 ,  13,  '-',  '-'   },
   { "hours"         , "Afternoon"     ,  14 ,  16,  '-',  '-'   },
   { "hours"         , "Dusk"          ,  17 ,  19,  '-',  '-'   },
   { "hours"         , "Evening"       ,  20 ,  22,  '-',  '-'   },
   /*--- minutes -----------------------------------------------*/
   { "minutes"       , "*"             ,   0 ,  59,  '-',  '-'   },
   /*--- done --------------------------------------------------*/
   { "---end---"     , "---end---"     ,   0 ,   0,  '-',  '-'   },
};

/*> else if (len == 2 && strchr("wq", beg[1]) != 0) {                                   <* 
 *>    /+---(legal values)-------------+/                                               <* 
 *>    if (strchr("12345L", beg[0]) == 0) RETURN (rce - 2);  /+ check legal values +/   <* 
 *>    /+---(in days)------------------+/                                               <* 
 *>    if (TYPE_DYS && beg[1] == 'w') {                                                 <* 
 *>       if (beg[0] == 'L') {                                                          <* 
 *>          end_val = its.ndys;                                                        <* 
 *>          beg_val = end_val - 6;                                                     <* 
 *>       } else {                                                                      <* 
 *>          beg_val = 7 * (beg[0] - '1') + 1;                                          <* 
 *>          end_val = beg_val + 6;                                                     <* 
 *>       }                                                                             <* 
 *>    }                                                                                <* 
 *>    /+---(in months)----------------+/                                               <* 
 *>    else if (TYPE_MOS && beg[1] == 'q') {                                            <* 
 *>       if (beg[0] == 'L')  beg_val = 10;                                             <* 
 *>       else                beg_val = 3 * (beg[0] - '1') + 1;                         <* 
 *>       end_val = beg_val + 2;                                                        <* 
 *>    }                                                                                <* 
 *>    /+---(can't use in field)-------+/                                               <* 
 *>    else RETURN (rce - 3);                                                           <* 
 *> }                                                                                   <*/

#define     TYPE_MNS      a_type == 0
#define     TYPE_HRS      a_type == 1
#define     TYPE_DYS      a_type == 2
#define     TYPE_MOS      a_type == 3
#define     TYPE_DOW      a_type == 4
#define     TYPE_WKS      a_type == 5
#define     TYPE_YRS      a_type == 6
#define     RETURN(RC)   { strcpy (a_array, x_error); strcpy (its.last, x_error); return RC; }

char *s_input     = NULL;
int   s_len       =  0;
char  s_not       = '-';
char  s_rev       = '-';
char  s_inv       = '-';
int   s_stp       =  1;
int   s_beg       =  0;
int   s_end       =  0;
int   s_min       =  0;
int   s_smax      =  0;
int   s_bmax      =  0;



char       /*----: update effective for a endpoint dates ---------------------*/
ySCHED__effend     (char *a_date, char a_side, long a_now);

/*====================------------------------------------====================*/
/*===----                           utility                            ----===*/
/*====================------------------------------------====================*/
static void      o___UTILITY_________________o (void) {;}

char*      /* ---- : return library versioning information -------------------*/
ySCHED_version     (void)
{
   char    t [20] = "";
#if    __TINYC__ > 0
   strncpy (t, "[tcc built]", 15);
#elif  __GNUC__  > 0
   strncpy (t, "[gnu gcc  ]", 15);
#else
   strncpy (t, "[unknown  ]", 15);
#endif
   snprintf (ySCHED_ver, 100, "%s   %s : %s", t, YSCHED_VER_NUM, YSCHED_VER_TXT);
   return ySCHED_ver;
}

char         /*--> set debugging mode --------------------[ ------ [ ------ ]-*/
ySCHED_debug       (char a_flag)
{
   /*---(set debug flag)-----------------*/
   if   (a_flag == '-' || a_flag == 'n')  its.debug   = '-';
   else                                   its.debug   = 'y';
   /*---(complete)-----------------------*/
   return 0;
}

char       /*----: initialize key globals ------------------------------------*/
ySCHED__init       (void)
{
   /*---(locals)-------------------------*/
   int       i         = 0;
   /*---(overall)------------------------*/
   its.status  = SUCCESS;
   /*---(working)------------------------*/
   its.full[0] = '\0';
   its.recd[0] = '\0';
   its.xmin    = NULL;
   its.xhrs    = NULL;
   its.xdys    = NULL;
   its.xmos    = NULL;
   its.xdow    = NULL;
   its.xwks    = NULL;
   its.xdur    = NULL;
   /*---(schedule)-----------------------*/
   for (i = 0; i < MAX_FIELD; ++i) {
      sched.min [i]    = '_';
      sched.hrs [i]    = '_';
      sched.dys [i]    = '_';
      sched.mos [i]    = '_';
      sched.dow [i]    = '_';
      sched.wks [i]    = '_';
   }
   /*---(effective)----------------------*/
   for (i = 0; i < 500      ; ++i) {
      sched.eff [i]    = '_';
   }
   /*---(duration)-----------------------*/
   /*> sched.dur   = 0;                                                               <*/
   /*---(debug)--------------------------*/
   its.debug   = '-';
   /*---(complete)-----------------------*/
   return 0;
}



/*====================------------------------------------====================*/
/*===----                         load and save                        ----===*/
/*====================------------------------------------====================*/
static void      o___STORE___________________o (void) {;}

char       /*----: save the results in an alternate location -----------------*/
ySCHED_save        (tSCHED *a_sched)
{
   if (its.status == FAILED) return FAILED;
   strncpy(a_sched->min, sched.min, 100);
   strncpy(a_sched->hrs, sched.hrs, 100);
   strncpy(a_sched->dys, sched.dys, 100);
   strncpy(a_sched->mos, sched.mos, 100);
   strncpy(a_sched->dow, sched.dow, 100);
   strncpy(a_sched->wks, sched.wks, 100);
   strncpy(a_sched->yrs, sched.yrs, 100);
   strncpy(a_sched->eff, sched.eff, 500);
   /*> a_sched->dur = sched.dur;                                                      <*/
   return 0;
}

char       /*----: load the interpretation from an alternate location --------*/
ySCHED_load        (tSCHED *a_sched)
{
   strncpy (sched.min, a_sched->min, 100);
   strncpy (sched.hrs, a_sched->hrs, 100);
   strncpy (sched.dys, a_sched->dys, 100);
   strncpy (sched.mos, a_sched->mos, 100);
   strncpy (sched.dow, a_sched->dow, 100);
   strncpy (sched.wks, a_sched->wks, 100);
   strncpy (sched.yrs, a_sched->yrs, 100);
   strncpy (sched.eff, a_sched->eff, 500);
   /*> sched.dur = a_sched->dur;                                                      <*/
   return 0;
}



/*====================------------------------------------====================*/
/*===----                              dates                           ----===*/
/*====================------------------------------------====================*/
static void      o___DATES___________________o (void) {;}

char       /*----: set the date for parsing ----------------------------------*/
ySCHED_resetdate   (void)
{
   its.nwks  =  its.ndys  = its.fdow  = 0;
   its.cdate =  its.cset              = 0;
   its.cyrs  =  its.cwks  = its.cmos  = 0;
   its.cdys  =  its.cdow  = its.coff  = 0;
   return 0;
}

char       /*----: set the date for parsing ----------------------------------*/
ySCHED_setdate     (int a_year, int a_month, int a_day)
{
   /*---(locals)-------------------------*/
   char        tmp         [200];
   long        now         = 0;
   /*---(reset date fields)--------------*/
   ySCHED_resetdate ();
   /*---(defenses : year)----------------*/
   if (a_year  <     0)      return  -1;
   if (a_year  >  1000)      return  -2;
   /*---(defenses : month)---------------*/
   if (a_month <     1)      return  -3;
   if (a_month >    12)      return  -4;
   /*---(defenses : day)-----------------*/
   if (a_day   <     1)      return  -5;
   if (a_day   >    31)      return  -6;
   /*---(store supplied values)----------*/
   its.cyrs            = a_year;
   its.cmos            = a_month;
   its.cdys            = a_day;
   DEBUG_YSCHED  printf ("ySCHED_setdate : yrs=%2d, mos=%2d, dys=%2d\n", its.cyrs, its.cmos, its.cdys);
   /*---(get current date)---------------*/
   now                 = time      (NULL);
   its.cbroke          = localtime (&now);
   strftime (tmp, 100, "%a, %b %d, %Y", its.cbroke);
   DEBUG_YSCHED  printf("current date   : %s\n", tmp);
   /*---(adjust to requsted)-------------*/
   its.cbroke->tm_year = a_year + 100;
   its.cbroke->tm_mon  = a_month - 1;
   its.cbroke->tm_mday = a_day;
   /*---(get the format version)---------*/
   its.cdate           = mktime    (its.cbroke);
   its.cbroke          = localtime (&its.cdate);
   strftime (tmp, 100, "%a, %b %d, %Y", its.cbroke);
   DEBUG_YSCHED  printf("specified date : %s, ", tmp);
   /*---(defenses : day)-----------------*/
   if (its.cbroke->tm_mon != a_month - 1) {
      ySCHED_resetdate ();
      return -7;
   }
   /*---(calculate the offsets)----------*/
   its.coff            = (its.cdate - now) / (60 * 60 * 24);
   its.cset            = its.cdate;
   /*> printf ("offset = %d\n", its.coff);                                            <*/
   /*---(work out the current time)----------*/
   its.cdow            = its.cbroke->tm_wday;
   if (its.cdow == 0) its.cdow = 7;        /* put sunday at the end                   */
   strftime (tmp, 10, "%V", its.cbroke);  /* first more than half week of the year   */
   its.cwks            = atoi(tmp);
   DEBUG_YSCHED  printf ("%02dday, %02dmon, %01dcdow, %02dwoy\n", its.cdys, its.cmos, its.cdow, its.cwks);
   /*---(work out the dow for 1st)-----------*/
   its.cbroke->tm_mday = 1;                           /* set  day to the 1st  */
   its.cdate           = mktime    (its.cbroke);      /* make epock format    */
   its.cbroke          = localtime (&its.cdate);      /* break it down again  */
   its.fdow            = its.cbroke->tm_wday;         /* get day of week      */
   if (its.fdow == 0) its.fdow = 7;                   /* put sunday at end    */
   strftime(tmp, 100, "%a, %b %d, %Y", its.cbroke);
   DEBUG_YSCHED  printf("month begin    : %s, ", tmp);
   DEBUG_YSCHED  printf ("%02dday, %02dmon, %01dfdow\n", its.cbroke->tm_mday, its.cbroke->tm_mon, its.fdow);
   /*---(work out the max days)--------------*/
   ++its.cbroke->tm_mon;                   /* go to the next month            */
   if (its.cbroke->tm_mon > 11) {          /* if moved to jan, increase year  */
      its.cbroke->tm_mon = 0;
      ++its.cbroke->tm_year;
   }
   its.cbroke->tm_mday = 1;                           /* set  day to the 1st  */
   its.cdate           = mktime    (its.cbroke);      /* make epoch format    */
   its.cdate           = its.cdate - (60 * 60 * 24);  /* subtract a full day  */
   its.cbroke          = localtime(&its.cdate);       /* break it down        */
   its.ndys            = its.cbroke->tm_mday;         /* the day is now max   */
   strftime (tmp, 100, "%a, %b %d, %Y", its.cbroke);
   DEBUG_YSCHED  printf("month end      : %s, ", tmp);
   DEBUG_YSCHED  printf("%02dndys\n", its.ndys);
   /*---(work out the max weeks)-------------*/
   its.cbroke->tm_year = its.cyrs + 101;
   its.cbroke->tm_mon  = 0;
   its.cbroke->tm_mday = 1;
   its.cdate           = mktime(its.cbroke);
   its.cdate           = its.cdate - (60 * 60 * 24);
   its.cbroke          = localtime(&its.cdate);
   int xdow = its.cbroke->tm_wday;
   if (xdow == 0) xdow = 7;
   strftime (tmp, 100, "%a, %b %d, %Y", its.cbroke);
   DEBUG_YSCHED  printf("year end       : %s, ", tmp);
   strftime(tmp, 10, "%V", its.cbroke);  /* first more than half week of the year   */
   its.nwks            = atoi(tmp);
   DEBUG_YSCHED  printf("%1ddow, %s, %02dnwks\n", its.cbroke->tm_wday, tmp, its.nwks);
   xdow = its.cbroke->tm_wday;
   if (xdow == 0) xdow = 7;
   if (xdow <  4)  its.cdate           = its.cdate - (xdow * 60 * 60 * 24);
   its.cbroke          = localtime(&its.cdate);
   strftime(tmp, 100, "%a, %b %d, %Y", its.cbroke);
   DEBUG_YSCHED  printf("year end (adj) : %s, ", tmp);
   strftime(tmp, 10, "%V", its.cbroke);  /* first more than half week of the year   */
   its.nwks            = atoi(tmp);
   DEBUG_YSCHED  printf("%1ddow, %s, %02dnwks\n", its.cbroke->tm_wday, tmp, its.nwks);
   /*---(complete)-----------------------*/
   return 0;
}

int        /*----: test for a specific time ----------------------------------*/
ySCHED_test        (tSCHED *a_sched, int a_hour, int a_minute)
{
   /*---(locals)-------------------------*/
   tSCHED   *x_sched   = NULL;
   /*---(check location)-----------------*/
   if (a_sched == NULL)  x_sched = &sched;
   else                  x_sched = a_sched;
   /*---(effective)----------------------*/
   if (x_sched->eff[100 + its.coff] == '_') {
      return FAILED;
   }
   /*---(based on setdate)---------------*/
   if (x_sched->wks[its.cwks]  == '_') {
      return FAILED;
   }
   if (x_sched->mos[its.cmos]  == '_') {
      return FAILED;
   }
   if (x_sched->dow[its.cdow]  == '_' || x_sched->dys[its.cdys]  == '_') {
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



/*====================------------------------------------====================*/
/*===----                            effective                         ----===*/
/*====================------------------------------------====================*/
static void      o___EFFECTIVE_______________o (void) {;}

char       /*----: reset the globals and effective dates ---------------------*/
ySCHED_reset       (void)
{
   /*---(locals)-------------------------*/
   int       i         = 0;            /* loop iterator -- day                */
   /*---(initialize)---------------------*/
   for (i = 0; i < 500; ++i) {
      effective[i] = '1';
   }
   ySCHED__effout();
   strncpy (global, effective, 500);
   strncpy (gloout, effout   , 100);
   /*---(initialize)---------------------*/
   return 0;
}

char       /*----: initialize the effective range ----------------------------*/
ySCHED__effinit    (void)
{
   /*---(locals)-------------------------*/
   int       i         = 0;            /* loop iterator -- day                */
   /*---(initialize)---------------------*/
   for (i = 0; i < 500; ++i) {
      effective[i] = '1';
      if (global[i] == '_') effective[i] = '_';
   }
   ySCHED__effout();
   /*---(complete)-----------------------*/
   return 0;
}

char       /*----: add efffective dates to global ----------------------------*/
ySCHED__globalize  (char *a_recd, long a_now)
{
   /*---(locals)-------------------------*/
   int       i         = 0;            /* loop iterator -- day                */
   char     *p         = NULL;         /* token pointer for strtok            */
   char     *q         = NULL;         /* record type                         */
   long      now       = 0;
   /*---(current time)-------------------*/
   if (a_now != 0)  now   = a_now;
   else             now   = time(NULL);
   /*---(run)----------------------------*/
   q = strtok (a_recd, " ");
   p = strtok (NULL  , " ");
   if (p == NULL) return -1;
   if (strcmp(q, ".validity")  == 0) {
      ySCHED__effend (p, 'b', now);
      p = strtok (NULL  , " ");
      ySCHED__effend (p, 'e', now);
   } else {
      ySCHED__effnot (p, now);
   }
   /*---(migrate)------------------------*/
   for (i = 0; i < 500; ++i) {
      if (effective[i] == '_')  global[i] = '_';
   }
   strncpy (effective, global , 500);
   ySCHED__effout  ();
   strncpy (gloout   , effout , 100);
   ySCHED__effinit ();
   /*> printf ("globalized as <%s>\n", gloout);                                       <*/
   /*---(complete)-----------------------*/
   return 0;
}

char       /*----: make a printable version of the effective dates -----------*/
ySCHED__effout     (void)
{
   int       i;
   effout [ 0] = '<';
   for (i =  0; i <  10; ++i)  effout [i + 1] = effective[i +  90];
   effout [11] = ' ';
   effout [12] = effective[100];
   effout [13] = ' ';
   for (i =  0; i <  10; ++i)  effout [i + 14] = effective[i + 101];
   effout [24] = '>';
   effout [25] = '\0';
   return 0;
}

char       /*----: update effective for a endpoint dates ---------------------*/
ySCHED__effend     (char *a_date, char a_side, long a_now)
{
   /*---(locals)-------------------------*/
   char     *p         = NULL;         /* token pointer for strtok            */
   long      rc        = 0;            /* simple return code                  */
   tTIME     d;                        /* broken down time                    */
   long      e         = 0;            /* unix/epoch time                     */
   long      xoff      = 0;            /* calculated offset                   */
   int       i         = 0;            /* loop iterator -- day                */
   /*---(initialize date)----------------*/
   localtime_r (&a_now, &d);
   /*---(beg date)-----------------------*/
   rc = 0;
   if (a_date == NULL || strncmp(a_date, "-", 1) == 0) {
      rc = 1;
      if (a_side = 'b') d.tm_year = 0;
      else              d.tm_year = 3000;
   }
   if (rc == 0) rc  = (int) strptime (a_date, "%m/%d/%y", &d);
   if (rc == 0) rc  = (int) strptime (a_date, "%m.%d.%y", &d);
   if (rc == 0) rc  = (int) strptime (a_date, "%m/%d",    &d);
   if (rc == 0) rc  = (int) strptime (a_date, "%m.%d",    &d);
   if (rc == 0) { printf ("OUCH\n"); ySCHED__effout(); return 0; }
   e      = mktime  (&d);
   xoff = (e - a_now) / (60 * 60 * 24);
   /*> printf ("a_date       = <%s>\n", a_date);                                      <* 
    *> printf ("   years     = %d\n", d.tm_year);                                     <* 
    *> printf ("   months    = %d\n", d.tm_mon);                                      <* 
    *> printf ("   days      = %d\n", d.tm_mday);                                     <* 
    *> printf ("   e         = %ld\n", e);                                            <* 
    *> printf ("   so xoff   = %ld\n", xoff);                                         <*/
   if (xoff < -100) xoff = -100;
   if (xoff <= 365) {
      xoff += 100;
      for (i = 0; i < 500; ++i) {
         if (a_side == 'b' && i <  xoff)   effective[i] = '_';
         if (a_side == 'e' && i >  xoff)   effective[i] = '_';
      }
   }
   /*---(complete)-----------------------*/
   return 0;
}

char       /*----: update effective for a list of exceptions -----------------*/
ySCHED__effnot     (char *a_list, long a_now)
{
   /*---(locals)-------------------------*/
   char     *p         = NULL;         /* token pointer for strtok            */
   long      rc        = 0;            /* simple return code                  */
   tTIME     d;                        /* broken down time                    */
   long      e         = 0;            /* unix/epoch time                     */
   long      xoff      = 0;            /* calculated offset                   */
   /*---(initialize date)----------------*/
   localtime_r (&a_now, &d);
   /*---(exceptions)---------------------*/
   p  =  strtok (a_list,    ",");
   while (p != NULL) {
      rc = 0;
      if (rc == 0) rc  = (int) strptime (p, "%m/%d/%y", &d);
      if (rc == 0) rc  = (int) strptime (p, "%m.%d.%y", &d);
      if (rc == 0) rc  = (int) strptime (p, "%m/%d",    &d);
      if (rc == 0) rc  = (int) strptime (p, "%m.%d",    &d);
      if (rc == 0) { printf ("OUCH\n"); ySCHED__effout(); return 0; }
      e      = mktime  (&d);
      xoff = (e - a_now) / (60 * 60 * 24);
      /*> printf ("exception at %s\n", p);                                            <* 
       *> printf ("   years     = %d\n", d.tm_year);                                  <* 
       *> printf ("   months    = %d\n", d.tm_mon);                                   <* 
       *> printf ("   days      = %d\n", d.tm_mday);                                  <* 
       *> printf ("   other     = %ld\n", e);                                         <* 
       *> printf ("   so xoff   = %ld\n", xoff);                                      <*/
      if (xoff >= -100 && xoff <= 365) {
         xoff += 100;
         effective[xoff] = '_';
      }
      p  =  strtok (NULL,    ",");
   }
   return 0;
}

char       /*----: set the effective date range ------------------------------*/
ySCHED__effective  (char *a_recd, long a_now)
{
   /*---(locals)-------------------------*/
   char     *p         = NULL;
   char     *xbeg      = NULL;
   char     *xend      = NULL;
   char     *xexcept   = NULL;
   long      now       = 0;
   long      other     = 0;
   long      rcp       = 0;
   int       i, j;
   /*---(initialize)---------------------*/
   /*> yLOG_info  ("effective", "clearing...");                                       <*/
   for (i = 0; i < 500; ++i) {
      effective[i] = '1';
      if (global[i] == '_') effective[i] = '_';
   }
   ySCHED__effout();
   /*> printf ("eff clean = <%s>\n", effout);                                         <*/
   /*---(defense)------------------------*/
   if (strlen (a_recd) < 10)                      return -1;
   if (strncmp(a_recd, "reset"    ,   5) == 0)    return -1;
   if (strncmp(a_recd, ".effective", 10) != 0)    return -1;
   /*---(break it down)------------------*/
   strncpy(its.recd, a_recd  , MAX_RECD);
   strtok(its.recd, " ");
   xbeg       = strtok(NULL    , " ");
   xend       = strtok(NULL    , " ");
   xexcept    = strtok(NULL    , " ");
   /*---(current time)-------------------*/
   if (a_now != 0)  now   = a_now;
   else             now   = time(NULL);
   /*---(beg and end)--------------------*/
   ySCHED__effend (xbeg, 'b', now);
   ySCHED__effend (xend, 'e', now);
   /*---(exceptions)---------------------*/
   ySCHED__effnot (xexcept, now);
   /*---(print)--------------------------*/
   ySCHED__effout ();
   /*> printf ("effective = <%s>\n", effout);                                         <*/
   /*> yLOG_info  ("effective", effout );                                             <*/
   /*---(complete)-----------------------*/
   return 0;
}



/*====================------------------------------------====================*/
/*===----                            updated                           ----===*/
/*====================------------------------------------====================*/
static void      o___UPDATED_________________o (void) {;}

/*
 *   grammar follows a standard format...
 *
 *   <num>   a specific number
 *   <rng>   is <num>-<num> and means all numbers between the first and last
 *   <bas>   either <num> or <lst>
 *   <stp>   is a increment which skips numbers in a range
 *   <pre>   is a prefix that modifies the meaning of a range
 *   <ful>   is <pre><bas>/<stp>
 *   <lst>   is <ful>,<ful>,<ful>
 *
 *
 */

char         /*--: interpret modifier --------------------[ leaf   [ ------ ]-*/
ySCHED__prep       (int a_type)
{
   /*---(initialize)---------------------*/
   s_beg       =  -1;
   s_end       =  -1;
   s_not       = '-';
   s_rev       = '-';
   s_inv       = '-';
   s_stp       =   1;
   /*---(set field limits)---------------*/
   s_min       = s_fields [a_type].min;
   s_bmax      = s_fields [a_type].max;
   s_smax      = s_bmax;
   if      (TYPE_DYS)     s_smax = its.ndys;
   else if (TYPE_WKS)     s_smax = its.nwks;
   /*---(measure input string)-----------*/
   s_len       = strlen (s_input);
   /*---(complete)-----------------------*/
   return 0;
}

char         /*--: interpret modifier --------------------[ leaf   [ ------ ]-*/
ySCHED__modifier   (void)
{ /*---(design rules)--------------------*/
   /*
    *  there can only be one modifier per range specifier and it must appear
    *  as the very first character
    *         none         turn each element on
    *     ~   not          turn off each element, instead of turning it on
    *     ^   reverse      count from the end, rather than the start
    *     !   inverse      turn on each element not listed, meaning, all but
    *
    */
   /*---(parse)--------------------------*/
   while (s_input [0] != '\0') {
      /*---(filter)----------------------*/
      if (strchr ("~^!", s_input [0]) == NULL)  return 0;
      /*---(parse)-----------------------*/
      if (s_input [0] == '!')  s_not = 'y';
      if (s_input [0] == '^')  s_rev = 'y';
      if (s_input [0] == '~')  s_inv = 'y';
      /*---(next)------------------------*/
      ++s_input;
      --s_len;
   }
   /*---(complete)-----------------------*/
   return 0;
}

char         /*--: interpret step ------------------------[ leaf   [ ------ ]-*/
ySCHED__step       (void)
{ /*---(design rules)--------------------*/
   /*
    *  there can be only one step value, it must appear last, and it is
    *  preceeded by the forward slash ('/')
    *
    */
   /*---(locals)-----------+-----------+-*/
   char       *p           = NULL;
   char       *q           = "/";
   char       *s           = NULL;
   char       *r           = NULL;
   char        rce         = -10;
   /*---(initialize)---------------------*/
   p      = s_input + s_len - 1;
   /*> printf ("start     %3d:%c\n", p[0], p[0]);                                     <*/
   /*---(find marker)--------------------*/
   while (p > s_input) {
      /*> printf ("checking  %3d:%c\n", p[0], p[0]);                                  <*/
      if (p [0] == '/')  break;
      --p;
   }
   if (p == s_input)   return 0;
   --rce;  if (p [1] == '\0'   )   return rce;
   p [0] = '\0';
   --s_len;
   /*---(test)---------------------------*/
   r = p + 1;
   --rce;
   while (r [0] != NULL) {
      /*> printf ("verifying %3d:%c\n", r[0], r[0]);                                  <*/
      if (strchr ("0123456789", r [0]) == NULL)  return rce;
      ++r;
      --s_len;
   }
   /*---(interpret)----------------------*/
   s_stp = atoi (p + 1);
   --rce;  if (s_stp <= 0)  return rce;
   /*---(complete)-----------------------*/
   return 0;
}

char         /*--: interpret constants -------------------[ leaf   [ ------ ]-*/
ySCHED__const      (int a_type)
{ /*---(design rules)--------------------*/
   /*
    *  constants are singluar and must be separated with commas
    *
    */
   /*---(locals)-----------+-----------+-*/
   int         i           = 0;
   /*---(look for constants)-------------*/
   while (s_const [i].field [0] != '-') {
      /*---(fast checks)-----------------*/
      if (s_const [i].field [0] != s_fields [a_type].label [0]) {
         ++i;
         continue;
      }
      if (s_const [i].label [0] != s_input [0]) {
         ++i;
         continue;
      }
      /*---(slow checks)-----------------*/
      if (strcmp (s_const [i].field, s_fields [a_type].label) != 0) {
         ++i;
         continue;
      }
      if (strcmp (s_const [i].label, s_input) != 0) {
         ++i;
         continue;
      }
      /*---(process)---------------------*/
      s_beg = s_const [i].min;
      s_end = s_const [i].max;
      if (s_const [i].rev == 'y')  s_rev = s_const [i].rev;
      if (s_const [i].inv == 'y')  s_inv = s_const [i].inv;
      break;
   }
   /*---(return if none found)-----------*/
   if (s_beg == -1) return 0;
   /*---(complete)-----------------------*/
   return i;
}

int          /*--: interpret number ----------------------[ leaf   [ ------ ]-*/
ySCHED__number     (int a_type, char *a_number)
{ /*---(design rules)--------------------*/
   /*
    * positive integers only with no modifiers
    *
    */
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;
   char       *r           = 0;
   int         x_num       = -1;
   /*---(defense: null)-----------------*/
   --rce;  if (a_number [0] == '\0')  return rce;
   /*---(defense: non-numeric)----------*/
   r = a_number;
   --rce;  while (r [0] != NULL) {
      /*> printf ("verifying %3d:%c\n", r[0], r[0]);                                  <*/
      if (strchr ("0123456789", r [0]) == NULL)  return rce;
      ++r;
   }
   /*---(interpret)----------------------*/
   x_num = atoi (a_number);
   /*---(denfense: universal)------------*/
   --rce;  if (x_num == 0 && ! strchr ("0", a_number) == 0)  return rce;
   --rce;  if (x_num <  0)                                   return rce;
   /*---(check ranges)-------------------*/
   --rce;  if (x_num < s_fields [a_type].min)  return rce;
   --rce;  if (x_num > s_fields [a_type].max)  return rce;
   /*---(check exceptions)---------------*/
   --rce;  if (TYPE_DYS)  if (x_num > its.ndys) return rce;
   --rce;  if (TYPE_WKS)  if (x_num > its.nwks) return rce;
   /*---(complete)-----------------------*/
   return x_num;
}

int          /*--: interpret special day references ------[ leaf   [ ------ ]-*/
ySCHED__day        (void)
{ /*---(design rules)--------------------*/
   /*---(locals)-----------+-----------+-*/
   int         rc          = 0;
   char        rce         = -10;
   char        x_type      = '-';
   int         x_day       =  0;
   int         x_dow       =  0;
   /*---(check request)------------------*/
   x_type = s_input [s_len - 1];
   s_input [s_len - 1] = '\0';
   rc = ySCHED__number (PARSE_DYS, s_input);
   if (rc < 0) {
      --rce;  if (strcmp ("L", s_input) != 0)  return rce;
      rc = its.ndys;
   }
   x_day = rc;
   /*---(determine day of week)----------*/
   x_dow = (((x_day + its.fdow) - 1) % 7);
   if (x_dow == 0) x_dow = 7;
   /*---(handle types)-------------------*/
   switch (x_type) {
   case 'a' :  /* after type, so push weekends forward  */
      if (x_dow == 7       )   { x_day += 1; x_dow = 1; }  /* next monday     */
      if (x_dow == 6       )   { x_day += 2; x_dow = 1; }  /* next monday     */
      if (x_day >  its.ndys)   { x_day -= 3; x_dow = 5; }  /* last friday     */
      break;
   case 'b' :  /* before type, so push weekends backward */
      if (x_dow == 7       )   { x_day -= 2; x_dow = 5; }  /* prev friday     */
      if (x_dow == 6       )   { x_day -= 1; x_dow = 5; }  /* prev friday     */
      if (x_day <= 0       )   { x_day += 3; x_dow = 1; }  /* first monday    */
      break;
   case 'n' :  /* nearist type, so push sat back and sun forward */
      if (x_dow == 7       )   { x_day += 1; x_dow = 1; }  /* next monday     */
      if (x_dow == 6       )   { x_day -= 1; x_dow = 5; }  /* prev friday     */
      if (x_day >  its.ndys)   { x_day -= 3; x_dow = 5; }  /* last friday     */
      if (x_day <= 0       )   { x_day += 3; x_dow = 1; }  /* first monday    */
      break;
   }
   /*---(complete)-----------------------*/
   return x_day;
}

char         /*--: interpret ranges ----------------------[ ------ [ ------ ]-*/
ySCHED__range      (int  a_type)
{ /*---(design rules)--------------------*/
   /*
    *  ranges are one of four forms...
    *     n         individual number
    *     <n        less than and equal to a number
    *     n>        greater than and equal to a number
    *     n-n       between to number, inclusive
    *     na        individual number
    *
    */
   /*---(locals)-----------+-----------+-*/
   int         i           = 0;
   char        rc          = 0;
   char        rce         = -10;
   char       *r           = NULL;
   /*---(handle lesser)------------------*/
   --rce;
   if (s_input [0] == '<') {
      s_input [0] = '\0';
      rc = ySCHED__number (a_type, s_input + 1);
      if (rc >= 0) {
         s_beg = 0;
         s_end = rc;
         return 0;
      }
      return rce;
   }
   /*---(handle greater)-----------------*/
   --rce;
   if (s_input [s_len - 1] == '>') {
      s_input [s_len - 1] = '\0';
      rc = ySCHED__number (a_type, s_input);
      if (rc >= 0) {
         s_beg = rc;
         s_end = 999;
         return 0;
      }
      return rce;
   }
   /*---(handle postfix)-----------------*/
   if (strchr ("abn", s_input [s_len - 1]) != NULL) {
      --rce;  if (a_type != PARSE_DYS)  return rce;
      rc = ySCHED__day ();
      --rce;  if (rc <  0)  return rce;
      s_beg = s_end = rc;
      return 0;
   }
   /*---(find dash)----------------------*/
   r = s_input;
   while (r [0] != NULL) {
      if (r [0] == '-')  break;
      ++r;
   }
   if (r [0] == '-') {
      r [0] = '\0';
      rc = ySCHED__number (a_type, s_input);
      --rce;  if (rc <  0)   return rce;
      s_beg = rc;
      rc = ySCHED__number (a_type, r + 1);
      --rce;  if (rc <  0) { s_beg = -1;  return rce; }
      s_end = rc;
      --rce;  if (s_beg > s_end) { s_beg = s_end = -1;  return rce; }
      return 0;
   }
   /*---(handle simple value)------------*/
   rc = ySCHED__number (a_type, s_input);
   --rce;  if (rc    <  0)                return rce;
   s_beg = s_end = rc;
   --rce;  if (s_stp != 1)                return rce;
   /*---(complete)-----------------------*/
   return 0;
}

char         /*--: apply grammar to array ----------------[ ------ [ ------ ]-*/
ySCHED__apply      (int a_type, char *a_array)
{
   /*---(locals)-----------+-----------+-*/
   char        x_mark      = '1';
   int         x_temp      =  0;
   int         i           =  0;
   /*---(check for not)------------------*/
   if (s_not == 'y')      x_mark = '_';
   /*---(check for reverse)--------------*/
   if (s_rev == 'y') {
      x_temp = s_end;
      s_end  = s_smax - s_beg;
      s_beg  = s_smax - x_temp;
   }
   /*---(defense)------------------------*/
   if (s_beg < s_min )  s_beg = s_min;
   if (s_end > s_smax)  s_end = s_smax;
   /*---(update inverse)-----------------*/
   if (s_inv == 'y') {
      for (i = s_min ; i <  s_beg; i += s_stp)   a_array [i] = x_mark;
      for (i = s_smax; i >  s_end; i -= s_stp)   a_array [i] = x_mark;
   }
   /*---(update normal)------------------*/
   else {
      for (i = s_beg ; i <= s_end; i += s_stp)   a_array [i] = x_mark;
   }
   /*---(get in dots)--------------------*/
   for (i = 0         ; i <  s_min ; ++i)  a_array [i] = '.';
   for (i = s_smax + 1; i <= s_bmax; ++i)  a_array [i] = '.';
   a_array     [s_bmax + 1] = '\0';
   /*---(complete)-----------------------*/
   return 0;
}

char         /*--: interpret one field of grammar --------[ ------ [ ------ ]-*/
ySCHED__field_NEW  (
      /*---(parameters)-------+--------+----*/
      char       *a_input     ,        /* clear ascii text source grammar     */
      char       *a_array     ,        /* return array of values              */
      int         a_type      )        /* field type for interpretation       */
{
   return ySCHED__field (a_input, a_array, a_type);
}

char         /*--: interpret one field of grammar --------[ ------ [ ------ ]-*/
ySCHED__field      (
      /*---(parameters)-------+--------+----*/
      char       *a_input     ,        /* clear ascii text source grammar     */
      char       *a_array     ,        /* return array of values              */
      int         a_type      )        /* field type for interpretation       */
{
   /*---(locals)-------------------------*/
   char       *p           = NULL;          /* strtok result pointer          */
   char       *q           = ",";           /* strtok delimiter               */
   char       *s           = NULL;          /* strtok context                 */
   char        rc          = 0;             /* generic return code            */
   char        rce         = -10;           /* return code for errors         */
   char        x_label     [ 20];           /* field type label               */
   char       *x_error     = "((error))";   /* default array contents         */
   int         i           = 0;             /* generic iterator               */
   char        x_array     [100];           /* working return values          */
   /*---(header)-------------------------*/
   DEBUG_YSCHED yLOG_enter   (__FUNCTION__);
   /*---(defense on output array)--------*/
   DEBUG_YSCHED yLOG_point   ("*a_array"  , a_array);
   --rce;  if (a_array == NULL      ) {
      DEBUG_YSCHED yLOG_exit  (__FUNCTION__);
      return rce;
   }
   DEBUG_YSCHED yLOG_value   ("a_type"    , a_type);
   --rce;  if (a_type  >= MAX_FIELDS)  {
      DEBUG_YSCHED yLOG_exit  (__FUNCTION__);
      return rce;
   }
   --rce;  if (a_type  <  PARSE_MNS)  {
      DEBUG_YSCHED yLOG_exit  (__FUNCTION__);
      return rce;
   }
   --rce;  if (a_type  >  PARSE_YRS) {
      DEBUG_YSCHED yLOG_exit  (__FUNCTION__);
      return rce;
   }
   DEBUG_YSCHED yLOG_point   ("*a_input"  , a_input);
   --rce;  if (a_input == NULL      )  {
      /*---(check for defaulting)-----------*/
      if (a_type == PARSE_WKS || a_type == PARSE_YRS) {
         ySCHED__prep     (a_type);
         s_beg = s_min;
         s_end = s_smax;
         ySCHED__apply    (a_type, x_array);
         strcpy (a_array , x_array);
         strcpy (its.last, a_array);
         return 0;
      }
      DEBUG_YSCHED yLOG_exit  (__FUNCTION__);
      return rce;
   }
   DEBUG_YSCHED yLOG_info    ("a_input"   , a_input);
   /*---(initialize arrays)--------------*/
   strcpy (a_array , x_error);
   strcpy (its.last, a_array);
   DEBUG_YSCHED yLOG_info    ("default"   , a_array);
   for (i = 0; i < MAX_FIELD; ++i) x_array   [i]    = '_';
   DEBUG_YSCHED yLOG_info    ("initial"   , x_array);
   /*---(parse into sections)------------*/
   p = strtok_r (a_input, q, &s);
   --rce;
   while (p != NULL) {
      s_input =  p;
      DEBUG_YSCHED yLOG_info    ("s_input"   , s_input);
      s_len = strlen (s_input);
      DEBUG_YSCHED yLOG_value   ("s_len"     , s_len);
      rc = ySCHED__prep     (a_type);
      DEBUG_YSCHED yLOG_value   ("prep_rc"   , rc);
      if (rc <  0) {
         DEBUG_YSCHED yLOG_exit  (__FUNCTION__);
         return rce;
      }
      rc = ySCHED__modifier ();
      DEBUG_YSCHED yLOG_value   ("mod_rc"    , rc);
      if (rc <  0) {
         DEBUG_YSCHED yLOG_exit  (__FUNCTION__);
         return rce - 1;
      }
      rc = ySCHED__step     ();
      DEBUG_YSCHED yLOG_value   ("step_rc"   , rc);
      if (rc <  0)  {
         DEBUG_YSCHED yLOG_exit  (__FUNCTION__);
         return rce - 2;
      }
      rc = ySCHED__const    (a_type);
      DEBUG_YSCHED yLOG_value   ("const_rc"  , rc);
      --rce;  if (rc < 0) {
         DEBUG_YSCHED yLOG_exit  (__FUNCTION__);
         return rce;
      }
      if (rc == 0)  {
         rc = ySCHED__range    (a_type);
         DEBUG_YSCHED yLOG_value   ("range_rc"  , rc);
         --rce; if (rc < 0)  {
            DEBUG_YSCHED yLOG_exit  (__FUNCTION__);
            return rce;
         }
      }
      rc = ySCHED__apply    (a_type, x_array);
      DEBUG_YSCHED yLOG_value   ("apply_rc"  , rc);
      --rce;  if (rc < 0) {
         DEBUG_YSCHED yLOG_exit  (__FUNCTION__);
         return rce;
      }
      p = strtok_r (NULL, q, &s);
   }
   /*---(move to final)------------------*/
   strcpy (a_array , x_array);
   strcpy (its.last, a_array);
   DEBUG_YSCHED yLOG_info    ("final"     , a_array);
   /*---(complete)-----------------------*/
   DEBUG_YSCHED yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                            parsing                           ----===*/
/*====================------------------------------------====================*/
static void      o___PARSE___________________o (void) {;}

int        /*----: scheduling grammar value checker --------------------------*/
ySCHED__convert    (cchar *a_field, cchar *a_input, cint a_min, cint a_max)
{
   int       i        = 0;
   int       len      = 0;
   int       value    = 0;
   if (a_input == NULL) return -1;
   len   = strlen(a_input);
   if (len > 2) {
      its.status = FAILED;
      /*> yLOG_info ("focus",    a_field);                                            <* 
       *> yLOG_info ("issue",    "value too long");                                   <* 
       *> yLOG_info ("input",    a_input);                                            <* 
       *> yLOG_info ("FAILED",   "whole line was rejected");                          <*/
      return -2;
   }
   for (i = 0; i < len; ++i) {
      if (!isdigit(a_input[i])) {
         its.status = FAILED;
         /*> yLOG_info ("focus",    a_field);                                         <* 
          *> yLOG_info ("issue",    "value contains non-numeric values");             <* 
          *> yLOG_info ("input",    a_input);                                         <* 
          *> yLOG_info ("FAILED",   "whole line was rejected");                       <*/
         return -3;
      }
   }
   value = atoi(a_input);
   if (value < a_min) {
      its.status = FAILED;
      /*> yLOG_info ("focus",    a_field);                                            <* 
       *> yLOG_info ("issue",    "value is too small");                               <* 
       *> yLOG_info ("input",    a_input);                                            <* 
       *> yLOG_value("value",    value);                                              <* 
       *> yLOG_info ("WARN",     "value adjusted");                                   <*/
      return -4;
   }
   if (value > a_max) {
      its.status = FAILED;
      /*> yLOG_info ("focus",    a_field);                                            <* 
       *> yLOG_info ("issue",    "value is tool large");                              <* 
       *> yLOG_info ("input",    a_input);                                            <* 
       *> yLOG_value("value",    value);                                              <* 
       *> yLOG_info ("WARN",     "value adjusted");                                   <*/
      return -5;
   }
   return value;
}

char         /*--: field value interpreter ---------------[ ------ [ ------ ]-*/
ySCHED__field_OLD  (
      /*---(parameters)-------+--------+----*/
      char       *a_input     ,        /* clear ascii text source grammar     */
      char       *a_array     ,        /* return array of values              */
      int         a_type      )        /* field type for interpretation       */
{
   /*---(locals)-----------+-----------+-*/
   int         rc          = 0;             /* generic return code            */
   char        rce         = -10;           /* return code for errors         */
   int         i           = 0;             /* loop iterator -- generic       */
   char       *list, *step, *range;         /* layers of specification        */
   char       *s1  , *s2,   *s3;            /* strtok_r pointers              */
   char       *beg, *end;                   /* beginning and end of ranges    */
   char       *weird;                       /* marker for extra separators    */
   int         beg_val, end_val, step_inc;  /* values of beg, end, and step   */
   char        upto, cont, revs, not, invs; /* marker for open beg/end ranges */
   int         len;                         /* length of range                */
   char        x_array     [100];           /* working return values          */
   char        x_label     [ 20];           /* field type label               */
   int         x_min       =  0;            /* field type minimum value       */
   int         x_max       =  0;            /* field type maximum value       */
   char        x_value     =  1 ;
   char       *x_error     = "((error))";
   /*---(defense on output array)--------*/
   --rce;  if (a_array == NULL  )      return rce;
   strcpy (a_array , x_error);
   strcpy (its.last, a_array);
   /*---(defense on type)----------------*/
   --rce;  if (a_type  <  PARSE_MNS)   RETURN(rce);
   --rce;  if (a_type  >  PARSE_YRS)   RETURN(rce);
   /*---(get the type details)-----------*/
   strncpy (x_label, s_fields [a_type].label, 20);
   x_min           = s_fields [a_type].min;
   x_max           = s_fields [a_type].max;
   /*---(initialize arrays)--------------*/
   for (i = 0; i < MAX_FIELD; ++i) {
      a_array   [i]    = '_';
      x_array   [i]    =  0 ;
   }
   if (x_min == 1)  a_array [0] = x_array [0] = '.';  /* fix beginning        */
   a_array [x_max + 1] = x_array [x_max + 1] = '\0';  /* fix end              */
   /*> printf ("%15s, min = %2d, max = %2d, %s\n", x_label, x_min, x_max, a_array);   <*/
   if (TYPE_DYS) x_max = its.ndys;
   if (TYPE_WKS) x_max = its.nwks;
   /*---(defense)------------------------*/
   --rce;  if (a_input == NULL)  {
      if (TYPE_WKS) {
         for (i = x_min; i <= its.nwks; ++i)  a_array   [i]    = '1';
         for (i = its.nwks + 1; i <= s_fields [a_type].max; ++i)  a_array [i] = '.';
         strncpy(its.last, a_array, MAX_FIELD);
      }
      if (TYPE_YRS) {
         for (i = x_min; i <= x_max; ++i)  a_array   [i]    = '1';
         strncpy(its.last, a_array, MAX_FIELD);
      }
      return rce;
   }
   /*---(parse into sections)------------*/
   list  = strtok_r (a_input, ",", &s1);
   --rce;
   while (list != NULL) {
      /*---(pull the value/range part)---*/
      range = strtok_r (list,  "/", &s2);
      /*---(pull the step part)----------*/
      step_inc = 1;
      step     = strtok_r (NULL,  "/", &s2);
      if (step != NULL) {
         rc = ySCHED__convert ("step", step, 1, x_max);
         if (rc >= 1) step_inc = rc;
         else         return rce;
      }
      /*---(handle value modifiers)---------*/
      revs = not = upto = cont = invs = 0;
      if (range[0]                 == '~') { not  = 1; range += 1; }
      if (range[0]                 == '^') { revs = 1; range += 1; }
      if (range[0]                 == '!') { invs = 1; range += 1; }
      /*---(handle range shortcuts)---------*/
      if (range[0]                 == '<') { upto = 1; range += 1; }
      len = strlen(range);
      if (range[len - 1]           == '>') { cont = 1; range[--len] = '\0'; }
      /*---(parse out the ranges)---------------*/
      beg   = strtok_r(range, "-", &s3);
      /*---(handle null)-----------------*/
      if (beg == NULL)  RETURN (rce - 1)
         /*---(literal for all)-------------*/
      else if (len == 1 && beg[0] == '*') {
         beg_val = x_min;
         end_val = x_max;
      }
      /*---(literal for last)------------*/
      else if (len == 1 && beg[0] == 'L') {
         if      (TYPE_DYS)  beg_val = end_val = its.ndys;
         else if (TYPE_WKS)  beg_val = end_val = its.nwks;
         else                                     beg_val = end_val = x_max;
      }
      /*---(handle week/quarter refs)----*/
      else if (len == 2 && strchr("wq", beg[1]) != 0) {
         /*---(legal values)-------------*/
         if (strchr("12345L", beg[0]) == 0) RETURN (rce - 2);  /* check legal values */
         /*---(in days)------------------*/
         if (TYPE_DYS && beg[1] == 'w') {
            if (beg[0] == 'L') {
               end_val = its.ndys;
               beg_val = end_val - 6;
            } else {
               beg_val = 7 * (beg[0] - '1') + 1;
               end_val = beg_val + 6;
            }
         }
         /*---(in months)----------------*/
         else if (TYPE_MOS && beg[1] == 'q') {
            if (beg[0] == 'L')  beg_val = 10;
            else                beg_val = 3 * (beg[0] - '1') + 1;
            end_val = beg_val + 2;
         }
         /*---(can't use in field)-------*/
         else RETURN (rce - 3);
      }
      /*---(handle ----------)-----------*/
      else if (len == 1 && strchr("WE", beg[0]) != 0) {
         if (TYPE_DOW) {
            if (beg[0] == 'W') {
               beg_val = 1;
               end_val = 5;
            } else {
               beg_val = 6;
               end_val = 7;
            }
         } else RETURN (rce - 4);
      }
      /*---(handle ----------)-----------*/
      else if (len == 1 && strchr("BO", beg[0]) != 0) {
         if (TYPE_HRS) {
            switch (beg[0]) {
            case 'B' : beg_val =  8; end_val = 16; break;
            case 'O' : beg_val =  8; end_val = 16; invs = 1; break;
            }
         } else RETURN (rce - 5);
      }
      /*---(handle ----------)-----------*/
      else if (len == 2 && strstr("MoTuWeThFrSaSu", beg) != 0) {
         if      (strcmp("Mo", beg) == 0) beg_val = end_val = 1;
         else if (strcmp("Tu", beg) == 0) beg_val = end_val = 2;
         else if (strcmp("We", beg) == 0) beg_val = end_val = 3;
         else if (strcmp("Th", beg) == 0) beg_val = end_val = 4;
         else if (strcmp("Fr", beg) == 0) beg_val = end_val = 5;
         else if (strcmp("Sa", beg) == 0) beg_val = end_val = 6;
         else if (strcmp("Su", beg) == 0) beg_val = end_val = 7;
      }
      /*---(handle ----------)-----------*/
      else if (len <= 3 && strchr("abn+", range[len - 1]) != 0) {
         if (TYPE_DYS && len > 1) {
            char xmod = range[len - 1];
            int  xday = 0;
            if (len == 2 && range[0] == 'L') {
               xday = 31;
               /*> printf ("\'L\' found\n");                                          <*/
            } else {
               for (i = 0; i < len - 1; ++i) {
                  if (len == 3 && i == 0)  xday += ((range[i] - '0') * 10);
                  else                     xday +=  (range[i] - '0');
               }
            }
            if (xday > its.ndys) xday = its.ndys;
            int  my_dow = ((xday + its.fdow - 1) % 7);
            if (my_dow == 0) my_dow = 7;
            /*> printf ("ndys = %2d, day = %2d, mod = %c, mdow = %d, fdow = %d\n", its.ndys, xday, xmod, my_dow, its.fdow);   <*/
            switch (xmod) {
            case 'a' :
               if (my_dow == 7)      { xday += 1; my_dow = 1; }
               if (my_dow == 6)      { xday += 2; my_dow = 1; }
               if (xday   >  its.ndys) { xday -= 3; my_dow = 5; }
               break;
            case 'b' :
               if (my_dow == 7)      { xday -= 2; my_dow = 5; }
               if (my_dow == 6)      { xday -= 1; my_dow = 5; }
               if (xday   <= 0)      { xday += 3; my_dow = 1; }
               break;
            case '+' :
               if (my_dow == 1)      { xday -= 3; my_dow = 5; }
               else if (my_dow == 7) { xday -= 3; my_dow  = 4; }
               else if (my_dow == 6) { xday -= 2; my_dow  = 4; }
               else                  { xday -= 1; my_dow -= 1; }
               if (xday   <= 0)      { xday += 1; my_dow  = 5; }
               if (xday   <= 0)      { xday += 3; my_dow  = 1; }
               break;
            case 'n' :
               if (my_dow == 7)     { xday += 1; my_dow = 1; }
               if (my_dow == 6)     { xday -= 1; my_dow = 5; }
               if (xday   >  its.ndys) { xday -= 3; my_dow = 5; }
               if (xday   <= 0)     { xday += 3; my_dow = 1; }
               break;
            }
            beg_val = end_val = xday;
            /*> printf ("           day = %2d, mod = %c, mdow = %d, fdow = %d\n", xday, xmod, my_dow, its.fdow);   <*/
         } else RETURN (rce - 6);
      }
      /*---(handle ----------)-----------*/
      else {
         rc = ySCHED__convert("beg", beg, x_min, x_max);
         if (rc >= 0) {
            beg_val = rc;
            end_val = rc;
         } else         RETURN (rce - 7);
         if (upto == 1) {
            beg_val = x_min;
         } else if (cont == 1) {
            end_val = x_max;
         } else {
            end   = strtok_r(NULL,  "-", &s3);
            if (end != NULL) {
               rc = ySCHED__convert("end", end, beg_val, x_max);
               if (rc >= 0) end_val = rc;
               else         RETURN (rce - 8);
            }
         }
      }
      /*---(handle modifiers/exceptions)----*/
      weird    = strtok_r(NULL,  "/", &s2);
      if (weird != NULL)  RETURN (rce - 9);
      weird    = strtok_r(NULL,  "-", &s3);
      if (weird != NULL)  RETURN (rce - 10);
      if (step_inc > 1 && (step_inc > (end_val - beg_val))) RETURN (rce - 11);
      /*---(update)-----------------------------*/
      /*> printf ("beg = %2d, end = %2d, stp = %2d\n", beg_val, end_val, step_inc);   <*/
      if (not == 0)        x_value = 1;
      else                 x_value = 0;
      /*---(update reverse)---------------------*/
      if      (revs == 1) {
         if (TYPE_DYS) {
            beg_val = its.ndys - beg_val;
            end_val = its.ndys - end_val;
         } else if (TYPE_WKS) {
            beg_val = its.nwks - beg_val;
            end_val = its.nwks - end_val;
         } else {
            beg_val = x_max - beg_val;
            end_val = x_max - end_val;
         }
         for (i = beg_val; i >= end_val; i -= step_inc) {
            x_array[i] = x_value;
         }
      }
      /*---(update inverse)---------------------*/
      else if (invs == 1) {
         for (i = x_min      ; i <  beg_val; ++i)  x_array[i] = x_value;
         for (i = end_val + 1; i <= x_max  ; ++i)  x_array[i] = x_value;
      }
      /*---(update normal)----------------------*/
      else {
         for (i = beg_val; i <= end_val; i += step_inc) {
            x_array[i] = x_value;
         }
      }
      /*---(next in list)-----------------------*/
      list  = strtok_r (NULL,  ",", &s1);
   }
   /*---(pass back results)---------------------*/
   x_min           = s_fields [a_type].min;
   x_max           = s_fields [a_type].max;
   for (i = 0    ; i <= x_max; ++i) {
      if      (x_array[i] ==  1 ) a_array[i]   = '1';
      else if (x_array[i] == '.') a_array[i]   = '.';
      else                        a_array[i]   = '_';
   }
   if (TYPE_DYS)  for (i = its.ndys + 1; i <= x_max; ++i)  a_array [i] = '.';
   if (TYPE_WKS)  for (i = its.nwks + 1; i <= x_max; ++i)  a_array [i] = '.';
   a_array     [x_max + 1] = '\0';
   strncpy (its.last, a_array, MAX_FIELD);
   /*> yLOG_info (x_label,    x_array);                                               <*/
   DEBUG_YSCHED  printf ("   %-10.10s (%2d-%2d) : %-60.60s\n", x_label, x_min, x_max, x_array);
   /*---(complete)------------------------------*/
   return  0;
}

char       /*----: scheduling grammer driver ---------------------------------*/
ySCHED_parse       (tSCHED *a_sched, char *a_recd)
{
   /*---(locals)-------------------------*/
   int       len       = 0;
   char      rc        = 0;
   /*---(initialize)---------------------*/
   ySCHED__init ();
   /*---(defense)------------------------*/
   if (a_recd == NULL || a_recd[0] == '\0' || a_recd[0] == ' ') {
      ySCHED__effinit ();
      its.status = FAILED;
      return its.status;
   }
   /*---(copy it)------------------------*/
   strncpy (its.full, a_recd, MAX_RECD);
   /*---(more defenses)------------------*/
   if (its.full[0] == '#') {
      ySCHED__effinit ();
      its.status = FAILED;
      return its.status;
   }
   /*---(process speciality)-------------*/
   /*> yLOG_break  ();                                                                <*/
   /*> yLOG_info   ("raw text", its.full);                                            <*/
   if (strncmp(its.full, ".holiday"  ,  8) == 0) {
      /*> yLOG_info   ("control" , "identified file scope holiday entry");            <*/
      ySCHED__globalize (a_recd, 0);
      its.status = SKIPPING;
      return its.status;
   }
   if (strncmp(its.full, ".vacation"  ,  9) == 0) {
      /*> yLOG_info   ("control" , "identified file scope vacation entry");           <*/
      ySCHED__globalize (a_recd, 0);
      its.status = SKIPPING;
      return its.status;
   }
   if (strncmp(its.full, ".blackout",   9) == 0) {
      yLOG_info   ("control" , "identified file scope blackout entry");
      ySCHED__globalize (a_recd, 0);
      its.status = SKIPPING;
      return its.status;
   }
   if (strncmp(its.full, ".validity",   9) == 0) {
      yLOG_info   ("control" , "identified file scope validity entry");
      ySCHED__globalize (a_recd, 0);
      its.status = SKIPPING;
      return its.status;
   }
   if (strncmp(its.full, ".effective", 10) == 0) {
      yLOG_info   ("control" , "identified effective range entry");
      ySCHED__effective (a_recd, 0);
      its.status = SKIPPING;
      return its.status;
   }
   if (its.full[0] == '.') {
      yLOG_info   ("future" , "record is a control for future use");
      its.status = FAILED;
      return its.status;
   }
   len = strlen(its.full);
   if (len <=  8) {
      yLOG_info   ("FAILURE", "record too short ( < 12 )");
      its.status = FAILED;
      return its.status;
   }
   if (len >  100) {
      yLOG_info   ("FAILURE", "record too long ( > 100 )");
      its.status = FAILED;
      return its.status;
   }
   /*---(break it down)------------------*/
   strncpy(its.recd, its.full, MAX_RECD);
   its.xmin  = strtok (its.recd  , " ");
   its.xhrs  = strtok (NULL      , " ");
   its.xdys  = strtok (NULL      , " ");
   its.xmos  = strtok (NULL      , " ");
   its.xdow  = strtok (NULL      , " ");
   its.xwks  = strtok (NULL      , " ");
   its.xyrs  = strtok (NULL      , " ");
   /*> its.xdur  = strtok(NULL      , " ");                                           <*/
   /*---(parse)--------------------------*/
   rc = 0;
   rc = ySCHED__field (its.xmin, sched.min, PARSE_MNS);
   rc = ySCHED__field (its.xhrs, sched.hrs, PARSE_HRS);
   rc = ySCHED__field (its.xdys, sched.dys, PARSE_DYS);
   rc = ySCHED__field (its.xmos, sched.mos, PARSE_MOS);
   rc = ySCHED__field (its.xdow, sched.dow, PARSE_DOW);
   rc = ySCHED__field (its.xwks, sched.wks, PARSE_WKS);
   rc = ySCHED__field (its.xyrs, sched.yrs, PARSE_YRS);
   /*> if (rc != 0) {                                                                 <* 
    *>    yLOG_info   ("FAILURE", "could not fully parse the record");                <* 
    *>    return FAILED;                                                              <* 
    *> }                                                                              <*/
   strncpy(sched.eff, effective, 500);
   yLOG_info  ("effective", effout);
   /*> sched.dur = ySCHED_duration (its.xdur);                                        <* 
    *> yLOG_value ("duration" , sched.dur);                                           <*/
   /*---(copy back)----------------------*/
   if (a_sched != NULL) {
      strncpy (a_sched->min, sched.min, 100);
      strncpy (a_sched->hrs, sched.hrs, 100);
      strncpy (a_sched->dys, sched.dys, 100);
      strncpy (a_sched->mos, sched.mos, 100);
      strncpy (a_sched->dow, sched.dow, 100);
      strncpy (a_sched->wks, sched.wks, 100);
      strncpy (a_sched->yrs, sched.yrs, 100);
      strncpy (a_sched->eff, sched.eff, 500);
      /*> a_sched->dur = sched.dur;                                                   <*/
   }
   /*---(complete)-----------------------*/
   return 0;
}



/*============================----end-of-source---============================*/
