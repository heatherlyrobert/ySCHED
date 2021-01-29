/*============================----beg-of-source---============================*/

#include   "ySCHED.h"
#include   "ySCHED_priv.h"



/*====================------------------------------------====================*/
/*===----                         unit testing                         ----===*/
/*====================------------------------------------====================*/
static void      o___UNITTEST________________o (void) {;};

#define       LEN_TEXT  2000
char          unit_answer [ LEN_TEXT ];

char*      /*----: unit testing accessor for clean validation interface ------*/
ysched__accessor   (char *a_question)
{
   char        t           [LEN_HUND]  = "";
   char        s           [LEN_HUND]  = "";
   /*---(answer)------------------------------------------*/
   strncpy(unit_answer, "unknown request", 100);
   if        (strcmp(a_question, "curr_date"    ) == 0) {
      sprintf (unit_answer, "current date   : ");
      if (mySCHED.s_month == 0)  sprintf (t, "  ·y,  ·m,  ·d / ");
      else                    sprintf (t, "%3dy, %2dm, %2dd / ", mySCHED.s_year, mySCHED.s_month, mySCHED.s_day);
      strcat (unit_answer, t);
      if (mySCHED.s_dow == 0)  sprintf (t, "·dow,  ·dim,   ·diy,   ·doy / ");
      else                    sprintf (t, "%1ddow, %2ddim, %3ddiy, %3ddoy / ", mySCHED.s_dow, mySCHED.s_dim, mySCHED.s_diy, mySCHED.s_doy);
      strcat (unit_answer, t);
      if (mySCHED.s_wiy == 0)  sprintf (t, "·fdow, ·wze,  ·wiy,  ·woy / ");
      else                    sprintf (t, "%1dfdow, %1dwze, %2dwiy, %2dwoy / ", mySCHED.s_fdow, mySCHED.s_wze, mySCHED.s_wiy, mySCHED.s_woy);
      strcat (unit_answer, t);
      if (mySCHED.s_off == 0)  sprintf (t, "   ·off");
      else                    sprintf (t, "%4doff", mySCHED.s_off);
      strcat (unit_answer, t);
   } else if (strcmp(a_question, "parsed"       ) == 0) {
      sprintf(unit_answer, "parsed freq    : %.35s", mySCHED.last);
   } else if (strcmp(a_question, "field"        ) == 0) {
      sprintf(unit_answer, "field          : %3d:%.45s", s_len, s_input);

   } else if (strcmp(a_question, "section"      ) == 0) {
      sprintf (t, "%2d[%-.20s]", strlen (s_section), s_section);
      sprintf (s, "%2d[%-.20s]", strlen (s_ptr  ), s_ptr  );
      sprintf(unit_answer, "section        : %-24.24s  %2db  %2de  %c!  %c^  %c~  %2d/  %s", t, s_beg, s_end, s_not, s_rev, s_inv, s_stp, s);
   } else if (strcmp(a_question, "trouble"      ) == 0) {
      sprintf (t, "[%.45s]", e_fancy);
      sprintf(unit_answer, "trouble  (%3d) : %-7.7s, %-15.15s %4d %2d %s", e_line, e_func, e_issue, e_pos, e_len, t);
   } else if (strcmp(a_question, "step"         ) == 0) {
      sprintf(unit_answer, "step           : %d", s_stp);
   } else if (strcmp(a_question, "modifiers"    ) == 0) {
      sprintf(unit_answer, "modifiers      : not=%c, rev=%c, inv=%c", s_not, s_rev, s_inv);
   } else if (strcmp(a_question, "range"        ) == 0) {
      sprintf(unit_answer, "range          : beg=%-3d, end=%d", s_beg, s_end);
   } else if (strcmp(a_question, "limits"       ) == 0) {
      sprintf(unit_answer, "limits    (%2d) : %-12.12s  min %3d to %-3d max -- tmax %d", s_type, s_label, s_min, s_max, s_tmax);
   } else if (strcmp(a_question, "effective"    ) == 0) {
      sprintf(unit_answer, "effective      : %.35s", mySCHED.effout);
   } else if (strcmp(a_question, "minutes"      ) == 0) {
      sprintf(unit_answer, "min : %.65s", g_sched.min);
   } else if (strcmp(a_question, "hours"        ) == 0) {
      sprintf(unit_answer, "hrs : %.35s", g_sched.hrs);
   } else if (strcmp(a_question, "days"         ) == 0) {
      sprintf(unit_answer, "dys : %.35s", g_sched.dys);
   } else if (strcmp(a_question, "months"       ) == 0) {
      sprintf(unit_answer, "mos : %.35s", g_sched.mos);
   } else if (strcmp(a_question, "dow"          ) == 0) {
      sprintf(unit_answer, "dow : %.35s", g_sched.dow);
   } else if (strcmp(a_question, "weeks"        ) == 0) {
      sprintf(unit_answer, "wks : %.60s", g_sched.wks);
   } else if (strcmp(a_question, "years"        ) == 0) {
      sprintf(unit_answer, "yrs : %.60s", g_sched.yrs);
   /*> } else if (strcmp(a_question, "duration"     ) == 0) {                         <* 
    *>    sprintf(unit_answer, "dur : %d"   , g_sched.dur);                           <*/
   }
   /*---(complete)----------------------------------------*/
   return unit_answer;
}

char       /*----: set up program urgents/debugging --------------------------*/
ysched__unit_quiet     (void)
{
   yLOGS_begin ("ySCHED" , YLOG_SYS, YLOG_QUIET);
   return 0;
}

char       /*----: set up program urgents/debugging --------------------------*/
ysched__unit__loud    (void)
{
   yLOGS_begin ("ySCHED" , YLOG_SYS, YLOG_NOISE);
   yURG_name  ("kitchen"      , YURG_ON);
   yURG_name  ("ysched"       , YURG_ON);
   DEBUG_YSCHED  yLOG_info     ("ySCHED"    , ySCHED_version   ());
   return 0;
}

char       /*----: stop logging ----------------------------------------------*/
ysched__unit_end   (void)
{
   yLOGS_end     ();
   return 0;
}


/*============================----end-of-source---============================*/
