#include "first-move.h"
#include <math.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

int examine_line(char* line, int row_num, int err_num, code* code_image);
void func_utilities(char* func_name, int *opcode, int *funct);
int valid_realtive_operand(char* arg);
int code_image_func(code* image_code, int opcode, int source_address, int source_register, int target_address,
	int target_register, int funct, int A, int R, int E, int immediate_num_1, int imeediate_num_2, char* arg_1, char* arg_2, int row_num);
int code_immediate(int number, code* image_code, int row_num);
int second_move(FILE* assem, code* code_image);
int fit_number(int number, int row_num);
int code_direct(code* image_code, char* arg, int row_num);
int code_relative(code* image_code,int jump_from, char* arg, int row_num);