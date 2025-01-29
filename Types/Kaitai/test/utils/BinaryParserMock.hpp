//
// Created by stefan on 1/28/2025.
//

#pragma once

#include <kaitai/BinaryParser>
#include <kaitai/Parser>
#include <iostream>

namespace kaitai::testing {
using detail::BinaryParser;
using detail::ParsedField;
using detail::ParsedBinary;

auto parsedData(auto&&... headers) {
  ParsedBinary parsed_binary {};
  (parsed_binary.fields.emplace_back(std::forward<decltype(headers)>(headers)), ...);
  return parsed_binary;
}

auto header(std::string&& name, auto&&... fields) {
  ParsedField result{std::move(name), ParsedField::CompoundData{}};
  auto& resultVector = std::get<ParsedField::CompoundData>(result.value);
  (resultVector.emplace_back(std::forward<decltype(fields)>(fields)), ...);
  return result;
}

inline auto content(std::string&& name, bool valid) { return ParsedField{std::move(name), valid}; }
inline auto type(std::string&& name, ParsedField::IntData data) { return ParsedField{std::move(name), data}; }
inline auto size(std::string&& name, ParsedField::StringData&& data) { return ParsedField{std::move(name), std::move(data)}; }

inline auto compareBinaries(ParsedBinary const& actual, ParsedBinary const& expected) {
  auto& actualFields = actual.fields;
  auto& expectedFields = expected.fields;

  auto compareSizes = [](auto&& rng1, auto&& rng2, auto&& rng1Name, auto&& rng2Name) {
    if (rng1.size() != rng2.size()) {
      std::cout << std::format("Non matching number of fields in {}(size={}) and {}(size={})\n", rng1Name, rng1.size(), rng2Name, rng2.size());
      return false;
    }
    return true;
  };

  if (!compareSizes(actualFields, expectedFields, "actual", "expected")) {
    return false;
  }

  for (auto actIt = actualFields.begin(), expIt = expectedFields.begin(); actIt != actualFields.end(); ++actIt, ++expIt) {
    if (actIt->name != expIt->name) {
      std::cout << std::format("Non matching fields: actualName='{}', expName='{}'\n", actIt->name, expIt->name);
      return false;
    }
    assert(std::holds_alternative<ParsedField::CompoundData>(actIt->value) && "Top level header should always be a vector");
    assert(std::holds_alternative<ParsedField::CompoundData>(expIt->value) && "Top level header on expected should always be a vector");

    auto actEntries = std::get<ParsedField::CompoundData>(actIt->value);
    auto expEntries = std::get<ParsedField::CompoundData>(expIt->value);

    if (!compareSizes(actEntries, expEntries, actIt->name, expIt->name)) {
      return false;
    }
    for (auto actIt = actEntries.begin(), expIt = expEntries.begin(); actIt != actEntries.end(); ++actIt, ++expIt) {
      if (actIt->name != expIt->name) {
        std::cout << std::format("Non matching fields: actualName='{}', expName='{}'\n", actIt->name, expIt->name);
        return false;
      }

      if (!std::visit([&expIt]<typename T>(T const& actData) {
        if constexpr (std::is_same_v<T, ParsedField::CompoundData>) {
          std::cout << std::format("Subfield cannot be a compound type (for field={})\n", expIt->name);
          return false;
        } else {
          if (!std::holds_alternative<T>(expIt->value)) {
            std::cout << std::format("Non matching types for field with name {}\n", expIt->name);
            return false;
          }
          auto& expData = std::get<T>(expIt->value);
          if (actData != expData) {
            std::cout << std::format("Non matching values for field {}: actual={}, exp={}\n", expIt->name, actData, expData);
            return false;
          }
          return true;
        }
      }, actIt->value)) {
        return false;
      }
    }
  }
  return true;
}
} // namespace kaitai::testing