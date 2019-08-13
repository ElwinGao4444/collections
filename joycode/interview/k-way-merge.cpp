/*
// =====================================================================================
// 
//       Filename:  k-way-merge.cpp
// 
//    Description:  题目：有20个数组，每个数组中有500个元素，每个元素为uint32_t类型
//    		求出这1W个元素中最小的100个
//    		解法：典型的多路归并算法，本程序将使用堆和败者树两种方法求解
//    		注意：该文件编译时需要使用c++0x标准，现行默认的C++标准缺少必要的库
// 
//        Version:  1.0
//        Created:  09/28/2012 08:35:20 PM
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
#include <cstdint>
#include <algorithm>
#include <vector>
#include <cassert>

#undef UINT32_MAX
#define UINT32_MAX 100

/*
// =====================================================================================
//        Class:  KWayMerge
//  Description:  使用堆和败者树两种方式实现多路归并
// =====================================================================================
*/
class KWayMerge {
public:
	// ====================  LIFECYCLE     =======================================
	KWayMerge();				// default constructor
	KWayMerge(int, int, int);		// constructor

	// ====================  INTERFACE     =======================================

	/* 
	// ===  FUNCTION  ======================================================================
	//         Name:  merge_by_heap
	//  Description:  使用堆进行多路归并
	// =====================================================================================
	*/
	const std::vector<uint32_t>& merge_by_heap();

	/* 
	// ===  FUNCTION  ======================================================================
	//         Name:  merge_by_tree
	//  Description:  使用败者树进行多路归并
	// =====================================================================================
	*/
	const std::vector<uint32_t>& merge_by_tree();

	// ====================  ACCESSORS     =======================================

	// ====================  MUTATORS      =======================================

	// ====================  OPERATORS     =======================================
	friend std::ostream& operator<<(std::ostream&, const KWayMerge&);

protected:
	// ====================  DATA MEMBERS  =======================================

private:
	// ====================  DATA MEMBERS  =======================================

	/* 
	// ===  FUNCTION  ======================================================================
	//         Name:  build_heap
	//  Description:  建立一个小根堆 （错误：应该是初始化堆，而非建堆）
	// =====================================================================================
	*/
	void build_heap();

	/* 
	// ===  FUNCTION  ======================================================================
	//         Name:  init_tree
	//  Description:  初始化败者树 
	// =====================================================================================
	*/
	void init_tree();

	/* 
	// ===  FUNCTION  ======================================================================
	//         Name:  compete_once
	//  Description:  败者树一次比较，选出最小值
	// =====================================================================================
	*/
	void compete_once();

	/* cmp函数不能为成员函数，隐含的this会导致参数匹配失败 */
	static bool cmp(uint32_t x, uint32_t y) { return x > y; }
	std::vector<uint32_t>::iterator& current(size_t index) { return m_heap[index]; }
	std::vector<uint32_t>::iterator& parent(size_t index) { return m_heap[index/2]; }
	std::vector<uint32_t>::iterator& left(size_t index) { return m_heap[index*2]; }
	std::vector<uint32_t>::iterator& right(size_t index) { return m_heap[index*2+1]; }

	/* 这里使用重载偷个懒，用于对m_tree进行操作 */
	int& current(int index) { return m_tree[index]; }
	int& parent(int index) { return m_tree[index/2]; }
	int& left(int index) { return m_tree[index*2]; }
	int& right(int index) { return m_tree[index*2+1]; }

	const uint32_t UINT32_MIN;
	/* 管理一个二维数组 */
	size_t m_num_of_array;
	size_t m_num_of_element;
	std::vector< std::vector<uint32_t> > m_source_list;	// 这里(> >)的空格是必须的，否则有可能导致编译错误
	/* 用于多路归并的数据结构，无论堆还是败者树实际上都是完全二插树结构 */
	std::vector<std::vector<uint32_t>::iterator> m_heap;
	std::vector<int> m_tree;
	/* 用于m_heap中保存最小值（0）的迭代器，虽然这个vector逻辑上是const的，但是不能定义成const容器，因为这会导致迭代器类型不匹配 */
	std::vector<uint32_t> m_guard;	
	/* 存储top n */
	size_t m_num_of_top;
	std::vector<uint32_t> m_dest_list;

};	// -----  end of class KWayMerge  -----

KWayMerge::KWayMerge() : UINT32_MIN(0), m_num_of_array(20), m_num_of_element(500), m_guard(1, 0), m_num_of_top(1000)
{
	srand((unsigned)time(NULL));
	for (size_t i=0; i!=m_num_of_array; ++i) {	// 推荐在循环外使用source_list.resize(num_of_array)一步到位的申请空间
		m_source_list.push_back(std::vector<uint32_t>(0));
		for (size_t j=0; j!=m_num_of_element; ++j) {	// 推荐在循环外使用source_list[i].resize(num_of_element)一步到位的申请空间
			m_source_list[i].push_back((uint32_t)rand() % UINT32_MAX);	// 强制类型转换优先级高于模运算
		}
		m_source_list[i].push_back(UINT32_MIN);	// 在每个数组最后放置一个最小值，作为哨兵
		/* 保证每个数组内部有序 */
		std::sort(m_source_list[i].begin(), m_source_list[i].end(), cmp);
	}
}		// -----  end of function KWayMerge  -----

KWayMerge::KWayMerge(int num_of_array, int num_of_element, int num_of_top) : UINT32_MIN(0), m_guard(1, 0)
{
	m_num_of_array = num_of_array;
	m_num_of_element = num_of_element;
	m_num_of_top = num_of_top;

	srand((unsigned)time(NULL));
	for (size_t i=0; i!=m_num_of_array; ++i) {	// 推荐在循环外使用source_list.resize(num_of_array)一步到位的申请空间
		m_source_list.push_back(std::vector<uint32_t>(0));
		for (size_t j=0; j!=m_num_of_element; ++j) {	// 推荐在循环外使用source_list[i].resize(num_of_element)一步到位的申请空间
			m_source_list[i].push_back((uint32_t) rand() % UINT32_MAX);	// 强制类型转换优先级高于模运算
		}
		m_source_list[i].push_back(UINT32_MIN);	// 在每个数组最后放置一个最小值，作为哨兵
		/* 保证每个数组内部有序 */
		std::sort(m_source_list[i].begin(), m_source_list[i].end(), cmp);
	}
}		// -----  end of function KWayMerge  -----

const std::vector<uint32_t>& KWayMerge::merge_by_heap()
{
	/* 
	 * 说明：
	 * 	虽然我们在这里使用了一些小技巧，例如哨兵之类，但目的仅仅是为了简化编程，没有更多的算法功能
	 * 	使用堆进行多路归并的核心思想就是使用堆可以在logn的复杂度内，找到多个数中的最大值
	 * 	这就使得，使用堆进行多路归并的复杂度为k*logn，k为题目要求的top个数，n为归并的路数乘2
	 *
	 * 特别注意：
	 * 	对于n（n为基数）个叶子节点的完全二叉树，有两种建法，一种是最后的叶节点为左节点的情况，另一种
	 * 	是最后的叶节点为右节点的情况，我们使用的是第二种情况，这样建堆仅仅为了方便编程
	 */

	/* 初始化堆，并将叶子节点赋值，需要注意，堆中存放的是各个vector的迭代器 */
	/* 我们构造的完全二叉树：非叶子节点个数 = 叶子节点个数 - 1 */
	m_heap.resize(m_num_of_array, m_guard.begin());	// index为0的元素仅起到占位的作用
	for (size_t i=0; i!=m_num_of_array; ++i) m_heap.push_back(m_source_list[i].begin());

	for (size_t i=0; i!=m_num_of_top; ++i) {
#ifdef DEBUG
		for (size_t i=0; i!=m_heap.size(); ++i)	std::cout << *m_heap[i] << ' '; std::cout << std::endl;
#endif
		build_heap();	// 使用堆的方法挑选最大值
#ifdef DEBUG
		for (size_t i=0; i!=m_heap.size(); ++i)	std::cout << *m_heap[i] << ' '; std::cout << std::endl;
#endif
		m_dest_list.push_back(*m_heap[1]);	// 将最值存储到结果数组中
		if (*m_heap[1] != UINT32_MIN) ++m_heap[1];	// 当迭代器所指的值为最小值时，迭代器不再移动，防止越界
#ifdef DEBUG
		for (size_t i=0; i!=m_heap.size(); ++i)	std::cout << *m_heap[i] << ' '; std::cout << std::endl; std::cout << std::endl;
#endif
	}
	return m_dest_list;
	/* 
	 * 再次阅读代码的评价：
	 * build_heap实际上为初始化堆的过程，而不是建堆的过程
	 * 堆在初始化过一次之后，后面选择最值的过程只要关心一条路径即可，无需关注其他节点
	 * 所以该函数虽然正确，并且看上去代码简洁，但实际上效率是非常糟糕的
	 */
}		// -----  end of function merge_by_heap  -----

const std::vector<uint32_t>& KWayMerge::merge_by_tree()
{
	/* 
	 * 说明：
	 * 	败者树的实现与堆的实现共用一类数据结构
	 * 	败者树建立完全二插树的方式于堆完全一样，二者结构上完全一样，但是保存的内容却不相同
	 * 	堆每个节点除了guard就是实际的数值，而败者树所有节点保存的是数据的下标，而不是数据的值
	 * 	败者树不能保存数值的原因为找到最终的胜者之后，无法根据胜者的数值找到胜者的位置，进行下一次操作
	 */

	/* 
	 * 注意：
	 * 	由于作者现在对败者树的实现并不熟练，
	 * 	所以当前算法属于实验性，硬编码，只允许8个叶子节点的情况
	 * 	参考链接：http://sandbox.mc.edu/~bennet/cs402/lec/llout/wrap000.html
	 */
	assert(m_source_list.size() == 8);
	m_tree.resize(m_num_of_array, 0);	// index为0的元素仅起到占位的作用
	for (size_t i=0; i!=m_num_of_array; ++i) m_tree.push_back((int)i);
#ifdef DEBUG
	for (size_t i=0; i!=m_tree.size(); ++i)	std::cout << m_tree[i] << ' '; std::cout << std::endl;
#endif
	init_tree();
#ifdef DEBUG
	for (size_t i=0; i!=m_tree.size(); ++i)	std::cout << m_tree[i] << ' '; std::cout << std::endl;
#endif
	for (int i=0; i<13; ++i) {
		compete_once();
		m_dest_list.push_back(m_source_list[m_tree[0]][0]);
		if (m_source_list[m_tree[0]].size() == 1) {
			m_source_list[m_tree[0]][0] = 1024;
		} else {
			m_source_list[m_tree[0]].erase(m_source_list[m_tree[0]].begin());
		}
	}

	return m_dest_list;
}		// -----  end of function merge_by_tree  -----

void KWayMerge::build_heap()
{
	/* 
	 * 说明：
	 * 	在元素移动时，可以两个元素比较（一父一子），也可以三个元素比较（一父两子）
	 * 	但是三个元素比较，可以有效的减少不必要的元素移动，所以我们这里使用三个元素比较的方法
	 * 	在建堆的过程中，我们始终保持j为某个子树的左孩子（j必为偶数），这样比较方便计算
	 */

	/* 数组下标为0的首元素不参与建堆，有效下标从1开始 */
	/* 在左右孩子节点和父节点中取最大值放在父节点上，节省不必要的元素移动 */
	size_t i=m_heap.size()/2+1;
	i += i % i;	// 处理第一个叶子节点为单独右节点的情况
	/* 此处i一定为左节点（偶数），但有可能不是第一个叶子节点，而是最后一个非叶子节点 */
	for (; i<m_heap.size(); i+=2) {
		size_t j = i;
		while (j != 1) {	// 到根节点退出判断
			j -= j % 2;	// 保证当前节点一定是左节点（偶数）
			/* 堆中存储的只是迭代器，需要解引用获得值 */
			size_t bigger = *current(j) > *current(j+1) ? j : j+1;
			if (*parent(bigger) < *current(bigger)) {	// 将最大值移动到父节点
				std::vector<uint32_t>::iterator tmp = parent(bigger);
				parent(bigger) = current(bigger);
				current(bigger) = tmp;
			}
			j /= 2;		// 将当前节点移动到父节点
		}
	}
	/* 
	 * 再次阅读代码的心得：
	 * 首先指出一个错误，该函数应该叫做init_heap，而不是build_heap，原因在merge_by_heap中有说明
	 * 另外，按照这种初始化堆的方式来看，实际上胜者树与初始化堆（或建堆）的过程非常类似
	 * 唯一的区别就是，堆在比较之后是做swap操作，而胜者树是直接用子节点给父节点赋值
	 * 除此以外，其余的操作都是一样的
	 */
}		// -----  end of function build_heap  -----

void KWayMerge::init_tree()
{
	/* 
	 * 败者树的思路很容易理解，但是使用败者树的难点之一就是如何构造一个败者树，即如何初始化树中的节点，一遍后续的运算
	 * 作者自己的方法是先构造胜者树，然后在胜者树的基础之上，再构造败者树，这样间接的完成败者树的构造过程
	 * 至于其他方法，目前作者还没有参考其他人的代码，如果有什么更好的方法，作者会以注释的形式在代码中描述出来，加以对比
	 */
	m_source_list[0][0] = 87;	m_source_list[0][1] = 99;	m_source_list[0][2] = 104;	m_source_list[0][3] = 119;	m_source_list[0][4] = 1024;
	m_source_list[1][0] = 48;	m_source_list[1][1] = 56;	m_source_list[1][2] = 88;	m_source_list[1][3] = 97;       m_source_list[1][4] = 1024;
	m_source_list[2][0] = 98;	m_source_list[2][1] = 104;	m_source_list[2][2] = 128;	m_source_list[2][3] = 151;      m_source_list[2][4] = 1024;
	m_source_list[3][0] = 58;	m_source_list[3][1] = 70;	m_source_list[3][2] = 76;	m_source_list[3][3] = 100;      m_source_list[3][4] = 1024;
	m_source_list[4][0] = 33;	m_source_list[4][1] = 91;	m_source_list[4][2] = 156;	m_source_list[4][3] = 205;      m_source_list[4][4] = 1024;
	m_source_list[5][0] = 48;	m_source_list[5][1] = 55;	m_source_list[5][2] = 60;	m_source_list[5][3] = 68;       m_source_list[5][4] = 1024;
	m_source_list[6][0] = 44;	m_source_list[6][1] = 55;	m_source_list[6][2] = 66;	m_source_list[6][3] = 77;       m_source_list[6][4] = 1024;
	m_source_list[7][0] = 80;	m_source_list[7][1] = 96;	m_source_list[7][2] = 106;	m_source_list[7][3] = 113;      m_source_list[7][4] = 1024;

	/* 构造胜者树 */
	/* 注意：这里必须用int，否则重载函数的调用会出问题 */
	for (int i=15; i>1; i-=2) {
		if (m_source_list[m_tree[i-1]][0] < m_source_list[m_tree[i]][0])
			parent(i) = m_tree[i-1];
		else
			parent(i) = m_tree[i];
	}

#ifdef DEBUG
	for (size_t i=0; i!=m_tree.size(); ++i)	std::cout << m_tree[i] << ' '; std::cout << std::endl;
#endif
	/* 在胜者树的基础上，构建败者树 */
	m_tree[0] = m_tree[1];	// 将最终胜者放到首位上
	for (int i=1; i<8; ++i) {
		if (m_source_list[left(i)][0] > m_source_list[right(i)][0])
			current(i) = left(i);
		else
			current(i) = right(i);
	}
#ifdef DEBUG
	for (size_t i=0; i!=m_tree.size(); ++i)	std::cout << m_tree[i] << ' '; std::cout << std::endl;
#endif

	return;
}		// -----  end of function init_tree  -----

void KWayMerge::compete_once()
{
	int condidate = 0; 	// 用于记录当前比较的节点
	int loser = 0;		// 上次比较的败者，用于与condidate进行比较

	/* 这里需要时刻注意一点：condidate是m_source_list的下标，而loser是m_tree的下标 */
	condidate = m_tree[0];		// 定位起始的叶子节点
	loser = (8 + m_tree[0]) / 2;	// 定位第一个loser

	while (loser > 0) {
		while (loser > 0 && m_source_list[condidate][0] < m_source_list[current(loser)][0]) 
			loser /= 2;
		if (loser > 0) {
			int temp = condidate;
			condidate = current(loser);
			current(loser) = temp;
			loser /= 2;
		}
	}
	m_tree[0] = condidate;

	return;
}		// -----  end of function compete_once  -----

std::ostream& operator<<(std::ostream &out, const KWayMerge &obj)
{
	out << "num_of_array: " << obj.m_num_of_array << "\t\t";
	out << "num_of_element: " << obj.m_num_of_element << "\t\t";
	out << "num_of_top: " << obj.m_num_of_top << std::endl;
	for (size_t i=0; i!=obj.m_source_list.size(); ++i) {
		for (size_t j=0; j!=obj.m_source_list[i].size(); ++j) {
			out << obj.m_source_list[i][j] << '\t';
		}
		out << std::endl;
	}
	return out;
}		// -----  end of function operator<<  -----

/* 
// ===  FUNCTION  ======================================================================
//         Name:  main
//  Description:  test driver
// =====================================================================================
*/
int main(int argc, char *argv[])
{
	KWayMerge obj1(8, 10, 10);
	KWayMerge obj2(8, 4, 10);
	std::vector<uint32_t> result;

#ifdef DEBUG
	std::cout << obj1;
	std::cout << "----------------------------------------------------------------" << std::endl;
	std::cout << obj2;
	std::cout << std::endl;
#endif

	//	result = obj1.merge_by_heap();
	//	std::cout << "result by heap:" << std::endl;
	//	for (size_t i=0; i!=result.size(); ++i)
	//		std::cout << result[i] << '\t';
	//	std::cout << std::endl;

	result = obj2.merge_by_tree();
	std::cout << "result by tree:" << std::endl;
	for (size_t i=0; i!=result.size(); ++i)
		std::cout << result[i] << '\t';
	std::cout << std::endl;
	return EXIT_SUCCESS;
}				// ----------  end of function main  ----------
