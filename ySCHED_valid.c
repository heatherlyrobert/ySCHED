/*============================----beg-of-source---============================*/
#include   "ySCHED.h"
#include   "ySCHED_priv.h"


/*> char        g_beg       [LEN_TERSE] = "00.01.01";                                 <*/
/*> char        g_end       [LEN_TERSE] = "50.01.01";                                 <*/


static int   s_bvalid   =  365;
static int   s_evalid   = -100;



/*====================------------------------------------====================*/
/*===----                            effective                         ----===*/
/*====================------------------------------------====================*/
static void      o___EFFECTIVE_______________o (void) {;}

/*> char                                                                              <* 
 *> ysched_valid__fill      ()                                                        <* 
 *> {                                                                                 <* 
 *>    int         i           =    0;                                                <* 
 *>    for (i = 0; i < LEN_VALID; ++i) {                                              <* 
 *>       if (i < s_bvalid + 100)      continue;                                      <* 
 *>       if (i > s_evalid + 100)      continue;                                      <* 
 *>       mySCHED.valid [i] = '1';                                                    <* 
 *>    }                                                                              <* 
 *>    return 0;                                                                      <* 
 *> }                                                                                 <*/

/*> char       /+----: make a printable version of the effective dates -----------+/   <* 
 *> ysched_valid__out     (void)                                                       <* 
 *> {                                                                                  <* 
 *>    int         n           =   -1;                                                 <* 
 *>    int         i           =    0;                                                 <* 
 *>    ysched_valid__fill ();                                                          <* 
 *>    mySCHED.effout [++n] = '<';                                                     <* 
 *>    for (i =  0; i <  29; ++i)  mySCHED.effout [++n] = mySCHED.valid [i +  71];     <* 
 *>    mySCHED.effout [++n] = ' ';                                                     <* 
 *>    mySCHED.effout [++n] = mySCHED.valid [100];                                     <* 
 *>    mySCHED.effout [++n] = ' ';                                                     <* 
 *>    for (i =  0; i <  29; ++i)  mySCHED.effout [++n] = mySCHED.valid[i + 101];      <* 
 *>    mySCHED.effout [++n] = '>';                                                     <* 
 *>    mySCHED.effout [++n] = '\0';                                                    <* 
 *>    return 0;                                                                       <* 
 *> }                                                                                  <*/

/*> char       /+----: initialize the effective range ----------------------------+/   <* 
 *> ysched_valid__init      (void)                                                     <* 
 *> {                                                                                  <* 
 *>    /+---(locals)-------------------------+/                                        <* 
 *>    int       i         = 0;            /+ loop iterator -- day                +/   <* 
 *>    /+---(initialize)---------------------+/                                        <* 
 *>    ystrlcpy (g_beg, "50.01.01", LEN_TERSE);                                         <* 
 *>    ystrlcpy (g_end, "00.01.01", LEN_TERSE);                                         <* 
 *>    s_bvalid =  365;                                                                <* 
 *>    s_evalid = -100;                                                                <* 
 *>    for (i = 0; i < LEN_VALID; ++i)  mySCHED.valid [i] = '_';                       <* 
 *>    ysched_valid__out();                                                            <* 
 *>    /+---(complete)-----------------------+/                                        <* 
 *>    return 0;                                                                       <* 
 *> }                                                                                  <*/

/*> char                                                                              <* 
 *> ysched_valid__wipe      (void)                                                    <* 
 *> {                                                                                 <* 
 *>    ysched_valid__init ();                                                         <* 
 *>    ysched_valid__out  ();                                                         <* 
 *> }                                                                                 <*/

/*> char       /+----: update effective for a endpoint dates ---------------------+/            <* 
 *> ysched_valid__end       (char *a_date, char a_side, long a_now)                             <* 
 *> {                                                                                           <* 
 *>    /+---(locals)-------------------------+/                                                 <* 
 *>    char      rce       =  -10;                                                              <* 
 *>    char      x_date    [LEN_TERSE] = "";                                                    <* 
 *>    char     *p         = NULL;         /+ token pointer for strtok            +/            <* 
 *>    long      rc        = 0;            /+ simple return code                  +/            <* 
 *>    tTIME     d;                        /+ broken down time                    +/            <* 
 *>    long      e         = 0;            /+ unix/epoch time                     +/            <* 
 *>    long      x_off     = 0;            /+ calculated offset                   +/            <* 
 *>    int       i         = 0;            /+ loop iterator -- day                +/            <* 
 *>    /+---(header)-------------------------+/                                                 <* 
 *>    DEBUG_YSCHED yLOG_enter   (__FUNCTION__);                                                <* 
 *>    DEBUG_YSCHED yLOG_char    ("a_side"    , a_side);                                        <* 
 *>    e_field   = 0;                                                                           <* 
 *>    e_section = 0;                                                                           <* 
 *>    DEBUG_YSCHED yLOG_point   ("a_date"    , a_date);                                        <* 
 *>    --rce;  if (a_date == NULL) {                                                            <* 
 *>       ysched__trouble ("effend" , __LINE__, "date is null"   , -1, 0);                      <* 
 *>       DEBUG_YSCHED yLOG_exitr (__FUNCTION__, rce);                                          <* 
 *>       return rce;                                                                           <* 
 *>    }                                                                                        <* 
 *>    DEBUG_YSCHED yLOG_info    ("a_date"    , a_date);                                        <* 
 *>    ystrlcpy (x_date, a_date, LEN_TERSE);                                                     <* 
 *>    /+---(field offset)-------------------+/                                                 <* 
 *>    e_section = 0;                                                                           <* 
 *>    e_field   = a_date - mySCHED.x_raw;                                                      <* 
 *>    if (e_field >= LEN_RECD)  e_field = 0;                                                   <* 
 *>    DEBUG_YSCHED yLOG_value   ("e_field"   , e_field);                                       <* 
 *>    /+---(defense)------------------------+/                                                 <* 
 *>    --rce;  if (YSCHED_NOT_READY) {                                                          <* 
 *>       ysched__trouble ("effend" , __LINE__, "epoch not set"  , -1, 0);                      <* 
 *>       DEBUG_YSCHED yLOG_exitr (__FUNCTION__, rce);                                          <* 
 *>       return rce;                                                                           <* 
 *>    }                                                                                        <* 
 *>    /+---(initialize date)----------------+/                                                 <* 
 *>    if (a_now < 0)   a_now = mySCHED.s_epoch;                                                <* 
 *>    localtime_r (&a_now, &d);                                                                <* 
 *>    /+---(beg date)-----------------------+/                                                 <* 
 *>    --rce;  if (a_side == 'b' && strcmp (x_date, "<") == 0) {                                <* 
 *>       ystrlcpy (x_date, "00.01.01", LEN_TERSE);                                              <* 
 *>       strptime (x_date, "%y.%m.%d", &d);                                                    <* 
 *>    } else if (a_side == 'b' && strcmp (x_date, "-") == 0) {                                 <* 
 *>       ystrlcpy (x_date, "50.01.01", LEN_TERSE);                                              <* 
 *>       strptime (x_date, "%y.%m.%d", &d);                                                    <* 
 *>    } else if (a_side == 'e' && strcmp (a_date, ">") == 0) {                                 <* 
 *>       ystrlcpy (x_date, "50.01.01", LEN_TERSE);                                              <* 
 *>       strptime (x_date, "%y.%m.%d", &d);                                                    <* 
 *>    } else if (a_side == 'e' && strcmp (x_date, "-") == 0) {                                 <* 
 *>       ystrlcpy (x_date, "00.01.01", LEN_TERSE);                                              <* 
 *>       strptime (x_date, "%y.%m.%d", &d);                                                    <* 
 *>    } else if (strlen (x_date) == 8) {                                                       <* 
 *>       p = strptime (x_date, "%y.%m.%d", &d);                                                <* 
 *>       if (p == NULL || p [0] != '\0') {                                                     <* 
 *>          ysched__trouble ("effend" , __LINE__, "bad date"       ,  0, strlen (x_date));     <* 
 *>          DEBUG_YSCHED yLOG_exitr (__FUNCTION__, rce);                                       <* 
 *>          return rce;                                                                        <* 
 *>       }                                                                                     <* 
 *>    } else {                                                                                 <* 
 *>       ysched__trouble ("effend" , __LINE__, "wrong length"   ,  0, strlen (x_date));        <* 
 *>       DEBUG_YSCHED yLOG_exitr (__FUNCTION__, rce);                                          <* 
 *>       return rce;                                                                           <* 
 *>    }                                                                                        <* 
 *>    e      = mktime  (&d);                                                                   <* 
 *>    DEBUG_YSCHED yLOG_value   ("a_now"     , a_now);                                         <* 
 *>    DEBUG_YSCHED yLOG_value   ("e"         , e);                                             <* 
 *>    /+> if (d.tm_isdst == 1)  e -= HOUR2SEC;                                           <+/   <* 
 *>    x_off = (e - a_now) / DAY2SEC;                                                           <* 
 *>    if (x_off < -100) x_off = -100;                                                          <* 
 *>    if (x_off >  365) x_off =  365;                                                          <* 
 *>    DEBUG_YSCHED yLOG_value   ("x_off"     , x_off);                                         <* 
 *>    switch (a_side) {                                                                        <* 
 *>    case 'b' :                                                                               <* 
 *>       ystrlcpy (g_beg, x_date, LEN_TERSE);                                                   <* 
 *>       s_bvalid = x_off;                                                                     <* 
 *>       break;                                                                                <* 
 *>    case 'e' :                                                                               <* 
 *>       ystrlcpy (g_end, x_date, LEN_TERSE);                                                   <* 
 *>       s_evalid = x_off;                                                                     <* 
 *>       break;                                                                                <* 
 *>    }                                                                                        <* 
 *>    --rce;  if (a_side == 'e' && s_bvalid > s_evalid) {                                      <* 
 *>       if (s_bvalid != 365 || s_evalid != -100) {                                            <* 
 *>          ysched__trouble ("effend" , __LINE__, "backward range" ,  0, strlen (a_date));     <* 
 *>          DEBUG_YSCHED yLOG_exitr (__FUNCTION__, rce);                                       <* 
 *>          return rce;                                                                        <* 
 *>       }                                                                                     <* 
 *>    }                                                                                        <* 
 *>    /+---(complete)-----------------------+/                                                 <* 
 *>    DEBUG_YSCHED yLOG_exit    (__FUNCTION__);                                                <* 
 *>    return 0;                                                                                <* 
 *> }                                                                                           <*/

/*> char       /+----: set the effective date range ------------------------------+/            <* 
 *> ySCHED_valid            (cchar *a_recd)                                                     <* 
 *> {                                                                                           <* 
 *>    /+---(locals)-------------------------+/                                                 <* 
 *>    char        rce         =  -10;                                                          <* 
 *>    char        rc          =    0;                                                          <* 
 *>    char       *p           = NULL;                                                          <* 
 *>    char       *r           = NULL;                                                          <* 
 *>    char       *x_beg       = NULL;                                                          <* 
 *>    char       *x_end       = NULL;                                                          <* 
 *>    char       *x_except    = NULL;                                                          <* 
 *>    int         i           = 0;                                                             <* 
 *>    /+---(header)-------------------------+/                                                 <* 
 *>    DEBUG_YSCHED yLOG_enter   (__FUNCTION__);                                                <* 
 *>    /+---(default)------------------------+/                                                 <* 
 *>    ysched__trouble ("-"      , 0       , "-"              , -1, 0);                         <* 
 *>    /+---(initialize)---------------------+/                                                 <* 
 *>    ysched_valid__wipe ();                                                                   <* 
 *>    /+---(defense)------------------------+/                                                 <* 
 *>    --rce;  if (a_recd == NULL) {                                                            <* 
 *>       ysched_valid__wipe ();                                                                <* 
 *>       ysched__trouble ("valid"  , __LINE__, "null input"     , -1, 0);                      <* 
 *>       DEBUG_YSCHED yLOG_exitr (__FUNCTION__, rce);                                          <* 
 *>       return rce;                                                                           <* 
 *>    }                                                                                        <* 
 *>    /+---(prepare)------------------------+/                                                 <* 
 *>    strncpy  (mySCHED.full, a_recd  , LEN_RECD);                                             <* 
 *>    strncpy  (mySCHED.x_raw, a_recd  , LEN_RECD);                                            <* 
 *>    /+---(defense)------------------------+/                                                 <* 
 *>    --rce;  if (YSCHED_NOT_READY) {                                                          <* 
 *>       ysched_valid__wipe ();                                                                <* 
 *>       ysched__trouble ("valid"  , __LINE__, "epoch not set"  , -1, 0);                      <* 
 *>       ysched_fancify ();                                                                    <* 
 *>       DEBUG_YSCHED yLOG_exitr (__FUNCTION__, rce);                                          <* 
 *>       return rce;                                                                           <* 
 *>    }                                                                                        <* 
 *>    /+---(break it down)------------------+/                                                 <* 
 *>    p = strtok_r (mySCHED.x_raw, " ", &r);                                                   <* 
 *>    --rce;  if (p == NULL || strcmp (p, ".valid") != 0) {                                    <* 
 *>       ysched_valid__wipe ();                                                                <* 
 *>       ysched__trouble ("valid"  , __LINE__, "prefix wrong"   , -1, 0);                      <* 
 *>       ysched_fancify ();                                                                    <* 
 *>       DEBUG_YSCHED yLOG_exitr (__FUNCTION__, rce);                                          <* 
 *>       return rce;                                                                           <* 
 *>    }                                                                                        <* 
 *>    /+---(beginning)----------------------+/                                                 <* 
 *>    x_beg  = strtok_r (NULL, " ", &r);                                                       <* 
 *>    if (strcmp (x_beg, "always") == 0) {                                                     <* 
 *>       rc = ysched_valid__end ("<"  , 'b', mySCHED.s_epoch);                                 <* 
 *>    } else  {                                                                                <* 
 *>       rc = ysched_valid__end (x_beg, 'b', mySCHED.s_epoch);                                 <* 
 *>    }                                                                                        <* 
 *>    --rce;  if (rc < 0) {                                                                    <* 
 *>       ysched_valid__wipe ();                                                                <* 
 *>       ysched_fancify ();                                                                    <* 
 *>       DEBUG_YSCHED yLOG_exitr (__FUNCTION__, rce);                                          <* 
 *>       return rce;                                                                           <* 
 *>    }                                                                                        <* 
 *>    /+---(ending)-------------------------+/                                                 <* 
 *>    if (strcmp (x_beg, "always") == 0) {                                                     <* 
 *>       rc = ysched_valid__end (">"  , 'e', mySCHED.s_epoch);                                 <* 
 *>    } else {                                                                                 <* 
 *>       x_end  = strtok_r (NULL, " ", &r);                                                    <* 
 *>       rc = ysched_valid__end (x_end, 'e', mySCHED.s_epoch);                                 <* 
 *>    }                                                                                        <* 
 *>    --rce;  if (rc < 0) {                                                                    <* 
 *>       ysched_valid__wipe ();                                                                <* 
 *>       ysched_fancify ();                                                                    <* 
 *>       DEBUG_YSCHED yLOG_exitr (__FUNCTION__, rce);                                          <* 
 *>       return rce;                                                                           <* 
 *>    }                                                                                        <* 
 *>    /+---(print)--------------------------+/                                                 <* 
 *>    ysched_valid__out ();                                                                    <* 
 *>    ysched_fancify ();                                                                       <* 
 *>    /+> printf ("effective = <%s>\n", effout);                                         <+/   <* 
 *>    /+> yLOG_info  ("effective", effout );                                             <+/   <* 
 *>    /+---(complete)-----------------------+/                                                 <* 
 *>    DEBUG_YSCHED yLOG_exit    (__FUNCTION__);                                                <* 
 *>    return 0;                                                                                <* 
 *> }                                                                                           <*/

/*> char                                                                              <* 
 *> ySCHED_newfile         (void)                                                     <* 
 *> {                                                                                 <* 
 *>    char        rc          =    0;                                                <* 
 *>    DEBUG_YSCHED yLOG_enter   (__FUNCTION__);                                      <* 
 *>    rc = ySCHED_valid (YSCHED_ALWAYS);                                             <* 
 *>    DEBUG_YSCHED yLOG_exit    (__FUNCTION__);                                      <* 
 *>    return rc;                                                                     <* 
 *> }                                                                                 <*/


/*====================------------------------------------====================*/
/*===----                         unit testing                         ----===*/
/*====================------------------------------------====================*/
static void      o___UNITTEST________________o (void) {;};

/*> char*      /+----: unit testing accessor for clean validation interface ------+/                   <* 
 *> ysched_valid__unit      (char *a_question)                                                         <* 
 *> {                                                                                                  <* 
 *>    char        t           [LEN_HUND]  = "";                                                       <* 
 *>    char        s           [LEN_HUND]  = "";                                                       <* 
 *>    /+---(detailed parsing)---------------+/                                                        <* 
 *>    strncpy (unit_answer, "unknown request", 100);                                                  <* 
 *>    if        (strcmp(a_question, "effective"    ) == 0) {                                          <* 
 *>       sprintf(unit_answer, "VALID effective  : %-8.8s %s %-8.8s", g_beg, mySCHED.effout, g_end);   <* 
 *>    }                                                                                               <* 
 *>    /+---(complete)-----------------------+/                                                        <* 
 *>    return unit_answer;                                                                             <* 
 *> }                                                                                                  <*/


