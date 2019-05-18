#include <iostream>
#include <string>
#include <cmath>
#include <random>
#include <chrono>
#include <functional>
#include <algorithm>

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
    std::uniform_real_distribution<double> dstnc(-max, max);

    return dstnc(gen);
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
    double mark;

    element* dad;
    element* son;
    element* next; //brother
};

struct dstnc {
    double distance; //distnce between 2 components
    double first_mark;
    double second_mark;
};

//sort for components
int partition (element* arr[], int low, int high)
{
    int pivot = arr[high]->mark;
    int i = (low - 1);

    for (int j = low; j <= high- 1; j++)
    {

        if (arr[j]->mark <= pivot)
        {
            i++;
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[high]);
    return (i + 1);
}

void quickSort(element* arr[], int low, int high)
{
    if (low < high)
    {

        int pi = partition(arr, low, high);

        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}


//sort for distances
int partition (dstnc* arr[], int low, int high)
{
    double pivot = arr[high]->distance;
    int i = (low - 1);

    for (int j = low; j <= high- 1; j++)
    {

        if (arr[j]->distance <= pivot)
        {
            i++;
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[high]);
    return (i + 1);
}

void quickSort(dstnc* arr[], int low, int high)
{
    if (low < high)
    {

        int pi = partition(arr, low, high);

        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

double mark(element* el) {
    double res = 0;
    for(int i = 0; i < el->count_func; i++) {
        res += el->functions[i].level*el->functions[i].level;
    }
    res = pow(res, V + 2*el->rr);
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

int create_distances(element* components[], struct dstnc* distances[], int count) {
    int k = 0;
    for(int i = 0; i < count; i++)
        for(int j = i + 1; j < count; j++) {
            dstnc* d = new dstnc;
            d->distance = dist(components[i], components[j]);
            d->first_mark = components[i]->mark;
            d->second_mark = components[j]->mark;
            distances[k] = d;
            k++;
        }
    return k;
}

element* create_random_element() {
    element* el = new element;
    el->type = char(75+int(rand_num(10)));
    el->id = int(rand_num(10)) + 10;
    el->count_func = abs(int(rand_num(5+V)));
    for(int i = 0; i < el->count_func; i++) {
        el->functions[i].name =char(75+int(rand_num(5)));
        el->functions[i].level = abs(int(rand_num(1000))) + 1;
    }
    el->rr = abs(rand_num(1));
    el->mark = mark(el);
    return el;
}

void print_element(element* el) {
    cout << endl;
    cout << "type - " << el->type << "\n";
    cout << "mark - " << el->mark << "\n";

}

void print_elements(element* components[], int count) {
    for(int i = 0; i < count; i++)
        print_element(components[i]);
}

void print_distance(struct dstnc* d) {
    cout << endl;
    cout << "distance - " << d->distance << "\n";
    cout << "first_mark - " << d->first_mark << "\n";
    cout << "second_mark - " << d->second_mark << "\n";
}

void print_distances(struct dstnc* distances[], int start, int end) {
    for(int i = start; i < end; i++)
        print_distance(distances[i]);
}



int main() {
    element* components[COUNT + 1];
    struct dstnc* distances[COUNT*COUNT + 1];

    for(int i = 0; i < COUNT; i++) {
        components[i] = create_random_element();
    }

    //this is for task one
//    quickSort(components, 0, COUNT - 1);
//    int count_dist;
//    count_dist = create_distances(components, distances, 126);
//    quickSort(distances, 0, count_dist - 1);
//    print_distances(distances, 0, 12);
//    print_distances(distances, count_dist - 12, count_dist);

    print_elements(components, COUNT);
    return 0;
}