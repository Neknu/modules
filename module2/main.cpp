#include <iostream>
#include <string>
#include <cmath>
#include <random>
#include <chrono>
#include <functional>
#include <algorithm>

int const V = 1;
int const COUNT = 127;
int const KOL = 40;
int const INF = 1000000000;

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

//task one

struct dstnc {
    double distance; //distnce between 2 components
    double first_mark;
    double second_mark;
    dstnc* left;
    dstnc* right;
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




//task 2

struct Tree {
    element* root;
    int length;
    Tree() {
        length = 0;
        root = nullptr;
    }
};

struct under_tree {
    element* root;
    int length;
    int depth;
    double max;
    double min;
    double sum;
    under_tree() {
        root = nullptr;
        length = 0;
        depth = 0;
        max = 0;
        min = 1000000000;
        sum = 0;
    }
};


Tree* create_empty_tree() {
    Tree* tre = new Tree;
    return tre;
}

Tree* add_root(Tree* tre) {
    element* root = create_random_element();
    tre->length = 1;
    tre->root = root;

    return tre;
}

element* add_new_son(Tree* tre, element* dad) {
    element* new_son = create_random_element();
    new_son->dad = dad;

    if(!dad->son)
        dad->son = new_son;
    else {
        element* curr = dad->son;
        while(curr->next)
                curr = curr->next;
        curr->next = new_son;
    }

    tre->length++;

    return new_son;
}

element* add_new_el(Tree* tre, element* dad, element* new_son) {
    new_son->dad = dad;

    if(!dad->son)
        dad->son = new_son;
    else {
        element* curr = dad->son;
        while(curr->next)
            curr = curr->next;
        curr->next = new_son;
    }

    return new_son;
}

void count_under_tree(under_tree* tre, element* root, int depth) {
    element* curr = root->son;
    while(curr) {
        tre->length++;
        tre->depth = std::max(tre->depth, depth);
        tre->max = std::max(tre->max, curr->mark);
        tre->min = std::min(tre->min, curr->mark);
        tre->sum += curr->mark;
        count_under_tree(tre, curr, depth + 1);
        curr = curr->next;
    }
}

under_tree* create_under_tree(element* root) {
    under_tree* un_tre = new under_tree;
    un_tre->root = root;
    un_tre->depth = 1;
    un_tre->length = 1;
    un_tre->max = un_tre->root->mark;
    un_tre->min = un_tre->root->mark;
    un_tre->sum = un_tre->root->mark;
    count_under_tree(un_tre, un_tre->root, 1);
    return un_tre;
}

void cout_under_tree(Tree* tre) {
    under_tree* un_tre = create_under_tree(tre->root->son);
    cout << "this is information about tree->root->son: " << "\n";
    cout << "length - " << un_tre->length << "\n";
    cout << "depth - " << un_tre->depth << "\n";
    cout << "max - " << un_tre->max << "\n";
    cout << "min - " << un_tre->min << "\n";
    cout << "sum / length - " << un_tre->sum / un_tre->length << "\n";
}

void transfer_under_tree(Tree* tre, under_tree* to_move, int level) {
    element* curr = tre->root;
    for(int i = 1; i < level; i++)
        curr = curr->son;
    element* mem = curr;
    double minim = 1000000000;
    int k = 0;
    int ans = 0;
    under_tree* un_tre;
    while(curr->next) {
        un_tre = create_under_tree(curr);
        minim = std::min(minim, un_tre->sum / un_tre->length);
        if(un_tre->sum / un_tre->length < minim) {
            minim = un_tre->length;
            ans = k;
        }
        curr = curr->next;
        k++;
    }
    k = 0;
    curr = mem;
    while(curr->next) {
        if(ans == k) {
            curr->son = to_move->root;
            to_move->root->dad->son = to_move->root->next;
            to_move->root->next = nullptr;
            cout << "success!";
        }
        curr = curr->next;
        k++;
    }

}

void cout_path_to_node(element* curr, int depth) {
    for(int i = 0; i < depth; i++)
        cout << " ---- ";
    cout << curr->mark << endl;
}

void print_tree_rekurs(Tree* tre, element* root, int depth) {
    element* curr = root->son;
    while(curr) {
        print_tree_rekurs(tre, curr, depth + 1);
        curr = curr->next;
    }
    cout_path_to_node(root, depth);
}


Tree* create_random_tree() {
    Tree* tre = create_empty_tree();
    add_root(tre);
    while(tre->length < 40) {
        int random_count = abs(int(rand_num(5))) + 1;
        for(int i = 0; i < random_count; i++)
            add_new_son(tre, tre->root);
        random_count = abs(int(rand_num(10))) + 1;
        int j = 0;
        element* curr = tre->root->son;
        while(j < random_count && curr->next) {
            add_new_son(tre, curr);
            curr = curr->next;
            j++;
        }
        random_count = abs(int(rand_num(10))) + 1;
        j = 0;
        curr = tre->root->son->son;
        while(j < random_count && curr->next) {
            add_new_son(tre, curr);
            curr= curr->next;
            j++;
        }
    }
    return tre;
}




//task 3


struct graph {
    int count;
    double matrix[KOL][KOL];
    int visited[2*KOL]; // for BFS
    int done[2*KOL]; // for Djkstra
    graph() {
        for(int i = 0; i < KOL; i++)
            for(int j = 0; j < KOL; j++)
                matrix[i][j] = 0;
        for(int i = 0; i < 2*KOL; i++)
            visited[i] = 0;
        for(int i = 0; i < 2*KOL; i++)
            done[i] = 0;
        count = 0;
    }
};

graph* build_graph(element* components[], double distance, int count) {
    graph* gr = new graph;
    gr->count = count;
    for(int i = 0; i < count; i++)
        for(int j = 0; j < count; j++)
            if(i != j)
            if(log(1 / dist(components[i], components[j])) < distance)
                gr->matrix[i][j] = int(log(1/dist(components[i], components[j])));
    return gr;
}

void print_graph(graph* gr) {
    for(int i = 0; i < gr->count; i++) {
        cout << "vertex - " << i << " - edges to - ";
        for(int j = 0; j < gr->count; j++)
            if(gr->matrix[i][j] > 0)
            cout << " " << j;
        cout << endl;
    }
}

void BFS(graph* gr, int num, int start) {
        int queue[KOL*KOL];
        queue[0] = start;
        int head = 0;
        int tail = 1;
        while (head != tail) {
            int curr = queue[head];
            gr->visited[curr] = num;
            head++;
            for (int i = 0; i < gr->count; i++) {
                if (gr->matrix[curr][i] > 0 && !gr->visited[i]) {
                    queue[tail] = i;
                    tail++;
                }
            }
        }
}


void BFS_components(graph* gr) {
    int num = 0;
    for(int i = 0; i < gr->count; i++)
        if(!gr->visited[i]) {
            num++;
            BFS(gr, num, i);
        }
    cout << "there are " << num << " components!" << "\n";
}

int minDist(graph* gr, int curr) {
    int len = INF;
    int res = -1;
    for(int i = 0; i < gr->count; i++)
        if(gr->matrix[curr][i] > 0 && !gr->visited[i] && gr->matrix[curr][i] < len) {
            len = gr->matrix[curr][i];
            res = i;
        }
    return res;
}

void Djkstra(graph* gr, int start) {
    for(int i = 0; i < gr->count; i++)
        gr->visited[i] = 0;
    int queue[2*KOL];
    double distance[2*KOL];
    for(int i = 0; i < gr->count; i++)
        distance[i] = INF;
    queue[0] = start;
    distance[start] = 0;
    int head = 0;
    int tail = 1;
    while(head != tail) {
        int curr = queue[head];
        if(gr->done[curr]) {
            head++;
            continue;
        }
        gr->done[curr] = true;
        head++;
        for(int i = 0; i < gr->count; i++)
            gr->visited[i] = false;
        int next = minDist(gr, curr);
        while(next != -1) {
            gr->visited[next] = true;
            if(!gr->done[next] && distance[curr] + gr->matrix[curr][next] < distance[next]) {
                queue[tail] = next;
                distance[next] = distance[curr] + gr->matrix[curr][next];
                tail++;
            }
            next = minDist(gr, curr);
        }
    }
    cout << "this is distances from vertex " << start << " to another" << "\n";
    for(int i = 0; i < gr->count; i++)
        cout << distance[i] << " ";
}

void print_matrix(graph* gr) {
    for(int i = 0; i < gr->count; i++) {
        for (int j = 0; j < gr->count; j++)
            cout << gr->matrix[i][j] << " ";
        cout << endl;
    }
}

//task 4



dstnc* newNode(double item) {
    dstnc *temp =  new dstnc;
    temp->distance = item;
    temp->left = temp->right = nullptr;
    return temp;
}


void printBinTree(struct dstnc *root, int depth) {
    if (root != nullptr)
    {
        printBinTree(root->left, depth + 1);
        for(int i = 0; i < depth; i++)
            cout << " --";
        cout << root->distance << "\n";
        printBinTree(root->right, depth + 1);
    }
}


dstnc* insert(dstnc* dstnc, double key) {

    if (dstnc == nullptr) return newNode(key);

    if (key < dstnc->distance)
        dstnc->left  = insert(dstnc->left, key);
    else if (key > dstnc->distance)
        dstnc->right = insert(dstnc->right, key);

    return dstnc;
}

dstnc* find(dstnc* dstnc, double key) {
    while(dstnc) {
        if(dstnc->distance == key)
            return dstnc;
        if(dstnc->distance > key)
            dstnc = dstnc->right;
        if(dstnc->distance < key)
            dstnc = dstnc->left;
    }
    return nullptr;
}


dstnc* build_bin_tree(dstnc* distances[], int count_dist) {
    dstnc *root = nullptr;
    root = insert(root, distances[0]->distance);
    for(int i = 0; i < count_dist; i++)
        insert(root, distances[i]->distance);
    printBinTree(root, 1);
    return root;
}

int main() {
    element* components[COUNT + 1];



    //this is for task one

    struct dstnc* distances[COUNT*COUNT + 1];
    for(int i = 0; i < COUNT; i++) {
        components[i] = create_random_element();
    }
    quickSort(components, 0, COUNT - 1);
    int count_dist;
    count_dist = create_distances(components, distances, 126);
    quickSort(distances, 0, count_dist - 1);
    print_distances(distances, 0, 12);
    print_distances(distances, count_dist - 12, count_dist);
    print_elements(components, COUNT);

    // task 2
//    Tree* tre = create_random_tree();
//    print_tree_rekurs(tre, tre->root, 1);
//    cout_under_tree(tre);
//    under_tree* un_tre = create_under_tree(tre->root->son);
//    transfer_under_tree(tre, un_tre, 3);
//    print_tree_rekurs(tre, tre->root, 1);

    // task 3
//    for(int i = 0; i < KOL; i++) {
//        components[i] = create_random_element();
//    }
//    cout << "enter distance for adding edges (advise to use 100)" << "\n";
//    double distance;
//    cin >> distance; // my advise to use 100KK
//    graph* gr = build_graph(components, distance, KOL - 1);
//    print_graph(gr);
// //   print_matrix(gr);
//    BFS_components(gr);
//    Djkstra(gr, 1);

    //task 4

//    dstnc* distances[KOL*KOL + 1];
//    for(int i = 0; i < KOL/4; i++) {
//       components[i] = create_random_element();
//    }
//    int count_dist = create_distances(components, distances, KOL/4);
//    dstnc* root = build_bin_tree(distances, count_dist);
//
//    dstnc* find_dist = find(root, distances[4]->distance);
//    if(find_dist)
//        cout << "this is data from finded node - " << find_dist->distance << "\n";
//    else
//        cout << "this node doesn't exist!" << "\n";

    return 0;
}