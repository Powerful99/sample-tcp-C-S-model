#include<iostream>
#include<WinSock2.h> //第2版本的网络库
#pragma comment(lib,"ws2_32.lib") //windows 32位版本的，没有64版本的，但是64位下也有32位版本的



int main()
{
	//初始化WSA  
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	int nRes = WSAStartup(sockVersion, &wsaData);//打开一个套接字

	if (0 != nRes)
	{
		switch (nRes)
		{
		case WSASYSNOTREADY: 
			printf("重启电脑，或者检查网络库");   break;
		case WSAVERNOTSUPPORTED: 
			printf("请更新网络库");  break;
		case WSAEINPROGRESS: 
			printf("请重新启动");  break;
		case WSAEPROCLIM:  
			printf("请关闭不必要的软件，以确保有足够的网络资源"); 
			break;
		}
	}
	if (2 != HIBYTE(wsaData.wVersion) || 2 != LOBYTE(wsaData.wVersion))
	{
		printf("网络库版本错误");
		WSACleanup();//关闭此套接字
		return 0;
	}

	//套接字的创建和关闭
	SOCKET slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (slisten == INVALID_SOCKET)
	{
		printf("socket error !");
		return 0;
	}
	//绑定IP和端口  
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(8888);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	if (bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		printf("bind error !");
	}

	//开始监听  
	if (listen(slisten, 5) == SOCKET_ERROR)
	{
		printf("listen error !");
		return 0;
	}

	//循环接收数据  
	SOCKET sClient;
	sockaddr_in remoteAddr;
	int nAddrlen = sizeof(remoteAddr);
	char revData[255];
	while (true)
	{
		printf("等待连接...\n");
		sClient = accept(slisten, (SOCKADDR *)&remoteAddr, &nAddrlen);
		if (sClient == INVALID_SOCKET)
		{
			printf("accept error !");
			continue;
		}
		printf("接受到一个连接：%s \r\n", inet_ntoa(remoteAddr.sin_addr));

		//接收数据  
		int ret = recv(sClient, revData, 255, 0);
		if (ret > 0)
		{
			revData[ret] = 0x00;
			printf(revData);
		}

		//发送数据  
		const char * sendData = "你好，TCP客户端！我是服务器\n";
		send(sClient, sendData, strlen(sendData), 0);
		closesocket(sClient);
	}

	closesocket(slisten);
	WSACleanup();

	return 0;
}

