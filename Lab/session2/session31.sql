create table Dept_Locations(
  Dnumber integer not null,
  Dlocation varchar(30) not null,
  primary key(Dnumber,Dlocation)
  );
  
create table project(
  pname varchar(30),
  Pnumber integer primary key,
  plocation varchar(30),
  Dnumber integer not null,
  foreign key(Dnumber) references Departement(Dnumber)
 );

