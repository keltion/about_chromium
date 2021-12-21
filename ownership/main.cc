#include "web_ui_data_source.h"
// 1. 내가 해주냐
// 2. Add()에서 해주냐
// -> std::unique_ptr을 이용해서 Add()에서 함을 명확히 할 수 있지

// void caller1() {
//     std::unique_ptr<WebUIDataSource> source = WebUIDataSource::Create();
//     source->AddString();
//     WebUIDataSource::Add(std::move(source));
// } 
// -> Add()에서 관리하겠구나

// 딱 내가 느꼈던 그 기분. 어? 이거 어디서 관리되는거야?를 해소해줄 수 있음

// =================== 근데 지금 하는건? =====================

void caller1() {
    WebUIDataSource *source = WebUIDataSource::Create();
    WebUIDataSource::Add(source);
    source->AddString();
}

void caller2() {
    WebUIDataSource *source = WebUIDataSource::Create();
    WebUIDataSource::Add(source);
    source->AddString();
}

void caller3() {
    WebUIDataSource *source = WebUIDataSource::Create();
    WebUIDataSource::Add(source);
    source->AddString();
}
int main() {
    caller1();
    caller2();
    caller3();
}