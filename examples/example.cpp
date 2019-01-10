#include "viewapp/Service.h"

int main()
{
    views_service::Service service;

    auto & view = service.createView()
        .setPosition(100, 100)
        .setSize(200, 45)
        .setText("app");

    auto & edit1 = view.createEdit()
        .setPosition(10, 10)
        .setSize(180, 25)
        .setText("");

    auto & edit2 = view.createEdit()
        .setPosition(10, 40)
        .setSize(180, 25)
        .setText("");

    auto & button1 = view.createButton()
        .setPosition(195, 10)
        .setSize(60, 25)
        .setText("to edit2")
        .setClickHandler(
            [&edit1, &edit2]() {
                edit2.setText(edit1.getText().value());
            });

    auto & button2 = view.createButton()
        .setPosition(195, 40)
        .setSize(60, 25)
        .setText("to edit1")
        .setClickHandler(
            [&edit1, &edit2]() {
                edit1.setText(edit2.getText().value());
            });

    service.run();
}