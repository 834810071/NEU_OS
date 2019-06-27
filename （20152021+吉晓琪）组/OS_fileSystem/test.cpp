#include<list>
#include<iterator>
#include<iostream>

using namespace std;

int test() {
	list<int> arr;
	arr.push_back(1);
	arr.push_back(2);
	arr.push_back(3);
	arr.push_back(4);
	cout << "arr: \t";
	list<int>::iterator iter;
	for (iter = arr.begin(); iter != arr.end(); iter++) {
		cout << *iter << "\t";
	}
	cout << endl;

	arr.remove(3);
	//cout<<"The remove element is "<<a<<endl;
	cout << "now the arr is: \t";
	for (iter = arr.begin(); iter != arr.end(); iter++) {
		cout << *iter << "\t";
	}
	cout << endl;
	return 0;
}