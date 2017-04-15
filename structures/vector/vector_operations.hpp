/* #ifndef VECTOR_HPP */
/* #error"This file should only be included by vector.hpp" */
/* #endif */

#ifndef VECTOR_OPERATIONS_HPP_
#define VECTOR_OPERATIONS_HPP_

#include"vector_utils.hpp"
#include"../matrices/normal/normal_operations.hpp" //TEMP
#include"tuple"

namespace numpp{
  namespace impl{
    namespace helper{
      template<typename T, typename... Args>
        constexpr auto multiply(const T& first_elem, const Args&... second){
          return std::make_tuple((first_elem * second)...);
        }

      template<typename T, std::size_t Size, typename Tuple, std::size_t... Elements>
        constexpr auto unpack(Tuple&& tup, std::index_sequence<Elements...>){
          return matrix::normal<T, Size, Size>{
            std::get<Elements>(tup) ...
          };
      }
    }
    template<typename T, typename U, std::size_t Size, bool Transposition, typename Operation, std::size_t... Index>
      constexpr auto accumulate(
          const vector<T,Size,Transposition>& first,
          const vector<U,Size,Transposition>& second,
          Operation&& op,
          std::index_sequence<Index...>
      ){
        return numpp::vector<std::common_type_t<T,U>, Size, Transposition>{
          op(first(Index), second(Index)) ...
        };
    }

    template<typename T, typename U, std::size_t Size, bool Transposition, typename Operation, std::size_t... Index>
      constexpr auto accumulate(
          T&& scalar,
          const vector<U,Size,Transposition>& vec,
          Operation&& op,
          std::index_sequence<Index...>
      ){
        return numpp::vector<std::common_type_t<T,U>, Size, Transposition>{
          op(vec(Index), scalar) ...
        };
    }

    template<typename T, typename U, std::size_t Size, typename Operation, std::size_t... Index>
      constexpr auto accumulate(
          const vector<T,Size,false>& first,
          const vector<U,Size,true>& second,
          Operation&& op,
          std::index_sequence<Index...>
      ){
        return helper::unpack<std::common_type_t<T,U>, Size>(
            std::tuple_cat(helper::multiply(first(Index), second(Index)...)...),
            std::make_index_sequence<Size*Size>{}
            );
      }
  }

  //MAIN VECTOR OPERATIONS
	template<typename T, typename U, std::size_t Size, bool Transposition>
		constexpr auto operator+(
				const vector<T,Size,Transposition>& first,
				const vector<U,Size,Transposition>& second
		){
      return impl::accumulate(first, second, std::plus<>{},
          std::make_index_sequence<Size>{});
		}

	template<typename T, typename U, std::size_t Size, bool Transposition>
		constexpr auto operator-(
				const vector<T,Size,Transposition>& first,
				const vector<U,Size,Transposition>& second
		){
      return impl::accumulate(first, second, std::minus<>{},
          std::make_index_sequence<Size>{});
		}


	template<typename T, typename U, std::size_t Size, bool Transposition>
		constexpr auto operator*(
				const vector<T,Size,Transposition>& vec,
				U&& scalar
		){
      return impl::accumulate(std::forward<U>(scalar), vec, std::multiplies<>{},
          std::make_index_sequence<Size>{});
		}


	template<typename T,typename U, std::size_t Size, bool Transposition>
		constexpr auto operator*(
				U&& scalar,
				const vector<T,Size,Transposition>& vec
		){
      return vec*scalar;
		}

	template<typename T,typename U, std::size_t Size>
		constexpr auto operator*(
				const vector<T, Size, true>& first,
				const vector<U, Size, false>& second
		){
      T ret{};
      for(std::size_t i=0; i<Size; ++i)
        ret += first(i)*second(i);
      return ret;
		}

	template<typename T,typename U, std::size_t Size>
		constexpr auto operator*(
				const vector<T, Size, false>& first,
				const vector<U, Size, true>& second
		){
      return impl::accumulate(first, second, std::multiplies<>{},
          std::make_index_sequence<Size>{}
      );
		}

	template<typename T, typename U, std::size_t Size, bool Transposition>
		constexpr auto operator/(
				const vector<T,Size,Transposition>& vec,
				U&& scalar
		){
      return impl::accumulate(std::forward<U>(scalar), vec, std::divides<>{},
          std::make_index_sequence<Size>{});
		}
}

#endif
