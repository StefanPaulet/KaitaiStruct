#pragma once

#include "Internal.hpp"
#include "AppCUI/include/AppCUI.hpp"

namespace GView::View::KaitaiViewer
{
using namespace AppCUI;
using namespace GView::Utils;

struct SettingsData {
    int analysisLevel = 0;
    SettingsData();
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
};

} // namespace GView::View::KaitaiViewer

; // namespace GView