/* Список основных таблиц*/ 
 config_flags(
	id int NOT NULL AUTO_INCREMENT
	Описание VARCHAR(100)
	flag int NOT NULL DEFAULT 0
	
/*Флаги:*/
(1,'состояние базы данных 	(0 - Ни разу не запускалась )
							(1 - Запущена
							(2 - Остановленна',0);
(2,'день отчетности успеваемости (1 - 28)',1),
(3,'день недели отчётности пропусков (0-6)',6);


 prefix
	id int NOT NULL AUTO_INCREMENT
	word VARCHAR(100) NOT NULL UNIQUE
	Описание VARCHAR(100) NOT NULL

/*Префиксы:*/
('LC_','успеваемость выполненно лаб'),
('PC_','успеваемость выполненно практических работ'),
('KRC_','успеваемость выполненно контрольных работ'),
('RGRC_','успеваемасть выполненно РГР'),
('LD_','успеваемость зашищенно лаб'),
('KRD_','успеваемость зашищенно практических работ'),
('RGRD_','успеваемость зашищенно РГР');


 времяПропуски
	id int NOT NULL AUTO_INCREMENT
	Даты date NOT NULL UNIQUE
  
 времяУроки
	id int NOT NULL AUTO_INCREMENT
	Даты date NOT NULL UNIQUE
	
 группы(
	num int NOT NULL DEFAULT 0
	Имя VARCHAR(100) NOT NULL
	
 предметы(
	num int NOT NULL DEFAULT 0
	Наименование VARCHAR(100) NOT NULL
 
/* Список процедур*/ 
/* Обновит тпблицу именем name*/
 update_table (IN name VARCHAR(100) 						
 */* создаст группу с именем name (
 имя должно начинаться с символов(а-я, a-z или _))*/
 createGroup(IN name VARCHAR(100) ) 						
 */* Создаст предмет с именем name (
 имя должно начинаться с символов(а-я, a-z или _) */
 createPredmet(IN Name VARCHAR(100)) 						
 */* Добавит предмет именем name в группу Gr*/
 addPredmet(IN Name VARCHAR(100),IN Gr VARCHAR(100)) 		
 */* удалит глобальный предмет name*/
 deleteGlobalPredmet(IN Name VARCHAR(100) )					
 */* удалит предмет группе  name*/
 deletePredmet(IN Name VARCHAR(100) ,IN Gr VARCHAR(100))	
 */* Добавит нового студента группе Gr с именем name*/
 addStudent(IN Name VARCHAR(100) ,IN group_ VARCHAR(100))	
 */* Удалит студента с именем name*/
 deleteStudent(IN Name VARCHAR(100) ,IN group_ VARCHAR(100))
 */* Удвлит группу с именем name */
 deleteGroup(IN Name VARCHAR(100))
  */* синхронизация времени 
  	данная функция вызывается системой для
  	 добовления в базу данных новых дат
  */
 upgrade_date_sync()
  */* глобальная синхронизация*/
 upgrade_date()
  */* остоновит синхронизацию бд*/
 stopBd()
  */* отчистит все даты из бд 
  (нужна для создания новой бд)*/
 clear()
  */* запустит синхронизацию со временим */
 startBd()
  */* для дебага отобразит пропуски группы name*/
 showGroup(IN name VARCHAR(100) )
  */* для дебага отобразит успеваемасть группы name*/
 showPredmet(IN name VARCHAR(100) ,IN Gr VARCHAR(100) , IN date_ DATE ))
