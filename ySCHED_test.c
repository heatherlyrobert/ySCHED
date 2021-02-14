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
   if (x_hour != ySCHED_ANY && x_sched->hrs [x_hour]  == '_') {
      return 0;
   }
   if (x_hour == ySCHED_ANY && strchr (x_sched->hrs, '1') == NULL) {
      return 0;
   }
   /*---(minutes)------------------------*/
   if (x_min != ySCHED_ANY && x_sched->min [x_min]  == '_') {
      return 0;
   }
   if (x_min == ySCHED_ANY && strchr (x_sched->min, '1') == NULL) {
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
   tSCHED     *x_sched     = NULL;
   /*---(check location)-----------------*/
   x_sched = (tSCHED *) a_sched;
   /*---(effective)----------------------*/
   if (x_sched->valid  != 'y') {
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



/*====================------------------------------------====================*/
/*===----                         unit testing                         ----===*/
/*====================------------------------------------====================*/
static void      o___UNITTEST________________o (void) {;};

char*      /*----: unit testing accessor for clean validation interface ------*/
ysched__accessor   (char *a_question)
{
   char        t           [LEN_HUND]  = "";
   char        s           [LEN_HUND]  = "";
   /*---(detailed parsing)---------------*/
   strncpy (unit_answer, "unknown request", 100);
   /*---(parsing)------------------------*/
   if      (strcmp(a_question, "parsed"       ) == 0) {
      sprintf(unit_answer, "parsed freq    : %.35s", mySCHED.last);
   } else if (strcmp(a_question, "field"        ) == 0) {
      sprintf(unit_answer, "field          : %3d:%.45s", s_len, s_input);

   } else if (strcmp(a_question, "section"      ) == 0) {
      sprintf (t, "%2d[%-.20s]", strlen (s_section), s_section);
      sprintf (s, "%2d[%-.20s]", strlen (s_ptr  ), s_ptr  );
      sprintf(unit_answer, "section        : %-24.24s  %2db  %2de  %c!  %c^  %c~  %2d/  %s", t, s_beg, s_end, s_not, s_rev, s_inv, s_stp, s);
   } else if (strcmp(a_question, "step"         ) == 0) {
      sprintf(unit_answer, "step           : %d", s_stp);
   } else if (strcmp(a_question, "modifiers"    ) == 0) {
      sprintf(unit_answer, "modifiers      : not=%c, rev=%c, inv=%c", s_not, s_rev, s_inv);
   } else if (strcmp(a_question, "range"        ) == 0) {
      sprintf(unit_answer, "range          : beg=%-3d, end=%d", s_beg, s_end);
   } else if (strcmp(a_question, "limits"       ) == 0) {
      sprintf(unit_answer, "limits    (%2d) : %-12.12s  min %3d to %-3d max -- tmax %d", s_type, s_label, s_min, s_max, s_tmax);
   }
   /*---(detailed parsing)---------------*/
   else if (strcmp(a_question, "minutes"      ) == 0) {
      sprintf(unit_answer, "min : %.65s", g_curr->min);
   } else if (strcmp(a_question, "hours"        ) == 0) {
      sprintf(unit_answer, "hrs : %.35s", g_curr->hrs);
   } else if (strcmp(a_question, "days"         ) == 0) {
      sprintf(unit_answer, "dys : %.35s", g_curr->dys);
   } else if (strcmp(a_question, "months"       ) == 0) {
      sprintf(unit_answer, "mos : %.35s", g_curr->mos);
   } else if (strcmp(a_question, "dow"          ) == 0) {
      sprintf(unit_answer, "dow : %.35s", g_curr->dow);
   } else if (strcmp(a_question, "weeks"        ) == 0) {
      sprintf(unit_answer, "wks : %.60s", g_curr->wks);
   } else if (strcmp(a_question, "years"        ) == 0) {
      sprintf(unit_answer, "yrs : %.60s", g_curr->yrs);
   } else if (strcmp(a_question, "valid"        ) == 0) {
      sprintf(unit_answer, "PARSE valid      : %-8.8s  %c  %-8.8s", g_curr->beg, g_curr->valid, g_curr->end);
   } else if (strcmp(a_question, "update"       ) == 0) {
      sprintf(unit_answer, "PARSE update     : %-10ld  %02d  %02d  %02d", g_curr->epoch, g_curr->year, g_curr->month, g_curr->day);
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
