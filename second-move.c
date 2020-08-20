#include "second-move.h"

/*does the first move of the compiler*/
int second_move(FILE* assem, code* code_image)
{
	char line[MAXLEN]; 
	int row_num = 1;            
		int err_num = 0; 
	int err = FALSE;
	while (fgets(line, MAXLEN, assem) != NULL)
	{
		if (strlen(line) > CHARS_MAX_LENGTH && line[MAX_ENTER_INDEX] != '\n')
		{
			fprintf(stdout, "line %d: too many chars in the line\n", row_num);
			err_num++;
		}
		else
			err_num += examine_line(line, row_num, err_num, code_image);
		++row_num;
	}

	if (err_num != 0)
		err = TRUE;

	return err;
}

int examine_line(char* line, int row_num, int err_num, code* code_image)
{
	int i = 0;
	int j = 0;

	int immediate_num_1 = 0;
	int immediate_num_2 = 0;

	int found_label = FALSE;
	int resume = TRUE;

	char func_name[MAXLEN];
	char label_name[MAXLEN]; /* represents the function name in the input line */
	char arg_enternal[MAXLEN]; /* represents the argument name of enternal in the input line */
	char arg_1[MAXLEN];
	char arg_2[MAXLEN];

	int opcode = 0;
	int source_adress = 0; /* represents the index in the array of complex numbers that is the first complex variable from the input line */
	int source_register = 0;
	int target_adress = 0;
	int target_register = 0;
	int funct = 0;
	int A = 0;
	int R = 0;
	int E = 0;

	/* skip space */
	while (isspace(line[i]))
		i++;

	/* check if it is not a comment line or an empty line */
	if (line[i] != ';' && line[i] != '\0')
	{
		/* it is an instruction line or an guidance line */
		found_label = has_label(line, i);

		if (found_label == TRUE)
		{
			/* check line of first move treated label so skip it */
			while(line[i++] != ':')
				;
		}

		/* skip space */
		while (isspace(line[i]))
			i++;

		/* check if it is a guidnace command */
		if (line[i] == '.')
		{
			i++;

			for (j = 0; !isspace(line[i]) && line[i] != '\0'; i++)
				func_name[j++] = line[i];
			func_name[j] = '\0';

			/* it is a data or string */
			if (equals(func_name, "data") || equals(func_name, "string") || equals(func_name, "extern"))
			{
				/* first_move business */
				return NO_ERROR;
			}
			else if (equals(func_name, "entry"))
			{
				/* skip space */
				while (isspace(line[i]))
					i++;		
				/* found the label - operand */				
				if(valid_ext_ent_arg(line, i, row_num, arg_enternal) == FALSE)
					return ERROR;
				if(has_label_in_table(label_head,arg_enternal) == FALSE)
				{
					fprintf(stdout, "line %d: Missing entry operand in label - table\n", row_num);
					return ERROR;
				}
				else
				{
					add_ent_guid(label_head, arg_enternal);
				}
			}
			else
			{
				fprintf(stdout, "line %d: not a valid guidenace line or not a valid extern or not valid entern line\n", row_num);
				return ERROR;
			}
		}
		else
		{
			/* it is a function */
			resume = valid_func_call(line, func_name, i, row_num); 

			if (found_label == TRUE)
			{
				if (has_label_in_table(label_head,label_name) == TRUE)
				{
					fprintf(stdout, "line %d: redefine of label\n", row_num);
					return ERROR;
				}
				add_label(&label_head, label_name, "code", IC, "\0");
			}

			if (resume == FALSE)
				return ERROR;

			/* find the opcode and funct of function */
			func_utilities(func_name, &funct, &opcode);

			/* check if stop or rts functions */
			if (resume != FINISH)
			{
				resume = valid_arg1(line, &i, func_name, arg_1, &source_adress, &source_register, &target_adress, &target_register, &immediate_num_1, row_num);
				if (resume == FALSE)
					return ERROR;
				if (resume != FINISH)
				{
					/* skip space */
					while (isspace(line[i]))
						i++;
					if (valid_comma(line, &i, row_num) == FALSE)
						return ERROR;
					/* skip space */
					while (isspace(line[i]))
						i++;
					resume = valid_target_addresing(line, &i, func_name, arg_2, &target_adress, &target_register, row_num, &immediate_num_2);

					if (resume == FALSE)
						return ERROR;
				}
			}
			if(code_image_func(code_image, opcode, source_adress, source_register, target_adress, 
							target_register, funct, A, R, E, immediate_num_1, immediate_num_2, arg_1, arg_2, row_num) == FALSE)
						return ERROR;
		}
	}
	return NO_ERROR;
}

int code_image_func(code* image_code, int opcode, int source_address, int source_register, int target_address,
	int target_register, int funct, int A, int R, int E, int immediate_num_1, int imeediate_num_2, char* arg_1, char* arg_2, int row_num)
{
	/*int OLD_IC = IC;
	int L;*/
	int first_IC = IC ;

	/* code first - word */
	image_code[IC - START_ALLOC].word.opcode = opcode;
	image_code[IC - START_ALLOC].word.source_address = source_address;
	image_code[IC - START_ALLOC].word.source_reg = source_register;
	image_code[IC - START_ALLOC].word.target_address = target_address;
	image_code[IC - START_ALLOC].word.target_reg = target_register;
	image_code[IC - START_ALLOC].word.funct = funct;
	image_code[IC - START_ALLOC].word.A = 1;
	image_code[IC - START_ALLOC].word.R = 0;
	image_code[IC - START_ALLOC].word.E = 0;

	IC++;

	if (source_address == IMMEDIATE_ADDRESS)
	{
		if(code_immediate(immediate_num_1, image_code, row_num) == FALSE)
			return FALSE;
	}
	else if (source_address == DIRECT_ADDRESS)
	{
		if(code_direct(image_code, arg_1, row_num) == FALSE)
			return FALSE; /* second move thing */
	}

	if (target_address == IMMEDIATE_ADDRESS)
	{
		if(code_immediate(imeediate_num_2, image_code, row_num) == FALSE)
			return FALSE;
	}
	else if (target_address == DIRECT_ADDRESS)
	{
		if(code_direct(image_code, arg_2, row_num) == FALSE)
			return FALSE;
	}
	else if (target_address == RELATIVE_ADDRESS)
	{
		if(code_relative(image_code, first_IC ,arg_2, row_num) == FALSE)
			return FALSE;
	}
/*	L = IC - OLD_IC;
	image_code[OLD_IC - START_ALLOC].L = L;
	image_code[IC - START_ALLOC].IC = OLD_IC;*/
	return TRUE;
}

int code_immediate(int number, code* image_code, int row_num)
{
	if(fit_number(number, row_num) == TRUE)
	{
		image_code[IC - START_ALLOC].extra_word.val = number;
		image_code[IC - START_ALLOC].extra_word.A = 1;
		image_code[IC - START_ALLOC].extra_word.R = 0;
		image_code[IC - START_ALLOC].extra_word.E = 0;
		IC++;
		return TRUE;
	}
	fprintf(stdout,"line %d: overflow of number argument\n", row_num);
	return FALSE;
}

int fit_number(int number, int row_num)
{
	if(number > pow(2, 21) && number < pow(2, 21)) /* change */
	{
		fprintf(stdout, "line %d: number overflow\n", row_num);
		return FALSE;
	}
	return TRUE;

	/*char num_str[MAXLEN] = "\0";
	char fit_num_str[FIT_NUM_SIZE] = "\0";
	_itoa(number, num_str, 10);
	strncat(fit_num_str + FIT_NUM_SIZE, num_str, FIT_NUM_SIZE - 1);
	number = atoi(fit_num_str);
	return number;*/
}
/* checks the addressing type of the argument */

void func_utilities(char* func_name, int *opcode, int *funct)
{
	*opcode = OPCODE[func_index(func_name)];
	*funct = FUNCT[func_index(func_name)];
}

int code_direct(code* image_code, char* arg, int row_num)
{
	int number;
	if(has_label_in_table(label_head, arg) == FALSE)
	{
		fprintf(stdout, "line %d: label argument did not find in label - table\n", row_num);
		return FALSE;
	}

	number = get_mem_num(label_head, arg);

	image_code[IC - START_ALLOC].extra_word.val = number;
	if(is_external_label(label_head,arg) == TRUE)
	{
		add_external(&external_head, IC, arg);
		image_code[IC - START_ALLOC].extra_word.A = 0;
		image_code[IC - START_ALLOC].extra_word.R = 0;
		image_code[IC - START_ALLOC].extra_word.E = 1;
	}
	else
	{	
		image_code[IC - START_ALLOC].extra_word.A = 0;
		image_code[IC - START_ALLOC].extra_word.R = 1;
		image_code[IC - START_ALLOC].extra_word.E = 0;
	}
	IC++;
	return TRUE;
}

/*code_to_int(int opcode, int source_address, int source_register, int target_address, int target_register, int funct, int A, int R, int E)
{
	int number;
	sprintf(str_number,"%d%d%d%d%d%d%d%d%d", opcode, source_address, source_register, target_address, target_register, funct, A, R, E);
	number = bin_to_int(str_number);
	return number;
}*/

/*void print_code(int opcode, int source_address, int source_register, int target_address, int target_register, int funct, int A, int R, int E)
{
	int number;
	char str_number[FIT_NUM_SIZE];
	char str[FIT_NUM_SIZE];
	sprintf(str_number,"%d%d%d%d%d%d%d%d%d", opcode, source_address, source_register, target_address, target_register, funct, A, R, E);
	ary2ex(str_number, str);
	fprintf(file_ob, "07%d %06s",  , str);
}*/

int code_relative(code* image_code, int jump_from, char* arg, int row_num)
{
	int length;
	int jump_to;

	if(valid_realtive_operand(arg) == FALSE)
	{
		fprintf(stdout, "line %d: operand of relative function was not found in label - table or was extern type\n", row_num);
		return FALSE;
	}
	
	jump_to = get_mem_num(label_head, arg);

	length = jump_to - jump_from;

	image_code[IC - START_ALLOC].extra_word.val = length;
	image_code[IC - START_ALLOC].extra_word.A = 1;
	image_code[IC - START_ALLOC].extra_word.R = 0;
	image_code[IC - START_ALLOC].extra_word.E = 0;
	IC++;
	return TRUE;
}

int valid_realtive_operand(char* arg)
{
	if(has_label_in_table(label_head,arg) == TRUE && is_external_label(label_head,arg) == FALSE)
		return TRUE;
	return FALSE;
}
/*checks if the function name is valid*/