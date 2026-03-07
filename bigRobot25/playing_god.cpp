// let's communicate in a standard way!
#include <iostream>
using namespace std;

// let's make a person outline!
class Person {
    private:
        int _age = 0;
        string _name;
        const int _MAX_AGE = 100;
        bool _alive = true;

    public:
        Person(string name) {
            _name = name;
        }

        void happy_birthday() {
            if (is_alive())                 
                _age++;
            cout << "Happy Birthday!! You're already " << _age << " years old. You're growing up so fast." << endl;
        }

        void die() {
            _alive = false;
        }

        bool is_alive() {
            if (_age >= _MAX_AGE) 
                die(); 

            return _alive;
        }

        int how_old() {
            return _age;
        }
};

int main() {
    // let's try making a person!!!
    string name = "test";
    Person test = (name);

    // happy birthday, friend! you're getting old :D
    while (test.is_alive()) {
        test.happy_birthday();
        cout << test.how_old() << endl;
    }

    // where did you go?
    cout << test.is_alive() << endl;
    cout << test.is_alive() << endl;
    cout << test.is_alive() << endl;
    cout << test.is_alive() << endl;
    cout << test.is_alive() << endl;

    // happybirthdayhappybirthdayhappybirthdayhappybirthdayhappybirthdayhappybirthdayhappybirthdayhappybirthdayhappybirthday
    while(true) {
        test.happy_birthday();
    }
}