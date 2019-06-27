#include "OS.h"
//用户的格式化
void formatUser(){
	//cout << fileSystem.sfd[1].sfdVec[0].id << endl;	//i节点的id号
	//登录时可选格式化所登录用户文件夹下的所有文件
	for (int i = 0; i < fileSystem.sfd[cur_SFD].sfdVec.size();){
		//cout<< fileSystem.sfd[cur_SFD].sfdVec.size();
		int inodeNo = fileSystem.sfd[cur_SFD].sfdVec[i].id;		//获得当前目录下的i节点id
		int type = fileSystem.iNode[inodeNo].type;
		if (type == 1) deleteDir(fileSystem.sfd[cur_SFD].sfdVec[i].name);	//删除目录	create_delete_dir.cpp
		else freeFile(fileSystem.sfd[cur_SFD].sfdVec[i].name);	//释放文件	create_delete_file.cpp
	}
}