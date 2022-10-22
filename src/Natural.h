//
// Created by 0x0 on 10/13/22.
//

#ifndef BIGNUM__INTEGER_H
#define BIGNUM__INTEGER_H

#include <string>
#include <optional>

class Natural final {
private:
  using uint_t = uint8_t;
  constexpr static size_t bits_width = 8 * sizeof(uint_t);

  static std::allocator<uint_t> alloc;

public:
  Natural() : size{0}, capacity{0}, data{nullptr} {};
  explicit Natural(uint64_t u);
  static std::optional<Natural> parse(std::string_view sv);

  ~Natural();

  Natural(const Natural &natural);
  Natural(Natural &&natural) noexcept;
  Natural &operator=(Natural &&natural) noexcept;
  Natural &operator=(const Natural &natural);

  [[nodiscard]] std::string to_string(size_t base = 10) const;
  friend std::ostream &operator<<(std::ostream &os, const Natural &natural);

  void reserve(size_t cap) { reserve_data((cap + (bits_width - 1)) / bits_width); }

  [[nodiscard]] bool operator==(const Natural &rhs) const;
  [[nodiscard]] std::strong_ordering operator<=>(const Natural &rhs) const;

  Natural operator>>(size_t offset) const;
  Natural operator<<(size_t offset) const;
  Natural &operator>>=(size_t offset) { return *this = operator>>(offset); }
  Natural &operator<<=(size_t offset) { return *this = operator<<(offset); }

  Natural operator+(const Natural &rhs) const;
  Natural operator-(const Natural &rhs) const;
  Natural &operator+=(const Natural &rhs);
  Natural &operator-=(const Natural &rhs);

private:
  void reserve_data(size_t cap);
  constexpr uint_t &operator[](size_t index) { return data[index]; };
  constexpr uint_t operator[](size_t index) const { return data[index]; };

  uint_t *data;
  size_t size;
  size_t capacity;
};

#endif //BIGNUM__INTEGER_H
