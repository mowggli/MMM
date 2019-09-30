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


#define PROBLEM -1
#define CONTINUE 1
#define ENOUGH 0
#define LISTMAX 50
#define MAX 3
#define SHORTLENGTH 50

#ifndef TETE_H_   /* Include guard */
#define TETE_H_


extern int shortlength;
extern int promptno;
extern char *newtext;

/************************************************************************************************/
/*                                                                                              */
/************************************************************************************************/

static const char STOP[] = "";
//static char dummyMU[] = "dummyMarkup.txt";
//static char dummyFileName[] = "dum:my?";
//static char dummyMU[] = "transcranialBeforeMU.txt";
//static char dummyFileName[] = "transcranial";
static char dummyMU[] = "Blip-is-blop-MU.txt";
static char seconddummyMU[] = "Blip-is-blop-MU2.txt";
static char dummyFileName[] = "BlipIsBlop";
static const char outputfolder[] = "./outputs";

/************************************************************************************************/
/*                                                                                              */
/************************************************************************************************/

typedef struct something CONTEXT;
typedef struct nothing ATOMS;

/* ATOM */
typedef struct {
	int depth;
   char* text;
	char * type;
   char* tag;
   CONTEXT* context;
	ATOMS* answers;
} ATOM ;

/* ENTRY */
typedef struct {
	int depth;
	char *type;
	char *term;
	char *definition;
} ENTRY;

/* Lexic */
typedef struct {
	int depth;
	int count;
	//char* type;
	ENTRY * list[LISTMAX];
} LEXIC;

/* List of Atoms*/
struct nothing {
	int depth;
	int count;
	char* type;
	ATOM* list[LISTMAX];
};

/* Context */
struct something {
	int depth;
	char* type;
   LEXIC *lexic;
   ATOMS* statements;
	ATOMS* examples;
}   ;
/************************************************************************************************/
/*                                                                                              */
/************************************************************************************************/
void printAllocate(bool fail, int depth, char *general,char *special, uintptr_t p);
void printFree(int depth, char *general,char *special,  uintptr_t p);
void printInside(char *fcn, char *type, uintptr_t p);
/************************************************************************************************/
/*                                                                                              */
/************************************************************************************************/
char *depth2indent(int depth);
char * uncledIndent(char *parentIndent);
char * ununcledIndent(char *parentIndent);
char *indentchild(bool neice, char *parentIndent);
char *before(char *indent, bool spaced);

int promptAndScan(char *prompt, char *text);
int promptAndScanInstruction(char *instruction, char *text, char *sub);
int promptAndScanManditoryInstruction(char *instruction, char *text, char *sub);
bool scanYesNo(char *instruction, char *sub);
char * ReplaceSpaces(char source[]);
char * RemoveSpaces(char source[]);
char *statementToQuestion(char *a);
char *questionToStatement(char *q);
char *questionToPointlessStatement(char *q);

char *resizetext(char *text);
/************************************************************************************************/
/*                                                                                              */
/************************************************************************************************/
void repeatfprintf(FILE *fp,char *s, int n);
void framefprintf(FILE *fp,char *s, int n);
void printNBHypothesis();
void printNBReason();
void printNBLexic();
void printNBpublication();
/************************************************************************************************/
/*                                                                                              */
/************************************************************************************************/
void createFiles(char givenname[], ATOM *q);
/************************************************************************************************/
/*                                                                                              */
/************************************************************************************************/
void getLexic(ATOM *a,char *expression, char *sub);
void getReference(ATOM *a, char *prompt);
ATOM * neuroscience_mmm();
ATOM *mergeQuestions(ATOM *p, ATOM *q);
//int mmm_static();
/************************************************************************************************/
/*                           STRUCTURES                                                         */
/************************************************************************************************/
ATOM *copyATOM(ATOM *a,int d);
void copyLEXIC(LEXIC *newl,LEXIC *l,int d);
void copyCONTEXT(ATOM *newa,ATOM *a,int d);
void copyATOMS(ATOMS *newa,ATOMS *a,int d);
void copyANSWERS(ATOM *newa,ATOM *a,int d);


void initLEXIC(CONTEXT *c);//
void freeLEXIC(LEXIC* l);//

void recordNewENTRY(LEXIC* l,char *term,char *definition);//
void freeENTRY(ENTRY *e, int n);

ATOMS *initATOMS(int depth,char *type);//
void freeATOMS(ATOMS* a);//

ATOM * recordNewATOM(ATOMS *a, char *text,char *tag);
//void recordNewATOMalt(ATOMS *a, char *text, char *type);

ATOM* initATOM(char* text,	char * type,char *tag, int depth);//
int freeATOM(ATOM *a);//

void initCONTEXT(ATOM* a);//
void freeCONTEXT(CONTEXT* c,char* type);//
/************************************************************************************************/
/*                           LATEX                                                              */
/************************************************************************************************/
void fprintLatexQUESTION(FILE *fp,ATOM* q);
void fprintLatexCONTEXT(FILE* fp, CONTEXT *c, char *indent);
void fprintLatexLEXIC(FILE* fp, LEXIC *l, char *indent);
void fprintLatexATOMS(FILE* fp, ATOMS *a, char *indent);
/************************************************************************************************/
/*                           FOREST                                                             */
/************************************************************************************************/
void fprintForestQUESTION(FILE *fp,ATOM* q);
void fprintForestANSWERS(FILE* fp, ATOMS *a, bool neice, char *indent);
void fprintForestCONTEXT(FILE* fp, CONTEXT *c, bool neice, bool bigbrother, char *indent);
void fprintForestLEXIC(FILE* fp, LEXIC *l, char *indent);
void fprintForestATOMS(FILE* fp, ATOMS *a, bool neice, bool bigbrother, bool spaced, char *indent);

void fprintForestQUESTIONperDepth(FILE *fp,ATOM* q,int d);
void fprintForestANSWERSperDepth(FILE* fp, ATOMS *a, bool neice, char *indent,int d);
void fprintForestCONTEXTperDepth(FILE* fp, CONTEXT *c, bool neice, bool bigbrother, char *indent,int d);
void fprintForestLEXICperDepth(FILE* fp, LEXIC *l,char *indent,int d);
void fprintForestATOMSperDepth(FILE* fp, ATOMS *a, bool neice, bool bigbrother, bool spaced, char *indent,int d);
/************************************************************************************************/
/*                           Tikz                                                                   */
/************************************************************************************************/
void fprintTikzQUESTION(FILE *fp,ATOM* q);
char * fprintfTikzENTRY(FILE *fp, ENTRY* e,  char *contextnode,char *place);
char * fprintfTikzATOM(FILE* fp, ATOM *a, char *contextnode, char *place);
void fprintfTikzANSWER(FILE* fp, ATOM *a);
void fprintfTikzCONTEXT(FILE *fp, CONTEXT* c, char *contextnode);
/************************************************************************************************/
/*                           MARKUP                                                             */
/************************************************************************************************/
void fprintMarkupQUESTION(FILE *fp,ATOM* q);
void fprintMarkupANSWERS(FILE* fp, ATOMS *a);
void fprintMarkupANSWER(FILE* fp, ATOM *a);
void fprintMarkupATOMS(FILE* fp, ATOMS *a);
void fprintMarkupATOM(FILE* fp, ATOM *a);
void fprintMarkupENTRY(FILE* fp, ENTRY *e);
void fprintMarkupLEXIC(FILE* fp, LEXIC *l);
void fprintMarkupCONTEXT(FILE* fp, CONTEXT *c);
/************************************************************************************************/
/*                            READ                                                              */
/************************************************************************************************/
ATOM * readQUESTION(char *filename);
#endif // TETE_H_