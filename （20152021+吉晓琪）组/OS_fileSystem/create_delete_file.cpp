#include "OS.h"

//�����ļ��ĵ�һ��������
int createFirstIndexB(){

	int num, iaddr[13] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };

	num = allocateOneBlock();
	fileSystem.diskBlock[num].strNum = 13;
	for (int i = 0; i < 13; i++){
		stringstream ss;
		string tmp;
		ss << iaddr[i];
		ss >> tmp;
		fileSystem.diskBlock[num].file = "  " + fileSystem.diskBlock[num].file + "  " + tmp + "  ";
	}
	return num;
}


//Ϊ�´������ļ�����һ��i���
int createiNode(){

	int EBcount, inodeNo, indexnum;
	EBcount = fileSystem.superBlock.freeDisk;  //���д��̿���
	if (EBcount == 0){
		textcolor(12);
		cout << "�޿��д��̿�\n";
		return -1;
	}
	else if (fileSystem.superBlock.freei_node == 0){   //��û�п���i���
		textcolor(12);
		cout << "�޿���i���\n";
		return -1;
	}
	else{

		indexnum = createFirstIndexB();  //������һ��������
		fileSystem.superBlock.freei_node--;
		inodeNo = fileSystem.superBlock.freeiid[0];   //����i�������
		vector<int>::iterator iter = fileSystem.superBlock.freeiid.begin();
		iter = fileSystem.superBlock.freeiid.erase(iter);
		iNode[inodeNo] = 0;

		createInitINode(inodeNo, 0, 0);    //Ϊ�����ļ���Ŀ¼��ʼ��i��㣬����Ϊ0����ʾ�ļ�������Ϊ0����ʼ��i���

		int master = checkUser(user);
		for (int i = 0; i < 8; i++)       //ֻ�е�ǰ�û�������Ȩ��ȥ��������ʼ��Ȩ��
			fileSystem.iNode[inodeNo].auth[i] = 0;
		fileSystem.iNode[inodeNo].auth[master - 1] = 1;
		fileSystem.iNode[inodeNo].diskBlockNum = 1;
		fileSystem.iNode[inodeNo].diskBlockId = indexnum;

		return inodeNo;
	}
	return -1;

}


//��ѯ��ǰĿ¼��һ�̶������ļ��±�
int checkExitsfd(string name){
	int pos = -1;
	for (int i = 0; i < fileSystem.sfd[cur_SFD].sfdVec.size(); i++){
		if (fileSystem.sfd[cur_SFD].sfdVec[i].name == name){
			pos = i;
			break;
		}
	}
	return pos;
}

//�����ļ�
int createFile(string name){

	SingleSFD tempSFD;
	int pos = checkExitsfd(name);
	if (pos != -1){
		textcolor(12);
		cout << "�ļ�����ͻ��\n";
		return 0;
	}
	else{
		int inodeNo = createiNode();
		if (inodeNo == -1){
			textcolor(12);
			cout << "����i��㲻�㣬�����ļ�ʧ�ܣ�\n";
			return 0;
		}
		else {
			cout << fileSystem.iNode[22].diskBlockNum << endl;
			tempSFD.id = inodeNo;
			tempSFD.name = name;
			fileSystem.sfd[cur_SFD].sfdVec.push_back(tempSFD);
			fileSystem.sfd[cur_SFD].sfdNum++;
			textcolor(10);
			cout << "�����ɹ���\n";
		}
	}
	return 1;
}


//ɾ����ɾ���ļ���Ӧ��i��㼰��ָ��Ĵ��̿�
void deleteINode(int pos){	//��Ϊi�ڵ�������ָ����̿�� �������յ�Ŀ����ɾ��i�ڵ��Ӧ�Ĵ��̿�

	int inodeNo = fileSystem.sfd[cur_SFD].sfdVec[pos].id;
	int indexnum = fileSystem.iNode[inodeNo].diskBlockId;  //��ռ���̿��id�ŵ�������
	int *iaddr = getIaddr(indexnum);	//read_write.cpp��
	for (int i = 0; i < 10; i++){	//10
		if (iaddr[i] != -1)	//ʹ�� ��������
			freeABlock(iaddr[i]);	//freeblock.cpp ����������
	}
	for (int i = 10; i < 13; i++){	
		if (iaddr[i] == -1)	//δʹ��	�༶����
			break;
		int *iaddr_1 = getIaddr(iaddr[i]);
		for (int j = 0; j < fileSystem.diskBlock[iaddr[i]].strNum; j++){
			if (iaddr_1[j] != -1)
				freeABlock(iaddr_1[j]);
			else
				break;
		}
		freeABlock(iaddr[i]);   //��������������Ӧ���ļ�����	������
	}
	freeABlock(indexnum);       //����������
	iNode[inodeNo] = 1;         //1��ʾ����	�޸�λ��ͼ
	fileSystem.superBlock.freei_node++;  //����i���
	fileSystem.superBlock.freeiid.push_back(inodeNo);
}


//����ɾ�����ɾ���ļ�������ļ�Ŀ¼
void findSinglesfd(int inodeNo){

	int flag = 0;
	for (int k = 0; k < 512; k++){
		vector<SingleSFD>::iterator iter = fileSystem.sfd[k].sfdVec.begin(); //����512��SFDblock
		for (int i = 0; i < fileSystem.sfd[k].sfdVec.size(); i++){	//��ΪͬһĿ¼�²���ͬ��	�����ҵ�1�����˳�
			if (fileSystem.sfd[k].sfdVec[i].id == inodeNo){
				flag = 1;
				break;
			}
			iter++;
		}
		if (flag == 1 && k != cur_SFD){
			iter = fileSystem.sfd[k].sfdVec.erase(iter);
			fileSystem.sfd[k].sfdNum--;
		}
		flag = 0;
	}
}


//ɾ��ָ�����ֵ��ļ�
int freeFile(string name){
	int pos = -1;
	int master;
	vector<SingleSFD>::iterator iter = fileSystem.sfd[cur_SFD].sfdVec.begin();
	for (int i = 0; i < fileSystem.sfd[cur_SFD].sfdVec.size(); i++){	//����
		if (fileSystem.sfd[cur_SFD].sfdVec[i].name == name){
			pos = i;
			break;
		}
		iter++;
	}
	if (pos == -1){
		textcolor(12);
		cout << "ɾ��ʧ�ܣ������ڵ��ļ�����\n";
		return 0;
	}
	else{
		int inodeNo = fileSystem.sfd[cur_SFD].sfdVec[pos].id;	//������ļ��Ļ� �����Ŀ¼ ���ҵ���Ӧ��i�ڵ�
		if ((master = fileSystem.iNode[inodeNo].id) == checkUser(user)){ //i�������Ӧ���û�
			if (fileSystem.iNode[inodeNo].qcount > 1){	//����ļ�������
				int select;
				cout << "1.��ɾ��������  2.����ɾ�����ļ�" << endl;
				cout << "��ѡ��";
				while (1){
					cin >> select;
					if (select == 1){
						iter = fileSystem.sfd[cur_SFD].sfdVec.erase(iter);    //����sdf
						fileSystem.sfd[cur_SFD].sfdNum--;
						break;
					}
					else if (select == 2){
						findSinglesfd(inodeNo);                               //������ɾ��
						deleteINode(pos);
						iter = fileSystem.sfd[cur_SFD].sfdVec.erase(iter);    //����sdf
						fileSystem.sfd[cur_SFD].sfdNum--;
						break;
					}
					else {
						cout << "�����������������룺";
					}
				}
			}
			else{
				deleteINode(pos);
				iter = fileSystem.sfd[cur_SFD].sfdVec.erase(iter);    //����sdf
				fileSystem.sfd[cur_SFD].sfdNum--;
			}
			
		}
	}
	return 1;
}