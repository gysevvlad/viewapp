#include "viewapp/Service.h"

int main()
{
    views_service::Service service;

    auto & view = service.createView()
        .setPosition(100, 100)
        .setSize(250, 195)
        .setText("app");

    auto & combobox = view.createComboBox()
        .setPosition(10, 10)
        .setSize(180, 180)
        .setText("");

    auto & add = view.createButton()
        .setPosition(195, 10)
        .setSize(45, 25)
        .setText("add")
        .setClickHandler(
            [&combobox]() {
                combobox.addItem(combobox.getInputText());
            });

    auto & del = view.createButton()
        .setPosition(195, 40)
        .setSize(45, 25)
        .setText("del")
        .setClickHandler(
            [&combobox]() {
                combobox.eraseItem(combobox.getSelectedItem());
            });

    return service.run();
}
