/*============================----beg-of-source---============================*/
#include   "ySCHED.h"
#include   "ySCHED_priv.h"


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
ysched__limits     (char a_type)
{
   /*---(set full limits)----------------*/
   s_min       = s_fields [a_type].min;
   s_max       = s_fields [a_type].max;
   s_smax      = s_max;
   /*---(overrides)----------------------*/
   if      (TYPE_DYS)     s_smax = mySCHED.s_dim;
   else if (TYPE_WKS)     s_smax = mySCHED.s_wiy;
   /*---(complete)-----------------------*/
   return 0;
}

char         /*--: interpret modifier --------------------[ leaf   [ ------ ]-*/
ysched__prep       (int a_type)
{
   /*---(initialize)---------------------*/
   s_beg       =  -1;
   s_end       =  -1;
   s_not       = '-';
   s_rev       = '-';
   s_inv       = '-';
   s_stp       =   1;
   /*---(measure input string)-----------*/
   s_len       = strlen (s_input);
   /*---(complete)-----------------------*/
   return 0;
}

char         /*--: interpret modifier --------------------[ leaf   [ ------ ]-*/
ysched__modifier   (void)
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
ysched__step       (void)
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
ysched__const      (int a_type)
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
ysched__number     (int a_type, char *a_number)
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
   --rce;  if (TYPE_DYS)  if (x_num > mySCHED.s_dim) return rce;
   --rce;  if (TYPE_WKS)  if (x_num > mySCHED.s_wiy) return rce;
   /*---(complete)-----------------------*/
   return x_num;
}

int          /*--: interpret special day references ------[ leaf   [ ------ ]-*/
ysched__day        (void)
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
   rc = ysched__number (PARSE_DYS, s_input);
   if (rc < 0) {
      --rce;  if (strcmp ("L", s_input) != 0)  return rce;
      rc = mySCHED.s_dim;
   }
   x_day = rc;
   /*---(determine day of week)----------*/
   x_dow = (((x_day + mySCHED.s_fdow) - 1) % 7);
   if (x_dow == 0) x_dow = 7;
   /*---(handle types)-------------------*/
   switch (x_type) {
   case 'a' :  /* after type, so push weekends forward  */
      if (x_dow == 7       )   { x_day += 1; x_dow = 1; }  /* next monday     */
      if (x_dow == 6       )   { x_day += 2; x_dow = 1; }  /* next monday     */
      if (x_day >  mySCHED.s_dim)   { x_day -= 3; x_dow = 5; }  /* last friday     */
      break;
   case 'b' :  /* before type, so push weekends backward */
      if (x_dow == 7       )   { x_day -= 2; x_dow = 5; }  /* prev friday     */
      if (x_dow == 6       )   { x_day -= 1; x_dow = 5; }  /* prev friday     */
      if (x_day <= 0       )   { x_day += 3; x_dow = 1; }  /* first monday    */
      break;
   case 'n' :  /* nearist type, so push sat back and sun forward */
      if (x_dow == 7       )   { x_day += 1; x_dow = 1; }  /* next monday     */
      if (x_dow == 6       )   { x_day -= 1; x_dow = 5; }  /* prev friday     */
      if (x_day >  mySCHED.s_dim)   { x_day -= 3; x_dow = 5; }  /* last friday     */
      if (x_day <= 0       )   { x_day += 3; x_dow = 1; }  /* first monday    */
      break;
   }
   /*---(complete)-----------------------*/
   return x_day;
}

char         /*--: interpret ranges ----------------------[ ------ [ ------ ]-*/
ysched__range      (int  a_type)
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
      rc = ysched__number (a_type, s_input + 1);
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
      rc = ysched__number (a_type, s_input);
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
      rc = ysched__day ();
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
      rc = ysched__number (a_type, s_input);
      --rce;  if (rc <  0)   return rce;
      s_beg = rc;
      rc = ysched__number (a_type, r + 1);
      --rce;  if (rc <  0) { s_beg = -1;  return rce; }
      s_end = rc;
      --rce;  if (s_beg > s_end) { s_beg = s_end = -1;  return rce; }
      return 0;
   }
   /*---(handle simple value)------------*/
   rc = ysched__number (a_type, s_input);
   --rce;  if (rc    <  0)                return rce;
   s_beg = s_end = rc;
   --rce;  if (s_stp != 1)                return rce;
   /*---(complete)-----------------------*/
   return 0;
}

char         /*--: apply grammar to array ----------------[ ------ [ ------ ]-*/
ysched__apply      (int a_type, char *a_array)
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
   for (i = s_smax + 1; i <= s_max; ++i)  a_array [i] = '.';
   a_array     [s_max + 1] = '\0';
   /*---(complete)-----------------------*/
   return 0;
}

char         /*--: interpret one field of grammar --------[ ------ [ ------ ]-*/
ysched__field      (
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
      DEBUG_YSCHED yLOG_exitr (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YSCHED yLOG_value   ("a_type"    , a_type);
   --rce;  if (a_type  >= MAX_FIELDS)  {
      DEBUG_YSCHED yLOG_exitr (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (a_type  <  PARSE_MNS)  {
      DEBUG_YSCHED yLOG_exitr (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (a_type  >  PARSE_YRS) {
      DEBUG_YSCHED yLOG_exitr (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YSCHED yLOG_point   ("*a_input"  , a_input);
   --rce;  if (a_input == NULL      )  {
      /*---(check for defaulting)-----------*/
      if (a_type == PARSE_WKS || a_type == PARSE_YRS) {
         DEBUG_YSCHED yLOG_note  ("requested wks or yrs");
         ysched__prep     (a_type);
         s_beg = s_min;
         s_end = s_smax;
         ysched__apply    (a_type, x_array);
         strcpy (a_array , x_array);
         strcpy (mySCHED.last, a_array);
         DEBUG_YSCHED yLOG_exit  (__FUNCTION__);
         return 0;
      }
      DEBUG_YSCHED yLOG_exitr (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YSCHED yLOG_info    ("a_input"   , a_input);
   /*---(initialize arrays)--------------*/
   strcpy (a_array , x_error);
   strcpy (mySCHED.last, a_array);
   DEBUG_YSCHED yLOG_info    ("default"   , a_array);
   for (i = 0; i < LEN_HUND; ++i) x_array   [i]    = '_';
   DEBUG_YSCHED yLOG_info    ("initial"   , x_array);
   /*---(parse into sections)------------*/
   p = strtok_r (a_input, q, &s);
   --rce;
   while (p != NULL) {
      s_input =  p;
      DEBUG_YSCHED yLOG_info    ("s_input"   , s_input);
      s_len = strlen (s_input);
      DEBUG_YSCHED yLOG_value   ("s_len"     , s_len);
      rc = ysched__prep     (a_type);
      DEBUG_YSCHED yLOG_value   ("prep_rc"   , rc);
      if (rc <  0) {
         DEBUG_YSCHED yLOG_exitr (__FUNCTION__, rce);
         return rce;
      }
      rc = ysched__modifier ();
      DEBUG_YSCHED yLOG_value   ("mod_rc"    , rc);
      if (rc <  0) {
         DEBUG_YSCHED yLOG_exitr (__FUNCTION__, rce);
         return rce - 1;
      }
      rc = ysched__step     ();
      DEBUG_YSCHED yLOG_value   ("step_rc"   , rc);
      if (rc <  0)  {
         DEBUG_YSCHED yLOG_exitr (__FUNCTION__, rce);
         return rce - 2;
      }
      rc = ysched__const    (a_type);
      DEBUG_YSCHED yLOG_value   ("const_rc"  , rc);
      --rce;  if (rc < 0) {
         DEBUG_YSCHED yLOG_exitr (__FUNCTION__, rce);
         return rce;
      }
      if (rc == 0)  {
         rc = ysched__range    (a_type);
         DEBUG_YSCHED yLOG_value   ("range_rc"  , rc);
         --rce; if (rc < 0)  {
            DEBUG_YSCHED yLOG_exitr (__FUNCTION__, rce);
            return rce;
         }
      }
      rc = ysched__apply    (a_type, x_array);
      DEBUG_YSCHED yLOG_value   ("apply_rc"  , rc);
      --rce;  if (rc < 0) {
         DEBUG_YSCHED yLOG_exitr (__FUNCTION__, rce);
         return rce;
      }
      p = strtok_r (NULL, q, &s);
   }
   /*---(move to final)------------------*/
   strcpy (a_array , x_array);
   strcpy (mySCHED.last, a_array);
   DEBUG_YSCHED yLOG_info    ("final"     , a_array);
   /*---(complete)-----------------------*/
   DEBUG_YSCHED yLOG_exit    (__FUNCTION__);
   return 0;
}

int
ysched_duration         (char *a_input)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_input     [LEN_HUND];
   int         x_len       =    0;
   char        x_unit      =  '-';
   char       *x_units     = "smthd";
   char       *x_nums      = "0123456789.";
   float       x_mult      =    1;
   float       x_mins      =    0;
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_YSCHED yLOG_enter   (__FUNCTION__);
   /*---(defenses)-----------------------*/
   DEBUG_YSCHED yLOG_point   ("a_input"   , a_input);
   --rce;  if (a_input == NULL) {
      DEBUG_YSCHED yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   strncpy (x_input, a_input, LEN_HUND);
   x_len = strlen (x_input);
   DEBUG_YSCHED yLOG_point   ("x_len"     , x_len);
   --rce;  if (x_len <= 0) {
      DEBUG_YSCHED yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check for units)----------------*/
   x_unit = x_input [x_len - 1];
   DEBUG_YSCHED yLOG_char    ("x_unit"    , x_unit);
   DEBUG_YSCHED yLOG_info    ("x_units"   , x_units);
   --rce;  if (strchr (x_units, x_unit) != NULL) {
      switch (x_unit) {
      case  's' : x_mult = 1.0 / 60.0;   break;
      case  'm' : x_mult = 1.0;          break;
      case  't' : x_mult = 10.0;         break;
      case  'h' : x_mult = 60.0;         break;
      case  'd' : x_mult = 480.0;        break;
      }
      x_input [--x_len] = '\0';
   }
   /*---(check remaining string)---------*/
   --rce;  for (i = 0; i < x_len; ++i) {
      if (strchr (x_nums, x_input [i]) == NULL) {
         DEBUG_YSCHED yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(convert number)-----------------*/
   x_mins   = atof (x_input);
   DEBUG_YSCHED yLOG_double  ("x_mins"    , x_mins);
   --rce;  if (x_mins < 0) {
      DEBUG_YSCHED yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YSCHED yLOG_double  ("x_mult"    , x_mult);
   x_mins *= x_mult;
   DEBUG_YSCHED yLOG_double  ("x_mins"    , x_mins);
   /*---(complete)-----------------------*/
   DEBUG_YSCHED yLOG_exit    (__FUNCTION__);
   return (int) x_mins;
}




