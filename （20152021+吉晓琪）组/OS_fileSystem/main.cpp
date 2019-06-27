#include "OS.h"

string user;    //全局变量，标志当前正在访问文件系统的用户
FileSystem fileSystem;  //文件系统

int iNode[128];         //i结点位势图，0表示占据，1表示空闲
int diskBlock[512];     //磁盘块位势图，0表示占据，1表示空闲
int SFDBlock[512];      //目录位势图，0表示占据，1表示空闲
int cur_SFD;        //当前所在sfd，当前所在目录
int copy_flag = 0;	//粘贴板标志
SingleSFD copySFD; //粘贴板
stack<int> staSFD;   //记录目录栈
string password;
char passwords[8][512];

int main()
{
	login();
	init();
	display();
	return 0;
}

//fclose(stdout);
//freopen("CON","w",stdout);
//freopen("CON","r",stdin);
//fclose(stdin);




