#include "../catch.hpp"
#include "../../differentiation/forward.hpp"

TEST_CASE(
		"forward finite difference tests",
		"[differentiation][forward]"
    ){

  class example_function{
    public:
      constexpr double operator()(double x){
        return (12.3*x*x)/std::sqrt(x);
      }
  };
  example_function f{};

  SECTION("basic result tests"){
    constexpr auto result1 = numpp::derivative::forward(f, 12., 0.0000001);
    constexpr auto result2 = numpp::derivative::forward(f, 12.);
    SECTION("correct results"){
      REQUIRE(result1 == Approx(63.9127));
    }
    SECTION("set h vs auto h"){
      REQUIRE(result1 == Approx(result2));
    }
  }

}