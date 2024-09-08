#include "Timer.h"

Timer::Timer()
    :
    last(std::chrono::steady_clock::now())
{}

float Timer::Mark() noexcept
{
    const auto old = last;
    last = std::chrono::steady_clock::now();

    return std::chrono::duration<float>(last - old).count(); // elapsed time

}

float Timer::Peek() const noexcept
{
    return std::chrono::duration<float>(
        std::chrono::steady_clock::now() - last).count();
}