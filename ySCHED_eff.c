/*============================----beg-of-source---============================*/
#include   "ySCHED.h"
#include   "ySCHED_priv.h"



/*====================------------------------------------====================*/
/*===----                            effective                         ----===*/
/*====================------------------------------------====================*/
static void      o___EFFECTIVE_______________o (void) {;}

char       /*----: initialize the effective range ----------------------------*/
ySCHED__effinit    (void)
{
   /*---(locals)-------------------------*/
   int       i         = 0;            /* loop iterator -- day                */
   /*---(initialize)---------------------*/
   for (i = 0; i < 500; ++i) {
      mySCHED.effective[i] = '1';
      if (mySCHED.global[i] == '_') mySCHED.effective[i] = '_';
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
      if (mySCHED.effective[i] == '_')  mySCHED.global[i] = '_';
   }
   strncpy (mySCHED.effective, mySCHED.global , 500);
   ySCHED__effout  ();
   strncpy (mySCHED.gloout   , mySCHED.effout , 100);
   ySCHED__effinit ();
   /*> printf ("globalized as <%s>\n", mySCHED.gloout);                                       <*/
   /*---(complete)-----------------------*/
   return 0;
}

char       /*----: make a printable version of the effective dates -----------*/
ySCHED__effout     (void)
{
   int       i;
   mySCHED.effout [ 0] = '<';
   for (i =  0; i <  10; ++i)  mySCHED.effout [i + 1] = mySCHED.effective[i +  90];
   mySCHED.effout [11] = ' ';
   mySCHED.effout [12] = mySCHED.effective[100];
   mySCHED.effout [13] = ' ';
   for (i =  0; i <  10; ++i)  mySCHED.effout [i + 14] = mySCHED.effective[i + 101];
   mySCHED.effout [24] = '>';
   mySCHED.effout [25] = '\0';
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
         if (a_side == 'b' && i <  xoff)   mySCHED.effective[i] = '_';
         if (a_side == 'e' && i >  xoff)   mySCHED.effective[i] = '_';
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
         mySCHED.effective[xoff] = '_';
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
      mySCHED.effective[i] = '1';
      if (mySCHED.global[i] == '_') mySCHED.effective[i] = '_';
   }
   ySCHED__effout();
   /*> printf ("eff clean = <%s>\n", effout);                                         <*/
   /*---(defense)------------------------*/
   if (strlen (a_recd) < 10)                      return -1;
   if (strncmp(a_recd, "reset"    ,   5) == 0)    return -1;
   if (strncmp(a_recd, ".effective", 10) != 0)    return -1;
   /*---(break it down)------------------*/
   strncpy(mySCHED.recd, a_recd  , MAX_RECD);
   strtok(mySCHED.recd, " ");
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
