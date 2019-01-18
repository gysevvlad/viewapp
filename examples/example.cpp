#include <iostream>
#include "viewapp/Service.h"

int main()
{
    views_service::Service service;

    auto & view = service.createView()
        .setPosition(100, 100)
        .setSize(200, 45)
        .setText("app");

    auto & button = view.createButton()
        .setPosition(10, 10)
        .setSize(180, 25)
        .setText("close")
        .setClickHandler(
            [&view]() {
                view.Close();
            });

    try {
        return service.run();
    }
    catch (const std::exception & e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }
}
