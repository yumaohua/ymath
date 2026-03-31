#ifndef POS_H
#define POS_H
#include "solver.h"
#include "story.h"
#include <random>
#include <iostream>
double POS(
    int num, //粒子数
    int partitions, // 将t_f分成的份数
    double omega_ini, // 迭代开始的惯性因子值
    double omega_end, // 迭代快结束时的惯性因子值
    double c1, // 记忆学习因子
    double c2, // 社会学习因子
    double theta, // 约束因子
    double perturb, // 小于1的非负数，表示当变化过小时随即重置粒子的比例
    size_t max_steps, // 最大迭代步数
    double tol = 1e-6,  // 最小变化量
    int patience = 10  // patience次变化低于tol则提前停止
) {
    std::vector<Vec> X(num, Vec(partitions));
    std::vector<Vec> pBest(num, Vec(partitions));
    Vec pBestVal(num);
    Vec gBest(partitions);
    double gBestval = -std::numeric_limits<double>::infinity();
    int gBestindex = 0;
    std::vector<Vec> V(num, Vec(partitions));
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    for (int i = 0; i < num; i++) {
        for (int j = 0; j < partitions; j++) {
            X[i][j] = dis(gen);
            V[i][j] = dis(gen);
            pBest[i][j] = X[i][j];
        }
    }
    for (int i = 0; i < num; i++) {
        double val = Val(X[i]);
        pBestVal[i] = val;
        if (val > gBestval) {
            gBestval = val;
            gBestindex = i;
        }
    }
    gBest = pBest[gBestindex];
    int stagnation_counter = 0;
    for (int step = 0; step < max_steps; step++) {
        double omega = omega_ini - (omega_ini - omega_end) * pow(static_cast<double>(step) / max_steps, 2);
        for (int i = 0; i < num; i++) {
            V[i] = addVectorsLim2(
                addVectors(multiplyVector(V[i], omega),
                           multiplyVector(deductVectors(pBest[i], X[i]), c1 * dis(gen))),
                multiplyVector(deductVectors(gBest, X[i]), c2 * dis(gen))
            );
            X[i] = addVectorsLim(X[i], multiplyVector(V[i], theta));
            double xVal = Val(X[i]);
            if (xVal > pBestVal[i]) {
                pBestVal[i] = xVal;
                pBest[i] = X[i];
            }
        }
        double old_gBestval = gBestval;
        for (int i = 0; i < num; i++) {
            double val = pBestVal[i];
            if (val > gBestval) {
                gBestval = val;
                gBestindex = i;
            }
        }
        if (gBestval > old_gBestval) {
            gBest = pBest[gBestindex];
            stagnation_counter = 0;
        } else {
            stagnation_counter++;
        }
        double diff = std::abs(gBestval - old_gBestval);
        if (diff < tol) {
            stagnation_counter++;
        } else {
            stagnation_counter = 0;  // 变化超过阈值重置计数器
        }

        if (stagnation_counter >= patience) {
            std::cout << "esrly stop at " << step << std::endl;
            break;
        }

        if (diff < 0.1) {
            for (int i = 0; i < num; i++) {
                if (dis(gen) < perturb) {
                    for (int j = 0; j < partitions; j++) {
                        X[i][j] = dis(gen);
                        V[i][j] = dis(gen);
                    }
                    pBestVal[i] = Val(X[i]);
                    pBest[i] = X[i];
                }
            }
        }

        std::cout << "step: " << step << ", gBestval: " << gBestval << std::endl;
    }

    return gBestval;
}


#endif