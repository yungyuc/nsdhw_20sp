#include <cstddef>
#include <vector>
#include <utility>

typedef std::pair<float, float> coord_2d; 

#ifndef LINE_HPP
#define LINE_HPP

class Line
{
public:
    Line() = default;
    Line(Line const & );
    Line(Line       &&);
    Line & operator=(Line const & );
    Line & operator=(Line       &&);
    Line(size_t size);
    ~Line();
    size_t size() const;
    float & x(size_t it) const;
    float & x(size_t it);
    float & y(size_t it) const;
    float & y(size_t it);

private:
    size_t _length = 0;
    std::vector<coord_2d> *m_coord = nullptr;
};

#endif
