#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <algorithm>
using namespace std;

struct S1 {
    char Name[20];
    char Section[10];

    S1() {
        strcpy_s(Name, "");
        strcpy_s(Section, "");
    }

    friend ostream& operator<<(ostream& out, S1& current);
    friend istream& operator>>(istream& in, S1& current);
    friend bool operator==(S1& first, S1& second);
    friend bool operator<(const S1& first, const S1& second);

};

ostream& operator<<(ostream& out, S1& current) {
    out << left << setw(20) << current.Name << setw(10) << current.Section;
    return out;
}
istream& operator>>(istream& in, S1& current) {
    string name;
    getline(in, name);
    strcpy_s(current.Name, name.c_str());
    string section;
    getline(in, section);
    strcpy_s(current.Section, section.c_str());
    return in;
}
bool operator==(S1& first, S1& second) {
    string n1 = ""; n1 += first.Name;
    string n2 = ""; n2 += second.Name;
    return n1 == n2;
}
bool operator<(const S1& first, const S1& second) {
    string n1 = ""; n1 += first.Name;
    string n2 = ""; n2 += second.Name;
    return n1 < n2;
}

struct S2 {
    char Name[20];
    int Num;
    char Section[10];

    S2() {
        strcpy_s(Name, "");
        Num = false;
        strcpy_s(Section, "");
    }

    friend ostream& operator<<(ostream& out, S2& current);
    friend istream& operator>>(istream& in, S2& current);
    friend bool operator==(S2& first, S2& second);
    friend bool operator<(const S2& first, const S2& second);
};

ostream& operator<<(ostream& out, S2& current) {
    out << left << setw(20) << current.Name << setw(10) << current.Num << setw(10) << current.Section;
    return out;
}
istream& operator>>(istream& in, S2& current) {
    string name;
    getline(in, name);
    strcpy_s(current.Name, name.c_str());
    string num; getline(in, num); current.Num = stoi(num);
    string section;
    getline(in, section);
    strcpy_s(current.Section, section.c_str());
    return in;
}
bool operator==(S2& first, S2& second) {
    string n1 = ""; n1 += first.Name;
    string n2 = ""; n2 += second.Name;
    return n1 == n2;
}
bool operator<(const S2& first, const S2& second) {
    string n1 = ""; n1 += first.Name;
    string n2 = ""; n2 += second.Name;
    return n1 < n2;
}

class ABS_COMP {
public:
    virtual void GetData(istream& in) = 0;
    virtual void PutData(ostream& out) const = 0;
    virtual void Merge(ABS_COMP* first, ABS_COMP* second) = 0;
    virtual void Sort() = 0;
};
class Fruit : public ABS_COMP {
private:
    int count_;
    S1* arr_;
   
public:
    Fruit() {
        count_ = 0;
        arr_ = nullptr;
    }
    Fruit(int size) : count_(size) {
        arr_ = new S1[size];
    }
    ~Fruit() {
        delete[] arr_;

    }
    void GetData(istream& in) override {
        in >> count_;
        arr_ = new S1[count_];
        string n; getline(in, n);
        for (int i = 0; i < count_; i++) {
            in >> arr_[i];
        }
    }
    void PutData(ostream& out) const override {
        out << left << setw(20) << "Name" << setw(10) << "Section" << endl << endl;;
        for (int i = 0; i < count_; i++) {
            out << arr_[i] << endl;
        }
    }
    void Sort() override {
        sort(arr_, arr_ + count_);
    }
    void SortBySection() {
        sort(arr_, arr_ + count_, [](const S1& a, const S1& b)->bool {string n1 = ""; n1 += a.Section;
        string n2 = ""; n2 += b.Section;
        return n1 < n2; });
    }
    Fruit& operator= (Fruit& other) {
        if (this != &other) {
            delete[] arr_;
            arr_ = new S1[other.count_];
            count_ = other.count_;
            for (int i = 0; i < other.count_; i++) {
                strcpy_s(arr_[i].Name, other.arr_[i].Name);
                strcpy_s(arr_[i].Section, other.arr_[i].Section);
            }
        }
        return *this;
    }
    int GetCount() {
        return count_;
    }
    void Merge(ABS_COMP* first, ABS_COMP* second) override {
        if (arr_ != nullptr) delete[] arr_;
        arr_ = new S1[((Fruit*)first)->count_ + ((Fruit*)second)->count_];
        first->Sort();
        second->Sort();
        int i = 0, j = 0, s = 0;
        for (; i < ((Fruit*)first)->count_ && j < ((Fruit*)second)->count_;) {
            if ((*(Fruit*)first).arr_[i] == (*(Fruit*)second).arr_[j]) {
                arr_[s] = (*(Fruit*)first).arr_[i];
                ++s; ++i; ++j;
            } 
            else if ((*(Fruit*)first).arr_[i] < (*(Fruit*)second).arr_[j]) ++i;
            else ++j;
        }
        S1* new_arr = new S1[s];
        for (int i = 0; i < s; i++) {
            strcpy_s(new_arr[i].Name, arr_[i].Name);
            strcpy_s(new_arr[i].Section, arr_[i].Section);
        }
        delete[] arr_;
        count_ = s;
        arr_ = new_arr;
    }
    S1 Find(string name) {
        Sort();
        S1 x; strcpy_s(x.Name, name.c_str());
        S1 result = *lower_bound(arr_, arr_ + count_, x, [](const S1& a, const S1& b)->bool {string n1 = ""; n1 += a.Name;
        string n2 = ""; n2 += b.Name;
        return n1 < n2; });
        if (name == result.Name) return result;
        else return S1();
    }

    void Print() {
        for (int i = 0; i < count_; i++) {
            cout << arr_[i] << endl;
        }
    }
};
class Bag : public ABS_COMP {
private:
    int count_;
    S2* arr_;
public: 
    Bag() {
        count_ = 0;
        arr_ = nullptr;
    }
    Bag(int size) : count_(size) {
        arr_ = new S2[size];
    }
    ~Bag() {
        delete[] arr_;

    }
    void GetData(istream& in) override {
        in >> count_;
        arr_ = new S2[count_];
        string n; getline(in, n);
        for (int i = 0; i < count_; i++) {
            in >> arr_[i];
        }
    }
    void PutData(ostream& out) const override {
        out << left << setw(20) << "Name" << setw(10) << "Num" << setw(10) << "Section" << endl << endl;;
        for (int i = 0; i < count_; i++) {
            out << arr_[i] << endl;
        }
    }
    void Sort() override {
        sort(arr_, arr_ + count_);
    }
    void SortBySection() {
        sort(arr_, arr_ + count_, [](const S2& a, const S2& b)->bool {string n1 = ""; n1 += a.Section;
        string n2 = ""; n2 += b.Section;
        return n1 < n2; });
    }
    Bag& operator= (Bag& other) {
        if (this != &other) {
            delete[] arr_;
            arr_ = new S2[other.count_];
            count_ = other.count_;
            for (int i = 0; i < other.count_; i++) {
                strcpy_s(arr_[i].Name, other.arr_[i].Name);
                strcpy_s(arr_[i].Section, other.arr_[i].Section);
            }
        }
        return *this;
    }
    int GetCount() {
        return count_;
    }
    void Merge(ABS_COMP* first, ABS_COMP* second) override {
        if (arr_ != nullptr) delete[] arr_;
        arr_ = new S2[((Bag*)first)->count_ + ((Bag*)second)->count_];
        first->Sort();
        second->Sort();
        int i = 0, j = 0, s = 0;
        for (; i < ((Bag*)first)->count_ && j < ((Bag*)second)->count_;) {
            if ((*(Bag*)first).arr_[i] == (*(Bag*)second).arr_[j]) {
                arr_[s] = (*(Bag*)first).arr_[i];
                ++s; ++i; ++j;
            }
            else if ((*(Bag*)first).arr_[i] < (*(Bag*)second).arr_[j]) ++i;
            else ++j;
        }
        S2* new_arr = new S2[s];
        for (int i = 0; i < s; i++) {
            strcpy_s(new_arr[i].Name, arr_[i].Name);
            new_arr[i].Num = arr_[i].Num;
            strcpy_s(new_arr[i].Section, arr_[i].Section);
        }
        delete[] arr_;
        count_ = s;
        arr_ = new_arr;
    }
    S2 Find(string name) {
        Sort();
        S2 x; strcpy_s(x.Name, name.c_str());
        S2 result = *lower_bound(arr_, arr_ + count_, x, [](const S2& a, const S2& b)->bool {string n1 = ""; n1 += a.Name;
        string n2 = ""; n2 += b.Name;
        return n1 < n2; });
        if (name == result.Name) return result;
        else return S2();
    }
};


int main() {
    Fruit M1, M2, M3;
    Bag K1, K2, K3;
    ABS_COMP* A1, * A2, * A3, * A4;
    ifstream in1("input\\Ob1.txt"); ifstream in2("input\\Ob2.txt");
    ofstream out1("output\\Ob.txt");
    A1 = &M1; A1->GetData(in1);
    A2 = &M2; A2->GetData(in2);
    A3 = &M3; A3->Merge(A1, A2); A3->PutData(out1);

    ifstream in3("input\\TOb1.txt"); ifstream in4("input\\TOb2.txt");
    ofstream out2("output\\TOb.txt");
    A1 = &K1; A1->GetData(in3);
    A2 = &K2; A2->GetData(in4);
    A4 = &K3; A4->Merge(A1, A2); A4->PutData(out2);

    S1 apple = M1.Find("Apple");
    S1 empty1 = M1.Find("QQQ");
    cout << apple << endl;
    cout << empty1 << endl;

    S2 banana = K1.Find("Banana");
    S2 empty2 = K1.Find("QQQ");
    cout << banana << endl;
    cout << empty2 << endl;
    
    M1.Print();
    cout << endl;
    M2.Print();
    cout << endl << "Merge: " << endl;
    M3.Print();


    return 1;
}
