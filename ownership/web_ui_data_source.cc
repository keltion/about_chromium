#include "web_ui_data_source.h"
#include "web_ui_data_source_impl.h"
#include <string>
#include <iostream>

WebUIDataSource* WebUIDataSource::Create() {
    return new WebUIDataSourceImpl();
}

void WebUIDataSource::Add(WebUIDataSource* source){

};

void WebUIDataSource::AddString() {
    std::cout << "call AddString" << std::endl;
}