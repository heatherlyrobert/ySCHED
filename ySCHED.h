/*============================----beg-of-source---============================*/

/*===[[ HEADER ]]=============================================================*

 *   focus         : (PS) programming support
 *   niche         : (sg) scheduling_grammar
 *   heritage      : horae (goddesses of seasons and porportions of time)
 *   purpose       : simple, elegant, and powerful kernighan scheduling grammar
 *
 *   base_system   : gnu/linux   (powerful, ubiquitous, technical, and hackable)
 *   lang_name     : ansi-c      (righteous, limitless, universal, and forever)
 *   dependencies  : +yLOG (optional)
 *   size goal     : tiny (approximately 500 slocL)
 *
 *   author        : the_heatherlys
 *   created       : 2011-03
 *   priorities    : direct, simple, brief, vigorous, and lucid (h.w. fowler)
 *   end goal      : loosely coupled, strict interface, maintainable, portable
 * 
 */
/*===[[ SUMMARY ]]============================================================*

 *   ySCHED is a simple, elegant, powerful, and extended kernighan scheduling
 *   grammar that builds on the classic cron to enable it to support other
 *   applications such as calendaring
 *
 */
/*===[[ PURPOSE ]]============================================================*

 *   ySCHED is a set of shared functions providing flexible, reliable, and
 *   consistent access to scheduling grammar interpretation.  its purpose is
 *   to take a complex and foundational capability and provide a shared service
 *   that can be tested, maintained, and documented once to avoid migrations.
 *
 *   it became obvious that this shared library made sense when i brought the
 *   scheduling code i used in khronos (cron) over to horae (calendaring), then
 *   added some very usuful features, and realized that backporting them as well
 *   as keeping the in sync would be a testing nightmare.
 *
 *   refactoring "DRY: don't repeat yourself" each piece of knowledge must have
 *   a single, unambiguous, authoritative representation in the system.
 *
 *   above all, this design has been overcome by greater minds than ours.  why
 *   would we develop new and interesting ways to solve this -- unique for every
 *   application.  no way.  so, we solve it once and improve the shared solution
 *   as we learn.
 *
 *   ySCHED will focus on...
 *      - kernighan inspired scheduling grammar from his original cron (1978)
 *      - extended with key adaptations from dcron, vixie-cron, perl libs, etc.
 *      - expanded to handle non-script assisted scheduling (clarity & accuracy)
 *      - given additional simplifiers and constants for readability
 *      - keeping this functionality isolated for clear and complete testing
 *
 *   ySCHED will provide...
 *      - interpretation of raw, single-line, ascii schedule descriptions
 *      - feedback to yLOG to show the interpretation if desired
 *      - adaptable to posix 5 field or heatherly 7 field versions
 *      - testing against specific times to see if that record is triggered
 *      - testing against a range of times to see if a record will trigger
 *      - ability to have line/group/global blackouts and exceptions
 *
 *   ySCHED will not...
 *      - deal with years -- this is about a rolling 365 days
 *      - do anything with a triggered record, this is just the library
 *      - provide any output or visual feedback other than logging
 *
 *   above all, ySCHED is meant to enable technical power users access to the
 *   best calentaring that we can provide, not to make it easy for novices.
 *
 *   ySCHED will break backward compatibility with existing crontab files given
 *   its addition of two fields and additional symbols, but will be very
 *   understandable to system administators who are familiar with crontab files.
 *
 *   as always, there are many, stable, accepted, existing programs and
 *   utilities built by better programmers that are likely superior in speed,
 *   size, capability, and reliability; BUT, i would not have learned nearly as
 *   much using them, so i follow the adage...
 *
 *   TO TRULY LEARN> do not seek to follow in the footsteps of the men of old;
 *   seek what they sought ~ Matsuo Basho
 *
 *   the underlying idea is to use this build process to learn new programming
 *   capabilities and put additional pressure on me to improve my standards,
 *   maintenance, and coding environment.  just using programs and ideas does
 *   not lead to understanding -- so get messy ;)
 *
 *   any one who wishes to become a good writer [programmer] should endeavour,
 *   before he allows himself to be tempted by the more showy qualities, to be
 *   direct, simple, brief, vigourous, and lucid -- henry watson fowler (1908)
 *
 *   simplicity is prerequisite for reliability and security
 *
 */
/*============================================================================*/



/*===[[ HEADER GUARD ]]=======================================================*/
#ifndef   YSCHED_hguard
#define   YSCHED_hguard     loaded


#include  <ySTR_solo.h>


/*===[[ CONSTANTS ]]======================================*/
#define   YSCHED_ANY         -1
#define   YSCHED_CURR        NULL

#define   YSCHED_ALWAYS      ".valid < >"



typedef    const int      cint;
typedef    const short    cshort;
typedef    const char     cchar;
typedef    unsigned char  uchar;
typedef    unsigned short ushort;
typedef    unsigned long  ulong;


/*===[[ PUBLIC FUNCTIONS ]]===============================*/

/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
char*       ySCHED_version          (void);

char        ySCHED_config_by_date   (char a_year, char a_month, char a_day);
char        ySCHED_config_by_epoch  (long a_epoch);

char        ySCHED_newfile          (void);
char        ySCHED_valid            (char *a_recd);

char        ySCHED_create           (void **a_sched, char *a_recd);
char        ySCHED_force            (void **a_sched, char *a_recd);
char        ySCHED_retire           (void **a_sched);

char        ySCHED_test_by_time     (void *a_sched, int a_hour, int a_minute);
char        ySCHED_test_by_epoch    (void *a_sched, long a_epoch);

char        ySCHED_feedback         (int *a_line, char *a_focus, char *a_issue, int *a_beg, int *a_len, char *a_fancy);
char        ySCHED_details          (char *a_min, char *a_hrs, char *a_dys, char *a_mos, char *a_dow, char *a_wks, char *a_yrs, char *a_beg, char *a_end, char *a_valid);




#endif
/*===[[ END ]]================================================================*/
