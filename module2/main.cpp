#include <iostream>
#include <string>
#include <cmath>
#include <random>
#include <chrono>
#include <functional>

int const V = 1;
int const COUNT = 127;

using std::string;
using std::cin;
using std::cout;
using std::endl;
using std::min;

double rand_num(double max) {
    static std::random_device rd;
    static std::seed_seq seed { rd(), static_cast<unsigned int>(time(nullptr))};
    static std::mt19937_64 gen(seed);
    std::uniform_real_distribution<double> dist(-max, max);

    return dist(gen);
}


struct func {
    string name;
    int level;
};

struct element {
    string type;
    int id;
    int count_func;
    func functions[5 + V];
    double rr;
    int mark;

    element* dad;
    element* son;
    element* next; //brother
};

struct dist {
    int distance;
    int first_id;
    int second_id;
};

double mark(element* el) {
    double res = 0;
    for(int i = 0; i < el->count_func; i++) {
        res += el->functions[i].level*el->functions[i].level;
    }
    res = pow(res, V + 2*el.rr);
    return res;
}

double dist(element* el, element* other) {
    double res = 0;
    for(int i = 0; i < el->count_func; i++)
        for(int j = 0; j < other->count_func; j++)
           if(el->functions[i].name == other->functions[j].name) {
               res += pow(min(el->functions[i].level, other->functions[j].level), 2);
           }
    res += 1;
    res = pow(res, min(el->rr, other->rr) - 2*V);
    return res;
}

element* create_random_element() {
    element* el = new element;
    el->type = char(60+int(rand_num(10)));
    el->id = int(rand_num(10)) + 10;
    el->count_func = abs(int(rand_num(5+V)));
    for(int i = 0; i < el->count_func; i++) {
        el->functions[i].name =char(60+int(rand_num(5)));
        el->functions[i].level = abs(int(rand_num(1000))) + 1;
    }
    el->rr = abs(rand_num(1));
    el->mark = mark(el);
}



int main() {
    std::cout << "Hello, World!" << std::endl;
    element* components[COUNT + 1];
    struct dist distances[COUNT*COUNT + 1];
    for(int i = 0; i < COUNT; i++) {
        components[i] = create_random_element();
    }
    return 0;
}