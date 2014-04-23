//Includes
#include "Application.hpp"

namespace Brimstone {
    ApplicationHandle Application::getHandle() {
        return m_cImpl.getHandle();
    }
}