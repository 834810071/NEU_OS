#include "OS.h"

//初始化超级块
void initSuperBlock() {
	//初始化超级块
	freopen("superBlock.txt", "r", stdin);	//输入重定向
	scanf("%d%d", &fileSystem.superBlock.i_node, &fileSystem.superBlock.freei_node);	//128个i节点数 108个空闲
	//cin>>fileSystem.superBlock.i_node>>fileSystem.superBlock.freei_node;
	//scanf("%d%d", &fileSystem.superBlock.i_node, &fileSystem.superBlock.freei_node);
	for (int i = 0; i < fileSystem.superBlock.freei_node; i++) {
		int tmp;
		scanf("%d", &tmp);	//从文件中输入哪个i节点为空闲
		iNode[tmp] = 1;      //i结点位示图	全局变量
		fileSystem.superBlock.freeiid.push_back(tmp);
	}
	//cin>>fileSystem.superBlock.sfd>>fileSystem.superBlock.free_SFD;
	scanf("%d%d", &fileSystem.superBlock.sfd, &fileSystem.superBlock.free_SFD);	//同理 输入目录的情况 初始时 512个 499空闲
	for (int i = 0; i < fileSystem.superBlock.free_SFD; i++) {
		int tmp;
		scanf("%d", &tmp);
		SFDBlock[tmp] = 1;      //空闲SFD（目录）初始化
		fileSystem.superBlock.freeSFD.push_back(tmp);
	}
	//cin>>fileSystem.superBlock.disk>>fileSystem.superBlock.freeDisk;
	scanf("%d%d", &fileSystem.superBlock.disk, &fileSystem.superBlock.freeDisk);   //空闲磁盘块	512个 503空闲
	fclose(stdin);	//关闭重定向
	freopen("CON", "r", stdin);  //重定位标准输入，将标准输入由键盘改成文件输入	目的是恢复句柄

	freopen("freeBlock.txt", "r", stdin);       //空闲的磁盘块成组链接法保存
	fileSystem.superBlock.freeDiskSta[0] = 1;     //放入一块
	fileSystem.superBlock.freeDiskSta[1] = -1;   //成组连接第一块
	for (int i = 0; i < fileSystem.superBlock.freeDisk; i++) {		//503个空闲 1 到 502
		int tmp;
		scanf("%d", &tmp);
		diskBlock[tmp] = 1;                               //1表示空闲，磁盘块位示图初始化
		int &pos = fileSystem.superBlock.freeDiskSta[0];  //每次都等于位示图栈顶数（栈顶是目前该块中存放的空闲块数)  成组链 大小为50  第0个里面存放当前空闲块数
		if (pos < 50)	//未放满
			fileSystem.superBlock.freeDiskSta[++pos] = tmp;    //放置空闲块
		else {   //超级块处理     //疑问
			for (int i = 0; i <= 50; i++) {
				stringstream ss;	//创建一个流
				string tmpstr;
				ss << fileSystem.superBlock.freeDiskSta[i];	//向流中传值
				ss >> tmpstr;	//向tmpstr中写入
				fileSystem.diskBlock[tmp].file += " ";
				fileSystem.diskBlock[tmp].file += tmpstr;
			}
			fileSystem.superBlock.freeDiskSta[0] = 1;
			fileSystem.superBlock.freeDiskSta[1] = tmp;
		}
	}
	//固定格式 重定位标准输入
	fclose(stdin);
	freopen("CON", "r", stdin);
}


//初始化i结点
void initINode() {
	//初始化前9个i结点
	freopen("iNode.txt", "r", stdin);
	for (int i = 0; i <= 8; i++) {
		scanf("%d%d%d%d%d", &fileSystem.iNode[i].id, &fileSystem.iNode[i].type, &fileSystem.iNode[i].sfd_id, &fileSystem.iNode[i].filelen, &fileSystem.iNode[i].qcount);
		//cin>>fileSystem.iNode[i].id>>fileSystem.iNode[i].type>>fileSystem.iNode[i].sfd_id>>fileSystem.iNode[i].filelen>>fileSystem.iNode[i].qcount;
		for (int j = 0; j < 8; j++)
			scanf("%d", &fileSystem.iNode[i].auth[j]);       //用户的权限
		//cin>>fileSystem.iNode[i].auth[j];
	}

	fclose(stdin);
	freopen("CON", "r", stdin);

	freopen("iNodeContext.txt", "r", stdin);
	//一共有128个i结点
	for (int i = 9; i < 128; i++) {
		if (!iNode[i]) {	//如果节点i被占据  则初始化
			scanf("%d%d%d%d%d", &fileSystem.iNode[i].id, &fileSystem.iNode[i].type, &fileSystem.iNode[i].sfd_id, &fileSystem.iNode[i].filelen, &fileSystem.iNode[i].qcount);
			//cin>>fileSystem.iNode[i].id>>fileSystem.iNode[i].type>>fileSystem.iNode[i].sfd_id>>fileSystem.iNode[i].filelen>>fileSystem.iNode[i].qcount;
			for (int j = 0; j < 8; j++)
				scanf("%d", &fileSystem.iNode[i].auth[j]);
			//cin>>fileSystem.iNode[i].auth[j];
			if (fileSystem.iNode[i].type == 0) {	//如果是文件 则输入i节点对应的 占用磁盘块个数 以及 对应磁盘块索引id
				scanf("%d%d", &fileSystem.iNode[i].diskBlockNum, &fileSystem.iNode[i].diskBlockId);
				//cin>>fileSystem.iNode[i].diskBlockNum;
				//cin>>fileSystem.iNode[i].diskBlockId;
			}
		}
	}

	fclose(stdin);
	freopen("CON", "r", stdin);
}

//初始化磁盘块
void initDiskBlock() {
	//磁盘块初始化

	freopen("diskBlock.txt", "r", stdin);    //diskBlock 中存放的是每个磁盘块大小
	for (int i = 0; i < 512; i++) {
		if (!diskBlock[i]) {                //如果磁盘块满的话 磁盘块被占用的话		9个
			scanf("%d", &fileSystem.diskBlock[i].strNum);   //读入磁盘块已用大小
			//cin>>fileSystem.diskBlock[i].strNum;
			string str;
			for (int j = 0; j < fileSystem.diskBlock[i].strNum; j++) {
				cin >> str;				//
				fileSystem.diskBlock[i].file = " " + fileSystem.diskBlock[i].file + " " + str + " ";  //文件内容
			}
		}
	}

	fclose(stdin);
	freopen("CON", "r", stdin);
}

//初始化SFD
void initSFD() {
	//初始化SFD
	freopen("sfdContext.txt", "r", stdin);	

	for (int i = 1; i <= 8; i++) {		//修改
		string tmps;
		stringstream ss;	//整形变为字符类型
		ss << i;
		ss >> tmps;
		SingleSFD singleSFD;	//单目录
		singleSFD.name = "root" + tmps;
		singleSFD.id = i;
		fileSystem.sfd[0].sfdVec.push_back(singleSFD);   //每个文件目录下的sfd动态数组
	}

	staSFD.push(0);    //记录目录栈

	for (int i = 1; i < 512; i++) {		//12个

		if (!SFDBlock[i]) {       //只要SFDBlock不空闲
			scanf("%d", &fileSystem.sfd[i].sfdNum);
			//cin>>fileSystem.sfd[i].sfdNum;
			for (int j = 0; j < fileSystem.sfd[i].sfdNum; j++) {	//sdf为0的时候是还没来得及回收
				SingleSFD singleSFD;
				cin >> singleSFD.name;
				scanf("%d", &singleSFD.id);     //文件对应i结点编号	
				fileSystem.sfd[i].sfdVec.push_back(singleSFD);
			}
		}
	}

	fclose(stdin);
	freopen("CON", "r", stdin);
}

//初始化
void init() {

	memset(iNode, 0, sizeof(iNode));      //初始化，iNode,diskBlock  SFDblock都初始化为0,先都初始为占据状态
	memset(diskBlock, 0, sizeof(diskBlock));
	memset(SFDBlock, 0, sizeof(SFDBlock));
	//初始化时候才开始把空闲的为1
	initSuperBlock();  //初始化超级块
	initINode();	//初始化i节点
	initDiskBlock();
	initSFD();
}