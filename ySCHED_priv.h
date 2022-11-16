/*===[[ ONE_LINERS ]]=========================================================*/

#define     P_FOCUS     "PS (programming support)"
#define     P_NICHE     "sg (scheduling grammar)"
#define     P_SUBJECT   "kernighan scheduling grammar"
#define     P_PURPOSE   "simple, elegant, and powerful kernighan scheduling grammar"

#define     P_NAMESAKE  "eunomia-horae (good order)"
#define     P_HERITAGE  "goddesses of law and appropriate time"
#define     P_IMAGERY   "beautiful, rose-bloomed maiden, sister of justice, peace, and fortune"
#define     P_REASON    "represents the appropriate and correct time"

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
#define     P_VERNUM    "1.4l"
#define     P_VERTXT    "eliminated ySCHED_valid logic and testing completely"

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

#define   LEN_VALID          500

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


#define    HOUR2SEC       (60 * 60)
#define    DAY2SEC        (24 * 60 * 60)

extern char g_ready;
#define    YSCHED_NOT_READY   g_ready != 'y'

typedef struct cLOCAL tLOCAL;
struct cLOCAL {
   /*---(overall)-----------*/
   char        status;
   char        debug;
   /*---(parsing)-----------*/
   char        full        [LEN_RECD];
   char        x_raw       [LEN_RECD];
   char        x_field     [LEN_RECD];
   char        x_eval;
   char       *x_min;
   char       *x_hrs;
   char       *x_dys;
   char       *x_mos;
   char       *x_dow;
   char       *x_wks;
   char       *x_yrs;
   char        last        [LEN_HUND];
   /*---(as-set date)-------*/
   long        s_epoch;                /* as-set unix epoch date              */
   char        s_year;                 /* as-set year (incremental from 1900) */
   char        s_month;                /* as_set month in the year            */
   char        s_day;                  /* as_set day in the month             */
   char        s_dst;                  /* flag on DST                         */
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
   /*---(done)--------------*/
};
extern  tLOCAL mySCHED;



/*===[[ PUBLIC STRUCTURE ]]===============================*/
typedef struct cSCHED  tSCHED;
struct  cSCHED
{
   /*---(id)----------------*/
   uchar     seq;
   /*---(input)-------------*/
   char      raw   [LEN_HUND];        /* raw scheduling grammar text          */
   /*---(set for)-----------*/
   long      epoch;                   /* time set for last parsing            */
   char      year;
   char      month;
   char      day;
   /*---(last test)---------*/
   char      hour;
   char      minute;
   char      result;
   /*---(details)-----------*/
   char      eval;                    /* normal, start at, end before         */
   char      min   [LEN_LONG];        /* minutes           0-59               */
   char      hrs   [LEN_TITLE];       /* hours             0-23               */
   char      dys   [LEN_DESC];        /* days of month     1-31               */
   char      mos   [LEN_LABEL];       /* month of year     1-12               */
   char      dow   [LEN_TERSE];       /* day of week       1-7                */
   char      wks   [LEN_LONG];        /* week of year      1-53               */
   char      yrs   [LEN_LONG];        /* years             1-50               */
   /*---(links)-------------*/
   tSCHED   *m_prev;
   tSCHED   *m_next;
   /*---(done)--------------*/
};
extern tSCHED     *g_head;
extern tSCHED     *g_tail;
extern tSCHED     *g_curr;
extern int         g_count;
extern int         g_max;
extern tSCHED      g_null;


extern char      unit_answer [LEN_RECD];


char *strtok_r (char*, cchar*, char**);
int   isdigit  (int);


extern char  s_raw     [LEN_RECD];
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

extern int   e_field;
extern int   e_section;
extern char  e_sect      [LEN_LABEL];
extern char  e_func      [LEN_LABEL];
extern int   e_line;
extern char  e_issue     [LEN_DESC];
extern int   e_pos;
extern int   e_len;
extern char  e_fancy     [LEN_RECD];



/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
char        ysched_date_reset       (void);
char        ysched_date__ymd2epoch  (cchar a_year, cchar a_month, cchar a_day);
char        ysched_date__current    (long a_now);
char        ysched_date__dow_1st    (void);
char        ysched_date__max_days   (void);
char        ysched_date__max_weeks  (void);
char*       ysched_date__unit       (char *a_question);

/*---(errors)---------------*/
char        ysched_error_reset      (void);
char        ysched__trouble         (int *a_func, int a_line, char *a_issue, int a_pos, int a_len);
char        ysched_fancify          (void);
char*       ysched_error__unit      (char *a_question);
/*---(support)--------------*/
char        ysched__limits          (char a_type);
char        ysched__prep            (char *p);
char        ysched__empty           (char *a_array);
char        ysched__apply           (char *a_array);
/*---(mods)-----------------*/
char        ysched__modify          (void);
char        ysched__step            (void);
int         ysched__nearest         (void);
/*---(values)---------------*/
int         ysched__number          (cchar *a_number);
char        ysched__const           (void);
/*---(range)----------------*/
char        ysched__lesser          (void);
char        ysched__greater         (void);
char        ysched__between         (void);
char        ysched__range           (void);
/*---(drivers)--------------*/
char        ysched__section         (cchar *a_sect, char *a_array);
char        ysched_field            (cchar *a_input, char *a_array, char a_type, char a_eval);
char*       ysched_field__unit      (char *a_question);
/*---(done)-----------------*/



char*       ysched_valid__unit      (char *a_question);





/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
char        ysched__unit_quiet      (void);
char        ysched__unit_loud       (void);
char        ysched__unit_end        (void);


/*---(support)--------------*/
char        ysched_parse__wipe      (tSCHED *a_sched);
char*       ysched_parse__memory    (tSCHED *a_sched);
/*---(memory)---------------*/
char        ysched_parse__new       (tSCHED **a_new);
char        ysched_parse__free      (tSCHED **a_old);
char        ysched_force            (void **a_sched, char *a_recd);
char        ysched_purge            (void);
char        ysched_update           (void *a_sched);
char        ysched_update_all       (void);
/*---(unittest)-------------*/
char*       ysched_parse__unit      (char *a_question, int a_num);
/*---(done)-----------------*/


char*       ysched_test__unit       (char *a_question);



#endif
/*===[[ END ]]================================================================*/
