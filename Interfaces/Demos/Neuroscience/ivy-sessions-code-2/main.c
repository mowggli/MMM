/************************************************************************************************/
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                                             M A I N . C                                      */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
#include "tete.h"
// gcc -o tiptop main.c structures.c structuresAddendum.c writeMarkup.c writeForest.c writeLatex.c writeTikz.c writeAddendum.c interface0.c interfaceNeuro.c notes.c instructions.c choice.c depths.c read.c files.c text.c -lm
/************************************************************************************************/
/* MAIN                                                                                         */
/************************************************************************************************/
int main(int argc, char *argv[]){
	char choice[5000];
	strcpy(choice,getFirstArgument(argc,argv));
	if (strcmp(choice,"B")==0){
		/**/
		return basicsourceinterface();
	} else if(strcmp(choice,"S")==0){
		/**/
		return buildFromFile(argc,argv);
	} else if(strcmp(choice,"P")==0){
		/**/
		return playNeuroscientist(argc,argv);
	} else if(strcmp(choice,"M")==0){
		/**/
		return tryMerge(argc,argv);		
	}
	return 1;
}