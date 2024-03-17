/// @file   numx_compsum.ixx
/// @brief  Accumulate sums with compensation (Kahan's summation algorithm).
/// @author D.R.Kuvshinov kuvshinovdr at yandex.ru
export module numx_compsum;

import numx.concepts;
import std;

export namespace numx
{

  namespace base
  {

    template <Additive_group T>
    class Kahan_compensated_sum
    {
    public:
      constexpr Kahan_compensated_sum() noexcept(std::is_nothrow_constructible_v<T>) 
        = default;

      constexpr Kahan_compensated_sum(T init) noexcept(std::is_nothrow_copy_constructible_v<T>)
        : _sum(init) {}

      constexpr Kahan_compensated_sum(T&& init) noexcept(std::is_nothrow_move_constructible_v<T>)
        : _sum(std::move(init)) {}

      /// @brief Get the compensated sum.
      constexpr auto operator()() const
        -> T { return _sum + _comp; }

      /// @brief Accumulate the sum with compensation update.
      constexpr void operator()(T next_addend)
      {
        auto const y { next_addend - _comp };
        auto const t { _sum + y };
        _comp = (t - _sum) - y;
        _sum  = t;
      }

    private:
      T _sum  {};
      T _comp {};
    };

  }


  template <std::ranges::input_range InputData>
  constexpr auto kahan_sum(InputData&& input_data)
    -> std::ranges::range_value_t<InputData>
  {
    base::Kahan_compensated_sum< std::ranges::range_value_t<InputData> > ks;
    
    for (auto&& item: input_data)
      ks(item);
    
    return ks();
  }

  template <std::ranges::input_range InputData, Additive_group SumType>
  constexpr auto kahan_sum(InputData&& input_data, SumType&& init_sum)
    -> std::decay_t<SumType>
  {
    base::Kahan_compensated_sum< std::decay_t<SumType> > ks{ std::forward<SumType>(init_sum) };

    for (auto&& item: input_data)
      ks(item);

    return ks();
  }



}
