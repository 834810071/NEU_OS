#include "OS.h"


//������һ��Ŀ¼
int goNextDir(string filename){		
	int pos = -1;
	for (int i = 0; i < fileSystem.sfd[cur_SFD].sfdVec.size(); i++){	//���ҵ���ǰĿ¼��λ��
		if (fileSystem.sfd[cur_SFD].sfdVec[i].name == filename){
			pos = i;
			break;
		}
	}
	if (pos == -1){
		textcolor(12);
		cout << "�����ڵ�Ŀ¼������ʧ�ܣ�\n";
		return 0;
	}

	int cur_iNode = fileSystem.sfd[cur_SFD].sfdVec[pos].id;	//�ļ���Ӧ��i�ڵ��id��
	if (fileSystem.iNode[cur_iNode].type == 0){
		textcolor(12);
		cout << "�����Ϊ�ļ���������ʧ�ܣ�\n";
		return 0;
	}
	cur_SFD = fileSystem.iNode[cur_iNode].sfd_id;
	return 1;
}


//��õ�ǰĿ¼��i����
int getInodeNum(){
	int pos = -1;
	for (int i = 0; i < 128; i++){
		if (iNode[i] == 0 && fileSystem.iNode[i].sfd_id == cur_SFD){
			pos = i;
			break;
		}
	}
	return pos;
}

//��ʾ��ǰĿ¼�µ������ļ���Ŀ¼
void showSFD(){
	for (int i = 0; i < fileSystem.sfd[cur_SFD].sfdVec.size(); i++){
		cout << fileSystem.sfd[cur_SFD].sfdVec[i].name << "\t\t";
		int id = fileSystem.sfd[cur_SFD].sfdVec[i].id;
		if (fileSystem.iNode[id].type == 0){	//�ļ�--0 Ŀ¼--1
			cout << "file" << endl;
		}
		else
		{
			cout << "dir" << endl;
		}
	}
	cout << endl;
}

//�ڵ���Ŀ¼�²��Ҷ�Ӧ���ļ���
void find_filedir(string cur_path, string name)	//cur_path����ֻ����������������Ŀ¼�����
{
	for (int i = 0; i < fileSystem.sfd[cur_SFD].sfdVec.size(); ++i){	//����
		if (fileSystem.sfd[cur_SFD].sfdVec[i].name == name){
			cout << name << "\t\t" << cur_path << "\\" << name << endl;
			break;
		}
	}
	for (int i = 0; i < fileSystem.sfd[cur_SFD].sfdVec.size(); ++i){	//����һ����Ų���
		if (fileSystem.sfd[cur_SFD].sfdVec[i].id != -1){	//����ж�Ӧ��i�ڵ�
			int temp_SFD = cur_SFD;
			cur_SFD = fileSystem.iNode[fileSystem.sfd[cur_SFD].sfdVec[i].id].sfd_id;
			find_filedir(cur_path + "\\" + fileSystem.sfd[temp_SFD].sfdVec[i].name, name);
			cur_SFD = temp_SFD;
		}
	}
}