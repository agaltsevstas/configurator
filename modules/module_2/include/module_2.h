EXPECTED_START
        // Пример добавления параметра
        PARAMETER("netdir", dtString, true, "../dir/");
        PARAMETER("netname", dtString, true, "yolov3");
        PARAMETER("threshold", dtDouble, true, 0.5);
        PARAMETER("messageId", dtInt, true, 5558);
        PARAMETER("port", dtString, true, "17103");
        PARAMETER("ip", dtString, true, "192.168.3.101");

        PARAMETER("weights", dtString, false, "");
        PARAMETER("send", dtInt, false, true);
        PARAMETER("show", dtInt, false, false);
        PARAMETER("roi", dtString, false, "0.0;0.0;1.0;1.0");
        PARAMETER("isTiny", dtInt, false, false);
        PARAMETER("classCut", dtInt, false, 0);
        PARAMETER("camViewCalib", dtString, false, "");
EXPECTED_END
