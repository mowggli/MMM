#include "tete.h"

//int promptno =1;
char status[]="ZZ";
char		hypothesisInstruction[5000] ;
char     yesReasonsInstruction[5000] ;
char      noReasonsInstruction[5000] ;
char     botReasonsInstruction[5000] ;
char    confoundersInstruction[5000] ;
char addconfoundersInstruction[5000] ;
char           dataInstruction[5000] ; 
char         authorInstruction[5000] ;
char     experimentInstruction[5000] ;
char         resultInstruction[5000] ;
char    publicationInstruction[5000] ;
char    statustext[5000] ;
char    defaultLexicprompt[5000] ;
char defaultPublicationPrompt[5000];

bool lexicNBtodo=true;
bool reasonNBtodo=true;
bool publicationNBtodo=true;

/************************************************************************************************/
/*                                                                                              */
/*                                                                                              */
/*                                	   I N T E R F A C E                                        */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/

/************************************************************************************************/
/*                                      GET STUFF                                               */
/************************************************************************************************/
void getLexic(ATOM *a,char *expression, char *sub){
	char term[5000], definition[5000], prompt[5000];
	printf("\t(%d%s) %s %s?\n\t    (type enter to skip at any point)\n",promptno-1,sub,defaultLexicprompt,expression);
	printNBLexic();
	//snprintf(prompt,5000,"\t(%d%s) %s %s?\n\t    (type enter to skip at any point)\nTERM ",promptno-1,sub,defaultLexicprompt,expression);
	snprintf(prompt,5000,"TERM "); //SIMPLIFY!
	if(promptAndScan(prompt, term)){
		snprintf(prompt,5000,"\t:= ");
		if(!promptAndScan(prompt, definition)){
				strcpy(definition,"??");
		}	
		if(!a->context){initCONTEXT(a);}
		recordNewENTRY(a->context->lexic,term,definition);
		snprintf(prompt,5000,"TERM ");
		while(promptAndScan(prompt, term)){
			snprintf(prompt,5000,"\t:= ");
			if(!promptAndScan(prompt, definition)){
				strcpy(definition,"??");
			}	
			recordNewENTRY(a->context->lexic,term,definition);
			snprintf(prompt,5000,"TERM ");
		}
	}
}

void getReference(ATOM *a, char *prompt){	
	char text[5000],publication[5000],altprompt[5000];
	snprintf(altprompt,5000,"\t(%db) %s",promptno-1,prompt);
	if(promptAndScan(altprompt, publication)){
		if(!a->context){initCONTEXT(a);}
		snprintf(text,5000,"Published in %s.",publication); 
		recordNewATOM(a->context->statements,text,"BIB");
		printNBpublication();
	}
}

bool getReferenceInstruction(ATOM *a){	
	char prompt[5000], text[5000],publication[5000];
	snprintf(prompt,5000,"\t(%d) %s\nBIB ",promptno,publicationInstruction);
	if(promptAndScan(prompt, publication)){
		snprintf(text,5000,"Published in %s.",publication); 
		//if(!a->context){printf("ok here??\n"); initCONTEXT(a);}
		recordNewATOM(a->context->statements,text,"BIB");
		printNBpublication();
		promptno++;
		return true;
	} else {
		snprintf(text,5000,"No publication referenced."); 
		//if(!a->context){printf("ok here??\n"); initCONTEXT(a);}
		recordNewATOM(a->context->statements,text,"NO BIB");
		promptno++;
		return false;
	}
}

void getReasons(CONTEXT * c, char *instruction, char *subinstruction){
	char text[5000],othertext[5000];
	ATOM * reason;
	if(promptAndScanInstruction(instruction,text,"")){ 
		reason = recordNewATOM(c->statements,text,"UPSTREAM");
		getLexic(reason,"your wording of this reason","a");//c->statements->list[c->statements->count-1]
		getReference(reason, defaultPublicationPrompt);
		promptno = promptno-1;
		while(promptAndScanInstruction(subinstruction,text,"")){
			reason = recordNewATOM(c->statements,text,"UPSTREAM");
			getLexic(reason,"your wording of this reason","a");
			getReference(reason, defaultPublicationPrompt);
			promptno = promptno-1;
		}
	}
	printNBReason();
}

void getConfounders(ATOM *experiment){
	char text[5000],othertext[5000], prompt[5000]; //,addressed[10];
	ATOM *confounder, *subexperiment;
	snprintf(prompt,5000,"%s\nCONFOUNDER/LIMIT ",confoundersInstruction); 
	while(promptAndScanInstruction(prompt,othertext,"")){
		if(scanYesNo(addconfoundersInstruction,"a")){
			snprintf(text,5000,"%s But this issue has been addressed.",othertext); 
			confounder = recordNewATOM(experiment->context->statements, text,"T1-CNFDR");
			initCONTEXT(confounder);
			if(promptAndScanInstruction("How? Please describe your experiment.\nEXPERIMENT ",text,"b")){
				subexperiment = recordNewATOM(confounder->context->statements, text,"EXPMT");
				initCONTEXT(subexperiment);
				getLexic(subexperiment,"your descripion of your experiment","b.a");	
			} else{
				subexperiment = recordNewATOM(confounder->context->statements, "Missing experiment description.","NO EXPMT");
			}
		} else {
			snprintf(text,5000,"%s But this is assumed not to happen.",othertext); 
			confounder = recordNewATOM(experiment->context->statements, text,"T2-CNFDR");	
		}
		promptno--;
	}
	confounder = recordNewATOM(experiment->context->statements, "Some unforseen thing could go wrong. But this is assumed not to happen.","T2-CNFDR");
}

ATOM *getExperiment(){
	char text[5000],othertext[5000];
	ATOM *experiment;
	if(!promptAndScanInstruction(experimentInstruction,othertext,"")){ 
		strcpy(text,"Missing experiment description.");
		experiment = initATOM(text,"Statement","EXPMT",5);
		initCONTEXT(experiment);
	} else {
		experiment = initATOM(othertext,"Statement","EXPMT",5);
		initCONTEXT(experiment);
		getLexic(experiment,"your descripion of your experiment","a");	//detailed explanation of certain jargon, experiment techniques or procedures , by referring to protocol document or other studies
	}
	getConfounders(experiment);
	// Data --------------------------------------------------------------------------------------
	if(promptAndScanInstruction(dataInstruction,text,"")){
		recordNewATOM(experiment->context->statements,text,"DATA");
	}
	return experiment;
}

ATOM *getArgument(ATOM *answer, ATOM *experiment){
	char text[5000],othertext[5000], author[5000];
	promptAndScanInstruction(authorInstruction,author,"");
	ATOM *argument,*actuality;
	if(!promptAndScanInstruction(resultInstruction,othertext,"")){
		snprintf(text,5000,"Some unspecified argument in favour of the answer being ' %s '.",answer->text);
		argument = recordNewATOM(answer->context->statements,text,"US MAIN"); 
		//argument = answer->context->statements->list[answer->context->statements->count-1];
		initCONTEXT(argument);
	} else{
		//snprintf(text,5000,"Efforts are being made to prove that %s : an experiment is currently being carried out by %s.",othertext,author);
		argument = recordNewATOM(answer->context->statements, othertext,"MAIN"); 
		//argument = answer->context->statements->list[answer->context->statements->count-1];
		initCONTEXT(argument);
		getLexic(argument,"what you wrote","a");
		getReferenceInstruction(argument);
	}
	if(strcmp(author,"")==0){
		snprintf(text,5000,"This statement ('%s') %s.",argument->text,statustext);
	} else {
		snprintf(text,5000,"This statement (refPA) %s by  %s.",statustext,author);
	} // Here we can put explicit reference to paren statement + refine the status
	actuality = recordNewATOM(argument->context->statements,text,"ACTU");
	//ATOM *actuality = argument->context->statements->list[argument->context->statements->count-1];
	initCONTEXT(actuality);
	actuality->context->statements->list[actuality->context->statements->count] = experiment;
	actuality->context->statements->count++;	
	return argument;
}
/************************************************************************************************/
/*                               STRIP CONFOUNDER                                               */
/************************************************************************************************/
char *stripConfounder(ATOM *a){
	static char text[5000];
	int l;
	if(strcmp(a->tag,"T1-CNFDR")==0){
		l = strlen(" But this is assumed not to happen.")+1;
		strcpy(text,a->text);
		text[strlen(a->text)-l] = '\0';
	} else if(strcmp(a->tag,"T2-CNFDR")==0){
		l = strlen(" But this issue has been addressed.")+1;
		strcpy(text,a->text);
		text[strlen(a->text)-l] = '\0';
	} else if(strcmp(a->tag,"T3-CNFDR")==0){
		l = strlen("We acknowledge there is this limitation to our study: ");
		strcpy(text,&a->text[l]);
	} else {
		fprintf(stderr,"Confounder expected, argument received by function stripConfounder isn't a confounder! Tag is %s and text is %s\n",a->tag,a->text);
	}
	return text;
}
/************************************************************************************************/
/*                               PRINT NOTES                                                    */
/************************************************************************************************/
void printNBHypothesis(){
	printf("\t");repeatfprintf(stdout,"_",120);printf("\n");
	framefprintf(stdout,"",120);
	framefprintf(stdout,"Here, the interface would ask the user if her working hypothesis is the same as",120);
	framefprintf(stdout,"similar ones previously recorded in the MMM by other users, assuming any similar ones are found.",120);
	printf("\t|");repeatfprintf(stdout,"_",118);printf("|\n\n");
	getchar();
}

void printNBLexic(){
	if(lexicNBtodo){
		printf("\t");repeatfprintf(stdout,"_",120);printf("\n");
		framefprintf(stdout,"",120);
		framefprintf(stdout,"Rather than have the user register her own new definition for each term or leave the definition blank,",120);
		framefprintf(stdout,"here the interface would make suggestions of definitions for her to choose from.",120);
		framefprintf(stdout,"These suggestions would be taken from the definitions previously given by other users for the same term.",120);
		framefprintf(stdout,"",120);
		framefprintf(stdout,"The user would also be asked if she wants to explicit relations between different terms.",120);
		framefprintf(stdout,"Relations between terms would be chosen among a small finite collection of possible relations, namely:",120);
		framefprintf(stdout,"",120);
		int i=1;
		char text[5000];
		snprintf(text,5000,"%d. Is an instance, type or special case of / Is an abstraction or generalisation of     ",i);i++;
		framefprintf(stdout,text,120);
		snprintf(text,5000,"%d. Is a defining part or property of is Is needed to define / to make sense, needs      ",i);i++;
		framefprintf(stdout,text,120);
		snprintf(text,5000,"%d. Is a part or property of /                                                           ",i);i++;
		framefprintf(stdout,text,120);
		snprintf(text,5000,"%d. Is about                                                                             ",i);i++;
		framefprintf(stdout,text,120);
		snprintf(text,5000,"%d. Is a representation of / is represented by or is an interpretation of (cf lexic's :=)",i);i++;
		framefprintf(stdout,text,120);
		snprintf(text,5000,"%d. Is translation or reformulation of (+explicitation of correspondence in both ways)   ",i);i++;
		framefprintf(stdout,text,120);
		snprintf(text,5000,"%d. Relates to (nb: using this type of link will be discouraged but still allowed)       ",i);i++;
		framefprintf(stdout,text,120);
		printf("\t|");repeatfprintf(stdout,"_",118);printf("|\n\n");
		lexicNBtodo=false;
		getchar();
	}
}

void printNBpublication(){
	if(publicationNBtodo){
		printf("\t");repeatfprintf(stdout,"_",120);printf("\n");
		framefprintf(stdout,"",120);
		framefprintf(stdout,"Some or all of the content of this publication might have been registered by someone else in the MMM already.",120);
		framefprintf(stdout,"In this case, the user would be shown the existing part(s) of the MMM relative to the publication and asked if",120);
		framefprintf(stdout,"one of them expresses the (specific) part of the publication that she means to refer to. If (the specific part",120);
		framefprintf(stdout,"of) the publication hasn't yet been registered in the MMM, the user would be given the possibility to register",120);
		framefprintf(stdout,"it herself. If she chose not to, then later, someone else might do it for her. Then, the user would be asked",120);
		framefprintf(stdout,"again (possibly years later) if she wanted to replace the reference by a link to the newly created part of",120);
		framefprintf(stdout,"the MMM relative to the publication.",120);
		printf("\t|");repeatfprintf(stdout,"_",118);printf("|\n\n");
		publicationNBtodo=false;
		getchar();
	}
}

void printNBReason(){
if(reasonNBtodo){
	printf("\t");repeatfprintf(stdout,"_",120);printf("\n");
	framefprintf(stdout,"",120);
	framefprintf(stdout,"Here, instead of typing statements expressing the reasons she is prompted to give,",120);
	framefprintf(stdout,"the user could also select statements already registered in the MMM.",120);
	framefprintf(stdout,"If preregistered statements are almost yet not exactly what the user wants to register,",120);
	framefprintf(stdout,"she is given the opportunity to explicit the relationship/difference between",120);
	framefprintf(stdout,"her newly registered statement and preregistered ones.",120);
	printf("\t|");repeatfprintf(stdout,"_",118);printf("|\n\n");
	reasonNBtodo=false;
	getchar();
}
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
	// Question -- Please frame your working hypothesis -------------------------------------------
	char text[5000], othertext[5000]; 
	ATOM *yes, *no, *absurd;
	promptAndScanManditoryInstruction(hypothesisInstruction, othertext, "");
	printNBHypothesis();
	//snprintf(text,5000,"Is it true that ' %s ' ?",othertext); 
	ATOM* q = initATOM(statementToQuestion(othertext),"Question","Question",0);
	initCONTEXT(q);
	getLexic(q,"your wording of your working hypothesis","a"); //add  definitions to help other people understand your research question;
	q->answers=initATOMS(q->depth+1,"Answers");
	// Yes answer ---------------------------------------------------------------------------------
	yes = recordNewATOM(q->answers, "Yes.","Answer");
	//ATOM *yes = q->answers->list[q->answers->count-1];
	initCONTEXT(yes);
	getReasons(yes->context,yesReasonsInstruction,"What reasons presently supports your working hypothesis?\nREASON FOR ");
	// No answer ----------------------------------------------------------------------------------
	no = recordNewATOM(q->answers, "No.","Answer");
	//ATOM *no = q->answers->list[q->answers->count-1];
	initCONTEXT(no);
	getReasons(no->context,noReasonsInstruction,"What reasons disagree/oppose/negate your working hypothesis?\nREASON AGAINST ");
	// Absurd answer ------------------------------------------------------------------------------
	absurd = recordNewATOM(q->answers, "The question doesn't make sense or is irrelevant.","Answer");
	//ATOM *absurd = q->answers->list[q->answers->count-1];
	initCONTEXT(absurd);
	getReasons(absurd->context,botReasonsInstruction,"What could make you working hypothesis irrelevant?\nREASON ");
	// Experiment and result ----------------------------------------------------------------------
	ATOM *experiment = getExperiment();
	ATOM *result = getArgument(yes, experiment);
	return q;
}
/************************************************************************************************/
/*                                                                                              */
/************************************************************************************************/
ATOM * closeStudy(ATOM *q){ 
	char text[5000],othertext[5000],author[5000],prompt[5000],publication[5000],situation[] = "ZZ";

	ATOM *yes = q->answers->list[0];
	ATOM *no = q->answers->list[1];
	ATOM *absurd = q->answers->list[2];
	ATOM *argument = yes->context->statements->list[yes->context->statements->count-1];
	ATOM *reference = argument->context->statements->list[0];
	ATOM *actuality =  argument->context->statements->list[argument->context->statements->count-1];
	ATOM *experiment = actuality->context->statements->list[0];
	//printf("Just checking:\nyes->text = %s\nargument->text = %s\nactuality->text = %s\nexperiment->text = %s\n",yes->text, argument->text,actuality->text,experiment->text);

	if(scanYesNo("Have you proven your working hypothesis? (YES --> Ivy's scenario 1)",  "")){
		/* Success */
		strcpy(resultInstruction, "What is the conclusion of your study supporting your working hypothesis?\nCONCLUSION ");
		if(promptAndScanInstruction(resultInstruction, text,"")){
			snprintf(prompt,5000,"Previously this is the conclusion you expected:\nPREVIOUSLY EXPECTED CONCLUSION: %s\n\t    Do you want to replace it by '%s'?\t",argument->text,text);
			if(scanYesNo(prompt,  "")){
				argument->text = (char *) realloc(argument->text, sizeof(text)+1);
				strcpy(argument->text,text);			
			}	
		}
		if(promptAndScanInstruction(authorInstruction,author,"")){
			snprintf(text,5000,"This argument (%s) has been experimentally proven by %s.", argument->text,author);
		} else {
			snprintf(text,5000,"This argument (%s) has been experimentally proven.", argument->text);
		}
		actuality->text = (char *) realloc(actuality->text, sizeof(text)+1);
		strcpy(actuality->text,text);			
		if(promptAndScanInstruction(publicationInstruction, publication, "")){
			snprintf(text,5000,"Published in %s.",publication);
			reference->text = (char *) realloc(reference->text, sizeof(text)+1);
			strcpy(reference->text,text);	
			strcpy(reference->tag,"BIB");	
		}	
	} else {
		/* Failure */
		printf("Please select the situation that best applies: / Why are you not finding a proof?");
		printf("\n\t    - You still tend to believe the hypothesis might be true.\t\t\t\t--> Type 'T'"); printf("\tSomething might have gone wrong or we tried proving the hypothesis the wrong way.");printf("\t(Ivy's scenario 3)");
		printf("\n\t    - You tend to think it is false.\t\t\t\t\t\t--> Type 'F'"); printf("\tThe hypothesis is false.");printf("\t(Ivy's scenario 2)");
		//printf("\n\t    - You don't know. As far as you can tell, it could be true or false.\t--> Type 'D'"); printf("\tEither the hypothesis is wrong or something when wrong in our study.");printf("\t(Ivy's scenario cross 2 & 3)");
		printf("\n\t    - You think it is not well formulated, absurd or irrelevant.\t\t--> Type 'A'"); printf("\tSomething doesn't make sense.");printf("\t(Ivy's scenario 4)");
		promptAndScanInstruction("\nSITUATION ", situation,"a");
		while(strcmp(situation,"T")!=0  && strcmp(situation,"F")!=0 && strcmp(situation,"A")!=0 ){//&& strcmp(situation,"D")!=0
			promptAndScan("",situation);
		}
		if(strcmp(situation,"T")==0 ){
			/* Still true */
			printf("\t(%d) If you don't believe it's because the hypothesis is wrong, then why do you think you are not finding a proof?\n",promptno);
			printf("\t    Does one of the following statements need updating?\n");
			promptno++;
			int i;
			ATOM *confounder;
			for(i=0;i<experiment->context->statements->count;i++){
				confounder = experiment->context->statements->list[i];//printf("i=%d, confounder->tag=%s, confounder->text=%s, text=%s\n",i,confounder->tag,confounder->text,text);
				if(strcmp(confounder->tag,"T1-CNFDR")==0||strcmp(confounder->tag,"T2-CNFDR")==0||strcmp(confounder->tag,"T3-CNFDR")==0){
					printf("\t    - %s\n",confounder->text);
				}
			}
			printf("\t");repeatfprintf(stdout,"_",120);printf("\n");
			framefprintf(stdout,"",120);
			framefprintf(stdout,"(Here the statements would be rewritten not in conditional form.)",120);
			framefprintf(stdout,"The user would also be given a chance to update this list of confounders.",120);
			printf("\t|");repeatfprintf(stdout,"_",118);printf("|\n\n");

			strcpy(othertext,argument->text);
			snprintf(text, 5000,"Unsuccessful efforts were made to prove that '%s' and either %s",othertext,stripConfounder(experiment->context->statements->list[0]));
			for(i=1;i<experiment->context->statements->count;i++){
				confounder = experiment->context->statements->list[i];
				if(strcmp(confounder->tag,"T1-CNFDR")==0||strcmp(confounder->tag,"T2-CNFDR")==0||strcmp(confounder->tag,"T3-CNFDR")==0){
					strcat(text," or ");
					strcat(text,stripConfounder(confounder)); //printf("i=%d, confounder->tag=%s, confounder->text=%s, text=%s\n",i,confounder->tag,confounder->text,text);
				}	
			}
			strcat(text,". /!\\NB: need to change conditional: Replace 'It could happen' with 'It has happened'");
			actuality->text = (char *) realloc(actuality->text, sizeof(text)+1);
			strcpy(actuality->text,text);			
			
			if(promptAndScanInstruction(authorInstruction,author,"")){
				snprintf(text,5000,"A study was performed by %s with the aim of proving that '%s'. No signficant results were found.",author,othertext);
			} else {
				snprintf(text,5000,"A study was performed with the aim of proving that '%s'. No signficant results were found.",othertext);
			}
			//snprintf(text, 5000,"We have trouble proving that ' %s '.",othertext);
			ATOM *negativeargument = recordNewATOM(no->context->statements,text,"MAIN");
			initCONTEXT(negativeargument);
			recordNewATOM(negativeargument->context->statements,"Here would be a link towards the experiment described above","EXPMT");
		} else if(strcmp(situation,"F")==0 ){
			/* Now false */
			strcpy(resultInstruction, "What is the conclusion of your study discrediting the original working hypothesis?\nCONCLUSION ");
			if(promptAndScanManditoryInstruction(resultInstruction, text,"")){
				strcpy(othertext,argument->text);
				printf("Previously this is the conclusion you expected:\nPREVIOUSLY EXPECTED CONCLUSION: %s\n\t    I'm replacing it by '%s'.\n",othertext,text);
				argument->text = (char *) realloc(argument->text, sizeof(text)+1);
				strcpy(argument->text,text);
				getLexic(argument,"what you wrote","a");	
		
				if(promptAndScanInstruction(authorInstruction,author,"")){
					snprintf(text,5000,"A study was performed by %s with the aim of proving that '%s'. No signficant results were found.",author,othertext);
				} else {
					snprintf(text,5000,"A study was performed with the aim of proving that '%s'. No signficant results were found.",othertext);
				}
				actuality->text = (char *) realloc(actuality->text, sizeof(text)+1);
				strcpy(actuality->text,text);			
				strcpy(   publicationInstruction, "Have you published your results? If so, enter reference of publication.\nPUBLICATION ");
				if(promptAndScanInstruction(publicationInstruction, publication, "")){
					snprintf(text,5000,"Published in %s.",publication);
					reference->text = (char *) realloc(reference->text, sizeof(text)+1);
					strcpy(reference->text,text);	
					strcpy(reference->tag,"BIB");	
				}	
			}
			no->context->statements->list[no->context->statements->count] = argument;
			no->context->statements->count++;	
			yes->context->statements->count --;	
		} else {
			/* Now Absurd */
			strcpy(resultInstruction, "What is the conclusion of your study?\nCONCLUSION ");
			strcpy(othertext,argument->text);
			promptAndScanManditoryInstruction(resultInstruction, text,"");
			argument->text = (char *) realloc(argument->text, sizeof(text)+1);
			strcpy(argument->text,text);
			getLexic(argument,"what you wrote","a");	
			strncpy(text, q->text+18, strlen(q->text)-22);
			text[strlen(q->text)-22]='\0';
			snprintf(defaultLexicprompt, 5000,"Please can you list the terms that you now understand differently in the working hypothesis as originally worded: ' %s '",text);
			getLexic(absurd,"","");	
			snprintf(defaultLexicprompt, 5000,"Are there also terms you understand differently now in ' %s ' (the conclusion you were originally targetting to support the hypothesis)",othertext);
			getLexic(argument,"","");	
			if(promptAndScanInstruction(authorInstruction,author,"")){
				snprintf(text,5000,"A study was performed by %s with the aim of proving that '%s'. No signficant results were found. The relevance and/or meaning of the study was reevaluated.",author,othertext);
			} else {
				snprintf(text,5000,"A study was performed with the aim of proving that '%s'. No signficant results were found. The relevance and/or meaning of the study was reevaluated.",othertext);
			}
			actuality->text = (char *) realloc(actuality->text, sizeof(text)+1);
			strcpy(actuality->text,text);			
			strcpy(publicationInstruction, "Have you published your results? If so, enter reference of publication.\nPUBLICATION ");
			if(promptAndScanInstruction(publicationInstruction, publication, "")){
				snprintf(text,5000,"Published in %s.",publication);
				reference->text = (char *) realloc(reference->text, sizeof(text)+1);
				strcpy(reference->text,text);	
				strcpy(reference->tag,"BIB");	
			}	
			absurd->context->statements->list[no->context->statements->count] = argument;
			absurd->context->statements->count++;	
			yes->context->statements->count --;	
		}
	/*else if(strcmp(situation,"D")==0){	COULD BE BOTH 		} */
	}
	return q;
}
/************************************************************************************************/
/*                                                                                              */
/************************************************************************************************/
void setInstructionsP(){
	strcpy(    hypothesisInstruction, "What is your working hypothesis?\nHYPOTHESIS ");
	strcpy(    yesReasonsInstruction, "What reasons presently supports your working hypothesis?\n\t    Please list one reason at a time.\n\t    Reasons can be motivations, statements drawn from the state of the art of your field and/or from logical inferences.\nREASON FOR ");
	strcpy(     noReasonsInstruction, "To your present knowledge are there reasons to expect your working hypothesis might not be true?\n\t    Please list one reason at a time.\n\t    Reasons can be negative results, facts or statements drawn from the state of the art of your field and/or from logical inferences, that disagree/oppose/negate your working hypothesis.\nREASON AGAINST ");
	strcpy(    botReasonsInstruction, "Are there reasons to expect your working hypothesis might be irrelevant or absurd?\n\t    Please list one reason at a time.\n\t    Reasons can be ambiguity in the wording of the hypothesis, uncertainty in the meaning of certain terms it involves, etc.\nREASON ");
	strcpy(   confoundersInstruction, "What might confound or undermine your study?\n\t    What could go wrong in your experiment?\n\t    Please list all possibilities you can think of.");
	strcpy(addconfoundersInstruction, "Have you addressed this confounder? Have you checked it couldn't go wrong?");
	strcpy(          dataInstruction, "What are your results so far? Please register/upload your (meta)data and your interpretation of it.\t  !TODO! \nDATA "); // & METADATA!
	strcpy(        authorInstruction, "Please name the individual(s), team(s) and/or institution(s) actively involved in performing this study\nAUTHOR ");
	strcpy(    experimentInstruction, "Please describe your experiment/study (eg: experiment design and main methods).\nEXPERIMENT ");// brief yet sufficient description of the study
	strcpy(        resultInstruction, "If this experiment is successful what will it show that supports your working hypothesis?\nINTERPRETATION/CONCLUSION ");
	strcpy(   publicationInstruction, "Have you published any results so far? If so, enter reference of publication.");
	strcpy(   statustext, "has not yet been proven. It is currently being studied");
	strcpy(   defaultLexicprompt,"Please can you list non-obvious terms that you used in ");
	strcpy(   status,"ZZ");
	strcpy(   defaultPublicationPrompt,"\tPlease bibliographic reference (or type enter to skip):\nBIB ");
	lexicNBtodo=true;
	reasonNBtodo=true;

}
void setInstructionsS(){
	strcpy(    hypothesisInstruction, "What is your working hypothesis?\nHYPOTHESIS ");
	strcpy(    yesReasonsInstruction, "What reasons presently supports your working hypothesis?\n\t    Please list one reason at a time.\n\t    Reasons can be motivations, statements drawn from the state of the art of your field and/or from logical inferences.\nREASON FOR ");
	strcpy(     noReasonsInstruction, "To your present knowledge are there reasons to expect your working hypothesis might not be true?\n\t    Please list one reason at a time.\n\t    Reasons can be negative results, facts or statements drawn from the state of the art of your field and/or from logical inferences, that disagree/oppose/negate your working hypothesis.\nREASON AGAINST ");
	strcpy(    botReasonsInstruction, "Are there reasons to expect your working hypothesis might be irrelevant or absurd?\n\t    Please list one reason at a time.\n\t    Reasons can be ambiguity in the wording of the hypothesis, uncertainty in the meaning of certain terms it involves, etc.\nREASON ");
	strcpy(   confoundersInstruction, "What might confound or undermine your study?\n\t    What could go wrong in your experiment?\n\t    Please list all possibilities you can think of.");
	strcpy(addconfoundersInstruction, "Can you address this confounder? Can you check it can't go wrong?");
	strcpy(          dataInstruction, "What are your results (so far)? Please register/upload your (meta)data and your interpretation of it.\t !TODO! \nDATA "); // & METADATA!
	strcpy(        authorInstruction, "Please name the individual(s), team(s) and/or institution(s) actively involved in performing this study\nAUTHOR ");
	strcpy(    experimentInstruction, "Please describe the experiment/study you plan to carry out (eg: experiment design and main methods).\nEXPERIMENT ");// brief yet sufficient description of the study
	strcpy(        resultInstruction, "If this experiment is successful what will it show that supports your working hypothesis?\nINTERPRETATION/CONCLUSION ");
	strcpy(   publicationInstruction, "Have you published any results so far? If so, enter reference of publication.");
	strcpy(   statustext, "has not yet been proven. It is currently being studied");
	strcpy(   defaultLexicprompt,"Please can you list non-obvious terms that you used in ");
	strcpy(   status,"ZZ");
	strcpy(   defaultPublicationPrompt,"\tPlease bibliographic reference (or type enter to skip):\nBIB ");
	lexicNBtodo=true;
	reasonNBtodo=true;

}
void setInstructionsF(){
	strcpy(    hypothesisInstruction, "What was the working hypothesis of your study?\nHYPOTHESIS ");
	strcpy(    yesReasonsInstruction, "Outside of your study, what reasons support this hypothesis?\n\t    Please list one reason at a time.\n\t    Reasons can be motivations, statements drawn from the state of the art of your field and/or from logical inferences.\nREASON FOR ");
	strcpy(     noReasonsInstruction, "What reasons outside of your study go against the hypothesis?\n\t    Please list one reason at a time.\n\t    Reasons can be negative results, facts or statements drawn from the state of the art of your field and/or from logical inferences, that disagree/oppose/negate your working hypothesis.\nREASON AGAINST ");
	strcpy(    botReasonsInstruction, "Are there reasons the hypothesis might actually be irrelevant or absurd worded as it is?\n\t    Please list one reason at a time.\n\t    Reasons can be ambiguity in the wording of the hypothesis, uncertainty in the meaning of certain terms it involves, etc.\nREASON ");
	strcpy(   confoundersInstruction, "What might have confounded or undermined your study?\n\t    What might have gone wrong in your experiment?\n\t    Please list all possibilities you can think of.");
	strcpy(addconfoundersInstruction, "Have you addressed this confounder? Have you checked it couldn't go wrong?");
	strcpy(          dataInstruction, "What are your results? Please register/upload your (meta)data and your interpretation of it.\t !TODO! \nDATA "); // & METADATA!
	strcpy(        authorInstruction, "Please name the individual(s), team(s) and/or institution(s) who actively participated in performing this study\nAUTHOR ");
	strcpy(    experimentInstruction, "Please describe the experiment/study you did (eg: experiment design and main methods).\nEXPERIMENT ");// brief yet sufficient description of the study
	strcpy(        resultInstruction, "What was the conclusion you expected to derive in support of the working hypothesis?\nINTERPRETATION/CONCLUSION ");
	strcpy(   publicationInstruction, "Have you published your results? If so, enter reference of publication.\nPUBLICATION ");
	strcpy(   statustext, "has been studied but not proven. Unsuccessful efforts were made to prove it experimentally");
	strcpy(   defaultLexicprompt,"Please can you list non-obvious terms that you used in ");
	strcpy(   status,"ZZ");
	strcpy(   defaultPublicationPrompt,"\tPlease bibliographic reference (or type enter to skip):\nBIB ");
	lexicNBtodo=true;
	reasonNBtodo=true;

}
/************************************************************************************************/
/*                                                                                              */
/*                                                                                              */
/*                           NEUROSCIENCE                                                       */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
ATOM *neuroscience_mmm(){
	ATOM *q; 
	char filename[5000],name[5000],text[5000],othername[5000], assumption[5000],whattodo[]="ZZ";

	promptno = 0;
	printf("\t(%da) Do you want to register a study that is...\n", promptno);
	printf("\t    ... not started --> type 'S' (for now same as option 'P'),\n");
	printf("\t    ...  in process --> type 'P',\n");
	printf("\t    ...    finished --> type 'F'\n");
	promptAndScan("STATUS ", status);
	while(strcmp(status,"S")!=0 && strcmp(status,"P")!=0 && strcmp(status,"F")!=0){
		promptAndScan("", status);
	}
	printf("\n\n");
	if(strcmp(status,"P")==0 || strcmp(status,"S")==0 ){
/************************************************************************************************/
/*                           STATUS NOT FINISHED                                                */
/************************************************************************************************/
		setInstructionsP();
		promptno++;
		q = registerStudy();
		printf("----------------------------------------------------------------------------------------------");
		printf("\n\nThis is what the study looks like in MMM format:\n");
		fprintForestQUESTION(stdout,q);
		printf("\n----------------------------------------------------------------------------------------------\n\n");
		strcpy(filename,questionToPointlessStatement(q->text));
		strcat(filename,"Original");
		createFiles(filename,q);
		return q;
	} 	else if (strcmp(status,"F")==0){
/************************************************************************************************/
/*                           STATUS     FINISHED                                                */
/************************************************************************************************/
		setInstructionsF();
		printf("\t(%db) If you want to...",promptno); // "Have you already registered the study into the MMM?"
		printf("\n\t     Update and close an pre-registered study...\t--> Type 'P'"); 
		printf("\n\t     Register and close an un-registered study...\t--> Type 'R'"); 
		promptAndScan("\nWHATTODO ", whattodo);
		while(strcmp(whattodo,"P")!=0  && strcmp(whattodo,"R")!=0 ){//&& strcmp(whattodo,"D")!=0 
			promptAndScan("",whattodo);
		}
		if(strcmp(whattodo,"P")==0 ){
			promptno++;
			snprintf(text,5000,"What is the name of the markup file where the MMM is described? (if you skip, defaulting to %s)\nFILENAME ", dummyMU);
			if(promptAndScanInstruction(text,filename,"c")){
				q = readQUESTION(filename);
			} else {
				q = readQUESTION(dummyMU);
			}			
			printf("----------------------------------------------------------------------------------------------");
			printf("\n\nThis is what the study looked like in MMM format:\n");
			fprintForestQUESTION(stdout,q);
			printf("\n----------------------------------------------------------------------------------------------\n\n");
			strcpy(filename,questionToPointlessStatement(q->text));
			strcat(filename,"Original");
			snprintf(text,5000,"Do you want to create folder %s with output files for old original version? ",othername);
			if(scanYesNo(text, "d")){
				createFiles(filename,q);
			}
			q = closeStudy(q);
			strcpy(filename,questionToPointlessStatement(q->text));
			strcat(filename,"Closed");
			createFiles(filename,q);
			return q;
		} else if(strcmp(whattodo,"R")==0 ){
			printf("\t");repeatfprintf(stdout,"_",120);printf("\n");
			framefprintf(stdout,"",120);
			framefprintf(stdout,"/!\\ Presently the program does not deal neatly with this case",120);
			printf("\t|");repeatfprintf(stdout,"_",118);printf("|\n\n");
			getchar();
			promptno =1;
			q = registerStudy(); 
			fprintForestQUESTION(stdout,q);
			return q;
		}	
	} 
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
	char indent[5000],childindent[2000];

	printf("\t");repeatfprintf(stdout,"_",120);printf("\n");
	framefprintf(stdout,"",120);
	framefprintf(stdout,"Eventually, we wouldn't wait for users to propose two registered candidates for merging. The merging would be done",120);
	framefprintf(stdout,"on the fly when new units are registered. The new units would be compared with the ones already in the MMM database.",120);
	framefprintf(stdout,"",120);
	framefprintf(stdout,"NB: 'Merging' might be misleading. The two original units are not erased. A third one, more general is created.",120);
	framefprintf(stdout,"The later becomes a new way to access each of the former.",120);
	printf("\t|");repeatfprintf(stdout,"_",118);printf("|\n\n");
	getchar();

	char text[5000], othertext[5000];
	snprintf(othertext,5000,"%s/",questionToStatement(p->text));
	strcat(othertext,questionToStatement(q->text));
	snprintf(text,5000,"%s",statementToQuestion(othertext)); //Encompassing/common question : 
	ATOM* qp = initATOM(text,"Question","Question",0);
	initCONTEXT(qp);
	qp->answers=initATOMS(qp->depth+1,"Answers");

	printf("SPECIFIC QUESTION 1: %s\n",p->text);	
	printf("SPECIFIC QUESTION 2: %s\n",q->text);	
	printf("GENERAL QUESTION  : %s\n",qp->text);	

	printf("\t");repeatfprintf(stdout,"_",120);printf("\n");
	framefprintf(stdout,"",120);
	framefprintf(stdout,"The formulation of the general question would be done more carefully. Actually this is where most of the work is",120);
	framefprintf(stdout,"done by the program: the program searches for similarities and differences in the wordings of the two questions. ",120);
	framefprintf(stdout,"It might also compare the content of answers and of contexts to find more similarities and differences it can use",120);
	framefprintf(stdout,"to design a new question.",120);
	framefprintf(stdout,"",120);
	framefprintf(stdout,"/!\\",120);
	framefprintf(stdout,"The role of the program is only to make a suggestion. Generally, the program is a very efficient assistant.",120);
	framefprintf(stdout,"All decisions are left to living, thinking humans in order to avoid building hidden biaises into the system.",120);
	framefprintf(stdout,"--> The program isn't indispensable. Everything it does can be done 'by hand'. The converse doesn't hold.",120);
	framefprintf(stdout," ",120);
	printf("\t|");repeatfprintf(stdout,"_",118);printf("|\n\n");
	getchar();

	int i,j,k;
	ATOM *a,*aqp;
	/* COPY LEXICS AND ANSWERS */
	for(i=0;i<p->answers->count;i++){
		a = p->answers->list[i];
		snprintf(text,5000,"%s (acc. 1st source)",a->text);
		aqp=recordNewATOM(qp->answers,text,"Answer");
		initCONTEXT(aqp);//printf("I'm here i=%d\n",i);
		copyLEXIC(aqp->context->lexic,a->context->lexic,1);
	}
	for(i=0;i<q->answers->count;i++){
		a = q->answers->list[i];
		snprintf(text,5000,"%s (acc. 2nd source)",a->text);
		aqp=recordNewATOM(qp->answers,text,"Answer");
		initCONTEXT(aqp);
		copyLEXIC(aqp->context->lexic,a->context->lexic,1);
	}

	/* COMMON QUESTION LEXIC*/
	bool recordedP[p->context->lexic->count],recordedQ[q->context->lexic->count];
	for (i=0;i<p->context->lexic->count;i++){recordedP[i] =false;}
	for (i=0;i<q->context->lexic->count;i++){recordedQ[i] =false;}
	ENTRY *ep, *eq, *e;
	for(i=0;i<p->context->lexic->count;i++){
		ep = p->context->lexic->list[i];
		for(j=0;j<q->context->lexic->count;j++){
			eq = q->context->lexic->list[j];
			if(strcmp(ep->term,eq->term)==0){
				if(strcmp(ep->definition,eq->definition)==0 ){
					recordNewENTRY(qp->context->lexic,ep->term,ep->definition);
					recordedP[i] =true;
					recordedQ[i] =true;
				} else {
					snprintf(text,5000,"?? ----->>----- %s \\/ %s",ep->definition,eq->definition);
					recordNewENTRY(qp->context->lexic,ep->term,text);
					recordedP[i] =true;
					recordedQ[i] =true;
				}
			} 
		}
	}

	/* PUSH IGNORED ENTRIES INTO ANSWER CONTEXTS */
	for (i=0;i<p->context->lexic->count;i++){
		e = p->context->lexic->list[i];
		//printf("---------- %s:=%s (recordedP[%d] =%d)\n",e->term,e->definition,i,(int) recordedP[i]);
		if(recordedP[i]){
			//printf("yes!\n");
		}
		else if(!recordedP[i]){
			//printf("no!\n");
			for(j=0;j<p->answers->count;j++){
				//printf("Answer %d------------------------\n",j);
				//printf("recordedP[%d] =%d (%s:=%s)\n",i,(int) recordedP[i],e->term,e->definition);
				recordNewENTRY(qp->answers->list[j]->context->lexic,e->term,e->definition);	
			}
		}
	}
	for (i=0;i<q->context->lexic->count;i++){
		e = q->context->lexic->list[i];
		//printf("---------- %s:=%s (recordedP[%d] =%d)\n",e->term,e->definition,i,(int) recordedQ[i]);
		if(recordedQ[i]){
			//printf("yes!\n");
		}
		else if(!recordedQ[i]){
			//printf("no!\n");
			for(j=0;j<q->answers->count;j++){
				//printf("Answer %d------------------------\n",j);
				//printf("recordedP[%d] =%d (%s:=%s)\n",i,(int) recordedP[i],e->term,e->definition);
				recordNewENTRY(qp->answers->list[p->answers->count+j]->context->lexic,e->term,e->definition);	
			}
		}
	}


	strcpy(childindent,indentchild(true,depth2indent(1)));
	printf("\n\nLexic of 1st question:\n");
	fprintForestLEXIC(stdout, p->context->lexic,childindent);
	//printf("\n----------------------------------------------------------------------------------------------");
	printf("\n\nLexic of 2nd question:\n");
	fprintForestLEXIC(stdout, q->context->lexic,childindent);
	//printf("\n----------------------------------------------------------------------------------------------");
	printf("\n\nLexic of resulting general question:\n");
	fprintForestLEXIC(stdout, qp->context->lexic,childindent);
	//printf("\n----------------------------------------------------------------------------------------------");

	printf("\n\n\t");repeatfprintf(stdout,"_",120);printf("\n");
	framefprintf(stdout,"",120);
	framefprintf(stdout,"In this demo, the program brutally rejects entries that aren't identical (term & definition) in the 2 source lexics.",120);
	framefprintf(stdout,"Eventually, it would identify similarities between entries and ask the source authors what they think should be done:",120);
	framefprintf(stdout,"merge and generalise, or distinguish and push down into their respective answers to the general question.",120);
	framefprintf(stdout,"",120);
	framefprintf(stdout,"When terms are identical but definitions are not and authors disagree, this can be taken as a sign their questions",120);
	framefprintf(stdout,"shouldn't be merged, or wordings should be tweaked to account for the discrepancy. If the discrepancy is not critical,",120);
	framefprintf(stdout,"the simple solution can be adopted, namely, put no definition but add a link to each of the original.",120);
	printf("\t|");repeatfprintf(stdout,"_",118);printf("|\n\n");

	getchar();
	//printf("----------------------------------------------------------------------------------------------");

	
	strcpy(indent,indentchild(false," "));	
	strcpy(childindent,indentchild(false,indent));
	printf("\nFirst source's original answers:\n");
	fprintForestANSWERSperDepth(stdout,p->answers,false,indent,1);
	printf("\n\n");
	printf("\nSecond source's original answers:\n");
	fprintForestANSWERSperDepth(stdout,q->answers,false,indent,1);
	printf("\n\n");
	printf("\nCombination of their answers:\n");
	fprintForestANSWERSperDepth(stdout,qp->answers,false,indent,3);
	printf("\n\n");
	fprintForestQUESTIONperDepth(stdout,qp,5);


	printf("\n\n");
	printf("\t");repeatfprintf(stdout,"_",120);printf("\n");
	framefprintf(stdout,"",120);
	framefprintf(stdout,"Todo! NB: no recursivity needed.",120);
	printf("\t|");repeatfprintf(stdout,"_",118);printf("|\n\n");
	getchar();

	printf("\n\n");

	return qp;
}
	/*
char *removeExtension(char* mystr) {
    char *retstr;
    char *lastdot;
    if (mystr == NULL)
         return NULL;
    if ((retstr = malloc (strlen (mystr) + 1)) == NULL)
        return NULL;
    strcpy (retstr, mystr);
    lastdot = strrchr (retstr, '.');
    if (lastdot != NULL)
        *lastdot = '\0';
    return retstr;
}

*/
	//// name
	//// frame nb here the user would actually select a MMM unit registered before.
	//// or not: start form scratch
	// create new question. free old one.rename old file.
			// snprintf(othername,200,"%sOld.txt",removeExtension(name)); int rename(name, othername); printf("Renamed file %s to %s\n",othername,name);
			// remove folder names at begin of other name
	
