#pragma once


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#ifndef OS_PRE_GRAPH

#define OS_PRE_GRAPH


#define  _CRT_SECURE_NO_WARNINGS

#define DEFINATION 0      //声明语句
#define OPRATION 1        //操作语句
#define ASSIGNMENT 2      //赋值语句
#define ERRORTYPE -1      //错误语句类型

#define KEYTYPE 0
#define VARTYPE 1
#define NUMTYPE 2

#define KEY_WORD_NUM  3        //关键字数组的个数

#define OPERATE_NUM 8


//char OPERATE[8][5] = { "+", "-", "*", "/", ">", "<","<=",">=" };


//线的描述
struct line {
	int sourceIndex;
	int endIndex;
};

//语句描述
struct statement {
	int index;
	char *stateText;          //语句描述
	int type;                    //类型
//	char *keyWord;              //如果是声明,存储关键字
//	char *opreate[10];            //操作符数组
	char leftVar[10];            //赋值变量
	char rightVar[20][10] = { };           //使用的变量数组     最多有20个变量
	int rightCount;
	int status;

	char *errMess;
};

//变量描述
struct Var {
	char *name;                  //变量名
	int useArray[3][10] = { 0 };            //*[1]用以声明的语句  *[2]用以赋值的语句   *[3]用以使用的语句    编号    
	int first_maxIndex;
	int second_maxIndex;
	int third_maxIndex;
};


//前驱图的描述
struct preGraph {
	char *stateDescri[20];    //语句数组  最多20个语句
	int relation[20][20] = {  };       //关系二维数组
	int stateCount;
};


struct exited_var {
	char *name;
	int lastedIndex;
};
struct waiting_var {
	char *name;
	int lastedIndex;
	char rightList[20][30] = { };
	int maxIndex ;
};

struct point {
	int x;
	int y;
	
};

preGraph addNode_(preGraph graph, int index);
char **back_words(char *words);                     //分行

int _cdecl wInDisk(char filePath[], char **inString, int length);

int _cdecl isKeyWord(char *word);                 //是否是关键字

int isVar(char *word);                              //是否是变量

int isNum(char *word);                               //是否是数字

statement * bulid_state_list(char **inString, int length);       //生成语句列表

preGraph build_preGraph(statement *state, int length);                      //生成前驱图描述

int search_var_index_from_list(char varList[][20], char *var, int length);

statement build_state(char *words, int index);


//line *build_line_list(preGraph preG);

//int **build_map(statement *state, int length);
//char **string_split(char *word);

preGraph addNode(preGraph graph, int index, int preNode, int lastNode);

preGraph deleNode(preGraph graph, int index);

preGraph addLine(preGraph graph, int sourceNode, int lastNode);

preGraph deleLine(preGraph &graph, int sourceNode, int lastNode);

int get_arr_length(char **instring);

#endif // !OS_PRE_GRAPH


