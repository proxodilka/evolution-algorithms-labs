#pragma once


namespace Utils{

template <typename T, typename Dest>
struct rebind_container_type {};

template <typename T, typename Dest, template<typename, typename...> class V, typename... Args>
struct rebind_container_type<V<T, Args...>, Dest>
{
	using type = T;
	using result = V<Dest>;
};

template <typename Src, typename Dest>
struct rebind_container {};

template <typename T, template<typename, typename...> class V, typename Y, template<typename, typename...> class Dest, typename... Args, typename... DArgs>
struct rebind_container<V<T, Args...>, Dest<Y, DArgs...>> {
	using value = Dest<T>;
};

template <typename T>
struct determine_container_type {};

template <typename T, template<typename, typename...> class V, typename... Args>
struct determine_container_type<V<T, Args...>>
{
	using value = T;
};

} // namespace Utils
