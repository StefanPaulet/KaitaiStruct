#include "kaitai.hpp"

using namespace AppCUI;
using namespace AppCUI::Utils;
using namespace AppCUI::Application;
using namespace AppCUI::Controls;
using namespace GView::Utils;
using namespace GView::Type;
using namespace GView;
using namespace GView::View;

#include "../../Types/Kaitai/include/kaitai/Parser"
#include "../../Types/Kaitai/include/kaitai/BinaryParser"
#include <fstream>



extern "C"
{
    PLUGIN_EXPORT bool Validate(const AppCUI::Utils::BufferView& buf, const std::string_view& extension)
    {
        return true;
    }
    PLUGIN_EXPORT TypeInterface* CreateInstance()
    {
        return new Kaitai::KaitaiFile();
    }
    PLUGIN_EXPORT bool PopulateWindow(Reference<WindowInterface> win)
    {
        auto ksy = win->GetObject()->GetContentType<Kaitai::KaitaiFile>();
        ksy->Update();

        GView::View::KaitaiViewer::Settings settings{};
        settings.SetKaitaiFormat(ksy->obj->GetData().Get(0, ksy->obj->GetData().GetSize(), true));

        auto filePath = Dialogs::FileDialog::ShowOpenFileWindow("", "", ".");
        if (filePath.has_value()) {
            auto path = std::ifstream{std::filesystem::path(filePath.value().u16string())};
            ksy->parsedBinaryInformation = settings.GetBinaryData(path);
        }

        win->CreateViewer(settings);
        win->AddPanel(Pointer<TabPage>(new Kaitai::Panels::ParsedBinary(win->GetObject(), ksy)), true);
        return true;
    }
    PLUGIN_EXPORT void UpdateSettings(IniSection sect)
    {
        sect["Extension"]   = "ksy";
        sect["Priority"]    = 1;
        sect["Description"] = "Kaitai struct files (*.ksy)";
    }
}

int main()
{
    return 0;
}
