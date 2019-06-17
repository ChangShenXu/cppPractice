//#include "pch.h"
#include <iostream>
#include <string>
#include <memory>

using namespace std;
class StrVec {
public:
	StrVec() :
		elements(nullptr), first_free(nullptr), cap(nullptr) {}
	StrVec(const StrVec&);              // 拷贝构造函数
	StrVec &operator = (const StrVec&); // 拷贝赋值运算符
	StrVec(StrVec&&) noexcept;             // 移动构造函数
	StrVec& operator = (StrVec&&) noexcept; // 移动赋值运算符

	size_t size() const { return first_free - elements; }
	size_t capacity() const { return cap - elements; }
	std::string* begin() const { return elements; }
	std::string* end() const { return first_free; }
	void push_back(const std::string&);
	~StrVec();
private:
	static std::allocator<std::string> alloc;
	std::pair<std::string*, std::string*> alloc_n_copy
	(const std::string*, const std::string*);
	void reallocate();
	void chk_n_alloc() {
		if (size() == capacity()) reallocate();
	}
	void free();
	std::string* elements;
	std::string* first_free;
	std::string* cap;
};
std::allocator<std::string> StrVec::alloc;
void StrVec::push_back(const string& s) {
	chk_n_alloc();
	alloc.construct(first_free++, s);
}
/*
void StrVec::reallocate() {
	auto newcapacity = size() ? 2 * size() : 1;
	auto newdata = alloc.allocate(newcapacity);
	auto dest = newdata;
	auto elem = elements;
	for (size_t i = 0; i != size(); i++) {
		//alloc.construct(dest++, *elem++); // string的拷贝构造函数
		alloc.construct(dest++, std::move(*elem++)); // string的移动构造函数
	}
	free();
	elements = newdata;
	first_free = dest;
	cap = elements + newcapacity;
}
*/ // 注意！ 此处的 移动构造函数都是指 string类中的
void StrVec::reallocate() {
	cout << "reallocate." << endl;
	auto newcapacity = size() ? 2 * size() : 1;
	auto first = alloc.allocate(newcapacity);
	// 移动构造函数
	auto last = uninitialized_copy(make_move_iterator(begin()),
		make_move_iterator(end()), first);
	free();
	elements = first;
	first_free = last;
	cap = elements + newcapacity;
}
// 析构函数
void StrVec::free() {
	if (elements) {
		for (auto p = first_free; p != elements; ) {
			alloc.destroy(--p);
		}
		alloc.deallocate(elements, cap - elements);
	}
	return;
}
StrVec::~StrVec() { free(); }
// 拷贝构造函数
std::pair<string*, string*>
StrVec::alloc_n_copy(const string* b, const string* e) {
	auto data = alloc.allocate(e - b);
	return { data, uninitialized_copy(b,e,data) };
}
StrVec::StrVec(const StrVec &s) {
	cout << "copy constructor." << endl;
	auto newdata = alloc_n_copy(s.begin(), s.end());
	elements = newdata.first;
	first_free = newdata.second;  
	cap = newdata.second; // 一定不要忘记还有cap!! 未定义的cap将在析构时出错！！
}
// 拷贝赋值运算符
StrVec &StrVec::operator=(const StrVec &rhs) {
	cout << "copy operator =." << endl;
	auto data = alloc_n_copy(rhs.begin(), rhs.end());
	free();
	elements = data.first;
	first_free = cap = data.second;
	return *this;
}
// 移动构造函数
StrVec::StrVec(StrVec&& s) noexcept
	:elements(s.elements), first_free(s.first_free), cap(s.cap) {
	cout << "move constructor." << endl;
	s.elements = s.first_free = s.cap = nullptr;
}
// 移动赋值运算符
StrVec& StrVec::operator=(StrVec&& rhs) noexcept {
	cout << "move operator =." << endl;
	if (this != &rhs) {
		free();
		elements = rhs.elements;
		first_free = rhs.first_free;
		cap = rhs.cap;
		rhs.elements = rhs.first_free = rhs.cap = nullptr;
	}
	return *this;
}
int main()
{
	StrVec sv;
	sv.push_back("hello world!");
	StrVec sv2(sv);   // 调用 拷贝构造函数
	StrVec sv3 = sv;  // 依然调用 拷贝构造函数 
	StrVec sv4;
	sv4 = sv;         // 此时才调用 拷贝赋值运算符
	StrVec sv5(std::move(sv)); // 移动构造函数
	StrVec sv6;
	sv6 = std::move(sv);  // 移动赋值运算符
	cout << "hello world!" << endl;
	return 0;
}

