#include "KaitaiViewer.hpp"

using namespace GView::View::KaitaiViewer;
using namespace AppCUI::Input;
using kaitai::detail::KaitaiStruct;

#include <fstream>

Settings::Settings()
{
    this->_data = new SettingsData();
}

void Settings::SetKaitaiFormat(std::string_view data) const
{
    std::stringstream stream {std::string(data)};
    _data->format = Parser{stream}();
}

std::vector<std::tuple<std::string, std::string>> Settings::GetBinaryData(std::ifstream& istream) const
{
    using kaitai::detail::ParsedField;
    std::vector<std::tuple<std::string, std::string>> result {};
    auto parsedBinary = BinaryParser{_data->format}(istream);

    for (auto const& field : parsedBinary.fields) {
        assert(std::holds_alternative<ParsedField::CompoundData>(field.value) && "Top level item should be compound data");
        for (auto const& [name, value] : std::get<ParsedField::CompoundData>(field.value)) {
            auto visitor = kaitai::detail::overloadSet {
                [](ParsedField::IntData data) -> std::string {
                    return std::to_string(data);
                },
                [](ParsedField::StringData const& data) -> std::string {
                    return data;
                },
                [](ParsedField::SignatureData data) -> std::string {
                    return data ? "valid" : "invalid";
                },
                [](ParsedField::CompoundData const&) -> std::string {
                    assert(false && "This should not be a compound field");
                    return "";
                }
            };
            result.emplace_back(name, std::visit(visitor, value));
        }
    }
    return result;
}
