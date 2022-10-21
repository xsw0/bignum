//
// Created by 0x0 on 10/17/22.
//

#include <vector>
#include <string>
#include <tuple>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_all.hpp>
#include "Natural.h"

TEST_CASE("Natural test") {
  std::vector<uint64_t> str_lhs{
    0ull, 0ull, 0ull,
    7194242981861588590ull, 15473620907548413927ull, 769343502203807540ull,
    17942260813800794056ull, 1800854534734379225ull, 17404438117666642047ull,
    9999553451150725678ull, 12163052843017038985ull, 17252615071130496830ull,
    6117139289241355238ull, 4144312164982867104ull, 3122722844659594025ull,
    1865633449789592660ull, 6072086501996192744ull, 17320716270483853138ull,
    7096638023984893621ull, 5566099654367076208ull, 7167647276077223405ull,
    235482184585947933ull, 572296530951552921ull, 12377953233029808462ull,
  };
  std::vector<uint64_t> str_rhs{
    0ull, 1ull, 1234567890ull,
    11331865548109331386ull, 7403273338117392513ull, 9353537842408888446ull,
    4694442591958682842ull, 3236011601003472751ull, 17563617168393676366ull,
    10491450073996074587ull, 11844443140852638581ull, 7078622443973910750ull,
    17964364026156749601ull, 6309278962924281944ull, 5497197030483817269ull,
    3802311891047930953ull, 15284109878391851033ull, 9867348684199777325ull,
    11621268646768908742ull, 1966209645253245949ull, 10148432623922814450ull,
    7578532006146994473ull, 6438008555534319824ull, 8972139050904822280ull,
  };
  std::vector<std::string> str_sum{
    "0", "1", "1234567890",
    "18526108529970919976", "22876894245665806440", "10122881344612695986",
    "22636703405759476898", "5036866135737851976", "34968055286060318413",
    "20491003525146800265", "24007495983869677566", "24331237515104407580",
    "24081503315398104839", "10453591127907149048", "8619919875143411294",
    "5667945340837523613", "21356196380388043777", "27188064954683630463",
    "18717906670753802363", "7532309299620322157", "17316079900000037855",
    "7814014190732942406", "7010305086485872745", "21350092283934630742",
  };
  std::vector<Natural> lhs, rhs;
  for (auto &&s : str_lhs) {
    lhs.emplace_back(Natural(s));
  }
  for (auto &&s : str_rhs) {
    rhs.emplace_back(Natural(s));
  }

  using namespace Catch::Generators;

  auto i = GENERATE_REF(range<size_t>(0, str_sum.size()));

  SECTION("test input and output") {
    REQUIRE(lhs[i].to_string() == std::to_string(str_lhs[i]));
    REQUIRE(rhs[i].to_string() == std::to_string(str_rhs[i]));
  }

  SECTION("parse") {
    auto natural = GENERATE_REF(lhs[i], rhs[i]);
    CAPTURE(natural.to_string());
    auto res = Natural::parse(natural.to_string());
    CAPTURE(res->to_string());
    REQUIRE(natural == res);
  }

  SECTION("add") {
    REQUIRE((lhs[i] + rhs[i]).to_string() == str_sum[i]);
    REQUIRE((rhs[i] + lhs[i]).to_string() == str_sum[i]);
  }

  SECTION("sub") {
    auto sum = lhs[i] + rhs[i];
    REQUIRE(sum.to_string() == str_sum[i]);
    REQUIRE(sum - lhs[i] == rhs[i]);
    REQUIRE(sum - rhs[i] == lhs[i]);
  }

  SECTION("cmp") {
    auto sum = lhs[i] + rhs[i];
    SECTION("check equal") {
      auto natural = GENERATE_REF(lhs[i], rhs[i], sum);
      REQUIRE(natural == natural);
      REQUIRE(natural <= natural);
      REQUIRE(natural >= natural);
      REQUIRE_FALSE(natural != natural);
      REQUIRE_FALSE(natural < natural);
      REQUIRE_FALSE(natural > natural);
    }

    SECTION("check less") {
      auto l = GENERATE_REF(lhs[i], rhs[i]);
      auto r = GENERATE_REF(l, sum) + Natural{1};
      REQUIRE(l != r);
      REQUIRE_FALSE(l == r);
      REQUIRE(l < r);
      REQUIRE(l <= r);
      REQUIRE_FALSE(l > r);
      REQUIRE_FALSE(l >= r);

      REQUIRE(r != l);
      REQUIRE_FALSE(r == l);
      REQUIRE(r > l);
      REQUIRE(r >= l);
      REQUIRE_FALSE(r < l);
      REQUIRE_FALSE(r <= l);
    };
  }

  SECTION("move") {
    auto natural = GENERATE_REF(lhs[i], lhs[i]);
    CAPTURE(natural.to_string());

    SECTION("left move 1") {
      CAPTURE((natural + natural).to_string());
      CAPTURE((natural << 1).to_string());
      REQUIRE(natural << 1 == (natural + natural));
    }

    SECTION("right move 1") {
      auto half = natural >> 1;
      CAPTURE(half.to_string());
      auto whole = half + half;
      CAPTURE(whole.to_string());
      REQUIRE(((whole == natural) || (whole + Natural{1} == natural)));
    }

    size_t offset = GENERATE(take(10, random(0, 100)));
    CAPTURE(offset);

    SECTION("left move assign") {
      Natural assign = natural;
      assign <<= offset;
      CAPTURE(assign);
      CAPTURE(natural << offset);
      REQUIRE(assign == natural << offset);
    }

    SECTION("right move assign") {
      Natural assign = natural;
      assign >>= offset;
      CAPTURE(assign);
      CAPTURE(natural >> offset);
      REQUIRE(assign == natural >> offset);
    }

    SECTION("left and right") {
      CAPTURE(natural << offset);
      CAPTURE(natural << offset >> offset);
      REQUIRE(natural << offset >> offset == natural);
    }
  }
}