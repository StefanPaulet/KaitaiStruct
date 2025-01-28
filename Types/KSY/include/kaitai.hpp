#pragma once

#include "GView.hpp"

namespace GView::Type::Kaitai {
namespace Panels
{
    class ParsedBinary;
}

    class KaitaiFile : public TypeInterface {
        public:
            KaitaiFile() = default;
            virtual ~KaitaiFile() = default;

            std::string_view GetTypeName() override
            {
                return "ksy";
            }

            void RunCommand(std::string_view commandName) override
            {

            }

            bool UpdateKeys(KeyboardControlsInterface* interface) override
            {
                return true;
            }

            bool Update();

        Panels::ParsedBinary* panelPtr;
        std::vector<std::tuple<std::string, std::string>> parsedBinaryInformation {};
    };
namespace Panels
{
    class ParsedBinary : public AppCUI::Controls::TabPage
    {
        Reference<Object> object;
        Reference<GView::Type::Kaitai::KaitaiFile> kaitai;
        Reference<AppCUI::Controls::ListView> general;

        void UpdateGeneralInformation();
        void RecomputePanelsPositions();

    public:
        ParsedBinary(Reference<Object> _object, Reference<GView::Type::Kaitai::KaitaiFile> _kaitai);

        void Update();
        virtual void OnAfterResize(int newWidth, int newHeight) override
        {
            RecomputePanelsPositions();
        }
    };
}

} // namespace GView::Type::Kaitai
