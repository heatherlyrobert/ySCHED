/*============================----beg-of-source---============================*/

/*===[[ HEADER ]]=============================================================*

 *   focus         : (PS) programming support
 *   niche         : (sg) scheduling_grammar
 *   application   : ySCHED
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



/*===[[ CONSTANTS ]]======================================*/
#define   ySCHED_ANY         -1
#define   ySCHED_CURR        NULL



/*===[[ PUBLIC STRUCTURE ]]===============================*/
#define   MAX_RAW            200
#define   MAX_FIELD          100
#define   MAX_EFF            500
typedef struct cSCHED  tSCHED;
struct  cSCHED
{
   char      raw   [MAX_RAW];         /* raw scheduling grammar text          */
   char      min   [MAX_FIELD];       /* minutes           0-59               */
   char      hrs   [MAX_FIELD];       /* hours             0-23               */
   char      dys   [MAX_FIELD];       /* days of month     1-31               */
   char      mos   [MAX_FIELD];       /* month of year     1-12               */
   char      dow   [MAX_FIELD];       /* day of week       1-7                */
   char      wks   [MAX_FIELD];       /* week of year      1-53               */
   char      yrs   [MAX_FIELD];       /* years             1-50               */
   char      eff   [MAX_EFF];         /* effective range (-100 to +365 days)  */
};



/*===[[ PUBLIC FUNCTIONS ]]===============================*/

char*        /*--> return library versioning info --------[ ------ [ ------ ]-*/
ySCHED_version     (void);

char         /*--> set debugging mode --------------------[ ------ [ ------ ]-*/
ySCHED_debug       (char a_flag);

char         /*--> set the date for parsing --------------[ ------ [ ------ ]-*/
ySCHED_setdate     (int a_year, int a_month, int a_day);

char         /*--> reset globals and effective dates -----[ ------ [ ------ ]-*/
ySCHED_reset       (void);

char         /*--> parse scheduling grammar --------------[ ------ [ ------ ]-*/
ySCHED_parse       (tSCHED *a_sched, char *a_recd);

int          /*--> test for a specific time --------------[ ------ [ ------ ]-*/
ySCHED_test        (tSCHED *a_sched, int a_hour, int a_minute);

char         /*--> save results to a alternate struct ----[ ------ [ ------ ]-*/
ySCHED_save        (tSCHED *a_sched);

char         /*--> load data from an alternate struct ----[ ------ [ ------ ]-*/
ySCHED_load        (tSCHED *a_sched);



#endif
/*===[[ END ]]================================================================*/
