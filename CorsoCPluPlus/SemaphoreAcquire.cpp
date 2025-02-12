#include <array>
#include <chrono>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <new>
#include <random>
#include <semaphore>
#include <thread>
#include <vector>

using namespace std::literals;

constexpr std::size_t max_threads{ 10U }; 
constexpr std::ptrdiff_t max_sema_threads{ 3 }; 
std::counting_semaphore semaphore{ max_sema_threads };
constexpr auto time_tick{ 10ms };

unsigned rnd()
{
    static std::uniform_int_distribution<unsigned> distribution{ 2U, 9U }; 
    static std::random_device engine;
    static std::mt19937 noise{ engine() };
    return distribution(noise);
}

class alignas(std::hardware_destructive_interference_size) Guide
{
    inline static std::mutex cout_mutex;
    inline static std::chrono::time_point<std::chrono::high_resolution_clock> started_at;
    unsigned delay{ rnd() }, occupy{ rnd() }, wait_on_sema{};

public:
    static void start_time() { started_at = std::chrono::high_resolution_clock::now(); }

    void initial_delay() { std::this_thread::sleep_for(delay * time_tick); }

    void occupy_sema()
    {
        wait_on_sema =
            static_cast<unsigned>(std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::high_resolution_clock::now() - started_at -
                delay * time_tick).count() / time_tick.count());
        std::this_thread::sleep_for(occupy * time_tick);
    }

    void visualize(unsigned id, unsigned x_scale = 2) const
    {
        auto cout_n = [=](auto str, unsigned n)
            {
                for (n *= x_scale; n-- > 0; std::cout << str)
                    ;
            };
        std::lock_guard lk{ cout_mutex };
        std::cout << '#' << std::setw(2) << id << ' ';
        cout_n("░", delay);
        cout_n("▒", wait_on_sema);
        cout_n("█", occupy);
        std::cout << '\n';
    }

    static void show_info()
    {
        std::cout << "\nThreads: " << max_threads << ", Throughput: " << max_sema_threads
            << " │ Legend: initial delay ░░ │ wait state ▒▒ │ sema occupation ██ \n"
            << std::endl;
    }
};

std::array<Guide, max_threads> guides;

void workerThread(unsigned id)
{
    guides[id].initial_delay();
    semaphore.acquire();
    guides[id].occupy_sema();
    semaphore.release();
    guides[id].visualize(id);
}

int main()
{
    std::vector<std::jthread> threads;
    threads.reserve(max_threads);

    Guide::show_info();
    Guide::start_time();

    for (auto id{ 0U }; id != max_threads; ++id)
        threads.push_back(std::jthread(workerThread, id));
}

/*
Default case: max_threads{10U}, max_sema_threads{3}

Threads: 10, Throughput: 3 │ Legend: initial delay ░░ │ wait state ▒▒ │ sema occupation ██

# 1 ░░░░██████
# 2 ░░░░████████
# 5 ░░░░░░██████████
# 8 ░░░░░░░░░░░░████████████
# 9 ░░░░░░░░░░░░██████████████
# 7 ░░░░░░░░░░░░▒▒▒▒████████████████
# 4 ░░░░░░░░░░░░░░▒▒▒▒▒▒▒▒▒▒▒▒████████
# 6 ░░░░░░░░░░░░░░▒▒▒▒▒▒▒▒▒▒██████████████████
# 3 ░░░░░░░░░░░░░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒████████████
# 0 ░░░░░░░░░░░░░░░░░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒██████████████

──────────────────────────────────────────────────────────────────────────────────────────────────────────────
"Enough for everyone" case (no wait states!): max_threads{10U}, max_sema_threads{10}

Threads: 10, Throughput: 10 │ Legend: initial delay ░░ │ wait state ▒▒ │ sema occupation ██

# 4 ░░░░██████
# 5 ░░░░░░████
# 3 ░░░░██████████
# 1 ░░░░██████████
# 8 ░░░░░░░░████████████
# 6 ░░░░░░░░░░░░░░░░██████
# 7 ░░░░░░░░░░░░░░░░██████
# 9 ░░░░░░░░░░░░░░░░██████████
# 0 ░░░░░░░░░░░░██████████████████
# 2 ░░░░░░░░░░░░░░░░░░████████████

──────────────────────────────────────────────────────────────────────────────────────────────────────────────
Binary semaphore case: max_threads{10U}, max_sema_threads{1}

Threads: 10, Throughput: 1 │ Legend: initial delay ░░ │ wait state ▒▒ │ sema occupation ██

# 6 ░░░░████
# 5 ░░░░▒▒▒▒████
# 4 ░░░░░░░░░░▒▒██████████
# 7 ░░░░░░░░░░▒▒▒▒▒▒▒▒▒▒▒▒████████████████
# 2 ░░░░░░░░░░░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒██████
# 3 ░░░░░░░░░░░░░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒████████████████
# 0 ░░░░░░░░░░░░░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒████████████
# 1 ░░░░░░░░░░░░░░░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒████████
# 8 ░░░░░░░░░░░░░░░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒██████
# 9 ░░░░░░░░░░░░░░░░░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒██████████████

*/