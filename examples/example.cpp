#include "viewapp/Service.h"

int main()
{
    views_service::Service service;

    auto & view = service.createView()
        .setPosition(100, 100)
        .setSize(250, 225)
        .setText("app");

    auto & edit = view.createEdit()
        .setPosition(10, 10)
        .setSize(180, 25)
        .setText("");

    auto & combobox = view.createComboBox()
        .useSimpleType()
        .setPosition(10, 40)
        .setSize(180, 180)
        .setText("");

    auto & add = view.createButton()
        .setPosition(195, 10)
        .setSize(45, 25)
        .setText("add")
        .setClickHandler(
            [&edit, &combobox]() {
                combobox.addItem(edit.getInputText());
            });

    return service.run();
}
