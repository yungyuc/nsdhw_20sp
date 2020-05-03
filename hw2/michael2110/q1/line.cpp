#include<iostream>
#include<vector>

using namespace std;

class Line
{
public:
    Line();

    Line(Line const & line2) : mem_x(line2.get_x()),mem_y(line2.get_y()){

    }

    //Line(Line       &&);
    Line & operator=(Line const & );
    Line & operator=(Line       &&);

    Line(size_t size){
        for(size_t i=0; i<size; i++){
            mem_x.push_back(0);
            mem_y.push_back(0);
        }
    }

    ~Line(){
        size_t const s = mem_x.size();
        for(size_t i=0; i<s; i++){
            mem_x.pop_back();
            mem_y.pop_back();
        }
    }

    size_t size() const{
        return mem_x.size();
    }

    float const & x(size_t it) const{
        return mem_x[it];
    }

    float & x(size_t it){
        return mem_x[it];
    }

    float const & y(size_t it) const{
        return mem_y[it];
    }

    float & y(size_t it){
        return mem_y[it];
    }

    vector<float>get_x() const{
        return mem_x;
    }

    vector<float>get_y() const{
        return mem_y;
    }
private:
    // Member data.
    vector<float> mem_x;
    vector<float> mem_y;
}; /* end class Line */

int main(int, char **)
{
    Line line(3);
    line.x(0) = 0; line.y(0) = 1;
    line.x(1) = 1; line.y(1) = 3;
    line.x(2) = 2; line.y(2) = 5;

    Line line2(line);
    line2.x(0) = 9;

    std::cout << "line: number of points = " << line.size() << endl;
    for (size_t it=0; it<line.size(); ++it)
    {
        cout << "point " << it << ":"
                  << " x = " << line.x(it)
                  << " y = " << line.y(it) << std::endl;
    }

    std::cout << "line2: number of points = " << line.size() << endl;
    for (size_t it=0; it<line.size(); ++it)
    {
        cout << "point " << it << ":"
                  << " x = " << line2.x(it)
                  << " y = " << line2.y(it) << std::endl;
    }

    return 0;
}