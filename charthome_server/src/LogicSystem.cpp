#include "LogicSystem.h"

using namespace std;

LogicSystem::LogicSystem():_b_stop(false){
	RegisterCallBacks();
	_worker_thread = std::thread (&LogicSystem::DealMsg, this);
}

LogicSystem::~LogicSystem(){
	_b_stop = true;
	_consume.notify_one();
	_worker_thread.join();
}

void LogicSystem::PostMsgToQue(shared_ptr < LogicNode> msg) {
	std::unique_lock<std::mutex> unique_lk(_mutex);
	_msg_que.push(msg);
	
	if (_msg_que.size() == 1) {
		unique_lk.unlock();
		_consume.notify_one();
	}
}

void LogicSystem::DealMsg() {
	for (;;) {
		std::unique_lock<std::mutex> unique_lk(_mutex);
		//
		while (_msg_que.empty() && !_b_stop) {
			_consume.wait(unique_lk);
		}

		//
		if (_b_stop ) {
			while (!_msg_que.empty()) {
				auto msg_node = _msg_que.front();
				cout << "recv_msg id  is " << msg_node->_recvnode->_msg_id << endl;
				auto call_back_iter = _fun_callbacks.find(msg_node->_recvnode->_msg_id);
				if (call_back_iter == _fun_callbacks.end()) {
					_msg_que.pop();
					continue;
				}
				call_back_iter->second(msg_node->_session, msg_node->_recvnode->_msg_id,
					std::string(msg_node->_recvnode->_data, msg_node->_recvnode->_cur_len));
				_msg_que.pop();
			}
			break;
		}

		//
		auto msg_node = _msg_que.front();
		cout << "recv_msg id  is " << msg_node->_recvnode->_msg_id << endl;
		auto call_back_iter = _fun_callbacks.find(msg_node->_recvnode->_msg_id);
		if (call_back_iter == _fun_callbacks.end()) {
			_msg_que.pop();
			continue;
		}
		call_back_iter->second(msg_node->_session, msg_node->_recvnode->_msg_id, 
			std::string(msg_node->_recvnode->_data, msg_node->_recvnode->_cur_len));
		_msg_que.pop();
	}
}

void LogicSystem::RegisterCallBacks() {
	/*_fun_callbacks[MSG_HELLO_WORD] = std::bind(&LogicSystem::HelloWordCallBack, this,
		placeholders::_1, placeholders::_2, placeholders::_3);*/
	_fun_callbacks[MSG_HELLO_WORD - 1] = std::bind(&LogicSystem::ChatReturnsCallBack, this,
		placeholders::_1, placeholders::_2, placeholders::_3);//1000
	_fun_callbacks[MSG_HELLO_WORD] = std::bind(&LogicSystem::AddNameCallBack, this,
		placeholders::_1, placeholders::_2, placeholders::_3);//1001
	
}

void LogicSystem::HelloWordCallBack(shared_ptr<CSession> session, const short &msg_id, const string &msg_data) {
	Json::Reader reader;
	Json::Value root;
	reader.parse(msg_data, root);
	std::cout << "recevie msg id  is " << root["id"].asInt() << " msg data is "
		<< root["data"].asString() << endl;
	root["data"] = "msg data is " + root["data"].asString();
	std::string return_str = root.toStyledString();
	session->Send(return_str, root["id"].asInt());
	
	
}
void LogicSystem::AddNameCallBack(shared_ptr<CSession> session, const short &msg_id, const string &msg_data) {
	Json::Reader reader;
	Json::Value root;
	reader.parse(msg_data, root);
	session->setUeserName(root["data"].asString());
	session->StartHandle();

}

void LogicSystem::ChatReturnsCallBack(shared_ptr<CSession> session, const short &msg_id, const string &msg_data) {
	std::cout << msg_id << endl;
	//unsigned short  host_msgid = boost::asio::detail::socket_ops::host_to_network_short(msg_id);
	//unsigned short host_datasize = boost::asio::detail::socket_ops::host_to_network_short();
	Json::Reader reader;
	Json::Value root;
	reader.parse(msg_data, root);
	std::cout  << "msgid is : " << root["id"].asInt() << " msg data is "
	<< root["data"].asString() << endl;
	root["data"] = session->getUeserName() + ":" + root["data"].asString();
	
	std::string return_str = root.toStyledString();
	auto sessions = session->GetServer()->GetSessions();
	for(auto it = sessions.begin(); it != sessions.end(); it++)
	{
		it->second->Send(return_str, msg_id);
	}
}
