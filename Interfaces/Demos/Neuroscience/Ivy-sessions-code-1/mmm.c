/*gcc -o tiptop mmm.c structures.c forest.c latex.c tikz.c markup.c feed.c interface.c addendum.c -lm
*/

#include "tete.h"

#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

int shortlength=SHORTLENGTH;
int promptno=0;
char *newtext;

/************************************************************************************************/
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
int choose(char *arguments[]){ 
	char text[5000],filename[5000], choice[5000],assumption[5000] ; //choice[]="ZZ", 
	ATOM * q, *p, *qp;
	int f; 
	if( (strcmp(arguments[0],"S")!=0) && (strcmp(arguments[0],"P")!=0)  && (strcmp(arguments[0],"M")!=0)  ){ 
		printf("\t(0) Please choose and type:\n");
		printf("\t    'S' if you want to create some files based on a dummy predefined question unit (described in %s).\n",arguments[1]);
		printf("\t    'P' if you want to play the part of a neuroscientist registering a new question unit.\n");
		printf("\t    'M' if you want to two pre-registered studies.\n");
		promptAndScan("CHOICE ",choice);
		while( (strcmp(choice,"S")!=0) && (strcmp(choice,"P")!=0)  && (strcmp(choice,"M")!=0)  ){
			promptAndScan("",choice);
		}
	} else {
		strcpy(choice,arguments[0]);
	}
	if(strcmp(choice,"S")==0){
/************************************************************************************************/
/*                       READ AND CREATE FILES                                                  */
/************************************************************************************************/
		snprintf(text,5000,"\t(0a) What is the name of the markup file where the study is described? (if you skip, defaults to %s)\nFILENAME ",arguments[1]);
		if(promptAndScan(text,filename)){
			q = readQUESTION(filename);
		} else {
			q = readQUESTION(arguments[1]);
		}			
		framefprintf(stdout,"",120);
		printf("\n\nThis is what the study looks like in MMM format:\n");
		fprintForestQUESTION(stdout,q);
		framefprintf(stdout,"",120);
		printf("\n\n\n");
		strcpy(filename,questionToPointlessStatement(q->text));
		snprintf(text,5000,"Do you want to create folder '%s' with output Latex and tree-like visualisation files? ",filename);
		if(scanYesNo(text, "a")){
			createFiles(filename,q);
		}
		f = freeATOM(q);
		if( f!= 0){
			fprintf(stderr, "Failed to free all memory allocations! (alloCount = %d)\n",f);		
		} else{
			printf("All memory allocations freed (alloCount = %d).\n",f);	
		}
		return f;
	} else if(strcmp(choice,"P")==0){
/************************************************************************************************/
/*                             SOURCE INTERFACE NEUROSCIENCE                                    */
/************************************************************************************************/
		q = neuroscience_mmm();
		f = freeATOM(q);
		if( f!= 0){
			fprintf(stderr, "Failed to free all memory allocations! (alloCount = %d)\n",f);		
		} else{
			printf("All memory allocations freed (alloCount = %d).\n",f);	
		}
		return f;
	}
	if(strcmp(choice,"M")==0){
/************************************************************************************************/
/*                             READ AND MERGE                                                   */
/************************************************************************************************/
		snprintf(text,5000,"\t(0a) What is the name of the markup file where the 1st study is described? (if you skip, defaults to %s)\nFILENAME ",arguments[1]);
		if(promptAndScan(text,filename)){
			q = readQUESTION(filename);
		} else {
			q = readQUESTION(arguments[1]);
		}			
		framefprintf(stdout,"",120);
		printf("\n\nThis is what the study looks like in MMM format:\n");
		fprintForestQUESTION(stdout,q);
		framefprintf(stdout,"",120);
		printf("\n\n");
		snprintf(text,5000,"\t(0b) What is the name of the markup file where the 2nd study is described? (if you skip, defaults to %s)\nFILENAME ",arguments[2]);
		if(promptAndScan(text,filename)){
			p = readQUESTION(filename);
		} else {
			p = readQUESTION(arguments[2]);
		}			
		framefprintf(stdout,"",120);
		printf("\n\nThis is what the 2nd study looks like in MMM format:\n");
		fprintForestQUESTION(stdout,p);
		framefprintf(stdout,"",120);
		printf("\n\n");qp = mergeQuestions(q,p);
		//printf("\n\nThis is what the overarching study looks like in MMM format:\n");
		//fprintForestQUESTION(stdout,qp);
		strcpy(filename,questionToPointlessStatement(qp->text));
		strcat(filename,"Common");
		snprintf(text,5000,"Do you want to create folder '%s' with output Latex and tree-like visualisation files? ",filename);
		if(scanYesNo(text, "c")){
			createFiles(filename,qp);
		}
		f = freeATOM(qp); 
		f = freeATOM(p);
		f = freeATOM(q);
		if( f!= 0){
			fprintf(stderr, "Failed to free everything! (alloCount = %d)\n",f);
			return f;		
		} 
		printf("All memory allocations freed (alloCount = %d).\n",f);	
		return f;		
	}
	return -1;
}
/************************************************************************************************/
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
int main(int argc, char *argv[]){
	//printNBpublication();return 0;

	newtext = (char *) malloc(shortlength +23);
	char *arguments[3];
	int i;
	if(argc == 1){
		arguments[0] = (char *) malloc (2);
		strcpy(arguments[0],"Z");
		arguments[1] = (char *) malloc (strlen(seconddummyMU) + 1);
		strcpy(arguments[1],dummyMU);
		arguments[2] = (char *) malloc (strlen(seconddummyMU) + 1);
		strcpy(arguments[2],seconddummyMU);
	} else if (argc ==2) {
		for(i=0;i<1;i++){
			arguments[i] = (char *) malloc (strlen(argv[i+1]) + 1);
			strcpy(arguments[i],argv[i+1]);
		}
		arguments[1] = (char *) malloc (strlen(seconddummyMU) + 1);
		strcpy(arguments[1],dummyMU);
		arguments[2] = (char *) malloc (strlen(seconddummyMU) + 1);
		strcpy(arguments[2],seconddummyMU);
	} else if (argc ==3) {
		for(i=0;i<2;i++){
			arguments[i] = (char *) malloc (strlen(argv[i+1]) + 1);
			strcpy(arguments[i],argv[i+1]);
		}
		arguments[2] = (char *) malloc (strlen(seconddummyMU) + 1);
		strcpy(arguments[2],seconddummyMU);
	} else if (argc >=4) {
		for(i=0;i<3;i++){
			arguments[i] = (char *) malloc (strlen(argv[i+1]) + 1);
			strcpy(arguments[i],argv[i+1]);
		}
	}
	for(i=0;i<3;i++){
		printf("arguments[%d] = %s\n",i,arguments[i]);
	}
	printf("\n----------------------------------------------------------------------------------------------\n\n");
	return choose(arguments);

}