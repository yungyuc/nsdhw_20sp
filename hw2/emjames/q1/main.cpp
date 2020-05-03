#include <iostream>
#include <ostream>
#include <vector>

using namespace std;

/* begin class Line */
class Line {
  private:
    int point_number;
    vector<float> m_vector_X;
    vector<float> m_vector_Y;

  public:
    Line(int);
    Line & operator = (Line const & );

    // getters and setters
    int const     size () const { return point_number; }
    int           x (size_t index) { return m_vector_X.at(index); }
    int           y (size_t index) { return m_vector_Y.at(index); }
    void const    x (size_t index, float value) { m_vector_X.at(index) = value; }
    void const    y (size_t index, float value) { m_vector_Y.at(index) = value; }
};

// Line constructor
Line::Line (int num) {
  // Keep track of the initialized size
  point_number = num;
  m_vector_X.resize(point_number);
  m_vector_Y.resize(point_number);
};
/* end class Line */

// Print all of the points in the line
void print_line(string line_name, Line line){
  cout << line_name << ": number of points = " << line.size() << endl;
  for (size_t it=0; it < line.size(); ++it){
    cout  << "point " << it << ":"
          << " x = " << line.x(it)
          << " y = " << line.y(it) << endl;
  }
}

int main(void)
{

  Line line(3);
  line.x(0, 0); line.y(0, 1);
  line.x(1, 1); line.y(1, 3);
  line.x(2, 2); line.y(2, 5);

  Line line2(line);
  line2.x(0, 9);

  print_line("line", line);
  print_line("line2", line2);

  return 0;
}
