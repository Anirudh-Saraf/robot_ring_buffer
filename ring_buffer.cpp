#include <iostream>
#include <vector>
#include <optional>
#include <string>

// A generic, fixed-size Circular Ring Buffer for high-frequency robot telemetry
template <typename T>
class CircularBuffer {
private:
    std::vector<T> m_buffer;
    size_t m_head{0}; // Points to the next write position
    size_t m_tail{0}; // Points to the oldest read position
    size_t m_size{0}; // Current number of elements stored
    size_t m_capacity;

public:
    explicit CircularBuffer(size_t capacity) 
        : m_capacity{capacity} {
        m_buffer.resize(capacity); // Allocate the memory once at startup
        std::cout << "⚙️  [System] Circular Buffer initialized with capacity: " << m_capacity << "\n";
    }

    // Push new data into the buffer. Overwrites the oldest data if full.
    void push(const T& item) {
        m_buffer[m_head] = item;
        
        if (is_full()) {
            // Overwriting oldest data: Advance tail to discard the oldest element
            m_tail = (m_tail + 1) % m_capacity;
        } else {
            ++m_size;
        }

        m_head = (m_head + 1) % m_capacity;
    }

    // Pop the oldest data out of the buffer (returns std::optional in case buffer is empty)
    std::optional<T> pop() {
        if (is_empty()) {
            return std::nullopt;
        }

        T item = m_buffer[m_tail];
        m_tail = (m_tail + 1) % m_capacity;
        --m_size;
        return item;
    }

    bool is_empty() const { return m_size == 0; }
    bool is_full() const { return m_size == m_capacity; }
    size_t size() const { return m_size; }
};

// Represents an IMU Sensor Packet (Yaw, Pitch, Roll)
struct IMUPacket {
    double yaw;
    double pitch;
    double roll;
};

int main() {
    std::cout << "===  Real-Time Robot State Estimator Started  ===\n\n";

    // Initialize a circular buffer with a maximum capacity of 3 readings
    CircularBuffer<IMUPacket> stateHistory(3);

    // 1. Simulating rapid IMU readings streaming in
    std::cout << "\n--- Streaming IMU Readings ---\n";
    std::cout << "Pushing Reading 1 [Yaw: 10.0]\n"; stateHistory.push({10.0, 0.0, 0.0});
    std::cout << "Pushing Reading 2 [Yaw: 20.0]\n"; stateHistory.push({20.0, 0.0, 0.0});
    std::cout << "Pushing Reading 3 [Yaw: 30.0]\n"; stateHistory.push({30.0, 0.0, 0.0});

    // The buffer is now full. The next push will overwrite Reading 1 (Yaw: 10.0).
    std::cout << "Pushing Reading 4 [Yaw: 40.0] (Expected to overwrite Reading 1)...\n";
    stateHistory.push({40.0, 0.0, 0.0});

    // 2. Reading state estimation history
    std::cout << "\n--- Reading Data from Buffer (Oldest -> Newest) ---\n";
    while (!stateHistory.is_empty()) {
        auto packet = stateHistory.pop();
        if (packet) {
            std::cout << " Pop Yaw State: " << packet->yaw << " degrees\n";
        }
    }

    std::cout << "\n===  State Estimator Shutting Down  ===\n";
    return 0;
}

