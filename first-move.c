#include "first-move.h"

/*does the first move of the compiler*/
int first_move(FILE* assem, code* code_image, int* ICF, int* DCF)
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
			err_num += check_line(line, row_num, err_num, code_image);
		++row_num;
	}

	if (err_num != 0)
		err = TRUE;

	*ICF = IC;
	*DCF = DC;

	return err;
}
/*return the number of errors in a line*/
int check_line(char* line, int row_num, int err_num, code* code_image)
{
	int i = 0;
	int j = 0;

	int immediate_num_1 = 0;
	int immediate_num_2 = 0;

	int found_label = FALSE;
	int resume = TRUE;

	char func_name[MAXLEN];
	char label_name[MAXLEN]; /* represents the function name in the input line */
	char arg_ext[MAXLEN]; /* represents the argument name of data in the input line */
	char arg_1[MAXLEN];
	char arg_2[MAXLEN];

	int source_adress = 0; /* represents the index in the array of complex numbers that is the first complex variable from the input line */
	int source_register = 0;
	int target_adress = 0;
	int target_register = 0;

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
			/*puts the label in a string*/
			for (j = 0; !isspace(line[i]) && line[i] != ':'; i++)
				label_name[j++] = line[i];
			label_name[j] = '\0';
			i++;
			if (line[i] == '\0')
			{
				fprintf(stdout, "line %d: invalid line, no arguments after label\n", row_num);
				return ERROR;
			}
			else if (valid_label(row_num, label_name) == FALSE)
			{
				return ERROR;
			}
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
			if (equals(func_name, "data"))
			{
				if (found_label == TRUE)
				{
					/* add label to label - table*/
					if (has_label_in_table(label_head, label_name) == TRUE)
					{
						fprintf(stdout, "line %d: already defined label\n", row_num);
						return ERROR;
					}
					add_label(&label_head, label_name, "data", DC, 0);
				}
				if (data_args(line, i, row_num) == FALSE)
					return ERROR;
			}
			else if (equals(func_name, "string"))
			{
				if (found_label == TRUE)
				{
					/* add label to label - table*/
					if (has_label_in_table(label_head, label_name) == TRUE)
					{
						fprintf(stdout, "line %d: already defined label\n", row_num);
						return ERROR;
					}
					add_label(&label_head, label_name, "data", DC, 0);
				}
				if (string_args(row_num, i, line) == FALSE)
					return ERROR;
			}
			else if (equals(func_name, "extern"))
			{
				if (valid_ext_ent_arg(line, i, row_num, arg_ext) == FALSE)
					return ERROR;
				/* add an data label with a temporary 0 memory value */
				if (has_label_in_table(label_head, label_name) == TRUE)
				{
					fprintf(stdout, "line %d: already defined label\n", row_num);
					return ERROR;
				}
				add_label(&label_head, arg_ext, "\0", 0, "data");
 			}
			else if (equals(func_name, "entry"))
			{
				/* second_move business */
				return NO_ERROR;
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
				if (has_label_in_table(label_head, label_name) == TRUE)
				{
					fprintf(stdout, "line %d: redefine of label\n", row_num);
					return ERROR;
				}
				add_label(&label_head, label_name, "code", IC, "\0");
			}

			if (resume == FALSE)
				return ERROR;

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
			advance_IC(source_adress, target_adress);
		}
	}
	return NO_ERROR;
}
/*checks if the first argument of the function is valid*/
int valid_arg1(char* line, int* ptr_i, char* func_name, char* arg_1, int* source_adress, int* source_register, int* target_adress, int* target_register, int* immediate_number, int row) /*pro*/
{
	if (func_index(func_name) < FIRST_FIVE_FUNC)/*the first five function has two argument and the other ones only have one*/
		return valid_source_addresing(line, ptr_i, func_name, arg_1, source_adress, source_register, immediate_number, row);
	else
		return valid_target_addresing(line, ptr_i, func_name, arg_1, target_adress, target_register, row, immediate_number);
}

int func_index(char* func_name)
{
	int index = -1;
	int j = 0;
	while (FUNC_NAMES[j])
	{
		if (equals(func_name, FUNC_NAMES[j]) == TRUE);
		index = j;
	}
	return index;
}

void advance_IC(int source_address, int target_address)
{
	IC++; /* advance for the function first word */

	if (source_address == IMMEDIATE_ADDRESS || source_address == DIRECT_ADDRESS)
		IC++;
	if (target_address == IMMEDIATE_ADDRESS || target_address == DIRECT_ADDRESS || target_address == RELATIVE_ADDRESS)
		IC++;
}

int valid_source_addresing(char* line, int* ptr_i, char* func_name, char* arg_1, int* ptr_source_adress, int* ptr_source_register, int* immediate_number, int row)
{
	int i = *ptr_i;
	int j = 0;
	int source_adress = 0;
	int source_register = 0;

	source_adress = found_adress(line, ptr_i, arg_1);

	if (source_adress == IMMEDIATE_ADDRESS)
	{
		if (equals(func_name, "lea") == TRUE)
		{
			fprintf(stdout, "line %d: argument to %s can not be from type immediate addresing\n", row, func_name);
			return FALSE;
		}
		if (is_num(arg_1, i, row) == FALSE)
			return FALSE;
		else if (arg_1[i] == '+')
			i++;
		*immediate_number = atoi(arg_1);
	}
	else if (source_adress == DIRECT_ADDRESS)
	{
		if (valid_label(row, arg_1) == FALSE)
			return FALSE;
	}
	else if (source_adress == RELATIVE_ADDRESS)
	{
		fprintf(stdout, "line %d: first argument can not be from type raltive addresing\n", row);
		return FALSE;
	}
	else
	{
		if (equals(func_name, "lea") == TRUE)
		{
			fprintf(stdout, "line %d: first argument can not be from type direct register address\n", row);
			return FALSE;
		}
		for (j = 0; REG_NAMES[j]; j++)
		{
			if (equals(arg_1, REG_NAMES[j]) == TRUE)
				source_register = DIRECT_REG_ADDRESS;
		}
	}
	*ptr_i = i;
	*ptr_source_adress = source_adress;
	*ptr_source_register = source_register;
	return TRUE;
}

int valid_target_addresing(char* line, int* ptr_i, char* func_name, char* arg, int* ptr_target_adress, int* ptr_target_register, int row, int* immediate_number)
{
	int i = *ptr_i;
	int j = 0;
	int target_adress = 0; /* represents the index in the array of complex numbers that is the first complex variable from the input line */
	int target_register = 0;

	target_adress = found_adress(line, ptr_i, arg);

	if (target_adress == IMMEDIATE_ADDRESS)
	{
		if (equals(func_name, "cmp") == TRUE || equals(func_name, "prn") == TRUE)
		{
			if (is_num(arg, i, row) == FALSE)
				return FALSE;
			else if (arg[i] == '+')
				i++;
			*immediate_number = atoi(arg);
		}
		else
		{
			fprintf(stdout, "line %d: argument to %s can not be from type immediate addresing\n", row, func_name);
			return FALSE;
		}
	}
	else if (target_adress == DIRECT_ADDRESS)
	{
		if (valid_label(row, arg) == FALSE)
			return FALSE;
	}
	else if (target_adress == RELATIVE_ADDRESS)
	{
		if (equals(func_name, "jmp") == TRUE || equals(func_name, "bne") == TRUE || equals(func_name, "jsr") == TRUE)
		{
			if (valid_label(row, arg) == FALSE)
				return FALSE;
		}
		else
		{
			fprintf(stdout, "line %d: argument to %s can not be from type relative addresing\n", row, func_name);
			return FALSE;
		}
	}
	else
	{
		if (equals(func_name, "jmp") == TRUE || equals(func_name, "bne") == TRUE || equals(func_name, "jsr") == TRUE)
		{
			fprintf(stdout, "line %d: argument to %s can not be from type direct register addresing\n", row, func_name);
			return FALSE;
		}
		for (j = 0; REG_NAMES[j]; j++)
		{
			if (equals(arg, REG_NAMES[j]) == TRUE)
				target_register = DIRECT_REG_ADDRESS;
		}
	}

	*ptr_i = i;
	*ptr_target_adress = target_adress;
	*ptr_target_register = target_register;
	return FINISH;
}
/*checks if the code has a label*/
int has_label(char* line, int i)
{
	while (!isspace(line[i]))
		i++;
	--i;
	if (line[i] == ':')
	{
		return TRUE;
	}
	return FALSE;
}
/* checks if there is a label after data and if its valid */
int valid_ext_ent_arg(char* line, int i, int row, char* arg)
{
	int j = 0;

	/* skip space */
	while (isspace(line[i]))
		i++;

	if (line[i] == '\0')
	{
		fprintf(stdout, "line %d: Missing label argument for data or enternal\n ", row);
		return FALSE;
	}

	for (j = 0; !isspace(line[i]); i++)
		arg[j++] = line[i];

	if (valid_label(row, arg) == FALSE)
		return FALSE;

	/* skip space */
	while (isspace(line[i]))
		i++;

	if (line[i] != '\0')
	{
		fprintf(stdout, "line %d: Extraneous text after end of data or enternal\n", row);
		return FALSE;
	}
	return TRUE;
}
/* checks the addressing type of the argument */
int found_adress(char* line, int* ptr_i, char* arg_1)
{
	int i = *ptr_i;
	int j = 0;
	int addressing = DEFUALT_VALUE;

	while (line[i] != ',' && line[i] != '\0' && isspace(!line[i]))
	{
		if (i == *ptr_i)
		{
			if (line[i++] == '#')
				addressing = IMMEDIATE_ADDRESS;
			else if (line[i++] == '&')
				addressing = RELATIVE_ADDRESS;
			else
				arg_1[j++] = line[i++];
		}
		else
			arg_1[j++] = line[i++];
	}
	arg_1[j] = '\0';

	for (j = 0; REG_NAMES[j]; j++)
	{
		if (equals(arg_1, REG_NAMES[j]) == TRUE)
			addressing = DIRECT_REG_ADDRESS;
	}
	if (addressing == DEFUALT_VALUE)
		addressing = DIRECT_ADDRESS;

	return addressing;
}

/*checks if the arguments after .data are valid*/
int data_args(char* line, int i, int row)
{
	int j;
	char temp[MAXLEN];

	/* skip space */
	while (isspace(line[i]))
		i++;

	while (line[i] != '\0')
	{
		if (is_num(line, i, row) == TRUE)
		{
			for (j = 0; !(isspace(line[i])) && (line[i] != ',') && (line[i] != '\0'); j++, i++)
			{
				temp[j] = line[i];
			}
			temp[j] = '\0';
			add_data(&data_head, DC, atoi(temp));
			DC++;
			/* skip space */
			while (isspace(line[i]))
				i++;
			if (line[i] != '\0')
			{
				if (valid_comma(line, &i, row) == FALSE)
				{
					return FALSE;
				}
				else
				{
					/* skip space */
					while (isspace(line[i]))
						i++;
				}
			}
		}
		else
		{
			return FALSE;
		}
	}
	return TRUE;
}

int is_num(char* line, int i, int row)
{
	if (line[i] == '+' || line[i] == '-')
	{
		i++;
		if (line[i] == '+' || line[i] == '-')
		{
			fprintf(stdout, "line %d: Multiple '+' or '-' in an argument\n", row);
			return FALSE;
		}
	}
	while (!(isspace(line[i])) && line[i] != ',')
	{
		if (line[i] == '\0')
		{
			return TRUE;
		}
		else if (isdigit(line[i]) == FALSE) /*  check if the number consist digit  */
		{
			fprintf(stdout, "line %d: unvalid char not a digit in the number argument\n", row);
			return FALSE;
		}
		else
		{
			i++;
		}
	}
	return TRUE;
}
/*checks if the arguments after .string are valid*/
int string_args(int row_num, int i, char* line)
{
	/* skip space */
	while (isspace(line[i]))
		i++;

	if (line[i++] != '\"')
	{
		fprintf(stdout, "line %d: missing an opening '\"' for the string\n ", row_num);
		return FALSE;
	}
	/* we are in the string*/

	if (valid_str_final(line, i, row_num) == FALSE)
	{
		return FALSE;
	}

	/*  end of string or unvalid character in string */
	while (line[i] != '\"' && valid_str_ch(line[i]) == TRUE)
	{
		add_data(&data_head, DC, line[i]);
		DC++;
	}

	if (valid_str_ch(line[i]) == FALSE)
	{
		fprintf(stdout, "line %d: invalid character in string\n", row_num);
		return FALSE;
	}

	/* add '\0' */
	add_data(&data_head, DC, '\0');
	DC++;

	/* skip space */
	while (isspace(line[i]))
		i++;

	if (line[i] != '\0')
	{
		fprintf(stdout, "line %d: Extraneous text after end of .string\n", row_num);
		return FALSE;
	}

	return TRUE;
}

int valid_str_ch(char c)
{
	if (c >= MINCHAR && c <= MAXCHAR)
		return TRUE;
	return FALSE;
}

int valid_comma(char* line, int* ptr_curr, int row_num)
{
	int curr = *ptr_curr;

	if (line[curr] != ',')
	{
		fprintf(stdout, "line %d: Missing comma\n", row_num);
		return FALSE;
	}
	else
	{
		curr++;

		/* skip spaces */
		while (isspace(line[curr]))
			curr++;
		if (line[curr] == '\0')
		{
			fprintf(stdout, "line %d: uneeded comma\n", row_num);
			return FALSE;
		}
		/*  check for multiple comma */
		if (line[curr] == ',')
		{
			fprintf(stdout, "line %d: Multiple consecutive commas\n", row_num);
			return FALSE;
		}
		else
		{
			*ptr_curr = curr;
			return TRUE;
		}
	}
}
/*checks if the last string is valid*/
int valid_str_final(char* line, int i, int row_num)
{
	while (line[i] != '\0' && !isspace(line[i]))
		i++;

	i--;
	if (line[i] != '\"')
	{
		fprintf(stdout, "line %d: missing ending apostrophes to finish string argument\n", row_num);
		return FALSE;
	}
	return TRUE;
}
/*check if the label of the line is valid*/
int valid_label(int row, char* name)
{
	int n;

	/* check label name */
	if (strlen(name) > MAXLABEL)
	{
		fprintf(stdout, "line %d: label is too long\n", row);
		return FALSE;
	}

	/* check for alphabetic label name */
	for (n = 1; name[n]; n++)
		if (isalpha(name[n] || isdigit(name[n])) == FALSE)
			return FALSE;

	/* saved function names check */
	for (n = 0; n < FUNCNUM; n++)
	{
		if (equals(name, FUNC_NAMES[n]) == TRUE)
		{
			fprintf(stdout, "line %d: unvalid label name, can not use a command name\n", row);
			return FALSE;
		}
	}
	/* saved registers names check */
	for (n = 0; n < REGNUM; n++)
	{
		if (equals(name, REG_NAMES[n]) == TRUE)
		{
			fprintf(stdout, "line %d: unvalid label name, can not use a register name\n", row);
			return FALSE;
		}
	}
	/* saved guidnace names check */
	for (n = 0; n < DIRECTNUM; n++)
	{
		if (equals(name, DIREC_NAMES[n]) == TRUE)
		{
			fprintf(stdout, "line %d: unvalid label name, can not use a directory name\n", row);
			return FALSE;
		}
	}
	return TRUE;
}
/*checks if the function name is valid*/
int valid_func_call(char* line, char func_name[], int i, int row_num)
{
	int j = 0;
	int illegal = FALSE;
	int index = -1;

	/*  skip spaces */
	while (isspace(line[i]))
		i++;

	/* copy function name to its string */
	for (j = 0; !isspace(line[i]) && line[i] != '\0'; i++)
		func_name[j++] = line[i];
	func_name[j] = '\0';

	/* check if it is a valid function name and if it is put the index of the array in index  */
	for (j = 0; j < FUNCNUM; j++)
		if (equals(func_name, FUNC_NAMES[j]) == TRUE)
			index = j;

	/* check for illegal comma  */
	for (j = 0; j < FUNCNUM; j++)
		if (strlen(func_name) >= strlen(FUNC_NAMES[j]) && strncmp(func_name, FUNC_NAMES[j], strlen(FUNC_NAMES[j])) == 0 && func_name[strlen(FUNC_NAMES[j])] == ',')
			illegal = TRUE;

	if (illegal == TRUE)
	{
		fprintf(stdout, "line %d: Illegal comma\n", row_num);
		return FALSE;
	}
	else if (index == -1)
	{
		fprintf(stdout, "line %d: Undefined command name\n", row_num);
		return FALSE;
	}

	/*skip spaces */
	while (isspace(line[i]))
		i++;

	/* check if the function name is stop or rts */
	if (equals(func_name, "stop") == TRUE || equals(func_name, "rts") == TRUE)
	{
		if (line[i] != '\0')
		{
			fprintf(stdout, "line %d: Extraneous text after end of command\n", row_num);
			return FALSE;
		}
		else
			return FINISH;
	}
	return TRUE;
}
/*restart all the values for the next line of the code*/
