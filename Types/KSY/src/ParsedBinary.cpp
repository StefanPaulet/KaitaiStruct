#include "kaitai.hpp"

using namespace AppCUI::Controls;

namespace GView::Type::Kaitai::Panels
{
 ParsedBinary::ParsedBinary(Reference<Object> _object, Reference<GView::Type::Kaitai::KaitaiFile> _kaitai) : TabPage("ParsedData")
{
    object  = _object;
    kaitai  = _kaitai;
    general = Factory::ListView::Create(this, "x:0,y:0,w:100%,h:10", { "n:Field,w:30", "n:Value,w:100" }, ListViewFlags::None);

    Update();
}

void ParsedBinary::UpdateGeneralInformation()
{
    general->DeleteAllItems();

    auto item = general->AddItem("Parsed Data Info");
    item.SetType(ListViewItem::Type::Category);
    for (auto const& [name, value] : kaitai->parsedBinaryInformation) {
        general->AddItem({name, value});
    }
}

void ParsedBinary::RecomputePanelsPositions()
{
    int32 py   = 0;
    int32 last = 0;
    int32 w    = this->GetWidth();
    int32 h    = this->GetHeight();

    if (!general.IsValid())
    {
     return;
    }
    this->general->Resize(w, std::min<>(h - py, (int32) this->general->GetItemsCount() + 3));
}

void ParsedBinary::Update()
 {
     UpdateGeneralInformation();
     RecomputePanelsPositions();
 }
}