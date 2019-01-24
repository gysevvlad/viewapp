#include "viewapp/Service.h"

int main()
{
    views_service::Service service;

    auto & view = service.createView()
        .setPosition(100, 100)
        .setSize(250, 225)
        .setText("app");

    auto & combobox1 = view.createComboBox()
        .useDropDownType()
        .setPosition(10, 10)
        .setSize(180, 180)
        .setText("");

    auto & combobox2 = view.createComboBox()
        .useSimpleType()
        .setPosition(10, 40)
        .setSize(180, 180)
        .setText("");

    auto & add = view.createButton()
        .setPosition(195, 10)
        .setSize(45, 25)
        .setText("add")
        .setClickHandler(
            [&combobox1, &combobox2]() {
                combobox1.addItem(combobox1.getInputText());
                combobox2.addItem(combobox1.getInputText());
            });

    auto & del = view.createButton()
        .setPosition(195, 40)
        .setSize(45, 25)
        .setText("del")
        .setClickHandler(
            [&combobox1, &combobox2]() {
                std::wstring selected{ combobox1.getSelectedItem() };
                combobox1.eraseItem(selected);
                combobox2.eraseItem(selected);
            });

    return service.run();
}
