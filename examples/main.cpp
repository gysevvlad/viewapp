#include "viewapp/Service.h"

int main()
{
    views_service::Service service;

    auto & view = service.createView()
        .setPosition(100, 100)
        .setSize(200, 45)
        .setText("app");

    auto & edit = view.createEdit()
        .setPosition(10, 10)
        .setSize(180, 25)
        .setText("");
    
    service.run();
}