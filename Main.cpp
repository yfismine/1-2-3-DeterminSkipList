#include"DeterminSkipList.h"
#include<iostream>
using namespace std;
using SkipList::DeterminSkipList;
int main(void)
{
	int max;
	cout << "请输入跳跃表的最大值：" << endl;
	cin >> max;
	DeterminSkipList List(max);
	int select;
	while (1)
	{
		cout << "1：插入 2：删除结点  3：打印 4：清空跳跃表 5：退出" << endl;
		cout << "请输入选择" << endl;
		try
		{
			cin >> select;
			if (!cin)
				throw runtime_error("输入的编号有误！");
		}
		catch (runtime_error err)
		{
			cout << err.what()
				<< "\n是否退出或者重新输入"
				<< "\nY:退出   N:重新输入" << endl;
			string c;
			cin.clear();
			cin.ignore(100, '\n');
			cin >> c;
			if (!cin || c == "Y")
				break;
			else
				continue;

		}
		switch (select)
		{
		case 1: insert(List,max); break;
		case 2: remove(List,max); break;
		case 3: printList(List); break;
		case 4: List.makeEmpty(); break;
		case 5: return 0;
		default:cout << "编号输入错误！\n请重新输入" << endl; break;
		}
	}
}
