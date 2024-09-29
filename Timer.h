#pragma once

#define START_TIMER(x) auto start_time_##x = std::chrono::high_resolution_clock::now()
#define STOP_TIMER(x) do { auto stop_time_##x = std::chrono::high_resolution_clock::now(); std::cout << std::format("Time: {}",  std::chrono::duration_cast<std::chrono::microseconds>((stop_time_##x - start_time_##x))) << std::endl;  } while (0)