#include "OS.h"

//登录用户名
void login(){
	system("cls");
	textcolor(11);
	cout << "--------------------------------------------------------------------------------" << endl;
	cout << "-----------                 欢迎使用文件管理系统                    ------------" << endl;
	cout << "--------------------------------------------------------------------------------" << endl;
	while (1)
	{
		textcolor(13);
		cout << "\n请登录文件系统(输入exit退出文件系统)： ";
		cin >> user;	//输入全局变量user值
		//cout << user;
		if (user == "exit") {
			textcolor(9);
			cout << "\n退出成功，谢谢使用！\n";
			exit(0);
		}

		int res = checkUser(user);	//判断用户	函数在当前文件中	useri -- i(返回值）
		if (!res) {	//如果返回值为0，表明输入用户不存在。
			textcolor(12);
			cout << "\n不存在的用户，请重新输入！\n";
			continue;
		}

		cout << "\n请输入登录密码： ";
		//inputPassword();
		cin >> password;
		int password_res = checkPassword();
		if (!password_res) {
			textcolor(12);
			user = "";
			cout << "\n用户密码不对，请重新输入!";
		    
			getchar();
			//getchar();
			continue;
		}

		
		textcolor(10);
		cout << "\n登陆成功，回车进入文件系统" << endl;
		getchar();
		//getchar();
		break;
	}

}

//判断此时用户
int checkUser(string user){
	if (user == "user1")
		return 1;
	else if (user == "user2")
		return 2;
	else if (user == "user3")
		return 3;
	else if (user == "user4")
		return 4;
	else if (user == "user5")
		return 5;
	else if (user == "user6")
		return 6;
	else if (user == "user7")
		return 7;
	else if (user == "user8")
		return 8;
	else
		return 0;
}


//检查当前用户对文件的读写权限
int checkFileAuth(string filename){
	int pos = -1;
	//查找该文件在SFD的位置
	for (int i = 0; i < fileSystem.sfd[cur_SFD].sfdVec.size(); i++){
		if (filename == fileSystem.sfd[cur_SFD].sfdVec[i].name){
			pos = i;
			break;
		}
	}
	if (pos == -1)	//表明在该目录下不存在改文件
		return -2;

	int cur_inode = fileSystem.sfd[cur_SFD].sfdVec[pos].id;		//获得该文件的i节点id
	
	if (fileSystem.iNode[cur_inode].type == 1)        //1表示当前i结点是目录
		return -3;
	
	int use = checkUser(user) - 1;	//因为auth[8] 数组是从0 开始的
	
	return fileSystem.iNode[cur_inode].auth[use];	
}



//检查当前用户对目录的读写权限
int checkDirAuth(string filename){
	int pos = -1;
	for (int i = 0; i < fileSystem.sfd[cur_SFD].sfdVec.size(); i++){   //检查当前目录下是否存在
		if (filename == fileSystem.sfd[cur_SFD].sfdVec[i].name){
			pos = i;
			break;
		}
	}
	if (pos != -1)
		return -2;

	int cur_inode = getInodeNum();       //获得当前目录的i结点号
	int use = checkUser(user);				//获得当前用户id
	if (use == fileSystem.iNode[cur_inode].id)   //检查是否有权限
		return 1;
	return 0;
}

//登出
void logout(){
	exitSystem();	//exit中
	user = "";
}

//校验密码
int checkPassword() {
	freopen("password.txt", "r", stdin); 
	int userid = checkUser(user) - 1;
	if (userid == -1) {
		return 0;
	}
	for (int i = 0; i < 8; i++) {
		scanf("%s", passwords[i]);
	}
	bool flag = 0;
	int i;
	if (password.size() == 0) {
		return 0;
	}
	for (i = 0; i < password.size(); i++) {
		//cout << password.at(i) << endl;
		//cout << passwords[userid][i] << endl;
		if (password.at(i) != (passwords[userid][i] ^ 5)) {
			break;
		}
	}
	if (i == password.size()) {
		flag = 1;
	}


	fclose(stdin);
	freopen("CON", "r", stdin);
	return flag;
}


/*
void inputPassword() {
	char c;
	int count = 0;
	int size = 512;
	char *tmppassword = new char[512]; // 动态申请空间
	while ((c = getch()) != '\r') {

		if (c == 8) { // 退格
			if (count == 0) {
				continue;
			}
			putchar('\b'); // 回退一格
			putchar(' '); // 输出一个空格将原来的*隐藏
			putchar('\b'); // 再回退一格等待输入
			count--;
		}
		if (count == size - 1) { // 最大长度为size-1
			continue;
		}
		if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')) {  // 密码只可包含数字和字母
			putchar('*');  // 接收到一个字符后, 打印一个*
			tmppassword[count] = c;
			count++;
		}
	}
	tmppassword[count] = '\0';
	password = tmppassword;
	delete[] tmppassword; // 释放空间
	cout << endl;
}*/
