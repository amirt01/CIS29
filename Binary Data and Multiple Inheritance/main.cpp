/******************************
 * Name: Amir Tadros          *
 * Assignment # 3             *
 * Compiler: CodeBlocks 20.03 *
 * OS: Windows 10             *
 *****************************/

#include <iostream>
#include <fstream>
#include <iomanip>
#include <typeinfo>
#include <string>
using namespace std;

const double PI = 3.1415926535897932;

/*************CLASSES*************/

class Thing
{
public:
    Thing() = default;
    virtual ~Thing() = 0;
    virtual string name() const;
};

Thing::~Thing(){}

string Thing::name() const
{
    string name = typeid(*this).name();
#ifdef _MSC_VER       // for MS Visual Studio
    name = quadname.name(6);
#else                 // for other compilers
    name = name.substr(name.find_first_not_of("0123456789"));
#endif
    return name;
}

class ColoredThing : virtual public Thing
{
protected:
    std::string color;
    double weight;
public:
    ColoredThing(std::string color, double weight)
        : Thing(), color(color), weight(weight) {}
    virtual const double density() const = 0;
    friend ostream& operator<<(ostream& out, const ColoredThing& ColoredThing);
};

ostream& operator<<(ostream& out, const ColoredThing& colthing)
{
    // first call the Quadrilateral insertion operator
    out << " " << setw(14) << left << colthing.color << setw(6) << right << setprecision(4) << fixed << colthing.density();
    return out;
}

class GeometricSolid : virtual public Thing
{
public:
    GeometricSolid()
        : Thing() {}
    virtual const double volume() const = 0;
    friend ostream& operator<<(ostream& out, const GeometricSolid& geomsolid);
};

ostream& operator<<(ostream& out, const GeometricSolid& geomsolid)
{
    out << setw(27) << left << geomsolid.name() << setw(9) << right << setprecision(4) << fixed << geomsolid.volume();
    return out;
}

class RectangularPrism : virtual public GeometricSolid
{
protected:
    float length, width, height;
public:
    RectangularPrism(double length, double width, double height)
        : GeometricSolid(), length(length), width(width), height(height) {}
    RectangularPrism() {}
    const double volume() const { return length * width * height; }
};

class Circular : virtual public GeometricSolid
{
protected:
    double radius;
public:
    Circular(double radius) : GeometricSolid(), radius(radius) {}
    Circular() {}
};

class Cylinder : public Circular
{
protected:
    double height;
public:
    Cylinder(double height, double radius)
        : Circular(radius), height(height) {}
    const double volume() const { return (PI * height * radius * radius); }
};

class Sphere : public Circular
{
public:
    Sphere(double radius)
        : Circular(radius), GeometricSolid() {}
    const double volume() const { return ((4 * PI * radius * radius * radius) / 3); }
};

class Cube : public RectangularPrism
{
public:
    Cube(float length)
        : RectangularPrism(length, length, length) {}
};

class ColoredCylinder : public Cylinder, virtual public ColoredThing
{
public:
    ColoredCylinder(double height, double radius, std::string color, double weight)
        : Cylinder(height, radius), ColoredThing(color, weight) {}
    const double density() const { return ( weight / volume()); }
};

class ColoredSphere : public Sphere, virtual public ColoredThing
{
public:
    ColoredSphere(double radius, std::string color, double weight)
        : Sphere(radius), ColoredThing(color, weight) {}
    const double density() const { return ( weight / volume()); }
};

class ColoredRectangularPrism : public RectangularPrism, virtual public ColoredThing
{
public:
    ColoredRectangularPrism (double length, double width, double height, std::string color, double weight)
        : RectangularPrism(length, width, height), ColoredThing(color, weight) {}
    const double density() const { return ( weight / volume()); }
};

class ColoredCube : public Cube, virtual public ColoredThing
{
public:
    ColoredCube (double length, std::string color, double weight)
        : Cube(length), ColoredThing(color, weight) {}
    const double density() const { return ( weight / volume()); }
};

/*************BIT MANIPULATION*************/

void toggle(unsigned char& data) { data ^= 0x81; }

unsigned char leftNibble(unsigned char data) { return data >> 4; }
unsigned char rightNibble(unsigned char data) { return data & 0xF; }

/*************MAIN*************/

int main()
{
    unsigned char data;
    double height, radius, length, width, weight;
    std::string color = "";
    Thing* thing;

    cout << "Type of Object                Volume Color        Density" << endl;

    fstream fin("ass3data.bin", ios::in | ios::binary);
    if(!fin.is_open())
    {
        cout << "Failed to open input file!" << endl;
        exit(0);
    }

    while(!fin.eof())
    {
        fin.read(reinterpret_cast<char*>(&data), 1);
        toggle(data);

        switch(rightNibble(data))
        {
        case(0x1): // Red
            color = "Red";
            break;
        case(0x2): // Blue
            color = "Blue";
            break;
        case(0x4): // Green
            color = "Green";
            break;
        case(0x8): // Yellow
            color = "Yellow";
            break;
        default:
            color = "";
            break;
        }

        switch(leftNibble(data))
        {
        case(0x1): // Cylinder
            fin.read(reinterpret_cast<char*>(&radius), sizeof(radius));
            fin.read(reinterpret_cast<char*>(&height), sizeof(height));
            if (color == "")
            {
                thing = new Cylinder(height, radius);
                cout << *dynamic_cast<GeometricSolid*>(thing);
            }
            else
            {
                fin.read(reinterpret_cast<char*>(&weight), sizeof(weight));
                thing = new ColoredCylinder(height, radius, color, weight);
                cout << *dynamic_cast<GeometricSolid*>(thing);
                cout << *dynamic_cast<ColoredThing*>(thing);
            }
            break;
        case(0x2): // Sphere
            fin.read(reinterpret_cast<char*>(&radius), sizeof(radius));
            if (color == "")
                cout << *dynamic_cast<GeometricSolid*>(new Sphere(radius));
            else
            {
                fin.read(reinterpret_cast<char*>(&weight), sizeof(weight));
                thing = new ColoredSphere(radius, color, weight);
                cout << *dynamic_cast<GeometricSolid*>(thing);
                cout << *dynamic_cast<ColoredThing*>(thing);
            }
            break;
        case(0x4): // Rectangular Prism
            fin.read(reinterpret_cast<char*>(&length), sizeof(length));
            fin.read(reinterpret_cast<char*>(&width), sizeof(width));
            fin.read(reinterpret_cast<char*>(&height), sizeof(radius));
            if (color == "")
            {
                thing = new RectangularPrism(length, width, height);
                cout << *dynamic_cast<GeometricSolid*>(thing);
            }
            else
            {
                fin.read(reinterpret_cast<char*>(&weight), sizeof(weight));
                thing = new ColoredRectangularPrism(length, width, height, color, weight);
                cout << *dynamic_cast<GeometricSolid*>(thing);
                cout << *dynamic_cast<ColoredThing*>(thing);
            }
            break;
        case(0x8): // Cube
            fin.read(reinterpret_cast<char*>(&length), sizeof(length));
            if (color == "")
            {
                thing = new Cube(length);
                cout << *dynamic_cast<GeometricSolid*>(thing);
            }
            else
            {
                fin.read(reinterpret_cast<char*>(&weight), sizeof(weight));
                thing = new ColoredCube(length, color, weight);
                cout << *dynamic_cast<GeometricSolid*>(thing);
                cout << *dynamic_cast<ColoredThing*>(thing);
            }
            break;
        }
        cout << endl;
    }
    delete thing;
    return 0;
}
