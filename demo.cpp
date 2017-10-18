#include <bits/stdc++.h>
#include "implicit_treap.h"

using namespace std;
template<class T> inline void chkmax(T &x, const T &y) { if(x < y) x = y; }
template<class T> inline void chkmin(T &x, const T &y) { if(x > y) x = y; }
const int MAXN = (1 << 20);

implicit_treap t;

int main()
{
	t.clear();
	
	int number_of_queries;
	cin >> number_of_queries;

	while(number_of_queries--)
	{
		string type;
		cin >> type;

		if(type == "insert")
		{
			int pos, key, val;
			cin >> pos >> key >> val;
			t.insert(pos, key, val);
		}
		else if(type == "shift")
		{
			int l, r, k;
			cin >> l >> r >> k;
			t.cyclic_shift(l, r, k);
		}
		else if(type == "reverse")
		{
			int l, r;
			cin >> l >> r;
			t.reverse(l, r);
		}
		else if(type == "erase_subarray")
		{
			int l, r;
			cin >> l >> r;
			t.erase(l, r);
		}
		else if(type == "erase_key")
		{
			int key;
			cin >> key;
			t.erase_key(key);
		}
		else if(type == "sum")
		{
			int l, r;
			cin >> l >> r;
			cout << t.query_sum(l, r) << endl;
		}
		else if(type == "print_values")
		{
			for(int i = 0; i < t.size(); i++)
				cout << "value at position " << i << " is " << t.get_value_at_positon(i) << endl;
		}
		else if(type == "print_keys")
		{
			for(int i = 0; i < t.size(); i++)
				cout << "key at position " << i << " is " << t.get_key_at_positon(i) << endl;
		}
		else if(type == "add_value")
		{
			int l, r, val;
			cin >> l >> r >> val;
			t.add_value(l, r, val);
		}
		else if(type == "find_position_of_key")
		{
			int key;
			cin >> key;
			cout << t.get_position_of_key(key) << endl;
		}
	}

	return 0;
}
