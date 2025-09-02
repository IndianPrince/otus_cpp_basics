#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>
#include <atomic>
#include <thread>

#include "CRC32.hpp"
#include "IO.hpp"

/// @brief Переписывает последние 4 байта значением value
void replaceLastFourBytes(std::vector<char> &data, uint32_t value) {
  std::copy_n(reinterpret_cast<const char *>(&value), 4, data.end() - 4);
}

/// @brief Пересчет последних 4х байт crc в диапазонах lo hi
void worker(const std::vector<char>& res,
            const uint32_t target, // ориг. CRC
            const uint32_t prefixCrc, // CRC (ориг + injection)
                  uint64_t lo, // верх-низ диапазона значений
                  uint64_t hi,
                  std::atomic<bool>& found,
                  std::atomic<uint32_t>& found_val) {
  std::vector<char> buf = res;
  char* tail = buf.data() + buf.size() - 4;
  
  for (uint64_t x = lo; x < hi && !found.load(); ++x) {
    // записываем 4 байта-кандидата
    uint32_t v = static_cast<uint32_t>(x);
    std::copy_n(reinterpret_cast<const char*>(&v), 4, tail);
    // продолжаем CRC от префикса именно этими 4 байтами
    uint32_t crc = crc32(tail, 4, prefixCrc);

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

  std::vector<char> result(original.size() + injection.size() + 4);
  auto it = std::copy(original.begin(), original.end(), result.begin());
  std::copy(injection.begin(), injection.end(), it);
  /*
   * Внимание: код ниже крайне не оптимален.
   * В качестве доп. задания устраните избыточные вычисления
   */
  //   const size_t maxVal = std::numeric_limits<uint32_t>::max();
  // for (size_t i = 0; i < maxVal; ++i) {
  //   // Заменяем последние четыре байта на значение i
  //   replaceLastFourBytes(result, uint32_t(i));
  //   // Вычисляем CRC32 текущего вектора result
  //   auto currentCrc32 = crc32(tail, 4, prefix_crc);

  //   if (currentCrc32 == originalCrc32) {
  //     std::cout << "Success\n";
  //     return result;
  //   }
  //   // Отображаем прогресс
  //   if (i % 1000 == 0) {
  //     std::cout << "progress: "
  //               << static_cast<double>(i) / static_cast<double>(maxVal)
  //               << std::endl;
  //   }
  // }
  // throw std::logic_error("Can't hack");

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
  const uint32_t prefixCrc = crc32(result.data(), result.size() - 4);
  for (size_t t = 0; t < tCnt; ++t) {
    uint64_t end = (t + 1 == tCnt) ? maxOne : (start + chunk);
    threads.emplace_back(worker, std::cref(result), originalCrc32, prefixCrc,
                          start, end, std::ref(found), std::ref(found_val));
    start = end;
  }

  for (auto& thread : threads) 
    thread.join();
  
  if (!found.load()) 
    throw std::logic_error("Can't hack");

  std::vector<char> res = result;
  replaceLastFourBytes(res, found_val.load());
  std::cout << "Success\n";

  return res;
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
