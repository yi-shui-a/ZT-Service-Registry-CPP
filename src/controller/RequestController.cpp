#include "RequestController.h"

json RequestController::handleRegister(RequestInfo request)
{
    if(request.isEmpty() == true){
        throw std::invalid_argument("Value cannot be negative");
    }
    
    
}