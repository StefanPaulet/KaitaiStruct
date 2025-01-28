#include "KaitaiViewer.hpp"
#include "TextViewer.hpp"

using namespace GView::View::KaitaiViewer;
using namespace AppCUI::Input;

Instance::Instance(Reference<GView::Object> _obj, Settings* _settings)
    : ViewControl("Kaitai View", UserControlFlags::ShowVerticalScrollBar | UserControlFlags::ScrollBarOutsideControl), settings(nullptr)
{
    this->obj = _obj;

    // settings
    if ((_settings) && (_settings->_data))
    {
        // move settings data pointer
        this->settings.reset((SettingsData*) _settings->_data);
        _settings->_data = nullptr;
    }
    else {
        // default setup
        this->settings.reset(new SettingsData());
    }
}

bool Instance::GoTo(uint64 offset)
{
    return true;
}
bool Instance::Select(uint64 offset, uint64 size)
{
    return false; // no selection is possible in this mode
}
bool Instance::ShowGoToDialog()
{
    return true;
}
bool Instance::ShowFindDialog()
{
    NOT_IMPLEMENTED(false);
}
bool Instance::ShowCopyDialog()
{
    NOT_IMPLEMENTED(false);
}
void Instance::PaintCursorInformation(AppCUI::Graphics::Renderer& r, uint32 width, uint32 height)
{
    if (height == 0) {
        return;
    }
    r.WriteSingleLineText(0, 0, "Cursor data", ColorPair{Color::Yellow, Color::Transparent});
}

bool Instance::GetPropertyValue(uint32 id, PropertyValue& value)
{
    return false;
}
bool Instance::SetPropertyValue(uint32 id, const PropertyValue& value, String& error)
{
    return false;
}
void Instance::SetCustomPropertyValue(uint32 propertyID)
{
    return;
}
bool Instance::IsPropertyValueReadOnly(uint32 propertyID)
{
    return false;
}
const vector<Property> Instance::GetPropertiesList()
{
    return {};
}

void Instance::Paint(Graphics::Renderer& renderer)
{
    constexpr auto startingX = 2;
    constexpr auto startingY = 2;
    ColorPair textColor      = ColorPair{ Color::Yellow, Color::Transparent };

    auto const& metaData = settings->format.getMeta();
    std::vector<std::tuple<unsigned, std::string>> headerLines{
        { 0, "Meta information:" },
        { 1, std::format("Ksy file id: {}", metaData.id) },
        { 1, std::format("Expected file extension: {}", metaData.fileExtension) },
        { 1, std::format("Endianess of binary file: {}", metaData.endian == kaitai::detail::Endian::BIG ? "big" : "little") }
    };

    headerLines.emplace_back(0, "");
    headerLines.emplace_back(0, "Expected file format:");
    auto const& sequence = settings->format.getSequence();
    auto const& types    = settings->format.getTypes();
    for (auto const& [fieldName, fieldTypeName] : sequence) {
        auto const& type = types.at(fieldTypeName);
        headerLines.emplace_back(1, fieldName + ": {");
        for (auto const& chunk : type.seq.chunks()) {
            auto currentLine{ std::format("{}: ", chunk.id) };
            auto fieldString = std::visit(
                  []<typename T>(T const& element) {
                      if constexpr (std::is_same_v<T, Chunk::Size>) {
                          return std::format("sizedField(size = '{}')", element);
                      } else if constexpr (std::is_same_v<T, Chunk::Constant>) {
                          return std::format("signature(expectedValue = '{}')", element);
                      } else if constexpr (std::is_same_v<T, Chunk::RawType>) {
                          return std::format("typedField(expectedType = '{}')", element);
                      }
                  },
                  chunk.data);
            headerLines.emplace_back(2, currentLine.append(std::move(fieldString)));
        }
        headerLines.emplace_back(1, "}");
    }

    auto lineIdx = 0;
    for (auto const& [indent, line] : headerLines) {
        renderer.WriteSingleLineText(startingX + indent, startingY + lineIdx++, line, textColor);
    }
}

bool Instance::OnUpdateCommandBar(Application::CommandBar& commandBar)
{
    commandBar.SetCommand(Commands::OpenBinaryFileCommand.Key, Commands::OpenBinaryFileCommand.Caption, Commands::OpenBinaryFileCommand.CommandId);
    return false;
}
bool Instance::UpdateKeys(KeyboardControlsInterface* interface)
{
    interface->RegisterKey(&Commands::OpenBinaryFileCommand);
    return true;
}

bool Instance::OnEvent(Reference<Control> sender, Event eventType, int controlID)
{
    if (eventType != Event::Command) {
        return false;
    }
    if (controlID == Commands::OpenBinaryFileCommand.CommandId) {
    }
    return false;
}
// #undef BT