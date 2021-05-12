/*============================----beg-of-source---============================*/
#include   "ySCHED.h"
#include   "ySCHED_priv.h"



tSCHED     g_null     = {
   255, "/", 1302854400, 11, 4, 15, -1, -1, 0,
   "////////////////////////////////////////////////////////////",
   "////////////////////////",
   "//////////////////////////////",
   "////////////",
   "///////",
   "/////////////////////////////////////////////////////",
   "///////////////////////////////////////////////////",
   "//.//.//", "//.//.//", '·'
};



tSCHED     *g_head      = NULL;   /* head entry pointer                  */
tSCHED     *g_tail      = NULL;   /* tail entry pointer                  */
tSCHED     *g_curr      = &g_null;
int         g_count     =    0;   /* number of entries                   */
int         g_max       =    0;
static      char        s_print     [LEN_RECD] = "";




/*> char       /+----: save the results in an alternate location -----------------+/   <* 
 *> ysched_parse__saveback  (tSCHED *a_sched)                                          <* 
 *> {                                                                                  <* 
 *>    /+---(header)-------------------------+/                                        <* 
 *>    DEBUG_YSCHED  yLOG_senter  (__FUNCTION__);                                      <* 
 *>    /+---(main)---------------------------+/                                        <* 
 *>    strncpy (a_sched->raw, g_curr.raw, LEN_HUND);                                  <* 
 *>    /+---(details)------------------------+/                                        <* 
 *>    a_sched->epoch = g_curr.epoch;                                                 <* 
 *>    strncpy (a_sched->min, g_curr.min, LEN_LONG);                                  <* 
 *>    strncpy (a_sched->hrs, g_curr.hrs, LEN_TITLE);                                 <* 
 *>    strncpy (a_sched->dys, g_curr.dys, LEN_DESC);                                  <* 
 *>    strncpy (a_sched->mos, g_curr.mos, LEN_LABEL);                                 <* 
 *>    strncpy (a_sched->dow, g_curr.dow, LEN_TERSE);                                 <* 
 *>    strncpy (a_sched->wks, g_curr.wks, LEN_LONG);                                  <* 
 *>    strncpy (a_sched->yrs, g_curr.yrs, LEN_LONG);                                  <* 
 *>    /+---(valid)--------------------------+/                                        <* 
 *>    strncpy (a_sched->beg, g_curr.beg, LEN_TERSE);                                 <* 
 *>    strncpy (a_sched->end, g_curr.end, LEN_TERSE);                                 <* 
 *>    a_sched->valid = g_curr.valid;                                                 <* 
 *>    /+---(complete)-----------------------+/                                        <* 
 *>    DEBUG_YSCHED  yLOG_sexit   (__FUNCTION__);                                      <* 
 *>    return 0;                                                                       <* 
 *> }                                                                                  <*/



/*====================------------------------------------====================*/
/*===----                     supporting functions                     ----===*/
/*====================------------------------------------====================*/
static void  o___SUPPORT_________o () { return; }

char
ysched_parse__wipe      (tSCHED *a_sched)
{
   /*---(defense)--------------*/
   if (a_sched == NULL)  return -1;
   /*---(main)---------------------------*/
   a_sched->seq    =  0;
   strncpy (a_sched->raw, STR_EDOTS  , LEN_HUND);
   /*---(date)---------------------------*/
   a_sched->epoch  = -1;
   a_sched->year   = -1;
   a_sched->month  = -1;
   a_sched->day    = -1;
   /*---(last test)----------------------*/
   a_sched->hour   = -1;
   a_sched->minute = -1;
   a_sched->result =  0;
   /*---(details)------------------------*/
   strncpy (a_sched->min, STR_EDOTS  , 60);
   strncpy (a_sched->hrs, STR_EDOTS  , 24);
   strncpy (a_sched->dys, STR_EDOTS  , 30);
   strncpy (a_sched->mos, STR_EDOTS  , 12);
   strncpy (a_sched->dow, STR_EDOTS  , 7);
   strncpy (a_sched->wks, STR_EDOTS  , 52);
   strncpy (a_sched->yrs, STR_EDOTS  , 50);
   /*---(valid)--------------------------*/
   strncpy (a_sched->beg, "00.01.01" , LEN_TERSE);
   strncpy (a_sched->end, "50.01.01" , LEN_TERSE);
   a_sched->valid  = '·';
   /*---(complete)-------------*/
   return 1;
}

char*
ysched_parse__memory    (tSCHED *a_sched)
{
   int         n           =    0;
   strlcpy (s_print, "[_.____.___._______.___.__]", LEN_RECD);
   ++n;  if (a_sched->raw [0] != '·')             s_print [n] = 'X';
   ++n;
   ++n;  if (a_sched->epoch   >= 0)               s_print [n] = 'X';
   ++n;  if (a_sched->year    >= 0)               s_print [n] = 'X';
   ++n;  if (a_sched->month   >= 0)               s_print [n] = 'X';
   ++n;  if (a_sched->day     >= 0)               s_print [n] = 'X';
   ++n;
   ++n;  if (a_sched->hour    >= 0)               s_print [n] = 'X';
   ++n;  if (a_sched->minute  >= 0)               s_print [n] = 'X';
   ++n;  if (a_sched->result  >  0)               s_print [n] = 'X';
   ++n;
   ++n;  if (a_sched->min [0] != '·')             s_print [n] = 'X';
   ++n;  if (a_sched->hrs [0] != '·')             s_print [n] = 'X';
   ++n;  if (a_sched->dys [0] != '·')             s_print [n] = 'X';
   ++n;  if (a_sched->mos [0] != '·')             s_print [n] = 'X';
   ++n;  if (a_sched->dow [0] != '·')             s_print [n] = 'X';
   ++n;  if (a_sched->wks [0] != '·')             s_print [n] = 'X';
   ++n;  if (a_sched->yrs [0] != '·')             s_print [n] = 'X';
   ++n;
   ++n;  if (a_sched->beg [0] != '·')             s_print [n] = 'X';
   ++n;  if (a_sched->end [0] != '·')             s_print [n] = 'X';
   ++n;  if (a_sched->valid   != '·')             s_print [n] = 'X';
   ++n;
   ++n;  if (a_sched->m_prev  != NULL)            s_print [n] = 'X';
   ++n;  if (a_sched->m_next  != NULL)            s_print [n] = 'X';
   return s_print;
}



/*====================------------------------------------====================*/
/*===----                    allocating and freeing                    ----===*/
/*====================------------------------------------====================*/
static void  o___MEMORY__________o () { return; }

char
ysched_parse__new       (tSCHED **a_new)
{
   /*---(locals)-----------+-----+-----+-*/
   int         rce         =  -10;
   int         x_tries     =    0;
   tSCHED     *x_new       = NULL;
   /*---(header)-------------------------*/
   DEBUG_YSCHED  yLOG_senter  (__FUNCTION__);
   /*---(check return)-------------------*/
   DEBUG_YSCHED  yLOG_spoint  (a_new);
   --rce;  if (a_new == NULL) {
      DEBUG_YSCHED  yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YSCHED  yLOG_spoint  (*a_new);
   --rce;  if (*a_new != NULL) {
      DEBUG_YSCHED  yLOG_snote   ("already set");
      DEBUG_YSCHED  yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(default)------------------------*/
   *a_new = NULL;
   /*---(allocate)-----------------------*/
   while (x_new == NULL) {
      ++x_tries;
      x_new = (tSCHED *) malloc (sizeof (tSCHED));
      if (x_tries > 3)   break;
   }
   DEBUG_YSCHED  yLOG_sint    (x_tries);
   DEBUG_YSCHED  yLOG_spoint  (x_new);
   --rce;  if (x_new == NULL) {
      DEBUG_YSCHED  yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(wipe)---------------------------*/
   ysched_parse__wipe (x_new);
   /*---(save return)--------------------*/
   *a_new = x_new;
   /*---(ground pointers)----------------*/
   x_new->m_prev = NULL;
   x_new->m_next = NULL;
   /*---(add to list)--------------------*/
   if (g_head == NULL) {
      DEBUG_YSCHED  yLOG_snote   ("add first");
      g_head         = x_new;
      g_tail         = x_new;
   } else {
      DEBUG_YSCHED  yLOG_snote   ("append to end");
      x_new->m_prev  = g_tail;
      g_tail->m_next = x_new;
      g_tail         = x_new;
   }
   /*---(update count)-------------------*/
   x_new->seq     = g_max++;
   ++g_count;
   DEBUG_YSCHED  yLOG_sint    (g_count);
   /*---(update current)-----------------*/
   g_curr = x_new;
   /*---(complete)-----------------------*/
   DEBUG_YSCHED  yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
ysched_parse__free      (tSCHED **a_old)
{
   /*---(locals)-----------+-----+-----+-*/
   int         rce         =  -10;
   int         rc          =    0;
   tSCHED     *x_old       = NULL;
   /*---(header)-------------------------*/
   DEBUG_YSCHED  yLOG_senter  (__FUNCTION__);
   /*---(check return)-------------------*/
   DEBUG_YSCHED  yLOG_spoint  (a_old);
   --rce;  if (a_old == NULL) {
      DEBUG_YSCHED   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YSCHED  yLOG_spoint  (*a_old);
   --rce;  if (*a_old == NULL) {
      DEBUG_YSCHED   yLOG_snote   ("never set");
      DEBUG_YSCHED   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(simplify)-----------------------*/
   x_old = *a_old;
   /*---(remove from lists)--------------*/
   DEBUG_YSCHED  yLOG_snote   ("remove from list");
   if (x_old->m_next != NULL)  x_old->m_next->m_prev = x_old->m_prev;
   else                        g_tail                = x_old->m_prev;
   if (x_old->m_prev != NULL)  x_old->m_prev->m_next = x_old->m_next;
   else                        g_head                = x_old->m_next;
   /*---(ground pointers)----------------*/
   x_old->m_prev = NULL;
   x_old->m_next = NULL;
   /*---(update current)-----------------*/
   if (g_curr == x_old) {
      g_curr = g_head;
      if (g_curr == NULL)  g_curr = &g_null;
   }
   /*---(clear and return)---------------*/
   free (x_old);
   *a_old = NULL;
   /*---(update count)-------------------*/
   --g_count;
   DEBUG_YSCHED  yLOG_sint    (g_count);
   /*---(complete)-----------------------*/
   DEBUG_YSCHED  yLOG_sexit   (__FUNCTION__);
   return 0;
}

char ySCHED_retire  (void **a_old) { return ysched_parse__free (a_old); }

char
ysched_purge            (void)
{
   tSCHED     *o           = NULL;
   char        c           =    0;
   DEBUG_YSCHED  yLOG_enter   (__FUNCTION__);
   o = g_head;
   DEBUG_YSCHED  yLOG_point   ("g_head"    , o);
   while (o != NULL) {
      ++c;
      ysched_parse__free (&o);
      o = g_head;
      DEBUG_YSCHED  yLOG_point   ("g_head"    , o);
   }
   g_max  = 0;
   DEBUG_YSCHED  yLOG_exit    (__FUNCTION__);
   return c;
}



/*====================------------------------------------====================*/
/*===----                       parsing functions                      ----===*/
/*====================------------------------------------====================*/
static void  o___PARSING_________o () { return; }

char
ysched_parse__prep      (char *a_recd)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         len         =    0;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_YSCHED  yLOG_senter  (__FUNCTION__);
   /*---(initialize)---------------------*/
   DEBUG_YSCHED  yLOG_snote   ("reset parsing globals");
   mySCHED.full  [0] = '\0';
   mySCHED.x_raw [0] = '\0';
   mySCHED.x_min    = NULL;
   mySCHED.x_hrs    = NULL;
   mySCHED.x_dys    = NULL;
   mySCHED.x_mos    = NULL;
   mySCHED.x_dow    = NULL;
   mySCHED.x_wks    = NULL;
   /*---(initialize)---------------------*/
   DEBUG_YSCHED  yLOG_snote   ("reset error status");
   ysched_error_reset ();
   /*---(defense)------------------------*/
   --rce;  if (a_recd == NULL || a_recd[0] == '\0' || a_recd[0] == ' ' || a_recd[0] == '#') {
      DEBUG_YSCHED  yLOG_sexitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(general defenses)---------------*/
   len = strlen (a_recd);
   --rce;  if (len <=  8) {
      DEBUG_YSCHED  yLOG_sinfo   ("FAILURE", "record too short ( < 12 )");
      mySCHED.status = FAILED;
      DEBUG_YSCHED  yLOG_sexitr   (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (len >  100) {
      DEBUG_YSCHED  yLOG_sinfo   ("FAILURE", "record too long ( > 100 )");
      mySCHED.status = FAILED;
      DEBUG_YSCHED  yLOG_sexitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(copy it)------------------------*/
   strncpy (mySCHED.full , a_recd, LEN_RECD);
   /*---(complete)-----------------------*/
   DEBUG_YSCHED  yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
ysched_parse__parse     (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         len         =    0;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_YSCHED  yLOG_enter   (__FUNCTION__);
   /*---(copy it)------------------------*/
   strncpy (mySCHED.x_raw, mySCHED.full, LEN_RECD);
   /*---(break it down)------------------*/
   mySCHED.x_min  = strtok (mySCHED.x_raw , " ");
   mySCHED.x_hrs  = strtok (NULL          , " ");
   mySCHED.x_dys  = strtok (NULL          , " ");
   mySCHED.x_mos  = strtok (NULL          , " ");
   mySCHED.x_dow  = strtok (NULL          , " ");
   mySCHED.x_wks  = strtok (NULL          , " ");
   mySCHED.x_yrs  = strtok (NULL          , " ");
   /*---(process it)---------------------*/
   if (rc >= 0)  rc = ysched_field  (mySCHED.x_min, g_curr->min, PARSE_MNS);
   if (rc >= 0)  rc = ysched_field  (mySCHED.x_hrs, g_curr->hrs, PARSE_HRS);
   if (rc >= 0)  rc = ysched_field  (mySCHED.x_dys, g_curr->dys, PARSE_DYS);
   if (rc >= 0)  rc = ysched_field  (mySCHED.x_mos, g_curr->mos, PARSE_MOS);
   if (rc >= 0)  rc = ysched_field  (mySCHED.x_dow, g_curr->dow, PARSE_DOW);
   if (rc >= 0)  rc = ysched_field  (mySCHED.x_wks, g_curr->wks, PARSE_WKS);
   if (rc >= 0)  rc = ysched_field  (mySCHED.x_yrs, g_curr->yrs, PARSE_YRS);
   ysched_fancify ();
   --rce;  if (rc < 0) {
      DEBUG_YSCHED  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_YSCHED  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ysched_parse__once      (void)
{
   /*---(header)-------------------------*/
   DEBUG_YSCHED  yLOG_senter  (__FUNCTION__);
   /*---(copy raw)-----------------------*/
   strlcpy (g_curr->raw, mySCHED.full, LEN_RECD);
   /*---(copy validity)------------------*/
   strlcpy (g_curr->beg, g_beg, LEN_TERSE);
   strlcpy (g_curr->end, g_end, LEN_TERSE);
   DEBUG_YSCHED  yLOG_snote   (g_curr->raw);
   /*---(complete)-----------------------*/
   DEBUG_YSCHED  yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
ysched_parse__finish    (void)
{
   char        x_valid     =  '·';
   /*---(header)-------------------------*/
   DEBUG_YSCHED  yLOG_senter  (__FUNCTION__);
   /*---(copy dates)---------------------*/
   g_curr->epoch   = mySCHED.s_epoch;
   g_curr->year    = mySCHED.s_year;
   g_curr->month   = mySCHED.s_month;
   g_curr->day     = mySCHED.s_day;
   g_curr->hour    = -1;
   g_curr->minute  = -1;
   g_curr->result  =  0;
   /*---(update validity)----------------*/
   if (mySCHED.valid [100] == '1')  x_valid = 'y';
   g_curr->valid = x_valid;
   /*---(complete)-----------------------*/
   DEBUG_YSCHED  yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
ySCHED_create           (void **a_sched, char *a_recd)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_YSCHED  yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_YSCHED  yLOG_point   ("a_sched"   , a_sched);
   --rce;  if (a_sched == NULL) {
      DEBUG_YSCHED  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YSCHED  yLOG_point   ("*a_sched"  , *a_sched);
   --rce;  if (*a_sched != NULL) {
      DEBUG_YSCHED  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YSCHED  yLOG_point   ("a_recd"    , a_recd);
   --rce;  if (a_recd  == NULL) {
      DEBUG_YSCHED  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YSCHED  yLOG_info    ("a_recd"    , a_recd);
   /*---(prepare)------------------------*/
   rc = ysched_parse__prep (a_recd);
   DEBUG_YSCHED  yLOG_value   ("prep"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_YSCHED  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(create)-------------------------*/
   rc = ysched_parse__new (a_sched);
   DEBUG_YSCHED  yLOG_value   ("new"       , rc);
   --rce;  if (rc < 0) {
      DEBUG_YSCHED  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(parse)--------------------------*/
   rc = ysched_parse__parse ();
   DEBUG_YSCHED  yLOG_value   ("split"     , rc);
   --rce;  if (rc < 0) {
      ysched_parse__free (a_sched);
      DEBUG_YSCHED  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(finish)-------------------------*/
   rc = ysched_parse__finish ();
   DEBUG_YSCHED  yLOG_value   ("finish"    , rc);
   rc = ysched_parse__once   ();
   DEBUG_YSCHED  yLOG_value   ("once"      , rc);
   DEBUG_YSCHED  yLOG_info    ("THE_RAW"   , g_curr->raw);
   /*---(complete)-----------------------*/
   DEBUG_YSCHED  yLOG_exit    (__FUNCTION__);
   return 0;
}

char*
ySCHED_raw              (void *a_sched)
{
   return ((tSCHED *) a_sched)->raw;
}

char
ySCHED_force            (void **a_sched, char *a_recd)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_YSCHED  yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_YSCHED  yLOG_point   ("a_sched"   , a_sched);
   --rce;  if (a_sched == NULL) {
      DEBUG_YSCHED  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(free if needed)-----------------*/
   DEBUG_YSCHED  yLOG_point   ("*a_sched"  , *a_sched);
   if (*a_sched != NULL)   ysched_parse__free (a_sched);
   /*---(run)----------------------------*/
   rc = ySCHED_create (a_sched, a_recd);
   --rce;  if (rc < 0) {
      DEBUG_YSCHED  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_YSCHED  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ysched_update           (void *a_sched)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tSCHED     *x_sched     = NULL;
   char        t           [LEN_HUND]  = "";
   /*---(header)-------------------------*/
   DEBUG_YSCHED  yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_YSCHED  yLOG_point   ("a_sched"   , a_sched);
   --rce;  if (a_sched == NULL) {
      DEBUG_YSCHED  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   x_sched = (tSCHED *) a_sched;
   /*---(assign to current)--------------*/
   g_curr = x_sched;
   DEBUG_YSCHED  yLOG_info    ("raw"       , g_curr->raw);
   /*---(prepare)------------------------*/
   rc = ysched_parse__prep (x_sched->raw);
   DEBUG_YSCHED  yLOG_value   ("prep"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_YSCHED  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(parse)--------------------------*/
   rc = ysched_parse__parse ();
   DEBUG_YSCHED  yLOG_value   ("split"     , rc);
   --rce;  if (rc < 0) {
      ysched_parse__free (&a_sched);
      DEBUG_YSCHED  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(update validity)----------------*/
   sprintf (t, ".valid %s %s", g_curr->beg, g_curr->end);
   ySCHED_valid (t);
   /*---(parse)--------------------------*/
   rc = ysched_parse__finish ();
   DEBUG_YSCHED  yLOG_value   ("finish"    , rc);
   /*---(complete)-----------------------*/
   DEBUG_YSCHED  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ysched_update_all       (void)
{
   tSCHED     *o           = NULL;
   char        rc          =    0;
   char        c           =    0;
   DEBUG_YSCHED  yLOG_enter   (__FUNCTION__);
   o = g_head;
   DEBUG_YSCHED  yLOG_point   ("g_head"    , o);
   while (o != NULL) {
      ++c;
      rc = ysched_update (o);
      o = o->m_next;
      DEBUG_YSCHED  yLOG_point   ("next"      , o);
   }
   DEBUG_YSCHED  yLOG_exit    (__FUNCTION__);
   return c;
}

char
ySCHED_details          (char *a_min, char *a_hrs, char *a_dys, char *a_mos, char *a_dow, char *a_wks, char *a_yrs, char *a_beg, char *a_end, char *a_valid)
{
   if (a_min   != NULL)  strlcpy (a_min, g_curr->min, LEN_LONG);
   if (a_hrs   != NULL)  strlcpy (a_hrs, g_curr->hrs, LEN_TITLE);
   if (a_dys   != NULL)  strlcpy (a_dys, g_curr->dys, LEN_DESC);
   if (a_mos   != NULL)  strlcpy (a_mos, g_curr->mos, LEN_LABEL);
   if (a_dow   != NULL)  strlcpy (a_dow, g_curr->dow, LEN_TERSE);
   if (a_wks   != NULL)  strlcpy (a_wks, g_curr->wks, LEN_LONG);
   if (a_yrs   != NULL)  strlcpy (a_yrs, g_curr->yrs, LEN_LONG);
   if (a_beg   != NULL)  strlcpy (a_beg, g_curr->beg, LEN_TERSE);
   if (a_end   != NULL)  strlcpy (a_end, g_curr->end, LEN_TERSE);
   if (a_valid != NULL)  *a_valid = g_curr->valid;
   return 0;
}



/*====================------------------------------------====================*/
/*===----                         unit testing                         ----===*/
/*====================------------------------------------====================*/
static void  o___UNITTEST________o () { return; }

char      unit_answer [LEN_RECD];

char*
ysched_parse__unit      (char *a_question, int a_num)
{
   /*---(locals)-----------+-----+-----+-*/
   int         x_fore      =    0;
   int         x_back      =    0;
   tSCHED     *o           = NULL;
   int         c           =    0;
   char        t           [LEN_HUND]  = "-";
   char        s           [LEN_HUND]  = "-";
   /*---(defense)------------------------*/
   snprintf (unit_answer, LEN_RECD, "PARSE unit       : question unknown");
   /*---(simple)-------------------------*/
   if  (strcmp (a_question, "count"     )     == 0) {
      strlcpy (t, "", LEN_HUND);
      o = g_head; while (o != NULL) { ++x_fore; sprintf (s, " %2d", o->seq); strlcat (t, s, LEN_HUND); o = o->m_next; }
      o = g_tail; while (o != NULL) { ++x_back; o = o->m_prev; }
      if (x_fore == 0)  strlcpy (t, "-", LEN_HUND);
      snprintf (unit_answer, LEN_RECD, "PARSE count      : %3dc %3df %3db  %s", g_count, x_fore, x_back, t);
      return unit_answer;
   }
   else if (strcmp (a_question, "entry"     )     == 0) {
      o = g_head; while (o != NULL) { if (c >= a_num)  break; ++c; o = o->m_next; }
      if (o != NULL) {
         sprintf  (t, "%2d[%.40s]", strlen (o->raw), o->raw);
         snprintf (unit_answer, LEN_RECD, "PARSE entry (%2d) : %2d  %s", a_num, o->seq, t);
      } else {
         snprintf (unit_answer, LEN_RECD, "PARSE entry (%2d) :  0   0[]", a_num);
      }
   }
   /*---(detailed parsing)---------------*/
   else if (strcmp(a_question, "minutes"      ) == 0) {
      sprintf(unit_answer, "PARSE minutes    : %.65s", g_curr->min);
   } else if (strcmp(a_question, "hours"        ) == 0) {
      sprintf(unit_answer, "PARSE hours      : %.35s", g_curr->hrs);
   } else if (strcmp(a_question, "days"         ) == 0) {
      sprintf(unit_answer, "PARSE days       : %.35s", g_curr->dys);
   } else if (strcmp(a_question, "months"       ) == 0) {
      sprintf(unit_answer, "PARSE months     : %.35s", g_curr->mos);
   } else if (strcmp(a_question, "dow"          ) == 0) {
      sprintf(unit_answer, "PARSE weekdays   : %.35s", g_curr->dow);
   } else if (strcmp(a_question, "weeks"        ) == 0) {
      sprintf(unit_answer, "PARSE weeks      : %.60s", g_curr->wks);
   } else if (strcmp(a_question, "years"        ) == 0) {
      sprintf(unit_answer, "PARSE years      : %.60s", g_curr->yrs);
   } else if (strcmp(a_question, "valid"        ) == 0) {
      sprintf(unit_answer, "PARSE valid      : %-8.8s  %c  %-8.8s", g_curr->beg, g_curr->valid, g_curr->end);
   } else if (strcmp(a_question, "update"       ) == 0) {
      sprintf(unit_answer, "PARSE update     : %-10ld  %02d  %02d  %02d", g_curr->epoch, g_curr->year, g_curr->month, g_curr->day);
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}





