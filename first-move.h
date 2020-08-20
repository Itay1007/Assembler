#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "extern.h"





int first_move(FILE* assem, code* code_image, int* ICF, int* DCF);
int check_line(char* line, int row_num, int err_num, code* code_image);
int valid_arg1(char* line, int* ptr_i, char* func_name, char* arg_1, int* source_adress, int* source_register, int* target_adress, int* target_register, int* immediate_number, int row);
int valid_source_addresing(char* line, int* ptr_i, char* func_name, char* arg_1, int* ptr_source_adress, int* ptr_source_register, int* immediate_number, int row);
int valid_target_addresing(char* line, int* ptr_i, char* func_name, char* arg, int* ptr_target_adress, int* ptr_target_register, int row, int* immediate_number);
int has_label(char* line, int i);
int valid_ext_ent_arg(char* line, int i, int row, char* arg);
int found_adress(char* line, int* ptr_i, char* arg_1);
int data_args(char* line, int i, int row);
int is_num(char* line, int i, int row);
int string_args(int row_num, int i, char* line);
int valid_str_ch(char c);
int valid_comma(char* line, int* ptr_curr, int row_num);
int valid_str_final(char* line, int i, int row_num);
int valid_label(int row, char* name);
void advance_IC(int source_address, int target_address);
int func_index(char* func_name);
int valid_func_call(char* line, char func_name[], int i, int row_num);