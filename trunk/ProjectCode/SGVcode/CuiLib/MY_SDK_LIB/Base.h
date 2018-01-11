#pragma once

#include "cpp_stl.h"

class Base
{
public:
	Base(void);
	~Base(void);
public:
    static	int str2int(int &int_temp, const string &string_temp);
	static  string int2str(int value_t);
public:

	//static string GetNameExt4path(void);

	static std::string base_name(string const & path);	

	static std::string file_name_without_ext(string const  path);

	static std::string comine_str(string f_str,string s_str);

	static std::vector<std::string> split  (const std::string &s, char delim);

	static std::vector<std::string> split  (const std::string &s, string delim);

	static void SaveString2Disk(
		string ffile_name_t,
		string str_t);
	
	static int IsEqual(std::string str_0,std::string str_1);
	
public:
	static void sleep(int ms);
	static int is_file_exist(const char * file_path);
	static int get_number(string str_t);
public:
	static double Math_GetAverageValue(double* Data, int DataNum);

	static float Math_GetAverageValueF(float* Data, int DataNum);
	
	static double Math_GetVarianceValue(
		double* Data,
		int DataNum,
		double avg,
		double *variance);

	static float Math_GetVarianceValueF(
		float* Data,
		int DataNum,
		float avg,
		float *variance);
	
	static double Math_GetMaxValue(double* Data, long DataNum);
	static void	  Math_AbsArray(double * Data, long Num);

	static void  Math_GetMaxValueIndexF(
		float* data,
		float size,
		int* sort,
		int sort_num);

	static void  Math_GetMaxValueIndex(
		double* data,
		float size,
		int* sort,
		int sort_num);


public:
	static float Math_GetSumF(float * Data,int DataNum);
public:
	static vector<float> CombineVector(vector<float> v0, vector<float> v1);
public:
	static void FS_getFiles(string path, string exd, vector<string>& files);
	static int CRACK_FILE_NAME_get_idx(string file_full_name);
	static string CRACK_FILR_NAME_get_ipAddr(string file_full_name);
};

