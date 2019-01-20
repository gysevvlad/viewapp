﻿#include "viewapp/Service.h"

int main()
{
    views_service::Service service;

    auto & view = service.createView()
        .setPosition(100, 100)
        .setSize(200, 75)
        .setText("app");

    auto & edit = view.createEdit()
        .setPosition(10, 10)
        .setSize(180, 25)
        .setText("");

    auto & button = view.createButton()
        .setPosition(10, 40)
        .setSize(180, 25)
        .setText("close")
        .setClickHandler(
            [&view]() {
                view.Close();
            });

    return service.run();
}
