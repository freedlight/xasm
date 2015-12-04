..\xasm tst8080
if errorlevel 1 goto ex
diff tst8080.hex tst8080.sav
if errorlevel 1 goto ex
diff 8080.lst 8080.sav
:ex
