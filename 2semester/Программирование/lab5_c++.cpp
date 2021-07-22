#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;




template<class T>
class CircularBuffer {
public:
    friend class Iterator;

    class Iterator{
    private:
        typedef input_iterator_tag  iterator_category;
        /// The type "pointed to" by the iterator.
        typedef T        value_type;
        /// Distance between iterators is represented as this typ

        friend class CircularBuffer;

        CircularBuffer<T>* parent;
        T* iter;

        int getPosition() const{
            return iter - parent->buffer;
        }

    public:

        Iterator(T* iter_, CircularBuffer<T> *parent_) : parent(parent_), iter(iter_)
        {}

        Iterator() = default;

        Iterator(const Iterator& other){
            iter = other.iter;
            parent = other.parent;
        }

        Iterator operator+ (int x) const{
            int newPosition = getPosition() + x;
            if (newPosition < 0) {
                newPosition += ((-newPosition) / parent->bufferSize + 1) * parent->bufferSize;
            }
            newPosition %= parent->bufferSize;
//            cout << "buffer size: " << parent -> bufferSize << endl;
            return Iterator(parent->buffer + newPosition, parent);
        }

        Iterator& operator= (const Iterator& other) {
            iter = other.iter;
            parent = other.parent;

            return *this;
        }

        void operator+= (int x) {
            *this = operator+(x);
        }

        Iterator& operator++ (){
            operator+=(1);
            return *this;
        }

        Iterator operator++ (int) {
            auto temp = *this;
            operator+=(1);
            return temp;
        }

        Iterator operator- (int x) const{
            return operator+(-x);
        }

        int operator- (const Iterator& other) const{
            return iter - other.iter;
        }

        void operator-= (int x) {
            operator+=(-x);
        }

        Iterator& operator-- () {
            operator-=(1);
            return *this;
        }

        Iterator operator-- (int) {
            auto temp = *this;
            operator-=(1);
            return temp;
        }

        bool operator== (const Iterator& other) const {
            return (iter == other.iter);
        }

        bool operator != (const Iterator& other) const{
            return !operator==(other);
        }

        T* operator-> () const{
            return iter;
        }

        T& operator* () const{
            return *iter;
        }

        bool operator <(const Iterator& other){
            return iter < other.iter;
        }

        ~Iterator() = default;

    };


private:
    T* buffer;
    int bufferSize;
    int sz;
    Iterator beg;
    Iterator finish;

public:
    explicit CircularBuffer(int n = 0){
        if (n <= 0) n = 1;

        sz = 0;
        bufferSize = 2 * n;
        buffer = (T*) new T[bufferSize]();

        beg = Iterator(buffer, this);
        finish = Iterator(buffer + sz, this);
    }

    CircularBuffer(const initializer_list<T>& a){
        sz = a.size();
        bufferSize = sz * 2;
        buffer = (T*) new T[bufferSize]();

        int i = 0;
        for (auto it = a.begin(); it != a.end(); it ++, i ++){
            buffer[i] = *it;
        }

        beg = Iterator(buffer, this);
        finish = Iterator(buffer + sz, this);
    }

    explicit CircularBuffer() = default;

    CircularBuffer& operator= (const vector<T>& a){
        clear();

        sz = a.size();
        bufferSize = sz * 2;
        buffer = (T*) new T[bufferSize]();

        for (int i = 0; i < sz; i ++){
            buffer[i] = a[i];
        }

        beg = Iterator(buffer, *this);
        finish = Iterator(buffer + sz, *this);

        return *this;
    }

    void push_back(const T& element) {
        if (size() == bufferSize - 1){
            resize(bufferSize * 2);
        }

        *finish = element;
        finish ++;
        sz ++;
    }

    void pop_back() {
        if (size() <= 0) return;
        finish--;
        sz--;

        if (size() < (bufferSize - 1) / 4) resize(size() * 2);
    }

    void pop_front(){
        if (sz <= 0) return;
        beg ++;
        sz --;

        if (size() < (bufferSize - 1) / 4) resize(size() * 2);
    }

    void push_front(const T& element) {
        if (size() == bufferSize - 1){
            resize(bufferSize * 2);
        }

        beg --;
        *beg = element;
        sz ++;
    }

    Iterator begin(){
        return beg;
    }

    Iterator end(){
        return finish;
    }

    T& operator[](int i){
        return *(begin() + i);
    }

    void clear() {
        delete buffer;
    }

    void write(){
        cout << "Iterators positions: \nbegin: " << begin().iter - buffer << "\nend: " << end().iter - buffer << "\n---------------------" << endl;
        cout << "Size: " << size() << "\nMax size: " << bufferSize << endl;
        bool fl = true;
        for (auto it = begin(); it != end() || fl; it++){
            cout << *it << " ";
            fl = false;
        }
        cout << "\n---------------------" << endl;
    }

    int size(){
        return sz;
    }

    void resize(int newBufferSize){
        T* newBuffer = (T*) new T[newBufferSize]();
        for (int i = 0; i < min(newBufferSize, bufferSize); i ++){
            newBuffer[i] = operator[](i);
        }

        delete buffer;
        buffer = newBuffer;
        bufferSize = newBufferSize;
        sz = min(sz, bufferSize);

        beg = Iterator(buffer, this);
        finish = Iterator(buffer + sz, this);
    }

    ~CircularBuffer(){
        clear();
    }
};

template<> struct std::iterator_traits<CircularBuffer<int>::Iterator>{
    typedef int value_type;
    typedef input_iterator_tag iterator_category;
    typedef ptrdiff_t difference_type;
};

int main() {
    CircularBuffer<int> mas2  = {3, 2, 1, 100, 10};

    mas2.write();

    for (int i = 0; i < 3; i ++){
        mas2.push_back(i + 5);
    }

    for (int i = 0; i < 100; i ++){
        mas2.push_front(i + 1000);
    }

    for (int i = 0; i < 3; i ++){
        mas2.pop_back();
    }

    for (int i = 0; i < 100; i ++){
        mas2.pop_front();
    }

    mas2.write();

    sort(mas2.begin(), mas2.end());

    mas2.write();

    return 0;
}
