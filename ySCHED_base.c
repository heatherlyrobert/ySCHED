/*============================----beg-of-source---============================*/
#include   "ySCHED.h"
#include   "ySCHED_priv.h"


tLOCAL    mySCHED;
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
   mySCHED.full  [0] = '\0';
   mySCHED.x_raw [0] = '\0';
   mySCHED.x_min    = NULL;
   mySCHED.x_hrs    = NULL;
   mySCHED.x_dys    = NULL;
   mySCHED.x_mos    = NULL;
   mySCHED.x_dow    = NULL;
   mySCHED.x_wks    = NULL;
   /*---(schedule)-----------------------*/
   /*> for (i = 0; i < LEN_HUND; ++i) {                                               <* 
    *>    g_curr.min [i]    = '_';                                                   <* 
    *>    g_curr.hrs [i]    = '_';                                                   <* 
    *>    g_curr.dys [i]    = '_';                                                   <* 
    *>    g_curr.mos [i]    = '_';                                                   <* 
    *>    g_curr.dow [i]    = '_';                                                   <* 
    *>    g_curr.wks [i]    = '_';                                                   <* 
    *> }                                                                              <*/
   /*---(effective)----------------------*/
   /*> strlcpy (g_curr.beg, "00.01.01", LEN_TERSE);                                  <* 
    *> strlcpy (g_curr.end, "50.01.01", LEN_TERSE);                                  <*/
   /*> for (i = 0; i < LEN_VALID; ++i) {                                              <* 
    *>    g_curr.valid [i] = '_';                                                    <* 
    *> }                                                                              <*/
   /*---(duration)-----------------------*/
   /*> g_curr.dur   = 0;                                                             <*/
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
   /*> for (i = 0; i < LEN_VALID; ++i) {                                              <* 
    *>    mySCHED.valid [i] = '1';                                                    <* 
    *> }                                                                              <*/
   /*> ysched_valid__out();                                                           <*/
   /*---(initialize)---------------------*/
   return 0;
}



/*====================------------------------------------====================*/
/*===----                         load and save                        ----===*/
/*====================------------------------------------====================*/
static void      o___STORE___________________o (void) {;}

/*> char       /+----: save the results in an alternate location -----------------+/   <* 
 *> ySCHED_save        (tSCHED *a_sched)                                               <* 
 *> {                                                                                  <* 
 *>    /+---(header)-------------------------+/                                        <* 
 *>    DEBUG_YSCHED  yLOG_senter  (__FUNCTION__);                                      <* 
 *>    if (mySCHED.status == FAILED) {                                                 <* 
 *>       DEBUG_YSCHED  yLOG_sexitr  (__FUNCTION__, FAILED);                           <* 
 *>       return FAILED;                                                               <* 
 *>    }                                                                               <* 
 *>    /+---(main)---------------------------+/                                        <* 
 *>    a_sched->epoch = g_curr.epoch;                                                 <* 
 *>    strncpy (a_sched->raw  , g_curr.raw  , LEN_HUND);                              <* 
 *>    /+---(details)------------------------+/                                        <* 
 *>    strncpy (a_sched->min  , g_curr.min  , LEN_LONG);                              <* 
 *>    strncpy (a_sched->hrs  , g_curr.hrs  , LEN_TITLE);                             <* 
 *>    strncpy (a_sched->dys  , g_curr.dys  , LEN_DESC);                              <* 
 *>    strncpy (a_sched->mos  , g_curr.mos  , LEN_LABEL);                             <* 
 *>    strncpy (a_sched->dow  , g_curr.dow  , LEN_TERSE);                             <* 
 *>    strncpy (a_sched->wks  , g_curr.wks  , LEN_LONG);                              <* 
 *>    strncpy (a_sched->yrs  , g_curr.yrs  , LEN_LONG);                              <* 
 *>    /+---(valid)--------------------------+/                                        <* 
 *>    strncpy (a_sched->beg  , g_curr.beg  , LEN_TERSE);                             <* 
 *>    strncpy (a_sched->end  , g_curr.end  , LEN_TERSE);                             <* 
 *>    strncpy (a_sched->valid, g_curr.valid, LEN_VALID);                             <* 
 *>    /+---(complete)-----------------------+/                                        <* 
 *>    DEBUG_YSCHED  yLOG_sexit   (__FUNCTION__);                                      <* 
 *>    return 0;                                                                       <* 
 *> }                                                                                  <*/

/*> char       /+----: load the interpretation from an alternate location --------+/   <* 
 *> ySCHED_load        (tSCHED *a_sched)                                               <* 
 *> {                                                                                  <* 
 *>    /+---(header)-------------------------+/                                        <* 
 *>    DEBUG_YSCHED  yLOG_senter  (__FUNCTION__);                                      <* 
 *>    /+---(main)---------------------------+/                                        <* 
 *>    g_curr.epoch = a_sched->epoch;                                                 <* 
 *>    strncpy (g_curr.raw  , a_sched->raw  , LEN_HUND);                              <* 
 *>    /+---(details)------------------------+/                                        <* 
 *>    strncpy (g_curr.min  , a_sched->min  , LEN_LONG);                              <* 
 *>    strncpy (g_curr.hrs  , a_sched->hrs  , LEN_TITLE);                             <* 
 *>    strncpy (g_curr.dys  , a_sched->dys  , LEN_DESC);                              <* 
 *>    strncpy (g_curr.mos  , a_sched->mos  , LEN_LABEL);                             <* 
 *>    strncpy (g_curr.dow  , a_sched->dow  , LEN_TERSE);                             <* 
 *>    strncpy (g_curr.wks  , a_sched->wks  , LEN_LONG);                              <* 
 *>    strncpy (g_curr.yrs  , a_sched->yrs  , LEN_LONG);                              <* 
 *>    /+---(valid)--------------------------+/                                        <* 
 *>    strncpy (g_curr.beg  , a_sched->beg  , LEN_VALID);                             <* 
 *>    strncpy (g_curr.end  , a_sched->end  , LEN_VALID);                             <* 
 *>    strncpy (g_curr.valid, a_sched->valid, LEN_VALID);                             <* 
 *>    /+---(complete)-----------------------+/                                        <* 
 *>    DEBUG_YSCHED  yLOG_sexit   (__FUNCTION__);                                      <* 
 *>    return 0;                                                                       <* 
 *> }                                                                                  <*/




/*============================----end-of-source---============================*/
