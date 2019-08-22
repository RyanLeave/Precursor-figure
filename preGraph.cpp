

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "stdafx.h"
#include "preGraph.h"
#pragma warning(disable:4996)

#define  _CRT_SECURE_NO_WARNINGS


//检查字符串是否是关键字
int isKeyWord(char *word)
{
	char KEYWORD[3][10] = { "int","float","double" };
	for (int i = 0; i < KEY_WORD_NUM; i++)
	{
		if (strcmp(word, KEYWORD[i]) != 0)
		{
			return 1;
		}
	}
	return 0;
};

//检查字符串是否是变量
int isVar(char *word)
{
	int maxLen = strlen(word);
	for (int i = 0; i < maxLen; i++)
	{
		if (i == 0 && !isalpha(word[i]) && word[i] != '_')
			return 0;
		else if (!isalpha(word[i]) && (word[i] < '0' || word[i] > '9' )&& word[i] != '_')
			return 0;
		
	}
	
	return 1;
}






//判断字符是否是变量
//int isLetter(char letter)
//{
//	if ((letter > 'a'&& letter < 'z') || (letter > 'A'&&letter < 'Z'))
//	{
//		return 1;
//	}
//	else
//	{
//		return 0;
//	}
//}


//判断字符是否是常量
int isNum(char *word)
{
	int count = strlen(word);
	for (int i = 0; i < count; i++)
	{
		if ( word[i] < '0' || word[i] > '9')
			return 0;
	}
	return 1;
}

//将程序段写入文件 txt文件
int wInDisk(char filePath[], char **inString, int length)
{
	FILE *fp;

	errno_t err;
	err = fopen_s(&fp, filePath, "wb");
	if (fp == NULL)
	{
		//....errror
		exit(0);
	}
	int i = 0;
	while (i < length)
	{
		fputs(inString[i], fp);
		i++;
	}
	fclose(fp);
	return 0;
}

//将所读的字符串格式化
/**
char **simParaBlock(char **inString)
{

}
**/

//获取字符串的类型
int get_string_type(char *word)
{
	if (isKeyWord(word) == 1)
		return KEYTYPE;
	else if (isVar(word) == 1)
		return VARTYPE;
	else if (isNum(word) == 1)
		return NUMTYPE;
	return -1;

}


//......................
int search_char(char *source, char value)
{
	int maxCount = strlen(source);
	for (int i = 0; i < maxCount; i++)
	{
		if (source[i] == value)
			return i;
	}
	return -1;
}

//是否是分隔符
char is_split(char word)
{
	char split_symble[10][1] = { '+', '-', '*', '/', '>', '<',' ',';' ,'=',' ' };
	for (int i = 0; i < 10; i++)
	{
		if (word == split_symble[i][0])
			return 1;
	}
	return 0;

}

/*
获取一个字符串中某个字符的个数
*/
int search_symble_num(char *words, char symble)
{
	int count = 0;
	int maxLen = strlen(words);
	for (int i = 0; i < maxLen; i++)
	{
		if (words[i] == symble)
			count++;
	}
	return count;
}



/*
***一行语句得分析
*word 一行字符串
index 行的编号
return newState  语句分析所得的语句结构体
***********************************************
规定一行只能有一个语句   即一个分号';'
*/
statement build_state(char *words, int index)
{
	int splitIndex = -1;                                  //最新的分隔符的下标
	int maxCount = strlen(words);


	statement newState;

	newState.index = index;
	newState.stateText = words;
	newState.status = 0;

	int semicolon_count = search_symble_num(words, ';');      //分号的数目




	int equal_index = search_char(words, '=');            //等于号的位置

	char tmp[10] = { 0 };
	int tmp_i = 0;

	int leftCount = 0;                            //左边变量计数
	int rightCount = 0;                           //右变量技术

	//超过10报错
	for (int i = 0; i < maxCount; i++)
	{
		//如果不是分隔符

		if (is_split(words[i]) == 0)
		{
			tmp[tmp_i] = words[i];
			tmp_i++;
		}
		else
		{
			//他不是连续的分割
			if (splitIndex + 1 != i)
			{
				if (i <= equal_index && leftCount < 1)
				{
					strcpy(newState.leftVar, tmp);
					leftCount++;
				}
				else if(leftCount < 2)
				{
					strcpy(newState.rightVar[rightCount], tmp);
					rightCount++;

				}
				else 
				{
					newState.type = ERRORTYPE;
					char error_out[] = "left var is more than 1";
					newState.errMess = error_out;
				
				}
			}
			splitIndex = i;
			tmp_i = 0;
		}


	}

//	if (equal_index == -1 && isKeyWord(newState.rightVar[0]) == 1)
//	{
//		newState.type = DEFINATION;
//	}
//	else 
	if (equal_index != -1)
	{
		int signal_num = 0;
		for (int i = 0; i < rightCount; i++)
		{
			if (isNum(newState.rightVar[i]) == 1)
				signal_num++;
		}
		if (signal_num == rightCount)
			newState.type = ASSIGNMENT;
		else
		{
			newState.type = OPRATION;
		}
	}

	newState.rightCount = rightCount;

	//错误处理
		//分号多于1
	if (semicolon_count > 1)
	{
		newState.type = ERRORTYPE;
		char err_mess_List[] = "The number of ';' is more than 1";
		newState.errMess = err_mess_List;
	}

	return newState;

}

int wh_meaning(int map[][20], int index)
{
	if (map[index][index] == -1)
		return 0;
	return 1;
}
//删除节点
preGraph deleNode(preGraph graph, int index)
{
	preGraph newPreGraph = graph;
	if (newPreGraph.stateCount > 1)
		newPreGraph.stateCount--;

	int exitNode = wh_meaning(newPreGraph.relation, index);
	if (exitNode == 1)
	{
		for (int i = 0; i < 10; i++)
		{
			newPreGraph.relation[i][index] = -1;
			newPreGraph.relation[index][i] = -1;
		}
	}
	return newPreGraph;
}

preGraph addNode_(preGraph graph, int index)
{
	preGraph newPreGraph = graph;
	if (newPreGraph.stateCount < 9)
		newPreGraph.stateCount++;
	if (index < 10 && graph.relation[index][index] == -1)
	{
		for (int i = 0; i < 10; i++)
		{
			newPreGraph.relation[i][index] = 0;
			newPreGraph.relation[index][i] = 0;
		}
	}
	return newPreGraph;
}
//增加节点
preGraph addNode(preGraph graph, int index, int preNode, int lastNode)
{
	preGraph newPreGraph = graph;
	if (index < 10 && newPreGraph.relation[index][index] == -1)
	{
		for (int i = 0; i < 10; i++)
		{
			newPreGraph.relation[i][index] = 0;
			newPreGraph.relation[index][i] = 0;
		}


		if (preNode < 10 && preNode != -1 && newPreGraph.relation[preNode][preNode] != -1)
		{
			newPreGraph.relation[preNode][index] = 1;
		}
		if (lastNode < 10 && lastNode != -1 && newPreGraph.relation[lastNode][lastNode] != -1)
			newPreGraph.relation[index][lastNode] = 1;
	}
	return newPreGraph;
}

//添加边
preGraph addLine(preGraph graph, int sourceNode, int lastNode)
{
	preGraph newPreGraph = graph;
	int exitSource = wh_meaning(newPreGraph.relation, sourceNode);
	int exitEnd = wh_meaning(newPreGraph.relation, lastNode);
	if (newPreGraph.relation[sourceNode][lastNode] != 1 && exitSource == 1 && exitEnd == 1)
		newPreGraph.relation[sourceNode][lastNode] = 1;

	return newPreGraph;

}

//删除边
preGraph deleLine(preGraph &graph, int sourceNode, int lastNode)
{
	preGraph newPreGraph = graph;
	int exitSource = wh_meaning(newPreGraph.relation, sourceNode);
	int exitEnd = wh_meaning(newPreGraph.relation, lastNode);
	if (newPreGraph.relation[sourceNode][lastNode] == 1 && exitSource == 1 && exitEnd == 1)
		newPreGraph.relation[sourceNode][lastNode] = 0;
	return newPreGraph;
}


//词法分析
statement *bulid_state_list(char** inString, int length)
{
	int M = 2 * length;                       //list长度
	int stateIndex = 0;
	statement *stateList = (statement *)malloc(M * sizeof(statement));


	for (int i = 0; i < length; i++)
	{
		stateList[i] = build_state(inString[i], i);
	}


	return stateList;
}

//查找是否有重复的变量数组
int search_var_index(Var *varList, char *varName, int length)
{
	if (length == 0)
	{
		return -1;
	}
	else {
		for (int i = 0; i < length; i++)
		{
			if (0 == strcmp(varList[i].name, varName))
				return i;
		}
	}
	return -1;
}

//在指定类型中查找字符串是否存在
int search_var(waiting_var *list, char *a, int length)
{
	if (length == 0)
		return -1;
	for (int i = 0; i < length; i++)
	{
		if (0 == strcmp(list[i].name,a))
			return i;
	}
	return -1;
}

/*

int **build_map(statement *state, int length)
{
	int **newMap = (int **)malloc(length * sizeof(int *));
	for (int i = 0; i < length; i++)
		newMap[i] = (int *)malloc(length * sizeof(int));



	char *using_var[20];
	int using_count = 0;

	exited_var *ev = (exited_var *)malloc(3 * length * sizeof(exited_var));                   //已完成的变量
	int ev_max_count = 0;

	exited_var *wv = (exited_var *)malloc(3 * length * sizeof(exited_var));                     //即将完成的变量
	int wv_max_count = 0;

	int isFinished = 0;                     //全部识别标记
	int round = 0;                          //防止无限循环
	while (isFinished == 0 && round <= length)
	{
		int k = 0;

		//一轮检索
		for (int i = 0; i < length; i++)
		{
			//对未完成的语句进心分析
			if (state[i].status == 0)
			{
				if (state[i].type == ASSIGNMENT)
				{
					int exit_in_ev_index = search_var(ev, state[i].leftVar, ev_max_count);                         //变量是否在已经赋值的列表中
					int exit_in_using_index = search_var_index_from_list(using_var, state[i].leftVar, using_count);            //变量是否在使用列表中
					//也没有正在定义或使用的
					if (exit_in_using_index == -1)
					{
						using_var[using_count] = state[i].leftVar;
						using_count++;

						wv[wv_max_count].name = state[i].leftVar;
						wv[wv_max_count].lastedIndex = i;
						wv_max_count++;
					}

					if (exit_in_ev_index != -1)
					{
						newMap[ev[exit_in_ev_index].lastedIndex][i] = 1;
					}
				}

				else if (state[i].type == OPRATION)
				{
					int able_symble = 1;
					for (int j = 0; i < state[i].rightCount; i++)
					{
						int svi = search_var(ev, state[i].rightVar[j], ev_max_count);               //变量是否已经赋值
						int isWating = search_var(wv, state[i].rightVar[j], wv_max_count);
						//右边变量均未赋值 或 右边变量有正在进行赋值
						if (1 == isVar(state[i].rightVar[j]) && -1 == svi && -1 != isWating)
						{
							able_symble = 0;
						}
					}

					int exit_in_using_index = search_var_index_from_list(using_var, state[i].leftVar, using_count);
					//int exit_in_wv_index = search_var(wv, state[i].leftVar[0], wv_max_count);
					//右边变量均以赋值并未正在进行赋值 左边变量未在使用且未进行赋值
					if (able_symble == 1 && -1 == exit_in_using_index)
					{
						wv[wv_max_count].name = state[i].leftVar;
						wv[wv_max_count].lastedIndex = i;
						wv_max_count++;

						using_var[using_count] = state[i].leftVar;
						using_count++;

						for (int n = 0; n < state[i].rightCount; n++)
						{
							int source = search_var(ev, state[i].rightVar[n], ev_max_count);
							newMap[source][i] = 1;
							if (search_var_index_from_list(using_var,state[i].leftVar,using_count) == -1 && isVar(state[i].rightVar[n]))
							{
								using_var[using_count] = state[i].rightVar[n];
								using_count++;
							}
						}
					}
				}

				else
				{
					//...
				}
			}



			else
				k++;

		}


		//将等待的变量写入
		for (int i = 0; i < wv_max_count; i++)
		{
			int isExit = search_var(ev, wv[i].name, ev_max_count);
			state[wv[i].lastedIndex].status = 1;
			if (isExit == -1)
			{
				ev[ev_max_count] = wv[i];
			}
			else
			{
				ev[isExit].lastedIndex = wv[i].lastedIndex;
			}
		}


		if (k == length)
			isFinished = 1;

		using_count = 0;
		wv_max_count = 0;

		round++;
	}
	return newMap;
	
}
*/
//二维数组中查找对应的字符串
int search_var_index_from_list(char varList[][30],char *var,int length)
{
	if (length != 0)
	{
		for (int i = 0; i < length; i++)
		{
			if (strcmp(varList[i], var) == 0)
			{
				return i;
			}
		}
	}
	return -1;
}


//生成前驱图
preGraph build_preGraph(statement *state, int length)
{
	preGraph newPreGraph;


	int varNum = 2 * length;
	Var *varList = (Var *)malloc(varNum * sizeof(Var));
	int varIndex = 0;

	//生成varList
	for (int i = 0; i < length; i++)
	{
		newPreGraph.stateDescri[i] = state[i].stateText;

		/*
		if (state[i].type == ERRORTYPE)
		{
			//...出错处理
		}
		else if (state[i].type == DEFINATION)
		{
			for (int j = 1; j < state[i].rightCount; i++)
			{
				int targetIndex = search_var_index(varList, state[i].rightVar[j], varIndex);
				if (-1 == targetIndex && isVar(state[i].rightVar[j]) == 1)
				{
					varList[varIndex].name = state[i].rightVar[j];
					varList[varIndex].useArray[0][0] = i;
					varList[varIndex].first_maxIndex = 1;
					varList[varIndex].second_maxIndex = 0;
					varList[varIndex].third_maxIndex = 0;
					varIndex++;
				}
				else {
					varList[targetIndex].useArray[0][varList[targetIndex].first_maxIndex] = i;
					varList[targetIndex].first_maxIndex++;
				}
			}
		}

		else if (state[i].type == OPRATION)
		{
			//左边变量
			int left_tar_index = search_var_index(varList, state[i].leftVar[0], varIndex);
			if (left_tar_index == -1 && 1 == isVar(state[i].leftVar[0]))
			{
				varList[varIndex].name = state[i].leftVar[0];
				varList[varIndex].useArray[1][0] = i;
				varList[varIndex].first_maxIndex = 0;
				varList[varIndex].second_maxIndex = 1;
				varList[varIndex].third_maxIndex = 0;
				varIndex++;
			}
			else if (left_tar_index != -1 && 1 == isVar(state[i].leftVar[0]))
			{
				varList[left_tar_index].useArray[1][varList[left_tar_index].second_maxIndex] = i;
				varList[left_tar_index].second_maxIndex++;
			}
			else
			{
				//....错误处理
			}


			//右变量

			for (int j = 0; j < state[i].rightCount; j++)
			{
				int right_tar_index = search_var_index(varList, state[i].rightVar[j], varIndex);

				if (right_tar_index == -1 && 1 == isVar(state[i].rightVar[j]))
				{
					varList[varIndex].name = state[i].rightVar[j];
					varList[varIndex].useArray[2][0] = i;
					varList[varIndex].first_maxIndex = 0;
					varList[varIndex].second_maxIndex = 0;
					varList[varIndex].third_maxIndex = 1;
					varIndex++;
				}
				else if (right_tar_index != -1 && 1 == isVar(state[i].rightVar[j]))
				{
					varList[right_tar_index].useArray[2][varList[right_tar_index].third_maxIndex] = i;
					varList[right_tar_index].third_maxIndex++;
				}
				else
				{
					//....错误处理
				}
			}
		}


		else if (state[i].type == ASSIGNMENT)
		{
			int tar_index = search_var_index(varList, state[i].leftVar[0], varIndex);

			if (tar_index == -1 && 1 == isVar(state[i].leftVar[0]))
			{
				varList[varIndex].name = state[i].leftVar[0];
				varList[varIndex].useArray[1][0] = i;
				varList[varIndex].first_maxIndex = 0;
				varList[varIndex].second_maxIndex = 1;
				varList[varIndex].third_maxIndex = 0;
				varIndex++;
			}
			else if (tar_index != -1 && 1 == isVar(state[i].leftVar[0]))
			{
				varList[tar_index].useArray[1][varList[tar_index].second_maxIndex] = i;
				varList[tar_index].second_maxIndex++;
			}
			else
			{
				//...错误处理
			}
		}

		*/

	}


	
	

	//生成map

	

	//int ** map = build_map(state, length);

	waiting_var *wv = (waiting_var *)malloc(50 * sizeof(waiting_var));
	int wv_count = 0;

	for (int i = 0; i < length; i++)
	{
		if (state[i].type == ASSIGNMENT)
		{
			int isExit = search_var(wv, state[i].leftVar, wv_count);
			if (isExit == -1)
			{
				wv[wv_count].name = state[i].leftVar;
				wv[wv_count].lastedIndex = i;
				wv[wv_count].maxIndex = 0;
				wv_count++;
			}
			else
			{
				wv[isExit].lastedIndex = i;
				newPreGraph.relation[isExit][i] = 1;
			}
		}

		else if (state[i].type == OPRATION)
		{
			int oldIndex;
			int right_sign = 1;
			int isExit = search_var(wv, state[i].leftVar, wv_count);
			if (isExit == -1)
			{
				wv[wv_count].name = state[i].leftVar;
				wv[wv_count].lastedIndex = i;
				wv[wv_count].maxIndex = 0;
				isExit = wv_count;
				wv_count++;
			}
			else
			{
				newPreGraph.relation[wv[isExit].lastedIndex][i] = 1;
				oldIndex = wv[isExit].lastedIndex;
				wv[isExit].lastedIndex	= i;
			}

				for (int j = 0; j < state[i].rightCount; j++)
				{
					int wh_var = isVar(state[i].rightVar[j]);
					int source = search_var(wv, state[i].rightVar[j], wv_count);
					int reshow = search_var_index_from_list(wv[isExit].rightList, state[i].rightVar[j], wv[isExit].maxIndex);
					if (wh_var == 1 && source != -1 && reshow == -1)
					{
						if (source == isExit)
						{
							newPreGraph.relation[oldIndex][i] = 1;
						}
						else
						{
							newPreGraph.relation[wv[source].lastedIndex][i] = 1;
						}
						
						int l = wv[isExit].maxIndex;
						strcpy(wv[isExit].rightList[l], state[i].rightVar[j]);
						char tmp[30];
						strcpy(tmp, state[i].rightVar[j]);

						strcpy(wv[isExit].rightList[l], tmp);
						//wv[isExit].rightList[l] = state[i].rightVar[j];
						//strcpy(wv[isExit].rightList[wv[isExit].maxIndex], state[i].rightVar[j]);
						//wv[isExit].rightList[wv[isExit].maxIndex] = state[i].rightVar[j];
						wv[isExit].maxIndex++;
					}
					else if (wh_var == 1 && source == -1)
					{
						right_sign = 0;
					}
				}
			//}
		}
	}

	for (int i = 0; i < 10; i++)
	{
		for (int j = i; j < 10; j++)
		{

			if (newPreGraph.relation[i][j] == 1)
			{
				for (int k = 1; j + k < 10; k++)
				{
					if (newPreGraph.relation[j][j + k] == 1)
						newPreGraph.relation[i][j + k] = 0;
				}
			}
		}
	}
	//newPreGraph.relation = map;


	newPreGraph.stateCount = length;

	return newPreGraph;
}

int search_var_index_from_list(char varList[][20], char * var, int length)
{
	return 0;
}

/**
int main()
{
	return 0;
}

*/
//
////更具前驱图 生成线列表
//line *build_line_list(preGraph preG)
//{
//	int length = preG.stateCount;
//	line *new_line_list = (line *)malloc(length * sizeof(line));
//	int lineCount = 0;
//	for (int i = 0; i < length; i++)
//	{
//		for (int j = 0; j < length; j++)
//		{
//			if (preG.relation[i][j] == 1)
//			{
//				new_line_list[lineCount].sourceIndex = i;
//				new_line_list[lineCount].endIndex = j;
//				lineCount++;
//			}
//		}
//	}
//
//	return new_line_list;
//}
//
////查找换行符得个数
//int search_row_num(char *words)
//{
//	int len = strlen(words);
//	int n = 0;
//	for (int i = 0; i < len-3; i++)
//	{
//		if (words[i] == '\r' && words[i + 1] == '\n')
//			n++;
//	}
//	return n;
//}

char **back_words(char *words)
{
	//int length;
	//length = search_row_num(words);

	char **inString = (char **)malloc(20 * sizeof(char *));
	for (int i = 0; i < 20; i++)
	{
		inString[i] = (char *)malloc(50 * sizeof(char));
	}

	int string_index = 0;

	int maxLen = strlen(words);

	char tmp_save[50];
	memset(tmp_save, 0, sizeof tmp_save);
	int tmp_index = 0;
	for (int i = 0; i < maxLen; i++)
	{
		if (words[i] != '\r' && words[i] != '\n' && i+1 != maxLen)
		{
			tmp_save[tmp_index] = words[i];
			tmp_index++;
		}
		else if ( words[i] == '\r' && words[i+1] == '\n')
		{
			if (tmp_index >3)
			{
				strcpy(inString[string_index], tmp_save);
				tmp_index = 0;
				memset(tmp_save, 0, sizeof tmp_save);
				string_index++;
			}
		}
		else if (i + 1 == maxLen)
		{
			tmp_save[tmp_index] = words[i];
			strcpy(inString[string_index], tmp_save);
			string_index++;
		}
	}
	char empty_arr[] = "";
	strcpy(inString[string_index], empty_arr);
	return inString;
}


//int store_graph(preGraph graph,char name[])
//{
//	char filepath[] = "D"
//}

//获取二维数组得行数
int get_arr_length(char **inString)
{
	int length = 0;
	char *tmp_char = inString[length];
	while (strlen(tmp_char) > 3)
	{
		length++;
		tmp_char = inString[length];
	}

	return length;
}