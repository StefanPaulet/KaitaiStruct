#include "KaitaiViewer.hpp"
#include "TextViewer.hpp"

using namespace GView::View::KaitaiViewer;
using namespace AppCUI::Input;

Instance::Instance(Reference<GView::Object> _obj, Settings* _settings)
    : ViewControl("Text View", UserControlFlags::ShowVerticalScrollBar | UserControlFlags::ScrollBarOutsideControl), settings(nullptr)
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
// #undef BT