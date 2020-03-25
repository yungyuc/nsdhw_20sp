#include <iostream>
#include <vector>

class Line
{
public:
    // Basic build-in function
    Line(){};
    Line(Line const &l2){
        mSize = l2.size();
        mX = std::vector<float> (l2.getXs());
        mY = std::vector<float> (l2.getYs());
    }

    Line & operator=(Line const & );
    Line & operator=(Line       &&);
    Line(size_t size)
    {
        mSize = size;
        mX.resize(size);
        mY.resize(size);
    }
    
    ~Line(){
        mX.clear();
        mY.clear();
    }
    
    // Method
    size_t size() const { return mSize; }

    float const & x(size_t it) const { return mX[it]; }
    float & x(size_t it) { return mX[it]; }
    std::vector<float> const & getXs() const { return mX; }

    float const & y(size_t it) const { return mY[it]; }
    float & y(size_t it) { return mY[it]; }
    std::vector<float> const & getYs() const { return mY; }

private:
    // Member data.
    int mSize = 0;
    std::vector<float> mX;
    std::vector<float> mY;

}; /* end class Line */

// Testcase
int main(int, char **)
{
    Line line(3);
    line.x(0) = 0; line.y(0) = 1;
    line.x(1) = 1; line.y(1) = 3;
    line.x(2) = 2; line.y(2) = 5;

    Line line2(line);
    line2.x(0) = 9;

    std::cout << "line: number of points = " << line.size() << std::endl;
    for (size_t it=0; it<line.size(); ++it)
    {
        std::cout << "point " << it << ":"
                  << " x = " << line.x(it)
                  << " y = " << line.y(it) << std::endl;
    }

    std::cout << "line2: number of points = " << line.size() << std::endl;
    for (size_t it=0; it<line.size(); ++it)
    {
        std::cout << "point " << it << ":"
                  << " x = " << line2.x(it)
                  << " y = " << line2.y(it) << std::endl;
    }

    return 0;
}