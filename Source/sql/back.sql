
create table limits(
lim INT DEFAULT 0,
группа VARCHAR(100),
предмет VARCHAR(100),
prefix_ VARCHAR(100),
FOREIGN KEY(группа) REFERENCES группы(Имя)
        ON UPDATE CASCADE
        ON DELETE CASCADE,
FOREIGN KEY(предмет) REFERENCES предметы(Наименование)
        ON UPDATE CASCADE
        ON DELETE CASCADE,
FOREIGN KEY(prefix_) REFERENCES prefix(word)
        ON UPDATE CASCADE
        ON DELETE CASCADE
)ENGINE=InnoDB CHARACTER SET=UTF8;

ALTER TABLE limits ADD UNIQUE INDEX (группа,предмет,prefix_);

