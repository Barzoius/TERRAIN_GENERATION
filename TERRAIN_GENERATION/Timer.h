#include <chrono>

class Timer
{
public:
    Timer();
    float Mark() noexcept;
    float Peek() const noexcept;

private:
    std::chrono::steady_clock::time_point last;
};