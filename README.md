# База данных ссылок на ресурсы интернет.

База данных для хранения ссылок на различный контент из сети интернет.
Для хранения данных используется база на основе sqllite3. Для начала работы
не нужно настраивать сервер баз данных.

## Возможности

База хранит ссылки и тэги.

## Настройка

Сама база данных sqllite должна быть создана заранее и распологаться
~/.local/share/UrlDataBase/base.db

1. Вариант. Создать базу внешними средствами

[Создание SQLite базы данных в DB Browser for SQLite](http://blog.harrix.org/article/5154)
Команды для создания базы:
```sql
CREATE TABLE `refs` (
	`id`	INTEGER PRIMARY KEY AUTOINCREMENT,
	`name`	TEXT NOT NULL,
	`datecreate`	TEXT,
	`url`	TEXT,
	`favorite`	INTEGER DEFAULT 0
);

CREATE TABLE `tags` (
	`fk_id`	INTEGER,
	`TagName`	TEXT
);
```

2.Вариант. Использовать базу из каталогов исходников.


