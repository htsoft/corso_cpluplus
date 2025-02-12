#include <chrono>
#include <iostream>
#include <semaphore>
#include <thread>

std::binary_semaphore smphSignalMainToThread{ 0 }, smphSignalThreadToMain{ 0 };

void ThreadProc()
{
    smphSignalMainToThread.acquire();

    std::cout << "[thread] Got the signal\n";

    using namespace std::literals;
    std::this_thread::sleep_for(3s);

    std::cout << "[thread] Send the signal\n";

    smphSignalThreadToMain.release();
}

int main()
{
    std::thread thrWorker(ThreadProc);

    std::cout << "[main] Send the signal\n";

    smphSignalMainToThread.release();

    smphSignalThreadToMain.acquire();

    std::cout << "[main] Got the signal\n";
    thrWorker.join();
}

/*
[main] Send the signal
[thread] Got the signal
[thread] Send the signal
[main] Got the signal
*/