#include <iostream>
#include <vector>
#include <valarray>
#include <unordered_map>
#include <unordered_set>

class Polynomial {
private:
    mutable std::unordered_map<int, double> polynomial_;

public:
    //конструктор по умолчанию
    Polynomial() = default;

    //конструктор
    Polynomial(const Polynomial &polynomial) = default;

    //конструктор для unordered map
    explicit Polynomial(const std::unordered_map<int, double> &polynomial)
    {
        polynomial_ = polynomial;
    }

    //конструктор для вектора
    explicit Polynomial(const std::vector<int> &polynomial) {
        for (int i = 0; i < polynomial.size(); i++)
            if (polynomial[i] != 0)
                polynomial_[i] = polynomial[i];
    }

    //перегрузка оператора присваивания
    Polynomial &operator=(const Polynomial &polynomial) = default;

    //перегрузка оператора присваивания для вектора
    Polynomial &operator= (const std::vector<double> &polynomial) {
        for (int i = 0; i < polynomial.size(); i++)
            if (polynomial[i] != 0)
                polynomial_[i] = polynomial[i];
        return *this;
    }

    //перегрузка оператора присваивания для числа
    Polynomial &operator= (const double coefficient) {
        polynomial_[0] = coefficient;
        return *this;
    }

    //условие для функции size()
    [[nodiscard]] int size() const {
        return polynomial_.size();
    }

    //удаление лишних нулей в полиноме
    void removeZeros() {
        for (auto i = polynomial_.begin(); i != polynomial_.end(); ) {
            if (i->second == 0)
                i = polynomial_.erase(i);
            else
                i++;
        }
    }

    //перегрузка оператора сравнения
    bool operator== (Polynomial &other) {
        removeZeros();
        other.removeZeros();
        return polynomial_ == other.polynomial_;
    }

    //перегрузка оператора равенства
    bool operator!= (Polynomial &other) {
        return !(other == *this);
    }

    //перегрузка оператора +(унарный)
    Polynomial &operator+ () {
        return *this;
    }

    //перегрузка оператора -(унарный)
    Polynomial operator- () const {
        auto new_polynomial = polynomial_;
        for (auto &pair: new_polynomial) {
            pair.second = - pair.second;
        }
        return Polynomial {new_polynomial};
    }

    //перегрузка оператора сложения(бинарный)
    Polynomial operator+ (const Polynomial &other) const {
        auto new_polynomial = polynomial_;
        std::unordered_set<int> used_pair;
        for (auto &our_pair: new_polynomial) {
                if (other.polynomial_.count(our_pair.first) > 0)
                    our_pair.second += other.polynomial_.at(our_pair.first);
                used_pair.insert(our_pair.first);
        }
        for (auto &pair: other.polynomial_) {
            if (used_pair.count(pair.first) == 0)
                new_polynomial[pair.first] += pair.second;
        }
        auto output {Polynomial {new_polynomial}};
        output.removeZeros();
        return output;
    }

    //перегрузка оператора разности(бинарный)
    Polynomial operator- (const Polynomial &other) const {
        return *this + (-other);
    }

    //перегрузка оператора +=
    Polynomial &operator+= (const Polynomial &other) {
        *this = *this + other;
        return *this;
    }

    //перегрузка оператора -=
    Polynomial &operator-= (const Polynomial &other) {
        *this = *this - other;
        return *this;
    }

    //перугрузка оператора умножения на число
    Polynomial operator* (const double number) const {
        auto new_pair = polynomial_;
        for (auto &i: new_pair) {
            i.second *= number;
        }
        return Polynomial{new_pair};
    }

    //перегрузка оператора умножения на полином
    Polynomial operator* (const Polynomial &other) const {
        std::unordered_map<int, double> new_polynomial;
        for (auto &our_pair: polynomial_) {
            for (auto &other_pair: other.polynomial_) {
                new_polynomial[our_pair.first + other_pair.first]
                    += other_pair.second;
            }
        }
        return Polynomial{new_polynomial};
    }

    //перегрузка оператора *=
    Polynomial &operator *=(const Polynomial &other) {
        *this = *this * other;
        return *this;
    }

    //перегрузка оператора деления на число
    Polynomial operator/ (const double number) const {
        return *this * (1 / number);
    }

    //перегрузка оператора *= (число)
    void operator*= (const double number) {
        *this = *this * number;
    }

    //перегрузка оператора /= (число)
    void operator/= (const double x) const {
        *this - *this / x;
    }

    friend std::ostream& operator <<(std::ostream& output, const Polynomial& polynomial);
    friend std::istream& operator >>(std::istream& input, Polynomial& polynomial);

    //перегрукза оператора ввода
    void operator<< (const int number) const{
        std::unordered_map<int, double> new_polynomial;
        for (auto pair: polynomial_)
            new_polynomial[pair.first + number] = pair.second;
        polynomial_ = new_polynomial;
    }

    //перегрузка оператора вывода
    void operator>> (const int number) const {
        std::unordered_map<int, double> new_polynomial;
        for (auto pair: polynomial_)
            new_polynomial[pair.first - number] = pair.second;
        polynomial_ = new_polynomial;
    }

    //перегрузка оператора []
    const double& operator[](const int i) const {
        return polynomial_[i];
    }

    ~Polynomial() = default;
};

//вывод
std::ostream& operator <<(std::ostream& output, const Polynomial& polynomial) {
    for (auto value: polynomial.polynomial_)
        output << value.first << " " << value.second << '\n';
    return output;
}

//считывание
std::istream& operator>> (std::istream& input, Polynomial &polynomial) {
    int n;
    input >> n;
    std::unordered_map<int, double> mas;
    for (int i = 0; i < n; ++i) {
        int key;
        double value;
        input >> key >> value;
        mas[key] = value;
    }
    polynomial = Polynomial{mas};
    return input;
}

int main() {

}
