#include "OS.h"

//Ϊ�����ļ���Ŀ¼��ʼ��i���
void createInitINode(int useINode, int type, int filelen){

	//�½�Ŀ¼���ļ�ʱ��ʼ��i���
	fileSystem.iNode[useINode].id = checkUser(user);  //i��������û�
	fileSystem.iNode[useINode].type = type;
	fileSystem.iNode[useINode].filelen = filelen;
	fileSystem.iNode[useINode].qcount = 1;
	iNode[useINode] = 0;	//�޸�λʸͼ
	if (type == 1){                                   //1��ʾĿ¼
		int useSFD = fileSystem.superBlock.freeSFD[0];	//ȡһ������Ŀ¼
		vector<int>::iterator iter = fileSystem.superBlock.freeSFD.begin();
		iter = fileSystem.superBlock.freeSFD.erase(iter);  //�ӳ�����freeSFD �в���
		fileSystem.iNode[useINode].sfd_id = useSFD;
		SFDBlock[useSFD] = 0;	//�޸�Ŀ¼λ��ͼ
		fileSystem.superBlock.free_SFD--;
	}
	else{
		fileSystem.iNode[useINode].sfd_id = -1;   //i���sfd_id=-1��ʾ����д�ŵ����ļ�����
	}
}

//����һ��Ŀ¼
int createDir(string filename){
	//����һ��Ŀ¼
	SingleSFD tempSFD;
	if (fileSystem.superBlock.freei_node == 0 || fileSystem.superBlock.free_SFD == 0){
		textcolor(12);
		cout << "i����Ŀ¼�ռ䲻�㣬����Ŀ¼ʧ�ܣ�\n";
		return 0;
	}

	//��ʼ��Ŀ¼
	tempSFD.name = filename;
	int useINode = fileSystem.superBlock.freeiid[0];	//ʹ�õ�i�ڵ�	����������
	vector<int>::iterator iter = fileSystem.superBlock.freeiid.begin();      //�������i���
	iter = fileSystem.superBlock.freeiid.erase(iter);
	fileSystem.superBlock.freei_node--;
	iNode[useINode] = 0;          //�޸�λʾͼΪռ��״̬	��ʾռ��0
	tempSFD.id = useINode;        //�޸�sfd��Ӧ��i���
	fileSystem.sfd[cur_SFD].sfdVec.push_back(tempSFD);
	fileSystem.sfd[cur_SFD].sfdNum++;
	//ͬ��i�����Ϣ
	createInitINode(useINode, 1, 0);    //���ﴴ�����ļ�����Ϊ0
	return 1;
}

//����ɾ��һ��Ŀ¼������Ŀ¼�����ļ�
int deleteDir(string name){
	//����ɾ��ָ�����ֵ�Ŀ¼������Ŀ¼���ļ�
	int pos = -1;
	vector<SingleSFD>::iterator iter = fileSystem.sfd[cur_SFD].sfdVec.begin();
	for (int i = 0; i < fileSystem.sfd[cur_SFD].sfdVec.size(); i++){
		if (fileSystem.sfd[cur_SFD].sfdVec[i].name == name){	//�ҵ�Ҫɾ����Ŀ¼���ڵ�λ��
			pos = i;
			break;
		}
		iter++;
	}
	if (pos == -1){	//��ֹ������Ŀ¼��
		textcolor(12);
		cout << "ɾ��ʧ�ܣ������ڵ�Ŀ¼����\n";
		return 0;
	}

	int useINode = fileSystem.sfd[cur_SFD].sfdVec[pos].id; //useINODe Ϊ��Ҫɾ��Ŀ¼����Ӧ��i���
	//���м���ɾ��
	deleteINodeOne(useINode);         //ɾ��i���
	fileSystem.superBlock.freeiid.push_back(useINode);  //���ͷŵ�i�����뵽���п�
	iNode[useINode] = 1;                                //�޸�λʾͼ
	fileSystem.superBlock.freei_node++;
	iter = fileSystem.sfd[cur_SFD].sfdVec.erase(iter);
	fileSystem.sfd[cur_SFD].sfdNum--;
}


//ɾ��һ��i���
void deleteINodeOne(int useINode){
	//����ɾ��i��㼰���¼�Ŀ¼
	int preSFD = cur_SFD;                              //��ǰĿ¼	�洢 �Ա����ʱ����
	int useSFD = fileSystem.iNode[useINode].sfd_id;     //i����Ӧ��Ŀ¼id����Ӧ�ļ�ʱΪ0
	cur_SFD = useSFD;                                   //��һ��Ŀ¼
	while (fileSystem.sfd[cur_SFD].sfdVec.size() != 0){
		int nextInode = fileSystem.sfd[cur_SFD].sfdVec[0].id;
		if (fileSystem.iNode[nextInode].type == 0)
			freeFile(fileSystem.sfd[cur_SFD].sfdVec[0].name);   //���ͱ�ʾ��0��ʾ�ļ�
		else
			deleteDir(fileSystem.sfd[cur_SFD].sfdVec[0].name);  //1��ʾĿ¼
	}
	fileSystem.sfd[cur_SFD].sfdVec.clear();	//Ŀ¼�ṹ���
	fileSystem.sfd[cur_SFD].sfdNum = 0;      //�����С
	fileSystem.superBlock.free_SFD++;
	fileSystem.superBlock.freeSFD.push_back(cur_SFD);
	SFDBlock[cur_SFD] = 1;	//�޸�λʸͼ
	cur_SFD = preSFD;       //������һ��Ŀ¼��
}


