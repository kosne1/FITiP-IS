#include <iostream>

//all_of
template<class InputIterator, class UnaryPredicate>
        bool all_of (InputIterator first, InputIterator last, UnaryPredicate predicate)
        {
            for (; first!=last; ++first) {
                if (!predicate(*first))
                    return false;
            }
            return true;
        }

//any_of
template<class InputIterator, class UnaryPredicate>
        bool any_of (InputIterator first, InputIterator last, UnaryPredicate predicate)
        {
            for (; first!=last; ++first) {
                if (predicate(*first))
                    return true;
            }
            return false;
        }

//none_of
template<class InputIterator, class UnaryPredicate>
        bool none_of (InputIterator first, InputIterator last, UnaryPredicate predicate)
        {
            for (; first!=last; ++first) {
                if (predicate(*first))
                    return false;
            }
            return true;
        }

//one_of
template<class InputIterator, class UnaryPredicate>
        bool one_of (InputIterator first, InputIterator last, UnaryPredicate predicate)
        {
            bool flag = false;
            for (; first!=last; ++first) {
                if (predicate(*first)) {
                    if (!flag)
                        flag = true;
                    else
                        return false;
                }
            }
            return true;
        }

//is_sorted
template<class InputIterator, class BinaryPredicate>
        bool is_sorted (InputIterator first, InputIterator last, BinaryPredicate predicate)
        {
            if (first == last)
                return true;
            for (; first != std::prev(last); ++first){
                if (!predicate(*first, *(std::next(first))))
                    return false;
            }
            return true;
        }

//is_partitioned
template <class InputIterator, class UnaryPredicate>
        bool is_partitioned (InputIterator first, InputIterator last, UnaryPredicate predicate)
        {
            while (first != last && predicate(*first)) {
                ++first;
            }
            while (first != last) {
                if (predicate(*first))
                    return false;
                ++first;
            }
            return true;
        }

//find_not
template<class InputIterator, class T>
        InputIterator find_not (InputIterator first, InputIterator last, const T & element)
        {
            for (; first != last; ++first) {
                if (*first != element)
                    return first;
            }
            return last;
        }

//find_backward
template<class InputIterator, class T>
        InputIterator find_backward (InputIterator first, InputIterator last, const T & element)
        {
            InputIterator flag = last;
            for (; first != last;) {
                --last;
                if (*last != element)
                    return last;
            }
            return flag;
        }

//is_palindrome
template<class InputIterator, class Predicate>
        bool is_palindrome(InputIterator first, InputIterator last, Predicate predicate)
        {
            for (; first != last ; ++first) {
                --last;
                if (first == last)
                    break;
                if (!predicate(*first, *last))
                    return false;
            }
            return true;
        }

int main(){
    int mas[] = {1, 2, 3, 4, 4, 6};
    std::string myStr = "aab";

//    find_not(&mas[0], &mas[6], 1) != nullptr ? std::cout << find_not(&mas[0], &mas[6], 1) << '\n' : std::cout << "true" << '\n';

    is_palindrome(&myStr[0], &myStr[2], [](const char string1, const char string2){return string1 == string2; }) ? std::cout << "true" : std::cout << "false";

    //std::cout << myStr[2];
    return 0;
}
