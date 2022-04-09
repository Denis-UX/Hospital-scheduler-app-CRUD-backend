#include <iostream>
#include <math.h>

using namespace std;

class Person //class person
{
    public:
        int age;
        const char *name;
    
        Person(const char *name, int age);//constructor 
        void getName() {
            cout <<"Hi, my name is "<< name << endl;//shows name
        }
        void setName(const char* newName) {//sets name
            name = newName;
        }
        void getAge() {//shows age
            cout << "my age is " << age << endl;;
        }       
        void setAge(int a) {//sets age
            age = a;
        }
        ~Person()
        {
            cout << "\nHello, this is a brief messege to say that the Destructor is activated!\n";
        }
};
Person::Person(const char* name, int age) {
    setName(name);
    setAge(age);
        
}


int main() //zad 8
{    
    const char *name; 
    name = "Jerry";
	Person jerry(name, 21);
    jerry.getName();
    jerry.getAge();
   
    return 0;
}

