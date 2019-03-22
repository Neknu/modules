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
    cout << "key: " << el->key << "\n";
}


List* create_empty_list() {
    List* spysok = new List;
    spysok->length = 0;
    spysok->root = nullptr;
    return spysok;
}


List* append_list(List* spysok) {
    element* el;
    if(spysok->length == 0) {
        el = create_random_element();
        spysok->root = el;
        el->next = spysok->root;
    }
    else {
        element* curr = spysok->root;
        for(int i = 0; i < spysok->length; i++)
            curr = curr->next;
        el = create_random_element();
        curr->next = el;
        el->next = spysok->root;
    }
    spysok->length++;
    return spysok;
}

void print_all(List* spysok) {
    int length = spysok->length;
    element* curr = spysok->root;
    for(int i = 0; i < length; i++) {
        write_element(curr);
        curr = curr->next;
    }
}

List* delete_and_create(List* spysok, int index, int count) {
    element* curr = spysok->root;
    for(int i = 0; i < index - 1; i++)
        curr = curr->next;
    for(int i = index - 1; i < index + count - 1; i++) {
        element* tmp = curr->next;
        curr->next = tmp->next;
        delete(tmp);
        spysok->length--;
    }
    for(int i = index - 1; i < index + (count / 4) - 1; i++) {
        element* new_el = create_random_element();
        new_el->next = curr->next->next;
        curr->next = new_el;
        spysok->length++;
    }
    return spysok;
}

List* transform_list(List* spysok) {
    element* curr = spysok->root;
    int deleted = 0;
    int prev_dat = 0;
    int curr_dat;
    int index_prev = 0;
    int index = 0;
    for(int i = 0; i < 1000000000000000000; i++) {
        curr_dat = curr->key - curr->next->key;
        cout << curr_dat << " ";
        cout << "\n";
        cout << index << "\n";
        if(curr_dat * prev_dat < 0)
            index++;
        else
            index = 0;
        if(index == 0 && index_prev >= 2) {
            delete_and_create(spysok, i - index_prev, index_prev + 2);
            deleted += index_prev;
        }
        index_prev = index;
        prev_dat = curr_dat;
        curr = curr->next;
        if(deleted > 1321) return spysok;
    }
    return spysok;
}

int main() {
    List* spysok = create_empty_list();
    for(int i = 0;i < 10000; i++)
        append_list(spysok);
    transform_list(spysok);
    print_all(spysok);
    return 0;
}