#pragma once
#include <sol/sol.hpp>
namespace dev::plg
{
    void registerImGuiFunctions(sol::table ns);
    void registerImPlotFunctions(sol::table ns);
}
