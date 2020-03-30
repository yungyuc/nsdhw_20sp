#include <iostream>
#include <vector>

using namespace std;

class Line
{

public:

    Line(){
	}

    Line(Line const& line2 ) : m_x(line2.getX()),m_y(line2.getY()){ 
	}

    //Line(Line       &&);
    Line & operator=(Line const & );
    Line & operator=(Line       &&);

    Line(size_t size) {
		for(size_t i=0;i<size;i++){
			m_x.push_back(0);
			m_y.push_back(0);
		}
	}

    ~Line(){
		size_t const s = m_x.size();
		for (size_t i =0;i<s;i++){
			m_x.pop_back();
			m_y.pop_back();
		}
	}
	
    size_t size() const{
		return m_x.size();	
	}

    float const& x(size_t it) const{
		return m_x[it];
	}
    float & x(size_t it){
		return m_x[it];
	}

    float const& y(size_t it) const{
		return m_y[it];
	}
    float & y(size_t it){
		return m_y[it];
	}
	
	vector<float> getX() const {
		return m_x;
	}
	vector<float> getY() const {
		return m_y;
	}

private:

    // Member data.
	vector<float> m_x;
	vector<float> m_y;

}; /* end class Line */

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
