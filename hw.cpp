#include <iostream>
#include <string>
#include <fstream>
//#include <io.h>
#include<ctype.h>
#include <algorithm>
#include <unordered_map>
#include <time.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <iostream>
using namespace std;
long long TotalNum_chars = 0;
long long TotalNum_lines = 0;
long long TotalNum_words = 0;
struct my_word
{
    string sort_word = "zzzzzzzzzzzzzzzzzz";
    size_t appear_count = 0;
};

my_word ten_word[11];
struct my_phrase
{
    string sort_phrase = "zzzzzzzzzzzzzzzzzz";
    size_t appear_count = 0;
};

my_phrase ten_phrase[11];
unordered_map<string, my_word>word_count;
unordered_map<string, my_phrase>phrase_count;

string transform_word(string raw_word)
{
    int len = raw_word.length();
    string simple_word;
    string temp_word = raw_word;
    transform(temp_word.begin(), temp_word.end(), temp_word.begin(), ::tolower);
    bool is_start = false;
    for (int i = len - 1; i >= 0; i--)
    {
        if (isalpha(temp_word[i]))
        {
            is_start = true;
            simple_word = temp_word.substr(0, i + 1);
            break;
        }
    }
    return simple_word;
}

bool if_update(string new_word, string present_wrod)
{
    if (new_word < present_wrod)
    {
        return true;
    }
    else return false;

}

void EnterMap(string last_word, string current_word)
{
    string simple_last_word;
    string simple_current_word;
    simple_last_word = transform_word(last_word);
    simple_current_word = transform_word(current_word);
    //此处可以优化，减少查找map的次数
    //
    word_count[simple_current_word].appear_count++;
    if (current_word<word_count[simple_current_word].sort_word)
    {
        word_count[simple_current_word].sort_word = current_word;
    }


    //phrase这里有问题，没有考虑最后的输出字典序最小的原型
    string simple_phrase = simple_last_word + '_' + simple_current_word;
    string raw_phrase = last_word + '_' + current_word;
    phrase_count[simple_phrase].appear_count++;
    if (raw_phrase< phrase_count[simple_phrase].sort_phrase)
    {
        phrase_count[simple_phrase].sort_phrase = raw_phrase;
    }
}


void NumOfCharsLinesInFile(string FileLocation)
{//读入文件，统计字符数、行数、单词数，并加入到全局变量中。并对单词进行处理，加入map字典中。
    //int NumberChars = 0;
    int NumberLines = 0;
    int NumberWords = 0;
    char last_char = ' ';
    char current_char;
    bool wordbegin = false;
    string current_word;
    string last_word;

    size_t sz;
    FILE * fp = fopen(FileLocation.c_str(), "rb");
    fseek(fp, 0L, SEEK_END);
    sz = ftell(fp);

    rewind(fp);
    char*buf;
    buf = (char*)malloc(sz * sizeof(char));
    int len = fread(buf, sizeof(char), sz, fp);
    if (len) {
        NumberLines++;
    }

    for(int i = 0;i<len;i++)
    {
        current_char = buf[i];
        if (current_char == -1) {
            break;
        }
        if (current_char < -1 || current_char>255)
        {
            current_char = ' ';
        }
        //判断是否为单词
        if ((!isalpha(last_char)) && (!isdigit(last_char)) && (isalpha(current_char)))
        {
            wordbegin = true;
            current_word = current_char;
        }
        else if (wordbegin)
        {
            if ((isalpha(current_char)) || (isdigit(current_char)))
            {
                current_word = current_word + current_char;
            }
            else
            {
                wordbegin = false;
                //判断现在的current_word是否满足word的要求：前四个字符都是字母
                if (isalpha(current_word[0]))
                {
                    if (isalpha(current_word[1]))
                    {
                        if (isalpha(current_word[2]))
                        {
                            if (isalpha(current_word[3]))
                            {
                                //说明current_word满足要求
                                //cout << current_word << endl;
                                NumberWords++;
                                EnterMap(last_word, current_word);
                                last_word = current_word;
                                current_word.clear();
                            }
                        }
                    }
                }

                //如果满足word要求，则将NumberWords++，并处理该word，并last_word=current_word


                //将current_word清空
            }
        }
        //判断是否为单词结束

        if (current_char == '\n') {
            NumberLines++;
        }
        last_char = current_char;
    }




    TotalNum_chars += sz;
    TotalNum_lines += NumberLines;
    TotalNum_words += NumberWords;
    fclose(fp);
    fp = NULL;
    //
}

//深度优先递归遍历当前目录下文件夹和文件及子文件夹和文件  
void listDir(char *path)  //main函数的argv[1] char * 作为 所需要遍历的路径 传参数给listDir
{
    DIR              *pDir ;  //定义一个DIR类的指针
    struct dirent    *ent  ;   //定义一个结构体 dirent的指针，dirent结构体见上
    int               i=0  ;
    char              childpath[512];  //定义一个字符数组，用来存放读取的路径

    pDir=opendir(path); //  opendir方法打开path目录，并将地址付给pDir指针
    memset(childpath,0,sizeof(childpath)); //将字符数组childpath的数组元素全部置零


    while((ent=readdir(pDir))!=NULL) //读取pDir打开的目录，并赋值给ent, 同时判断是否目录为空，不为空则执行循环体
    {

        if(ent->d_type & DT_DIR)  //读取 打开目录的文件类型 并与 DT_DIR进行位与运算操作，即如果读取的d_type类型为DT_DIR (=4 表示读取的为目录)
        {

            if(strcmp(ent->d_name,".")==0 || strcmp(ent->d_name,"..")==0)
//如果读取的d_name为 . 或者.. 表示读取的是当前目录符和上一目录符, 用contiue跳过，不进行下面的输出
                continue;

            sprintf(childpath,"%s/%s",path,ent->d_name);  //如果非. ..则将 路径 和 文件名d_name 付给childpath, 并在下一行prinf输出
            //printf("path:%s\n",childpath);

            listDir(childpath);  //递归读取下层的字目录内容， 因为是递归，所以从外往里逐次输出所有目录（路径+目录名），
//然后才在else中由内往外逐次输出所有文件名

        }
        else  //如果读取的d_type类型不是 DT_DIR, 即读取的不是目录，而是文件，则直接输出 d_name, 即输出文件名
        {
            //cout<<ent->d_name<<endl;  //cout<<childpath<<"/"<<ent->d_name<<endl;  输出文件名 带上了目录
            sprintf(childpath,"%s/%s",path,ent->d_name);
            NumOfCharsLinesInFile(childpath);

        }
    }

}

bool compare(my_word a, my_word b)
{
    return a.appear_count>b.appear_count;   //升序排列
}

bool phrase_compare(my_phrase a, my_phrase b)
{
    return a.appear_count>b.appear_count;   //升序排列
}


void Getten_word() {

    my_word temporary_word;
    for (const auto &w : word_count)
    {
        ten_word[10] = w.second;
        for (int i = 0; i <= 9; i++)
        {
            if (ten_word[i].appear_count < ten_word[i + 1].appear_count)
            {
                temporary_word = ten_word[i];
                ten_word[i] = ten_word[i + 1];
                ten_word[i + 1] = temporary_word;
            }
        }
    }
    sort(ten_word, ten_word + 10, compare);
}

void Getten_phrase()
{
    my_phrase temporary_phrase;
    for (const auto &w : phrase_count)
    {
        ten_phrase[10] = w.second;
        for (int i = 0; i <= 9; i++)
        {
            if (ten_phrase[i].appear_count < ten_phrase[i + 1].appear_count)
            {
                temporary_phrase = ten_phrase[i];
                ten_phrase[i] = ten_phrase[i + 1];
                ten_phrase[i + 1] = temporary_phrase;
            }
        }
    }
    sort(ten_phrase, ten_phrase + 10, phrase_compare);
}

int main(int argc, char *argv[])
{
    clock_t tStart = clock();
    //递归遍历文件夹
    listDir("/home/ruizhao/test/newsample");
    //递归遍历文件夹结束
    cout << "characters: " << TotalNum_chars << endl;
    cout << "words: " << TotalNum_words << endl;
    cout << "lines: " << TotalNum_lines << endl;
    Getten_word();
    cout << "=====================word=====================" << endl;
    for (int i = 0; i < 10; i++)
    {
        cout << ten_word[i].sort_word << "  " << ten_word[i].appear_count << endl;

    }
    Getten_phrase();
    cout << "====================phrase===================" << endl;
    for (int i = 0; i < 10; i++)
    {
        cout << ten_phrase[i].sort_phrase << "  " << ten_phrase[i].appear_count << endl;

    }
    printf("Time taken: %.2fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);
    return 0;
}
