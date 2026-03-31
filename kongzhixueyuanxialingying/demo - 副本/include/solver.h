#ifndef SOLVER_H
#define SOLVER_H

#include <stdexcept>
#include <vector>
#include <functional>
#include <map>
#include <memory>
#include <cmath>
#include <iostream>

using Vec = std::vector<double>;
using ODEFunc = std::function<Vec(const Vec&, double, const Vec&, double)>;
Vec multiplyVector(const Vec& a, double scalar);
Vec addVectors(const Vec& a, const Vec& b);
Vec deductVectors(const Vec& a, const Vec& b);
template <typename... Vecs>
Vec addVectors(const Vec& a, const Vec& b, const Vecs&... rest) {
    return addVectors(addVectors(a, b), rest...);
}
class Solver {
public:
    virtual Vec solve(const ODEFunc& f, const Vec& y0, const Vec& para, double t, int partitions) = 0;
    virtual std::string getType() const = 0;
};

class RungeKutta4Solver : public Solver {
public:
    Vec solve(const ODEFunc& f, const Vec& y0, const Vec& para, double x_length, int partitions ) override {
        size_t n = y0.size();
        size_t steps = static_cast<size_t>(partitions);
        Vec y = y0;
        double t = 0;
        double h = x_length/partitions;
        for (size_t i = 0; i < steps; ++i) {
            Vec k1 = f(y, t, para, h);
            Vec k2(n), k3(n), k4(n);
            for (size_t j = 0; j < n; ++j) {
                k2[j] = f(addVectors(y, multiplyVector(k1, h / 2)), t + h / 2, para, h)[j];
                k3[j] = f(addVectors(y, multiplyVector(k2, h / 2)), t + h / 2, para, h)[j];
                k4[j] = f(addVectors(y, multiplyVector(k3, h)), t + h, para, h)[j];
            }
            for (size_t j = 0; j < n; ++j) {
                y[j] += h / 6 * (k1[j] + 2 * k2[j] + 2 * k3[j] + k4[j]);
            }
            t += h;
        }
        return y;
    }

    std::string getType() const override {return "RK4";}
};

class RungeKutta5Solver : public Solver {
public:
    Vec solve(const ODEFunc& f, const Vec& y0, const Vec& para, double x_length, int partitions) override {
        size_t n = y0.size();
        size_t steps = static_cast<size_t>(partitions);
        Vec y = y0;
        double t = 0;
        double h = x_length / partitions;

        for (size_t i = 0; i < steps; ++i) {
            Vec k1 = f(y, t, para, h);
            Vec k2(n), k3(n), k4(n), k5(n), k6(n);

            for (size_t j = 0; j < n; ++j) {
                k2[j] = f(addVectors(y, multiplyVector(k1, h / 4.0)), t + h / 4.0, para, h)[j];
                k3[j] = f(addVectors(y, multiplyVector(k1, 3.0 * h / 8.0), multiplyVector(k2, 9.0 * h / 32.0)), t + 3.0 * h / 8.0, para, h)[j];
                k4[j] = f(addVectors(y, multiplyVector(k1, 1932.0 * h / 2197.0), multiplyVector(k2, -7200.0 * h / 2197.0), multiplyVector(k3, 7296.0 * h / 2197.0)), t + 12.0 * h / 13.0, para, h)[j];
                k5[j] = f(addVectors(y, multiplyVector(k1, 439.0 * h / 216.0), multiplyVector(k2, -8.0 * h), multiplyVector(k3, 3680.0 * h / 513.0), multiplyVector(k4, -845.0 * h / 4104.0)), t + h, para, h)[j];
                k6[j] = f(addVectors(y, multiplyVector(k1, -8.0 * h / 27.0), multiplyVector(k2, 2.0 * h), multiplyVector(k3, -3544.0 * h / 2565.0), multiplyVector(k4, 1859.0 * h / 4104.0), multiplyVector(k5, -11.0 * h / 40.0)), t + h / 2.0, para, h)[j];
            }

            for (size_t j = 0; j < n; ++j) {
                y[j] += h * (25.0 / 216.0 * k1[j] + 1408.0 / 2565.0 * k3[j] + 2197.0 / 4104.0 * k4[j] - 1.0 / 5.0 * k5[j]);
            }
            t += h;
        }

        return y;
    }
    std::string getType() const override {return "RK5";}
};

class AdaptiveRungeKuttaSolver : public Solver {
public:
    Vec solve(const ODEFunc& f, const Vec& y0, const Vec& para, double x_length, int partitions) override {
        size_t n = y0.size();
        Vec y = y0;
        double t = 0;
        double step_size = x_length / partitions;
        double min_step_size = std::max(step_size / 1000, std::numeric_limits<double>::epsilon());
        double max_step_size = step_size;
        double current_step_size = step_size;
        double tol = 1e-6;

        while (t < x_length) {
            if (t + current_step_size > x_length) {
                current_step_size = x_length - t;
            }

            Vec k1 = f(y, t, para, step_size);
            Vec k2 = f(addVectors(y, multiplyVector(k1, current_step_size / 2)), t + current_step_size / 2, para, step_size);
            Vec k3 = f(addVectors(y, multiplyVector(k2, current_step_size / 2)), t + current_step_size / 2, para, step_size);
            Vec k4 = f(addVectors(y, multiplyVector(k3, current_step_size)), t + current_step_size, para, step_size);
            
            Vec y4(n);
            for (size_t i = 0; i < n; ++i) {
                y4[i] = y[i] + current_step_size / 6 * (k1[i] + 2 * k2[i] + 2 * k3[i] + k4[i]);
            }

            Vec k1_high = f(y, t, para, step_size);
            Vec k2_high = f(addVectors(y, multiplyVector(k1_high, current_step_size / 4.0)), t + current_step_size / 4.0, para, step_size);
            Vec k3_high = f(addVectors(y, multiplyVector(k1_high, 3.0 * current_step_size / 8.0), multiplyVector(k2_high, 9.0 * current_step_size / 32.0)), t + 3.0 * current_step_size / 8.0, para, step_size);
            Vec k4_high = f(addVectors(y, multiplyVector(k1_high, 1932.0 * current_step_size / 2197.0), multiplyVector(k2_high, -7200.0 * current_step_size / 2197.0), multiplyVector(k3_high, 7296.0 * current_step_size / 2197.0)), t + 12.0 * current_step_size / 13.0, para, step_size);
            Vec k5_high = f(addVectors(y, multiplyVector(k1_high, 439.0 * current_step_size / 216.0), multiplyVector(k2_high, -8.0 * current_step_size), multiplyVector(k3_high, 3680.0 * current_step_size / 513.0), multiplyVector(k4_high, -845.0 * current_step_size / 4104.0)), t + current_step_size, para, step_size);
            Vec k6_high = f(addVectors(y, multiplyVector(k1_high, -8.0 * current_step_size / 27.0), multiplyVector(k2_high, 2.0 * current_step_size), multiplyVector(k3_high, -3544.0 * current_step_size / 2565.0), multiplyVector(k4_high, 1859.0 * current_step_size / 4104.0), multiplyVector(k5_high, -11.0 * current_step_size / 40.0)), t + current_step_size / 2.0, para, step_size);

            Vec y5_high(n);
            for (size_t i = 0; i < n; ++i) {
                y5_high[i] = y[i] + current_step_size * (16.0 / 135.0 * k1_high[i] + 6656.0 / 12825.0 * k3_high[i] + 28561.0 / 56430.0 * k4_high[i] - 9.0 / 50.0 * k5_high[i] + 2.0 / 55.0 * k6_high[i]);
            }

            Vec y4_high(n), err(n);
            for (size_t i = 0; i < n; ++i) {
                y4_high[i] = y[i] + current_step_size * (25.0 / 216.0 * k1_high[i] + 1408.0 / 2565.0 * k3_high[i] + 2197.0 / 4104.0 * k4_high[i] - 1.0 / 5.0 * k5_high[i]);
                err[i] = std::abs(y5_high[i] - y4_high[i]);
            }

            double err_norm = *std::max_element(err.begin(), err.end());
            if (err_norm <= tol) {
                y = y5_high;
                t += current_step_size;
            }

            double safety = 0.9;
            double h_factor = safety * std::pow(tol / (err_norm + 1e-10), 0.2);
            h_factor = std::min(5.0, std::max(0.2, h_factor));
            current_step_size = std::max(min_step_size, std::min(max_step_size, current_step_size * h_factor));
        }

        return y;
    }
    std::string getType() const override {return "ERK";}
};




class SolverFactory {
public:
    static SolverFactory& getInstance() {
        static SolverFactory instance;
        return instance;
    }
    void registerSolver(const std::string& type, std::shared_ptr<Solver> solver) {
        solvers[type] = solver;
    }
    std::shared_ptr<Solver> getSolver(const std::string& type) {
        if (solvers.find(type) != solvers.end()) {
            return solvers[type];
        } else {
            throw std::runtime_error("Solver not found");
        }
    }
    
private:
    SolverFactory() {}
    std::map<std::string, std::shared_ptr<Solver>> solvers;
};

inline Vec addVectors(const Vec& a, const Vec& b) {
    size_t n = a.size();
    Vec result(n);
    for (size_t i = 0; i < n; ++i) {
        result[i] = a[i] + b[i];
    }
    return result;
}

inline Vec deductVectors(const Vec& a, const Vec& b) {
    size_t n = a.size();
    Vec result(n);
    for (size_t i = 0; i < n; ++i) {
        result[i] = a[i] - b[i];
    }
    return result;
}

inline Vec multiplyVector(const Vec& a, double scalar) {
    size_t n = a.size();
    Vec result(n);
    for (size_t i = 0; i < n; ++i) {
        result[i] = a[i] * scalar;
    }
    return result;
}

inline Vec addVectorsLim(const Vec& a, const Vec& b) {
    size_t n = a.size();
    Vec result(n);
    for (size_t i = 0; i < n; ++i) {
        result[i] = a[i] + b[i];
        if(result[i]<0.0){
            result[i]=0.0;
        }
        else if(result[i]>1.0){
            result[1]=1.0;
        }
    }
    return result;
}

inline Vec addVectorsLim2(const Vec& a, const Vec& b) {
    size_t n = a.size();
    Vec result(n);
    for (size_t i = 0; i < n; ++i) {
        result[i] = a[i] + b[i];
        if(result[i]<-5){
            result[i]=-5;
        }
        else if(result[i]>5){
            result[1]=5;
        }
    }
    return result;
}



// inline int findClosestLowerMultiple(double t, double h) {
//     if (h == 0) {
//         throw std::invalid_argument("h cannot be zero");
//     }
//     int n = static_cast<int>(std::floor(t / h));
//     return n;
// }


#endif //SOLVER_H