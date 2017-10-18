#include <bits/stdc++.h>
#define endl '\n'

using namespace std;

random_device random_seed;
mt19937 mt(random_seed());

class implicit_treap
{	
	public:
		struct node
		{
			int sz, priority, lazy, rev, sum;
			int val, key;
			node *l, *r, *par;

			node() { lazy = 0; rev = 0; val = 0; sz = 0; key = 0; priority = 0; l = NULL; r = NULL; par = NULL;}
			node(int _val)
			{
				val = _val;
				sum = _val;
				rev = 0;
				lazy = 0;
				key = 0;
				sz = 1;
				priority = mt();

				l = nullptr;
				r = nullptr;
				par = nullptr;
			}
		};

		typedef node* pnode;

	private:
		pnode root;
		map<int, pnode> position;

		int size(pnode p) { return p ? p->sz : 0; }
		void update_size(pnode &p) { if(p) p->sz = size(p->l) + size(p->r) + 1; }

		void update_parent(pnode &p)
		{
			if(!p) return;
			if(p->l) p->l->par = p;
			if(p->r) p->r->par = p;
		}

		void push(pnode &p)
		{
			if(!p) return;		
			p->sum += size(p) * p->lazy;
			p->val += p->lazy;

			if(p->rev) swap(p->l, p->r);

			if(p->l) { p->l->lazy += p->lazy; p->l->rev ^= p->rev; }
			if(p->r) { p->r->lazy += p->lazy; p->r->rev ^= p->rev; }

			p->lazy = 0;
			p->rev = 0;
		}

		void reset(pnode &t) { if(t) t->sum = t->val; }

		void combine(pnode &t, pnode l, pnode r)
		{
			if(!l) { t = r; return; }
			if(!r) { t = l; return; }
			t->sum = l->sum + r->sum;
		}

		void operation(pnode &t)
		{
			if(!t) return;

			reset(t);
			push(t->l);
			push(t->r);

			combine(t, t->l, t);
			combine(t, t, t->r);
		}

		void split(pnode t, pnode &l, pnode &r, int k, int add = 0)
		{
			if(t == NULL) { l = NULL; r = NULL; return; }
			push(t);

			int idx	= add + size(t->l);
			if(idx <= k) 
				split(t->r, t->r, r, k, idx + 1), l = t;
			else
				split(t->l, l, t->l, k, add), r = t;

			update_parent(t);
			update_size(t);
			operation(t);
		}

		void merge(pnode &t, pnode l, pnode r)
		{
			push(l);
			push(r);

			if(!l) { t = r; return; }
			if(!r) { t = l; return; }

			if(l->priority > r->priority)
				merge(l->r, l->r, r), t = l;
			else 
				merge(r->l, l, r->l), t = r;

			update_parent(t);
			update_size(t);
			operation(t);
		}

		int get_pos(pnode curr, pnode son = nullptr)
		{
			if(!son)
			{
				if(!curr->par) return size(curr->l);
				else return size(curr->l) + get_pos(curr->par, curr);
			}

			if(!curr->par)
			{
				if(son == curr->l) return 0;
				else return size(curr->l) + 1;
			}

			if(curr->l == son) return get_pos(curr->par, curr);
			else return get_pos(curr->par, curr) + size(curr->l) + 1;
		}

	public:	
		void clear()    /// function for clearing the implicit treap.
		{
			root = NULL;
			position.clear();
		}

		inline int size() { return size(root); }    /// function which return the size of the implicit treap (the number of elements).

		implicit_treap() { clear(); }

		void insert(int pos, int key, int val)      /// function for inserting an element with value "val" at position "pos" with key "key". 
		{
			if(!root)
			{
				pnode to_add = new node(val);
				to_add->key = key;
				root = to_add;
				position[key] = root;
				return;
			}

			pnode l, r, mid;
			mid = new node(val);
			position[key] = mid;
			mid->key = key;

			split(root, l, r, pos - 1);
			merge(l, l, mid);
			merge(root, l, r);
		}

		void erase(int qL, int qR)  /// function for erasing all elements in the subarray [qL; qR].
		{
			pnode l, r, mid;

			split(root, l, r, qL - 1);
			split(r, mid, r, qR - qL);
			merge(root, l, r);
		}

		int query_sum(int qL, int qR) /// function for finding the sum of values of all element in the range [qL; qR].
		{
			pnode l, r, mid;

			split(root, l, r, qL - 1);
			split(r, mid, r, qR - qL);

			int answer = mid->sum;

			merge(r, mid, r);
			merge(root, l, r);

			return answer;
		}

		void add_value(int qL, int qR, int val)     /// function for adding "val" to the values of all elements in the interval [qL; qR]. 
		{
			pnode l, r, mid;

			split(root, l, r, qL - 1);
			split(r, mid, r, qR - qL);

			mid->lazy += val;

			merge(r, mid, r);
			merge(root, l, r);
		}


		void reverse(int qL, int qR)                /// funtion for reversing the subarray [qL; qR].
		{
			pnode l, r, mid;

			split(root, l, r, qL - 1);
			split(r, mid, r, qR - qL);

			mid->rev ^= 1;
			merge(r, mid, r);
			merge(root, l, r);
		}

		void cyclic_shift(int qL, int qR, int k)    /// function for cyclic shifting of the subbaray [qL; qR] with "k" positions.
		{
			if(qL == qR) return;
			k %= (qR - qL + 1);

			pnode l, r, mid, fh, sh;
			split(root, l, r, qL - 1);
			split(r, mid, r, qR - qL);

			split(mid, fh, sh, (qR - qL + 1) - k - 1);
			merge(mid, sh, fh);

			merge(r, mid, r);
			merge(root, l, r);
		}

		int get_position_of_key(int key) { return get_pos(position[key]); }     /// function for finding the position of the element with key "key". 

		int get_value_at_positon(int pos)    /// function for finding the value of the element at position "pos". 
		{  
			pnode l, r, mid;
			split(root, l, r, pos - 1);
			split(r, mid, r, 0);

			int ret = mid ? mid->val : -1; 
			merge(r, mid, r);
			merge(root, l, r);
			return ret;
		}

		int get_key_at_positon(int pos)      /// function for finding the key of the element at position "pos". 
		{  
			pnode l, r, mid;
			split(root, l, r, pos - 1);
			split(r, mid, r, 0);

			int ret = mid ? mid->key : -1; 
			merge(r, mid, r);
			merge(root, l, r);
			return ret;
		}

		void erase_key(int key)             /// erases the element with key "key".
		{
			pnode l, r, mid;

			int pos = get_position_of_key(key);
			
			split(root, l, r, pos - 1);
			split(r, mid, r, 0);
			merge(root, l, r);
		}
};
