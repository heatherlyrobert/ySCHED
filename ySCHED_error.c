/*============================----beg-of-source---============================*/
#include   "ySCHED.h"
#include   "ySCHED_priv.h"




int         e_field     =    0;
int         e_section   =    0;

char        e_func      [LEN_LABEL] = "";
int         e_line      =    0;
char        e_issue     [LEN_DESC]  = "";
int         e_pos       =    0;
int         e_len       =    0;
char        e_fancy     [LEN_RECD]  = "";



/*====================------------------------------------====================*/
/*===----                      error reporting                         ----===*/
/*====================------------------------------------====================*/
static void      o___ERRORS__________________o (void) {;};

char
ysched__trouble         (int *a_func, int a_line, char *a_issue, int a_pos, int a_len)
{
   strlcpy (e_func , a_func , LEN_LABEL);
   e_line  = a_line;
   strlcpy (e_issue, a_issue, LEN_DESC);
   e_pos   = a_pos;
   e_len   = a_len;
   strlcpy (e_fancy, ""     , LEN_RECD);
   return 0;
}

char
ysched_error_reset      (void)
{
   strlcpy (mySCHED.x_field, "", LEN_HUND);
   e_field    = 0;
   e_section  = 0;
   ysched__trouble ("-", 0, "-", -1, 0);
   return 0;
}

char
ysched_fancify          (void)
{
   char        t           [LEN_RECD]  = "";
   char        o           =    0;
   if (e_line <= 0) {
      strlcpy (e_fancy, BOLD_GRN, LEN_RECD);
      strlcat (e_fancy, mySCHED.full, LEN_RECD);
      strlcat (e_fancy, BOLD_OFF, LEN_RECD);
      return 0;
   }
   if (e_pos  <  0) {
      strlcpy (e_fancy, BOLD_MAG, LEN_RECD);
      strlcat (e_fancy, mySCHED.full, LEN_RECD);
      strlcat (e_fancy, BOLD_OFF, LEN_RECD);
      return 0;
   }
   o = e_field + e_section + e_pos;
   if (o > 0)  sprintf (t, "%-*.*s", o, o, mySCHED.full);
   /*> printf ("\n");                                                                 <*/
   strlcpy (e_fancy, t, LEN_RECD);
   /*> printf ("%s\n", e_fancy);                                                      <*/
   sprintf (t, "%s%-*.*s%s", BOLD_ERR, e_len, e_len, mySCHED.full + o, BOLD_OFF);
   strlcat (e_fancy, t, LEN_RECD);
   /*> printf ("%s\n", e_fancy);                                                      <*/
   sprintf (t, "%s", mySCHED.full + o + e_len);
   strlcat (e_fancy, t, LEN_RECD);
   /*> printf ("%s\n", e_fancy);                                                      <*/
   return 0;
}

char
ySCHED_feedback         (int *a_line, char *a_focus, char *a_issue, int *a_beg, int *a_len, char *a_fancy)
{
   if (a_line  != NULL)  *a_line = e_line;
   if (a_focus != NULL)  strlcpy (a_focus, e_func , LEN_LABEL);
   if (a_issue != NULL)  strlcpy (a_issue, e_issue, LEN_DESC);
   if (a_beg   != NULL)  *a_beg   = e_pos;
   if (a_len   != NULL)  *a_len   = e_len;
   if (a_fancy != NULL)  strlcpy (a_fancy, e_fancy, LEN_RECD);
   return 0;
}


/*====================------------------------------------====================*/
/*===----                         unit testing                         ----===*/
/*====================------------------------------------====================*/
static void      o___UNITTEST________________o (void) {;};

char*      /*----: unit testing accessor for clean validation interface ------*/
ysched_error__unit      (char *a_question)
{
   char        t           [LEN_HUND]  = "";
   char        s           [LEN_HUND]  = "";
   /*---(detailed parsing)---------------*/
   strncpy (unit_answer, "unknown request", 100);
   if        (strcmp(a_question, "trouble"      ) == 0) {
      sprintf(unit_answer, "ERROR curr (%3d) : %-7.7s, %-20.20s   %4d   %4d   %4d   %4d", e_line, e_func, e_issue, e_pos, e_len, e_field, e_section);
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}


