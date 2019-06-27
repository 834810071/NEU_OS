#include "OS.h"

//��װ����ջ����д��һ���ض����̿�
void writeABlock(int r){

	for (int i = 0; i <= 50; i++){
		stringstream ss;
		string str;
		ss << fileSystem.superBlock.freeDiskSta[i]; //�������ӷ����д��̿�ջ
		ss >> str;
		fileSystem.diskBlock[r].file = "  " + fileSystem.diskBlock[r].file + "  " + str + "  ";
		fileSystem.diskBlock[r].strNum++;
	}
}



//��һ��д�ſ��д��̿�ŵĴ��̿�����д��ջ ----
void readABlock(int r){		
	int tmp = 0, f = -1, pos = 0;
	for (int i = 0; i < fileSystem.diskBlock[r].file.size(); i++){
		if (fileSystem.diskBlock[r].file[i] == ' ' && f == -1)
			continue;
		else if (fileSystem.diskBlock[r].file[i] == ' '){
			fileSystem.superBlock.freeDiskSta[pos++] = tmp;	//��ȡ���ǿո� ���ÿ��ȡ����
			tmp = 0;
			f = -1;
		}
		else{
			tmp = tmp * 10 + fileSystem.diskBlock[r].file[i] - '0';	//��Ϊ��ȡ���ַ� ������Ҫ���ϵ�ת��Ϊ����
			f = 1;
		}
	}
}

//�ڳ��������з���һ�����д��̿�
int allocateOneBlock(){
	int r, mid;
	if (fileSystem.superBlock.freeDiskSta[0] == 1){
		if (fileSystem.superBlock.freeDiskSta[1] == -1){	//û�пյĿ�
			textcolor(12);
			cout << "���д��̿����ʧ��";
		}
		else{
			r = fileSystem.superBlock.freeDiskSta[1];
			readABlock(r);
			fileSystem.superBlock.freeDisk--;
			fileSystem.diskBlock[r].file = "";
			fileSystem.diskBlock[r].strNum = 0;
		}
	}
	else {
		mid = fileSystem.superBlock.freeDiskSta[0];
		r = fileSystem.superBlock.freeDiskSta[mid];
		fileSystem.superBlock.freeDiskSta[0]--;
		fileSystem.superBlock.freeDisk--;
		fileSystem.diskBlock[r].file = "";
		fileSystem.diskBlock[r].strNum = 0;
	}
	diskBlock[r] = 0;
	return r;
}


//�ڳ��������л���һ�����д��̿�
void freeABlock(int BlockNo){
	int r;
	diskBlock[BlockNo] = 1;       //�޸�λʾͼ
	if (fileSystem.superBlock.freeDiskSta[0] == 50){
		writeABlock(BlockNo);
		fileSystem.superBlock.freeDiskSta[0] = 1;
		fileSystem.superBlock.freeDiskSta[1] = BlockNo;
	}
	else{
		fileSystem.superBlock.freeDiskSta[0]++;
		r = fileSystem.superBlock.freeDiskSta[0];
		fileSystem.superBlock.freeDiskSta[r] = BlockNo;
		SFDBlock[r] = 1;          //Ŀ¼λʾͼ
	}
	
	fileSystem.diskBlock[BlockNo].file = "";
	fileSystem.diskBlock[BlockNo].strNum = 0;


	fileSystem.superBlock.freeDisk++;
	//SFDBlock[r] = 1;          //Ŀ¼λʾͼ
}
