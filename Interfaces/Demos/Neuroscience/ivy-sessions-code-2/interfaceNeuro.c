/************************************************************************************************/
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                              I N T E R F A C E    N E U R O  . C                             */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
#include "tete.h"
int PROMPTNO=1;
/************************************************************************************************/
/*                                      GET STUFF                                               */
/************************************************************************************************/
void getLexic(ATOM *a,char *expression, char *sub){

}

void getReference(ATOM *a, char *prompt){	

}

bool getReferenceInstruction(ATOM *a){	
	return true;
}

void getReasons(CONTEXT * c, char *instruction, char *subinstruction){

}

void getConfounders(ATOM *experiment){

}

ATOM *getExperiment(){

	return NULL;
}

ATOM *getArgument(ATOM *answer, ATOM *experiment){
	
	return NULL;
}
/************************************************************************************************/
/*                               STRIP CONFOUNDER                                               */
/************************************************************************************************/
char *stripConfounder(ATOM *a){
	return NULL;
}

/************************************************************************************************/
/*                                                                                              */
/*                                                                                              */
/*                            REGISTER AND CLOSE                                                */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/

ATOM * registerStudy(){
	int DEPTH=QUESTIONdepth(0);
	char text[5000], author[5000], tempid[5000];
	numberedinstruction("What is the working hypothesis of your study?"); /*Please frame your working hypothesis*/
	strcpy(text,promptandscan("HYPOTHESIS",true));
	hypothesisNote();
	/* Author */
	numberedinstruction("Who are the authors of this study?"); 
	strcpy(author,promptandscan("AUTHOR",false));	
	authorNote();
	/* Meta */
	META *m=initMETA(QUESTIONTAG,author,DEPTH,newid(),"NONE");
	/* Object */
	ATOM* p = initATOM(m,statementToQuestion(text),NULL,NULL);
	/* Lexic */
	strcpy(tempid,newid()); /* id for CONTEXT parent of lexic */
	numberedinstruction("Please can you list non-obvious terms that you used in  your wording of your working hypothesis?");
	unmberedinstruction("(type enter to skip at any point)");
	LEXIC *lexic = getLEXIC(QUESTIONtoENTRYdepth(m->Depth),initMETA(LEXICTAG,author,QUESTIONtoLEXICdepth(DEPTH),newid(),tempid));
	/* Contexts to put lexic in */
	if(lexic){
		p->Contexts = initCONTEXTS(initMETA(CONTEXTSTAG,author,QUESTIONtoCONTEXTSdepth(m->Depth),newid(),m->ID));
		recordNewCONTEXT(tempid,p->Contexts,lexic,NULL,NULL,NULL,NULL);
	}
	/* Answers */
	DEPTH=QUESTIONtoANSWERSdepth(DEPTH);
	p->Answers=initATOMS(initMETA(ANSWERSTAG,author,DEPTH,newid(),m->ID));
	DEPTH=ANSWERStoANSWERdepth(DEPTH);
	ATOM *yes = recordNewANSWER(newid(),p->Answers,"Yes.");
	ATOM *no  = recordNewANSWER(newid(),p->Answers,"No.");
	ATOM *wat = recordNewANSWER(newid(),p->Answers,"The question doesn't make sense or is irrelevant.");
	/* Answer CONTEXTS */
	DEPTH=ATOMtoCONTEXTSdepth(DEPTH);	
	yes->Contexts=initCONTEXTS(initMETA(CONTEXTSTAG,author,DEPTH,newid(),yes->Meta->ID));
	no->Contexts =initCONTEXTS(initMETA(CONTEXTSTAG,author,DEPTH,newid(), no->Meta->ID));
	wat->Contexts=initCONTEXTS(initMETA(CONTEXTSTAG,author,DEPTH,newid(),wat->Meta->ID));
	/* Answer CONTEXTs*/
	DEPTH=CONTEXTStoCONTEXTdepth(DEPTH);	
	CONTEXT *yesContext = recordNewCONTEXT(newid(),yes->Contexts,NULL,NULL,NULL,NULL,NULL);
	CONTEXT *noContext  = recordNewCONTEXT(newid(),no->Contexts, NULL,NULL,NULL,NULL,NULL);
	CONTEXT *watContext = recordNewCONTEXT(newid(),wat->Contexts,NULL,NULL,NULL,NULL,NULL);
	DEPTH=CONTEXTtoATOMSdepth(DEPTH);
	/*Support */
	numberedinstruction("What reasons presently supports your working hypothesis?"); 
	unmberedinstruction("Please list one reason at a time.");
	unmberedinstruction("Reasons can be motivations, statements drawn from the state of the art of your field and/or from logical inferences."); 
	changeATOMPROMPT("SUPPORTING REASON");
	yesContext->Support = getREASONS(initMETA(ATOMPROMPT,author,DEPTH,newid(),yesContext->Meta->ID));
	/* Disport */
	numberedinstruction("What reasons disagree/oppose/negate your working hypothesis? To your present knowledge are there reasons to expect your working hypothesis might not be true?"); 
	unmberedinstruction("Please list one reason at a time.");
	unmberedinstruction("Reasons can be negative results, facts or statements drawn from the state of the art of your field and/or from logical inferences, that disagree/oppose/negate your working hypothesis."); 
	changeATOMPROMPT("REASON AGAINST");
	noContext->Support = getREASONS(initMETA(ATOMPROMPT,author,DEPTH,newid(), noContext->Meta->ID));
	/* Obsoletion */
	numberedinstruction("Are there reasons to expect your working hypothesis might be irrelevant or absurd? What could make you working hypothesis irrelevant/obsolete/innefficient?"); 
	unmberedinstruction("Please list one reason at a time.");
	unmberedinstruction("Reasons can be ambiguity in the wording of the hypothesis, uncertainty in the meaning of certain terms it involves, etc."); 
	changeATOMPROMPT("OBSOLETING REASON");
	watContext->Support = getREASONS(initMETA(ATOMPROMPT,author,DEPTH,newid(),watContext->Meta->ID));
	/* Experiment */
	DEPTH = ATOMStoATOMdepth(DEPTH);
	numberedinstruction("Please describe your experiment/study (eg: experiment design and main methods)."); 
	changeATOMPROMPT("EXPERIMENT");
	ATOM * experiment = getNecessaryATOM(initMETA("EXPERIMENT",author,DEPTH,newid(),yesContext->Support->Meta->ID),"Missing experiment description.");
	yesContext->Support->List[yesContext->Support->Count] = experiment;
	yesContext->Support->Count++;
	/* Confounders */
	numberedinstruction("What might confound/undermine/limit/go wrong in your study/experiment? Please list all possibilities you can think of. Please lits things you know/one might expect /are likely to undermine your study."); 
	unmberedinstruction("Please list one confounder at a time.");
	unmberedinstruction("..."); 
	changeATOMPROMPT("CONFOUNDER/LIMIT");
	experiment->Contexts->List[0] = getCONFOUNDERS(initMETA(tag,authorm,depth,newid(),parentid));
	return p;
}

/************************************************************************************************/
/*                                                                                              */
/************************************************************************************************/
ATOM * closeStudy(ATOM *q){ 
//setInstructionsF();
	return NULL;
}
/************************************************************************************************/
/*                                                                                              */
/************************************************************************************************/
void setInstructionsP(){
	

}
void setInstructionsS(){
	

}
void setInstructionsF(){
	

}
/************************************************************************************************/
/*                                                                                              */
/*                                                                                              */
/*                           NEUROSCIENCE                                                       */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
ATOM *neuroscience_mmm(){
	
	return NULL;
}

/************************************************************************************************/
/*                                                                                              */
/*                                                                                              */
/*                                    MERGE                                                     */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
ATOM *mergeQuestions(ATOM *p, ATOM *q){
	return NULL;
}
