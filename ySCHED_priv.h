
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



/*===[[ VERSION ]]========================================*/
/* rapidly evolving version number to aid with visual change confirmation     */
#define   YSCHED_VER_NUM     "1.4a"
#define   YSCHED_VER_TXT     "update debugging, testing, modularity, and duration"



typedef struct tm        tTIME;
typedef const char       cchar;
typedef const int        cint;
typedef unsigned int     uint;


#define     SUCCESS             0
#define     NOT_READY          -1
#define     FAILED             -2
#define     SKIPPING           -3
#define     MAX_RECD           1000

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
   char      full  [MAX_RECD];
   char      debug;
   /*---(parsing)-----------*/
   char      recd  [MAX_RECD];
   char     *xmin;
   char     *xhrs;
   char     *xdys;
   char     *xmos;
   char     *xdow;
   char     *xwks;
   char     *xyrs;
   char     *xdur;
   char      last [MAX_FIELD];
   /*---(statistics)--------*/
   int       nwks;         /* number of weeks in current year             */
   int       ndys;         /* number of days in current month             */
   int       fdow;         /* dow of the 1st of current month             */
   /*---(current)-----------*/
   long      cdate;        /* current unix date                           */
   tTIME    *cbroke;       /* current unix broke-down date                */
   int       cyrs;         /* current year                                */
   int       cwks;         /* current week in the year                    */
   int       cmos;         /* current month in the year                   */
   int       cdys;         /* current day in the month                    */
   int       cdow;         /* current dow of the week                     */
   int       coff;         /* offset of current day from today            */
   long      cset;         /* set date                                    */
   /*---(effective)---------*/
   char        effective   [500];
   char        effout      [500];
   char        global      [500];
   char        gloout      [500];
   /*---(done)--------------*/
};
extern  tLOCAL mySCHED;


extern tSCHED    sched;

char *strtok_r (char*, cchar*, char**);
int   isdigit  (int);


extern char *s_input;
extern int   s_len;
extern char  s_not;
extern char  s_rev;
extern char  s_inv;
extern char  s_bef;
extern char  s_aft;
extern int   s_stp;
extern int   s_beg;
extern int   s_end;
extern int   s_min;
extern int   s_smax;
extern int   s_bmax;




int        /*----: scheduling grammar value checker --------------------------*/
ysched__convert    (cchar *a_field, cchar *a_input, cint a_min, cint a_max);

char         /*--: interpret modifier --------------------[ ------ [ ------ ]-*/
ysched__prep       (int a_type);

char         /*--: interpret step ------------------------[ ------ [ ------ ]-*/
ysched__step       (void);

char         /*--: interpret modifier --------------------[ ------ [ ------ ]-*/
ysched__modifier   (void);

char         /*--: interpret constants -------------------[ ------ [ ------ ]-*/
ysched__const      (int a_type);

int          /*--: interpret number ----------------------[ ------ [ ------ ]-*/
ysched__number     (int a_type, char *a_number);

int          /*--: interpret special day references ------[ ------ [ ------ ]-*/
ysched__day        (void);

char         /*--: interpret ranges ----------------------[ ------ [ ------ ]-*/
ysched__range      (int a_type);

char         /*--: apply grammar to array ----------------[ ------ [ ------ ]-*/
ysched__apply      (int a_type, char *a_array);

char       /*----: scheduling grammar field interpreter ----------------------*/
ysched__field      (char *a_input, char *a_array, int a_type);

char       /*----: make a printable version of the effective dates -----------*/
ySCHED__effout     (void);

char       /*----: set the effective date range ------------------------------*/
ySCHED__effective  (char *a_recd, long a_now);

char*      /*----: unit testing accessor for clean validation interface ------*/
ySCHED__accessor   (char *a_question);

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


int         ysched_duration         (char *a_input);

/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
char        ysched__unit_quiet      (void);
char        ysched__unit_loud       (void);
char        ysched__unit_end        (void);







#endif
/*===[[ END ]]================================================================*/
