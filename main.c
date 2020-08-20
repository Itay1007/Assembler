#include "main.h"

int IC = START_ALLOC;
int DC;
label* label_head = NULL;
data* data_head = NULL;
external* external_head = NULL;
char* FUNC_NAMES[FUNCNUM] = { "move", "cmp", "add", "sub", "lea", "clr", "not", "inc", "dec", "jmp", "bne", "jsr", "red", "prn", "rts", "stop" };
int OPCODE[FUNCNUM] = { 0,1,2,2,4, 5,5, 5, 5,9,9,9,12,13, 14, 15 };
int FUNCT[FUNCNUM] = { 0,0,1,2,0, 1, 2,3, 4,1,2,3,0,0,0,0 };
char* REG_NAMES[REGNUM] = { "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7" };
char* DIREC_NAMES[DIRECTNUM] = { "string", "data" };

#define PAD 5

int valid_files_num(int);

int main(int argc, char** argv)
{
	int err;
	label* label_head = NULL;
	data* data_head = NULL;
	char* name;
	int ICF = 0, DCF = 0;
	FILE* assembler;

	code* code_image = (code*)malloc(sizeof(code) * 10000);
	
	if(valid_malloc(code_image) == FALSE)
		return FALSE;

	err = FALSE;
	
	if (valid_files_num(argc) == FALSE)
		return 1;

	while(--argc)
	{
		err = FALSE;
		name = *(argv + argc);
		if (open_file(name, &assembler) == FALSE)
			return 1;
		err = first_move(assembler, code_image, &ICF, &DCF);
		update_label(label_head, ICF);
		update_data(data_head, ICF);

		if (err != TRUE)
		{
			IC = START_ALLOC;
			err = second_move(assembler,code_image);
			if (err != TRUE)
			{
				create_files(name, ICF, DCF, label_head, code_image, data_head, external_head);
			}
		}
	}

	return 0;
}

int open_file(char* name, FILE** assembler)
{
	char* assem_file = (char*)malloc((strlen(name) + PAD) * sizeof(char));

	if (assem_file == NULL)
	{
		fprintf(stderr, "could not make enough memory\n");
		exit(0);
	}

	strcpy(assem_file, name);
	strcat(assem_file, ".as");

	if ((*assembler = fopen(assem_file, "r")) == NULL)
	{
		fprintf(stderr, "error in the open of an assembler file\n");
		return FALSE;
	}
	return TRUE;
}

int valid_files_num(int argc)
{
	/*check if there is only program name*/
	if (argc == 1)
	{
		fprintf(stderr, "there are no assembly files\n");
		return FALSE;
	}
	return TRUE;
}

int valid_malloc(code* code_image) 
{
	if(code_image == NULL)
	{
		return FALSE;
	}
	return TRUE;
}


void restart(label *label_head, data *data_head, external *external_head)
{
	IC = START_ALLOC;
	DC = 0;
	free_label(&label_head);
	free_data(&data_head);
	free_external(&external_head);
}
