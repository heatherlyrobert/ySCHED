/*============================----beg-of-source---============================*/
#include   "ySCHED.h"
#include   "ySCHED_priv.h"




#define   MAX_FIELDS      10
struct {
   char        label       [LEN_LABEL];
   int         min;
   int         max;
} s_fields [MAX_FIELDS] = {
   /* 12345678901234   123   123   */
   { "minutes"       ,   0 ,  59   },
   { "hours"         ,   0 ,  23   },
   { "days"          ,   1 ,  31   },
   { "months"        ,   1 ,  12   },
   { "daysofweek"    ,   1 ,   7   },
   { "weeks"         ,   0 ,  53   },
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
   { "years"         , "*"             ,   0 ,  99,  '-',  '-'   },
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
   { "daysofweek"    , "O"             ,   6 ,   7,  '-',  '-'   },
   { "daysofweek"    , "Off"           ,   6 ,   7,  '-',  '-'   },
   /*--- hours : ranges ----------------------------------------*/
   { "hours"         , "*"             ,   0 ,  23,  '-',  '-'   },
   { "hours"         , "W"             ,   8 ,  16,  '-',  '-'   },
   { "hours"         , "Work"          ,   8 ,  16,  '-',  '-'   },
   { "hours"         , "O"             ,   8 ,  16,  '-',  'y'   },
   { "hours"         , "Off"           ,   8 ,  16,  '-',  'y'   },
   { "hours"         , "B"             ,   5 ,  22,  '-',  'y'   },
   { "hours"         , "Batch"         ,   5 ,  22,  '-',  'y'   },
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



/*====================------------------------------------====================*/
/*===----                       parsing support                        ----===*/
/*====================------------------------------------====================*/
static void      o___SUPPORT_________________o (void) {;};

char         /*--: interpret modifier --------------------[ leaf   [ ------ ]-*/
ysched__limits          (char a_type)
{
   char        rce         =  -10;
   /*---(default)------------------------*/
   s_type      = -1;
   strlcpy (s_label, "-", LEN_LABEL);
   s_min       = 0;
   s_max       = 0;
   s_tmax      = 0;
   /*---(defense)------------------------*/
   --rce;  if (YSCHED_NOT_READY) {
      ysched__trouble (""       , __LINE__, "epoch unset", 0, 0);
      return rce;
   }
   --rce;  if (a_type <  0) {
      ysched__trouble (""       , __LINE__, "type not valid", 0, 0);
      return rce;
   }
   --rce;  if (a_type >= MAX_FIELDS)  return rce;
   --rce;  if (s_fields [a_type].label [0] == '-')  return rce;
   /*---(set full limits)----------------*/
   s_type      = a_type;
   strlcpy (s_label, s_fields [a_type].label, LEN_LABEL);
   s_min       = s_fields [a_type].min;
   s_max       = s_fields [a_type].max;
   s_tmax      = s_max;
   /*---(overrides)----------------------*/
   if      (TYPE_DYS) {
      s_max = mySCHED.s_dim;
   }
   else if (TYPE_WKS) {
      s_min = mySCHED.s_wze;
      s_max = mySCHED.s_wiy;
   }
   /*---(complete)-----------------------*/
   return 0;
}

char         /*--: interpret modifier --------------------[ leaf   [ ------ ]-*/
ysched__prep       (char *p)
{
   char        rce         =  -10;
   /*---(default)------------------------*/
   s_beg       =  -1;
   s_end       =  -1;
   s_not       = '-';
   s_rev       = '-';
   s_inv       = '-';
   s_flp       = '-';
   s_stp       =   1;
   strlcpy (s_section, "", LEN_HUND);
   strlcpy (s_input  , "", LEN_HUND);
   s_len       =   0;
   s_ptr       = s_input;
   /*---(defense)------------------------*/
   --rce;  if (YSCHED_NOT_READY) {
      ysched__trouble (""       , __LINE__, "epoch unset", 0, 0);
      return rce;
   }
   --rce;  if (p == NULL)   return rce;
   /*---(measure input string)-----------*/
   strlcpy (s_section, p, LEN_HUND);
   strlcpy (s_input  , p, LEN_HUND);
   DEBUG_YSCHED yLOG_info    ("s_input"   , s_input);
   s_len       = strlen (s_input);
   DEBUG_YSCHED yLOG_value   ("s_len"     , s_len);
   --rce;  if (s_len <= 0)  return rce;
   /*---(section offset)-----------------*/
   if (strcmp (mySCHED.x_field, "") == 0)  e_section = 0;
   else {
      e_section = p - mySCHED.x_field;
      if (e_section >= LEN_HUND)   e_section = 0;
   }
   DEBUG_YSCHED yLOG_point   ("e_section" , e_section);
   /*---(complete)-----------------------*/
   return 0;
}

char         /*--: apply grammar to array ----------------[ ------ [ ------ ]-*/
ysched__empty           (char *a_array)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;
   int         i           =    0;
   --rce;  if (a_array == NULL)  return rce;
   for (i = 0;         i <  s_min;  ++i) a_array   [i]    = '.';
   for (i = s_min;     i <= s_max;  ++i) a_array   [i]    = '_';
   for (i = s_max + 1; i <= s_tmax; ++i) a_array   [i]    = '.';
   a_array [s_tmax + 1] = '\0';
   return 0;
}

char         /*--: apply grammar to array ----------------[ ------ [ ------ ]-*/
ysched__apply           (char *a_array)
{
   /*---(locals)-----------+-----------+-*/
   char        x_mark      = '1';
   int         x_temp      =  0;
   int         i           =  0;
   int         n           =  0;
   int         c           =  0;
   /*---(check for not)------------------*/
   if (s_not == 'y')      x_mark = '_';
   /*---(defense)------------------------*/
   /*> if (s_beg < s_min ) s_beg = s_min;                                             <* 
    *> if (s_beg > s_max ) s_beg = s_max;                                             <* 
    *> if (s_end < s_min ) s_end = s_min;                                             <* 
    *> if (s_end > s_max)  s_end = s_max;                                             <*/
   /*---(update normal)------------------*/
   /*> printf ("\n");                                                                 <*/
   /*> printf ("%2dn, %2db, %2de, %2dx, %2d/, %c %c %c %c\n", s_min, s_beg, s_end, s_max, s_stp, s_not, s_rev, s_inv, s_flp);   <*/
   for (i = 0; i <= s_max; ++i) {
      /*> printf ("  %3di, %3d, %3d\n", i, (i - s_beg), (i - s_beg) % s_stp);         <*/
      if (s_rev != 'y' && i < s_min) continue;
      if ((i - s_beg) % s_stp != 0)  continue;
      if      (s_inv != 'y' && (i <  s_beg || i >  s_end))  continue;
      else if (s_inv == 'y' && (i >= s_beg && i <= s_end))  continue;
      /*> printf ("    %3di, %3d\n", i, s_max - i);                                   <*/
      if (s_rev == 'y')   n = s_max - i;
      else                n = i;
      a_array [n] = x_mark;
      ++c;
   }
   /*---(complete)-----------------------*/
   return c;
}



/*====================------------------------------------====================*/
/*===----                      modifiers                               ----===*/
/*====================------------------------------------====================*/
static void      o___MODS____________________o (void) {;};

char         /*--: interpret modifier --------------------[ leaf   [ ------ ]-*/
ysched__modify          (void)
{  
   /*---(design rules)--------------------*/
   /*
    *  there can only be one modifier per range specifier and it must appear
    *  as the very first character
    *         none         turn each element on
    *     ~   not          turn off each element, instead of turning it on
    *     ^   reverse      count from the end, rather than the start
    *     !   inverse      turn on each element not listed, meaning, all but
    *
    */
   char        rce         =  -10;
   char        c           =    0;
   /*---(default)------------------------*/
   ysched__trouble ("-"      , 0       , "-"              , -1, 0);
   /*---(defense)------------------------*/
   --rce;  if (YSCHED_NOT_READY) {
      ysched__trouble ("modify" , __LINE__, "epoch not set"  , -1, 0);
      return rce;
   }
   /*---(parse)--------------------------*/
   while (s_ptr [0] != '\0') {
      /*---(filter)----------------------*/
      if (strchr ("~^!|", s_ptr [0]) == NULL)  break;
      /*---(parse)-----------------------*/
      if (s_ptr [0] == '!')  s_not = 'y';
      if (s_ptr [0] == '^')  s_rev = 'y';
      if (s_ptr [0] == '~')  s_inv = 'y';
      if (s_ptr [0] == '|')  s_flp = 'y';
      /*---(next)------------------------*/
      ++s_ptr;
      --s_len;
      ++c;
   }
   /*---(complete)-----------------------*/
   return c;
}

char         /*--: interpret step ------------------------[ leaf   [ ------ ]-*/
ysched__step       (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char       *p           = NULL;
   int         l           =    0;
   int         i           =    0;
   int         x_num       =   -1;
   char        e           [LEN_LABEL] = "";
   char        n           =    0;
   /*---(default)------------------------*/
   ysched__trouble ("-"      , 0       , "-"              , -1, 0);
   /*---(defense)------------------------*/
   --rce;  if (YSCHED_NOT_READY) {
      ysched__trouble ("step"   , __LINE__, "epoch not set"  , -1, 0);
      return rce;
   }
   /*---(find marker)--------------------*/
   /*> printf ("%s\n", s_ptr);                                                        <*/
   p = strrchr (s_ptr, '/');
   if (p == NULL)   return 0;
   --rce;  if (p == s_ptr) {
      ysched__trouble ("step"   , __LINE__, "no base number" , n, 1);
      return rce;
   }
   n = p - s_ptr;
   --rce;  if (p [1] == '\0'   ) {
      ysched__trouble ("step"   , __LINE__, "no actual step" , n, 1);
      return rce;
   }
   p [0] = '\0';
   --s_len;
   /*---(check characters)--------------*/
   l = strlen (p + 1);
   /*> printf ("%2d[%s]\n", l, p + 1);                                                <*/
   --rce;  for (i = 1; i <= l; ++i) {
      if (strchr ("0123456789", p [i]) == NULL) {
         sprintf (e, "bad char (%c)", p [i]);
         ysched__trouble ("step"   , __LINE__, e                , n + i, 1);
         return rce;
      }
   }
   /*---(interpret)----------------------*/
   x_num = atoi (p + 1);
   --rce;  if (x_num <= 0) {
      sprintf (e, "too small <%d", 1);
      ysched__trouble ("step"   , __LINE__, e                , n + 1, strlen (p + 1));
      return rce;
   }
   --rce;  if (x_num > (s_max / 2)) {
      sprintf (e, "too large >%d", s_max / 2);
      ysched__trouble ("step"   , __LINE__, e                , n + 1, strlen (p + 1));
      return rce;
   }
   /*---(save back)----------------------*/
   s_stp = x_num;
   /*---(complete)-----------------------*/
   return 1;
}

int          /*--: interpret special day references ------[ leaf   [ ------ ]-*/
ysched__nearest    (void)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;
   int         rc          = 0;
   char        x_type      = '-';
   int         x_day       =  0;
   int         x_dow       =  0;
   int         a           =  0;
   char        e           [LEN_LABEL] = "";
   /*---(header)-------------------------*/
   DEBUG_YSCHED yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   --rce;  if (YSCHED_NOT_READY) {
      ysched__trouble ("nearest", __LINE__, "epoch not set", -1, 0);
      DEBUG_YSCHED yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(get type)-----------------------*/
   x_type = s_ptr [s_len - 1];
   DEBUG_YSCHED yLOG_char    ("x_type"    , x_type);
   --rce;  if (strchr ("abnABN", x_type) == NULL) {
      ysched__trouble ("nearest", __LINE__, "bad or no type" , s_len - 1, 1);
      DEBUG_YSCHED yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (s_type != PARSE_DYS) {
      ysched__trouble ("nearest", __LINE__, "only for days"  , 0, 0);
      DEBUG_YSCHED yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(get day)------------------------*/
   s_ptr [--s_len] = '\0';
   if (strcmp ("", s_ptr) == 0)  {
      ysched__trouble ("nearest", __LINE__, "no base value"  , 0, 0);
      DEBUG_YSCHED yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YSCHED yLOG_info    ("s_ptr"     , s_ptr);
   if (strcmp ("L", s_ptr) == 0)  x_day = mySCHED.s_dim;
   else                           x_day = ysched__number (s_ptr);
   DEBUG_YSCHED yLOG_value   ("x_day"     , x_day);
   --rce;  if (x_day < 0) {
      DEBUG_YSCHED yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(determine day of week)----------*/
   x_dow = (((x_day + mySCHED.s_fdow) - 1) % 7);
   if (x_dow == 0) x_dow = 7;
   DEBUG_YSCHED yLOG_value   ("x_dow"     , x_dow);
   /*---(help with nearist)--------------*/
   if (x_type == 'N') {
      if (x_dow >= 4)  x_type = 'A';
      else             x_type = 'B';
   }
   if (x_type == 'n') {
      if (x_dow == 7)  x_type = 'a';
      else             x_type = 'b';
   }
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
   case 'A' :  /* after type, so push to next saturday */
      if (x_dow >= 1 && x_dow <= 5) {
         x_day += 6 - x_dow;
         x_dow  = 6;
      }
      if (x_day >  mySCHED.s_dim)   { x_day -= 6; x_dow = 7; }  /* last sun   */
      break;
   case 'B' :  /* before type, so push to previous sunday */
      if (x_dow >= 1 && x_dow <= 5) {
         x_day -= x_dow;
         x_dow  = 7;
      }
      if (x_day <= 0)               { x_day += 6; x_dow = 6; }  /* next sat   */
      break;
   }
   /*---(save)---------------------------*/
   s_beg = s_end = x_day;
   DEBUG_YSCHED yLOG_value   ("s_beg"     , s_beg);
   /*---(complete)-----------------------*/
   DEBUG_YSCHED yLOG_exit    (__FUNCTION__);
   return 1;
}



/*====================------------------------------------====================*/
/*===----                       base values                            ----===*/
/*====================------------------------------------====================*/
static void      o___VALUES__________________o (void) {;};

int          /*--: interpret number ----------------------[ leaf   [ ------ ]-*/
ysched__number     (cchar *a_number)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         l           =    0;
   int         i           =    0;
   int         x_num       =   -1;
   char        e           [LEN_LABEL] = "";
   /*---(header)-------------------------*/
   DEBUG_YSCHED yLOG_senter  (__FUNCTION__);
   /*---(default)------------------------*/
   /*> printf ("   %-7.7s  %4d  %d\n", a_number, x_num, e_pos);                       <*/
   ysched__trouble ("-"      , 0       , "-"              , -1, 0);
   /*> printf ("   %-7.7s  %4d  %d\n", a_number, x_num, e_pos);                       <*/
   /*---(defense)------------------------*/
   --rce;  if (YSCHED_NOT_READY) {
      ysched__trouble ("number" , __LINE__, "epoch not set"  , -1, 0);
      DEBUG_YSCHED yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (a_number     == NULL) {
      ysched__trouble ("number" , __LINE__, "NULL input"     , 0, 0);
      DEBUG_YSCHED yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (a_number [0] == '\0') {
      ysched__trouble ("number" , __LINE__, "empty input"    , 0, 0);
      DEBUG_YSCHED yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YSCHED yLOG_snote   (a_number);
   /*---(defense: non-numeric)----------*/
   /*> printf ("   %-7.7s  %4d  %d\n", a_number, x_num, e_pos);                       <*/
   l = strlen (a_number);
   DEBUG_YSCHED yLOG_sint    (l);
   --rce;  for (i = 0; i < l; ++i) {
      if (strchr ("0123456789", a_number [i]) == NULL) {
         sprintf (e, "bad char (%c)", a_number [i]);
         ysched__trouble ("number" , __LINE__, e                , i, 1);
         DEBUG_YSCHED yLOG_sexitr  (__FUNCTION__, rce);
         return rce;
      }
   }
   /*> printf ("   %-7.7s  %4d  %d\n", a_number, x_num, e_pos);                       <*/
   /*---(interpret)----------------------*/
   x_num = atoi (a_number);
   DEBUG_YSCHED yLOG_sint    (x_num);
   /*> printf ("   %-7.7s  %4d  %d\n", a_number, x_num, e_pos);                       <*/
   /*---(defense)------------------------*/
   --rce;  if (x_num == 0 && strcmp ("0", a_number) != 0) {
      ysched__trouble ("number" , __LINE__, "not real zero"  , 0, strlen (a_number));
      DEBUG_YSCHED yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (x_num <  0) {
      ysched__trouble ("number" , __LINE__, "negative"       , 0, strlen (a_number));
      DEBUG_YSCHED yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(check ranges)-------------------*/
   --rce;  if (s_rev != 'y' && x_num < s_min) {
      sprintf (e, "too small <%d", s_min);
      ysched__trouble ("number" , __LINE__, e                , 0, strlen (a_number));
      DEBUG_YSCHED yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (s_rev != 'y' && x_num > s_max) {
      sprintf (e, "too large >%d", s_max);
      ysched__trouble ("number" , __LINE__, e                , 0, strlen (a_number));
      DEBUG_YSCHED yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (s_rev == 'y' && x_num > s_max - 1) {
      sprintf (e, "too large >%d", s_max - 1);
      ysched__trouble ("number" , __LINE__, e                , 0, strlen (a_number));
      DEBUG_YSCHED yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*> printf ("   %-7.7s  %4d  %d\n", a_number, x_num, e_pos);                       <*/
   /*---(complete)-----------------------*/
   DEBUG_YSCHED yLOG_sint    (x_num);
   DEBUG_YSCHED yLOG_sexit   (__FUNCTION__);
   return x_num;
}

char         /*--: interpret constants -------------------[ leaf   [ ------ ]-*/
ysched__const           (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         i           =    0;
   /*---(default)------------------------*/
   ysched__trouble ("-"      , 0       , "-"              , -1, 0);
   /*---(defense)------------------------*/
   --rce;  if (YSCHED_NOT_READY) {
      ysched__trouble ("const"  , __LINE__, "epoch not set"  , -1, 0);
      return rce;
   }
   /*---(look for constants)-------------*/
   while (s_const [i].field [0] != '-') {
      /*---(fast checks)-----------------*/
      if (s_const [i].field [0] != s_label [0]) {
         ++i;
         continue;
      }
      if (s_const [i].label [0] != s_ptr [0]) {
         ++i;
         continue;
      }
      /*---(slow checks)-----------------*/
      if (strcmp (s_const [i].field, s_label) != 0) {
         ++i;
         continue;
      }
      if (strcmp (s_const [i].label, s_ptr) != 0) {
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
   return 1;
}



/*====================------------------------------------====================*/
/*===----                       number ranges                          ----===*/
/*====================------------------------------------====================*/
static void      o___RANGES__________________o (void) {;};

char
ysched__lesser          (void)
{
   char        rce         =  -10;
   int         x_num       =    0;
   /*---(header)-------------------------*/
   DEBUG_YSCHED yLOG_enter   (__FUNCTION__);
   /*---(default)------------------------*/
   ysched__trouble ("-"      , 0       , "-"              , -1, 0);
   /*> printf ("%-10.10s  %4d  %d\n", s_ptr, x_num, e_pos);                           <*/
   /*---(defense)------------------------*/
   --rce;  if (YSCHED_NOT_READY) {
      ysched__trouble ("lesser" , __LINE__, "epoch not set"  , -1, 0);
      DEBUG_YSCHED yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*> printf ("[%s]\n", s_ptr);                                                      <*/
   --rce;  if (s_ptr [0] != '<') {
      ysched__trouble ("lesser" , __LINE__, "no lead <"      , 0, 1);
      DEBUG_YSCHED yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   s_ptr [0] = '\0';
   ++s_ptr;
   --rce;  if (s_ptr [0] == '\0') {
      ysched__trouble ("lesser" , __LINE__, "no base value"  , 0, 1);
      DEBUG_YSCHED yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*> printf (" %-9.9s  %4d  %d\n", s_ptr, x_num, e_pos);                            <*/
   x_num = ysched__number (s_ptr);
   --rce;  if (x_num <  0) {
      ysched__trouble (e_func, e_line, e_issue, e_pos + 1, e_len);
      DEBUG_YSCHED yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   s_beg = 0;
   s_end = x_num;
   /*> printf (" %-9.9s  %4d  %d\n", s_ptr, x_num, e_pos);                            <*/
   DEBUG_YSCHED yLOG_exit    (__FUNCTION__);
   return 2;
}

char
ysched__greater         (void)
{
   char        rce         =  -10;
   int         x_num       =    0;
   /*---(header)-------------------------*/
   DEBUG_YSCHED yLOG_enter   (__FUNCTION__);
   /*---(default)------------------------*/
   ysched__trouble ("-"      , 0       , "-"              , -1, 0);
   /*---(defense)------------------------*/
   --rce;  if (YSCHED_NOT_READY) {
      ysched__trouble ("greater", __LINE__, "epoch not set"  , -1, 0);
      DEBUG_YSCHED yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (s_ptr [s_len - 1] != '>') {
      ysched__trouble ("greater", __LINE__, "no trail >"     , s_len - 1, 1);
      DEBUG_YSCHED yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   s_ptr [--s_len] = '\0';
   --rce;  if (s_ptr [0] == '\0') {
      ysched__trouble ("greater", __LINE__, "no base value"  , 0, 1);
      DEBUG_YSCHED yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   x_num = ysched__number (s_ptr);
   /*> printf ("[%s]  %d\n", s_ptr, x_num);                                           <*/
   --rce;  if (x_num <  0) {
      DEBUG_YSCHED yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   s_beg = x_num;
   s_end = 99;
   DEBUG_YSCHED yLOG_exit    (__FUNCTION__);
   return 2;
}

char
ysched__between         (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char       *p           = NULL;
   int         x_num       =    0;
   int         n           =    0;
   int         l           =    0;
   /*---(header)-------------------------*/
   DEBUG_YSCHED yLOG_enter   (__FUNCTION__);
   /*---(default)------------------------*/
   ysched__trouble ("-"      , 0       , "-"              , -1, 0);
   /*---(defense)------------------------*/
   --rce;  if (YSCHED_NOT_READY) {
      ysched__trouble ("between", __LINE__, "epoch not set"  , -1, 0);
      DEBUG_YSCHED yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   l = strlen (s_ptr);
   p = strchr (s_ptr, '-');
   --rce;  if (p == NULL) {
      ysched__trouble ("between", __LINE__, "no infix -"     , 0, 1);
      DEBUG_YSCHED yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   p [0] = '\0';
   /*---(beginning)----------------------*/
   --rce;  if (s_ptr [0] == '\0') {
      ysched__trouble ("between", __LINE__, "no beg value"   , 0, 1);
      DEBUG_YSCHED yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   x_num = ysched__number (s_ptr);
   /*> printf ("[%s]  %d\n", s_ptr, x_num);                                           <*/
   --rce;  if (x_num <  0) {
      ysched__trouble (e_func, e_line, e_issue, e_pos, e_len);
      DEBUG_YSCHED yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   s_beg = x_num;
   /*---(ending)-------------------------*/
   n = p - s_ptr;
   --rce;  if (p [1] == '\0') {
      ysched__trouble ("between", __LINE__, "no end value"   , n, 1);
      DEBUG_YSCHED yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   x_num = ysched__number (p + 1);
   /*> printf ("[%s]  %d\n", p + 1, x_num);                                           <*/
   --rce;  if (x_num <  0) {
      ysched__trouble (e_func, e_line, e_issue, n + 1 + e_pos, e_len);
      s_beg = -1;
      DEBUG_YSCHED yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   s_end = x_num;
   /*---(range trouble)------------------*/
   --rce;  if (s_beg > s_end) {
      ysched__trouble ("between", __LINE__, "beg value > end", 0, l);
      s_beg = s_end = -1;
      DEBUG_YSCHED yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_YSCHED yLOG_exit    (__FUNCTION__);
   return 3;
}

char         /*--: interpret ranges ----------------------[ ------ [ ------ ]-*/
ysched__range           (void)
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
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_YSCHED yLOG_enter   (__FUNCTION__);
   /*---(default)------------------------*/
   ysched__trouble ("-"      , 0       , "-"              , -1, 0);
   /*---(defense)------------------------*/
   --rce;  if (YSCHED_NOT_READY) {
      ysched__trouble ("range"  , __LINE__, "epoch not set"  , -1, 0);
      DEBUG_YSCHED yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(handle lesser)------------------*/
   --rce;  if (s_ptr [0] == '<') {
      rc = ysched__lesser  ();
      if (rc < 0) {
         DEBUG_YSCHED yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      DEBUG_YSCHED yLOG_exit    (__FUNCTION__);
      return rc;
   }
   /*---(handle greater)-----------------*/
   --rce;  if (s_ptr [s_len - 1] == '>') {
      rc = ysched__greater ();
      if (rc < 0) {
         DEBUG_YSCHED yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      DEBUG_YSCHED yLOG_exit    (__FUNCTION__);
      return rc;
   }
   /*---(handle between)-----------------*/
   --rce;  if (strchr (s_ptr, '-') != NULL) {
      rc = ysched__between ();
      if (rc < 0) {
         DEBUG_YSCHED yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      DEBUG_YSCHED yLOG_exit    (__FUNCTION__);
      return rc;
   }
   /*---(handle postfix)-----------------*/
   --rce;  if (strchr ("abnABN", s_ptr [s_len - 1]) != NULL) {
      if (s_type != PARSE_DYS)  return rce;
      rc = ysched__nearest ();
      if (rc < 0) {
         DEBUG_YSCHED yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      DEBUG_YSCHED yLOG_exit    (__FUNCTION__);
      return rc;
   }
   /*---(handle simple value)------------*/
   rc = ysched__number (s_ptr);
   --rce;  if (rc    <  0) {
      DEBUG_YSCHED yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   s_beg = s_end = rc;
   --rce;  if (s_stp != 1) {
      DEBUG_YSCHED yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_YSCHED yLOG_exit    (__FUNCTION__);
   return 1;
}



/*====================------------------------------------====================*/
/*===----                     main drivers                             ----===*/
/*====================------------------------------------====================*/
static void      o___DRIVERS_________________o (void) {;};

char         /*--: interpret one field of grammar --------[ ------ [ ------ ]-*/
ysched__section    (cchar *a_sect, char *a_array)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_recd      [LEN_RECD]  = "";
   /*---(header)-------------------------*/
   DEBUG_YSCHED yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_YSCHED yLOG_point   ("a_sect"    , a_sect);
   --rce;  if (a_sect == NULL      ) {
      DEBUG_YSCHED yLOG_exitr (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YSCHED yLOG_info    ("a_sect"    , a_sect);
   strlcpy (x_recd, a_sect, LEN_RECD);
   /*---(prepare)------------------------*/
   rc = ysched__prep     (a_sect);
   DEBUG_YSCHED yLOG_value   ("prep"      , rc);
   if (rc <  0) {
      DEBUG_YSCHED yLOG_exitr (__FUNCTION__, rce);
      return rce;
   }
   /*---(prefix modifiers)---------------*/
   rc = ysched__modify ();
   DEBUG_YSCHED yLOG_value   ("modifier"  , rc);
   --rce;  if (rc <  0) {
      DEBUG_YSCHED yLOG_exitr (__FUNCTION__, rce);
      return rce;
   }
   /*---(suffix steps)-------------------*/
   rc = ysched__step     ();
   DEBUG_YSCHED yLOG_value   ("step"      , rc);
   --rce;  if (rc <  0)  {
      DEBUG_YSCHED yLOG_exitr (__FUNCTION__, rce);
      return rce;
   }
   /*---(constants)----------------------*/
   rc = ysched__const    ();
   DEBUG_YSCHED yLOG_value   ("const"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_YSCHED yLOG_exitr (__FUNCTION__, rce);
      return rce;
   }
   /*---(ranges)-------------------------*/
   --rce;  if (rc == 0)  {
      rc = ysched__range    ();
      DEBUG_YSCHED yLOG_value   ("range"     , rc);
      if (rc < 0)  {
         DEBUG_YSCHED yLOG_exitr (__FUNCTION__, rce);
         return rce;
      }
      DEBUG_YSCHED yLOG_value   ("s_beg"     , s_beg);
   }
   /*---(apply to array)-----------------*/
   rc = ysched__apply    (a_array);
   DEBUG_YSCHED yLOG_value   ("apply"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_YSCHED yLOG_exitr (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YSCHED yLOG_info    ("result"    , a_array);
   /*---(complete)-----------------------*/
   DEBUG_YSCHED yLOG_exit    (__FUNCTION__);
   return 1;
}

char         /*--: interpret one field of grammar --------[ ------ [ ------ ]-*/
ysched_field       (cchar *a_field, char *a_array, char a_type)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char       *p           = NULL;          /* strtok result pointer          */
   char       *q           = ",";           /* strtok delimiter               */
   char       *s           = NULL;          /* strtok context                 */
   char       *x_error     = "((error))";   /* default array contents         */
   int         c           = 0;
   char        x_array     [100];           /* working return values          */
   /*---(header)-------------------------*/
   DEBUG_YSCHED yLOG_enter   (__FUNCTION__);
   /*---(default)------------------------*/
   if (a_array != NULL)  a_array [0] = '\0';
   /*---(default)------------------------*/
   ysched__trouble ("-"      , 0       , "-"              , -1, 0);
   /*---(defense)------------------------*/
   --rce;  if (YSCHED_NOT_READY) {
      ysched__trouble ("number" , __LINE__, "epoch not set"  , -1, 0);
      DEBUG_YSCHED yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YSCHED yLOG_point   ("*a_array"  , a_array);
   --rce;  if (a_array == NULL      ) {
      DEBUG_YSCHED yLOG_exitr (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YSCHED yLOG_value   ("a_type"    , a_type);
   rc = ysched__limits   (a_type);
   DEBUG_YSCHED yLOG_value   ("limits"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_YSCHED yLOG_exitr (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YSCHED yLOG_point   ("a_field"   , a_field);
   --rce;  if (a_field == NULL)  {
      /*---(check for defaulting)-----------*/
      if (a_type == PARSE_WKS || a_type == PARSE_YRS) {
         strlcpy (mySCHED.x_field, "*", LEN_RECD);
      } else {
         DEBUG_YSCHED yLOG_exitr (__FUNCTION__, rce);
         return rce;
      }
   } else {
      strlcpy (mySCHED.x_field, a_field, LEN_RECD);
   }
   DEBUG_YSCHED yLOG_point   ("x_field"   , mySCHED.x_field);
   /*---(field offset)-------------------*/
   e_field = a_field - mySCHED.x_raw;
   if (e_field >= LEN_RECD)  e_field = 0;
   DEBUG_YSCHED yLOG_point   ("e_field"   , e_field);
   /*---(initialize arrays)--------------*/
   strcpy (mySCHED.last, x_error);
   rc = ysched__empty (x_array);
   DEBUG_YSCHED yLOG_info    ("initial"   , x_array);
   /*---(parse into sections)------------*/
   p = strtok_r (mySCHED.x_field, q, &s);
   --rce;
   while (p != NULL) {
      rc = ysched__section (p, x_array);
      DEBUG_YSCHED yLOG_value   ("section"   , rc);
      if (rc <  0) {
         DEBUG_YSCHED yLOG_exitr (__FUNCTION__, rce);
         return rce;
      }
      p = strtok_r (NULL, q, &s);
      ++c;
   }
   /*---(move to final)------------------*/
   strcpy (a_array , x_array);
   strcpy (mySCHED.last, a_array);
   DEBUG_YSCHED yLOG_info    ("final"     , a_array);
   /*---(complete)-----------------------*/
   DEBUG_YSCHED yLOG_exit    (__FUNCTION__);
   return c;
}





