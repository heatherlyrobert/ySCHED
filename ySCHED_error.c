/*============================----beg-of-source---============================*/
#include   "ySCHED.h"
#include   "ySCHED_priv.h"




int         e_field     =    0;
int         e_section   =    0;

char        e_sect      [LEN_LABEL] = "";
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
   ystrlcpy (e_func , a_func , LEN_LABEL);
   e_line  = a_line;
   ystrlcpy (e_issue, a_issue, LEN_DESC);
   e_pos   = a_pos;
   e_len   = a_len;
   ystrlcpy (e_fancy, ""     , LEN_RECD);
   return 0;
}

char
ysched_error_reset      (void)
{
   ystrlcpy (mySCHED.x_field, "", LEN_HUND);
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
      ystrlcpy (e_fancy, BOLD_GRN, LEN_RECD);
      ystrlcat (e_fancy, mySCHED.full, LEN_RECD);
      ystrlcat (e_fancy, BOLD_OFF, LEN_RECD);
      return 0;
   }
   if (e_pos  <  0) {
      ystrlcpy (e_fancy, BOLD_MAG, LEN_RECD);
      ystrlcat (e_fancy, mySCHED.full, LEN_RECD);
      ystrlcat (e_fancy, BOLD_OFF, LEN_RECD);
      return 0;
   }
   o = e_field + e_section + e_pos;
   if (o > 0)  sprintf (t, "%s%-*.*s%s", BOLD_GRN, o, o, mySCHED.full, BOLD_OFF);
   /*> printf ("\n");                                                                 <*/
   ystrlcpy (e_fancy, t, LEN_RECD);
   /*> printf ("%s\n", e_fancy);                                                      <*/
   sprintf (t, "%s%-*.*s%s", BOLD_ERR, e_len, e_len, mySCHED.full + o, BOLD_OFF);
   ystrlcat (e_fancy, t, LEN_RECD);
   /*> printf ("%s\n", e_fancy);                                                      <*/
   sprintf (t, "%s%s%s", BOLD_BLK, mySCHED.full + o + e_len, BOLD_OFF);
   ystrlcat (e_fancy, t, LEN_RECD);
   /*> printf ("%s\n", e_fancy);                                                      <*/
   return 0;
}

char
ySCHED_feedback         (int *r_line, char *r_sect, char *r_focus, char *r_issue, int *r_beg, int *r_len, char *r_fancy)
{
   if (r_line  != NULL)  *r_line = e_line;
   if (r_sect  != NULL)  ystrlcpy (r_sect , e_sect , LEN_LABEL);
   if (r_focus != NULL)  ystrlcpy (r_focus, e_func , LEN_LABEL);
   if (r_issue != NULL)  ystrlcpy (r_issue, e_issue, LEN_DESC);
   if (r_beg   != NULL)  *r_beg   = e_pos;
   if (r_len   != NULL)  *r_len   = e_len;
   if (r_fancy != NULL)  ystrlcpy (r_fancy, e_fancy, LEN_RECD);
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


