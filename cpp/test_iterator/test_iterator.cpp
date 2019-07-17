#include<iostream>
#include<vector>

using namespace std;

int main() {
    vector<int> st = {100, 200, 300, 400};
    cout << "capacity:" << st.capacity() << endl;
    cout << "size:" << st.size() << endl;
    
    vector<int>::iterator it;
    for(it=st.begin(); it!=st.end(); it++) {
        cout << "before value:" << *it << endl;
        if(*it==300 || *it==400) {
            //st.erase(it); // 该条语句后，it指向400了，随后it++又会执行，循环结束, 虽未运行崩溃，但功能不符
            //st.push_back(500); // 触发vector的内存重新分配, it还指向原来的内存, 运行结果不可预料
            //itor=st.erase(itor);//此时itor指向400
        }
        cout << "after value:" << *it << endl;
    }

    for(it=st.begin();it!=st.end();it++)
        cout<<*it<<" ";
    cout << endl;
    return 0;
}

