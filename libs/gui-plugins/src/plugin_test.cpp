#include <plugin_test.hpp>
namespace dev::gui
{

PluginTest::PluginTest()
    : Window{"PLUGIN TEST"}
{
    plugin_.addPlugin("lua/plugins/test.lua");
}
PluginTest::~PluginTest()
{}
void PluginTest::updateContent()
{}

} // namespace dev::gui
