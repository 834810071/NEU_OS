#ifndef OS_H_INCLUDED
#define OS_H_INCLUDED
#pragma warning(disable:4996)	//将一个warning禁用掉
#include <iostream>
#include <conio.h>
#include <cstdio>
#include <cstring>
#include <vector>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <stack>
#include <windows.h>
#include <cstdlib>
#include <fstream>
#include <cmath>
#include "OS.h"

using namespace std;

struct SingleSFD {	//单个目录项
	string name;    //文件名
	int id;         //文件对应的i结点的id号
};

struct SFD {						//目录结构
	int sfdNum;                 //动态数组的大小
	vector<SingleSFD> sfdVec;   //目录下的sfd数组
};

struct DiskBlock {	//磁盘块结构
	string file;	//文件内容	若空闲存放对应的磁盘块的成组链中的对应磁盘块 否则存放对应的文件内容（初始化的时候用到）
	int strNum;		//磁盘块大小
};

struct INode {			//i结点结构
	int id;				//i结点所属的用户
	int type;			//文件类型，0-文件，1-目录
	int sfd_id;			//i结点对应的目录id
	int filelen;		//文件长度
	int auth[8];		//8个user的访问权限
	int diskBlockNum;   //文件占用磁盘块个数
	int diskBlockId;    //所占磁盘块的id号的索引块
	int qcount;         //文件的引用数
};

struct SuperBlock {		 //超级块结构
	int i_node;          //i结点总数
	int freei_node;      //空闲i结点总数
	vector<int> freeiid; //空闲i结点id数组

	int sfd;             //目录数目
	int free_SFD;        //空闲目录数
	vector<int> freeSFD; //空闲目录下表数组

	int disk;			 //磁盘块总块数
	int freeDisk;        //空闲磁盘块块数
	int freeDiskSta[51]; //成组链接空闲磁盘块栈	只存主存中的 然后对应的链接在磁盘块DiskBlock.file 中
};

struct FileSystem {			 //文件系统结构
	int boot;                //引导区
	SuperBlock superBlock;   //超级块
	INode iNode[128];        //i结点
	DiskBlock diskBlock[512];//磁盘块
	SFD sfd[512];            //目录块
};

extern string user;    //全局变量，标志当前正在访问文件系统的用户
extern FileSystem fileSystem;  //文件系统

extern int iNode[128];      //i结点位势图，0表示占据，1表示空闲
extern int diskBlock[512];  //磁盘块位势图，0表示占据，1表示空闲
extern int SFDBlock[512];   //目录位势图，0表示占据，1表示空闲
extern int cur_SFD;			//当前所在sfd，当前所在目录
extern int copy_flag;		//粘贴板标志
extern SingleSFD copySFD;	//粘贴板
extern stack<int> staSFD;   //记录目录栈
extern string password;			//密码
extern char passwords[8][512];	//密码数组

//**************************初始化模块***************************
void initSuperBlock();      //初始化超级块
void initINode();           //初始化i结点
void initDiskBlock();       //初始化磁盘块
void initSFD();             //初始化SFD
void init();                //初始化

//**************************用户模块*****************************
void login();               //登录用户名
void logout();              //登出
int checkUser(string user); //判断此时用户
int getInodeNum();            //获得当前目录的i结点号
int checkFileAuth(string filename); //检查当前用户的读写权限
int checkDirAuth(string filename);  //检查当前用户对目录的读写权限
int checkPassword();
void inputPassword();

//**************************界面模块*****************************
void dis_help();			//显示帮助
void display();				//界面主函数，用来实现大部分输入输出功能
void textcolor(int color);	//设置字体颜色
int checkIn(string in);		//用来检测命令是否存在以及指令的种类

//**************************自动补全模块*****************************
void cin_name(string &name);//文件/目录名输入
void cin_command(string &in);//指令输入  

//**************************空闲磁盘块分配释放模块*****************************
void writeABlock(int r);    //将装满的栈内容写入一个特定磁盘块
void readABlock(int r);     //将一个写着空闲磁盘块好的磁盘块内容写入栈
void freeABlock(int BlockNo);   //在成组链接中回收一个空闲磁盘块
int allocateOneBlock();         //在成组链接中分配一个空闲磁盘块

//**************************文件的创建与删除模块*****************************
int createFirstIndexB();        //创建文件的第一个索引块
int createiNode();              //为新创建的文件分配一个i结点
int checkExitsfd(string name);  //查询当前目录下一固定名的文件下标
int createFile(string name);    //创建文件
int *getIaddr(int indexnum);     //得到待删除文件的索引块中的磁盘块号数组
int freeFile(string name);       //删除指定名字的文件
void deleteINode(int pos);       //删除待删除文件对应的i结点及其指向的磁盘块
void findSinglesfd(int inodeNo); //遍历删除与待删除文件共享的文件目录

//**************************文件的读写模块*****************************
void writeiaddr(int BlockNo, int *iaddr);//将数组中的索引内容写回到相应的索引块中，磁盘文件中
void outputBlock(int blockNO);  //输出文件磁盘块内容
void readFile(string name);     //读文件内容函数
int writeIndexBlock(int indexnum, int BlockNo);//文件内容的写入
int getCur_blockNo(int inodeNo);//返回当前文件i节点，所占用的最后磁盘块
int writeFile(string name);     //写指定文件名的文件
int transform(int sindex, char s[], string file);
//void inputNewPassword(char *newpassword, int size);

//**************************目录的创建删除模块*****************************
void showSFD(); //展示SFD
void createInitINode(int useINode, int type, int filelen);//为创建文件或目录初始化i结点
int createDir(string filename); //创建一个目录
void deleteINodeOne(int useINode);//删除一个i结点
int deleteDir(string name);//级联删除一个目录及其子目录和子文件
int goNextDir(string filename); //进入下一级目录

//**************************复制粘贴模块*****************************
int copyContext(string filename);   //复制指定名字的目录或文件
int cutContext(string filename);	//剪切指定名字的目录或文件
void find_filedir(string cur_path, string name);//查找文件
int pasteContext();             //粘贴粘贴板上的内容到当前目录下

//**************************格式化模块*****************************
void formatUser();          //用户的格式化

//**************************退出模块*****************************
void exitSFD();             //退出前保存SFD内容
void exitDiskBlock();       //退出前保存磁盘块内容
void exitINode();           //退出前保存i节点内容
void exitSuperBlock();      //退出前保存超级块内容
void exitSystem();          //退出系统

#endif // OS_H_INCLUDED


