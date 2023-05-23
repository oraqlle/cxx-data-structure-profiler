#ifndef CXX_CONTAINER_TESTING_RUNNERS
#define CXX_CONTAINER_TESTING_RUNNERS

#include <fmt/core.h>
#include <fmt/std.h>
#include <fmt/color.h>

#include <benchmark.hpp>
#include <csv-writer.hpp>
#include <makers.hpp>
#include <tests.hpp>
#include <types.hpp>

#include <range/v3/range/conversion.hpp>

#include <chrono>
#include <deque>
#include <filesystem>
#include <list>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

namespace runners {

namespace fs = std::filesystem;
using namespace std::literals;

auto to_count = std::views::transform([](auto x) { return x.count(); });

auto create_sizes = [](std::size_t scale) -> std::vector<std::size_t> {
    return std::views::iota(1)
        | std::views::transform([&](auto x) { return x * scale; })
        | std::views::take(10)
        | ranges::to<std::vector<std::size_t>>();
};

template <typename T = void>
struct PushBack {

    static constexpr std::string_view name = "Push Back";

    static auto run() -> void
    {
        auto sizes = create_sizes(100'000uL);

        auto list_results = benchmark::run<std::list<T>, tests::PushBack, makers::Empty, std::chrono::microseconds>(sizes, name, "std::list");
        auto deque_results = benchmark::run<std::deque<T>, tests::PushBack, makers::Empty, std::chrono::microseconds>(sizes, name, "std::deque");
        auto vec_results = benchmark::run<std::vector<T>, tests::PushBack, makers::Empty, std::chrono::microseconds>(sizes, name, "std::vector");
        auto pre_vec_results = benchmark::run<std::vector<T>, tests::PushBack, makers::Preallocated, std::chrono::microseconds>(sizes, name, "Preallocated std::vector");

        auto fname = ""s.append(types::name<T>()) + ".csv"s;
        csv::write(fs::path { "pushback" }, fname, "elements"s, sizes);
        csv::write(fs::path { "pushback" }, fname, "std::list"s, list_results | to_count | ranges::to<std::vector<long double>>());
        csv::write(fs::path { "pushback" }, fname, "std::deque"s, deque_results | to_count | ranges::to<std::vector<long double>>());
        csv::write(fs::path { "pushback" }, fname, "std::vector"s, vec_results | to_count | ranges::to<std::vector<long double>>());
        csv::write(fs::path { "pushback" }, fname, "preallocated std::vector"s, pre_vec_results | to_count | ranges::to<std::vector<long double>>());
    }
}; // struct PushBack

template <typename T = void>
struct LinearSearch {

    static constexpr std::string_view name = "Linear Search";

    static auto run() -> void
    {
        auto sizes = create_sizes(1'000uL);

        auto list_results = benchmark::run<std::list<T>, tests::LinearSearch, makers::FilledRandom, std::chrono::microseconds>(sizes, name, "std::list");
        auto deque_results = benchmark::run<std::deque<T>, tests::LinearSearch, makers::FilledRandom, std::chrono::microseconds>(sizes, name, "std::deque");
        auto vec_results = benchmark::run<std::vector<T>, tests::LinearSearch, makers::FilledRandom, std::chrono::microseconds>(sizes, name, "std::vector");

        auto fname = ""s.append(types::name<T>()) + ".csv"s;
        csv::write(fs::path { "linearsearch" }, fname, "elements"s, sizes);
        csv::write(fs::path { "linearsearch" }, fname, "std::list"s, list_results | to_count | ranges::to<std::vector<long double>>());
        csv::write(fs::path { "linearsearch" }, fname, "std::deque"s, deque_results | to_count | ranges::to<std::vector<long double>>());
        csv::write(fs::path { "linearsearch" }, fname, "std::vector"s, vec_results | to_count | ranges::to<std::vector<long double>>());
    }
}; // struct LinearSearch

template <template <typename> class Runner>
auto run_for_types() -> void
{
}

template <template <typename> class Runner, typename T, typename... Ts>
auto run_for_types() -> void
{
    fmt::print(
        "{} {} -- {} {}\n",
        fmt::styled(
            "[ Started Benchmark ]:",
            fmt::emphasis::bold | fmt::fg(fmt::color::orange)
        ),
        Runner<void>::name,
        fmt::styled(
            "[ Element Type ]:",
            fmt::emphasis::bold | fmt::fg(fmt::color::purple)
        ),
        types::name<T>()
    );

    Runner<T>::run();

    fmt::print(
        "{} {} -- {} {}\n",
        fmt::styled(
            "[ Finished Benchmark ]:",
            fmt::emphasis::bold | fmt::fg(fmt::color::green)
        ),
        Runner<void>::name,
        fmt::styled(
            "[ Element Type ]:",
            fmt::emphasis::bold | fmt::fg(fmt::color::purple)
        ),
        types::name<T>()
    );

    run_for_types<Runner, Ts...>();
}

template <typename... Ts>
auto all() -> void
{
    fmt::print(
        "{}\n",
        fmt::styled(
            "[ Started All Benchmarks ]",
            fmt::emphasis::bold | fmt::fg(fmt::color::gold)
        )
    );

    run_for_types<PushBack, Ts...>();
    run_for_types<LinearSearch, Ts...>();

    fmt::print(
        "{}\n",
        fmt::styled(
            "[ Finished All Benchmarks ]",
            fmt::emphasis::bold | fmt::fg(fmt::color::light_green)
        )
    );
}

} // namespace runners

#endif // CXX_CONTAINER_TESTING_RUNNERS
