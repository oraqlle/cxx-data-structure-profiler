#ifndef CXX_CONTAINER_TESTING_CLI
#define CXX_CONTAINER_TESTING_CLI

#include <runners.hpp>
#include <types.hpp>

#include <cxxopts.hpp>

#include <fmt/core.h>
#include <fmt/ranges.h>
#include <fmt/std.h>

#include <array>
#include <string>
#include <string_view>

namespace cli {

using namespace std::literals;

constexpr std::array<std::string_view, 4> test_names = {
    "all           - Run all tests"sv,
    "push-back     - Tests containers ability to add elements to the back."sv,
    "linear-search - Tests ability linearly search for elements in the container."sv,
    "random-insert - Tests containers ability to insert elements into a random position."sv
};

auto run_tests(cxxopts::ParseResult& args) noexcept -> void
{
    try {
        auto test_args = args["test"].as<std::vector<std::string>>();

        for (auto& test_arg : test_args) {
            if (test_arg == "all"sv) {
                runners::all<
                    types::TrivialSmallType,
                    types::TrivialMediumType,
                    types::TrivialLargeType,
                    types::TrivialHugeType,
                    types::TrivialMonsterType>();

                break;
            } else if (test_arg == "push-back") {
                runners::run_for_types<
                    runners::PushBack,
                    types::TrivialSmallType,
                    types::TrivialMediumType,
                    types::TrivialLargeType,
                    types::TrivialHugeType,
                    types::TrivialMonsterType>();
            } else if (test_arg == "linear-search"sv) {
                runners::run_for_types<
                    runners::LinearSearch,
                    types::TrivialSmallType,
                    types::TrivialMediumType,
                    types::TrivialLargeType,
                    types::TrivialHugeType,
                    types::TrivialMonsterType>();
            } else if (test_arg == "random-insert"sv) {
                runners::run_for_types<
                    runners::RandomInsert,
                    types::TrivialSmallType,
                    types::TrivialMediumType,
                    types::TrivialLargeType,
                    types::TrivialHugeType,
                    types::TrivialMonsterType>();
            }
        }
    } catch (cxxopts::exceptions::option_has_no_value& e) {
        return;
    }
}

auto list_tests(cxxopts::ParseResult& args) noexcept -> void
{
    if (args["list"].as<bool>()) {

        fmt::print(
            "C++ Container Tester\n\nAvailable Tests:\n{}\n\n",
            fmt::join(test_names, "\n"sv));
    }
}

} // namespace cli

#endif // CXX_CONTAINER_TESTING_CLI