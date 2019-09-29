/************************************************************************************************/
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                                    T E T E . H                                               */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
/************************************************************************************************/
/* LIBRARIES                                                                                    */
/************************************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/time.h>
#include <time.h>
/************************************************************************************************/
/* CONSTANTS                                                                                    */
/************************************************************************************************/
#define LISTMAX 50
#define SHORTLENGTH 50

#ifndef TETE_H_   /* Include guard */
#define TETE_H_


static char OUTPUTFOLDER[] = "./outputs";
static char DEFAULTNAME[] = "last";
static char DUMMYMARKUP[] = "./Inputs/Blip-is-blop-MU.txt";
static char OTHERDUMMYMARKUP[] = "./Inputs/Blip-is-blop-MU2.txt";
static char DUMMYNAME[] = "BlipIsBlop";
static char DEFAULTFILETOREAD[]="./Inputs/lastMU.txt"; //"./Inputs/stuff.txt" "./Inputs/lastMU.txt"
static char DEFAULTOUTPUT[] = "LastOutput";
/************************************************************************************************/
/* VARIABLES                                                                                    */
/************************************************************************************************/
extern bool MULTIPLECONTEXTS;
//initialised in interface0:
extern bool PROMPTMETA;
extern bool PROMPTQUESTION;
extern bool PROMPTCONTEXTS;
extern bool PROMPTLEXIC;
extern bool PROMPTANSWERS;
extern bool PROMPTATOMS;
extern bool PROMPTATOM;
extern bool PROMPTCONTEXT;
extern bool PROMPTENTRY;
// initialised in interfaceNEURO.c
extern int PROMPTNO;
//initialised in instructions.c
extern int promptno;
// Initialised in notes.c
extern bool lexicNBtodo;
extern bool reasonNBtodo;
extern bool publicationNBtodo;
// Initialised in files.c
extern int shortlength;
// Text recorded is sometimes annoyingly long. This is the length we truncate it to:
//extern int SHORTLENGTH; // initialised in writeAddendum.c
// Number of times a malloc has been made for an object that hasn't yet been freed: 
extern int ALLOCCOUNT;  // initialised in structures.c
// Number used to id objects:
extern int NUMBER;  // initialised in structuresAddendum.c
// initialised in read.c
extern char CURRENTTAG[5000];
extern char CURRENTTEXT[5000];
bool CURRENTTAGISOPEN;
// initialised in structuresAddendum.c
extern char REFTAG[5000];
extern char LEXICTAG[5000];
extern char SUPPORTTAG[5000];
extern char DISPORTTAG[5000];
extern char OBSOLETIONTAG[5000];
extern char INSTANTIATIONTAG[5000];
extern char CONTEXTTAG[5000];
extern char CONTEXTSTAG[5000];
extern char ANSWERSTAG[5000];
extern char ANSWERTAG[5000];
extern char QUESTIONTAG[5000];
extern char ATOMTAG[5000];
extern char ATOMSTAG[5000];
extern char ENTRYTAG[5000];
extern char TERMTAG[5000];
extern char DEFINITIONTAG[5000];
extern char TEXTTAG[5000];
extern char METATAG[5000];
extern char TAGTAG[5000];
extern char AUTHORTAG[5000];
extern char IDTAG[5000];
extern char PARENTIDTAG[5000];
extern char COUNTTAG[5000];
// initialised in instructions.c
extern char AUTHORPROMPT[5000];
extern char REFPROMPT[5000];
extern char LEXICPROMPT[5000];
extern char SUPPORTPROMPT[5000];
extern char DISPORTPROMPT[5000];
extern char OBSOLETIONPROMPT[5000];
extern char INSTANTIATIONPROMPT[5000];
extern char CONTEXTPROMPT[5000];
extern char CONTEXTSPROMPT[5000];
extern char ANSWERSPROMPT[5000];
extern char ANSWERPROMPT[5000];
extern char QUESTIONPROMPT[5000];
extern char ATOMPROMPT[5000];
extern char STOREDATOMPROMPT[5000];
extern char ATOMSPROMPT[5000];
extern char ENTRYPROMPT[5000];
extern char TERMPROMPT[5000];
extern char DEFINITIONPROMPT[5000];
extern char REASONPROMPT[5000];
extern char REASONSPROMPT[5000];

/************************************************************************************************/
/* STRUCTURE TYPES                                                                              */
/************************************************************************************************/
typedef struct something CONTEXT;
typedef struct nothing ATOMS;
typedef struct otherthing CONTEXTS;

/* META */
typedef struct {
   char     *Tag;
	char     *Author;
	int      Depth;
   char     *ID;
   char     *ParentID;
} META;

/* ATOM */
typedef struct {
	META     *Meta;
   char     *Text;
   CONTEXTS *Contexts;
	ATOMS    *Answers;
} ATOM;

/* ENTRY */
typedef struct {
	META     *Meta;
	char     *Term;
	char     *Definition;
} ENTRY;

/* Lexic */
typedef struct {
	META     *Meta;
	int      Count;
	ENTRY    *List[LISTMAX];
} LEXIC;

/* List of Atoms*/
struct nothing {
	META     *Meta;
	int      Count;
	ATOM     *List[LISTMAX];
};

/* Context */
struct something {
	META     *Meta;
   LEXIC    *Lexic;
   ATOMS    *Support;
   ATOMS    *Disport;
   ATOMS    *Obsoletion;
	ATOMS    *Instantiation;
};

/* List of Contexts*/
struct otherthing {
	META     *Meta;
	int      Count;
	CONTEXT  *List[LISTMAX];
};

/************************************************************************************************/
/* W R I T E    A D D E N D U M . C                                                             */
/************************************************************************************************/
char *depth2indent(int depth);
char * uncledIndent(char *parentIndent);
char * ununcledIndent(char *parentIndent);
char *indentchild(bool neice, char *parentIndent);
char *before(char *indent, bool spaced);
char *tagtotitle(char *tag);
/************************************************************************************************/
/* W R I T E   M A R K U P . C                                                                  */
/************************************************************************************************/
int writeMarkupMETA(FILE *fp,META *p,bool entitle);
int writeMarkupQUESTION(FILE *fp,ATOM *p,bool entitle);
int writeMarkupATOM(FILE *fp,ATOM *p,bool entitle);
int writeMarkupATOMS(FILE *fp,ATOMS *p,bool entitle);
int writeMarkupCONTEXT(FILE *fp,CONTEXT *p,bool entitle);
int writeMarkupCONTEXTS(FILE *fp,CONTEXTS *p,bool entitle);
int writeMarkupENTRY(FILE *fp,ENTRY *p,bool entitle);
int writeMarkupLEXIC(FILE *fp,LEXIC *p,bool entitle);
/************************************************************************************************/
/* W R I T E    F O R E S T . C                                                                 */
/************************************************************************************************/
int writeForestMETA(FILE *fp,int DEPTH,META *p,char *indent);
int writeForestQUESTION(FILE *fp,int DEPTH,ATOM *p,char *indent);
int writeForestANSWERS(FILE* fp,int DEPTH,ATOMS *p,char *indent,bool neice);
int writeForestATOM(FILE *fp,int DEPTH,ATOM *p,char *indent);
void writeForestSTATEMENTLINE(FILE *fp,ATOM *p,char *indent,bool spaced);
int writeForestATOMS(FILE *fp,int DEPTH,ATOMS *p,char *indent,bool neice,bool bigbrother,bool spaced);
int writeForestCONTEXT(FILE *fp,int DEPTH,CONTEXT *p,char *indent,bool neice,bool bigbrother);
int writeForestCONTEXTS(FILE *fp,int DEPTH,CONTEXTS *p,char *indent,bool neice,bool bigbrother);
int writeForestENTRY(FILE *fp,int depth,ENTRY *p,char *indent);
int writeForestLEXIC(FILE *fp,int DEPTH,LEXIC *p,char *indent);
/************************************************************************************************/
/*  W R I T E   L A T E X . C                                                                   */
/************************************************************************************************/
int writeLatexMETA(FILE *fp,META *p,char *indent);
int writeLatexQUESTION(FILE *fp,ATOM *p,char *indent);
int writeLatexATOM(FILE *fp,ATOM *p,char *indent);
int writeLatexATOMS(FILE *fp,ATOMS *p,char *indent);
int writeLatexCONTEXT(FILE *fp,CONTEXT *p,char *indent);
int writeLatexCONTEXTS(FILE *fp,CONTEXTS *p,char *indent);
int writeLatexENTRY(FILE *fp,ENTRY *p,char *indent);
int writeLatexLEXIC(FILE *fp,LEXIC *p,char *indent);
/************************************************************************************************/
/* W R I T E   T I K Z . C                                                                      */
/************************************************************************************************/
void writeTikzTitle(FILE *fp, char *title);
void openTikzEnvironment(FILE *fp,ATOM *q);
int writeTikzMETA(FILE *fp,META *p,char *indent);
int writeTikzQUESTION(FILE *fp,ATOM *p,char *indent);
void writeTikzANSWER(FILE* fp, ATOM *p, char *branchnode);
int writeTikzANSWERS(FILE *fp,ATOMS *p,char *branchnode);
char *writeTikzATOM(FILE *fp,ATOM *p,char *parentcontextnode,char *place);
int writeTikzATOMS(FILE *fp,ATOMS *p,char *parentcontextnode,char *place);
int writeTikzCONTEXT(FILE *fp,CONTEXT *p,char *contextnode);
int writeTikzCONTEXTS(FILE *fp,CONTEXTS *p,char *contextnode);
char *writeTikzENTRY(FILE *fp,ENTRY *p,char *contextnode, char *place);
int writeTikzLEXIC(FILE *fp,LEXIC *p,char *indent);
/************************************************************************************************/
/* S T R U C T U R E S    A D D E N D U M . C                                                   */
/************************************************************************************************/
int signalAllocate(bool fail, int depth, char *tag,char *text,uintptr_t p);
int signalFree(int depth, char *tag,char *text,uintptr_t p);
int signalAllocateFromMeta(bool fail, META*m,uintptr_t p);
int signalFreeFromMeta(META*m,uintptr_t p);
char *newid();
char *subtag(char *tag,int n);
bool isemptyCONTEXT(CONTEXT *p);
/************************************************************************************************/
/* S T R U C T U R E S . C                                                                      */
/************************************************************************************************/
META *initMETA(char *tag,char *author,int depth,char *id,char *parentid);
int freeMETA(META *p);
ATOM		*initATOM(META *meta,char* text,CONTEXTS *contexts, ATOMS *answers);
int freeATOM(ATOM *p);
ATOMS 	*initATOMS(META *meta);
int freeATOMS(ATOMS* p);
CONTEXT 	*initCONTEXT(META *meta,LEXIC* lexic,ATOMS* support,ATOMS* disport,ATOMS* obsoletion,ATOMS* instantiation);
int freeCONTEXT(CONTEXT* p);
CONTEXTS *initCONTEXTS(META *meta);
int freeCONTEXTS(CONTEXTS* p);
ENTRY 	*initENTRY(META *meta,char *term,char *definition);
int freeENTRY(ENTRY *p);
LEXIC 	*initLEXIC(META *meta);
int freeLEXIC(LEXIC* p);
ATOM    	*recordNewATOM(char *id,ATOMS *p,char *text);
ATOM 		*recordNewANSWER(char *id,ATOMS *p,char *text);
CONTEXT 	*recordNewCONTEXT(char *id, CONTEXTS *p,LEXIC* lexic,ATOMS* support,ATOMS* disport,ATOMS* obsoletion,ATOMS* instantiation);
ENTRY   	*recordNewENTRY(char *id,LEXIC *p,char *term,char *definition);
ATOM 		*copyATOM(META *meta,ATOM *p);
ATOMS 	*copyATOMS(META *meta,ATOMS *p);
CONTEXT 	*copyCONTEXT(META *meta,CONTEXT *p);
CONTEXTS *copyCONTEXTS(META *meta,CONTEXTS *p);
ENTRY 	*copyENTRY(META *meta,ENTRY *p);
LEXIC 	*copyLEXIC(META *meta,LEXIC *p);
/************************************************************************************************/
/* I N T E R F A C E  0 . C                                                                     */
/************************************************************************************************/
ATOM *getATOM(int maxdepth,META *meta);
ATOM *getREASON(META *meta);
ATOM *getNecessaryATOM(META *meta,char *alttext);
ATOMS *getATOMS(int maxdepth,META *meta);
ATOMS *getREASONS(META *meta);
ATOM *incrementSUPPORT(ATOM *p,char *text,char *author,char *tag);
CONTEXT *getCONTEXT(int maxdepth,META *meta);
CONTEXTS *getCONTEXTS(int maxdepth,META *meta);
LEXIC *getLEXIC(int maxdepth,META *meta);
ENTRY *getENTRY(int maxdepth,META *meta);
ATOM *getCONFOUNDER(META *meta);
ATOMS *getCONFOUNDERS(META *meta);
/************************************************************************************************/
/* I N T E R F A C E    N E U R O  . C                                                          */
/************************************************************************************************/
void getLexic(ATOM *a,char *expression, char *sub);
void getReference(ATOM *a, char *prompt);
bool getReferenceInstruction(ATOM *a);
void getReasons(CONTEXT * c, char *instruction, char *subinstruction);
void getConfounders(ATOM *experiment);
ATOM *getExperiment();
ATOM *getArgument(ATOM *answer, ATOM *experiment);
char *stripConfounder(ATOM *a);
ATOM * registerStudy();
ATOM * closeStudy(ATOM *q);
void setInstructionsP();
void setInstructionsS();
void setInstructionsF();
ATOM *neuroscience_mmm();
ATOM *mergeQuestions(ATOM *p, ATOM *q);
/************************************************************************************************/
/* N O T E S . C                                                                                */
/************************************************************************************************/
void hypothesisNote();
void authorNote();
void lexicNote();
void publicationNote();
void reasonNote();
void mergeNote();
void mergedQuestionNote();
void mergeLexicNote();
void recursionNote();
void conditionalNote();
void presentlyNote();
/************************************************************************************************/
/* D E P T H S . C                                                                              */
/************************************************************************************************/
int ANYTHINGtoMETAdepth(int d);
int METAtoINSIDEdepth(int d);
int ANYTHINGtoINSIDEMETAdepth(int d);
int QUESTIONdepth(int d);
int QUESTIONtoANSWERSdepth(int d);
int ANSWERStoANSWERdepth(int d);
int ATOMStoATOMdepth(int d);
int CONTEXTtoATOMSdepth(int d);
int CONTEXTtoATOMdepth(int d);
int QUESTIONtoCONTEXTSdepth(int d);
int ATOMtoCONTEXTSdepth(int d);
int CONTEXTStoCONTEXTdepth(int d);
int CONTEXTtoLEXICdepth(int d);
int QUESTIONtoLEXICdepth(int d);
int LEXICtoENTRYdepth(int d);
int QUESTIONtoENTRYdepth(int d);
int QUESTIONtoATOMdepth(int d);
int QUESTIONtoANSWERdepth(int d);
int ATOMtoCONTEXTdepth(int d);
int ATOMtoATOMdepth(int d);
int CONTEXTtoENTRYdepth(int d);
/************************************************************************************************/
/* R E A D . C                                                                                  */
/************************************************************************************************/
META *readMETA(FILE *fp,int depth);
ATOM *readATOM(FILE *fp,int depth);
ATOMS *readATOMS(FILE *fp,int depth);
CONTEXT *readCONTEXT(FILE *fp,int depth);
CONTEXTS *readCONTEXTS(FILE *fp,int depth);
ENTRY *readENTRY(FILE *fp,int depth);
LEXIC *readLEXIC(FILE *fp,int depth);
/************************************************************************************************/
/* F I L E S . C                                                                                */
/************************************************************************************************/
void visualisefile(FILE *fp,char *filename);
FILE *openTheFile(char *string);
void createFiles(char givenname[], ATOM *p);
/************************************************************************************************/
/* T E X T  . C                                                                                 */
/************************************************************************************************/
char *statementToQuestion(char *a);
char *questionToStatement(char *q);
char *questionToPointlessStatement(char *q);
char *removeMarks(char* source);
char *resizetext(char *text,char *ending);
char * ReplaceSpaces(char source[]);
char * RemoveSpaces(char source[]);
void fprintfRepeat(FILE *fp, char *c, int i);
void repeatfprintf(FILE *fp,char *s, int n);
bool startsWith(const char *pre, const char *str);
char *lowerthecase(char *input);
void framefprintf(FILE *fp,char *s, int n);
void readUntilNextTag(FILE* fp);
char *stampString(char *string);
void printLineOnScreen(void);
/************************************************************************************************/
/* C H O I C E  . C                                                                             */
/************************************************************************************************/
char *getFirstArgument(int argc, char *argv[]);
char *getAfilename(int argc, char *argv[],int i,char *defaultfilename,char *prompt);
void writeContent(int argc, char *argv[],int i,char *defaultfilename,ATOM *p);

void getSecondArgument(int argc,char *argv[],char *arguments[]);
void getThirdArgument( int argc,char *argv[],char *arguments[]);

int checkDeallocation(int f);
int buildFromFile(int argc, char *argv[]);
int basicsourceinterface();
int playNeuroscientist(int argc, char *argv[]);
int tryMerge(int argc, char *argv[]);
/************************************************************************************************/
/* I N S T R U C T I O N S  . C                                                                 */
/************************************************************************************************/
void changeATOMPROMPT(char *newprompt);
void restoreATOMPROMPT();
char *promptandscan(char *prompttext,bool manditory);
int promptAndScan(char *prompt, char *text);
int promptAndScanInstruction(char *instruction, char *text, char *sub);
int promptAndScanManditoryInstruction(char *instruction, char *text, char *sub);
bool scanYesNo(char *instruction, char *sub);
void numberedinstruction(char *string);
void unmberedinstruction(char *string);
/************************************************************************************************/
/*                                                                                      */
/************************************************************************************************/
/************************************************************************************************/
/*                                                                                      */
/************************************************************************************************/
/************************************************************************************************/
/*                                                                                      */
/************************************************************************************************/
#endif // TETE_H_
