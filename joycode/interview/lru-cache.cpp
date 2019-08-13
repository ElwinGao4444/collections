/*
// =====================================================================================
// 
//       Filename:  lru-cache.cpp
// 
//    Description:  多线程安全的LRUCache实现
// 
//        Version:  1.0
//        Created:  2017/10/05 16时47分16秒
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
#include <string>
#include <atomic>
#include <memory>
#include <mutex>
#include <map>

// =====================================================================================
//        Class:  LRUCache
//  Description:  这里的LRUCache采用库的形式，只能在确保多线程调用安全的情况下，
//                尽可能的从数据结构层面支持高并发操作
//                如果做成服务形式，则可以在进程和线程层面做到更多的优化
//                比如，在充分考虑到CPU Cache的命中率情况下，可以将线程绑定到CPU上
//                从而避免Cache换入换出对性能的影响
//                同时，还可以将单一Hash表变为多Hash表，每个线程负责一个Hash表的操作，
//                而LRU淘汰策略可以对多个链表进行K路归并，这样做的好处是避免了线程之间
//                大量争抢临界资源造成的性能损失
//                最后，还可以在LRU淘汰时，选择批量淘汰，而非一次淘汰一个，这样也可以
//                充分利用K路归并的结果，将堆中的数据全部淘汰，达到提升性能的作用
// =====================================================================================
class LRUCache
{
public:
	// 将哈希表与链表统一存储
	// 哈希表用户快速查找
	// 双向循环链表用于记录LRU序列
	struct Node {
		uint64_t hash;		// key的hash值，用于加速比较
		std::string key;
		std::string value;
		struct Node *before;	// 链表前置节点
		struct Node *after;	// 链表后置节点
		struct Node *next;	// 哈希表next节点
	};

	// 使用hash表结构，必然会有相同分桶内的操作，需要进行互斥的问题
	// 此处使用了简单的自旋锁方式，当然也可以根据不同的使用场景，选择不同的锁
	// 比如对于大量读，少量写的场景，使用读写锁会有更好的读性能
	struct ArrayNode {
		ArrayNode() : spin_lock(ATOMIC_FLAG_INIT), hash_head(nullptr) {}
		void lock() {
			while (spin_lock.test_and_set(std::memory_order_acquire));
		}
		void unlock() {
			spin_lock.clear(std::memory_order_release);
		}

		// 这里使用atomic_flag实现自旋锁，相比mutex代价更低
		// 但这种方式更加适合value较小的情况，如果value非常大，value拷贝时间过长
		// 有可能导致多个线程长时间空转，此时不如使用mutex，让出CPU
		std::atomic_flag spin_lock;	// 自旋锁
		struct Node *hash_head;		// 链表首地址
	};

public:
	// ====================  LIFECYCLE     =======================================
	LRUCache(size_t array_size, size_t key_max) :
		array_size(array_size),
		key_max(key_max),
		size(0)
	{
		// 使用定长数组的问题在于可扩展性比较差，一旦数组长度改变，数据迁移代价太大
		// 此处也可以选择使用map代替数组，这样就可以模拟一个2^64长度的数组
		// 如果使用map结构，那么就解决插入和删除过程中互斥的问题
		// 最简单的方式就是挂全局锁，但是这种方式代价太大，难以支持很高的并发
		// 比较可行的方案是使用bitmap对2^64长度的数组进行划分，划分的粒度越细，占用的空间越大
		// 但是锁的影响面越小，加之2^64次方长度的数组，冲突概率极低，并不需要太长的bitmap
		hash_table = new(std::nothrow) ArrayNode[array_size];

		// 创建双向循环链表的Guar节点
		link_head = new(std::nothrow) struct Node;
		link_head->before = link_head;
		link_head->after = link_head;
	}

	~LRUCache()
	{
	}

	// ====================  INTERFACE     =======================================

	/* 
	// ===  FUNCTION  ======================================================================
	//         Name:  set
	//  Description:  success return 0, cover return 1, error return -1
	// =====================================================================================
	*/
	int set(std::string key, std::string value)
	{
		std::size_t hash = std::hash<std::string>{}(key);
		std::size_t index = hash % array_size;
		std::lock_guard<struct ArrayNode> guard(hash_table[index]);

		Node *p = hash_table[index].hash_head;
		while (p != nullptr) {
			// exist key
			if (p->hash == hash) {
				p->value = value;
				remove_link_node(p);
				add_to_link_head(p);
				return 1;
			}
			p = p->next;
		}

		// delete key as LRU
		if (size.load() == key_max) {
			p = get_link_tail();
			std::size_t tail_index = p->hash % array_size;
			remove_link_node(p);
			remove_from_hash_table(p->hash, index, tail_index);
			delete p;
		}
		// add new key
		p = new(std::nothrow) struct Node;
		p->hash = hash;
		p->key = key;
		p->value = value;
		p->next = hash_table[index].hash_head;
		hash_table[index].hash_head = p;
		add_to_link_head(p);
		++size;
		return 0;
	}		// -----  end of function set  -----

	/* 
	// ===  FUNCTION  ======================================================================
	//         Name:  get
	//  Description:  success return value, non-exist return empty string
	// =====================================================================================
	*/
	std::string get(std::string key)
	{
		std::size_t hash = std::hash<std::string>{}(key);
		std::size_t index = hash % array_size;
		std::lock_guard<struct ArrayNode> guard(hash_table[index]);

		Node *p = hash_table[index].hash_head;
		while (p != nullptr) {
			// exist key
			if (p->hash == hash) {
				remove_link_node(p);
				add_to_link_head(p);
				return p->value;
			}
			p = p->next;
		}
		return std::string();
	}		// -----  end of function get  -----

private:
	// ==================== PRIVATE METHOD =======================================
	
	/* 
	// ===  FUNCTION  ======================================================================
	//         Name:  remove_link_node
	//  Description:  将节点从链表中移除
	// =====================================================================================
	*/
	inline void remove_link_node(Node *p)
	{
		p->after->before = p->before;
		p->before->after = p->after;
	}		// -----  end of function remove_link_node  -----

	/* 
	// ===  FUNCTION  ======================================================================
	//         Name:  add_to_link_head
	//  Description:  将节点插入到表头
	// =====================================================================================
	*/
	inline void add_to_link_head(Node *p)
	{
		p->after = link_head->after;
		p->before = link_head;
		link_head->after->before = p;
		link_head->after = p;
	}		// -----  end of function add_to_link_head  -----

	/* 
	// ===  FUNCTION  ======================================================================
	//         Name:  get_link_tail
	//  Description:  获取链表尾节点
	// =====================================================================================
	*/
	inline Node* get_link_tail()
	{
		return link_head->before;
	}		// -----  end of function get_link_tail  -----

	/* 
	// ===  FUNCTION  ======================================================================
	//         Name:  remove_from_hash_table
	//  Description:  从hash表中移除节点
	// =====================================================================================
	*/
	inline void remove_from_hash_table(uint64_t hash, uint64_t cur_index, uint64_t tail_index)
	{
		// 此处删除hash表采用的是遍历查找的方法
		// 考虑到如果hash表的数组长度足够，且hash算法比较分散的情况下，
		// bash表中的链表长度不会很长，遍历查找也是可以接受的
		// 如果不使用遍历查找的方法，就需要将hash表中的链表也做成双向链表
		// 这样每个node就会增加一个指针的开销，如果节点数量很多，内存消耗也是不能忽略的
		if (cur_index != tail_index) {
			std::lock_guard<struct ArrayNode> guard(hash_table[tail_index]);
			Node *p = hash_table[tail_index].hash_head;
			while (p != nullptr) {
				if (p->hash == hash) {
					p = p->next;
					return;
				}
			}
		} else {
			Node **p = &hash_table[cur_index].hash_head;
			while (*p != nullptr) {
				if ((*p)->hash == hash) {
					(*p) = (*p)->next;
					return;
				}
			}
		}
	}		// -----  end of function remove_from_hash_table  -----

	// ====================  DATA MEMBERS  =======================================
	const size_t array_size;
	const size_t key_max;
	std::atomic<size_t> size;
	struct ArrayNode *hash_table;	// hash表数组
	struct Node *link_head;		// 记录访问次序的链表
};		// -----  end of class LRUCache  -----


/* 
// ===  FUNCTION  ======================================================================
//         Name:  main
//  Description:  
// =====================================================================================
*/
int main(int argc, char *argv[])
{
	LRUCache cache(1, 2);
	cache.set("a", "a");
	std::cout << cache.get("a") << std::endl;
	cache.set("b", "b");
	std::cout << cache.get("b") << std::endl;
	std::cout << cache.get("a") << std::endl;
	cache.set("c", "c");
	std::cout << cache.get("c") << std::endl;
	std::cout << cache.get("a") << std::endl;
	std::cout << cache.get("b") << std::endl;
	std::cout << "Hello World" << std::endl;
	return EXIT_SUCCESS;
}				// ----------  end of function main  ----------

