#include "OS.h"

string user;    //ȫ�ֱ�������־��ǰ���ڷ����ļ�ϵͳ���û�
FileSystem fileSystem;  //�ļ�ϵͳ

int iNode[128];         //i���λ��ͼ��0��ʾռ�ݣ�1��ʾ����
int diskBlock[512];     //���̿�λ��ͼ��0��ʾռ�ݣ�1��ʾ����
int SFDBlock[512];      //Ŀ¼λ��ͼ��0��ʾռ�ݣ�1��ʾ����
int cur_SFD;        //��ǰ����sfd����ǰ����Ŀ¼
int copy_flag = 0;	//ճ�����־
SingleSFD copySFD; //ճ����
stack<int> staSFD;   //��¼Ŀ¼ջ
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




