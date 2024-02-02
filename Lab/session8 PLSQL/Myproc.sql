create or replace procedure Myproc
is
temp_year Number;
n Number;
 begin
	select count(*) into n from DEPARTMENT4;
	for i in 1..n loop
		select YearA into temp_year from DEPARTMENT4
		where DNUMBER = i;
		if temp_year > 1960 then
			UPDATE DEPARTMENT4 SET Founded = '01-jan-1960' WHERE YearA=temp_year; 
		end if;
	end loop;
end myproc;