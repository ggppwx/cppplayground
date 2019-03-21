#include <iostream>
#include <shared_mutex>
#include <mutex>
#include <thread>
#include <chrono>
#include <map>
#include <algorithm>
#include <string>

using namespace std;
shared_timed_mutex bookMutex;

map<string, int> books{{"cpp prime", 111}, {"children ", 112},
                       {"adult book" , 113}}; // initialization list


// a template example 
template<typename K, typename V> // K should be before V
V getWithDefault(const map<K,V> &dict,
                 const K &key,
                 const V &defaultVal) {

    // How to use template variable 
    K k;
    //cout << typeid(k).name() << endl;

    // lambda example
    auto flambda = [key] (const pair<K,V> &t) -> bool {
                       return t.first == key;
                   };
    
    // it needs to have typename since K,V in the map
    // find_if:
    // pred(*first) -> *first = pair<K,V>
    typename map<K, V>::const_iterator it = find_if(dict.begin(),
                                                    dict.end(),
                                                    flambda);
    if (it == dict.end()) {
        return defaultVal;
    } else {
        return it->second;
    }
}

void reader(const string &bookName) {
    // shared_lock to have shared access
    shared_lock<shared_timed_mutex> readerLock(bookMutex);
    cout << "reading book: " << bookName << " -> "
         << getWithDefault(books, bookName, -1) << endl;
}

void writer(const string &bookName, int bookNumber) {
    // locker guard always do lock/unlock - similar to unique_lock
    lock_guard<shared_timed_mutex> writerLock(bookMutex);

    cout << "updating the books ... "<< bookName << endl;
    books[bookName] = bookNumber;
    this_thread::sleep_for(chrono::seconds(5));
    cout << "ending update ...." << endl;
}

int main() {
    cout << "Hello world" << endl;

    // c++ thread definition
    thread readerThread1([](){ reader("cpp prime"); } );
    thread writerThread1([]{ writer("hibook", 114); });
    thread readerThread2([](){ reader("hibook"); } );
    thread readerThread3([](){ reader("adult book"); } );
    cout << "done spawning threads ... " << endl;
    readerThread1.join();
    writerThread1.join();
    readerThread2.join();
    readerThread3.join();
    cout << "All threads joined ... " << endl;
}
