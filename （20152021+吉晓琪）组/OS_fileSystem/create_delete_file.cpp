#include "OS.h"

//创建文件的第一个索引块
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


//为新创建的文件分配一个i结点
int createiNode(){

	int EBcount, inodeNo, indexnum;
	EBcount = fileSystem.superBlock.freeDisk;  //空闲磁盘块数
	if (EBcount == 0){
		textcolor(12);
		cout << "无空闲磁盘块\n";
		return -1;
	}
	else if (fileSystem.superBlock.freei_node == 0){   //有没有空闲i结点
		textcolor(12);
		cout << "无空闲i结点\n";
		return -1;
	}
	else{

		indexnum = createFirstIndexB();  //创建第一个索引块
		fileSystem.superBlock.freei_node--;
		inodeNo = fileSystem.superBlock.freeiid[0];   //空闲i结点数组
		vector<int>::iterator iter = fileSystem.superBlock.freeiid.begin();
		iter = fileSystem.superBlock.freeiid.erase(iter);
		iNode[inodeNo] = 0;

		createInitINode(inodeNo, 0, 0);    //为创建文件或目录初始化i结点，类型为0，表示文件，长度为0，初始化i结点

		int master = checkUser(user);
		for (int i = 0; i < 8; i++)       //只有当前用户可以有权限去创建，初始化权限
			fileSystem.iNode[inodeNo].auth[i] = 0;
		fileSystem.iNode[inodeNo].auth[master - 1] = 1;
		fileSystem.iNode[inodeNo].diskBlockNum = 1;
		fileSystem.iNode[inodeNo].diskBlockId = indexnum;

		return inodeNo;
	}
	return -1;

}


//查询当前目录下一固定名的文件下标
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

//创建文件
int createFile(string name){

	SingleSFD tempSFD;
	int pos = checkExitsfd(name);
	if (pos != -1){
		textcolor(12);
		cout << "文件名冲突！\n";
		return 0;
	}
	else{
		int inodeNo = createiNode();
		if (inodeNo == -1){
			textcolor(12);
			cout << "空闲i结点不足，创建文件失败！\n";
			return 0;
		}
		else {
			cout << fileSystem.iNode[22].diskBlockNum << endl;
			tempSFD.id = inodeNo;
			tempSFD.name = name;
			fileSystem.sfd[cur_SFD].sfdVec.push_back(tempSFD);
			fileSystem.sfd[cur_SFD].sfdNum++;
			textcolor(10);
			cout << "创建成功！\n";
		}
	}
	return 1;
}


//删除待删除文件对应的i结点及其指向的磁盘块
void deleteINode(int pos){	//因为i节点最终是指向磁盘块的 所以最终的目的是删除i节点对应的磁盘块

	int inodeNo = fileSystem.sfd[cur_SFD].sfdVec[pos].id;
	int indexnum = fileSystem.iNode[inodeNo].diskBlockId;  //所占磁盘块的id号的索引块
	int *iaddr = getIaddr(indexnum);	//read_write.cpp中
	for (int i = 0; i < 10; i++){	//10
		if (iaddr[i] != -1)	//使用 单级索引
			freeABlock(iaddr[i]);	//freeblock.cpp 成组链回收
	}
	for (int i = 10; i < 13; i++){	
		if (iaddr[i] == -1)	//未使用	多级索引
			break;
		int *iaddr_1 = getIaddr(iaddr[i]);
		for (int j = 0; j < fileSystem.diskBlock[iaddr[i]].strNum; j++){
			if (iaddr_1[j] != -1)
				freeABlock(iaddr_1[j]);
			else
				break;
		}
		freeABlock(iaddr[i]);   //回收索引块所对应的文件内容	成组链
	}
	freeABlock(indexnum);       //回收索引块
	iNode[inodeNo] = 1;         //1表示空闲	修改位势图
	fileSystem.superBlock.freei_node++;  //回收i结点
	fileSystem.superBlock.freeiid.push_back(inodeNo);
}


//遍历删除与待删除文件共享的文件目录
void findSinglesfd(int inodeNo){

	int flag = 0;
	for (int k = 0; k < 512; k++){
		vector<SingleSFD>::iterator iter = fileSystem.sfd[k].sfdVec.begin(); //遍历512个SFDblock
		for (int i = 0; i < fileSystem.sfd[k].sfdVec.size(); i++){	//因为同一目录下不能同名	所以找到1个就退出
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


//删除指定名字的文件
int freeFile(string name){
	int pos = -1;
	int master;
	vector<SingleSFD>::iterator iter = fileSystem.sfd[cur_SFD].sfdVec.begin();
	for (int i = 0; i < fileSystem.sfd[cur_SFD].sfdVec.size(); i++){	//查找
		if (fileSystem.sfd[cur_SFD].sfdVec[i].name == name){
			pos = i;
			break;
		}
		iter++;
	}
	if (pos == -1){
		textcolor(12);
		cout << "删除失败，不存在的文件名！\n";
		return 0;
	}
	else{
		int inodeNo = fileSystem.sfd[cur_SFD].sfdVec[pos].id;	//如果是文件的话 则根据目录 查找到对应的i节点
		if ((master = fileSystem.iNode[inodeNo].id) == checkUser(user)){ //i结点所对应的用户
			if (fileSystem.iNode[inodeNo].qcount > 1){	//如果文件被引用
				int select;
				cout << "1.仅删除该链接  2.彻底删除该文件" << endl;
				cout << "请选择：";
				while (1){
					cin >> select;
					if (select == 1){
						iter = fileSystem.sfd[cur_SFD].sfdVec.erase(iter);    //回收sdf
						fileSystem.sfd[cur_SFD].sfdNum--;
						break;
					}
					else if (select == 2){
						findSinglesfd(inodeNo);                               //将级联删除
						deleteINode(pos);
						iter = fileSystem.sfd[cur_SFD].sfdVec.erase(iter);    //回收sdf
						fileSystem.sfd[cur_SFD].sfdNum--;
						break;
					}
					else {
						cout << "输入有误，请重新输入：";
					}
				}
			}
			else{
				deleteINode(pos);
				iter = fileSystem.sfd[cur_SFD].sfdVec.erase(iter);    //回收sdf
				fileSystem.sfd[cur_SFD].sfdNum--;
			}
			
		}
	}
	return 1;
}