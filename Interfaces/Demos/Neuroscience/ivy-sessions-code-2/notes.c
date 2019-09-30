/************************************************************************************************/
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                                     N O T E S . C                                            */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
#include "tete.h"
bool lexicNBtodo=true;
bool reasonNBtodo=true;
bool publicationNBtodo=true;

/************************************************************************************************/
/* HYPOTHESIS                                                                                   */
/************************************************************************************************/
void hypothesisNote(){
	printf("\t");repeatfprintf(stdout,"_",120);printf("\n");
	framefprintf(stdout,"",120);
	framefprintf(stdout,"Here, the interface would ask the user if her working hypothesis is the same as",120);
	framefprintf(stdout,"similar ones previously recorded in the MMM by other users, assuming any similar ones are found.",120);
	printf("\t|");repeatfprintf(stdout,"_",118);printf("|\n\n");
	getchar();
}
/************************************************************************************************/
/* AUTHOR                                                                                       */
/************************************************************************************************/
void authorNote(){
	printf("\t");repeatfprintf(stdout,"_",120);printf("\n");
	framefprintf(stdout,"",120);
	framefprintf(stdout,"Missing author note",120);
	printf("\t|");repeatfprintf(stdout,"_",118);printf("|\n\n");
	getchar();
}
/************************************************************************************************/
/* LEXIC                                                                                        */
/************************************************************************************************/
void lexicNote(){
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
/************************************************************************************************/
/*  Publication                                                                                 */
/************************************************************************************************/
void publicationNote(){
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
/************************************************************************************************/
/* Reason                                                                                       */
/************************************************************************************************/

void reasonNote(){
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
/* Merge                                                                                        */
/************************************************************************************************/
void mergeNote(){
	printf("\t");repeatfprintf(stdout,"_",120);printf("\n");
	framefprintf(stdout,"",120);
	framefprintf(stdout,"Eventually, we wouldn't wait for users to propose two registered candidates for merging. The merging would be done",120);
	framefprintf(stdout,"on the fly when new units are registered. The new units would be compared with the ones already in the MMM database.",120);
	framefprintf(stdout,"",120);
	framefprintf(stdout,"NB: 'Merging' might be misleading. The two original units are not erased. A third one, more general is created.",120);
	framefprintf(stdout,"The later becomes a new way to access each of the former.",120);
	printf("\t|");repeatfprintf(stdout,"_",118);printf("|\n\n");
	getchar();
}
/************************************************************************************************/
/*  Merged question                                                                             */
/************************************************************************************************/
void mergedQuestionNote(){
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
}
/************************************************************************************************/
/* Merge lexic                                                                                  */
/************************************************************************************************/
void mergeLexicNote(){
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
}
/************************************************************************************************/
/* recursivity                                                                                  */
/************************************************************************************************/
void recursionNote(){
	printf("\n\n");
	printf("\t");repeatfprintf(stdout,"_",120);printf("\n");
	framefprintf(stdout,"",120);
	framefprintf(stdout,"Todo! NB: no recursivity needed.",120);
	printf("\t|");repeatfprintf(stdout,"_",118);printf("|\n\n");
	getchar();

	printf("\n\n");
}
/************************************************************************************************/
/*                                                                                      */
/************************************************************************************************/
void conditionalNote(){
		printf("\t");repeatfprintf(stdout,"_",120);printf("\n");
		framefprintf(stdout,"",120);
		framefprintf(stdout,"(Here the statements would be rewritten not in conditional form.)",120);
		framefprintf(stdout,"The user would also be given a chance to update this list of confounders.",120);
		printf("\t|");repeatfprintf(stdout,"_",118);printf("|\n\n");
}
/************************************************************************************************/
/*                                                                                      */
/************************************************************************************************/
void presentlyNote(){
			printf("\t");repeatfprintf(stdout,"_",120);printf("\n");
			framefprintf(stdout,"",120);
			framefprintf(stdout,"/!\\ Presently the program does not deal neatly with this case",120);
			printf("\t|");repeatfprintf(stdout,"_",118);printf("|\n\n");
			getchar();
}
/************************************************************************************************/
/*                                                                                      */
/************************************************************************************************/
