#include "OS.h"

//���ݴ��̿�id�õ���Ӧ�ĵ�ַ
int *getIaddr(int indexnum) {
	string str[256];
	int *iaddr = new int[256];
	int flag = 0;
	for (int j = 0, i = 0; j < fileSystem.diskBlock[indexnum].file.size(); j++) {
		if (fileSystem.diskBlock[indexnum].file[j] == ' ' && flag == 0) {
			continue;
		}
		else if (fileSystem.diskBlock[indexnum].file[j] == ' ') {
			i++;
			flag = 0;
		}
		else {
			flag = 1;
			str[i] += fileSystem.diskBlock[indexnum].file[j];
		}
	}

	for (int i = 0; i < fileSystem.diskBlock[indexnum].strNum; i++) {
		iaddr[i] = atoi(str[i].c_str());   //atoi �ǽ��ַ���ת��Ϊint�ͣ�c_str()�ṩһ���ɶ����ɸ��ַ�����ָ��
	}
	return iaddr;
}


//���ص�ǰ�ļ���ռ�õ������̿�
int getCur_blockNo(int inodeNo) {
	int blocknum = fileSystem.iNode[inodeNo].diskBlockNum;  //�ļ�ռ�ô��̿����
	int indexnum = fileSystem.iNode[inodeNo].diskBlockId;   //��ռ���̿��id�ŵ�������
	if (blocknum == 1) {
		return -2;
	}
	else if (blocknum > 1) {
		int *iaddr = getIaddr(indexnum);   //�õ��������д�ŵ�����
		for (int i = 0; i < 10; i++) {
			if (iaddr[i] == -1)
				return iaddr[i - 1];    //���һ����-1����
		}
		for (int j = 10; j < 13; j++) {
			if (iaddr[j] == -1) {
				if (j == 10)
					return iaddr[j - 1];
				else {
					int *iaddr_f1 = getIaddr(iaddr[j - 1]);
					return iaddr_f1[127];           //0-9��ֱ�ӣ�10-12���
				}
			}
			else {
				int *iaddr_1 = getIaddr(iaddr[j]);//��������
				for (int i = 0; i < 128; i++) {
					if (iaddr_1[i] == -1)
						return iaddr_1[i - 1];
				}
			}
		}
	}
	return -1;
}




//�������е���������д�ص���Ӧ���������У������ļ���
void writeiaddr(int BlockNo, int *iaddr) {   //д������ļ��У����ļ���д�ļ�,Block�Ƕ�Ӧ��������Ŀ��
	fileSystem.diskBlock[BlockNo].file = "";
	for (int i = 0; i < 13; i++) {
		string str;
		stringstream ss;
		ss << iaddr[i];	//����ת�����ַ���
		ss >> str;
		fileSystem.diskBlock[BlockNo].file = " " + fileSystem.diskBlock[BlockNo].file + " " + str + " ";
	}
}





int writeIndexBlock(int indexnum, int BlockNo) {	//indexnum --- ���̿�id��	BlockNo --- ���뵽�Ĵ��̿���

	int *iaddr = getIaddr(indexnum);         //��������������
	for (int i = 0; i < 10; i++) {
		if (iaddr[i] == -1) {

			iaddr[i] = BlockNo;
			writeiaddr(indexnum, iaddr);
			delete[] iaddr;
			return BlockNo;                 //�������д��Ŀ��
		}
	}

	for (int j = 10; j < 13; j++) {					//10-13�Ǽ������
		if (iaddr[j] == -1) {                          //˵����һ����μ�ַ���������ģ�ֻ�ܴӵ�ǰ���ٽ��µĶ�������
			iaddr[j] = BlockNo;                      //BlockNo��Ҫ����������еĿ��
			int cur_B = allocateOneBlock();			//freeblock.cpp
			fileSystem.diskBlock[BlockNo].strNum = 128;		//���̿��СΪ128	����������
			int iaddr_all1[128];
			for (int i = 0; i < 128; i++) {
				iaddr_all1[i] = -1;
			}
			iaddr_all1[0] = cur_B;                  //iaddre_all1[0]�д�ŵ��Ƕ�����������
			writeiaddr(BlockNo, iaddr_all1);
			delete[] iaddr_all1;
			delete[] iaddr;
			return cur_B;                           //�������д��Ŀ��
		}
		else {                                 //����ֱ���ڶ�������������Ӧ�Ŀ�λд�뵱ǰ���
			int *iaddr_1 = getIaddr(iaddr[j]);
			for (int i = 0; i < 128; i++) {
				if (iaddr_1[i] == -1) {
					iaddr_1[i] = BlockNo;
					writeiaddr(iaddr[j], iaddr_1);
					return BlockNo;
				}
			}
			delete iaddr_1;
		}
	}
	delete[] iaddr;
	return -1;
}


//дָ���ļ������ļ�
int writeFile(string name) {          //Ӧ���������ļ�������ȥ���ļ�����Ӧ��i��㣬����ط�Ӧ���ǿ���ʵ�ֹ����
	int pos = checkExitsfd(name);      //��鵱ǰĿ¼����û�и��ļ�
	char c;
	char num[100];
	char s[1025];
	//string s = "";
	int a;	//�������� 1--�ļ����� 2--�ֶ�����
	int len2;
	int sindex;
	bool flagfile = false;
	printf("�ļ�����-----1\n");
	printf("�ֶ�����-----2\n");
	cin >> a;
	if (a == 2) {
		readFile(name);	//��ȡ�ļ������������̨
		cin >> s;
		len2 = strlen(s);
		if (len2 == 0)
		{
			textcolor(12);
			cout << "����Ϊ��\n";
			return 1;
		}
	}
	else {
		printf("�������ļ�����");
		cin >> num;
		char filename[100];
		sprintf(filename, "%s", num);	//num���ַ�������ʽ������filename��
		ifstream fin(filename);		//��Ӳ�̵��ڴ�
		fin.getline(s, 1024, '#');	//��#���Ƿָ���	���ļ����ݶ��뵽s��
		if (s[0] == '#') {
			textcolor(12);
			printf("�ļ�Ϊ��\n");
			return 1;
		}
		
		if (s[0] == '\0') {
			textcolor(12);
			flagfile = true;
			//cout << "�ļ�������\n";
			//return 1;
		}

		if (flagfile) {
			int pos = checkExitsfd(num);  //�鿴�ǵ�ǰĿ¼�µĵڼ����ļ�
			if (pos == -1) {
				textcolor(12);
				cout << "�ļ�������\n";
				//return 1;
			}
			else {
				sindex = 0;
				int inodeNo = fileSystem.sfd[cur_SFD].sfdVec[pos].id;   //�ļ���Ӧ��i�����
				int indexnum = fileSystem.iNode[inodeNo].diskBlockId;    //�ļ���Ӧ��������
				int *iaddr = getIaddr(indexnum);                      //��Ӧ���̿��е�����
				for (int i = 0; i < 10; i++) {
					if (iaddr[i] != -1) {
						string output;
						output = fileSystem.diskBlock[iaddr[i]].file;
						sindex = transform(sindex, s, output);

					}
					//iaddr[i]��ֱ�Ӵ���ļ����ݣ���ֱ�����
					else {
						//cout << endl; //�ļ��������
						//return 0;
						break;
					}
				}
				/*
				for (int j = 10; j < 13; j++) {
					if (iaddr[j] == -1)
						return;
					else {
						int *iaddr_1 = getIaddr(iaddr[j]);
						for (int i = 0; i < 128; i++) {
							if (iaddr_1[i] != -1)
								outputBlock(iaddr_1[i]);
							else
								return;
						}
						delete iaddr_1;
					}
				}
				delete[] iaddr;
				delete iaddr_1;*/
				delete[] iaddr;
			}
		}
	}
	int len;
	if (!flagfile) {
		len = strlen(s);
	}
	else {
		len = sindex;
	}
	//�ļ�������ʱ�� ���õ��ǳ�������
	int inodeNo = fileSystem.sfd[cur_SFD].sfdVec[pos].id;     //�ҵ������ļ���i����
	int indexnum = fileSystem.iNode[inodeNo].diskBlockId;     //��ռ���̿��id�ŵ�������
	int cur_blockNo = getCur_blockNo(inodeNo);                //���ص�ǰ�ļ�����ռ�����һ����̿�

	if (cur_blockNo == -1) {
		textcolor(12);
		cout << "д�ļ����ִ��̿�洢����\n";
		return 0;
	}
	else if (cur_blockNo == -2) {               //�ļ�����Ӧ�Ĵ��̿����Ϊ1������-2
		cur_blockNo = allocateOneBlock();	//freeblock.cpp
		fileSystem.iNode[inodeNo].diskBlockNum++;	//�ļ�ռ�õĴ��̿������1
		cur_blockNo = writeIndexBlock(indexnum, cur_blockNo);	//��ռ�õĴ��̿��д��������
	}
	int remainstr = len;	//ʣ���ַ����ĳ���
	int m = 512 - fileSystem.diskBlock[cur_blockNo].file.size();  //�ļ���Ӧ�����̿�    ʣ��ռ�
	int flag = 0, num1 = 0;
	for (int n = m; n > 0; n--) {	//�����Ĵ��̿�ʣ��ռ�����
		c = s[0];	//����������
		for (int ii = 1; ii <= len - num1; ii++) {	//ͳһ����
			s[ii - 1] = s[ii];                      //��s����������һλ
		}
		num1++;
		remainstr--;
		fileSystem.diskBlock[cur_blockNo].file += c;
		if (remainstr == 0)
			break;  //�Ѿ�ȫ��������
		if (c != ' ' && flag == 0) { 
			fileSystem.diskBlock[cur_blockNo].strNum++;	//���̿��С����
			flag = 1;
			continue;
		}
		else if (c != ' ')
			continue;
		else
			flag = 0;
	}
	if (remainstr != 0) {	//��������ַ�������ʣ��
		flag = 0;
		cur_blockNo = allocateOneBlock();	//��������һ�����̿�
		fileSystem.iNode[inodeNo].diskBlockNum++;
		cur_blockNo = writeIndexBlock(indexnum, cur_blockNo);
		for (int k = 0, j = 0; k < remainstr; k++, j++) {
			if (j == 512) {                  //��ǰ���е����һ���ֽ�
				k--;
				j = 0;
				cur_blockNo = allocateOneBlock();
				fileSystem.iNode[inodeNo].diskBlockNum++;
				cur_blockNo = writeIndexBlock(indexnum, cur_blockNo);	//ÿ����һ�����̿� ��Ҫд����Ӧ��������
				continue;
			}
			else {
				c = s[0];
				for (int ii = 1; ii <= len - num1; ii++) {
					s[ii - 1] = s[ii];
				}
				num1++;
				fileSystem.diskBlock[cur_blockNo].file += c;
				if (c != ' ' && flag == 0) {
					fileSystem.diskBlock[cur_blockNo].strNum++;
					flag = 1;
					continue;
				}
				else if (c != ' ')
					continue;
				else
					flag = 0;
			}
		}
	}
	textcolor(10);
	cout << "д�ļ��ɹ���\n";
}

//����ļ����̿�����
void outputBlock(int blockNO) {
	string output;
	output = fileSystem.diskBlock[blockNO].file;
	textcolor(15);
	cout << output;
}

//���ļ����ݺ���
void readFile(string name) {

	int pos = checkExitsfd(name);  //�鿴�ǵ�ǰĿ¼�µĵڼ����ļ�	create_delete_file.cpp
	if (pos == -1) {
		textcolor(12);
		cout << "�ļ�������\n";
	}
	else {
		int inodeNo = fileSystem.sfd[cur_SFD].sfdVec[pos].id;   //�ļ���Ӧ��i�����
		int indexnum = fileSystem.iNode[inodeNo].diskBlockId;    //�ļ���Ӧ��������
		int *iaddr = getIaddr(indexnum);                      //��Ӧ���̿��е�����
		for (int i = 0; i < 10; i++) {
			if (iaddr[i] != -1)
				outputBlock(iaddr[i]);  //iaddr[i]��ֱ�Ӵ���ļ����ݣ���ֱ�����
			else {
				//cout << endl; //�ļ��������
				return;
			}
		}

		for (int j = 10; j < 13; j++) {	//�ж϶��������Ƿ�ʹ��
			if (iaddr[j] == -1)
				return;
			else {
				int *iaddr_1 = getIaddr(iaddr[j]);
				for (int i = 0; i < 128; i++) {	//��ѯ���i�ڵ��λʸͼ ����������
					if (iaddr_1[i] != -1)
						outputBlock(iaddr_1[i]);
					else
						return;
				}
				delete iaddr_1;
			}
		}
		delete[] iaddr;
	}
}



int transform(int sindex, char s[], string file) {
	int index = sindex;

	for (int i = 0; i < file.size(); i++) {
		s[index] = file[i];
		index++;
	}
	return index;
}