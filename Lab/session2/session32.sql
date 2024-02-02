
create table works_on 
(
  Essn integer not null,
  pnumber integer not null,
  hours integer,
  primary key (essn,pnumber),
  foreign key(essn) references Employee(SSN),
  foreign key(pnumber) references project(pnumber)
 );