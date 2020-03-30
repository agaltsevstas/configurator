/// Перечисление ожидаемых и обязательных параметров модуля
EXPECTED_START
    // Пример добавления параметра
    // PARAMETER( /*NAME*/ "param", /*TYPE*/ dtString, /*REQUIRED*/ true, /*DEFAULT*/ "");
    PARAMETER("modelPath", dtString, true, "/path/");
    PARAMETER("send", dtInt, false, true);
    PARAMETER("classId", dtInt, true, 0);
    PARAMETER("threshold", dtDouble, true, 0.5);
    PARAMETER("doCrop", dtInt, true, true);
    PARAMETER("crop", dtInt, true, 448);
    PARAMETER("height", dtInt, true, 512);
    PARAMETER("width", dtInt, true, 1024);
    PARAMETER("doPreprocess", dtInt, true, false);
    PARAMETER("sendMaskOn", dtInt, true, true);

    PARAMETER("messageId", dtInt, true, 5558);
    PARAMETER("port", dtString, true, "17103");
    PARAMETER("ip", dtString, true, "192.168.3.101");
EXPECTED_END    
