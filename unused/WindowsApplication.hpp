#ifndef BS_WINDOWS_APPLICATION_HPP
#define BS_WINDOWS_APPLICATION_HPP




//Includes
#include "WindowsHeader.hpp"




namespace Brimstone {
namespace Private {

class WindowsApplication {
public:
    HINSTANCE getHandle();
};

}
}




#endif //BS_WINDOWS_APPLICATION_HPP