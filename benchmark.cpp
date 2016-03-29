//
// Created by artyom on 23.03.16.
//

#include <chrono>
#include "utils.h"
#include "count.h"

#include <iostream>
#include <fstream>
#include <sstream>

typedef chrono::duration<double, milli> duration;

const int runTimes = 100;

struct stats {
    double durations[runTimes];

    double mean;
    double stDev;
    double stErr;
    double margin;

    void calc() {
        const double z = 1.96; // 95%
        const int n = runTimes;

        double sum = 0;
        for (int i = 0; i < n; i++) {
            sum += durations[i];
        }
        mean = sum/n;
        double variance = 0;
        for (int i = 0; i < n; i++) {
            variance += pow(durations[i] - mean, 2);
        }
        variance = variance/(n-1);
        stDev = sqrt(variance);
        stErr = stDev/sqrt(n);
        margin = z*stErr;
    }

    void print(std::string method, int threadNum) {
        std::string upperCase = method;
        for (auto & c: upperCase) c = (char) toupper(c);
        cout << " -- " << upperCase << " " << std::to_string(threadNum) << " threads" << " --" << endl;
        cout << "Mean " << mean << endl;
        cout << "StDev " << stDev << endl;
        cout << "Margin " << margin << endl;
    }

    std::string plotFormat(int threadNum) {
        std::stringstream ss;
        ss << threadNum << " " << mean << " " << margin << " " << stDev << endl;
        return ss.str();
    }
};

stats doBench(std::function<void ()> functor) {
    stats res;
    cout << "Run number ";
    for (int i = 0; i < runTimes; i++) {
        cout << i << " " << flush;
        auto start = std::chrono::steady_clock::now();
        functor();
        auto end = std::chrono::steady_clock::now();
        auto diff = end - start;
        res.durations[i] = duration(diff).count();
    }
    cout << endl;
    res.calc();

    return res;
}

int main() {
    pair<const char*, size_t> text = loadTextFromFile("book.txt");

    ofstream seqPlotData("stats/seq.txt");
    ofstream mpPlotData("stats/openmp.txt");
    ofstream pthPlotData("stats/pthreads.txt");

    stats sequential = doBench([&text]() {
        countWords(text.first, text.second);
    });
    sequential.print("sequential", 1);

    std:array<int, 8> threadCases;
    std::iota(threadCases.begin(), threadCases.end(), 1);
    for (int threadNum: threadCases) {
        stats openMP = doBench([&text, threadNum]() {
            countWordsOpenMP(text.first, text.second, threadNum);
        });
        openMP.print("openMP", threadNum);
        mpPlotData << openMP.plotFormat(threadNum);

        stats pthread = doBench([&text, threadNum]() {
            countWordsPthreads(text.first, text.second, threadNum);
        });
        pthread.print("pthread", threadNum);
        pthPlotData << pthread.plotFormat(threadNum);

        seqPlotData << threadNum << " " << sequential.mean/threadNum
        << " " << sequential.margin/threadNum << endl;
    }

    auto counted = countWordsOpenMP(text.first, text.second, 8);
    stats flip = doBench([&counted]() {
        flipMap<string, int>(counted);
    });
    flip.print("flipMap", 1);
}
