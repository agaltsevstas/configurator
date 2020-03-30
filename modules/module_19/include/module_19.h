/// Перечисление ожидаемых и обязательных параметров модуля
EXPECTED_START
        // Пример добавления параметра
        PARAMETER("struct", dtString, true, "" );
        PARAMETER("labels", dtString, true, "" );
        PARAMETER("weights", dtString, true, "" );
        PARAMETER("threshold", dtDouble, false, 0.5 );
        PARAMETER("maxBatchSize", dtInt, false, 2 );
EXPECTED_END	
