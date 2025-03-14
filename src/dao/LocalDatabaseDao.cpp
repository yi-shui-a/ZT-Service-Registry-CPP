#include "LocalDatabaseDao.h"



bool LocalDatabaseDao::existService(std::string& service){
    LocalDatabase* database = LocalDatabase::getInstance();
    json services = database->getJsonDatabase();
    for (json& element : services) {
        if(element["service_name"] == service){
            return true;
        }
    }
    return false;
}


bool LocalDatabaseDao::existInstance(std::string& service){

}

bool LocalDatabaseDao::existMetadata(std::string& service){

}


bool LocalDatabaseDao::existInstanceMetadata(std::string& service, std::string& server){



}


