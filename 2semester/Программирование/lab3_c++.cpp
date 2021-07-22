#ifndef _DEBUG
#define PUGIXML_HEADER_ONLY
#endif
#include "pugixml-1.11/src/pugixml.hpp"
#include <bits/stdc++.h>
#include <Windows.h>

//класс точка
class Point {
private:
    double x_;
    double y_;
    bool used_;
    double cost_;
    std::string name_;

public:
     //сеттер
    void setX(double x) {
        x_ = x;
    };

    void setY(double y) {
        y_ = y;
    };

    void setUsed(bool used) {
        used_ = used;
    };

    void setName(std::string name) {
        name_ = name;
    };

    void setCost(double cost) {
        cost_ = cost;
    }

    //геттер
    double getX() const {
        return x_;
    };

    double getY() const {
        return y_;
    };

    bool getUsed() const {
        return used_;
    };

    double getCost() const {
        return cost_;
    }

    std::string getName() const {
        return name_;
    }

    //конструктор по умолчанию
    Point(double x = 0, double y = 0, bool used = false, double cost = -1, std::string name = "")
        : x_(x)
        , y_(y)
        , used_(used)
        , cost_(cost)
        , name_(name)
    {

    }

    //конструктор
    Point(const std::string& coord, Point& point) {
        std::string coordinate;
        for(int i = 0; i < coord.size(); i++){
            if(coord[i] == ','){
                point.x_ = 111*atof(coordinate.c_str());
                coordinate.erase(0);
            }
            else if(coord[i] != ',')
                coordinate += coord[i];
            if(i == coord.size()-1){
                point.y_ = 111*atof(coordinate.c_str());
                coordinate.erase(0);
            }
        }
    }

    //конструктор копирования
    Point(const Point &point)
        : x_(point.x_)
        , y_(point.y_)
        , used_(point.used_)
    {

    }

    //перегрузка оператора присваивания
    Point& operator= (const Point &point) {
        x_ = point.x_;
        y_ = point.y_;
        used_ = point.used_;
        return *this;
    }

    double length(const Point& p2){
        return sqrt((p2.getX() - getX()) * (p2.getX() - getX())
                     + (p2.getY() - getY()) * (p2.getY() - getY()));
    }

    //деструктор
    ~Point()
    {

    }
};

//класс транспорт
class Transport{
private:
    std::vector<std::unordered_map<std::string , std::vector<Point>>> transport_;
    std::vector<std::unordered_map <std::string, int>> routes_;
    //route with the most number of stops
    std::vector<std::pair<int, std::string>> maxRoute_;
    //макисмальный путь
    std::vector<Point> maxPath_;
    //
    std::pair<std::string, int> streetMaxStops_;
    std::unordered_map<std::string, int> streets_;


public:
    //конструктор
    Transport() {
        transport_.resize(3);
        routes_.resize(3);
        maxRoute_.resize(3);
        maxPath_.resize(3);
        streetMaxStops_.first.erase(0);
        streetMaxStops_.second = 0;
    }

    //геттер
    int getMaxRoute(const int num) const {
        return maxRoute_[num].first;
    }

    std::string getMaxRouteName(const int num) const {
        return maxRoute_[num].second;
    }


    std::string getMaxPathName(const int num) const {
        return maxPath_[num].getName();
    }

    double getMaxPathCost(const int num) const{
        return maxPath_[num].getCost();
    }

    std::string getStreetMaxStopsName() const {
        return streetMaxStops_.first;
    }

    int getStreetMaxStops() const {
        return streetMaxStops_.second;
    }

    //определение типа транспорта
    int typeOfTransport (const std::string& tempTransport) {
        const std::string tram = "РўСЂР°РјРІР°Р№";
        const std::string trolleybus = "РўСЂРѕР»Р»РµР№Р±СѓСЃ";
        const std::string bus = "РђРІС‚РѕР±СѓСЃ";
        if (tempTransport == tram) {
            return 0;
        }
        else if (tempTransport == trolleybus) {
            return 1;
        }
        else if (tempTransport == bus) {
            return 2;
        }
     }

    //маршрут с наибольшим количеством остановок
    void maxRoute(const pugi::xml_node tempNode) {
        Point tempPoint(tempNode.child_value("coordinates"), tempPoint);
        std::string tempRouteName;
        int type = typeOfTransport(tempNode.child_value("type_of_vehicle"));
        std::string routeName = tempNode.child_value("routes");
        for (int i = 0; i < routeName.size(); i++) {
            if (routeName[i] != ',' && routeName[i] != '.') {
                tempRouteName += routeName[i];
            }
            if (routeName[i] == ',' || routeName[i] == '.' || i == routeName.size() - 1) {
                routes_[type][tempRouteName] += 1;
                transport_[type][tempRouteName].push_back(tempPoint);
                if (routes_[type][tempRouteName] > maxRoute_[type].first) {
                    maxRoute_[type].first = routes_[type][tempRouteName];
                    maxRoute_[type].second = tempRouteName;
                }
                tempRouteName.erase(0);
            }
        }
    }

    //task 2
    void longRoute(const int num){
        for(auto i : transport_[num]){
            int n = i.second.size();
            std::vector<double> dist(n, INT_MAX);
            dist[0] = 0;
            for(int j = 0; j < n; j++){
                int min_i = -1;
                for (int k = 0; k < n; k++)
                    if (!i.second[k].getUsed() && (min_i == -1 || dist[k] > dist[min_i])) {
                        min_i = k;
                    }

                i.second[min_i].setUsed(true);

                for (int k = 0; k < n; k++)
                    if (!i.second[k].getUsed() && i.second[min_i].length(i.second[k]) < dist[k])
                        dist[k] = i.second[min_i].length(i.second[k]);
            }

            double path = 0;
            for(double j : dist){
                if (j != INT_MAX)
                    path += j;
            }

            if (path > maxPath_[num].getCost()) {
                maxPath_[num].setName(i.first);
                maxPath_[num].setCost(path);
                //std::cout << (int)path << '\n';
            }
        }
    }

    //task 3
    void findStreetMaxStops(const pugi::xml_node tempNode) {
        std::string newLocation;
        std::string location = tempNode.child_value("location");
        for (int i = 0; i < location.size(); ++i) {
            if (location[i] != ','){
                if (location[i] == '.'){
                    if (location[i-1] == 'ш'){
                        newLocation += "оссе";
                    }
                    if (location[i-1] == 'л'){
                        newLocation += "ица";
                    }
                    if (location[i-1] == 'р'){
                        newLocation+= "оспект";
                    }
                }
                else
                    newLocation += location[i];
            }
            if (location[i] == ',' || i == location.size() - 1) {
                streets_[newLocation] += 1;
                if (streets_[newLocation] > streetMaxStops_.second){
                    streetMaxStops_.second = streets_[newLocation];
                    streetMaxStops_.first = newLocation;
                }
                newLocation.erase(0);
                i++;
            }
        }
    }
};

int main() {
    SetConsoleOutputCP(CP_UTF8);
    pugi::xml_document doc;
    doc.load_file("data.xml");
    pugi::xml_node dataset = doc.child("dataset");
    Transport ourTransport;

    for(pugi::xml_node stat = dataset.first_child(); stat; stat = stat.next_sibling()) {
        ourTransport.maxRoute(stat);
        ourTransport.findStreetMaxStops(stat);
    }
    for (int i = 0; i < 3; ++i) {
        ourTransport.longRoute(i);
    }
    std::cout << "1 task" << '\n';
    std::cout << "Tram route with the most number of stops: " << ourTransport.getMaxRoute(0) << ", route number: " << ourTransport.getMaxRouteName(0) << '\n';
    std::cout << "Trolleybus route with the most number of stops: " << ourTransport.getMaxRoute(1) << ", route number: " << ourTransport.getMaxRouteName(1) << '\n';
    std::cout << "Bus route with the most number of stops: " << ourTransport.getMaxRoute(2) << ", route number: " << ourTransport.getMaxRouteName(2) << '\n';
    std::cout << "2 task" << '\n';
    std::cout << "longest tram route: " << ourTransport.getMaxPathName(0) << " " << ourTransport.getMaxPathCost(0) << '\n';
    std::cout << "longest trolleybus route: " << ourTransport.getMaxPathName(1) << " " << ourTransport.getMaxPathCost(1) << '\n';
    std::cout << "longest bus route: " << ourTransport.getMaxPathName(2) << " " << ourTransport.getMaxPathCost(2) << '\n';
    std::cout << "task 3" << '\n';
    std::cout << "Street with the most stops: " << ourTransport.getStreetMaxStopsName() << ". with " << ourTransport.getStreetMaxStops() << " stops" << '\n';
    return 0;
}
