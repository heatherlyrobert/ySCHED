/*============================----beg-of-source---============================*/
#include   "ySCHED.h"
#include   "ySCHED_priv.h"


tLOCAL    mySCHED;
tSCHED    sched;
int       offset = 0;

char      ySCHED_ver   [500];


/*> else if (len == 2 && strchr("wq", beg[1]) != 0) {                                   <* 
 *>    /+---(legal values)-------------+/                                               <* 
 *>    if (strchr("12345L", beg[0]) == 0) RETURN (rce - 2);  /+ check legal values +/   <* 
 *>    /+---(in days)------------------+/                                               <* 
 *>    if (TYPE_DYS && beg[1] == 'w') {                                                 <* 
 *>       if (beg[0] == 'L') {                                                          <* 
 *>          end_val = mySCHED.ndys;                                                        <* 
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
   sched.dur   = 0;
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
   strncpy(a_sched->min, sched.min, 100);
   strncpy(a_sched->hrs, sched.hrs, 100);
   strncpy(a_sched->dys, sched.dys, 100);
   strncpy(a_sched->mos, sched.mos, 100);
   strncpy(a_sched->dow, sched.dow, 100);
   strncpy(a_sched->wks, sched.wks, 100);
   strncpy(a_sched->yrs, sched.yrs, 100);
   strncpy(a_sched->eff, sched.eff, 500);
   a_sched->dur = sched.dur;
   DEBUG_YSCHED  yLOG_sexit   (__FUNCTION__);
   return 0;
}

char       /*----: load the interpretation from an alternate location --------*/
ySCHED_load        (tSCHED *a_sched)
{
   /*---(header)-------------------------*/
   DEBUG_YSCHED  yLOG_senter  (__FUNCTION__);
   strncpy (sched.min, a_sched->min, 100);
   strncpy (sched.hrs, a_sched->hrs, 100);
   strncpy (sched.dys, a_sched->dys, 100);
   strncpy (sched.mos, a_sched->mos, 100);
   strncpy (sched.dow, a_sched->dow, 100);
   strncpy (sched.wks, a_sched->wks, 100);
   strncpy (sched.yrs, a_sched->yrs, 100);
   strncpy (sched.eff, a_sched->eff, 500);
   sched.dur = a_sched->dur;
   DEBUG_YSCHED  yLOG_sexit   (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                            parsing                           ----===*/
/*====================------------------------------------====================*/
static void      o___PARSE___________________o (void) {;}

int        /*----: scheduling grammar value checker --------------------------*/
ysched__convert    (cchar *a_field, cchar *a_input, cint a_min, cint a_max)
{
   int       i        = 0;
   int       len      = 0;
   int       value    = 0;
   if (a_input == NULL) return -1;
   len   = strlen(a_input);
   if (len > 2) {
      mySCHED.status = FAILED;
      /*> yLOG_info ("focus",    a_field);                                            <* 
       *> yLOG_info ("issue",    "value too long");                                   <* 
       *> yLOG_info ("input",    a_input);                                            <* 
       *> yLOG_info ("FAILED",   "whole line was rejected");                          <*/
      return -2;
   }
   for (i = 0; i < len; ++i) {
      if (!isdigit(a_input[i])) {
         mySCHED.status = FAILED;
         /*> yLOG_info ("focus",    a_field);                                         <* 
          *> yLOG_info ("issue",    "value contains non-numeric values");             <* 
          *> yLOG_info ("input",    a_input);                                         <* 
          *> yLOG_info ("FAILED",   "whole line was rejected");                       <*/
         return -3;
      }
   }
   value = atoi(a_input);
   if (value < a_min) {
      mySCHED.status = FAILED;
      /*> yLOG_info ("focus",    a_field);                                            <* 
       *> yLOG_info ("issue",    "value is too small");                               <* 
       *> yLOG_info ("input",    a_input);                                            <* 
       *> yLOG_value("value",    value);                                              <* 
       *> yLOG_info ("WARN",     "value adjusted");                                   <*/
      return -4;
   }
   if (value > a_max) {
      mySCHED.status = FAILED;
      /*> yLOG_info ("focus",    a_field);                                            <* 
       *> yLOG_info ("issue",    "value is tool large");                              <* 
       *> yLOG_info ("input",    a_input);                                            <* 
       *> yLOG_value("value",    value);                                              <* 
       *> yLOG_info ("WARN",     "value adjusted");                                   <*/
      return -5;
   }
   return value;
}

char       /*----: scheduling grammer driver ---------------------------------*/
ySCHED_parse       (tSCHED *a_sched, char *a_recd)
{
   /*---(locals)-------------------------*/
   int       len       = 0;
   char      rc        = 0;
   /*---(header)-------------------------*/
   DEBUG_YSCHED  yLOG_enter   (__FUNCTION__);
   /*---(initialize)---------------------*/
   ySCHED__init ();
   /*---(defense)------------------------*/
   if (a_recd == NULL || a_recd[0] == '\0' || a_recd[0] == ' ') {
      ySCHED__effinit ();
      mySCHED.status = FAILED;
      return mySCHED.status;
   }
   /*---(copy it)------------------------*/
   strncpy (mySCHED.full, a_recd, MAX_RECD);
   /*---(more defenses)------------------*/
   if (mySCHED.full[0] == '#') {
      ySCHED__effinit ();
      mySCHED.status = FAILED;
      return mySCHED.status;
   }
   /*---(process speciality)-------------*/
   /*> yLOG_break  ();                                                                <*/
   /*> yLOG_info   ("raw text", mySCHED.full);                                            <*/
   if (strncmp (mySCHED.full, ".holiday"  ,  8) == 0) {
      /*> yLOG_info   ("control" , "identified file scope holiday entry");            <*/
      ySCHED__globalize (a_recd, 0);
      mySCHED.status = SKIPPING;
      return mySCHED.status;
   }
   if (strncmp (mySCHED.full, ".vacation"  ,  9) == 0) {
      DEBUG_YSCHED  yLOG_info   ("control" , "identified file scope vacation entry");
      ySCHED__globalize (a_recd, 0);
      mySCHED.status = SKIPPING;
      return mySCHED.status;
   }
   if (strncmp (mySCHED.full, ".blackout",   9) == 0) {
      DEBUG_YSCHED  yLOG_info   ("control" , "identified file scope blackout entry");
      ySCHED__globalize (a_recd, 0);
      mySCHED.status = SKIPPING;
      return mySCHED.status;
   }
   if (strncmp (mySCHED.full, ".validity",   9) == 0) {
      DEBUG_YSCHED  yLOG_info   ("control" , "identified file scope validity entry");
      ySCHED__globalize (a_recd, 0);
      mySCHED.status = SKIPPING;
      return mySCHED.status;
   }
   if (strncmp (mySCHED.full, ".effective", 10) == 0) {
      DEBUG_YSCHED  yLOG_info   ("control" , "identified effective range entry");
      ySCHED__effective (a_recd, 0);
      mySCHED.status = SKIPPING;
      return mySCHED.status;
   }
   if (mySCHED.full[0] == '.') {
      DEBUG_YSCHED  yLOG_info   ("future" , "record is a control for future use");
      mySCHED.status = FAILED;
      return mySCHED.status;
   }
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
   strncpy(mySCHED.recd, mySCHED.full, MAX_RECD);
   mySCHED.xmin  = strtok (mySCHED.recd  , " ");
   mySCHED.xhrs  = strtok (NULL          , " ");
   mySCHED.xdys  = strtok (NULL          , " ");
   mySCHED.xmos  = strtok (NULL          , " ");
   mySCHED.xdow  = strtok (NULL          , " ");
   mySCHED.xwks  = strtok (NULL          , " ");
   mySCHED.xyrs  = strtok (NULL          , " ");
   mySCHED.xdur  = strtok (NULL          , " ");
   /*---(parse)--------------------------*/
   rc = 0;
   rc = ysched__field (mySCHED.xmin, sched.min, PARSE_MNS);
   rc = ysched__field (mySCHED.xhrs, sched.hrs, PARSE_HRS);
   rc = ysched__field (mySCHED.xdys, sched.dys, PARSE_DYS);
   rc = ysched__field (mySCHED.xmos, sched.mos, PARSE_MOS);
   rc = ysched__field (mySCHED.xdow, sched.dow, PARSE_DOW);
   rc = ysched__field (mySCHED.xwks, sched.wks, PARSE_WKS);
   rc = ysched__field (mySCHED.xyrs, sched.yrs, PARSE_YRS);
   if (mySCHED.xdur != NULL) {
      sched.dur = ysched_duration (mySCHED.xdur);
      DEBUG_YSCHED  yLOG_value ("duration" , sched.dur);
   }
   /*> if (rc != 0) {                                                                 <* 
    *>    yLOG_info   ("FAILURE", "could not fully parse the record");                <* 
    *>    return FAILED;                                                              <* 
    *> }                                                                              <*/
   strncpy (sched.eff, mySCHED.effective, 500);
   DEBUG_YSCHED  yLOG_info  ("effective", mySCHED.effout);
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
      a_sched->dur = sched.dur;
   }
   /*---(complete)-----------------------*/
   DEBUG_YSCHED  yLOG_exit    (__FUNCTION__);
   return 0;
}



/*============================----end-of-source---============================*/
