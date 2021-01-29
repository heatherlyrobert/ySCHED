/*===[[ ONE_LINERS ]]=========================================================*/

#define     P_FOCUS     "PS (programming support)"
#define     P_NICHE     "sg (scheduling grammar)"
#define     P_SUBJECT   "kernighan scheduling grammar"
#define     P_PURPOSE   "simple, elegant, and powerful kernighan scheduling grammar"

#define     P_NAMESAKE  "eunomia-horae (correct moment)"
#define     P_HERITAGE  "goddesses of seasons and porportions of time"
#define     P_IMAGERY   ""
#define     P_REASON    ""

#define     P_ONELINE   P_NAMESAKE " " P_SUBJECT

#define     P_BASENAME  ""
#define     P_FULLPATH  ""
#define     P_SUFFIX    ""
#define     P_CONTENT   ""

#define     P_SYSTEM    "gnu/linux   (powerful, ubiquitous, technical, and hackable)"
#define     P_LANGUAGE  "ansi-c      (wicked, limitless, universal, and everlasting)"
#define     P_CODESIZE  "small       (appoximately 10,000 slocl)"
#define     P_DEPENDS   "none"

#define     P_AUTHOR    "heatherlyrobert"
#define     P_CREATED   "2011-03"

#define     P_VERMAJOR  "1.--, in production and general use"
#define     P_VERMINOR  "1.4-, clean, rework, and expand testing"
#define     P_VERNUM    "1.4c"
#define     P_VERTXT    "broke out field parsing, deeper unit testing, error reporting"

#define     P_PRIORITY  "direct, simple, brief, vigorous, and lucid (h.w. fowler)"
#define     P_PRINCIPAL "[grow a set] and build your wings on the way down (r. bradbury)"
#define     P_REMINDER  "there are many better options, but i *own* every byte of this one"

/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-12345678901-12345678901-*/
/*===[[ END_HEADER ]]=========================================================*/




/*===[[ HEADER GUARD ]]=======================================================*/
#ifndef YSCHED_priv_hguard
#define YSCHED_priv_hguard loaded

#include    "ySCHED.h"
#include    <yLOG.h>                    /* heatherly program logger            */
#include    <yURG.h>                    /* heatherly program logger            */

#define   _XOPEN_SOURCE
#include  <stdio.h>
#include  <stdlib.h>                  /* getenv()                            */
#include  <string.h>
#include  <time.h>
#include  <sys/time.h>
#include  <unistd.h>

#include  <ySTR.h>
#include  <yCOLOR_solo.h>



/*===[[ VERSION ]]========================================*/
/* rapidly evolving version number to aid with visual change confirmation     */



typedef struct tm        tTIME;
typedef const char       cchar;
typedef const int        cint;
typedef unsigned int     uint;


#define     SUCCESS             0
#define     NOT_READY          -1
#define     FAILED             -2
#define     SKIPPING           -3

#define     PARSE_MNS          0
#define     PARSE_HRS          1
#define     PARSE_DYS          2
#define     PARSE_MOS          3
#define     PARSE_DOW          4
#define     PARSE_WKS          5
#define     PARSE_YRS          6

#define     TYPE_MNS      a_type == 0
#define     TYPE_HRS      a_type == 1
#define     TYPE_DYS      a_type == 2
#define     TYPE_MOS      a_type == 3
#define     TYPE_DOW      a_type == 4
#define     TYPE_WKS      a_type == 5
#define     TYPE_YRS      a_type == 6
#define     RETURN(RC)   { strcpy (a_array, x_error); strcpy (mySCHED.last, x_error); return RC; }

typedef struct cLOCAL tLOCAL;
struct cLOCAL {
   /*---(overall)-----------*/
   char      status;
   char      full  [LEN_RECD];
   char      debug;
   /*---(parsing)-----------*/
   char      recd  [LEN_RECD];
   char     *xmin;
   char     *xhrs;
   char     *xdys;
   char     *xmos;
   char     *xdow;
   char     *xwks;
   char     *xyrs;
   char     *xdur;
   char      last [LEN_HUND];
   /*---(statistics)--------*/
   /*---(as-set date)-------*/
   long        s_epoch;                /* as-set unix epoch date              */
   tTIME      *s_broke;                /* as-set unix broke-down date         */
   ushort      s_year;                 /* as-set year (incremental from 1900) */
   uchar       s_month;                /* as_set month in the year            */
   uchar       s_day;                  /* as_set day in the month             */
   /*---(as-set stats)------*/
   uchar       s_dim;                  /* number of days in as-set month      */
   ushort      s_diy;                  /* number of days in as-set year       */
   ushort      s_doy;                  /* as_set day number in year           */
   uchar       s_wze;                  /* number of minium week               */
   uchar       s_wiy;                  /* number of weeks in as-set year      */
   uchar       s_woy;                  /* as_set week number in the year      */
   uchar       s_dow;                  /* as_set dow of the week              */
   uchar       s_fdow;                 /* dow of the 1st of current month     */
   int         s_off;                  /* offset of as-set day from today     */
   /*---(effective)---------*/
   char        effective   [500];
   char        effout      [500];
   char        global      [500];
   char        gloout      [500];
   /*---(done)--------------*/
};
extern  tLOCAL mySCHED;


extern tSCHED    g_sched;

char *strtok_r (char*, cchar*, char**);
int   isdigit  (int);


extern char  s_section [LEN_HUND];
extern char  s_input   [LEN_HUND];
extern char *s_ptr;
extern int   s_len;

extern char  s_not;
extern char  s_rev;
extern char  s_inv;
extern char  s_flp;

extern char  s_bef;
extern char  s_aft;
extern int   s_stp;
extern int   s_beg;
extern int   s_end;

extern char  s_type;
extern char  s_label   [LEN_LABEL];
extern int   s_min;
extern int   s_tmax;
extern int   s_max;

extern char  e_func      [LEN_LABEL];
extern int   e_line;
extern char  e_issue     [LEN_DESC];
extern int   e_pos;
extern int   e_len;
extern char  e_fancy     [LEN_RECD];



/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
char        ysched_resetdate        (void);
char        ysched_date__defense    (cint a_year, cint a_month, cint a_day);
char        ysched_date__current    (long a_now);
char        ysched_date__dow_1st    (void);
char        ysched_date__max_days   (void);
char        ysched_date__max_weeks  (void);

char        ysched__limits          (char a_type);
char        ysched__prep            (char *p);
int         ysched__number          (cchar *a_number);
char        ysched__modify          (void);
char        ysched__step            (void);
char        ysched__lesser          (void);
char        ysched__greater         (void);
char        ysched__between         (void);
int         ysched__dow             (void);
char        ysched__range           (void);
char        ysched__const           (void);
char        ysched__empty           (char *a_array);
char        ysched__apply           (char *a_array);
char        ysched__section         (cchar *a_sect, char *a_array);
char        ysched_field            (cchar *a_input, char *a_array, char a_type);




char       /*----: make a printable version of the effective dates -----------*/
ySCHED__effout     (void);

char       /*----: set the effective date range ------------------------------*/
ySCHED__effective  (char *a_recd, long a_now);

char*      /*----: unit testing accessor for clean validation interface ------*/
ysched__accessor   (char *a_question);

char       /*----: initialize the effective range ----------------------------*/
ySCHED__effinit    (void);

char       /*----: add efffective dates to global ----------------------------*/
ySCHED__globalize  (char *a_recd, long a_now);

char       /*----: make a printable version of the effective dates -----------*/
ySCHED__effout     (void);

char       /*----: update effective for a endpoint dates ---------------------*/
ySCHED__effend     (char *a_date, char a_side, long a_now);

char       /*----: update effective for a list of exceptions -----------------*/
ySCHED__effnot     (char *a_list, long a_now);



/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
char        ysched__unit_quiet      (void);
char        ysched__unit_loud       (void);
char        ysched__unit_end        (void);







#endif
/*===[[ END ]]================================================================*/
