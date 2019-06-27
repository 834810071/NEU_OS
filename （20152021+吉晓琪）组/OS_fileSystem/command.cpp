#include "OS.h"
#define com_num  24
int flag = 1;
string com[com_num] = { "link", "cd", "create", "rname", "mkdir", "find", "write", "read", "deld", "delf", "copy", "cut", "paste", "cd..", "cd/", "ls", "cls", "format", "logout", "exit", "help", "print", "changepass", "showpassword"};

void display() 
{
	system("cls");
	dis_help();	//提示帮助
	string in1, filename, str = user + "@file\\";	//构成系统命令行前面的提示
	while (1){
		//cout << fileSystem.superBlock.freeDiskSta[0] << endl;
		int tmp = -1;
		while (1){
			//showSFD();
			textcolor(14);
			cout << endl << str << ">";
			cin.sync();	//清除缓存区的数据流
			in1.clear();
			cin_command(in1);	//用户输入命令
			if (in1.size() == 0) continue;
			//cin >> in1;
			for (int i = 0; i < in1.size(); i++){
				if (in1[i] >= 'A' && in1[i] <= 'Z'){
					in1[i] = in1[i] - 'A';
					in1[i] = in1[i] + 'a';     //大写转化为小写
				}
			}
			tmp = checkIn(in1);	//不同的功能分属于不同的类别  有1， 2， 3三类
			if (tmp == 1 && flag){	//如果是单目指令
				getchar();
				cin.sync();
			}
			else if (!flag)flag = 1;
			if (tmp != 0)break;

			textcolor(12);
			cout << "系统无此命令!\n请输入help获取帮助!" << endl;
		}
		//清屏
		if (in1 == "cls")system("cls");
		//帮助
		else if (in1 == "help"){
			dis_help();		//当前文件中
		}
		//退出系统
		else if (in1 == "exit"){
			logout();	//user中的

			textcolor(10);
			cout << "退出成功，谢谢使用！\n";
			exit(0);
		}
		//退出当前用户
		else if (in1 == "logout"){
			user = "";
			textcolor(10);
			cout << "退出成功!\n";
			system("cls");
			login();	//user.cpp
			//init();
			cout << "欢迎使用文件管理系统！\n" << endl;
			cur_SFD = 0;
			display();		//循环调用自身
			//cur_SFD = 0;
			//textcolor(14);
			//str = "file:\\"+user;
		}
		else if (in1 == "ls"){
			textcolor(11);
			showSFD();	//dir文件中
			continue;
		}
		//格式化当前用户
		else if (in1 == "format"){
			if (user.size() == 0){
				textcolor(10);
				cout << "请先登录，在进行格式化！\n";
				continue;
			}

			textcolor(12);
			cout << "警告：格式化将删除该用户的所有文件数据。\n" << "继续请输入Y，若想退出，请输入N： ";
			string strtmp;
			while (1){
				cin >> strtmp;
				if (strtmp == "N" || strtmp == "n")
					break;
				else if (strtmp != "Y" && strtmp != "y"){

					textcolor(12);
					cout << "输入不合法，请重新输入\n";
					continue;
				}
				break;
			}
			if (strtmp == "N" || strtmp == "n")
				continue;
			cur_SFD = checkUser(user);	//useri -- i;	先检查用户权限
			formatUser();	//format.cpp中
			cur_SFD = 0;	//将当前目录置为0


			str = user + "@file\\";

			textcolor(10);
			cout << "格式化成功！\n";
		}
		//创建文件
		else if (in1 == "create"){
			filename.clear(); 
			cin >> filename;
			cin.sync();
			int cur_inode = getInodeNum();	//dir目录下	获得当前目录的i结点号
			if (fileSystem.iNode[cur_inode].id != checkUser(user)){	//所属用户不对应
				textcolor(12);
				cout << "创建失败，您没有在该目录下创建文件的权限" << endl;
				continue;
			}
			if (createFile(filename)){
				textcolor(10);
				cout << "是否立刻写入内容？\n" << "立刻写入请输入Y，稍后写入请输入N： ";
				string strtmp;
				while (1){
					cin >> strtmp;
					if (strtmp == "Y" || strtmp == "y"){
						writeFile(filename);	//read_write.cpp 文件
						break;
					}
					else if (strtmp == "N" || strtmp == "n")
						break;
					else{
						textcolor(12);
						cout << "输入不合法，请重新输入\n";
					}
				}
			}
			
		}
		else if (in1 == "open"){

		}
		//读文件
		else if (in1 == "read"){
			filename.clear(); 
			cin_name(filename); 
			if (flag){
				 
				getchar();
				cin.sync();
			}
			else flag = 1;
			int tmpauth = checkFileAuth(filename);	//获得文件所属用户	user.cpp里面
			if (tmpauth == -2){
				textcolor(12);
				cout << "读取失败，不存在的文件名！\n";
				continue;
			}
			else if (tmpauth == -3){
				textcolor(12);
				cout << "读取失败，读取的文件为目录文件！\n";
				continue;
			}
			else if (tmpauth == -1){
				textcolor(12);
				cout << "读取失败，您不具有读取权限！\n";
				continue;
			}
			else{
				readFile(filename);
				cout << endl;
			}
		}
		//创建目录
		else if (in1 == "mkdir"){
			filename.clear(); 
			cin >> filename;
			int tmpauth = checkDirAuth(filename);
			if (tmpauth == -2){
				textcolor(12);
				cout << "已存在的文件名或目录名，创建目录失败！\n";
				continue;
			}
			else if (tmpauth == 0){
				textcolor(12);
				cout << "创建失败，您不具有创建该目录权限！\n";
				continue;
			}
			else{
				if (createDir(filename))	//create_delete_dir.cpp
					textcolor(10);
				cout << "创建成功！\n";
			}
		}
		//删除目录
		else if (in1 == "deld"){
			filename.clear(); 
			cin_name(filename);
			if (flag){ 
				getchar();
				cin.sync();
			}
			else flag = 1;
			int cur_inode = getInodeNum();	//获得当前目录的i节点号
			int use = checkUser(user);
			if (use != fileSystem.iNode[cur_inode].id){
				textcolor(12);
				cout << "删除失败，您不具有删除目录权限！\n";
				continue;
			}
			else{
				if (deleteDir(filename)){
					textcolor(10);
					cout << "删除成功！\n";
				}
			}
		}
		//删除文件
		else if (in1 == "delf"){
			filename.clear(); 
			cin_name(filename);
			if (flag){
				 
				getchar();
				cin.sync();
			}
			else flag = 1;
			int cur_inode = getInodeNum();
			if (fileSystem.iNode[cur_inode].id != checkUser(user)){
				textcolor(12);
				cout << "删除失败，您没有在该目录下删除文件的权限" << endl;
				continue;
			}
			if (freeFile(filename)){
				textcolor(10);
				cout << "删除文件成功！\n";
			}
		}

		//进入下一级目录
		else if (in1 == "cd"){
			filename.clear(); 
			cin_name(filename);
			if (flag){
				 
				getchar();
				cin.sync();
			}
			else flag = 1;
			str = str + ">" + filename;
			if (goNextDir(filename)){	//dir
				textcolor(6);
				//cout << "操作成功！\n";
				staSFD.push(cur_SFD);
			}
			else{	//如果没有顺利进入  则保持系统输入提示不变
				int len = filename.size() + 1;
				int pos = str.find(">" + filename);
				str.erase(pos, len);
			}
		}
		//写文件
		else if (in1 == "write"){
			filename.clear(); 
			cin_name(filename);
			if (flag){			 
				getchar();
				cin.sync();
			}
			else flag = 1;
			int tmpauth = checkFileAuth(filename);
			if (tmpauth == -2){
				textcolor(12);
				cout << "写文件失败，不存在的文件名！\n";
				continue;
			}
			else if (tmpauth == -3){
				textcolor(12);
				cout << "写文件失败，待写的文件为目录文件！\n";
				continue;
			}
			else if (tmpauth == -1 || tmpauth == 0){
				textcolor(12);
				cout << "写文件失败，您不具有写该文件的权限！\n";
				continue;
			}
			else{
				writeFile(filename);	//read_write.cpp
			}
		}
		//复制文件
		else if (in1 == "copy"){
			filename.clear(); 
			cin_name(filename);
			if (flag){
				getchar();
				cin.sync();
			}
			else flag = 1;
			if (copyContext(filename)){	//copy_paste.cpp	通过复制对应的文件名字来实现文件的复制
				copy_flag = 1;//copy
				textcolor(10);
				cout << "复制成功\n";
			}
		}
		else if (in1 == "cut"){
			filename.clear(); 
			cin_name(filename);
			if (flag){
				copy_flag = 2;//cut
				getchar();
				cin.sync();
			}
			else flag = 1;
			if (cutContext(filename)){
				textcolor(10);
				cout << "剪切成功\n";
			}
		}
		else if (in1 == "link"){
			filename.clear(); 
			cin_name(filename);
			if (flag){
				getchar();
				cin.sync();
			}
			else flag = 1;
			if (copyContext(filename)){
				copy_flag = 3;//link
				textcolor(10);
				cout << "已准备好链接\n";
			}
		}
		//粘贴文件
		else if (in1 == "paste"){
			if (pasteContext())
				cout << "粘贴成功\n";
		}
		//返回上级目录
		else if (in1 == "cd.."){
			if (staSFD.top() == 0){	//进入主目录
				str = user + "@file\\";
				cur_SFD = 0;
				continue;
			}
			int pos = -1, len = 0;
			for (int k = str.size() - 1; k >= 0; k--){
				if (str[k] == '>'){
					pos = k;
					break;
				}
				len++;
			}
			if (pos == -1){
				str = user + "@file\\";
				cur_SFD = 0;
				continue;
			}
			str.erase(pos, len + 1);
			staSFD.pop();
			cur_SFD = staSFD.top();	//获得对应的目录结构SFD
		}
		//返回根目录
		else if (in1 == "cd/"){
			str = user + "@file\\";
			cur_SFD = 0;
			while (!staSFD.empty()){	//清空SFD栈
				staSFD.pop();
			}
		}
		//else if (in1 == "signin"){}
		else if (in1 == "rname"){
			string rname;
			filename.clear(); 
			cin_name(filename);	//输入要更改的文件
			cin >> rname;	//输入更改后的名字
			cin.sync();
			int i, cur_inode = getInodeNum();
			if (fileSystem.iNode[cur_inode].id != checkUser(user)){
				textcolor(12);
				cout << "更名失败，您没有在该目录下更名的权限" << endl;
				continue;
			}
			else {
				for (i = 0; i < fileSystem.sfd[cur_SFD].sfdVec.size(); i++){
					if (fileSystem.sfd[cur_SFD].sfdVec[i].name == filename){
						break;
					}
				}
				if (i != fileSystem.sfd[cur_SFD].sfdVec.size()){
					fileSystem.sfd[cur_SFD].sfdVec[i].name = rname;
					textcolor(10);
					cout << "更名成功\n";
				}
				else{
					textcolor(12);
					cout << "未找到指定文件或目录名\n";
				}
			}
		}
		else if (in1 == "find"){
			filename.clear(); 
			cin_name(filename); 
			if (flag){
				 
				getchar();
				cin.sync();
			}
			else flag = 1;
			textcolor(15);
			cout << "\n文件名\t\t路径" << endl;
			textcolor(11);
			if (str.size() < 12)	//根目录
				find_filedir(str.substr(11), filename);		//dir.cpp
			else find_filedir("\\" + str.substr(12), filename);
		}
		else if (in1 == "print")
		{
			cout << "索引结点位矢图\n" << endl;
			for (int i = 0; i < fileSystem.superBlock.i_node; i++)
			{
				cout << " " << iNode[i] << " ";
				if ((i + 1) % 16 == 0) cout << endl;
			}
			cout << endl << endl << endl;
			cout << "磁盘块位矢图\n" << endl;
			for (int i = 0; i < fileSystem.superBlock.disk; i++)
			{
				cout << " " << diskBlock[i] << " ";
				if ((i + 1) % 16 == 0) cout << endl;
			}
			cout << endl << endl << endl;
			cout << "sfd位矢图\n" << endl;
			for (int i = 0; i < fileSystem.superBlock.sfd; i++)
			{
				cout << " " << SFDBlock[i] << " ";
				if ((i + 1) % 16 == 0) cout << endl;
			}
			cout << endl;
		}
		else if (in1 == "changepass")
		{
			char newpassword[512];
			cout << "请输入新密码:  ";
			scanf("%s", newpassword);
			//inputNewPassword(newpassword, 512);
			int userid = checkUser(user) - 1;
			for (int i = 0; i < strlen(passwords[userid]); i++) {
				passwords[userid][i] = newpassword[i] ^ 5;
			}
			freopen("password.txt", "w", stdout);

			for (int i = 0; i < 8; i++) {
				printf(" %s ", passwords[i]);
			}

			fclose(stdout);
			freopen("CON", "w", stdout);

			cout << endl;
		}
		else if (in1 == "showpassword")
		{
			cout << endl;
			cout << password;
			cout << endl;
		}
		else{
			textcolor(12);
			//cout << fileSystem.superBlock.freeDisk << "  " << fileSystem.superBlock.freei_node << "  " << fileSystem.superBlock.free_SFD << endl;
			cout << "无此命令，无此功能；\n更多功能，敬请期待！" << endl;
		}
	}
}

//功能判断函数	单目指令还是多目指令
int checkIn(string in){
	if (in == "paste" || in == "showpassword" || in == "print" || in == "cls" || in == "help" || in == "exit" || in == "format" || in == "logout" || in == "cd.." || in == "ls" || in == "signin" || in == "cd/")
		return 1;
	else if (in == "cut" || in == "changepass" || in == "link" || in == "copy" || in == "find" || in == "create" || in == "read" || in == "mkdir" || in == "deld" || in == "delf" || in == "cd" || in == "write")
		return 2;
	else if (in == "rname")
		return 3;
	else
		return 0;
}


//设置文本输出颜色
void textcolor(int color)//10绿色  14黄色   12红色
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}


void dis_help()
{
	textcolor(10);
	cout << "--------------------------------------------------------------------------------" << endl;
	Sleep(20);    //程序延时100ms
	cout << "-----------                        命令提示                          -----------" << endl;
	Sleep(20);    //程序延时100ms
	cout << "---------------------------------文件/目录操作----------------------------------" << endl;
	Sleep(10);    //程序延时100ms
	cout << "-----------  - 更名:     rname <O> <N>#  - 链接: link   <name>      ------------" << endl;
	Sleep(10);    //程序延时100ms
	cout << "-----------  - 查找:     find  <name> #  - 剪切: cut    <name>       -----------" << endl;
	Sleep(10);    //程序延时100ms
	cout << "-----------  - 复制:     copy  <name> #  - 粘贴: paste               -----------" << endl;
	Sleep(20);    //程序延时100ms
	cout << "---------------------------------目录操作---------------------------------------" << endl;
	Sleep(10);    //程序延时100ms
	cout << "-----------  - 创建目录: mkdir <name> #  - 显示目录:     ls         ------------" << endl;
	Sleep(10);    //程序延时100ms
	cout << "-----------  - 切换目录: cd    <name> #  - 返回根目录:   cd/        ------------" << endl;
	Sleep(10);    //程序延时100ms
	cout << "-----------  - 删除目录: deld  <name> #  - 返回上级目录: cd..       ------------" << endl;
	Sleep(20);    //程序延时100ms
	cout << "----------------------------------文件操作--------------------------------------" << endl;
	Sleep(10);    //程序延时100ms
	cout << "------------  - 创建文件: create <name>#  - 删除文件: delf <name>    -----------" << endl;
	Sleep(10);    //程序延时100ms
	cout << "------------  - 改写文件: write <name> #  - 读取文件: read <name>    -----------" << endl;
	Sleep(20);    //程序延时100ms
	cout << "----------------------------------其他操作--------------------------------------" << endl;
	Sleep(10);    //程序延时100ms
	cout << "------------  - 清屏:     cls          #  - 格式化:   format         -----------" << endl;
	Sleep(10);    //程序延时100ms
	cout << "------------  - 注销登录: logout       #  - 位示图:   print          -----------" << endl;
	Sleep(10);    //程序延时100ms
	cout << "------------  - 自动补全: Tab          #  - 关闭系统: exit           -----------" << endl;
	Sleep(10);    //程序延时100ms
	cout << "------------  - 重新输入: ↑           #  - 显示帮助: help           -----------" << endl;
	Sleep(10);    //程序延时100ms
	cout << "------------  - 修改密码: changepass   #  - 显示密码: showpassword   -----------" << endl;
	Sleep(10);    //程序延时100ms
	cout << "--------------------------------------------------------------------------------" << endl;
}

void cin_name(string &name)
{
	char c;
	while ((c = getch()) == ' ' && name.size() == 0)putchar(' ');
	while (c > 32){
		name += c;
		cout << c;
		_getch();
		c = getch();
	}
	_getch();
	if (c == '\t'){
		int pos, num = 0;
		for (int i = 0; i < fileSystem.sfd[cur_SFD].sfdVec.size(); ++i){
			if (fileSystem.sfd[cur_SFD].sfdVec[i].name.find(name) != string::npos){
				pos = i;
				num++;
			}
		}
		if (num == 1){
			for (int i = 0; i < name.size(); ++i)putchar('\b');
			cout << fileSystem.sfd[cur_SFD].sfdVec[pos].name;
			name = fileSystem.sfd[cur_SFD].sfdVec[pos].name;
			return;
		}
		else cin_name(name);
	}
	else if (c == 8){
		if (name.size() > 0){
			name.erase(name.size() - 1);
			fflush(stdout); putchar('\b'); fflush(stdout);
			putchar(' ');
			fflush(stdout);	putchar('\b'); fflush(stdout);
		}
		cin_name(name);
	}
	else {
		if (c == -32){
			cin.sync();
			cin_command(name);
		}
		else if (c == 13){
			cout << endl;
			flag = 0;
		}
		else cout << c;
		return;
	}
}

void cin_command(string &in)
{
	char c;
	while ((c = _getch()) == ' ' && in.size() == 0 ) putchar(' ');	//输入空格 或者 未输入
	while (c > 32){	//如果输入的c不是特殊字符的话
		in += c;
		cout << c;
		//getchar();
		_getch();
		c = _getch();	//继续接收输入	从控制台读取一个字符，但不显示在屏幕上
		//cin >> c;
	}
	_getch();
	//getchar();
	//getchar();
	if (c == '\t'){		//请求提示
		int pos, num = 0;
		for (int i = 0; i < com_num; ++i){	//在给出的所有命令中查找包含输入字符的命令
			if (com[i].find(in) != string::npos){
				num++;
				pos = i;
			}
		}
		if (num == 1){
			for (int i = 0; i < in.size(); ++i) putchar('\b');		//删除前面的Tab键
			cout << com[pos];
			in = com[pos];
			return;
		}
		else cin_command(in);	//否则 继续输入
	}
	else if (c == 8){	//'\b'	删除键
		if (in.size() > 0){
			in.erase(in.size() - 1);	//删除

			fflush(stdout);	//清空输出缓冲区
			putchar('\b'); 
			fflush(stdout);

			putchar(' ');

			fflush(stdout);	
			putchar('\b');
			fflush(stdout);
		}		
		cin_command(in);
	}
	else {
		if (c == -32){
			cin.sync(); 
			cin_command(in);
		}
		else if (c == 13){	//回车
			cout << endl;
			flag = 0;
		}
		else cout << c;
		return;
	}
}

/*
void inputNewPassword(char *newpassword, int size) {
	char c;
	int count = 0;
	//int size = 512;
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
	newpassword = tmppassword;
	delete[] tmppassword; // 释放空间
	cout << endl;
}*/