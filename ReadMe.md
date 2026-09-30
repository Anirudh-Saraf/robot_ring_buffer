# Real-Time Robotic State Circular Ring Buffer

![Language](https://img.shields.io/badge/Language-C%2B%2B17%20%2F%20C%2B%2B20-blue)
![Platform](https://img.shields.io/badge/Platform-Ubuntu%20Linux-orange)
![Complexity](https://img.shields.io/badge/Time%20Complexity-O(1)%20Constant-brightgreen)

A high-performance, template-based Circular Ring Buffer designed for high-frequency robotic telemetry (IMU, Wheel Encoders, LiDAR) state estimation. This implementation guarantees deterministic execution times and bounded memory layouts required for real-time robotic control loops.

---

## 🛰️ The Problem & The Solution

High-frequency sensors in robotics (like IMUs running at 500Hz) generate massive amounts of continuous data. 
* **The Problem:** Dynamically resizing collections (like `std::vector::push_back`) causes frequent heap allocations, leading to non-deterministic execution times (latency spikes) and potential memory fragmentation.
* **The Solution:** A fixed-capacity **Circular Ring Buffer**. It pre-allocates memory once during system initialization and automatically overwrites the oldest data when capacity is reached. This design delivers **O(1) constant-time writes/reads** and **zero runtime dynamic allocations**.

---

## 🗺️ How It Works (Visual Representation)

The buffer utilizes two tracking indices—a `head` (write pointer) and a `tail` (read pointer)—which wrap around the underlying array using modulo arithmetic:

```text
[Initial State - Capacity: 3]
  [ Empty ]  [ Empty ]  [ Empty ]
   ^ Head/Tail

[Stream Reading 1, 2, 3 - Buffer Full]
  [ Yaw: 10.0 ]  [ Yaw: 20.0 ]  [ Yaw: 30.0 ]
   ^ Tail                                      ^ Head (Wraps around to 0)

[Stream Reading 4 - Reading 1 Overwritten]
  [ Yaw: 40.0 ]  [ Yaw: 20.0 ]  [ Yaw: 30.0 ]
   ^ Head         ^ Tail (Advanced to Index 1)
