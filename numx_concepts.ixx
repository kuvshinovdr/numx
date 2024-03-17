/// @file   numx_concepts.ixx
/// @brief  Concepts definitions for numx library.
/// @author D.R.Kuvshinov kuvshinovdr at yandex.ru
export module numx.concepts;

import std;

export namespace numx
{

  /// @brief User specializable configuration.
  namespace config
  {

    template <typename T>
    constexpr bool is_floating_point = std::floating_point<T>;

    template <typename T>
    constexpr bool is_integral = std::integral<T>;

  }

  using config::is_floating_point;
  using config::is_integral;

  template <typename T>
  concept Floating_point =
    is_floating_point<T>;

  template <typename T>
  concept Integral =
    is_integral<T>;


  template <typename T, typename U>
  concept Addable_with =
    requires(T const& t, T& s, U const& u)
    {
      t + u;
      s += u;
    };

  template <typename T>
  concept Addable =
    Addable_with<T, T> &&
    requires(T const& t, T& s)
    {
      {t + t}  -> std::same_as<T>;
      {s += t} -> std::same_as<T&>;
    };

  template <typename T>
  concept Additive_group =
    std::regular<T> &&
    Addable<T> &&
    requires(T const& t, T& s)
    {
      {+t}     -> std::same_as<T>;
      {-t}     -> std::same_as<T>;
      {t - t}  -> std::same_as<T>;
      {s -= t} -> std::same_as<T&>;
    };

}
