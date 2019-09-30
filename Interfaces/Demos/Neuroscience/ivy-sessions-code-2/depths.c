/************************************************************************************************/
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                                        D E P T H S . C                                       */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/*                                                                                              */
/************************************************************************************************/
#include "tete.h"
/************************************************************************************************/
/* META                                                                                         */
/************************************************************************************************/
int ANYTHINGtoMETAdepth(int d){
	return d+1;
}
int METAtoINSIDEdepth(int d){
	return d+1;
}
int ANYTHINGtoINSIDEMETAdepth(int d){
	return ANYTHINGtoMETAdepth(METAtoINSIDEdepth(d));
}
/************************************************************************************************/
/* QUESTION                                                                                     */
/************************************************************************************************/
int QUESTIONdepth(int d){
	return 0;
}
/************************************************************************************************/
/* ANSWERS                                                                                      */
/************************************************************************************************/
int QUESTIONtoANSWERSdepth(int d){
	return d+1;
}
/************************************************************************************************/
/* ANSWER                                                                                       */
/************************************************************************************************/
int ANSWERStoANSWERdepth(int d){
	return d+1;
}

int QUESTIONtoANSWERdepth(int d){
	return QUESTIONtoANSWERSdepth(ANSWERStoANSWERdepth(d));
}
/************************************************************************************************/
/* ATOM                                                                                         */
/************************************************************************************************/
int ATOMStoATOMdepth(int d){
	return d+1;
}
int QUESTIONtoATOMdepth(int d){
	return QUESTIONdepth(0)+QUESTIONtoCONTEXTSdepth(CONTEXTStoCONTEXTdepth(CONTEXTtoATOMSdepth(ATOMStoATOMdepth(0))));
}
int CONTEXTtoATOMdepth(int d){
	return CONTEXTtoATOMSdepth(ATOMStoATOMdepth(d));
}
int ATOMtoATOMdepth(int d){
	return ATOMtoCONTEXTdepth(CONTEXTtoATOMdepth(d));
}
/************************************************************************************************/
/* ATOMS                                                                                        */
/************************************************************************************************/
int CONTEXTtoATOMSdepth(int d){
	return d+2;
}
/************************************************************************************************/
/* CONTEXTS                                                                                     */
/************************************************************************************************/
int QUESTIONtoCONTEXTSdepth(int d){
	return d;
}
int ATOMtoCONTEXTSdepth(int d){
	return d;
}
/************************************************************************************************/
/* CONTEXT                                                                                      */
/************************************************************************************************/
int CONTEXTStoCONTEXTdepth(int d){
	return d+1;
}
int ATOMtoCONTEXTdepth(int d){
	return ATOMtoCONTEXTSdepth(CONTEXTStoCONTEXTdepth(d));
}
/************************************************************************************************/
/* LEXIC                                                                                        */
/************************************************************************************************/
int CONTEXTtoLEXICdepth(int d){
	return d;
}
int QUESTIONtoLEXICdepth(int d){
	return QUESTIONtoCONTEXTSdepth(CONTEXTStoCONTEXTdepth(CONTEXTtoLEXICdepth(d)));
}

/************************************************************************************************/
/* ENTRY                                                                                        */
/************************************************************************************************/
int LEXICtoENTRYdepth(int d){
	return d+1;
}
int QUESTIONtoENTRYdepth(int d){
	return QUESTIONtoLEXICdepth(LEXICtoENTRYdepth(d));
}
int CONTEXTtoENTRYdepth(int d){
	return CONTEXTtoLEXICdepth(LEXICtoENTRYdepth(d));
}


/*
0QUESTION
	1ANSWERS
		2ANSWER
		2CONTEXTS
			3CONTEXT
			3LEXIC
				4ENTRY
			3ATOMS
				4ATOM
			3ATOMS
			3CONTEXT
		2ANSWER
0CONTEXTS
	1CONTEXT
	1LEXIC
		2ENTRY
		2ENTRY
	1ATOMS
		2ATOMS
	1ATOMS
	1CONTEXT
*/