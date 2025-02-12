#include <thread>
#include <mutex>
#include <queue>


template <typename T>
class SafeQueue {
public:
	void push(T value) {
		std::lock_guard<std::mutex> lock(mutex);
		queue.push(value);
	}
	bool empty() {
		std::lock_guard<std::mutex> lock(mutex);
		return queue.empty();
	}
	bool try_pop(T& value) {
		std::lock_guard<std::mutex> lock(mutex);
		if (queue.empty()) {
			return false;
		}
		value = queue.front();
		queue.pop();
		return true;
	}
	void wait_and_pop(T& value) {
		std::unique_lock<std::mutex> lock(mutex);
		while (queue.empty()) {
			lock.unlock();
			std::this_thread::yield();
			lock.lock();
		}
		value = queue.front();
		queue.pop();
	}
private:
	std::queue<T> queue;
	std::mutex mutex;
};

/*
this_thread::yield() è una funzione che permette di cedere il controllo della CPU al sistema operativo,
this_thread::sleep_for() permette di mettere in pausa il thread per un certo periodo di tempo
this_thread::sleep_until() permette di mettere in pausa il thread fino ad un certo istante temporale
*/