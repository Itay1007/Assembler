#include "tables.h"

#define FUNCNUM 16
#define REGNUM 8
#define DIRECTNUM 2
#define MAXLEN 82
#define FINISH 2
#define MINCHAR 32
#define MAXCHAR 127
#define MAXNUMSIZE 24
#define IMMEDIATE_ADDRESS 0
#define DIRECT_ADDRESS 1 
#define RELATIVE_ADDRESS 2
#define DIRECT_REG_ADDRESS 3
#define START_ALLOC 100
#define FIT_NUM_SIZE 22
#define FIRST_FIVE_FUNC 5
#define ERROR 1
#define NO_ERROR 0
#define DEFUALT_VALUE -1
#define MAX_ENTER_INDEX 81
#define CHARS_MAX_LENGTH 80

typedef struct {

	struct {
		unsigned int  A : 1;
		unsigned int  R : 1;
		unsigned int  E : 1;
		unsigned int  funct : 5;
		unsigned int  target_reg : 3;
		unsigned int  target_address : 2;
		unsigned int  source_reg : 3;
		unsigned int  source_address : 2;
		unsigned int  opcode : 6;
	} word;

	struct {
		unsigned int  A : 1;
		unsigned int  R : 1;
		unsigned int  E : 1;
		unsigned int  val : 21;
	} extra_word;

	int L;

	int IC;

} code;

extern int IC ;
extern int DC;
extern external* external_head;
extern label* label_head;
extern data* data_head;
extern char* FUNC_NAMES[FUNCNUM];
extern int OPCODE[FUNCNUM];
extern int FUNCT[FUNCNUM];
extern char* REG_NAMES[REGNUM];
extern char* DIREC_NAMES[DIRECTNUM]; 



/*ls;gmp;gsdkpk;gnspkgpnsk*/