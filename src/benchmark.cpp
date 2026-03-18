
// Minesweeper headers
#include "cell.hpp"
#include "game.hpp"
#include "map.hpp"
#include "cli.hpp"

// C++ standard library
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

using namespace std;

void benchmarkWorker(int threadId, int mapsToGenerate)
{
    unsigned long long baseSeed = static_cast<unsigned long long>(threadId) * mapsToGenerate;

    for (int i = 0; i < mapsToGenerate; ++i)
    {
        Map map(32, 32, baseSeed + i, 0);
        for (int j = 0; j < 32 * 32; ++j)
            ++map;
        // map.revealAll();
    }
}

int main()
{
    unsigned int cpuNumThreads = std::thread::hardware_concurrency();
    if (cpuNumThreads == 0)
        cpuNumThreads = 4;

    const int mapsPerThread = 1000;
    int totalMaps = cpuNumThreads * mapsPerThread;

    cout << "Running Single-Threaded Benchmark..." << endl;
    auto start_time_st = chrono::high_resolution_clock::now();

    benchmarkWorker(0, mapsPerThread);

    auto end_time_st = chrono::high_resolution_clock::now();
    chrono::duration<double, std::milli> duration_st = end_time_st - start_time_st;

    cout << "Running Multi-Threaded Benchmark (" << cpuNumThreads << " threads)..." << endl;
    auto start_time_mt = chrono::high_resolution_clock::now();

    vector<thread> threads;
    for (unsigned int i = 0; i < cpuNumThreads; ++i)
        threads.emplace_back(benchmarkWorker, i, mapsPerThread);

    for (auto &t : threads)
        t.join();

    auto end_time_mt = chrono::high_resolution_clock::now();
    chrono::duration<double, std::milli> duration_mt = end_time_mt - start_time_mt;

    double st_score = (mapsPerThread / duration_st.count()) * 1000.0;
    double mt_score = (totalMaps / duration_mt.count()) * 1000.0;
    double scaling_ratio = mt_score / st_score;

    cout << "\n--- Benchmark Results ---\n";

    cout << "Benchmark finished in:  " << duration_mt.count() << " ms" << endl;
    cout << "Average time per map:   " << duration_mt.count() / totalMaps << " ms" << endl;
    cout << "\n";

    cout << "Single-threaded Score:  " << (int)st_score << " (Maps/sec)" << endl;
    cout << "Multi-threaded Score:   " << (int)mt_score << " (Maps/sec)" << endl;
    cout << "MP Ratio:               " << scaling_ratio << "x" << endl;
    cout << "\n";

    return 0;
}
