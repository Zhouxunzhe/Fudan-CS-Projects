#include<stdc++.h>
#include<omp.h>
using namespace std;
#define NUMPROCS 8
#include <iostream>
#include <string>
#include <vector>
//注意这个头文件
#include <io.h>
using namespace std;


//核心代码
void getFileNames(string path, vector<string>& files)
{
	//文件句柄
	//注意：我发现有些文章代码此处是long类型，实测运行中会报错访问异常
	intptr_t hFile = 0;
	//文件信息
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("/*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			//如果是目录,递归查找
			//如果不是,把文件绝对路径存入vector中
			if ((fileinfo.attrib & _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					getFileNames(p.assign(path).append("/").append(fileinfo.name), files);
			}
			else
			{
				files.push_back(p.assign(path).append("/").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}


struct word_reader : std::ctype<char> {
	word_reader(std::string const& delims) : std::ctype<char>(get_table(delims)) {}
	static std::ctype_base::mask const* get_table(std::string const& delims) {
		static std::vector<std::ctype_base::mask> rc(table_size, std::ctype_base::mask());

		for (char ch : delims)
			rc[ch] = std::ctype_base::space;
		return &rc[0];
	}
};

int main() {
	omp_set_num_threads(NUMPROCS);
	int num = NUMPROCS;
	string str;
	vector<string> vals;
	vector<string> fileNames;

	string path("C:/Users/ZhouXunZhe/Desktop/WordCount/input"); 	//自己选择目录测试
	getFileNames(path, fileNames);
	for (const auto& file : fileNames) {
		ifstream inpstream;
		inpstream.open(file);
		while (getline(inpstream, str)) {
			istringstream in(str);
			in.imbue(std::locale(std::locale(), new word_reader(" ,.\r\n")));
			std::string word;

			while (in >> word) {
				//std::cout << word << "\n";
				vals.push_back(word);
			}
		}
	}
	int len = vals.size();
	string* arr = new string[len];
	double starttime = omp_get_wtime();
	for (int i = 0; i < len; i++) {
		arr[i] = vals[i];
	}
	map<string, int> mapper[NUMPROCS];
#pragma omp parallel for
	for (int i = 0; i < len; i++)
	{
		if (mapper[omp_get_thread_num()].find(arr[i]) != mapper[omp_get_thread_num()].end()) {
			mapper[omp_get_thread_num()][arr[i]] = 1;

		}
		else
		{
			mapper[omp_get_thread_num()][arr[i]] += 1;
		}

	}
	map <string, int> res;
#pragma omp critical
	{   for (int i = 0; i < num; i++)
		for (map<string, int>::iterator it = mapper[i].begin(); it != mapper[i].end(); it++) {
			if (res.find(it->first) != res.end())
				res[it->first] += it->second;
			else
				res[it->first] = it->second;
		}
	}
	cout << omp_get_wtime() - starttime << endl;
	FILE* ptr2 = fopen("C:/Users/ZhouXunZhe/Desktop/WordCount/resultopenmp.dat", "a");
	fprintf(ptr2, "%d\t%f\n", num, omp_get_wtime() - starttime);
	fclose(ptr2);
	FILE* fptr = fopen("C:/Users/ZhouXunZhe/Desktop/WordCount/output/outputopenmp.txt", "w+");
	for (map<string, int>::iterator it = res.begin(); it != res.end(); it++)
		fprintf(fptr, "%s\t%d\n", (it->first).c_str(), it->second);
}