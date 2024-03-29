#include "pch.h"
#include <iostream>
#include <vector>
#include <string>
#include <memory>

using namespace std;
class StrBlobPtr;
class StrBlob {
public:
	friend class StrBlobPtr;
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
	StrBlobPtr begin();
	StrBlobPtr end();
private:
	// 希望该类的不同拷贝之间共享 该数据
	std::shared_ptr<std::vector<std::string>> data;

	void check(size_type i, const std::string &msg) const;
};
StrBlob::StrBlob() :data(make_shared<vector<string>>()) {
	cout << "constructor 1" << endl;
}
StrBlob::StrBlob(std::initializer_list<string> il) :
	data(make_shared<vector<string>>(il)) {
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

// 该类的目的 是什么？
class StrBlobPtr {
public:
	friend class StrBlob;
	StrBlobPtr() :curr(0) { cout << "StrBlobPtr constructor 1" << endl; }
	StrBlobPtr(StrBlob &a, size_t sz = 0) :
		wptr(a.data), curr(sz) {
		cout << "StrBlobPtr constructor 2" << endl;
	}
	std::string& deref() const; // 解引用
	StrBlobPtr& incr(); // 前缀递增
private:
	// 检查当前wptr指向的对象是否存在，给定索引是否合法
	std::shared_ptr<std::vector<std::string>>
		check(std::size_t, const std::string&) const;

	std::weak_ptr<std::vector<std::string>> wptr;
	std::size_t curr; // 数组中的当前位置
};
std::shared_ptr<std::vector<std::string>>
StrBlobPtr::check(std::size_t i, const std::string& msg) const {
	auto ret = wptr.lock();
	if (!ret)
		throw std::runtime_error("unbound StrBlobPtr");
	if (i >= ret->size())
		throw std::out_of_range(msg);
	return ret;
}
std::string& StrBlobPtr::deref() const {
	auto p = check(curr, "dereference past end");
	return (*p)[curr];
}
StrBlobPtr& StrBlobPtr::incr() {
	check(curr, "increment past and end of StrBlobPtr");
	++curr;
	return *this;
}
// can't be defined until StrBlobPtr.
StrBlobPtr StrBlob::begin() { 
	cout << "StrBlob::begin()" << endl;
	return StrBlobPtr(*this); 
} 
StrBlobPtr StrBlob::end() {
	auto ret = StrBlobPtr(*this, data->size());
	return ret;
}

int main() {
	StrBlobPtr svp;
	{
		StrBlob sv = { "I","wanna","say" };
		StrBlob sv2;
		sv2 = sv;
		sv2.push_back("hello world!");
		sv.show();  // sv2和sv 共享数据
		StrBlobPtr svp2(sv);
		svp = sv.begin();
		string s = svp.deref();
		if(!s.empty()) cout << s << endl;
	}
	string s2 = svp.deref(); // 此时，底层数据已经被释放了
	//if (!s2.empty()) cout << s2 << endl;
	return 0;
}
