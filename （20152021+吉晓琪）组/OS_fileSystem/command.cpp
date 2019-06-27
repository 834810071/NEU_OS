#include "OS.h"
#define com_num  24
int flag = 1;
string com[com_num] = { "link", "cd", "create", "rname", "mkdir", "find", "write", "read", "deld", "delf", "copy", "cut", "paste", "cd..", "cd/", "ls", "cls", "format", "logout", "exit", "help", "print", "changepass", "showpassword"};

void display() 
{
	system("cls");
	dis_help();	//��ʾ����
	string in1, filename, str = user + "@file\\";	//����ϵͳ������ǰ�����ʾ
	while (1){
		//cout << fileSystem.superBlock.freeDiskSta[0] << endl;
		int tmp = -1;
		while (1){
			//showSFD();
			textcolor(14);
			cout << endl << str << ">";
			cin.sync();	//�����������������
			in1.clear();
			cin_command(in1);	//�û���������
			if (in1.size() == 0) continue;
			//cin >> in1;
			for (int i = 0; i < in1.size(); i++){
				if (in1[i] >= 'A' && in1[i] <= 'Z'){
					in1[i] = in1[i] - 'A';
					in1[i] = in1[i] + 'a';     //��дת��ΪСд
				}
			}
			tmp = checkIn(in1);	//��ͬ�Ĺ��ܷ����ڲ�ͬ�����  ��1�� 2�� 3����
			if (tmp == 1 && flag){	//����ǵ�Ŀָ��
				getchar();
				cin.sync();
			}
			else if (!flag)flag = 1;
			if (tmp != 0)break;

			textcolor(12);
			cout << "ϵͳ�޴�����!\n������help��ȡ����!" << endl;
		}
		//����
		if (in1 == "cls")system("cls");
		//����
		else if (in1 == "help"){
			dis_help();		//��ǰ�ļ���
		}
		//�˳�ϵͳ
		else if (in1 == "exit"){
			logout();	//user�е�

			textcolor(10);
			cout << "�˳��ɹ���ллʹ�ã�\n";
			exit(0);
		}
		//�˳���ǰ�û�
		else if (in1 == "logout"){
			user = "";
			textcolor(10);
			cout << "�˳��ɹ�!\n";
			system("cls");
			login();	//user.cpp
			//init();
			cout << "��ӭʹ���ļ�����ϵͳ��\n" << endl;
			cur_SFD = 0;
			display();		//ѭ����������
			//cur_SFD = 0;
			//textcolor(14);
			//str = "file:\\"+user;
		}
		else if (in1 == "ls"){
			textcolor(11);
			showSFD();	//dir�ļ���
			continue;
		}
		//��ʽ����ǰ�û�
		else if (in1 == "format"){
			if (user.size() == 0){
				textcolor(10);
				cout << "���ȵ�¼���ڽ��и�ʽ����\n";
				continue;
			}

			textcolor(12);
			cout << "���棺��ʽ����ɾ�����û��������ļ����ݡ�\n" << "����������Y�������˳���������N�� ";
			string strtmp;
			while (1){
				cin >> strtmp;
				if (strtmp == "N" || strtmp == "n")
					break;
				else if (strtmp != "Y" && strtmp != "y"){

					textcolor(12);
					cout << "���벻�Ϸ�������������\n";
					continue;
				}
				break;
			}
			if (strtmp == "N" || strtmp == "n")
				continue;
			cur_SFD = checkUser(user);	//useri -- i;	�ȼ���û�Ȩ��
			formatUser();	//format.cpp��
			cur_SFD = 0;	//����ǰĿ¼��Ϊ0


			str = user + "@file\\";

			textcolor(10);
			cout << "��ʽ���ɹ���\n";
		}
		//�����ļ�
		else if (in1 == "create"){
			filename.clear(); 
			cin >> filename;
			cin.sync();
			int cur_inode = getInodeNum();	//dirĿ¼��	��õ�ǰĿ¼��i����
			if (fileSystem.iNode[cur_inode].id != checkUser(user)){	//�����û�����Ӧ
				textcolor(12);
				cout << "����ʧ�ܣ���û���ڸ�Ŀ¼�´����ļ���Ȩ��" << endl;
				continue;
			}
			if (createFile(filename)){
				textcolor(10);
				cout << "�Ƿ�����д�����ݣ�\n" << "����д��������Y���Ժ�д��������N�� ";
				string strtmp;
				while (1){
					cin >> strtmp;
					if (strtmp == "Y" || strtmp == "y"){
						writeFile(filename);	//read_write.cpp �ļ�
						break;
					}
					else if (strtmp == "N" || strtmp == "n")
						break;
					else{
						textcolor(12);
						cout << "���벻�Ϸ�������������\n";
					}
				}
			}
			
		}
		else if (in1 == "open"){

		}
		//���ļ�
		else if (in1 == "read"){
			filename.clear(); 
			cin_name(filename); 
			if (flag){
				 
				getchar();
				cin.sync();
			}
			else flag = 1;
			int tmpauth = checkFileAuth(filename);	//����ļ������û�	user.cpp����
			if (tmpauth == -2){
				textcolor(12);
				cout << "��ȡʧ�ܣ������ڵ��ļ�����\n";
				continue;
			}
			else if (tmpauth == -3){
				textcolor(12);
				cout << "��ȡʧ�ܣ���ȡ���ļ�ΪĿ¼�ļ���\n";
				continue;
			}
			else if (tmpauth == -1){
				textcolor(12);
				cout << "��ȡʧ�ܣ��������ж�ȡȨ�ޣ�\n";
				continue;
			}
			else{
				readFile(filename);
				cout << endl;
			}
		}
		//����Ŀ¼
		else if (in1 == "mkdir"){
			filename.clear(); 
			cin >> filename;
			int tmpauth = checkDirAuth(filename);
			if (tmpauth == -2){
				textcolor(12);
				cout << "�Ѵ��ڵ��ļ�����Ŀ¼��������Ŀ¼ʧ�ܣ�\n";
				continue;
			}
			else if (tmpauth == 0){
				textcolor(12);
				cout << "����ʧ�ܣ��������д�����Ŀ¼Ȩ�ޣ�\n";
				continue;
			}
			else{
				if (createDir(filename))	//create_delete_dir.cpp
					textcolor(10);
				cout << "�����ɹ���\n";
			}
		}
		//ɾ��Ŀ¼
		else if (in1 == "deld"){
			filename.clear(); 
			cin_name(filename);
			if (flag){ 
				getchar();
				cin.sync();
			}
			else flag = 1;
			int cur_inode = getInodeNum();	//��õ�ǰĿ¼��i�ڵ��
			int use = checkUser(user);
			if (use != fileSystem.iNode[cur_inode].id){
				textcolor(12);
				cout << "ɾ��ʧ�ܣ���������ɾ��Ŀ¼Ȩ�ޣ�\n";
				continue;
			}
			else{
				if (deleteDir(filename)){
					textcolor(10);
					cout << "ɾ���ɹ���\n";
				}
			}
		}
		//ɾ���ļ�
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
				cout << "ɾ��ʧ�ܣ���û���ڸ�Ŀ¼��ɾ���ļ���Ȩ��" << endl;
				continue;
			}
			if (freeFile(filename)){
				textcolor(10);
				cout << "ɾ���ļ��ɹ���\n";
			}
		}

		//������һ��Ŀ¼
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
				//cout << "�����ɹ���\n";
				staSFD.push(cur_SFD);
			}
			else{	//���û��˳������  �򱣳�ϵͳ������ʾ����
				int len = filename.size() + 1;
				int pos = str.find(">" + filename);
				str.erase(pos, len);
			}
		}
		//д�ļ�
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
				cout << "д�ļ�ʧ�ܣ������ڵ��ļ�����\n";
				continue;
			}
			else if (tmpauth == -3){
				textcolor(12);
				cout << "д�ļ�ʧ�ܣ���д���ļ�ΪĿ¼�ļ���\n";
				continue;
			}
			else if (tmpauth == -1 || tmpauth == 0){
				textcolor(12);
				cout << "д�ļ�ʧ�ܣ���������д���ļ���Ȩ�ޣ�\n";
				continue;
			}
			else{
				writeFile(filename);	//read_write.cpp
			}
		}
		//�����ļ�
		else if (in1 == "copy"){
			filename.clear(); 
			cin_name(filename);
			if (flag){
				getchar();
				cin.sync();
			}
			else flag = 1;
			if (copyContext(filename)){	//copy_paste.cpp	ͨ�����ƶ�Ӧ���ļ�������ʵ���ļ��ĸ���
				copy_flag = 1;//copy
				textcolor(10);
				cout << "���Ƴɹ�\n";
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
				cout << "���гɹ�\n";
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
				cout << "��׼��������\n";
			}
		}
		//ճ���ļ�
		else if (in1 == "paste"){
			if (pasteContext())
				cout << "ճ���ɹ�\n";
		}
		//�����ϼ�Ŀ¼
		else if (in1 == "cd.."){
			if (staSFD.top() == 0){	//������Ŀ¼
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
			cur_SFD = staSFD.top();	//��ö�Ӧ��Ŀ¼�ṹSFD
		}
		//���ظ�Ŀ¼
		else if (in1 == "cd/"){
			str = user + "@file\\";
			cur_SFD = 0;
			while (!staSFD.empty()){	//���SFDջ
				staSFD.pop();
			}
		}
		//else if (in1 == "signin"){}
		else if (in1 == "rname"){
			string rname;
			filename.clear(); 
			cin_name(filename);	//����Ҫ���ĵ��ļ�
			cin >> rname;	//������ĺ������
			cin.sync();
			int i, cur_inode = getInodeNum();
			if (fileSystem.iNode[cur_inode].id != checkUser(user)){
				textcolor(12);
				cout << "����ʧ�ܣ���û���ڸ�Ŀ¼�¸�����Ȩ��" << endl;
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
					cout << "�����ɹ�\n";
				}
				else{
					textcolor(12);
					cout << "δ�ҵ�ָ���ļ���Ŀ¼��\n";
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
			cout << "\n�ļ���\t\t·��" << endl;
			textcolor(11);
			if (str.size() < 12)	//��Ŀ¼
				find_filedir(str.substr(11), filename);		//dir.cpp
			else find_filedir("\\" + str.substr(12), filename);
		}
		else if (in1 == "print")
		{
			cout << "�������λʸͼ\n" << endl;
			for (int i = 0; i < fileSystem.superBlock.i_node; i++)
			{
				cout << " " << iNode[i] << " ";
				if ((i + 1) % 16 == 0) cout << endl;
			}
			cout << endl << endl << endl;
			cout << "���̿�λʸͼ\n" << endl;
			for (int i = 0; i < fileSystem.superBlock.disk; i++)
			{
				cout << " " << diskBlock[i] << " ";
				if ((i + 1) % 16 == 0) cout << endl;
			}
			cout << endl << endl << endl;
			cout << "sfdλʸͼ\n" << endl;
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
			cout << "������������:  ";
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
			cout << "�޴�����޴˹��ܣ�\n���๦�ܣ������ڴ���" << endl;
		}
	}
}

//�����жϺ���	��Ŀָ��Ƕ�Ŀָ��
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


//�����ı������ɫ
void textcolor(int color)//10��ɫ  14��ɫ   12��ɫ
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}


void dis_help()
{
	textcolor(10);
	cout << "--------------------------------------------------------------------------------" << endl;
	Sleep(20);    //������ʱ100ms
	cout << "-----------                        ������ʾ                          -----------" << endl;
	Sleep(20);    //������ʱ100ms
	cout << "---------------------------------�ļ�/Ŀ¼����----------------------------------" << endl;
	Sleep(10);    //������ʱ100ms
	cout << "-----------  - ����:     rname <O> <N>#  - ����: link   <name>      ------------" << endl;
	Sleep(10);    //������ʱ100ms
	cout << "-----------  - ����:     find  <name> #  - ����: cut    <name>       -----------" << endl;
	Sleep(10);    //������ʱ100ms
	cout << "-----------  - ����:     copy  <name> #  - ճ��: paste               -----------" << endl;
	Sleep(20);    //������ʱ100ms
	cout << "---------------------------------Ŀ¼����---------------------------------------" << endl;
	Sleep(10);    //������ʱ100ms
	cout << "-----------  - ����Ŀ¼: mkdir <name> #  - ��ʾĿ¼:     ls         ------------" << endl;
	Sleep(10);    //������ʱ100ms
	cout << "-----------  - �л�Ŀ¼: cd    <name> #  - ���ظ�Ŀ¼:   cd/        ------------" << endl;
	Sleep(10);    //������ʱ100ms
	cout << "-----------  - ɾ��Ŀ¼: deld  <name> #  - �����ϼ�Ŀ¼: cd..       ------------" << endl;
	Sleep(20);    //������ʱ100ms
	cout << "----------------------------------�ļ�����--------------------------------------" << endl;
	Sleep(10);    //������ʱ100ms
	cout << "------------  - �����ļ�: create <name>#  - ɾ���ļ�: delf <name>    -----------" << endl;
	Sleep(10);    //������ʱ100ms
	cout << "------------  - ��д�ļ�: write <name> #  - ��ȡ�ļ�: read <name>    -----------" << endl;
	Sleep(20);    //������ʱ100ms
	cout << "----------------------------------��������--------------------------------------" << endl;
	Sleep(10);    //������ʱ100ms
	cout << "------------  - ����:     cls          #  - ��ʽ��:   format         -----------" << endl;
	Sleep(10);    //������ʱ100ms
	cout << "------------  - ע����¼: logout       #  - λʾͼ:   print          -----------" << endl;
	Sleep(10);    //������ʱ100ms
	cout << "------------  - �Զ���ȫ: Tab          #  - �ر�ϵͳ: exit           -----------" << endl;
	Sleep(10);    //������ʱ100ms
	cout << "------------  - ��������: ��           #  - ��ʾ����: help           -----------" << endl;
	Sleep(10);    //������ʱ100ms
	cout << "------------  - �޸�����: changepass   #  - ��ʾ����: showpassword   -----------" << endl;
	Sleep(10);    //������ʱ100ms
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
	while ((c = _getch()) == ' ' && in.size() == 0 ) putchar(' ');	//����ո� ���� δ����
	while (c > 32){	//��������c���������ַ��Ļ�
		in += c;
		cout << c;
		//getchar();
		_getch();
		c = _getch();	//������������	�ӿ���̨��ȡһ���ַ���������ʾ����Ļ��
		//cin >> c;
	}
	_getch();
	//getchar();
	//getchar();
	if (c == '\t'){		//������ʾ
		int pos, num = 0;
		for (int i = 0; i < com_num; ++i){	//�ڸ��������������в��Ұ��������ַ�������
			if (com[i].find(in) != string::npos){
				num++;
				pos = i;
			}
		}
		if (num == 1){
			for (int i = 0; i < in.size(); ++i) putchar('\b');		//ɾ��ǰ���Tab��
			cout << com[pos];
			in = com[pos];
			return;
		}
		else cin_command(in);	//���� ��������
	}
	else if (c == 8){	//'\b'	ɾ����
		if (in.size() > 0){
			in.erase(in.size() - 1);	//ɾ��

			fflush(stdout);	//������������
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
		else if (c == 13){	//�س�
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
	char *tmppassword = new char[512]; // ��̬����ռ�
	while ((c = getch()) != '\r') {

		if (c == 8) { // �˸�
			if (count == 0) {
				continue;
			}
			putchar('\b'); // ����һ��
			putchar(' '); // ���һ���ո�ԭ����*����
			putchar('\b'); // �ٻ���һ��ȴ�����
			count--;
		}
		if (count == size - 1) { // ��󳤶�Ϊsize-1
			continue;
		}
		if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')) {  // ����ֻ�ɰ������ֺ���ĸ
			putchar('*');  // ���յ�һ���ַ���, ��ӡһ��*
			tmppassword[count] = c;
			count++;
		}
	}
	tmppassword[count] = '\0';
	newpassword = tmppassword;
	delete[] tmppassword; // �ͷſռ�
	cout << endl;
}*/