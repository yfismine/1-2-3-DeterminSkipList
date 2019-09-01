#ifndef _DETERMINSKIPLIST_
#define _DETERMINSKIPLIST_
#pragma once
#include<string>
using std::string;
namespace SkipList
{
	constexpr unsigned max = -1;
	class DeterminSkipList
	{
	public:
		explicit DeterminSkipList(const int inf = max);
		~DeterminSkipList();
		void insert(int number, string name);
		bool contains(int number);
		bool remove(int number);
		bool isEmpty();
		void makeEmpty();
		void printSkipList();
	private:
		struct SkipNode
		{
			int number = 0;
			string name="Null";
			SkipNode *right = nullptr;
			SkipNode *down = nullptr;
			SkipNode() = default;
			SkipNode(int num, string nam = string("Null"), SkipNode *rt = nullptr, SkipNode *dw = nullptr)
				:number(num), name(nam), right(rt), down(dw) {};
		};
		int maxNumber;
		int nodeNumber = 0;
		SkipNode *header;
		SkipNode *bottom;
		SkipNode *tail;
		void FindAndModifyRemoveHelp(int removeNumber, int replaceNumber, string replaceName);  //调整辅助函数
		void LowerHeadRemoveHelp();   //降低header，tail结点高度的辅助函数
	};
	void insert(DeterminSkipList &List,const int max);
	void remove(DeterminSkipList &List,const int max);
	void printList(DeterminSkipList &List);
}
#endif

