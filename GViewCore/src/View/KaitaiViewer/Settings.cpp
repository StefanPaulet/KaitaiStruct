#include "KaitaiViewer.hpp"

using namespace GView::View::KaitaiViewer;
using namespace AppCUI::Input;

SettingsData::SettingsData()
{

}
Settings::Settings()
{
    this->_data = new SettingsData();
}

void Settings::setAnalysisLevel(int level) const
{
    _data->analysisLevel = level;
}
