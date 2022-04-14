#include "gui_elements.hpp"
#include <array>
#include <string_view>
#include <imgui.h>

namespace dev::gui
{
bool drawIpProtocolSelection(con::IpProtocol &protocol)
{
    static constexpr std::array<std::string_view, 3> kNamesProtocol{"none", "ipv4", "ipv6"};
    bool has_changed = false;
    if (ImGui::BeginCombo("Listen protocol",
                          (protocol >= con::IpProtocol::none && protocol <= con::IpProtocol::ipv6)
                              ? kNamesProtocol[static_cast<int>(protocol)].data()
                              : "unknown"))
    {
        for (int i = 0; i < kNamesProtocol.size(); i++)
        {

            if (ImGui::Selectable(kNamesProtocol[i].data(), protocol == con::IpProtocol(i)))
            {
                protocol = con::IpProtocol(i);
                has_changed = true;
            }
        }
        ImGui::EndCombo();
    }

    return has_changed;
}
} // namespace dev::gui
