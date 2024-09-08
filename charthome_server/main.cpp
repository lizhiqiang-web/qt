#include <iostream>
#include "CServer.h"
#include "Singleton.h"
#include "LogicSystem.h"
#include <csignal>
#include <thread>
#include <mutex>

#include <boost/asio.hpp>

#include <iostream>

#include <json/json.h>
#include <json/value.h>
#include <json/reader.h>

using namespace std;
using namespace boost::asio::ip;

int main(int argc, char *argv[])
{
	cout << "main" << endl;
	if (strcmp(argv[1], "s") == 0)
	{
		cout << "server" << endl;
		try
		{
			boost::asio::io_context io_context;
			boost::asio::signal_set signals(io_context, SIGINT, SIGTERM);
			signals.async_wait([&io_context](auto, auto)
							   { io_context.stop(); });
			CServer s(io_context, 6666);
			io_context.run();
		}
		catch (std::exception &e)
		{
			std::cerr << "Exception: " << e.what() << endl;
		}
	}
	else if (strcmp(argv[1], "c") == 0)
	{
		// SyncClient.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
		//
		cout << "server" << endl;
		// int MAX_LENGTH = 1024 * 2;
		const int HEAD_LENGTH = 2;
		const int HEAD_TOTAL = 4;

		try
		{
			// 创建上下文服务
			boost::asio::io_context ioc;
			// 构造endpoint
			tcp::endpoint remote_ep(address::from_string("127.0.0.1"), 6666);
			tcp::socket sock(ioc);
			boost::system::error_code error = boost::asio::error::host_not_found;
			;
			sock.connect(remote_ep, error);
			if (error)
			{
				cout << "connect failed, code is " << error.value() << " error msg is " << error.message();
				return 0;
			}
			while (true)
			{
				string data;
				cin >> data;
				Json::Value root;
				root["id"] = 1001;
				root["data"] = data.c_str();
				std::string request = root.toStyledString();
				std::cout << request << endl;
				size_t request_length = request.length();
				char send_data[MAX_LENGTH] = {0};
				int msgid = 1001;
				int msgid_host = boost::asio::detail::socket_ops::host_to_network_short(msgid);
				memcpy(send_data, &msgid_host, 2);
				// 转为网络字节序
				int request_host_length = boost::asio::detail::socket_ops::host_to_network_short(request_length);
				memcpy(send_data + 2, &request_host_length, 2);
				memcpy(send_data + 4, request.c_str(), request_length);
				boost::asio::write(sock, boost::asio::buffer(send_data, request_length + 4));
				cout << "begin to receive..." << endl;

				char reply_head[HEAD_TOTAL];
				size_t reply_length = boost::asio::read(sock, boost::asio::buffer(reply_head, HEAD_TOTAL));

				msgid = 0;
				memcpy(&msgid, reply_head, HEAD_LENGTH);
				short msglen = 0;
				memcpy(&msglen, reply_head + 2, HEAD_LENGTH);
				// 转为本地字节序
				msglen = boost::asio::detail::socket_ops::network_to_host_short(msglen);
				msgid = boost::asio::detail::socket_ops::network_to_host_short(msgid);
				char msg[MAX_LENGTH] = {0};
				size_t msg_length = boost::asio::read(sock, boost::asio::buffer(msg, msglen));
				Json::Reader reader;
				reader.parse(std::string(msg, msg_length), root);
				std::cout << "msg id is " << root["id"] << " msg is " << root["data"] << endl;
			}
		}
		catch (std::exception &e)
		{
			std::cerr << "Exception: " << e.what() << endl;
		}
	}

	return 0;
}