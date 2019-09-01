#include "DeterminSkipList.h"
#include<iostream>
using std::cout;
using std::endl;
using std::ends;
using std::cin;
using namespace SkipList;


DeterminSkipList::DeterminSkipList(int inf):maxNumber(inf)
{
	bottom = new SkipNode();
	bottom->right = bottom->down = bottom;
	tail = new SkipNode(maxNumber);
	tail->right = tail; tail->down = tail;
	header = new SkipNode(maxNumber, string("Null"), tail, bottom);
}

DeterminSkipList::~DeterminSkipList()
{
	makeEmpty();
	delete header, delete tail, delete bottom;
}

void DeterminSkipList::insert(int number, string name)    //插入过程中每一行都会多一个空结点
{
	SkipNode *current = header;
	bottom->number = number;  //确保if条件可以退出
	bottom->name = name;
	while (current!=bottom)
	{
		while (current->number < number)   //将current置于键值大于number的位置
			current = current->right;
		if (current->down->right->right->number < current->number)   //当下层的间隙容量为3时
		{
			current->right = new SkipNode(current->number, current->name, current->right, current->down->right->right);
			current->number = current->down->right->number;
			current->name = current->down->right->name;
		}
		else
			current = current->down;
	}
	if (header->right != tail)    //提升头结点高度保证下次插入的顺利进行
		header = new SkipNode(maxNumber,string("Null"), tail, header);
	nodeNumber++;
}

bool DeterminSkipList::contains(int number)
{
	SkipNode *current = header;
	bottom->number = number;
	while (1)
	{
		if (number < current->number)
			current = current->down;
		else if (number > current->number)
			current = current->right;
		else
			return current != bottom;
	}
}

bool DeterminSkipList::remove(int number)
{
	if (isEmpty())
		return false;        //空表的时候不能删除直接退出
	SkipNode *current, *previous, *temp, *next;     //current为当前结点，previous为当前结点的前导结点，next为当前结点的down结点
	/*这个赋值比较关键，不能是header,因为1.header并非实际
	 *结点如果表中存在结点的话第一个实际结点应该在header->
	 *down  2.就是如果从header开始后面有一个判断current->
	 right->down觉对会出错
	 */
	current = header->down;
	int nodeHeight = 0;     //统计待删除的结点的高度
	while(1)
	{
		previous = nullptr;
		while (number > current->number)   //使得当前结点置于大于或者等于number的结点位置
		{
			previous = current;
			current = current->right;
		}
		if (number == current->number)   //统计该删除结点的高度
			nodeHeight++;
		if (current->down == bottom)     //实际删除操作的执行部分，由于自顶向下的调整，所以不会出现间隙容量为1的删除情况
		{
			if (nodeHeight == 0)  //删除结点的高度为0说明该结点不存在
			{
				LowerHeadRemoveHelp();
				return false;
			}
			else
			{
				if (nodeHeight == 1)   //删除后不会出现像高层次结点删除后的高层结点值未调整的情况
				{
					/*将真正的删除结点转化为删除结点右结点，然后删除其右结点
					 *这样并不是多次一举，而是防止该删除结点为底层的第一个结点
					 *时删除这结点会导致链断开
					 */
					temp = current->right;
					current->number = current->right->number;
					current->name = current->right->name;
					current->right = temp->right;
					delete temp;
					LowerHeadRemoveHelp();
				}
				else
				{
					/*由于删除结点的层次大于1,所以在上层的结点中可能还存在该结点
					 *又因为可能该结点是高度最高结点中的唯一个，所以为了保险起见
					 *直接将其删除而由于他的结点高度不为1所以它在最低层一定是有
					 *previou结点的于是我们就用preious结点的值用以替换它
					*/
				    LowerHeadRemoveHelp();
					FindAndModifyRemoveHelp(current->number, previous->number,previous->name);
					previous->right = current->right;
					delete current;
				}
				nodeNumber--;
				return true;
			}
		}
		next = current->down;  //current在后面的操作中会改变，next保存当前结点的后续
		if (current->number == current->down->right->number&&nodeNumber!=1)    //后续结点的间隙容量为1时需要进行扩容操作且只有一个结点时不进行扩容操作
		{
			if (previous == nullptr)   //没有前导结点说明当前结点是该层的第一个结点，扩容操作只能从其后续结点中考虑
			{
				if (current->right->number > current->right->down->right->number)  //其后续结点的下层间隙容量大于1，可以借出
				{
					current->number = current->right->down->number;   //相当于把current->right->down的高度进行了提升,current高度降低
					current->name = current->right->down->name;
					current->right->down = current->right->down->right;
				}
				else if(current->right!=tail)    //后续结点的下层间隙容量也为1，就将其自身结点高度降低 考虑要排除降低tail的可能
				{
					temp = current->right;
					current->number = temp->number;
					current->name = temp->name;
					current->right = temp->right;
					delete temp;
				}
			}
			else  //当前结点有前导结点
			{
				if (previous->number > previous->down->right->number)     //前导结点的下层间隙容量大于1
				{
					temp = previous->down->right;
					if (temp->right->number != previous->number)  //保证temp是最邻近current的下层结点
						temp = temp->right;
					previous->number = temp->number;  //相当于把preious的高度降低，temp的高度提升
					previous->name = temp->name;
					current->down = temp->right;
				}
				else   //后续结点的下层间隙容量也为1，就将其前导结点高度降低
				{
					previous->number = current->number;
					previous->name = current->name;
					previous->right = current->right;
					delete current;
				}
			}
		}
		current = next; //进行下一层的查找
	}
}

bool DeterminSkipList::isEmpty()
{
	if (header->down == bottom)
		return true;
	else
		return false;
}

void SkipList::DeterminSkipList::makeEmpty()
{
	SkipNode *current = header->down;
	SkipNode *downNext,*rightNext;
	while (current != bottom)
	{
		downNext = current->down;
		while (current->number!=maxNumber)
		{
			rightNext = current->right;
			delete current;
			current = rightNext;
		}
		delete current;
		current = downNext;
	}
	header->down = bottom;
}

void SkipList::DeterminSkipList::printSkipList()
{
	SkipNode *current = header->down;
	SkipNode *next;
	while (current!=bottom)
	{
		next = current->down;
		while (current->number!=maxNumber)
		{
			cout << "(" << current->number << "," << current->name << ")" << ends;
			current = current->right;
		}
		cout << endl;
		current = next;
	}
	cout << endl;
}

void DeterminSkipList::FindAndModifyRemoveHelp(int removeNumber, int replaceNumber, string replaceName)
{
	SkipNode *current;
	current = header;
	while (current != bottom)
	{
		while (removeNumber > current->number)
			current = current->right;
		if (removeNumber == current->number)
		{
			current->number = replaceNumber;
			current->name = replaceName;
		}
		current = current->down;
	}
}

void DeterminSkipList::LowerHeadRemoveHelp()
{
	SkipNode *temp;
	if (header->down->right == tail)      //在remove成功的情况下，如果tail结点只有一个结点必然是”伪尾结点“，此时就需要降低header的高度
	{
		temp = header->down;
		header->down = temp->down;
		delete temp;
	}
}

void SkipList::insert(DeterminSkipList & List,const int max)
{
	int number;
	string name;
	cout << "请输入待插入结点的键值和名称：" << endl;
	cin >> number >> name;
	if (number >= max)
	{
		cout << "插入失败：该键值过大！" << endl;
		return;
	}
	else if (List.contains(number))
	{
		cout << "插入失败：该键值已经存在！" << endl;
		return;
	}
	List.insert(number, name);
	cout << "插入成功！" << endl;
}

void SkipList::remove(DeterminSkipList & List,const int max)
{
	int number;
	cout << "请输入待删除结点的键值：" << endl;
	cin >> number;
	if (number >= max)
	{
		cout << "删除失败：删除结点不存在！" << endl;
		return;
	}
	if (List.remove(number))
		cout << "删除成功！" << endl;
	else
		cout << "删除失败！" << endl;
}

void SkipList::printList(DeterminSkipList & List)
{
	if (List.isEmpty())
		cout << "打印失败：跳跃表为空，无法打印！" << endl;
	else
		List.printSkipList();
}
