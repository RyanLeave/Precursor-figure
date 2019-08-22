#pragma once


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#ifndef OS_PRE_GRAPH

#define OS_PRE_GRAPH


#define  _CRT_SECURE_NO_WARNINGS

#define DEFINATION 0      //�������
#define OPRATION 1        //�������
#define ASSIGNMENT 2      //��ֵ���
#define ERRORTYPE -1      //�����������

#define KEYTYPE 0
#define VARTYPE 1
#define NUMTYPE 2

#define KEY_WORD_NUM  3        //�ؼ�������ĸ���

#define OPERATE_NUM 8


//char OPERATE[8][5] = { "+", "-", "*", "/", ">", "<","<=",">=" };


//�ߵ�����
struct line {
	int sourceIndex;
	int endIndex;
};

//�������
struct statement {
	int index;
	char *stateText;          //�������
	int type;                    //����
//	char *keyWord;              //���������,�洢�ؼ���
//	char *opreate[10];            //����������
	char leftVar[10];            //��ֵ����
	char rightVar[20][10] = { };           //ʹ�õı�������     �����20������
	int rightCount;
	int status;

	char *errMess;
};

//��������
struct Var {
	char *name;                  //������
	int useArray[3][10] = { 0 };            //*[1]�������������  *[2]���Ը�ֵ�����   *[3]����ʹ�õ����    ���    
	int first_maxIndex;
	int second_maxIndex;
	int third_maxIndex;
};


//ǰ��ͼ������
struct preGraph {
	char *stateDescri[20];    //�������  ���20�����
	int relation[20][20] = {  };       //��ϵ��ά����
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
char **back_words(char *words);                     //����

int _cdecl wInDisk(char filePath[], char **inString, int length);

int _cdecl isKeyWord(char *word);                 //�Ƿ��ǹؼ���

int isVar(char *word);                              //�Ƿ��Ǳ���

int isNum(char *word);                               //�Ƿ�������

statement * bulid_state_list(char **inString, int length);       //��������б�

preGraph build_preGraph(statement *state, int length);                      //����ǰ��ͼ����

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


