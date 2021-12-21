#pragma once
class WebUIDataSource {
public:
    // 왜 이렇게 생성?
    static WebUIDataSource* Create();
    static void Add(WebUIDataSource* source);
    static void AddString();
};
