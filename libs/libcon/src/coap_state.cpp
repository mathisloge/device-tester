#include "coap_state.hpp"
#include <mutex>
#include <coap3/coap.h>
#include <spdlog/spdlog.h>

namespace dev::con
{
static std::once_flag coap_init_called;
static void coap_init_once();
void ensure_coap()
{
    std::call_once(coap_init_called, coap_init_once);
}

static void coap_init_once()
{
    spdlog::info("coap client initialised");
    coap_startup();
    const coap_log_t log_level = LOG_DEBUG;
    coap_dtls_set_log_level(log_level);
    coap_set_log_level(log_level);
}
} // namespace dev::con
