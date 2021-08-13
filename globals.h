#ifndef GLOBALS_H
#define GLOBALS_H

#define TRUE 1
#define FALSE 0
#define LINE_MAX_SIZE 81
#define WHITESPACE ' '
#define COMMENT_CHAR ';'

#define STREQ(x,y) strcmp(x,y)==0?1:0

typedef enum label_tags{
	external = 1
} label_tags;

#endif
