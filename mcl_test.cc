#include <iostream>
#include <cmath>
#include <mycadlib.h>
#include <memory>
#include <chrono>
#include <random>
#include <algorithm> //sort
#include <bitset>


template <class T>
class circle3_colored : public circle3<T> {
public:
    using circle3<T>::circle3;

    void echo() { std::cout<<"circle3_colored: "<<rgba<<std::endl; }

    size_t rgba = 0xFF00FF'FF;
};


int main() {
    using namespace std;

    using space_type = double;

    std::mt19937_64 r(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<size_t> dis_vector_size(10, 20);
    std::uniform_int_distribution<size_t> dis_curve_type(0, 3); // ellispe3, circle3, helix3, circle3_colored
    std::uniform_real_distribution<> dis_space(-10, 10);
    std::uniform_real_distribution<> dis_positive(0, 10);

    /// Populate a container (e.g. vector or list) of objects of these types created in random manner with random parameters
    std::vector<std::shared_ptr<curve3<space_type>>> curves;
    curves.reserve(dis_vector_size(r));
    for(size_t i=0; i<curves.capacity(); i++) {
        switch(dis_curve_type(r)) {
            case 0:
                curves.push_back(make_shared<ellipse3<space_type>>(dis_space(r), dis_space(r), dis_positive(r), dis_positive(r)));
                break;
            case 1:
                curves.push_back(make_shared<circle3<space_type>>(dis_space(r), dis_space(r), dis_positive(r)));
                break;
            case 2:
                curves.push_back(make_shared<helix3<space_type>>(dis_space(r), dis_space(r), dis_space(r), dis_positive(r), dis_positive(r)));
                break;
            case 3:
            default:
                curves.push_back(make_shared<circle3_colored<space_type>>(dis_space(r), dis_space(r), dis_positive(r)));
        }
    }

    /// Populate a second container that would contain only circles from the first container
    std::vector<std::shared_ptr<circle3<space_type>>> circles;
    size_t c_ellipse = 0;
    size_t c_circle = 0;
    size_t c_helix = 0;
    size_t c_circle_colored = 0;
    for(auto& curve_p : curves) {
        cout<<typeid(*curve_p.get()).name()<<endl;
        if (dynamic_pointer_cast<ellipse3<space_type>>(curve_p)) {
            cout<<"  ellipse3"<<endl;
            c_ellipse++;
        }
        if (auto circle = dynamic_pointer_cast<circle3<space_type>>(curve_p)) {
            cout<<"  circle3"<<endl;
            c_circle++;
            circles.push_back(circle);
            if (&circles.back()->position != &curve_p.get()->position)
                throw std::runtime_error("Make sure the second container shares (i.e. not clones) circles of the first one");
        }
        if (dynamic_pointer_cast<helix3<space_type>>(curve_p)) {
            cout<<"  helix3"<<endl;
            c_helix++;
        }
        if (dynamic_pointer_cast<circle3_colored<space_type>>(curve_p)) {
            cout<<"  circle3_colored"<<endl;
            c_circle_colored++;
        }
    }
    cout<<"curves: "+to_string(curves.size())+", ellipses: "+to_string(c_ellipse)+", circles: "+to_string(c_circle)+", helices: "+to_string(c_helix)+", circles_colored: "+to_string(c_circle_colored)<<endl;

    if (!(c_ellipse && c_circle && c_helix))
        throw std::runtime_error("Ensure that the container will contain objects of all supported types");

    /// Print coordinates of points and derivatives of all curves in the container at t=PI/4.
    cout<<std::fixed<<std::setprecision(2);
    for(auto& curve_p : curves) {
        auto *curve = curve_p.get();
        cout<< typeid(*curve).name()<<": C(t)="<<curve->calculate(M_PIl/4)<<", dC(t)/dt="<<curve->derivative(M_PIl/4)<<", where t=PI/4"<<endl;
    }
    /// Sort the second container in the ascending order of circles’ radii
    std::sort(circles.begin(), circles.end(), [](std::shared_ptr<circle3<space_type>> &lhs, std::shared_ptr<circle3<space_type>> &rhs) {
        return lhs->a < rhs->a;
    });

    /// That is, the first element has the smallest radius, the last - the greatest.
    cout<<"first: "<<circles.front()->a<<", last: "<<circles.back()->a<<endl;

    /// Compute the total sum of radii of all curves in the second container
    space_type radii_sum = 0;
    for(const auto& circle_p : circles) {
        radii_sum += circle_p->a;
        cout<<"  circle radii: "<<circle_p->a<<endl;
    }
    cout<<"total sum of radii: "<<radii_sum<<endl;

    /// any point on helix satisfies the condition C(t + 2*PI) = C(t) + {0, 0, step}.
    helix3<space_type> h1(
            1, 2, 3,    // x, y, z
            5, 5,       // a, b
            2,          // step
            M_PIl/3     // angle_start
    );
    vector3<space_type> a = h1.calculate(10);
    a.z += h1.step;
    vector3<space_type> b = h1.calculate(10 + 2*M_PIl);
    cout<<"a: "<<a<<", b: "<<b<<endl;

    return 0;
}
