#include "OS.h"

//�˳�ǰ����SFD����
void exitSFD(){

	freopen("sfdContext.txt", "w", stdout);

	for (int i = 1; i < 512; i++){
		if (!SFDBlock[i]){	//�����Ŀ¼��ռ��
			printf("  %d  ", fileSystem.sfd[i].sfdNum);      //sdf ����
			//cout<<"  "<<fileSystem.sfd[i].sfdNum<<"  ";
			for (int j = 0; j < fileSystem.sfd[i].sfdNum; j++){
				cout << "  " << fileSystem.sfd[i].sfdVec[j].name << "  ";   //ÿһ��Ŀ¼������
				printf("  %d  ", fileSystem.sfd[i].sfdVec[j].id);     //����Ӧ��i�����
				//cout<<fileSystem.sfd[i].sfdVec[j].name<<"  "<<fileSystem.sfd[i].sfdVec[j].id<<"  ";
			}
		}
	}

	fclose(stdout);
	freopen("CON", "w", stdout);
}

//�˳�ǰ������̿�����
void exitDiskBlock(){

	//д�ش��̿�����
	freopen("diskBlock.txt", "w", stdout);
	for (int i = 0; i < 512; i++){
		if (!diskBlock[i]){	//������̿鱻ռ��
			printf("  %d  ", fileSystem.diskBlock[i].strNum);  //�ļ�ռ�ô��̿����
			//cout<<fileSystem.diskBlock[i].strNum;
			string str;
			for (int j = 0; j < fileSystem.diskBlock[i].file.size(); j++){
				if (fileSystem.diskBlock[i].file[j] == ' ' && str.size() == 0)
					continue;
				else if (fileSystem.diskBlock[i].file[j] == ' '){
					cout << " " << str << " ";  //�ļ�����
					str.clear();	//ÿ����һ���ļ�����  ���
				}
				else{
					str += fileSystem.diskBlock[i].file[j];
				}
			}
			cout << " " << str << " ";
		}
	}

	fclose(stdout);
	freopen("CON", "w", stdout);
}

//�˳�ǰ����i�ڵ�����
void exitINode(){

	//д��i�������
	freopen("iNodeContext.txt", "w", stdout);
	for (int i = 9; i < 128; i++){
		if (!iNode[i]){
			//
			printf("  %d  %d  %d  %d  %d", fileSystem.iNode[i].id, fileSystem.iNode[i].type, fileSystem.iNode[i].sfd_id, fileSystem.iNode[i].filelen, fileSystem.iNode[i].qcount);
			//cout<<"  "<<fileSystem.iNode[i].id<<"  "<<fileSystem.iNode[i].type<<"  "<<fileSystem.iNode[i].sfd_id<<"  "<<fileSystem.iNode[i].filelen<<"  "<<fileSystem.iNode[i].qcount<<"  ";
			for (int j = 0; j < 8; j++)
				printf("  %d  ", fileSystem.iNode[i].auth[j]);
			//cout<<"  "<<fileSystem.iNode[i].auth[j]<<"  ";
			if (fileSystem.iNode[i].type == 0){
				printf("  %d  %d  ", fileSystem.iNode[i].diskBlockNum, fileSystem.iNode[i].diskBlockId);
				//cout<<"  "<<fileSystem.iNode[i].diskBlockNum<<"  ";
				//cout<<"  "<<fileSystem.iNode[i].diskBlockId<<"  ";
			}
		}
	}

	fclose(stdout);
	freopen("CON", "w", stdout);
}


//�˳�ǰ���泬��������
void exitSuperBlock(){

	//д�س���������
	freopen("superBlock.txt", "w", stdout);
	printf("  %d  %d  ", fileSystem.superBlock.i_node, fileSystem.superBlock.freei_node);	//i�ڵ�
	//cout<<"  "<<fileSystem.superBlock.i_node<<"  "<<fileSystem.superBlock.freei_node;
	for (int i = 0; i < fileSystem.superBlock.freei_node; i++){	//���п�idд���ļ���
		printf("  %d  ", fileSystem.superBlock.freeiid[i]);
		//cout<<"  "<<fileSystem.superBlock.freeiid[i]<<"  ";
	}

	printf("  %d  %d  ", fileSystem.superBlock.sfd, fileSystem.superBlock.free_SFD);	//Ŀ¼
	//cout<<"  "<<fileSystem.superBlock.sfd<<"  "<<fileSystem.superBlock.free_SFD<<"  ";
	for (int i = 0; i < fileSystem.superBlock.free_SFD; i++){
		printf("  %d  ", fileSystem.superBlock.freeSFD[i]);
		//cout<<"  "<<fileSystem.superBlock.freeSFD[i]<<"  ";
	}
	printf("  %d  %d  ", fileSystem.superBlock.disk, fileSystem.superBlock.freeDisk);
	//cout<<"  "<<fileSystem.superBlock.disk<<"  "<<fileSystem.superBlock.freeDisk<<"  ";

	fclose(stdout);
	freopen("CON", "w", stdout);

	freopen("freeBlock.txt", "w", stdout);	//���п�	������
	for (int i = 0; i < 512; i++){
		if (diskBlock[i] == 1){
			printf("  %d  ", i);
			//cout<<" "<<i<<" ";
		}
	}

	fclose(stdout);
	freopen("CON", "w", stdout);
}

//�˳�ϵͳ
void exitSystem(){

	exitSFD();
	exitDiskBlock();
	exitINode();
	exitSuperBlock();
}