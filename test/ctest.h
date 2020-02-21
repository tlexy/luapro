#include <string>
#include <iostream>

class A 
{
    public:
        static int staticData;
        static float staticProperty;

        static float getStaticProperty () { return staticProperty; }
        static void setStaticProperty (float f) { staticProperty = f; }
		static void staticFunc() { std::cout << "staticFunc call" << std::endl; }

        std::string dataMember;

        char dataProperty;
        char getProperty () const { return dataProperty; }
        void setProperty (char v) { dataProperty = v; }
        std::string toString () const { return dataMember; }

        void func1 () { }
        virtual void virtualFunc () { }

};

enum e_fire
{
    think = 1,
    through
};

class AnimalDuck
{
public:
    AnimalDuck()
    {}
    AnimalDuck(int age)
        :_age(age)
    {}

    void print()
    {
        std::cout << "I\' a  duck, my age is " << _age << std::endl;
    }

	static void name()
	{
		std::cout << "static function name..." << std::endl;
	}

private:
    int _age;
};
