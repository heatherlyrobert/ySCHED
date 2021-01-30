/*============================----beg-of-source---============================*/
#include   "ySCHED.h"
#include   "ySCHED_priv.h"


tLOCAL    mySCHED;
tSCHED    g_sched;
int       offset = 0;

char      ySCHED_ver   [100];


/*> else if (len == 2 && strchr("wq", beg[1]) != 0) {                                   <* 
 *>    /+---(legal values)-------------+/                                               <* 
 *>    if (strchr("12345L", beg[0]) == 0) RETURN (rce - 2);  /+ check legal values +/   <* 
 *>    /+---(in days)------------------+/                                               <* 
 *>    if (TYPE_DYS && beg[1] == 'w') {                                                 <* 
 *>       if (beg[0] == 'L') {                                                          <* 
 *>          end_val = mySCHED.s_dim;                                                        <* 
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


char  s_raw       [LEN_RECD]  = "";
char  s_section   [LEN_HUND]  = "";
char  s_input     [LEN_HUND]  = "";
char *s_ptr       = NULL;
int   s_len       =    0;

char  s_not       =  '-';
char  s_rev       =  '-';
char  s_inv       =  '-';
char  s_flp       =  '-';

int   s_stp       =    1;
int   s_beg       =    0;
int   s_end       =    0;

char  s_type      =   -1;
char  s_label     [LEN_LABEL] = "";
int   s_min       =    0;
int   s_tmax      =    0;
int   s_max       =    0;



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
   strncpy (t, "[tcc built  ]", 15);
#elif  __GNUC__  > 0
   strncpy (t, "[gnu gcc    ]", 15);
#else
   strncpy (t, "[unknown    ]", 15);
#endif
   snprintf (ySCHED_ver, 100, "%s   %s : %s", t, P_VERNUM, P_VERTXT);
   return ySCHED_ver;
}

char       /*----: initialize key globals ------------------------------------*/
ySCHED__init       (void)
{
   /*---(locals)-------------------------*/
   int       i         = 0;
   /*---(header)-------------------------*/
   DEBUG_YSCHED  yLOG_enter   (__FUNCTION__);
   /*---(overall)------------------------*/
   mySCHED.status  = SUCCESS;
   /*---(working)------------------------*/
   mySCHED.full[0] = '\0';
   mySCHED.recd[0] = '\0';
   mySCHED.xmin    = NULL;
   mySCHED.xhrs    = NULL;
   mySCHED.xdys    = NULL;
   mySCHED.xmos    = NULL;
   mySCHED.xdow    = NULL;
   mySCHED.xwks    = NULL;
   mySCHED.xdur    = NULL;
   /*---(schedule)-----------------------*/
   for (i = 0; i < LEN_HUND; ++i) {
      g_sched.min [i]    = '_';
      g_sched.hrs [i]    = '_';
      g_sched.dys [i]    = '_';
      g_sched.mos [i]    = '_';
      g_sched.dow [i]    = '_';
      g_sched.wks [i]    = '_';
   }
   /*---(effective)----------------------*/
   for (i = 0; i < 500      ; ++i) {
      g_sched.eff [i]    = '_';
   }
   /*---(duration)-----------------------*/
   /*> g_sched.dur   = 0;                                                             <*/
   /*---(debug)--------------------------*/
   mySCHED.debug   = '-';
   /*---(complete)-----------------------*/
   DEBUG_YSCHED  yLOG_exit    (__FUNCTION__);
   return 0;
}

char       /*----: reset the globals and effective dates ---------------------*/
ySCHED_reset       (void)
{
   /*---(locals)-------------------------*/
   int       i         = 0;            /* loop iterator -- day                */
   /*---(initialize)---------------------*/
   for (i = 0; i < 500; ++i) {
      mySCHED.effective[i] = '1';
   }
   ySCHED__effout();
   strncpy (mySCHED.global, mySCHED.effective, 500);
   strncpy (mySCHED.gloout, mySCHED.effout   , 100);
   /*---(initialize)---------------------*/
   return 0;
}



/*====================------------------------------------====================*/
/*===----                         load and save                        ----===*/
/*====================------------------------------------====================*/
static void      o___STORE___________________o (void) {;}

char       /*----: save the results in an alternate location -----------------*/
ySCHED_save        (tSCHED *a_sched)
{
   /*---(header)-------------------------*/
   DEBUG_YSCHED  yLOG_senter  (__FUNCTION__);
   if (mySCHED.status == FAILED) {
      DEBUG_YSCHED  yLOG_sexitr  (__FUNCTION__, FAILED);
      return FAILED;
   }
   strncpy(a_sched->min, g_sched.min, 100);
   strncpy(a_sched->hrs, g_sched.hrs, 100);
   strncpy(a_sched->dys, g_sched.dys, 100);
   strncpy(a_sched->mos, g_sched.mos, 100);
   strncpy(a_sched->dow, g_sched.dow, 100);
   strncpy(a_sched->wks, g_sched.wks, 100);
   strncpy(a_sched->yrs, g_sched.yrs, 100);
   strncpy(a_sched->eff, g_sched.eff, 500);
   /*> a_sched->dur = g_sched.dur;                                                    <*/
   DEBUG_YSCHED  yLOG_sexit   (__FUNCTION__);
   return 0;
}

char       /*----: load the interpretation from an alternate location --------*/
ySCHED_load        (tSCHED *a_sched)
{
   /*---(header)-------------------------*/
   DEBUG_YSCHED  yLOG_senter  (__FUNCTION__);
   strncpy (g_sched.min, a_sched->min, 100);
   strncpy (g_sched.hrs, a_sched->hrs, 100);
   strncpy (g_sched.dys, a_sched->dys, 100);
   strncpy (g_sched.mos, a_sched->mos, 100);
   strncpy (g_sched.dow, a_sched->dow, 100);
   strncpy (g_sched.wks, a_sched->wks, 100);
   strncpy (g_sched.yrs, a_sched->yrs, 100);
   strncpy (g_sched.eff, a_sched->eff, 500);
   /*> g_sched.dur = a_sched->dur;                                                    <*/
   DEBUG_YSCHED  yLOG_sexit   (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                            parsing                           ----===*/
/*====================------------------------------------====================*/
static void      o___PARSE___________________o (void) {;}

char
ysched__special         (char *a_recd)
{
   if (a_recd [0] != '.')   return 0;
   if (strncmp (a_recd, ".holiday"  ,  8) == 0) {
      DEBUG_YSCHED  yLOG_info   ("control" , "identified file scope holiday entry");
      ySCHED__globalize (a_recd, 0);
      mySCHED.status = SKIPPING;
      return mySCHED.status;
   }
   if (strncmp (a_recd, ".vacation"  ,  9) == 0) {
      DEBUG_YSCHED  yLOG_info   ("control" , "identified file scope vacation entry");
      ySCHED__globalize (a_recd, 0);
      mySCHED.status = SKIPPING;
      return mySCHED.status;
   }
   if (strncmp (a_recd, ".blackout",   9) == 0) {
      DEBUG_YSCHED  yLOG_info   ("control" , "identified file scope blackout entry");
      ySCHED__globalize (a_recd, 0);
      mySCHED.status = SKIPPING;
      return mySCHED.status;
   }
   if (strncmp (a_recd, ".validity",   9) == 0) {
      DEBUG_YSCHED  yLOG_info   ("control" , "identified file scope validity entry");
      ySCHED__globalize (a_recd, 0);
      mySCHED.status = SKIPPING;
      return mySCHED.status;
   }
   if (strncmp (a_recd, ".effective", 10) == 0) {
      DEBUG_YSCHED  yLOG_info   ("control" , "identified effective range entry");
      ySCHED__effective (a_recd, 0);
      mySCHED.status = SKIPPING;
      return mySCHED.status;
   }
   DEBUG_YSCHED  yLOG_info   ("future" , "record is a control for future use");
   mySCHED.status = FAILED;
   return mySCHED.status;
}

char       /*----: scheduling grammer driver ---------------------------------*/
ySCHED_parse       (tSCHED *a_sched, cchar *a_recd)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         len         =    0;
   char        rc          =    0;
   char        x_recd      [LEN_RECD]  = "";
   /*---(header)-------------------------*/
   DEBUG_YSCHED  yLOG_enter   (__FUNCTION__);
   /*---(initialize)---------------------*/
   ySCHED__init ();
   ysched_reseterror ();
   /*---(defense)------------------------*/
   if (a_recd == NULL || a_recd[0] == '\0' || a_recd[0] == ' ' || a_recd[0] == '#') {
      ySCHED__effinit ();
      mySCHED.status = FAILED;
      return mySCHED.status;
   }
   /*---(copy it)------------------------*/
   strncpy (s_raw       , a_recd, LEN_RECD);
   strncpy (mySCHED.full, a_recd, LEN_RECD);
   strncpy (x_recd      , a_recd, LEN_RECD);
   /*---(process speciality)-------------*/
   --rce;  if (mySCHED.full[0] == '.') {
      rc = ysched__special (mySCHED.full);
      if (rc < 0)  return rce;
   }
   /*---(general defenses)---------------*/
   len = strlen(mySCHED.full);
   if (len <=  8) {
      DEBUG_YSCHED  yLOG_info   ("FAILURE", "record too short ( < 12 )");
      mySCHED.status = FAILED;
      return mySCHED.status;
   }
   if (len >  100) {
      DEBUG_YSCHED  yLOG_info   ("FAILURE", "record too long ( > 100 )");
      mySCHED.status = FAILED;
      return mySCHED.status;
   }
   /*---(break it down)------------------*/
   strncpy (mySCHED.recd, mySCHED.full, LEN_RECD);
   mySCHED.xmin  = strtok (mySCHED.recd  , " ");
   mySCHED.xhrs  = strtok (NULL          , " ");
   mySCHED.xdys  = strtok (NULL          , " ");
   mySCHED.xmos  = strtok (NULL          , " ");
   mySCHED.xdow  = strtok (NULL          , " ");
   mySCHED.xwks  = strtok (NULL          , " ");
   mySCHED.xyrs  = strtok (NULL          , " ");
   /*---(parse)--------------------------*/
   rc = 0;
   if (rc >= 0)  rc = ysched_field  (mySCHED.xmin, g_sched.min, PARSE_MNS);
   if (rc >= 0)  rc = ysched_field  (mySCHED.xhrs, g_sched.hrs, PARSE_HRS);
   if (rc >= 0)  rc = ysched_field  (mySCHED.xdys, g_sched.dys, PARSE_DYS);
   if (rc >= 0)  rc = ysched_field  (mySCHED.xmos, g_sched.mos, PARSE_MOS);
   if (rc >= 0)  rc = ysched_field  (mySCHED.xdow, g_sched.dow, PARSE_DOW);
   if (rc >= 0)  rc = ysched_field  (mySCHED.xwks, g_sched.wks, PARSE_WKS);
   if (rc >= 0)  rc = ysched_field  (mySCHED.xyrs, g_sched.yrs, PARSE_YRS);
   ysched_fancify ();
   --rce;  if (rc < 0) {
      DEBUG_YSCHED  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   strncpy (g_sched.eff, mySCHED.effective, 500);
   DEBUG_YSCHED  yLOG_info  ("effective", mySCHED.effout);
   /*---(copy back)----------------------*/
   if (a_sched != NULL) {
      strncpy (a_sched->min, g_sched.min, 100);
      strncpy (a_sched->hrs, g_sched.hrs, 100);
      strncpy (a_sched->dys, g_sched.dys, 100);
      strncpy (a_sched->mos, g_sched.mos, 100);
      strncpy (a_sched->dow, g_sched.dow, 100);
      strncpy (a_sched->wks, g_sched.wks, 100);
      strncpy (a_sched->yrs, g_sched.yrs, 100);
      strncpy (a_sched->eff, g_sched.eff, 500);
   }
   /*---(complete)-----------------------*/
   DEBUG_YSCHED  yLOG_exit    (__FUNCTION__);
   return 0;
}



/*============================----end-of-source---============================*/
