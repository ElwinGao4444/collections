/*
// =====================================================================================
// 
//       Filename:  ring-list.cpp
// 
//       Description:  带环链表问题：
//       1、如何判断一个链表是否有环路？（要考虑0和6两种情况）
//       2、如果链表带环，如何判断环的长度？
//       3、如果链表带环，如何判断柄的长度？
//       Reference:
//       http://www.360doc.com/content/12/0402/23/9568648_200376434.shtml
//
//       Reference:
//       http://www.360doc.com/content/12/0402/23/9568648_200376434.shtml
// 
//        Version:  1.0
//        Created:  09/27/2012 11:22:26 AM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Elwin.Gao (elwin), elwin.gao4444@gmail.com
//        Company:  
// 
// =====================================================================================
*/


#include <cstdlib>
#include <iostream>
#include <cassert>

struct node
{
	int data;
	struct node *next;
};				// ----------  end of struct node  ----------

/*
// =====================================================================================
//        Class:  List
//  Description:  该List类操作的链表类型为单向链表（含头节点），该类只负责创建符合条件的
//  链表，以及对链表进行环路的判断和相关计算，除此以外，不实现任何其他链表功能
// =====================================================================================
*/
class List {
public:
	// ====================  LIFECYCLE     =======================================
	List () :
		m_handle_size(0), 
		m_ring_size(0),
		m_list_head(NULL) {}		// constructor

	// 释放链表，避免内存泄漏
	~List();

	// ====================  INTERFACE     =======================================
	
	/* 
	// ===  FUNCTION  ======================================================================
	//         Name:  CreateList
	//  Description:  根据柄的长度和环的长度，创建满足要求的链表
	//   Parameters:  柄的长度（0表示创建一个循环链表）;
	//   		环的长度（0表示创建一个无环的链表）;
	//  ReturnValue:  创建成功返回0，否则返回-1
	// =====================================================================================
	*/
	int CreateList(int handle_size, int ring_size);

	/* 
	// ===  FUNCTION  ======================================================================
	//         Name:  CheckRing
	//  Description:  用于检测链表是否存在环路
	//   Parameters:  null
	//  ReturnValue:  返回值为NULL表示无环路，否则表示有环路，返回值的值表示快慢两个指针的重
	//  合地址（不一定是环路起点），这个值除了用于后续计算以外，没有其他意义
	// =====================================================================================
	*/
	struct node* CheckRing();

	/* 
	// ===  FUNCTION  ======================================================================
	//         Name:  CalculateRingSize
	//  Description:  计算链表中环的长度
	//   Parameters:  CheckRing()函数返回的指针，实际上就是环上某一个node的地址，是哪个node的
	//   地址算法上无所谓，关键是这个地址必须在环上
	//  ReturnValue:  返回环的长度，这个长度应该与CreateList()中第二个参数相等
	// =====================================================================================
	*/
	int CalculateRingSize(struct node *p);


	/* 
	// ===  FUNCTION  ======================================================================
	//         Name:  CalculateHandleSize
	//  Description:  计算链表中柄的长度 
	//   Parameters:  ChechRing()函数返回的指针，实际上就是环上某一个node的地址，这个node的位
	//   		置很重要，不能是任意一个地址，因为它要与head保持一定的代数关系，否则无法
	//   		正确运算
	//  ReturnValue:  返回柄的长度，这个长度应该与CreateList()中第一个参数加一相等（含头节点）
	// =====================================================================================
	*/
	int CalculateHandleSize(struct node *p);

	// ====================  ACCESSORS     =======================================

	// ====================  MUTATORS      =======================================

	// ====================  OPERATORS     =======================================

protected:
	// ====================  DATA MEMBERS  =======================================

private:
	// ====================  INTERFACE     =======================================
	
	// ====================  DATA MEMBERS  =======================================

	/* 
	// ===  FUNCTION  ======================================================================
	//         Name:  m_CreateList
	//  Description:  创建一个单循环链表，并返回头节点指针
	//   Parameters:  null
	//  ReturnValue:  创建成功返回0，否则返回-1
	// =====================================================================================
	*/
	int m_CreateList();

	/* 
	// ===  FUNCTION  ======================================================================
	//         Name:  m_AddNode
	//  Description:  向链表中添加一个新的node 
	//   Parameters:  需要写入的数值
	//  ReturnValue:  添加成功返回0，否则返回-1
	// =====================================================================================
	*/
	int m_AddNode(int n);

	int m_handle_size;
	int m_ring_size;
	struct node *m_list_head;

}; // -----  end of class List  -----

List::~List()
{
	struct node *p = m_list_head;
	int n = m_handle_size + m_ring_size + 1;	// 还有一个额外的头节点
	// 如果链表有环，则一般的链表释放方法将导致段错误
	for (int i=0; i<n; ++i) {
		struct node *tmp = p->next;
		free(p);
		p = tmp;
	}
}

int List::CreateList(int handle_size, int ring_size)
{
	struct node *head = NULL;
	m_handle_size = handle_size;
	m_ring_size = ring_size;

	srand((unsigned)time(0));
	try {
		/* 创建链表 */
		if (m_CreateList() == -1) return -1;
		/* 构造环路 */
		for (int i=0; i<ring_size; ++i)
			if (m_AddNode(rand()%100) == -1) return -1;
		struct node *p = m_list_head;
		while (p->next != NULL) p = p->next;
		p->next = m_list_head->next;

		/* 构造柄 */
		for (int i=0; i<handle_size; ++i)
			if (m_AddNode(rand()%100) == -1) return -1;
	} catch (std::bad_alloc &e) {
		std::cout << e.what();
		return -1;
	} catch (std::exception &e) {
		std::cout << e.what();
		return -1;
	}


	return 0;
}		// -----  end of function CreateList  -----

struct node* List::CheckRing()
{
	struct node *slow = m_list_head;
	struct node *fast = m_list_head;

	for ( ; ; ) {
		if (fast == NULL || fast->next == NULL) return NULL;
		slow = slow->next;
		fast = fast->next->next;
		if (slow == fast) return slow;
	}

	return NULL;
}		// -----  end of function CheckRing  -----

int List::CalculateRingSize(struct node *p)
{
	if (p == NULL) return 0;

	struct node *slow = p;
	struct node *fast = p;
	int n = 0;
	do {
		/* 这里不可能出现fast->NULL的情况 */
		assert(fast != NULL);
		assert(fast->next != NULL);
		slow = slow->next;
		fast = fast->next->next;
		++n;
	} while (slow != fast);

	return n;
}		// -----  end of function CalculateRingSize  -----

int List::CalculateHandleSize(struct node *p)
{
	if (p == NULL) {
		struct node *p = m_list_head;
		int n = 0;
		while (p != NULL) {
			p = p->next;
			++n;
		}
		return n;
	}

	struct node *slow = p;
	struct node *node = m_list_head;
	int n = 0;
	while (node != slow) {
		/* 这里不可能出现node==NULL或slow==NULL的情况 */
		assert(node != NULL);
		assert(slow != NULL);
		node = node->next;
		slow = slow->next;
		++n;
	}

	return n;
}		// -----  end of function CalculateHandleSize  -----

int List::m_CreateList()
{
	// 头节点的data区域用于保存链表中节点的个数
	m_list_head = (struct node*) malloc(sizeof(struct node));
	if (m_list_head == NULL) throw std::bad_alloc();
	m_list_head->data = 0;
	m_list_head->next = NULL;
	return 0;
}		// -----  end of function m_CreateList  -----

int List::m_AddNode(int n)
{
	if (m_list_head == NULL) return -1;
	struct node *p = (struct node*) malloc(sizeof(struct node));
	if (p == NULL) throw std::bad_alloc();
	p->next = m_list_head->next;
	p->data = n;
	m_list_head->next = p;
	++m_list_head->data;
	return 0;
}		// -----  end of function m_AddNode  -----

/* 
// ===  FUNCTION  ======================================================================
//         Name:  main
//  Description:  test driver
// =====================================================================================
*/

int main(int argc, char *argv[])
{
	if (argc != 3) {
		std::cerr << "Usage: " << argv[0] << " handle-size ring-size" << std::endl;
		exit(EXIT_FAILURE);
	}

	int handle_size = atoi(argv[1]);
	int ring_size = atoi(argv[2]);

	List l;
	l.CreateList(handle_size, ring_size);
	struct node *p = l.CheckRing();
	std::cout << "has ring?\t"<< (p ? "true" : "false") << std::endl;
	std::cout << "handle size: " << l.CalculateHandleSize(p) << std::endl;
	std::cout << "ring size: " << l.CalculateRingSize(p) << std::endl;
	return EXIT_SUCCESS;
}				// ----------  end of function main  ----------
