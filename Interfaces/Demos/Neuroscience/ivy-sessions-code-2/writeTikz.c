/************************************************************************************************/
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                                 W R I T E   T I K Z . C                                      */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
#include "tete.h"
int ATOMNB = 0;
int ANSWERX=0;
int ANSWERY=0;
int ENTRYNB = 0;
int STATEMENTNB = 0;
int ANSWERNB = 0;
int EXAMPLENB = 0;

char LEXICTikzStyle[]="LEXIC";
char SUPPORTTikzStyle[]="SUPPORT";
char DISPORTTikzStyle[]="DISPORT";
char OBSOLETIONTikzStyle[]="OBSOLETION";
char INSTANTIATIONTikzStyle[]="Example";
char CONTEXTTikzStyle[]="Context";
char CONTEXTSTikzStyle[]="CONTEXTS";
char ANSWERSTikzStyle[]="Answers";
char ANSWERTikzStyle[]="ANSWER";
char QUESTIONTikzStyle[]="Question";
char ATOMTikzStyle[]="Statement";
char ATOMSTikzStyle[]="ATOMS";
char ENTRYTikzStyle[]="Entry";
char TERMTikzStyle[]="TERM";
char DEFINITIONTikzStyle[]="DEFINITION";
char TEXTTikzStyle[]="TEXT";
char METATikzStyle[]="META";
char TAGTikzStyle[]="TAG";
char AUTHORTikzStyle[]="AUTHOR";
char IDTikzStyle[]="ID";
char PARENTIDTikzStyle[]="PARENTID";
char COUNTTikzStyle[]="COUNT";
/************************************************************************************************/
/* TITLE IN TIKZ                                                                                */
/************************************************************************************************/
void writeTikzTitle(FILE *fp, char *title){
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
/* BEGIN  & END OF TIKZ ENVIRONMENT                                                             */
/************************************************************************************************/
void openTikzEnvironment(FILE *fp,ATOM *q){
	fprintf(fp,"\\begin{center}\n");
	fprintf(fp,"\t\\scalebox{0.5}{\\fbox{\\footnotesize\\begin{tikzpicture}[scale=0.5,node distance=1mm and 1mm,\n");
	fprintf(fp,"\t%s/.style={diamond,draw,fill=orange,minimum size=10pt,align=center,inner sep=0},\n",QUESTIONTikzStyle);
	fprintf(fp,"\tPoint/.style={circle,inner sep=0pt,minimum size=5pt,fill=black,draw},\n");
	fprintf(fp,"\tBranch/.style={circle,inner sep=0pt,minimum size=5pt,fill=black,draw},\n");
	fprintf(fp,"\tsAnd/.style={circle,draw,fill=yellow,inner sep=0.5pt,minimum size=5pt},\n");
	fprintf(fp,"\tqAnd/.style={circle,draw,fill=orange,inner sep=0.5pt,minimum size=5pt},\n");
	fprintf(fp,"\t%s/.style={text width= 5cm,ellipse,draw,fill=blue!30,align=center,inner sep=0},\n",ENTRYTikzStyle);
	fprintf(fp,"\t%s/.style={rectangle, fill=white,draw,align=center,inner sep=2},\n",INSTANTIATIONTikzStyle);
	fprintf(fp,"\t%s/.style={text width= 5cm,rectangle,draw,fill=yellow,align=center,inner sep=1},\n",ANSWERTikzStyle);
	fprintf(fp,"\t%s/.style={text width= 5cm,rectangle,draw,fill=yellow,align=center,inner sep=1}]\n",ATOMTikzStyle);
	fprintf(fp,"\t\\tikzset{forexample/.style={->,>={Turned Square[open,length=0.3cm]},orange}}\n");
}
void closeTikzEnvironment(FILE *fp){
	fprintf(fp,"\\end{tikzpicture}}}\\end{center}\n");
}
/************************************************************************************************/
/* QUESTION                                                                                     */
/************************************************************************************************/
/************************************************************************************************/
/* META                                                                                         */
/************************************************************************************************/
int writeTikzMETA(FILE *fp,META *p,char *indent){
	return 0;
}
/************************************************************************************************/
/* QUESTION                                                                                     */
/************************************************************************************************/
int writeTikzQUESTION(FILE *fp,ATOM *p,char *indent){
	if(p){
		openTikzEnvironment(fp,p);
		writeTikzTitle(fp, "QUESTION");
		char node[5000], branchnode[5000], contextnode[5000];
		strcpy(node,p->Meta->ID);
		strcpy(branchnode,p->Meta->ID);strcat(branchnode,"Branch");
		strcpy(contextnode,p->Meta->ID);strcat(contextnode,"Context");
		fprintf(fp,"\\path (0,0) node [%s,text width= 3cm] (%s) {%s};\n",QUESTIONTikzStyle,node,resizetext(p->Text,"...?"));
		fprintf(fp,"\\path (%s.south)+(0,-45) node [Branch] (%s) {};\n",node,branchnode);
		fprintf(fp,"\\path ($(%s.south)!.1!(%s.north) $) node [qAnd] (%s) {\\tiny $\\wedge$};\n",node,branchnode,contextnode);
		fprintf(fp,"\\draw (%s) -- (%s) -- (%s);\n",node,contextnode,branchnode);
		writeTikzCONTEXTS(fp,p->Contexts,contextnode);
		writeTikzANSWERS(fp,p->Answers,branchnode);
		closeTikzEnvironment(fp);
	}
	return 0;
}
/************************************************************************************************/
/* ANSWER                                                                                       */
/************************************************************************************************/
void writeTikzANSWER(FILE* fp, ATOM *p, char *branchnode){
	if(p){
		char node[5000], contextnode[5000];
		strcpy (node,p->Meta->ID);
		strcpy(contextnode,p->Meta->ID);strcat(contextnode,"Context");
		fprintf(fp,"\\draw (%s) -- ++(%d,%d) node (%s) [%s] {%s};\n",branchnode,ANSWERX,ANSWERY,node,ANSWERTikzStyle,resizetext(p->Text,"..."));
		fprintf(fp,"\\path ($(%s)!0.9!(%s)$) node [sAnd] (%s) {\\tiny $\\wedge$};\n",branchnode,node,contextnode);
		ANSWERNB++;
		ANSWERX=ANSWERX+3;
		ANSWERY=ANSWERY-5;
		if(p->Contexts){
			writeTikzCONTEXTS(fp,p->Contexts,contextnode);	
		}
	}
}
/************************************************************************************************/
/* ATOM                                                                                         */
/************************************************************************************************/
char *writeTikzATOM(FILE *fp,ATOM *p,char *parentcontextnode,char *place){
	if(p){
		static char newplace[2000];
		char node[5000], contextnode[5000], branchnode[5000], questionnode[5000];
		strcpy(node,p->Meta->ID);
		strcpy(branchnode,p->Meta->ID);strcat(branchnode,"Branch");
		strcpy(contextnode,p->Meta->ID);strcat(contextnode,"Context");
		strcpy(questionnode,p->Meta->ID);strcat(questionnode,"Question");
		fprintf(fp,"\\node  [%s,%s] (%s) {%s};\n",ATOMTikzStyle,place,node,resizetext(p->Text,"..."));
		fprintf(fp,"\\draw (%s) --  (%s.east) ;\n",parentcontextnode,node);
		if(p->Contexts){
			int i;
			for(i=0;i<p->Contexts->Count;i++){
				fprintf(fp,"\\path (%s.north)+(0,1) node [Branch] (%s) {};\n",node,branchnode);
				fprintf(fp,"\\path ($(%s.north)!.3!(%s.south) $) node [sAnd] (%s) {\\tiny $\\wedge$};\n",node,branchnode,contextnode);
				fprintf(fp,"\\path (%s.north)+(0,0.5) node [Question] (%s) {};\n",branchnode,questionnode);
				fprintf(fp,"\\draw (%s) -- (%s) -- (%s) --  (%s);\n",node,contextnode,branchnode,questionnode);
				ATOMNB++;	
				writeTikzTitle(fp," CONTEXT");
				writeTikzCONTEXT(fp,p->Contexts->List[i],contextnode);
				snprintf(newplace,2000,"above=1.5cm of %s",node);
			}			
		} else{
			snprintf(newplace,2000,"above= of %s",node);
		}
		return newplace;
	}
	return NULL;
}
/************************************************************************************************/
/* ANSWERS                                                                                        */
/************************************************************************************************/
int writeTikzANSWERS(FILE *fp,ATOMS *p,char *branchnode){
	if(p && p->Count>0){
		ANSWERX=-p->Count;
		ANSWERY=-3;
		int i;
		for(i=0;i<p->Count;i++){
				writeTikzTitle(fp," ANSWER ");
				writeTikzANSWER(fp,p->List[i],branchnode);
		}			
	}
	return 0;		
}
/************************************************************************************************/
/* ATOMS                                                                                        */
/************************************************************************************************/
int writeTikzATOMS(FILE *fp,ATOMS *p,char *parentcontextnode,char *place){
	if(p && p->Count>0){
		int i;
		for(i=0;i<p->Count;i++){
			writeTikzTitle(fp," ATOM");
			writeTikzATOM(fp,p->List[i],parentcontextnode,place);
		}			
	}
	return 0;		
}
/************************************************************************************************/
/* CONTEXT                                                                                      */
/************************************************************************************************/
int writeTikzCONTEXT(FILE *fp,CONTEXT *p,char *contextnode){
	if(p && !isemptyCONTEXT(p)){
		char place[2000];
		snprintf(place,2000,"left=2.5cm of %s",contextnode);
		int i;
		if(p->Lexic && p->Lexic->Count>0){
			writeTikzTitle(fp, "  LEXIC ");
			for(i=0;i<p->Lexic->Count;i++){
				strcpy(place,writeTikzENTRY(fp,p->Lexic->List[i],contextnode,place));
			}		
		}
		if(p->Support && p->Support->Count>0){
			writeTikzTitle(fp, "  SUPPORT");
			for(i=0;i<p->Support->Count;i++){
				strcpy(place,writeTikzATOM(fp,p->Support->List[i],contextnode,place));
			}		
		}
		if(p->Disport && p->Disport->Count>0){
			writeTikzTitle(fp, "  DISPORT");
			for(i=0;i<p->Disport->Count;i++){
				strcpy(place,writeTikzATOM(fp,p->Disport->List[i],contextnode,place));
			}		
		}
		if(p->Obsoletion && p->Obsoletion->Count>0){
			writeTikzTitle(fp, "OBSOLETION");
			for(i=0;i<p->Obsoletion->Count;i++){
				strcpy(place,writeTikzATOM(fp,p->Obsoletion->List[i],contextnode,place));
			}		
		}
		if(p->Instantiation && p->Instantiation->Count>0){
			writeTikzTitle(fp, "INSTANTIATION");
			for(i=0;i<p->Instantiation->Count;i++){
				strcpy(place,writeTikzATOM(fp,p->Instantiation->List[i],contextnode,place));
			}		
		}
	}
	return 0;	
}
/************************************************************************************************/
/* CONTEXTS                                                                                     */
/************************************************************************************************/
int writeTikzCONTEXTS(FILE *fp,CONTEXTS *p,char *contextnode){
	if(p && p->Count>0){
		int i;
		for(i=0;i<p->Count;i++){
			writeTikzTitle(fp," CONTEXT");
			writeTikzCONTEXT(fp,p->List[i],contextnode);
		}			
	}
	return 0;		
}
/************************************************************************************************/
/* ENTRY                                                                                        */
/************************************************************************************************/
char *writeTikzENTRY(FILE *fp,ENTRY *p,char *contextnode, char *place){
	char node[5000];
	strcpy(node,p->Meta->ID);
	fprintf(fp,"\\node  [%s,%s] (%s) {%s:=%s};\n",ENTRYTikzStyle,place,node,p->Term,resizetext(p->Definition,"..."));
	fprintf(fp,"\\draw (%s) --  (%s) ;\n",contextnode,node);
	static char newplace[2000];
	snprintf(newplace,2000,"above= of %s",node);
	return newplace;
}
/************************************************************************************************/
/* LEXIC                                                                                        */
/************************************************************************************************/
int writeTikzLEXIC(FILE *fp,LEXIC *p,char *indent){
	if(p){
		int i;
		for(i=0;i<p->Count;i++){
			//writeTikzENTRY(fp,p->List[i],indent);
		}
	}
	return 0;		
}
