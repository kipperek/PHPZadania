CREATE TABLE kat(
  id_kat INTEGER PRIMARY KEY AUTO_INCREMENT,
  nazwa nVARCHAR(50) NOT NULL,
  id_nadkat INTEGER references kat(id_kat)
);

CREATE TABLE zad(
	id_zad INT PRIMARY KEY AUTO_INCREMENT,
	id_kat INT references kat(id_kat),
	nazwa nvarchar(50),
	plik_pdf nvarchar(50),
	trudnosc int
	);
	
CREATE TABLE user(
	id_user INT PRIMARY KEY AUTO_INCREMENT,
	login nvarchar(50),
	haslo nvarchar(50)
	);

INSERT INTO kat VALUES(1,'Matematyka',NULL);
INSERT INTO kat VALUES(2,'Fizyka',NULL);
INSERT INTO kat VALUES(3,'Chemia',NULL);
INSERT INTO kat VALUES(4,'Algebra',1);
INSERT INTO kat VALUES(5,'Dzialania na potęgach',4);
INSERT INTO kat VALUES(6,'Dzialania na pierwiastkach',4);
INSERT INTO kat VALUES(7,'Pierwiastki drugiego stopnia',6);
INSERT INTO kat VALUES(8,'Pierwiastki trzeciego stopnia',6);
	
INSERT INTO user values(1,'ania', 'ania123');
INSERT INTO user values(2,'marek', 'marek123');
INSERT INTO user values(3,'gacek', 'gacek123');
INSERT INTO user values(4,'ewa', 'ewa123');
	
