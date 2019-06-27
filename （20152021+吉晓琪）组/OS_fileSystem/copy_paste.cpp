#include "OS.h"

//����ָ�����ֵ�Ŀ¼���ļ�ʵ���ļ�����
int copyContext(string filename){
	int pos = -1;
	for (int i = 0; i < fileSystem.sfd[cur_SFD].sfdVec.size(); i++){
		if (fileSystem.sfd[cur_SFD].sfdVec[i].name == filename){
			pos = i;
			break;
		}
	}
	if (pos == -1){
		textcolor(12);
		cout << "����ʧ�ܣ������ڵ��ļ���\n";
		return 0;
	}
	copySFD.name = fileSystem.sfd[cur_SFD].sfdVec[pos].name;
	copySFD.id = fileSystem.sfd[cur_SFD].sfdVec[pos].id;
	copy_flag = 1;
	return 1;
}

int cutContext(string filename){
	int flag = -1;
	vector<SingleSFD>::iterator iter = fileSystem.sfd[cur_SFD].sfdVec.begin(); //512��SFDblock
	for (int i = 0; i < fileSystem.sfd[cur_SFD].sfdVec.size(); i++){
		if (iter->name == filename){
			flag = 1;
			break;
		}
		iter++;
	}
	if (flag == -1){
		textcolor(12);
		cout << "����ʧ�ܣ������ڵ��ļ���\n";
		return 0;
	}
	else if (flag == 1){
		copySFD.name = iter->name;
		copySFD.id = iter->id;
		fileSystem.sfd[cur_SFD].sfdVec.erase(iter);
		fileSystem.sfd[cur_SFD].sfdNum--;
		copy_flag = 2;
		return 1;
	}
}

//ճ��ճ�����ϵ����ݵ���ǰĿ¼��
int pasteContext()
{
	if (copy_flag > 0){
		int pos = -1;
		for (int i = 0; i < fileSystem.sfd[cur_SFD].sfdVec.size(); i++){
			if (copySFD.name == fileSystem.sfd[cur_SFD].sfdVec[i].name){
				pos = i;
				break;
			}
		}
		if (pos != -1){
			textcolor(12);
			cout << "ʧ�ܣ���ǰĿ¼���Ѿ�������ճ������ͬ�����ļ���Ŀ¼��\n";
			return 0;
		}
		if (copy_flag == 2 || copy_flag == 3){//���ӻ����
			int id = copySFD.id;
			fileSystem.iNode[id].qcount++;	//�����ļ���+1
			fileSystem.sfd[cur_SFD].sfdNum++;//�޸�
			fileSystem.sfd[cur_SFD].sfdVec.push_back(copySFD);//ֻ����sfd��Ϣ
			textcolor(10);
			return 1;
			//cout << "ճ���ɹ�\n";
		}
		else{//����
			int cur_inode = getInodeNum();
			if (fileSystem.iNode[cur_inode].id != checkUser(user)){
				textcolor(12);
				cout << "����ʧ�ܣ���û���ڸ�Ŀ¼�´����ļ���Ȩ��" << endl;
				return 0;
			}
			if (createFile(copySFD.name)){		//create_delete_file.cpp
				for (int i = 0; i < fileSystem.sfd[cur_SFD].sfdVec.size(); i++){
					if (copySFD.name == fileSystem.sfd[cur_SFD].sfdVec[i].name){
						pos = i;
						break;
					}
				}
				if (pos < 0){			//δ�ҵ�
					textcolor(12);
					cout << "����ʧ�ܣ�" << endl;
					return 0;
				}
				int inodeNo = fileSystem.sfd[cur_SFD].sfdVec[pos].id;     //�ҵ������ļ���i����
				int indexnum = fileSystem.iNode[inodeNo].diskBlockId;     //��ռ���̿��id�ŵ�������
				int *iaddr = getIaddr(indexnum);   //�õ��������д�ŵ�����

				int copy_indexnum = fileSystem.iNode[copySFD.id].diskBlockId; //�����ļ���ռ���̿��id�ŵ�������
				int *copy_iaddr = getIaddr(copy_indexnum);   //�õ��������д�ŵ�����
				
				//iaddr[0] = copy_iaddr[0];
				for (int i = 0; i < 10; ++i){	//�����ļ�����
					if (copy_iaddr[i] != -1){
						iaddr[i] = allocateOneBlock();
						fileSystem.iNode[inodeNo].diskBlockNum++;
						fileSystem.diskBlock[iaddr[i]].file = fileSystem.diskBlock[copy_iaddr[i]].file;
					}
				}
				writeiaddr(indexnum, iaddr);	//�������е���������д�ص���Ӧ���������У������ļ���	read_write.cpp
				
				return 1;

			}
		}
	}
	else{
		textcolor(12);
		cout << "ճ����Ϊ��\n";
		return 0;
	}
}




//���ļ������ض�Ӧ�ļ���i����±�  Ŀ¼�ж�Ӧ���ļ�
int openFile(string name){

	int pos = -1;
	for (int i = 0; i < fileSystem.sfd[cur_SFD].sfdVec.size(); i++){
		if (name == fileSystem.sfd[cur_SFD].sfdVec[i].name){
			pos = i;
			break;
		}
	}
	return pos;
}