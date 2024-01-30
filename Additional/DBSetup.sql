CREATE DATABASE geotrackdb;
use geotrackdb;

DROP TABLE IF EXISTS SecGuard;
DROP TABLE IF EXISTS Records;
DROP TABLE IF EXISTS AdminLogin;

CREATE TABLE SecGuard (
	SecGuardID INT PRIMARY KEY AUTO_INCREMENT, 
	Name VARCHAR(50) NOT NULL,
	SecBTAdrs VARCHAR(17) NOT NULL
);

CREATE TABLE Records (
	RecordID INT PRIMARY KEY AUTO_INCREMENT, 
	SecGuardID INT NOT NULL, 
	SecBTAdrs VARCHAR(17) NOT NULL,
	RecordTime DATETIME NOT NULL, 
	Altitude FLOAT NOT NULL, 
	Latitude FLOAT NOT NULL, 
	Longitude FLOAT NOT NULL, 
	Floor INT NOT NULL,
	FOREIGN KEY (SecGuardID) REFERENCES SecGuard(SecGuardID)
);

CREATE TABLE AdminLogin (
	LoginID INT PRIMARY KEY AUTO_INCREMENT, 
	Email VARCHAR(100) NOT NULL, 
	Password VARCHAR(200) NOT NULL
);

