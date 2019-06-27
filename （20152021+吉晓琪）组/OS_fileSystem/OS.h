#ifndef OS_H_INCLUDED
#define OS_H_INCLUDED
#pragma warning(disable:4996)	//��һ��warning���õ�
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

struct SingleSFD {	//����Ŀ¼��
	string name;    //�ļ���
	int id;         //�ļ���Ӧ��i����id��
};

struct SFD {						//Ŀ¼�ṹ
	int sfdNum;                 //��̬����Ĵ�С
	vector<SingleSFD> sfdVec;   //Ŀ¼�µ�sfd����
};

struct DiskBlock {	//���̿�ṹ
	string file;	//�ļ�����	�����д�Ŷ�Ӧ�Ĵ��̿�ĳ������еĶ�Ӧ���̿� �����Ŷ�Ӧ���ļ����ݣ���ʼ����ʱ���õ���
	int strNum;		//���̿��С
};

struct INode {			//i���ṹ
	int id;				//i����������û�
	int type;			//�ļ����ͣ�0-�ļ���1-Ŀ¼
	int sfd_id;			//i����Ӧ��Ŀ¼id
	int filelen;		//�ļ�����
	int auth[8];		//8��user�ķ���Ȩ��
	int diskBlockNum;   //�ļ�ռ�ô��̿����
	int diskBlockId;    //��ռ���̿��id�ŵ�������
	int qcount;         //�ļ���������
};

struct SuperBlock {		 //������ṹ
	int i_node;          //i�������
	int freei_node;      //����i�������
	vector<int> freeiid; //����i���id����

	int sfd;             //Ŀ¼��Ŀ
	int free_SFD;        //����Ŀ¼��
	vector<int> freeSFD; //����Ŀ¼�±�����

	int disk;			 //���̿��ܿ���
	int freeDisk;        //���д��̿����
	int freeDiskSta[51]; //�������ӿ��д��̿�ջ	ֻ�������е� Ȼ���Ӧ�������ڴ��̿�DiskBlock.file ��
};

struct FileSystem {			 //�ļ�ϵͳ�ṹ
	int boot;                //������
	SuperBlock superBlock;   //������
	INode iNode[128];        //i���
	DiskBlock diskBlock[512];//���̿�
	SFD sfd[512];            //Ŀ¼��
};

extern string user;    //ȫ�ֱ�������־��ǰ���ڷ����ļ�ϵͳ���û�
extern FileSystem fileSystem;  //�ļ�ϵͳ

extern int iNode[128];      //i���λ��ͼ��0��ʾռ�ݣ�1��ʾ����
extern int diskBlock[512];  //���̿�λ��ͼ��0��ʾռ�ݣ�1��ʾ����
extern int SFDBlock[512];   //Ŀ¼λ��ͼ��0��ʾռ�ݣ�1��ʾ����
extern int cur_SFD;			//��ǰ����sfd����ǰ����Ŀ¼
extern int copy_flag;		//ճ�����־
extern SingleSFD copySFD;	//ճ����
extern stack<int> staSFD;   //��¼Ŀ¼ջ
extern string password;			//����
extern char passwords[8][512];	//��������

//**************************��ʼ��ģ��***************************
void initSuperBlock();      //��ʼ��������
void initINode();           //��ʼ��i���
void initDiskBlock();       //��ʼ�����̿�
void initSFD();             //��ʼ��SFD
void init();                //��ʼ��

//**************************�û�ģ��*****************************
void login();               //��¼�û���
void logout();              //�ǳ�
int checkUser(string user); //�жϴ�ʱ�û�
int getInodeNum();            //��õ�ǰĿ¼��i����
int checkFileAuth(string filename); //��鵱ǰ�û��Ķ�дȨ��
int checkDirAuth(string filename);  //��鵱ǰ�û���Ŀ¼�Ķ�дȨ��
int checkPassword();
void inputPassword();

//**************************����ģ��*****************************
void dis_help();			//��ʾ����
void display();				//����������������ʵ�ִ󲿷������������
void textcolor(int color);	//����������ɫ
int checkIn(string in);		//������������Ƿ�����Լ�ָ�������

//**************************�Զ���ȫģ��*****************************
void cin_name(string &name);//�ļ�/Ŀ¼������
void cin_command(string &in);//ָ������  

//**************************���д��̿�����ͷ�ģ��*****************************
void writeABlock(int r);    //��װ����ջ����д��һ���ض����̿�
void readABlock(int r);     //��һ��д�ſ��д��̿�õĴ��̿�����д��ջ
void freeABlock(int BlockNo);   //�ڳ��������л���һ�����д��̿�
int allocateOneBlock();         //�ڳ��������з���һ�����д��̿�

//**************************�ļ��Ĵ�����ɾ��ģ��*****************************
int createFirstIndexB();        //�����ļ��ĵ�һ��������
int createiNode();              //Ϊ�´������ļ�����һ��i���
int checkExitsfd(string name);  //��ѯ��ǰĿ¼��һ�̶������ļ��±�
int createFile(string name);    //�����ļ�
int *getIaddr(int indexnum);     //�õ���ɾ���ļ����������еĴ��̿������
int freeFile(string name);       //ɾ��ָ�����ֵ��ļ�
void deleteINode(int pos);       //ɾ����ɾ���ļ���Ӧ��i��㼰��ָ��Ĵ��̿�
void findSinglesfd(int inodeNo); //����ɾ�����ɾ���ļ�������ļ�Ŀ¼

//**************************�ļ��Ķ�дģ��*****************************
void writeiaddr(int BlockNo, int *iaddr);//�������е���������д�ص���Ӧ���������У������ļ���
void outputBlock(int blockNO);  //����ļ����̿�����
void readFile(string name);     //���ļ����ݺ���
int writeIndexBlock(int indexnum, int BlockNo);//�ļ����ݵ�д��
int getCur_blockNo(int inodeNo);//���ص�ǰ�ļ�i�ڵ㣬��ռ�õ������̿�
int writeFile(string name);     //дָ���ļ������ļ�
int transform(int sindex, char s[], string file);
//void inputNewPassword(char *newpassword, int size);

//**************************Ŀ¼�Ĵ���ɾ��ģ��*****************************
void showSFD(); //չʾSFD
void createInitINode(int useINode, int type, int filelen);//Ϊ�����ļ���Ŀ¼��ʼ��i���
int createDir(string filename); //����һ��Ŀ¼
void deleteINodeOne(int useINode);//ɾ��һ��i���
int deleteDir(string name);//����ɾ��һ��Ŀ¼������Ŀ¼�����ļ�
int goNextDir(string filename); //������һ��Ŀ¼

//**************************����ճ��ģ��*****************************
int copyContext(string filename);   //����ָ�����ֵ�Ŀ¼���ļ�
int cutContext(string filename);	//����ָ�����ֵ�Ŀ¼���ļ�
void find_filedir(string cur_path, string name);//�����ļ�
int pasteContext();             //ճ��ճ�����ϵ����ݵ���ǰĿ¼��

//**************************��ʽ��ģ��*****************************
void formatUser();          //�û��ĸ�ʽ��

//**************************�˳�ģ��*****************************
void exitSFD();             //�˳�ǰ����SFD����
void exitDiskBlock();       //�˳�ǰ������̿�����
void exitINode();           //�˳�ǰ����i�ڵ�����
void exitSuperBlock();      //�˳�ǰ���泬��������
void exitSystem();          //�˳�ϵͳ

#endif // OS_H_INCLUDED


