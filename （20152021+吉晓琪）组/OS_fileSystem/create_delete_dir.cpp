#include "OS.h"

//为创建文件或目录初始化i结点
void createInitINode(int useINode, int type, int filelen){

	//新建目录或文件时初始化i结点
	fileSystem.iNode[useINode].id = checkUser(user);  //i结点所属用户
	fileSystem.iNode[useINode].type = type;
	fileSystem.iNode[useINode].filelen = filelen;
	fileSystem.iNode[useINode].qcount = 1;
	iNode[useINode] = 0;	//修改位矢图
	if (type == 1){                                   //1表示目录
		int useSFD = fileSystem.superBlock.freeSFD[0];	//取一个空闲目录
		vector<int>::iterator iter = fileSystem.superBlock.freeSFD.begin();
		iter = fileSystem.superBlock.freeSFD.erase(iter);  //从超级块freeSFD 中擦除
		fileSystem.iNode[useINode].sfd_id = useSFD;
		SFDBlock[useSFD] = 0;	//修改目录位势图
		fileSystem.superBlock.free_SFD--;
	}
	else{
		fileSystem.iNode[useINode].sfd_id = -1;   //i结点sfd_id=-1表示结点中存放的是文件内容
	}
}

//创建一个目录
int createDir(string filename){
	//创建一个目录
	SingleSFD tempSFD;
	if (fileSystem.superBlock.freei_node == 0 || fileSystem.superBlock.free_SFD == 0){
		textcolor(12);
		cout << "i结点或目录空间不足，创建目录失败！\n";
		return 0;
	}

	//初始化目录
	tempSFD.name = filename;
	int useINode = fileSystem.superBlock.freeiid[0];	//使用的i节点	？？？？？
	vector<int>::iterator iter = fileSystem.superBlock.freeiid.begin();      //分配空闲i结点
	iter = fileSystem.superBlock.freeiid.erase(iter);
	fileSystem.superBlock.freei_node--;
	iNode[useINode] = 0;          //修改位示图为占用状态	表示占用0
	tempSFD.id = useINode;        //修改sfd对应的i结点
	fileSystem.sfd[cur_SFD].sfdVec.push_back(tempSFD);
	fileSystem.sfd[cur_SFD].sfdNum++;
	//同步i结点信息
	createInitINode(useINode, 1, 0);    //这里创建的文件长度为0
	return 1;
}

//级联删除一个目录及其子目录和子文件
int deleteDir(string name){
	//级联删除指定名字的目录及其子目录子文件
	int pos = -1;
	vector<SingleSFD>::iterator iter = fileSystem.sfd[cur_SFD].sfdVec.begin();
	for (int i = 0; i < fileSystem.sfd[cur_SFD].sfdVec.size(); i++){
		if (fileSystem.sfd[cur_SFD].sfdVec[i].name == name){	//找到要删除的目录所在的位置
			pos = i;
			break;
		}
		iter++;
	}
	if (pos == -1){	//防止不存在目录名
		textcolor(12);
		cout << "删除失败，不存在的目录名！\n";
		return 0;
	}

	int useINode = fileSystem.sfd[cur_SFD].sfdVec[pos].id; //useINODe 为将要删除目录所对应的i结点
	//进行级联删除
	deleteINodeOne(useINode);         //删除i结点
	fileSystem.superBlock.freeiid.push_back(useINode);  //把释放的i结点放入到空闲块
	iNode[useINode] = 1;                                //修改位示图
	fileSystem.superBlock.freei_node++;
	iter = fileSystem.sfd[cur_SFD].sfdVec.erase(iter);
	fileSystem.sfd[cur_SFD].sfdNum--;
}


//删除一个i结点
void deleteINodeOne(int useINode){
	//级联删除i结点及其下级目录
	int preSFD = cur_SFD;                              //当前目录	存储 以便结束时返回
	int useSFD = fileSystem.iNode[useINode].sfd_id;     //i结点对应的目录id，对应文件时为0
	cur_SFD = useSFD;                                   //下一级目录
	while (fileSystem.sfd[cur_SFD].sfdVec.size() != 0){
		int nextInode = fileSystem.sfd[cur_SFD].sfdVec[0].id;
		if (fileSystem.iNode[nextInode].type == 0)
			freeFile(fileSystem.sfd[cur_SFD].sfdVec[0].name);   //类型表示，0表示文件
		else
			deleteDir(fileSystem.sfd[cur_SFD].sfdVec[0].name);  //1表示目录
	}
	fileSystem.sfd[cur_SFD].sfdVec.clear();	//目录结构清空
	fileSystem.sfd[cur_SFD].sfdNum = 0;      //数组大小
	fileSystem.superBlock.free_SFD++;
	fileSystem.superBlock.freeSFD.push_back(cur_SFD);
	SFDBlock[cur_SFD] = 1;	//修改位矢图
	cur_SFD = preSFD;       //返回上一级目录中
}


