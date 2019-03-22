#include <bits/stdc++.h>

using std::cout;
using std::cin;

struct complex {
    float real;
    float imagine;
};

struct element {
    complex first;
    complex second;
    int key;
    element* next;
};

struct List{
    element* root;
    int length;
};


const int V = 3; // номер варіанту
const int RAND = 100; // max random
const int KOEF = 23;



double rand_num(double max) {
    static std::random_device rd;
    static std::seed_seq seed { rd(), static_cast<unsigned int>(time(nullptr))};
    static std::mt19937_64 gen(seed);
    std::uniform_real_distribution<double> dist(-max, max);

    return dist(gen);
}



complex sum_complex(complex z, complex w) {
    complex sum;
    sum.real = z.real + w.real;
    sum.imagine = z.imagine + w.imagine;
    return sum;
}

complex multiple_complex(complex z, complex w) {
    complex multiple;
    multiple.real = z.real*w.real - z.imagine*w.imagine;
    multiple.imagine = z.real*w.imagine + z.imagine*w.real;
    return multiple;
}

complex rizn_complex(complex z, complex w) {
    complex rizn;
    rizn.real = z.real - w.real;
    rizn.imagine = z.imagine - w.imagine;
    return rizn;
}

complex conj(complex z){
    complex n;
    n.real = z.real;
    n.imagine = (-1)*n.imagine;
    return n;
}

complex int_to_complex(int num) {
    complex z;
    z.imagine = 0;
    z.real = num;
    return z;
}

float abss(complex z) {
    return multiple_complex(z, conj(z)).real;
}

complex create_random_complex() {
    float real, im;
    real = rand_num(RAND);
    im = rand_num(RAND);
    complex z;
    z.real = real;
    z.imagine = im;
    return z;
}


int count_value(complex z, complex w) {
    return floor(abss(sum_complex(rizn_complex(sum_complex(multiple_complex(z, z), multiple_complex(int_to_complex(2),multiple_complex(z,w))), multiple_complex(w,w)), rizn_complex(multiple_complex(int_to_complex(V),z), w))));
}



element* create_random_element() {
    element* el = new element;
    complex z = create_random_complex();
    complex w = create_random_complex();
    el->first = z;
    el->second = w;
    el->key = count_value(z, w) % KOEF;
    el->next = nullptr;
    return el;
}


void write_element(element* el) {
    cout << "first -- real: " << el->first.real << " imagine: " << el->first.imagine << "\n";
    cout << "second -- real: " << el->second.real << " imagine: " << el->second.imagine << "\n";
    cout << "value: " << count_value(el->first, el->second) << "\n";
}


List* create_empty_list() {
    List* spysok = new List;
    spysok->length = 0;
    spysok->root = nullptr;
    return spysok;
}

element* add_element_list(element* next, complex z, complex w) {
    element* el = new element;
    el->first = z;
    el->second = w;
    el->next = next;
    return el;
}

List* append_list(List* spysok, complex z, complex w) {
    element* el;
    if(spysok->length == 0) {
        el = add_element_list(nullptr, z, w);
        spysok->root = el;
    }
    else {
        element* curr = spysok->root;
        while(curr->next != nullptr)
            curr = curr->next;
        el = add_element_list(nullptr, z, w);
        curr->next = el;
    }
    spysok->length++;
    return spysok;
}

void print_all(List* spysok) {
    int length = spysok->length;
    element* curr = spysok->root;
    for(int i = 0; i < length; i++) {
        write_element(curr);
        curr= curr->next;
    }
}

int main() {
    List* spysok = create_empty_list();
    element* el = create_random_element();
    append_list(spysok, el->first, el->second);
    append_list(spysok, el->first, el->second);
    print_all(spysok);
    return 0;
}