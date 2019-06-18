// MyfirstApp.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <vector>
#include <string>
#include <memory>

using namespace std;

class StrBlob {
public:
	typedef std::vector<std::string>::size_type size_type;
	StrBlob();
	StrBlob(std::initializer_list<string>);

	size_type size() const { return data->size(); }
	bool empty() const { return data->empty(); }

	void push_back(const std::string &t) { 
		cout << "push back an element." << endl;
		data->push_back(t); 
	}
	void pop_back();
	void show();
	std::string& front();
	std::string& back();
private:
	// 希望该类的不同拷贝之间共享 该数据
	std::shared_ptr<std::vector<std::string>> data;
	
	void check(size_type i, const std::string &msg) const;
};
StrBlob::StrBlob():data(make_shared<vector<string>>()) {
	cout << "constructor 1" << endl;
}
StrBlob::StrBlob(std::initializer_list<string> il):
	data(make_shared<vector<string>>(il)){
	cout << "constructor 2" << endl;
}

void StrBlob::check(size_type i, const string &msg) const {
	if (i >= data->size()) {
		throw out_of_range(msg);
	}
}

string& StrBlob::front() {
	check(0, "front on empty StrBlob.");
	return data->front();
}
string& StrBlob::back() {
	check(0, "back on empty StrBlob.");
	return data->back();
}
void StrBlob::pop_back() {
	check(0, "pop_back on empty StrBlob.");
	data->pop_back();
}
void StrBlob::show() {
	for (int i = 0; i < (int)size(); i++) {
		cout << data->at(i) << " ";
	}
	cout << endl;
}
int main() {
	StrBlob sv = {"I","wanna","say"};
	StrBlob sv2;
	sv2 = sv;
	sv2.push_back("hello world!");
	sv.show();  // sv2和sv 共享数据
	return 0;
}
