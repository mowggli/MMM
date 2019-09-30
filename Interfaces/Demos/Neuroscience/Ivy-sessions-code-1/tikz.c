/*

*/



#include "tete.h"
int atomNo = 0;
int answerX=0;
int answerY=0;

int entryNo = 0;

int statementNo = 0;
int answerNo = 0;

int exampleNo = 0;
/************************************************************************************************/
/*                                                                                              */
/************************************************************************************************/
void fprintfRepeat(FILE *fp, char *c, int i){
	int j;
	for(j=0;j<i;j++){
		fprintf(fp, "%s",c);
	}
}
void fprintfCommentedTitle(FILE *fp, char *title){
	int l=strlen(title);
	int whites=(80-l-4)/2;
	fprintfRepeat(fp,"\%",80);
	fprintf(fp,"\n");
	fprintfRepeat(fp,"\%",2);
	fprintfRepeat(fp," ",whites);
	fprintf(fp,"%s",title);
	fprintfRepeat(fp," ",whites);
	fprintfRepeat(fp,"\%",2);
	fprintf(fp,"\n");
	fprintfRepeat(fp,"\%",80);
	fprintf(fp,"\n");
}
/************************************************************************************************/
/*                                                                                              */
/************************************************************************************************/
void fprintTikzBegin(FILE *fp,ATOM *q){
	fprintf(fp,"\\begin{center}\n");
	fprintf(fp,"\t\\scalebox{0.5}{\\fbox{\\footnotesize\\begin{tikzpicture}[scale=0.5,node distance=1mm and 1mm,\n");
	fprintf(fp,"\tQuestion/.style={diamond,draw,fill=orange,minimum size=10pt,align=center,inner sep=0},\n");
	fprintf(fp,"\tPoint/.style={circle,inner sep=0pt,minimum size=5pt,fill=black,draw},\n");
	fprintf(fp,"\tBranch/.style={circle,inner sep=0pt,minimum size=5pt,fill=black,draw},\n");
	fprintf(fp,"\tsAnd/.style={circle,draw,fill=yellow,inner sep=0.5pt,minimum size=5pt},\n");
	fprintf(fp,"\tqAnd/.style={circle,draw,fill=orange,inner sep=0.5pt,minimum size=5pt},\n");
	fprintf(fp,"\tEntry/.style={text width= 5cm,ellipse,draw,fill=blue!30,align=center,inner sep=0},\n");
	fprintf(fp,"\tExample/.style={rectangle, fill=white,draw,align=center,inner sep=2},\n");
	fprintf(fp,"\tAnswer/.style={text width= 5cm,rectangle,draw,fill=yellow,align=center,inner sep=1},\n");
	fprintf(fp,"\tStatement/.style={text width= 5cm,rectangle,draw,fill=yellow,align=center,inner sep=1}]\n");
	fprintf(fp,"\t\\tikzset{forexample/.style={->,>={Turned Square[open,length=0.3cm]},orange}}\n");
}
void fprintTikzEnd(FILE *fp){
	fprintf(fp,"\\end{tikzpicture}}}\\end{center}\n");
}
/************************************************************************************************/
/*                                                                                              */
/************************************************************************************************/
void fprintTikzQUESTION(FILE *fp,ATOM* q){
	fprintTikzBegin(fp,q);
	fprintfCommentedTitle(fp, "QUESTION");
	fprintf(fp,"\\path (0,0) node [Question,text width= 3cm] (Question) {%s};\n",resizetext(q->text));
	fprintf(fp,"\\path (Question.south)+(0,-45) node [Branch] (QuestionBranch) {};\n");
	fprintf(fp,"\\path ($(Question.south)!.1!(QuestionBranch.north) $) node [qAnd] (QuestionContext) {\\tiny $\\wedge$};\n");
	fprintf(fp,"\\draw (Question) -- (QuestionContext) -- (QuestionBranch);\n");
	if(q->context){
		fprintfCommentedTitle(fp, " CONTEXT");
		fprintfTikzCONTEXT(fp, q->context, "QuestionContext");
	}
	answerX=-q->answers->count;
	answerY=-3;
	int i;
	for(i=0;i<q->answers->count;i++){
		fprintfCommentedTitle(fp, " ANSWER ");
		fprintfTikzANSWER(fp, q->answers->list[i]);
	}	
	fprintTikzEnd(fp);
}
/************************************************************************************************/
/*                                                                                              */
/************************************************************************************************/
char * fprintfTikzENTRY(FILE *fp, ENTRY* e,  char *contextnode, char *place){
	//fprintf(fp,"\\draw (%s) -- ++(-4,%d) node (%s) [Entry] {%s:=%s};\n",contextnode,y,e->term,e->term,e->definition);
	fprintf(fp,"\\node  [Entry,%s] (%s) {%s:=%s};\n",place,RemoveSpaces(e->term),e->term,resizetext(e->definition));
	fprintf(fp,"\\draw (%s) --  (%s) ;\n",contextnode,RemoveSpaces(e->term));
	static char newplace[2000];
	snprintf(newplace,2000,"above= of %s",RemoveSpaces(e->term));
	return newplace;
}
/************************************************************************************************/
/*                                                                                              */
/************************************************************************************************/
char * fprintfTikzATOM(FILE* fp, ATOM *a, char *contextnode, char *place){
	char node[2000];
	snprintf(node,2000,"%s%d",a->type,atomNo);
	char newcontextnode[2000];
	snprintf(newcontextnode,2000,"%sContext",node);
	static char newplace[2000];
	//fprintf(fp,"\\draw (%s) -- +(-4,%d) node (%s) [%s] {%s};\n",contextnode,y,node,a->type,a->text);
	fprintf(fp,"\\node  [%s,%s] (%s) {%s};\n",a->type,place,node,resizetext(a->text));
	fprintf(fp,"\\draw (%s) --  (%s.east) ;\n",contextnode,node);
	if(a->context){
		fprintf(fp,"\\path (%s.north)+(0,1) node [Branch] (%sBranch) {};\n",node,node);
		fprintf(fp,"\\path ($(%s.north)!.3!(%sBranch.south) $) node [sAnd] (%s) {\\tiny $\\wedge$};\n",node,node,newcontextnode);
		fprintf(fp,"\\path (%sBranch.north)+(0,0.5) node [Question] (%sQuestion) {};\n",node,node);
		fprintf(fp,"\\draw (%s) -- (%s) -- (%sBranch) --  (%sQuestion);\n",node,newcontextnode,node,node);
		atomNo++;	
		fprintfCommentedTitle(fp, " CONTEXT");
		fprintfTikzCONTEXT(fp, a->context, newcontextnode);
		snprintf(newplace,2000,"above=1.5cm of %s",node);
	}
	else{
		snprintf(newplace,2000,"above= of %s",node);
	}
	return newplace;
}
void fprintfTikzANSWER(FILE* fp, ATOM *a){
	char node[2000];
	snprintf(node,2000,"%s%d",a->type,answerNo);
	char newcontextnode[2000];
	snprintf(newcontextnode,2000,"%sContext",node);
	fprintf(fp,"\\draw (QuestionBranch) -- ++(%d,%d) node (%s) [%s] {%s};\n",answerX,answerY,node,a->type,resizetext(a->text));
	fprintf(fp,"\\path ($(QuestionBranch)!0.9!(%s)$) node [sAnd] (%s) {\\tiny $\\wedge$};\n",node,newcontextnode);
	answerNo++;
	answerX=answerX+3;
	answerY=answerY-5;
	if(a->context){
		fprintfCommentedTitle(fp, " CONTEXT");
		fprintfTikzCONTEXT(fp, a->context, newcontextnode);
	}
}
/************************************************************************************************/
/*                                                                                              */
/************************************************************************************************/
void fprintfTikzCONTEXT(FILE *fp, CONTEXT* c, char *contextnode){
	if(c){
	char place[2000];
	snprintf(place,2000,"left=2.5cm of %s",contextnode);
	int i;
	//int y=ceil((c->lexic->count+c->statements->count+c->examples->count)/2);
	if(c->lexic->count>0){
		fprintfCommentedTitle(fp, "  LEXIC ");
		for(i=0;i<c->lexic->count;i++){
			strcpy(place,fprintfTikzENTRY(fp, c->lexic->list[i],contextnode, place));
		}		
	}
	for(i=0;i<c->examples->count;i++){
		fprintfCommentedTitle(fp, " EXAMPLE");
		strcpy(place,fprintfTikzATOM(fp, c->examples->list[i], contextnode, place));
	}	
	for(i=0;i<c->statements->count;i++){
		fprintfCommentedTitle(fp, " STATEMENT");
		strcpy(place,fprintfTikzATOM(fp, c->statements->list[i], contextnode, place));
	}	
	}
}
/************************************************************************************************/
/*                                                                                              */
/************************************************************************************************/

