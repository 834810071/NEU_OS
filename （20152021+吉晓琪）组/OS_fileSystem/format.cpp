#include "OS.h"
//�û��ĸ�ʽ��
void formatUser(){
	//cout << fileSystem.sfd[1].sfdVec[0].id << endl;	//i�ڵ��id��
	//��¼ʱ��ѡ��ʽ������¼�û��ļ����µ������ļ�
	for (int i = 0; i < fileSystem.sfd[cur_SFD].sfdVec.size();){
		//cout<< fileSystem.sfd[cur_SFD].sfdVec.size();
		int inodeNo = fileSystem.sfd[cur_SFD].sfdVec[i].id;		//��õ�ǰĿ¼�µ�i�ڵ�id
		int type = fileSystem.iNode[inodeNo].type;
		if (type == 1) deleteDir(fileSystem.sfd[cur_SFD].sfdVec[i].name);	//ɾ��Ŀ¼	create_delete_dir.cpp
		else freeFile(fileSystem.sfd[cur_SFD].sfdVec[i].name);	//�ͷ��ļ�	create_delete_file.cpp
	}
}