#include "OS.h"

//��¼�û���
void login(){
	system("cls");
	textcolor(11);
	cout << "--------------------------------------------------------------------------------" << endl;
	cout << "-----------                 ��ӭʹ���ļ�����ϵͳ                    ------------" << endl;
	cout << "--------------------------------------------------------------------------------" << endl;
	while (1)
	{
		textcolor(13);
		cout << "\n���¼�ļ�ϵͳ(����exit�˳��ļ�ϵͳ)�� ";
		cin >> user;	//����ȫ�ֱ���userֵ
		//cout << user;
		if (user == "exit") {
			textcolor(9);
			cout << "\n�˳��ɹ���ллʹ�ã�\n";
			exit(0);
		}

		int res = checkUser(user);	//�ж��û�	�����ڵ�ǰ�ļ���	useri -- i(����ֵ��
		if (!res) {	//�������ֵΪ0�����������û������ڡ�
			textcolor(12);
			cout << "\n�����ڵ��û������������룡\n";
			continue;
		}

		cout << "\n�������¼���룺 ";
		//inputPassword();
		cin >> password;
		int password_res = checkPassword();
		if (!password_res) {
			textcolor(12);
			user = "";
			cout << "\n�û����벻�ԣ�����������!";
		    
			getchar();
			//getchar();
			continue;
		}

		
		textcolor(10);
		cout << "\n��½�ɹ����س������ļ�ϵͳ" << endl;
		getchar();
		//getchar();
		break;
	}

}

//�жϴ�ʱ�û�
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


//��鵱ǰ�û����ļ��Ķ�дȨ��
int checkFileAuth(string filename){
	int pos = -1;
	//���Ҹ��ļ���SFD��λ��
	for (int i = 0; i < fileSystem.sfd[cur_SFD].sfdVec.size(); i++){
		if (filename == fileSystem.sfd[cur_SFD].sfdVec[i].name){
			pos = i;
			break;
		}
	}
	if (pos == -1)	//�����ڸ�Ŀ¼�²����ڸ��ļ�
		return -2;

	int cur_inode = fileSystem.sfd[cur_SFD].sfdVec[pos].id;		//��ø��ļ���i�ڵ�id
	
	if (fileSystem.iNode[cur_inode].type == 1)        //1��ʾ��ǰi�����Ŀ¼
		return -3;
	
	int use = checkUser(user) - 1;	//��Ϊauth[8] �����Ǵ�0 ��ʼ��
	
	return fileSystem.iNode[cur_inode].auth[use];	
}



//��鵱ǰ�û���Ŀ¼�Ķ�дȨ��
int checkDirAuth(string filename){
	int pos = -1;
	for (int i = 0; i < fileSystem.sfd[cur_SFD].sfdVec.size(); i++){   //��鵱ǰĿ¼���Ƿ����
		if (filename == fileSystem.sfd[cur_SFD].sfdVec[i].name){
			pos = i;
			break;
		}
	}
	if (pos != -1)
		return -2;

	int cur_inode = getInodeNum();       //��õ�ǰĿ¼��i����
	int use = checkUser(user);				//��õ�ǰ�û�id
	if (use == fileSystem.iNode[cur_inode].id)   //����Ƿ���Ȩ��
		return 1;
	return 0;
}

//�ǳ�
void logout(){
	exitSystem();	//exit��
	user = "";
}

//У������
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
	password = tmppassword;
	delete[] tmppassword; // �ͷſռ�
	cout << endl;
}*/
