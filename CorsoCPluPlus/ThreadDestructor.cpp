#include <thread>
using namespace std::chrono_literals;

int main()
{
    auto bleah = std::thread{ [] { std::this_thread::sleep_for(13ms); } };

}   // ~thread calls std::terminate()