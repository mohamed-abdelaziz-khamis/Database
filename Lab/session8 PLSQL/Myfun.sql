create or replace function Myfun ( Dnum Number) return Number (10)
is
  retnum Number;
begin 
    select count(*) into retnum from Employee 
    where DEPT_NO = Dnum;
    return (Dnum);
 
end Myfun;