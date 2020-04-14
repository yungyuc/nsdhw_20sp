#include <cstdint> // size_t
#include <stdexcept> // std::out_of_range
#include <vector>

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
        float const & x(size_t it) const;
        float & x(size_t it) ;
        float const & y(size_t it) const;
        float & y(size_t it) ;
    private:
        void check_range(size_t it) const;
        std::vector<float> m_coord;
}; /* end class Line */