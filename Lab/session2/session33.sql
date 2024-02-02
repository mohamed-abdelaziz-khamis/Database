create table Dependent(
  essn integer not null,
  serial integer not null,
  Dep_name varchar(30),
  sex varchar(30),
  DOB date,
  Relationship varchar(30),
  primary key(essn,serial),
  foreign key (essn) references employee(id)
  );