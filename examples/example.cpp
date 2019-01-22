#include "viewapp/Service.h"

int main()
{
    views_service::Service service;

    auto & view = service.createView()
        .setPosition(100, 100)
        .setSize(200, 195)
        .setText("app");

    auto & edit = view.createEdit()
        .setPosition(10, 10)
        .setSize(180, 25)
        .setText("edit: ");

    auto & combobox = view.createComboBox()
        .setPosition(10, 70)
        .setSize(180, 25)
        .setText("combobox");

    auto & button = view.createButton()
        .setPosition(10, 40)
        .setSize(180, 25)
        .setText("close")
        .setClickHandler(
            [&combobox]() {
                combobox.addItems({ "1", "2", "3", "4" });
            });

    return service.run();
}
