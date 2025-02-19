#include <thread>
#include <mutex>
#include <queue>

/**
 * @class SafeQueue
 * @brief Una coda thread-safe che permette operazioni di push e pop in modo sicuro.
 *
 * @tparam T Il tipo degli elementi contenuti nella coda.
 */
template <typename T>
class SafeQueue {
public:
    /**
     * @brief Inserisce un valore nella coda.
     *
     * @param value Il valore da inserire nella coda.
     */
    void push(T value) {
        std::lock_guard<std::mutex> lock(mutex);
        queue.push(value);
    }

    /**
     * @brief Verifica se la coda è vuota.
     *
     * @return true se la coda è vuota, false altrimenti.
     */
    bool empty() {
        std::lock_guard<std::mutex> lock(mutex);
        return queue.empty();
    }

    /**
     * @brief Prova a rimuovere un elemento dalla coda.
     *
     * @param value Riferimento al valore dove verrà memorizzato l'elemento rimosso.
     * @return true se un elemento è stato rimosso, false se la coda era vuota.
     */
    bool try_pop(T& value) {
        std::lock_guard<std::mutex> lock(mutex);
        if (queue.empty()) {
            return false;
        }
        value = queue.front();
        queue.pop();
        return true;
    }

    /**
     * @brief Attende fino a quando un elemento è disponibile e lo rimuove dalla coda.
     *
     * @param value Riferimento al valore dove verrà memorizzato l'elemento rimosso.
     */
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
    std::queue<T> queue; ///< La coda contenente gli elementi.
    std::mutex mutex; ///< Mutex per garantire l'accesso thread-safe alla coda.
};

/*
this_thread::yield() è una funzione che permette di cedere il controllo della CPU al sistema operativo,
this_thread::sleep_for() permette di mettere in pausa il thread per un certo periodo di tempo
this_thread::sleep_until() permette di mettere in pausa il thread fino ad un certo istante temporale
*/
