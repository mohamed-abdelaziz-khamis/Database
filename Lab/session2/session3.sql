create table Departement(
  Dname varchar(30),
  Dnumber integer primary key
  );
create table Employee(
	name varchar(30),
	id integer primary key not null,
	DOB date,
	Address varchar(100),
	sex varchar(10),
	salary float,
	Dep_no integer unique,
	foreign key (Dep_no) references Departement (Dnumber)
);

 