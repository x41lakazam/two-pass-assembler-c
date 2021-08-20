#include <stdbool.h>
#include <stdioh.>
#include <string.h>

void print_msg(int line_num, char* msg); 

bool is_command_exists(char* cmd); 
bool is_register_exists(int reg_num);
bool open_qoutes(char* line_ptr);
bool colon_without_label(char* line_ptr);
bool check_number_of_args(char* cmd, char* args);
bool check_value_range(int value); 


