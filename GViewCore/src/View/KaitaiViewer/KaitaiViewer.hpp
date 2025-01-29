#pragma once

#include "Internal.hpp"
#include "AppCUI/include/AppCUI.hpp"

#include <kaitai/Parser>
#include <kaitai/BinaryParser>

namespace GView::View::KaitaiViewer
{
using namespace AppCUI;
using namespace GView::Utils;

using kaitai::detail::BinaryParser;
using kaitai::detail::Chunk;
using kaitai::detail::KaitaiStruct;
using kaitai::detail::ParsedBinary;
using kaitai::detail::ParsedField;
using kaitai::detail::Parser;

namespace Commands
{
    constexpr int OPEN_FILE_TO_PARSE             = 0x01;
    static KeyboardControl OpenBinaryFileCommand = { Input::Key::F6, "OpenBinaryFile", "Open the binary file to parse", OPEN_FILE_TO_PARSE };
} // namespace Commands

struct SettingsData {
    KaitaiStruct format{};

    SettingsData() = default;
};

struct Config {
    bool Loaded;

    static void Update(IniSection sect);
    void Initialize();
};

class Instance : public View::ViewControl {
private:
    Pointer<SettingsData> settings;
    Reference<GView::Object> obj;
    static Config config;
  public:
    Instance(Reference<GView::Object> obj, Settings* settings);

    bool GoTo(uint64 offset) override;
    bool Select(uint64 offset, uint64 size) override;
    bool ShowGoToDialog() override;
    bool ShowCopyDialog() override;
    bool ShowFindDialog() override;
    void PaintCursorInformation(AppCUI::Graphics::Renderer& renderer, uint32 width, uint32 height) override;

    bool GetPropertyValue(uint32 id, PropertyValue& value) override;
    bool SetPropertyValue(uint32 id, const PropertyValue& value, String& error) override;
    void SetCustomPropertyValue(uint32 propertyID) override;
    bool IsPropertyValueReadOnly(uint32 propertyID) override;
    const vector<Property> GetPropertiesList() override;

    void Paint(Graphics::Renderer& renderer) override;
    bool OnUpdateCommandBar(Application::CommandBar& commandBar) override;
    bool UpdateKeys(KeyboardControlsInterface*) override;
    bool OnEvent(Reference<Control> sender, Event eventType, int controlID) override;
};

} // namespace GView::View::KaitaiViewer

; // namespace GView