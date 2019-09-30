#include "tete.h"bool PROMPTMETA=false;bool PROMPTQUESTION=true;bool PROMPTCONTEXTS=false;bool PROMPTLEXIC=false;bool PROMPTANSWERS=false;bool PROMPTATOMS=false;bool PROMPTATOM=true;bool PROMPTCONTEXT=false;bool PROMPTENTRY=false;bool MULTIPLECONTEXTS=false;ATOM *getATOM(int maxdepth,META *meta);
ATOM *getREASON(META *meta);
ATOMS *getATOMS(int maxdepth,META *meta);
ATOMS *getREASONS(META *meta);
ATOM *incrementSUPPORT(ATOM *p,char *text,char *author,char *tag);
CONTEXT *getCONTEXT(int maxdepth,META *meta);
CONTEXTS *getCONTEXTS(int maxdepth,META *meta);
LEXIC *getLEXIC(int maxdepth,META *meta);
ENTRY *getENTRY(int maxdepth,META *meta);
