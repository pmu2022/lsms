

#include <iostream>

#include "Structure.hpp"
#include <Eigen/Dense>

int main(int argc, char *argv[]) {

    using namespace SC;
    using namespace Eigen;

    int k;
    int q;

    Matrix<double, 3, 3> lattice;
    Matrix<double, 3, 3> lll;
    Matrix<double, 3, 3> lll_mapping;
    Matrix<double, 3, 3> lll_inverse;

    lattice << 2.0, 0.0, 0.0,
            0.1, 1.8, 0.0,
            0.1, 0.2, 0.9;

    Matrix<double, 3, 3> a;
    Matrix<double, 3, 3> b;
    Matrix<double, 3, 3> u;
    Vector<double, 3> m;
    double mu;
    Matrix<double, 3, 3> mapping;
    Vector<double, 10> uu;
    Vector<double, 3> v;
    Vector<double, 3> v_m;
    Matrix<double, 3, 3> qq;
    Vector<double, 3> pp;

    double delta = 0.75;

    // Basis vectors need to be in column representation
    a = lattice.transpose();

    b.setZero(); // # Vectors after the Gram-Schmidt process
    u.setZero(); // Gram-Schmidt coefficients
    m.setZero(); //

    b.col(0) = a.col(0);
    m(0) = b.col(0).dot(b.col(0));

    for (auto i = 1; i <= 2; i++) {
        u(i, seq(0, i - 1)) = a(indexing::all, i).transpose() * b(indexing::all, seq(0, i - 1));
        u(i, seq(0, i - 1)) = u(i, seq(0, i - 1)).array() / m(seq(0, i - 1)).transpose().array();
        b(indexing::all, i) = a(indexing::all, i) -
                              b(indexing::all, seq(0, i - 1)) * u(i, seq(0, i - 1)).transpose();
        m(i) = b(indexing::all, i).transpose() * b(indexing::all, i);
    }

    k = 2;

    mapping.setIdentity();

    while (k <= 3) {


        /*
         * Size reduction
         */
        for (int i = k - 1; i > 0; i--) {

            mu = u(k - 1, i - 1);


            if (std::fabs(mu) > 0.5) {

                q = std::rint(u(k - 1, i - 1));
                a(indexing::all, k - 1) = a(indexing::all, k - 1) - q * a(indexing::all, i - 1);
                mapping(indexing::all, k - 1) = mapping(indexing::all, k - 1) - q * mapping(indexing::all, i - 1);

                uu.setZero();


                if (i >= 2) {
                    uu(seq(0, (i - 2))) = u(i - 1, seq(0, (i - 2)));
                }

                uu(i - 1) = 1.0;

                u(k - 1, seq(0, i - 1)).array() = u(k - 1, seq(0, i - 1)).array()
                                                  - q * uu(seq(0, i - 1)).transpose().array();

            }
        }


        double norm1 = b(indexing::all, k - 1).squaredNorm();
        double norm2 = (delta - std::pow(std::fabs(u(k - 1, k - 2)), 2)) * b(indexing::all, k - 2).squaredNorm();

        if (norm1 >= norm2) {
            k += 1;
        } else {

            //v = a(indexing::all, k - 1);
            //a(indexing::all, k - 1) = a(indexing::all, k - 2);
            //a(indexing::all, k - 2) = v;

            a.col(k - 1).swap(a.col(k - 2));
            mapping.col(k - 1).swap(mapping.col(k - 2));

            // H.col(i).swap(H.col(j))

            //v_m = mapping(indexing::all, k - 1);
            //mapping(indexing::all, k - 1) = mapping(indexing::all, k - 2);
            //mapping(indexing::all, k - 2) = v_m;


            for (int s = k - 1; s < k + 1; s++) {
                u(s - 1, seq(0, s - 2)) = a(indexing::all, s - 1).transpose() * b(indexing::all, seq(0, s - 2));
                u(s - 1, seq(0, s - 2)) = u(s - 1, seq(0, s - 2)).array() / m(seq(0, s - 2)).transpose().array();

                b(indexing::all, s - 1) = a(indexing::all, s - 1) -
                                          b(indexing::all, seq(0, s - 2)) * u(s - 1, seq(0, s - 2)).transpose();
                m(s - 1) = b(indexing::all, s - 1).transpose() * b(indexing::all, s - 1);

            }

            if (k > 2) {
                k -= 1;
            }


        }


    }

    std::cout << a.transpose() << std::endl;
    std::cout << mapping.transpose() << std::endl;

    /**
     *
     *
     */

    lll = a.transpose();
    lll_mapping = mapping.transpose();
    lll_inverse = lll_mapping.inverse();

    /**
     *
     */

    Vector<double, 3> fcoords1;
    Vector<double, 3> fcoords2;

    fcoords1 << 0.5, 0.5, 0.5;
    fcoords2 << 0.25, 0.15, 0.85;


    Vector<double, 3> lll_fcoords1;
    Vector<double, 3> lll_fcoords2;

    Vector<double, 3> pre_image;
    Vector<double, 3> cart1;
    Vector<double, 3> cart2;
    Vector<double, 3> dv;

    Matrix<double, 3, 27> images;
    Matrix<double, 3, 27> cart_images;

    images << -1, -1, -1,
            -1, -1, 0,
            -1, -1, 1,
            -1, 0, -1,
            -1, 0, 0,
            -1, 0, 1,
            -1, 1, -1,
            -1, 1, 0,
            -1, 1, 1,

            0, -1, -1,
            0, -1, 0,
            0, -1, 1,
            0, 0, -1,
            0, 0, 0,
            0, 0, 1,
            0, 1, -1,
            0, 1, 0,
            0, 1, 1,

            1, -1, -1,
            1, -1, 0,
            1, -1, 1,
            1, 0, -1,
            1, 0, 0,
            1, 0, 1,
            1, 1, -1,
            1, 1, 0,
            1, 1, 1;


    lll_fcoords1 = fcoords1.transpose() * lll_inverse;
    lll_fcoords2 = fcoords2.transpose() * lll_inverse;

    lll_fcoords1.unaryExpr([](const int x) { return x % 2; });
    lll_fcoords2.unaryExpr([](const int x) { return x % 2; });


    cart1 = lll_fcoords1.transpose() * lll;
    cart2 = lll_fcoords2.transpose() * lll;

    cart_images = lll.transpose() * images;

    pre_image = cart2 - cart1;

    double best = 1.0e20;
    int bestk = 0;

    for (auto i = 0; i < 27; i++) {
        dv = pre_image + cart_images(indexing::all, i);
        double d = dv.squaredNorm();

        if (d < best) {
            best = d;
            bestk = i;
        }

    }

    auto dist_vec = pre_image + cart_images(indexing::all, bestk);
    double dist = std::sqrt(best);

    std::cout << dist_vec << std::endl;
    std::cout << dist << std::endl;

    /**
     *
     */

    lattice
        << 1.0, 0.0, 0.0,
           0.0, 1.0, 0.0,
           0.0, 0.0, 1.0;


    Matrix<double, Dynamic, 3> coordinates(2, 3);
    Vector<int, Dynamic> species(2);


    coordinates <<
            0.5, 0.5, 0.5,
            1.5, 0.5, 0.5;

    species << 1, 0;

    SC::Structure structure(lattice, coordinates, species);


    Vector<double, 3> f1;
    Vector<double, 3> f2;

    f1 << 0.0, 0.0, 0.0;
    f2 << 0.9, 0.9, 0.9;


    structure.get_distances(f1, f2);



}