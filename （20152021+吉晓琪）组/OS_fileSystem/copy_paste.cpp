#include "OS.h"

//复制指定名字的目录或文件实现文件共享
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
		cout << "复制失败，不存在的文件名\n";
		return 0;
	}
	copySFD.name = fileSystem.sfd[cur_SFD].sfdVec[pos].name;
	copySFD.id = fileSystem.sfd[cur_SFD].sfdVec[pos].id;
	copy_flag = 1;
	return 1;
}

int cutContext(string filename){
	int flag = -1;
	vector<SingleSFD>::iterator iter = fileSystem.sfd[cur_SFD].sfdVec.begin(); //512个SFDblock
	for (int i = 0; i < fileSystem.sfd[cur_SFD].sfdVec.size(); i++){
		if (iter->name == filename){
			flag = 1;
			break;
		}
		iter++;
	}
	if (flag == -1){
		textcolor(12);
		cout << "剪切失败，不存在的文件名\n";
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

//粘贴粘贴板上的内容到当前目录下
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
			cout << "失败，当前目录下已经存在与粘贴板上同名的文件或目录！\n";
			return 0;
		}
		if (copy_flag == 2 || copy_flag == 3){//链接或剪切
			int id = copySFD.id;
			fileSystem.iNode[id].qcount++;	//引用文件数+1
			fileSystem.sfd[cur_SFD].sfdNum++;//修改
			fileSystem.sfd[cur_SFD].sfdVec.push_back(copySFD);//只保存sfd信息
			textcolor(10);
			return 1;
			//cout << "粘贴成功\n";
		}
		else{//拷贝
			int cur_inode = getInodeNum();
			if (fileSystem.iNode[cur_inode].id != checkUser(user)){
				textcolor(12);
				cout << "创建失败，您没有在该目录下创建文件的权限" << endl;
				return 0;
			}
			if (createFile(copySFD.name)){		//create_delete_file.cpp
				for (int i = 0; i < fileSystem.sfd[cur_SFD].sfdVec.size(); i++){
					if (copySFD.name == fileSystem.sfd[cur_SFD].sfdVec[i].name){
						pos = i;
						break;
					}
				}
				if (pos < 0){			//未找到
					textcolor(12);
					cout << "拷贝失败！" << endl;
					return 0;
				}
				int inodeNo = fileSystem.sfd[cur_SFD].sfdVec[pos].id;     //找到已有文件的i结点号
				int indexnum = fileSystem.iNode[inodeNo].diskBlockId;     //所占磁盘块的id号的索引块
				int *iaddr = getIaddr(indexnum);   //得到索引块中存放的内容

				int copy_indexnum = fileSystem.iNode[copySFD.id].diskBlockId; //复制文件所占磁盘块的id号的索引块
				int *copy_iaddr = getIaddr(copy_indexnum);   //得到索引块中存放的内容
				
				//iaddr[0] = copy_iaddr[0];
				for (int i = 0; i < 10; ++i){	//增加文件链接
					if (copy_iaddr[i] != -1){
						iaddr[i] = allocateOneBlock();
						fileSystem.iNode[inodeNo].diskBlockNum++;
						fileSystem.diskBlock[iaddr[i]].file = fileSystem.diskBlock[copy_iaddr[i]].file;
					}
				}
				writeiaddr(indexnum, iaddr);	//将数组中的索引内容写回到相应的索引块中，磁盘文件中	read_write.cpp
				
				return 1;

			}
		}
	}
	else{
		textcolor(12);
		cout << "粘贴板为空\n";
		return 0;
	}
}




//读文件，返回对应文件的i结点下标  目录中对应的文件
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