#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

using namespace std::chrono_literals;

void print(auto txt)
{
    std::cout << std::this_thread::get_id() << ' ' << txt;
}

int main()
{
    std::jthread sleepy_worker(
        [](std::stop_token stoken)
        {
            for (int i = 10; i; --i)
            {
                std::this_thread::sleep_for(300ms);
                if (stoken.stop_requested())
                {
                    print("Sleepy worker is requested to stop\n");
                    return;
                }
                print("Sleepy worker goes back to sleep\n");
            }
        });

    std::jthread waiting_worker(
        [](std::stop_token stoken)
        {
            std::mutex mutex;
            std::unique_lock lock(mutex);
            std::condition_variable_any().wait(lock, stoken, [] { return false; });
            print("Waiting worker is requested to stop\n");
            return;
        });

    std::this_thread::sleep_for(400ms);

    print("Requesting stop of sleepy worker\n");
    sleepy_worker.request_stop();
    sleepy_worker.join();
    print("Sleepy worker joined\n");

}