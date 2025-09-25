#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>
#include <atomic>
#include <thread>
#include <array>

#include "CRC32.hpp"
#include "IO.hpp"

/// @brief Переписывает последние 4 байта значением value
void replaceLastFourBytes(std::vector<char> &data, uint32_t value) {
  std::copy_n(reinterpret_cast<const char *>(&value), 4, data.end() - 4);
}

/// @brief Копирует 4 байта числа value в массив data (размером 4 байта)
inline void replaceLastFourBytes(std::array<char,4>& data, uint32_t value) {
    std::copy_n(reinterpret_cast<const char*>(&value), 4, data.data());
}

/// @brief Пересчет последних 4х байт crc в диапазонах lo hi
void worker(const uint32_t target, // ориг. CRC
            const uint32_t prefixCrc, // CRC (ориг + injection)
                  uint64_t lo, // верх-низ диапазона значений
                  uint64_t hi,
                  std::atomic<bool>& found,
                  std::atomic<uint32_t>& found_val) {

  std::array<char, 4> tailBuf;

  for (uint64_t x = lo; x < hi && !found.load(); ++x) {
    // записываем 4 байта-кандидата
    uint32_t v = static_cast<uint32_t>(x);
    replaceLastFourBytes( tailBuf, v );
    // продолжаем CRC от префикса именно этими 4 байтами
    uint32_t crc = crc32(tailBuf.data(), 4, ~prefixCrc);

    if (crc == target) {
        found_val.store(v);
        found.store(true);
        return;
    }
  }
}

/**
 * @brief Формирует новый вектор с тем же CRC32, добавляя в конец оригинального
 * строку injection и дополнительные 4 байта
 * @details При формировании нового вектора последние 4 байта не несут полезной
 * нагрузки и подбираются таким образом, чтобы CRC32 нового и оригинального
 * вектора совпадали
 * @param original оригинальный вектор
 * @param injection произвольная строка, которая будет добавлена после данных
 * оригинального вектора
 * @return новый вектор
 */
std::vector<char> hack(const std::vector<char> &original,
                       const std::string &injection) {
  const uint32_t originalCrc32 = crc32(original.data(), original.size());
  const uint64_t maxVal = std::numeric_limits<uint32_t>::max();
  std::vector<std::thread> threads;
  // < 1 быть не должно, но hardware_concurrency может вернуть 0 в экзотических слчаях, 
  // на всякий случай проверяем
  unsigned int tCnt = std::max(1u, std::thread::hardware_concurrency());
  threads.reserve(tCnt);
  const uint64_t maxOne = maxVal + 1;
  uint64_t chunk = maxOne / tCnt;
  uint64_t start = 0;
  std::atomic<bool> found{false};
  std::atomic<uint32_t> found_val{0};
  // ~ crc32 update ожидает не инвертированное значение
  const uint32_t prefixCrc = crc32(injection.data(), injection.size(), ~originalCrc32);
  for (size_t t = 0; t < tCnt; ++t) {
    uint64_t end = (t + 1 == tCnt) ? maxOne : (start + chunk);
    threads.emplace_back(worker, originalCrc32, prefixCrc,
                          start, end, std::ref(found), std::ref(found_val));
    start = end;
  }

  for (auto& thread : threads) {
    if ( thread.joinable() )
      thread.join();
  }
  
  if (!found.load()) 
    throw std::logic_error("Can't hack");

  std::vector<char> result(original.size() + injection.size() + 4);
  auto it = std::copy(original.begin(), original.end(), result.begin());
  std::copy(injection.begin(), injection.end(), it);
  replaceLastFourBytes(result, found_val.load());
  if (crc32(result.data(), result.size()) != originalCrc32) {
    std::cout << "Failure, crc's do not match\n";
    result.clear();
  }
  else {
    std::cout << "Success\n";
  }

  return result;
}

int main(int argc, char **argv) {
  if (argc != 3) {
    std::cerr << "Call with two args: " << argv[0]
              << " <input file> <output file>\n";
    return 1;
  }

  try {
    const std::vector<char> data = readFromFile(argv[1]);
    const std::vector<char> badData = hack(data, "He-he-he");
    writeToFile(argv[2], badData);
  } catch (std::exception &ex) {
    std::cerr << ex.what() << '\n';
    return 2;
  }
  return 0;
}
