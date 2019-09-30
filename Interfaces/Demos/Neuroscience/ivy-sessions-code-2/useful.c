/************************************************************************************************/
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                                         U S E F U L . C                                      */
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

// gcc -o prune useful.c

/************************************************************************************************/
/*                                                                                      */
/************************************************************************************************/
int prune(char *filename){
	FILE *fp = fopen(filename,"r"); 
   if( fp == NULL){
		fprintf(stderr, "Failed to open file '%s'!\n",filename);		
		return 0;
	}
	static char copy[5000];	
	int i=0,countB=0,countP=0;
	bool incomments=false;
	char ch= fgetc(fp),lastch=' ';

	while(ch != EOF){
		if(ch=='\n'){
		} else if(incomments){
			if(lastch=='*' && ch=='/'){
				incomments=false;
			} 
		} else {
			if(ch=='/') {
			} else if(lastch=='/' && ch=='*') {
				incomments = true;
			} else if(ch == '{'){
				countB++;
			} else if(ch == '}'){
				countB--;
			} else {
				if(countB==0){
					copy[i]=ch;
					i++; 
					if(ch == '('){
						countP++;
					}
					if(ch == ')'){
						countP--;
						copy[i]=';';
						i++; 
						copy[i]='\n';
						i++; 
					}
				}
			}
		}
		lastch = ch;
		ch = fgetc(fp);
	}
	fclose(fp);
	fp = fopen("pruned.c","w+"); 
	if( fp ){
		fprintf(fp,"%s",copy);
	} else {
		fprintf(stderr, "Failed to open file '%s'!\n","pruned.c");		
		return 0;
	}
	fclose(fp);
	printf("%s\n",copy);
	return 0;
}
/************************************************************************************************/
/*                                                                                      */
/************************************************************************************************/
int main(int argc, char *argv[]){
	if(argc<2){
		printf("No file given!\n");
	}
	prune(argv[1]);
	return 0;
}