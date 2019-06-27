#include "OS.h"

//将装满的栈内容写入一个特定磁盘块
void writeABlock(int r){

	for (int i = 0; i <= 50; i++){
		stringstream ss;
		string str;
		ss << fileSystem.superBlock.freeDiskSta[i]; //成组连接法空闲磁盘块栈
		ss >> str;
		fileSystem.diskBlock[r].file = "  " + fileSystem.diskBlock[r].file + "  " + str + "  ";
		fileSystem.diskBlock[r].strNum++;
	}
}



//将一个写着空闲磁盘块号的磁盘块内容写入栈 ----
void readABlock(int r){		
	int tmp = 0, f = -1, pos = 0;
	for (int i = 0; i < fileSystem.diskBlock[r].file.size(); i++){
		if (fileSystem.diskBlock[r].file[i] == ' ' && f == -1)
			continue;
		else if (fileSystem.diskBlock[r].file[i] == ' '){
			fileSystem.superBlock.freeDiskSta[pos++] = tmp;	//读取的是空格 即该块读取结束
			tmp = 0;
			f = -1;
		}
		else{
			tmp = tmp * 10 + fileSystem.diskBlock[r].file[i] - '0';	//因为读取是字符 所以需要不断的转换为整数
			f = 1;
		}
	}
}

//在成组链接中分配一个空闲磁盘块
int allocateOneBlock(){
	int r, mid;
	if (fileSystem.superBlock.freeDiskSta[0] == 1){
		if (fileSystem.superBlock.freeDiskSta[1] == -1){	//没有空的块
			textcolor(12);
			cout << "空闲磁盘块分配失败";
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


//在成组链接中回收一个空闲磁盘块
void freeABlock(int BlockNo){
	int r;
	diskBlock[BlockNo] = 1;       //修改位示图
	if (fileSystem.superBlock.freeDiskSta[0] == 50){
		writeABlock(BlockNo);
		fileSystem.superBlock.freeDiskSta[0] = 1;
		fileSystem.superBlock.freeDiskSta[1] = BlockNo;
	}
	else{
		fileSystem.superBlock.freeDiskSta[0]++;
		r = fileSystem.superBlock.freeDiskSta[0];
		fileSystem.superBlock.freeDiskSta[r] = BlockNo;
		SFDBlock[r] = 1;          //目录位示图
	}
	
	fileSystem.diskBlock[BlockNo].file = "";
	fileSystem.diskBlock[BlockNo].strNum = 0;


	fileSystem.superBlock.freeDisk++;
	//SFDBlock[r] = 1;          //目录位示图
}
