//#include <iostream>
//#include <functional>
//#include <algorithm>
//#include <cassert>
//#include <memory>
//#include "Safedeque.h"
//
//int main()
//{
//    cout << "********** print empty deque *********" << endl;
//    Safedeque<int> sd;
//    sd.print();
//    cout << "********** add elements *********" << endl;
//    // add elements
//    sd.push_front(1);
//    sd.push_front(2);
//    sd.pop_back();
//    sd.push_front(3);
//    sd.push_back(4);
//    sd.push_back(5);
//    sd.push_front(6);
//    sd.push_back(7);
//    sd.print();
//    cout << "********* real array elements**********" << endl;
//    sd.real_print();
//    cout << "********** element squares *********" << endl;
//    Iterator<int> iter1 = sd.begin();
//    for_each(sd.begin(), sd.end(), [](int &x){x=x*x;});
//    sd.print();
//    cout << "********* work with iterators **********" << endl;
//    auto iter2 = sd.begin();
//    ++iter2;
//    cout << *iter2 << endl;
//    iter2 = sd.end();
//    --iter2;
//    *iter2 = 100;
//    sd[3] = 20;
//    sd.print();
//    cout << "********* throw error **********" << endl;
//    auto myiter = sd.end();
//    // ++myiter; // triggers assert exception
//    // sd[1000] = 10; // triggers assert execption
//    return 0;
//}
