## Анализ структуры проекта

В реультате провереденной инспекции было определено
UrlDataBase и RefRecord - божественные классы
Tag - ленивый класс

Были примены шаблоны проектирования GoF

1. Паттерн Template Method (шаблонный метод)
    Был создан класс DataObject для работы с базой данных. Определен обший интерфейс.
    Потомками стали RefRecord, Tag

2. Паттерн Composite (компоновщик)
    Для управления множеством тэгов ссылки был создан класс TagsList, который скрыл от RefRecord внутрнюю реализацию

3. Паттерн Фабричный метод (Factory Method)
    Для генерации объектов был создан класс DataObjectFactory, потомками стали RefRecords(создание ссылок) и Tags(создание тэгов)
    Так же был реализован статический метод remove() для каждой фабрики. Данный метод удаляет записи в таблицах базы с сохранением ссылочной целостности, очищает таблицу отношений объектов.
4. UrlDatabase - Паттерн Facade (фасад)
    В начеле проектирования на визуализацию пользольвательского интерфейса отвечал класс MainWindow
    Класс UrlDatabase был перепрофилирован.
    Реализованы методы:
    - отображение списка ссылок без фильтра
    - отображение тэгов в отдельной таблице
    - отображение списка избранных ссылок
    
    Для разделения ответственности были созданы 2 класса диалогов RefDialog и TagDialog

## Статический анализ кода

Для анализа был использован Cppcheck
Для работы с cppcheck в qt creactor был использован плагин
(Cppcheck в Qt Creator)[https://gres.biz/qtc-cppcheck/]

### Выданные анализатором сообщения

`dataobject.h:13: предупреждение: Cppcheck: Technically the member function 'DataObject::isNew' can be const.` - внесено дополнение
`dataobject.h:14: предупреждение: Cppcheck: Technically the member function 'DataObject::isModified' can be const.` - внесено дополнение
`tag.h:11: предупреждение: Cppcheck: Class 'Tag' has a constructor with 1 argument that is not explicit.` - внесено дополнение
`factorymethod.h:27: предупреждение: Cppcheck: Technically the member function 'RefRecords::createTags' can be static.` - не является ошибкой и задумывался как вспомогательный. Оставлен без изменений
`refrecord.cpp:22: предупреждение: Cppcheck: Member variable 'RefRecord::dateCreate_m' is not initialized in the constructor.` - исправлено
`urllockchecker.h:21: предупреждение: Cppcheck: Class 'UrlLockChecker' has a constructor with 1 argument that is not explicit.` - внесено дополнение
`tag.h:8: предупреждение: Cppcheck: The class 'Tag' has 'copy constructor' but lack of 'operator='.` - перегружен оператор =
`refrecord.h:11: предупреждение: Cppcheck: The class 'RefRecord' has 'copy constructor' but lack of 'operator='.` - перегружен оператор =
