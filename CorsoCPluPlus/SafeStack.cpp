#include <stack>
#include <mutex>
#include <thread>

/**
 * @class SafeStack
 * @brief Uno stack thread-safe che permette operazioni di push e pop in modo sicuro.
 *
 * @tparam T Il tipo degli elementi contenuti nello stack.
 */
template <typename T>
class SafeStack {
public:
    /**
     * @brief Inserisce un valore nello stack.
     *
     * @param value Il valore da inserire nello stack.
     */
    void push(T value) {
        std::lock_guard<std::mutex> lock(mutex);
        stack.push(value);
    }

    /**
     * @brief Verifica se lo stack è vuoto.
     *
     * @return true se lo stack è vuoto, false altrimenti.
     */
    bool empty() {
        std::lock_guard<std::mutex> lock(mutex);
        return stack.empty();
    }

    /**
     * @brief Prova a rimuovere un elemento dallo stack.
     *
     * @param value Riferimento al valore dove verrà memorizzato l'elemento rimosso.
     * @return true se un elemento è stato rimosso, false se lo stack era vuoto.
     */
    bool try_pop(T& value) {
        std::lock_guard<std::mutex> lock(mutex);
        if (stack.empty()) {
            return false;
        }
        value = stack.top();
        stack.pop();
        return true;
    }

    /**
     * @brief Attende fino a quando un elemento è disponibile e lo rimuove dallo stack.
     *
     * @param value Riferimento al valore dove verrà memorizzato l'elemento rimosso.
     */
    void wait_and_pop(T& value) {
        std::unique_lock<std::mutex> lock(mutex);
        while (stack.empty()) {
            lock.unlock();
            std::this_thread::yield();
            lock.lock();
        }
        value = stack.top();
        stack.pop();
    }

    /**
    * @brief Svuota lo stack.
    */
    void clear() {
        std::lock_guard<std::mutex> lock(mutex);
        while (!stack.empty()) {
            stack.pop();
        }
    }

private:
    std::stack<T> stack; ///< Lo stack contenente gli elementi.
    std::mutex mutex; ///< Mutex per garantire l'accesso thread-safe allo stack.
};
