#pragma once
#include <libcon/manager.hpp>
#include <gui-common/window.hpp>
namespace dev::gui
{
class ConnectionManagerWin final : public Window
{
  public:
    explicit ConnectionManagerWin(const std::shared_ptr<con::Manager> &manager);
    ~ConnectionManagerWin();

  private:
    void updateContent() override;

  private:
    std::shared_ptr<con::Manager> manager_;
};
} // namespace dev::gui
